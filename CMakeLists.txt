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
  set(AocExcludedProblems 2017_14 # https://github.com/ericniebler/range-v3/issues/1141
  )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(AocExcludedProblems 2015_14     # looks like MSVC has issues with range-v3
                          2015_23     # looks like MSVC has issues with boost::hof
                          2016_04
                          2016_05
                          2016_07
                          2016_08
                          2017_02
                          2017_04
                          2018_04
                          2018_05
                          2018_06
                          2018_07
                          2018_11
  )
endif()

set(AocExcludedProblems ${AocExcludedProblems} 2016_08) # https://github.com/ericniebler/range-v3/issues/1141

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
