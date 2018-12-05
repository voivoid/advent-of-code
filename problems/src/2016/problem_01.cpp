#include "AoC/2016/problem_01.h"

#include "AoC/problems_map.h"
#include "AoC/utils/geo.h"
#include "AoC/utils/parse.h"

#include "range/v3/istream_range.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/to_container.hpp"
#include "range/v3/view/exclusive_scan.hpp"
#include "range/v3/view/indices.hpp"
#include "range/v3/view/join.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/view/zip_with.hpp"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/spirit/home/x3.hpp"

#include <istream>
#include <set>
#include <stdexcept>
#include <string>

namespace
{

using Coord = AoC::Coord;

struct Instruction
{
  enum class Rotation
  {
    Left,
    Right,
    Forward
  };

  Rotation rotation;
  size_t steps_num;
};

struct State
{
  enum class Heading
  {
    North,
    East,
    South,
    West
  };

  Heading heading;
  Coord coord;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( Instruction, rotation, steps_num )

namespace
{

Instruction parse_instruction( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  const auto rotation_parser = x3::symbols<Instruction::Rotation>();
  rotation_parser.add( "L", Instruction::Rotation::Left )( "R", Instruction::Rotation::Right );
  const auto parser = rotation_parser > AoC::size_t_parser;

  Instruction instruction;
  const bool is_parsed = AoC::x3_parse( line.cbegin(), line.cend(), parser, x3::space | x3::punct, instruction );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input instruction data" );
  }

  return instruction;
}

State::Heading turn_right( const State::Heading heading )
{
  switch ( heading )
  {
    case State::Heading::North: return State::Heading::East;
    case State::Heading::East: return State::Heading::South;
    case State::Heading::South: return State::Heading::West;
    case State::Heading::West: return State::Heading::North;
  }

  throw std::runtime_error( "Unexpected heading" );
}

State::Heading turn_left( const State::Heading heading )
{
  switch ( heading )
  {
    case State::Heading::North: return State::Heading::West;
    case State::Heading::West: return State::Heading::South;
    case State::Heading::South: return State::Heading::East;
    case State::Heading::East: return State::Heading::North;
  }

  throw std::runtime_error( "Unexpected heading" );
}

Coord move( const Coord coord, const State::Heading heading, const size_t steps_num )
{
  const auto diff = [=]() -> Coord {
    const int d = static_cast<int>( steps_num );
    switch ( heading )
    {
      case State::Heading::North: return { 0, -d };
      case State::Heading::East: return { d, 0 };
      case State::Heading::South: return { 0, d };
      case State::Heading::West: return { -d, 0 };
    }

    throw std::runtime_error( "Unexpected heading" );
  }();

  return { coord.x + diff.x, coord.y + diff.y };
}

State::Heading calc_new_heading( const State::Heading heading, const Instruction::Rotation rotation )
{
  switch ( rotation )
  {
    case Instruction::Rotation::Left: return turn_left( heading );
    case Instruction::Rotation::Right: return turn_right( heading );
    case Instruction::Rotation::Forward: return heading;
  }

  throw std::runtime_error( "Unexpected rotation" );
}

State run_instruction( const State state, const Instruction instruction )
{
  const auto new_heading = calc_new_heading( state.heading, instruction.rotation );
  const auto new_coord   = move( state.coord, new_heading, instruction.steps_num );
  return { new_heading, new_coord };
}

auto run_instruction_steps_by_step( State state, const Instruction instruction )
{
  state.heading = calc_new_heading( state.heading, instruction.rotation );
  return ranges::view::closed_indices( size_t( 1 ), instruction.steps_num ) |
         ranges::view::exclusive_scan(
             state, std::bind( &run_instruction, std::placeholders::_1, Instruction{ Instruction::Rotation::Forward, 1 } ) );
}

int calc_distance( const Coord c1, const Coord c2 )
{
  return std::abs( c2.x - c1.x ) + std::abs( c2.y - c1.y );
}

template <typename Range>
Coord find_already_visited_coord( Range coords )
{
  std::set<Coord> visited_coords;
  for ( const auto coord : coords )
  {
    auto [ _, is_inserted ] = visited_coords.insert( coord );
    if ( !is_inserted )
    {
      return coord;
    }
  }

  throw std::runtime_error( "None of locations was visited twice" );
}

auto parse_instructions( std::istream& input )
{
  return ranges::istream_range<std::string>( input ) | ranges::view::transform( &parse_instruction );
}

constexpr State start_state = { State::Heading::North, { 0, 0 } };

}  // namespace

namespace AoC_2016
{

namespace problem_01
{

int solve_1( std::istream& input )
{
  auto instructions      = parse_instructions( input );
  const auto final_state = ranges::accumulate( instructions, start_state, &run_instruction );

  return calc_distance( start_state.coord, final_state.coord );
}

int solve_2( std::istream& input )
{
  const auto instructions  = parse_instructions( input ) | ranges::to_vector;
  const auto corner_states = instructions | ranges::view::exclusive_scan( start_state, &run_instruction );

  const auto all_visited_coords = ranges::view::zip_with( &run_instruction_steps_by_step, corner_states, instructions ) |
                                  ranges::view::join | ranges::view::transform( []( const State& s ) { return s.coord; } );

  const auto already_visited_coord = find_already_visited_coord( all_visited_coords );
  return calc_distance( start_state.coord, already_visited_coord );
}

AOC_REGISTER_PROBLEM( 2016_01, solve_1, solve_2 );

}  // namespace problem_01

}  // namespace AoC_2016

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  const auto i1 = parse_instruction( "L11" );
  assert( 11 == i1.steps_num );
  assert( Instruction::Rotation::Left == i1.rotation );

  const auto i2 = parse_instruction( "R22" );
  assert( 22 == i2.steps_num );
  assert( Instruction::Rotation::Right == i2.rotation );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
