#pragma once

#include "range/v3/algorithm/for_each.hpp"
#include "range/v3/range/access.hpp"
#include "range/v3/range/traits.hpp"
#include "range/v3/view/facade.hpp"
#include "range/v3/view/subrange.hpp"
#include "range/v3/view/transform.hpp"

#include <algorithm>
#include <vector>

namespace AoC
{

namespace details
{

// Flattens a range of ranges by iterating the inner
// ranges in round-robin fashion.
template <class Rngs>
class interleave_view : public ranges::view_facade<interleave_view<Rngs>>
{
  friend ranges::range_access;
  std::vector<ranges::range_value_t<Rngs>> rngs_;
  struct cursor;
  cursor begin_cursor()
  {
    return { 0, &rngs_, rngs_ | ranges::view::transform( ranges::begin ) | ranges::to<std::vector> };
  }

public:
  interleave_view() = default;
  explicit interleave_view( Rngs rngs ) : rngs_( std::move( rngs ) | ranges::to<std::vector> )
  {
  }
};

template <class Rngs>
struct interleave_view<Rngs>::cursor
{
  using RngVal = ranges::range_value_t<Rngs>;

  std::size_t n_;
  std::vector<RngVal>* rngs_;
  std::vector<ranges::iterator_t<RngVal>> its_;

  auto read() const
  {
    return *its_[ n_ ];
  }

  void next()
  {
    if ( 0 == ( ( ++n_ ) %= its_.size() ) )
      ranges::for_each( its_, []( auto& it ) { ++it; } );
  }

  bool equal( ranges::default_sentinel_t ) const
  {
    if ( n_ != 0 )
    {
      return false;
    }

    auto ends = *rngs_ | ranges::view::transform( ranges::end );
    return its_.end() != std::mismatch( its_.begin(), its_.end(), ends.begin(), std::not_equal_to<>{} ).first;
  }

  CPP_member auto equal( cursor const& that ) const -> CPP_ret( bool )( requires ranges::ForwardRange<RngVal> )
  {
    return n_ == that.n_ && its_ == that.its_;
  }
};

}  // namespace details

// In:  Range<Range<T>>
// Out: Range<T>, flattened by walking the ranges
//                round-robin fashion.
inline auto interleave()
{
  return ranges::make_pipeable( []( auto&& rngs ) {
    using Rngs = decltype( rngs );
    return details::interleave_view<ranges::view::all_t<Rngs>>( ranges::view::all( std::forward<Rngs>( rngs ) ) );
  } );
}

}  // namespace AoC
