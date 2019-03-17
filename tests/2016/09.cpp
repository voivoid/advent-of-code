#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2016/problem_09.h"

using namespace AoC_2016::problem_09;

BOOST_FIXTURE_TEST_CASE( problem2016_09_1, AocFixture )
{
  BOOST_CHECK_EQUAL( "6", run( &solve_1, "ADVENT" ) );
  BOOST_CHECK_EQUAL( "7", run( &solve_1, "A(1x5)BC" ) );
  BOOST_CHECK_EQUAL( "9", run( &solve_1, "(3x3)XYZ" ) );
  BOOST_CHECK_EQUAL( "11", run( &solve_1, "A(2x2)BCD(2x2)EFG" ) );
  BOOST_CHECK_EQUAL( "6", run( &solve_1, "(6x1)(1x3)A" ) );
  BOOST_CHECK_EQUAL( "18", run( &solve_1, "X(8x2)(3x3)ABCY" ) );
}

BOOST_FIXTURE_TEST_CASE( problem2016_09_2, AocFixture )
{
  BOOST_CHECK_EQUAL( "9", run( &solve_2, "(3x3)XYZ" ) );
  BOOST_CHECK_EQUAL( "20", run( &solve_2, "X(8x2)(3x3)ABCY" ) );
  BOOST_CHECK_EQUAL( "241920", run( &solve_2, "(27x12)(20x12)(13x14)(7x10)(1x12)A" ) );
  BOOST_CHECK_EQUAL( "445", run( &solve_2, "(25x3)(3x3)ABC(2x3)XY(5x2)PQRSTX(18x9)(3x2)TWO(5x7)SEVEN" ) );
}
