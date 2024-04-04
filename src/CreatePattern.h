#pragma once
#include <vector>
#include "AntennasGrid.h"
#include <complex>

struct Vertex3 {
	double x;	// Azimuth
	double y;	// Theta
	double z;	// Power
	Vertex3(double x, double y, double z);
};

std::vector<std::pair<double, double>> GetSourceCoords(AntennasGrid* grid, double d);

std::vector<Vertex3> GetPatternSurface(AntennasGrid* grid, int azPts, int elPts , double r, double lambda, double d);
std::vector<Vertex3> GetPatternSurfaceCartesian(AntennasGrid* grid, int xpts, int ypts, double r, double lambda, double d);