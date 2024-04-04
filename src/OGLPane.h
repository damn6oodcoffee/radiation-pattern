#pragma once

#include "wx/wx.h"
#include "wx/glcanvas.h"
#include <gl/gl.h>
#include <gl/GLU.h>
#include "CreatePattern.h"

using namespace std;

class OGLPane : public wxGLCanvas
{
	wxGLContext* m_context;
	bool bLeftDown = false;
	bool bRightDown = false;

	double zAngle = 0;
	double xyAngle = 0;
	double offsetX = 0;
	double offsetY = 0;
	wxPoint mouseClickPt;
	//double zoomScale = 1.0;
	double zoomScale = 2.44140625;

	double xmin = 0;
	double xmax = 1;
	double ymin = 0;
	double ymax = 1;
	double xlen = 1;
	double ylen = 1;

	vector<vector<Vertex3>> vertices;
	void DrawSurface();

public:
	OGLPane(wxFrame* parent, int* args);
	virtual ~OGLPane();

	void resized(wxSizeEvent& evt);

	int getWidth();
	int getHeight();

	void render(wxPaintEvent& evt);
	void SetProjection();
	void drawAxisLabels();
	void SetBoundaries(double xmin, double xmax, double ymin, double ymax);
	void SetVertices(vector<vector<Vertex3>> vx);

	// events
	void mouseMoved(wxMouseEvent& event);
	void leftDown(wxMouseEvent& event);
	void leftUp(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void rightDown(wxMouseEvent& event);
	void rightUp(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);

	DECLARE_EVENT_TABLE()
};


void drawBox(double scale);
void drawAxisBox(double scalex, double scaley, double scalez);
void drawBoxGrid();
