if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  if(${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER "10")
    if(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "10.2")
      set(AocExcludedProblems 2015_13)
    endif()
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
  set(AocExcludedProblems 2015_23 # Boost HOF
                          2016_07 # Boost HOF
                          2016_08 # Boost HOF
                          2017_02 # Boost HOF
                          2017_16 # Boost HOF
                          2017_18 # Boost HOF
                          2017_19 # Boost HOF
                          2017_20 # Boost HOF
                          2018_05 # Boost HOF
  )
endif()

foreach(Problem ${AocExcludedProblems})
  message("Problem ${Problem} would not be built due to ${CMAKE_CXX_COMPILER_ID} compiler issues")
endforeach()
