#include "AoC/2015/problem_18.h"

#include <AoC/problems_map.h>

#include <range/v3/algorithm/count.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>

#include <boost/multi_array.hpp>

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
using Field     = boost::multi_array<Cell, 2>;
using CellIndex = Field::index;
using Cells     = std::vector<Cell>;
struct Coord
{
  const CellIndex x;
  const CellIndex y;
};
using Coords = std::vector<Coord>;

Field make_field( const size_t side )
{
  const auto side_max_index = static_cast<CellIndex>( side );
  Field      field( boost::extents[ side_max_index ][ side_max_index ] );
  return field;
}

CellIndex get_max_cell_index( const Field& field )
{
  const auto side = field.shape()[ 0 ];
  assert( side == field.shape()[ 1 ] );

  return static_cast<CellIndex>( side );
}

Field populate_field( std::istream& input, const size_t side )
{
  auto input_cells =
      ranges::istream<char>( input ) | ranges::view::transform( []( const auto c ) {
        return c == '#' ? Cell::Alive : c == '.' ? Cell::Empty : throw std::invalid_argument( "Failed to parse lights input data" );
      } );

  Field field = make_field( side );
  std::copy_n( input_cells.begin(), side * side, field.data() );

  return field;
}

auto make_field_range( const Field& field )
{
  return ranges::make_iterator_range( field.data(), field.data() + field.num_elements() );
}

Coords get_neighbours( const CellIndex x, const CellIndex y, const CellIndex max_index )
{
  Coords coords = { { x - 1, y - 1 }, { x, y - 1 },     { x + 1, y - 1 }, { x - 1, y },
                    { x + 1, y },     { x - 1, y + 1 }, { x, y + 1 },     { x + 1, y + 1 } };

  return coords | ranges::view::filter( [max_index]( const auto coord ) {
           return coord.x >= 0 && coord.x < max_index && coord.y >= 0 && coord.y < max_index;
         } );
}

Cell get_cell( const Field& field, const CellIndex x, const CellIndex y )
{
  return field[ y ][ x ];
}

Cell get_cell( const Field& field, const Coord& coord )
{
  return field[ coord.y ][ coord.x ];
}

void set_cell( Field& field, const CellIndex x, const CellIndex y, const Cell cell )
{
  field[ y ][ x ] = cell;
}

template <typename Cells>
size_t count_alive_cells( const Cells& cells )
{
  return static_cast<size_t>( ranges::count( cells, Cell::Alive ) );
}

size_t get_alive_neighbours_num( const Field& field, const CellIndex x, const CellIndex y )
{
  const CellIndex max_side_index   = get_max_cell_index( field );
  const auto      neighbour_coords = get_neighbours( x, y, max_side_index );
  const auto      alive_neighbour_cells =
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

void process_step( Field& field )
{
  const auto side = field.shape()[ 0 ];
  assert( side == field.shape()[ 1 ] );

  Field new_field = make_field( side );

  const CellIndex max_side_index = get_max_cell_index( field );
  for ( CellIndex x = 0; x < max_side_index; ++x )
  {
    for ( CellIndex y = 0; y < max_side_index; ++y )
    {
      const auto cell                       = get_cell( field, x, y );
      const auto alive_neighbours_cells_num = get_alive_neighbours_num( field, x, y );

      Cell new_cell = calc_new_state( cell, alive_neighbours_cells_num );
      set_cell( new_field, x, y, new_cell );
    }
  }

  field = std::move( new_field );
}

void enable_corner_lights( Field& field )
{
  const CellIndex max_side_index = get_max_cell_index( field );
  set_cell( field, 0, 0, Cell::Alive );
  set_cell( field, 0, max_side_index - 1, Cell::Alive );
  set_cell( field, max_side_index - 1, 0, Cell::Alive );
  set_cell( field, max_side_index - 1, max_side_index - 1, Cell::Alive );
}

int solve( std::istream& input, const size_t side, size_t steps, const bool force_corner_lights_on = false )
{
  Field field = populate_field( input, side );
  if ( force_corner_lights_on )
  {
    enable_corner_lights( field );
  }

  for ( ; steps > 0; --steps )
  {
    process_step( field );

    if ( force_corner_lights_on )
    {
      enable_corner_lights( field );
    }
  }

  return static_cast<int>( count_alive_cells( make_field_range( field ) ) );
}
}  // namespace

namespace AoC_2015
{

namespace problem_18
{

int solve_1( std::istream& input )
{
  return solve( input, 100, 100 );
}

int solve_2( std::istream& input )
{
  return solve( input, 100, 100, true );
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
    auto field = populate_field( input, test_sample_side );

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
    assert( 4 == solve( input, test_sample_side, 4 ) );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
