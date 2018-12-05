#include "AoC/2015/problem_08.h"

#include "AoC/problems_map.h"
#include "AoC/utils/parse.h"


#include "range/v3/getlines.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/spirit/home/x3.hpp"

#include <istream>
#include <stdexcept>
#include <string>

namespace
{
namespace x3 = boost::spirit::x3;

auto inc( const size_t x )
{
  return [x]( auto& ctx ) { x3::_val( ctx ) += x; };
}

auto hex_digit()
{
  return x3::ascii::digit | 'a' | 'b' | 'c' | 'd' | 'e' | 'f';
}

auto make_parser_1()
{
  const auto slash_quote = x3::string( "\\\"" )[ inc( 1 ) ];
  const auto slash_slash = x3::string( "\\\\" )[ inc( 1 ) ];
  const auto escaped_hex = ( x3::string( "\\x" ) >> hex_digit() >> hex_digit() )[ inc( 1 ) ];
  const auto quote       = x3::char_( '"' );
  const auto any_char    = x3::char_[ inc( 1 ) ];

  return *( slash_quote | slash_slash | escaped_hex | quote | any_char );
}

auto make_parser_2()
{
  const auto slash_quote = x3::string( "\\\"" )[ inc( 4 ) ];
  const auto slash_slash = x3::string( "\\\\" )[ inc( 4 ) ];
  const auto escaped_hex = ( x3::string( "\\x" ) >> hex_digit() >> hex_digit() )[ inc( 5 ) ];
  const auto quote       = x3::char_( '"' )[ inc( 3 ) ];
  const auto any_char    = x3::char_[ inc( 1 ) ];

  return *( slash_quote | slash_slash | escaped_hex | quote | any_char );
}

template <typename Parser>
size_t run_parser( const std::string& line, const Parser& parser )
{
  size_t n = 0;

  const auto rule = x3::rule<struct _, size_t>{} = parser;
  const bool is_parsed                           = AoC::x3_parse( line.cbegin(), line.cend(), rule, n );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input line data" );
  }

  return n;
}


size_t calc_encoded_line_len_1( const std::string& line )
{
  const auto n = run_parser( line, make_parser_1() );
  return line.size() - n;
}

size_t calc_encoded_line_len_2( const std::string& line )
{
  const auto n = run_parser( line, make_parser_2() );
  return n - line.size();
}

template <size_t ( *calc_encoded_line_len )( const std::string& )>
size_t calc_lines( std::istream& input )
{
  return ranges::accumulate( ranges::getlines( input ) | ranges::view::transform( calc_encoded_line_len ), size_t( 0 ) );
}

}  // namespace

namespace AoC_2015
{

namespace problem_08
{

int solve_1( std::istream& input )
{
  return static_cast<int>( calc_lines<&calc_encoded_line_len_1>( input ) );
}

int solve_2( std::istream& input )
{
  return static_cast<int>( calc_lines<&calc_encoded_line_len_2>( input ) );
}

AOC_REGISTER_PROBLEM( 2015_08, solve_1, solve_2 );

}  // namespace problem_08

}  // namespace AoC_2015
