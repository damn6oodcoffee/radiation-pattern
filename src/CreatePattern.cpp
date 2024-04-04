#include "CreatePattern.h"
#include <algorithm>
#include "Angle.h"

using namespace std;
std::vector<std::pair<double, double>> GetSourceCoords(AntennasGrid* grid, double d) {

	vector <pair<double, double>> coords;
	double xShift = ((grid->antennaCells.size()-1) * d) / 2;
	double yShift = ((grid->antennaCells.size()-1) * d) / 2;

	for (int i = 0; i < grid->antennaCells.size(); i++) {
		for (int j = 0; j < grid->antennaCells[0].size(); j++) {
			if (grid->antennaCells[i][j]->IsMarked()) {
				coords.push_back({ i * d - xShift, j * d - yShift});
			}
		}
	}


	return coords;

}

vector<Vertex3> GetPatternSurface(AntennasGrid* grid, int azPts, int elPts, double r, double lambda, double d) {

	vector <pair<double, double>> coords;
	coords = GetSourceCoords(grid, d);

	vector<Vertex3> vertices;
	if (azPts <= 0 || elPts <= 0) return vertices;

	int azimuthPts = azPts;
	int elevationPts = elPts;

	double deltaPhi = 2*M_PI / azimuthPts;	// Phi = [-pi, pi]
	double deltaTheta = M_PI/ 2 / elevationPts; // Theta = [0, pi/2]

	double thetaMin = -0*M_PI / 2;
	double phiMin = -M_PI;

	double theta = thetaMin;
	double phi = phiMin;

	double k = 2 * M_PI / lambda;
	double ri = 0;
	double r_x = 0;
	double r_y = 0;
	double r_z = 0;
	double delta_rxy;
	complex<double> power = 0;

	for (int i = 0; i < azimuthPts; i++) {	// Azimuth(Phi)
		for (int j = 0; j < elevationPts; j++) {	// Elevation(Theta)

			power = 0;

			phi = phiMin + i * deltaPhi;
			theta = thetaMin + j * deltaTheta;

			r_x = r * sin(theta) * cos(phi);
			r_y = r * sin(theta) * sin(phi);
			r_z = r * cos(theta);

			for (auto& src : coords) {

				delta_rxy = (r_x - src.first) * (r_x - src.first) + (r_y - src.second) * (r_y - src.second);
				ri = sqrt(r_z * r_z + delta_rxy);

				power += 1 / ri * exp(complex<double>(0, ri * k));
				
			}

			//vertices.push_back(Vertex3(ToDegree(phi), ToDegree(theta), sqrt(power.real() * power.real() + power.imag() * power.imag())));
			vertices.push_back(Vertex3(phi,theta, sqrt(power.real() * power.real() + power.imag() * power.imag())));
			//vertices.push_back(Vertex3(sin(theta) * cos(phi), sin(theta) * sin(phi), sqrt(power.real() * power.real() + power.imag() * power.imag())));

		}
	}


	sort(vertices.begin(), vertices.end(), [] (const Vertex3& a,const Vertex3& b) {
			
			if (a.y < b.y) return true;
			if (b.y < a.y) return false;

			// a=b for primary condition, go to secondary
			if (a.x < b.x) return true;
			if (b.x < a.x) return false;
		});

	//for (int i = 0; i < elevationPts; i++) {
	//	sort(vertices.begin() + i * azimuthPts, vertices.begin() + (i+1) * azimuthPts, [](Vertex3& a, Vertex3& b) {
	//		return ((a.y <= b.y)) && (a.x < b.x);
	//		});
	//}
	

	double max = abs(vertices[0].z);
	for (auto& v : vertices) {
		if (abs(v.z) > max) max = abs(v.z);
		phi = v.x;
		theta = v.y;
		v.x = sin(theta) * cos(phi);
		v.y = sin(theta) * sin(phi);
	}

	for (auto& v : vertices) {
		v.z /= max;
	}
	return vertices;

}


std::vector<Vertex3> GetPatternSurfaceCartesian(AntennasGrid* grid, int xpts, int ypts, double r, double lambda, double d) {
	vector <pair<double, double>> coords;
	coords = GetSourceCoords(grid, d);

	vector<Vertex3> vertices;
	if (xpts <= 0 || ypts <= 0) return vertices;

	double dx = 2*r / xpts;
	double dy = 2*r / ypts;

	double xMin = -r;
	double xMax = r;
	double yMin = -r;
	double yMax = r;

	double rz = 0;
	double rx = 0;
	double ry = 0;
	double k = 2 * M_PI / lambda;
	complex<double> power = 0;
	double ri = 0;
	for (int i = 0; i <= xpts; i++) {
		for (int j = 0; j <= ypts; j++) {
			rx = xMin + i * dx;
			ry = yMin + j * dy;
			rz = sqrt(abs(r * r - rx * rx - ry * ry));

			power = 0;

			for (auto& src : coords) {
				ri = sqrt((rx - src.first) * (rx - src.first) + (ry - src.second) * (ry - src.second) + rz * rz);
				power += 1 / ri * exp(complex<double>(0, ri * k));
			}

			vertices.push_back(Vertex3(rx, ry, sqrt(power.real() * power.real() + power.imag() * power.imag())));

		}
	}

	return vertices;
}

Vertex3::Vertex3(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}