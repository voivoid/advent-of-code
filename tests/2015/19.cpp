#include "boost/test/unit_test.hpp"

#include "AoC/2015/problem_19.h"
#include "aoc_fixture.h"

namespace
{
auto get_test_input1()
{
  return R"(e => H
            e => O
            H => HO
            H => OH
            O => HH

            HOH)";
}

auto get_test_input2()
{
  return R"(e => H
            e => O
            H => HO
            H => OH
            O => HH

            HOHOHO)";
}
}  // namespace

using namespace AoC_2015::problem_19;

BOOST_FIXTURE_TEST_CASE( problem2015_19_1, AocFixture )
{
  BOOST_CHECK_EQUAL( 4, run( &solve_1, get_test_input1() ) );
  BOOST_CHECK_EQUAL( 7, run( &solve_1, get_test_input2() ) );
}

//BOOST_FIXTURE_TEST_CASE( problem2015_19_2, AocFixture )
//{
//  BOOST_CHECK_EQUAL( 3, run( &solve_2, get_test_input1() ) );
//  BOOST_CHECK_EQUAL( 6, run( &solve_2, get_test_input2() ) );
//}
