#include "AoC/2015/problem_06.h"

#include "AoC/problems_map.h"
#include "AoC/utils/2d_array.h"
#include "AoC/utils/geo.h"
#include "AoC/utils/parse.h"

#include "range/v3/getlines.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/c_str.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/fusion/include/vector.hpp"
#include "boost/numeric/conversion/cast.hpp"
#include "boost/spirit/home/x3.hpp"
#include "boost/variant.hpp"

#include <functional>
#include <istream>
#include <numeric>
#include <stdexcept>
#include <string>

#define THROW_WRONG_CMD_EX() throw std::invalid_argument( "wrong cmd" )

namespace
{

struct Cmd
{
  enum class Mode
  {
    on,
    off,
    toggle
  };

  Mode mode;
  AoC::URectangle rect;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( AoC::UPoint, x, y )
BOOST_FUSION_ADAPT_STRUCT( AoC::URectangle, left_top, right_bottom )
BOOST_FUSION_ADAPT_STRUCT( Cmd, mode, rect )

namespace
{

using Lamp = size_t;

template <size_t grid_side>
using Lamps = AoC::dd_static_heap_array<Lamp, grid_side, grid_side>;

Cmd parse_cmd_line( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  x3::symbols<Cmd::Mode> on_off_modes;
  on_off_modes.add( "on", Cmd::Mode::on )( "off", Cmd::Mode::off );

  x3::symbols<Cmd::Mode> toggle_modes;
  toggle_modes.add( "toggle", Cmd::Mode::toggle );

  const auto coord_parser = x3::rule<struct _point, AoC::UPoint>{} = AoC::x3_size_t_ > ',' > AoC::x3_size_t_;
  const auto rect_parser = x3::rule<struct _rect, AoC::URectangle>{} = coord_parser > "through" > coord_parser;
  const auto parser = ( "turn" > on_off_modes > rect_parser ) | ( toggle_modes > rect_parser );

  Cmd cmd;
  const bool is_parsed = AoC::x3_parse( line.cbegin(), line.cend(), parser, x3::space, cmd );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input command data" );
  }

  return cmd;
}


using LampManipulator    = void ( * )( Lamp& );
using ManipulatorFactory = LampManipulator ( * )( Cmd::Mode );

LampManipulator make_lamp_manipulator_1( const Cmd::Mode mode )
{
  switch ( mode )
  {
    case Cmd::Mode::on: return []( Lamp& l ) { l = 1; };
    case Cmd::Mode::off: return []( Lamp& l ) { l = 0; };
    case Cmd::Mode::toggle: return []( Lamp& l ) { l = 1 - l; };
  }

  THROW_WRONG_CMD_EX();
}

LampManipulator make_lamp_manipulator_2( const Cmd::Mode mode )
{
  switch ( mode )
  {
    case Cmd::Mode::on: return []( Lamp& l ) { ++l; };
    case Cmd::Mode::off: return []( Lamp& l ) { l == 0 ? 0 : --l; };
    case Cmd::Mode::toggle: return []( Lamp& l ) { l += 2; };
  }

  THROW_WRONG_CMD_EX();
}

template <ManipulatorFactory manipulator_factory, size_t side>
void apply_cmd( Lamps<side>& lamps, const Cmd& cmd )
{
  const auto& rect       = cmd.rect;
  const auto manipulator = manipulator_factory( cmd.mode );
  for ( auto y = rect.left_top.y; y <= rect.right_bottom.y; ++y )
  {
    for ( auto x = rect.left_top.x; x <= rect.right_bottom.x; ++x )
    {
      manipulator( lamps[ x ][ y ] );
    }
  }
}

template <size_t side>
size_t calc_lamps_brightness( const Lamps<side>& lamps )
{
  return ranges::accumulate( lamps, size_t{ 0 } );
}

template <ManipulatorFactory manipulator_factory>
int solve( std::istream& input )
{
  auto cmds                  = ranges::getlines( input ) | ranges::view::transform( &parse_cmd_line );
  constexpr size_t grid_side = 1000;
  Lamps<grid_side> lamps;

  for ( const auto& cmd : cmds )
  {
    apply_cmd<manipulator_factory, grid_side>( lamps, cmd );
  }

  return boost::numeric_cast<int>( calc_lamps_brightness( lamps ) );
}

}  // namespace

namespace AoC_2015
{

namespace problem_06
{

int solve_1( std::istream& input )
{
  return solve<&make_lamp_manipulator_1>( input );
}

int solve_2( std::istream& input )
{
  return solve<&make_lamp_manipulator_2>( input );
}

AOC_REGISTER_PROBLEM( 2015_06, solve_1, solve_2 );

}  // namespace problem_06

}  // namespace AoC_2015



#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  {
    const Cmd cmd = parse_cmd_line( "turn off 111,222 through 333,444" );
    assert( cmd.mode == Cmd::Mode::off );
    assert( cmd.rect == ( AoC::URectangle{ { 111, 222 }, { 333, 444 } } ) );
  }

  {
    Lamps<10> lamps;
    assert( 0 == calc_lamps_brightness( lamps ) );

    apply_cmd<&make_lamp_manipulator_1, 10>( std::ref( lamps ), parse_cmd_line( "toggle 0,0 through 1,1" ) );

    assert( 4 == calc_lamps_brightness( lamps ) );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
