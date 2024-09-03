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


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

double GetTimeAsDouble() {
    using namespace std::chrono;
    using SecondsFP = std::chrono::duration<double>;
    return duration_cast<SecondsFP>(high_resolution_clock::now().time_since_epoch()).count();
}

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

OGLPane::OGLPane(wxFrame* parent, const wxGLAttributes &canvasAttrs)
    : wxGLCanvas(parent, canvasAttrs)
{

    // To avoid flasing on MSW
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    // Setting up context
    wxGLContextAttrs ctxAttrs;
    ctxAttrs.PlatformDefaults().CoreProfile().OGLVersion(3, 3).EndList();
    context = std::make_unique<wxGLContext>(this, nullptr, &ctxAttrs);
    if (!context){
        wxLogError("failed to set up context");
        throw std::runtime_error("context set up failed");
    }
    SetCurrent(*context);

    // Load OpenGL extensions.
    int version = gladLoadGL();
    if (version == 0) {
        wxLogError("failed to load glad");
        throw std::runtime_error("failed to load glad");
    }
    wxLogDebug("Loaded OpenGL.");

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    // For text rendering:
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up shaders
    surfaceShader = std::make_unique<Shader>("shaders/surface.vert", "shaders/surface.frag");
    axisShader = std::make_unique<Shader>("shaders/axis.vert", "shaders/axis.frag");
    textShader = std::make_unique<Shader>("shaders/text.vert", "shaders/text.frag");
    gridShader = std::make_unique<Shader>("shaders/grid.vert", "shaders/grid.frag");
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    surfaceShader->use();
    surfaceShader->setMat4("projection", glm::value_ptr(projection));
    axisShader->use();
    axisShader->setMat4("projection", glm::value_ptr(projection));
    textShader->use();
    textShader->setMat4("projection", glm::value_ptr(projection));
    gridShader->use();
    gridShader->setMat4("projection", glm::value_ptr(projection));

    
    antennaGrid = std::make_unique<AntennaGrid>(10, 2, 2, 100);
    antennaGrid->toggleCell(5,4);
    antennaGrid->toggleCell(5,5);
    antennaGrid->toggleCell(5,3);
    antennaGrid->toggleCell(5,6);

    size_t azPointCount{ 512 }, thPointCount{ 512 };
    auto radPattern = antennaGrid->computeRadiationPattern(azPointCount, thPointCount);
    std::vector<SphericalPoint> upperHemispherePattern{};
#if 1
    std::copy_if(radPattern.begin(), radPattern.end(), std::back_inserter(upperHemispherePattern), [](auto&& elt){
        return elt.theta <= std::numbers::pi / 2;
    });
    radPattern = upperHemispherePattern;
    thPointCount = 256;
#endif
    std::vector<double> vertices{};
    CoordsColoringRule coloringRule{};
    for (auto&& sphPoint: radPattern) {
#if 0
        auto cartPoint{ sphericalToCartesian(sphPoint)};
        vertices.insert(vertices.end(), { cartPoint.x, cartPoint.y, cartPoint.z });
        coloringRule = [](double x, double y, double z) {
            return std::sqrt(x*x + y*y + z*z);
        };
#else
        auto uvPoint{ sphericalToUV(sphPoint) };
        vertices.insert(vertices.end(), { uvPoint.u, uvPoint.v, uvPoint.z });
        coloringRule = defaultColoringRule;
#endif
    }

    Surface3D surface;
    surface.computeVertices(sinc3D, { -1.0, 2.5, -1.3, 6.3 }, { 512, 512 });
    surface.setVertices(vertices, { static_cast<unsigned>(azPointCount), static_cast<unsigned>(thPointCount) });
    
    plot3d = std::make_unique<Plot3D>("font/IBMPlexSans-Regular.ttf");
    plot3d->plot(surface, turboColormap, coloringRule);

    isOpenGLInitialized = true;
}

void OGLPane::resized(wxSizeEvent& event) {
    bool firstApperance = IsShownOnScreen() && !isOpenGLInitialized;

    if (firstApperance)    {
        return;
    }

    if (isOpenGLInitialized)
    {
        auto viewPortSize = event.GetSize() * GetContentScaleFactor();
        glViewport(0, 0, viewPortSize.x, viewPortSize.y);
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

    double currentFrame{ GetTimeAsDouble() };
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClearColor(0.2f, 0.3, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera.GetViewMatrix();

    textShader->use();
    textShader->setMat4("view", glm::value_ptr(view));
    textShader->setVec3("textColor", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

    gridShader->use();
    gridShader->setMat4("view", glm::value_ptr(view));

    surfaceShader->use();
    surfaceShader->setMat4("view", glm::value_ptr(view));
    plot3d->render(*gridShader, *textShader, *surfaceShader);

    SwapBuffers();
}