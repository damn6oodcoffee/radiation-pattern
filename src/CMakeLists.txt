add_executable(RadiationPattern WIN32
               Angle.cpp
               Angle.h
               AntennasGrid.cpp
               AntennasGrid.h
               CreatePattern.cpp 
               CreatePattern.h  
               MyApp.cpp
               MyApp.h  
               myfont.h  
               MyFrame.cpp 
               MyFrame.h  
               OGLPane.cpp 
               OGLPane.h  
               RenderTimer.cpp  
               RenderTimer.h
               )


target_link_libraries(RadiationPattern PRIVATE external_deps)