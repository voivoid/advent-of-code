#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2018/problem_06.h"

using namespace AoC_2018::problem_06;

namespace
{
auto get_test_input()
{
  return R"(1, 1
            1, 6
            8, 3
            3, 4
            5, 5
            8, 9)";
}
}  // namespace

BOOST_FIXTURE_TEST_CASE( problem2018_06_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 17, run( &solve_1, get_test_input() ) );
}
