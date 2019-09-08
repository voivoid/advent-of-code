#include "parse_cmd_line.h"

#include "boost/program_options.hpp"

#include <sstream>

namespace AocApp
{

parse_result parse_cmd_args( const int argc, char** const argv )
{
  boost::program_options::options_description description( "Options" );
  description.add_options()( "help", "show help message" )( "problem", boost::program_options::value<std::string>(), "solve problem" )(
      "repeats", boost::program_options::value<size_t>()->default_value( size_t{ 1 } ) )( "list", "show list of solved problems" )(
      "generate", boost::program_options::value<std::string>(), "generate data for problems" )(
      "seed", boost::program_options::value<size_t>(), "seed for data generator" )(
      "size", boost::program_options::value<size_t>(), "size of data to be generated" );

  boost::program_options::positional_options_description po_description;
  po_description.add( "problem", -1 );

  boost::program_options::variables_map vars_map;
  boost::program_options::store(
      boost::program_options::command_line_parser( argc, argv ).options( description ).positional( po_description ).run(), vars_map );
  boost::program_options::notify( vars_map );

  if ( vars_map.count( "list" ) )
  {
    return cmd_list_problems{};
  }

  if ( vars_map.count( "generate" ) )
  {
    return cmd_generate_data{ vars_map[ "generate" ].as<std::string>(), vars_map[ "seed" ].as<size_t>(), vars_map[ "size" ].as<size_t>() };
  }

  if ( vars_map.count( "problem" ) )
  {
    return cmd_solve_problem{ vars_map[ "problem" ].as<std::string>(), vars_map[ "repeats" ].as<size_t>() };
  }

  std::ostringstream ss;
  description.print( ss );
  return cmd_help{ ss.str() };
}

}  // namespace AocApp
