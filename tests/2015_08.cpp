#include <boost/test/unit_test.hpp>

#include "utils/aoc_fixture.h"
#include <AoC/2015/problem_08.h>

using namespace AoC_2015::problem_08;

BOOST_FIXTURE_TEST_CASE( problem2015_08_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 2, run( &solve_1, "\"\"" ) );
  BOOST_CHECK_EQUAL( 2, run( &solve_1, "\"abc\"" ) );
  BOOST_CHECK_EQUAL( 3, run( &solve_1, "\"aaa\"aaa\"" ) );
  BOOST_CHECK_EQUAL( 5, run( &solve_1, "\"\\x27\"" ) );
  BOOST_CHECK_EQUAL( 9, run( &solve_1, R"("sjdivfriyaaqa\xd2v\"k\"mpcu\"yyu\"en")" ) );
}

BOOST_FIXTURE_TEST_CASE( problem2015_08_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 4, run( &solve_2, "\"\"" ) );
  BOOST_CHECK_EQUAL( 4, run( &solve_2, "\"abc\"" ) );
  BOOST_CHECK_EQUAL( 6, run( &solve_2, "\"aaa\"aaa\"" ) );
  BOOST_CHECK_EQUAL( 5, run( &solve_2, "\"\\x27\"" ) );
}
