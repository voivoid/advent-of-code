#include "AoC/2018/problem_01.h"

#include "AoC/problems_map.h"

#include "range/v3/istream_range.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/to_container.hpp"
#include "range/v3/view/cycle.hpp"
#include "range/v3/view/partial_sum.hpp"

#include <istream>
#include <stdexcept>
#include <unordered_set>

namespace AoC_2018
{

namespace problem_01
{

int solve_1( std::istream& input )
{
  ranges::istream_range<int> frequencies( input );
  return ranges::accumulate( frequencies, 0 );
}

int solve_2( std::istream& input )
{
  const auto frequencies = ranges::istream_range<int>( input ) | ranges::to_vector;

  std::unordered_set<int> resulting_frequencies = { 0 };
  for ( const auto f : frequencies | ranges::view::cycle | ranges::view::partial_sum )
  {
    if ( resulting_frequencies.find( f ) != resulting_frequencies.cend() )
    {
      return f;
    }

    resulting_frequencies.insert( f );
  }

  throw std::runtime_error( "This should never be executed" );
}

AOC_REGISTER_PROBLEM( 2018_01, solve_1, solve_2 );

}  // namespace problem_01

}  // namespace AoC_2018
