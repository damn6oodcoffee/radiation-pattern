#include "AntennasGrid.h"



wxBEGIN_EVENT_TABLE(AntennaCell, wxPanel)
EVT_LEFT_UP(AntennaCell::OnClicked)
EVT_PAINT(AntennaCell::OnPaint)
wxEND_EVENT_TABLE()

AntennaCell::AntennaCell(wxWindow* parent, const wxPoint& pos, const wxSize& size) : wxPanel(parent, wxID_ANY, pos, size, wxFULL_REPAINT_ON_RESIZE)
{
	
	this->SetBackgroundStyle(wxBG_STYLE_PAINT); // needed for windows
	return;
}

bool AntennaCell::IsMarked() {
	return bMarked;
}

void AntennaCell::OnPaint(wxPaintEvent &event) {
	
	Refresh();
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

	double width = GetSize().GetWidth();
	double height = GetSize().GetHeight();

	if (gc) {
		//gc->DrawRoundedRectangle(0, 0, width, height, width / 10);
		gc->SetPen(*wxBLACK_PEN);
		gc->DrawRectangle(0, 0, width-1, height-1);
		if (bMarked) {
			wxPen olivePen(wxColor("Dark Green"), 0);
			wxBrush greenBrush(wxColor("Green"), wxBRUSHSTYLE_SOLID);
			gc->SetPen(olivePen);
			gc->SetBrush(greenBrush);
			gc->DrawEllipse(width / 2 - width / 10, height / 2 - height / 10, width / 5, height / 5);

		}
	}
}

void AntennaCell::OnClicked(wxMouseEvent& event) {
	bMarked = !bMarked;
	Refresh();
}


AntennasGrid::AntennasGrid(wxWindow* parent, const wxPoint& pos, const wxSize& size, int rows, int cols) : wxPanel(parent, wxID_ANY, pos, size)
{
	
	auto gridSizer = new wxGridSizer(rows, cols, 0, 0);
	antennaCells.resize(rows);
	for (auto& row : antennaCells) {
		row.resize(cols);
	}

	for (auto& row : antennaCells) {
		for (auto& cell : row) {
			cell = new AntennaCell(this, wxDefaultPosition, wxDefaultSize);
			gridSizer->Add((wxPanel*) cell, 1, wxEXPAND | wxALL, FromDIP(5));
		}
	}

	SetSizerAndFit(gridSizer);
	Layout();

	return;
}
