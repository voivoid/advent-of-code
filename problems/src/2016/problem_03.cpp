#include "AoC/2016/problem_03.h"

#include "AoC/problems_map.h"
#include "AoC/utils/parse.h"
#include "AoC/utils/ranges.h"

#include "range/v3/algorithm/count_if.hpp"
#include "range/v3/getlines.hpp"
#include "range/v3/to_container.hpp"
#include "range/v3/view/chunk.hpp"
#include "range/v3/view/join.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/fusion/container/vector.hpp"
#include "boost/spirit/home/x3.hpp"

#include <algorithm>
#include <array>

namespace
{

using Side     = int;
using Triangle = std::array<Side, 3>;

Triangle parse_triangle( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  const auto parser = x3::int_ > x3::int_ > x3::int_;

  boost::fusion::vector<Side, Side, Side> result;
  AoC::x3_parse( line.cbegin(), line.cend(), parser, x3::space, result );

  return { boost::fusion::at_c<0>( result ), boost::fusion::at_c<1>( result ), boost::fusion::at_c<2>( result ) };
}

bool are_valid_triangle_side( const Side a, const Side b, const Side c )
{
  const auto min = std::min( a, std::min( b, c ) );
  const auto max = std::max( a, std::max( b, c ) );
  const auto mid = a + b + c - min - max;

  return min + mid > max;
}

bool is_valid_triangle( const Triangle t )
{
  return are_valid_triangle_side( t[ 0 ], t[ 1 ], t[ 2 ] );
}

}  // namespace

namespace AoC_2016
{

namespace problem_03
{

int solve_1( std::istream& input )
{
  auto triangles = ranges::getlines( input ) | ranges::view::transform( &parse_triangle );
  return static_cast<int>( ranges::count_if( triangles, &is_valid_triangle ) );
}

int solve_2( std::istream& input )
{
  auto triangles = ranges::getlines( input ) | ranges::view::transform( &parse_triangle ) | ranges::view::chunk( 3 ) |
                   ranges::view::transform( ranges::to_vector ) |
                   ranges::view::transform( []( const auto chunk ) { return chunk | AoC::transpose(); } ) | ranges::view::join;

  const auto valid_triangles_num = ranges::count_if( triangles, []( const auto t ) {
    auto iter    = t.begin();
    const Side a = *iter++;
    const Side b = *iter++;
    const Side c = *iter++;
    return are_valid_triangle_side( a, b, c );
  } );

  return static_cast<int>( valid_triangles_num );
}

AOC_REGISTER_PROBLEM( 2016_03, solve_1, solve_2 );

}  // namespace problem_03

}  // namespace AoC_2016