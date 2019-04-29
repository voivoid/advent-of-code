#pragma once

#include <stdexcept>

#ifdef NDEBUG
#  define AOC_ASSERT_FALSE() throw std::runtime_error( "This should never be executed" )
#else
#  define AOC_ASSERT_FALSE() assert( false )
#endif
