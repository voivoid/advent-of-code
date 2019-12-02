#pragma once

#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <utility>

#include "boost/numeric/conversion/cast.hpp"

namespace AoC
{
template <typename Container>
class Zipper
{
public:
  Zipper( Container& c ) : container( c ), current_elem_iter( container.begin() ), current_position( 0 )
  {
    ensure_is_not_empty();
  }

  Zipper( Container& c, typename Container::iterator i ) : container( c ), current_elem_iter( i )
  {
    current_position = static_cast<size_t>( std::distance( container.begin(), current_elem_iter ) );
    ensure_is_not_empty();
  }

  Zipper( const Zipper& other ) :
      container( other.container ), current_elem_iter( other.current_elem_iter ), current_position( other.current_position )
  {
  }

  Zipper& operator=( const Zipper& ) = delete;

  using Elem = typename Container::value_type;

  void set_next_position( const size_t next_pos )
  {
    const auto diff = boost::numeric_cast<ptrdiff_t>( next_pos ) - boost::numeric_cast<std::ptrdiff_t>( pos() );

    current_position = next_pos;
    std::advance( current_elem_iter, diff );
  }

  void next( size_t n = 1 )
  {
    set_next_position( ( pos() + n ) % size() );
  }

  void prev( size_t n = 1 )
  {
    set_next_position( ( pos() + size() - ( n % size() ) ) % size() );
  }

  void move( ptrdiff_t n )
  {
    if ( n >= 0 )
    {
      next( static_cast<size_t>( n ) );
    }
    else
    {
      prev( static_cast<size_t>( -n ) );
    }
  }

  const Elem& current() const
  {
    return get_current_elem();
  }

  Elem& current()
  {
    return get_current_elem();
  }

  void update( Elem elem )
  {
    get_current_elem() = std::move( elem );
  }

  void append( Elem elem )
  {
    assert( current_elem_iter != container.cend() );

    current_elem_iter = container.insert( ++current_elem_iter, std::move( elem ) );
    --current_elem_iter;
  }

  void remove()
  {
    assert( size() > 1 );
    current_elem_iter = container.erase( current_elem_iter );

    if ( current_elem_iter == container.end() )
    {
      current_elem_iter = container.begin();
    }
  }

  size_t size() const
  {
    return container.size();
  }

  size_t pos() const
  {
    return current_position;
  }

private:
  void ensure_is_not_empty() const
  {
    if ( empty() )
    {
      throw std::runtime_error( "Zipper should be constructed from non-empty container" );
    }
  }

  bool empty() const
  {
    return container.empty();
  }

  const Elem& get_current_elem() const
  {
    assert( current_elem_iter != container.end() );
    return *current_elem_iter;
  }

  Elem& get_current_elem()
  {
    assert( current_elem_iter != container.end() );
    return *current_elem_iter;
  }


private:
  Container& container;
  typename Container::iterator current_elem_iter;
  size_t current_position;
};
}  // namespace AoC
