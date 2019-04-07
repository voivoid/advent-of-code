#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2017/problem_11.h"

using namespace AoC_2017::problem_11;

BOOST_FIXTURE_TEST_CASE( problem2017_11_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 3, run( &solve_1, "ne,ne,ne" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_1, "ne,ne,sw,sw" ) );
  BOOST_CHECK_EQUAL( 2, run( &solve_1, "ne,ne,s,s" ) );
  BOOST_CHECK_EQUAL( 3, run( &solve_1, "se,sw,se,sw,sw" ) );
}
