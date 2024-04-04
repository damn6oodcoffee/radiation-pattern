#include "OGLPane.h"


BEGIN_EVENT_TABLE(OGLPane, wxGLCanvas)
EVT_MOTION(OGLPane::mouseMoved)
EVT_LEFT_DOWN(OGLPane::leftDown)
EVT_LEFT_UP(OGLPane::leftUp)
EVT_RIGHT_DOWN(OGLPane::rightDown)
EVT_RIGHT_UP(OGLPane::rightUp)
EVT_LEAVE_WINDOW(OGLPane::mouseLeftWindow)
EVT_SIZE(OGLPane::resized)
EVT_KEY_DOWN(OGLPane::keyPressed)
EVT_KEY_UP(OGLPane::keyReleased)
EVT_MOUSEWHEEL(OGLPane::mouseWheelMoved)
EVT_PAINT(OGLPane::render)
END_EVENT_TABLE()

OGLPane::OGLPane(wxFrame* parent, int* args) :
	wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
	m_context = new wxGLContext(this);

	// To avoid flashing on MSW
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

}

OGLPane::~OGLPane()
{
	delete m_context;
}

int OGLPane::getWidth()
{
	return GetSize().x;
}

int OGLPane::getHeight()
{
	return GetSize().y;
}

void OGLPane::resized(wxSizeEvent& evt)
{
	//	wxGLCanvas::OnSize(evt);

	Refresh();
}

void OGLPane::SetProjection()
{


	glFrontFace(GL_CW);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0f, 2.0, -2.0f, 2.0f, 0.0f, 100.0f);
	//gluPerspective(45.0f, (GLfloat)getWidth() / (GLfloat)getHeight(), 0.1f, 100.0f);


	glViewport(0, 0, getWidth(), getHeight());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}


#include "myfont.h"


void OGLPane::render(wxPaintEvent& evt)
{

	if (!IsShown()) return;

	wxGLCanvas::SetCurrent(*m_context);
	wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ------------- draw some 3D ----------------
	SetProjection();
	glLoadIdentity();

	// Point the camera at the origin from the positive octant (x,y,z > 0).
	gluLookAt(5.0f, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);



	// Lighting 
	{
		GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };

		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		GLfloat white[] = { 0.8f, 0.8f, 0.8f, 0.0f };
		GLfloat cyan[] = { 1.f, .0f, .0f, 0.f };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
		glMaterialfv(GL_FRONT, GL_SPECULAR, white);
		glMaterialf(GL_FRONT, GL_SHININESS, 128.0f);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	//

	glRotatef(xyAngle, -1.0f, 1.0f, 0.0f);
	glRotatef(zAngle, 0.0f, 0.0f, 1.0f);
	glTranslatef(offsetX, offsetY, 0);
	glPushMatrix();
	glScalef(zoomScale / (xlen), zoomScale / (ylen), zoomScale);



	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	//glColor3f(1.0, 1.0, 1.0);
	//glRasterPos3f(2 * M_PI, 0.0, 0.0);
	//for (int i = 0; i < 96; i++) {
	//	glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, rasters[i]);
	//}
	/*
	string tickLabel;
	int precision = 2;
	glColor3f(1.0, 1.0, 1.0);
	int axisTicks = 4;
	double dx = 2 * M_PI / axisTicks;
	for (int i = 0; i <= axisTicks; i++) {
		glRasterPos3f(dx * i, M_PI / 2 * 1.05, 0.0);
		tickLabel = std::to_string(dx * i * 180.0 / M_PI);
		precision = 2;
		for (auto& ch : tickLabel) {
			glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, rasters[ch-32]);
			if (ch == '.' || precision != 2) precision--;
			if (precision == 0) break;
		}
	}

	double dy = M_PI / axisTicks / 2;
	for (int i = 0; i <= axisTicks; i++) {
		glRasterPos3f(2 * M_PI * 1.1, dy * i, 0.0);
		tickLabel = std::to_string(dy * i * 180.0 / M_PI);
		precision = 2;
		for (auto& ch : tickLabel) {
			glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, rasters[ch - 32]);
			if (ch == '.' || precision != 2) precision--;
			if (precision == 0) break;
		}
	}

	double dz = 1.0 / axisTicks;
	for (int i = 1; i <= axisTicks; i++) {
		glRasterPos3f(2 * M_PI * 0, M_PI / 2 * 1.05, dz * i);
		tickLabel = std::to_string(dz * i);
		precision = 2;
		for (auto& ch : tickLabel) {
			glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, rasters[ch - 32]);
			if (ch == '.' || precision != 2) precision--;
			if (precision == 0) break;
		}
	}

	glColor3f(0.0, 0.0, 1.0);
	glRasterPos3f(2 * M_PI * 1.35, M_PI / 4, 0.0);
	tickLabel = "elevation";
	for (auto& ch : tickLabel) {
		glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, rasters[ch - 32]);
		if (ch == '.' || precision != 2) precision--;
		if (precision == 0) break;
	}
	glColor3f(0.0, 1.0, 0.0);
	glRasterPos3f(2 * M_PI / 2, M_PI / 2 * 1.2, 0.0);
	tickLabel = "azimuth";
	for (auto& ch : tickLabel) {
		glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, rasters[ch - 32]);
		if (ch == '.' || precision != 2) precision--;
		if (precision == 0) break;
	}
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos3f(0, M_PI / 2 * 1.2, 0.5);
	tickLabel = "amplitude";
	for (auto& ch : tickLabel) {
		glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, rasters[ch - 32]);
		if (ch == '.' || precision != 2) precision--;
		if (precision == 0) break;
	}
	*/
	drawAxisLabels();
	glTranslatef(-xmin, -ymin, 0);
	DrawSurface();
	glTranslatef(xmin, ymin, 0);
	drawAxisBox(xlen, ylen, 1);


	/*
	for (int i = 0; i < silicon->atoms.size(); i++) {
		if (i == 0) {
			auto atom = silicon->atoms[i];
			for (auto& nb : atom.neighbours) {
				glBegin(GL_LINES);
				glColor3f(0.0f, 1.0f, 1.0f);
				glVertex3d(atom.x * 1.5e9, atom.y * 1.5e9, atom.z * 1.5e9);
				glVertex3d(nb->x * 1.5e9, nb->y * 1.5e9, nb->z * 1.5e9);
				glEnd();
			}
		}
	}
	*/

	glPopMatrix();
	glBegin(GL_LINES);
	glColor3f(0, 1, 0); glVertex3d(0.0f, 0.0f, 0.0f); glVertex3d(10.0f, 0.0f, 0.0f);	// x axis.
	glColor3f(0, 0, 1); glVertex3d(0.0f, 0.0f, 0.0f); glVertex3d(0.0f, 10.0f, 0.0f);	// y axis.
	glColor3f(1, 0, 0); glVertex3d(0.0f, 0.0f, 0.0f); glVertex3d(0.0f, 0.0f, 10.0f);	// z axis.
	glEnd();

	
	

	glFlush();
	SwapBuffers();
}



void OGLPane::mouseMoved(wxMouseEvent& event) {
	if (bLeftDown) {
		double scale = 2;
		if (event.GetPosition().x - mouseClickPt.x > 0) zAngle += 0.5 * scale;
		else if (event.GetPosition().x - mouseClickPt.x < 0) zAngle -= 0.5 * scale;
		if (event.GetPosition().y - mouseClickPt.y > 0) xyAngle += 0.5 * scale;
		else if (event.GetPosition().y - mouseClickPt.y < 0) xyAngle -= 0.5 * scale;

		if (abs(zAngle) > 360) zAngle = 0;
		if (abs(xyAngle) > 360) xyAngle = 0;
		mouseClickPt = event.GetPosition();
		//Refresh();
	}
	if (bRightDown) {
		double zAngleRad = -zAngle * M_PI / 180 + M_PI / 4;
		double scale = 0.025;
		/*
		if (event.GetPosition().x - mouseClickPt.x > 0) offsetX += scale;
		else if (event.GetPosition().x - mouseClickPt.x < 0) offsetX -= scale;
		if (event.GetPosition().y - mouseClickPt.y > 0) offsetY += scale;
		else if (event.GetPosition().y - mouseClickPt.y < 0) offsetY -= scale;
		*/

		if (event.GetPosition().x - mouseClickPt.x > 0) { offsetY += scale * cos(zAngleRad); offsetX -= scale * sin(zAngleRad); }
		else if (event.GetPosition().x - mouseClickPt.x < 0) { offsetY -= scale * cos(zAngleRad); offsetX += scale * sin(zAngleRad); }
		if (event.GetPosition().y - mouseClickPt.y > 0) { offsetY += scale * sin(zAngleRad); offsetX += scale * cos(zAngleRad); }
		else if (event.GetPosition().y - mouseClickPt.y < 0) { offsetY -= scale * sin(zAngleRad); offsetX -= scale * cos(zAngleRad); }
		mouseClickPt = event.GetPosition();
		//Refresh();
	}
}

void OGLPane::leftDown(wxMouseEvent& event) {
	bLeftDown = true;
	mouseClickPt = event.GetPosition();
}

void OGLPane::leftUp(wxMouseEvent& event) {
	bLeftDown = false;
}

void OGLPane::mouseWheelMoved(wxMouseEvent& event) {
	int delta = event.GetWheelRotation();
	wxDouble zoomPrev = zoomScale;
	if (delta > 0) {
		zoomScale *= 1.25;
	}
	else {
		zoomScale = (zoomScale / 1.25) > 1.0 ? (zoomScale / 1.25) : 1.0;
	}
	//Refresh();
}

void OGLPane::rightDown(wxMouseEvent& event) {
	bRightDown = true;
}

void OGLPane::rightUp(wxMouseEvent& event) {
	bRightDown = false;
}

void OGLPane::mouseLeftWindow(wxMouseEvent& event) {
	bLeftDown = false;
	bRightDown = false;
}

void OGLPane::keyPressed(wxKeyEvent& event) {}

void OGLPane::keyReleased(wxKeyEvent& event) {}

void OGLPane::SetBoundaries(double xmin, double xmax, double ymin, double ymax) {
	this->xmin = xmin;
	this->xmax = xmax;
	this->ymin = ymin;
	this->ymax = ymax;
	this->xlen = abs(xmax - xmin);
	this->ylen = abs(ymax - ymin);
};

void OGLPane::DrawSurface() {
	if (vertices.empty()) return;
	
	// i - radius
	// j - angle
	for (int i = 0; i < vertices.size() - 1; i++) {
		glBegin(GL_QUAD_STRIP); 
		for (int j = 0; j < vertices[0].size(); j++) {
			glColor3f(vertices[i][j].z, 0 , 1 - vertices[i][j].z);
			glVertex3f(vertices[i][j].x, vertices[i][j].y, vertices[i][j].z);
			glVertex3f(vertices[i + 1][j].x, vertices[i + 1][j].y, vertices[i + 1][j].z);
			
		}
		glColor3f(vertices[i][0].z, 0, 1 - vertices[i][0].z);
		glVertex3f(vertices[i][0].x, vertices[i][0].y, vertices[i][0].z);
		glVertex3f(vertices[i + 1][0].x, vertices[i + 1][0].y, vertices[i + 1][0].z);
		glEnd();
	}


}

void drawBox(double scale)
{
	glPushMatrix();
	glScalef(scale, scale, scale);


	glBegin(GL_LINES);



	glColor3f(1.0f, 1.0f, 1.0f);


	// TOP
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 1.0f);

	glVertex3f(0.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);

	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	// BOTTOM
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	// VERTICAL LINES
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 1.0f);

	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	

	glEnd();
	glPopMatrix();
}
void drawBoxGrid() {

	int ticks = 8;
	double dx;
	double dy;
	double dz;
	dx = dy = dz = 1.0 / ticks;
	glBegin(GL_LINES);

	for (int i = 0; i < ticks; i++) {
		glVertex3f(i * dx, 0.0f, 0.0f);
		glVertex3f(i * dx, 1.0f, 0.0f);

		glVertex3f(0.0f, i * dy, 0.0f);
		glVertex3f(1.0f, i * dy, 0.0f);

		glVertex3f(0.0f, 0.0f, i * dz);
		glVertex3f(0.0f, 1.0f, i * dz);

		glVertex3f(0.0f, 0.0f, i * dz);
		glVertex3f(1.0f, 0.0f, i * dz);

		glVertex3f(i * dx, 0.0f, 0.0f);
		glVertex3f(i * dx, 0.0f, 1.0f);

		glVertex3f(0.0f, i * dy, 0.0f);
		glVertex3f(0.0f, i * dy, 1.0f);
	}


	glEnd();


}


void drawAxisBox(double scalex, double scaley, double scalez)
{
	glPushMatrix();
	glScalef(scalex, scaley, scalez);


	glBegin(GL_LINES);



	glColor3f(1.0f, 1.0f, 1.0f);


	// TOP
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 1.0f);

	//glVertex3f(0.0f, 1.0f, 1.0f);
	//glVertex3f(1.0f, 1.0f, 1.0f);

	//glVertex3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(1.0f, 0.0f, 1.0f);

	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	// BOTTOM
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	// VERTICAL LINES
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 1.0f);

	//glVertex3f(1.0f, 1.0f, 0.0f);
	//glVertex3f(1.0f, 1.0f, 1.0f);

	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);

	

	glEnd();
	drawBoxGrid();
	glPopMatrix();
}

void OGLPane::SetVertices(vector<vector<Vertex3>> vx) {
	vertices = vx;
}

void OGLPane::drawAxisLabels() {

	string xlabel = "U";
	string ylabel = "V";

	string tickLabel;
	int precision = 2;
	glColor3f(1.0, 1.0, 1.0);
	int axisTicks = 4;
	double dx = xlen / axisTicks;
	for (int i = 0; i <= axisTicks; i++) {
		glRasterPos3f(dx * i, ylen * 1.05, 0.0);
		tickLabel = std::to_string(xmin + dx * i);
		precision = 2;
		for (auto& ch : tickLabel) {
			glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, rasters[ch - 32]);
			if (ch == '.' || precision != 2) precision--;
			if (precision == 0) break;
		}
	}

	double dy = ylen/ axisTicks;
	for (int i = 0; i <= axisTicks; i++) {
		
		tickLabel = std::to_string(ymin + dy * i);
		glRasterPos3f(xlen * 1.1, dy * i, 0.0);
		precision = 2;
		for (auto& ch : tickLabel) {
			glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, rasters[ch - 32]);
			if (ch == '.' || precision != 2) precision--;
			if (precision == 0) break;
		}
	}

	double dz = 1.0 / axisTicks;
	for (int i = 1; i <= axisTicks; i++) {
		glRasterPos3f(0, ylen * 1.05, dz * i);
		tickLabel = std::to_string(dz * i);
		precision = 2;
		for (auto& ch : tickLabel) {
			glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, rasters[ch - 32]);
			if (ch == '.' || precision != 2) precision--;
			if (precision == 0) break;
		}
	}

	glColor3f(0.0, 0.4, 1.0);
	glRasterPos3f(xlen * 1.35, ylen / 2, 0.0);
	tickLabel = ylabel;
	for (auto& ch : tickLabel) {
		glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, rasters[ch - 32]);
		if (ch == '.' || precision != 2) precision--;
		if (precision == 0) break;
	}
	glColor3f(0.0, 1.0, 0.0);
	glRasterPos3f(xlen / 2, ylen * 1.2, 0.0);
	tickLabel = xlabel;
	for (auto& ch : tickLabel) {
		glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, rasters[ch - 32]);
		if (ch == '.' || precision != 2) precision--;
		if (precision == 0) break;
	}
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos3f(0, ylen * 1.2, 0.5);
	tickLabel = "Amplitude";
	for (auto& ch : tickLabel) {
		glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, rasters[ch - 32]);
		if (ch == '.' || precision != 2) precision--;
		if (precision == 0) break;
	}
}