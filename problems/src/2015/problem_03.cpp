#include "AoC/2015/problem_03.h"

#include "range/v3/istream_range.hpp"
#include "range/v3/to_container.hpp"
#include "range/v3/view/concat.hpp"
#include "range/v3/view/drop.hpp"
#include "range/v3/view/partial_sum.hpp"
#include "range/v3/view/single.hpp"
#include "range/v3/view/stride.hpp"
#include "range/v3/view/transform.hpp"

#include "AoC/problems_map.h"
#include "AoC/utils/geo.h"

#include <istream>
#include <set>
#include <stdexcept>
#include <tuple>

namespace
{
using Pos = AoC::Coord;

using Instruction   = char;
using VisitedHouses = std::set<Pos>;

Pos parse_instruction( const Instruction instruction )
{
  switch ( instruction )
  {
    case '^': return { 0, -1 };
    case '<': return { -1, 0 };
    case '>': return { 1, 0 };
    case 'v': return { 0, 1 };
  }

  throw std::runtime_error( "Unexpected input" );
}

template <typename Range>
VisitedHouses solve( Range instructions )
{
  const auto initial_pos = ranges::view::single( Pos{ 0, 0 } );
  auto positions         = ranges::view::concat( initial_pos, instructions | ranges::view::transform( parse_instruction ) );
  auto visited           = positions | ranges::view::partial_sum;

  return visited | ranges::to<std::set>;
}
}  // namespace

namespace AoC_2015
{

namespace problem_03
{
int solve_1( std::istream& input )
{
  auto instructions         = ranges::istream_range<Instruction>( input );
  const auto visited_houses = solve( instructions );
  return static_cast<int>( visited_houses.size() );
}

int solve_2( std::istream& input )
{
  const auto instructions        = ranges::istream<Instruction>( input ) | ranges::to_vector;
  const auto santas_instructions = instructions | ranges::view::stride( 2 );
  const auto robo_instructions   = instructions | ranges::view::drop( 1 ) | ranges::view::stride( 2 );

  const auto santas_houses = solve( santas_instructions );
  const auto robo_houses   = solve( robo_instructions );

  std::set<Pos> visited_houses;
  visited_houses.insert( santas_houses.cbegin(), santas_houses.cend() );
  visited_houses.insert( robo_houses.cbegin(), robo_houses.cend() );

  return static_cast<int>( visited_houses.size() );
}

AOC_REGISTER_PROBLEM( 2015_03, solve_1, solve_2 );

}  // namespace problem_03

}  // namespace AoC_2015
