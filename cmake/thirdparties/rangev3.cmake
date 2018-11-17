include(FetchContent)

find_package(Git REQUIRED)

set(RangeV3Ver master)

FetchContent_Declare(
  rangev3
  GIT_REPOSITORY https://github.com/ericniebler/range-v3/
  GIT_TAG ${RangeV3Ver}
  )

message("Building RangeV3 ...")
FetchContent_Populate(rangev3)
FetchContent_GetProperties(rangev3)

add_library(range-v3 INTERFACE)
set_target_properties(range-v3 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${rangev3_SOURCE_DIR}/include)
set_target_properties(range-v3 PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${rangev3_SOURCE_DIR}/include)

message("Building RangeV3 done")
