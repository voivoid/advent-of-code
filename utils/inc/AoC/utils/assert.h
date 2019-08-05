#pragma once

#include <stdexcept>

#include "boost/preprocessor/stringize.hpp"

#define AOC_UNEXPECTED() throw std::runtime_error( __FILE__ ":" BOOST_PP_STRINGIZE( __LINE__ ) ": This should never be executed" )

#ifdef NDEBUG
#  define AOC_ASSERT_FALSE() AOC_UNEXPECTED();
#else
#  define AOC_ASSERT_FALSE()                                                                                                               \
    {                                                                                                                                      \
      assert( false );                                                                                                                     \
      AOC_UNEXPECTED();                                                                                                                    \
    }
#endif
