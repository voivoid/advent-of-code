#pragma once

#include "AoC/utils/geo.h"

#include "range/v3/view/indices.hpp"

#include <array>
#include <optional>

namespace AoC
{

template <typename T, size_t width, size_t height>
class dd_array
{
  template <typename Arr>
  struct Proxy;

    public:
  using ArrayImpl = std::array<std::array<T, width>, height>;

  dd_array() : data( ArrayImpl{} )
  {
  }

  dd_array( ArrayImpl arr ) : data( std::move( arr ) )
  {
  }

  auto operator[]( const size_t x )
  {
    return Proxy<decltype( *this )>{ x, *this };
  }

  auto operator[]( const size_t x ) const
  {
    return Proxy<decltype( *this )>{ x, *this };
  }

  size_t get_width() const
  {
    return width;
  }

  size_t get_height() const
  {
    return height;
  }

  size_t size() const
  {
    return width * height;
  }

  T* begin()
  {
    return &( data[ 0 ][ 0 ] );
  }

  const T* begin() const
  {
    return cbegin();
  }

  T* end()
  {
    return begin() + size();
  }

  const T* end() const
  {
    return cend();
  }

  const T* cbegin() const
  {
    return &( data[ 0 ][ 0 ] );
  }

  const T* cend() const
  {
    return cbegin() + size();
  }

    private:
  template <typename Arr>
  struct Proxy
  {

    auto& operator[]( const size_t y )
    {
      assert( x <= width );
      assert( y <= height );
      return ( arr.data )[ y ][ x ];
    }

    size_t x;
    Arr& arr;
  };

    private:
  ArrayImpl data;
};

template <typename T, size_t width, size_t height>
std::optional<UPoint> dd_array_find_elem_indices( const dd_array<T, width, height>& arr, const T& elem )
{
  for ( size_t x : ranges::view::indices( size_t( 0 ), width ) )
  {
    for ( size_t y : ranges::view::indices( size_t( 0 ), height ) )
    {
      if ( arr[ x ][ y ] == elem )
      {
        return { { x, y } };
      }
    }
  }

  return {};
}

}  // namespace AoC
