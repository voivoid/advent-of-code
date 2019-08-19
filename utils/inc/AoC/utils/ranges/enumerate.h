#pragma once

#include "range/v3/view/iota.hpp"
#include "range/v3/view/zip.hpp"

namespace AoC
{



template <typename T>
inline auto enumerate_with_()
{
  return ranges::make_pipeable( []( auto&& rng ) {
    using Rng = decltype( rng );
    return ranges::views::zip( ranges::views::iota( T{ 0 } ), std::forward<Rng>( rng ) );
  } );
}
}  // namespace AoC
