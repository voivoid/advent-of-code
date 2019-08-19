#pragma once

#include "AoC/utils/ranges/interleave.h"

#include "range/v3/iterator/operations.hpp"
#include "range/v3/view/chunk.hpp"

namespace AoC
{

// In:  Range<Range<T>>
// Out: Range<Range<T>>, transposing the rows and columns.
inline auto transpose()
{
  return ranges::make_pipeable( []( auto&& rngs ) {
    using Rngs = decltype( rngs );
    CPP_assert( ranges::forward_range<Rngs> );
    return std::forward<Rngs>( rngs ) | interleave() | ranges::views::chunk( static_cast<std::size_t>( ranges::distance( rngs ) ) );
  } );
}

}  // namespace AoC
