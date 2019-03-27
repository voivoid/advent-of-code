#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2017/problem_03.h"

using namespace AoC_2017::problem_03;

BOOST_FIXTURE_TEST_CASE( problem2017_03_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 0, run( &solve_1, "1" ) );
  BOOST_CHECK_EQUAL( 1, run( &solve_1, "2" ) );
  BOOST_CHECK_EQUAL( 2, run( &solve_1, "3" ) );
  BOOST_CHECK_EQUAL( 1, run( &solve_1, "4" ) );
  BOOST_CHECK_EQUAL( 3, run( &solve_1, "12" ) );
  BOOST_CHECK_EQUAL( 2, run( &solve_1, "23" ) );
  BOOST_CHECK_EQUAL( 31, run( &solve_1, "1024" ) );
}
