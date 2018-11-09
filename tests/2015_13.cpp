#include "boost/test/unit_test.hpp"

#include "AoC/2015/problem_13.h"
#include "utils/aoc_fixture.h"

using namespace AoC_2015::problem_13;


BOOST_FIXTURE_TEST_CASE( problem2015_13_1, AocFixture )
{
  const auto test_input = R"(Alice would gain 54 happiness units by sitting next to Bob.
                               Alice would lose 79 happiness units by sitting next to Carol.
                               Alice would lose 2 happiness units by sitting next to David.
                               Bob would gain 83 happiness units by sitting next to Alice.
                               Bob would lose 7 happiness units by sitting next to Carol.
                               Bob would lose 63 happiness units by sitting next to David.
                               Carol would lose 62 happiness units by sitting next to Alice.
                               Carol would gain 60 happiness units by sitting next to Bob.
                               Carol would gain 55 happiness units by sitting next to David.
                               David would gain 46 happiness units by sitting next to Alice.
                               David would lose 7 happiness units by sitting next to Bob.
                               David would gain 41 happiness units by sitting next to Carol.)";

  BOOST_CHECK_EQUAL( 330, run( &solve_1, test_input ) );
}
