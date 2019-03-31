#include "AoC/2016/problem_08.h"

#include "AoC/problems_map.h"
#include "AoC/utils/2d_array.h"
#include "AoC/utils/curry.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/count.hpp"
#include "range/v3/getlines.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/to_container.hpp"
#include "range/v3/view/chunk.hpp"
#include "range/v3/view/indices.hpp"
#include "range/v3/view/join.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/view/zip.hpp"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/hof/lift.hpp"
#include "boost/numeric/conversion/cast.hpp"
#include "boost/spirit/home/x3.hpp"
#include "boost/variant.hpp"

#include <algorithm>
#include <bitset>
#include <istream>
#include <stdexcept>
#include <string>
#include <unordered_map>


namespace
{
using Coord = size_t;

namespace Commands
{

struct Rect
{
  Coord x;
  Coord y;
};

struct RotateCol
{
  Coord x;
  Coord by;
};

struct RotateRow
{
  Coord y;
  Coord by;
};
}  // namespace Commands

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( Commands::Rect, x, y )
BOOST_FUSION_ADAPT_STRUCT( Commands::RotateCol, x, by )
BOOST_FUSION_ADAPT_STRUCT( Commands::RotateRow, y, by )

namespace
{
using CharCode    = unsigned long;
using CharCodeMap = std::unordered_map<CharCode, char>;
using Command     = boost::variant<Commands::Rect, Commands::RotateCol, Commands::RotateRow>;

Command parse_command( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  const auto cp = AoC::x3_size_t_;

  const auto rect_parser = x3::rule<struct _rect, Commands::Rect>{} = "rect" > cp > 'x' > cp;
  const auto rotate_col_parser = x3::rule<struct _col, Commands::RotateCol>{} = ( x3::lit( "rotate" ) >> "column" ) > "x=" > cp > "by" > cp;
  const auto rotate_row_parser = x3::rule<struct _row, Commands::RotateRow>{} = ( x3::lit( "rotate" ) >> "row" ) > "y=" > cp > "by" > cp;

  const auto parser = rect_parser | rotate_col_parser | rotate_row_parser;

  Command command;
  const bool is_parsed = AoC::x3_parse( line.cbegin(), line.cend(), parser, x3::space, command );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input command data" );
  }

  return command;
}

constexpr size_t ScreenWidth  = 50;
constexpr size_t ScreenHeight = 6;
constexpr size_t LetterWidth  = 5;
using Screen                  = AoC::dd_static_stack_array<bool, ScreenWidth, ScreenHeight>;

void apply_command( Screen& screen, const Commands::Rect cmd )
{
  assert( cmd.x < screen.get_width() );
  assert( cmd.y < screen.get_height() );

  for ( size_t y = 0; y < cmd.y; ++y )
  {
    for ( size_t x = 0; x < cmd.x; ++x )
    {
      screen[ x ][ y ] = true;
    }
  }
}

template <typename T>
void rotate( const T& colrow, const size_t by )
{
  const auto rotate_steps = boost::numeric_cast<std::ptrdiff_t>( by % colrow.size() );
  std::rotate( ranges::rbegin( colrow ), ranges::rbegin( colrow ) + rotate_steps, ranges::rend( colrow ) );
}

void apply_command( Screen& screen, const Commands::RotateCol cmd )
{
  assert( cmd.x < screen.get_width() );
  rotate( AoC::column( screen, cmd.x ), cmd.by );
}

void apply_command( Screen& screen, const Commands::RotateRow cmd )
{
  assert( cmd.y < screen.get_height() );
  rotate( AoC::row( screen, cmd.y ), cmd.by );
}

void run_command( Screen& screen, const Command& command )
{
  const auto visitor = AoC::curry( BOOST_HOF_LIFT( apply_command ) )( std::ref( screen ) );
  boost::apply_visitor( visitor, command );
}

int calc_lit_pixels( const Screen& screen )
{
  return boost::numeric_cast<int>( ranges::count( screen, true ) );
}

Screen run_commands( std::istream& istream )
{
  auto commands = ranges::getlines( istream ) | ranges::view::transform( &parse_command );

  Screen screen;
  for ( const auto& cmd : commands )
  {
    run_command( screen, cmd );
  }

  return screen;
}

auto letter_to_code()
{
  return ranges::view::transform( ranges::view::join ) | ranges::view::transform( []( auto bits ) {
           std::string bitstr = bits;
           assert( bitstr.length() == LetterWidth * ScreenHeight );

           return std::bitset<ScreenWidth>( bitstr ).to_ulong();
         } );
}

template <typename Range, typename B>
auto bits_to_letters( const Range& bits, const size_t letters_num, const B bit1 )
{
  assert( bits.size() == letters_num * LetterWidth * ScreenHeight );
  const auto letter_chunks = bits | ranges::view::transform( [bit1]( const auto c ) { return c == bit1 ? '1' : '0'; } ) |
                             ranges::view::chunk( letters_num * LetterWidth ) |
                             ranges::view::transform( ranges::view::chunk( LetterWidth ) );
  return ranges::view::indices( letters_num ) | ranges::view::transform( [letter_chunks]( const auto n ) {
           return letter_chunks | ranges::view::transform( [n]( const auto line ) {
                    assert( n < line.size() );
                    return line[ boost::numeric_cast<ptrdiff_t>( n ) ];
                  } );
         } );
}

CharCodeMap init_charcode_map()
{
  const std::string graphchars =
      " ##  ###   ##  #### ####  ##  #  #  ###   ## #  # #     ##  ###  ###   ### #  # #   ##### "
      "#  # #  # #  # #    #    #  # #  #   #     # # #  #    #  # #  # #  # #    #  # #   #   # "
      "#  # ###  #    ###  ###  #    ####   #     # ##   #    #  # #  # #  # #    #  #  # #   #  "
      "#### #  # #    #    #    # ## #  #   #     # # #  #    #  # ###  ###   ##  #  #   #   #   "
      "#  # #  # #  # #    #    #  # #  #   #  #  # # #  #    #  # #    # #     # #  #   #  #    "
      "#  # ###   ##  #### #     ### #  #  ###  ##  #  # ####  ##  #    #  # ###   ##    #  #### ";
  const std::string chars = "abcefghijkloprsuyz";

  const auto letter_codes = bits_to_letters( graphchars, chars.length(), '#' ) | letter_to_code();

  return ranges::view::zip( letter_codes, chars );
}

const CharCodeMap& get_charcode_map()
{
  static CharCodeMap map = init_charcode_map();
  return map;
}

char get_char_from_charcode( const CharCode charcode )
{
  const auto& map = get_charcode_map();
  auto iter       = map.find( charcode );
  if ( iter == map.cend() )
  {
    throw std::runtime_error( "Unexpected charcode" );
  }

  return iter->second;
}

}  // namespace

namespace AoC_2016
{

namespace problem_08
{

int solve_1( std::istream& istream )
{
  const auto screen = run_commands( istream );
  return calc_lit_pixels( screen );
}

std::string solve_2( std::istream& istream )
{
  const auto screen = run_commands( istream );

  constexpr auto letters_num = ScreenWidth / LetterWidth;
  const auto letters         = bits_to_letters( screen, letters_num, true );
  const auto letter_codes    = letters | letter_to_code();

  const std::string result = letter_codes | ranges::view::transform( &get_char_from_charcode );
  return result;
}

AOC_REGISTER_PROBLEM( 2016_08, solve_1, solve_2 );

}  // namespace problem_08

}  // namespace AoC_2016

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  auto rect = boost::get<Commands::Rect>( parse_command( "rect 3x2" ) );
  assert( rect.x == 3 );
  assert( rect.y == 2 );

  auto col = boost::get<Commands::RotateCol>( parse_command( "rotate column x=1 by 1" ) );
  assert( col.x == 1 );
  assert( col.by == 1 );

  auto row = boost::get<Commands::RotateRow>( parse_command( "rotate row y=0 by 4" ) );
  assert( row.y == 0 );
  assert( row.by == 4 );

  {
    Screen screen;
    apply_command( screen, Commands::Rect{ 2, 2 } );
    assert( 4 == calc_lit_pixels( screen ) );

    apply_command( screen, Commands::RotateCol{ 0, 0 } );
    apply_command( screen, Commands::RotateRow{ 0, 0 } );
    assert( 4 == calc_lit_pixels( screen ) );
    assert( screen[ 0 ][ 0 ] == true );
    assert( screen[ 0 ][ 1 ] == true );
    assert( screen[ 1 ][ 0 ] == true );
    assert( screen[ 1 ][ 1 ] == true );

    apply_command( screen, Commands::RotateCol{ 0, 6 } );
    apply_command( screen, Commands::RotateRow{ 0, 50 } );
    assert( 4 == calc_lit_pixels( screen ) );
    assert( screen[ 0 ][ 0 ] == true );
    assert( screen[ 0 ][ 1 ] == true );
    assert( screen[ 1 ][ 0 ] == true );
    assert( screen[ 1 ][ 1 ] == true );

    apply_command( screen, Commands::RotateCol{ 0, 2 } );
    apply_command( screen, Commands::RotateCol{ 1, 2 } );
    apply_command( screen, Commands::RotateRow{ 2, 2 } );
    apply_command( screen, Commands::RotateRow{ 3, 2 } );
    assert( 4 == calc_lit_pixels( screen ) );
    assert( screen[ 2 ][ 2 ] == true );
    assert( screen[ 2 ][ 3 ] == true );
    assert( screen[ 3 ][ 2 ] == true );
    assert( screen[ 3 ][ 3 ] == true );
  }

  {
    const std::string letter =
        "#### "
        "   # "
        "  #  "
        " #   "
        "#    "
        "#### ";
    const auto letter_codes = bits_to_letters( letter, 1, '#' ) | letter_to_code();
    assert( 1008869918ul == letter_codes[ 0 ] );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
