#pragma once

#include "boost/spirit/home/x3.hpp"

namespace AoC
{

template <typename Iter, typename Parser, typename Attr>
bool x3_parse( Iter begin, Iter end, const Parser& parser, Attr& attr )
{
  const bool parsed = boost::spirit::x3::parse( begin, end, parser, attr );
  return parsed && ( begin == end );
}

template <typename Iter, typename Parser, typename Skipper, typename Attr>
bool x3_parse( Iter begin, Iter end, const Parser& parser, const Skipper& skipper, Attr& attr )
{
  const bool parsed = boost::spirit::x3::phrase_parse( begin, end, parser, skipper, attr );
  return parsed && ( begin == end );
}

constexpr auto x3_size_t_parser = boost::spirit::x3::ulong_;



}  // namespace AoC
