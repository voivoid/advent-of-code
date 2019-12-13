#include "boost/test/unit_test.hpp"

#include "AoC/utils/geo.h"
#include "AoC/utils/geo_io.h"

BOOST_AUTO_TEST_CASE( AoC_utils_geo_point )
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

  BOOST_CHECK_LT( p1, p2 );
  BOOST_CHECK_LE( p1, p2 );
  BOOST_CHECK_GT( p2, p1 );
  BOOST_CHECK_GE( p2, p1 );

  AoC::Point p4{ 4, 4 };
  BOOST_CHECK_EQUAL( p4, p4 );
  BOOST_CHECK_LE( p4, p4 );
  BOOST_CHECK_GE( p4, p4 );
}

BOOST_AUTO_TEST_CASE( AoC_utils_geo_segment )
{
  AoC::Segment s1{ { 1, 2 }, { 3, 4 } };
  BOOST_CHECK_EQUAL( 1, s1.p1.x );
  BOOST_CHECK_EQUAL( 2, s1.p1.y );
  BOOST_CHECK_EQUAL( 3, s1.p2.x );
  BOOST_CHECK_EQUAL( 4, s1.p2.y );
  BOOST_CHECK_EQUAL( s1, s1 );

  AoC::Segment s2{ { 0, 0 }, { 3, 3 } };
  BOOST_CHECK_NE( s1, s2 );

  AoC::Segment hor{ { 0, 0 }, { 3, 0 } };
  BOOST_CHECK( hor.is_horizontal() );
  BOOST_CHECK( !hor.is_vertical() );

  AoC::Segment ver{ { 0, 0 }, { 0, 3 } };
  BOOST_CHECK( !ver.is_horizontal() );
  BOOST_CHECK( ver.is_vertical() );

  AoC::Segment diag{ { 0, 0 }, { 3, 3 } };
  BOOST_CHECK( !diag.is_horizontal() );
  BOOST_CHECK( !diag.is_vertical() );
}

BOOST_AUTO_TEST_CASE( AoC_utils_geo_segment_perpendicular_intersections )
{
  static const auto check_intersection =
      []( const AoC::Segment& segment1, const AoC::Segment& segment2, const AoC::Point& expected_intersection ) {
        static const auto check = [&]( const AoC::Segment& s1, const AoC::Segment& s2 ) {
          const auto intersection = AoC::get_intersection_with_perpendicular( s1, s2 );
          if ( intersection )
          {
            return *intersection == expected_intersection;
          }

          return false;
        };


        const auto result = check( segment1, segment2 );
        assert( result == check( segment2, segment1 ) );
        return result;
      };

  {
    AoC::Segment s1{ { 0, 0 }, { 5, 0 } };
    AoC::Segment s2{ { 0, 0 }, { 0, 5 } };
    BOOST_CHECK( check_intersection( s1, s2, AoC::Point{ 0, 0 } ) );
  }

  {
    AoC::Segment s1{ { -5, 1 }, { 1, 1 } };
    AoC::Segment s2{ { 1, -5 }, { 1, 1 } };
    BOOST_CHECK( check_intersection( s1, s2, AoC::Point{ 1, 1 } ) );
  }

  {
    AoC::Segment s1{ { -2, 1 }, { 2, 1 } };
    AoC::Segment s2{ { 0, -2 }, { 0, 2 } };
    BOOST_CHECK( check_intersection( s1, s2, AoC::Point{ 0, 1 } ) );
  }

  {
    AoC::Segment s1{ { -2, -2 }, { -4, -2 } };
    AoC::Segment s2{ { -3, -3 }, { -3, -1 } };
    BOOST_CHECK( check_intersection( s1, s2, AoC::Point{ -3, -2 } ) );
  }

  {
    AoC::Segment s1{ { 0, 0 }, { 5, 0 } };
    AoC::Segment s2{ { -1, 0 }, { -1, 5 } };
    BOOST_CHECK( !check_intersection( s1, s2, {} ) );
  }
}

BOOST_AUTO_TEST_CASE( AoC_utils_geo_rect )
{
  AoC::Rectangle r1{ { 1, 2 }, { 3, 4 } };
  BOOST_CHECK_EQUAL( r1.left_top, ( AoC::Point{ 1, 2 } ) );
  BOOST_CHECK_EQUAL( r1.right_bottom, ( AoC::Point{ 3, 4 } ) );
  BOOST_CHECK_EQUAL( r1, r1 );

  BOOST_CHECK_EQUAL( 3, AoC::get_width( r1 ) );
  BOOST_CHECK_EQUAL( 3, AoC::get_height( r1 ) );

  AoC::Rectangle r2{ { 5, 6 }, { 7, 8 } };
  AoC::Rectangle r3 = r2;
  BOOST_CHECK_NE( r1, r2 );
  BOOST_CHECK_NE( r2, r1 );
  BOOST_CHECK_EQUAL( r2, r3 );
  BOOST_CHECK_EQUAL( r3, r2 );

  BOOST_CHECK( AoC::contains( r1, { 1, 2 } ) );
  BOOST_CHECK( AoC::contains( r1, { 2, 3 } ) );
  BOOST_CHECK( AoC::contains( r1, { 3, 4 } ) );
  BOOST_CHECK( !AoC::contains( r1, { 1, 1 } ) );
}
