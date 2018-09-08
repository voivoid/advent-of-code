#include "AoC/2015/problem_09.h"

#include <AoC/problems_map.h>
#include <AoC_utils/parse.h>

#include <range/v3/algorithm/permutation.hpp>
#include <range/v3/algorithm/sort.hpp>
#include <range/v3/getlines.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/map.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/unique.hpp>
#include <range/v3/view/zip.hpp>

#include <boost/fusion/include/vector.hpp>
#include <boost/spirit/home/x3.hpp>

#include <map>
#include <vector>

namespace
{
using Location = std::string;
using Distance = size_t;
using Path     = std::pair<Location, Location>;

using Route = std::vector<Location>;

struct PathDistance
{
  Path     path;
  Distance distance;
};

using PathMap = std::map<Path, Distance>;

PathDistance parse_path_distance( const std::string& s )
{
  namespace x3 = boost::spirit::x3;

  const auto location_parser = x3::lexeme[ +x3::alpha ];
  const auto parser          = location_parser > "to" > location_parser > "=" > x3::int_;

  boost::fusion::vector<std::string, std::string, int> parsed_data;
  const bool                                           is_parsed = AoC::x3_parse( s.cbegin(), s.cend(), parser, x3::space, parsed_data );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse path data" );
  }

  return { { boost::fusion::at_c<0>( parsed_data ), boost::fusion::at_c<1>( parsed_data ) },
           static_cast<size_t>( boost::fusion::at_c<2>( parsed_data ) ) };
}

size_t get_distance( const Path& path, const PathMap& path_map )
{
  auto path_iter = path_map.find( path );
  assert( path_iter != path_map.cend() );

  return path_iter->second;
}

size_t calc_route_distance( const Route& route, const PathMap& path_map )
{
  const auto paths = ranges::view::zip( route, route | ranges::view::drop( 1 ) );
  return ranges::accumulate( paths, size_t( 0 ), [&path_map]( size_t acc_distance, const auto& path ) {
    return acc_distance + get_distance( { path.first, path.second }, path_map );
  } );
}

PathMap parse_path_map( std::istream& input )
{
  auto travel_data = ranges::getlines( input ) | ranges::view::transform( &parse_path_distance );

  PathMap path_map;
  ranges::accumulate( travel_data, std::ref( path_map ), []( std::reference_wrapper<PathMap> map, const PathDistance& pd ) {
    const auto& [ from, to ] = pd.path;
    map.get().emplace( Path{ to, from }, pd.distance );
    map.get().emplace( Path{ from, to }, pd.distance );
    return map;
  } );
  return path_map;
}

template <const Distance& ( *select_distance )( const Distance&, const Distance& )>
int solve( std::istream& input )
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

  return static_cast<int>( distance );
}
}  // namespace

namespace AoC_2015
{

namespace problem_09
{

int solve_1( std::istream& input )
{
  return solve<&std::min<Distance>>( input );
}

int solve_2( std::istream& input )
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
    auto               pm = parse_path_map( ss );
    assert( 10 == get_distance( { "A", "B" }, pm ) );
  }

  {
    std::istringstream ss( "A to B = 10\nB to C = 20" );
    auto               pm = parse_path_map( ss );
    assert( 30 == calc_route_distance( { "A", "B", "C" }, pm ) );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
