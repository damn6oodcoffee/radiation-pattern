#ifndef SURFACE_HPP
#define SURFACE_HPP

#include <vector>
#include <functional>
#include <algorithm>
#include <unordered_set>
#include "colormaps.hpp"
#include "CoordinateSystem.hpp"

struct SurfaceRange {
    double xBegin, xEnd, yBegin, yEnd;
};

struct RangeBox {
    double xBegin, xEnd;
    double yBegin, yEnd;
    double zBegin, zEnd;
    double xExtent() const{ return xEnd - xBegin; }
    double yExtent() const { return yEnd - yBegin; }
    double zExtent() const { return zEnd - zBegin; }
};

inline double sinc3D(double x, double y) {
    if (x == 0 && y == 0) return 1;
    double rad { sqrt(x*x + y*y) };
    return sin(4.0*rad) / rad;
}

using SurfaceFunc = std::function<double(double, double)>;
using CoordsColoringRule = std::function<double(double, double, double)>;

inline double defaultColoringRule(double x, double y, double z) {
    return z;
}

class Surface3D {
public:
    Surface3D()
        : vertices{}
        , maxVal{}
        , minVal{}
        , rangeBox{}
        , gridDim{}
    {}

    struct GridDimensions {
        unsigned int verticalLineCount; 
        unsigned int horizontalLineCount; 
    };
    
    void computeVertices(SurfaceFunc surfaceFunc, const SurfaceRange& range, GridDimensions dim) {
        double dx { (range.xEnd - range.xBegin) / (dim.verticalLineCount - 1) };
        double dy { (range.yEnd - range.yBegin) / (dim.horizontalLineCount - 1) };
        double x{}, y{}, z{};
        std::vector<double> newVertices;
        minVal = maxVal = surfaceFunc(range.xBegin, range.yBegin);
        for (int i { 0 }; i < dim.verticalLineCount; ++i) {
            x = i*dx + range.xBegin;
            for (int j { 0 }; j < dim.horizontalLineCount; ++j) {
                y = j*dy + range.yBegin;
                z = surfaceFunc(x, y);
                newVertices.insert(newVertices.end(), {x, y, z});
            }
        }
        gridDim = dim;
        vertices = std::move(newVertices);
        defineRangeBox();
    }

    void setVertices(const std::vector<double>& newVertices, GridDimensions dim) {
        gridDim = dim;
        vertices = newVertices;
        defineRangeBox();
    }

    // void setPolarVertices(std::vector<SphericalPoint> points) {
    //     std::sort(points.begin(), points.end(), [](const auto& a, const auto& b){
    //         if (a.phi < b.phi)
    //             return true;
    //         else if (a.phi > b.phi)
    //             return false;
    //         if (a.theta < b.theta)
    //             return true;
    //         return false;
    //     });
    //     std::vector<double> newVertices{};
    //     for (auto&& point: points) {
    //         CartesianPoint pt{ sphericalToCartesian(point) };
    //         newVertices.insert(vertices.end(), { pt.x, pt.y, pt.z });
    //     }
    //     vertices = std::move(newVertices);
    //     defineRangeBox();
    // }

    // void setVertices(std::vector<CartesianPoint> points) {
    //     std::sort(points.begin(), points.end(), [](const auto& a, const auto& b){
    //         if (a.x < b.x)
    //             return true;
    //         else if (a.x > b.x)
    //             return false;
    //         if (a.y < b.y)
    //             return true;
    //         return false;
    //     });
    //     std::vector<double> newVertices{};
    //     for (auto&& point: points)
    //         newVertices.insert(vertices.end(), { point.x, point.y, point.z });
    //     vertices = std::move(newVertices);
    //     defineRangeBox();
    // }

    std::vector<double>::const_iterator begin() const { return vertices.cbegin(); }
    std::vector<double>::const_iterator end() const  { return vertices.cend(); }
    const std::vector<double>& getVertices() const { return vertices; }
    const RangeBox& getRangeBox() const { return rangeBox; }
    GridDimensions getGridDimensions() const { return gridDim; }

private:
    std::vector<double> vertices;
    double maxVal;
    double minVal;
    RangeBox rangeBox;
    GridDimensions gridDim;

    void defineRangeBox() {
        /* c++23:
            auto it = std::ranges::[min|max]_element(
                std::ranges::subrange(vec.begin()+0, vec.end()) | std::ranges::views::stride(3));
            it = std::ranges::[min|max]_element(
                std::ranges::subrange(vec.begin()+1, vec.end()) | std::ranges::views::stride(3));
            ...
        */
        if (vertices.empty()) {
            rangeBox = {};
            return;
        }
        double minVal{ vertices[0] }, maxVal{ vertices[0] };
        rangeBox.xBegin = vertices[0];
        rangeBox.xEnd = vertices[0];
        for (size_t i{ 0 }, size{ vertices.size() }; i < size; i += 3) {
            rangeBox.xBegin = std::min(rangeBox.xBegin, vertices[i]);
            rangeBox.xEnd = std::max(rangeBox.xEnd, vertices[i]);
        }
        rangeBox.yBegin = vertices[1];
        rangeBox.yEnd = vertices[1];
        for (size_t i{ 1 }, size{ vertices.size() }; i < size; i += 3) {
            rangeBox.yBegin = std::min(rangeBox.yBegin, vertices[i]);
            rangeBox.yEnd = std::max(rangeBox.yEnd, vertices[i]);
        }
        rangeBox.zBegin = vertices[2];
        rangeBox.zEnd = vertices[2];
        for (size_t i{ 2 }, size{ vertices.size() }; i < size; i += 3) {
            rangeBox.zBegin = std::min(rangeBox.zBegin, vertices[i]);
            rangeBox.zEnd = std::max(rangeBox.zEnd, vertices[i]);
        }
    }

};

/*
class VAO {
public:
    VAO() {
        glGenVertexArrays(1, &vao);

    }

    ~VAO() {
        glDeleteVertexArrays(1, &vao);
    }
private:
    GLuint vao;
};

class VBO {
public:
    VBO() {
        glGenBuffers(1, &vbo);
    }

    ~VBO() {
        glDeleteBuffers(1, &vbo);
    }
private:
    GLuint vbo;
};

class EBO {
public:
    EBO() {
        glGenBuffers(1, &ebo);
    }

    ~EBO() {
        glDeleteBuffers(1, &ebo);
    }
private:
    GLuint ebo;
};
*/


class SurfaceRenderer {
public:
    SurfaceRenderer()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
    }

    ~SurfaceRenderer() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void plot(const Surface3D& surface, ColormapFunc colormapFunc = viridisColormap, CoordsColoringRule coloringRule = defaultColoringRule) {
        setUpVertices(surface, colormapFunc, coloringRule);
    }

    void render() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    
    void bind() {
        int vertSize = static_cast<int>(posAndColorVertices.size());
        int indicesSize = static_cast<int>(indices.size());
        glBindVertexArray(VAO); 
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertSize, &posAndColorVertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesSize, &indices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);   
        glBindVertexArray(0); 
    }

private:
    unsigned int VAO, VBO, EBO;
    std::vector<float> posAndColorVertices;
    std::vector<unsigned int> indices;


    void setUpPosAndColor(const Surface3D& surface, ColormapFunc colormapFunc, CoordsColoringRule coloringRule) {
        // double normalizingConstant{ surface.getRangeBox().zExtent() };
        // for (auto it { surface.begin() }, itEnd { surface.end() }; it != itEnd; it += 3) {
        //     newVertices.insert(newVertices.end(), it, it+3);
        //     double normalizedValue { *(it+2) / normalizingConstant };
        //     RGB color { colormapFunc(normalizedValue) };
        //     newVertices.insert(newVertices.end(), { color.r, color.g, color.b });
        // }
        // posAndColorVertices = std::move(newVertices);

        std::vector<float> newVertices{};
        std::vector<float> coordsMap{};
        auto& vertices{ surface.getVertices() };
        for (size_t i{ 0 }, size{ vertices.size() }; i < size; i += 3) {
            newVertices.insert(newVertices.end(), { 
                static_cast<float>(vertices[i]),
                static_cast<float>(vertices[i + 1]), 
                static_cast<float>(vertices[i + 2]) 
            });
            newVertices.insert(newVertices.end(), { 0.0f, 0.0f, 0.0f, });
            coordsMap.emplace_back(coloringRule(vertices[i], vertices[i + 1], vertices[i + 2]));
        }
        auto [minIt, maxIt] = std::minmax_element(coordsMap.begin(), coordsMap.end());
        float extent{ (*maxIt) - (*minIt) };
    
        for (size_t i{ 0 }, size{ coordsMap.size() }; i < size; ++i) {
            double colormapIndex{};
            if (extent == 0)
                colormapIndex = 1.0;
            else
                colormapIndex = (coordsMap[i] - (*minIt)) / extent;
            Colormap::rgb color{ colormapFunc(colormapIndex) };
            newVertices[i*6 + 3] = color.r;
            newVertices[i*6 + 3 + 1] = color.g;
            newVertices[i*6 + 3 + 2] = color.b;
        }
        posAndColorVertices = std::move(newVertices);
    }

    void setUpIndices(const Surface3D& surface) {
        auto [verticalLineCount, horizontalLineCount] = surface.getGridDimensions();
        std::vector<unsigned int> newIndices{};
        for (unsigned int i { 0 }; i < verticalLineCount; ++i) {
            for (unsigned int j { 0 }; j < horizontalLineCount; ++j) {
                if (i == verticalLineCount - 1 || j == 0)
                    continue;
                newIndices.insert(newIndices.end(), 
                    { j + i*horizontalLineCount, j - 1 + i*horizontalLineCount, j - 1 + (i+1)*horizontalLineCount });
                newIndices.insert(newIndices.end(), 
                    { j + i*horizontalLineCount, j - 1 + (i+1)*horizontalLineCount ,j + (i+1)*horizontalLineCount });
            }
        }
        indices = std::move(newIndices); 
    }

    void setUpVertices(const Surface3D& surface, ColormapFunc colormapFunc, CoordsColoringRule coloringRule) {
        setUpPosAndColor(surface, colormapFunc, coloringRule);
        setUpIndices(surface);
        bind();
    }
};


class DrawableFig2D {
public:
    DrawableFig2D()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
    }

    ~DrawableFig2D() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void bind() {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0); 
        glBindVertexArray(0); 
    }

    void draw() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    unsigned int VAO, VBO, EBO;
    std::vector<float> vertices = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    std::vector<unsigned int> indices = {  // note that we start from 0!
        1, 0, 3,  // first Triangle
        2, 1, 3   // second Triangle
    };

};


#endif