#include "AoC/2017/problem_19.h"

#include "AoC/problems_map.h"
#include "AoC/utils/2d_array.h"
#include "AoC/utils/assert.h"
#include "AoC/utils/curry.h"
#include "AoC/utils/geo.h"
#include "AoC/utils/ranges/generate.h"

#include "range/v3/algorithm/find.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/take_while.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/numeric/conversion/cast.hpp"

#include <algorithm>
#include <istream>
#include <optional>
#include <string>

namespace
{

using DiagramCell = char;
using Diagram     = AoC::dd_dynamic_heap_array<DiagramCell>;

using CoordPos = size_t;
using Coord    = AoC::GenericPoint<CoordPos>;

enum class Dir
{
  Up,
  Right,
  Down,
  Left
};

struct State
{
  Coord coord;
  Dir dir;
};

CoordPos get_start_x( const Diagram& diagram )
{
  const auto& top_row = AoC::row( diagram, 0 );
  auto iter           = ranges::find( top_row, '|' );
  assert( iter != top_row.end() );

  return boost::numeric_cast<size_t>( std::distance( top_row.begin(), iter ) );
}

DiagramCell get_cell( const Diagram& diagram, const Coord coord )
{
  const auto [ x, y ] = coord;
  return diagram[ x ][ y ];
}

DiagramCell get_cell( const Diagram& diagram, const State state )
{
  return get_cell( diagram, state.coord );
}

std::optional<Coord> get_forward_coord( const Diagram& diagram, const Coord coord, const Dir dir )
{
  const auto [ x, y ] = coord;
  const auto max_x    = diagram.get_width() - 1;
  const auto max_y    = diagram.get_height() - 1;

  using O = std::optional<Coord>;
  switch ( dir )
  {
    // clang-format off
    case Dir::Up:    return y != 0     ? O{ { x,     y - 1 } } : O{};
    case Dir::Left:  return x != 0     ? O{ { x - 1, y     } } : O{};
    case Dir::Right: return x != max_x ? O{ { x + 1, y     } } : O{};
    case Dir::Down:  return y != max_y ? O{ { x,     y + 1 } } : O{};
      // clang-format on
  }

  AOC_ASSERT_FALSE();
}

template <int diff>
Dir do_turn( const Dir dir )
{
  return static_cast<Dir>( ( static_cast<int>( dir ) + 4 + diff ) % 4 );
}

Dir turn_left( const Dir dir )
{
  return do_turn<-1>( dir );
}

Dir turn_right( const Dir dir )
{
  return do_turn<+1>( dir );
}

bool is_empty_cell( const DiagramCell next_cell )
{
  return next_cell != ' ';
}

std::optional<State> try_to_move_forward( const Diagram& diagram, const State state )
{
  const auto forward_coord = get_forward_coord( diagram, state.coord, state.dir );
  if ( !forward_coord )
  {
    return {};
  }

  const auto forward_cell = get_cell( diagram, *forward_coord );
  if ( !is_empty_cell( forward_cell ) )
  {
    return {};
  }

  return { { *forward_coord, state.dir } };
}

std::optional<State> try_to_turn( const Diagram& diagram, const State state )
{
  const auto left_turn_state = try_to_move_forward( diagram, { state.coord, turn_left( state.dir ) } );
  if ( left_turn_state )
  {
    return left_turn_state;
  }

  const auto right_turn_state = try_to_move_forward( diagram, { state.coord, turn_right( state.dir ) } );
  if ( right_turn_state )
  {
    return right_turn_state;
  }

  return {};
}

std::optional<State> do_move( const Diagram& diagram, const State current_state )
{
  const auto forward_state = try_to_move_forward( diagram, current_state );
  if ( forward_state )
  {
    return forward_state;
  }

  const auto turn_state = try_to_turn( diagram, current_state );
  if ( turn_state )
  {
    return turn_state;
  }

  return {};
}

Diagram make_diagram( std::istream& input )
{
  return ranges::getlines( input ) | ranges::views::transform( ranges::to_vector ) | ranges::to_vector;
}

auto get_travelled_path_states( const Diagram& diagram )
{
  const State start_state = { { get_start_x( diagram ), 0 }, Dir::Down };

  return AoC::generate_while( start_state, AOC_CURRY( do_move, std::cref( diagram ) ) );
}

}  // namespace

namespace AoC_2017
{

namespace problem_19
{

std::string solve_1( std::istream& input )
{
  const auto diagram    = make_diagram( input );
  auto travelled_states = get_travelled_path_states( diagram );

  auto letters_seen = travelled_states | ranges::views::transform( AOC_CURRY( get_cell, std::cref( diagram ) ) ) |
                      ranges::views::filter( []( const DiagramCell c ) { return std::isalpha( c ); } );
  return letters_seen | ranges::to<std::string>();
}

size_t solve_2( std::istream& input )
{
  const auto diagram    = make_diagram( input );
  auto travelled_states = get_travelled_path_states( diagram );

  return 1 + boost::numeric_cast<size_t>( ranges::distance( travelled_states ) );
}

AOC_REGISTER_PROBLEM( 2017_19, solve_1, solve_2 );

}  // namespace problem_19
}  // namespace AoC_2017

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  assert( turn_left( Dir::Up ) == Dir::Left );
  assert( turn_left( Dir::Left ) == Dir::Down );
  assert( turn_right( Dir::Up ) == Dir::Right );
  assert( turn_right( Dir::Left ) == Dir::Up );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
