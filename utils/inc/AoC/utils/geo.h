#pragma once

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

using Point  = GenericPoint<int>;
using UPoint = GenericPoint<size_t>;

using Coord  = GenericPoint<int>;
using UCoord = GenericPoint<size_t>;

using Rectangle  = GenericRectangle<int>;
using URectangle = GenericRectangle<size_t>;

}  // namespace AoC
