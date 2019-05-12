#pragma once

#include <stdexcept>

#include "boost/preprocessor/stringize.hpp"

#ifdef NDEBUG
#  define AOC_ASSERT_FALSE() throw std::runtime_error( __FILE__ ":" BOOST_PP_STRINGIZE( __LINE__ ) ": This should never be executed" )
#else
#  define AOC_ASSERT_FALSE() assert( false )
#endif
