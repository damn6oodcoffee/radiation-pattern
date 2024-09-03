
include(FetchContent)
#add_subdirectory(glew)
add_subdirectory(wxwidgets)
add_subdirectory(glad)
add_subdirectory(freetype)
add_subdirectory(glm)

add_library(external_deps INTERFACE)
target_link_libraries(external_deps INTERFACE
                      wxWidgets_external
                      glad_external
                      freetype_external
                      glm_external)