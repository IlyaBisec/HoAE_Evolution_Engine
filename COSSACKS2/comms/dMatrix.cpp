#include "comms.h"

const dMatrix dMatrix::Zero2D(0.0, 0.0,
							  0.0, 0.0);
const dMatrix dMatrix::Identity2D(1.0, 0.0,
								  0.0, 1.0);

const dMatrix dMatrix::Zero3D(0.0, 0.0, 0.0,
							  0.0, 0.0, 0.0,
							  0.0, 0.0, 0.0);
const dMatrix dMatrix::Identity3D(1.0, 0.0, 0.0,
								  0.0, 1.0, 0.0,
								  0.0, 0.0, 1.0);

const dMatrix dMatrix::Zero4D(0.0, 0.0, 0.0, 0.0,
							  0.0, 0.0, 0.0, 0.0,
							  0.0, 0.0, 0.0, 0.0,
							  0.0, 0.0, 0.0, 0.0);
const dMatrix dMatrix::Identity4D(1.0, 0.0, 0.0, 0.0,
								  0.0, 1.0, 0.0, 0.0,
								  0.0, 0.0, 1.0, 0.0,
								  0.0, 0.0, 0.0, 1.0);

// dMatrix::CalcDeterminant
double dMatrix::CalcDeterminant() const {
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
			cAssert(m_Dim >= 2 && m_Dim <= 4);
			return 0.0;
	}
} // dMatrix::CalcDeterminant

// dMatrix::Invert
bool dMatrix::Invert() {
	double Det, InvDet;
	switch(m_Dim) {
		case 2: // (2x2)
			Det = m[0][0] * m[1][1] - m[0][1] * m[1][0];

			if(cMath::IsZero(Det)) return false;
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

			if(cMath::IsZero(Det)) return false;
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

			if(cMath::IsZero(Det)) return false;
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
			cAssert(m_Dim >= 2 && m_Dim <= 4);
			return false;
	}
} // dMatrix::Invert

// dMatrix::Transpose
dMatrix & dMatrix::Transpose() {
	for(int row = 0; row < m_Dim; row++)
		for(int col = row + 1; col < m_Dim; col++) {
			cMath::Swap(m[row][col], m[col][row]);
		}
	return *this;
} // dMatrix::Transpose

// dMatrix::Transposed
dMatrix dMatrix::Transposed() const {
	dMatrix T = *this;
	return T.Transpose();
} // dMatrix::Transposed

// dMatrix::OfTranslation
dMatrix dMatrix::OfTranslation(const dVector &u) {
	dMatrix T;
	T.LoadIdentity(u.Dim() + 1);
	for(int row = 0; row < u.Dim(); row++) {
		T.m[row][T.m_Dim - 1] = u[row];
	}
	return T;
} // dMatrix::OfTranslation

// dMatrix::OfRotation(const dVector &, double)
dMatrix dMatrix::OfRotation(const dVector &Axis, double Angle) {
	dMatrix R;
	R.LoadIdentity(3);
	double Cos = cos(Angle), Sin = sin(Angle);
	if(Axis == dVector::xAxis) { // Pitch
		// yAxis:
		R.m[1][1] = Cos;
		R.m[1][2] = - Sin;
		// zAxis:
		R.m[2][1] = Sin;
		R.m[2][2] = Cos;
	} else
		if(Axis == dVector::yAxis) { // Yaw
			// xAxis:
			R.m[0][0] = Cos;
			R.m[0][2] = Sin;
			// zAxis:
			R.m[2][0] = - Sin;
			R.m[2][2] = Cos;
		} else
			if(Axis == dVector::zAxis) { // Roll
				// xAxis:
				R.m[0][0] = Cos;
				R.m[0][1] = - Sin;
				// yAxis:
				R.m[1][0] = Sin;
				R.m[1][1] = Cos; 
			} else {
				dVector l = Axis;
				l.Normalize();
   				// xAxis:
				R.m[0][0] = cMath::Square(l.x) + (1. - cMath::Square(l.x)) * Cos;
				R.m[0][1] = l.x * l.y * (1. - Cos) - l.z * Sin;
				R.m[0][2] = l.x * l.z * (1. - Cos) + l.y * Sin;
				// yAxis:
				R.m[1][0] = l.x * l.y * (1. - Cos) + l.z * Sin;
				R.m[1][1] = cMath::Square(l.y) + (1. - cMath::Square(l.y)) * Cos;
				R.m[1][2] = l.y * l.z * (1. - Cos) - l.x * Sin;
				// zAxis:
				R.m[2][0] = l.x * l.z * (1. - Cos) - l.y * Sin;
				R.m[2][1] = l.y * l.z * (1. - Cos) + l.x * Sin;
				R.m[2][2] = cMath::Square(l.z) + (1. - cMath::Square(l.z)) * Cos;
			}
	return R;
} // dMatrix::OfRotation(const dVector &, double)

// dMatrix::OfRotation(double, double, double)
dMatrix dMatrix::OfRotation(double Yaw, double Pitch, double Roll) {
	return OfRotation(dVector::yAxis, Yaw) *
		OfRotation(dVector::zAxis, Roll) *
		OfRotation(dVector::xAxis, Pitch);
} // dMatrix::OfRotation(double, double, double)

// dMatrix::OfRotation(double)
dMatrix dMatrix::OfRotation(double Angle) {
	dMatrix R;
	R.LoadIdentity(2);
	double Cos = cos(Angle), Sin = sin(Angle);
// xAxis:
	R.m[0][0] = Cos;
	R.m[0][1] = - Sin;
	// yAxis:
	R.m[1][0] = Sin;
	R.m[1][1] = Cos; 

	return R;
} // dMatrix::OfRotation(double)

// dMatrix::OfScale
dMatrix dMatrix::OfScale(const dVector &u) {
	dMatrix S;
	S.LoadIdentity(u.Dim());
	for(int i = 0; i < u.Dim(); i++) {
		S.m[i][i] = u[i];
	}
	return S;
} // dMatrix::OfScale

// dMatrix::OfMirror
dMatrix dMatrix::OfMirror(const dVector &Axis) {
	dMatrix M;
	M.LoadIdentity(Axis.Dim());
	for(int i = 0; i < Axis.Dim(); i++) {
		if(Axis[i] == 1) M(i, i) = -1;
	}
	return M;
} // dMatrix::OfMirror

// dMatrix::OfTransform(double, double, double, double, const dVector &)
dMatrix dMatrix::OfTransform(double SinA, double CosA, double SinB, double CosB, const dVector &Pos) {
	dMatrix T = dMatrix::Identity4D;

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
} // dMatrix::OfTransform(double, double, double, double, const dVector &)

// dMatrix::OfTransform(double, double, const dVector &)
dMatrix dMatrix::OfTransform(double Alpha, double Beta, const dVector &Pos) {
	double SinA = sin(Alpha), CosA = cos(Alpha), SinB = sin(Beta), CosB = cos(Beta);
	return OfTransform(SinA, CosA, SinB, CosB, Pos);
} // dMatrix::OfTransform(double, double, const dVector &)

// dMatrix::OfTransform(const dVector &, const dVector &)
dMatrix dMatrix::OfTransform(const dVector &Dir, const dVector &Pos) {
	if(cMath::IsZero(Dir.x) && cMath::IsZero(Dir.y)) {
		if(Dir.z >= 0.0) {
			return dMatrix::OfTranslation(- Pos);
		} else {
			return dMatrix::OfTranslation(dVector(Pos.x, - Pos.y, Pos.z));
		}
	}

	double L = Dir.Length();
	cAssert(!cMath::IsZero(L));
	double l = sqrt(cMath::Square(Dir.x) + cMath::Square(Dir.y));
	double SinA = - Dir.x / l, CosA = - Dir.y / l, SinB = l / L, CosB = - Dir.z / L;

	return OfTransform(SinA, CosA, SinB, CosB, Pos);
} // dMatrix::OfTransform(const dVector &, const dVector &)

// dMatrix::OfLookAt
/*dMatrix dMatrix::OfLookAt(const dVector &Eye, const dVector &Center, const dVector &Up) {
	// zAxis:
	dVector zAxis = Eye - Center;
	zAxis.Normalize();
	// xAxis:
	dVector xAxis = Up.Cross(zAxis);
	xAxis.Normalize();
	// yAxis:
	dVector yAxis = xAxis.Cross(zAxis);
	return dMatrix(dMatrix(xAxis, yAxis, zAxis), dVector::Origin);
}*/ // dMatrix::OfLookAt

//----------------------------------------------------------------------------------------------------------
// dMatrix::OfLookAt : dMatrix(const dVector &, const dVector &, const dVector &, eCoordSystemType)
//----------------------------------------------------------------------------------------------------------
dMatrix dMatrix::OfLookAt(const dVector &Eye, const dVector &At, const dVector &Up, eCoordSystemType Type) {
	dVector zAxis;
	switch(Type) {
		case CSLeftHanded:
			zAxis = (At - Eye).Normalize();
			break;
		case CSRightHanded:
			zAxis = (Eye - At).Normalize();
			break;
		default:
			cAssert(0);
	}
	dVector xAxis = Up.Cross(zAxis).Normalize();
	dVector yAxis = zAxis.Cross(xAxis);
	return dMatrix(dMatrix(xAxis, yAxis, zAxis),
		dVector(- xAxis.Dot(Eye), - yAxis.Dot(Eye), - zAxis.Dot(Eye)));
} // dMatrix::OfLookAt : dMatrix(const dVector &, const dVector &, const dVector &, eCoordSystemType)

//-----------------------------------------------------------------------------------------------------------
// dMatrix::OfLookAt : dMatrix(const dVector &, double, double, double)
//-----------------------------------------------------------------------------------------------------------
dMatrix dMatrix::OfLookAt(const dVector &At, double Azim, double Elev, double toEye, eCoordSystemType Type) {
	cAssert(0);
	return dMatrix::Identity4D;
/*	dVector Ref = dVector::Cartesian(1.0, Azim, Elev);
	dVector Up = dVector::Cartesian(1.0, Azim, Elev - Rad(45.0)) - Ref;
	dVector Eye = At + Ref.Normalize() * toEye;
	dVector zAxis;
	switch(Type) {
		case CSLeftHanded:
			zAxis = - Ref;
			break;
		case CSRightHanded:
			zAxis = Ref;
			break;
		default:
			cAssert(0);
	}
	dVector xAxis = Up.Cross(zAxis).Normalize();
	dVector yAxis = zAxis.Cross(xAxis);
	return dMatrix(dMatrix(xAxis, yAxis, zAxis),
		dVector(- xAxis.Dot(Eye), - yAxis.Dot(Eye), - zAxis.Dot(Eye)));*/
} // dMatrix::OfLookAt : dMatrix(const dVector &, const dVector &, const dVector &, eCoordSystemType)

// dMatrix::OfPerspective
dMatrix dMatrix::OfPerspective(double c) {
	return dMatrix(1., 0., 0., 0.,
		0., 1., 0., 0.,
		0., 0., 1., 0.,
		0., 0., - 1. / c, 1.);
} // dMatrix::OfPerspective

// dMatrix::OfProjection
dMatrix dMatrix::OfProjection(double Fov, double AspectWtoH, double zNear, double zFar) {
	double CoTan = 1. / tan(Fov * 0.5);
	double dZ = zFar - zNear;

	dMatrix P = dMatrix::Zero4D;

	P.m[0][0] = CoTan;
	P.m[1][1] = CoTan * AspectWtoH;
	P.m[2][2] = - (zFar + zNear) / dZ;
	P.m[3][2] = - 1.;
	P.m[2][3] = - 2 * zNear * zFar / dZ;
	P.m[3][3] = 1.;

	return P;
} // dMatrix::OfProjection

// dMatrix::OfInfiniteProjection
dMatrix dMatrix::OfInfiniteProjection(double Fov, double AspectWtoH, double zNear) {
	static const double Epsilon = 1 - 1. / (1 << 23);
	
	double CoTan = 1. / tan(Fov * 0.5);
	dMatrix P = dMatrix::Zero4D;

	P.m[0][0] = CoTan;
	P.m[1][1] = CoTan * AspectWtoH;
	P.m[2][2] = Epsilon - 1.;
	P.m[3][2] = - 1.;
	P.m[2][3] = (Epsilon - 2.) * zNear;

	return P;
} // dMatrix::OfInfiniteProjection

// dMatrix::EigenSolve
bool dMatrix::EigenSolve(doubles &EigenValues, dVectors &EigenVectors) {
	EigenValues.Free();
	EigenVectors.Free();

//	bool fIsSymmetric = IsSymmetric();
//	cAssert(fIsSymmetric);
//	if(!fIsSymmetric) return false;

	double q[3];
	dVector y[3], u;
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
			cAssert(m_Dim >= 2 && m_Dim <= 4);
			return false;
	}
} // dMatrix::EigenSolve

// dMatrix::EigenSolve
bool dMatrix::EigenSolve(double &EigenValue, dVector &EigenVector) const {
	const int MaxIterations = 50;
	double PrevMax = 0., CurMax;
	dVector x;
	switch(m_Dim) {
		case 2:
			x.Set(-1., -1.);
			for(int i = 0; i < MaxIterations; i++) {
				x *= (*this);
				CurMax = cMath::Max(x.x, x.y);
				x /= CurMax;
				if(cMath::IsZero((PrevMax - CurMax) / CurMax, 0.0001)) break;
				PrevMax = CurMax;
			}
			break;
		case 3:
			x.Set(1., 1., 1.);
			for(int i = 0; i < MaxIterations; i++) {
				x *= (*this);
				CurMax = cMath::Max(x.x, x.y, x.z);
				x /= CurMax;
				if(cMath::IsZero((PrevMax - CurMax) / CurMax, 0.0001)) break;
				PrevMax = CurMax;
			}
			break;
	}
	x.Normalize();
	EigenValue = 1. / CurMax;
	EigenVector = x;
	return true;
} // dMatrix::EigenSolve

//-----------------------------------------------------------------------------
// operator << : cStr & (cStr &, const dMatrix &)
//-----------------------------------------------------------------------------
cStr & operator << (cStr &Str, const dMatrix &M) {
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
} // operator << : cStr & (cStr &, const dMatrix &)