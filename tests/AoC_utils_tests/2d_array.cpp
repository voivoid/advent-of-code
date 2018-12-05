#include "boost/test/unit_test.hpp"

#include "AoC/utils/2d_array.h"

#include <algorithm>

struct dd_array_fixture
{
  dd_array_fixture() : arr{ { { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 } } } }
  {
  }

  AoC::dd_array<int, 4, 3> arr;
};

BOOST_FIXTURE_TEST_CASE( AoC_utils_dd_array_sizes, dd_array_fixture )
{
  BOOST_CHECK_EQUAL( 4, arr.get_width() );
  BOOST_CHECK_EQUAL( 3, arr.get_height() );
  BOOST_CHECK_EQUAL( 12, arr.size() );
}

BOOST_FIXTURE_TEST_CASE( AoC_utils_dd_array_reading, dd_array_fixture )
{
  BOOST_CHECK_EQUAL( 1, arr[ 0 ][ 0 ] );
  BOOST_CHECK_EQUAL( 5, arr[ 0 ][ 1 ] );
  BOOST_CHECK_EQUAL( 2, arr[ 1 ][ 0 ] );
  BOOST_CHECK_EQUAL( 11, arr[ 2 ][ 2 ] );
  BOOST_CHECK_EQUAL( 8, arr[ 3 ][ 1 ] );
  BOOST_CHECK_EQUAL( 12, arr[ 3 ][ 2 ] );

  const AoC::dd_array<int, 4, 3>& carr = arr;
  BOOST_CHECK_EQUAL( 6, carr[ 1 ][ 1 ] );
}

BOOST_FIXTURE_TEST_CASE( AoC_utils_dd_array_find_elem, dd_array_fixture )
{
  auto r = AoC::dd_array_find_elem_indices( arr, 7 );
  BOOST_REQUIRE( r );
  BOOST_CHECK_EQUAL( r->x, 2 );
  BOOST_CHECK_EQUAL( r->y, 1 );

  BOOST_CHECK( !AoC::dd_array_find_elem_indices( arr, 42 ) );
}

BOOST_FIXTURE_TEST_CASE( AoC_utils_dd_array_iterators, dd_array_fixture )
{
  const AoC::dd_array<int, 4, 3>& carr = arr;

  std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
  BOOST_CHECK_EQUAL_COLLECTIONS( arr.begin(), arr.end(), v.cbegin(), v.cend() );
  BOOST_CHECK_EQUAL_COLLECTIONS( carr.begin(), carr.end(), v.cbegin(), v.cend() );
  BOOST_CHECK_EQUAL_COLLECTIONS( arr.cbegin(), arr.cend(), v.cbegin(), v.cend() );

  std::copy( v.crbegin(), v.crend(), arr.begin() );
  BOOST_CHECK_EQUAL_COLLECTIONS( arr.cbegin(), arr.cend(), v.crbegin(), v.crend() );
}

BOOST_FIXTURE_TEST_CASE( AoC_utils_dd_array_writing, dd_array_fixture )
{
  arr[ 0 ][ 0 ] = 42;
  BOOST_CHECK_EQUAL( 42, arr[ 0 ][ 0 ] );
}
