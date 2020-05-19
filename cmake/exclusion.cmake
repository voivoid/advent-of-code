if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  if(${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER "10")
    set(AocExcludedProblems 2015_13)
  elseif(${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER "9")
    set(AocExcludedProblems 2015_13     # gcc 9.* segfaults
                            2015_18     # gcc 9.* segfaults
                            2016_07     # gcc 9.* segfaults ( debug build only )
    )

    if(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "9.3")
      list(APPEND AocExcludedProblems 2016_08     # gcc 9.1, 9.2 segfaults
                                      2017_03     # gcc 9.1, 9.2 segfaults
                                      2019_03     # gcc 9.1, 9.2 segfaults
      )
    endif()
  elseif(${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER "8")
    set(AocExcludedProblems 2016_05)
  endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(AocExcludedProblems 2015_14
                          2015_16
                          2015_19
                          2016_04
                          2016_05
                          2016_09
                          2017_14
                          2018_03
                          2018_04
                          2018_07
                          2018_11
                          2019_02
                          2019_03

                          2015_23 # HOF
                          2016_07 # HOF
                          2016_08 # HOF
                          2017_02 # HOF
                          2017_16 # HOF
                          2017_18 # HOF
                          2017_19 # HOF
                          2017_20 # HOF
                          2018_05 # HOF
  )
endif()

foreach(Problem ${AocExcludedProblems})
  message("Problem ${Problem} would not be built due to ${CMAKE_CXX_COMPILER_ID} compiler issues")
endforeach()
