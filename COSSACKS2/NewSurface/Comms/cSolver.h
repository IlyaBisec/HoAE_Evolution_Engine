#ifndef __cSolver__
#define __cSolver__

#include "cUtils.h"
#include "cVectors.h"

namespace comms {

class cSolver {
public:
	static int SolveQuadraticEquation(doubles &x, double a, double b, double c);
	static int SolveCubicEquation(doubles &x, double a, double b, double c);

	static bool AggregateOfPointsApproximation(const cVectors &AggregateOfPoints,
		cVector &Centre, cVector &Normal, double Epsilon = 1.);
private:
	static bool AggregateOfPointsApproximation2D(const cVectors &AggregateOfPoints2D,
		cVector &Centre, cVector &Normal, double Epsilon);
	static bool AggregateOfPointsApproximation3D(const cVectors &AggregateOfPoints3D,
		cVector &Centre, cVector &Normal, double Epsilon);
};

} // comms

#endif // __cSolver__