#include "AoC/2016/problem_11.h"

#include "AoC/problems_map.h"
#include "AoC/utils/anon_ret.h"
#include "AoC/utils/parse.h"
#include "AoC/utils/ranges/2dvector.h"
#include "AoC/utils/ranges/append.h"

#include "range/v3/algorithm/all_of.hpp"
#include "range/v3/algorithm/find.hpp"
#include "range/v3/algorithm/min.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/any_view.hpp"
#include "range/v3/view/concat.hpp"
#include "range/v3/view/empty.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/move.hpp"
#include "range/v3/view/repeat_n.hpp"
#include "range/v3/view/single.hpp"
#include "range/v3/view/tail.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/numeric/conversion/cast.hpp"

#include <array>
#include <cassert>
#include <functional>
#include <istream>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace
{

using FloorNumber      = size_t;
using DeviceMaterialId = size_t;

enum class DeviceType
{
  Chip,
  Generator
};

struct Device
{
  DeviceMaterialId material;
  DeviceType type;

  struct Hasher
  {
    size_t operator()( const Device& ) const
    {
      return 0;
    }
  };
};

using Devices = std::set<Device>;

struct Floor
{
  FloorNumber number;
  Devices devices;
};

constexpr FloorNumber MinFloor    = 0;
constexpr FloorNumber MaxFloor    = 3;
constexpr FloorNumber TotalFloors = MaxFloor - MinFloor + 1;
using Floors                      = std::array<Floor, TotalFloors>;

struct Facility
{
  FloorNumber elevator_floor_num;
  Floors floors;
  size_t steps;

  struct Hasher
  {
    size_t operator()( const Facility& ) const
    {
      return 0;
    }
  };
};

auto partition_devices( const Devices& devices )
{
  const auto filter_by = []( const DeviceType type ) {
    return ranges::view::filter( [type]( const Device& device ) { return device.type == type; } );
  };

  const auto chips      = devices | filter_by( DeviceType::Chip );
  const auto generators = devices | filter_by( DeviceType::Generator );

  AOC_RETURN_ANONYMOUS_STRUCT( chips, generators );
}

auto tie_dev( const Device& d )
{
  return std::tie( d.type, d.material );
}

[[maybe_unused]] bool operator==( const Device& d1, const Device& d2 )
{
  return tie_dev( d1 ) == tie_dev( d2 );
}

bool operator<( const Device& d1, const Device& d2 )
{
  return tie_dev( d1 ) < tie_dev( d2 );
}

bool operator<( const Floor& f1, const Floor& f2 )
{
  return f1.devices < f2.devices;
}

bool operator<( const Facility& s1, const Facility& s2 )
{
  return std::tie( s1.elevator_floor_num, s1.floors ) < std::tie( s2.elevator_floor_num, s2.floors );
}

DeviceMaterialId map_material_name( const std::string& material )
{
  static DeviceMaterialId id_counter = 1;
  static std::unordered_map<std::string, DeviceMaterialId> materials;

  auto [ iter, inserted ] = materials.emplace( material, id_counter );
  if ( inserted )
  {
    assert( id_counter != DeviceMaterialId{ 1 } << ( sizeof( DeviceMaterialId ) * 8 - 1 ) );
    id_counter <<= 2;
  }

  return iter->second;
}

Floor parse_floor( const std::string& line )
{
  // the problem has inconsistent input format so we will ignore ',' symbol

  namespace x3 = boost::spirit::x3;

  x3::symbols<size_t> floor_num_parser;
  floor_num_parser.add( "first", 0 )( "second", 1 )( "third", 2 )( "fourth", 3 );

  const auto make_device      = []( const DeviceType type, auto& ctx ) { x3::_val( ctx ) = Device{ x3::_attr( ctx ), type }; };
  const auto make_material_id = []( auto& ctx ) { x3::_val( ctx ) = map_material_name( x3::_attr( ctx ) ); };

  const auto material_parser = x3::rule<struct _material, DeviceMaterialId>{} = ( x3::lexeme[ +x3::alpha ] )[ make_material_id ];
  const auto chip_parser                                                      = x3::rule<struct _chip, Device>{} =
      ( ( material_parser >> "-compatible" ) > "microchip" )[ std::bind( make_device, DeviceType::Chip, std::placeholders::_1 ) ];
  const auto generator_parser = x3::rule<struct _generator, Device>{} =
      ( material_parser >> "generator" )[ std::bind( make_device, DeviceType::Generator, std::placeholders::_1 ) ];
  const auto device_parser = 'a' >> ( chip_parser | generator_parser );

  const auto empty_floor    = x3::lit( "nothing" ) > "relevant";
  const auto devices_parser = -( ( *device_parser ) >> "and" ) >> device_parser;
  const auto floor_parser   = "The" > floor_num_parser > "floor" > "contains" > ( empty_floor | devices_parser ) > '.';

  FloorNumber floor_num = 0;
  Devices devices;
  const bool is_parsed = AoC::x3_parse_m( line, floor_parser, x3::space | ',', floor_num, devices );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse floor input data" );
  }

  return { floor_num, std::move( devices ) };
}

template <typename Range>
bool check_chip_has_generator( const Device& chip, Range&& generators )
{
  auto generator_materials = ranges::view::transform( generators, &Device::material );

  return ranges::find( generator_materials, chip.material ) != generator_materials.end();
}

bool is_valid_floor( const Floor& facility )
{
  auto devices    = partition_devices( facility.devices );
  auto chips      = devices.chips;
  auto generators = devices.generators;

  return ranges::empty( generators ) ||
         ranges::all_of( chips, [&generators]( const Device& chip ) { return check_chip_has_generator( chip, generators ); } );
}

template <typename Facility>
auto& get_floor( Facility& facility, const FloorNumber n )
{
  return facility.floors.at( n );
}

template <typename Facility>
auto& get_current_floor( Facility& facility )
{
  return get_floor( facility, facility.elevator_floor_num );
}

bool are_all_devices_collected( const Facility& facility, const size_t total_devices )
{
  return get_floor( facility, MaxFloor ).devices.size() == total_devices;
}

ranges::any_view<ranges::any_view<Device>> generate_device_pairs( ranges::any_view<Device> devices )
{
  if ( devices.begin() == devices.end() )
  {
    return ranges::view::empty<ranges::any_view<Device>>;
  }

  const auto head = ranges::view::single( *devices.begin() );
  auto tail       = devices | ranges::view::tail;

  auto pairs = tail | ranges::view::transform( [head]( auto e ) { return head | AoC::append( e ); } );

  return ranges::view::concat( pairs, generate_device_pairs( tail ) );
}

template <typename Range>
auto generate_device_combinations( Range&& all_floor_devices )
{
  const auto single_devices = all_floor_devices | ranges::view::transform( ranges::view::single );
  auto pair_devices         = generate_device_pairs( all_floor_devices );
  return ranges::view::concat( single_devices, pair_devices );
}

using DevicesCombinations = std::vector<std::vector<Device>>;

const DevicesCombinations& generate_current_floor_devices_combinations( const Facility& facility )
{
  static std::map<Devices, DevicesCombinations> cached_combinations;

  const auto& devices = get_current_floor( facility ).devices;
  auto iter           = cached_combinations.find( devices );
  if ( iter == cached_combinations.cend() )
  {
    iter = cached_combinations.emplace( devices, generate_device_combinations( devices ) | AoC::to_2d_vector() ).first;
  }

  return iter->second;
}

Facility get_initial_state( std::istream& input )
{
  Facility initial_state;
  initial_state.steps              = 0;
  initial_state.elevator_floor_num = 0;

  for ( const auto& floor : ranges::getlines( input ) | ranges::view::transform( &parse_floor ) )
  {
    initial_state.floors.at( floor.number ) = floor;
  }

  return initial_state;
}

void remove_devices_from_the_floor( const std::vector<Device>& devices, Floor& floor )
{
  for ( const auto& dev : devices )
  {
    [[maybe_unused]] auto devs_removed = floor.devices.erase( dev );
    assert( devs_removed == 1 );
  }
}

void add_devices_to_the_floor( const std::vector<Device>& devices, Floor& floor )
{
  for ( const auto& dev : devices )
  {
    [[maybe_unused]] auto [ _, inserted ] = floor.devices.insert( dev );
    assert( inserted );
  }
}

std::optional<Facility>
    try_to_move_devices_to_the_floor( const std::vector<Device>& devices, const Facility& facility, const size_t floor_num )
{
  Facility new_state = facility;
  new_state.steps += 1;

  auto& source_floor = get_current_floor( new_state );
  remove_devices_from_the_floor( devices, source_floor );

  if ( !is_valid_floor( source_floor ) )
  {
    return {};
  }

  assert( new_state.elevator_floor_num != floor_num );
  new_state.elevator_floor_num = floor_num;
  auto& destination_floor      = get_current_floor( new_state );
  add_devices_to_the_floor( devices, destination_floor );

  if ( !is_valid_floor( destination_floor ) )
  {
    return {};
  }

  return { std::move( new_state ) };
}

size_t count_devices( const Facility& facility )
{
  auto floor_nums = ranges::view::closed_iota( MinFloor, MaxFloor );
  auto devices_by_floor =
      ranges::view::transform( floor_nums, [&facility]( const FloorNumber n ) { return get_floor( facility, n ).devices.size(); } );
  return ranges::accumulate( devices_by_floor, size_t{ 0 } );
}

int solve( const Facility& initial_state )
{
  const size_t device_count = count_devices( initial_state );

  std::set<Facility> states_history;
  std::queue<Facility> states_queue;

  states_queue.push( initial_state );
  while ( !states_queue.empty() )
  {
    Facility current_state = std::move( states_queue.front() );
    states_queue.pop();

    if ( are_all_devices_collected( current_state, device_count ) )
    {
      assert( current_state.elevator_floor_num == MaxFloor );

      return boost::numeric_cast<int>( current_state.steps );
    }

    for ( const auto& devices : generate_current_floor_devices_combinations( current_state ) )
    {
      const auto try_to_move_devices_to = [&devices, &current_state, &states_queue, &states_history]( const size_t floor_num ) {
        auto new_state = try_to_move_devices_to_the_floor( devices, current_state, floor_num );
        if ( new_state )
        {
          auto [ _, inserted ] = states_history.insert( *new_state );
          if ( inserted )
          {
            states_queue.push( std::move( *new_state ) );
          }
        }
      };

      if ( current_state.elevator_floor_num != MaxFloor )
      {
        try_to_move_devices_to( current_state.elevator_floor_num + 1 );
      }

      if ( current_state.elevator_floor_num != MinFloor )
      {
        try_to_move_devices_to( current_state.elevator_floor_num - 1 );
      }
    }
  }

  throw std::runtime_error( "Failed to find the solution" );
}

}  // namespace

namespace AoC_2016
{

namespace problem_11
{

int solve_1( std::istream& input )
{
  return solve( get_initial_state( input ) );
}

int solve_2( std::istream& input )
{
  auto state    = get_initial_state( input );
  auto& devices = state.floors[ MinFloor ].devices;
  devices.insert( { map_material_name( "elerium" ), DeviceType::Chip } );
  devices.insert( { map_material_name( "elerium" ), DeviceType::Generator } );
  devices.insert( { map_material_name( "dilithium" ), DeviceType::Chip } );
  devices.insert( { map_material_name( "dilithium" ), DeviceType::Generator } );
  return solve( state );
}

AOC_REGISTER_PROBLEM( 2016_11, solve_1, solve_2 );

}  // namespace problem_11

}  // namespace AoC_2016

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

namespace
{

auto generate_dummy_devices( const int devices_num )
{
  static const Device dummy{ 0, DeviceType::Chip };
  return ranges::view::repeat_n( dummy, devices_num );
}

bool test_device_pairs( const int devices_num, const int expected_pairs_num )
{
  return ranges::distance( generate_device_pairs( generate_dummy_devices( devices_num ) ) ) == expected_pairs_num;
}

bool test_device_combinations( const int devices_num, const int expected_combinations_num )
{
  return ranges::distance( generate_device_combinations( generate_dummy_devices( devices_num ) ) ) == expected_combinations_num;
}

void impl_tests()
{
  {
    const auto f = parse_floor( "The first floor contains a hydrogen-compatible microchip and a hydrogen generator." );
    assert( 0 == f.number );
    assert( 2 == f.devices.size() );
    assert( ( Devices{ { map_material_name( "hydrogen" ), DeviceType::Generator },
                       { map_material_name( "hydrogen" ), DeviceType::Chip } } ) == f.devices );
    assert( is_valid_floor( f ) );
  }

  {
    const auto f = parse_floor( "The second floor contains a hydrogen generator." );
    assert( 1 == f.number );
    assert( 1 == f.devices.size() );
    assert( ( Devices{ { map_material_name( "hydrogen" ), DeviceType::Generator } } ) == f.devices );
    assert( is_valid_floor( f ) );
  }

  {
    const auto f = parse_floor( "The third floor contains a lithium generator." );
    assert( 2 == f.number );
    assert( 1 == f.devices.size() );
    assert( ( Devices{ { map_material_name( "lithium" ), DeviceType::Generator } } ) == f.devices );
    assert( is_valid_floor( f ) );
  }

  {
    const auto f = parse_floor( "The fourth floor contains nothing relevant." );
    assert( 3 == f.number );
    assert( 0 == f.devices.size() );
    assert( is_valid_floor( f ) );
  }

  assert( is_valid_floor( parse_floor(
      "The first floor contains a thulium generator, a thulium-compatible microchip, a plutonium generator, and a strontium generator." ) ) );
  assert(
      is_valid_floor( parse_floor( "The second floor contains a plutonium-compatible microchip and a strontium-compatible microchip." ) ) );
  assert( is_valid_floor( parse_floor(
      "The third floor contains a promethium generator, a promethium-compatible microchip, a ruthenium generator, and a ruthenium-compatible microchip." ) ) );


  assert( !is_valid_floor( parse_floor( "The first floor contains a hydrogen-compatible microchip and a lithium generator." ) ) );
  assert( !is_valid_floor( parse_floor(
      "The first floor contains a thulium generator, a thulium-compatible microchip and a ruthenium-compatible microchip." ) ) );

  assert( test_device_pairs( 0, 0 ) );
  assert( test_device_pairs( 1, 0 ) );
  assert( test_device_pairs( 2, 1 ) );
  assert( test_device_pairs( 3, 3 ) );

  assert( test_device_combinations( 0, 0 ) );
  assert( test_device_combinations( 1, 1 ) );
  assert( test_device_combinations( 2, 3 ) );
  assert( test_device_combinations( 3, 6 ) );

  {
    Facility s;
    assert( are_all_devices_collected( s, 0 ) );
  }

  {
    Facility s;
    s.floors[ 3 ] = parse_floor( "The fourth floor contains a hydrogen-compatible microchip and a hydrogen generator." );
    assert( are_all_devices_collected( s, 2 ) );
  }

  {
    Facility s;
    s.floors[ 0 ] = parse_floor( "The first floor contains a lithium generator." );
    s.floors[ 3 ] = parse_floor( "The fourth floor contains a hydrogen-compatible microchip and a hydrogen generator." );
    assert( !are_all_devices_collected( s, 3 ) );
  }

  {
    const auto f = parse_floor(
        "The first floor contains a thulium generator, a thulium-compatible microchip, a plutonium generator, and a strontium generator." );
    auto devs = partition_devices( f.devices );

    assert( ranges::distance( devs.chips ) == 1 );
    assert( ranges::distance( devs.generators ) == 3 );
  }
}

}  // namespace

REGISTER_IMPL_TEST( impl_tests );

#endif
