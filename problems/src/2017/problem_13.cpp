#include "AoC/2017/problem_13.h"

#include "AoC/problems_map.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/find_if.hpp"
#include "range/v3/iterator/operations.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/fusion/adapted/struct.hpp"

#include <functional>
#include <istream>
#include <optional>
#include <stdexcept>
#include <string>

namespace
{
struct Layer
{
  size_t depth;
  size_t range;
};

using Picosecond = size_t;

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( Layer, depth, range )

namespace
{

Layer parse_layer( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  const auto parser = AoC::x3_size_t_ > ':' > AoC::x3_size_t_;

  Layer layer;
  const bool is_parsed = AoC::x3_parse( line, parser, x3::space, layer );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input layer data" );
  }
  return layer;
}

bool is_caught( const Picosecond time_offset, const Layer& layer )
{
  assert( layer.range != 1 );

  const auto period = 2 * layer.range - 2;
  assert( period != 0 );

  const auto time = time_offset + layer.depth;
  return time % period == 0;
}

template <typename Range>
auto get_violations( const Picosecond time_offset, Range&& layers )
{
  auto layers_with_caught_packet = layers | ranges::views::filter( std::bind( &is_caught, time_offset, std::placeholders::_1 ) );
  return layers_with_caught_packet | ranges::views::transform( []( const Layer layer ) { return layer.depth * layer.range; } );
}

template <typename Range>
bool has_violations( const Picosecond time_offset, Range&& layers )
{
  auto severities = get_violations( time_offset, layers );
  return !ranges::empty( severities );
}

template <typename Range>
size_t calc_severity( const Picosecond time_offset, Range&& layers )
{
  auto severities = get_violations( time_offset, layers );
  return ranges::accumulate( severities, size_t{ 0 } );
}

auto get_layers( std::istream& input )
{
  return ranges::getlines( input ) | ranges::views::transform( &parse_layer );
}

}  // namespace

namespace AoC_2017
{

namespace problem_13
{

size_t solve_1( std::istream& input )
{
  auto layers = get_layers( input );
  return calc_severity( 0, layers );
}

size_t solve_2( std::istream& input )
{
  const auto layers       = get_layers( input ) | ranges::to_vector;
  const auto time_offsets = ranges::views::iota( Picosecond{ 0 } );

  const auto smallest_delay =
      *ranges::find_if( time_offsets, [&layers]( const Picosecond time_offset ) { return !has_violations( time_offset, layers ); } );

  return smallest_delay;
}

AOC_REGISTER_PROBLEM( 2017_13, solve_1, solve_2 );

}  // namespace problem_13
}  // namespace AoC_2017

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  {
    const auto layer = parse_layer( "2: 3" );
    assert( layer.depth == 2 );
    assert( layer.range == 3 );
  }

  assert( is_caught( 0, { 0, 3 } ) );
  assert( !is_caught( 0, { 1, 2 } ) );
  assert( !is_caught( 0, { 4, 4 } ) );
  assert( is_caught( 0, { 6, 4 } ) );

  assert( is_caught( 4, { 0, 3 } ) );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
