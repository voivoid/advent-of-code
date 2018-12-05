#include "boost/test/unit_test.hpp"

#include "utils/aoc_fixture.h"

#include "AoC/2018/problem_01.h"

using namespace AoC_2018::problem_01;

BOOST_FIXTURE_TEST_CASE( problem2018_01_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 3, run( &solve_1, "+1 -2 +3 +1" ) );
}

BOOST_FIXTURE_TEST_CASE( problem2016_08_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 2, run( &solve_2, "+1 -2 +3 +1" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_2, "+1 -1" ) );
  BOOST_CHECK_EQUAL( 10, run( &solve_2, "+3 +3 +4 -2 -4" ) );
  BOOST_CHECK_EQUAL( 5, run( &solve_2, "-6 +3 +8 +5 -6" ) );
  BOOST_CHECK_EQUAL( 14, run( &solve_2, "+7 +7 -2 -7 -4" ) );
}
