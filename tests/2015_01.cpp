#include "boost/test/unit_test.hpp"

#include "utils/aoc_fixture.h"

#include "AoC/2015/problem_01.h"

#include <stdexcept>

using namespace AoC_2015::problem_01;

BOOST_FIXTURE_TEST_CASE( problem2015_01_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 0, run( &solve_1, "(())" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_1, "()()" ) );
  BOOST_CHECK_EQUAL( 3, run( &solve_1, "(((" ) );
  BOOST_CHECK_EQUAL( 3, run( &solve_1, "(()(()(" ) );
  BOOST_CHECK_EQUAL( -1, run( &solve_1, "())" ) );
  BOOST_CHECK_EQUAL( -1, run( &solve_1, "))(" ) );
  BOOST_CHECK_EQUAL( -3, run( &solve_1, ")))" ) );
  BOOST_CHECK_EQUAL( -3, run( &solve_1, ")())())" ) );

  BOOST_CHECK_EQUAL( 0, run( &solve_1, "" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_1, "   " ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_1, " ( ( ) ) " ) );
  BOOST_CHECK_THROW( run( &solve_1, "(x)" ), std::invalid_argument );
}

BOOST_FIXTURE_TEST_CASE( problem2015_01_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 1, run( &solve_2, ")" ) );
  BOOST_CHECK_EQUAL( 5, run( &solve_2, "()())" ) );
}
