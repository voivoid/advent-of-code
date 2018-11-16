#include "solve_problem.h"

#include "AoC/problems_map.h"

#include <iostream>

bool solve_problem( const std::string& problem )
{
  const auto solve_func = AoC::get_solve_func( problem );
  if ( !solve_func )
  {
    std::cerr << "Problem " << problem << " not found" << std::endl;
    return false;
  }

  try
  {
    const auto result = ( *solve_func )( std::cin );
    std::visit( []( const auto& output ) { std::cout << output << std::endl; }, result );
  }
  catch ( const std::exception& ex )
  {
    std::cerr << "Failed to solve the problem due to: " << ex.what() << std::endl;
    return false;
  }

  return true;
}
