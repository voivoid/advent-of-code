#pragma once

#include "AoC/utils/geo.h"

#include "range/v3/algorithm/equal.hpp"
#include "range/v3/view/cartesian_product.hpp"
#include "range/v3/view/indices.hpp"
#include "range/v3/view/join.hpp"
#include "range/v3/view/move.hpp"
#include "range/v3/view/transform.hpp"

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

template <typename Impl, typename T>
class dd_array_base
{
protected:
  T& get( const size_t x, const size_t y )
  {
    return Impl::impl_get( *static_cast<Impl*>( this ), x, y );
  }

  const T& get( const size_t x, const size_t y ) const
  {
    return Impl::impl_get( *static_cast<const Impl*>( this ), x, y );
  }
};

template <typename Impl, typename T, size_t width, size_t height>
class dd_array_static_base : public dd_array_base<Impl, T>
{
protected:
  using Data = std::array<std::array<T, width>, height>;
  using Init = Data;

  size_t get_width() const
  {
    return width;
  }

  size_t get_height() const
  {
    return height;
  }
};

template <typename T, size_t width, size_t height>
class dd_array_static_stack_impl : public dd_array_static_base<dd_array_static_stack_impl<T, width, height>, T, width, height>
{
  using typename dd_array_static_base<dd_array_static_stack_impl<T, width, height>, T, width, height>::Data;

public:
  template <typename Impl>
  static auto& impl_get( Impl& impl, const size_t x, const size_t y )
  {
    assert( x < width );
    assert( y < height );
    return impl.data[ y ][ x ];
  }

protected:
  dd_array_static_stack_impl() : data{}
  {
  }

  dd_array_static_stack_impl( Data init ) : data( std::move( init ) )
  {
  }

private:
  Data data;
};

template <typename T, size_t width, size_t height>
class dd_array_static_heap_impl : public dd_array_static_base<dd_array_static_heap_impl<T, width, height>, T, width, height>
{
  using typename dd_array_static_base<dd_array_static_heap_impl<T, width, height>, T, width, height>::Data;

public:
  template <typename Impl>
  static auto& impl_get( Impl& impl, const size_t x, const size_t y )
  {
    assert( x < width );
    assert( y < height );
    return ( *impl.data )[ y ][ x ];
  }

protected:
  dd_array_static_heap_impl() : data( std::make_unique<Data>() )
  {
  }

  dd_array_static_heap_impl( Data init ) : data( std::make_unique<Data>( std::move( init ) ) )
  {
  }

private:
  std::unique_ptr<Data> data;
};

template <typename T>
class dd_array_dynamic_heap_impl : public dd_array_base<dd_array_dynamic_heap_impl<T>, T>
{
  using Base = dd_array_base<dd_array_dynamic_heap_impl<T>, T>;

public:
  template <typename Impl>
  static auto& impl_get( Impl& impl, const size_t x, const size_t y )
  {
    assert( x < impl.get_width() );
    assert( y < impl.get_height() );
    return impl.data[ impl.get_elem_index( x, y ) ];
  }

  void resize( size_t new_width, size_t new_height )
  {
    if ( new_width <= get_width() && new_height <= get_height() )
    {
      return;
    }

    new_width  = std::max( get_width(), new_width );
    new_height = std::max( get_height(), new_height );

    Data new_data( new_width * new_height );
    for ( size_t y = 0; y < get_height(); ++y )
    {
      for ( size_t x = 0; x < get_width(); ++x )
      {
        new_data[ get_elem_index( x, y, new_width ) ] = Base::get( x, y );
      }
    }
    data   = std::move( new_data );
    width  = new_width;
    height = new_height;
  }

protected:
  using Data = std::vector<T>;
  using Init = std::vector<std::vector<T>>;

  dd_array_dynamic_heap_impl() : height( 0 ), width( 0 )
  {
  }

  dd_array_dynamic_heap_impl( Init arr ) :
      height( arr.size() ),
      width( height ? arr.front().size() : size_t{ 0 } ),
      data( arr | ranges::view::join | ranges::view::move | ranges::to<std::vector<T>> )
  {
    assert( data.size() == height * width );
  }

  dd_array_dynamic_heap_impl( size_t w, size_t h ) : height( h ), width( w ), data( width * height )
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

private:
  size_t get_elem_index( const size_t x, const size_t y ) const
  {
    return get_elem_index( x, y, width );
  }

  size_t get_elem_index( const size_t x, const size_t y, const size_t w ) const
  {
    return y * w + x;
  }

private:
  size_t height;
  size_t width;
  Data data;
};

template <typename T, typename Impl>
class dd_array : public Impl
{
  template <typename Arr>
  struct Proxy;

public:
  dd_array()
  {
  }

  dd_array( typename Impl::Init arr ) : Impl( std::move( arr ) )
  {
  }

  template <typename... Args>
  dd_array( Args&&... args ) : Impl( std::forward<Args>( args )... )
  {
  }

  bool operator==( const dd_array<T, Impl>& arr ) const
  {
    return ranges::equal( *this, arr );
  }

  bool operator!=( const dd_array<T, Impl>& arr ) const
  {
    return !( *this == arr );
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
    return Impl::get_width();
  }

  size_t get_height() const
  {
    return Impl::get_height();
  }

  size_t size() const
  {
    return get_width() * get_height();
  }

  T* begin()
  {
    return &( Impl::get( 0, 0 ) );
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
    return &( Impl::get( 0, 0 ) );
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
      return arr.get( x, y );
    }

    const size_t x;
    Arr& arr;
  };
};

}  // namespace Details



template <typename T, size_t width, size_t height>
using dd_static_stack_array = Details::dd_array<T, Details::dd_array_static_stack_impl<T, width, height>>;

template <typename T, size_t width, size_t height>
using dd_static_heap_array = Details::dd_array<T, Details::dd_array_static_heap_impl<T, width, height>>;

template <typename T>
using dd_dynamic_heap_array = Details::dd_array<T, Details::dd_array_dynamic_heap_impl<T>>;

template <typename T, typename Impl>
std::optional<UPoint> dd_array_find_elem_indices( const Details::dd_array<T, Impl>& arr, const T& elem )
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

namespace Details
{
template <typename E, typename T>
auto column_impl( T& arr, const size_t n )
{
  assert( n < arr.get_width() );
  return ranges::view::indices( size_t{ 0 }, arr.get_height() ) |
         ranges::view::transform( [&arr, n]( const size_t i ) -> E& { return arr[ n ][ i ]; } );
}

template <typename E, typename T>
auto row_impl( T& arr, const size_t n )
{
  assert( n < arr.get_height() );
  return ranges::view::indices( size_t{ 0 }, arr.get_width() ) |
         ranges::view::transform( [&arr, n]( const size_t i ) -> E& { return arr[ i ][ n ]; } );
}

}  // namespace Details

template <typename T, typename Impl>
auto column( Details::dd_array<T, Impl>& arr, const size_t n )
{
  return Details::column_impl<T>( arr, n );
}

template <typename T, typename Impl>
auto column( const Details::dd_array<T, Impl>& arr, const size_t n )
{
  return Details::column_impl<const T>( arr, n );
}

template <typename Arr>
auto columns( Arr& arr )
{
  return ranges::view::indices( arr.get_width() ) | ranges::view::transform( [&arr]( const size_t n ) { return column( arr, n ); } );
}

template <typename T, typename Impl>
auto row( Details::dd_array<T, Impl>& arr, const size_t n )
{
  return Details::row_impl<T>( arr, n );
}

template <typename T, typename Impl>
auto row( const Details::dd_array<T, Impl>& arr, const size_t n )
{
  return Details::row_impl<const T>( arr, n );
}

template <typename Arr>
auto rows( Arr& arr )
{
  return ranges::view::indices( arr.get_height() ) | ranges::view::transform( [&arr]( const size_t n ) { return row( arr, n ); } );
}

}  // namespace AoC
