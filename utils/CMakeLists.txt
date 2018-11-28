# AocUtils
add_library(AocUtils
  inc/AoC_utils/algo.h
  inc/AoC_utils/anon_ret.h
  inc/AoC_utils/geo.h geo.cpp
  inc/AoC_utils/match.h
  inc/AoC_utils/md5.h md5.cpp
  inc/AoC_utils/parse.h
  inc/AoC_utils/ranges.h
  inc/AoC_utils/string.h string.cpp
  inc/AoC_utils/utils.h
  )

target_include_directories(AocUtils PUBLIC inc)
target_link_libraries(AocUtils PRIVATE Boost::boost PUBLIC range-v3)
