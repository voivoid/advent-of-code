#pragma once

#include "range/v3/view/concat.hpp"
#include "range/v3/view/single.hpp"

namespace AoC
{


// In: Range<T>, T
// Out: Range<T> with T element appended
template <typename T>
inline auto append( T elem )
{
  return ranges::make_pipeable( [e = std::move( elem )]( auto&& rng ) {
    using Rng = decltype( rng );
    return ranges::views::concat( std::forward<Rng>( rng ), ranges::views::single( std::move( e ) ) );
  } );
}

}  // namespace AoC
