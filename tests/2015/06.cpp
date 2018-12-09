#include "boost/test/unit_test.hpp"

#include "AoC/2015/problem_06.h"
#include "aoc_fixture.h"

using namespace AoC_2015::problem_06;

BOOST_FIXTURE_TEST_CASE( problem2015_06_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 9, run( &solve_1, "turn on 0,0 through 2,2" ) );
  BOOST_CHECK_EQUAL( 1000000, run( &solve_1, "turn on 0,0 through 999,999" ) );
  BOOST_CHECK_EQUAL( 1000, run( &solve_1, "turn on 0,0 through 999,0" ) );
  BOOST_CHECK_EQUAL( 4, run( &solve_1, "toggle 499,499 through 500,500" ) );
}

BOOST_FIXTURE_TEST_CASE( problem2015_06_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 1, run( &solve_2, "turn on 0,0 through 0,0" ) );
  BOOST_CHECK_EQUAL( 2000000, run( &solve_2, "toggle 0,0 through 999,999" ) );
}
