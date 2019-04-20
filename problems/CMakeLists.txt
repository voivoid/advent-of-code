macro(append_problem_srcs ProblemsSources Year)
  foreach(ProblemNum ${ARGN})
    if(NOT "${Year}_${ProblemNum}" IN_LIST AocExcludedProblems)
      list(APPEND ${ProblemsSources} inc/AoC/${Year}/problem_${ProblemNum}.h src/${Year}/problem_${ProblemNum}.cpp)
    endif()
  endforeach(ProblemNum)
  source_group(${Year} inc/AoC/${Year}/*|src/${Year}/*)
endmacro()


append_problem_srcs(AocProblemsSources 2015 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25)
append_problem_srcs(AocProblemsSources 2016 01 02 03 04 05 06 07 08 09 10)
append_problem_srcs(AocProblemsSources 2017 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17)
append_problem_srcs(AocProblemsSources 2018 01 02 03 04 05 06 07 08 09    11)
list(APPEND AocProblemsSources src/2015/problem_01_generator.cpp)

add_library(AocProblems OBJECT ${AocProblemsSources}
                               src/impl_tests.h src/impl_tests.cpp
                               inc/AoC/problems_map.h src/problems_map.cpp
                               inc/AoC/generators_map.h src/generators_map.cpp)
target_include_directories(AocProblems PUBLIC inc PRIVATE src )
target_link_libraries(AocProblems PUBLIC AocUtils Boost::boost Boost::context PRIVATE range-v3 json)

if(MSVC)
  target_link_options(AocProblems PUBLIC /STACK:4194304)
endif()
