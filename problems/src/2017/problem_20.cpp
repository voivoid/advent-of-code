#include "AoC/2017/problem_20.h"

#include "AoC/problems_map.h"
#include "AoC/utils/parse.h"
#include "AoC/utils/ranges/enumerate.h"
#include "AoC/utils/ranges/unique_pair_combinations.h"

#include "range/v3/algorithm/min.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/hof/lift.hpp"
#include "boost/numeric/conversion/cast.hpp"

#include <tuple>

namespace
{
using Coord = int;
struct Vec
{
  Coord x;
  Coord y;
  Coord z;
};

struct Particle
{
  Vec pos;
  Vec vel;
  Vec acc;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( Vec, x, y, z )
BOOST_FUSION_ADAPT_STRUCT( Particle, pos, vel, acc )

namespace
{

Particle parse_particle( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  const auto vec_parser = x3::rule<struct _vec, Vec>{} = '<' > x3::int_ > ',' > x3::int_ > ',' > x3::int_ > '>';
  const auto pos_parser                                = 'p' > ( '=' > vec_parser );
  const auto vel_parser                                = 'v' > ( '=' > vec_parser );
  const auto acc_parser                                = 'a' > ( '=' > vec_parser );

  const auto parser = pos_parser > ',' > vel_parser > ',' > acc_parser;

  Particle particle;
  const bool is_parsed = AoC::x3_parse( line, parser, x3::space, particle );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input particle data" );
  }

  return particle;
}

bool has_same_sign( const Coord c1, const Coord c2 )
{
  return ( c1 <= 0 && c2 <= 0 ) || ( c1 >= 0 && c2 >= 0 );
}

Coord calc_abs( const Coord c1, const Coord c2 )
{
  const auto abs = std::abs( c1 );
  return has_same_sign( c1, c2 ) ? abs : -abs;
}

Coord calc_vec_sum( const Vec vec, const Vec acc )
{
  return calc_abs( vec.x, acc.x ) + calc_abs( vec.y, acc.y ) + calc_abs( vec.z, acc.z );
}

using Projection = std::tuple<Coord, Coord, Coord>;
Projection calc_distance_projection( const Particle& p )
{
  const auto acc_sum = calc_vec_sum( p.acc, p.acc );
  const auto vel_sum = calc_vec_sum( p.vel, p.acc );
  const auto pos_sum = calc_vec_sum( p.pos, p.acc );

  return { acc_sum, vel_sum, pos_sum };
}

auto get_particles( std::istream& input )
{
  return ranges::getlines( input ) | ranges::views::transform( &parse_particle );
}

}  // namespace


namespace AoC_2017
{

namespace problem_20
{

size_t solve_1( std::istream& input )
{
  auto particles        = get_particles( input );
  auto projs            = particles | ranges::views::transform( &calc_distance_projection );
  auto enumerated_projs = projs | AoC::enumerate_with_<size_t>();

  const auto less_distant_particle =
      ranges::min( enumerated_projs,
                   std::less<Projection>{},
                   std::bind( &ranges::common_pair<long unsigned int, Projection>::second, std::placeholders::_1 ) );
  const size_t particle_index = less_distant_particle.first;

  return particle_index;
}

size_t solve_2( std::istream& )
{
  //  const auto particles = get_particles( input ) | ranges::to_vector;
  //  auto cs              = AoC::get_unique_pair_combinations<Particle>( particles );

  //  return static_cast<size_t>( ranges::distance( cs ) );
  return 0;
}

AOC_REGISTER_PROBLEM( 2017_20, solve_1, solve_2 );

}  // namespace problem_20
}  // namespace AoC_2017

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  {
    const auto p = parse_particle( "p=< 1,2,3>, v=< 4,5,6>, a=<-1,-2,-3>" );
    assert( p.pos.x == 1 );
    assert( p.pos.y == 2 );
    assert( p.pos.z == 3 );

    assert( p.vel.x == 4 );
    assert( p.vel.y == 5 );
    assert( p.vel.z == 6 );

    assert( p.acc.x == -1 );
    assert( p.acc.y == -2 );
    assert( p.acc.z == -3 );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
