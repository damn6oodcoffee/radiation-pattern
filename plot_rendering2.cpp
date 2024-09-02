
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <filesystem>
#include <numbers>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader.hpp"
#include "camera.hpp"
#include "surface.hpp"
#include "text_render.hpp"
#include "plot3D.hpp"
#include "Antenna.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 1024;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    std::cout << "Current path is " << std::filesystem::current_path() << '\n';
    std::filesystem::current_path("C:/Users/Vlad/Desktop/ProgEx/GIT/modern_opengl_test_text");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    //if (!gladLoadGL())
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);  
    // glFrontFace(GL_CCW);  
    // For text rendering:
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader figShader("shaders/fig.vert", "shaders/fig.frag");
    Shader surfaceShader("shaders/surface.vert", "shaders/surface.frag");
    Shader axisShader("shaders/axis.vert", "shaders/axis.frag");
    Shader textShader("shaders/text.vert", "shaders/text.frag");
    Shader gridShader("shaders/grid.vert", "shaders/grid.frag");
    //glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    figShader.use();
    figShader.setMat4("projection", glm::value_ptr(projection));
    surfaceShader.use();
    surfaceShader.setMat4("projection", glm::value_ptr(projection));
    axisShader.use();
    axisShader.setMat4("projection", glm::value_ptr(projection));
    textShader.use();
    textShader.setMat4("projection", glm::value_ptr(projection));
    gridShader.use();
    gridShader.setMat4("projection", glm::value_ptr(projection));

    size_t h;
    // VAO/VBO for axes
    // ----------------------------------------------------------------------
    unsigned int VAO1, VBO1;
    float axes_vertices[] = {
        // coords                 // color
          0.0f,   0.0f,   0.0f,   1.0f, 0.0f, 0.0f,   // z - red
          0.0f,   0.0f, 100.0f,   1.0f, 0.0f, 0.0f,
          0.0f,   0.0f,   0.0f,   0.0f, 0.0f, 1.0f,   // y - blue
          0.0f, 100.0f,   0.0f,   0.0f, 0.0f, 1.0f,
          0.0f,   0.0f,   0.0f,   0.0f, 1.0f, 0.0f,   // x = green
        100.0f,   0.0f,   0.0f,   0.0f, 1.0f, 0.0f,
    };
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axes_vertices), axes_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // ----------------------------------------------------------------------

    std::string fontName { "C:/Windows/Fonts/arial.ttf" };
    //  AxisLabelsRenderer axesRenderer(fontName, "x Axis Label", "y Axis Label", "z Axis Label");
    //  axesRenderer.bind();
    //  axesRenderer.setTickLabels(RangeBox{0.0, 1.0, 0.0, 1.0, 0.0, 1.0});

    PlotGridRenderer plotGridRenderer;
    plotGridRenderer.setUpGrid(6, 6, 6);
    plotGridRenderer.bind();

    AntennaGrid antennaGrid(10, 2, 2, 100);
    antennaGrid.toggleCell(5,4);
    antennaGrid.toggleCell(5,5);
    antennaGrid.toggleCell(5,3);
    antennaGrid.toggleCell(5,6);
    //antennaGrid.toggleCell(4,4);
    // antennaGrid.toggleCell(4,5);
    // antennaGrid.toggleCell(6,3);
    // antennaGrid.toggleCell(6,6);
    // antennaGrid.toggleCell(3,3);
    // antennaGrid.toggleCell(3,6);
    // for (int i {0} ; i < 10; i++ )
    //     for (int j {0} ; j < 10; j++ )
    //         antennaGrid.toggleCell(i, j);
    //antennaGrid.toggleCell(4,6);
    //antennaGrid.toggleCell(0,0);
    size_t azPointCount{512}, thPointCount{512};
    auto radPattern = antennaGrid.computeRadiationPattern(azPointCount, thPointCount);
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
        auto cartPoint{sphericalToCartesian(sphPoint)};
        vertices.insert(vertices.end(), {cartPoint.x, cartPoint.y, cartPoint.z});
        coloringRule = [](double x, double y, double z) {
            return std::sqrt(x*x + y*y + z*z);
        };
#else
        auto uvPoint{sphericalToUV(sphPoint)};
        vertices.insert(vertices.end(), {uvPoint.u, uvPoint.v, uvPoint.z});
        coloringRule = defaultColoringRule;
#endif
    }

    Surface3D surface;
    surface.computeVertices(sinc3D, {-1.0, 2.5, -1.3, 6.3}, {256, 256});
    surface.setVertices(vertices, { static_cast<unsigned>(azPointCount), static_cast<unsigned>(thPointCount) });
   
    Plot3D plot3d("font/IBMPlexSans-Regular.ttf");

    // Plot3d.plot(sinc3D, { -1.0, 2.5, -1.3, 6.3 });
    // Plot3d.setColormap(plasmaColormap);
    plot3d.plot(surface, turboColormap, coloringRule);
    

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        processInput(window);
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();

        axisShader.use();
        axisShader.setMat4("view", glm::value_ptr(view));
        glBindVertexArray(VAO1);
        glDrawArrays(GL_LINES, 0, 6);
        textShader.use();
        textShader.setMat4("view", glm::value_ptr(view));
        textShader.setVec3("textColor", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        //  axesRenderer.render(textShader);

        gridShader.use();
        gridShader.setMat4("view", glm::value_ptr(view));
        //plotGridRenderer.render(gridShader);

        surfaceShader.use();
        surfaceShader.setMat4("view", glm::value_ptr(view));
        plot3d.render(gridShader, textShader, surfaceShader);
#if 0
#endif

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}




void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
        camera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
        return;
    const auto default_precision{std::cout.precision()};
    
    std::cout << "POSIT(" << std::setprecision(1) << camera.Position[0] << ", " 
                          << std::setprecision(1) << camera.Position[1] << ", "
                          << std::setprecision(1) << camera.Position[2] << ")\t";
    std::cout << "FRONT(" << std::setprecision(1) << camera.Front[0] << ", " 
                          << std::setprecision(1) << camera.Front[1] << ", "
                          << std::setprecision(1) << camera.Front[2] << ")\n";
    std::cout << std::setprecision(default_precision);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}