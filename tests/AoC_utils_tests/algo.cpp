#include "boost/test/unit_test.hpp"

#include "AoC/utils/algo.h"

BOOST_AUTO_TEST_CASE( AoC_utils_algo_iterate )
{
  auto inc = []( const auto x ) { return x + 1; };
  BOOST_CHECK_EQUAL( 1, AoC::iterate_n( 1, inc, 0 ) );
  BOOST_CHECK_EQUAL( 10, AoC::iterate_n( 0, inc, 10 ) );

  auto add_char = []( const auto s ) { return s + 'x'; };
  BOOST_CHECK_EQUAL( "abcxxxxx", AoC::iterate_n( std::string( "abc" ), add_char, 5 ) );

  int i        = 0;
  auto inc_ref = [&i]( int& ri ) {
    BOOST_CHECK_EQUAL( &i, &ri );
    ++ri;
    return ri;
  };
  int& ri = AoC::iterate_n( i, inc_ref, 5 );
  BOOST_CHECK_EQUAL( 5, ri );
  BOOST_CHECK_EQUAL( &i, &ri );
}

BOOST_AUTO_TEST_CASE( AoC_utils_hex_to_num )
{
  BOOST_CHECK_EQUAL( 10, AoC::hex_to_<size_t>( 'a' ) );
  BOOST_CHECK_EQUAL( 11, AoC::hex_to_<int>( 'b' ) );
}
