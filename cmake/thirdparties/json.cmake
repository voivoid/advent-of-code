include(ExternalProject)

find_package(Git REQUIRED)

set(JsonVer v3.5.0)

ExternalProject_Add(
  get_json
  PREFIX thirdparties
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG ${JsonVer}
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
