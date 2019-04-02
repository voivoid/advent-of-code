#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2017/problem_08.h"

using namespace AoC_2017::problem_08;

namespace
{
auto get_input()
{
  return R"(b inc 5 if a > 1
            a inc 1 if b < 5
            c dec -10 if a >= 1
            c inc -20 if c == 10)";
}
}  // namespace

BOOST_FIXTURE_TEST_CASE( problem2017_08_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 1, run( &solve_1, get_input() ) );
}

BOOST_FIXTURE_TEST_CASE( problem2017_08_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 10, run( &solve_2, get_input() ) );
}
