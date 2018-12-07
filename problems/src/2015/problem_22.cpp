#include "AoC/2015/problem_22.h"

#include "AoC/problems_map.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/min.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/fusion/algorithm/iteration/for_each.hpp"
#include "boost/fusion/container/map.hpp"
#include "boost/fusion/container/vector.hpp"
#include "boost/mp11.hpp"
#include "boost/spirit/home/x3.hpp"

#include <istream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace
{

struct Boss
{
  int hp;
  int damage;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( Boss, hp, damage )

namespace
{

struct Player;
struct EffectfulSpell
{
  static void start( Player&, Boss& )
  {
  }
  static void make_effect( Player&, Boss& )
  {
  }
  static void stop( Player&, Boss& )
  {
  }
};

struct MagicMissile
{
  static void cast( Player& player, Boss& boss );
  static constexpr int mana_cost = 53;
};

struct Drain
{
  static void cast( Player& player, Boss& boss );
  static constexpr int mana_cost = 73;
};

struct Shield : EffectfulSpell
{
  static void start( Player& player, Boss& boss );
  static void stop( Player& player, Boss& boss );

  static constexpr int mana_cost      = 113;
  static constexpr int turns_duration = 6;

private:
  static constexpr int armor_modifier = +7;
};

struct Poison : EffectfulSpell
{
  static void make_effect( Player&, Boss& );

  static constexpr int mana_cost      = 173;
  static constexpr int turns_duration = 6;
};

struct Recharge : EffectfulSpell
{
  static void make_effect( Player&, Boss& );

  static constexpr int mana_cost      = 229;
  static constexpr int turns_duration = 5;
};

struct Player
{
  using SpellsList = boost::mp11::mp_list<MagicMissile, Drain, Shield, Poison, Recharge>;

  int hp;
  int armor;
  int mana;

  template <typename Spell>
  bool is_active_spell() const;

  template <typename Spell>
  void play_turn_with_spell( Boss& boss );

private:
  template <typename Spell>
  void cast_spell( Boss& boss );

  template <typename Spell>
  void start_effectful_spell( Boss& boss );

  template <typename Spell>
  void end_effectful_spell( Boss& boss );

  void update_effectful_spells( Boss& boss );

private:
  template <typename Spell>
  using IsEffectfulSpell    = boost::mp11::mp_bool<!std::is_base_of<EffectfulSpell, Spell>::value>;
  using EffectfulSpellsList = boost::mp11::mp_remove_if<SpellsList, IsEffectfulSpell>;

  template <typename Spell>
  using make_fusion_spell_pair = boost::fusion::pair<Spell, int>;
  using SpellEffects = boost::mp11::mp_rename<boost::mp11::mp_transform<make_fusion_spell_pair, EffectfulSpellsList>, boost::fusion::map>;

  template <typename Spell>
  constexpr static bool is_effectful();

private:
  SpellEffects spell_effects;
};

void MagicMissile::cast( Player&, Boss& boss )
{
  boss.hp -= 4;
}

void Drain::cast( Player& player, Boss& boss )
{
  player.hp += 2;
  boss.hp -= 2;
}

void Shield::start( Player& player, Boss& )
{
  player.armor += armor_modifier;
}

void Shield::stop( Player& player, Boss& )
{
  player.armor -= armor_modifier;
}

void Poison::make_effect( Player&, Boss& boss )
{
  boss.hp -= 3;
}

void Recharge::make_effect( Player& player, Boss& )
{
  player.mana += 101;
}

template <typename Spell>
bool is_castable( const Player& player )
{
  const bool is_enough_mana = player.mana >= Spell::mana_cost;
  const bool is_active      = player.is_active_spell<Spell>();

  return is_enough_mana && !is_active;
}

void Player::update_effectful_spells( Boss& boss )
{
  boost::fusion::for_each( spell_effects, [this, &boss]( auto& v ) {
    using Spell        = typename std::decay_t<decltype( v )>::first_type;
    auto& active_turns = v.second;
    if ( active_turns > 0 )
    {
      Spell::make_effect( *this, boss );

      if ( active_turns == 1 )
      {
        end_effectful_spell<Spell>( boss );
      }

      --active_turns;
    }
  } );
}

template <typename Spell>
void Player::play_turn_with_spell( Boss& boss )
{
  // player turn
  cast_spell<Spell>( boss );
  update_effectful_spells( boss );

  // boss turn
  if ( boss.hp > 0 )
  {
    const auto boss_damage = std::max( 1, boss.damage - armor );
    hp -= boss_damage;
  }
  update_effectful_spells( boss );
}

template <typename Spell>
constexpr bool Player::is_effectful()
{
  return boost::mp11::mp_to_bool<boost::mp11::mp_contains<Player::EffectfulSpellsList, Spell>>::value;
}

template <typename Spell>
void Player::cast_spell( Boss& boss )
{
  assert( is_castable<Spell>( *this ) );

  mana -= Spell::mana_cost;

  if constexpr ( is_effectful<Spell>() )
  {
    start_effectful_spell<Spell>( boss );
  }
  else
  {
    Spell::cast( *this, boss );
  }
}

template <typename Spell>
bool Player::is_active_spell() const
{
  if constexpr ( is_effectful<Spell>() )
  {
    return boost::fusion::at_key<Spell>( spell_effects ) > 0;
  }
  return false;
}

template <typename Spell>
void Player::start_effectful_spell( Boss& boss )
{
  assert( !is_active_spell<Spell>() );
  boost::fusion::at_key<Spell>( spell_effects ) = Spell::turns_duration;

  Spell::start( *this, boss );
}

template <typename Spell>
void Player::end_effectful_spell( Boss& boss )
{
  assert( is_active_spell<Spell>() );
  Spell::stop( *this, boss );
}

using SpellsVariant = boost::mp11::mp_rename<Player::SpellsList, std::variant>;
using Spells        = std::vector<SpellsVariant>;

int calc_spells_mana_cost( const Spells& spells )
{
  const auto spell_costs = spells | ranges::view::transform( []( const auto& spell_variant ) {
                             return std::visit( []( auto spell ) { return decltype( spell )::mana_cost; }, spell_variant );
                           } );

  return ranges::accumulate( spell_costs, 0 );
}

enum class GameMode
{
  Easy,
  Hard
};

std::vector<Spells> generate_spells( const Player& player, const Boss& boss, const GameMode mode )
{
  if ( player.hp <= 0 )
  {
    return {};
  }
  if ( boss.hp <= 0 )
  {
    return { {} };
  }

  std::vector<Spells> result_spells;
  boost::mp11::mp_for_each<Player::SpellsList>( [&result_spells, player, boss, mode]( auto spell ) {
    using Spell = decltype( spell );
    if ( is_castable<Spell>( player ) )
    {
      Boss b   = boss;
      Player p = player;

      if ( mode == GameMode::Hard )
      {
        p.hp -= 1;
      }

      p.play_turn_with_spell<Spell>( b );
      auto list_of_spells = generate_spells( p, b, mode );

      for ( auto& spells : list_of_spells )
      {
        spells.push_back( { Spell{} } );
        result_spells.push_back( std::move( spells ) );
      }
    }
  } );

  return result_spells;
}

Boss parse_boss( const std::string& input )
{
  namespace x3 = boost::spirit::x3;

  Boss boss;
  const auto parser = "Hit Points:" > x3::int_ > "Damage:" > x3::int_;

  const bool is_parsed = AoC::x3_parse( input.cbegin(), input.cend(), parser, x3::space, boss );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse boss data" );
  }

  return boss;
}

int solve( std::istream& input, const GameMode mode )
{
  std::string input_str{ std::istream_iterator<char>( input >> std::noskipws ), std::istream_iterator<char>() };
  Boss boss = parse_boss( input_str );
  Player player{};
  player.hp   = 50;
  player.mana = 500;

  auto spells     = generate_spells( player, boss, mode );
  auto mana_costs = spells | ranges::view::transform( &calc_spells_mana_cost );

  assert( !mana_costs.empty() );
  return ranges::min( mana_costs );
}

}  // namespace

namespace AoC_2015
{

namespace problem_22
{

int solve_1( std::istream& input )
{
  return solve( input, GameMode::Easy );
}

int solve_2( std::istream& input )
{
  return solve( input, GameMode::Hard );
}

AOC_REGISTER_PROBLEM( 2015_22, solve_1, solve_2 );

}  // namespace problem_22

}  // namespace AoC_2015


#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  {
    Player p{};
    p.mana = MagicMissile::mana_cost;
    assert( is_castable<MagicMissile>( p ) );
  }

  {
    Player p{};
    p.mana = MagicMissile::mana_cost - 1;
    assert( !is_castable<MagicMissile>( p ) );
  }

  {
    Player p{};
    p.mana = Shield::mana_cost;
    assert( is_castable<Shield>( p ) );
  }

  {
    Player p{};
    p.mana = Shield::mana_cost;
    Boss b{};
    assert( !p.is_active_spell<Shield>() );
    p.play_turn_with_spell<Shield>( b );
    assert( p.armor == 7 );
    assert( p.is_active_spell<Shield>() );
  }

  {
    Player p{};
    p.hp   = 10;
    p.mana = 250;
    Boss b{};
    b.hp     = 13;
    b.damage = 8;

    p.play_turn_with_spell<Poison>( b );
    p.play_turn_with_spell<MagicMissile>( b );

    assert( p.hp == 2 );
    assert( p.mana == 24 );
    assert( b.hp <= 0 );
  }

  {
    Player p{};
    p.hp   = 10;
    p.mana = 250;
    Boss b{};
    b.hp     = 14;
    b.damage = 8;

    p.play_turn_with_spell<Recharge>( b );
    p.play_turn_with_spell<Shield>( b );
    p.play_turn_with_spell<Drain>( b );
    p.play_turn_with_spell<Poison>( b );
    p.play_turn_with_spell<MagicMissile>( b );

    assert( p.hp == 1 );
    assert( p.mana == 114 );
    assert( b.hp <= 0 );
  }

  {
    Player p{};
    p.hp   = 50;
    p.mana = 500;
    Boss b{};
    b.hp     = 71;
    b.damage = 10;

    p.play_turn_with_spell<Recharge>( b );
    p.play_turn_with_spell<Poison>( b );
    p.play_turn_with_spell<Shield>( b );
    p.play_turn_with_spell<Recharge>( b );
    p.play_turn_with_spell<Poison>( b );
    p.play_turn_with_spell<Shield>( b );
    p.play_turn_with_spell<Recharge>( b );
    p.play_turn_with_spell<Poison>( b );
    p.play_turn_with_spell<Shield>( b );
    p.play_turn_with_spell<MagicMissile>( b );
    p.play_turn_with_spell<Poison>( b );
    p.play_turn_with_spell<MagicMissile>( b );

    assert( b.hp <= 0 );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
