#include "AoC/2017/problem_16.h"

#include "AoC/problems_map.h"

#include "AoC/utils/curry.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/find.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/take.hpp"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/numeric/conversion/cast.hpp"
#include "boost/variant.hpp"

#include <algorithm>
#include <istream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{

using Program  = char;
using Programs = std::string;

struct Spin
{
  size_t moves_num;
};

struct Exchange
{
  size_t a_pos;
  size_t b_pos;
};

struct Partner
{
  Program a_prog;
  Program b_prog;
};

using DanceMove  = boost::variant<Spin, Exchange, Partner>;
using DanceMoves = std::vector<DanceMove>;

struct DanceState
{
  Programs programs;
  size_t spin;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( Spin, moves_num )
BOOST_FUSION_ADAPT_STRUCT( Exchange, a_pos, b_pos )
BOOST_FUSION_ADAPT_STRUCT( Partner, a_prog, b_prog )

namespace
{
DanceMoves parse_moves( std::istream& input )
{
  namespace x3 = boost::spirit::x3;

  const auto spin_parser = x3::rule<struct _spin, Spin>{} = 's' > AoC::x3_size_t_;
  const auto exchange_parser = x3::rule<struct _exchange, Exchange>{} = 'x' > AoC::x3_size_t_ > '/' > AoC::x3_size_t_;
  const auto partner_parser = x3::rule<struct _partner, Partner>{} = 'p' > x3::char_ > '/' > x3::char_;

  const auto move_parser = spin_parser | exchange_parser | partner_parser;

  const auto parser = move_parser % ',';

  DanceMoves moves;
  const bool is_parsed = AoC::x3_parse( input, parser, x3::space, moves );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input dance moves data" );
  }

  return moves;
}

Programs get_initial_programs_order( const size_t programs_num )
{
  return ranges::view::iota( 'a' ) | ranges::view::take( programs_num ) | ranges::to<std::string>;
}

DanceState make_dance_state( std::string programs )
{
  return { std::move( programs ), 0 };
}

DanceState make_dance_state( const size_t programs_num )
{
  return make_dance_state( get_initial_programs_order( programs_num ) );
}

std::string get_result_programs( const DanceState& state )
{
  std::string result = state.programs;
  std::rotate( result.rbegin(), result.rbegin() + boost::numeric_cast<ptrdiff_t>( state.spin ), result.rend() );
  return result;
}

void exec_move( DanceState& state, const Spin spin )
{
  state.spin += spin.moves_num;
  state.spin %= state.programs.size();
}

void exec_move( DanceState& state, const Exchange ex )
{
  const auto get_program_index = [&state]( const size_t n ) { return ( state.programs.size() + n - state.spin ) % state.programs.size(); };

  const size_t from = get_program_index( ex.a_pos );
  const size_t to   = get_program_index( ex.b_pos );

  assert( from < state.programs.size() );
  assert( to < state.programs.size() );

  std::swap( state.programs[ from ], state.programs[ to ] );
}

void exec_move( DanceState& state, const Partner partner )
{
  const auto find_program = [& programs = state.programs]( const Program program ) {
    auto iter = ranges::find( programs, program );
    assert( iter != programs.cend() );
    return iter;
  };

  const auto a_iter = find_program( partner.a_prog );
  const auto b_iter = find_program( partner.b_prog );

  std::iter_swap( a_iter, b_iter );
}

void exec_moves( DanceState& state, const DanceMoves& moves )
{
  auto do_move = AOC_CURRY( exec_move )( std::ref( state ) );
  for ( const auto& move : moves )
  {
    boost::apply_visitor( do_move, move );
  }
}

std::string solve_1_impl( std::istream& input, const size_t programs_num )
{
  const auto moves = parse_moves( input );

  DanceState state = make_dance_state( programs_num );
  exec_moves( state, moves );

  return get_result_programs( state );
}

}  // namespace



namespace AoC_2017
{

namespace problem_16
{

constexpr size_t ProgramsNum = 16;
constexpr size_t DancesNum   = 1'000'000'000;

std::string solve_1( std::istream& input )
{
  return solve_1_impl( input, ProgramsNum );
}

std::string solve_2( std::istream& input )
{
  const auto moves            = parse_moves( input );
  const auto initial_programs = get_initial_programs_order( ProgramsNum );

  DanceState state = make_dance_state( ProgramsNum );
  for ( size_t dance = 1; dance <= DancesNum; ++dance )
  {
    exec_moves( state, moves );

    if ( state.spin == 0 && get_result_programs( state ) == initial_programs )
    {
      dance = DancesNum - ( DancesNum % dance );
    }
  }

  return get_result_programs( state );
}

AOC_REGISTER_PROBLEM( 2017_16, solve_1, solve_2 );

}  // namespace problem_16
}  // namespace AoC_2017



#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>
#  include <sstream>

static void impl_tests()
{
  {
    std::istringstream ss( "s1,x3/4,pe/b" );
    const auto res = parse_moves( ss );
    assert( res.size() == 3 );
    assert( boost::get<Spin>( res[ 0 ] ).moves_num == 1 );
    assert( boost::get<Exchange>( res[ 1 ] ).a_pos == 3 );
    assert( boost::get<Exchange>( res[ 1 ] ).b_pos == 4 );
    assert( boost::get<Partner>( res[ 2 ] ).a_prog == 'e' );
    assert( boost::get<Partner>( res[ 2 ] ).b_prog == 'b' );
  }

  {
    auto s = make_dance_state( "abcde" );
    assert( "abcde" == get_result_programs( s ) );
  }

  {
    auto s = make_dance_state( "abcde" );
    exec_move( s, Spin{ 1 } );
    assert( "eabcd" == get_result_programs( s ) );
  }

  {
    auto s = make_dance_state( "abcde" );
    exec_move( s, Spin{ 5 } );
    assert( "abcde" == get_result_programs( s ) );
  }

  {
    auto s = make_dance_state( "abcde" );
    exec_move( s, Exchange{ 1, 2 } );
    assert( "acbde" == get_result_programs( s ) );
  }

  {
    auto s = make_dance_state( "abcde" );
    exec_move( s, Spin{ 1 } );
    exec_move( s, Exchange{ 3, 4 } );
    assert( "eabdc" == get_result_programs( s ) );
  }

  {
    auto s = make_dance_state( "abcde" );
    exec_move( s, Spin{ 4 } );
    exec_move( s, Exchange{ 3, 4 } );
    assert( "bcdae" == get_result_programs( s ) );
  }

  {
    auto s = make_dance_state( "abcde" );
    exec_move( s, Partner{ 'b', 'd' } );
    assert( "adcbe" == get_result_programs( s ) );
  }

  {
    auto s = make_dance_state( "abcde" );
    exec_move( s, Spin{ 1 } );
    exec_move( s, Exchange{ 3, 4 } );
    exec_move( s, Partner{ 'e', 'b' } );
    assert( "baedc" == get_result_programs( s ) );
  }

  {
    std::istringstream ss( "s1,x3/4,pe/b" );
    assert( "baedc" == solve_1_impl( ss, 5 ) );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
