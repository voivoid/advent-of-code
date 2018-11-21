include(ExternalProject)

set(BoostVersion "1.68.0")
set(BoostLibs test program_options context)
set(BoostSHA256 da3411ea45622579d419bfda66f45cd0f8c32a181d84adfa936f5688388995cf)


if(WIN32)
  set(BoostBootstrapCmd cmd /C "<SOURCE_DIR>/bootstrap.bat")
  set(BoostB2 "<SOURCE_DIR>/b2.exe")
else()
  set(BoostBootstrapCmd $ENV{SHELL} "<SOURCE_DIR>/bootstrap.sh")
  set(BoostB2 "<SOURCE_DIR>/b2")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(BoostToolset "gcc")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(BoostToolset "clang")
endif()

if(BoostToolset)
  set(BoostBootstrapToolset "--with-toolset=${BoostToolset}")
  set(BoostB2Toolset "toolset=${BoostToolset}")
endif()

if(UNIX AND NOT "${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
  set(BoostBuildVariant "variant=debug")
endif()

if(MSVC)
  if("${CMAKE_GENERATOR}" MATCHES "Win64")
    set(BoostAdressModel "address-model=64")
  else()
    set(BoostAdressModel "address-model=32")
  endif()

  set(BoostLayout "--layout=tagged")
endif()

foreach(BoostLib ${BoostLibs})
  list(APPEND BoostLibsCmdLine "--with-${BoostLib}")
endforeach()

string(REPLACE . _ BoostVersionUnderscored ${BoostVersion})
ExternalProject_Add(
  get_boost
  PREFIX thirdparties
  URL "https://dl.bintray.com/boostorg/release/${BoostVersion}/source/boost_${BoostVersionUnderscored}.tar.gz"
  URL_HASH SHA256=${BoostSHA256}
  CONFIGURE_COMMAND ${BoostBootstrapCmd} ${BoostBootstrapToolset}
  BUILD_COMMAND ${BoostB2} ${BoostB2Toolset} link=static threading=multi runtime-link=shared ${BoostLayout} ${BoostAdressModel} ${BoostBuildVariant} ${BoostLibsCmdLine} -j 4
  BUILD_IN_SOURCE TRUE
  INSTALL_COMMAND ""
  )


ExternalProject_Get_Property(get_boost SOURCE_DIR)
set(BoostSrcDir ${SOURCE_DIR})
set(BoostLibDir ${BoostSrcDir}/stage/lib)

add_library(boost_headers INTERFACE)
add_library(Boost::boost ALIAS boost_headers)
set_target_properties(boost_headers PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${BoostSrcDir})
set_target_properties(boost_headers PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${BoostSrcDir})

if(MSVC)
  set_property(TARGET boost_headers PROPERTY INTERFACE_COMPILE_DEFINITIONS BOOST_ALL_NO_LIB)
endif()

add_library(Boost::program_options STATIC IMPORTED)
add_library(Boost::unit_test_framework STATIC IMPORTED)
add_library(Boost::context STATIC IMPORTED)

macro(set_boost_libs_location Component)
  if(MSVC)
    set_property(TARGET Boost::${Component} APPEND PROPERTY
      IMPORTED_CONFIGURATIONS RELEASE)
    set_target_properties(Boost::${Component} PROPERTIES
      IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
      IMPORTED_LOCATION_RELEASE "${BoostLibDir}/libboost_${Component}-mt.lib")
    set_property(TARGET Boost::${Component} APPEND PROPERTY
      IMPORTED_CONFIGURATIONS DEBUG)
    set_target_properties(Boost::${Component} PROPERTIES
      IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
      IMPORTED_LOCATION_DEBUG "${BoostLibDir}/libboost_${Component}-mt-gd.lib")
  else()
    set_target_properties(Boost::${Component} PROPERTIES
      IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
      IMPORTED_LOCATION "${BoostLibDir}/libboost_${Component}.a")
  endif()
endmacro()

set_boost_libs_location(program_options)
set_boost_libs_location(unit_test_framework)
set_boost_libs_location(context)

add_dependencies(boost_headers get_boost)
add_dependencies(Boost::program_options get_boost)
add_dependencies(Boost::unit_test_framework get_boost)
add_dependencies(Boost::context get_boost)
