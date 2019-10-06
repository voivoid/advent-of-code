cmake_minimum_required(VERSION 3.13.0)

project(CppAoc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/thirdparties/boost.cmake)
include(cmake/thirdparties/rangev3.cmake)
include(cmake/thirdparties/json.cmake)
include(cmake/build_utils.cmake)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND ${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER "9")
  set(AocExcludedProblems 2015_13     # gcc 9.* segfaults
                          2016_07     # gcc 9.* segfaults ( debug build only )
                          2016_08     # gcc 9.* segfaults
                          2017_03     # gcc 9.* segfaults
   )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(AocExcludedProblems 2015_14
                          2015_17
                          2015_19
                          2016_01
                          2016_04
                          2016_05
                          2017_14
                          2018_03
                          2018_04
                          2018_11

                          2015_23 # HOF
                          2016_07 # HOF
                          2016_08 # HOF
                          2017_02 # HOF
                          2017_16 # HOF
                          2017_18 # HOF
                          2017_19 # HOF
                          2017_20 # HOF
                          2018_05 # HOF
   )
endif()

foreach(Problem ${AocExcludedProblems})
  message("Problem ${Problem} would not be built due to ${CMAKE_CXX_COMPILER_ID} compiler issues")
endforeach()

add_subdirectory(utils)
add_subdirectory(problems)
add_subdirectory(app)

enable_testing()
add_subdirectory(tests)


find_program(ClangFormat clang-format)
if(ClangFormat)
  set(AocTargets AocApp AocProblems AocLibTests AocUtils)
  foreach(AocTarget ${AocTargets})
    get_target_property(AocTargetSources ${AocTarget} SOURCES)
    get_target_property(AocTargetDir ${AocTarget} SOURCE_DIR)
    foreach(Src ${AocTargetSources})
      list(APPEND AocSources "${AocTargetDir}/${Src}")
    endforeach()
  endforeach()

  list(FILTER AocSources INCLUDE REGEX "^.*\.(h|cpp)\$")

  add_custom_target(clangformat
    COMMAND clang-format -style=file -i ${AocSources}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()
