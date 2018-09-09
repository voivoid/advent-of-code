cmake_minimum_required(VERSION 3.12.0)

project(CppAoc LANGUAGES CXX)

include(cmake/thirdparties/boost.cmake)
include(cmake/thirdparties/rangev3.cmake)
include(cmake/thirdparties/json.cmake)
include(cmake/build_utils.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(AOC_CXX_WARNINGS -Wall -Wextra -Wpedantic -Werror -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wrestrict -Wnull-dereference -Wold-style-cast -Wuseless-cast -Wdouble-promotion -Wshadow -Wformat=2 -Wconversion -Wsign-conversion -Wfloat-equal -Wcast-qual -Winit-self -Wpointer-arith)
endif()


# AocUtils
add_library(AocUtils utils/inc/AoC_utils/algo.h
                     utils/inc/AoC_utils/geo.h utils/geo.cpp
                     utils/inc/AoC_utils/match.h
                     utils/inc/AoC_utils/md5.h utils/md5.cpp
                     utils/inc/AoC_utils/parse.h
                     utils/inc/AoC_utils/ranges.h
                     utils/inc/AoC_utils/utils.h
)

target_include_directories(AocUtils PUBLIC utils/inc)
target_link_libraries(AocUtils PRIVATE Boost::boost PUBLIC range-v3)

# AocProblemsMap
add_library(AocProblemsMap inc/AoC/problems_map.h src/problems_map.cpp)
target_include_directories(AocProblemsMap PUBLIC inc)

# AocProblems
add_library(AocProblems OBJECT
                        inc/AoC/2015/problem_01.h src/2015/problem_01.cpp
                        inc/AoC/2015/problem_02.h src/2015/problem_02.cpp
                        inc/AoC/2015/problem_03.h src/2015/problem_03.cpp
                        inc/AoC/2015/problem_04.h src/2015/problem_04.cpp
                        inc/AoC/2015/problem_05.h src/2015/problem_05.cpp
                        inc/AoC/2015/problem_06.h src/2015/problem_06.cpp
                        inc/AoC/2015/problem_07.h src/2015/problem_07.cpp
                        inc/AoC/2015/problem_08.h src/2015/problem_08.cpp
                        inc/AoC/2015/problem_09.h src/2015/problem_09.cpp
                        inc/AoC/2015/problem_10.h src/2015/problem_10.cpp
                        inc/AoC/2015/problem_11.h src/2015/problem_11.cpp
                        inc/AoC/2015/problem_12.h src/2015/problem_12.cpp
                        inc/AoC/2015/problem_13.h src/2015/problem_13.cpp
                        inc/AoC/2015/problem_14.h src/2015/problem_14.cpp
                        src/impl_tests.h src/impl_tests.cpp)
target_include_directories(AocProblems PUBLIC inc PRIVATE src)
target_link_libraries(AocProblems PUBLIC AocProblemsMap AocUtils PRIVATE Boost::boost range-v3 json)

# AocApp
add_executable(AocApp app/main.cpp
                       app/solve_problem.h app/solve_problem.cpp
                       app/parse_cmd_line.h app/parse_cmd_line.cpp)
target_include_directories(AocApp PRIVATE app)
target_link_libraries(AocApp PRIVATE AocProblems Boost::program_options)



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
                           tests/impl_test.cpp
                           tests/AoC_utils_tests/algo.cpp
                           tests/AoC_utils_tests/geo.cpp
                           tests/AoC_utils_tests/match.cpp
                           tests/AoC_utils_tests/md5.cpp
                           tests/AoC_utils_tests/ranges.cpp
                           tests/AoC_utils_tests/utils.cpp
                           tests/utils/aoc_fixture.cpp
                           tests/utils/aoc_fixture.h
                           tests/utils/boost_unit_test_module.cpp)
target_link_libraries(AocLibTests PRIVATE AocProblems AocUtils Boost::unit_test_framework Boost::program_options)




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
