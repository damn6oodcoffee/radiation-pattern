#include <chrono>
#include <stdexcept>
#include <numbers>

#include "OGLPane.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader.hpp"
#include "camera.hpp"
#include "surface.hpp"


// const unsigned int SCR_WIDTH = 800;
// const unsigned int SCR_HEIGHT = 600;

// Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// float lastX = SCR_WIDTH / 2.0f;
// float lastY = SCR_HEIGHT / 2.0f;
// bool firstMouse = true;

// float deltaTime = 0.0f;
// float lastFrame = 0.0f;



BEGIN_EVENT_TABLE(OGLPane, wxGLCanvas)
EVT_MOTION(OGLPane::mouseMoved)
EVT_LEFT_DOWN(OGLPane::mouseDown)
EVT_LEFT_UP(OGLPane::mouseReleased)
EVT_RIGHT_DOWN(OGLPane::rightClick)
EVT_LEAVE_WINDOW(OGLPane::mouseLeftWindow)
EVT_SIZE(OGLPane::resized)
EVT_KEY_DOWN(OGLPane::keyPressed)
EVT_KEY_UP(OGLPane::keyReleased)
EVT_MOUSEWHEEL(OGLPane::mouseWheelMoved)
EVT_PAINT(OGLPane::render)
END_EVENT_TABLE()

void OGLPane::mouseMoved(wxMouseEvent& event) {}
void OGLPane::mouseDown(wxMouseEvent& event) {}
void OGLPane::mouseWheelMoved(wxMouseEvent& event) {}
void OGLPane::mouseReleased(wxMouseEvent& event) {}
void OGLPane::rightClick(wxMouseEvent& event) {}
void OGLPane::mouseLeftWindow(wxMouseEvent& event) {}
void OGLPane::keyPressed(wxKeyEvent& event) {}
void OGLPane::keyReleased(wxKeyEvent& event) {}

OGLPane::OGLPane(wxWindow* parent, const wxGLAttributes &canvasAttrs,  wxGLContext* sharedContext)
    : wxGLCanvas(parent, canvasAttrs)
{
    // To avoid flashing on MSW
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    setUpContext(sharedContext);
    loadExtensions();
    setUpOpenGL();
    
    // Set up the scene
    plotScene = std::make_unique<PlotScene>();
    isOpenGLInitialized = true;
}

OGLPane::OGLPane(wxWindow* parent, const wxGLAttributes& canvasAttrs, wxGLContext* sharedContext, bool dummy)
    : wxGLCanvas(parent, canvasAttrs)
{
    // To avoid flashing on MSW
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    wxGLContextAttrs ctxAttrs;
    ctxAttrs.PlatformDefaults().CoreProfile().OGLVersion(3, 3).EndList();
    context = std::unique_ptr<wxGLContext>(new wxGLContext(this, nullptr, &ctxAttrs));
    plotScene = nullptr;
    //plotScene = std::make_unique<PlotScene>();
    isOpenGLInitialized = true;
}

void OGLPane::setUpContext(wxGLContext* sharedContext) {
    wxGLContextAttrs ctxAttrs;
    ctxAttrs.PlatformDefaults().CoreProfile().OGLVersion(3, 3).EndList();
    context = std::make_unique<wxGLContext>(this, sharedContext, &ctxAttrs);
    if (!context){
        wxLogError("failed to set up context");
        throw std::runtime_error("context set up failed");
    }
}

void OGLPane::loadExtensions() {
    SetCurrent(*context);
     // Load OpenGL extensions.
    int version = gladLoadGL();
    if (version == 0) {
        wxLogError("failed to load glad");
        throw std::runtime_error("failed to load glad");
    }
    wxLogDebug("Loaded OpenGL.");
}

void OGLPane::setUpOpenGL() {
    SetCurrent(*context);
    /* OpenGL settings */
    glEnable(GL_DEPTH_TEST);
    // For text rendering:
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OGLPane::resized(wxSizeEvent& event) {
    // if (!plotScene) {
    //     event.Skip();
    //     return;
    // }

    bool firstApperance = IsShownOnScreen() && !isOpenGLInitialized;
    if (firstApperance) {
        return;
    }
    
    if (isOpenGLInitialized) {
        SetCurrent(*context);
        auto size = event.GetSize();
        auto viewPortSize = size * GetContentScaleFactor();
        glViewport(0, 0, viewPortSize.x, viewPortSize.y);
        if (plotScene && size.GetWidth() && size.GetHeight())
            plotScene->setPerspective(
                glm::radians(45.0f), 
                static_cast<float>(size.GetWidth()) / static_cast<float>(size.GetHeight()),
                0.1f, 
                100.0f
            );
    }
    event.Skip();
    //Refresh();
}

void OGLPane::render(wxPaintEvent& event) {
 
   
    wxPaintDC dc(this);

    if (!isOpenGLInitialized) {
        return;
    }
    SetCurrent(*context);

    // double currentFrame{ GetTimeAsDouble() };
    // deltaTime = currentFrame - lastFrame;
    // lastFrame = currentFrame;

    if (id == 999) {
        static int i = 0;
        if (i < 50) {
            glClearColor(0.2f, 0.3, 0.3f, 1.0f);
        } else {
            glClearColor(0.2f, 0.0f, 0.6f, 1.0f);
        }
        i = (i + 1) % 100;
    } else {
        glClearColor(0.2f, 0.3, 0.3f, 1.0f);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (plotScene) 
        plotScene->render();
    
    SwapBuffers();
}

void OGLPane::plot(const Surface3D& surface, 
                   ColormapFunc colormapFunc, 
                   CoordsColoringRule coloringRule)
{
    SetCurrent(*context);
    if (plotScene)
        plotScene->plot(surface, colormapFunc, coloringRule);
}