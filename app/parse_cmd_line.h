#pragma once

#include <string>
#include <variant>

namespace AocApp
{

struct cmd_help
{
  std::string description;
};

struct cmd_solve_problem
{
  std::string problem;
  size_t repeats;
};

struct cmd_list_problems
{
};

struct cmd_generate_data
{
  std::string problem;
  size_t seed;
  size_t size;
};

using parse_result = std::variant<cmd_help, cmd_solve_problem, cmd_list_problems, cmd_generate_data>;
parse_result parse_cmd_args( int argc, char** const argv );

}  // namespace AocApp
