#include "boost/test/unit_test.hpp"

#include "AoC/utils/ranges.h"

#include "range/v3/algorithm/equal.hpp"
#include "range/v3/iterator_range.hpp"
#include "range/v3/to_container.hpp"
#include "range/v3/view/all.hpp"
#include "range/v3/view/empty.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/single.hpp"
#include "range/v3/view/take.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/coroutine2/all.hpp"

#include <vector>

BOOST_AUTO_TEST_CASE( AoC_utils_ranges_interleave )
{
  const std::vector<std::vector<int>> vecs = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
  const auto interleaved                   = vecs | AoC::interleave() | ranges::to_vector;

  BOOST_CHECK( ranges::equal( interleaved, { 1, 4, 7, 2, 5, 8, 3, 6, 9 } ) );
}

BOOST_AUTO_TEST_CASE( AoC_utils_ranges_transpose )
{
  std::vector<std::vector<int>> vecs = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
  const auto transposed              = vecs | AoC::transpose() | AoC::to_2d_vector();
  const auto expected                = std::vector<std::vector<int>>{ { 1, 4, 7 }, { 2, 5, 8 }, { 3, 6, 9 } };

  BOOST_CHECK( transposed == expected );

  const auto transposed2 = transposed | AoC::transpose() | AoC::to_2d_vector();

  BOOST_CHECK( transposed2 == vecs );
}

BOOST_AUTO_TEST_CASE( AoC_utils_ranges_to_2d_vector )
{
  {
    const auto v        = ranges::view::single( ranges::view::empty<int>() ) | AoC::to_2d_vector();
    const auto expected = std::vector<std::vector<int>>{ std::vector<int>{} };
    BOOST_CHECK( v == expected );
  }

  {
    const auto v = ranges::view::ints( 0, 4 ) | ranges::view::transform( []( const int n ) { return ranges::view::ints( 0, n ); } ) |
                   AoC::to_2d_vector();
    const auto expected = std::vector<std::vector<int>>{ {}, { 0 }, { 0, 1 }, { 0, 1, 2 } };
    BOOST_CHECK( v == expected );
  }
}

BOOST_AUTO_TEST_CASE( AoC_utils_ranges_coro )
{
  using coro_t = boost::coroutines2::coroutine<int>;

  coro_t::pull_type fib_seq( [&]( coro_t::push_type& yield ) {
    int first = 1, second = 1;
    yield( first );
    yield( second );
    while ( true )
    {
      int third = first + second;
      first     = second;
      second    = third;
      yield( third );
    }
  } );

  const auto vec = fib_seq | ranges::view::take( 10 ) | ranges::to_vector;

  BOOST_CHECK_EQUAL( 10, vec.size() );

  const auto expected = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
  BOOST_CHECK_EQUAL_COLLECTIONS( vec.cbegin(), vec.cend(), expected.begin(), expected.end() );
}

BOOST_AUTO_TEST_CASE( AoC_utils_ranges_prepend )
{
  std::vector<int> v{ 1, 2, 3 };
  auto r = v | ranges::view::all | AoC::prepend( 0 );

  BOOST_CHECK( ranges::equal( r, { 0, 1, 2, 3 } ) );
}

BOOST_AUTO_TEST_CASE( AoC_utils_ranges_append )
{
  std::vector<int> v{ 1, 2, 3 };
  auto r = v | ranges::view::all | AoC::append( 4 );

  BOOST_CHECK( ranges::equal( r, { 1, 2, 3, 4 } ) );
}
