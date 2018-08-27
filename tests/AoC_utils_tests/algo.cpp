#include <boost/test/unit_test.hpp>

#include <AoC_utils/algo.h>

BOOST_AUTO_TEST_CASE( aoc_utils_algo_iterate )
{
    auto inc = [](const auto x) { return x + 1; };

    BOOST_CHECK_EQUAL( 1, AoC::iterate_n( 1, inc, 0 ) );
    BOOST_CHECK_EQUAL( 10, AoC::iterate_n( 0, inc, 10 ) );

    auto add_char = [](const auto s) { return s + 'x'; };

    BOOST_CHECK_EQUAL( "abcxxxxx", AoC::iterate_n( std::string("abc"), add_char, 5 ) );
}
