#include "AoC/2015/problem_03.h"

#include "rangev3.h"
#include <AoC/problems_map.h>

#include <set>
#include <stdexcept>

namespace
{
struct Pos
{
  const int x;
  const int y;
};

Pos operator+( const Pos p1, const Pos p2 )
{
  return { p1.x + p2.x, p1.y + p2.y };
}

bool operator<( const Pos p1, const Pos p2 )
{
  return std::tie( p1.x, p1.y ) < std::tie( p2.x, p2.y );
}

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


VisitedHouses solve( ranges::any_view<Instruction> instructions )
{
  const auto initial_pos = { Pos{ 0, 0 } };
  auto       positions   = ranges::view::concat( initial_pos, instructions | ranges::view::transform( parse_instruction ) );
  auto       visited     = positions | ranges::view::partial_sum();

  return visited | ranges::to_<std::set>();
}
}  // namespace

namespace AoC_2015
{

namespace problem_03
{
int solve_1( std::istream& input )
{
  ranges::istream_range<Instruction> instructions   = input;
  const auto                         visited_houses = solve( instructions );
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
