#pragma once

namespace AoC
{

void test_impl();

using ImplTest = void ( * )( void );
void register_impl_test( ImplTest impl_test );

namespace Details
{
struct ImplTestRegistrator
{
  ImplTestRegistrator( ImplTest impl_test )
  {
    register_impl_test( impl_test );
  }
};
}  // namespace Details
}  // namespace AoC

#define REGISTER_IMPL_TEST( func ) static const AoC::Details::ImplTestRegistrator registrator( func )
