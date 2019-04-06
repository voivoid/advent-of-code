#include "AoC/2017/problem_05.h"

#include "AoC/problems_map.h"

#include "range/v3/range/conversion.hpp"
#include "range/v3/view/istream.hpp"

#include <iostream>
#include <vector>

namespace
{
using Value = int;

template <Value ( *mod_value )( Value& )>
int solve( std::istream& input )
{
  auto nums = ranges::istream<Value>( input ) | ranges::to_vector;

  int steps_num = 0;
  auto iter     = nums.begin();
  while ( true )
  {
    const Value current_val = mod_value( *iter );
    ++steps_num;

    const auto vals_on_right = ( nums.cend() - iter );
    const auto vals_on_left  = ( iter - nums.cbegin() );

    const bool right_exit = current_val > 0 && current_val >= vals_on_right;
    const bool left_exit  = current_val < 0 && -current_val >= vals_on_left;
    if ( left_exit || right_exit )
    {
      break;
    }

    iter += current_val;
  }

  return steps_num;
}

Value mod_value_1( Value& val )
{
  return val++;
}

Value mod_value_2( Value& val )
{
  return val >= 3 ? val-- : val++;
}

}  // namespace

namespace AoC_2017
{

namespace problem_05
{

int solve_1( std::istream& input )
{
  return solve<&mod_value_1>( input );
}

int solve_2( std::istream& input )
{
  return solve<&mod_value_2>( input );
}

AOC_REGISTER_PROBLEM( 2017_05, solve_1, solve_2 );

}  // namespace problem_05

}  // namespace AoC_2017
