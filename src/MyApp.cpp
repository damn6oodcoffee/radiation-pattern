#include "MyApp.h"

#include "MyApp.h"


wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
	wxInitAllImageHandlers();
	MyFrame* frame = new MyFrame("Radiation Pattern", wxDefaultPosition, wxDefaultSize);
	frame->Show(true);
	return true;
}