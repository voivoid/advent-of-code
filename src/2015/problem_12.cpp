#include "AoC/2015/problem_12.h"

#include <AoC/problems_map.h>

#include <algorithm>
#include <istream>
#include <numeric>
#include <stdexcept>

#include <nlohmann/json.hpp>

using mjson = nlohmann::json;

namespace
{
enum mode
{
  basic,
  ignore_red
};

bool has_red_value( const mjson& json )
{
  return std::any_of(
      json.cbegin(), json.cend(), []( const mjson& value ) { return value.is_string() && value.get<std::string>() == "red"; } );
}

template <mode mode>
int calc( const mjson& json )
{
  if ( json.is_number() )
  {
    return json.get<int>();
  }
  else if ( json.is_string() )
  {
    return 0;
  }
  else if ( json.is_array() || json.is_object() )
  {
    if ( mode == ignore_red && json.is_object() && has_red_value( json ) )
    {
      return 0;
    }

    return std::accumulate( json.cbegin(), json.cend(), 0, []( int sum, const mjson& element ) { return sum + calc<mode>( element ); } );
  }

  throw std::runtime_error( "Unexpected json element" );
}

template <mode mode>
int solve( std::istream& input )
{
  mjson json;
  input >> json;

  return calc<mode>( json );
}
}  // namespace


namespace AoC_2015
{

namespace problem_12
{

int solve_1( std::istream& input )
{
  return solve<basic>( input );
}

int solve_2( std::istream& input )
{
  return solve<ignore_red>( input );
}

AOC_REGISTER_PROBLEM( 2015_12, solve_1, solve_2 );

}  // namespace problem_12

}  // namespace AoC_2015
