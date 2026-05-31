#pragma once

class cSolver {
public:
	// GetRoots2: a * x ^ 2 + b * x + c = 0
	// Returns:
	// -3 - Infinite number of roots (a == b == c == 0)
	// -2 - No roots (a == b == 0)
	// -1 - One real root (a == 0)
	//  0 - Two conjugate complex roots
	//  1 - One real root (d == 0)
	//  2 - Two real roots
	static int GetRoots2(const float a, const float b, const float c, float *pRoots2);
	static bool GetLowestPositiveRoot2(const float a, const float b, const float c, const float MaxRoot, float &Root);
	
	// GetRoots3: x ^ 3 + a * x ^ 2 + b * x + c = 0
	// Returns:
	// 3 - Three real roots
	// 2 - One real root + one complex (imaginary part is zero, i.e. two real roots)
	// 1 - One real roots + two complex
	static int GetRoots3(const float a, const float b, const float c, float *pRoots3);

	static bool ApproximatePoints(const cVec2 *pPoints, const int nCount,
		cVec2 &Center, cVec2 &Normal, const float Eps = 1.0f);

	static bool ApproximatePoints(const cVec3 *pPoints, const int nCount,
		cVec3 &Center, cVec3 &Normal, const float Eps = 1.0f);

	static int SolveQuadraticEquation(doubles &x, double a, double b, double c);
	static int SolveCubicEquation(doubles &x, double a, double b, double c);

	static bool AggregateOfPointsApproximation(const dVectors &AggregateOfPoints,
		dVector &Centre, dVector &Normal, double Epsilon = 1.);
private:
	static bool AggregateOfPointsApproximation2D(const dVectors &AggregateOfPoints2D,
		dVector &Centre, dVector &Normal, double Epsilon);
	static bool AggregateOfPointsApproximation3D(const dVectors &AggregateOfPoints3D,
		dVector &Centre, dVector &Normal, double Epsilon);
};