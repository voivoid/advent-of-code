#include "boost/test/unit_test.hpp"

#include "boost/mpl/placeholders.hpp"
#include "boost/mpl/transform.hpp"
#include "boost/mpl/vector.hpp"
#include <boost/mpl/joint_view.hpp>

#include "AoC/utils/2d_array.h"

#include "range/v3/algorithm/copy.hpp"
#include "range/v3/algorithm/equal.hpp"
#include "range/v3/view/reverse.hpp"

namespace
{

template <typename Array>
struct dd_array_fixture
{
  dd_array_fixture() : array{ { { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 } } } }
  {
  }

  Array array;
};

template <typename T>
struct AddConstnessToFixture;

template <typename T>
struct AddConstnessToFixture<dd_array_fixture<T>>
{
  using type = dd_array_fixture<const T>;
};

using NonConstFixtures = boost::mpl::vector<dd_array_fixture<AoC::dd_static_stack_array<int, 4, 3>>,
                                            dd_array_fixture<AoC::dd_static_heap_array<int, 4, 3>>,
                                            dd_array_fixture<AoC::dd_dynamic_heap_array<int>>>;

using ConstFixtures = boost::mpl::transform<NonConstFixtures, AddConstnessToFixture<boost::mpl::_1>>::type;

using AllFixtures = boost::mpl::joint_view<NonConstFixtures, ConstFixtures>;

}  // namespace

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_dd_array_sizes, T, AllFixtures, T )
{
  auto& arr = T::array;

  BOOST_CHECK_EQUAL( 4, arr.get_width() );
  BOOST_CHECK_EQUAL( 3, arr.get_height() );
  BOOST_CHECK_EQUAL( 12, arr.size() );
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_dd_array_reading, T, AllFixtures, T )
{
  auto& arr = T::array;

  BOOST_CHECK_EQUAL( 1, arr[ 0 ][ 0 ] );
  BOOST_CHECK_EQUAL( 5, arr[ 0 ][ 1 ] );
  BOOST_CHECK_EQUAL( 2, arr[ 1 ][ 0 ] );
  BOOST_CHECK_EQUAL( 11, arr[ 2 ][ 2 ] );
  BOOST_CHECK_EQUAL( 8, arr[ 3 ][ 1 ] );
  BOOST_CHECK_EQUAL( 12, arr[ 3 ][ 2 ] );
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_dd_array_find_elem, T, AllFixtures, T )
{
  auto& arr = T::array;

  auto r = AoC::dd_array_find_elem_indices( arr, 7 );
  BOOST_REQUIRE( r );
  BOOST_CHECK_EQUAL( r->x, 2 );
  BOOST_CHECK_EQUAL( r->y, 1 );

  BOOST_CHECK( !AoC::dd_array_find_elem_indices( arr, 42 ) );
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_dd_array_iterators_reading, T, AllFixtures, T )
{
  auto& arr = T::array;

  const std::initializer_list<int> v = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
  BOOST_CHECK_EQUAL_COLLECTIONS( arr.begin(), arr.end(), v.begin(), v.end() );
  BOOST_CHECK_EQUAL_COLLECTIONS( arr.cbegin(), arr.cend(), v.begin(), v.end() );
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_dd_array_iterators_writing, T, NonConstFixtures, T )
{
  auto& arr = T::array;

  const std::initializer_list<int> v = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
  ranges::copy( v | ranges::view::reverse, arr.begin() );
  BOOST_CHECK_EQUAL_COLLECTIONS( arr.cbegin(), arr.cend(), std::crbegin( v ), std::crend( v ) );
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_dd_array_writing, T, NonConstFixtures, T )
{
  auto& arr = T::array;

  arr[ 0 ][ 0 ] = 42;
  arr[ 1 ][ 2 ] = 12;
  arr[ 2 ][ 1 ] = 21;
  BOOST_CHECK_EQUAL( 42, arr[ 0 ][ 0 ] );
  BOOST_CHECK_EQUAL( 12, arr[ 1 ][ 2 ] );
  BOOST_CHECK_EQUAL( 21, arr[ 2 ][ 1 ] );

  const std::initializer_list<int> expected = { 42, 2, 3, 4, 5, 6, 21, 8, 9, 12, 11, 12 };
  BOOST_CHECK_EQUAL_COLLECTIONS( arr.cbegin(), arr.cend(), expected.begin(), expected.end() );
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_dd_array_column_reading, T, AllFixtures, T )
{
  auto& arr = T::array;

  auto col = AoC::column( arr, 0 );
  BOOST_CHECK_EQUAL( 3, col.size() );
  const std::initializer_list<int> expected = { 1, 5, 9 };
  BOOST_CHECK_EQUAL_COLLECTIONS( col.begin(), col.end(), expected.begin(), expected.end() );
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_dd_array_row_reading, T, AllFixtures, T )
{
  auto& arr = T::array;

  auto row = AoC::row( arr, 0 );
  BOOST_CHECK_EQUAL( 4, row.size() );
  const std::initializer_list<int> expected = { 1, 2, 3, 4 };
  BOOST_CHECK_EQUAL_COLLECTIONS( row.begin(), row.end(), expected.begin(), expected.end() );
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_dd_array_column_writing, T, NonConstFixtures, T )
{
  auto& arr = T::array;

  auto col = AoC::column( arr, 1 );
  BOOST_CHECK_EQUAL( 3, col.size() );
  col[ 0 ] = 10;
  col[ 1 ] = 20;
  col[ 2 ] = 30;

  const std::initializer_list<int> expected = { 1, 10, 3, 4, 5, 20, 7, 8, 9, 30, 11, 12 };
  BOOST_CHECK_EQUAL_COLLECTIONS( arr.cbegin(), arr.cend(), expected.begin(), expected.end() );
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_dd_array_row_writing, T, NonConstFixtures, T )
{
  auto& arr = T::array;

  auto row = AoC::row( arr, 1 );
  BOOST_CHECK_EQUAL( 4, row.size() );
  row[ 0 ] = 10;
  row[ 1 ] = 20;
  row[ 2 ] = 30;
  row[ 3 ] = 40;

  const std::initializer_list<int> expected = { 1, 2, 3, 4, 10, 20, 30, 40, 9, 10, 11, 12 };
  BOOST_CHECK_EQUAL_COLLECTIONS( arr.cbegin(), arr.cend(), expected.begin(), expected.end() );
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_dd_array_rows_cols, T, AllFixtures, T )
{
  auto& arr = T::array;

  auto rows = AoC::rows( arr );
  BOOST_CHECK_EQUAL( 3, rows.size() );

  auto columns = AoC::columns( arr );
  BOOST_CHECK_EQUAL( 4, columns.size() );
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_dd_array_equal_op, T, AllFixtures, T )
{
  auto& arr = T::array;

  BOOST_CHECK( arr == arr );
  BOOST_CHECK( !( arr != arr ) );
}

BOOST_AUTO_TEST_CASE( AoC_utils_dd_dynamic_array )
{
  AoC::dd_dynamic_heap_array<int> arr( size_t{ 10 }, size_t{ 20 } );
  BOOST_CHECK_EQUAL( 200, arr.size() );
  BOOST_CHECK_EQUAL( 10, arr.get_width() );
  BOOST_CHECK_EQUAL( 20, arr.get_height() );
}

BOOST_AUTO_TEST_CASE( AoC_utils_dd_dynamic_array_resize )
{
  AoC::dd_dynamic_heap_array<int> arr( size_t{ 10 }, size_t{ 20 } );

  arr[ 9 ][ 19 ] = 42;
  arr.resize( 20, 30 );
  BOOST_CHECK_EQUAL( 20, arr.get_width() );
  BOOST_CHECK_EQUAL( 30, arr.get_height() );
  BOOST_CHECK_EQUAL( 42, arr[ 9 ][ 19 ] );


  BOOST_CHECK_EQUAL( 0, arr[ 19 ][ 29 ] );
  arr[ 19 ][ 29 ] = 111;
  BOOST_CHECK_EQUAL( 111, arr[ 19 ][ 29 ] );
}
