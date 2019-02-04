include(ExternalProject)

find_package(Git REQUIRED)

set(RangeV3Ver v1.0-beta)

ExternalProject_Add(
  get_rangev3
  PREFIX thirdparties
  GIT_REPOSITORY https://github.com/ericniebler/range-v3/
  GIT_TAG ${RangeV3Ver}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  )

add_library(range-v3 INTERFACE)

ExternalProject_Get_Property(get_rangev3 SOURCE_DIR)
set_target_properties(range-v3 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR}/include)
set_target_properties(range-v3 PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${SOURCE_DIR}/include)

add_dependencies(range-v3 get_rangev3)
