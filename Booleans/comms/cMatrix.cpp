#include "cMatrix.h"
#include "cVectors.h"
#include "cSolver.h"

namespace comms {

const cMatrix cMatrix::Zero2D(0.0, 0.0,
							  0.0, 0.0);
const cMatrix cMatrix::Identity2D(1.0, 0.0,
								  0.0, 1.0);

const cMatrix cMatrix::Zero3D(0.0, 0.0, 0.0,
							  0.0, 0.0, 0.0,
							  0.0, 0.0, 0.0);
const cMatrix cMatrix::Identity3D(1.0, 0.0, 0.0,
								  0.0, 1.0, 0.0,
								  0.0, 0.0, 1.0);

const cMatrix cMatrix::Zero4D(0.0, 0.0, 0.0, 0.0,
							  0.0, 0.0, 0.0, 0.0,
							  0.0, 0.0, 0.0, 0.0,
							  0.0, 0.0, 0.0, 0.0);
const cMatrix cMatrix::Identity4D(1.0, 0.0, 0.0, 0.0,
								  0.0, 1.0, 0.0, 0.0,
								  0.0, 0.0, 1.0, 0.0,
								  0.0, 0.0, 0.0, 1.0);

// cMatrix::CalcDeterminant
double cMatrix::CalcDeterminant() const {
	switch(m_Dim) {
		case 2: // (2x2)
			return m[0][0] * m[1][1] - m[0][1] * m[1][0];
		case 3: // (3x3)
			double d2_12_01, d2_12_02, d2_12_12;
			d2_12_01 = m[1][0] * m[2][1] - m[1][1] * m[2][0];
			d2_12_02 = m[1][0] * m[2][2] - m[1][2] * m[2][0];
			d2_12_12 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
			return m[0][0] * d2_12_12 - m[0][1] * d2_12_02 + m[0][2] * d2_12_01;
		case 4: // (4x4)
			double d2_01_01, d2_01_02, d2_01_03, d2_01_12, d2_01_13, d2_01_23;
			d2_01_01 = m[0][0] * m[1][1] - m[0][1] * m[1][0];
			d2_01_02 = m[0][0] * m[1][2] - m[0][2] * m[1][0];
			d2_01_03 = m[0][0] * m[1][3] - m[0][3] * m[1][0];
			d2_01_12 = m[0][1] * m[1][2] - m[0][2] * m[1][1];
			d2_01_13 = m[0][1] * m[1][3] - m[0][3] * m[1][1];
			d2_01_23 = m[0][2] * m[1][3] - m[0][3] * m[1][2];

			double d3_012_012, d3_012_013, d3_012_023, d3_012_123;
			d3_012_012 = m[2][0] * d2_01_12 - m[2][1] * d2_01_02 + m[2][2] * d2_01_01;
			d3_012_013 = m[2][0] * d2_01_13 - m[2][1] * d2_01_03 + m[2][3] * d2_01_01;
			d3_012_023 = m[2][0] * d2_01_23 - m[2][2] * d2_01_03 + m[2][3] * d2_01_02;
			d3_012_123 = m[2][1] * d2_01_23 - m[2][2] * d2_01_13 + m[2][3] * d2_01_12;

			return - m[3][0] * d3_012_123 + m[3][1] * d3_012_023 -
				m[3][2] * d3_012_013 + m[3][3] * d3_012_012;
		default:
			assert(m_Dim >= 2 && m_Dim <= 4);
			return 0.0;
	}
} // cMatrix::CalcDeterminant

// cMatrix::Invert
bool cMatrix::Invert() {
	double Det, InvDet;
	switch(m_Dim) {
		case 2: // (2x2)
			Det = m[0][0] * m[1][1] - m[0][1] * m[1][0];

			if(cUtils::IsZero(Det)) return false;
			InvDet = 1.0 / Det;

			double m00;
			m00 = m[0][0];
			m[0][0] = m[1][1] * InvDet;
			m[0][1] = - m[0][1] * InvDet;
			m[1][0] = - m[1][0] * InvDet;
			m[1][1] = m00 * InvDet;

			return true;
		case 3: // (3x3)
			double d2_12_01, d2_12_02, d2_12_12;
			d2_12_01 = m[1][0] * m[2][1] - m[1][1] * m[2][0];
			d2_12_02 = m[1][0] * m[2][2] - m[1][2] * m[2][0];
			d2_12_12 = m[1][1] * m[2][2] - m[1][2] * m[2][1];

			Det =  m[0][0] * d2_12_12 - m[0][1] * d2_12_02 + m[0][2] * d2_12_01;

			if(cUtils::IsZero(Det)) return false;
			InvDet = 1.0 / Det;

			double d2_01_01, d2_01_02, d2_01_12;
			d2_01_01 = m[0][0] * m[1][1] - m[0][1] * m[1][0];
			d2_01_02 = m[0][0] * m[1][2] - m[0][2] * m[1][0];
			d2_01_12 = m[0][1] * m[1][2] - m[0][2] * m[1][1];

			double d2_02_01, d2_02_02, d2_02_12;
			d2_02_01 = m[0][0] * m[2][1] - m[0][1] * m[2][0];
			d2_02_02 = m[0][0] * m[2][2] - m[0][2] * m[2][0];
			d2_02_12 = m[0][1] * m[2][2] - m[0][2] * m[2][1];

			m[0][0] = + d2_12_12 * InvDet;
			m[1][0] = - d2_12_02 * InvDet;
			m[2][0] = + d2_12_01 * InvDet;

			m[0][1] = - d2_02_12 * InvDet;
			m[1][1] = + d2_02_02 * InvDet;
			m[2][1] = - d2_02_01 * InvDet;

			m[0][2] = + d2_01_12 * InvDet;
			m[1][2] = - d2_01_02 * InvDet;
			m[2][2] = + d2_01_01 * InvDet;
			
			return true;
		case 4: // (4x4)
			double d2_01_03, d2_01_13, d2_01_23;
			d2_01_01 = m[0][0] * m[1][1] - m[0][1] * m[1][0];
			d2_01_02 = m[0][0] * m[1][2] - m[0][2] * m[1][0];
			d2_01_03 = m[0][0] * m[1][3] - m[0][3] * m[1][0];
			d2_01_12 = m[0][1] * m[1][2] - m[0][2] * m[1][1];
			d2_01_13 = m[0][1] * m[1][3] - m[0][3] * m[1][1];
			d2_01_23 = m[0][2] * m[1][3] - m[0][3] * m[1][2];

			double d3_012_012, d3_012_013, d3_012_023, d3_012_123;
			d3_012_012 = m[2][0] * d2_01_12 - m[2][1] * d2_01_02 + m[2][2] * d2_01_01;
			d3_012_013 = m[2][0] * d2_01_13 - m[2][1] * d2_01_03 + m[2][3] * d2_01_01;
			d3_012_023 = m[2][0] * d2_01_23 - m[2][2] * d2_01_03 + m[2][3] * d2_01_02;
			d3_012_123 = m[2][1] * d2_01_23 - m[2][2] * d2_01_13 + m[2][3] * d2_01_12;

			Det =  - m[3][0] * d3_012_123 + m[3][1] * d3_012_023 -
				m[3][2] * d3_012_013 + m[3][3] * d3_012_012;

			if(cUtils::IsZero(Det)) return false;
			InvDet = 1.0 / Det;

			double d2_03_01, d2_03_02, d2_03_03, d2_03_12, d2_03_13, d2_03_23;
			d2_03_01 = m[0][0] * m[3][1] - m[0][1] * m[3][0];
			d2_03_02 = m[0][0] * m[3][2] - m[0][2] * m[3][0];
			d2_03_03 = m[0][0] * m[3][3] - m[0][3] * m[3][0];
			d2_03_12 = m[0][1] * m[3][2] - m[0][2] * m[3][1];
			d2_03_13 = m[0][1] * m[3][3] - m[0][3] * m[3][1];
			d2_03_23 = m[0][2] * m[3][3] - m[0][3] * m[3][2];

			double d2_13_01, d2_13_02, d2_13_03, d2_13_12, d2_13_13, d2_13_23;
			d2_13_01 = m[1][0] * m[3][1] - m[1][1] * m[3][0];
			d2_13_02 = m[1][0] * m[3][2] - m[1][2] * m[3][0];
			d2_13_03 = m[1][0] * m[3][3] - m[1][3] * m[3][0];
			d2_13_12 = m[1][1] * m[3][2] - m[1][2] * m[3][1];
			d2_13_13 = m[1][1] * m[3][3] - m[1][3] * m[3][1];
			d2_13_23 = m[1][2] * m[3][3] - m[1][3] * m[3][2];

			double d3_023_012, d3_023_013, d3_023_023, d3_023_123;
			d3_023_012 = m[2][0] * d2_03_12 - m[2][1] * d2_03_02 + m[2][2] * d2_03_01;
			d3_023_013 = m[2][0] * d2_03_13 - m[2][1] * d2_03_03 + m[2][3] * d2_03_01;
			d3_023_023 = m[2][0] * d2_03_23 - m[2][2] * d2_03_03 + m[2][3] * d2_03_02;
			d3_023_123 = m[2][1] * d2_03_23 - m[2][2] * d2_03_13 + m[2][3] * d2_03_12;

			double d3_123_012, d3_123_013, d3_123_023, d3_123_123;
			d3_123_012 = m[2][0] * d2_13_12 - m[2][1] * d2_13_02 + m[2][2] * d2_13_01;
			d3_123_013 = m[2][0] * d2_13_13 - m[2][1] * d2_13_03 + m[2][3] * d2_13_01;
			d3_123_023 = m[2][0] * d2_13_23 - m[2][2] * d2_13_03 + m[2][3] * d2_13_02;
			d3_123_123 = m[2][1] * d2_13_23 - m[2][2] * d2_13_13 + m[2][3] * d2_13_12;

			double d3_013_012, d3_013_013, d3_013_023, d3_013_123;
			d3_013_012 = m[3][0] * d2_01_12 - m[3][1] * d2_01_02 + m[3][2] * d2_01_01;
			d3_013_013 = m[3][0] * d2_01_13 - m[3][1] * d2_01_03 + m[3][3] * d2_01_01;
			d3_013_023 = m[3][0] * d2_01_23 - m[3][2] * d2_01_03 + m[3][3] * d2_01_02;
			d3_013_123 = m[3][1] * d2_01_23 - m[3][2] * d2_01_13 + m[3][3] * d2_01_12;

			m[0][0] = - d3_123_123 * InvDet;
			m[1][0] = + d3_123_023 * InvDet;
			m[2][0] = - d3_123_013 * InvDet;
			m[3][0] = + d3_123_012 * InvDet;

			m[0][1] = + d3_023_123 * InvDet;
			m[1][1] = - d3_023_023 * InvDet;
			m[2][1] = + d3_023_013 * InvDet;
			m[3][1] = - d3_023_012 * InvDet;

			m[0][2] = + d3_013_123 * InvDet;
			m[1][2] = - d3_013_023 * InvDet;
			m[2][2] = + d3_013_013 * InvDet;
			m[3][2] = - d3_013_012 * InvDet;

			m[0][3] = - d3_012_123 * InvDet;
			m[1][3] = + d3_012_023 * InvDet;
			m[2][3] = - d3_012_013 * InvDet;
			m[3][3] = + d3_012_012 * InvDet;

			return true;
		default:
			assert(m_Dim >= 2 && m_Dim <= 4);
			return false;
	}
} // cMatrix::Invert

// cMatrix::Transpose
cMatrix & cMatrix::Transpose() {
	for(int row = 0; row < m_Dim; row++)
		for(int col = row + 1; col < m_Dim; col++) {
			Swap(m[row][col], m[col][row]);
		}
	return *this;
} // cMatrix::Transpose

// cMatrix::Transposed
cMatrix cMatrix::Transposed() const {
	cMatrix T = *this;
	return T.Transpose();
} // cMatrix::Transposed

// cMatrix::OfTranslation
cMatrix cMatrix::OfTranslation(const cVector &u) {
	cMatrix T;
	T.LoadIdentity(u.Dim() + 1);
	for(int row = 0; row < u.Dim(); row++) {
		T.m[row][T.m_Dim - 1] = u[row];
	}
	return T;
} // cMatrix::OfTranslation

// cMatrix::OfRotation(const cVector &, double)
cMatrix cMatrix::OfRotation(const cVector &Axis, double Angle) {
	cMatrix R;
	R.LoadIdentity(3);
	double Cos = cos(Angle), Sin = sin(Angle);
	if(Axis == cVector::xAxis) { // Pitch
		// yAxis:
		R.m[1][1] = Cos;
		R.m[1][2] = - Sin;
		// zAxis:
		R.m[2][1] = Sin;
		R.m[2][2] = Cos;
	} else
		if(Axis == cVector::yAxis) { // Yaw
			// xAxis:
			R.m[0][0] = Cos;
			R.m[0][2] = Sin;
			// zAxis:
			R.m[2][0] = - Sin;
			R.m[2][2] = Cos;
		} else
			if(Axis == cVector::zAxis) { // Roll
				// xAxis:
				R.m[0][0] = Cos;
				R.m[0][1] = - Sin;
				// yAxis:
				R.m[1][0] = Sin;
				R.m[1][1] = Cos; 
			} else {
				cVector l = Axis;
				l.Normalize();
   				// xAxis:
				R.m[0][0] = Square(l.x) + (1. - Square(l.x)) * Cos;
				R.m[0][1] = l.x * l.y * (1. - Cos) - l.z * Sin;
				R.m[0][2] = l.x * l.z * (1. - Cos) + l.y * Sin;
				// yAxis:
				R.m[1][0] = l.x * l.y * (1. - Cos) + l.z * Sin;
				R.m[1][1] = Square(l.y) + (1. - Square(l.y)) * Cos;
				R.m[1][2] = l.y * l.z * (1. - Cos) - l.x * Sin;
				// zAxis:
				R.m[2][0] = l.x * l.z * (1. - Cos) - l.y * Sin;
				R.m[2][1] = l.y * l.z * (1. - Cos) + l.x * Sin;
				R.m[2][2] = Square(l.z) + (1. - Square(l.z)) * Cos;
			}
	return R;
} // cMatrix::OfRotation(const cVector &, double)

// cMatrix::OfRotation(double, double, double)
cMatrix cMatrix::OfRotation(double Yaw, double Pitch, double Roll) {
	return OfRotation(cVector::yAxis, Yaw) *
		OfRotation(cVector::zAxis, Roll) *
		OfRotation(cVector::xAxis, Pitch);
} // cMatrix::OfRotation(double, double, double)

// cMatrix::OfRotation(double)
cMatrix cMatrix::OfRotation(double Angle) {
	cMatrix R;
	R.LoadIdentity(2);
	double Cos = cos(Angle), Sin = sin(Angle);
// xAxis:
	R.m[0][0] = Cos;
	R.m[0][1] = - Sin;
	// yAxis:
	R.m[1][0] = Sin;
	R.m[1][1] = Cos; 

	return R;
} // cMatrix::OfRotation(double)

// cMatrix::OfScale
cMatrix cMatrix::OfScale(const cVector &u) {
	cMatrix S;
	S.LoadIdentity(u.Dim());
	for(int i = 0; i < u.Dim(); i++) {
		S.m[i][i] = u[i];
	}
	return S;
} // cMatrix::OfScale

// cMatrix::OfMirror
cMatrix cMatrix::OfMirror(const cVector &Axis) {
	cMatrix M;
	M.LoadIdentity(Axis.Dim());
	for(int i = 0; i < Axis.Dim(); i++) {
		if(Axis[i] == 1) M(i, i) = -1;
	}
	return M;
} // cMatrix::OfMirror

// cMatrix::OfTransform(double, double, double, double, const cVector &)
cMatrix cMatrix::OfTransform(double SinA, double CosA, double SinB, double CosB, const cVector &Pos) {
	cMatrix T = cMatrix::Identity4D;

	// xAxis:
	T.m[0][0] = CosA;
	T.m[0][1] = - SinA;
	T.m[0][3] = - T.m[0][0] * Pos.x - T.m[0][1] * Pos.y;

	// yAxis:
	T.m[1][0] = SinA * CosB;
	T.m[1][1] = CosA * CosB;
	T.m[1][2] = - SinB;
	T.m[1][3] = - T.m[1][0] * Pos.x - T.m[1][1] * Pos.y - T.m[1][2] * Pos.z;

	// zAxis:
	T.m[2][0] = SinA * SinB;
	T.m[2][1] = CosA * SinB;
	T.m[2][2] = CosB;
	T.m[2][3] = - T.m[2][0] * Pos.x - T.m[2][1] * Pos.y - T.m[2][2] * Pos.z;

	return T;
} // cMatrix::OfTransform(double, double, double, double, const cVector &)

// cMatrix::OfTransform(double, double, const cVector &)
cMatrix cMatrix::OfTransform(double Alpha, double Beta, const cVector &Pos) {
	double SinA = sin(Alpha), CosA = cos(Alpha), SinB = sin(Beta), CosB = cos(Beta);
	return OfTransform(SinA, CosA, SinB, CosB, Pos);
} // cMatrix::OfTransform(double, double, const cVector &)

// cMatrix::OfTransform(const cVector &, const cVector &)
cMatrix cMatrix::OfTransform(const cVector &Dir, const cVector &Pos) {
	if(cUtils::IsZero(Dir.x) && cUtils::IsZero(Dir.y)) {
		if(Dir.z >= 0.0) {
			return cMatrix::OfTranslation(- Pos);
		} else {
			return cMatrix::OfTranslation(cVector(Pos.x, - Pos.y, Pos.z));
		}
	}

	double L = Dir.Length();
	assert(cUtils::NotZero(L));
	double l = sqrt(Square(Dir.x) + Square(Dir.y));
	double SinA = - Dir.x / l, CosA = - Dir.y / l, SinB = l / L, CosB = - Dir.z / L;

	return OfTransform(SinA, CosA, SinB, CosB, Pos);
} // cMatrix::OfTransform(const cVector &, const cVector &)

// cMatrix::OfLookAt
/*cMatrix cMatrix::OfLookAt(const cVector &Eye, const cVector &Center, const cVector &Up) {
	// zAxis:
	cVector zAxis = Eye - Center;
	zAxis.Normalize();
	// xAxis:
	cVector xAxis = Up.Cross(zAxis);
	xAxis.Normalize();
	// yAxis:
	cVector yAxis = xAxis.Cross(zAxis);
	return cMatrix(cMatrix(xAxis, yAxis, zAxis), cVector::Origin);
}*/ // cMatrix::OfLookAt

//----------------------------------------------------------------------------------------------------------
// cMatrix::OfLookAt : cMatrix(const cVector &, const cVector &, const cVector &, eCoordSystemType)
//----------------------------------------------------------------------------------------------------------
cMatrix cMatrix::OfLookAt(const cVector &Eye, const cVector &At, const cVector &Up, eCoordSystemType Type) {
	cVector zAxis;
	switch(Type) {
		case CSLeftHanded:
			zAxis = (At - Eye).Normalize();
			break;
		case CSRightHanded:
			zAxis = (Eye - At).Normalize();
			break;
		default:
			assert(0);
	}
	cVector xAxis = Up.Cross(zAxis).Normalize();
	cVector yAxis = zAxis.Cross(xAxis);
	return cMatrix(cMatrix(xAxis, yAxis, zAxis),
		cVector(- xAxis.Dot(Eye), - yAxis.Dot(Eye), - zAxis.Dot(Eye)));
} // cMatrix::OfLookAt : cMatrix(const cVector &, const cVector &, const cVector &, eCoordSystemType)

//-----------------------------------------------------------------------------------------------------------
// cMatrix::OfLookAt : cMatrix(const cVector &, double, double, double)
//-----------------------------------------------------------------------------------------------------------
cMatrix cMatrix::OfLookAt(const cVector &At, double Azim, double Elev, double toEye, eCoordSystemType Type) {
	cVector Ref = cVector::Cartesian(1.0, Azim, Elev);
	cVector Up = cVector::Cartesian(1.0, Azim, Elev - Rad(45.0)) - Ref;
	cVector Eye = At + Ref.Normalize() * toEye;
	cVector zAxis;
	switch(Type) {
		case CSLeftHanded:
			zAxis = - Ref;
			break;
		case CSRightHanded:
			zAxis = Ref;
			break;
		default:
			assert(0);
	}
	cVector xAxis = Up.Cross(zAxis).Normalize();
	cVector yAxis = zAxis.Cross(xAxis);
	return cMatrix(cMatrix(xAxis, yAxis, zAxis),
		cVector(- xAxis.Dot(Eye), - yAxis.Dot(Eye), - zAxis.Dot(Eye)));
} // cMatrix::OfLookAt : cMatrix(const cVector &, const cVector &, const cVector &, eCoordSystemType)

// cMatrix::OfPerspective
cMatrix cMatrix::OfPerspective(double c) {
	return cMatrix(1., 0., 0., 0.,
		0., 1., 0., 0.,
		0., 0., 1., 0.,
		0., 0., - 1. / c, 1.);
} // cMatrix::OfPerspective

// cMatrix::OfProjection
cMatrix cMatrix::OfProjection(double Fov, double AspectWtoH, double zNear, double zFar) {
	double CoTan = 1. / tan(Fov * 0.5);
	double dZ = zFar - zNear;

	cMatrix P = cMatrix::Zero4D;

	P.m[0][0] = CoTan;
	P.m[1][1] = CoTan * AspectWtoH;
	P.m[2][2] = - (zFar + zNear) / dZ;
	P.m[3][2] = - 1.;
	P.m[2][3] = - 2 * zNear * zFar / dZ;
	P.m[3][3] = 1.;

	return P;
} // cMatrix::OfProjection

// cMatrix::OfInfiniteProjection
cMatrix cMatrix::OfInfiniteProjection(double Fov, double AspectWtoH, double zNear) {
	static const double Epsilon = 1 - 1. / (1 << 23);
	
	double CoTan = 1. / tan(Fov * 0.5);
	cMatrix P = cMatrix::Zero4D;

	P.m[0][0] = CoTan;
	P.m[1][1] = CoTan * AspectWtoH;
	P.m[2][2] = Epsilon - 1.;
	P.m[3][2] = - 1.;
	P.m[2][3] = (Epsilon - 2.) * zNear;

	return P;
} // cMatrix::OfInfiniteProjection

// cMatrix::EigenSolve
bool cMatrix::EigenSolve(doubles &EigenValues, cVectors &EigenVectors) {
	EigenValues.Free();
	EigenVectors.Free();

//	bool fIsSymmetric = IsSymmetric();
//	assert(fIsSymmetric);
//	if(!fIsSymmetric) return false;

	double q[3];
	cVector y[3], u;
	doubles l;
	switch(m_Dim) {
		case 2: // (2x2)
			// Eigen Values:
			double b, c;
			b = - (m[0][0] + m[1][1]);
			c = m[0][0] * m[1][1] - m[0][1] * m[1][0];
			int r;
			r = cSolver::SolveQuadraticEquation(l, 1., b, c);
			if(r != 2 /* two real roots */) return false;
			EigenValues = l;
			// Eigen Vectors:
			y[0].Set(0., 1.);
			y[1] = (*this) * y[0];
			for(int i = 0; i < 2; i++) {
				// Horner's method:
				q[0] = 1.;
				q[1] = l[i] * q[0] + b;
				u = q[0] * y[1] + q[1] * y[0];
				u.Normalize();
				EigenVectors.Add(u);
			}
			return true;
		case 3: // (3x3)
			// Eigen Values:
			double a;
			a = - (m[0][0] + m[1][1] + m[2][2]);
			b = m[0][0] * m[1][1] + m[0][0] * m[2][2] + m[1][1] * m[2][2] -
				m[1][2] * m[2][1] - m[0][1] * m[1][0] - m[0][2] * m[2][0];
			c = - (m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] -
				m[0][1] * m[1][0] * m[2][2] + m[0][1] * m[1][2] * m[2][0] +
				m[0][2] * m[1][0] * m[2][1] - m[0][2] * m[1][1] * m[2][0]);
			r = cSolver::SolveCubicEquation(l, a, b, c);
			if(r != 3 /* three real roots */) return false;
			EigenValues = l;
			// Eigen Vectors:
			y[0].Set(0., 0., 1.);
			y[1] = (*this) * y[0];
			y[2] = (*this) * y[1];
			for(i = 0; i < 3; i++) {
				// Horner's method:
				q[0] = 1.;
				q[1] = l[i] * q[0] + a;
				q[2] = l[i] * q[1] + b;
				u = q[0] * y[2] + q[1] * y[1] + q[2] * y[0];
				u.Normalize();
				EigenVectors.Add(u);
			}
			return true;
		default:
			assert(m_Dim >= 2 && m_Dim <= 4);
			return false;
	}
} // cMatrix::EigenSolve

// cMatrix::EigenSolve
bool cMatrix::EigenSolve(double &EigenValue, cVector &EigenVector) const {
	const int MaxIterations = 50;
	double PrevMax = 0., CurMax;
	cVector x;
	switch(m_Dim) {
		case 2:
			x.Set(-1., -1.);
			for(int i = 0; i < MaxIterations; i++) {
				x *= (*this);
				CurMax = Max(x.x, x.y);
				x /= CurMax;
				if(cUtils::IsZero((PrevMax - CurMax) / CurMax, 0.0001)) break;
				PrevMax = CurMax;
			}
			break;
		case 3:
			x.Set(1., 1., 1.);
			for(int i = 0; i < MaxIterations; i++) {
				x *= (*this);
				CurMax = Max(x.x, x.y, x.z);
				x /= CurMax;
				if(cUtils::IsZero((PrevMax - CurMax) / CurMax, 0.0001)) break;
				PrevMax = CurMax;
			}
			break;
	}
	x.Normalize();
	EigenValue = 1. / CurMax;
	EigenVector = x;
	return true;
} // cMatrix::EigenSolve

//-----------------------------------------------------------------------------
// operator << : cStr & (cStr &, const cMatrix &)
//-----------------------------------------------------------------------------
cStr & operator << (cStr &Str, const cMatrix &M) {
	Str += "[";
	for(int row = 0; row < M.Dim(); row++) {
		if(row) Str += ", ";
		Str += "[";
		for(int col = 0; col < M.Dim(); col++) {
			if(col) Str += ", ";
			Str += M(row, col);
		}
		Str += "]";
	}
	Str += "]";
	return Str;
} // operator << : cStr & (cStr &, const cMatrix &)

} // comms