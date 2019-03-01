#include "AoC/2015/problem_23.h"

#include "AoC/problems_map.h"
#include "AoC/utils/parse.h"

#include "range/v3/getlines.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/to_container.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/hof.hpp"
#include "boost/spirit/home/x3.hpp"
#include "boost/variant.hpp"

#include <functional>
#include <istream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
enum class Register
{
  A,
  B
};

namespace Instructions
{
struct Hlf
{
  Register reg;
};

struct Tpl
{
  Register reg;
};

struct Inc
{
  Register reg;
};

struct Jmp
{
  int offset;
};

struct Jie
{
  Register reg;
  int offset;
};

struct Jio
{
  Register reg;
  int offset;
};
}  // namespace Instructions

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( Instructions::Hlf, reg )
BOOST_FUSION_ADAPT_STRUCT( Instructions::Tpl, reg )
BOOST_FUSION_ADAPT_STRUCT( Instructions::Inc, reg )
BOOST_FUSION_ADAPT_STRUCT( Instructions::Jmp, offset )
BOOST_FUSION_ADAPT_STRUCT( Instructions::Jie, reg, offset )
BOOST_FUSION_ADAPT_STRUCT( Instructions::Jio, reg, offset )

namespace
{

using Instruction =
    boost::variant<Instructions::Hlf, Instructions::Tpl, Instructions::Inc, Instructions::Jmp, Instructions::Jie, Instructions::Jio>;

Instruction parse_instruction( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  x3::symbols<Register> register_parser;
  register_parser.add( "a", Register::A )( "b", Register::B );

  const auto hlf_parser = x3::rule<struct _hlf, Instructions::Hlf>{} = "hlf" > register_parser;
  const auto tpl_parser = x3::rule<struct _tpl, Instructions::Tpl>{} = "tpl" > register_parser;
  const auto inc_parser = x3::rule<struct _inc, Instructions::Inc>{} = "inc" > register_parser;
  const auto jmp_parser = x3::rule<struct _jmp, Instructions::Jmp>{} = "jmp" > x3::int_;
  const auto jie_parser = x3::rule<struct _jie, Instructions::Jie>{} = "jie" > register_parser > ',' > x3::int_;
  const auto jio_parser = x3::rule<struct _jio, Instructions::Jio>{} = "jio" > register_parser > ',' > x3::int_;

  const auto parser = hlf_parser | tpl_parser | inc_parser | jmp_parser | jie_parser | jio_parser;

  Instruction instruction;
  const bool is_parsed = AoC::x3_parse( line.cbegin(), line.cend(), parser, x3::space, instruction );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input instruction data" );
  }

  return instruction;
}

struct State
{
  std::map<Register, size_t> registers;
};

int run_instruction( const Instructions::Hlf& i, State& state )
{
  state.registers[ i.reg ] /= 2;
  return +1;
}

int run_instruction( const Instructions::Tpl& i, State& state )
{
  state.registers[ i.reg ] *= 3;
  return +1;
}

int run_instruction( const Instructions::Inc& i, State& state )
{
  state.registers[ i.reg ] += 1;
  return +1;
}

int run_instruction( const Instructions::Jmp& i, State& )
{
  return i.offset;
}

int run_instruction( const Instructions::Jie& i, State& s )
{
  const bool is_even = s.registers[ i.reg ] % 2 == 0;
  return is_even ? i.offset : +1;
}

int run_instruction( const Instructions::Jio& i, State& s )
{
  const bool is_one = s.registers[ i.reg ] == 1;
  return is_one ? i.offset : +1;
}

void run_instructions( const std::vector<Instruction>& instructions, State& state, size_t instruction_index )
{
  if ( instruction_index >= instructions.size() )
  {
    return;
  }

  const auto& instruction = instructions[ instruction_index ];
  const auto visitor      = std::bind( BOOST_HOF_LIFT( run_instruction ), std::placeholders::_1, std::ref( state ) );
  const int index_offset  = boost::apply_visitor( visitor, instruction );

  const int next_index = index_offset + static_cast<int>( instruction_index );
  if ( next_index < 0 )
  {
    return;
  }

  run_instructions( instructions, state, static_cast<size_t>( next_index ) );
}

int solve( std::istream& input, State& state )
{
  const auto instructions = ranges::getlines( input ) | ranges::view::transform( &parse_instruction ) | ranges::to_vector;
  run_instructions( instructions, state, 0 );
  return static_cast<int>( state.registers[ Register::B ] );
}

}  // namespace

namespace AoC_2015
{

namespace problem_23
{

int solve_1( std::istream& input )
{
  State state;
  state.registers[ Register::A ] = 0;
  state.registers[ Register::B ] = 0;

  return solve( input, state );
}

int solve_2( std::istream& input )
{
  State state;
  state.registers[ Register::A ] = 1;
  state.registers[ Register::B ] = 0;

  return solve( input, state );
}

AOC_REGISTER_PROBLEM( 2015_23, solve_1, solve_2 );

}  // namespace problem_23

}  // namespace AoC_2015


#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  const auto instruction = parse_instruction( "tpl b" );

  auto visitor = boost::hof::first_of( []( Instructions::Tpl i ) { assert( i.reg == Register::B ); }, []( auto ) { assert( false ); } );

  boost::apply_visitor( visitor, instruction );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
