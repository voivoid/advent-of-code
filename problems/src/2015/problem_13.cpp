#include "AoC/2015/problem_13.h"

#include "AoC/problems_map.h"
#include "AoC/utils/fusion.h"
#include "AoC/utils/parse.h"

#include "range/v3/action/sort.hpp"
#include "range/v3/action/unique.hpp"
#include "range/v3/algorithm/permutation.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/cycle.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/map.hpp"
#include "range/v3/view/sliding.hpp"
#include "range/v3/view/take_exactly.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/functional/hash.hpp"
#include "boost/fusion/include/vector.hpp"


#include <functional>
#include <istream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>


namespace
{
using Name       = std::string;
using ModDiff    = int;
using Neighbours = std::pair<Name, Name>;

struct NeighboursMapCmp
{
  using is_transparent = void;

  bool operator()( const Neighbours& n1, const Neighbours& n2 ) const
  {
    return n1 == n2;
  }

  bool operator()( const std::pair<const Name&, const Name&>& n1, const Neighbours& n2 ) const
  {
    return n1.first == n2.first && n1.second == n2.second;
  }
};

using NeighboursMap = std::unordered_map<Neighbours, ModDiff, boost::hash<Neighbours>, NeighboursMapCmp>;

struct Behavior
{
  Name neighbour_1;
  Name neighbour_2;
  ModDiff diff;
};

Behavior parse_behavior( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  enum class Mode
  {
    gain,
    lose
  };

  x3::symbols<Mode> mode_parser;
  mode_parser.add( "gain", Mode::gain )( "lose", Mode::lose );

  const auto name   = x3::lexeme[ +x3::alpha ];
  const auto parser = name > "would" > mode_parser > x3::int_ > "happiness" > "units" > "by" > "sitting" > "next" > "to" > name > ".";

  boost::fusion::vector<Name, Mode, ModDiff, Name> attrs;
  const bool is_parsed = AoC::x3_parse( line, parser, x3::space, attrs );
  if ( !is_parsed )
  {
    throw std::runtime_error( "Failed to parse behavior input" );
  }

  auto [ neighbour_1, mode, diff, neighbour_2 ] = AoC::fusion_to_std_tuple( attrs );
  return { std::move( neighbour_1 ), std::move( neighbour_2 ), diff * ( mode == Mode::lose ? -1 : +1 ) };
}

void add_neighbours_to_map( Name n1, Name n2, const ModDiff diff, NeighboursMap& map )
{
  map.emplace( std::make_pair( std::move( n1 ), std::move( n2 ) ), diff );
}

NeighboursMap make_neighbours_map( std::istream& input )
{
  auto behaviors = ranges::getlines( input ) | ranges::views::transform( &parse_behavior );

  NeighboursMap neighbours_map;
  for ( const auto& behavior : behaviors )
  {
    add_neighbours_to_map( std::move( behavior.neighbour_1 ), std::move( behavior.neighbour_2 ), behavior.diff, neighbours_map );
  }

  return neighbours_map;
}

ModDiff get_mood( const Name& name1, const Name& name2, const NeighboursMap& neighbours_map )
{
  std::pair<const Name&, const Name&> p = { name1, name2 };
  auto iter                             = neighbours_map.find( p );
  assert( iter != neighbours_map.cend() );
  return iter->second;
}

ModDiff calc_mood( const std::vector<Name>& names, const NeighboursMap& neighbours_map )
{
  assert( names.size() > 2 );

  const auto neighbour_groups =
      names | ranges::views::cycle | ranges::views::take_exactly( names.size() + 1 ) | ranges::views::sliding( 2 );
  const auto total_mood =
      ranges::accumulate( neighbour_groups, ModDiff(), [ &neighbours_map ]( const ModDiff diff, const auto neighbours ) {
        return diff + get_mood( neighbours[ 0 ], neighbours[ 1 ], neighbours_map ) +
               get_mood( neighbours[ 1 ], neighbours[ 0 ], neighbours_map );
      } );
  return total_mood;
}

std::vector<Name> get_names( const NeighboursMap& neighbours_map )
{
  return neighbours_map | ranges::views::keys | ranges::views::transform( &Neighbours::first ) | ranges::to_vector | ranges::actions::sort |
         ranges::actions::unique;
}

ModDiff calc_best_mood( const NeighboursMap& neighbours_map )
{
  auto unique_names = get_names( neighbours_map );

  auto best_mood = calc_mood( unique_names, neighbours_map );
  while ( ranges::next_permutation( unique_names ) )
  {
    best_mood = std::max( best_mood, calc_mood( unique_names, neighbours_map ) );
  }

  return best_mood;
}
}  // namespace

namespace AoC_2015
{

namespace problem_13
{

int solve_1( std::istream& input )
{
  const auto neighbours_map = make_neighbours_map( input );
  return calc_best_mood( neighbours_map );
}

int solve_2( std::istream& input )
{
  auto neighbours_map = make_neighbours_map( input );
  auto unique_names   = get_names( neighbours_map );

  const auto me = "me";
  for ( const auto& name : unique_names )
  {
    add_neighbours_to_map( me, name, 0, neighbours_map );
    add_neighbours_to_map( name, me, 0, neighbours_map );
  }

  return calc_best_mood( neighbours_map );
}

AOC_REGISTER_PROBLEM( 2015_13, solve_1, solve_2 );

}  // namespace problem_13

}  // namespace AoC_2015



#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>
#  include <sstream>

static void impl_tests()
{
  {
    const auto behavior = parse_behavior( "Alice would gain 54 happiness units by sitting next to Bob." );
    assert( behavior.neighbour_1 == "Alice" );
    assert( behavior.neighbour_2 == "Bob" );
    assert( behavior.diff == 54 );
  }
  {
    const auto behavior = parse_behavior( "Foo would lose 99 happiness units by sitting next to Bar." );
    assert( behavior.neighbour_1 == "Foo" );
    assert( behavior.neighbour_2 == "Bar" );
    assert( behavior.diff == -99 );
  }
  {
    std::istringstream ss(
        "Alice would gain 54 happiness units by sitting next to Bob.\nBob would lose 11 happiness units by sitting next to Alice." );
    const auto map = make_neighbours_map( ss );
    auto iter      = map.find( { "Alice", "Bob" } );
    assert( iter != map.cend() );
    assert( iter->second == 54 );
    assert( -11 == get_mood( "Bob", "Alice", map ) );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
