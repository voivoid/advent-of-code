#include "AoC/2016/problem_10.h"

#include "AoC/problems_map.h"
#include "AoC/utils/fusion.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/find_if.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/map.hpp"
#include "range/v3/view/transform.hpp"



#include <functional>
#include <istream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace
{

using Value = size_t;
using Id    = size_t;

struct SlotMaps;
using Instruction = std::function<void( SlotMaps& )>;

struct Bot
{
  Id id;
  std::optional<Instruction> instr;
  std::optional<Value> low_val;
  std::optional<Value> high_val;
};

struct Output
{
  Id id;
  std::optional<Value> value;
};

template <typename T>
using SlotsMap = std::unordered_map<Id, T>;
using Bots     = SlotsMap<Bot>;
using Outputs  = SlotsMap<Output>;

struct SlotMaps
{
  Bots bots;
  Outputs outputs;
};

enum class Destination
{
  Bot,
  Output
};

void set_value( const Value value, Bot& bot )
{
  assert( !( bot.low_val && bot.high_val ) );

  if ( !bot.low_val )
  {
    bot.low_val = value;
    return;
  }

  bot.high_val = value;
  if ( bot.low_val > bot.high_val )
  {
    std::swap( bot.low_val, bot.high_val );
  }
}

void set_value( const Value value, Output& output )
{
  assert( !output.value );
  output.value = value;
}

template <typename T>
T& get_slot( SlotsMap<T>& slots, const Id id )
{
  auto slot_iter = slots.find( id );
  if ( slot_iter != slots.cend() )
  {
    return slot_iter->second;
  }

  T slot{};
  slot.id = id;
  return slots.emplace( id, std::move( slot ) ).first->second;
}

const auto get_bot    = get_slot<Bot>;
const auto get_output = get_slot<Output>;

bool is_ready( const Bot& bot )
{
  return bot.low_val && bot.high_val && bot.instr;
}

bool try_to_run_instruction( SlotMaps& slots, Bot& bot )
{
  if ( !is_ready( bot ) )
  {
    return false;
  }

  assert( bot.instr );
  ( *bot.instr )( slots );
  return true;
}

void give_value( const Value value, SlotMaps& slots, const Destination dest, const Id dest_id )
{

  switch ( dest )
  {
    case Destination::Bot:
    {
      auto& other_bot = get_bot( slots.bots, dest_id );
      set_value( value, other_bot );
      try_to_run_instruction( slots, other_bot );
      return;
    }

    case Destination::Output:
    {
      set_value( value, get_output( slots.outputs, dest_id ) );
      return;
    }
  }

  throw std::runtime_error( "Unexpected destination" );
}

void run_bot_instruction( SlotMaps& slots,
                          Bot& bot,
                          const Destination low_dest_type,
                          const Id low_dest_id,
                          const Destination high_dest_type,
                          const Id high_dest_id )
{
  assert( is_ready( bot ) );
  assert( bot.low_val );
  assert( bot.high_val );

  give_value( *bot.low_val, slots, low_dest_type, low_dest_id );
  give_value( *bot.high_val, slots, high_dest_type, high_dest_id );
}

void process_input_instruction( SlotMaps& slots, const Value value, const Id bot_id )
{
  auto& bot = get_bot( slots.bots, bot_id );
  set_value( value, bot );

  try_to_run_instruction( slots, bot );
}

void process_output_instruction( SlotMaps& slots,
                                 const Id bot_id,
                                 const Destination low_dest_type,
                                 const Id low_dest_id,
                                 const Destination high_dest_type,
                                 const Id high_dest_id )
{
  auto& bot = get_bot( slots.bots, bot_id );

  bot.instr =
      std::bind( &run_bot_instruction, std::placeholders::_1, std::ref( bot ), low_dest_type, low_dest_id, high_dest_type, high_dest_id );

  try_to_run_instruction( slots, bot );
}

Instruction parse_instruction( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  const auto make_input_instruction = []( auto& ctx ) {
    const auto [ val, bot_id ] = AoC::fusion_to_std_tuple( x3::_attr( ctx ) );
    x3::_val( ctx )            = std::bind( &process_input_instruction, std::placeholders::_1, val, bot_id );
  };

  const auto make_output_instruction = []( auto& ctx ) {
    const auto [ bot_id, low_dest_type, low_dest_id, high_dest_type, high_dest_id ] = AoC::fusion_to_std_tuple( x3::_attr( ctx ) );

    x3::_val( ctx ) =
        std::bind( &process_output_instruction, std::placeholders::_1, bot_id, low_dest_type, low_dest_id, high_dest_type, high_dest_id );
  };

  const auto input_parser = x3::rule<struct _input, Instruction>{} =
      ( "value" > AoC::x3_size_t_ > "goes" > "to" > "bot" > AoC::x3_size_t_ )[ make_input_instruction ];

  x3::symbols<Destination> destination_parser;
  destination_parser.add( "bot", Destination::Bot )( "output", Destination::Output );

  const auto output_parser = x3::rule<struct _output, Instruction>{} =
      ( "bot" > AoC::x3_size_t_ > "gives" > "low" > "to" > destination_parser > AoC::x3_size_t_ > "and" > "high" > "to" >
        destination_parser > AoC::x3_size_t_ )[ make_output_instruction ];

  const auto parser = input_parser | output_parser;

  Instruction instruction;
  const bool is_parsed = AoC::x3_parse( line, parser, x3::space, instruction );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input data" );
  }

  return instruction;
}

SlotMaps execute_instructions( std::istream& input )
{
  auto instructions = ranges::getlines( input ) | ranges::view::transform( &parse_instruction );

  SlotMaps slots;
  for ( const auto& run_instruction : instructions )
  {
    run_instruction( slots );
  }

  return slots;
}

size_t solve_1_impl( std::istream& input, const Value low_val_to_find, const Value high_val_to_find )
{
  const SlotMaps slots = execute_instructions( input );

  const auto bots     = slots.bots | ranges::view::values;
  const auto bot_iter = ranges::find_if( bots, [low_val_to_find, high_val_to_find]( const Bot& bot ) {
    return bot.low_val && bot.high_val && bot.low_val == low_val_to_find && bot.high_val == high_val_to_find;
  } );

  assert( bot_iter != bots.end() );
  return bot_iter->id;
}

}  // namespace

namespace AoC_2016
{

namespace problem_10
{

size_t solve_1( std::istream& input )
{
  return solve_1_impl( input, 17, 61 );
}

size_t solve_2( std::istream& input )
{
  auto slots = execute_instructions( input );

  const auto get_output_val = [& outputs = slots.outputs]( const Id n ) {
    const auto value = get_output( outputs, n ).value;
    assert( value );
    return *value;
  };

  const auto output_ids  = { 0, 1, 2 };
  const auto output_vals = output_ids | ranges::view::transform( get_output_val );

  return ranges::accumulate( output_vals, Value{ 1 }, std::multiplies<Value>{} );
}

AOC_REGISTER_PROBLEM( 2016_10, solve_1, solve_2 );

}  // namespace problem_10

}  // namespace AoC_2016


#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>
#  include <sstream>

static void impl_tests()
{
  SlotMaps map;
  parse_instruction( "value 5 goes to bot 2" )( map );
  parse_instruction( "value 1 goes to bot 2" )( map );
  assert( map.bots[ 2 ].low_val );
  assert( 1 == *map.bots[ 2 ].low_val );
  assert( map.bots[ 2 ].high_val );
  assert( 5 == *map.bots[ 2 ].high_val );

  parse_instruction( "bot 2 gives low to output 1 and high to bot 0" )( map );
  assert( map.outputs[ 1 ].value );
  assert( 1 == *map.outputs[ 1 ].value );
  assert( map.bots[ 0 ].low_val );
  assert( 5 == *map.bots[ 0 ].low_val );

  const auto input = R"(value 5 goes to bot 2
                        bot 2 gives low to bot 1 and high to bot 0
                        value 3 goes to bot 1
                        bot 1 gives low to output 1 and high to bot 0
                        bot 0 gives low to output 2 and high to output 0
                        value 2 goes to bot 2)";
  {
    std::istringstream ss( input );
    assert( 2 == solve_1_impl( ss, 2, 5 ) );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
