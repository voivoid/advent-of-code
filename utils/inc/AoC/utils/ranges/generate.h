#pragma once

#include "range/v3/view/generate.hpp"
#include "range/v3/view/take_while.hpp"
#include "range/v3/view/transform.hpp"

#include <optional>

namespace AoC
{

template <typename T, typename F>
inline auto generate_range( T start_val, F func )
{
  return ranges::view::generate( [v = std::move( start_val ), f = std::move( func )]() mutable {
    v = f( v );
    return v;
  } );
}

template <typename T, typename F>
inline auto generate_while( T start_val, F func )
{
  auto r = ranges::view::generate( [v = std::optional<T>{ std::move( start_val ) }, f = std::move( func )]() mutable {
    assert( v.has_value() );
    v = f( *v );
    return v;
  } );

  return r | ranges::view::take_while( []( const auto& opt ) { return opt.has_value(); } ) |
         ranges::view::transform( []( const auto& opt ) { return opt.value(); } );
}
}  // namespace AoC
