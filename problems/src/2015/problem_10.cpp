#include "AoC/2015/problem_10.h"

#include "AoC/problems_map.h"
#include "AoC/utils/algo.h"

#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/group_by.hpp"
#include "range/v3/view/iota.hpp"

#include <functional>
#include <istream>
#include <sstream>
#include <string>

namespace
{
std::string transform( const std::string& digits )
{
  const auto digit_groups = digits | ranges::view::group_by( std::equal_to<char>{} );

  std::ostringstream acc;
  for ( const auto group : digit_groups )
  {
    acc << ranges::distance( group );
    acc << group[ 0 ];
  }

  return acc.str();
}

int solve( std::istream& input, const size_t iterations )
{
  std::string digits;
  input >> digits;

  digits = AoC::iterate_n( digits, &transform, iterations );

  return static_cast<int>( digits.length() );
}
}  // namespace

namespace AoC_2015
{

namespace problem_10
{

int solve_1( std::istream& input )
{
  return solve( input, 40 );
}

int solve_2( std::istream& input )
{
  return solve( input, 50 );
}

AOC_REGISTER_PROBLEM( 2015_10, solve_1, solve_2 );

}  // namespace problem_10

}  // namespace AoC_2015


#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  assert( "11" == transform( "1" ) );
  assert( "21" == transform( "11" ) );
  assert( "1211" == transform( "21" ) );
  assert( "111221" == transform( "1211" ) );
  assert( "312211" == transform( "111221" ) );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
