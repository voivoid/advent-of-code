#pragma once

#include "AoC/utils/geo.h"

#include "range/v3/view/cartesian_product.hpp"
#include "range/v3/view/indices.hpp"
#include "range/v3/view/join.hpp"
#include "range/v3/view/move.hpp"

#include <algorithm>
#include <array>
#include <optional>
#include <type_traits>
#include <vector>

#include <cassert>

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
  using Init  = Array;

  dd_array_stack_impl() : data{}
  {
  }
  dd_array_stack_impl( Array init ) : data( std::move( init ) )
  {
  }

  size_t get_width() const
  {
    return width;
  }

  size_t get_height() const
  {
    return height;
  }

  T& get( const size_t x, const size_t y )
  {
    assert( x <= width );
    assert( y <= height );
    return data[ y ][ x ];
  }

  const T& get( const size_t x, const size_t y ) const
  {
    assert( x <= width );
    assert( y <= height );
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
  using Init  = Array;

  dd_array_heap_impl() : data( std::make_unique<Array>() )
  {
  }
  dd_array_heap_impl( Array init ) : data( std::make_unique<Array>( std::move( init ) ) )
  {
  }

  size_t get_width() const
  {
    return width;
  }

  size_t get_height() const
  {
    return height;
  }

  T& get( const size_t x, const size_t y )
  {
    assert( x <= width );
    assert( y <= height );
    return ( *data )[ y ][ x ];
  }

  const T& get( const size_t x, const size_t y ) const
  {
    assert( x <= width );
    assert( y <= height );
    return ( *data )[ y ][ x ];
  }

private:
  std::unique_ptr<Array> data;
};

template <typename T>
class dd_array_dynamic_impl
{
public:
  using Array = std::vector<T>;
  using Init  = std::vector<std::vector<T>>;

  dd_array_dynamic_impl()
  {
  }
  dd_array_dynamic_impl( Init arr ) :
      height( arr.size() ),
      width( height ? arr.front().size() : size_t{ 0 } ),
      data( arr | ranges::view::join | ranges::view::move )
  {
  }

  dd_array_dynamic_impl( size_t w, size_t h ) : height( h ), width( w )
  {
    data.resize( width * height );
  }

  size_t get_width() const
  {
    return width;
  }

  size_t get_height() const
  {
    return height;
  }

  T& get( const size_t x, const size_t y )
  {
    assert( x <= get_width() );
    assert( y <= get_height() );
    return data[ get_elem_index( x, y ) ];
  }

  const T& get( const size_t x, const size_t y ) const
  {
    assert( x <= width );
    assert( y <= height );
    return data[ get_elem_index( x, y ) ];
  }

private:
  size_t get_elem_index( size_t x, size_t y ) const
  {
    return y * width + x;
  }

private:
  size_t height;
  size_t width;
  Array data;
};

enum class dd_array_alloc_type
{
  stack,
  heap
};

template <typename T, size_t width, size_t height, dd_array_alloc_type alloc>
struct static_array
{
};

template <typename T>
struct dynamic_array
{
};

template <typename T>
struct select_impl;

template <typename T, size_t width, size_t height, dd_array_alloc_type alloc>
struct select_impl<static_array<T, width, height, alloc>>
{
  using Impl = std::conditional_t<alloc == dd_array_alloc_type::stack,
                                  Details::dd_array_stack_impl<T, width, height>,
                                  Details::dd_array_heap_impl<T, width, height>>;
};

template <typename T>
struct select_impl<dynamic_array<T>>
{
  using Impl = Details::dd_array_dynamic_impl<T>;
};

template <typename T, typename Array>
class dd_array
{
  template <typename Arr>
  struct Proxy;

  using Impl = typename Details::select_impl<Array>::Impl;

public:
  dd_array()
  {
  }

  dd_array( typename Impl::Init arr ) : impl( std::move( arr ) )
  {
  }

  template <typename... Args>
  dd_array( Args&&... args ) : impl( std::forward<Args>( args )... )
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
    return impl.get_width();
  }

  size_t get_height() const
  {
    return impl.get_height();
  }

  size_t size() const
  {
    return get_width() * get_height();
  }

  T* begin()
  {
    return &( impl.get( 0, 0 ) );
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
    return &( impl.get( 0, 0 ) );
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
      return arr.impl.get( x, y );
    }

    const size_t x;
    Arr& arr;
  };

private:
  Impl impl;
};

}  // namespace Details



template <typename T, size_t width, size_t height>
using dd_static_stack_array = Details::dd_array<T, Details::static_array<T, width, height, Details::dd_array_alloc_type::stack>>;

template <typename T, size_t width, size_t height>
using dd_static_heap_array = Details::dd_array<T, Details::static_array<T, width, height, Details::dd_array_alloc_type::heap>>;

template <typename T>
using dd_dynamic_heap_array = Details::dd_array<T, Details::dynamic_array<T>>;

template <typename Array, typename T>
std::optional<UPoint> dd_array_find_elem_indices( const Array& arr, const T& elem )
{
  const auto xs = ranges::view::indices( size_t{ 0 }, arr.get_width() );
  const auto ys = ranges::view::indices( size_t{ 0 }, arr.get_height() );

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
