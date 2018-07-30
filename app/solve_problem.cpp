#include "solve_problem.h"

#include <AoC/problems_map.h>

#include <iostream>

namespace
{

struct ResultPrinter
{
  ResultPrinter( std::ostream& output ) : _output( output )
  {
  }

  template <typename T>
  void operator()( const T& value ) const
  {
    _output << value << std::endl;
  }

    private:
  std::ostream& _output;
};

}  // namespace

void solve_problem( const std::string& problem, std::ostream& output )
{
  const auto solve_func = AoC::get_solve_func( problem );
  if ( !solve_func )
  {
    output << "Problem " << problem << " not found" << std::endl;
    return;
  }

  const auto result = ( *solve_func )( std::cin );
  std::visit( ResultPrinter{ output }, result );
}
