#include <boost/test/unit_test.hpp>

#include "utils/aoc_fixture.h"
#include <AoC/2015/problem_03.h>

using namespace AoC_2015::problem_03;

BOOST_FIXTURE_TEST_CASE( problem2015_03_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 2, run( &solve_1, ">" ) );
  BOOST_CHECK_EQUAL( 4, run( &solve_1, "^>v<" ) );
  BOOST_CHECK_EQUAL( 2, run( &solve_1, "^v^v^v^v^v" ) );
}

BOOST_FIXTURE_TEST_CASE( problem2015_03_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 3, run( &solve_2, "^v" ) );
  BOOST_CHECK_EQUAL( 3, run( &solve_2, "^>v<" ) );
  BOOST_CHECK_EQUAL( 11, run( &solve_2, "^v^v^v^v^v" ) );
}
