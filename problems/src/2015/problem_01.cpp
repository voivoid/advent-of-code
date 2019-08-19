#include "AoC/2015/problem_01.h"

#include "AoC/problems_map.h"

#include "range/v3/iterator/operations.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/istream.hpp"
#include "range/v3/view/partial_sum.hpp"
#include "range/v3/view/take_while.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/numeric/conversion/cast.hpp"

#include <istream>
#include <stdexcept>

#include <cctype>

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

  throw std::invalid_argument( "Failed to parse instruction input data" );
}

bool is_not_space( const char c )
{
  return std::isspace( c ) == 0;
}

auto get_instructions( std::istream& input )
{
  return ranges::istream<Instruction>( input ) | ranges::views::filter( &is_not_space ) | ranges::views::transform( &parse_instruction );
}

int calc_last_visited_floor( std::istream& input )
{
  return ranges::accumulate( get_instructions( input ), 0 );
}

size_t calc_number_of_steps_to_reach_basement( std::istream& input )
{
  auto instructions_to_reach_basement =
      get_instructions( input ) | ranges::views::partial_sum | ranges::views::take_while( []( int floor ) { return floor >= 0; } );

  return 1 + boost::numeric_cast<size_t>( ranges::distance( instructions_to_reach_basement ) );
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

size_t solve_2( std::istream& input )
{
  return calc_number_of_steps_to_reach_basement( input );
}

AOC_REGISTER_PROBLEM( 2015_01, solve_1, solve_2 );

}  // namespace problem_01

}  // namespace AoC_2015
