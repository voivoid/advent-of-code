#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2016/problem_07.h"

using namespace AoC_2016::problem_07;

BOOST_FIXTURE_TEST_CASE( problem2016_07_1, AocFixture )
{
  const auto input = R"(abba[mnop]qrst
                          abcd[bddb]xyyx
                          aaaa[qwer]tyui
                          ioxxoj[asdfgh]zxcvbn)";
  BOOST_CHECK_EQUAL( 2, run( &solve_1, input ) );
}

BOOST_FIXTURE_TEST_CASE( problem2016_07_2, AocFixture )
{
  const auto input = R"(aba[bab]xyz
                          xyx[xyx]xyx
                          aaa[kek]eke
                          zazbz[bzb]cdb)";
  BOOST_CHECK_EQUAL( 3, run( &solve_2, input ) );
}
