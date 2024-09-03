
#include <stdexcept>
#include <numbers>
#include <cmath>
#include "Antenna.hpp"

AntennaGrid::AntennaGrid(size_t gridSize, double interval, 
                         double wavelength, double distance)
    : interval{ interval }
    , wavelength{ wavelength }
    , distance{ distance }
{
    setGridSize(gridSize);
}

void AntennaGrid::setGridSize(size_t newSize) {
    std::vector<std::vector<AntennaCell>> newCells(newSize);
    double originPosX{}, originPosY{};
    if (newSize % 2 == 0) 
        originPosX = originPosY = -(static_cast<long long>(newSize) / 2) * interval;
    else
        originPosX = originPosY = -(static_cast<long long>(newSize) / 2) * interval + 0.5*interval;

    for (size_t i{ 0 }; i < newSize; ++i) {
        newCells[i].reserve(newSize);
        for (size_t j{ 0 }; j < newSize; ++j) {
            newCells[i].emplace_back(
                originPosX + i*interval,
                originPosY + j*interval,
                false
            );
        }
    }
    gridSize = newSize;
    cells = std::move(newCells);
    markedCellIndices.clear();
}

size_t AntennaGrid::getGridSize() { return gridSize; }

void AntennaGrid::toggleCell(size_t row, size_t col) {
    if (row >= gridSize || col >= gridSize)
        throw  std::invalid_argument("out of grid size");

    auto it { markedCellIndices.find({ row, col }) };
    if (it != markedCellIndices.end()) {
        markedCellIndices.erase(it);
        cells[row][col].marked = false;
    } else {
        markedCellIndices.emplace(row, col);
        cells[row][col].marked = true;
    }
}

std::vector<SphericalPoint> AntennaGrid::computeRadiationPattern(size_t azimuthPointCount, size_t polarPointCount) {
    double azimuthStep{ 2.0 * std::numbers::pi / (azimuthPointCount - 1) };
    double polarStep{ std::numbers::pi / (polarPointCount - 1)};

    std::vector<SphericalPoint> vertices{};
    SphericalPoint targetPoint{ distance, 0.0, 0.0 };
    double azimuthBegin{ 0.0 };
    double thetaBegin{ 0.0 };
    for (size_t i{ 0 }; i < azimuthPointCount; ++i) {
        targetPoint.phi = azimuthBegin + i*azimuthStep;
        for (size_t j{ 0 }; j < polarPointCount; ++j) {
            targetPoint.theta = thetaBegin + j*polarStep;
            vertices.emplace_back(
                computePowerAtPoint(sphericalToCartesian(targetPoint)),
                targetPoint.theta,
                targetPoint.phi
            );
        }
    }
    return vertices;
}

double AntennaGrid::computePowerAtPoint(const CartesianPoint& point) {
    double wavenumber{ 2 * std::numbers::pi / wavelength };
    std::complex amplitude{ 0.0, 0.0 };
    for (auto&& [row, col]: markedCellIndices) {
        double cellToTargetDistance{std::sqrt(
            (point.x - cells[row][col].x)*(point.x - cells[row][col].x)
            + (point.y - cells[row][col].y)*(point.y - cells[row][col].y)
            + point.z*point.z
        )};
        amplitude += 1.0 / cellToTargetDistance
            * exp(std::complex{ 0.0, cellToTargetDistance * wavenumber});
    }
    return std::abs(amplitude)*std::abs(amplitude);
}
