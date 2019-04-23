#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2017/problem_18.h"

using namespace AoC_2017::problem_18;

BOOST_FIXTURE_TEST_CASE( problem2017_18_1, AocFixture )
{
  const char* const test_input = R"(set a 1
                                    add a 2
                                    mul a a
                                    mod a 5
                                    snd a
                                    set a 0
                                    rcv a
                                    jgz a -1
                                    set a 1
                                    jgz a -2)";
  BOOST_CHECK_EQUAL( "4", run( &solve_1, test_input ) );
}

BOOST_FIXTURE_TEST_CASE( problem2017_18_2, AocFixture )
{
  const char* const test_input = R"(snd 1
                                    snd 2
                                    snd p
                                    rcv a
                                    rcv b
                                    rcv c
                                    rcv d)";
  BOOST_CHECK_EQUAL( 3, run( &solve_2, test_input ) );
}
