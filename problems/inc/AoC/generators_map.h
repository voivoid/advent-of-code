#pragma once

#include <functional>
#include <iosfwd>
#include <optional>
#include <string>

namespace AoC
{
using GeneratorName = std::string;
using GeneratorFunc = std::function<void( std::ostream&, std::size_t, std::size_t )>;

std::optional<GeneratorFunc> get_generator_func( const GeneratorName& name );
void register_solve_func( GeneratorName name, GeneratorFunc func );

namespace Details
{

struct GeneratorRegistrator
{
  GeneratorRegistrator( GeneratorName generator_name, GeneratorFunc gen_1, GeneratorFunc gen_2 )
  {
    register_solve_func( generator_name + "_1", gen_1 );
    register_solve_func( generator_name + "_2", gen_2 );
  }
};

}  // namespace Details

}  // namespace AoC

#define AOC_REGISTER_GENERATOR( generator_name, gen_1, gen_2 )                                                                             \
  static const AoC::Details::GeneratorRegistrator registrator( #generator_name, gen_1, gen_2 )
