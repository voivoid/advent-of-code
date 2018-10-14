#pragma once

#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/mismatch.hpp>
#include <range/v3/distance.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view_facade.hpp>

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
  std::vector<ranges::range_value_type_t<Rngs>> rngs_;
  struct cursor;
  cursor begin_cursor()
  {
    return { 0, &rngs_, ranges::view::transform( rngs_, ranges::begin ) };
  }

    public:
  interleave_view() = default;
  explicit interleave_view( Rngs rngs ) : rngs_( std::move( rngs ) )
  {
  }
};

template <class Rngs>
struct interleave_view<Rngs>::cursor
{
  std::size_t                                                       n_;
  std::vector<ranges::range_value_type_t<Rngs>>*                    rngs_;
  std::vector<ranges::iterator_t<ranges::range_value_type_t<Rngs>>> its_;
  decltype( auto )                                                  read() const
  {
    return *its_[ n_ ];
  }
  void next()
  {
    if ( 0 == ( ( ++n_ ) %= its_.size() ) )
      ranges::for_each( its_, []( auto& it ) { ++it; } );
  }
  bool equal( ranges::default_sentinel ) const
  {
    return n_ == 0 && its_.end() != ranges::mismatch( its_, *rngs_, std::not_equal_to<>(), ranges::ident(), ranges::end ).in1();
  }
  CONCEPT_REQUIRES( ranges::ForwardRange<ranges::range_value_type_t<Rngs>>() )
  bool equal( cursor const& that ) const
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

// In:  Range<Range<T>>
// Out: Range<Range<T>>, transposing the rows and columns.
inline auto transpose()
{
  return ranges::make_pipeable( []( auto&& rngs ) {
    using Rngs = decltype( rngs );
    CONCEPT_ASSERT( ranges::ForwardRange<Rngs>() );
    return std::forward<Rngs>( rngs ) | interleave() | ranges::view::chunk( static_cast<std::size_t>( ranges::distance( rngs ) ) );
  } );
}

// In: Range<Range<T>>
// Out: std::vector<std::vector<T>>
inline auto to_2d_vector()
{
  return ranges::make_pipeable( []( auto&& rngs ) {
    using Rngs = decltype( rngs );
    return std::forward<Rngs>( rngs ) | ranges::view::transform( []( auto r ) { return r | ranges::to_vector; } ) | ranges::to_vector;
  } );
}

}  // namespace AoC
