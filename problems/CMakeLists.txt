macro(append_problem_srcs ProblemsSources Year)
  foreach(ProblemNum ${ARGN})
    list(APPEND ${ProblemsSources} inc/AoC/${Year}/problem_${ProblemNum}.h src/${Year}/problem_${ProblemNum}.cpp)
  endforeach(ProblemNum)
endmacro()


append_problem_srcs(AocProblemsSources 2015 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25)
append_problem_srcs(AocProblemsSources 2016 01 02)
append_problem_srcs(AocProblemsSources 2018 01 02 03 04)
list(APPEND AocProblemsSources src/2015/problem_01_generator.cpp)

add_library(AocProblems OBJECT ${AocProblemsSources}
                               src/impl_tests.h src/impl_tests.cpp
                               inc/AoC/problems_map.h src/problems_map.cpp
                               inc/AoC/generators_map.h src/generators_map.cpp)
target_include_directories(AocProblems PUBLIC inc PRIVATE src )
target_link_libraries(AocProblems PUBLIC AocUtils Boost::context PRIVATE Boost::boost range-v3 json)