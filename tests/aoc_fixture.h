#pragma once

#include <filesystem>
#include <fstream>
#include <string_view>

struct AocFixture
{
  AocFixture();

  template <typename T, typename... Args>
  auto run( T* const func, std::string_view data, Args&&... args )
  {
    std::istringstream ss( data.data() );
    return func( ss, std::forward<Args>( args )... );
  }
};
