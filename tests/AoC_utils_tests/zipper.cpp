#include "boost/mpl/vector.hpp"
#include "boost/test/unit_test.hpp"

#include "AoC/utils/zipper.h"

#include <list>
#include <vector>

namespace
{
using Fixtures = boost::mpl::vector<std::vector<int>, std::list<int>>;
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_zipper_creation, T, Fixtures, T )
{
  T container = { 0 };
  AoC::Zipper<T>{ container };
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_zipper_size, T, Fixtures, T )
{
  T container       = { 1, 2, 3 };
  const auto zipper = AoC::Zipper<T>( container );
  BOOST_CHECK_EQUAL( 3, zipper.size() );
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_zipper_current, T, Fixtures, T )
{
  T container             = { 1, 2, 3 };
  const auto const_zipper = AoC::Zipper<T>( container );
  auto zipper             = AoC::Zipper<T>( container );

  BOOST_CHECK_EQUAL( 1, const_zipper.current() );
  BOOST_CHECK_EQUAL( 1, const_zipper.current() );
  BOOST_CHECK_EQUAL( 1, zipper.current() );
  zipper.current() = 2;
  BOOST_CHECK_EQUAL( 2, zipper.current() );
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_zipper_single_step_navigation, T, Fixtures, T )
{
  T container = { 1, 2, 3 };
  auto zipper = AoC::Zipper<T>( container );
  BOOST_CHECK_EQUAL( 1, zipper.current() );

  zipper.next();
  BOOST_CHECK_EQUAL( 2, zipper.current() );
  zipper.next();
  BOOST_CHECK_EQUAL( 3, zipper.current() );
  zipper.next();
  BOOST_CHECK_EQUAL( 1, zipper.current() );

  zipper.prev();
  BOOST_CHECK_EQUAL( 3, zipper.current() );
  zipper.prev();
  BOOST_CHECK_EQUAL( 2, zipper.current() );
  zipper.prev();
  BOOST_CHECK_EQUAL( 1, zipper.current() );
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_zipper_multi_step_navigation, T, Fixtures, T )
{
  T container = { 1, 2, 3, 4, 5, 6 };
  auto zipper = AoC::Zipper<T>( container );
  BOOST_CHECK_EQUAL( 1, zipper.current() );

  zipper.next( 3 );
  BOOST_CHECK_EQUAL( 4, zipper.current() );
  zipper.next( 4 );
  BOOST_CHECK_EQUAL( 2, zipper.current() );
  zipper.next( 6 );
  BOOST_CHECK_EQUAL( 2, zipper.current() );
  zipper.next( 36 );
  BOOST_CHECK_EQUAL( 2, zipper.current() );

  zipper.prev( 36 );
  BOOST_CHECK_EQUAL( 2, zipper.current() );
  zipper.prev( 6 );
  BOOST_CHECK_EQUAL( 2, zipper.current() );
  zipper.prev( 4 );
  BOOST_CHECK_EQUAL( 4, zipper.current() );
  zipper.prev( 3 );
  BOOST_CHECK_EQUAL( 1, zipper.current() );
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_zipper_update, T, Fixtures, T )
{
  T container = { 1, 2, 3 };
  auto zipper = AoC::Zipper<T>( container );
  BOOST_CHECK_EQUAL( 1, zipper.current() );
  zipper.update( 111 );
  BOOST_CHECK_EQUAL( 111, zipper.current() );

  zipper.next();
  BOOST_CHECK_EQUAL( 2, zipper.current() );
  zipper.update( 222 );
  BOOST_CHECK_EQUAL( 222, zipper.current() );

  zipper.next();
  BOOST_CHECK_EQUAL( 3, zipper.current() );

  zipper.next();
  BOOST_CHECK_EQUAL( 111, zipper.current() );
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_zipper_append, T, Fixtures, T )
{
  T container = { 0 };
  auto zipper = AoC::Zipper<T>( container );

  zipper.append( 1 );
  BOOST_CHECK_EQUAL( 0, zipper.current() );

  zipper.append( 2 );
  BOOST_CHECK_EQUAL( 0, zipper.current() );

  zipper.append( 3 );
  BOOST_CHECK_EQUAL( 0, zipper.current() );

  zipper.next();
  BOOST_CHECK_EQUAL( 3, zipper.current() );
  zipper.next();
  BOOST_CHECK_EQUAL( 2, zipper.current() );
  zipper.next();
  BOOST_CHECK_EQUAL( 1, zipper.current() );
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( AoC_utils_zipper_remove, T, Fixtures, T )
{
  T container = { 1, 2, 3, 4, 5 };
  auto zipper = AoC::Zipper<T>( container );

  BOOST_CHECK_EQUAL( 1, zipper.current() );
  zipper.remove();
  BOOST_CHECK_EQUAL( 2, zipper.current() );

  zipper.next( 3 );
  zipper.remove();
  BOOST_CHECK_EQUAL( 2, zipper.current() );
}
