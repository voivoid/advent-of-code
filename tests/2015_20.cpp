#include "boost/test/unit_test.hpp"

#include "AoC/2015/problem_20.h"
#include "utils/aoc_fixture.h"

using namespace AoC_2015::problem_20;

BOOST_FIXTURE_TEST_CASE( problem2015_20_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 1, run( &solve_1, "10" ) );
  BOOST_CHECK_EQUAL( 2, run( &solve_1, "30" ) );
  BOOST_CHECK_EQUAL( 3, run( &solve_1, "40" ) );
  BOOST_CHECK_EQUAL( 4, run( &solve_1, "70" ) );
  BOOST_CHECK_EQUAL( 6, run( &solve_1, "120" ) );
}
