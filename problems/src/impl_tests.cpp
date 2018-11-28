#include "impl_tests.h"

#include <algorithm>
#include <set>

namespace
{

using ImplTests = std::set<AoC::ImplTest>;
ImplTests& get_impl_tests()
{
  static ImplTests impl_tests;
  return impl_tests;
}
}  // namespace

namespace AoC
{
void test_impl()
{
  const auto& tests = get_impl_tests();
  std::for_each( tests.cbegin(), tests.cend(), []( const auto test ) { return test(); } );
}

void register_impl_test( ImplTest impl_test )
{
  get_impl_tests().insert( impl_test );
}

}  // namespace AoC
