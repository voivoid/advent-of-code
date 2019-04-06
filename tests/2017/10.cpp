#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2017/problem_10.h"

using namespace AoC_2017::problem_10;

BOOST_FIXTURE_TEST_CASE( problem2017_10_2, AocFixture )
{
  BOOST_CHECK_EQUAL( "a2582a3a0e66e6e86e3812dcb672a272", run( &solve_2, "" ) );
  BOOST_CHECK_EQUAL( "33efeb34ea91902bb2f59c9920caa6cd", run( &solve_2, "AoC 2017" ) );
  BOOST_CHECK_EQUAL( "3efbe78a8d82f29979031a4aa0b16a9d", run( &solve_2, "1,2,3" ) );
  BOOST_CHECK_EQUAL( "63960835bcdc130f0b66d7ff4f6a5a8e", run( &solve_2, "1,2,4" ) );
}
