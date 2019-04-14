#include "AoC/2015/problem_09.h"

#include "AoC/problems_map.h"
#include "AoC/utils/fusion.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/permutation.hpp"
#include "range/v3/algorithm/sort.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/drop.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/map.hpp"
#include "range/v3/view/sliding.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/view/unique.hpp"

#include "boost/fusion/include/vector.hpp"


#include <algorithm>
#include <functional>
#include <istream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace
{
using Location = std::string;
using Distance = size_t;
using Path     = std::pair<Location, Location>;

using Route = std::vector<Location>;

struct PathDistance
{
  Path path;
  Distance distance;
};

using PathMap = std::map<Path, Distance>;

PathDistance parse_path_distance( const std::string& s )
{
  namespace x3 = boost::spirit::x3;

  const auto location_parser = x3::lexeme[ +x3::alpha ];
  const auto parser          = location_parser > "to" > location_parser > "=" > AoC::x3_size_t_;

  boost::fusion::vector<std::string, std::string, size_t> parsed_data;
  const bool is_parsed = AoC::x3_parse( s.cbegin(), s.cend(), parser, x3::space, parsed_data );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input path data" );
  }

  auto [ from, to, distance ] = AoC::fusion_to_std_tuple( parsed_data );
  return { { std::move( from ), std::move( to ) }, distance };
}

size_t get_distance( const Path& path, const PathMap& path_map )
{
  const auto path_iter = path_map.find( path );
  assert( path_iter != path_map.cend() );

  return path_iter->second;
}

size_t calc_route_distance( const Route& route, const PathMap& path_map )
{
  const auto distances = route | ranges::view::sliding( 2 ) | ranges::view::transform( [&path_map]( const auto paths ) {
                           assert( paths.size() == 2 );
                           return get_distance( { paths[ 0 ], paths[ 1 ] }, path_map );
                         } );
  return ranges::accumulate( distances, size_t{ 0 } );
}

PathMap parse_path_map( std::istream& input )
{
  auto travel_data = ranges::getlines( input ) | ranges::view::transform( &parse_path_distance );

  PathMap path_map;
  for ( const auto& pd : travel_data )
  {
    const auto& [ from, to ] = pd.path;
    path_map.emplace( Path{ to, from }, pd.distance );
    path_map.emplace( Path{ from, to }, pd.distance );
  }

  return path_map;
}

template <const Distance& ( *select_distance )( const Distance&, const Distance& )>
size_t solve( std::istream& input )
{
  const auto path_map = parse_path_map( input );

  Route route = path_map | ranges::view::keys | ranges::view::transform( []( const Path& path ) { return path.first; } ) |
                ranges::view::unique | ranges::to_vector;
  ranges::sort( route );

  auto distance = calc_route_distance( route, path_map );
  while ( ranges::next_permutation( route ) )
  {
    distance = select_distance( distance, calc_route_distance( route, path_map ) );
  }

  return distance;
}
}  // namespace

namespace AoC_2015
{

namespace problem_09
{

size_t solve_1( std::istream& input )
{
  return solve<&std::min<Distance>>( input );
}

size_t solve_2( std::istream& input )
{
  return solve<&std::max<Distance>>( input );
}

AOC_REGISTER_PROBLEM( 2015_09, solve_1, solve_2 );

}  // namespace problem_09

}  // namespace AoC_2015


#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>
#  include <sstream>

static void impl_tests()
{
  {
    auto pd = parse_path_distance( "A to B = 10" );
    assert( "A" == pd.path.first );
    assert( "B" == pd.path.second );
    assert( 10 == pd.distance );
  }

  {
    std::istringstream ss( "A to B = 10" );
    auto pm = parse_path_map( ss );
    assert( 10 == get_distance( { "A", "B" }, pm ) );
  }

  {
    std::istringstream ss( "A to B = 10\nB to C = 20" );
    auto pm = parse_path_map( ss );
    assert( 30 == calc_route_distance( { "A", "B", "C" }, pm ) );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
