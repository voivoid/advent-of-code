#include "boost/test/unit_test.hpp"

#include "AoC/utils/string.h"

BOOST_AUTO_TEST_CASE( AoC_utils_string_count_substrings )
{
  BOOST_CHECK_EQUAL( 0, AoC::count_substrings( "", "abc" ) );
  BOOST_CHECK_EQUAL( 1, AoC::count_substrings( "abc", "abc" ) );

  BOOST_CHECK_EQUAL( 1, AoC::count_substrings( "abc", "a" ) );
  BOOST_CHECK_EQUAL( 2, AoC::count_substrings( "abca", "a" ) );
  BOOST_CHECK_EQUAL( 3, AoC::count_substrings( "aaa", "a" ) );

  BOOST_CHECK_EQUAL( 1, AoC::count_substrings( "xxyyzz", "xx" ) );
  BOOST_CHECK_EQUAL( 2, AoC::count_substrings( "xxyyxxzz", "xx" ) );
  BOOST_CHECK_EQUAL( 5, AoC::count_substrings( "xxxxxx", "xx" ) );

  BOOST_CHECK_EQUAL( 1, AoC::count_substrings( "", "" ) );
  BOOST_CHECK_EQUAL( 4, AoC::count_substrings( "abc", "" ) );
}
