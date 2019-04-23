find_package(Patch REQUIRED)

include(ExternalProject)

set(BoostVersion "1.70.0")
set(BoostLibs test program_options context)
set(BoostSHA256 882b48708d211a5f48e60b0124cf5863c1534cd544ecd0664bb534a4b5d506e9)


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
    set(BoostAddressModel "address-model=64")
    set(BoostAddressModelTag "-x64")
  else()
    set(BoostAddressModel "address-model=32")
    set(BoostAddressModelTag "-x32")
  endif()

  set(BoostLayout "--layout=tagged")
  set(BoostAsmFlags "asmflags=\\safeseh")
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
  PATCH_COMMAND ${Patch_EXECUTABLE} -s -p0 < ${CMAKE_CURRENT_LIST_DIR}/boost.patch # https://github.com/boostorg/safe_numerics/issues/71
  CONFIGURE_COMMAND ${BoostBootstrapCmd} ${BoostBootstrapToolset}
  BUILD_COMMAND ${BoostB2} ${BoostB2Toolset} link=static threading=multi runtime-link=shared ${BoostLayout} ${BoostAddressModel} ${BoostBuildVariant} ${BoostLibsCmdLine} ${BoostAsmFlags} -j 4
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
      IMPORTED_LOCATION_RELEASE "${BoostLibDir}/libboost_${Component}-mt${BoostAddressModelTag}.lib")
    set_property(TARGET Boost::${Component} APPEND PROPERTY
      IMPORTED_CONFIGURATIONS DEBUG)
    set_target_properties(Boost::${Component} PROPERTIES
      IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
      IMPORTED_LOCATION_DEBUG "${BoostLibDir}/libboost_${Component}-mt-gd${BoostAddressModelTag}.lib")
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
