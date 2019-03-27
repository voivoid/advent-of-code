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
  const auto indices = ranges::view::iota( size_t{ 0 } );
  return *ranges::find_if( indices, std::bind( &is_desired_md5_index, std::placeholders::_1, std::ref( prefix ), std::ref( secret ) ) );
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
