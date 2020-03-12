#include "AoC/2015/problem_17.h"

#include "AoC/problems_map.h"
#include "AoC/utils/ranges/2dvector.h"

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
using CacheKey               = std::pair<Volume, Containers>;

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

ContainersCombinations generate_combinations( ranges::any_view<Volume> containers, const Volume volume_left, Cache& cache )
{
  const auto cache_iter = cache.find( { volume_left, ranges::to<Containers>( containers ) } );
  if ( cache_iter != cache.cend() )
  {
    return cache_iter->second;
  }

  if ( volume_left == 0 )
  {
    return ranges::views::single( ranges::views::empty<Volume> );
  }
  else if ( ( containers.begin() == containers.end() ) || volume_left < 0 )
  {
    return {};
  }

  auto head = *containers.begin();
  if ( volume_left < head )
  {
    return {};
  }

  auto tail = containers | ranges::views::tail;

  ContainersCombinations result1 = generate_combinations( tail, volume_left - head, cache ) | ranges::views::transform( [head]( auto rng ) {
                                     return ranges::views::concat( ranges::views::single( head ), rng );
                                   } );

  ContainersCombinations result2 = generate_combinations( tail, volume_left, cache );

  auto result = ranges::views::concat( result1, result2 );

  cache.emplace( std::make_pair( volume_left, ranges::to<Containers>( containers ) ), result );

  return result;
}

ContainersCombinations generate_combinations( std::istream& input, const Volume volume_to_store )
{
  auto containers = ranges::istream<Volume>( input ) | ranges::to_vector | ranges::actions::sort;
  Cache cache;
  return generate_combinations( containers, volume_to_store, cache );
}

size_t get_possible_combinations_num( std::istream& input, const Volume volume_to_store )
{
  return boost::numeric_cast<size_t>( ranges::distance( generate_combinations( input, volume_to_store ) ) );
}

size_t get_min_containers_combinations_num( std::istream& input, const Volume volume_to_store )
{
  const auto combinations = generate_combinations( input, volume_to_store ) | AoC::to_2d_vector();

  const auto combination_with_min_containers =
      ranges::min( combinations, []( const auto c1, const auto c2 ) { return c1.size() < c2.size(); } );

  const auto min_num_of_containers = combination_with_min_containers.size();

  auto combitions_with_min_containers =
      combinations | ranges::views::filter( [min_num_of_containers]( const auto c ) { return c.size() == min_num_of_containers; } );

  return boost::numeric_cast<size_t>( ranges::distance( combitions_with_min_containers ) );
}

}  // namespace

namespace AoC_2015
{

namespace problem_17
{

size_t solve_1( std::istream& input )
{
  return get_possible_combinations_num( input, 150 );
}

size_t solve_2( std::istream& input )
{
  return get_min_containers_combinations_num( input, 150 );
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
    const std::vector<Volume> v{};
    Cache cache;
    auto r = generate_combinations( v, 1, cache ) | AoC::to_2d_vector();
    assert( r == VV{} );
  }

  {
    const std::vector<Volume> v{ 1 };
    Cache cache;
    auto r = generate_combinations( v, 0, cache ) | AoC::to_2d_vector();
    assert( r == VV{ {} } );
  }

  {
    const std::vector<Volume> v{ 3 };
    Cache cache;
    auto r = generate_combinations( v, 1, cache ) | AoC::to_2d_vector();
    assert( r == VV{} );
  }

  {
    const std::vector<Volume> v{ 3 };
    Cache cache;
    auto r = generate_combinations( v, 3, cache ) | AoC::to_2d_vector();
    assert( r == VV{ { 3 } } );
  }

  {
    const std::vector<Volume> v{ 2, 3, 5 };
    Cache cache;
    auto r = generate_combinations( v, 10, cache ) | AoC::to_2d_vector();
    assert( r == ( VV{ { 2, 3, 5 } } ) );
  }

  {
    const std::vector<Volume> v{ 5, 5, 100, 100 };
    Cache cache;
    auto r = generate_combinations( v, 10, cache ) | AoC::to_2d_vector();
    assert( r == ( VV{ { 5, 5 } } ) );
  }


  {
    std::istringstream ss( "20 15 10 5 5" );
    assert( 4 == get_possible_combinations_num( ss, 25 ) );
  }

  {
    std::istringstream ss( "20 15 10 5 5" );
    assert( 3 == get_min_containers_combinations_num( ss, 25 ) );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
