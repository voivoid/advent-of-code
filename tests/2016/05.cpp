#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2016/problem_05.h"

using namespace AoC_2016::problem_05;

BOOST_FIXTURE_TEST_CASE( problem2016_05_1, AocFixture )
{
  BOOST_CHECK_EQUAL( "18f47a30", run( &solve_1, "abc" ) );
}

BOOST_FIXTURE_TEST_CASE( problem2016_05_2, AocFixture )
{
  BOOST_CHECK_EQUAL( "05ace8e3", run( &solve_2, "abc" ) );
}
