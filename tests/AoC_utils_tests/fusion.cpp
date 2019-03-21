#include "boost/test/unit_test.hpp"

#include "AoC/utils/fusion.h"
#include "boost/fusion/container/vector.hpp"

#include <string>

// TODO: parametrize tests with fusion container types

BOOST_AUTO_TEST_CASE( AoC_utils_fusion_empty_vector_to_tuple )
{
  boost::fusion::vector<> v;
  const auto tuple = AoC::fusion_to_std_tuple( v );
  BOOST_CHECK_EQUAL( 0, std::tuple_size<decltype( tuple )>::value );
}

BOOST_AUTO_TEST_CASE( AoC_utils_fusion_non_empty_vector_to_tuple )
{
  {
    boost::fusion::vector<int> v( 42 );
    const auto tuple = AoC::fusion_to_std_tuple( v );
    BOOST_REQUIRE_EQUAL( 1, std::tuple_size<decltype( tuple )>::value );
    BOOST_CHECK_EQUAL( 42, std::get<0>( tuple ) );
  }

  {
    boost::fusion::vector<int, char, std::string> v( 42, 'x', "hello" );
    const auto tuple = AoC::fusion_to_std_tuple( v );
    BOOST_REQUIRE_EQUAL( 3, std::tuple_size<decltype( tuple )>::value );
    BOOST_CHECK_EQUAL( 42, std::get<0>( tuple ) );
    BOOST_CHECK_EQUAL( 'x', std::get<1>( tuple ) );
    BOOST_CHECK_EQUAL( "hello", std::get<2>( tuple ) );
  }
}
