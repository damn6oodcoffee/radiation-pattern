
#include "ControlPanel.hpp"
#include <numbers>

ControlPanel::ControlPanel(wxWindow *parent, wxWindowID winid,
                           const wxPoint &pos, const wxSize &size)
    : wxPanel(parent, winid, pos, size)
    , azPointCount{256}
    , thPointCount{256}
    , antennaGrid(10, 2, 2, 100)
{
    antennaGrid.toggleCell(5,4);
    antennaGrid.toggleCell(5,5);
    antennaGrid.toggleCell(5,3);
    antennaGrid.toggleCell(5,6);
}

void ControlPanel::computeRadiationPattern() {
    auto radPattern = antennaGrid.computeRadiationPattern(azPointCount, 2 * thPointCount);
    std::vector<SphericalPoint> newUpperHemispherePattern{};
    std::copy_if(radPattern.begin(), radPattern.end(), std::back_inserter(newUpperHemispherePattern), [](auto&& elt){
        return elt.theta <= std::numbers::pi / 2;
    });
    upperHemispherePattern = std::move(newUpperHemispherePattern);
}

Surface3D ControlPanel::getPolarSurface() {
    surface.setVertices(getPolarVertices(), 
        { static_cast<unsigned int>(azPointCount),
          static_cast<unsigned int>(thPointCount) });
    return surface;
}

Surface3D ControlPanel::getUVSurface() {
    surface.setVertices(getUVVertices(), 
        { static_cast<unsigned int>(azPointCount),
          static_cast<unsigned int>(thPointCount) });
    return surface;
}

std::vector<double> ControlPanel::getPolarVertices() {
    std::vector<double> vertices{};
    for (auto&& sphPoint: upperHemispherePattern) {
        auto cartPoint{sphericalToCartesian(sphPoint)};
        vertices.insert(vertices.end(), {cartPoint.x, cartPoint.y, cartPoint.z});
    }
    return vertices;
}

std::vector<double> ControlPanel::getUVVertices() {
    std::vector<double> vertices{};
    for (auto&& sphPoint: upperHemispherePattern) {
        auto uvPoint{ sphericalToUV(sphPoint) };
        vertices.insert(vertices.end(), { uvPoint.u, uvPoint.v, uvPoint.z });
    }
    return vertices;
}

