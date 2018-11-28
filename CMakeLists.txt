cmake_minimum_required(VERSION 3.12.0)
project(CppAoc LANGUAGES CXX)

include(cmake/thirdparties/boost.cmake)
include(cmake/thirdparties/rangev3.cmake)
include(cmake/thirdparties/json.cmake)
include(cmake/build_utils.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
    COMMAND clang-format -verbose -style=file -i ${AocSources}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()


