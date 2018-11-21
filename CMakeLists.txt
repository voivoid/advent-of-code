cmake_minimum_required(VERSION 3.12.0)

project(CppAoc LANGUAGES CXX)

include(cmake/thirdparties/boost.cmake)
include(cmake/thirdparties/rangev3.cmake)
include(cmake/thirdparties/json.cmake)
include(cmake/build_utils.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# AocUtils
add_library(AocUtils utils/inc/AoC_utils/algo.h
  utils/inc/AoC_utils/anon_ret.h
  utils/inc/AoC_utils/geo.h utils/geo.cpp
  utils/inc/AoC_utils/match.h
  utils/inc/AoC_utils/md5.h utils/md5.cpp
  utils/inc/AoC_utils/parse.h
  utils/inc/AoC_utils/ranges.h
  utils/inc/AoC_utils/string.h utils/string.cpp
  utils/inc/AoC_utils/utils.h
  )

target_include_directories(AocUtils PUBLIC utils/inc)
target_link_libraries(AocUtils PRIVATE Boost::boost PUBLIC range-v3)

# AocProblemsMap
add_library(AocProblemsMap inc/AoC/problems_map.h src/problems_map.cpp)
target_include_directories(AocProblemsMap PUBLIC inc)

append_problem_srcs(AocProblemsSources 2015 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23)

# AocProblems
add_library(AocProblems OBJECT ${AocProblemsSources}
  src/impl_tests.h src/impl_tests.cpp)
target_include_directories(AocProblems PUBLIC inc PRIVATE src)
target_link_libraries(AocProblems PUBLIC AocProblemsMap AocUtils Boost::context PRIVATE Boost::boost range-v3 json)

# AocApp
add_executable(AocApp app/main.cpp
  app/solve_problem.h app/solve_problem.cpp
  app/parse_cmd_line.h app/parse_cmd_line.cpp)
target_include_directories(AocApp PRIVATE app)
target_link_libraries(AocApp PRIVATE AocProblems Boost::boost Boost::program_options)



# AocLibTests
add_executable(AocLibTests tests/2015_01.cpp
  tests/2015_02.cpp
  tests/2015_03.cpp
  tests/2015_05.cpp
  tests/2015_06.cpp
  tests/2015_07.cpp
  tests/2015_08.cpp
  tests/2015_09.cpp
  tests/2015_11.cpp
  tests/2015_12.cpp
  tests/2015_13.cpp
  tests/2015_15.cpp
  tests/2015_19.cpp
  tests/2015_20.cpp
  tests/2015_23.cpp
  tests/impl_test.cpp
  tests/AoC_utils_tests/algo.cpp
  tests/AoC_utils_tests/anon_ret.cpp
  tests/AoC_utils_tests/geo.cpp
  tests/AoC_utils_tests/match.cpp
  tests/AoC_utils_tests/md5.cpp
  tests/AoC_utils_tests/ranges.cpp
  tests/AoC_utils_tests/string.cpp
  tests/AoC_utils_tests/utils.cpp
  tests/utils/aoc_fixture.cpp
  tests/utils/aoc_fixture.h
  tests/utils/boost_unit_test_module.cpp)
target_link_libraries(AocLibTests PRIVATE AocProblems AocUtils Boost::boost Boost::unit_test_framework)




find_program(ClangFormat clang-format)
if(ClangFormat)
  foreach(AocTarget ${AocTargets})
    get_target_property(AocTargetSources ${AocTarget} SOURCES)
    list(APPEND AocSources ${AocTargetSources})
  endforeach()

  list(FILTER AocSources INCLUDE REGEX "^.*\.(h|cpp)\$")

  add_custom_target(clangformat
    COMMAND clang-format -style=file -i ${AocSources}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

include(tests/tests.cmake)
