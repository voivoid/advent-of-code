#include "parse_cmd_line.h"
#include "solve_problem.h"

#include <boost/functional/overloaded_function.hpp>

#include <iostream>



int main( const int argc, char** const argv )
{
  const auto cmd = parse_cmd_args( argc, argv );
  std::visit(
      boost::make_overloaded_function( []( const cmd_help& help_cmd ) { std::cout << help_cmd.description << std::endl; },
                                       []( const cmd_solve_problem& solve_cmd ) { solve_problem( solve_cmd.problem, std::cout ); } ),
      cmd );
  return 0;
}
