#include "comms.h"

//-----------------------------------------------------------------------------------
// cSolver::GetRoots2
//-----------------------------------------------------------------------------------
int cSolver::GetRoots2(const float a, const float b, const float c, float *pRoots2) {
	if(a == 0.0f) {
		if(b == 0.0f) {
			if(c == 0.0f) {
				return -3; // Infinite number of roots (a == b == c == 0)
			}
			return -2; // No roots (a == b == 0)
		}
		pRoots2[0] = - c / b;
		return -1; // One real root (a == 0)
	}
	float d = b * b - 4.0f * a * c;
	if(d == 0.0f) {
		pRoots2[0] = - b / (2.0f * a);
		return 1; // One real root (d == 0)
	}
	if(d < 0.0f) { // Two conjugate complex roots (d < 0)
		float t = 0.5f / a;
		pRoots2[0] = - b * t;
		pRoots2[1] = cMath::Sqrt(-d) * t;
		return 0;
	}
	if(b >= 0.0f) {
		d = -0.5f * (b + cMath::Sqrt(d));
	} else {
		d = -0.5f * (b - cMath::Sqrt(d));
	}
	pRoots2[0] = d / a;
	pRoots2[1] = c / d;
	return 2;
}

//-------------------------------------------------------------------------------------------------------------------
// cPolynom::CalcLowestPositiveRoot
//-------------------------------------------------------------------------------------------------------------------
bool cSolver::GetLowestPositiveRoot2(const float a, const float b, const float c, const float MaxRoot, float &Root) {
	float Roots[2];
	int nRoots = GetRoots2(a, b, c, Roots);
	if(nRoots <= 0) {
		return false;
	}
	if(Roots[0] > Roots[1]) {
		cMath::Swap(Roots[0], Roots[1]);
	}

	if(Roots[0] > 0 && Roots[0] < MaxRoot) {
		Root = Roots[0];
		return true;
	}

	if(Roots[1] > 0 && Roots[1] < MaxRoot) {
		Root = Roots[1];
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------
// cSolver::GetRoots3
//-----------------------------------------------------------------------------------
int cSolver::GetRoots3(const float a, const float b, const float c, float *pRoots3) {
	float q = (a * a - 3.0f * b) / 9.0f;
	float r = (a * (2.0f * a * a - 9.0f * b) + 27.0f * c) / 54.0f;
	float r2 = r * r;
	float q3 = q * q * q;
	float aOver3 = a / 3.0f;
	if(r2 <= q3) {
		float t = cMath::ACos(r / cMath::Sqrt(q3));
		q = - 2.0f * cMath::Sqrt(q);
		pRoots3[0] = q * cMath::Cos(t / 3.0f) - aOver3;
		pRoots3[1] = q * cMath::Cos((t + cMath::TwoPi) / 3.0f) - aOver3;
		pRoots3[2] = q * cMath::Cos((t - cMath::TwoPi) / 3.0f) - aOver3;
		return 3;
	} else {
		if(r < 0.0f) r = - r;
		float aa = - cMath::Pow(r + cMath::Sqrt(r2 - q3), 1.0f / 3.0f);
		float bb = 0.0f;
		if(aa != 0.0f) bb = q / aa;
		q = aa + bb, r = aa - bb;
		pRoots3[0] = q - aOver3;
		pRoots3[1] = -0.5f * q - aOver3;
		float Im = cMath::Sqrt(3.0f) * 0.5f * cMath::Abs(r);
		if(Im == 0.0f) return 2;
		pRoots3[2] = Im;
		return 1;
	}
}

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
	if(cMath::IsZero(a)) {
		if(cMath::IsZero(b)) {
			if(cMath::IsZero(c)) return -3;
			return -2;
		}
		x.Add(- c / b);
		return -1;
	}
	double d = b * b - 4. * a * c;
	if(cMath::IsZero(d)) { // one real root:
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
	double q = (a * a - 3. * b) / 9.;
	double r = (a * (2. * a * a - 9. * b) + 27. * c) / 54.;
	double r2 = r * r;
	double q3 = q * q * q;
	if(r2 <= q3) {
		double t = acos(r / sqrt(q3));
		a /= 3., q = - 2. * sqrt(q);
		x.Add(q * cos(t / 3.) - a);
		x.Add(q * cos((t + cMath::dTwoPi) / 3.) - a);
		x.Add(q * cos((t - cMath::dTwoPi) / 3.) - a);
		return 3;
	} else {
		if(r < 0.) r = - r;
		double aa = - pow(r + sqrt(r2 - q3), 1. / 3.);
		double bb = 0.;
		if(!cMath::IsZero(aa)) bb = q / aa;
		a /= 3., q = aa + bb, r = aa - bb;
		x.Add(q - a);
		x.Add((- 0.5) * q - a);
		double Im = (sqrt(3.) * 0.5) * fabs(r);
		if(cMath::IsZero(Im)) return 2;
		x.Add(Im);
		return 1;
	}
} // cSolver::SolveCubicEquation

// cSolver::AggregateOfPointsApproximation
bool cSolver::AggregateOfPointsApproximation(const dVectors &AggregateOfPoints,
							dVector &Centre, dVector &Normal, double Epsilon) {
	Centre.LoadZero();
	Normal.LoadZero();
	if(!AggregateOfPoints.Count()) return false;
	int nDim = AggregateOfPoints.Dim();
	cAssert(nDim == 2 || nDim == 3);
	if(nDim == 2) {
		return AggregateOfPointsApproximation2D(AggregateOfPoints, Centre, Normal, Epsilon);
	} else if(nDim == 3) {
		return AggregateOfPointsApproximation3D(AggregateOfPoints, Centre, Normal, Epsilon);
	}
	return false;
} // cSolver::AggregateOfPointsApproximation

// cSolver::AggregateOfPointsApproximation2D
bool cSolver::AggregateOfPointsApproximation2D(const dVectors &AggregateOfPoints2D,
								dVector &Centre, dVector &Normal, double Epsilon) {
	Centre = dVector::Origin2D;
	Normal = dVector::Origin2D;

	if(AggregateOfPoints2D.Count() < 2) return false;

	// Centre:
	Centre = AggregateOfPoints2D.Centre();
	// BBox:
	dVector bbMin, bbMax;
	AggregateOfPoints2D.CalcBoundBox(bbMin, bbMax);
	dVector b = bbMax - bbMin;
	if(cMath::IsZero(b.x, Epsilon)) { // Along yAxis:
		Normal = dVector::xAxis2D;
		return true;
	} else if(cMath::IsZero(b.y, Epsilon)) { // Along xAxis:
		Normal = dVector::yAxis2D;
		return true;
	}
	// M:
	dMatrix M(Centre, dVector(0.0, 0.0));
	M = AggregateOfPoints2D.Count() * M.Transposed() * M;
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 2; j++) {
			double s = 0.;
			for(int k = 0; k < AggregateOfPoints2D.Count(); k++)
				s += AggregateOfPoints2D[k][i] * AggregateOfPoints2D[k][j];
			M(i, j) -= s;
		}
	cAssert(M.IsSymmetric());
	// Eigen values:
	doubles EigenValues;
	dVectors EigenVectors;
	if(M.EigenSolve(EigenValues, EigenVectors)) {
		Normal = EigenVectors[cMath::MaxIndex(EigenValues[0], EigenValues[1])];
		return true;
	}
	return false;
} // cSolver::AggregateOfPointsApproximation2D

// cSolver::AggregateOfPointsApproximation3D
bool cSolver::AggregateOfPointsApproximation3D(const dVectors &AggregateOfPoints3D,
							   dVector &Centre, dVector &Normal, double Epsilon) {
	Centre = dVector::Origin;
	Normal = dVector::Origin;

	if(AggregateOfPoints3D.Count() < 3) return false;

	// Centre:
	Centre = AggregateOfPoints3D.Centre();
	// BBox:
	dVector bbMin, bbMax;
	AggregateOfPoints3D.CalcBoundBox(bbMin, bbMax);
	dVector b = bbMax - bbMin;
	if(cMath::IsZero(b.x, Epsilon)) { // Plane YZ:
		Normal = dVector::xAxis;
		return true;
	} else if(cMath::IsZero(b.y, Epsilon)) { // Plane XZ:
		Normal = dVector::yAxis;
		return true;
	} else if(cMath::IsZero(b.z, Epsilon)) { // Plane XY:
		Normal = dVector::zAxis;
		return true;
	}
	// M:
	dMatrix M(Centre, dVector::Origin, dVector::Origin);
	M = AggregateOfPoints3D.Count() * M * M.Transposed();// * M;
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++) {
			double s = 0.;
			for(int k = 0; k < AggregateOfPoints3D.Count(); k++)
				s += AggregateOfPoints3D[k][i] * AggregateOfPoints3D[k][j];
			M(i, j) -= s;
		}
	cAssert(M.IsSymmetric(Epsilon));
	// Eigen values:
	doubles EigenValues;
	dVectors EigenVectors;
	if(M.EigenSolve(EigenValues, EigenVectors)) {
		Normal = EigenVectors[cMath::MaxIndex(EigenValues[0], EigenValues[1], EigenValues[2])];
		return true;
	}
	return false;
} // cSolver::AggregateOfPointsApproximation3D