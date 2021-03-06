#include "AoC/2015/problem_02.h"

#include "AoC/problems_map.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/min.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/istream.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/algorithm/string.hpp"
#include "boost/fusion/include/adapt_struct.hpp"


#include <stdexcept>
#include <string>
#include <utility>

#include <cassert>

namespace
{
using Dim = size_t;
struct Dimensions
{
  Dim l;
  Dim w;
  Dim h;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( Dimensions, l, w, h )

namespace
{

Dimensions parse_dimensions( const std::string& str )
{
  namespace x3      = boost::spirit::x3;
  const auto parser = AoC::x3_size_t_ > 'x' > AoC::x3_size_t_ > 'x' > AoC::x3_size_t_;

  Dimensions dims;
  const bool is_parsed = AoC::x3_parse( str, parser, x3::space, dims );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input dimensions data" );
  }

  return dims;
}

Dim box_area( const Dimensions dims )
{
  const auto [ l, w, h ]   = dims;
  const auto sides         = { l * w, w * h, h * l };
  const auto doubled_sides = sides | ranges::views::transform( []( const Dim s ) { return s * 2; } );
  const auto area          = ranges::accumulate( doubled_sides, Dim{ 0 } );
  const auto smallest_side = ranges::min( sides );
  return area + smallest_side;
}

std::pair<Dim, Dim> get_smallest_dims( const Dim d1, const Dim d2, const Dim d3 )
{
  const auto max = std::max( std::max( d1, d2 ), d3 );
  const auto min = std::min( std::min( d1, d2 ), d3 );
  const auto mid = d1 + d2 + d3 - max - min;
  return { min, mid };
}

Dim ribbon_area( const Dimensions dims )
{
  const auto [ l, w, h ] = dims;
  const auto [ d1, d2 ]  = get_smallest_dims( l, w, h );

  const auto wrap = d1 * 2 + d2 * 2;
  const auto bow  = l * w * h;

  return wrap + bow;
}

using AreaFunc = Dim ( * )( Dimensions );
template <AreaFunc calc_area, typename Range>
size_t calc( Range&& dimensions )
{
  auto areas = dimensions | ranges::views::transform( &parse_dimensions ) | ranges::views::transform( calc_area );
  return ranges::accumulate( areas, Dim{ 0 } );
}

}  // namespace

namespace AoC_2015
{

namespace problem_02
{

size_t solve_1( std::istream& input )
{
  return calc<&box_area>( ranges::istream<std::string>( input ) );
}

size_t solve_2( std::istream& input )
{
  return calc<&ribbon_area>( ranges::istream<std::string>( input ) );
}

AOC_REGISTER_PROBLEM( 2015_02, solve_1, solve_2 );

}  // namespace problem_02

}  // namespace AoC_2015
