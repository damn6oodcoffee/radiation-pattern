#ifndef OGLPANE_HPP
#define OGLPANE_HPP

#include <memory>

#include <glad/glad.h>
#include <gl/gl.h>

#include <wx/glcanvas.h>
#include <wx/wx.h>

#include "shader.hpp"
#include "Antenna.hpp"
#include "plot3D.hpp"
#include "PlotScene.hpp"

class OGLPane: public wxGLCanvas {
public:
    OGLPane(wxWindow* parent, const wxGLAttributes& canvasAttrs, wxGLContext* sharedContext);
    OGLPane(wxWindow* parent, const wxGLAttributes& canvasAttrs, wxGLContext* sharedContext, bool dummy);
    
    void resized(wxSizeEvent& event);
    
    wxGLContext* getContext(){return context.get();}
    int getWidth();
    int getHeight();

    void render(wxPaintEvent& event);
    void prepare3DViewport(int topleft_x, int topleft_y, int bottomright_x, int bottonright_y);
    void prepare2DViewport(int topleft_x, int topleft_y, int bottomright_x, int bottonright_y);

    void mouseMoved(wxMouseEvent& event);
    void mouseDown(wxMouseEvent& event);
    void mouseWheelMoved(wxMouseEvent& event);
    void mouseReleased(wxMouseEvent& event);
    void rightClick(wxMouseEvent& event);
    void mouseLeftWindow(wxMouseEvent& event);
    void keyPressed(wxKeyEvent& event);
    void keyReleased(wxKeyEvent& event);

    void plot(const Surface3D& surface, 
              ColormapFunc colormapFunc, 
              CoordsColoringRule coloringRule);
    
    int id;
//private:
    bool isOpenGLInitialized{ false };
    std::unique_ptr<wxGLContext> context;
    std::unique_ptr<PlotScene> plotScene;

    void setUpContext( wxGLContext* sharedContext);
    void loadExtensions();
    void setUpOpenGL();

    DECLARE_EVENT_TABLE();

};


#endif