#include "AoC/2016/problem_02.h"

#include "AoC/problems_map.h"
#include "AoC/utils/2d_array.h"
#include "AoC/utils/geo.h"
#include "AoC/utils/parse.h"
#include "AoC/utils/ranges/append.h"

#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/exclusive_scan.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/tail.hpp"
#include "range/v3/view/transform.hpp"

#include <istream>
#include <stdexcept>
#include <string>
#include <vector>


namespace
{

enum class Instruction
{
  Up,
  Right,
  Down,
  Left
};

using Instructions = std::vector<Instruction>;
using CursorPos    = AoC::UPoint;

constexpr char NoButton = '0';

template <size_t side>
using Keypad = AoC::dd_static_stack_array<char, side, side>;

Instructions parse_instructions( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  x3::symbols<Instruction> instruction_parser;
  instruction_parser.add( "U", Instruction::Up )( "R", Instruction::Right )( "D", Instruction::Down )( "L", Instruction::Left );

  const auto parser = *instruction_parser;

  Instructions instructions;
  const bool is_parsed = AoC::x3_parse( line, parser, x3::space, instructions );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input instruction data" );
  }

  return instructions;
}

template <size_t side>
bool check_keypad_has_button( const CursorPos cursor_pos, const Keypad<side>& keypad )
{
  return keypad[ cursor_pos.x ][ cursor_pos.y ] != NoButton;
}

auto dec_cursor_coord( const size_t coord )
{
  return coord > 0 ? coord - 1 : 0;
}

auto inc_cursor_coord( const size_t coord, const size_t side )
{
  return coord < ( side - 1 ) ? coord + 1 : ( side - 1 );
}

CursorPos calc_new_cursor_pos( const CursorPos cursor_pos, const Instruction instruction, const size_t side )
{
  switch ( instruction )
  {
    case Instruction::Up: return { cursor_pos.x, dec_cursor_coord( cursor_pos.y ) };
    case Instruction::Down: return { cursor_pos.x, inc_cursor_coord( cursor_pos.y, side ) };
    case Instruction::Left: return { dec_cursor_coord( cursor_pos.x ), cursor_pos.y };
    case Instruction::Right: return { inc_cursor_coord( cursor_pos.x, side ), cursor_pos.y };
  };

  throw std::runtime_error( "Unexpected instruction" );
}

template <size_t side>
CursorPos move_cursor( const CursorPos cursor_pos, const Instruction instruction, const Keypad<side>& keypad )
{
  const auto new_cursor_pos = calc_new_cursor_pos( cursor_pos, instruction, side );
  return check_keypad_has_button( new_cursor_pos, keypad ) ? new_cursor_pos : cursor_pos;
}

template <typename Instructions, size_t side>
CursorPos run_instructions( const CursorPos cursor_pos, Instructions instructions, const Keypad<side>& keypad )
{
  return ranges::accumulate( instructions, cursor_pos, [&keypad]( const CursorPos cursor, const Instruction instruction ) {
    return move_cursor( cursor, instruction, keypad );
  } );
}

template <size_t side>
char code_by_position( const CursorPos cursor_pos, const Keypad<side>& keypad )
{
  return keypad[ cursor_pos.x ][ cursor_pos.y ];
}

template <size_t side>
std::string solve( std::istream& input, const Keypad<side>& keypad )
{
  const CursorPos start_cursor_pos = *AoC::dd_array_find_elem_indices( keypad, '5' );

  // appending dummy instructions since ranges v3 has no inclusive_scan yet :(
  auto keypad_positions = ranges::getlines( input ) | ranges::view::transform( &parse_instructions ) | AoC::append( Instructions{} ) |
                          ranges::view::exclusive_scan( start_cursor_pos, [&keypad]( const CursorPos cursor, const auto instructions ) {
                            return run_instructions( cursor, instructions, keypad );
                          } );

  auto codes = keypad_positions | ranges::view::tail |
               ranges::view::transform( [&keypad]( const CursorPos cursor ) { return code_by_position( cursor, keypad ); } );

  return codes | ranges::to<std::string>;
}

}  // namespace

namespace AoC_2016
{

namespace problem_02
{

std::string solve_1( std::istream& input )
{
  const Keypad<3> keypad{ { { { '1', '2', '3' }, { '4', '5', '6' }, { '7', '8', '9' } } } };
  return solve( input, keypad );
}

std::string solve_2( std::istream& input )
{
  const Keypad<5> keypad{ { { { '0', '0', '1', '0', '0' },
                              { '0', '2', '3', '4', '0' },
                              { '5', '6', '7', '8', '9' },
                              { '0', 'A', 'B', 'C', '0' },
                              { '0', '0', 'D', '0', '0' } } } };
  return solve( input, keypad );
}

AOC_REGISTER_PROBLEM( 2016_02, solve_1, solve_2 );

}  // namespace problem_02

}  // namespace AoC_2016
