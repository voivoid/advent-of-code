#pragma once

#include <ostream>

namespace AoC
{
template <typename T>
std::ostream& operator<<( std::ostream& s, const AoC::GenericPoint<T>& point )
{
  s << "(" << point.x << "," << point.y << ")";
  return s;
}

template <typename T>
std::ostream& operator<<( std::ostream& s, const AoC::GenericSegment<T>& segment )
{
  s << "(" << segment.p1 << "," << segment.p2 << ")";
  return s;
}

template <typename T>
std::ostream& operator<<( std::ostream& s, const AoC::GenericRectangle<T>& rect )
{
  s << "(" << rect.left_top << "," << rect.right_bottom << ")";
  return s;
}
}  // namespace AoC

