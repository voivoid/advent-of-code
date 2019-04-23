#pragma once

#include "boost/spirit/home/x3.hpp"
#include "boost/spirit/include/support_istream_iterator.hpp"

namespace AoC
{

template <typename Iter, typename Parser, typename Skipper, typename Attr>
bool x3_parse( Iter begin, Iter end, const Parser& parser, const Skipper& skipper, Attr& attr )
{
  const bool parsed = boost::spirit::x3::phrase_parse( begin, end, parser, skipper, attr );
  return parsed && ( begin == end );
}

template <typename Parser, typename Skipper, typename Attr>
bool x3_parse( std::istream& input, const Parser& parser, const Skipper& skipper, Attr& attr )
{
  input >> std::noskipws;
  return x3_parse( boost::spirit::istream_iterator( input ), {}, parser, skipper, attr );
}

template <typename Parser, typename Skipper, typename Attr>
bool x3_parse( const std::string& line, const Parser& parser, const Skipper& skipper, Attr& attr )
{
  return x3_parse( line.cbegin(), line.cend(), parser, skipper, attr );
}



template <typename Iter, typename Parser, typename Skipper>
bool x3_parse( Iter begin, Iter end, const Parser& parser, const Skipper& skipper )
{
  const bool parsed = boost::spirit::x3::phrase_parse( begin, end, parser, skipper );
  return parsed && ( begin == end );
}

template <typename Parser, typename Skipper>
bool x3_parse( std::istream& input, const Parser& parser, const Skipper& skipper )
{
  input >> std::noskipws;
  return x3_parse( boost::spirit::istream_iterator( input ), {}, parser, skipper );
}

template <typename Parser, typename Skipper>
bool x3_parse( const std::string& line, const Parser& parser, const Skipper& skipper )
{
  return x3_parse( line.cbegin(), line.cend(), parser, skipper );
}

constexpr auto x3_size_t_ = boost::spirit::x3::ulong_;



}  // namespace AoC
