#include "AoC/2015/problem_11.h"

#include "AoC/problems_map.h"

#include "range/v3/algorithm/any_of.hpp"
#include "range/v3/algorithm/sort.hpp"
#include "range/v3/to_container.hpp"
#include "range/v3/view/drop.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/group_by.hpp"
#include "range/v3/view/reverse.hpp"
#include "range/v3/view/sliding.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/view/unique.hpp"

#include <functional>
#include <istream>
#include <string>

#include <cassert>

namespace
{
using Password = std::string;

void assert_char( [[maybe_unused]] const char c )
{
  assert( c >= 'a' && c <= 'z' );
}

char next_char( char c )
{
  assert_char( c );
  return c == 'z' ? 'a' : ++c;
}

bool has_incrementing_letters( const Password& password )
{
  const auto triples = password | ranges::view::sliding( 3 );

  for ( const auto triple : triples )
  {
    const auto [ a, b, c ] = std::tie( triple[ 0 ], triple[ 1 ], triple[ 2 ] );
    assert_char( a );
    assert_char( b );
    assert_char( c );

    if ( a == b - 1 && a == c - 2 )
      return true;
  }

  return false;
}

bool has_no_forbidden_letters( const Password& password )
{
  return password.find_first_of( "iol" ) == std::string::npos;
}

bool has_two_letter_pairs( const Password& password )
{
  auto same_letters = password | ranges::view::group_by( std::equal_to<char>{} ) |
                      ranges::view::filter( []( const auto same_letters_group ) { return ranges::distance( same_letters_group ) >= 2; } ) |
                      ranges::view::transform( []( const auto same_letters_group ) { return same_letters_group[ 0 ]; } ) |
                      ranges::to_vector;

  ranges::sort( same_letters );

  return ranges::distance( same_letters | ranges::view::unique ) >= 2;
}

bool is_good_password( const Password& password )
{
  return has_incrementing_letters( password ) && has_no_forbidden_letters( password ) && has_two_letter_pairs( password );
}

Password& update_to_next_password( Password& password )
{
  for ( auto& c : ranges::view::reverse( password ) )
  {
    c = next_char( c );
    if ( c != 'a' )
    {
      return password;
    }
  }

  return password.insert( 0, 1, 'a' );
}

std::string solve( Password password )
{
  do
  {
    update_to_next_password( password );
  } while ( !is_good_password( password ) );

  return password;
}
}  // namespace

namespace AoC_2015
{

namespace problem_11
{

std::string solve_1( std::istream& input )
{
  Password old_password;
  input >> old_password;

  return solve( old_password );
}

std::string solve_2( std::istream& input )
{
  Password old_password;
  input >> old_password;

  return solve( solve( old_password ) );
}

AOC_REGISTER_PROBLEM( 2015_11, solve_1, solve_2 );

}  // namespace problem_11

}  // namespace AoC_2015


#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  Password p1 = "a";
  assert( "b" == update_to_next_password( p1 ) );

  Password p2 = "b";
  assert( "c" == update_to_next_password( p2 ) );

  Password p3 = "z";
  assert( "aa" == update_to_next_password( p3 ) );

  Password p4 = "zzz";
  assert( "aaaa" == update_to_next_password( p4 ) );

  assert( has_incrementing_letters( "abc" ) );
  assert( has_incrementing_letters( "xxabc" ) );
  assert( has_incrementing_letters( "abcxx" ) );
  assert( !has_incrementing_letters( "acc" ) );

  assert( has_no_forbidden_letters( "abc" ) );
  assert( !has_no_forbidden_letters( "aboc" ) );

  assert( has_two_letter_pairs( "abcddefgghi" ) );
  assert( !has_two_letter_pairs( "abcddefg" ) );

  assert( is_good_password( "abcdffaa" ) );
  assert( is_good_password( "ghjaabcc" ) );
  assert( !is_good_password( "hijklmmn" ) );
  assert( !is_good_password( "abbceffg" ) );
  assert( !is_good_password( "abbcegjk" ) );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
