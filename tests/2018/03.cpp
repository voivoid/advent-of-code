#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2018/problem_03.h"

using namespace AoC_2018::problem_03;

namespace
{
auto get_test_input()
{
  return R"(#1 @ 1,3: 4x4
            #2 @ 3,1: 4x4
            #3 @ 5,5: 2x2)";
}
}  // namespace

BOOST_FIXTURE_TEST_CASE( problem2018_03_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 4, run( &solve_1, get_test_input() ) );
}

BOOST_FIXTURE_TEST_CASE( problem2018_03_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 3, run( &solve_2, get_test_input() ) );
}
