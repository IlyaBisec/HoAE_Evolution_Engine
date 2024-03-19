#include "cSolver.h"

namespace comms {

// cSolver::SolveQuadraticEquation
// a * x ^ 2 + b * x + c = 0
// Returns:
// -3 - infinite number of roots (a == b == c == 0)
// -2 - no roots (a == b == 0)
// -1 - one real root (a == 0)
//  0 - two complex roots
//  1 - one real root (d == 0)
//  2 - two real roots
int cSolver::SolveQuadraticEquation(doubles &x, double a, double b, double c) {
	x.Free();
	if(cUtils::IsZero(a)) {
		if(cUtils::IsZero(b)) {
			if(cUtils::IsZero(c)) return -3;
			return -2;
		}
		x.Add(- c / b);
		return -1;
	}
	double d = Square(b) - 4. * a * c;
	if(cUtils::IsZero(d)) { // one real root:
		x.Add(- b / (2. * a));
		return 1;
	}
	if(d < 0.) { // conjugate complex roots:
		double t = 0.5 / a;
		x.Add(- b * t);
		x.Add(sqrt(-d) * t);
		return 0;
	}
	// two real roots:
	if(b >= 0.) d = (- 0.5) * (b + sqrt(d));
	else d = (- 0.5) * (b - sqrt(d));
	x.Add(d / a);
	x.Add(c / d);
	return 2;
} // cSolver::SolveQuadraticEquation

// cSolver::SolveCubicEquation
// x ^ 3 + a * x ^ 2 + b * x + c = 0
// Returns:
// 3 - 3 real roots
// 2 - 1 real root + 1 complex (imaginary part is zero, i.e. 2 real roots)
// 1 - 1 real roots + 2 complex
int cSolver::SolveCubicEquation(doubles &x, double a, double b, double c) {
	x.Free();
	double q = (Square(a) - 3. * b) / 9.;
	double r = (a * (2. * Square(a) - 9. * b) + 27. * c) / 54.;
	double r2 = Square(r);
	double q3 = Cube(q);
	if(r2 <= q3) {
		double t = acos(r / sqrt(q3));
		a /= 3., q = - 2. * sqrt(q);
		x.Add(q * cos(t / 3.) - a);
		x.Add(q * cos((t + c2Pi) / 3.) - a);
		x.Add(q * cos((t - c2Pi) / 3.) - a);
		return 3;
	} else {
		if(r < 0.) r = - r;
		double aa = - pow(r + sqrt(r2 - q3), 1. / 3.);
		double bb = 0.;
		if(cUtils::NotZero(aa)) bb = q / aa;
		a /= 3., q = aa + bb, r = aa - bb;
		x.Add(q - a);
		x.Add((- 0.5) * q - a);
		double Im = (sqrt(3.) * 0.5) * fabs(r);
		if(cUtils::IsZero(Im)) return 2;
		x.Add(Im);
		return 1;
	}
} // cSolver::SolveCubicEquation

// cSolver::AggregateOfPointsApproximation
bool cSolver::AggregateOfPointsApproximation(const cVectors &AggregateOfPoints,
							cVector &Centre, cVector &Normal, double Epsilon) {
	Centre.LoadZero();
	Normal.LoadZero();
	if(!AggregateOfPoints.Count()) return false;
	int nDim = AggregateOfPoints.Dim();
	assert(nDim == 2 || nDim == 3);
	if(nDim == 2) {
		return AggregateOfPointsApproximation2D(AggregateOfPoints, Centre, Normal, Epsilon);
	} else if(nDim == 3) {
		return AggregateOfPointsApproximation3D(AggregateOfPoints, Centre, Normal, Epsilon);
	}
	return false;
} // cSolver::AggregateOfPointsApproximation

// cSolver::AggregateOfPointsApproximation2D
bool cSolver::AggregateOfPointsApproximation2D(const cVectors &AggregateOfPoints2D,
								cVector &Centre, cVector &Normal, double Epsilon) {
	Centre = cVector::Origin2D;
	Normal = cVector::Origin2D;

	if(AggregateOfPoints2D.Count() < 2) return false;

	// Centre:
	Centre = AggregateOfPoints2D.Centre();
	// BBox:
	cVector bbMin, bbMax;
	AggregateOfPoints2D.BBox(bbMin, bbMax);
	cVector b = bbMax - bbMin;
	if(cUtils::IsZero(b.x, Epsilon)) { // Along yAxis:
		Normal = cVector::xAxis2D;
		return true;
	} else if(cUtils::IsZero(b.y, Epsilon)) { // Along xAxis:
		Normal = cVector::yAxis2D;
		return true;
	}
	// M:
	cMatrix M(Centre, cVector(0.0, 0.0));
	M = AggregateOfPoints2D.Count() * M.Transposed() * M;
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 2; j++) {
			double s = 0.;
			for(int k = 0; k < AggregateOfPoints2D.Count(); k++)
				s += AggregateOfPoints2D[k][i] * AggregateOfPoints2D[k][j];
			M(i, j) -= s;
		}
	assert(M.IsSymmetric());
	// Eigen values:
	doubles EigenValues;
	cVectors EigenVectors;
	if(M.EigenSolve(EigenValues, EigenVectors)) {
		Normal = EigenVectors[MaxIndex(EigenValues[0], EigenValues[1])];
		return true;
	}
	return false;
} // cSolver::AggregateOfPointsApproximation2D

// cSolver::AggregateOfPointsApproximation3D
bool cSolver::AggregateOfPointsApproximation3D(const cVectors &AggregateOfPoints3D,
							   cVector &Centre, cVector &Normal, double Epsilon) {
	Centre = cVector::Origin;
	Normal = cVector::Origin;

	if(AggregateOfPoints3D.Count() < 3) return false;

	// Centre:
	Centre = AggregateOfPoints3D.Centre();
	// BBox:
	cVector bbMin, bbMax;
	AggregateOfPoints3D.BBox(bbMin, bbMax);
	cVector b = bbMax - bbMin;
	if(cUtils::IsZero(b.x, Epsilon)) { // Plane YZ:
		Normal = cVector::xAxis;
		return true;
	} else if(cUtils::IsZero(b.y, Epsilon)) { // Plane XZ:
		Normal = cVector::yAxis;
		return true;
	} else if(cUtils::IsZero(b.z, Epsilon)) { // Plane XY:
		Normal = cVector::zAxis;
		return true;
	}
	// M:
	cMatrix M(Centre, cVector::Origin, cVector::Origin);
	M = AggregateOfPoints3D.Count() * M.Transposed() * M;
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++) {
			double s = 0.;
			for(int k = 0; k < AggregateOfPoints3D.Count(); k++)
				s += AggregateOfPoints3D[k][i] * AggregateOfPoints3D[k][j];
			M(i, j) -= s;
		}
	assert(M.IsSymmetric(Epsilon));
	// Eigen values:
	doubles EigenValues;
	cVectors EigenVectors;
	if(M.EigenSolve(EigenValues, EigenVectors)) {
		Normal = EigenVectors[MaxIndex(EigenValues[0], EigenValues[1], EigenValues[2])];
		return true;
	}
	return false;
} // cSolver::AggregateOfPointsApproximation3D

} // comms