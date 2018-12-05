#include <boost/test/unit_test.hpp>

#include <AoC/utils/anon_ret.h>

auto f1()
{
  int x = 1;
  int y = 2;
  int z = 3;
  AOC_RETURN_ANONYMOUS_STRUCT( x, y, z );
}

BOOST_AUTO_TEST_CASE( AoC_utils_anonymous_return )
{
  auto test1 = f1();
  BOOST_CHECK_EQUAL( test1.x, 1 );
  BOOST_CHECK_EQUAL( test1.y, 2 );
  BOOST_CHECK_EQUAL( test1.z, 3 );
}
