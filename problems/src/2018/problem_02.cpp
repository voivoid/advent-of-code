#include "AoC/2018/problem_02.h"

#include "AoC/problems_map.h"

#include "range/v3/action/sort.hpp"
#include "range/v3/algorithm/count.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/cartesian_product.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/group_by.hpp"
#include "range/v3/view/istream.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/view/zip.hpp"

#include "boost/numeric/conversion/cast.hpp"

#include <istream>
#include <stdexcept>
#include <string>

#include <cassert>


namespace
{

struct BoxStats
{
  size_t doubles;
  size_t triples;
};

BoxStats operator+( const BoxStats b1, const BoxStats b2 )
{
  return { b1.doubles + b2.doubles, b1.triples + b2.triples };
}

using BoxId = std::string;

BoxStats analyze_box_id( BoxId id )
{
  const auto sorted_chars   = std::move( id ) | ranges::actions::sort;
  auto groups_lengths = sorted_chars | ranges::views::group_by( std::equal_to<char>() ) |
                              ranges::views::transform( []( const auto group ) { return ranges::distance( group ); } );
  const bool has_doubles = ranges::count( groups_lengths, 2 ) > 0;
  const bool has_triples = ranges::count( groups_lengths, 3 ) > 0;
  return { has_doubles ? size_t{ 1 } : size_t{ 0 }, has_triples ? size_t{ 1 } : size_t{ 0 } };
}

size_t count_mismatches( const BoxId& b1, const BoxId& b2 )
{
  assert( b1.size() == b2.size() );
  const auto mismatches = ranges::views::transform( b1, b2, std::equal_to<char>() );
  return boost::numeric_cast<size_t>( ranges::count( mismatches, false ) );
}

std::string find_common_letters( const BoxId& b1, const BoxId& b2 )
{
  assert( b1.size() == b2.size() );
  return ranges::views::zip( b1, b2 ) | ranges::views::filter( []( const auto chars_pair ) {
           auto [ c1, c2 ] = chars_pair;
           return c1 == c2;
         } ) |
         ranges::views::transform( []( const auto chars_pair ) { return chars_pair.first; } ) | ranges::to<std::string>();
}

}  // namespace

namespace AoC_2018
{

namespace problem_02
{

size_t solve_1( std::istream& input )
{
  auto box_stats          = ranges::istream<std::string>( input ) | ranges::views::transform( &analyze_box_id );
  const auto result_stats = ranges::accumulate( box_stats, BoxStats{} );

  return result_stats.doubles * result_stats.triples;
}

std::string solve_2( std::istream& input )
{
  const auto boxes              = ranges::istream<std::string>( input ) | ranges::to_vector;
  const auto boxes_combinations = ranges::views::cartesian_product( boxes, boxes );

  auto boxes_with_one_mismatch = boxes_combinations | ranges::views::filter( []( const auto box_pair ) {
                                   auto [ b1, b2 ] = box_pair;
                                   return count_mismatches( b1, b2 ) == 1;
                                 } );

  if ( boxes_with_one_mismatch.empty() )
  {
    throw std::runtime_error( "No solution found" );
  }

  const auto& [ b1, b2 ] = *boxes_with_one_mismatch.begin();
  return find_common_letters( b1, b2 );
}

AOC_REGISTER_PROBLEM( 2018_02, solve_1, solve_2 );

}  // namespace problem_02

}  // namespace AoC_2018

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

bool operator==( const BoxStats b1, const BoxStats b2 )
{
  return b1.doubles == b2.doubles && b1.triples == b2.triples;
}

static void impl_tests()
{
  assert( ( BoxStats{ 0, 0 } ) == analyze_box_id( "abcdef" ) );
  assert( ( BoxStats{ 1, 1 } ) == analyze_box_id( "bababc" ) );
  assert( ( BoxStats{ 1, 0 } ) == analyze_box_id( "abbcde" ) );
  assert( ( BoxStats{ 0, 1 } ) == analyze_box_id( "abcccd" ) );
  assert( ( BoxStats{ 1, 0 } ) == analyze_box_id( "aabcdd" ) );
  assert( ( BoxStats{ 1, 0 } ) == analyze_box_id( "abcdee" ) );
  assert( ( BoxStats{ 0, 1 } ) == analyze_box_id( "ababab" ) );

  assert( 2 == ( count_mismatches( "abcde", "axcye" ) ) );
  assert( 1 == ( count_mismatches( "fghij", "fguij" ) ) );

  assert( "fgij" == ( find_common_letters( "fghij", "fguij" ) ) );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
