#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

#include "AoC/2018/problem_04.h"

using namespace AoC_2018::problem_04;

namespace
{
auto get_test_input()
{
  return R"(
[1518-11-01 00:00] Guard #10 begins shift
[1518-11-01 00:05] falls asleep
[1518-11-01 00:25] wakes up
[1518-11-01 00:30] falls asleep
[1518-11-01 00:55] wakes up
[1518-11-01 23:58] Guard #99 begins shift
[1518-11-02 00:40] falls asleep
[1518-11-02 00:50] wakes up
[1518-11-03 00:05] Guard #10 begins shift
[1518-11-03 00:24] falls asleep
[1518-11-03 00:29] wakes up
[1518-11-04 00:02] Guard #99 begins shift
[1518-11-04 00:36] falls asleep
[1518-11-04 00:46] wakes up
[1518-11-05 00:03] Guard #99 begins shift
[1518-11-05 00:45] falls asleep
[1518-11-05 00:55] wakes up)";
}
}  // namespace

BOOST_FIXTURE_TEST_CASE( problem2018_04_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 240, run( &solve_1, get_test_input() ) );
}

BOOST_FIXTURE_TEST_CASE( problem2018_04_2, AocFixture )
{
  BOOST_CHECK_EQUAL( 4455, run( &solve_2, get_test_input() ) );
}
