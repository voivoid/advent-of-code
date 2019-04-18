#include "AoC/2015/problem_07.h"

#include "AoC/problems_map.h"
#include "AoC/utils/fusion.h"
#include "AoC/utils/parse.h"

#include "range/v3/action/split.hpp"
#include "range/v3/algorithm/all_of.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/transform.hpp"

#include "AoC/utils/match.h"

#include "boost/functional/overloaded_function.hpp"
#include "boost/fusion/adapted/struct.hpp"
#include "boost/fusion/container/vector.hpp"

#include "boost/variant.hpp"

#include <istream>
#include <limits>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include <cctype>
#include <cstdint>

#define THROW_WRONG_INSTRUCTION_EX() throw std::invalid_argument( "wrong instruction" )

namespace
{
using Signal = std::uint16_t;
using Wire   = std::string;
using Source = boost::variant<Signal, Wire>;

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

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( UnaryGate, op, arg )
BOOST_FUSION_ADAPT_STRUCT( BinaryGate, arg1, op, arg2 )

namespace
{

using Instruction = boost::variant<Source, UnaryGate, BinaryGate>;
using Circuit     = std::unordered_map<Wire, Instruction>;

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
  return boost::apply_visitor( instruction_visitor, instruction );
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

  return boost::apply_visitor( source_visitor, source );
}

std::pair<Wire, Instruction> parse_instruction( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  x3::symbols<UnaryGate::Op> unary_op;
  unary_op.add( "NOT", UnaryGate::Op::Not );

  x3::symbols<BinaryGate::Op> binary_op;
  binary_op.add( "AND", BinaryGate::Op::And )( "OR", BinaryGate::Op::Or )( "LSHIFT", BinaryGate::Op::LShift )( "RSHIFT",
                                                                                                               BinaryGate::Op::RShift );

  const auto signal      = x3::uint16;
  const auto wire        = +x3::lower;
  const auto source      = signal | wire;
  const auto unary_gate  = unary_op > source;
  const auto binary_gate = source >> binary_op > source;
  const auto instruction = unary_gate | binary_gate | source;

  const auto parser = instruction > "->" > wire;

  boost::fusion::vector<Instruction, Wire> result;
  const bool is_parsed = AoC::x3_parse( line.cbegin(), line.cend(), parser, x3::space, result );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input instruction data" );
  }

  auto [ instr, dest ] = AoC::fusion_to_std_tuple( result );

  return { std::move( dest ), std::move( instr ) };
}

Circuit parse_circuit( std::istream& input )
{
  return ranges::getlines( input ) | ranges::view::transform( &parse_instruction );
}

}  // namespace

namespace AoC_2015
{

namespace problem_07
{

size_t solve_1( std::istream& input )
{
  Circuit circuit = parse_circuit( input );
  return get_signal( Wire( "a" ), circuit );
}

size_t solve_2( std::istream& input )
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

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  {
    const auto i = parse_instruction( "123 -> x" );
    assert( i.first == "x" );
    const auto source = boost::get<Source>( i.second );
    const auto signal = boost::get<Signal>( source );
    assert( signal == 123 );
  }
  {
    const auto i = parse_instruction( "NOT x -> h" );
    assert( i.first == "h" );
    const auto gate = boost::get<UnaryGate>( i.second );
    assert( gate.op == UnaryGate::Op::Not );
    const auto wire = boost::get<Wire>( gate.arg );
    assert( wire == "x" );
  }
  {
    const auto i = parse_instruction( "x AND y -> d" );
    assert( i.first == "d" );
    const auto gate = boost::get<BinaryGate>( i.second );
    assert( gate.op == BinaryGate::Op::And );
    const auto wire1 = boost::get<Wire>( gate.arg1 );
    assert( wire1 == "x" );
    const auto wire2 = boost::get<Wire>( gate.arg2 );
    assert( wire2 == "y" );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
