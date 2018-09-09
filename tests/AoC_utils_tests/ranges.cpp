#include <boost/test/unit_test.hpp>

#include <AoC_utils/ranges.h>

#include <range/v3/algorithm/equal.hpp>
#include <range/v3/to_container.hpp>
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
  std::vector<std::vector<int>> vecs = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
  const auto                    transposed =
      vecs | AoC::transpose() | ranges::view::transform( []( const auto& r ) { return r | ranges::to_vector; } ) | ranges::to_vector;
  const auto expected = std::vector<std::vector<int>>{ { 1, 4, 7 }, { 2, 5, 8 }, { 3, 6, 9 } };

  BOOST_CHECK( transposed == expected );

  const auto transposed2 =
      transposed | AoC::transpose() | ranges::view::transform( []( const auto& r ) { return r | ranges::to_vector; } ) | ranges::to_vector;

  BOOST_CHECK( transposed2 == vecs );
}
