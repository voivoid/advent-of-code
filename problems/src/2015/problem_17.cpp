#include "AoC/2015/problem_17.h"

#include "AoC/problems_map.h"
#include "AoC/utils/ranges.h"

#include "range/v3/action/sort.hpp"
#include "range/v3/algorithm/lexicographical_compare.hpp"
#include "range/v3/algorithm/min.hpp"
#include "range/v3/iterator/operations.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/any_view.hpp"
#include "range/v3/view/concat.hpp"
#include "range/v3/view/empty.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/istream.hpp"
#include "range/v3/view/single.hpp"
#include "range/v3/view/tail.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/numeric/conversion/cast.hpp"

#include <istream>
#include <map>

namespace
{
using Volume                 = int;
using ContainersCombinations = ranges::any_view<ranges::any_view<Volume>>;
using Containers             = std::vector<Volume>;
using CacheKey               = std::pair<Volume, ranges::any_view<Volume, ranges::category::random_access>>;

struct CacheComparator
{
  bool operator()( const CacheKey& k1, const CacheKey& k2 ) const
  {
    if ( k1.first != k2.first )
    {
      return k1.first < k2.first;
    }

    auto r1 = k1.second;
    auto r2 = k2.second;

    return ranges::lexicographical_compare( r1, r2 );
  }
};

using Cache = std::map<CacheKey, ContainersCombinations, CacheComparator>;

ContainersCombinations
    generate_combinations( ranges::any_view<Volume, ranges::category::random_access> containers, const Volume volume_left, Cache& cache )
{
  const auto cache_iter = cache.find( std::make_pair( volume_left, containers ) );
  if ( cache_iter != cache.cend() )
  {
    return cache_iter->second;
  }

  if ( volume_left == 0 )
  {
    return ranges::view::single( ranges::view::empty<Volume> );
  }
  else if ( containers.empty() || volume_left < 0 )
  {
    return {};
  }

  auto head = containers.front();
  if ( volume_left < head )
  {
    return {};
  }

  auto tail = containers | ranges::view::tail;

  ContainersCombinations result1 = generate_combinations( tail, volume_left - head, cache ) | ranges::view::transform( [head]( auto rng ) {
                                     return ranges::view::concat( ranges::view::single( head ), rng );
                                   } );

  ContainersCombinations result2 = generate_combinations( tail, volume_left, cache );

  auto result = ranges::view::concat( result1, result2 );

  cache.emplace( std::make_pair( volume_left, containers ), result );

  return result;
}

ContainersCombinations generate_combinations( std::istream& input, const Volume volume_to_store )
{
  auto containers = ranges::istream<Volume>( input ) | ranges::to_vector | ranges::action::sort;
  Cache cache;
  return generate_combinations( containers, volume_to_store, cache );
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
  return boost::numeric_cast<int>( get_possible_combinations_num( input, 150 ) );
}

int solve_2( std::istream& input )
{
  return boost::numeric_cast<int>( get_min_containers_combinations_num( input, 150 ) );
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
  using VV = std::vector<Containers>;

  {
    std::vector<Volume> v{};
    Cache cache;
    auto r = generate_combinations( v, 1, cache ) | AoC::to_2d_vector();
    assert( r == VV{} );
  }

  {
    std::vector<Volume> v{ 1 };
    Cache cache;
    auto r = generate_combinations( v, 0, cache ) | AoC::to_2d_vector();
    assert( r == VV{ {} } );
  }

  {
    std::vector<Volume> v{ 3 };
    Cache cache;
    auto r = generate_combinations( v, 1, cache ) | AoC::to_2d_vector();
    assert( r == VV{} );
  }

  {
    std::vector<Volume> v{ 3 };
    Cache cache;
    auto r = generate_combinations( v, 3, cache ) | AoC::to_2d_vector();
    assert( r == VV{ { 3 } } );
  }

  {
    std::vector<Volume> v{ 2, 3, 5 };
    Cache cache;
    auto r = generate_combinations( v, 10, cache ) | AoC::to_2d_vector();
    assert( r == ( VV{ { 2, 3, 5 } } ) );
  }

  {
    std::vector<Volume> v{ 5, 5, 100, 100 };
    Cache cache;
    auto r = generate_combinations( v, 10, cache ) | AoC::to_2d_vector();
    assert( r == ( VV{ { 5, 5 } } ) );
  }


  {
    std::stringstream ss( "20 15 10 5 5" );
    assert( 4 == get_possible_combinations_num( ss, 25 ) );
  }

  {
    std::stringstream ss( "20 15 10 5 5" );
    assert( 3 == get_min_containers_combinations_num( ss, 25 ) );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
