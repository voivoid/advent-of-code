#include "boost/test/unit_test.hpp"

#include "AoC_utils/geo.h"

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
std::ostream& operator<<( std::ostream& s, const AoC::GenericRectangle<T>& rect )
{
  s << "(" << rect.left_top << "," << rect.right_bottom << ")";
  return s;
}
}  // namespace AoC

BOOST_AUTO_TEST_CASE( aoc_utils_geo_point )
{
  AoC::Point p1{ 1, 2 };
  BOOST_CHECK_EQUAL( p1.x, 1 );
  BOOST_CHECK_EQUAL( p1.y, 2 );
  BOOST_CHECK_EQUAL( p1, p1 );

  AoC::Point p2{ 3, 4 };
  AoC::Point p3 = p2;
  BOOST_CHECK_NE( p1, p2 );
  BOOST_CHECK_NE( p2, p1 );
  BOOST_CHECK_EQUAL( p2, p3 );
  BOOST_CHECK_EQUAL( p3, p2 );
}

BOOST_AUTO_TEST_CASE( aoc_utils_geo_rect )
{
  AoC::Rectangle r1{ { 1, 2 }, { 3, 4 } };
  BOOST_CHECK_EQUAL( r1.left_top, ( AoC::Point{ 1, 2 } ) );
  BOOST_CHECK_EQUAL( r1.right_bottom, ( AoC::Point{ 3, 4 } ) );
  BOOST_CHECK_EQUAL( r1, r1 );

  AoC::Rectangle r2{ { 5, 6 }, { 7, 8 } };
  AoC::Rectangle r3 = r2;
  BOOST_CHECK_NE( r1, r2 );
  BOOST_CHECK_NE( r2, r1 );
  BOOST_CHECK_EQUAL( r2, r3 );
  BOOST_CHECK_EQUAL( r3, r2 );
}
