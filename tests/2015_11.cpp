#include <boost/test/unit_test.hpp>

#include "utils/aoc_fixture.h"
#include <AoC/2015/problem_11.h>

using namespace AoC_2015::problem_11;


BOOST_FIXTURE_TEST_CASE( problem2015_11_1, AocFixture )
{
  BOOST_CHECK_EQUAL( "abcdffaa", run( &solve_1, "abcdefgh" ) );
}
