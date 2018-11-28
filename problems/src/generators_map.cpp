#include "AoC/generators_map.h"

#include <map>

#include <cassert>

namespace
{
using GeneratorsMap = std::map<AoC::GeneratorName, AoC::GeneratorFunc>;
GeneratorsMap& get_generators_map()
{
  static GeneratorsMap problems_map;
  return problems_map;
}
}  // namespace

namespace AoC
{

std::optional<GeneratorFunc> get_generator_func( const GeneratorName& name )
{
  const auto& map = get_generators_map();
  auto func_iter  = map.find( name );
  if ( func_iter != map.cend() )
  {
    return func_iter->second;
  }

  return {};
}

void register_solve_func( GeneratorName name, GeneratorFunc func )
{
  auto& map = get_generators_map();
  assert( map.find( name ) == map.cend() );
  map.emplace( std::move( name ), func );
}

}  // namespace AoC
