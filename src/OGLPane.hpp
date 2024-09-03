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

class OGLPane: public wxGLCanvas {
public:
    OGLPane(wxFrame* parent, const wxGLAttributes& canvasAttrs);
    
    void resized(wxSizeEvent& event);
    
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

    DECLARE_EVENT_TABLE();

private:
    bool isOpenGLInitialized{ false };
    std::unique_ptr<wxGLContext> context;

// TODO: refactor into a separate class:
    std::unique_ptr<Shader> surfaceShader;
    std::unique_ptr<Shader> axisShader;
    std::unique_ptr<Shader> textShader;
    std::unique_ptr<Shader> gridShader;
    std::unique_ptr<AntennaGrid> antennaGrid;
    std::unique_ptr<Plot3D> plot3d;
};


#endif