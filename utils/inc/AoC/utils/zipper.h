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
    if ( empty() )
    {
      throw std::runtime_error( "Zipper should be constructed from non-empty container" );
    }
  }

  Zipper( const Zipper& ) = delete;
  Zipper& operator=( const Zipper& ) = delete;

  using Elem = typename Container::value_type;

  void next( size_t n = 1 )
  {
    assert( current_elem_iter != container.cend() );
    ++current_elem_iter;

    if ( current_elem_iter == container.cend() )
    {
      current_elem_iter = container.begin();
    }

    if ( n > 1 )
    {
      next( n - 1 );
    }
  }

  void prev( size_t n = 1 )
  {
    assert( current_elem_iter != container.cend() );
    if ( current_elem_iter == container.cbegin() )
    {
      current_elem_iter = --container.end();
    }
    else
    {
      --current_elem_iter;
    }

    if ( n > 1 )
    {
      prev( n - 1 );
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