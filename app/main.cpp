#include "parse_cmd_line.h"
#include "solve_problem.h"

#include <boost/functional/overloaded_function.hpp>

#include <iostream>

namespace
{

bool exec_cmd_help( const cmd_help& help_cmd )
{
  std::cout << help_cmd.description << std::endl;
  return true;
}

bool exec_solve_problem( const cmd_solve_problem& solve_cmd )
{
  return solve_problem( solve_cmd.problem );
}

int safe_main( const int argc, char** const argv )
{
  const auto cmd = parse_cmd_args( argc, argv );

  const auto cmd_visitor = boost::make_overloaded_function( &exec_cmd_help, &exec_solve_problem );

  const bool success = std::visit( cmd_visitor, cmd );
  return success ? 0 : 1;
}

}  // namespace

int main( const int argc, char** const argv )
{
  try
  {
    return safe_main( argc, argv );
  }
  catch ( const std::exception& ex )
  {
    std::cerr << "Unexpected exception: " << ex.what() << std::endl;
  }
  catch ( ... )
  {
    std::cerr << "Unexpected unknown exception" << std::endl;
  }

  return 1;
}
