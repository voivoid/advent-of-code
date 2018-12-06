#include "AoC/2015/problem_14.h"

#include "AoC/problems_map.h"
#include "AoC/utils/parse.h"
#include "AoC/utils/ranges.h"

#include "range/v3/algorithm/max.hpp"
#include "range/v3/getlines.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/to_container.hpp"
#include "range/v3/view/concat.hpp"
#include "range/v3/view/cycle.hpp"
#include "range/v3/view/partial_sum.hpp"
#include "range/v3/view/repeat_n.hpp"
#include "range/v3/view/take_exactly.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/spirit/home/x3.hpp"

#include <functional>
#include <istream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
struct Deer
{
  std::string name;
  unsigned int speed;
  unsigned int time_to_fly;
  unsigned int time_to_rest;
};

using Seconds                 = size_t;
using Distance                = size_t;
constexpr Seconds santas_time = 2503;

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( Deer, name, speed, time_to_fly, time_to_rest )

namespace
{

Deer parse_deer( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  const auto name_parser = x3::lexeme[ +x3::alpha ];
  const auto parser = name_parser > "can" > "fly" > x3::uint_ > "km/s" > "for" > x3::uint_ > "seconds" > "but" > "then" > "must" > "rest" >
                      "for" > x3::uint_ > "seconds";

  Deer deer;
  const bool is_parsed = AoC::x3_parse( line.cbegin(), line.cend(), parser, x3::space | x3::punct, deer );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse deer data" );
  }

  return deer;
}

auto get_time_speed_range( const Deer& deer, const Seconds time_to_travel )
{
  const auto flying_range  = ranges::view::repeat_n( deer.speed, deer.time_to_fly );
  const auto resting_range = ranges::view::repeat_n( 0, deer.time_to_rest );

  return ranges::view::concat( flying_range, resting_range ) | ranges::view::cycle | ranges::view::take_exactly( time_to_travel );
}

Distance calc_distance( const Deer& deer, const Seconds time_to_travel )
{
  auto time_speed_range = get_time_speed_range( deer, time_to_travel );
  return ranges::accumulate( time_speed_range, Seconds{ 0 } );
}

int calc_max_score( const std::vector<Deer>& deers, const Seconds time_to_travel )
{
  const auto distances_by_time = deers |
                                 ranges::view::transform( std::bind( &get_time_speed_range, std::placeholders::_1, time_to_travel ) ) |
                                 ranges::view::transform( ranges::view::partial_sum ) | AoC::transpose() | AoC::to_2d_vector();

  auto scores =
      distances_by_time | ranges::view::transform( []( const auto& distances ) {
        auto max_distance = ranges::max( distances );
        return distances | ranges::view::transform( [max_distance]( const auto& distance ) { return distance == max_distance ? 1 : 0; } );
      } ) |
      AoC::transpose() | ranges::view::transform( []( const auto& deer_scores ) { return ranges::accumulate( deer_scores, 0 ); } );

  return ranges::max( scores );
}

}  // namespace

namespace AoC_2015
{

namespace problem_14
{

int solve_1( std::istream& input )
{
  auto travelled_distances = ranges::getlines( input ) | ranges::view::transform( &parse_deer ) |
                             ranges::view::transform( std::bind( &calc_distance, std::placeholders::_1, santas_time ) );
  return static_cast<int>( ranges::max( travelled_distances ) );
}

int solve_2( std::istream& input )
{
  const auto deers = ranges::getlines( input ) | ranges::view::transform( &parse_deer ) | ranges::to_vector;

  return calc_max_score( deers, santas_time );
}

AOC_REGISTER_PROBLEM( 2015_14, solve_1, solve_2 );

}  // namespace problem_14

}  // namespace AoC_2015


#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  Deer deer1 = parse_deer( "Comet can fly 14 km/s for 10 seconds, but then must rest for 127 seconds." );

  assert( deer1.name == "Comet" );
  assert( deer1.speed = 14 );
  assert( deer1.time_to_fly == 10 );
  assert( deer1.time_to_rest == 127 );

  assert( calc_distance( deer1, 1 ) == 14 );
  assert( calc_distance( deer1, 10 ) == 140 );
  assert( calc_distance( deer1, 11 ) == 140 );
  assert( calc_distance( deer1, 1000 ) == 1120 );

  Deer deer2 = parse_deer( "Dancer can fly 16 km/s for 11 seconds, but then must rest for 162 seconds." );

  assert( calc_distance( deer2, 1000 ) == 1056 );
  assert( calc_max_score( { deer1, deer2 }, 1000 ) == 689 );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
