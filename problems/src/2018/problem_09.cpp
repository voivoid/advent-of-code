#include "AoC/2018/problem_09.h"

#include "AoC/problems_map.h"
#include "AoC/utils/anon_ret.h"
#include "AoC/utils/parse.h"
#include "AoC/utils/zipper.h"

#include "range/v3/algorithm/max.hpp"

#include "boost/fusion/container/vector.hpp"
#include "boost/numeric/conversion/cast.hpp"


#include <istream>
#include <list>
#include <stdexcept>
#include <string>

namespace
{
using Marble = size_t;

auto parse_input( std::istream& input )
{
  namespace x3 = boost::spirit::x3;

  const auto parser = AoC::x3_size_t_ > "players;" > "last" > "marble" > "is" > "worth" > AoC::x3_size_t_ > "points";

  size_t players, points;
  const bool is_parsed = AoC::x3_parse_m( input, parser, x3::space, players, points );
  if ( !is_parsed )
  {
    throw std::runtime_error( "Failed to parse task input" );
  }

  AOC_RETURN_ANONYMOUS_STRUCT( players, points );
}

size_t solve( const size_t players, const size_t last_marble )
{
  std::list<Marble> marbles = { 0 };
  AoC::Zipper marbles_zipper( marbles );

  std::vector<size_t> scores;
  scores.resize( players );

  for ( size_t i = 1; i <= last_marble; ++i )
  {
    if ( i % 23 == 0 )
    {
      const auto player_num = i % players;

      marbles_zipper.prev( 7 );
      const auto score = i + marbles_zipper.current();
      marbles_zipper.remove();

      scores[ player_num ] += score;
    }
    else
    {
      marbles_zipper.next();
      marbles_zipper.append( i );
      marbles_zipper.next();
    }
  }

  return ranges::max( scores );
}

}  // namespace

namespace AoC_2018
{

namespace problem_09
{

size_t solve_1( std::istream& input )
{
  const auto input_data = parse_input( input );
  return solve( input_data.players, input_data.points );
}

size_t solve_2( std::istream& input )
{
  const auto input_data = parse_input( input );
  return solve( input_data.players, input_data.points * 100 );
}

AOC_REGISTER_PROBLEM( 2018_09, solve_1, solve_2 );

}  // namespace problem_09

}  // namespace AoC_2018
