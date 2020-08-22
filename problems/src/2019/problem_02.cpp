#include "AoC/2019/problem_02.h"

#ifdef _MSC_VER
  #pragma warning( disable: 4804 ) // disable range-v3 warnings
#endif

#include "AoC/problems_map.h"
#include "AoC/utils/assert.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/find_if.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/cartesian_product.hpp"
#include "range/v3/view/chunk.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/take_while.hpp"

#include <istream>
#include <vector>

namespace
{
using IntCode = size_t;
using Ints    = std::vector<IntCode>;

constexpr IntCode AddOpCode = 1;
constexpr IntCode MulOpCode = 2;
constexpr IntCode HaltCode  = 99;

Ints parse_ints( std::istream& input )
{
  namespace x3 = boost::spirit::x3;

  const auto parser = AoC::x3_size_t_ % ',';

  Ints ints;
  const bool is_parsed = AoC::x3_parse( input, parser, x3::space, ints );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input int codes data" );
  }

  return ints;
}

void run_op( const IntCode op, const IntCode& in1, const IntCode& in2, IntCode& out )
{
  if ( op == AddOpCode )
  {
    out = in1 + in2;
  }
  else if ( op == MulOpCode )
  {
    out = in1 * in2;
  }
  else
  {
    AOC_ASSERT_FALSE();
  }
}

IntCode run_program( Ints& int_codes )
{
  auto int_chunks =
      int_codes | ranges::views::chunk( 4 ) | ranges::views::take_while( []( const auto& chunk ) { return chunk[ 0 ] != HaltCode; } );

  for ( const auto chunk : int_chunks )
  {
    const auto op   = chunk[ 0 ];
    const auto& in1 = int_codes[ chunk[ 1 ] ];
    const auto& in2 = int_codes[ chunk[ 2 ] ];
    auto& out       = int_codes[ chunk[ 3 ] ];

    run_op( op, in1, in2, out );
  }

  return int_codes[ 0 ];
}

IntCode run_program( Ints int_codes, const IntCode noun, const IntCode verb )
{
  int_codes[ 1 ] = noun;
  int_codes[ 2 ] = verb;

  return run_program( int_codes );
}

}  // namespace

namespace AoC_2019
{

namespace problem_02
{

size_t solve_1( std::istream& input )
{
  auto int_codes = parse_ints( input );
  return run_program( int_codes, 12, 2 );
}

size_t solve_2( std::istream& input )
{
  const auto int_codes = parse_ints( input );

  const auto nouns    = ranges::views::closed_iota( IntCode{ 0 }, IntCode{ 99 } );
  const auto verbs    = ranges::views::closed_iota( IntCode{ 0 }, IntCode{ 99 } );
  const auto nv_pairs = ranges::views::cartesian_product( nouns, verbs );

  const auto pair_iter = ranges::find_if( nv_pairs, [ &int_codes ]( const auto& pair ) {
    const auto [ noun, verb ] = pair;
    return run_program( int_codes, noun, verb ) == 19690720;
  } );

  assert( pair_iter != nv_pairs.end() );

  const auto [ result_noun, result_verb ] = *pair_iter;
  return 100 * result_noun + result_verb;
}

AOC_REGISTER_PROBLEM( 2019_02, solve_1, solve_2 );

}  // namespace problem_02

}  // namespace AoC_2019

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

#  include <sstream>

IntCode run_program( const char* input )
{
  std::istringstream ss( input );
  auto int_codes = parse_ints( ss );
  return run_program( int_codes );
}

static void impl_tests()
{
  assert( 3500 == run_program( "1,9,10,3,2,3,11,0,99,30,40,50" ) );
  assert( 2 == run_program( "1,0,0,0,99" ) );
  assert( 2 == run_program( "2,3,0,3,99" ) );
  assert( 2 == run_program( "2,4,4,5,99,0" ) );
  assert( 30 == run_program( "1,1,1,4,99,5,6,0,99" ) );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
