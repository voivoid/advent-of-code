include(FetchContent)

find_package(Git REQUIRED)

set(JsonVer v3.4.0)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG ${JsonVer}
)

message("Building Json ...")
FetchContent_Populate(json)
FetchContent_GetProperties(json)

add_library(json INTERFACE)
set_target_properties(json PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${json_SOURCE_DIR}/include)
set_target_properties(json PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${json_SOURCE_DIR}/include)

message("Building Json done")



