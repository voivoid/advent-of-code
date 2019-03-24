#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2016/problem_11.h"

using namespace AoC_2016::problem_11;

BOOST_FIXTURE_TEST_CASE( problem2016_11_1, AocFixture )
{
  const auto input = R"(The first floor contains a hydrogen-compatible microchip and a lithium-compatible microchip.
                        The second floor contains a hydrogen generator.
                        The third floor contains a lithium generator.
                        The fourth floor contains nothing relevant.)";

  BOOST_CHECK_EQUAL( 11, run( &solve_1, input ) );
}
