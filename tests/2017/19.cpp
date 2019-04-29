#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2017/problem_19.h"

using namespace AoC_2017::problem_19;

namespace
{

const auto* get_test_input()
{
  return "     |          \n"
         "     |  +--+    \n"
         "     A  |  C    \n"
         " F---|----E|--+ \n"
         "     |  |  |  D \n"
         "     +B-+  +--+ \n";
}
}  // namespace

BOOST_FIXTURE_TEST_CASE( problem2017_19_1, AocFixture )
{
  BOOST_CHECK_EQUAL( "ABCDEF", run( &solve_1, get_test_input() ) );
}

BOOST_FIXTURE_TEST_CASE( problem2017_19_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 38, run( &solve_2, get_test_input() ) );
}
