#include "AoC/generators_map.h"

#include <ostream>
#include <random>

#include "range/v3/algorithm/copy.hpp"
#include "range/v3/view/generate_n.hpp"

namespace AoC_2015
{

namespace problem_01
{

void generate_01( std::ostream& /*output*/, const std::size_t /*output_size*/, const size_t /*seed*/ )
{
  //  std::mt19937 gen( seed );
  //  std::uniform_int_distribution dis( 0, 1 );

  //  auto generator = [&gen, &dis]() { return dis( gen ) == 0 ? '(' : ')'; };
  //  auto data      = ranges::view::generate_n( generator, output_size );

  //  ranges::copy( data, std::ostream_iterator<char>( output ) );
}

void generate_02( std::ostream& /*output*/, const std::size_t /*output_size*/, const size_t /*seed*/ )
{
}
AOC_REGISTER_GENERATOR( 2015_01, generate_01, generate_02 );

}  // namespace problem_01

}  // namespace AoC_2015
