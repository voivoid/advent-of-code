#include "boost/test/unit_test.hpp"


#include "AoC/2015/problem_23.h"
#include "utils/aoc_fixture.h"

using namespace AoC_2015::problem_23;

namespace
{
auto get_input()
{
  return R"(inc b
            jio b, +2
            tpl b
            inc b)";
}
}  // namespace


BOOST_FIXTURE_TEST_CASE( problem2015_23_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 2, run( &solve_1, get_input() ) );
}
