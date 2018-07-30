#include <boost/test/unit_test.hpp>

#include <AoC_utils/md5.h>

BOOST_AUTO_TEST_CASE( aoc_utils_md5 )
{
  BOOST_CHECK_EQUAL( "d41d8cd98f00b204e9800998ecf8427e", AoC::md5( "" ) );
  BOOST_CHECK_EQUAL( "0cc175b9c0f1b6a831c399e269772661", AoC::md5( "a" ) );
  BOOST_CHECK_EQUAL( "900150983cd24fb0d6963f7d28e17f72", AoC::md5( "abc" ) );
  BOOST_CHECK_EQUAL( "f96b697d7cb7938d525a2f31aaf161d0", AoC::md5( "message digest" ) );
  BOOST_CHECK_EQUAL( "c3fcd3d76192e4007dfb496cca67e13b", AoC::md5( "abcdefghijklmnopqrstuvwxyz" ) );
  BOOST_CHECK_EQUAL( "d174ab98d277d9f5a5611c2c9f419d9f", AoC::md5( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" ) );
  BOOST_CHECK_EQUAL( "57edf4a22be3c955ac49da2e2107b67a",
                     AoC::md5( "12345678901234567890123456789012345678901234567890123456789012345678901234567890" ) );
  BOOST_CHECK_EQUAL( "9e107d9d372bb6826bd81d3542a419d6", AoC::md5( "The quick brown fox jumps over the lazy dog" ) );
  BOOST_CHECK_EQUAL( "e4d909c290d0fb1ca068ffaddf22cbd0", AoC::md5( "The quick brown fox jumps over the lazy dog." ) );
}
