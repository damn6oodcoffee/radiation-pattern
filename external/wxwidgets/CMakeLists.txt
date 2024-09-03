
message("Searching for wxWidgets")

add_library(wxWidgets_external INTERFACE)
find_package(wxWidgets COMPONENTS core base gl REQUIRED)
target_link_libraries(wxWidgets_external INTERFACE wxWidgets::wxWidgets)
#target_include_directories(wxWidgets_external INTERFACE ${wxWidgets_INCLUDE_DIRS})

message("wxWidgets include dirs")
message(${wxWidgets_INCLUDE_DIRS})
message("===========================")