#include "AoC/2017/problem_03.h"

#include "AoC/problems_map.h"
#include "AoC/utils/2d_array.h"
#include "AoC/utils/assert.h"
#include "AoC/utils/geo.h"

#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/cycle.hpp"
#include "range/v3/view/exclusive_scan.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/join.hpp"
#include "range/v3/view/repeat_n.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/view/zip.hpp"

#include "boost/numeric/conversion/cast.hpp"

#include <cmath>
#include <istream>
#include <stdexcept>

namespace
{
int calc_steps( const int n )
{
  if ( n == 1 )
  {
    return 0;
  }

  const int sqrt     = boost::numeric_cast<int>( std::ceil( std::sqrt( n ) ) );
  const int odd_sqrt = sqrt % 2 ? sqrt - 1 : sqrt;
  const int prev_sqr = ( odd_sqrt - 1 ) * ( odd_sqrt - 1 );
  const int offset   = n - ( prev_sqr + 1 );

  const int rem       = offset % odd_sqrt;
  const int half_sqrt = odd_sqrt / 2;
  return half_sqrt + std::abs( rem + 1 - half_sqrt );
}

using Value    = size_t;
using Coord    = int;
using Pos      = AoC::GenericPoint<Coord>;
using Quadrant = AoC::dd_dynamic_heap_array<Value>;

enum class Dir
{
  Right,
  Up,
  Left,
  Down
};

struct Quadrants
{
  Quadrant left_top;
  Quadrant left_bottom;
  Quadrant right_top;
  Quadrant right_bottom;
};

void resize( Quadrants& quadrants, const size_t n )
{
  const std::initializer_list<std::reference_wrapper<Quadrant>> qs{
    quadrants.left_top, quadrants.left_bottom, quadrants.right_top, quadrants.right_bottom
  };

  for ( auto& quad : qs )
  {
    quad.get().resize( n * 2, n * 2 );
  }
}

template <typename Quads>
auto& get_value( Quads& quads, const Coord x, const Coord y )
{
  const size_t ix = boost::numeric_cast<size_t>( x >= 0 ? x : std::abs( x ) + 1 );
  const size_t iy = boost::numeric_cast<size_t>( y >= 0 ? y : std::abs( y ) + 1 );

  if ( x >= 0 && y >= 0 )
  {
    return quads.right_top[ ix ][ iy ];
  }
  else if ( x < 0 && y >= 0 )
  {
    return quads.left_top[ ix ][ iy ];
  }
  else if ( x < 0 && y < 0 )
  {
    return quads.left_bottom[ ix ][ iy ];
  }
  else if ( x >= 0 && y < 0 )
  {
    return quads.right_bottom[ ix ][ iy ];
  }

  AOC_ASSERT_FALSE();
}

auto get_rotations()
{
  static const std::initializer_list<Dir> rotations = { Dir::Right, Dir::Up, Dir::Left, Dir::Down };
  return rotations | ranges::views::cycle;
}

auto get_steps()
{
  return ranges::views::iota( 1 ) | ranges::views::transform( []( const Coord n ) { return ranges::views::repeat_n( n, 2 ); } ) |
         ranges::views::join;
}

auto get_spiral_steps()
{
  auto spiral_moves = ranges::views::zip( get_rotations(), get_steps() );
  return spiral_moves | ranges::views::transform( []( const auto move ) {
           const auto [ dir, steps_num ] = move;
           return ranges::views::repeat_n( dir, steps_num );
         } ) |
         ranges::views::join;
}

void set_value( Quadrants& quads, const Coord x, const Coord y, const Value value )
{
  get_value( quads, x, y ) = value;
}

Value calc_new_value( Quadrants& quads, const Coord x, const Coord y )
{
  if ( x == 0 && y == 0 )
  {
    return 1;
  }

  const std::initializer_list<Pos> neighbour_positions = { { x - 1, y + 1 }, { x, y + 1 },     { x + 1, y + 1 }, { x - 1, y },
                                                           { x + 1, y },     { x - 1, y - 1 }, { x, y - 1 },     { x + 1, y - 1 } };

  const auto neighbour_values = neighbour_positions | ranges::views::transform( [ &quads ]( const Pos p ) {
                                  const auto [ px, py ] = p;
                                  return get_value( quads, px, py );
                                } );

  return ranges::accumulate( neighbour_values, Value{ 0 } );
}

Value update_value( Quadrants& quads, const Coord x, const Coord y )
{
  resize( quads, boost::numeric_cast<size_t>( std::max( std::abs( x + 2 ), std::abs( y + 2 ) ) ) );
  assert( get_value( quads, x, y ) == 0 );

  const auto new_value = calc_new_value( quads, x, y );
  set_value( quads, x, y, new_value );
  return new_value;
}

Pos get_next_pos( const Pos current_pos, const Dir dir )
{
  const auto [ x, y ] = current_pos;
  switch ( dir )
  {
    case Dir::Up: return { x, y + 1 };
    case Dir::Left: return { x - 1, y };
    case Dir::Down: return { x, y - 1 };
    case Dir::Right: return { x + 1, y };
  }

  AOC_ASSERT_FALSE();
}

template <typename Move>
Pos get_pos( const Pos current_pos, const Move move )
{
  const auto [ dir, steps ] = move;
  return get_next_pos_s( current_pos, dir, steps );
}

}  // namespace

namespace AoC_2017
{

namespace problem_03
{

size_t solve_1( std::istream& input )
{
  int n = 0;
  input >> n;

  return boost::numeric_cast<size_t>( calc_steps( n ) );
}

size_t solve_2( std::istream& input )
{
  Value n = 0;
  input >> n;

  Quadrants quads;
  resize( quads, 10 );

  const Pos initial_pos = { 0, 0 };

  auto spiral_steps        = get_spiral_steps();
  auto positions_to_update = spiral_steps | ranges::views::exclusive_scan( initial_pos, &get_next_pos );

  for ( const auto pos : positions_to_update )
  {
    const auto [ x, y ] = pos;
    const auto val      = update_value( quads, x, y );

    if ( val > n )
    {
      return val;
    }
  }

  AOC_ASSERT_FALSE();
}

AOC_REGISTER_PROBLEM( 2017_03, solve_1, solve_2 );

}  // namespace problem_03

}  // namespace AoC_2017
