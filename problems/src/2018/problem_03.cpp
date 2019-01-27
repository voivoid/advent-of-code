#include "AoC/2018/problem_03.h"

#include "AoC/problems_map.h"
#include "AoC/utils/2d_array.h"
#include "AoC/utils/geo.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/count_if.hpp"
#include "range/v3/algorithm/find_if.hpp"
#include "range/v3/getlines.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/cartesian_product.hpp"
#include "range/v3/view/indices.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/fusion/container/vector.hpp"
#include "boost/spirit/home/x3.hpp"

#include <istream>
#include <stdexcept>
#include <string>

namespace
{
using ClaimId = int;
struct Claim
{
  ClaimId id;
  AoC::URectangle area;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( AoC::UPoint, x, y )
BOOST_FUSION_ADAPT_STRUCT( Claim, id, area )

namespace
{

constexpr size_t side                                   = 1000;
using Overlaps                                          = AoC::dd_static_heap_array<ClaimId, side, side>;
constexpr ClaimId overlapped_area                       = -1;
[[maybe_unused]] constexpr ClaimId non_initialized_area = 0;

Claim parse_claim( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  auto set_rect_attr = []( auto& ctx ) {
    auto& rect_lt = _val( ctx ).left_top;
    auto& rect_rb = _val( ctx ).right_bottom;
    auto& attr    = _attr( ctx );
    rect_lt       = boost::fusion::at_c<0>( attr );
    rect_rb.x     = rect_lt.x + boost::fusion::at_c<1>( attr ) - 1;
    rect_rb.y     = rect_lt.y + boost::fusion::at_c<2>( attr ) - 1;
  };

  const auto coord_parser = x3::rule<struct _point, AoC::UPoint>{} = AoC::x3_size_t_ > ',' > AoC::x3_size_t_;
  const auto area_parser                                           = x3::rule<struct _rect, AoC::URectangle>{} =
      ( coord_parser > ':' > AoC::x3_size_t_ > 'x' > AoC::x3_size_t_ )[ set_rect_attr ];
  const auto id_parser = x3::int_;
  const auto parser    = '#' > id_parser > '@' > area_parser;

  Claim claim;
  const bool is_parsed = AoC::x3_parse( line.cbegin(), line.cend(), parser, x3::space, claim );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse claim data" );
  }

  assert( claim.id != non_initialized_area );
  assert( claim.id != overlapped_area );

  return claim;
}

auto get_all_claim_coords( const Claim& claim )
{
  auto xs = ranges::view::closed_indices( claim.area.left_top.x, claim.area.right_bottom.x );
  auto ys = ranges::view::closed_indices( claim.area.left_top.y, claim.area.right_bottom.y );
  return ranges::view::cartesian_product( ys, xs );
}

bool check_is_not_overlapped( const Overlaps& overlaps, const Claim& claim )
{
  for ( auto [ y, x ] : get_all_claim_coords( claim ) )
  {
    if ( overlaps[ x ][ y ] != claim.id )
    {
      return false;
    }
  }

  return true;
}

auto apply_claim( Overlaps& overlaps, const Claim& claim )
{
  for ( auto [ y, x ] : get_all_claim_coords( claim ) )
  {
    assert( x < side );
    assert( y < side );

    auto& location = overlaps[ x ][ y ];
    if ( location == 0 )
    {
      location = claim.id;
    }
    else
    {
      location = overlapped_area;
    }
  }

  return std::ref( overlaps );
}

auto get_claims( std::istream& input )
{
  return ranges::getlines( input ) | ranges::view::transform( &parse_claim );
}

template <typename Claims>
Overlaps calc_overlaps( Claims& claims )
{
  Overlaps overlaps;
  ranges::accumulate( claims, std::ref( overlaps ), &apply_claim );
  return overlaps;
}

}  // namespace

namespace AoC_2018
{

namespace problem_03
{

int solve_1( std::istream& input )
{
  auto claims = get_claims( input );

  const auto overlaps     = calc_overlaps( claims );
  const auto overlaps_num = ranges::count_if( overlaps, []( const ClaimId claim_id ) { return claim_id == overlapped_area; } );
  return static_cast<int>( overlaps_num );
}

int solve_2( std::istream& input )
{
  const auto claims = get_claims( input ) | ranges::to_vector;

  const auto overlaps = calc_overlaps( claims );
  const auto non_overlapped_claim_iter =
      ranges::find_if( claims, [&overlaps]( const Claim& claim ) { return check_is_not_overlapped( overlaps, claim ); } );

  if ( non_overlapped_claim_iter == claims.end() )
  {
    throw std::runtime_error( "Solution not found" );
  }

  return non_overlapped_claim_iter->id;
}

AOC_REGISTER_PROBLEM( 2018_03, solve_1, solve_2 );

}  // namespace problem_03

}  // namespace AoC_2018

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  const auto claim = parse_claim( "#1 @ 342,645: 25x20" );
  assert( claim.id == 1 );
  assert( claim.area == ( AoC::URectangle{ 342, 645, 342 + 24, 645 + 19 } ) );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
