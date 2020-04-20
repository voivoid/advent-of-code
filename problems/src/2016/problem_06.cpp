#include "AoC/2016/problem_06.h"

#include "AoC/problems_map.h"
#include "AoC/utils/ranges/transpose.h"

#include "range/v3/action/sort.hpp"
#include "range/v3/iterator/operations.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/group_by.hpp"
#include "range/v3/view/istream.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/numeric/conversion/cast.hpp"

#include <istream>
#include <string>
#include <vector>

namespace
{
struct CharInfo
{
  size_t frequency;
  char chr;
};

template <template <typename> typename CharsFrequencySortPred, typename Range>
char get_most_common_char( Range chars_range )
{
  auto chars_vec = chars_range | ranges::to_vector;
  chars_vec |= ranges::actions::sort;
  auto char_infos = chars_vec | ranges::views::group_by( std::equal_to<char>{} ) | ranges::views::transform( []( const auto cs ) {
                      return CharInfo{ boost::numeric_cast<size_t>( ranges::distance( cs ) ), cs.front() };
                    } ) |
                    ranges::to_vector;

  char_infos |= ranges::actions::sort( CharsFrequencySortPred<size_t>{}, &CharInfo::frequency );
  return char_infos[ 0 ].chr;
}

template <template <typename> typename CharsFrequencySortPred>
std::string solve( std::istream& input )
{
  const auto lines = ranges::istream<std::string>( input ) | ranges::to_vector;
  const std::string result =
      lines | AoC::transpose() |
      ranges::views::transform( []( auto range ) { return get_most_common_char<CharsFrequencySortPred>( range ); } ) |
      ranges::to<std::string>();
  return result;
}
}  // namespace

namespace AoC_2016
{

namespace problem_06
{

std::string solve_1( std::istream& input )
{
  return solve<std::greater>( input );
}

std::string solve_2( std::istream& input )
{
  return solve<std::less>( input );
}

AOC_REGISTER_PROBLEM( 2016_06, solve_1, solve_2 );

}  // namespace problem_06

}  // namespace AoC_2016
