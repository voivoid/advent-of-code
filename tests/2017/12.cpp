#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2017/problem_12.h"

using namespace AoC_2017::problem_12;

namespace
{
const auto* get_test_input()
{
  return R"(0 <-> 2
            1 <-> 1
            2 <-> 0, 3, 4
            3 <-> 2, 4
            4 <-> 2, 3, 6
            5 <-> 6
            6 <-> 4, 5)";
}
}  // namespace

BOOST_FIXTURE_TEST_CASE( problem2017_12_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 6, run( &solve_1, get_test_input() ) );
}

BOOST_FIXTURE_TEST_CASE( problem2017_12_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 2, run( &solve_2, get_test_input() ) );
}
