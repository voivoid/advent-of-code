#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2017/problem_15.h"

using namespace AoC_2017::problem_15;

namespace
{
const auto* get_test_input()
{
  return R"(Generator A starts with 65
            Generator B starts with 8921)";
}
}  // namespace

BOOST_FIXTURE_TEST_CASE( problem2017_15_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 588, run( &solve_1, get_test_input() ) );
}

BOOST_FIXTURE_TEST_CASE( problem2017_15_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 309, run( &solve_2, get_test_input() ) );
}
