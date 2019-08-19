#pragma once

#include "range/v3/view/all.hpp"
#include "range/v3/view/any_view.hpp"
#include "range/v3/view/concat.hpp"
#include "range/v3/view/empty.hpp"
#include "range/v3/view/tail.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/coroutine2/all.hpp"

namespace AoC
{

// template <typename T, typename Range>
// auto get_unique_pair_combinations( Range range )
//{
//  using Coro = boost::coroutines2::coroutine<std::pair<T, T>>;
//  typename Coro::pull_type coro( [range = std::move( range )]( typename Coro::push_type& yield ) {
//    auto iter1 = range.begin();
//    while ( iter1 != range.end() )
//    {
//      auto iter2 = iter1;
//      while ( ++iter2 != range.end() )
//      {
//        yield( std::make_pair( *iter1, *iter2 ) );
//      }

//      ++iter1;
//    }

//    return;
//  } );
//  return coro;
//}

template <typename T>
ranges::any_view<std::pair<T, T>> get_unique_pair_combinations( ranges::any_view<T> elems )
{
  if ( elems.begin() == elems.end() )
  {
    return ranges::views::empty<std::pair<T, T>>;
  }

  const auto& head = *elems.begin();
  auto tail        = elems | ranges::views::tail;

  auto head_combinations = tail | ranges::views::transform( [head]( const auto& e ) { return std::make_pair( head, e ); } );
  auto tail_combinations = get_unique_pair_combinations<T>( tail );

  return ranges::views::concat( head_combinations, tail_combinations );
}

}  // namespace AoC
