#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2018/problem_09.h"

using namespace AoC_2018::problem_09;

BOOST_FIXTURE_TEST_CASE( problem2018_09_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 32, run( &solve_1, "9 players; last marble is worth 25 points" ) );
  BOOST_CHECK_EQUAL( 8317, run( &solve_1, "10 players; last marble is worth 1618 points" ) );
  BOOST_CHECK_EQUAL( 146373, run( &solve_1, "13 players; last marble is worth 7999 points" ) );
  BOOST_CHECK_EQUAL( 2764, run( &solve_1, "17 players; last marble is worth 1104 points" ) );
  BOOST_CHECK_EQUAL( 54718, run( &solve_1, "21 players; last marble is worth 6111 points" ) );
  BOOST_CHECK_EQUAL( 37305, run( &solve_1, "30 players; last marble is worth 5807 points" ) );
}
