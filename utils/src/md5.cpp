#include "AoC/utils/md5.h"

#include "boost/uuid/detail/md5.hpp"
#include <cstddef>

namespace
{

union md5_digest {
  boost::uuids::detail::md5::digest_type digest;

  using digest_bytes = std::byte[ sizeof( digest ) ];
  digest_bytes bytes;
};


char byte_to_char( const std::byte b )
{
  static constexpr char chars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

  assert( static_cast<size_t>( b ) < sizeof( chars ) );
  return chars[ static_cast<int>( b ) ];
}

std::string md5_to_string( const md5_digest::digest_bytes& bytes )
{
  std::string str;
  str.reserve( 32 );

  for ( const auto byte : bytes )
  {
    const auto octet1 = byte >> 4;
    const auto octet2 = byte & std::byte( 0xf );

    str.push_back( byte_to_char( octet1 ) );
    str.push_back( byte_to_char( octet2 ) );
  }

  return str;
}

}  // namespace

namespace AoC
{

std::string md5( const std::string_view& input )
{
  boost::uuids::detail::md5 md5;
  md5.process_bytes( input.data(), input.size() );

  md5_digest digest;
  md5.get_digest( digest.digest );

  return md5_to_string( digest.bytes );
}

}  // namespace AoC
