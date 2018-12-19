#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2018/problem_05.h"

using namespace AoC_2018::problem_05;

namespace
{
auto get_test_input()
{
  return "dabAcCaCBAcCcaDA";
}
}  // namespace

BOOST_FIXTURE_TEST_CASE( problem2018_05_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 10, run( &solve_1, get_test_input() ) );
  BOOST_CHECK_EQUAL( 6, run( &solve_1, "dbcCCBcCcD" ) );
  BOOST_CHECK_EQUAL( 8, run( &solve_1, "daAcCaCAcCcaDA" ) );
  BOOST_CHECK_EQUAL( 4, run( &solve_1, "dabAaBAaDA" ) );
  BOOST_CHECK_EQUAL( 6, run( &solve_1, "abAcCaCBAcCcaA" ) );
}

BOOST_FIXTURE_TEST_CASE( problem2018_05_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 4, run( &solve_2, get_test_input() ) );
}
