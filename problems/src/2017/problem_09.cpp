#include "AoC/2017/problem_09.h"

#include "AoC/problems_map.h"
#include "AoC/utils/parse.h"

#include "boost/numeric/conversion/cast.hpp"

#include <istream>
#include <stdexcept>

namespace
{

struct ParseResult
{
  size_t groups_score;
  size_t chars_inside_garbage;
};

ParseResult solve( std::istream& input )
{
  namespace x3 = boost::spirit::x3;

  ParseResult result{};
  size_t group_num = 0;

  const auto calc_garbage_action = [ &result ]( auto& ) { ++result.chars_inside_garbage; };
  const auto group_start_action  = [ &group_num ]( auto& ) { ++group_num; };
  const auto group_end_action    = [ &group_num, &result ]( auto& ) {
    result.groups_score += group_num;
    --group_num;
  };

  const auto cancel      = '!' > x3::char_;
  const auto garbage     = '<' > *( cancel | ( x3::char_ - '>' )[ calc_garbage_action ] ) > '>';
  const auto group_start = x3::lit( '{' )[ group_start_action ];
  const auto group_end   = x3::lit( '}' )[ group_end_action ];

  const auto parser = x3::lexeme[ *( cancel | garbage | group_start | group_end | x3::char_ ) ];

  const bool is_parsed = AoC::x3_parse( input, parser, x3::space );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input groups data" );
  }

  return result;
}

}  // namespace

namespace AoC_2017
{

namespace problem_09
{

size_t solve_1( std::istream& input )
{
  return solve( input ).groups_score;
}

size_t solve_2( std::istream& input )
{
  return solve( input ).chars_inside_garbage;
}

AOC_REGISTER_PROBLEM( 2017_09, solve_1, solve_2 );

}  // namespace problem_09
}  // namespace AoC_2017
