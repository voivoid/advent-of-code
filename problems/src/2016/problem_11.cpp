#include "AoC/2016/problem_11.h"

#include "AoC/problems_map.h"
#include "AoC/utils/curry.h"
#include "AoC/utils/fusion.h"
#include "AoC/utils/parse.h"
#include "AoC/utils/ranges.h"

#include "range/v3/algorithm/all_of.hpp"
#include "range/v3/algorithm/copy.hpp"
#include "range/v3/getlines.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/move.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/view/concat.hpp"
#include "range/v3/view/single.hpp"
#include "range/v3/view/join.hpp"
#include "range/v3/view/any_view.hpp"
#include "range/v3/view/tail.hpp"

#include "boost/fusion/container/vector.hpp"
#include "boost/hof/partial.hpp"
#include "boost/spirit/home/x3.hpp"

#include <array>
#include <istream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{

enum class DeviceType
{
  Chip,
  Generator
};

using FloorNum = size_t;
using Device   = std::string;
using Devices  = std::set<Device>;

struct Floor
{
  FloorNum floor;
  Devices chips;
  Devices generators;
};
constexpr FloorNum MinFloor = 1;
constexpr FloorNum MaxFloor = 4;
using Floors = std::array<Floor, MaxFloor>;

struct State
{
  FloorNum current_floor;
  Floors floors;
};

// bool operator<( const Floor& f1, const Floor& f2 )
//{
//  return std::tie( f1.floor, f1.chips, f1.generators ) < std::tie( f2.floor, f2.chips, f2.generators );
//}

// bool operator<( const State& s1, const State& s2 )
//{
//  return std::tie( s1.current_floor, s1.floors ) < std::tie( s2.current_floor, s2.floors );
//}

bool check_chip_has_generator( const Floor& floor, const Device& chip )
{
  const auto& generators = floor.generators;
  return generators.find( chip ) != generators.cend();
}

bool is_valid_floor( const Floor& floor )
{
  if( floor.generators.empty() )
  {
      return true;
  }

  return ranges::all_of( floor.chips, [&floor]( const Device& chip ) { return check_chip_has_generator( floor, chip ); } );
}

//bool all_collected( const State& state )
//{
//    std::initializer_list<FloorNum> floor_nums = { 1, 2, 3 };
//    return ranges::all_of( floor_nums, [&state]( const FloorNum n ) {
//        const auto& floor = state.floors[n];
//        return floor.chips.empty() && floor.generators.empty();
//    } );
//}

//std::vector<FloorNum> generate_elevator_moves( const State& state )
//{
//    if( state.current_floor == MinFloor )
//    {
//        return { MinFloor + 1 };
//    }
//    else if( state.current_floor == MaxFloor )
//    {
//        return { MaxFloor - 1 };
//    }

//    return { state.current_floor - 1, state.current_floor + 1 };
//}

const Floor& get_current_floor( const State& state )
{
  return state.floors[ state.current_floor ];
}

ranges::any_view<ranges::any_view<Device>> generate_device_pairs( ranges::any_view<Device, ranges::category::bidirectional | ranges::category::sized> devices )
{
    auto head = ranges::view::single( devices.front() );
    auto tail = devices | ranges::view::tail;

    auto pairs = tail | ranges::view::transform( [head]( auto e ) {
        return head | AoC::append( e );
    });

    generate_device_pairs( tail );
    //return ranges::view::concat( pairs, generate_device_pairs( tail ) );
}

auto generate_next_states( const State& state )
{
    const Floor& current_floor = get_current_floor( state );
    const auto all_floor_devices = ranges::view::concat( current_floor.chips, current_floor.generators );

    const auto single_devices = all_floor_devices | ranges::view::transform( ranges::view::single );
    auto pair_devices = generate_device_pairs( all_floor_devices );
    return ranges::view::concat( single_devices, pair_devices );
}

Floor parse_floor( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  x3::symbols<size_t> floor_num_parser;
  floor_num_parser.add( "first", 1 )( "second", 2 )( "third", 3 )( "fourth", 4 );

  struct GenericDevice
  {
    Device device;
    DeviceType type;
  };

  const auto make_device = []( const DeviceType type, auto& ctx ) { x3::_val( ctx ) = GenericDevice{ x3::_attr( ctx ), type }; };

  const auto name_parser = x3::lexeme[ +x3::alpha ];
  const auto chip_parser = x3::rule<struct _chip, GenericDevice>{} =
      ( ( name_parser >> "-compatible" ) > "microchip" )[ AoC::curry( make_device )( DeviceType::Chip ) ];
  const auto generator_parser = x3::rule<struct _generator, GenericDevice>{} =
      ( name_parser >> "generator" )[ AoC::curry( make_device )( DeviceType::Generator ) ];
  const auto device_parser = 'a' > ( chip_parser | generator_parser );

  const auto empty_floor    = x3::lit( "nothing" ) > "relevant";
  const auto devices_parser = ( device_parser % "and" ) | empty_floor;
  const auto floor_parser   = "The" > floor_num_parser > "floor" > "contains" > devices_parser > '.';

  boost::fusion::vector<FloorNum, std::vector<GenericDevice>> result;
  const bool is_parsed = AoC::x3_parse( line.cbegin(), line.cend(), floor_parser, x3::space, result );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse floor input data" );
  }

  auto [ floor_num, devices ] = AoC::fusion_to_std_tuple( result );

  const auto filter_by = []( const DeviceType type ) {
    return ranges::view::filter( [type]( const GenericDevice& device ) { return device.type == type; } ) |
           ranges::view::transform( &GenericDevice::device ) | ranges::view::move;
  };

  auto chips      = devices | filter_by( DeviceType::Chip );
  auto generators = devices | filter_by( DeviceType::Generator );

  return { floor_num, { chips }, { generators } };
}

State get_initial_state( std::istream& input )
{
  State initial_state;
  initial_state.current_floor = 1;

  auto floors = ranges::getlines( input ) | ranges::view::transform( &parse_floor );
  ranges::copy( floors, initial_state.floors.begin() );

  return initial_state;
}

}  // namespace

namespace AoC_2016
{

namespace problem_11
{

int solve_1( std::istream& input )
{
  const State initial_state = get_initial_state( input );
  generate_next_states( initial_state );

  return 0;
}

int solve_2( std::istream& )
{
  return 0;
}

AOC_REGISTER_PROBLEM( 2016_11, solve_1, solve_2 );

}  // namespace problem_11

}  // namespace AoC_2016

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  {
    const auto f = parse_floor( "The first floor contains a hydrogen-compatible microchip and a lithium-compatible microchip." );
    assert( 1 == f.floor );
    assert( 2 == f.chips.size() );
    assert( 0 == f.generators.size() );
    assert( ( Devices{ "hydrogen", "lithium" } ) == f.chips );
  }

  {
    const auto f = parse_floor( "The second floor contains a hydrogen generator." );
    assert( 2 == f.floor );
    assert( 0 == f.chips.size() );
    assert( 1 == f.generators.size() );
    assert( ( Devices{ "hydrogen" } ) == f.generators );
  }

  {
    const auto f = parse_floor( "The third floor contains nothing relevant." );
    assert( 3 == f.floor );
    assert( 0 == f.chips.size() );
    assert( 0 == f.generators.size() );
  }

  assert( is_valid_floor( parse_floor( "The first floor contains a hydrogen-compatible microchip and a hydrogen generator." ) ) );
  assert( is_valid_floor( parse_floor( "The second floor contains a hydrogen generator." ) ) );
  assert(
      !is_valid_floor( parse_floor( "The first floor contains a hydrogen-compatible microchip and a lithium-compatible microchip." ) ) );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
