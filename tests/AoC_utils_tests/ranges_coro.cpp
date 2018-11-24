#include "boost/test/unit_test.hpp"

#include "range/v3/iterator_range.hpp"
#include "range/v3/to_container.hpp"
#include "range/v3/view/take.hpp"

#include "boost/coroutine2/all.hpp"

BOOST_AUTO_TEST_CASE( aoc_utils_ranges_coro )
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
