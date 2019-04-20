#include "AoC/2017/problem_17.h"

#include "AoC/problems_map.h"
#include "AoC/utils/algo.h"
#include "AoC/utils/zipper.h"

#include <istream>
#include <vector>

namespace
{

size_t solve_1_impl( const size_t steps_forward )
{
  std::vector<size_t> values{ 0 };
  AoC::Zipper zipper( values );

  size_t current_elem = 1;
  while ( current_elem <= 2017 )
  {
    zipper.next( steps_forward );
    zipper.append( current_elem );
    zipper.next();

    ++current_elem;
  }

  zipper.next();
  return zipper.current();
}

size_t get_steps_num( std::istream& input )
{
  size_t steps_forward = 0;
  input >> steps_forward;
  return steps_forward;
}

}  // namespace

namespace AoC_2017
{

namespace problem_17
{

size_t solve_1( std::istream& input )
{
  return solve_1_impl( get_steps_num( input ) );
}

size_t solve_2( std::istream& input )
{
  const auto steps = get_steps_num( input );

  size_t next_value_after_zero = 0;
  for ( size_t i = 1, index = 0; i <= 50'000'000; ++i )
  {
    index = ( index + steps + 1 ) % i;
    if ( index == 0 )
    {
      next_value_after_zero = i;
    }
  }

  return next_value_after_zero;
}

AOC_REGISTER_PROBLEM( 2017_17, solve_1, solve_2 );

}  // namespace problem_17
}  // namespace AoC_2017

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  assert( 638 == solve_1_impl( 3 ) );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
