# AocLibTests
add_executable(AocLibTests
  2015_01.cpp
  2015_02.cpp
  2015_03.cpp
  2015_05.cpp
  2015_06.cpp
  2015_07.cpp
  2015_08.cpp
  2015_09.cpp
  2015_11.cpp
  2015_12.cpp
  2015_13.cpp
  2015_15.cpp
  2015_19.cpp
  2015_20.cpp
  2015_23.cpp
  2015_24.cpp
  2015_25.cpp
  2016_01.cpp
  impl_test.cpp
  AoC_utils_tests/algo.cpp
  AoC_utils_tests/anon_ret.cpp
  AoC_utils_tests/geo.cpp
  AoC_utils_tests/match.cpp
  AoC_utils_tests/md5.cpp
  AoC_utils_tests/ranges.cpp
  AoC_utils_tests/ranges_coro.cpp
  AoC_utils_tests/string.cpp
  AoC_utils_tests/utils.cpp
  utils/aoc_fixture.cpp
  utils/aoc_fixture.h
  utils/boost_unit_test_module.cpp)
target_link_libraries(AocLibTests PRIVATE AocProblems AocUtils Boost::boost Boost::unit_test_framework)


find_program(Bash bash)
if(Bash)

  function(add_aoc_test problem result1 result2)
    set(App $<TARGET_FILE:AocApp>)
    set(Input ${CMAKE_CURRENT_SOURCE_DIR}/input/${problem})

    add_test(NAME ${problem}_1
      COMMAND bash -c "${App} ${problem}_1 < ${Input}" )
    set_tests_properties(${problem}_1 PROPERTIES PASS_REGULAR_EXPRESSION "^${result1}\n$")

    add_test(NAME ${problem}_2
      COMMAND bash -c "${App} --problem ${problem}_2 < ${Input}")
    set_tests_properties(${problem}_2 PROPERTIES PASS_REGULAR_EXPRESSION "^${result2}\n$")
  endfunction()

  add_test(NAME AocUtilsTests
    COMMAND $<TARGET_FILE:AocLibTests> --run_test=*aoc_utils*)

  add_test(NAME AocLibTests_2015
    COMMAND $<TARGET_FILE:AocLibTests> --run_test=*2015*)

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

endif()
