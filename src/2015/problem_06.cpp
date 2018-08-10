#include "AoC/2015/problem_06.h"

#include <AoC/problems_map.h>
#include <AoC_utils/geo.h>
#include <AoC_utils/match.h>


#include <range/v3/action/split.hpp>
#include <range/v3/getlines.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/c_str.hpp>
#include <range/v3/view/transform.hpp>

#include <boost/multi_array.hpp>
#include <stdexcept>

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
  } mode;
  AoC::Rectangle rect;
};

using Lamp  = size_t;
using Lamps = boost::multi_array<Lamp, 2>;

Cmd::Mode parse_cmd_mode( const std::string& mode )
{
  if ( mode == "on" )
  {
    return Cmd::Mode::on;
  }
  else if ( mode == "off" )
  {
    return Cmd::Mode::off;
  }
  else if ( mode == "toggle" )
  {
    return Cmd::Mode::toggle;
  }

  THROW_WRONG_CMD_EX();
}

AoC::Point parse_cmd_coord( const std::string& coord )
{
  auto coords = ranges::action::split( coord, ranges::view::c_str( "," ) );
  return { std::stoi( coords[ 0 ] ), std::stoi( coords[ 1 ] ) };
}

AoC::Rectangle parse_cmd_rect( const std::string& from, const std::string& to )
{
  return { parse_cmd_coord( from ), parse_cmd_coord( to ) };
}

Cmd parse_cmd( const std::string& mode, const std::string& from, const std::string& to )
{
  return { parse_cmd_mode( mode ), parse_cmd_rect( from, to ) };
}

Cmd parse_cmd_line( const std::string& line )
{
  const auto tokens = ranges::action::split( line, ' ' );

  using S        = const std::string&;
  const auto cmd = AoC::match_container( tokens,
                                         []( S /*turn*/, S mode, S from, S /*through*/, S to ) { return parse_cmd( mode, from, to ); },
                                         []( S mode, S from, S /*through*/, S to ) { return parse_cmd( mode, from, to ); },
                                         []( const auto& ) -> Cmd { THROW_WRONG_CMD_EX(); } );

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

template <ManipulatorFactory manipulator_factory>
auto apply_cmd( std::reference_wrapper<Lamps> lamps, const Cmd& cmd )
{
  const auto& rect        = cmd.rect;
  const auto  manipulator = manipulator_factory( cmd.mode );
  for ( auto x = rect.left_top.x; x <= rect.right_bottom.x; ++x )
  {
    for ( auto y = rect.left_top.y; y <= rect.right_bottom.y; ++y )
    {
      manipulator( lamps.get()[ y ][ x ] );
    }
  }
  return lamps;
}

size_t calc_lamps_brightness( const Lamps& lamps )
{
  return std::accumulate( lamps.data(), lamps.data() + lamps.num_elements(), size_t( 0 ) );
}

template <ManipulatorFactory manipulator_factory>
int solve( std::istream& input )
{
  auto  cmds = ranges::getlines( input ) | ranges::view::transform( &parse_cmd_line );
  Lamps lamps( boost::extents[ 1000 ][ 1000 ] );

  auto run_cmd = &apply_cmd<manipulator_factory>;
  ranges::accumulate( cmds, std::ref( lamps ), run_cmd );

  return static_cast<int>( calc_lamps_brightness( lamps ) );
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
  assert( Cmd::Mode::on == parse_cmd_mode( "on" ) );
  assert( ( AoC::Point{ 123, 456 } ) == parse_cmd_coord( "123,456" ) );
  assert( ( AoC::Rectangle{ { 111, 222 }, { 333, 444 } } ) == parse_cmd_rect( "111,222", "333,444" ) );

  {
    const Cmd cmd = parse_cmd_line( "turn off 111,222 through 333,444" );
    assert( cmd.mode == Cmd::Mode::off );
    assert( cmd.rect == ( AoC::Rectangle{ { 111, 222 }, { 333, 444 } } ) );
  }

  {
    Lamps lamps( boost::extents[ 10 ][ 10 ] );
    assert( 0 == calc_lamps_brightness( lamps ) );

    apply_cmd<&make_lamp_manipulator_1>( std::ref( lamps ), parse_cmd_line( "toggle on 0,0 through 1,1" ) );

    assert( 4 == calc_lamps_brightness( lamps ) );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
