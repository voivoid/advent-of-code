#pragma once

#include <string>
#include <variant>

struct cmd_help
{
  std::string description;
};

struct cmd_solve_problem
{
  std::string problem;
};

using parse_result = std::variant<cmd_help, cmd_solve_problem>;
parse_result parse_cmd_args( const int argc, char** argv );
