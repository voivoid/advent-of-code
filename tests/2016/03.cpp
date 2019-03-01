#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2016/problem_03.h"

using namespace AoC_2016::problem_03;

BOOST_FIXTURE_TEST_CASE( problem2016_03_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 1, run( &solve_1, "5 10 25\n101 102 103" ) );
}

BOOST_FIXTURE_TEST_CASE( problem2016_03_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 6, run( &solve_2, R"(101 301 501
                                          102 302 502
                                          103 303 503
                                          201 401 601
                                          202 402 602
                                          203 403 603)" ) );
}
