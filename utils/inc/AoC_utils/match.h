#pragma once

#include <functional>

#include "boost/callable_traits/args.hpp"

namespace AoC
{

namespace Details
{

template <typename Container, typename Func, std::size_t... I>
auto apply( const Container& container, const Func& func, std::index_sequence<I...> )
{
  return func( container[ I ]... );
}

}  // namespace Details

template <typename Container, typename Func>
auto match_container( const Container& container, const Func& func )
{
  return func( container );
}

template <typename Container, typename Func, typename... Funcs>
auto match_container( const Container& container, const Func& func, const Funcs&... funcs )
{
  const auto arity = std::tuple_size<boost::callable_traits::args_t<Func>>::value;

  if ( container.size() == arity )
  {
    return Details::apply( container, func, std::make_index_sequence<arity>{} );
  }

  return match_container( container, funcs... );
}

}  // namespace AoC
