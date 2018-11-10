#include "AoC/2015/problem_07.h"

#include "AoC/problems_map.h"

#include "range/v3/action/split.hpp"
#include "range/v3/algorithm/all_of.hpp"
#include "range/v3/getlines.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/transform.hpp"

#include "AoC_utils/match.h"

#include "boost/functional/overloaded_function.hpp"

#include <istream>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>
#include <variant>

#include <cctype>
#include <cstdint>

#define THROW_WRONG_INSTRUCTION_EX() throw std::invalid_argument( "wrong instruction" )

namespace
{
using Signal = std::uint16_t;
using Wire   = std::string;
using Source = std::variant<Signal, Wire>;

struct UnaryGate
{
  enum class Op
  {
    Not
  } op;

  Source arg;
};

struct BinaryGate
{
  enum class Op
  {
    Or,
    And,
    LShift,
    RShift
  } op;

  Source arg1;
  Source arg2;
};

using Instruction = std::variant<Source, UnaryGate, BinaryGate>;
using Circuit     = std::map<Wire, Instruction>;

struct CircuitInstruction
{
  Instruction instruction;
  Wire destination;
};

bool is_alpha( const char c )
{
  return std::isalpha( c ) != 0;
}

Signal calc_unary_gate( const UnaryGate::Op op, const Signal arg )
{
  switch ( op )
  {
    case UnaryGate::Op::Not: return Signal( 0xffff & ( ~arg ) );
  }

  THROW_WRONG_INSTRUCTION_EX();
}

Signal calc_binary_gate( const BinaryGate::Op op, const Signal arg1, const Signal arg2 )
{
  switch ( op )
  {
    case BinaryGate::Op::Or: return arg1 | arg2;
    case BinaryGate::Op::And: return arg1 & arg2;
    case BinaryGate::Op::LShift: return Signal( arg1 << arg2 );
    case BinaryGate::Op::RShift: return Signal( arg1 >> arg2 );
  }

  THROW_WRONG_INSTRUCTION_EX();
}

Signal get_signal( const Source& source, Circuit& circuit );
Signal run_instruction( const Instruction& instruction, Circuit& circuit )
{
  const auto instruction_visitor = boost::make_overloaded_function(
      [&circuit]( const Source& source ) { return get_signal( source, circuit ); },
      [&circuit]( const UnaryGate& gate ) { return calc_unary_gate( gate.op, get_signal( gate.arg, circuit ) ); },
      [&circuit]( const BinaryGate& gate ) {
        return calc_binary_gate( gate.op, get_signal( gate.arg1, circuit ), get_signal( gate.arg2, circuit ) );
      } );
  return std::visit( instruction_visitor, instruction );
}

const Instruction& get_instruction( const Wire& wire, const Circuit& circuit )
{
  const auto iter = circuit.find( wire );
  assert( iter != circuit.cend() );

  return iter->second;
}

void update_circuit_wire_signal( Circuit& circuit, const Wire& wire, const Signal new_signal )
{
  circuit[ wire ] = new_signal;
}

Signal get_signal( const Source& source, Circuit& circuit )
{
  const auto source_visitor = boost::make_overloaded_function( []( const Signal& signal ) { return signal; },
                                                               [&circuit]( const Wire& wire ) {
                                                                 const auto& instruction = get_instruction( wire, circuit );
                                                                 const auto result       = run_instruction( instruction, circuit );
                                                                 update_circuit_wire_signal( circuit, wire, result );
                                                                 return result;
                                                               } );

  return std::visit( source_visitor, source );
}

Source parse_source( const std::string& arg )
{
  if ( ranges::all_of( arg, is_alpha ) )
  {
    return Source{ arg };
  }

  const auto signal = std::stoi( arg );
  assert( signal <= std::numeric_limits<Signal>::max() );
  return Source{ signal };
}

Instruction parse_unary_gate( const std::string& gate, const std::string& arg )
{
  if ( gate == "NOT" )
  {
    return UnaryGate{ UnaryGate::Op::Not, parse_source( arg ) };
  }

  THROW_WRONG_INSTRUCTION_EX();
}

Instruction parse_binary_gate( const std::string& gate, const std::string& arg1, const std::string& arg2 )
{
  const auto op = [&gate]() {
    if ( gate == "AND" )
      return BinaryGate::Op::And;
    else if ( gate == "OR" )
      return BinaryGate::Op::Or;
    else if ( gate == "LSHIFT" )
      return BinaryGate::Op::LShift;
    else if ( gate == "RSHIFT" )
      return BinaryGate::Op::RShift;

    THROW_WRONG_INSTRUCTION_EX();
  }();

  return BinaryGate{ op, parse_source( arg1 ), parse_source( arg2 ) };
}

CircuitInstruction parse_instruction( const std::string& line )
{
  const auto tokens = ranges::action::split( line, ' ' );

  using S                = const std::string&;
  const auto instruction = AoC::match_container(
      tokens,
      []( const S& arg1, const S& binary_op, const S& arg2, const S& /*->*/, const S& /*dest*/ ) {
        return parse_binary_gate( binary_op, arg1, arg2 );
      },
      []( const S& unary_op, const S& arg, const S& /*->*/, const S& /*dest*/ ) { return parse_unary_gate( unary_op, arg ); },
      []( const S& source, const S& /*->*/, const S& /*dest*/ ) { return Instruction{ parse_source( source ) }; },
      []( const auto& ) -> Instruction { THROW_WRONG_INSTRUCTION_EX(); } );

  const std::string& destination = tokens.back();

  return { instruction, destination };
}

Circuit parse_circuit( std::istream& input )
{
  auto instructions = ranges::getlines( input ) | ranges::view::transform( &parse_instruction );

  Circuit circuit;
  for ( const auto& [ instruction, destination ] : instructions )
  {
    circuit[ destination ] = instruction;
  };

  return circuit;
}

}  // namespace

namespace AoC_2015
{

namespace problem_07
{

int solve_1( std::istream& input )
{
  Circuit circuit = parse_circuit( input );
  return get_signal( Wire( "a" ), circuit );
}

int solve_2( std::istream& input )
{
  Circuit circuit1 = parse_circuit( input );
  Circuit circuit2 = circuit1;

  const auto circuit1_a_signal = get_signal( Wire( "a" ), circuit1 );

  update_circuit_wire_signal( circuit2, Wire( "b" ), circuit1_a_signal );
  return get_signal( Wire( "a" ), circuit2 );
}

AOC_REGISTER_PROBLEM( 2015_07, solve_1, solve_2 );

}  // namespace problem_07

}  // namespace AoC_2015
