#include "AoC/problems_map.h"

#include <cassert>

namespace
{
using ProblemsMap = std::map<AoC::ProblemName, AoC::ProblemFunc>;
ProblemsMap& get_problems_map()
{
  static ProblemsMap problems_map;
  return problems_map;
}
}  // namespace

namespace AoC
{

std::optional<ProblemFunc> get_solve_func( const ProblemName& name )
{
  const auto& map       = get_problems_map();
  auto        func_iter = map.find( name );
  if ( func_iter != map.cend() )
  {
    return func_iter->second;
  }

  return {};
}

void register_solve_func( ProblemName name, ProblemFunc func )
{
  auto& map = get_problems_map();
  assert( map.find( name ) == map.cend() );
  map.emplace( std::move( name ), func );
}

}  // namespace AoC
