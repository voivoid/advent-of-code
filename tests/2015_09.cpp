#include <boost/test/unit_test.hpp>

#include "utils/aoc_fixture.h"
#include <AoC/2015/problem_09.h>

using namespace AoC_2015::problem_09;


namespace
{
std::string test_input()
{
  return "London to Dublin = 464\nLondon to Belfast = 518\nDublin to Belfast = 141";
}
}  // namespace

BOOST_FIXTURE_TEST_CASE( problem2015_09_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 605, run( &solve_1, test_input() ) );
}

BOOST_FIXTURE_TEST_CASE( problem2015_09_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 982, run( &solve_2, test_input() ) );
}
