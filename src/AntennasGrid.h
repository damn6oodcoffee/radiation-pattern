#pragma once
#include "wx\wx.h""
#include "wx\graphics.h"
#include "wx\dcbuffer.h"
#include <vector>


class AntennaCell : public wxPanel {
public:
	AntennaCell(wxWindow* parent, const wxPoint& pos, const wxSize& size);
	bool IsMarked();
private:
	bool bMarked = false;
	void OnClicked(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);

	wxDECLARE_EVENT_TABLE();
};

class AntennasGrid : public wxPanel
{
public:
	AntennasGrid(wxWindow* parent, const wxPoint& pos, const wxSize& size, int cols, int rows);
	std::vector<std::vector<AntennaCell*>> antennaCells;
};

