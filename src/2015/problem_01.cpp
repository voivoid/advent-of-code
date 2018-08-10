#include "AoC/2015/problem_01.h"

#include <range/v3/distance.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/partial_sum.hpp>
#include <range/v3/view/take_while.hpp>
#include <range/v3/view/transform.hpp>


#include <AoC/problems_map.h>

#include <stdexcept>

namespace
{

using Instruction = char;

int parse_instruction( const Instruction instruction )
{
  switch ( instruction )
  {
    case '(': return +1;
    case ')': return -1;
  }

  throw std::runtime_error( "Unexpected input" );
}

int calc_last_visited_floor( ranges::istream_range<Instruction> instructions )
{
  auto offsets = instructions | ranges::view::transform( &parse_instruction );
  return ranges::accumulate( offsets, 0 );
}

int calc_number_of_steps_to_reach_basement( ranges::istream_range<Instruction> instructions )
{
  auto moves_to_reach_basement = instructions | ranges::view::transform( &parse_instruction ) | ranges::view::partial_sum() |
                                 ranges::view::take_while( []( int floor ) { return floor >= 0; } );

  return 1 + static_cast<int>( ranges::distance( moves_to_reach_basement ) );
}

}  // namespace

namespace AoC_2015
{

namespace problem_01
{

int solve_1( std::istream& input )
{
  return calc_last_visited_floor( input );
}

int solve_2( std::istream& input )
{
  return calc_number_of_steps_to_reach_basement( input );
}

AOC_REGISTER_PROBLEM( 2015_01, solve_1, solve_2 );

}  // namespace problem_01

}  // namespace AoC_2015
