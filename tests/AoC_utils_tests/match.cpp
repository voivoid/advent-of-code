#include <boost/test/unit_test.hpp>

#include <AoC_utils/match.h>

#include <numeric>
#include <vector>

BOOST_AUTO_TEST_CASE( aoc_utils_match )
{
  const auto match = []( const std::vector<int>& v ) {
    return AoC::match_container( v,
                                 []( auto, auto, auto, auto ) { return 99999; },
                                 []( int a, int b, int c ) { return a * 100 + b * 10 + c; },
                                 []( int a, int b ) { return a * 10 + b; },
                                 []( int a ) { return a; },
                                 []( const std::vector<int>& vec ) { return std::accumulate( vec.cbegin(), vec.cend(), 0 ); } );
  };

  BOOST_CHECK_EQUAL( 1, match( std::vector<int>{ 1 } ) );
  BOOST_CHECK_EQUAL( 12, match( std::vector<int>{ 1, 2 } ) );
  BOOST_CHECK_EQUAL( 123, match( std::vector<int>{ 1, 2, 3 } ) );
  BOOST_CHECK_EQUAL( 0, match( std::vector<int>{} ) );
  BOOST_CHECK_EQUAL( 15, match( std::vector<int>{ 1, 2, 3, 4, 5 } ) );
}
