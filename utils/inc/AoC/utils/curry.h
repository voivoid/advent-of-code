#pragma once

#include "boost/hof/lift.hpp"
#include "boost/hof/partial.hpp"

#define AOC_CURRY( F, A ) boost::hof::partial( BOOST_HOF_LIFT( F ) )( A )
