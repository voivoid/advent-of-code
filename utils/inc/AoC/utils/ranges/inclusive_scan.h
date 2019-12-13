#pragma once

#include "range/v3/view/exclusive_scan.hpp"

#include "AoC/utils/ranges/append.h"

#include "boost/callable_traits/args.hpp"

#include <tuple>

namespace AoC
{
template <typename T, typename F>
inline auto inclusive_scan( T init, F func )
{
  return ranges::make_pipeable( [i = std::move( init ), f = std::move( func )]( auto&& rng ) {
    using Rng       = decltype( rng );
    using DummyType = std::tuple_element_t<1, boost::callable_traits::args_t<F>>;

    // appending dummy value since ranges v3 has no inclusive_scan yet :(
    return std::forward<Rng>( rng ) | AoC::append( DummyType{} ) | ranges::views::exclusive_scan( std::move( i ), std::move( f ) );
  } );
}

}  // namespace AoC
