#include "AoC/2017/problem_08.h"

#include "AoC/problems_map.h"
#include "AoC/utils/fusion.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/max.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/map.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/fusion/adapted/struct.hpp"


#include <functional>
#include <istream>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace
{

using Register     = std::string;
using Value        = int;
using ValueChecker = std::function<bool( Value )>;
struct Instruction
{
  Register reg_to_write;
  Value diff_to_apply;
  Register reg_to_check;
  ValueChecker value_checker;
};

struct Cpu
{
  std::unordered_map<Register, Value> registers;
  Value max_register_value;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( Instruction, reg_to_write, diff_to_apply, reg_to_check, value_checker )

namespace
{
Instruction parse_instruction( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  x3::symbols<std::function<bool( Value, Value )>> conditions;
  conditions.add( ">", std::greater<Value>{} )( "<", std::less<Value>{} )( ">=", std::greater_equal<Value>{} )(
      "<=", std::less_equal<Value>{} )( "==", std::equal_to<Value>{} )( "!=", std::not_equal_to<Value>{} );

  const auto reg_parser   = x3::lexeme[ +x3::alpha ];
  const auto value_parser = x3::int_;
  const auto inc_parser   = ( "inc" > value_parser );
  const auto dec_parser   = x3::rule<struct _diff, Value>{} =
      ( "dec" > value_parser )[ ( []( auto& ctx ) { x3::_val( ctx ) = -x3::_attr( ctx ); } ) ];
  const auto diff_parser          = inc_parser | dec_parser;
  const auto value_checker_parser = x3::rule<struct _cond, ValueChecker>{} = ( conditions > value_parser )[ ( []( auto& ctx ) {
    auto [ cond, val ] = AoC::fusion_to_std_tuple( x3::_attr( ctx ) );
    x3::_val( ctx )    = std::bind( cond, std::placeholders::_1, val );
  } ) ];

  /* b inc 5 if a > 1 */
  const auto parser = reg_parser > diff_parser > "if" > reg_parser > value_checker_parser;

  Instruction instruction;
  const bool is_parsed = AoC::x3_parse( line, parser, x3::space, instruction );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input instruction data" );
  }

  return instruction;
}

void run_instruction( const Instruction& instruction, Cpu& cpu )
{
  if ( instruction.value_checker( cpu.registers[ instruction.reg_to_check ] ) )
  {
    auto& reg_value_to_update = cpu.registers[ instruction.reg_to_write ];
    reg_value_to_update += instruction.diff_to_apply;
    cpu.max_register_value = std::max( cpu.max_register_value, reg_value_to_update );
  }
}

Cpu run_instructions( std::istream& input )
{
  Cpu cpu{};

  auto instructions = ranges::getlines( input ) | ranges::view::transform( &parse_instruction );
  for ( const auto& instruction : instructions )
  {
    run_instruction( instruction, cpu );
  }

  return cpu;
}

}  // namespace

namespace AoC_2017
{

namespace problem_08
{

int solve_1( std::istream& input )
{
  Cpu cpu = run_instructions( input );
  return ranges::max( cpu.registers | ranges::view::values );
}

int solve_2( std::istream& input )
{
  Cpu cpu = run_instructions( input );
  return cpu.max_register_value;
}

AOC_REGISTER_PROBLEM( 2017_08, solve_1, solve_2 );

}  // namespace problem_08

}  // namespace AoC_2017

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  {
    const auto i = parse_instruction( "b inc 5 if a > 1" );
    assert( i.reg_to_write == "b" );
    assert( i.diff_to_apply == 5 );
    assert( i.reg_to_check == "a" );
    assert( i.value_checker( 1 ) == false );
    assert( i.value_checker( 2 ) == true );
  }

  {
    const auto i = parse_instruction( "c dec -10 if a >= 1" );
    assert( i.reg_to_write == "c" );
    assert( i.diff_to_apply == 10 );
    assert( i.reg_to_check == "a" );
    assert( i.value_checker( 1 ) == true );
    assert( i.value_checker( 0 ) == false );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
