#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2017/problem_07.h"

using namespace AoC_2017::problem_07;

namespace
{
auto get_input()
{
  return R"(pbga (66)
            xhth (57)
            ebii (61)
            havc (66)
            ktlj (57)
            fwft (72) -> ktlj, cntj, xhth
            qoyq (66)
            padx (45) -> pbga, havc, qoyq
            tknk (41) -> ugml, padx, fwft
            jptl (61)
            ugml (68) -> gyxo, ebii, jptl
            gyxo (61)
            cntj (57))";
}
}  // namespace

BOOST_FIXTURE_TEST_CASE( problem2017_07_1, AocFixture )
{
  BOOST_CHECK_EQUAL( "tknk", run( &solve_1, get_input() ) );
}

BOOST_FIXTURE_TEST_CASE( problem2017_07_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 60, run( &solve_2, get_input() ) );
}
