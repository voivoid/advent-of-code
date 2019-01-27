#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2018/problem_08.h"

using namespace AoC_2018::problem_08;

namespace
{
auto get_test_input()
{
  return "2 3 0 3 10 11 12 1 1 0 1 99 2 1 1 2";
}
}  // namespace

BOOST_FIXTURE_TEST_CASE( problem2018_08_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 138, run( &solve_1, get_test_input() ) );
}

BOOST_FIXTURE_TEST_CASE( problem2018_08_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 66, run( &solve_2, get_test_input() ) );
}
