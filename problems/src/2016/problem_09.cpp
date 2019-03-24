#include "AoC/2016/problem_09.h"

#include "AoC/problems_map.h"
#include "AoC/utils/fusion.h"
#include "AoC/utils/parse.h"

#include "boost/spirit/home/x3.hpp"

#include "boost/spirit/include/phoenix_core.hpp"
#include "boost/spirit/include/phoenix_operator.hpp"

#include <istream>
#include <stdexcept>
#include <string>
#include <vector>

#include <cassert>

namespace
{
template <bool recursive, typename Iter>
size_t decompress( const Iter begin, const Iter end )
{
  namespace x3  = boost::spirit::x3;
  namespace phx = boost::phoenix;

  size_t to_skip = 0;
  size_t result  = 0;

  const auto decompress_marker = [&to_skip, &result]( auto& ctx ) {
    const auto [ chars, to_repeat ] = AoC::fusion_to_std_tuple( x3::_attr( ctx ) );

    assert( to_skip == 0 );
    to_skip = chars;

    if constexpr ( recursive )
    {
      auto decompress_begin = x3::_where( ctx ).begin();
      auto decompress_end   = decompress_begin + static_cast<ptrdiff_t>( to_skip );
      result += ( decompress<true>( decompress_begin, decompress_end ) * to_repeat );
    }
    else
    {
      result += ( chars * to_repeat );
    }
  };

  const auto skip_parser   = x3::eps( phx::cref( to_skip ) > 0 ) >> x3::char_[ ( [&to_skip]( auto& ) { --to_skip; } ) ];
  const auto letter_parser = x3::alpha[ ( [&result]( auto& ) { ++result; } ) ];
  const auto marker_parser = ( '(' > AoC::x3_size_t_ > 'x' > AoC::x3_size_t_ > ')' )[ decompress_marker ];

  const auto parser = *( skip_parser | letter_parser | marker_parser );

  const bool is_parsed = AoC::x3_parse( begin, end, parser, x3::space );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input data" );
  }

  return result;
}

template <bool recursive>
std::string solve( std::istream& input )
{
  std::string input_str;
  std::getline( input, input_str );

  return std::to_string( decompress<recursive>( input_str.cbegin(), input_str.cend() ) );
}

}  // namespace

namespace AoC_2016
{

namespace problem_09
{

std::string solve_1( std::istream& input )
{
  return solve<false>( input );
}

std::string solve_2( std::istream& input )
{
  return solve<true>( input );
}

AOC_REGISTER_PROBLEM( 2016_09, solve_1, solve_2 );

}  // namespace problem_09

}  // namespace AoC_2016
