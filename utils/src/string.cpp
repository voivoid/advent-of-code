#include "AoC/utils/string.h"

#include "boost/numeric/conversion/cast.hpp"

#include "range/v3/algorithm/count_if.hpp"
#include "range/v3/algorithm/equal.hpp"
#include "range/v3/view/sliding.hpp"

namespace AoC
{

size_t count_substrings( const std::string_view input, const std::string_view substring )
{
  if ( substring.empty() )
  {
    return input.size() + 1;
  }

  auto substrings = input | ranges::views::sliding( substring.size() );
  return boost::numeric_cast<size_t>(
      ranges::count_if( substrings, [&substring]( const auto& s ) { return ranges::equal( s, substring ); } ) );
}

}  // namespace AoC
