#pragma once

#include "boost/spirit/home/x3.hpp"
#include "boost/spirit/include/support_istream_iterator.hpp"

namespace AoC
{

template <typename Parser, typename Skipper, typename Attr>
bool x3_parse( std::istream& input, const Parser& parser, const Skipper& skipper, Attr& attr )
{
  input >> std::noskipws;
  auto begin = boost::spirit::istream_iterator( input );
  auto end   = boost::spirit::istream_iterator();

  const bool parsed = boost::spirit::x3::phrase_parse( begin, end, parser, skipper, attr );
  return parsed && ( begin == end );
}

template <typename Iter, typename Parser, typename Skipper, typename Attr>
bool x3_parse( Iter begin, Iter end, const Parser& parser, const Skipper& skipper, Attr& attr )
{
  const bool parsed = boost::spirit::x3::phrase_parse( begin, end, parser, skipper, attr );
  return parsed && ( begin == end );
}

template <typename Parser, typename Skipper>
bool x3_parse( std::istream& input, const Parser& parser, const Skipper& skipper )
{
  input >> std::noskipws;
  auto begin = boost::spirit::istream_iterator( input );
  auto end   = boost::spirit::istream_iterator();

  const bool parsed = boost::spirit::x3::phrase_parse( begin, end, parser, skipper );
  return parsed && ( begin == end );
}

template <typename Iter, typename Parser, typename Skipper>
bool x3_parse( Iter begin, Iter end, const Parser& parser, const Skipper& skipper )
{
  const bool parsed = boost::spirit::x3::phrase_parse( begin, end, parser, skipper );
  return parsed && ( begin == end );
}

constexpr auto x3_size_t_ = boost::spirit::x3::ulong_;



}  // namespace AoC
