#pragma once

#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace AoC
{
template <typename Container>
class Zipper
{
public:
  Zipper( Container& c ) : container( c ), current_elem_iter( container.begin() )
  {
    ensure_is_not_empty();
  }

  Zipper( Container& c, typename Container::iterator i ) : container( c ), current_elem_iter( i )
  {
    ensure_is_not_empty();
  }

  Zipper( const Zipper& other ) : container( other.container ), current_elem_iter( other.current_elem_iter )
  {
  }

  Zipper& operator=( const Zipper& ) = delete;

  using Elem = typename Container::value_type;

  void next( size_t n = 1 )
  {
    if ( n >= size() )
    {
      n = n % size();
    }

    if ( n == 0 )
    {
      return;
    }

    assert( current_elem_iter != container.cend() );
    ++current_elem_iter;

    if ( current_elem_iter == container.cend() )
    {
      current_elem_iter = container.begin();
    }

    next( n - 1 );
  }

  void prev( size_t n = 1 )
  {
    if ( n >= size() )
    {
      n = n % size();
    }

    if ( n == 0 )
    {
      return;
    }

    assert( current_elem_iter != container.cend() );
    if ( current_elem_iter == container.cbegin() )
    {
      current_elem_iter = --container.end();
    }
    else
    {
      --current_elem_iter;
    }

    prev( n - 1 );
  }

  void move( int n )
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
    current_elem_iter = --container.insert( ++current_elem_iter, std::move( elem ) );
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
};
}  // namespace AoC
