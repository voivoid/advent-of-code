#pragma once

#include <functional>
#include <iosfwd>
#include <map>
#include <optional>
#include <string>
#include <variant>

namespace AoC
{
using ProblemName   = std::string;
using ProblemResult = std::variant<int, std::string>;
using ProblemFunc   = std::function<ProblemResult( std::istream& )>;

std::optional<ProblemFunc> get_solve_func( const ProblemName& name );
void register_solve_func( ProblemName name, ProblemFunc func );

namespace Details
{

struct ProblemRegistrator
{
  ProblemRegistrator( std::string problem_name, ProblemFunc solve_1, ProblemFunc solve_2 )
  {
    register_solve_func( problem_name + "_1", solve_1 );
    register_solve_func( problem_name + "_2", solve_2 );
  }
};

}  // namespace Details

}  // namespace AoC

#define AOC_REGISTER_PROBLEM( problem_name, solve_1, solve_2 )                                                                             \
  static const AoC::Details::ProblemRegistrator registrator( #problem_name, solve_1, solve_2 )
