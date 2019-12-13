#pragma once

#include "boost/functional/hash.hpp"

#include <optional>
#include <tuple>

namespace AoC
{

template <typename T>
struct GenericPoint
{
  T x;
  T y;
};

template <typename T>
bool operator==( const GenericPoint<T> p1, const GenericPoint<T> p2 )
{
  return p1.x == p2.x && p1.y == p2.y;
}

template <typename T>
bool operator!=( const GenericPoint<T> p1, const GenericPoint<T> p2 )
{
  return !( p1 == p2 );
}

template <typename T>
bool operator<( const GenericPoint<T> p1, const GenericPoint<T> p2 )
{
  return std::tie( p1.x, p1.y ) < std::tie( p2.x, p2.y );
}

template <typename T>
bool operator>( const GenericPoint<T> p1, const GenericPoint<T> p2 )
{
  return !( p1 < p2 ) && p1 != p2;
}

template <typename T>
bool operator<=( const GenericPoint<T> p1, const GenericPoint<T> p2 )
{
  return p1 < p2 || p1 == p2;
}

template <typename T>
bool operator>=( const GenericPoint<T> p1, const GenericPoint<T> p2 )
{
  return p1 > p2 || p1 == p2;
}

template <typename T>
GenericPoint<T> operator+( const GenericPoint<T> p1, const GenericPoint<T> p2 )
{
  return { p1.x + p2.x, p1.y + p2.y };
}

template <typename T>
struct GenericSegment
{
  GenericPoint<T> p1;
  GenericPoint<T> p2;

  bool is_horizontal() const
  {
    return p1.y == p2.y;
  }

  bool is_vertical() const
  {
    return p1.x == p2.x;
  }
};

template <typename T>
bool operator==( const GenericSegment<T>& s1, const GenericSegment<T>& s2 )
{
  return s1.p1 == s2.p1 && s1.p2 == s2.p2;
}

template <typename T>
bool operator!=( const GenericSegment<T>& s1, const GenericSegment<T>& s2 )
{
  return !( s1 == s2 );
}

template <typename T>
struct GenericRectangle
{
  GenericPoint<T> left_top;
  GenericPoint<T> right_bottom;
};

template <typename T>
bool operator==( const GenericRectangle<T>& r1, const GenericRectangle<T>& r2 )
{
  return r1.left_top == r2.left_top && r1.right_bottom == r2.right_bottom;
}

template <typename T>
bool operator!=( const GenericRectangle<T>& r1, const GenericRectangle<T>& r2 )
{
  return !( r1 == r2 );
}

template <typename T>
bool contains( const GenericRectangle<T>& rect, const GenericPoint<T> p )
{
  return p >= rect.left_top && p <= rect.right_bottom;
}

template <typename T>
T get_width( const GenericRectangle<T>& rect )
{
  return rect.right_bottom.x - rect.left_top.x + 1;
}

template <typename T>
T get_height( const GenericRectangle<T>& rect )
{
  return rect.right_bottom.y - rect.left_top.y + 1;
}

template <typename T>
std::optional<GenericPoint<T>> get_intersection_with_perpendicular( const GenericSegment<T>& s1, const GenericSegment<T>& s2 )
{
  assert( ( s1.is_horizontal() && s2.is_vertical() ) || ( s1.is_vertical() && s2.is_horizontal() ) );

  const auto& hor = s1.is_horizontal() ? s1 : s2;
  const auto& ver = s2.is_vertical() ? s2 : s1;

  const auto x = ver.p1.x;
  const auto y = hor.p1.y;

  const auto right = std::max( hor.p1.x, hor.p2.x );
  const auto left  = std::min( hor.p1.x, hor.p2.x );
  const auto up    = std::max( ver.p1.y, ver.p2.y );
  const auto down  = std::min( ver.p1.y, ver.p2.y );

  if ( x >= left && x <= right && y >= down && y <= up )
  {
    return GenericPoint<T>{ x, y };
  }

  return {};
}

using Point  = GenericPoint<int>;
using UPoint = GenericPoint<size_t>;

using Coord  = GenericPoint<int>;
using UCoord = GenericPoint<size_t>;

using Segment  = GenericSegment<int>;
using USegment = GenericSegment<size_t>;

using Rectangle  = GenericRectangle<int>;
using URectangle = GenericRectangle<size_t>;

struct GeoHasher
{
  template <typename T>
  size_t operator()( GenericPoint<T> p ) const
  {
    std::size_t seed = 0;

    boost::hash_combine( seed, boost::hash_value( p.x ) );
    boost::hash_combine( seed, boost::hash_value( p.y ) );

    return seed;
  }

  template <typename T>
  size_t operator()( GenericSegment<T> s ) const
  {
    std::size_t seed = 0;

    boost::hash_combine( seed, boost::hash_value( s.p1 ) );
    boost::hash_combine( seed, boost::hash_value( s.p2 ) );

    return seed;
  }

  template <typename T>
  size_t operator()( const GenericRectangle<T>& p ) const
  {
    std::size_t seed = 0;

    boost::hash_combine( seed, ( *this )( p.left_top ) );
    boost::hash_combine( seed, ( *this )( p.right_bottom ) );

    return seed;
  }
};

}  // namespace AoC
