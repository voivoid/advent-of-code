#include "AoC/generators_map.h"

#include <ostream>
#include <random>

#include <range/v3/view/generate.hpp>

#include <algorithm>

namespace AoC_2015
{

namespace problem_01
{

void generate_01( std::ostream& output, const std::size_t size, const size_t seed )
{
  std::mt19937 gen( seed );
  std::uniform_int_distribution dis( 0, 1 );

  auto generator = [&gen, &dis]() { return dis( gen ) == 0 ? '(' : ')'; };
  auto data      = ranges::view::generate( generator );

  std::copy_n( data.begin(), size, std::ostream_iterator<char>( output ) );
}

//    void generate_02( std::ostream& output,const std::size_t size, const size_t seed )
//    {

//    }

AOC_REGISTER_GENERATOR( 2015_01, generate_01, generate_01 );

}  // namespace problem_01

}  // namespace AoC_2015
