#pragma once

#include "AoC/utils/geo.h"

#include "range/v3/view/cartesian_product.hpp"
#include "range/v3/view/indices.hpp"

#include <algorithm>
#include <array>
#include <optional>
#include <type_traits>
#include <vector>

namespace AoC
{

namespace Details
{
template <typename T, size_t width, size_t height>
using Array = std::array<std::array<T, width>, height>;

template <typename T, size_t width, size_t height>
class dd_array_stack_impl
{
public:
  using Array = Details::Array<T, width, height>;

  dd_array_stack_impl() : data{}
  {
  }
  dd_array_stack_impl( Array init ) : data( std::move( init ) )
  {
  }

  T& get( const size_t x, const size_t y )
  {
    return data[ y ][ x ];
  }

  const T& get( const size_t x, const size_t y ) const
  {
    return data[ y ][ x ];
  }

private:
  Array data;
};

template <typename T, size_t width, size_t height>
class dd_array_heap_impl
{
public:
  using Array = Details::Array<T, width, height>;

  dd_array_heap_impl() : data( std::make_unique<Array>() )
  {
  }
  dd_array_heap_impl( Array init ) : data( std::make_unique<Array>( std::move( init ) ) )
  {
  }

  T& get( const size_t x, const size_t y )
  {
    return ( *data )[ y ][ x ];
  }

  const T& get( const size_t x, const size_t y ) const
  {
    return ( *data )[ y ][ x ];
  }

private:
  std::unique_ptr<Array> data;
};
}  // namespace Details

enum class dd_array_alloc_type
{
  stack,
  heap
};

template <typename T, size_t width, size_t height, dd_array_alloc_type alloc = dd_array_alloc_type::stack>
class dd_array
{
  template <typename Arr>
  struct Proxy;

  using Impl = std::conditional_t<alloc == dd_array_alloc_type::stack,
                                  Details::dd_array_stack_impl<T, width, height>,
                                  Details::dd_array_heap_impl<T, width, height>>;

public:
  dd_array()
  {
  }

  dd_array( typename Impl::Array arr ) : impl( std::move( arr ) )
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
    return &( impl.template get( 0, 0 ) );
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
    return &( impl.template get( 0, 0 ) );
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
      return arr.impl.get( x, y );
    }

    const size_t x;
    Arr& arr;
  };

private:
  Impl impl;
};

template <typename T, size_t width, size_t height, dd_array_alloc_type alloc>
std::optional<UPoint> dd_array_find_elem_indices( const dd_array<T, width, height, alloc>& arr, const T& elem )
{
  const auto xs = ranges::view::indices( size_t{ 0 }, width );
  const auto ys = ranges::view::indices( size_t{ 0 }, height );

  for ( auto [ y, x ] : ranges::view::cartesian_product( ys, xs ) )
  {
    if ( arr[ x ][ y ] == elem )
    {
      return { { x, y } };
    }
  }

  return {};
}

}  // namespace AoC
