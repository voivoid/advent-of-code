#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2017/problem_14.h"

using namespace AoC_2017::problem_14;

namespace
{
const auto* get_test_input()
{
  return "flqrgnkx";
}
}  // namespace

BOOST_FIXTURE_TEST_CASE( problem2017_14_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 8108, run( &solve_1, get_test_input() ) );
}

BOOST_FIXTURE_TEST_CASE( problem2017_14_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 1242, run( &solve_2, get_test_input() ) );
}
