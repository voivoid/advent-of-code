#include "AoC/2017/problem_02.h"

#include "AoC/problems_map.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/find_if.hpp"
#include "range/v3/algorithm/minmax.hpp"
#include "range/v3/getlines.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/to_container.hpp"
#include "range/v3/view/cartesian_product.hpp"
#include "range/v3/view/common.hpp"
#include "range/v3/view/split_when.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/hof/lift.hpp"
#include "boost/spirit/home/x3.hpp"

#include <istream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
using Num  = int;
using Nums = std::vector<Num>;

template <typename Pair>
bool is_divisible( const Pair p )
{
  const auto [ x, y ] = p;
  return x != y && x % y == 0;
}

Nums parse_row( const std::string& line )
{
  namespace x3      = boost::spirit::x3;
  const auto parser = *x3::int_;

  Nums nums;
  const bool is_parsed = AoC::x3_parse( line.cbegin(), line.cend(), parser, x3::space, nums );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input nums data" );
  }

  return nums;
}

Num get_minmax_diff( const Nums& nums )
{
  const auto mm = ranges::minmax( nums );
  return mm.max - mm.min;
}

Num get_evenly_div( const Nums& nums )
{
  const auto pairs = ranges::view::cartesian_product( nums, nums );
  const auto pairs_iter  = ranges::find_if( pairs, BOOST_HOF_LIFT( is_divisible ) );
  assert( pairs_iter != pairs.end() );

  const auto [ x, y ] = *pairs_iter;
  return x / y;
}

template <Num ( *calc_row )( const Nums& )>
int solve( std::istream& input )
{
  auto row_diffs = ranges::getlines( input ) | ranges::view::transform( &parse_row ) | ranges::view::transform( calc_row );
  return ranges::accumulate( row_diffs, 0 );
}

}  // namespace

namespace AoC_2017
{

namespace problem_02
{

int solve_1( std::istream& input )
{
  return solve<&get_minmax_diff>( input );
}

int solve_2( std::istream& input )
{
  return solve<&get_evenly_div>( input );
}

AOC_REGISTER_PROBLEM( 2017_02, solve_1, solve_2 );

}  // namespace problem_02

}  // namespace AoC_2017


#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  assert( ( std::vector<Num>{ 5, 1, 9, 5 } ) == parse_row( "5 1 9 5" ) );
  assert( 4 == ( get_evenly_div( { 5, 9, 2, 8 } ) ) );
}


REGISTER_IMPL_TEST( impl_tests );

#endif
