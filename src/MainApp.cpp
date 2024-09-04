
#include "MainApp.hpp"
#include <filesystem>

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {
	// TODELETE:
    std::filesystem::current_path("../../../");
    //
    if (!wxApp::OnInit())
        return false;

    MainFrame* frame = new MainFrame("Hello OpenGL");
    frame->Show();
    return true;
}