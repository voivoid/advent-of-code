#include "AoC/2015/problem_20.h"

#include "AoC/problems_map.h"

#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/iota.hpp"

#include <vector>

#include <cassert>
#include <cmath>

namespace
{

std::vector<size_t> get_all_divisors( const size_t n )
{
  assert( n > 0 );

  std::vector<size_t> divisors;
  const auto sqrtn = static_cast<size_t>( sqrt( static_cast<double>( n ) ) );
  for ( size_t i = 1; i <= sqrtn; ++i )
  {
    if ( n % i == 0 )
    {
      divisors.push_back( i );

      if ( n / i != i )
      {
        divisors.push_back( n / i );
      }
    }
  }

  return divisors;
}

size_t calc_presents1( size_t house_index )
{
  auto divisors = get_all_divisors( house_index );
  return 10 * ranges::accumulate( divisors, size_t( 0 ) );
}

size_t calc_presents2( size_t house_index )
{
  auto divisors = get_all_divisors( house_index );
  return 11 *
         ranges::accumulate( divisors | ranges::view::filter( [house_index]( const auto divisor ) { return divisor * 50 >= house_index; } ),
                             size_t( 0 ) );
}

template <size_t ( *calc_presents )( size_t )>
int solve( std::istream& input )
{
  size_t presents_num = 0;
  input >> presents_num;

  auto houses_with_at_least_as_many_presents =
      ranges::view::ints( size_t( 1 ) ) |
      ranges::view::filter( [presents_num]( const size_t house_index ) { return calc_presents( house_index ) >= presents_num; } );

  const size_t first_house = houses_with_at_least_as_many_presents.front();

  return static_cast<int>( first_house );
}

}  // namespace

namespace AoC_2015
{

namespace problem_20
{

int solve_1( std::istream& input )
{
  return solve<&calc_presents1>( input );
}

int solve_2( std::istream& input )
{
  return solve<&calc_presents2>( input );
}

AOC_REGISTER_PROBLEM( 2015_20, solve_1, solve_2 );

}  // namespace problem_20

}  // namespace AoC_2015



#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

#  include "range/v3/action/sort.hpp"

static void impl_tests()
{
  assert( ( get_all_divisors( 1 ) | ranges::action::sort ) == std::vector<size_t>{ 1 } );
  assert( ( get_all_divisors( 2 ) | ranges::action::sort ) == ( std::vector<size_t>{ 1, 2 } ) );
  assert( ( get_all_divisors( 4 ) | ranges::action::sort ) == ( std::vector<size_t>{ 1, 2, 4 } ) );
  assert( ( get_all_divisors( 12 ) | ranges::action::sort ) == ( std::vector<size_t>{ 1, 2, 3, 4, 6, 12 } ) );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
