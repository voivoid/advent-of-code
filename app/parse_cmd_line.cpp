#include "parse_cmd_line.h"

#include "boost/program_options.hpp"

#include <sstream>

parse_result parse_cmd_args( const int argc, char** argv )
{
  boost::program_options::options_description description( "Options" );
  description.add_options()( "help", "show help message" )( "problem", boost::program_options::value<std::string>(), "solve problem" );

  boost::program_options::positional_options_description po_description;
  po_description.add( "problem", -1 );

  boost::program_options::variables_map vars_map;
  boost::program_options::store(
      boost::program_options::command_line_parser( argc, argv ).options( description ).positional( po_description ).run(), vars_map );
  boost::program_options::notify( vars_map );

  if ( vars_map.count( "problem" ) )
  {
    return cmd_solve_problem{ vars_map[ "problem" ].as<std::string>() };
  }

  std::ostringstream ss;
  description.print( ss );
  return cmd_help{ ss.str() };
}
