
message("Loading glm...")

add_library(glm_external INTERFACE)

FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/releases/download/1.0.1/glm-1.0.1-light.zip
    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/glm
)
FetchContent_GetProperties(glm)
if (NOT glm_POPULATED)
    FetchContent_Populate(glm)
endif()

message("glm info:")
message("glm_SOURCE_DIR: " ${glm_SOURCE_DIR})
message("glm BINARY DIR: " ${glm_BINARY_DIR})
message("=========================")

target_include_directories(glm_external INTERFACE ${glm_SOURCE_DIR}/../)