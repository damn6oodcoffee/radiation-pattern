#ifndef ANTENNA_GRID_HPP
#define ANTENNA_GRID_HPP

#include <vector> 
#include <unordered_set>
#include <complex>
#include "CoordinateSystem.hpp"
//double a = std::numbers::pi;

template<typename T>
void hashCombine(std::size_t& seed, const T& v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

namespace std {
template<typename T, typename U>
struct hash<std::pair<T, U>> {
    std::size_t operator()(const std::pair<T, U>& p) const {
        std::size_t seed{ 0 };
        hashCombine(seed, p.first);
        hashCombine(seed, p.second);
        return seed;
    }
};
}


struct AntennaCell {
    double x;
    double y;
    bool marked;
};

class AntennaGrid {
public:
    AntennaGrid(size_t gridSize, double interval, double wavelength, double distance);
    void setGridSize(size_t newSize);
    size_t getGridSize();
    void toggleCell(size_t row, size_t col);
    double computePowerAtPoint(const CartesianPoint& point);
    std::vector<SphericalPoint> computeRadiationPattern(size_t azimuthPointCount, size_t polarPointCount);
    
    double interval;
    double wavelength;
    double distance;

private:
    size_t gridSize;
    std::vector<std::vector<AntennaCell>> cells;
    std::unordered_set<std::pair<size_t, size_t>> markedCellIndices;
};


#endif 