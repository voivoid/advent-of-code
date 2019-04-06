#include "AoC/2016/problem_04.h"

#include "AoC/problems_map.h"
#include "AoC/utils/parse.h"

#include "range/v3/action/sort.hpp"
#include "range/v3/algorithm/equal.hpp"
#include "range/v3/algorithm/find_if.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/c_str.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/group_by.hpp"
#include "range/v3/view/take.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/numeric/conversion/cast.hpp"
#include "boost/spirit/home/x3.hpp"

#include <functional>
#include <istream>
#include <stdexcept>
#include <string>

namespace
{

using SectorId                = size_t;
constexpr size_t checksum_len = 5;

struct Room
{
  std::string name;
  SectorId sector_id;
  std::string common_letters;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( Room, name, sector_id, common_letters )

namespace
{

Room parse_room( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  const auto ch                    = x3::alpha | x3::char_( '-' );
  const auto name_parser           = +( ch >> &ch );
  const auto sector_id_parser      = AoC::x3_size_t_;
  const auto common_letters_parser = x3::repeat( checksum_len )[ x3::alpha ];

  const auto parser = name_parser > '-' > sector_id_parser > '[' > common_letters_parser > ']';

  Room room;
  const bool is_parsed = x3::phrase_parse( line.cbegin(), line.cend(), parser, x3::space, room );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse input room data" );
  }

  return room;
}

struct LetterInfo
{
  size_t freq;
  char chr;
};

bool compare_by_freq_then_by_char( const LetterInfo i1, const LetterInfo i2 )
{
  const auto [ f1, c1 ] = i1;
  const auto [ f2, c2 ] = i2;
  if ( f1 != f2 )
  {
    return f1 > f2;
  }

  return c1 < c2;
}

bool is_real_room( const Room& room )
{
  const auto sorted_letters =
      room.name | ranges::view::filter( []( const auto c ) { return c != '-'; } ) | ranges::to_vector | ranges::action::sort;

  const auto letters_sorted_by_frequency = sorted_letters | ranges::view::group_by( std::equal_to<char>{} ) |
                                           ranges::view::transform( []( const auto range ) {
                                             return LetterInfo{ boost::numeric_cast<size_t>( ranges::distance( range ) ), range.front() };
                                           } ) |
                                           ranges::to_vector | ranges::action::sort( &compare_by_freq_then_by_char );

  const auto most_common_letters =
      letters_sorted_by_frequency | ranges::view::take( checksum_len ) | ranges::view::transform( &LetterInfo::chr );

  return ranges::equal( most_common_letters, room.common_letters );
}

char rotate_char( const char c, const size_t shift )
{
  if ( c == '-' )
  {
    return ' ';
  }

  constexpr auto ascii_letters_diff = 'z' - 'a';

  assert( isalpha( c ) );
  assert( islower( c ) );

  const auto diff = ( c - 'a' + boost::numeric_cast<int>( shift ) ) % ( ascii_letters_diff + 1 );
  assert( diff <= ascii_letters_diff );

  return boost::numeric_cast<char>( 'a' + diff );
}

auto dechiper_room_name( const Room& room )
{
  return room.name | ranges::view::transform( std::bind( &rotate_char, std::placeholders::_1, room.sector_id ) );
}

bool is_storage_room( const Room& room )
{
  return ranges::equal( dechiper_room_name( room ), ranges::view::c_str( "northpole object storage" ) );
}

auto get_real_rooms( std::istream& input )
{
  return ranges::getlines( input ) | ranges::view::transform( &parse_room ) | ranges::view::filter( &is_real_room );
}

}  // namespace

namespace AoC_2016
{

namespace problem_04
{

size_t solve_1( std::istream& input )
{
  auto real_rooms     = get_real_rooms( input );
  const auto checksum = ranges::accumulate( real_rooms, SectorId{ 0 }, std::plus<SectorId>{}, &Room::sector_id );

  return checksum;
}

size_t solve_2( std::istream& input )
{
  auto real_rooms = get_real_rooms( input );

  const auto& storage_room_iter = ranges::find_if( real_rooms, &is_storage_room );
  assert( storage_room_iter != real_rooms.end() );

  const auto& storage_room = *storage_room_iter;
  return storage_room.sector_id;
}

AOC_REGISTER_PROBLEM( 2016_04, solve_1, solve_2 );

}  // namespace problem_04

}  // namespace AoC_2016

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  const auto room = parse_room( "aaaaa-bbb-z-y-x-123[abxyz]" );
  assert( room.name == "aaaaa-bbb-z-y-x" );
  assert( room.sector_id == 123 );
  assert( room.common_letters == "abxyz" );

  assert( ' ' == rotate_char( '-', 0 ) );
  assert( 'a' == rotate_char( 'a', 0 ) );
  assert( 'b' == rotate_char( 'a', 1 ) );
  assert( 'a' == rotate_char( 'z', 1 ) );
  assert( 'z' == rotate_char( 'a', 25 ) );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
