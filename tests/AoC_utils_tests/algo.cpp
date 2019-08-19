#include "boost/test/unit_test.hpp"

#include "range/v3/view/transform.hpp"

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

BOOST_AUTO_TEST_CASE( AoC_utils_algo_hex_to_num )
{
  const std::string digits = "0123456789abcdef";
  const auto nums          = digits | ranges::views::transform( &AoC::hex_to_<size_t> );
  const auto expected      = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

  BOOST_CHECK_EQUAL_COLLECTIONS( expected.begin(), expected.end(), nums.begin(), nums.end() );
  BOOST_CHECK_EQUAL( 10, AoC::hex_to_<int>( 'a' ) );
}
