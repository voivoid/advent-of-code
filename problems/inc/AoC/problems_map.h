#pragma once

#include <functional>
#include <iosfwd>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace AoC
{
using ProblemName   = std::string;
using ProblemResult = std::variant<int, size_t, std::string>;
using ProblemFunc   = std::function<ProblemResult( std::istream& )>;

std::vector<ProblemName> get_solved_problems_list();
std::optional<ProblemFunc> get_solve_func( const ProblemName& name );
void register_solve_func( ProblemName name, ProblemFunc func );

namespace Details
{

struct ProblemRegistrator
{
  ProblemRegistrator( ProblemName problem_name, ProblemFunc solve_1, ProblemFunc solve_2 )
  {
    register_solve_func( problem_name + "_1", solve_1 );
    register_solve_func( problem_name + "_2", solve_2 );
  }
};

}  // namespace Details

}  // namespace AoC

#define AOC_REGISTER_PROBLEM( problem_name, solve_1, solve_2 )                                                                             \
  static const AoC::Details::ProblemRegistrator registrator( #problem_name, solve_1, solve_2 )
