#pragma once

#include <stdexcept>
#include <utility>

#include <cassert>
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

template <typename T>
T hex_to_( const char hex )
{
  switch ( hex )
  {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'a': return 10;
    case 'b': return 11;
    case 'c': return 12;
    case 'd': return 13;
    case 'e': return 14;
    case 'f': return 15;
  }

  assert( false );
  throw std::runtime_error( "This should never be executed" );
}

}  // namespace AoC
