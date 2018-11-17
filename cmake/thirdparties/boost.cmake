include(FetchContent)

set(BoostVersion "1.68.0")
set(BoostLibs test program_options)
set(BoostSHA256 da3411ea45622579d419bfda66f45cd0f8c32a181d84adfa936f5688388995cf)
set(Boost_USE_STATIC_LIBS TRUE)

string(REPLACE . _ BoostVersionUnderscored ${BoostVersion})
FetchContent_Declare(
  boost
  URL "https://dl.bintray.com/boostorg/release/${BoostVersion}/source/boost_${BoostVersionUnderscored}.tar.gz"
  URL_HASH SHA256=${BoostSHA256}
  )

message("Building Boost...")
FetchContent_Populate(boost)
FetchContent_GetProperties(boost)


set(BoostSrcDir ${boost_SOURCE_DIR})
set(BoostLibDir ${BoostSrcDir}/stage/lib)

foreach(BoostLib ${BoostLibs})
  list(APPEND BoostLibsCmdLine "--with-${BoostLib}")
endforeach()

string(REPLACE test unit_test_framework BoostComponents "${BoostLibs}")


if(DEFINED BoostLibsCmdLine)

  if(WIN32)
    set(BoostBootstrapCmd cmd /C ${BoostSrcDir}/bootstrap.bat)
    set(BoostB2 ${BoostSrcDir}/b2.exe)
  else()
    set(BoostBootstrapCmd $ENV{SHELL} ${BoostSrcDir}/bootstrap.sh)
    set(BoostB2 ${BoostSrcDir}/b2)
  endif()

  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(BoostToolset "gcc")
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(BoostToolset "clang")
  endif()

  if(BoostToolset)
    set(BoostBootstrapToolset "--with-toolset=${BoostToolset}")
    set(BoostB2Toolset "toolset=${BoostToolset}")
  endif()

  if(EXISTS ${BoostB2} )
    message("Boost is already bootstrapped")
  else()
    message("Bootstrapping boost...")
    execute_process(COMMAND ${BoostBootstrapCmd} ${BoostBootstrapToolset}
      WORKING_DIRECTORY ${BoostSrcDir}
      RESULT_VARIABLE BoostBootstrapResult)

    if( NOT ${BoostBootstrapResult} EQUAL 0 )
      message(ERROR "${BoostBoostBootstrapCmd} failed")
    endif()

  endif()


  file(
    GLOB BoostBuiltLibs
    RELATIVE ${BoostLibDir}
    ${BoostLibDir}/*)

  set(BoostAllLibsBuilt TRUE)
  foreach(Component ${BoostComponents})
    string(FIND "${BoostBuiltLibs}" ${Component} ComponentFound)
    if(ComponentFound EQUAL -1)
      set(BoostAllLibsBuilt FALSE)
    endif()
  endforeach()

  if(BoostAllLibsBuilt)
    message("Boost libraries are already built")
  else()
    message("Building boost libraries...")
    set(BoostB2Cmd ${BoostB2} ${BoostB2Toolset} ${BoostLibsCmdLine} -j 8)
    execute_process(COMMAND ${BoostB2Cmd}
      WORKING_DIRECTORY ${BoostSrcDir}
      RESULT_VARIABLE BoostB2Result)

    if( NOT ${BoostB2Result} EQUAL 0 )
      message(ERROR "${BoostB2} failed")
    endif()

  endif()

endif()



set(BOOST_ROOT ${BoostSrcDir})
find_package(Boost ${BoostVersion} REQUIRED COMPONENTS ${BoostComponents})

message("Building boost done")
