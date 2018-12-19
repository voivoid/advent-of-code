#include "AoC/2015/problem_18.h"

#include "AoC/problems_map.h"
#include "AoC/utils/2d_array.h"

#include "range/v3/algorithm/count.hpp"
#include "range/v3/istream_range.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/transform.hpp"

#include <algorithm>
#include <functional>
#include <istream>
#include <stdexcept>
#include <vector>

namespace
{
enum class Cell
{
  Alive,
  Empty
};

template <size_t side>
using Field = AoC::dd_static_stack_array<Cell, side, side>;
using Cells = std::vector<Cell>;
struct Coord
{
  const size_t x;
  const size_t y;
};
using Coords = std::vector<Coord>;

template <size_t side>
Field<side> populate_field( std::istream& input )
{
  auto input_cells =
      ranges::istream<char>( input ) | ranges::view::transform( []( const auto c ) {
        return c == '#' ? Cell::Alive : c == '.' ? Cell::Empty : throw std::invalid_argument( "Failed to parse lights input data" );
      } );

  Field<side> field;
  std::copy_n( input_cells.begin(), side * side, field.begin() );

  return field;
}

Coords get_neighbours( const size_t x, const size_t y, const size_t max_index )
{
  const std::initializer_list<Coord> coords = { { x - 1, y - 1 }, { x, y - 1 },     { x + 1, y - 1 }, { x - 1, y },
                                                { x + 1, y },     { x - 1, y + 1 }, { x, y + 1 },     { x + 1, y + 1 } };

  return coords | ranges::view::filter( [max_index]( const auto coord ) { return coord.x < max_index && coord.y < max_index; } );
}

template <size_t side>
Cell get_cell( const Field<side>& field, const size_t x, const size_t y )
{
  return field[ x ][ y ];
}

template <size_t side>
Cell get_cell( const Field<side>& field, const Coord& coord )
{
  return get_cell( field, coord.x, coord.y );
}

template <size_t side>
void set_cell( Field<side>& field, const size_t x, const size_t y, const Cell cell )
{
  field[ x ][ y ] = cell;
}

template <typename Cells>
size_t count_alive_cells( const Cells& cells )
{
  return static_cast<size_t>( ranges::count( cells, Cell::Alive ) );
}

template <size_t side>
size_t get_alive_neighbours_num( const Field<side>& field, const size_t x, const size_t y )
{
  const auto neighbour_coords = get_neighbours( x, y, side );
  const auto alive_neighbour_cells =
      neighbour_coords | ranges::view::transform( [&field]( const auto& coord ) { return get_cell( field, coord ); } );
  return count_alive_cells( alive_neighbour_cells );
}

Cell calc_new_state( const Cell cell, const size_t alive_neighbours_cells_num )
{
  if ( cell == Cell::Alive )
  {
    return alive_neighbours_cells_num == 2 || alive_neighbours_cells_num == 3 ? Cell::Alive : Cell::Empty;
  }
  else
  {
    return alive_neighbours_cells_num == 3 ? Cell::Alive : Cell::Empty;
  }
}

template <size_t side>
void process_step( Field<side>& field )
{
  Field<side> new_field;

  for ( size_t x = 0; x < side; ++x )
  {
    for ( size_t y = 0; y < side; ++y )
    {
      const auto cell                       = get_cell( field, x, y );
      const auto alive_neighbours_cells_num = get_alive_neighbours_num( field, x, y );

      Cell new_cell = calc_new_state( cell, alive_neighbours_cells_num );
      set_cell( new_field, x, y, new_cell );
    }
  }

  field = std::move( new_field );
}

template <size_t side>
void enable_corner_lights( Field<side>& field )
{
  set_cell( field, 0, 0, Cell::Alive );
  set_cell( field, 0, side - 1, Cell::Alive );
  set_cell( field, side - 1, 0, Cell::Alive );
  set_cell( field, side - 1, side - 1, Cell::Alive );
}

enum class CornerLightsMode
{
  On,
  Off
};

template <size_t side>
int solve( std::istream& input, size_t steps, const CornerLightsMode corner_lights_mode )
{
  Field<side> field = populate_field<side>( input );
  if ( corner_lights_mode == CornerLightsMode::On )
  {
    enable_corner_lights( field );
  }

  for ( ; steps > 0; --steps )
  {
    process_step( field );

    if ( corner_lights_mode == CornerLightsMode::On )
    {
      enable_corner_lights( field );
    }
  }

  return static_cast<int>( count_alive_cells( field ) );
}
}  // namespace

namespace AoC_2015
{

namespace problem_18
{

int solve_1( std::istream& input )
{
  return solve<100>( input, 100, CornerLightsMode::Off );
}

int solve_2( std::istream& input )
{
  return solve<100>( input, 100, CornerLightsMode::On );
}

AOC_REGISTER_PROBLEM( 2015_18, solve_1, solve_2 );

}  // namespace problem_18

}  // namespace AoC_2015


#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>
#  include <sstream>

std::stringstream get_test_sample()
{
  return std::stringstream(
      ".#.#.#\n"
      "...##.\n"
      "#....#\n"
      "..#...\n"
      "#.#..#\n"
      "####.." );
}

static void impl_tests()
{
  constexpr size_t test_sample_side = 6;

  {
    auto input = get_test_sample();
    auto field = populate_field<test_sample_side>( input );

    assert( Cell::Empty == get_cell( field, 0, 0 ) );
    assert( Cell::Alive == get_cell( field, 1, 0 ) );
    assert( Cell::Empty == get_cell( field, 0, 1 ) );
    assert( Cell::Alive == get_cell( field, 5, 0 ) );
    assert( Cell::Alive == get_cell( field, 0, 5 ) );
    assert( Cell::Empty == get_cell( field, 5, 5 ) );

    const auto neighbours1 = get_neighbours( 3, 4, 6 );
    assert( neighbours1.size() == 8 );

    const auto neighbours2 = get_neighbours( 1, 0, 6 );
    assert( neighbours2.size() == 5 );


    assert( get_alive_neighbours_num( field, 2, 2 ) == 2 );
    assert( get_alive_neighbours_num( field, 0, 0 ) == 1 );
    assert( get_alive_neighbours_num( field, 0, 5 ) == 2 );
    assert( get_alive_neighbours_num( field, 2, 4 ) == 4 );
  }

  {
    auto input = get_test_sample();
    assert( 4 == solve<test_sample_side>( input, 4, CornerLightsMode::Off ) );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
