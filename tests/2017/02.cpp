#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2017/problem_02.h"

using namespace AoC_2017::problem_02;

BOOST_FIXTURE_TEST_CASE( problem2017_02_1, AocFixture )
{
  const auto input = R"(5 1 9 5
                        7 5 3
                        2 4 6 8)";

  BOOST_CHECK_EQUAL( 18, run( &solve_1, input ) );
}

BOOST_FIXTURE_TEST_CASE( problem2017_02_2, AocFixture )
{
  const auto input = R"(5 9 2 8
                        9 4 7 3
                        3 8 6 5)";

  BOOST_CHECK_EQUAL( 9, run( &solve_2, input ) );
}
