#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2018/problem_07.h"

using namespace AoC_2018::problem_07;

auto get_test_input()
{
  return R"(Step C must be finished before step A can begin.
            Step C must be finished before step F can begin.
            Step A must be finished before step B can begin.
            Step A must be finished before step D can begin.
            Step B must be finished before step E can begin.
            Step D must be finished before step E can begin.
            Step F must be finished before step E can begin.)";
}

BOOST_FIXTURE_TEST_CASE( problem2018_07_1, AocFixture )
{
  BOOST_CHECK_EQUAL( "CABDFE", run( &solve_1, get_test_input() ) );
}
