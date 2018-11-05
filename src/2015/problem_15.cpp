#include "AoC/2015/problem_15.h"

#include <AoC/problems_map.h>
#include <AoC_utils/parse.h>

#include <range/v3/algorithm/max.hpp>
#include <range/v3/getlines.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/any_view.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/empty.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/indices.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/single.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip_with.hpp>

#include <boost/fusion/adapted/struct.hpp>
#include <boost/spirit/home/x3.hpp>

#include <algorithm>
#include <istream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
using Quantity = size_t;

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
  const int qi = static_cast<int>( q );
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
  const bool is_parsed = AoC::x3_parse( line.cbegin(), line.cend(), parser, x3::space | ( x3::punct - '-' ), ingredient );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse ingredient data" );
  }

  return ingredient;
}

ranges::any_view<ranges::any_view<Quantity>> generate_quantity_combinations( const size_t ingredients_left, const Quantity quantity_left )
{
  if ( ingredients_left == 0 || quantity_left == 0 )
  {
    return ranges::view::single( ranges::view::empty<Quantity>() );
  }

  const Quantity start_quantity = ingredients_left == 1 ? quantity_left : 0;

  auto combinations =
      ranges::view::closed_indices( start_quantity, quantity_left ) |
      ranges::view::transform( [ingredients_left, quantity_left]( const auto quantity ) {
        return generate_quantity_combinations( ingredients_left - 1, quantity_left - quantity ) |
               ranges::view::transform( [quantity]( auto qs ) { return ranges::view::concat( ranges::view::single( quantity ), qs ); } );
      } ) |
      ranges::view::join;

  return combinations;
}

auto generate_ingredient_combinations( const std::vector<Ingredient>& ingredients, const Quantity max_quantity )
{
  return generate_quantity_combinations( ingredients.size(), max_quantity ) | ranges::view::transform( [&ingredients]( auto quantities ) {
           return ranges::view::zip_with(
               []( const Quantity quantity, const Ingredient& ingredient ) { return ingredient * quantity; }, quantities, ingredients );
         } );
}

template <typename Recipes>
auto calc_scores( Recipes recipes )
{
  return recipes | ranges::view::transform( []( auto recipe ) {
           auto score = ranges::accumulate( recipe, Ingredient{} );

           score.capacity   = std::max( score.capacity, 0 );
           score.durability = std::max( score.durability, 0 );
           score.flavor     = std::max( score.flavor, 0 );
           score.texture    = std::max( score.texture, 0 );

           return score;
         } );
}

template <typename Scores>
int calc_max_score( Scores scores )
{
  auto total_scores =
      scores | ranges::view::transform( []( const Ingredient& i ) { return i.capacity * i.durability * i.flavor * i.texture; } );

  return ranges::max( total_scores );
}

int solve( std::istream& input, const Quantity max_quantity, const std::optional<int> calories = {} )
{
  const auto ingredients       = ranges::getlines( input ) | ranges::view::transform( &parse_ingredient ) | ranges::to_vector;
  auto ingredient_combinations = generate_ingredient_combinations( ingredients, max_quantity );
  auto scores                  = calc_scores( ingredient_combinations );

  if ( calories )
  {
    auto filtered_by_calories =
        scores | ranges::view::filter( [&calories]( const Ingredient& ingredient ) { return ingredient.calories == *calories; } );
    return calc_max_score( filtered_by_calories );
  }
  return calc_max_score( scores );
}


}  // namespace

namespace AoC_2015
{

namespace problem_15
{

int solve_1( std::istream& input )
{
  return solve( input, 100 );
}

int solve_2( std::istream& input )
{
  return solve( input, 100, 500 );
}

AOC_REGISTER_PROBLEM( 2015_15, solve_1, solve_2 );

}  // namespace problem_15

}  // namespace AoC_2015


#ifndef NDEBUG

#  include "impl_tests.h"
#  include <AoC_utils/ranges.h>
#  include <cassert>

static void impl_tests()
{
  using VV = std::vector<std::vector<Quantity>>;

  auto r1 = generate_quantity_combinations( 0, 0 ) | AoC::to_2d_vector();
  assert( r1 == VV{ {} } );

  auto r2 = generate_quantity_combinations( 1, 1 ) | AoC::to_2d_vector();
  assert( r2 == VV{ { 1 } } );

  auto r3 = generate_quantity_combinations( 1, 3 ) | AoC::to_2d_vector();
  assert( r3 == VV{ { 3 } } );

  auto r4 = generate_quantity_combinations( 2, 2 ) | AoC::to_2d_vector();
  assert( r4 == ( VV{ { 0, 2 }, { 1, 1 }, { 2 } } ) );

  auto r5 = generate_quantity_combinations( 2, 3 ) | AoC::to_2d_vector();
  assert( r5 == ( VV{ { 0, 3 }, { 1, 2 }, { 2, 1 }, { 3 } } ) );

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
