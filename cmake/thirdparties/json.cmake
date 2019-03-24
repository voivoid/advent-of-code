include(ExternalProject)

find_package(Git REQUIRED)

set(JsonVer v3.6.1)

ExternalProject_Add(
  get_json
  PREFIX thirdparties
  URL https://github.com/nlohmann/json/archive/${JsonVer}.zip
  URL_HASH SHA256=5de16b67ee5efed9cf114a29384abe0ae449b40367a16f48e8c6fa9ccba73634
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  UPDATE_COMMAND ""
  )

add_library(json INTERFACE)

ExternalProject_Get_Property(get_json SOURCE_DIR)
set_target_properties(json PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR}/include)
set_target_properties(json PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${SOURCE_DIR}/include)

add_dependencies(json get_json)
