#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2017/problem_06.h"

using namespace AoC_2017::problem_06;

namespace
{
auto get_input()
{
  return "0 2 7 0";
}
}  // namespace

BOOST_FIXTURE_TEST_CASE( problem2017_06_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 5, run( &solve_1, get_input() ) );
}

BOOST_FIXTURE_TEST_CASE( problem2017_06_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 4, run( &solve_2, get_input() ) );
}
