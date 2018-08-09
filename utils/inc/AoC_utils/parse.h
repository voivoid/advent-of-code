#pragma once

#include <boost/spirit/home/x3.hpp>

namespace AoC
{

template <typename Iter, typename Parser, typename Attr>
bool x3_parse( Iter begin, Iter end, const Parser& parser, Attr& attr )
{
  const bool parsed = boost::spirit::x3::parse( begin, end, parser, attr );
  return parsed && ( begin == end );
}

}  // namespace AoC
