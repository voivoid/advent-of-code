#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2017/problem_13.h"

using namespace AoC_2017::problem_13;

namespace
{
const auto* get_test_input()
{
  return R"(0: 3
            1: 2
            4: 4
            6: 4)";
}
}  // namespace

BOOST_FIXTURE_TEST_CASE( problem2017_13_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 24, run( &solve_1, get_test_input() ) );
}

BOOST_FIXTURE_TEST_CASE( problem2017_13_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 10, run( &solve_2, get_test_input() ) );
}
