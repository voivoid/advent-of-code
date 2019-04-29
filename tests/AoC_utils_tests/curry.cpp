#include "boost/test/unit_test.hpp"


#include "AoC/utils/curry.h"


namespace
{
int f1( int a )
{
  return a;
}

int f2( int a, int b )
{
  return a + b;
}

int f3( int a, int b, int c )
{
  return a + b + c;
}
}  // namespace

BOOST_AUTO_TEST_CASE( AoC_utils_curry_capture_1_arg )
{
  {
    const auto f = AOC_CURRY( f1, 42 );
    BOOST_CHECK_EQUAL( 42, f );
  }

  {
    const auto f = AOC_CURRY( f2, 42 );
    BOOST_CHECK_EQUAL( 43, f( 1 ) );
  }

  {
    const auto f = AOC_CURRY( f3, 42 );
    BOOST_CHECK_EQUAL( 45, f( 1, 2 ) );
  }
}
