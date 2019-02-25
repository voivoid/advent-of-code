#include "AoC/2017/problem_01.h"

#include "AoC/problems_map.h"
#include "AoC/utils/ranges.h"

#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/cycle.hpp"
#include "range/v3/view/drop.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/view/zip.hpp"

#include <istream>
#include <string>

namespace
{
int digit_to_num( const char c )
{
  assert( isdigit( c ) );
  return c - '0';
}

std::string get_input_str( std::istream& input )
{
  std::string s;
  std::getline( input, s );
  return s;
}

template <typename Range>
int sum_matching_pairs( const Range& pairs )
{
  auto matching_pairs = pairs | ranges::view::filter( []( const auto pair ) {
                          auto [ a, b ] = pair;
                          return a == b;
                        } ) |
                        ranges::view::transform( []( const auto pair ) { return pair.first; } );

  return ranges::accumulate( matching_pairs, 0 );
}

int solve( std::istream& input, const bool drop_half )
{
  const std::string s = get_input_str( input );
  assert( !s.empty() );

  const size_t next_digits_shift = drop_half ? s.length() / 2 : 1;

  const auto digits = s | ranges::view::transform( &digit_to_num );
  const auto pairs  = ranges::view::zip( digits, digits | ranges::view::cycle | ranges::view::drop( next_digits_shift ) );

  return sum_matching_pairs( pairs );
}

}  // namespace

namespace AoC_2017
{

namespace problem_01
{

int solve_1( std::istream& input )
{
  return solve( input, false );
}

int solve_2( std::istream& input )
{
  return solve( input, true );
}

AOC_REGISTER_PROBLEM( 2017_01, solve_1, solve_2 );

}  // namespace problem_01

}  // namespace AoC_2017