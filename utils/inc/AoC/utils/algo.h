#pragma once

#include <utility>

#include <cstddef>

namespace AoC
{

template <typename T, typename F>
auto&& iterate_n( T&& val, F&& func, size_t n )
{
  while ( n > 0 )
  {
    val = func( std::forward<T>( val ) );
    --n;
  }

  return std::forward<T&&>( val );
}

template <typename F>
void repeat( const size_t n, const F& f )
{
  for ( size_t i = 0; i < n; ++i )
  {
    f();
  }
}

}  // namespace AoC
