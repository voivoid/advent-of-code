#include "AoC/2017/problem_15.h"

#include "AoC/problems_map.h"
#include "AoC/utils/fusion.h"
#include "AoC/utils/ranges/generate.h"

#include "range/v3/algorithm/count.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/take_exactly.hpp"
#include "range/v3/view/zip_with.hpp"

#include "boost/fusion/container/vector.hpp"
#include "boost/numeric/conversion/cast.hpp"

#include "AoC/utils/parse.h"

namespace
{
using Value = size_t;

std::pair<Value, Value> parse_start_generator_values( std::istream& input )
{
  namespace x3 = boost::spirit::x3;

  const auto make_generator_parser = []( auto name_parser ) { return "Generator" > name_parser > "starts with" > AoC::x3_size_t_; };

  const auto parser = make_generator_parser( x3::lit( 'A' ) ) > make_generator_parser( x3::lit( 'B' ) );

  boost::fusion::vector<size_t, size_t> result;
  AoC::x3_parse( input, parser, x3::space, result );

  const auto [ a, b ] = AoC::fusion_to_std_tuple( result );
  return { a, b };
}

auto make_generator( const Value start_value, const Value multiplier )
{
  return AoC::generate_range( start_value, [multiplier]( const Value v ) { return ( v * multiplier ) % 2147483647; } );
}

template <typename Range1, typename Range2>
auto judge_sequences( Range1&& r1, Range2&& r2 )
{
  const auto judge_values = []( const Value v1, const Value v2 ) {
    const auto crop = []( const Value v ) { return v & 0xffff; };
    return crop( v1 ) == crop( v2 );
  };

  return ranges::view::zip_with( judge_values, r1, r2 );
}

template <Value mul, Value rem>
auto gen_sequence( const Value start_value, const size_t values_to_take )
{
  return make_generator( start_value, mul ) | ranges::view::filter( []( const Value v ) { return v % rem == 0; } ) |
         ranges::view::take_exactly( values_to_take );
}

template <Value a_rem, Value b_rem>
size_t solve( std::istream& input, const size_t values_to_take )
{
  const auto [ a_start_value, b_start_value ] = parse_start_generator_values( input );

  auto as = gen_sequence<16807, a_rem>( a_start_value, values_to_take );
  auto bs = gen_sequence<48271, b_rem>( b_start_value, values_to_take );

  auto matches = judge_sequences( as, bs );

  return boost::numeric_cast<size_t>( ranges::count( matches, true ) );
}

}  // namespace

namespace AoC_2017
{

namespace problem_15
{

size_t solve_1( std::istream& input )
{
  return solve<1, 1>( input, 40'000'000 );
}

size_t solve_2( std::istream& input )
{
  return solve<4, 8>( input, 5'000'000 );
}

AOC_REGISTER_PROBLEM( 2017_15, solve_1, solve_2 );

}  // namespace problem_15
}  // namespace AoC_2017

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

#  include "range/v3/range/conversion.hpp"

static void impl_tests()
{
  auto seq = gen_sequence<16807, 1>( 65, 5 ) | ranges::to_vector;
  assert( seq[ 0 ] == 1092455 );
  assert( seq[ 1 ] == 1181022009 );
  assert( seq[ 2 ] == 245556042 );
  assert( seq[ 3 ] == 1744312007 );
  assert( seq[ 4 ] == 1352636452 );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
