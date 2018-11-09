#include "boost/test/unit_test.hpp"

#include "AoC/2015/problem_12.h"
#include "utils/aoc_fixture.h"

using namespace AoC_2015::problem_12;


BOOST_FIXTURE_TEST_CASE( problem2015_12_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 6, run( &solve_1, R"([1,2,3])" ) );
  BOOST_CHECK_EQUAL( 6, run( &solve_1, R"({"a":2,"b":4})" ) );
  BOOST_CHECK_EQUAL( 3, run( &solve_1, "[[[3]]]" ) );
  BOOST_CHECK_EQUAL( 3, run( &solve_1, R"({"a":{"b":4},"c":-1})" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_1, R"({"a":[-1,1]}))" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_1, R"([-1,{"a":1}])" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_1, "[]" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_1, "{}" ) );
}

BOOST_FIXTURE_TEST_CASE( problem2015_12_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 6, run( &solve_2, "[1,2,3]" ) );
  BOOST_CHECK_EQUAL( 4, run( &solve_2, R"([1,{"c":"red","b":2},3])" ) );
  BOOST_CHECK_EQUAL( 0, run( &solve_2, R"({"d":"red","e":[1,2,3,4],"f":5}" ) );
  BOOST_CHECK_EQUAL( 6, run( &solve_2, R"([1,"red",5])" ) );
}
