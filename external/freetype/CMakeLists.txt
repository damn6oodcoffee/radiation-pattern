
message("Loading Freetype...")

add_library(freetype_external INTERFACE)

if(ON)
    message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
    include(ExternalProject)
    ExternalProject_Add(freetype
        URL https://download.savannah.gnu.org/releases/freetype/ft2132.zip
        #SOURCE_DIR ${CMAKE_SOURCE_DIR}/freetype_library
        #SOURCE_SUBDIR freetype-prefix/src/freetype
        CONFIGURE_COMMAND cmake ../freetype -B build
        BUILD_COMMAND cmake --build build --config Release
        INSTALL_COMMAND cmake --install ./build --prefix ./bin
    )
    ExternalProject_Get_property(freetype BINARY_DIR)
    message("Binary DIR of FREETYPE : ${BINARY_DIR}")
    # find_package(Freetype REQUIRED PATHS ${BINARY_DIR}/bin/lib/cmake/freetype)
    # target_link_libraries(freetype_external Freetype::Freetype)
    target_include_directories(freetype_external INTERFACE ${BINARY_DIR}/bin/include/freetype2)
    target_link_libraries(freetype_external INTERFACE ${BINARY_DIR}/bin/lib/freetype.lib)
endif()
