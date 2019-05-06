#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2017/problem_20.h"

using namespace AoC_2017::problem_20;

namespace
{

}  // namespace

BOOST_FIXTURE_TEST_CASE( problem2017_20_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 0, run( &solve_1, R"(p=<3,0,0>, v=<2,0,0>, a=<-1,0,0>
                                          p=<4,0,0>, v=<0,0,0>, a=<-2,0,0>)" ) );

  BOOST_CHECK_EQUAL( 1, run( &solve_1, R"(p=<0,0,0>, v=< 0,0,0>, a=<1,0,0>
                                          p=<0,0,0>, v=<-1,0,0>, a=<1,0,0>)" ) );

  BOOST_CHECK_EQUAL( 1, run( &solve_1, R"(p=<0,0,0>, v=<-1,0,500>, a=<1,0,0>
                                          p=<0,0,0>, v=< 0,0,0>,   a=<1,0,0>
                                          p=<0,0,0>, v=< 1,0,0>,   a=<1,0,0>)" ) );

  BOOST_CHECK_EQUAL( 1, run( &solve_1, R"(p=<1,0,0>, v=<0,0,0>, a=< 2,0,0>
                                          p=<2,0,0>, v=<0,0,0>, a=<-2,0,0>)" ) );

  BOOST_CHECK_EQUAL( 0, run( &solve_1, R"(p=<-1,0,0>, v=<1,0,0>, a=<-1,0,0>
                                          p=< 1,0,0>, v=<1,0,0>, a=< 1,0,0>)" ) );
}

//BOOST_FIXTURE_TEST_CASE( problem2017_20_2, AocFixture )
//{
//  const auto* const input = R"(p=<-6,0,0>, v=< 3,0,0>, a=< 0,0,0>
//                                 p=<-4,0,0>, v=< 2,0,0>, a=< 0,0,0>    -6 -5 -4 -3 -2 -1  0  1  2  3
//                                 p=<-2,0,0>, v=< 1,0,0>, a=< 0,0,0>    (0)   (1)   (2)            (3)
//                                 p=< 3,0,0>, v=<-1,0,0>, a=< 0,0,0>)";

//  BOOST_CHECK_EQUAL( 1, run( &solve_2, input ) );
//}
