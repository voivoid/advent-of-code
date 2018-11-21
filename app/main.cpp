#include "list_problems.h"
#include "parse_cmd_line.h"
#include "solve_problem.h"

#include "boost/functional/overloaded_function.hpp"

#include <iostream>

namespace
{

bool exec_cmd_help( const AocApp::cmd_help& help_cmd )
{
  std::cout << help_cmd.description << std::endl;
  return true;
}

bool exec_cmd_solve_problem( const AocApp::cmd_solve_problem& solve_cmd )
{
  return AocApp::solve_problem( solve_cmd.problem, solve_cmd.repeats );
}

bool exec_cmd_list_problems( const AocApp::cmd_list_problems& )
{
  return AocApp::list_problems();
}

bool exec_cmd_generate_data( const AocApp::cmd_generate_data& )
{
  return true;
}

int safe_main( const int argc, char** const argv )
{
  const auto cmd = AocApp::parse_cmd_args( argc, argv );

  const auto cmd_visitor =
      boost::make_overloaded_function( &exec_cmd_help, &exec_cmd_solve_problem, &exec_cmd_list_problems, &exec_cmd_generate_data );

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
