#include "AoC/2017/problem_14.h"
#include "AoC/2017/problem_10.h"

#include "AoC/problems_map.h"
#include "AoC/utils/2d_array.h"
#include "AoC/utils/algo.h"
#include "AoC/utils/geo.h"
#include "AoC/utils/ranges/enumerate.h"

#include "range/v3/algorithm/copy.hpp"
#include "range/v3/algorithm/count.hpp"
#include "range/v3/view/all.hpp"
#include "range/v3/view/enumerate.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/indices.hpp"
#include "range/v3/view/join.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/view/zip.hpp"

#include "boost/numeric/conversion/cast.hpp"

#include <array>
#include <bitset>
#include <istream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>

namespace
{

using Coord                = AoC::GenericPoint<size_t>;
constexpr size_t HashesNum = 128;

using Bit                         = int;
constexpr Bit b1                  = true;
[[maybe_unused]] constexpr Bit b0 = false;

using Grid  = std::unordered_set<Coord, AoC::GeoHasher>;
using Quad  = std::array<Bit, 4>;
using Quads = std::array<Quad, 16>;

constexpr Quad hex_to_quad( size_t n )
{
  const std::bitset<4> bs( n );
  const Quad quad{ bs[ 3 ], bs[ 2 ], bs[ 1 ], bs[ 0 ] };
  return quad;
}

constexpr Quads get_quads()
{
  Quads quads{};

  for ( size_t i = 0; i < 16; ++i )
  {
    quads[ i ] = hex_to_quad( i );
  }

  return quads;
}

auto get_hash_bits( const std::string& hash )
{
  return hash | ranges::view::transform( []( const char h ) {
           static constexpr auto quads = get_quads();

           const auto& quad = quads[ AoC::hex_to_<size_t>( h ) ];
           return quad | ranges::view::all;
         } ) |
         ranges::view::join;
}

std::string calc_hash( const std::string& key, const int postfix )
{
  std::stringstream ss;
  ss << key << '-' << postfix;
  return AoC_2017::problem_10::solve_2( ss );
}

auto get_hashes( std::istream& input )
{
  std::string key;
  input >> key;

  return ranges::view::indices( HashesNum ) | ranges::view::transform( std::bind( &calc_hash, key, std::placeholders::_1 ) );
}

Grid make_grid( std::istream& input )
{
  Grid grid;

  auto hashes = get_hashes( input );
  for ( auto&& [ y, hash ] : hashes | AoC::enumerate_with_<size_t>() )
  {
    auto bits = get_hash_bits( hash );
    for ( const auto [ x, bit ] : bits | AoC::enumerate_with_<size_t>() )
    {
      if ( bit == b1 )
      {
        grid.insert( { x, y } );
      }
    }
  }

  return grid;
}

using Neighbours = std::array<Coord, 4>;
Neighbours get_neighbours( const Coord coord )
{
  const auto [ x, y ] = coord;
  return { { { x - 1, y }, { x, y - 1 }, { x + 1, y }, { x, y + 1 } } };
}

void find_and_erase_region( Grid& grid, const Coord coord )
{
  if ( grid.erase( coord ) )
  {
    for ( const auto neighbour : get_neighbours( coord ) )
    {
      find_and_erase_region( grid, neighbour );
    }
  }
}

size_t count_regions( Grid& grid )
{
  size_t counter = 0;

  while ( !grid.empty() )
  {
    find_and_erase_region( grid, *grid.cbegin() );
    ++counter;
  }

  return counter;
}



}  // namespace

namespace AoC_2017
{

namespace problem_14
{

size_t solve_1( std::istream& input )
{
  const auto grid = make_grid( input );
  return grid.size();
}

size_t solve_2( std::istream& input )
{
  auto grid = make_grid( input );
  return count_regions( grid );
}

AOC_REGISTER_PROBLEM( 2017_14, solve_1, solve_2 );

}  // namespace problem_14
}  // namespace AoC_2017

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  {
    constexpr auto r = hex_to_quad( 2 );
    assert( ( Quad{ b0, b0, b1, b0 } ) == r );
  }

  {
    constexpr auto r = hex_to_quad( 11 );
    assert( ( Quad{ b1, b0, b1, b1 } ) == r );
  }

  {
    constexpr auto qs = get_quads();
    assert( ( Quad{ b0, b1, b0, b1 } ) == qs[ 5 ] );
  }

  {
    const auto bits = get_hash_bits( "a0c2017" ) | ranges::to_vector;

    // clang-format off
    std::vector<Bit> expected{ b1, b0, b1, b0,
                               b0, b0, b0, b0,
                               b1, b1, b0, b0,
                               b0, b0, b1, b0,
                               b0, b0, b0, b0,
                               b0, b0, b0, b1,
                               b0, b1, b1, b1 };
    // clang-format on
    assert( expected == bits );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
