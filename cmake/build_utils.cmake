include_guard()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(AocCxxWarnings -Wall -Wextra -Wpedantic -Werror -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wrestrict -Wnull-dereference -Wold-style-cast -Wuseless-cast -Wdouble-promotion -Wshadow -Wformat=2 -Wconversion -Wsign-conversion -Wfloat-equal -Wcast-qual -Winit-self -Wpointer-arith)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(AocCxxWarnings -Wall -Wextra -Wpedantic -Werror -Wnull-dereference -Wold-style-cast -Wdouble-promotion -Wshadow -Wformat=2 -Wconversion -Wsign-conversion -Wfloat-equal -Wcast-qual -Winit-self -Wpointer-arith)
elseif (MSVC)
  set(AocCxxWarnings /permissive- /W2 /WX)
endif()

macro(process_aoc_target Target)
  target_compile_options(${Target} PUBLIC ${AocCxxWarnings})
endmacro()

macro(add_library LibraryName)
  _add_library(${ARGV})
  process_aoc_target(${LibraryName})
endmacro()

macro(add_executable ExecutableName)
  _add_executable(${ARGV})
  process_aoc_target(${ExecutableName})
endmacro()
