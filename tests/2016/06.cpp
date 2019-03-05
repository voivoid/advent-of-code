#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2016/problem_06.h"

namespace
{
auto get_test_input()
{
  return R"(eedadn
            drvtee
            eandsr
            raavrd
            atevrs
            tsrnev
            sdttsa
            rasrtv
            nssdts
            ntnada
            svetve
            tesnvt
            vntsnd
            vrdear
            dvrsen
            enarar)";
}
}  // namespace

using namespace AoC_2016::problem_06;

BOOST_FIXTURE_TEST_CASE( problem2016_06_1, AocFixture )
{
  BOOST_CHECK_EQUAL( "easter", run( &solve_1, get_test_input() ) );
}

BOOST_FIXTURE_TEST_CASE( problem2016_06_2, AocFixture )
{
  BOOST_CHECK_EQUAL( "advent", run( &solve_2, get_test_input() ) );
}
