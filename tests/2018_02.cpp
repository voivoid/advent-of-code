#include "boost/test/unit_test.hpp"

#include "utils/aoc_fixture.h"

#include "AoC/2018/problem_02.h"

using namespace AoC_2018::problem_02;

BOOST_FIXTURE_TEST_CASE( problem2018_02_1, AocFixture )
{
  const auto input = R"(abcdef
                          bababc
                          abbcde
                          abcccd
                          aabcdd
                          abcdee
                          ababab)";

  BOOST_CHECK_EQUAL( 12, run( &solve_1, input ) );
}

BOOST_FIXTURE_TEST_CASE( problem2018_02_2, AocFixture )
{
  const auto input = R"(abcde
                          fghij
                          klmno
                          pqrst
                          fguij
                          axcye
                          wvxyz)";

  BOOST_CHECK_EQUAL( "fgij", run( &solve_2, input ) );
}
