#include "boost/test/unit_test.hpp"

#include "utils/aoc_fixture.h"

#include "AoC/2016/problem_01.h"

using namespace AoC_2016::problem_01;

BOOST_FIXTURE_TEST_CASE( problem2016_01_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 5, run( &solve_1, "R2, L3" ) );
  BOOST_CHECK_EQUAL( 2, run( &solve_1, "R2, R2, R2" ) );
  BOOST_CHECK_EQUAL( 12, run( &solve_1, "R5, L5, R5, R3" ) );
}

BOOST_FIXTURE_TEST_CASE( problem2016_01_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 4, run( &solve_2, "R8, R4, R4, R8" ) );
}
