

add_executable(${PROJECT_NAME} WIN32
               #main.cpp
               MainApp.cpp
               MainFrame.cpp
               OGLPane.cpp
               Antenna.cpp
               PlotScene.cpp
               ControlPanel.cpp
               GridPanel.cpp
               ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c
              )
