#include "AoC/2015/problem_25.h"

#include "AoC/problems_map.h"
#include "AoC/utils/algo.h"
#include "AoC/utils/parse.h"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/spirit/home/x3.hpp"

namespace
{
struct CodePos
{
  size_t row;
  size_t col;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( CodePos, row, col )

namespace
{

CodePos parse_code_pos( const std::string& input )
{
  namespace x3 = boost::spirit::x3;

  auto parser = x3::int_ > x3::int_;

  CodePos code_pos;
  const bool is_parsed = AoC::x3_parse( input.cbegin(), input.cend(), parser, x3::char_ - x3::digit, code_pos );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input code position data" );
  }

  return code_pos;
}

size_t calc_code_index( const CodePos pos )
{
  assert( pos.row > 0 );
  assert( pos.col > 0 );

  const auto n         = pos.row - 1 + pos.col - 1;
  const auto row_index = ( n * ( n + 1 ) / 2 ) + 1;
  return row_index + pos.col - 1;
}

int calc_code( const size_t code_index )
{
  std::uint64_t code = 20151125;
  AoC::iterate_n( code, []( const auto c ) { return ( c * 252533 ) % 33554393; }, code_index - 1 );
  return static_cast<int>( code );
}

}  // namespace

namespace AoC_2015
{

namespace problem_25
{

int solve_1( std::istream& input )
{
  std::string input_str;
  std::getline( input, input_str );
  const auto code_pos   = parse_code_pos( input_str );
  const auto code_index = calc_code_index( code_pos );
  return calc_code( code_index );
}

int solve_2( std::istream& )
{
  // there is no part 2 in the problem
  return 0;
}

AOC_REGISTER_PROBLEM( 2015_25, solve_1, solve_2 );

}  // namespace problem_25

}  // namespace AoC_2015


#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  const auto code_pos =
      parse_code_pos( "To continue, please consult the code grid in the manual.  Enter the code at row 2981, column 3075." );
  assert( 3075 == code_pos.col );
  assert( 2981 == code_pos.row );

  assert( 12 == calc_code_index( { 4, 2 } ) );
  assert( 14 == calc_code_index( { 2, 4 } ) );
  assert( 15 == calc_code_index( { 1, 5 } ) );
  assert( 11 == calc_code_index( { 5, 1 } ) );
  assert( 1 == calc_code_index( { 1, 1 } ) );

  assert( 20151125 == calc_code( 1 ) );
  assert( 31916031 == calc_code( 2 ) );
  assert( 18749137 == calc_code( 3 ) );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
