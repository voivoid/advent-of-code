#pragma once

#include <functional>

namespace AoC
{

namespace Details
{

template <typename Container, typename Func, std::size_t... I>
auto apply( const Container& container, const Func& func, std::index_sequence<I...> )
{
  return func( container[ I ]... );
}

template <typename T>
std::integral_constant<int, 1> arity( std::function<void( T )> )
{
  return {};
}
template <typename T>
std::integral_constant<int, 2> arity( std::function<void( T, T )> )
{
  return {};
}
template <typename T>
std::integral_constant<int, 3> arity( std::function<void( T, T, T )> )
{
  return {};
}
template <typename T>
std::integral_constant<int, 4> arity( std::function<void( T, T, T, T )> )
{
  return {};
}
template <typename T>
std::integral_constant<int, 5> arity( std::function<void( T, T, T, T, T )> )
{
  return {};
}
template <typename T>
std::integral_constant<int, 6> arity( std::function<void( T, T, T, T, T, T )> )
{
  return {};
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
  using T     = typename Container::value_type;
  using Arity = decltype( Details::arity<T>( func ) );

  if ( container.size() == Arity::value )
  {
    return Details::apply( container, func, std::make_index_sequence<Arity::value>{} );
  }

  return match_container( container, funcs... );
}

}  // namespace AoC
