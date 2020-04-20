#include "AoC/2017/problem_18.h"

#include "AoC/problems_map.h"
#include "AoC/utils/curry.h"
#include "AoC/utils/parse.h"

#include "range/v3/range/conversion.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/functional/overloaded_function.hpp"
#include "boost/fusion/adapted/struct.hpp"
#include "boost/safe_numerics/safe_integer.hpp"
#include "boost/variant.hpp"

#include <istream>
#include <limits>
#include <optional>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace
{

using Value            = int64_t;
using Reg              = char;
using Source           = boost::variant<Value, Reg>;
using InstructionIndex = Value;
using JumpOffset       = InstructionIndex;

template <typename _>
struct RegOp
{
  Reg reg;
  Source src;
};

struct Snd
{
  Source src;
};

using Set = RegOp<struct _set>;
using Add = RegOp<struct _add>;
using Mul = RegOp<struct _mul>;
using Mod = RegOp<struct _mod>;

struct Rcv
{
  Source src;
};

struct Jgz
{
  Source clause;
  Source offset;
};

using Instruction  = boost::variant<Snd, Set, Add, Mul, Mod, Rcv, Jgz>;
using Instructions = std::vector<Instruction>;

using Registers = std::unordered_map<Reg, Value>;

struct State
{
  Registers regs;
};

struct SoundState : State
{
  Value sound;
};

using Queue = std::queue<Value>;
struct ParallelState : State
{
  size_t send_counter;
  Queue queue;
  Queue* other_queue;
};



}  // namespace

BOOST_FUSION_ADAPT_STRUCT( Snd, src )
BOOST_FUSION_ADAPT_STRUCT( Set, reg, src )
BOOST_FUSION_ADAPT_STRUCT( Add, reg, src )
BOOST_FUSION_ADAPT_STRUCT( Mul, reg, src )
BOOST_FUSION_ADAPT_STRUCT( Mod, reg, src )
BOOST_FUSION_ADAPT_STRUCT( Rcv, src )
BOOST_FUSION_ADAPT_STRUCT( Jgz, clause, offset )

namespace
{


Instruction parse_instruction( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  const auto value  = x3::int64;
  const auto reg    = x3::char_;
  const auto source = x3::rule<struct _op, Source>{} = value | reg;

  const auto snd = x3::rule<struct _snd, Snd>{} = "snd" > source;
  const auto set = x3::rule<struct _set, Set>{} = "set" > reg > source;
  const auto add = x3::rule<struct _add, Add>{} = "add" > reg > source;
  const auto mul = x3::rule<struct _mul, Mul>{} = "mul" > reg > source;
  const auto mod = x3::rule<struct _mod, Mod>{} = "mod" > reg > source;
  const auto rcv = x3::rule<struct _rcv, Rcv>{} = "rcv" > source;
  const auto jgz = x3::rule<struct _jgz, Jgz>{} = "jgz" > source > source;

  const auto parser = snd | set | add | mul | mod | rcv | jgz;

  Instruction instruction;
  const bool is_parsed = AoC::x3_parse( line, parser, x3::space, instruction );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input instruction data" );
  }

  return instruction;
}

Value read_reg( State& state, const Reg reg )
{
  auto& regs = state.regs;

  auto iter = regs.find( reg );
  if ( iter == regs.cend() )
  {
    iter = regs.emplace( reg, 0 ).first;
  }
  return iter->second;
}

Value get_src_value( State& state, const Source& op )
{
  const auto reader =
      boost::make_overloaded_function( []( const Value v ) { return v; }, [ &state ]( const Reg r ) { return read_reg( state, r ); } );

  return boost::apply_visitor( reader, op );
}

void set_reg_value( State& state, const Reg reg, const Value val )
{
  state.regs[ reg ] = val;
}

template <typename Func, typename RegOp>
JumpOffset exec_regop( State& state, const RegOp& op )
{
  const auto a       = read_reg( state, op.reg );
  const auto b       = get_src_value( state, op.src );
  const auto new_val = Func{}( a, b );
  set_reg_value( state, op.reg, new_val );
  return 1;
}

JumpOffset exec( SoundState& state, const Snd snd )
{
  state.sound = get_src_value( state, snd.src );

  return 1;
}

JumpOffset exec( ParallelState& state, const Snd snd )
{
  state.queue.push( get_src_value( state, snd.src ) );
  ++state.send_counter;

  return 1;
}

JumpOffset exec( State& state, const Set set )
{
  struct Assign
  {
    Value operator()( const Value /*old_value*/, const Value new_value ) const
    {
      return new_value;
    }
  };

  return exec_regop<Assign>( state, set );
}

JumpOffset exec( State& state, const Add add )
{
  return exec_regop<std::plus<Value>>( state, add );
}

JumpOffset exec( State& state, const Mul mul )
{
  return exec_regop<std::multiplies<Value>>( state, mul );
}

JumpOffset exec( State& state, const Mod mul )
{
  return exec_regop<std::modulus<Value>>( state, mul );
}

JumpOffset exec( SoundState& state, const Rcv rcv )
{
  if ( get_src_value( state, rcv.src ) != 0 )
  {
    // stop program by jumping off
    return std::numeric_limits<JumpOffset>::min();
  }

  return 1;
}

JumpOffset exec( ParallelState& state, const Rcv rcv )
{
  if ( state.other_queue->empty() )
  {
    return 0;
  }

  set_reg_value( state, boost::get<Reg>( rcv.src ), state.other_queue->front() );
  state.other_queue->pop();

  return 1;
}

JumpOffset exec( State& state, const Jgz jgz )
{
  if ( get_src_value( state, jgz.clause ) > 0 )
  {
    return get_src_value( state, jgz.offset );
  }

  return 1;
}

template <typename State>
JumpOffset exec_instruction( State& state, const Instruction& instruction )
{
  const auto visitor = AOC_CURRY( exec, std::ref( state ) );
  return boost::apply_visitor( visitor, instruction );
}

template <typename State>
boost::optional<InstructionIndex>
    exec_instruction( State& state, const Instructions& instructions, const boost::optional<InstructionIndex>& index )
{
  if ( !index )
  {
    return {};
  }

  boost::safe_numerics::safe<InstructionIndex> current_instruction_idx = *index;
  if ( current_instruction_idx < 0 || current_instruction_idx >= instructions.size() )
  {
    return {};
  }

  const auto& instruction       = instructions[ current_instruction_idx ];
  const auto instruction_offset = exec_instruction( state, instruction );

  const InstructionIndex next_instruction_idx = current_instruction_idx + instruction_offset;
  return { next_instruction_idx };
}

std::pair<boost::optional<InstructionIndex>, boost::optional<InstructionIndex>>
    exec_instructions( ParallelState& s1,
                       ParallelState& s2,
                       const Instructions& instructions,
                       boost::optional<InstructionIndex> index_1,
                       boost::optional<InstructionIndex> index_2 )
{
  const auto next_index_1 = exec_instruction( s1, instructions, index_1 );
  const auto next_index_2 = exec_instruction( s2, instructions, index_2 );

  const bool deadlock = next_index_1 == index_1 && next_index_2 == index_2;
  if ( deadlock )
  {
    return { {}, {} };
  }

  return { next_index_1, next_index_2 };
}

Instructions read_instructions( std::istream& input )
{
  return ranges::getlines( input ) | ranges::views::transform( &parse_instruction ) | ranges::to_vector;
}

}  // namespace

namespace AoC_2017
{

namespace problem_18
{

std::string solve_1( std::istream& input )
{
  const auto instructions = read_instructions( input );
  SoundState state{};

  boost::optional<InstructionIndex> current_instruction = 0;
  do
  {
    current_instruction = exec_instruction( state, instructions, current_instruction );
  } while ( current_instruction );

  return std::to_string( state.sound );
}

size_t solve_2( std::istream& input )
{
  const auto instructions = read_instructions( input );

  ParallelState state_1{};
  state_1.regs.emplace( 'p', 0 );

  ParallelState state_2{};
  state_2.regs.emplace( 'p', 1 );

  state_1.other_queue = &state_2.queue;
  state_2.other_queue = &state_1.queue;

  boost::optional<InstructionIndex> instruction_index_1 = 0;
  boost::optional<InstructionIndex> instruction_index_2 = 0;
  do
  {
    std::tie( instruction_index_1, instruction_index_2 ) =
        exec_instructions( state_1, state_2, instructions, instruction_index_1, instruction_index_2 );
  } while ( instruction_index_1 || instruction_index_2 );

  return state_2.send_counter;
}

AOC_REGISTER_PROBLEM( 2017_18, solve_1, solve_2 );

}  // namespace problem_18
}  // namespace AoC_2017

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  assert( 10 == boost::get<Value>( boost::get<Snd>( parse_instruction( "snd 10" ) ).src ) );
  assert( 'a' == boost::get<Reg>( boost::get<Snd>( parse_instruction( "snd a" ) ).src ) );

  assert( 'a' == boost::get<Set>( parse_instruction( "set a 1" ) ).reg );
  assert( 'b' == boost::get<Add>( parse_instruction( "add b 2" ) ).reg );
  assert( 'c' == boost::get<Mul>( parse_instruction( "mul c a" ) ).reg );
  assert( 'd' == boost::get<Mod>( parse_instruction( "mod d 5" ) ).reg );
  assert( 'e' == boost::get<Reg>( boost::get<Rcv>( parse_instruction( "rcv e" ) ).src ) );
  assert( 'f' == boost::get<Reg>( boost::get<Jgz>( parse_instruction( "jgz f -1" ) ).clause ) );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
