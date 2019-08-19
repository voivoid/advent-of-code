#include "AoC/2016/problem_05.h"

#include "AoC/problems_map.h"
#include "AoC/utils/md5.h"

#include "range/v3/view/filter.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/take.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/algorithm/string/predicate.hpp"
#include "boost/numeric/conversion/cast.hpp"

#include <functional>
#include <istream>
#include <string>

namespace
{
[[maybe_unused]] bool is_hex_digit( const char c )
{
  return c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f';
}

size_t hex_digit_to_num( const char c )
{
  assert( isdigit( c ) || is_hex_digit( c ) );

  return boost::numeric_cast<size_t>( isdigit( c ) ? c - '0' : 10 + c - 'a' );
}

std::string make_md5( const size_t index, const std::string& door_id )
{
  return AoC::md5( door_id + std::to_string( index ) );
}

bool is_valid_md5s( const std::string& md5 )
{
  return boost::algorithm::starts_with( md5, "00000" );
}

std::string get_door_id( std::istream& input )
{
  std::string door_id;
  input >> door_id;
  return door_id;
}

auto get_valid_md5s( const std::string& door_id )
{
  const auto md5s = ranges::views::iota( 0 ) | ranges::views::transform( std::bind( &make_md5, std::placeholders::_1, std::ref( door_id ) ) );
  const auto valid_md5s = md5s | ranges::views::filter( &is_valid_md5s );

  return valid_md5s;
}

}  // namespace

namespace AoC_2016
{

namespace problem_05
{

std::string solve_1( std::istream& input )
{
  const std::string door_id = get_door_id( input );
  auto valid_md5s           = get_valid_md5s( door_id );

  auto valid_md5s_sixths_chars = valid_md5s | ranges::views::transform( []( const auto& md5 ) { return md5[ 5 ]; } );

  return valid_md5s_sixths_chars | ranges::views::take( 8 ) | ranges::to<std::string>;
}

std::string solve_2( std::istream& input )
{
  const std::string door_id = get_door_id( input );
  auto valid_md5s           = get_valid_md5s( door_id );

  size_t chars_found    = 0;
  const char empty_char = '_';
  std::string result( 8, empty_char );

  for ( const auto& md5 : valid_md5s )
  {
    if ( chars_found == 8 )
    {
      break;
    }

    const size_t result_char_index = hex_digit_to_num( md5[ 5 ] );
    if ( result_char_index >= 8 )
    {
      continue;
    }

    const char result_char = md5[ 6 ];
    if ( result[ result_char_index ] == empty_char )
    {
      ++chars_found;
      result[ result_char_index ] = result_char;
    }
  }

  return result;
}

AOC_REGISTER_PROBLEM( 2016_05, solve_1, solve_2 );

}  // namespace problem_05

}  // namespace AoC_2016
