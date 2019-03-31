#include "AoC/2015/problem_05.h"

#include "range/v3/algorithm/adjacent_find.hpp"
#include "range/v3/algorithm/any_of.hpp"
#include "range/v3/algorithm/count_if.hpp"
#include "range/v3/getlines.hpp"
#include "range/v3/view/drop.hpp"
#include "range/v3/view/sliding.hpp"
#include "range/v3/view/zip.hpp"

#include "AoC/problems_map.h"

#include "boost/algorithm/cxx11/any_of.hpp"
#include "boost/numeric/conversion/cast.hpp"

#include <istream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace
{

template <bool ( *is_good_line )( const std::string& )>
int solve( std::istream& input )
{
  auto lines = ranges::getlines( input );
  return boost::numeric_cast<int>( ranges::count_if( lines, is_good_line ) );
}

bool is_vowel( const char c )
{
  static const auto vowels = { 'a', 'e', 'i', 'o', 'u' };
  return boost::algorithm::any_of_equal( vowels, c );
}

bool contains_at_least_3_vowels( const std::string& s )
{
  return ranges::count_if( s, &is_vowel ) >= 3;
}

bool has_double_letter( const std::string& s )
{
  return ranges::adjacent_find( s ) != ranges::end( s );
}

bool has_no_forbidden_sequences( const std::string& s )
{
  const auto pairs = ranges::view::zip( s, s | ranges::view::drop( 1 ) );

  static const auto forbidden_pairs = {
    std::make_pair( 'a', 'b' ), std::make_pair( 'c', 'd' ), std::make_pair( 'p', 'q' ), std::make_pair( 'x', 'y' )
  };

  const auto is_forbidden_pair = []( const auto& pair ) { return boost::algorithm::any_of_equal( forbidden_pairs, pair ); };

  return !ranges::any_of( pairs, is_forbidden_pair );
}

bool has_repeating_pairs_of_letters( const std::string_view& s )
{
  if ( s.size() < 2 )
    return false;

  const auto first_two_letters = s.substr( 0, 2 );
  const auto rest_string       = s.substr( 2 );

  const bool pair_found = rest_string.find( first_two_letters ) != std::string::npos;

  return pair_found ? true : has_repeating_pairs_of_letters( s.substr( 1 ) );
}

bool has_a_letter_between_same_letters( const std::string& s )
{
  const auto triples                     = s | ranges::view::sliding( 3 );
  const auto are_first_and_last_the_same = []( const auto t ) { return t.front() == t.back(); };
  return ranges::any_of( triples, are_first_and_last_the_same );
}

bool is_good_line_1( const std::string& s )
{
  return contains_at_least_3_vowels( s ) && has_double_letter( s ) && has_no_forbidden_sequences( s );
}

bool is_good_line_2( const std::string& s )
{
  return has_repeating_pairs_of_letters( s ) && has_a_letter_between_same_letters( s );
}

}  // namespace

namespace AoC_2015
{

namespace problem_05
{

int solve_1( std::istream& input )
{
  return solve<&is_good_line_1>( input );
}

int solve_2( std::istream& input )
{
  return solve<&is_good_line_2>( input );
}

AOC_REGISTER_PROBLEM( 2015_05, solve_1, solve_2 );

}  // namespace problem_05

}  // namespace AoC_2015
