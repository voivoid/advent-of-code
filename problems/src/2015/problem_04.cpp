#include "AoC/2015/problem_04.h"

#include "AoC/problems_map.h"
#include "AoC/utils/md5.h"

#include "boost/algorithm/string/predicate.hpp"

#include "range/v3/algorithm/find_if.hpp"
#include "range/v3/view/iota.hpp"

#include <functional>
#include <istream>
#include <string>

namespace
{
std::string make_md5( const std::string& secret, const size_t index )
{
  return AoC::md5( secret + std::to_string( index ) );
}

bool is_desired_md5_index( const size_t index, const std::string& prefix, const std::string& secret )
{
  const auto md5 = make_md5( secret, index );
  return boost::algorithm::starts_with( md5, prefix );
}

size_t find_md5_that_starts_with( const std::string& prefix, const std::string& secret )
{
  const auto indices = ranges::views::iota( size_t{ 0 } );
  return *ranges::find_if( indices, std::bind( &is_desired_md5_index, std::placeholders::_1, std::ref( prefix ), std::ref( secret ) ) );
}

size_t find_key_postfix_producing_hash_starting_with( const std::string& prefix, std::istream& input )
{
  std::string secret;
  input >> secret;

  return find_md5_that_starts_with( prefix, secret );
}
}  // namespace

namespace AoC_2015
{

namespace problem_04
{

size_t solve_1( std::istream& input )
{
  return find_key_postfix_producing_hash_starting_with( "00000", input );
}

size_t solve_2( std::istream& input )
{
  return find_key_postfix_producing_hash_starting_with( "000000", input );
}

AOC_REGISTER_PROBLEM( 2015_04, solve_1, solve_2 );

}  // namespace problem_04

}  // namespace AoC_2015
