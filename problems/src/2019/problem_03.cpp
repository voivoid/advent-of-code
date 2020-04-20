#include "AoC/2019/problem_03.h"

#include "AoC/problems_map.h"
#include "AoC/utils/assert.h"
#include "AoC/utils/geo.h"
#include "AoC/utils/parse.h"
#include "AoC/utils/ranges/inclusive_scan.h"

#include "range/v3/action/sort.hpp"
#include "range/v3/algorithm/min.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/any_view.hpp"
#include "range/v3/view/cartesian_product.hpp"
#include "range/v3/view/empty.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/indices.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/join.hpp"
#include "range/v3/view/single.hpp"
#include "range/v3/view/sliding.hpp"
#include "range/v3/view/tail.hpp"
#include "range/v3/view/take.hpp"
#include "range/v3/view/take_while.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/view/zip.hpp"
#include "range/v3/view/zip_with.hpp"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/fusion/container/vector.hpp"
#include "boost/numeric/conversion/cast.hpp"

#include <functional>
#include <istream>
#include <limits>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace
{

struct Wire
{
  enum class Dir
  {
    Up,
    Right,
    Down,
    Left
  };

  Dir dir;
  size_t len;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( Wire, dir, len )

namespace
{

using Wires = std::vector<Wire>;

std::pair<Wires, Wires> parse_wires( std::istream& input )
{
  namespace x3 = boost::spirit::x3;

  x3::symbols<Wire::Dir> dir;
  dir.add( "U", Wire::Dir::Up )( "R", Wire::Dir::Right )( "D", Wire::Dir::Down )( "L", Wire::Dir::Left );

  const auto wire = x3::rule<struct _wire, Wire>{} = dir > AoC::x3_size_t_;
  const auto wires                                 = x3::lexeme[ ( wire % ',' ) ];
  const auto parser                                = wires > wires;

  boost::fusion::vector<Wires, Wires> wires_pair;
  const bool is_parsed = AoC::x3_parse( input, parser, x3::space, wires_pair );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input int codes data" );
  }

  return { boost::fusion::at_c<0>( wires_pair ), boost::fusion::at_c<1>( wires_pair ) };
}

using Location = AoC::Coord;
using Segment  = AoC::Segment;
using Segments = std::vector<Segment>;

Location get_wire_end_point( const Location loc, const Wire& wire )
{
  const auto wire_len = boost::numeric_cast<int>( wire.len );

  switch ( wire.dir )
  {
    case Wire::Dir::Up: return { loc.x, loc.y + wire_len };
    case Wire::Dir::Right: return { loc.x + wire_len, loc.y };
    case Wire::Dir::Down: return { loc.x, loc.y - wire_len };
    case Wire::Dir::Left: return { loc.x - wire_len, loc.y };
  }

  AOC_ASSERT_FALSE()
}

auto get_wire_steps( const Wire& wire, const Location loc )
{
  return ranges::views::closed_indices( size_t{ 1 }, wire.len ) | ranges::views::transform( [ loc, wire ]( const auto len ) {
           return get_wire_end_point( loc, Wire{ wire.dir, len } );
         } );
}

auto get_path_locations( const Location start_location, const Wires& wires )
{
  return wires | AoC::inclusive_scan( start_location, &get_wire_end_point );
}

std::vector<Segment> make_segments( const Location start_location, const Wires& wires )
{
  return get_path_locations( start_location, wires ) | ranges::views::sliding( 2 ) | ranges::views::transform( []( auto locs_pair ) {
           auto iter   = locs_pair.begin();
           Location l1 = *iter++;
           Location l2 = *iter;
           return Segment{ l1, l2 };
         } ) |
         ranges::to_vector;
}

std::pair<int, int> get_mid_coords( const int a, const int b, const int c, const int d )
{
  std::array<int, 4> arr = { a, b, c, d };
  ranges::actions::sort( arr );
  return { arr[ 1 ], arr[ 2 ] };
}

// Looks like segments of different wires shouldn't lay under each other. Nevertheless let's support this
std::optional<std::pair<int, int>> get_parallel_intersections( const int p1_a, const int p1_b, const int p2_c, const int p2_d )
{
  const auto in_between = []( const int c, const int c1, const int c2 ) { return c >= c1 && c <= c2; };

  if ( in_between( p1_a, p2_c, p2_d ) && in_between( p1_b, p2_c, p2_d ) )
  {
    return { { p1_a, p1_b } };
  }
  else if ( in_between( p2_c, p1_a, p1_b ) && in_between( p2_d, p1_a, p1_b ) )
  {
    return { { p2_c, p2_d } };
  }
  else if ( in_between( p1_a, p2_c, p2_d ) || in_between( p1_b, p2_c, p2_d ) )
  {
    return { get_mid_coords( p1_a, p1_b, p2_c, p2_d ) };
  }

  return {};
}

ranges::any_view<Location> get_parallel_intersections( const Segment& s1, const Segment& s2 )
{
  assert( ( s1.is_vertical() && s2.is_vertical() ) || ( s1.is_horizontal() && s2.is_horizontal() ) );

  const auto const_coord = s1.is_horizontal() ? &Location::y : &Location::x;
  if ( s1.p1.*const_coord != s2.p1.*const_coord )
  {
    return ranges::views::empty<Location>;
  }

  const auto non_const_coord = s1.is_horizontal() ? &Location::x : &Location::y;
  auto intersections =
      get_parallel_intersections( s1.p1.*non_const_coord, s1.p2.*non_const_coord, s2.p1.*non_const_coord, s2.p2.*non_const_coord );

  if ( !intersections )
  {
    return ranges::views::empty<Location>;
  }

  return ranges::views::closed_iota( intersections->first, intersections->second ) | ranges::views::transform( [ & ]( const int c ) {
           Location loc{};
           loc.*const_coord     = s1.p1.*const_coord;
           loc.*non_const_coord = c;
           return loc;
         } );
}

ranges::any_view<Location> get_intersections( const Segment& s1, const Segment& s2 )
{
  assert( s1.is_vertical() || s1.is_horizontal() );
  assert( s2.is_vertical() || s2.is_horizontal() );

  if ( ( s1.is_vertical() && s2.is_vertical() ) || ( s1.is_horizontal() && s2.is_horizontal() ) )
  {
    return get_parallel_intersections( s1, s2 );
  }

  const auto intersection = AoC::get_intersection_with_perpendicular( s1, s2 );
  if ( intersection )
  {
    return ranges::views::single( *intersection );
  }

  return ranges::views::empty<Location>;
}

auto get_intersections( const Segments& wireSegments1, const Segments& wireSegments2 )
{
  return ranges::views::cartesian_product( wireSegments1, wireSegments2 ) | ranges::views::transform( []( const auto segments_pair ) {
           const auto [ s1, s2 ] = segments_pair;
           return get_intersections( s1, s2 );
         } ) |
         ranges::views::join;
}

size_t calc_distance( const Location from, const Location to )
{
  return static_cast<size_t>( std::abs( to.x - from.x ) + std::abs( to.y - from.y ) );
}

auto get_path( const Location start_location, const Wires& wires )
{
  return ranges::views::zip_with( &get_wire_steps, wires, get_path_locations( start_location, wires ) ) | ranges::views::join;
}

using LocationToStepsNumMap = std::unordered_map<Location, size_t, AoC::GeoHasher>;

size_t get_path1_steps_to_intersection( const LocationToStepsNumMap& path1_intersections_to_steps, const Location intersection )
{
  const auto path1_location_iter = path1_intersections_to_steps.find( intersection );
  if ( path1_location_iter != path1_intersections_to_steps.cend() )
  {
    return path1_location_iter->second;
  }
  else
    throw std::runtime_error( "Unexpected flow" );
}

auto parse_segments( std::istream& input, const Location start_location )
{
  auto wires     = parse_wires( input );
  auto segments1 = make_segments( start_location, wires.first );
  auto segments2 = make_segments( start_location, wires.second );

  return std::make_tuple( std::move( wires ), std::move( segments1 ), std::move( segments2 ) );
}

}  // namespace

namespace AoC_2019
{

namespace problem_03
{

size_t solve_1( std::istream& input )
{
  const auto start_location                  = Location{ 0, 0 };
  const auto [ wires, segments1, segments2 ] = parse_segments( input, start_location );

  auto intersections = get_intersections( segments1, segments2 ) | ranges::views::tail;  // get tail to ignore start position intersection

  const auto calc_distance_to_start = std::bind( &calc_distance, start_location, std::placeholders::_1 );
  auto distances                    = intersections | ranges::views::transform( calc_distance_to_start );
  return ranges::min( distances );
}

size_t solve_2( std::istream& input )
{
  const auto start_location                  = Location{ 0, 0 };
  const auto [ wires, segments1, segments2 ] = parse_segments( input, start_location );

  auto path1_indexed = ranges::views::zip( get_path( start_location, wires.first ), ranges::views::iota( size_t{ 1 } ) );
  auto path2_indexed = ranges::views::zip( get_path( start_location, wires.second ), ranges::views::iota( size_t{ 1 } ) );

  const auto intersections =
      get_intersections( segments1, segments2 ) | ranges::views::tail | ranges::to<std::unordered_set<Location, AoC::GeoHasher>>;

  const auto filter_intersection_locations = ranges::views::filter( [ &intersections ]( const auto step ) {
    const auto [ location, _ ] = step;
    return intersections.find( location ) != intersections.cend();
  } );

  const auto path1_intersections_to_steps = path1_indexed | filter_intersection_locations | ranges::to<LocationToStepsNumMap>;

  const auto get_of_steps_to_intersection_for_both_paths = ranges::views::transform( [ &path1_intersections_to_steps ]( const auto step ) {
    const auto [ location, step_num ] = step;
    return step_num + get_path1_steps_to_intersection( path1_intersections_to_steps, location );
  } );

  auto steps_to_intersections = path2_indexed | filter_intersection_locations | get_of_steps_to_intersection_for_both_paths;

  return ranges::min( steps_to_intersections );
}

AOC_REGISTER_PROBLEM( 2019_03, solve_1, solve_2 );

}  // namespace problem_03

}  // namespace AoC_2019

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

#  include <sstream>

static void impl_tests()
{
  {
    std::istringstream ss( "R8,U5,L5,D3\nR10" );
    const auto [ wires, wires2 ] = parse_wires( ss );
    assert( wires.size() == 4 );
    assert( wires2.size() == 1 );

    assert( wires[ 0 ].dir == Wire::Dir::Right );
    assert( wires[ 0 ].len == 8 );
    assert( wires[ 1 ].dir == Wire::Dir::Up );
    assert( wires[ 1 ].len == 5 );
    assert( wires[ 2 ].dir == Wire::Dir::Left );
    assert( wires[ 2 ].len == 5 );
    assert( wires[ 3 ].dir == Wire::Dir::Down );
    assert( wires[ 3 ].len == 3 );

    assert( wires2[ 0 ].dir == Wire::Dir::Right );
    assert( wires2[ 0 ].len == 10 );

    const auto segments = make_segments( { 0, 0 }, wires );
    assert( 4 == segments.size() );

    const auto s1 = Segment{ { 0, 0 }, { 8, 0 } };
    assert( s1 == segments[ 0 ] );

    const auto s2 = Segment{ { 8, 0 }, { 8, 5 } };
    assert( s2 == segments[ 1 ] );

    const auto s3 = Segment{ { 8, 5 }, { 3, 5 } };
    assert( s3 == segments[ 2 ] );

    const auto s4 = Segment{ { 3, 5 }, { 3, 2 } };
    assert( s4 == segments[ 3 ] );
  }

  {
    assert( std::make_pair( 0, 10 ) == *get_parallel_intersections( 0, 10, 0, 10 ) );
    assert( std::make_pair( -5, 5 ) == *get_parallel_intersections( -5, 5, -10, 10 ) );
    assert( std::make_pair( -5, 5 ) == *get_parallel_intersections( -10, 10, -5, 5 ) );
    assert( std::make_pair( -5, 5 ) == *get_parallel_intersections( -10, 5, -5, 10 ) );
    assert( std::make_pair( -5, 5 ) == *get_parallel_intersections( -5, 10, -10, 5 ) );
    assert( std::make_pair( 0, 0 ) == *get_parallel_intersections( -10, 0, 0, 10 ) );

    assert( !get_parallel_intersections( -10, -5, 5, 10 ) );
    assert( !get_parallel_intersections( 1, 10, -10, -1 ) );
  }

  {
    std::vector<Location> v{ { 1, 0 }, { 2, 0 }, { 3, 0 }, { 3, 1 }, { 3, 2 } };
    auto path = get_path( Location{ 0, 0 }, { { Wire::Dir::Right, 3 }, { Wire::Dir::Up, 2 } } ) | ranges::to_vector;
    assert( v == path );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
