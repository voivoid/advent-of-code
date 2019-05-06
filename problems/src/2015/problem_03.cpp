#include "AoC/2015/problem_03.h"

#include "AoC/problems_map.h"
#include "AoC/utils/geo.h"

#include "range/v3/range/conversion.hpp"
#include "range/v3/view/concat.hpp"
#include "range/v3/view/drop.hpp"
#include "range/v3/view/istream.hpp"
#include "range/v3/view/partial_sum.hpp"
#include "range/v3/view/single.hpp"
#include "range/v3/view/stride.hpp"
#include "range/v3/view/transform.hpp"

#include <istream>
#include <stdexcept>
#include <tuple>
#include <unordered_set>

namespace
{
using Pos = AoC::Coord;

using Instruction   = char;
using VisitedHouses = std::unordered_set<Pos, AoC::GeoHasher>;

Pos instruction_to_offset( const Instruction instruction )
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
VisitedHouses get_visited_houses( Range&& instructions )
{
  const auto initial_pos = ranges::view::single( Pos{ 0, 0 } );

  auto move_offsets      = instructions | ranges::view::transform( &instruction_to_offset );
  auto positions_visited = ranges::view::concat( initial_pos, move_offsets ) | ranges::view::partial_sum;

  return positions_visited | ranges::to<VisitedHouses>();
}
}  // namespace

namespace AoC_2015
{

namespace problem_03
{
size_t solve_1( std::istream& input )
{
  auto instructions = ranges::istream<Instruction>( input );
  return get_visited_houses( instructions ).size();
}

size_t solve_2( std::istream& input )
{
  const auto instructions        = ranges::istream<Instruction>( input ) | ranges::to_vector;
  const auto santas_instructions = instructions | ranges::view::stride( 2 );
  const auto robo_instructions   = instructions | ranges::view::drop( 1 ) | ranges::view::stride( 2 );

  const auto santas_houses = get_visited_houses( santas_instructions );
  const auto robo_houses   = get_visited_houses( robo_instructions );

  VisitedHouses all_visited_houses;
  all_visited_houses.insert( santas_houses.cbegin(), santas_houses.cend() );
  all_visited_houses.insert( robo_houses.cbegin(), robo_houses.cend() );

  return all_visited_houses.size();
}

AOC_REGISTER_PROBLEM( 2015_03, solve_1, solve_2 );

}  // namespace problem_03

}  // namespace AoC_2015
