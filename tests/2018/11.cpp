#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2018/problem_11.h"

using namespace AoC_2018::problem_11;

BOOST_FIXTURE_TEST_CASE( problem2018_11_1, AocFixture )
{
  BOOST_CHECK_EQUAL( "33,45", run( &solve_1, "18" ) );
  BOOST_CHECK_EQUAL( "21,61", run( &solve_1, "42" ) );
}

BOOST_FIXTURE_TEST_CASE( problem2018_11_2, AocFixture )
{
  BOOST_CHECK_EQUAL( "90,269,16", run( &solve_2, "18" ) );
  BOOST_CHECK_EQUAL( "232,251,12", run( &solve_2, "42" ) );
}
