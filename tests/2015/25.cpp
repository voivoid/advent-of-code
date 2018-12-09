#include "boost/test/unit_test.hpp"

#include "AoC/2015/problem_25.h"
#include "aoc_fixture.h"

using namespace AoC_2015::problem_25;

BOOST_FIXTURE_TEST_CASE( problem2015_25_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 20151125,
                     run( &solve_1, "To continue, please consult the code grid in the manual.  Enter the code at row 1, column 1." ) );
  BOOST_CHECK_EQUAL( 31916031,
                     run( &solve_1, "To continue, please consult the code grid in the manual.  Enter the code at row 2, column 1." ) );
  BOOST_CHECK_EQUAL( 18749137,
                     run( &solve_1, "To continue, please consult the code grid in the manual.  Enter the code at row 1, column 2." ) );
  BOOST_CHECK_EQUAL( 27995004,
                     run( &solve_1, "To continue, please consult the code grid in the manual.  Enter the code at row 6, column 6." ) );
}
