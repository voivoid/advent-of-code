#include "AoC_utils/string.h"

namespace AoC
{

size_t count_substrings( const std::string_view input, const std::string_view substring )
{
  if ( substring.empty() )
  {
    return 0;
  }

  size_t counter                  = 0;
  std::string_view::size_type pos = 0;

  while ( true )
  {
    pos = input.find( substring, pos );
    if ( pos == std::string_view::npos )
    {
      return counter;
    }

    pos += substring.length();
    ++counter;
  }
}

}  // namespace AoC
