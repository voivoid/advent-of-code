#include "AoC/2018/problem_11.h"

#include "AoC/problems_map.h"
#include "AoC/utils/2d_array.h"
#include "AoC/utils/geo.h"

#include "range/v3/algorithm/max.hpp"
#include "range/v3/view/cartesian_product.hpp"
#include "range/v3/view/indices.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/numeric/conversion/cast.hpp"

namespace
{
using Power = int;
using Coord = AoC::UCoord;

constexpr size_t grid_size = 300;
using Grid                 = AoC::dd_static_heap_array<Power, grid_size, grid_size>;

size_t calc_hundreds_digit( const size_t p )
{
  return ( p / 100 ) % 10;
}

Power calc_power_level( const size_t x, const size_t y, const size_t sn )
{
  const auto rack_id = x + 10;
  return boost::numeric_cast<Power>( calc_hundreds_digit( ( rack_id * y + sn ) * rack_id ) ) - 5;
}

auto get_grid_coords( const size_t from, const size_t to )
{
  const auto grid_indices = ranges::view::closed_indices( from, to );
  return ranges::view::cartesian_product( grid_indices, grid_indices ) | ranges::view::transform( []( const auto c ) {
           const auto [ y, x ] = c;
           return Coord{ x, y };
         } );
}

Power get_grid_power( const Grid& grid, const size_t x, const size_t y )
{
  if ( x == 0 || y == 0 )
  {
    return 0;
  }

  assert( x <= grid_size );
  assert( y <= grid_size );

  return grid[ x - 1 ][ y - 1 ];
}

void set_grid_power( Grid& grid, const size_t x, const size_t y, const Power power )
{
  assert( x <= grid_size );
  assert( y <= grid_size );

  grid[ x - 1 ][ y - 1 ] = power;
}

Power calc_partial_sum_power( const Grid& grid, const size_t x, const size_t y )
{
  return get_grid_power( grid, x, y ) + get_grid_power( grid, x - 1, y ) + get_grid_power( grid, x, y - 1 ) -
         get_grid_power( grid, x - 1, y - 1 );
}

Power get_total_power( const Grid& grid, const Coord left_top, const size_t square_side )
{
  const auto [ left, top ] = left_top;
  const auto right         = left + ( square_side - 1 );
  const auto bottom        = top + ( square_side - 1 );

  const Power a = get_grid_power( grid, left - 1, top - 1 );
  const Power b = get_grid_power( grid, left - 1, bottom );
  const Power c = get_grid_power( grid, right, top - 1 );
  const Power d = get_grid_power( grid, right, bottom );

  return d - b - c + a;  // https://en.wikipedia.org/wiki/Summed-area_table
}

template <typename F>
void fill_grid( Grid& grid, const F& calc_value )
{
  for ( const auto coord : get_grid_coords( 1, grid_size ) )
  {
    const auto [ x, y ] = coord;
    const auto power    = calc_value( x, y );
    set_grid_power( grid, x, y, power );
  }
}

void fill_grid_with_power( Grid& grid, const size_t serial_num )
{
  auto calc_power = std::bind( &calc_power_level, std::placeholders::_1, std::placeholders::_2, serial_num );
  fill_grid( grid, calc_power );
}

void calc_partial_sums( Grid& grid )
{
  auto calc_partial_sum = std::bind( &calc_partial_sum_power, std::ref( grid ), std::placeholders::_1, std::placeholders::_2 );
  fill_grid( grid, calc_partial_sum );
}

struct MaxPower
{
  Power power;
  Coord top_left;
  size_t square_side;
};

MaxPower find_max_power( const Grid& grid, const size_t square_side )
{
  const auto coords        = get_grid_coords( 1, grid_size - square_side + 1 );
  const Coord max_top_left = ranges::max( coords, [&grid, square_side]( const Coord c1, const Coord c2 ) {
    return get_total_power( grid, c1, square_side ) < get_total_power( grid, c2, square_side );
  } );

  const Power max_power = get_total_power( grid, max_top_left, square_side );
  return { max_power, max_top_left, square_side };
}

std::string coord_to_string( const Coord c )
{
  return std::to_string( c.x ) + "," + std::to_string( c.y );
}

Grid create_grid( std::istream& input )
{
  size_t serial_num = 0;
  input >> serial_num;

  Grid grid;
  fill_grid_with_power( grid, serial_num );
  calc_partial_sums( grid );

  return grid;
}

}  // namespace

namespace AoC_2018
{

namespace problem_11
{

std::string solve_1( std::istream& input )
{
  const auto grid = create_grid( input );

  auto max_power = find_max_power( grid, 3 );
  return coord_to_string( max_power.top_left );
}

std::string solve_2( std::istream& input )
{
  const auto grid = create_grid( input );

  const auto square_sizes = ranges::view::closed_indices( size_t{ 1 }, grid_size );
  const auto max_powers   = square_sizes | ranges::view::transform( std::bind( &find_max_power, std::ref( grid ), std::placeholders::_1 ) );
  const auto max_power    = ranges::max( max_powers, std::less<Power>{}, &MaxPower::power );

  return coord_to_string( max_power.top_left ) + "," + std::to_string( max_power.square_side );
}

AOC_REGISTER_PROBLEM( 2018_11, solve_1, solve_2 );

}  // namespace problem_11

}  // namespace AoC_2018

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  assert( 9 == calc_hundreds_digit( 949 ) );
  assert( 0 == calc_hundreds_digit( 99 ) );
  assert( 4 == calc_power_level( 3, 5, 8 ) );
  assert( -5 == calc_power_level( 122, 79, 57 ) );
  assert( 0 == calc_power_level( 217, 196, 39 ) );
  assert( 4 == calc_power_level( 101, 153, 71 ) );

  Grid grid{ { { { 31, 2, 4 }, { 12, 26, 9 }, { 13, 17, 21 } } } };

  assert( 31 == get_grid_power( grid, 1, 1 ) );
  assert( 0 == get_grid_power( grid, 0, 0 ) );
  assert( 21 == get_grid_power( grid, 3, 3 ) );

  assert( 31 + 0 + 0 - 0 == calc_partial_sum_power( grid, 1, 1 ) );
  assert( 4 + 2 + 0 - 0 == calc_partial_sum_power( grid, 3, 1 ) );
  assert( 13 + 0 + 12 - 0 == calc_partial_sum_power( grid, 1, 3 ) );
  assert( 26 + 12 + 2 - 31 == calc_partial_sum_power( grid, 2, 2 ) );

  Grid psum_grid{ { { { 31, 33, 37 }, { 43, 71, 84 }, { 56, 101, 135 } } } };

  assert( 31 == get_total_power( psum_grid, { 1, 1 }, 1 ) );
  assert( 71 == get_total_power( psum_grid, { 1, 1 }, 2 ) );
  assert( 73 == get_total_power( psum_grid, { 2, 2 }, 2 ) );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
