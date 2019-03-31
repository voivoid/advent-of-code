#include "AoC/2015/problem_21.h"

#include "AoC/problems_map.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/min.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/for_each.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/numeric/conversion/cast.hpp"
#include "boost/spirit/home/x3.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <istream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
struct Char
{
  int hp;
  int damage;
  int armor;
};
}  // namespace

BOOST_FUSION_ADAPT_STRUCT( Char, hp, damage, armor )

namespace
{

struct Item
{
  int cost;
  int damage;
  int armor;
};

using Items = std::vector<Item>;

static const auto store_weapon = { Item{ 8, 4, 0 }, Item{ 10, 5, 0 }, Item{ 25, 6, 0 }, Item{ 40, 7, 0 }, Item{ 74, 8, 0 } };

static const auto store_armor = {
  Item{ 0, 0, 0 }, Item{ 13, 0, 1 }, Item{ 31, 0, 2 }, Item{ 53, 0, 3 }, Item{ 75, 0, 4 }, Item{ 102, 0, 5 }
};

static const auto store_rings = { Item{ 0, 0, 0 },   Item{ 0, 0, 0 },  Item{ 25, 1, 0 }, Item{ 50, 2, 0 },
                                  Item{ 100, 3, 0 }, Item{ 20, 0, 1 }, Item{ 40, 0, 2 }, Item{ 80, 0, 3 } };

const Char& fight( const Char& char1, const Char& char2 )
{
  const int char1_dmg = std::max( 1, char1.damage - char2.armor );
  const int char2_dmg = std::max( 1, char2.damage - char1.armor );

  const int char1_round_to_win =
      boost::numeric_cast<int>( std::ceil( boost::numeric_cast<float>( char2.hp ) / boost::numeric_cast<float>( char1_dmg ) ) );
  const int char2_round_to_win =
      boost::numeric_cast<int>( std::ceil( boost::numeric_cast<float>( char1.hp ) / boost::numeric_cast<float>( char2_dmg ) ) );

  const Char& winner = char2_round_to_win < char1_round_to_win ? char2 : char1;
  return winner;
}

Char wear_items( Char character, const Items& items )
{
  for ( const auto& item : items )
  {
    character.armor += item.armor;
    character.damage += item.damage;
  }

  return character;
}

int calc_items_cost( const Items& items )
{
  return ranges::accumulate( items, 0, {}, &Item::cost );
}

auto generate_items_combination()
{
  using namespace ranges::view;
  return store_weapon | for_each( []( const Item& weapon ) {
           return store_armor | for_each( [&weapon]( const Item& armor ) {
                    return store_rings | for_each( [&weapon, &armor]( const Item& ring1 ) {
                             return store_rings | for_each( [&weapon, &armor, &ring1]( const Item& ring2 ) {
                                      return ranges::yield_if( &ring1 != &ring2, std::vector<Item>{ weapon, armor, ring1, ring2 } );
                                    } );
                           } );
                  } );
         } );
}

Char parse_boss( const std::string& input )
{
  namespace x3 = boost::spirit::x3;

  Char boss;
  const auto parser = "Hit Points:" > x3::int_ > "Damage:" > x3::int_ > "Armor:" > x3::int_;

  const bool is_parsed = AoC::x3_parse( input.cbegin(), input.cend(), parser, x3::space, boss );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse boss data" );
  }

  return boss;
}

bool are_winnable_items( const Items& items, const Char& player, const Char& boss )
{
  const auto player_with_items = wear_items( player, items );
  const auto& winner           = fight( player_with_items, boss );

  return &winner == &player_with_items;
}

bool are_loser_items( const Items& items, const Char& player, const Char& boss )
{
  return !are_winnable_items( items, player, boss );
}

template <bool ( *items_filter )( const Items&, const Char&, const Char& )>
auto get_suitable_items_costs( std::istream& input )
{
  std::string input_str{ std::istream_iterator<char>( input >> std::noskipws ), std::istream_iterator<char>() };

  const Char player = { 100, 0, 0 };
  const Char boss   = parse_boss( input_str );

  auto items_combinations = generate_items_combination();

  auto suitable_items_costs = items_combinations | ranges::view::filter( std::bind( items_filter, std::placeholders::_1, player, boss ) ) |
                              ranges::view::transform( &calc_items_cost );

  return suitable_items_costs;
}

}  // namespace

namespace AoC_2015
{

namespace problem_21
{

int solve_1( std::istream& input )
{
  auto suitable_items_costs = get_suitable_items_costs<&are_winnable_items>( input );
  return ranges::min( suitable_items_costs );
}

int solve_2( std::istream& input )
{
  auto suitable_items_costs = get_suitable_items_costs<&are_loser_items>( input );
  return ranges::max( suitable_items_costs );
}

AOC_REGISTER_PROBLEM( 2015_21, solve_1, solve_2 );

}  // namespace problem_21

}  // namespace AoC_2015


#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  {
    const Char b = parse_boss( "Hit Points: 103\nDamage: 9\nArmor: 2\n" );
    assert( b.hp == 103 );
    assert( b.damage == 9 );
    assert( b.armor == 2 );
  }
  {
    const auto player  = Char{ 8, 5, 5 };
    const auto boss    = Char{ 12, 7, 2 };
    const auto& winner = fight( player, boss );
    assert( &winner == &player );
  }

  {
    const auto player  = Char{ 100, 7, 4 };
    const auto boss    = Char{ 103, 9, 2 };
    const auto& winner = fight( player, boss );
    assert( &winner == &boss );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
