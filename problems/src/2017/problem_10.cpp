#include "AoC/2017/problem_10.h"

#include "AoC/problems_map.h"
#include "AoC/utils/algo.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/reverse.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/chunk.hpp"
#include "range/v3/view/concat.hpp"
#include "range/v3/view/cycle.hpp"
#include "range/v3/view/indices.hpp"
#include "range/v3/view/istream.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/numeric/conversion/cast.hpp"

#include <iomanip>
#include <istream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
using Num     = size_t;
using Nums    = std::vector<Num>;
using Length  = Num;
using Lengths = std::vector<Length>;

Lengths parse_lengths( std::istream& input )
{
  const auto parser = AoC::x3_size_t_ % ',';

  Lengths lengths;
  const bool is_parsed = AoC::x3_parse( input, parser, boost::spirit::x3::space, lengths );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input lengths data" );
  }

  return lengths;
}

template <typename Range>
void run_round( Nums& nums, const Range& lengths, size_t& skip_size, size_t& current )
{
  const Num max_num = nums.size();
  auto cycled_nums  = nums | ranges::view::cycle;

  for ( const auto len : lengths )
  {
    assert( len <= max_num );

    if ( len > 1 )
    {
      auto left  = cycled_nums.begin() + boost::numeric_cast<ptrdiff_t>( current );
      auto right = left + boost::numeric_cast<ptrdiff_t>( len );
      ranges::reverse( left, right );
    }

    current += ( len + skip_size );
    current %= max_num;
    ++skip_size;
  }
}

template <typename Range>
void run_rounds( Nums& nums, const Range& lengths, const size_t rounds_num )
{
  size_t skip_size = 0;
  size_t current   = 0;

  AoC::repeat( rounds_num, [&]() { run_round( nums, lengths, skip_size, current ); } );
}

template <typename Range>
Num xor_nums( Range&& nums )
{
  static const auto xor_func = []( const Num n1, const Num n2 ) { return n1 ^ n2; };
  return ranges::accumulate( nums, Num{ 0 }, xor_func );
}

auto make_dense_hash( const Nums& nums )
{
  return nums | ranges::view::chunk( 16 ) | ranges::view::transform( []( auto&& chunk ) { return xor_nums( chunk ); } );
}

template <typename Range>
std::string to_hex_string( Range&& nums )
{
  std::ostringstream ss;
  ss << std::setfill( '0' ) << std::hex;

  for ( const auto n : nums )
  {
    ss << std::setw( 2 ) << n;
  }

  return ss.str();
}

Nums make_nums( const Num max_num )
{
  return ranges::view::indices( max_num ) | ranges::to_vector;
}

size_t solve_1_impl( std::istream& input, const Num max_num )
{
  const Lengths lengths = parse_lengths( input );

  Nums nums = make_nums( max_num );
  run_rounds( nums, lengths, 1 );

  return nums[ 0 ] * nums[ 1 ];
}

}  // namespace

namespace AoC_2017
{

namespace problem_10
{

size_t solve_1( std::istream& input )
{
  return solve_1_impl( input, 256 );
}

std::string solve_2( std::istream& input )
{
  input >> std::noskipws;
  auto input_codes = ranges::istream<char>( input ) | ranges::view::transform( []( const char c ) { return static_cast<Num>( c ); } );

  const auto codes_suffix = { 17, 31, 73, 47, 23 };
  const auto lengths      = ranges::view::concat( input_codes, codes_suffix ) | ranges::to_vector;

  auto nums = make_nums( 256 );
  run_rounds( nums, lengths, 64 );

  return to_hex_string( make_dense_hash( nums ) );
}

AOC_REGISTER_PROBLEM( 2017_10, solve_1, solve_2 );

}  // namespace problem_10
}  // namespace AoC_2017

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  {
    std::istringstream ss{ "3, 4, 1, 5" };
    assert( ( Lengths{ 3, 4, 1, 5 } ) == parse_lengths( ss ) );
  }

  {
    std::istringstream ss{ "3, 4, 1, 5" };
    assert( 12 == solve_1_impl( ss, 5 ) );
  }

  assert( 64 == xor_nums( std::initializer_list<Num>{ 65, 27, 9, 1, 4, 3, 40, 50, 91, 7, 6, 0, 2, 5, 68, 22 } ) );
  assert( "4007ff" == to_hex_string( std::initializer_list<Num>{ 64, 7, 255 } ) );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
