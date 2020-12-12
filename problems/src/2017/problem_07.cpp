#include "AoC/2017/problem_07.h"

#include "AoC/problems_map.h"
#include "AoC/utils/assert.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/find_if.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/numeric/conversion/cast.hpp"

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning( disable: 4715 )
#endif

#include "boost/safe_numerics/safe_integer.hpp"

#ifdef _MSC_VER
  #pragma warning( pop )
#endif


#include <istream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace
{
using Name   = std::string;
using Weight = size_t;

struct Program
{
  Name name;
  Weight weight;

  std::vector<Name> subprograms;
};

using ProgamMap      = std::unordered_map<Name, Program>;
using TopToBottomMap = std::unordered_map<Name, Name>;

struct WeightMismatch : std::exception
{
  size_t correct_weight;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( Program, name, weight, subprograms )

namespace
{
Program parse_program( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  const auto name_parser        = +x3::alpha;
  const auto subprograms_parser = "->" > ( name_parser % ',' );
  const auto program_parser     = name_parser > '(' > AoC::x3_size_t_ > ')';

  const auto parser = program_parser > -subprograms_parser;

  Program prog;
  const bool is_parsed = AoC::x3_parse( line, parser, x3::space, prog );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input program data" );
  }

  return prog;
}

ProgamMap get_program_map( std::istream& input )
{
  ProgamMap program_map;
  auto programs = ranges::getlines( input ) | ranges::views::transform( &parse_program );
  for ( auto&& program : programs )
  {
    auto name = program.name;
    program_map.emplace( std::move( name ), std::move( program ) );
  }

  return program_map;
}

TopToBottomMap get_top_to_bottom_map( const ProgamMap& program_map )
{
  TopToBottomMap top_to_bottom_map;

  for ( const auto& program : program_map )
  {
    const auto& [ _, prg ] = program;
    for ( const auto& top_prg_name : prg.subprograms )
    {
      top_to_bottom_map[ top_prg_name ] = prg.name;
    }
  }

  return top_to_bottom_map;
}

Name get_bottom_name( const TopToBottomMap& top_to_bottom_map )
{
  auto iter = top_to_bottom_map.begin();
  while ( true )
  {
    auto next_iter = top_to_bottom_map.find( iter->second );
    if ( next_iter == top_to_bottom_map.cend() )
    {
      return iter->second;
    }
    iter = next_iter;
  }
}

struct Mismatch
{
  size_t subprog_index;
  int diff;
};

template <typename Range>
boost::optional<Mismatch> find_mismatch( const Range& range )
{
  if ( range.size() <= 1 )
  {
    return {};
  }

  if ( range.size() == 2 )
  {
    assert( range[ 0 ] == range[ 1 ] );
    return {};
  }

  const boost::safe_numerics::safe<int> a = range[ 0 ];
  const boost::safe_numerics::safe<int> b = range[ 1 ];
  const boost::safe_numerics::safe<int> c = range[ 2 ];

  if ( a == b )
  {
    auto iter = ranges::find_if( range, [ a ]( const Weight w ) { return w != a; } );

    if ( iter == range.end() )
    {
      return {};
    }

    return { Mismatch{ boost::numeric_cast<size_t>( iter - range.begin() ), a - boost::numeric_cast<int>( *iter ) } };
  }

  if ( a != b && b == c )
  {
    return { Mismatch{ 0, b - a } };
  }

  if ( a != b && b != c )
  {
    return { Mismatch{ 1, a - b } };
  }

  AOC_ASSERT_FALSE();
}

const Program& get_program( const std::string& name, const ProgamMap& program_map )
{
  auto program_iter = program_map.find( name );
  assert( program_iter != program_map.cend() );
  return program_iter->second;
}

Weight get_weights_sum( const std::string& name, const ProgamMap& program_map )
{
  const auto& prog        = get_program( name, program_map );
  const auto& subprograms = prog.subprograms;

  const auto subprogram_sums = subprograms | ranges::views::transform( [ &program_map ]( const Name& subprog_name ) {
                                 return get_weights_sum( subprog_name, program_map );
                               } );

  if ( const auto mismatch = find_mismatch( subprogram_sums ); mismatch )
  {
    const auto mismatched_weight = get_program( subprograms[ mismatch->subprog_index ], program_map ).weight;
    const auto correct_weight    = boost::numeric_cast<int>( mismatched_weight ) + mismatch->diff;

    WeightMismatch ex;
    ex.correct_weight = boost::numeric_cast<size_t>( correct_weight );
    throw ex;
  }

  return prog.weight + ranges::accumulate( subprogram_sums, Weight{ 0 } );
}

Weight find_weights_mismatch( const std::string& name, const ProgamMap& program_map )
{
  try
  {
    get_weights_sum( name, program_map );
  }
  catch ( const WeightMismatch& ex )
  {
    return ex.correct_weight;
  }

  AOC_ASSERT_FALSE();
}

}  // namespace

namespace AoC_2017
{

namespace problem_07
{

std::string solve_1( std::istream& input )
{
  const auto program_map       = get_program_map( input );
  const auto top_to_bottom_map = get_top_to_bottom_map( program_map );

  return get_bottom_name( top_to_bottom_map );
}

size_t solve_2( std::istream& input )
{
  const auto program_map       = get_program_map( input );
  const auto top_to_bottom_map = get_top_to_bottom_map( program_map );

  const auto bottom_name = get_bottom_name( top_to_bottom_map );

  return find_weights_mismatch( bottom_name, program_map );
}

AOC_REGISTER_PROBLEM( 2017_07, solve_1, solve_2 );

}  // namespace problem_07

}  // namespace AoC_2017

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  {
    const auto p = parse_program( "pbga (66)" );
    assert( p.name == "pbga" );
    assert( p.weight == 66 );
    assert( p.subprograms.empty() );
  }
  {
    const auto p = parse_program( "fwft (72) -> ktlj, cntj, xhth" );
    assert( p.name == "fwft" );
    assert( p.weight == 72 );

    const auto& sp = p.subprograms;
    assert( sp.size() == 3 );
    assert( sp[ 0 ] == "ktlj" );
    assert( sp[ 1 ] == "cntj" );
    assert( sp[ 2 ] == "xhth" );
  }


  using Ws = std::vector<Weight>;
  assert( !find_mismatch( Ws{ 10 } ) );
  assert( !find_mismatch( Ws{ 10, 10 } ) );
  assert( !find_mismatch( Ws{ 10, 10, 10 } ) );

  {
    const auto m = find_mismatch( Ws{ 15, 10, 10 } );
    assert( m );
    assert( m->subprog_index == 0 );
    assert( m->diff == -5 );
  }

  {
    const auto m = find_mismatch( Ws{ 10, 15, 10 } );
    assert( m );
    assert( m->subprog_index == 1 );
    assert( m->diff == -5 );
  }

  {
    const auto m = find_mismatch( Ws{ 10, 10, 15 } );
    assert( m );
    assert( m->subprog_index == 2 );
    assert( m->diff == -5 );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
