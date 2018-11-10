find_program(Bash bash)
find_program(Diff diff)

if(Bash)

  enable_testing()

  function(add_aoc_test problem result1 result2)
    add_test(NAME ${problem}_1
      COMMAND bash -c "$<TARGET_FILE:AocApp> ${problem}_1 < ${CMAKE_CURRENT_SOURCE_DIR}/tests/input/${problem}")
    set_tests_properties(${problem}_1 PROPERTIES PASS_REGULAR_EXPRESSION "^${result1}\n$")
    add_test(NAME ${problem}_2
      COMMAND bash -c "$<TARGET_FILE:AocApp> --problem ${problem}_2 < ${CMAKE_CURRENT_SOURCE_DIR}/tests/input/${problem}")
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
  add_aoc_test(2015_20 665280 705600)
  add_aoc_test(2015_21 121 201)

endif()
