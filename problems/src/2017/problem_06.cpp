#include "AoC/2017/problem_06.h"

#include "AoC/problems_map.h"
#include "AoC/utils/zipper.h"

#include "range/v3/algorithm/max_element.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/istream.hpp"

#include "boost/numeric/conversion/cast.hpp"

#include <iostream>
#include <set>
#include <vector>

namespace
{
using Bank          = size_t;
using Configuration = std::vector<Bank>;

size_t solve( Configuration& configuration )
{
  std::set<std::vector<Bank>> configurations;

  size_t steps = 0;
  while ( configurations.find( configuration ) == configurations.cend() )
  {
    configurations.insert( configuration );
    auto max_elem_iter = ranges::max_element( configuration );

    AoC::Zipper zipper( configuration, max_elem_iter );
    Bank bank        = zipper.current();
    zipper.current() = 0;
    while ( bank > 0 )
    {
      zipper.next();
      ++zipper.current();
      --bank;
    }

    ++steps;
  }

  return steps;
}

Configuration get_configuration( std::istream& input )
{
  return ranges::istream<Bank>( input ) | ranges::to_vector;
}

}  // namespace

namespace AoC_2017
{

namespace problem_06
{

int solve_1( std::istream& input )
{
  auto configuration = get_configuration( input );
  return boost::numeric_cast<int>( solve( configuration ) );
}

int solve_2( std::istream& input )
{
  auto configuration = get_configuration( input );
  solve( configuration );
  return boost::numeric_cast<int>( solve( configuration ) );
}

AOC_REGISTER_PROBLEM( 2017_06, solve_1, solve_2 );

}  // namespace problem_06

}  // namespace AoC_2017
