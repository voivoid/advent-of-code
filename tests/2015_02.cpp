#include "boost/test/unit_test.hpp"

#include "AoC/2015/problem_02.h"
#include "utils/aoc_fixture.h"

using namespace AoC_2015::problem_02;

BOOST_FIXTURE_TEST_CASE( problem2015_02_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 58, run( &solve_1, "2x3x4" ) );
  BOOST_CHECK_EQUAL( 43, run( &solve_1, "1x1x10" ) );
}

BOOST_FIXTURE_TEST_CASE( problem2015_02_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 34, run( &solve_2, "2x3x4" ) );
  BOOST_CHECK_EQUAL( 14, run( &solve_2, "1x1x10" ) );
}
