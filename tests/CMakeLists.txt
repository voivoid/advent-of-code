macro(append_tests_srcs TestsSources Year)
  foreach(ProblemNum ${ARGN})
    if(NOT "${Year}_${ProblemNum}" IN_LIST AocExcludedProblems)
      list(APPEND ${TestsSources} ${Year}/${ProblemNum}.cpp)
    endif()
  endforeach(ProblemNum)
  source_group(${Year} ${Year}/*)
endmacro()

append_tests_srcs(AocTestsSources 2015 01 02 03    05 06 07 08 09    11 12 13    15          19 20       23 24 25)
append_tests_srcs(AocTestsSources 2016 01 02 03 04 05 06 07 08 09 10)
append_tests_srcs(AocTestsSources 2017 01 02 03 04 05 06 07 08 09 10)
append_tests_srcs(AocTestsSources 2018 01 02 03 04 05 06 07 08 09    11)

# AocLibTests
add_executable(AocLibTests
  ${AocTestsSources}
  AoC_utils_tests/2d_array.cpp
  AoC_utils_tests/algo.cpp
  AoC_utils_tests/anon_ret.cpp
  AoC_utils_tests/fusion.cpp
  AoC_utils_tests/geo.cpp
  AoC_utils_tests/match.cpp
  AoC_utils_tests/md5.cpp
  AoC_utils_tests/ranges.cpp
  AoC_utils_tests/string.cpp
  AoC_utils_tests/zipper.cpp
  impl_test.cpp
  aoc_fixture.cpp
  aoc_fixture.h
  boost_unit_test_module.cpp)
target_link_libraries(AocLibTests PRIVATE AocProblems AocUtils Boost::boost Boost::unit_test_framework)
target_include_directories(AocLibTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


find_program(Bash bash)
if(Bash)

  function(add_aoc_test problem result1 result2)
    if(NOT ${problem} IN_LIST AocExcludedProblems)
      set(App $<TARGET_FILE:AocApp>)
      set(Input ${CMAKE_CURRENT_SOURCE_DIR}/input/${problem})

      add_test(NAME ${problem}_1
        COMMAND bash -c "${App} ${problem}_1 < ${Input}" )
      set_tests_properties(${problem}_1 PROPERTIES PASS_REGULAR_EXPRESSION "^${result1}\n$")

      add_test(NAME ${problem}_2
        COMMAND bash -c "${App} --problem ${problem}_2 < ${Input}")
      set_tests_properties(${problem}_2 PROPERTIES PASS_REGULAR_EXPRESSION "^${result2}\n$")
    endif()
  endfunction()

  add_test(NAME AocUtilsTests
    COMMAND $<TARGET_FILE:AocLibTests> --run_test=*AoC_utils*)

  add_test(NAME AocLibTests_2015
      COMMAND $<TARGET_FILE:AocLibTests> --run_test=*2015*)

  add_test(NAME AocLibTests_2016
      COMMAND $<TARGET_FILE:AocLibTests> --run_test=*2016*)

  add_test(NAME AocLibTests_2017
      COMMAND $<TARGET_FILE:AocLibTests> --run_test=*2017*)

  add_test(NAME AocLibTests_2018
      COMMAND $<TARGET_FILE:AocLibTests> --run_test=*2018*)

  add_test(NAME AocLibTestsImpl
      COMMAND $<TARGET_FILE:AocLibTests> --run_test=impl_test)

  add_aoc_test(2015_01 280 1797)
  add_aoc_test(2015_02 1586300 3737498)
  add_aoc_test(2015_03 2592 2360)
  add_aoc_test(2015_04 254575 1038736)
  add_aoc_test(2015_05 255 55)
  add_aoc_test(2015_06 569999 17836115)
  add_aoc_test(2015_07 46065 14134)
  add_aoc_test(2015_08 1333 2046)
  add_aoc_test(2015_09 207 804)
  add_aoc_test(2015_10 252594 3579328)
  add_aoc_test(2015_11 hxbxxyzz hxcaabcc)
  add_aoc_test(2015_12 119433 68466)
  add_aoc_test(2015_13 618 601)
  add_aoc_test(2015_14 2660 1256)
  add_aoc_test(2015_15 13882464 11171160)
  add_aoc_test(2015_16 103 405)
  add_aoc_test(2015_17 4372 4)
  add_aoc_test(2015_18 821 886)
  add_aoc_test(2015_19 576 0)
  add_aoc_test(2015_20 665280 705600)
  add_aoc_test(2015_21 121 201)
  add_aoc_test(2015_22 1824 1937)
  add_aoc_test(2015_23 307 160)
  add_aoc_test(2015_24 11266889531 77387711)
  add_aoc_test(2015_25 9132360 0)

  add_aoc_test(2016_01 273 115)
  add_aoc_test(2016_02 78293 AC8C8)
  add_aoc_test(2016_03 983 1836)
  add_aoc_test(2016_04 278221 267)
  add_aoc_test(2016_05 4543c154 1050cbbd)
  add_aoc_test(2016_06 wkbvmikb evakwaga)
  add_aoc_test(2016_07 118 260)
  add_aoc_test(2016_08 110 zjhrkcplyj)
  add_aoc_test(2016_09 107035 11451628995)
  add_aoc_test(2016_10 161 133163)

  add_aoc_test(2017_01 1034 1356)
  add_aoc_test(2017_02 58975 308)
  add_aoc_test(2017_03 438 266330)
  add_aoc_test(2017_04 455 186)
  add_aoc_test(2017_05 355965 26948068)
  add_aoc_test(2017_06 3156 1610)
  add_aoc_test(2017_07 dgoocsw 1275)
  add_aoc_test(2017_08 5752 6366)
  add_aoc_test(2017_09 10800 4522)
  add_aoc_test(2017_10 826 d067d3f14d07e09c2e7308c3926605c4)

  add_aoc_test(2018_01 411 56360)
  add_aoc_test(2018_02 5681 uqyoeizfvmbistpkgnocjtwld)
  add_aoc_test(2018_03 116491 707)
  add_aoc_test(2018_04 39584 55053)
  add_aoc_test(2018_05 11242 5492)
  add_aoc_test(2018_06 3358 45909)
  add_aoc_test(2018_07 GJFMDHNBCIVTUWEQYALSPXZORK 1050)
  add_aoc_test(2018_08 47464 23054)
  add_aoc_test(2018_09 398730 3349635509)
  add_aoc_test(2018_11 34,72 233,187,13)

endif()
