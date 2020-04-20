#pragma once

#include "range/v3/view/concat.hpp"
#include "range/v3/view/single.hpp"

namespace AoC
{

// In: Range<T>, T
// Out: Range<T> with T element prepended
template <typename T>
inline auto prepend( T elem )
{
  return ranges::make_pipeable( [ e = std::move( elem ) ]( auto&& rng ) {
    using Rng = decltype( rng );
    return ranges::views::concat( ranges::views::single( std::move( e ) ), std::forward<Rng>( rng ) );
  } );
}

}  // namespace AoC
