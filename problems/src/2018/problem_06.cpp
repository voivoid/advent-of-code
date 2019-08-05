#include "AoC/2018/problem_06.h"

#include "AoC/problems_map.h"
#include "AoC/utils/geo.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/count.hpp"
#include "range/v3/algorithm/max.hpp"
#include "range/v3/algorithm/min.hpp"
#include "range/v3/algorithm/minmax.hpp"
#include "range/v3/iterator/operations.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/cartesian_product.hpp"
#include "range/v3/view/concat.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/indices.hpp"
#include "range/v3/view/map.hpp"
#include "range/v3/view/repeat.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/view/zip.hpp"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/numeric/conversion/cast.hpp"


#include <cmath>
#include <istream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

BOOST_FUSION_ADAPT_STRUCT( AoC::Coord, x, y )

namespace
{

using CoordType     = int;
using Distance      = CoordType;
using Coord         = AoC::GenericPoint<CoordType>;
using Coords        = std::vector<Coord>;
using Rect          = AoC::GenericRectangle<CoordType>;
using LocationIndex = int;
struct Location
{
  Coord coord;
  LocationIndex index;
};
using Locations = std::vector<Location>;

constexpr LocationIndex overlapped_location_index = -1;

Coord parse_coord( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  const auto parser = x3::int_ > ',' > x3::int_;

  Coord coord;
  const bool is_parsed = AoC::x3_parse( line, parser, x3::space, coord );
  if ( !is_parsed )
  {
    throw std::runtime_error( "Failed to parse coord input" );
  }

  return coord;
}

Rect get_bound_rect( const Locations& locations )
{
  const auto coords = locations | ranges::view::transform( &Location::coord );
  const auto xs     = ranges::minmax( coords, std::less<CoordType>{}, &Coord::x );
  const auto ys     = ranges::minmax( coords, std::less<CoordType>{}, &Coord::y );

  return { { xs.min.x, ys.min.y }, { xs.max.x, ys.max.y } };
}

template <typename Range>
Locations index_coords( Range&& coords )
{
  return ranges::view::zip_with(
             []( const Coord c, const LocationIndex i ) {
               return Location{ c, i };
             },
             coords,
             ranges::view::iota( 1 ) ) |
         ranges::to<Locations>;
}

Distance calc_distance( const Coord c1, const Coord c2 )
{
  return std::abs( c2.x - c1.x ) + std::abs( c2.y - c1.y );
}

LocationIndex find_closest_location_index( const Coord current_coord, const Locations& locations )
{
  struct DistanceIndex
  {
    Distance distance;
    LocationIndex index;
  };

  const auto indices_and_distances = locations | ranges::view::transform( [current_coord]( const auto location ) {
                                       const Distance distance = calc_distance( current_coord, location.coord );
                                       return DistanceIndex{ distance, location.index };
                                     } );

  const auto loc_index_with_min_distance = ranges::min( indices_and_distances, std::less<Distance>{}, &DistanceIndex::distance );

  const bool only_one_closest_location =
      ranges::count( indices_and_distances, loc_index_with_min_distance.distance, &DistanceIndex::distance ) == 1;

  return only_one_closest_location ? loc_index_with_min_distance.index : overlapped_location_index;
}

auto find_border_indices( const Locations& locations )
{
  const auto brect = get_bound_rect( locations );

  const auto xs = ranges::view::closed_indices( brect.left_top.x, brect.right_bottom.x );
  const auto ys = ranges::view::closed_indices( brect.left_top.y, brect.right_bottom.y );

  const auto border_coords = ranges::view::concat( ranges::view::zip( xs, ranges::view::repeat( brect.left_top.y ) ),
                                                   ranges::view::zip( ranges::view::repeat( brect.left_top.x ), ys ),
                                                   ranges::view::zip( xs, ranges::view::repeat( brect.right_bottom.y ) ),
                                                   ranges::view::zip( ranges::view::repeat( brect.right_bottom.x ), ys ) );

  return border_coords | ranges::view::transform( [&locations]( const auto coord_pair ) {
           auto [ x, y ] = coord_pair;
           return find_closest_location_index( { x, y }, locations );
         } );
}

Distance sum_all_distances( const Coord current_coord, const Locations& locations )
{
  return ranges::accumulate( locations | ranges::view::transform( [current_coord]( const auto location ) {
                               return calc_distance( current_coord, location.coord );
                             } ),
                             0 );
}

Locations parse_locations( std::istream& input )
{
  auto coords = ranges::getlines( input ) | ranges::view::transform( &parse_coord );
  return index_coords( coords );
}

template <auto calc_map_element>
auto make_locations_map( const Locations& locations )
{
  const auto brect = get_bound_rect( locations );

  return ranges::view::cartesian_product( ranges::view::closed_indices( brect.left_top.x, brect.right_bottom.x ),
                                          ranges::view::closed_indices( brect.left_top.y, brect.right_bottom.y ) ) |
         ranges::view::transform( [&locations]( const auto xy ) {
           auto [ x, y ] = xy;
           return calc_map_element( Coord{ x, y }, locations );
         } );
}

int solve_2_impl( std::istream& input, const int total_distance_limit )
{
  const auto locations     = parse_locations( input );
  const auto locations_map = make_locations_map<&sum_all_distances>( locations );
  return boost::numeric_cast<int>( ranges::distance(
      locations_map | ranges::view::filter( [total_distance_limit]( const auto sum ) { return sum < total_distance_limit; } ) ) );
}

}  // namespace

namespace AoC_2018
{

namespace problem_06
{

int solve_1( std::istream& input )
{
  const auto locations     = parse_locations( input );
  const auto locations_map = make_locations_map<&find_closest_location_index>( locations );

  const auto border_location_indices = find_border_indices( locations ) | ranges::to<std::unordered_set>;

  auto finite_location_indices =
      locations_map | ranges::view::filter( [&border_location_indices]( const LocationIndex index ) {
        return index != overlapped_location_index && border_location_indices.find( index ) == border_location_indices.cend();
      } );

  std::unordered_map<LocationIndex, int> index_counter;
  for ( const LocationIndex index : finite_location_indices )
  {
    ++index_counter[ index ];
  }

  return ranges::max( index_counter | ranges::view::values );
}

int solve_2( std::istream& input )
{
  const int total_distance_limit = 10000;
  return solve_2_impl( input, total_distance_limit );
}

AOC_REGISTER_PROBLEM( 2018_06, solve_1, solve_2 );

}  // namespace problem_06

}  // namespace AoC_2018

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>
#  include <sstream>

static void impl_tests()
{
  const Coords coords       = { { 2, 2 }, { 1, 8 }, { 5, 6 } };
  const Locations locations = index_coords( coords );
  assert( locations.size() == 3 );
  assert( locations[ 0 ].index == 1 );
  assert( locations[ 1 ].index == 2 );
  assert( locations[ 2 ].index == 3 );

  const auto rect = get_bound_rect( locations );
  assert( rect.left_top == ( Coord{ 1, 2 } ) );
  assert( rect.right_bottom == ( Coord{ 5, 8 } ) );

  assert( ( overlapped_location_index ==
            find_closest_location_index( { 1, 1 }, std::initializer_list<Location>{ { { 0, 0 }, 1 }, { { 2, 2 }, 2 } } ) ) );

  std::stringstream input( R"(1, 1
                              1, 6
                              8, 3
                              3, 4
                              5, 5
                              8, 9)" );
  assert( 16 == solve_2_impl( input, 32 ) );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
