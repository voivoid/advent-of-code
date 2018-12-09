#include "boost/test/unit_test.hpp"

#include "aoc_fixture.h"

namespace AoC
{
void test_impl();
}

BOOST_FIXTURE_TEST_CASE( impl_test, AocFixture )
{
  AoC::test_impl();
}
