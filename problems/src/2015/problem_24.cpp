#include "AoC/2015/problem_24.h"

#include "AoC/problems_map.h"

#include "range/v3/action/sort.hpp"
#include "range/v3/algorithm/any_of.hpp"
#include "range/v3/algorithm/min.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/any_view.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/istream.hpp"
#include "range/v3/view/set_algorithm.hpp"
#include "range/v3/view/take_while.hpp"

#include "boost/coroutine2/all.hpp"

#include <istream>
#include <optional>
#include <string>
#include <vector>

namespace
{

using Weight  = size_t;
using Weights = std::vector<Weight>;

template <typename Range>
std::uint64_t calc_entanglement( Range&& rng )
{
  return ranges::accumulate( rng, std::uint64_t( 1 ), std::multiplies<Weight>() );
}

template <typename Range>
Weight calc_weights_sum( Range&& rng )
{
  return ranges::accumulate( rng, Weight( 0 ) );
}

auto find_all_weights_combinations( const Weights& sorted_weights )
{
  using coro_t = boost::coroutines2::coroutine<Weights>;

  coro_t::pull_type seq( [&sorted_weights]( coro_t::push_type& yield ) {
    std::vector weights_dfs_stack = { std::make_pair( Weights{}, sorted_weights.cbegin() ) };
    auto end_iter                 = sorted_weights.cend();
    using WeightsStack            = decltype( weights_dfs_stack );

    while ( !weights_dfs_stack.empty() )
    {
      WeightsStack new_stack;
      for ( auto [ weights, begin_iter ] : weights_dfs_stack )
      {
        for ( auto iter = begin_iter; iter < end_iter; )
        {
          auto result_weights = weights;
          result_weights.push_back( *iter );
          yield( result_weights );

          new_stack.emplace_back( std::move( result_weights ), ++iter );
        }
      }

      weights_dfs_stack = std::move( new_stack );
    }
  } );

  return seq;
}

bool check_range_can_be_split_into_groups( ranges::any_view<Weight> rest_weights,
                                           ranges::any_view<Weight> compartment_weights,
                                           const size_t groups_num,
                                           const size_t group_weight )
{
  assert( calc_weights_sum( rest_weights ) == group_weight * groups_num );

  auto diff = ranges::view::set_difference( rest_weights, compartment_weights );

  if ( groups_num == 2 )
  {
    if ( calc_weights_sum( compartment_weights ) == group_weight )
    {
      assert( calc_weights_sum( diff ) == group_weight );
      return true;
    }
    return false;
  }

  return ranges::any_of( find_all_weights_combinations( diff ), [&diff, group_weight, groups_num]( auto ws ) {
    return calc_weights_sum( ws ) == group_weight && check_range_can_be_split_into_groups( diff, ws, groups_num - 1, group_weight );
  } );
}

size_t solve( std::istream& input, const size_t number_of_compartments )
{
  const auto sorted_weights       = ranges::istream<size_t>( input ) | ranges::to_vector | ranges::action::sort;
  const Weight total_weight       = ranges::accumulate( sorted_weights, size_t{ 0 } );
  const Weight compartment_weight = total_weight / number_of_compartments;

  auto weights_combinations = find_all_weights_combinations( sorted_weights );
  auto passenger_weights    = weights_combinations |
                           ranges::view::filter( [compartment_weight]( auto ws ) { return calc_weights_sum( ws ) == compartment_weight; } );

  auto balanced_passenger_weights =
      passenger_weights | ranges::view::filter( [&sorted_weights, number_of_compartments, compartment_weight]( auto ws ) {
        return check_range_can_be_split_into_groups( sorted_weights, ws, number_of_compartments, compartment_weight );
      } );

  std::optional<size_t> min_size;
  auto min_groups = balanced_passenger_weights | ranges::view::take_while( [&min_size]( const auto& r ) {
                      if ( !min_size )
                      {
                        min_size = r.size();
                        return true;
                      }
                      assert( r.size() >= min_size );
                      return r.size() == min_size;
                    } );

  return calc_entanglement( *min_groups.begin() );
}

}  // namespace

namespace AoC_2015
{

namespace problem_24
{

size_t solve_1( std::istream& input )
{
  return solve( input, 3 );
}

size_t solve_2( std::istream& input )
{
  return solve( input, 4 );
}

AOC_REGISTER_PROBLEM( 2015_24, solve_1, solve_2 );

}  // namespace problem_24

}  // namespace AoC_2015


#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  assert( ( find_all_weights_combinations( { 1 } ) | ranges::to_vector ) == std::vector<Weights>{ { 1 } } );
  assert( ( find_all_weights_combinations( { 1, 2 } ) | ranges::to_vector ) == ( std::vector<Weights>{ { 1 }, { 2 }, { 1, 2 } } ) );
  assert( ( find_all_weights_combinations( { 1, 2, 3 } ) | ranges::to_vector ) ==
          ( std::vector<Weights>{ { 1 }, { 2 }, { 3 }, { 1, 2 }, { 1, 3 }, { 2, 3 }, { 1, 2, 3 } } ) );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
