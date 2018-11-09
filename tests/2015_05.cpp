#include "boost/test/unit_test.hpp"

#include "AoC/2015/problem_05.h"
#include "utils/aoc_fixture.h"

using namespace AoC_2015::problem_05;

BOOST_FIXTURE_TEST_CASE( problem2015_05_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 1, run( &solve_1, "ugknbfddgicrmopn" ) );
  BOOST_CHECK_EQUAL( 1, run( &solve_1, "aaa" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_1, "jchzalrnumimnmhp" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_1, "haegwjzuvuyypxyu" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_1, "dvszwmarrgswjxmb" ) );
}

BOOST_FIXTURE_TEST_CASE( problem2015_05_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 1, run( &solve_2, "qjhvhtzxzqqjkmpb" ) );
  BOOST_CHECK_EQUAL( 1, run( &solve_2, "xxyxx" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_2, "uurcxstgmygtbstg" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_2, "ieodomkazucvgmuy" ) );
}
