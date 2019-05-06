#pragma once

#include "boost/fusion/include/at_c.hpp"

#include <tuple>
#include <utility>

namespace AoC
{

namespace Details
{
template <template <typename... Args> class Container, typename... Args, size_t... Indices>
std::tuple<Args...> fusion_to_std_tuple( const Container<Args...>& vector, std::index_sequence<Indices...> )
{
  return std::make_tuple( std::move( boost::fusion::at_c<Indices>( vector ) )... );
}

}  // namespace Details

template <template <typename... Args> class Container, typename... Args>
auto fusion_to_std_tuple( const Container<Args...>& vector )
{
  return Details::fusion_to_std_tuple( vector, std::index_sequence_for<Args...>{} );
}


}  // namespace AoC
