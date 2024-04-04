#pragma once
#include "wx\wx.h"
#include "AntennasGrid.h"
#include "OGLPane.h"
#include "RenderTimer.h"
#include <mutex>

class MyFrame : public wxFrame
{
public:
	MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
	wxButton* createButton;
	wxButton* changeGridButton;
	wxTextCtrl* textCtrlR;
	wxTextCtrl* textCtrlLambda;
	wxTextCtrl* textCtrlDistance;
	wxTextCtrl* textCtrlGridWidth;
	wxTextCtrl* textCtrlGridHeight;

	wxBoxSizer* horSizer;
	mutex* mtx;
	AntennasGrid* antGrid;
	OGLPane* oglPane;
	RenderTimer* timer;

	void OnCreatePattern(wxCommandEvent& event);
	void OnChangeGrid(wxCommandEvent& event);
};

