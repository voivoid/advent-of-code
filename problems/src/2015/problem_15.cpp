#include "AoC/2015/problem_15.h"

#include "AoC/problems_map.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/max.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/indices.hpp"
#include "range/v3/view/indirect.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/view/zip_with.hpp"

#include "boost/coroutine2/all.hpp"
#include "boost/fusion/adapted/struct.hpp"
#include "boost/numeric/conversion/cast.hpp"


#include <algorithm>
#include <istream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
using Quantity                = size_t;
using Quantities              = std::vector<Quantity>;
using QuantityCombinationCoro = boost::coroutines2::coroutine<Quantities*>;
using QuantityCombinations    = QuantityCombinationCoro::pull_type;

struct Ingredient
{
  std::string name;
  int capacity;
  int durability;
  int flavor;
  int texture;
  int calories;
};

Ingredient operator+( Ingredient i1, const Ingredient& i2 )
{
  i1.capacity += i2.capacity;
  i1.durability += i2.durability;
  i1.flavor += i2.flavor;
  i1.texture += i2.texture;
  i1.calories += i2.calories;

  return i1;
}

Ingredient operator*( Ingredient i, const Quantity q )
{
  const int qi = boost::numeric_cast<int>( q );
  i.capacity *= qi;
  i.durability *= qi;
  i.flavor *= qi;
  i.texture *= qi;
  i.calories *= qi;

  return i;
}
}  // namespace

BOOST_FUSION_ADAPT_STRUCT( Ingredient, name, capacity, durability, flavor, texture, calories )

namespace
{
Ingredient parse_ingredient( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  const auto name_parser = x3::lexeme[ +x3::alpha ];
  const auto parser =
      name_parser > "capacity" > x3::int_ > "durability" > x3::int_ > "flavor" > x3::int_ > "texture" > x3::int_ > "calories" > x3::int_;

  Ingredient ingredient;
  const bool is_parsed = AoC::x3_parse( line, parser, x3::space | ( x3::punct - '-' ), ingredient );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse ingredient data" );
  }

  return ingredient;
}

void generate_quantity_combinations( const size_t ingredients_left,
                                     const Quantity quantity_left,
                                     Quantities& quantities_stack,
                                     QuantityCombinationCoro::push_type& yield )
{
  if ( ingredients_left == 0 )
  {
    yield( &quantities_stack );
    return;
  }

  const Quantity start_quantity = ingredients_left == 1 ? quantity_left : 0;
  for ( const auto quantity : ranges::views::closed_indices( start_quantity, quantity_left ) )
  {
    quantities_stack.push_back( quantity );
    generate_quantity_combinations( ingredients_left - 1, quantity_left - quantity, quantities_stack, yield );
    quantities_stack.pop_back();
  };
}

template <typename Recipes>
auto calc_scores( Recipes recipes )
{
  return recipes | ranges::views::transform( []( auto recipe ) {
           auto score = ranges::accumulate( recipe, Ingredient{} );

           score.capacity   = std::max( score.capacity, 0 );
           score.durability = std::max( score.durability, 0 );
           score.flavor     = std::max( score.flavor, 0 );
           score.texture    = std::max( score.texture, 0 );

           return score;
         } );
}

template <typename Scores>
size_t calc_max_score( Scores scores )
{
  auto total_scores =
      scores | ranges::views::transform( []( const Ingredient& i ) { return i.capacity * i.durability * i.flavor * i.texture; } );

  return boost::numeric_cast<size_t>( ranges::max( total_scores ) );
}

QuantityCombinations generate_quantity_combinations( const size_t ingredients_num, const Quantity max_quantity )
{
  QuantityCombinationCoro::pull_type quantity_combinations( [ ingredients_num, max_quantity ]( QuantityCombinationCoro::push_type& yield ) {
    Quantities quantities_stack;
    generate_quantity_combinations( ingredients_num, max_quantity, quantities_stack, yield );
  } );

  return quantity_combinations;
}

auto generate_ingredient_combinations( QuantityCombinations& quantity_combinations, const std::vector<Ingredient>& ingredients )
{
  return quantity_combinations | ranges::views::indirect | ranges::views::transform( [ &ingredients ]( const auto& quantities ) {
           auto multiply = []( const Ingredient& ingredient, const Quantity quantity ) { return ingredient * quantity; };
           return ranges::views::zip_with( multiply, ingredients, quantities );
         } );
}

size_t solve( std::istream& input, const Quantity max_quantity, const std::optional<int> calories = {} )
{
  const auto ingredients = ranges::getlines( input ) | ranges::views::transform( &parse_ingredient ) | ranges::to_vector;

  auto quantity_combinations         = generate_quantity_combinations( ingredients.size(), max_quantity );
  const auto ingredient_combinations = generate_ingredient_combinations( quantity_combinations, ingredients );
  const auto scores                  = calc_scores( ingredient_combinations );

  if ( calories )
  {
    const auto filtered_by_calories =
        scores | ranges::views::filter( [ &calories ]( const Ingredient& ingredient ) { return ingredient.calories == *calories; } );
    return calc_max_score( filtered_by_calories );
  }
  return calc_max_score( scores );
}


}  // namespace

namespace AoC_2015
{

namespace problem_15
{

size_t solve_1( std::istream& input )
{
  return solve( input, 100 );
}

size_t solve_2( std::istream& input )
{
  return solve( input, 100, 500 );
}

AOC_REGISTER_PROBLEM( 2015_15, solve_1, solve_2 );

}  // namespace problem_15

}  // namespace AoC_2015


#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

bool test_combinations( const size_t ingredients_num, const size_t quantity, const std::vector<Quantities> quantities )
{
  auto qs      = generate_quantity_combinations( ingredients_num, quantity );
  const auto r = qs | ranges::views::indirect | ranges::to_vector;
  return r == quantities;
}

static void impl_tests()
{
  assert( test_combinations( 0, 0, { {} } ) );
  assert( test_combinations( 1, 1, { { 1 } } ) );
  assert( test_combinations( 1, 3, { { 3 } } ) );
  assert( test_combinations( 2, 2, { { 0, 2 }, { 1, 1 }, { 2, 0 } } ) );
  assert( test_combinations( 2, 3, { { 0, 3 }, { 1, 2 }, { 2, 1 }, { 3, 0 } } ) );

  const auto ingredient1 = parse_ingredient( "Butterscotch: capacity -1, durability -2, flavor 6, texture 3, calories 8" );
  assert( ingredient1.name == "Butterscotch" );
  assert( ingredient1.capacity == -1 );
  assert( ingredient1.durability == -2 );
  assert( ingredient1.flavor == 6 );
  assert( ingredient1.texture == 3 );
  assert( ingredient1.calories == 8 );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
