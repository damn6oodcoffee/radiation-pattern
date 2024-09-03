
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)


project(wx_modern_ogl)

add_subdirectory(external)
add_subdirectory(src)

#target_link_libraries(${PROJECT_NAME} PRIVATE wxWidgets_external)
#target_link_libraries(${PROJECT_NAME} PRIVATE glew_external)
#target_link_libraries(${PROJECT_NAME} PRIVATE glad_external)
target_link_libraries(${PROJECT_NAME} PRIVATE external_deps)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)