include_guard()

macro(process_aoc_target Target)
  list(APPEND AocTargets ${Target})
  target_compile_options(${Target} PUBLIC ${AOC_CXX_WARNINGS})
endmacro()

macro(add_library LibraryName)
  _add_library(${ARGV})
  process_aoc_target(${LibraryName})
endmacro()

macro(add_executable ExecutableName)
  _add_executable(${ARGV})
  process_aoc_target(${ExecutableName})
endmacro()
