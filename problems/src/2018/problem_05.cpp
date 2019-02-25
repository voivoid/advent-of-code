#include "AoC/2018/problem_05.h"

#include "AoC/problems_map.h"

#include "range/v3/algorithm/min.hpp"
#include "range/v3/istream_range.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/hof/lift.hpp"

#include <istream>
#include <set>
#include <stack>

#include <cctype>

namespace
{
char to_lower( const char c )
{
  return static_cast<char>( std::tolower( static_cast<int>( c ) ) );
}

bool is_uppercase( const char c )
{
  return std::isupper( static_cast<int>( c ) );
}

bool should_be_reduced( const char c1, const char c2 )
{
  return to_lower( c1 ) == to_lower( c2 ) && ( is_uppercase( c1 ) != is_uppercase( c2 ) );
}

std::set<char> get_unique_polymers( const std::string& input )
{
  return input | ranges::view::transform( &to_lower );
}

auto reduce_polymer( std::stack<char>& stack, const char p )
{
  if ( stack.empty() || !should_be_reduced( stack.top(), p ) )
  {
    stack.push( p );
  }
  else
  {
    stack.pop();
  }

  return std::ref( stack );
}

template <typename Range>
size_t do_reductions( Range data )
{
  std::stack<char> polymers_stack;
  ranges::accumulate( data, std::ref( polymers_stack ), &reduce_polymer );

  return polymers_stack.size();
}

}  // namespace

namespace AoC_2018
{

namespace problem_05
{

int solve_1( std::istream& input )
{
  return static_cast<int>( do_reductions( ranges::istream<char>( input ) ) );
}

int solve_2( std::istream& input )
{
  const std::string data{ std::istream_iterator<char>( input ), std::istream_iterator<char>() };
  const auto unique_polymers = get_unique_polymers( data );

  const auto reduction_sizes = unique_polymers | ranges::view::transform( [&data]( const char polymer ) {
                                 return data | ranges::view::filter( [polymer]( const char c ) { return to_lower( c ) != polymer; } );
                               } ) |
                               ranges::view::transform( BOOST_HOF_LIFT( &do_reductions ) );

  return static_cast<int>( ranges::min( reduction_sizes ) );
}

AOC_REGISTER_PROBLEM( 2018_05, solve_1, solve_2 );

}  // namespace problem_05

}  // namespace AoC_2018
