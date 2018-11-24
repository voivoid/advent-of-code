#include "boost/test/unit_test.hpp"

#include "AoC/2015/problem_24.h"
#include "utils/aoc_fixture.h"

using namespace AoC_2015::problem_24;

namespace
{
auto get_input()
{
  return "1 2 3 4 5 7 8 9 10 11";
}
}  // namespace


BOOST_FIXTURE_TEST_CASE( problem2015_24_1, AocFixture )
{
  BOOST_CHECK_EQUAL( "99", run( &solve_1, get_input() ) );
}

// BOOST_FIXTURE_TEST_CASE( problem2015_24_2, AocFixture )
//{
//  BOOST_CHECK_EQUAL( "44", run( &solve_2, get_input() ) );
//}
