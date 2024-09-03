
#include "MyApp.hpp"
#include <filesystem>

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
	// TODELETE:
    std::filesystem::current_path("../../../");
    //
    if (!wxApp::OnInit())
        return false;

    MyFrame* frame = new MyFrame("Hello OpenGL");
    frame->Show();
    return true;
}