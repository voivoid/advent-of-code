#include "AoC/2015/problem_04.h"

#include <AoC/problems_map.h>
#include <AoC_utils/md5.h>

#include <boost/algorithm/string/predicate.hpp>

#include <istream>
#include <string>

namespace
{
std::string make_md5( const std::string& secret, size_t index )
{
  return AoC::md5( secret + std::to_string( index ) );
}

size_t find_md5_that_starts_with( const std::string& prefix, const std::string& secret )
{
  size_t index = 0;
  for ( ;; ++index )
  {
    const auto md5 = make_md5( secret, index );
    if ( boost::algorithm::starts_with( md5, prefix ) )
    {
      break;
    }
  }

  return index;
}

int solve( std::istream& input, const std::string& prefix )
{
  std::string secret;
  input >> secret;

  return static_cast<int>( find_md5_that_starts_with( prefix, secret ) );
}
}  // namespace

namespace AoC_2015
{

namespace problem_04
{

int solve_1( std::istream& input )
{
  return solve( input, "00000" );
}

int solve_2( std::istream& input )
{
  return solve( input, "000000" );
}

AOC_REGISTER_PROBLEM( 2015_04, solve_1, solve_2 );

}  // namespace problem_04

}  // namespace AoC_2015
