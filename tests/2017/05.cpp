#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2017/problem_05.h"

using namespace AoC_2017::problem_05;

namespace
{
auto get_input()
{
  return R"(0
            3
            0
            1
            -3)";
}
}  // namespace

BOOST_FIXTURE_TEST_CASE( problem2017_05_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 5, run( &solve_1, get_input() ) );
}

BOOST_FIXTURE_TEST_CASE( problem2017_05_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 10, run( &solve_2, get_input() ) );
}
