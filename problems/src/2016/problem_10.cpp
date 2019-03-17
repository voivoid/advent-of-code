#include "AoC/2016/problem_10.h"

#include "AoC/problems_map.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/find_if.hpp"
#include "range/v3/getlines.hpp"
#include "range/v3/view/map.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/numeric/accumulate.hpp"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/hof/lift.hpp"
#include "boost/hof/partial.hpp"
#include "boost/spirit/home/x3.hpp"
#include "boost/variant.hpp"

#include <istream>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>

namespace
{

using Value = size_t;
using Id    = size_t;

struct InputInstruction
{
  Value value;
  Id bot_num;
};

enum class Destination
{
  Output,
  Bot
};

struct OutputInstruction
{
  Id from;

  Destination low_val_destination;
  Id low_val_dest_id;

  Destination high_val_destination;
  Id high_val_dest_id;
};

struct Bot
{
  Id num;
  std::optional<OutputInstruction> instr;
  std::optional<Value> low;
  std::optional<Value> high;
  std::optional<Value> output;
};

using Bots = std::map<Id, Bot>;

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( InputInstruction, value, bot_num )
BOOST_FUSION_ADAPT_STRUCT( OutputInstruction, from, low_val_destination, low_val_dest_id, high_val_destination, high_val_dest_id )

namespace
{

using Instruction = boost::variant<InputInstruction, OutputInstruction>;

Instruction parse_instruction( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  const auto input_parser = "value" > AoC::x3_size_t_ > "goes" > "to" > "bot" > AoC::x3_size_t_;

  x3::symbols<Destination> destination_parser;
  destination_parser.add( "bot", Destination::Bot )( "output", Destination::Output );

  const auto output_parser = "bot" > AoC::x3_size_t_ > "gives" > "low" > "to" > destination_parser > AoC::x3_size_t_ > "and" > "high" >
                             "to" > destination_parser > AoC::x3_size_t_;
  const auto parser = input_parser | output_parser;

  Instruction instruction;
  const bool is_parsed = AoC::x3_parse( line.cbegin(), line.cend(), parser, x3::space, instruction );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input data" );
  }

  return instruction;
}

void give_value( const Value value, Bot& bot )
{
  assert( !( bot.low && bot.high ) );

  if ( !bot.low )
  {
    bot.low = value;
    return;
  }

  bot.high = value;
  if ( bot.low > bot.high )
  {
    std::swap( bot.low, bot.high );
  }
}

Bot& get_bot( Bots& bots, const Id bot_num )
{
  auto bot_iter = bots.find( bot_num );
  if ( bot_iter != bots.cend() )
  {
    return bot_iter->second;
  }

  return bots.emplace( bot_num, Bot{ bot_num, {}, {}, {}, {} } ).first->second;
}

bool is_ready( const Bot& bot )
{
  return bot.low && bot.high && bot.instr;
}

void try_to_run_instruction( Bots& bots, const Bot& bot )
{
  if ( !is_ready( bot ) )
  {
    return;
  }

  {
    auto& other_bot = get_bot( bots, bot.instr->low_val_dest_id );
    if ( bot.instr->low_val_destination == Destination::Bot )
    {
      give_value( *bot.low, other_bot );
      try_to_run_instruction( bots, other_bot );
    }
    else
    {
      assert( !other_bot.output );
      other_bot.output = *bot.low;
    }
  }

  {
    auto& other_bot = get_bot( bots, bot.instr->high_val_dest_id );
    if ( bot.instr->high_val_destination == Destination::Bot )
    {

      give_value( *bot.high, other_bot );
      try_to_run_instruction( bots, other_bot );
    }
    else
    {
      assert( !other_bot.output );
      other_bot.output = *bot.high;
    }
  }
}

void load_instruction( Bots& bots, const InputInstruction instruction )
{
  auto& bot = get_bot( bots, instruction.bot_num );
  give_value( instruction.value, bot );

  try_to_run_instruction( bots, bot );
}

void load_instruction( Bots& bots, const OutputInstruction instruction )
{
  Bot& bot  = get_bot( bots, instruction.from );
  bot.instr = instruction;

  try_to_run_instruction( bots, bot );
}

Bots execute_instructions( std::istream& input )
{
  auto instructions = ranges::getlines( input ) | ranges::view::transform( &parse_instruction );

  Bots bots_map;
  const auto instruction_loader = boost::hof::partial( BOOST_HOF_LIFT( load_instruction ) )( std::ref( bots_map ) );
  for ( const auto& instruction : instructions )
  {
    boost::apply_visitor( instruction_loader, instruction );
  }

  return bots_map;
}

int solve_1_impl( std::istream& input, const Value low_val_to_find, const Value high_val_to_find )
{
  const auto bots_map = execute_instructions( input );

  const auto bots     = bots_map | ranges::view::values;
  const auto bot_iter = ranges::find_if( bots, [low_val_to_find, high_val_to_find]( const Bot& bot ) {
    return bot.low && bot.high && bot.low == low_val_to_find && bot.high == high_val_to_find;
  } );

  assert( bot_iter != bots.end() );
  return static_cast<int>( bot_iter->num );
}

}  // namespace

namespace AoC_2016
{

namespace problem_10
{

int solve_1( std::istream& input )
{
  return solve_1_impl( input, 17, 61 );
}

int solve_2( std::istream& input )
{
  const auto bots_map = execute_instructions( input );

  const auto outputs = std::initializer_list<Id>{ 0, 1, 2 };

  const auto output_vals = outputs | ranges::view::transform( [&bots_map]( const Id n ) {
        const auto iter = bots_map.find( n );
        assert( iter != bots_map.cend() );

        const auto& output = iter->second.output;
        assert( output );

        return *output;
  });

  return static_cast<int>( ranges::accumulate( output_vals, Value{1}, std::multiplies<Value>{} ) );
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
  {
    auto i = boost::get<InputInstruction>( parse_instruction( "value 5 goes to bot 2" ) );
    assert( 5 == i.value );
    assert( 2 == i.bot_num );
  }

  {
    auto i = boost::get<OutputInstruction>( parse_instruction( "bot 2 gives low to output 1 and high to bot 0" ) );
    assert( i.from == 2 );
    assert( i.low_val_dest_id == 1 );
    assert( i.low_val_destination == Destination::Output );
    assert( i.high_val_dest_id == 0 );
    assert( i.high_val_destination == Destination::Bot );
  }

  {
    auto i = boost::get<OutputInstruction>( parse_instruction( "bot 2 gives low to bot 1 and high to output 0" ) );
    assert( i.from == 2 );
    assert( i.low_val_dest_id == 1 );
    assert( i.low_val_destination == Destination::Bot );
    assert( i.high_val_dest_id == 0 );
    assert( i.high_val_destination == Destination::Output );
  }


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
