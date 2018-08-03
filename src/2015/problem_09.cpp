#include "AoC/2015/problem_09.h"

#include <AoC/problems_map.h>
#include <AoC_utils/match.h>

#include "rangev3.h"

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
  auto tokens = ranges::action::split( s, ' ' );

  using S = std::string;
  const auto path_distance =
      AoC::match_container( tokens,
                            []( S from, const S& /*to*/, S to, const S& /*=*/, const S& distance ) {
                              return PathDistance{ { std::move( from ), std::move( to ) }, static_cast<size_t>( std::stoi( distance ) ) };
                            },
                            []( const auto& ) -> PathDistance { throw std::runtime_error( "wrong path distance" ); } );

  return path_distance;
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
