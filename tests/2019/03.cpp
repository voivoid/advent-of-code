#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2019/problem_03.h"

using namespace AoC_2019::problem_03;

BOOST_FIXTURE_TEST_CASE( problem2019_03_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 6, run( &solve_1, R"(R8,U5,L5,D3
                                          U7,R6,D4,L4)" ) );
  BOOST_CHECK_EQUAL( 159, run( &solve_1, R"(R75,D30,R83,U83,L12,D49,R71,U7,L72
                                            U62,R66,U55,R34,D71,R55,D58,R83)" ) );
  BOOST_CHECK_EQUAL( 135, run( &solve_1, R"(R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51
                                            U98,R91,D20,R16,D67,R40,U7,R15,U6,R7)" ) );
}

BOOST_FIXTURE_TEST_CASE( problem2019_03_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 30, run( &solve_2, R"(R8,U5,L5,D3
                                           U7,R6,D4,L4)" ) );
  BOOST_CHECK_EQUAL( 610, run( &solve_2, R"(R75,D30,R83,U83,L12,D49,R71,U7,L72
                                            U62,R66,U55,R34,D71,R55,D58,R83)" ) );
  BOOST_CHECK_EQUAL( 410, run( &solve_2, R"(R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51
                                            U98,R91,D20,R16,D67,R40,U7,R15,U6,R7)" ) );
}
