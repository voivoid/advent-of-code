#pragma once

#include "boost/hof/lift.hpp"
#include "boost/hof/partial.hpp"

#define AOC_CURRY( F ) boost::hof::partial( BOOST_HOF_LIFT( F ) )
