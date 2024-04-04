#include "MyFrame.h"
#include "CreatePattern.h"
#include "Angle.h"


MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame(nullptr, wxID_ANY, title, pos, size)
{

	SetBackgroundColour(wxColour(0xe3e3e3));
	mtx = new mutex();
	antGrid = new AntennasGrid(this, wxDefaultPosition, wxDefaultSize, 10, 10);
	antGrid->SetBackgroundColour(wxColour(0xc0c0c0));
	antGrid->SetWindowStyle(wxSUNKEN_BORDER);
	int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };
	oglPane = new OGLPane(this, args);
	auto controlsPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(100,150)); 
	controlsPanel->SetBackgroundColour(wxColour(0xe3e3e3));

	auto vertSizer = new wxBoxSizer(wxVERTICAL);
	horSizer = new wxBoxSizer(wxHORIZONTAL);
	auto gridSizer = new wxGridSizer(3, 5, FromDIP(5), FromDIP(5));

	createButton = new wxButton(controlsPanel, wxID_ANY, "Построить ДН", wxDefaultPosition, wxDefaultSize);
	createButton->Bind(wxEVT_BUTTON, &MyFrame::OnCreatePattern, this);
	changeGridButton = new wxButton(controlsPanel, wxID_ANY, "Изменить", wxDefaultPosition, wxDefaultSize);
	changeGridButton->Bind(wxEVT_BUTTON, &MyFrame::OnChangeGrid, this);
	textCtrlR = new wxTextCtrl(controlsPanel, wxID_ANY);
	textCtrlLambda = new wxTextCtrl(controlsPanel, wxID_ANY);
	textCtrlDistance = new wxTextCtrl(controlsPanel, wxID_ANY);
	textCtrlGridWidth = new wxTextCtrl(controlsPanel, wxID_ANY);
	textCtrlGridHeight = new wxTextCtrl(controlsPanel, wxID_ANY);

	textCtrlGridWidth->SetValue(wxT("10"));
	textCtrlGridHeight->SetValue(wxT("10"));

	textCtrlR->SetValue(wxT("100"));
	textCtrlLambda->SetValue(wxT("1"));
	textCtrlDistance->SetValue(wxT("2"));

	auto staticR = new wxStaticText(controlsPanel, wxID_ANY, "R = ");
	auto staticLambda = new wxStaticText(controlsPanel, wxID_ANY, "Lambda = ");
	auto staticDistance = new wxStaticText(controlsPanel, wxID_ANY, "d = ");
	auto staticGridWidth = new wxStaticText(controlsPanel, wxID_ANY, "Ширина сетки: ");
	auto staticGridHeight = new wxStaticText(controlsPanel, wxID_ANY, "Высота сетки : ");

	gridSizer->Add(staticGridHeight, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	gridSizer->Add(textCtrlGridHeight);
	gridSizer->Add(staticR, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	gridSizer->Add(textCtrlR);
	gridSizer->Add(createButton);
	gridSizer->Add(staticGridWidth, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	gridSizer->Add(textCtrlGridWidth);
	gridSizer->Add(staticLambda, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	gridSizer->Add(textCtrlLambda);
	gridSizer->AddSpacer(1);
	gridSizer->AddSpacer(1);
	gridSizer->Add(changeGridButton);
	gridSizer->Add(staticDistance, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	gridSizer->Add(textCtrlDistance);

	controlsPanel->SetSizerAndFit(gridSizer);

	horSizer->Add((wxPanel*)antGrid, 0, wxSHAPED | wxEXPAND | wxALL, FromDIP(5));
	horSizer->Add(oglPane, 1, wxEXPAND | wxALL, FromDIP(5));
	vertSizer->Add(horSizer, 1, wxEXPAND);
	vertSizer->Add(controlsPanel, 0, wxALL, FromDIP(5));
	SetSizerAndFit(vertSizer);
	Layout();

	
	SetSize(wxSize(1280, 720));

	timer = new RenderTimer(oglPane);
	timer->start();

	timer->mtx = mtx;


	return;
}


void MyFrame::OnCreatePattern(wxCommandEvent& event) {

	timer->Stop();
	int xpts = 256;
	int ypts = 256;
	
	double r = std::stod((string)textCtrlR->GetValue());
	double lambda = std::stod((string)textCtrlLambda->GetValue());
	//double d = lambda * std::stod((string)textCtrlDistance->GetValue());
	double d = std::stod((string)textCtrlDistance->GetValue());

	vector<vector<Vertex3>> verts2d;
	vector<Vertex3> allVerts = GetPatternSurface(antGrid, xpts, ypts, r, lambda, d);

	for (int i = 0; i < xpts; i++) {
		verts2d.push_back(vector<Vertex3>(allVerts.begin() + i * ypts, allVerts.begin() + (i + 1) * ypts));
	}

	oglPane->SetVertices(verts2d);
	//oglPane->SetBoundaries(ToDegree(-M_PI), ToDegree(M_PI), ToDegree(0), ToDegree(M_PI/2));
	oglPane->SetBoundaries(-1, 1, -1, 1);
	oglPane->Refresh();
	timer->Start();
	return;
}

void MyFrame::OnChangeGrid(wxCommandEvent& event) {
	delete antGrid;
	int width = std::stoi((string)textCtrlGridWidth->GetValue());
	int height = std::stoi((string)textCtrlGridHeight->GetValue());
	horSizer->Clear(false);
	antGrid = new AntennasGrid(this, wxDefaultPosition, wxDefaultSize, height, width);
	horSizer->Add((wxPanel*)antGrid, 0, wxSHAPED | wxEXPAND | wxALL, FromDIP(5));
	horSizer->Add(oglPane, 1, wxEXPAND | wxALL, FromDIP(5));
	Layout();
	Refresh();
}