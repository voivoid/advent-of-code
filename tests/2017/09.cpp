#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2017/problem_09.h"

using namespace AoC_2017::problem_09;

BOOST_FIXTURE_TEST_CASE( problem2017_09_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 1, run( &solve_1, "{}" ) );
  BOOST_CHECK_EQUAL( 6, run( &solve_1, "{{{}}}" ) );
  BOOST_CHECK_EQUAL( 5, run( &solve_1, "{{},{}}" ) );
  BOOST_CHECK_EQUAL( 16, run( &solve_1, "{{{},{},{{}}}}" ) );
  BOOST_CHECK_EQUAL( 1, run( &solve_1, "{<a>,<a>,<a>,<a>}" ) );
  BOOST_CHECK_EQUAL( 9, run( &solve_1, "{{<ab>},{<ab>},{<ab>},{<ab>}}" ) );
  BOOST_CHECK_EQUAL( 9, run( &solve_1, "{{<!!>},{<!!>},{<!!>},{<!!>}}" ) );
  BOOST_CHECK_EQUAL( 3, run( &solve_1, "{{<a!>},{<a!>},{<a!>},{<ab>}}" ) );
}

BOOST_FIXTURE_TEST_CASE( problem2017_09_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 0, run( &solve_2, "<>" ) );
  BOOST_CHECK_EQUAL( 17, run( &solve_2, "<random characters>" ) );
  BOOST_CHECK_EQUAL( 3, run( &solve_2, "<<<<>" ) );
  BOOST_CHECK_EQUAL( 2, run( &solve_2, "<{!>}>" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_2, "<!!>" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_2, "<!!!>>" ) );
  BOOST_CHECK_EQUAL( 10, run( &solve_2, "<{o\"i!a,<{i<a>" ) );
}
