cmake_minimum_required(VERSION 3.27)
project(PlotRendering)

find_package(OpenGL REQUIRED)
set(Freetype_DIR "${CMAKE_SOURCE_DIR}/../../../freetype/bin/lib/cmake/freetype")
find_package(Freetype REQUIRED PATHS Freetype_DIR)


set(GLFW_DIR "${CMAKE_SOURCE_DIR}/../../../glfw-3.4")
set(GLFW_DIR_LIB ${GLFW_DIR}/build/src)
set(GLAD_DIR "${CMAKE_SOURCE_DIR}/../../../glad")
set(GLM_DIR "${CMAKE_SOURCE_DIR}/../../../glm")


add_executable(${PROJECT_NAME} 
               plot_rendering2.cpp
               Antenna.cpp
               ${GLAD_DIR}/src/glad.c)

target_include_directories(${PROJECT_NAME} PRIVATE 
                           ${GLFW_DIR}/include
                           ${GLAD_DIR}/include 
                           ${GLM_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE 
                      ${OpenGL_LIBRARIES} 
                      ${GLFW_DIR_LIB}/Debug/glfw3.lib
                      Freetype::Freetype)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)