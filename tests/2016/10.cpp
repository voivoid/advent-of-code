#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2016/problem_10.h"

using namespace AoC_2016::problem_10;

BOOST_FIXTURE_TEST_CASE( problem2016_10_2, AocFixture )
{
  const auto input = R"(value 5 goes to bot 2
                        bot 2 gives low to bot 1 and high to bot 0
                        value 3 goes to bot 1
                        bot 1 gives low to output 1 and high to bot 0
                        bot 0 gives low to output 2 and high to output 0
                        value 2 goes to bot 2)";

  BOOST_CHECK_EQUAL( 30, run( &solve_2, input ) );
}
