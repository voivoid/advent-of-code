#include "AoC/2017/problem_11.h"

#include "AoC/problems_map.h"
#include "AoC/utils/geo.h"
#include "AoC/utils/parse.h"
#include "AoC/utils/ranges.h"

#include "boost/numeric/conversion/cast.hpp"

#include "range/v3/algorithm/max.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/exclusive_scan.hpp"
#include "range/v3/view/transform.hpp"

#include <istream>
#include <stdexcept>
#include <vector>

namespace
{
enum class Dir
{
  N,
  NE,
  SE,
  S,
  SW,
  NW,
  Dummy
};

using Dirs     = std::vector<Dir>;
using Pos      = AoC::Coord;
using Distance = size_t;

Pos make_step( const Pos pos, const Dir d )
{
  const auto [ x, y ] = pos;
  switch ( d )
  {
    case Dir::N: return { x, y + 2 };
    case Dir::NE: return { x + 1, y + 1 };
    case Dir::SE: return { x + 1, y - 1 };
    case Dir::S: return { x, y - 2 };
    case Dir::SW: return { x - 1, y - 1 };
    case Dir::NW: return { x - 1, y + 1 };
    case Dir::Dummy: return pos;
  }

  throw std::runtime_error( "This should never be executed" );
}

Distance calc_min_distance( const Pos pos )
{
  const auto [ x, y ] = pos;

  const auto abs_x = std::abs( x );
  const auto abs_y = std::abs( y );
  return boost::numeric_cast<Distance>( abs_x + ( ( abs_y - abs_x ) / 2 ) );
}

Dirs parse_dirs( std::istream& input )
{
  boost::spirit::x3::symbols<Dir> directions;
  directions.add( "n", Dir::N )( "ne", Dir::NE )( "se", Dir::SE )( "s", Dir::S )( "sw", Dir::SW )( "nw", Dir::NW );

  const auto parser = directions % ',';

  Dirs dirs;
  const bool is_parsed = AoC::x3_parse( input, parser, boost::spirit::x3::space, dirs );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input directions data" );
  }

  return dirs;
}

}  // namespace

namespace AoC_2017
{

namespace problem_11
{

size_t solve_1( std::istream& input )
{
  const auto dirs = parse_dirs( input );

  const auto start_pos  = Pos{ 0, 0 };
  const auto finish_pos = ranges::accumulate( dirs, start_pos, &make_step );

  return calc_min_distance( finish_pos );
}

size_t solve_2( std::istream& input )
{
  const auto dirs = parse_dirs( input );

  // appending dummy dir since ranges v3 has no inclusive_scan yet :(
  const auto all_steps         = dirs | AoC::append( Dir::Dummy ) | ranges::view::exclusive_scan( Pos{ 0, 0 }, &make_step );
  const auto furthest_distance = ranges::max( all_steps | ranges::view::transform( &calc_min_distance ) );

  return furthest_distance;
}

AOC_REGISTER_PROBLEM( 2017_11, solve_1, solve_2 );

}  // namespace problem_11
}  // namespace AoC_2017
