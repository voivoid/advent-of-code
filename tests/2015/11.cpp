#include "boost/test/unit_test.hpp"

#include "AoC/2015/problem_11.h"
#include "aoc_fixture.h"

using namespace AoC_2015::problem_11;


BOOST_FIXTURE_TEST_CASE( problem2015_11_1, AocFixture )
{
  BOOST_CHECK_EQUAL( "abcdffaa", run( &solve_1, "abcdefgh" ) );
}
