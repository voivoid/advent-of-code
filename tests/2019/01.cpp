#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2019/problem_01.h"

using namespace AoC_2019::problem_01;

BOOST_FIXTURE_TEST_CASE( problem2019_01_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 2, run( &solve_1, "12"));
  BOOST_CHECK_EQUAL( 2, run( &solve_1, "14"));
  BOOST_CHECK_EQUAL( 654, run( &solve_1, "1969"));
  BOOST_CHECK_EQUAL( 33583, run( &solve_1, "100756"));
}

BOOST_FIXTURE_TEST_CASE( problem2019_01_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 2, run( &solve_2, "12"));
  BOOST_CHECK_EQUAL( 966, run( &solve_2, "1969"));
  BOOST_CHECK_EQUAL( 50346, run( &solve_2, "100756"));
}
