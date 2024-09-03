
add_library(glad_external INTERFACE)

target_include_directories(glad_external INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
#target_sources(${PROJECT_NAME} PRIVATE src/glad.c)