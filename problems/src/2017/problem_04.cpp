#include "AoC/2017/problem_04.h"

#include "AoC/problems_map.h"

#include "range/v3/action/sort.hpp"
#include "range/v3/algorithm/count_if.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/split.hpp"

#include "boost/numeric/conversion/cast.hpp"

#include <string>
#include <unordered_set>

namespace
{

std::string id( std::string s )
{
  return s;
}

std::string sort_by_letters( std::string s )
{
  return std::move( s ) | ranges::actions::sort;
}

template <std::string ( *rearrange_word )( std::string )>
bool is_secure_passphrase( const std::string& line )
{
  std::unordered_set<std::string> words;
  for ( const auto word_range : line | ranges::views::split( ' ' ) )
  {
    const auto word            = rearrange_word( word_range | ranges::to<std::string> );
    const auto [ _, inserted ] = words.insert( word );
    if ( !inserted )
    {
      return false;
    }
  }

  return true;
}

template <std::string ( *rearrange_word )( std::string )>
size_t calc_secure_passphrases( std::istream& input )
{
  return boost::numeric_cast<size_t>( ranges::count_if( ranges::getlines( input ), &is_secure_passphrase<rearrange_word> ) );
}

}  // namespace

namespace AoC_2017
{

namespace problem_04
{

size_t solve_1( std::istream& input )
{
  return calc_secure_passphrases<&id>( input );
}

size_t solve_2( std::istream& input )
{
  return calc_secure_passphrases<&sort_by_letters>( input );
}

AOC_REGISTER_PROBLEM( 2017_04, solve_1, solve_2 );

}  // namespace problem_04

}  // namespace AoC_2017
