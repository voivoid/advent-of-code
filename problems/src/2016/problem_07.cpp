#include "AoC/2016/problem_07.h"

#include "AoC/problems_map.h"
#include "AoC/utils/curry.h"

#include "range/v3/algorithm/any_of.hpp"
#include "range/v3/algorithm/count_if.hpp"
#include "range/v3/algorithm/equal.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/istream.hpp"
#include "range/v3/view/join.hpp"
#include "range/v3/view/sliding.hpp"
#include "range/v3/view/split_when.hpp"
#include "range/v3/view/stride.hpp"
#include "range/v3/view/tail.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/hof/lift.hpp"
#include "boost/numeric/conversion/cast.hpp"

#include <array>
#include <functional>
#include <istream>
#include <string>

namespace
{
constexpr size_t abba_len = 4;
constexpr size_t aba_len  = 3;

template <typename Range>
bool is_abba_seq( const Range& seq )
{
  assert( seq.size() == abba_len );
  return seq[ 0 ] == seq[ 3 ] && seq[ 1 ] == seq[ 2 ] && seq[ 0 ] != seq[ 1 ];
}

template <typename Range>
bool is_aba_seq( const Range& seq )
{
  assert( seq.size() == aba_len );
  return seq[ 0 ] == seq[ 2 ] && seq[ 0 ] != seq[ 1 ];
}

template <typename Range>
bool has_abba( const Range& part )
{
  return ranges::any_of( part | ranges::view::sliding( abba_len ), BOOST_HOF_LIFT( is_abba_seq ) );
}

template <typename Range>
auto aba_to_bab( const Range& seq )
{
  assert( seq.size() == aba_len );
  return std::array<char, aba_len>{ seq[ 1 ], seq[ 0 ], seq[ 1 ] };
}

template <typename R1, typename R2>
bool is_abba( const R1& parts_outside_square_brackets, const R2& parts_inside_square_brackets )
{
  return ranges::any_of( parts_outside_square_brackets, BOOST_HOF_LIFT( has_abba ) ) &&
         !ranges::any_of( parts_inside_square_brackets, BOOST_HOF_LIFT( has_abba ) );
}

template <typename R1, typename R2>
bool is_aba( const R1& parts_outside_square_brackets, const R2& parts_inside_square_brackets )
{
  static const auto get_3chars_sequences = ranges::view::transform( ranges::view::sliding( aba_len ) ) | ranges::view::join;

  auto aba_sequences = parts_outside_square_brackets | get_3chars_sequences | ranges::view::filter( BOOST_HOF_LIFT( is_aba_seq ) );
  auto bab_sequences = aba_sequences | ranges::view::transform( BOOST_HOF_LIFT( aba_to_bab ) );


  return ranges::any_of( bab_sequences, [&parts_inside_square_brackets]( const auto bab ) {
    return ranges::any_of( parts_inside_square_brackets | get_3chars_sequences, [&bab]( const auto seq ) {
      assert( seq.size() == aba_len );
      assert( bab.size() == aba_len );
      return ranges::equal( seq, bab );
    } );
  } );
}

template <typename PartsChecker>
bool check_ip( const PartsChecker& check_parts, const std::string& ip )
{
  const auto parts                         = ip | ranges::view::split_when( []( const char c ) { return c == '[' || c == ']'; } );
  const auto parts_outside_square_brackets = parts | ranges::view::stride( 2 );
  const auto parts_inside_square_brackets  = parts | ranges::view::tail | ranges::view::stride( 2 );

  return check_parts( parts_outside_square_brackets, parts_inside_square_brackets );
}

template <typename IpFilterPredicate>
size_t solve( std::istream& istream, const IpFilterPredicate& ip_filter_predicate )
{
  const auto ip_filter = AoC::curry( BOOST_HOF_LIFT( check_ip ) )( std::cref( ip_filter_predicate ) );
  return boost::numeric_cast<size_t>( ranges::count_if( ranges::istream<std::string>( istream ), ip_filter ) );
}

}  // namespace

namespace AoC_2016
{

namespace problem_07
{

size_t solve_1( std::istream& istream )
{
  return solve( istream, BOOST_HOF_LIFT( is_abba ) );
}

size_t solve_2( std::istream& istream )
{
  return solve( istream, BOOST_HOF_LIFT( is_aba ) );
}

AOC_REGISTER_PROBLEM( 2016_07, solve_1, solve_2 );

}  // namespace problem_07

}  // namespace AoC_2016
