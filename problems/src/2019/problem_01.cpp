#include "AoC/2019/problem_01.h"

#include "AoC/problems_map.h"
#include "AoC/utils/ranges/generate.h"

#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/istream.hpp"
#include "range/v3/view/take_while.hpp"
#include "range/v3/view/transform.hpp"

#include <istream>

namespace
{
int calcFuel( const int mass )
{
  return mass / 3 - 2;
}

int calcFuelRecursive( const int mass )
{
  auto fuelSeries = AoC::generate_range( mass, &calcFuel ) | ranges::views::take_while( []( const auto& m ) { return m > 0; } );
  return ranges::accumulate( fuelSeries, 0 );
}

template <int ( *calc )( int )>
int solve( std::istream& input )
{
  auto fuelPerModule = ranges::istream<int>( input ) | ranges::views::transform( calc );
  return ranges::accumulate( fuelPerModule, 0 );
}
}  // namespace

namespace AoC_2019
{

namespace problem_01
{

int solve_1( std::istream& input )
{
  return solve<&calcFuel>( input );
}

int solve_2( std::istream& input )
{
  return solve<&calcFuelRecursive>( input );
}

AOC_REGISTER_PROBLEM( 2019_01, solve_1, solve_2 );

}  // namespace problem_01

}  // namespace AoC_2019
