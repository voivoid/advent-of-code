#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2016/problem_02.h"

using namespace AoC_2016::problem_02;

namespace
{
auto get_test_input()
{
  return R"(ULL
            RRDDD
            LURDL
            UUUUD)";
}
}  // namespace

BOOST_FIXTURE_TEST_CASE( problem2016_02_1, AocFixture )
{
  BOOST_CHECK_EQUAL( "1985", run( &solve_1, get_test_input() ) );
}

BOOST_FIXTURE_TEST_CASE( problem2016_02_2, AocFixture )
{
  BOOST_CHECK_EQUAL( "5DB3", run( &solve_2, get_test_input() ) );
}
