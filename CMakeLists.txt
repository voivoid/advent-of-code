cmake_minimum_required(VERSION 3.13.0)
project(CppAoc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/thirdparties/boost.cmake)
include(cmake/thirdparties/rangev3.cmake)
include(cmake/thirdparties/json.cmake)
include(cmake/build_utils.cmake)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(AocExcludedProblems 2015_23     # boost variant has a problem that should be fixed in boost 1.70 - https://github.com/boostorg/variant/pull/51
  )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(AocExcludedProblems 2015_08     # MSVC raw string issues
                          2015_14     # looks like MSVC has issues with range-v3
                          2015_23     # looks like MSVC has issues with boost::hof issue
                          2018_02
                          2018_04
                          2018_06
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


