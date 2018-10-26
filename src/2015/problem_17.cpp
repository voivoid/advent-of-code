#include "AoC/2015/problem_17.h"

#include <AoC/problems_map.h>
#include <AoC_utils/ranges.h>

#include <range/v3/action/sort.hpp>
#include <range/v3/algorithm/min.hpp>
#include <range/v3/distance.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/any_view.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/empty.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/single.hpp>
#include <range/v3/view/tail.hpp>
#include <range/v3/view/transform.hpp>

#include <istream>

namespace
{
using Volume            = int;
using ContainersCombinations = ranges::any_view<ranges::any_view<Volume>>;

ContainersCombinations generate_combinations( ranges::any_view<Volume, ranges::category::random_access> containers, const Volume volume_left )
{
  if ( containers.empty() || volume_left <= 0 )
  {
    return ranges::view::single( ranges::view::empty<Volume>() );
  }

  auto head = containers.front();
  auto tail = ranges::view::tail( containers );

  ContainersCombinations result1 = generate_combinations( tail, volume_left - head ) | ranges::view::transform( [head]( auto rng ) {
                                return ranges::view::concat( ranges::view::single( head ), rng );
                              } );

  ContainersCombinations result2 = generate_combinations( tail, volume_left );

  auto result = ranges::view::concat( result1, result2 ) |
                ranges::view::filter( [volume_left]( auto rng ) { return ranges::accumulate( rng, 0 ) == volume_left; } );

  return result;
}

ContainersCombinations generate_combinations( std::istream& input, const Volume volume_to_store )
{
  auto containers = ranges::istream_range<Volume>( input ) | ranges::to_vector | ranges::action::sort;
  return generate_combinations( containers, volume_to_store );
}

auto get_possible_combinations_num( std::istream& input, const Volume volume_to_store )
{
  return ranges::distance( generate_combinations( input, volume_to_store ) );
}

auto get_min_containers_combinations_num( std::istream& input, const Volume volume_to_store )
{
    const auto combinations = generate_combinations( input, volume_to_store ) | AoC::to_2d_vector();

    const auto combination_with_min_containers =
        ranges::min( combinations, []( const auto c1, const auto c2 ) { return c1.size() < c2.size(); } );

    const auto min_num_of_containers = combination_with_min_containers.size();

    auto combitions_with_min_containers =
        combinations | ranges::view::filter( [min_num_of_containers]( const auto c ) { return c.size() == min_num_of_containers; } );

    return ranges::distance( combitions_with_min_containers );
}

}  // namespace

namespace AoC_2015
{

namespace problem_17
{

int solve_1( std::istream& input )
{
  return static_cast<int>( get_possible_combinations_num( input, 150 ) );
}

int solve_2( std::istream& input )
{
    return static_cast<int>( get_min_containers_combinations_num( input, 150 ) );
}

AOC_REGISTER_PROBLEM( 2015_17, solve_1, solve_2 );

}  // namespace problem_17

}  // namespace AoC_2015


#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>
#  include <sstream>

static void impl_tests()
{
    {
        std::stringstream ss ( "20 15 10 5 5" );
        assert( 4 == get_possible_combinations_num( ss , 25 ) );
    }

    {
        std::stringstream ss ( "20 15 10 5 5" );
        assert( 3 == get_min_containers_combinations_num( ss , 25 ) );
    }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
