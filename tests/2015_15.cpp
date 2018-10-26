#include <boost/test/unit_test.hpp>

#include "utils/aoc_fixture.h"
#include <AoC/2015/problem_15.h>

namespace
{
auto get_test_input()
{
  return R"(Butterscotch: capacity -1, durability -2, flavor  6, texture  3, calories 8
                Cinnamon: capacity  2, durability  3, flavor -2, texture -1, calories 3)";
}
}  // namespace

using namespace AoC_2015::problem_15;

BOOST_FIXTURE_TEST_CASE( problem2015_15_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 62842880, run( &solve_1, get_test_input() ) );
}

BOOST_FIXTURE_TEST_CASE( problem2015_15_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 57600000, run( &solve_2, get_test_input() ) );
}
