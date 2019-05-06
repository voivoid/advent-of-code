#pragma once

#include "range/v3/range/conversion.hpp"

#include <vector>

namespace AoC
{

// In: Range<Range<T>>
// Out: std::vector<std::vector<T>>
inline auto to_2d_vector()
{
  return ranges::make_pipeable( []( auto&& rngs ) {
    using Rngs = decltype( rngs );
    using T    = ranges::range_value_type_t<ranges::range_value_type_t<Rngs>>;
    return std::vector<std::vector<T>>{ std::forward<Rngs>( rngs ) };
  } );
}

}  // namespace AoC
