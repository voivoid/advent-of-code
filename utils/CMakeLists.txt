# AocUtils
add_library(AocUtils
  inc/AoC/utils/2d_array.h
  inc/AoC/utils/algo.h
  inc/AoC/utils/anon_ret.h
  inc/AoC/utils/assert.h
  inc/AoC/utils/constexpr.h
  inc/AoC/utils/curry.h
  inc/AoC/utils/fusion.h
  inc/AoC/utils/geo.h src/geo.cpp
  inc/AoC/utils/match.h
  inc/AoC/utils/md5.h src/md5.cpp
  inc/AoC/utils/parse.h
  inc/AoC/utils/ranges/2dvector.h
  inc/AoC/utils/ranges/append.h
  inc/AoC/utils/ranges/enumerate.h
  inc/AoC/utils/ranges/generate.h
  inc/AoC/utils/ranges/interleave.h
  inc/AoC/utils/ranges/prepend.h
  inc/AoC/utils/ranges/transpose.h
  inc/AoC/utils/ranges/unique_pair_combinations.h
  inc/AoC/utils/string.h src/string.cpp
  inc/AoC/utils/zipper.h
  )

target_include_directories(AocUtils PUBLIC inc)
target_link_libraries(AocUtils PRIVATE Boost::boost PUBLIC range-v3)
