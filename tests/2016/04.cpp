#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2016/problem_04.h"

using namespace AoC_2016::problem_04;

BOOST_FIXTURE_TEST_CASE( problem2016_04_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 1514, run( &solve_1, R"(aaaaa-bbb-z-y-x-123[abxyz]
                                            a-b-c-d-e-f-g-h-987[abcde]
                                            not-a-real-room-404[oarel]
                                            totally-real-room-200[decoy])" ) );
}
