#include "boost/test/unit_test.hpp"

#include "AoC/2015/problem_07.h"
#include "utils/aoc_fixture.h"

using namespace AoC_2015::problem_07;

BOOST_FIXTURE_TEST_CASE( problem2015_07_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 123, run( &solve_1, "123 -> a" ) );
  BOOST_CHECK_EQUAL( 72, run( &solve_1, "123 -> x\n456 -> y\nx AND y -> a" ) );
  BOOST_CHECK_EQUAL( 507, run( &solve_1, "507 -> x\n456 -> y\nx OR y -> a" ) );
  BOOST_CHECK_EQUAL( 492, run( &solve_1, "123 -> x\nx LSHIFT 2 -> a" ) );
  BOOST_CHECK_EQUAL( 114, run( &solve_1, "456 -> y\ny RSHIFT 2 -> a" ) );
  BOOST_CHECK_EQUAL( 65412, run( &solve_1, "123 -> x\nNOT x -> a" ) );
  BOOST_CHECK_EQUAL( 65079, run( &solve_1, "456 -> y\nNOT y -> a" ) );
}
