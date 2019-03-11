#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2016/problem_08.h"

using namespace AoC_2016::problem_08;

BOOST_FIXTURE_TEST_CASE( problem2016_08_1, AocFixture )
{
  const auto input = R"(rect 3x2
                        rotate column x=1 by 1
                        rotate row y=0 by 4
                        rotate column x=1 by 1)";
  BOOST_CHECK_EQUAL( 6, run( &solve_1, input ) );
}

BOOST_FIXTURE_TEST_CASE( problem2016_08_2, AocFixture )
{
}
