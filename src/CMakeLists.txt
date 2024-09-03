

add_executable(${PROJECT_NAME} WIN32
               #main.cpp
               MyApp.cpp
               MyFrame.cpp
               OGLPane.cpp
               Antenna.cpp
               ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c
              )
