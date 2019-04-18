#include "AoC/2017/problem_12.h"

#include "AoC/problems_map.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/count.hpp"
#include "range/v3/algorithm/find_if.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/map.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/numeric/conversion/cast.hpp"

#include <istream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace
{
using ProgId  = size_t;
using ProgIds = std::vector<ProgId>;

struct PipeInfo
{
  ProgId from;
  ProgIds to;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( PipeInfo, from, to )

namespace
{

PipeInfo parse_pipes( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  const auto prog_id = AoC::x3_size_t_;
  const auto parser  = prog_id > "<->" > ( prog_id % ',' );

  PipeInfo pipe_info;
  const bool is_parsed = AoC::x3_parse( line.cbegin(), line.cend(), parser, x3::space, pipe_info );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input command data" );
  }

  return pipe_info;
}

using Connections = std::unordered_map<ProgId, ProgId>;

ProgId& get_base_id( const ProgId id, Connections& conns, bool normalize )
{
  auto iter = conns.find( id );
  if ( iter == conns.cend() )
  {
    iter = conns.emplace( id, id ).first;
  }

  auto& dependent_id = iter->second;
  if ( id == dependent_id )
  {
    return dependent_id;
  }

  auto& base_id = get_base_id( dependent_id, conns, normalize );

  if ( normalize )
  {
    dependent_id = base_id;
  }

  return base_id;
}

void connect( const ProgId p1, const ProgId p2, Connections& conns )
{
  get_base_id( p1, conns, false ) = get_base_id( p2, conns, false );
}

void normalize( Connections& conns )
{
  for ( const auto prog_id : conns | ranges::view::keys )
  {
    get_base_id( prog_id, conns, true );
  }
}

Connections make_connections( std::istream& input )
{
  Connections connections;

  auto pipes = ranges::getlines( input ) | ranges::view::transform( &parse_pipes );
  for ( const auto& pipe : pipes )
  {
    const auto from = pipe.from;
    for ( const auto to : pipe.to )
    {
      connect( from, to, connections );
    }
  }

  normalize( connections );
  return connections;
}

}  // namespace

namespace AoC_2017
{

namespace problem_12
{

size_t solve_1( std::istream& input )
{
  const auto connections = make_connections( input );

  const auto conn_iter = connections.find( 0 );

  if ( conn_iter == connections.cend() )
  {
    assert( false );
    throw std::runtime_error( "This should never be executed" );
  }

  const auto conn_id = conn_iter->second;

  return boost::numeric_cast<size_t>( ranges::count( connections | ranges::view::values, conn_id ) );
}

size_t solve_2( std::istream& input )
{
  const auto connections = make_connections( input );
  const auto groups      = connections | ranges::view::values | ranges::to<std::unordered_set>;
  return groups.size();
}

AOC_REGISTER_PROBLEM( 2017_12, solve_1, solve_2 );

}  // namespace problem_12
}  // namespace AoC_2017

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  {
    auto pipes = parse_pipes( "2 <-> 0, 3, 4" );
    assert( 2 == pipes.from );
    assert( ( ProgIds{ 0, 3, 4 } ) == pipes.to );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
