#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2017/problem_04.h"

using namespace AoC_2017::problem_04;

BOOST_FIXTURE_TEST_CASE( problem2017_04_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 1, run( &solve_1, "aa bb cc dd ee" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_1, "aa bb cc dd aa" ) );
  BOOST_CHECK_EQUAL( 1, run( &solve_1, "aa bb cc dd aaa" ) );
}

BOOST_FIXTURE_TEST_CASE( problem2017_04_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 1, run( &solve_2, "abcde fghij" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_2, "abcde xyz ecdab" ) );
  BOOST_CHECK_EQUAL( 1, run( &solve_2, "a ab abc abd abf abj" ) );
  BOOST_CHECK_EQUAL( 1, run( &solve_2, "iiii oiii ooii oooi oooo" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_2, "oiii ioii iioi iiio" ) );
}
