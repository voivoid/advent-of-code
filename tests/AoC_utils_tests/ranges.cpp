#include <boost/test/unit_test.hpp>

#include <AoC_utils/ranges.h>

#include <range/v3/algorithm/equal.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/empty.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/single.hpp>
#include <range/v3/view/transform.hpp>

#include <vector>

BOOST_AUTO_TEST_CASE( aoc_utils_ranges_interleave )
{
  const std::vector<std::vector<int>> vecs        = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
  const auto                          interleaved = vecs | AoC::interleave() | ranges::to_vector;

  BOOST_CHECK( ranges::equal( interleaved, std::vector<int>{ 1, 4, 7, 2, 5, 8, 3, 6, 9 } ) );
}

BOOST_AUTO_TEST_CASE( aoc_utils_ranges_transpose )
{
  std::vector<std::vector<int>> vecs       = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
  const auto                    transposed = vecs | AoC::transpose() | AoC::to_2d_vector();
  const auto                    expected   = std::vector<std::vector<int>>{ { 1, 4, 7 }, { 2, 5, 8 }, { 3, 6, 9 } };

  BOOST_CHECK( transposed == expected );

  const auto transposed2 = transposed | AoC::transpose() | AoC::to_2d_vector();

  BOOST_CHECK( transposed2 == vecs );
}

BOOST_AUTO_TEST_CASE( aoc_utils_ranges_to_2d_vector )
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
