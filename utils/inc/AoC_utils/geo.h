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
bool operator==( const GenericPoint<T>& p1, const GenericPoint<T>& p2 )
{
  return p1.x == p2.x && p1.y == p2.y;
}

template <typename T>
bool operator!=( const GenericPoint<T>& p1, const GenericPoint<T>& p2 )
{
  return !( p1 == p2 );
}

template <typename T>
bool operator<( const GenericPoint<T>& p1, const GenericPoint<T>& p2 )
{
  return std::tie( p1.x, p1.y ) < std::tie( p2.x, p2.y );
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

using Point     = GenericPoint<int>;
using Coord     = GenericPoint<int>;
using Rectangle = GenericRectangle<int>;

}  // namespace AoC
