#include "AoC/2015/problem_02.h"

#include <AoC/problems_map.h>

#include "rangev3.h"

#include <boost/algorithm/string.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <cassert>
#include <functional>

namespace
{
using Dim = int;
struct Dimensions
{
  Dim l;
  Dim w;
  Dim h;
};

}

BOOST_FUSION_ADAPT_STRUCT(Dimensions, l, w, h)

namespace
{


using CalcAreaFunc = std::function<int( Dimensions )>;

Dimensions parse_dimensions( const std::string& str )
{
    namespace x3 = boost::spirit::x3;
    const auto parser = x3::int_ >> 'x' >> x3::int_ >> 'x' >> x3::int_;

    Dimensions dims;
    x3::parse( str.cbegin(), str.cend(), parser, dims );
    return dims;
}

int calc_box_area( const Dimensions dims )
{
  const auto [ l, w, h ]   = dims;
  const auto sides         = { l * w, w * h, h * l };
  const auto area          = ranges::accumulate( sides | ranges::view::transform( []( int s ) { return s * 2; } ), 0 );
  const auto smallest_side = ranges::min( sides );
  return area + smallest_side;
}

std::pair<Dim, Dim> get_largest_dims( const Dim d1, const Dim d2, const Dim d3 )
{
  std::vector dims = { d1, d2, d3 };
  ranges::sort( dims );
  return { dims[ 0 ], dims[ 1 ] };
}

int calc_ribbon_area( const Dimensions dims )
{
  const auto [ l, w, h ] = dims;
  const auto [ d1, d2 ]  = get_largest_dims( l, w, h );

  const auto wrap = d1 * 2 + d2 * 2;
  const auto bow  = l * w * h;

  return wrap + bow;
}

using AreaFunc = int ( * )( Dimensions );
template <AreaFunc area_func>
int solve( ranges::istream_range<std::string> dimensions )
{
  auto areas = dimensions | ranges::view::transform( &parse_dimensions ) | ranges::view::transform( area_func );
  return ranges::accumulate( areas, 0 );
}

}  // namespace

namespace AoC_2015
{

namespace problem_02
{

int solve_1( std::istream& input )
{
  return solve<&calc_box_area>( input );
}

int solve_2( std::istream& input )
{
  return solve<&calc_ribbon_area>( input );
}

AOC_REGISTER_PROBLEM( 2015_02, solve_1, solve_2 );

}  // namespace problem_02

}  // namespace AoC_2015
