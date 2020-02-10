#include "boost/test/unit_test.hpp"

#include "AoC/utils/ranges/2dvector.h"
#include "AoC/utils/ranges/append.h"
#include "AoC/utils/ranges/enumerate.h"
#include "AoC/utils/ranges/generate.h"
#include "AoC/utils/ranges/interleave.h"
#include "AoC/utils/ranges/prepend.h"
#include "AoC/utils/ranges/transpose.h"
#include "AoC/utils/ranges/unique_pair_combinations.h"

#include "range/v3/algorithm/equal.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/all.hpp"
#include "range/v3/view/common.hpp"
#include "range/v3/view/empty.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/single.hpp"
#include "range/v3/view/subrange.hpp"
#include "range/v3/view/take.hpp"
#include "range/v3/view/take_exactly.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/coroutine2/all.hpp"

#include <vector>

namespace
{
template <typename T>
void print_pair( std::ostream& os, const T& p )
{
  os << "<" << p.first << "," << p.second << ">";
}
}  // namespace

namespace boost
{
namespace test_tools
{
namespace tt_detail
{
template <typename T, typename U>
struct print_log_value<std::pair<T, U>>
{
  void operator()( std::ostream& os, std::pair<T, U> const& p )
  {
    print_pair( os, p );
  }
};

template <typename T, typename U>
struct print_log_value<ranges::common_pair<T, U>>
{
  void operator()( std::ostream& os, std::pair<T, U> const& p )
  {
    print_pair( os, p );
  }
};
}  // namespace tt_detail
}  // namespace test_tools
}  // namespace boost


BOOST_AUTO_TEST_CASE( AoC_utils_ranges_interleave )
{
  const std::vector<std::vector<int>> vecs = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
  const auto interleaved                   = vecs | AoC::interleave() | ranges::to_vector;

  const auto expected = { 1, 4, 7, 2, 5, 8, 3, 6, 9 };
  BOOST_CHECK_EQUAL_COLLECTIONS( interleaved.begin(), interleaved.end(), expected.begin(), expected.end() );
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
    const auto v        = ranges::views::single( ranges::views::empty<int> ) | AoC::to_2d_vector();
    const auto expected = std::vector<std::vector<int>>{ std::vector<int>{} };
    BOOST_CHECK( v == expected );
  }

  {
    const auto v = ranges::views::iota( 0, 4 ) | ranges::views::transform( []( const int n ) { return ranges::views::iota( 0, n ); } ) |
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

  const auto vec = fib_seq | ranges::views::take( 10 ) | ranges::to_vector;

  BOOST_CHECK_EQUAL( 10, vec.size() );

  const auto expected = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
  BOOST_CHECK_EQUAL_COLLECTIONS( vec.cbegin(), vec.cend(), expected.begin(), expected.end() );
}

BOOST_AUTO_TEST_CASE( AoC_utils_ranges_prepend )
{
  std::vector<int> v{ 1, 2, 3 };
  auto r = v | ranges::views::all | AoC::prepend( 0 );

  const auto expected = { 0, 1, 2, 3 };
  BOOST_CHECK_EQUAL_COLLECTIONS( r.begin(), r.end(), expected.begin(), expected.end() );
}

BOOST_AUTO_TEST_CASE( AoC_utils_ranges_append )
{
  std::vector<int> v{ 1, 2, 3 };
  auto r = v | ranges::views::all | AoC::append( 4 );

  const auto expected = { 1, 2, 3, 4 };
  BOOST_CHECK_EQUAL_COLLECTIONS( r.begin(), r.end(), expected.begin(), expected.end() );
}

BOOST_AUTO_TEST_CASE( AoC_utils_ranges_generate )
{
  auto range = AoC::generate_range( 0, []( const int n ) { return n + 1; } ) | ranges::views::take_exactly( 5 ) | ranges::views::common;
  const auto expected = { 1, 2, 3, 4, 5 };

  BOOST_CHECK_EQUAL_COLLECTIONS( expected.begin(), expected.end(), range.begin(), range.end() );
}

BOOST_AUTO_TEST_CASE( AoC_utils_ranges_generate_while )
{
  auto range = AoC::generate_while( 0,
                                    []( const int n ) {
                                      using O = std::optional<int>;
                                      return n < 5 ? O{ n + 1 } : O{};
                                    } ) |
               ranges::views::common;
  const auto expected = { 1, 2, 3, 4, 5 };

  BOOST_CHECK_EQUAL_COLLECTIONS( expected.begin(), expected.end(), range.begin(), range.end() );
}

BOOST_AUTO_TEST_CASE( AoC_utils_ranges_enumerate_with )
{
  std::string in = "0123";
  auto result    = in | AoC::enumerate_with_<size_t>() | ranges::views::common;

  std::initializer_list<ranges::common_pair<size_t, char>> expected = { { 0, '0' }, { 1, '1' }, { 2, '2' }, { 3, '3' } };
  BOOST_CHECK_EQUAL_COLLECTIONS( expected.begin(), expected.end(), result.begin(), result.end() );
}

#define DO_PAIRS_TEST( in, ex )                                                                                                            \
  {                                                                                                                                        \
    const std::initializer_list<int> input                    = in;                                                                        \
    auto combinations                                         = AoC::get_unique_pair_combinations<int>( input );                           \
    auto result                                               = combinations | ranges::views::all | ranges::views::common;                 \
    const std::initializer_list<std::pair<int, int>> expected = ex;                                                                        \
    BOOST_CHECK_EQUAL_COLLECTIONS( expected.begin(), expected.end(), result.begin(), result.end() );                                       \
  }

BOOST_AUTO_TEST_CASE( AoC_utils_ranges_unique_pairs )
{
  DO_PAIRS_TEST( {}, {} );
  DO_PAIRS_TEST( { 1 }, {} );

  {
    const std::initializer_list<int> in = { 1, 2 };
    const std::initializer_list<std::pair<int, int>> ex{ { 1, 2 } };
    DO_PAIRS_TEST( in, ex );
  }

  {
    const std::initializer_list<int> in = { 1, 2, 3 };
    const std::initializer_list<std::pair<int, int>> ex{ { 1, 2 }, { 1, 3 }, { 2, 3 } };
    DO_PAIRS_TEST( in, ex );
  }

  {
    const std::initializer_list<int> in = { 1, 2, 3, 4 };
    const std::initializer_list<std::pair<int, int>> ex{ { 1, 2 }, { 1, 3 }, { 1, 4 }, { 2, 3 }, { 2, 4 }, { 3, 4 } };
    DO_PAIRS_TEST( in, ex );
  }
}
