#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2017/problem_01.h"

using namespace AoC_2017::problem_01;

BOOST_FIXTURE_TEST_CASE( problem2017_01_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 3, run( &solve_1, "1122" ) );
  BOOST_CHECK_EQUAL( 4, run( &solve_1, "1111" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_1, "1234" ) );
  BOOST_CHECK_EQUAL( 9, run( &solve_1, "91212129" ) );
}

BOOST_FIXTURE_TEST_CASE( problem2017_01_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 6, run( &solve_1, "1212" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_1, "1221" ) );
  BOOST_CHECK_EQUAL( 4, run( &solve_1, "123425" ) );
  BOOST_CHECK_EQUAL( 12, run( &solve_1, "123123" ) );
  BOOST_CHECK_EQUAL( 4, run( &solve_1, "12131415" ) );
}
