#pragma once

namespace comms {

/*
template<class Type, unsigned Rows, unsigned Cols>
struct cMatrix {
	cMatrix();
	cMatrix(const Type m00, const Type m01,
			const Type m10, const Type m11);
	cMatrix(const Type m00, const Type m01,
			const Type m10, const Type m11,
			const Type m20, const Type m21);
	cMatrix(const Type m00, const Type m01, const Type m02,
			const Type m10, const Type m11, const Type m12,
			const Type m20, const Type m21, const Type m22);
	cMatrix(const Type m00, const Type m01, const Type m02,
			const Type m10, const Type m11, const Type m12,
			const Type m20, const Type m21, const Type m22,
			const Type m30, const Type m31, const Type m32);
	cMatrix(const Type m00, const Type m01, const Type m02, const Type m03,
			const Type m10, const Type m11, const Type m12, const Type m13,
			const Type m20, const Type m21, const Type m22, const Type m23,
			const Type m30, const Type m31, const Type m32, const Type m33);
	cMatrix(const cMatrix<Type, Rows, Cols> &Src);
	cMatrix<Type, Rows, Cols> & operator = (const cMatrix<Type, Rows, Cols> &Src);

	cMatrix<Type, Rows, Cols> & Copy(const Type *pSrc);
	cMatrix<Type, Rows, Cols> & CopyTransposed(const Type *pSrc);

	cVector<Type, Cols> & operator [] (const unsigned row);
	const cVector<Type, Cols> & operator [] (const unsigned row) const;

	Type * ToPtr();
	const Type * ToPtr() const;
	//cStr ToStr() const;

	cVector<Type, Cols> m[Rows];
};

typedef cMatrix<float, 2, 2> cMatrix2;
typedef cMatrix<float, 3, 2> cMatrix32;
typedef cMatrix<float, 3, 3> cMatrix3;
typedef cMatrix<float, 4, 3> cMatrix43;
typedef cMatrix<float, 4, 4> cMatrix4;

// cMatrix<Type, Rows, Cols>.ctor : cMatrix()
template<class Type, unsigned Rows, unsigned Cols>
inline cMatrix<Type, Rows, Cols>::cMatrix() {
	for(unsigned d = 0; d < Min(Cols, Rows); d++) {
		m[d][d] = (Type)1;
	}
}

// cMatrix<Type, 2, 2>.ctor : cMatrix(const Type m00, const Type m01,
//									  const Type m10, const Type m11)
template<class Type, unsigned Rows, unsigned Cols>
inline cMatrix<Type, Rows, Cols>::cMatrix(const Type m00, const Type m01,
										  const Type m10, const Type m11) {
	assert(Rows == 2 && Cols == 2);
	m[0][0] = m00, m[0][1] = m01;
	m[1][0] = m10, m[1][1] = m11;
}

// cMatrix<Type, 3, 2>.ctor : cMatrix(const Type m00, const Type m01,
//								      const Type m10, const Type m11,
//								      const Type m20, const Type m21)
template<class Type, unsigned Rows, unsigned Cols>
inline cMatrix<Type, Rows, Cols>::cMatrix(const Type m00, const Type m01,
										  const Type m10, const Type m11,
										  const Type m20, const Type m21) {
	assert(Rows == 3 && Cols == 2);
	m[0][0] = m00, m[0][1] = m01;
	m[1][0] = m10, m[1][1] = m11;
	m[2][0] = m20, m[2][1] = m21;
}

// cMatrix<Type, 3, 3>.ctor : cMatrix(const Type m00, const Type m01, const Type m02,
//								      const Type m10, const Type m11, const Type m12,
//								      const Type m20, const Type m21, const Type m22)
template<class Type, unsigned Rows, unsigned Cols>
inline cMatrix<Type, Rows, Cols>::cMatrix(const Type m00, const Type m01, const Type m02,
										  const Type m10, const Type m11, const Type m12,
										  const Type m20, const Type m21, const Type m22) {
	assert(Rows == 3 && Cols == 3);
	m[0][0] = m00, m[0][1] = m01, m[0][2] = m02;
	m[1][0] = m10, m[1][1] = m11, m[1][2] = m12;
	m[2][0] = m20, m[2][1] = m21, m[2][2] = m22;
}

// cMatrix<Type, 4, 3>.ctor : cMatrix(const Type m00, const Type m01, const Type m02,
//									  const Type m10, const Type m11, const Type m12,
//									  const Type m20, const Type m21, const Type m22,
//									  const Type m30, const Type m31, const Type m32)
template<class Type, unsigned Rows, unsigned Cols>
inline cMatrix<Type, Rows, Cols>::cMatrix(const Type m00, const Type m01, const Type m02,
										  const Type m10, const Type m11, const Type m12,
										  const Type m20, const Type m21, const Type m22,
										  const Type m30, const Type m31, const Type m32) {
	assert(Rows == 4 && Cols == 3);
	m[0][0] = m00, m[0][1] = m01, m[0][2] = m02;
	m[1][0] = m10, m[1][1] = m11, m[1][2] = m12;
	m[2][0] = m20, m[2][1] = m21, m[2][2] = m22;
	m[3][0] = m30, m[3][1] = m31, m[3][2] = m32;
}

// cMatrix<Type, 4, 4>.ctor : cMatrix(const Type m00, const Type m01, const Type m02, const Type m03,
//									  const Type m10, const Type m11, const Type m12, const Type m13,
//									  const Type m20, const Type m21, const Type m22, const Type m23,
//									  const Type m30, const Type m31, const Type m32, const Type m33)
template<class Type, unsigned Rows, unsigned Cols>
inline cMatrix<Type, Rows, Cols>::cMatrix(const Type m00, const Type m01, const Type m02, const Type m03,
										  const Type m10, const Type m11, const Type m12, const Type m13,
										  const Type m20, const Type m21, const Type m22, const Type m23,
										  const Type m30, const Type m31, const Type m32, const Type m33) {
	assert(Rows == 4 && Cols == 4);
	m[0][0] = m00, m[0][1] = m01, m[0][2] = m02, m[0][3] = m03;
	m[1][0] = m10, m[1][1] = m11, m[1][2] = m12, m[1][3] = m13;
	m[2][0] = m20, m[2][1] = m21, m[2][2] = m22, m[2][3] = m23;
	m[3][0] = m30, m[3][1] = m31, m[3][2] = m32, m[3][3] = m33;
}

// cMatrix<Type, Rows, Cols>.ctor : cMatrix(const cMatrix<Type, Rows, Cols> &Src)
template<class Type, unsigned Rows, unsigned Cols>
inline cMatrix<Type, Rows, Cols>::cMatrix(const cMatrix<Type, Rows, Cols> &Src) {
	Copy(Src.ToPtr());
}

// cMatrix<Type, Rows, Cols>::operator = : cMatrix<Type, Rows, Cols> & (const cMatrix<Type, Rows, Cols> &Src)
template<class Type, unsigned Rows, unsigned Cols>
inline cMatrix<Type, Rows, Cols> & cMatrix<Type, Rows, Cols>::operator = (const cMatrix<Type, Rows, Cols> &Src) {
	Copy(Src.ToPtr());
	return *this;
}

// cMatrix<Type, Rows, Cols>::Copy : cMatrix<Type, Rows, Cols> & (const Type *pSrc)
template<class Type, unsigned Rows, unsigned Cols>
inline cMatrix<Type, Rows, Cols> & cMatrix<Type, Rows, Cols>::Copy(const Type *pSrc) {
	assert(pSrc);
	for(unsigned row = 0; row < Rows; row++) {
		for(unsigned col = 0; col < Cols; col++) {
			m[row][col] = *pSrc;
			pSrc++;
		}
	}
	return *this;
}

// cMatrix<Type, Rows, Cols>::CopyTransposed : cMatrix<Type, Rows, Cols> & (const Type *pSrc)
template<class Type, unsigned Rows, unsigned Cols>
inline cMatrix<Type, Rows, Cols> & cMatrix<Type, Rows, Cols>::CopyTransposed(const Type *pSrc) {
	assert(pSrc);
	for(unsigned col = 0; col < Cols; col++) {
		for(unsigned row = 0; row < Rows; row++) {
			m[row][col] = *pSrc;
			pSrc++;
		}
	}
	return *this;
}

// cMatrix<Type, Rows, Cols>::operator [] : cVector<Type, Cols> & (const unsigned row)
template<class Type, unsigned Rows, unsigned Cols>
inline cVector<Type, Cols> & cMatrix<Type, Rows, Cols>::operator [] (const unsigned row) {
	assert(row < Rows);
	return m[row];
}

// cMatrix<Type, Rows, Cols>::operator [] : const cVector<Type, Cols> & (const unsigned row) const
template<class Type, unsigned Rows, unsigned Cols>
inline const cVector<Type, Cols> & cMatrix<Type, Rows, Cols>::operator [] (const unsigned row) const {
	assert(row < Rows);
	return m[row];
}

// cMatrix<Type, Rows, Cols>::ToPtr : Type * ()
template<class Type, unsigned Rows, unsigned Cols>
inline Type * cMatrix<Type, Rows, Cols>::ToPtr() {
	return m[0].ToPtr();
}

// cMatrix<Type, Rows, Cols>::ToPtr : const Type * () const
template<class Type, unsigned Rows, unsigned Cols>
inline const Type * cMatrix<Type, Rows, Cols>::ToPtr() const {
	return m[0].ToPtr();
}

// operator - : cMatrix<Type, Rows, Cols>(const cMatrix<Type, Rows, Cols> &)
template<class Type, unsigned Rows, unsigned Cols>
inline cMatrix<Type, Rows, Cols> operator - (const cMatrix<Type, Rows, Cols> &M) {
	cMatrix<Type, Rows, Cols> R;
	for(unsigned row = 0; row < Rows; row++) {
		for(unsigned col = 0; col < Cols; col++) {
			R[row][col] = - M[row][col];
		}
	}
	return R;
}

// operator += : cMatrix<Type, Rows, Cols> & (cMatrix<Type, Rows, Cols> &, const cMatrix<Type, Rows, Cols> &)
template<class Type, unsigned Rows, unsigned Cols>
inline cMatrix<Type, Rows, Cols> & operator += (cMatrix<Type, Rows, Cols> &A, const cMatrix<Type, Rows, Cols> &B) {
	for(unsigned row = 0; row < Rows; row++) {
		for(unsigned col = 0; col < Cols; col++) {
			A[row][col] += B[row][col];
		}
	}
	return A;
}

// operator -= : cMatrix<Type, Rows, Cols> & (cMatrix<Type, Rows, Cols> &, const cMatrix<Type, Rows, Cols> &)
template<class Type, unsigned Rows, unsigned Cols>
inline cMatrix<Type, Rows, Cols> & operator -= (cMatrix<Type, Rows, Cols> &A, const cMatrix<Type, Rows, Cols> &B) {
	for(unsigned row = 0; row < Rows; row++) {
		for(unsigned col = 0; col < Cols; col++) {
			A[row][col] -= B[row][col];
		}
	}
	return A;
}

// operator *= : cMatrix<Type, Rows, Cols> & (cMatrix<Type, Rows, Cols> &, const Scalar)
template<class Type, unsigned Rows, unsigned Cols, class Scalar>
inline cMatrix<Type, Rows, Cols> & operator *= (cMatrix<Type, Rows, Cols> &M, const Scalar s) {
	for(unsigned row = 0; row < Rows; row++) {
		for(unsigned col = 0; col < Cols; col++) {
			M[row][col] *= (Type)s;
		}
	}
	return M;
}

// Mul : cMatrix<Type, Rows, Cols>(const cMatrix<Type, Rows, Dim> &, const cMatrix<Type, Dim, Cols> &)
template<class Type, unsigned Rows, unsigned Dim, unsigned Cols>
inline cMatrix<Type, Rows, Cols> Mul(const cMatrix<Type, Rows, Dim> &A, const cMatrix<Type, Dim, Cols> &B) {
	cMatrix<Type, Rows, Cols> R;
	Zero(R);
	for(unsigned row = 0; row < Rows; row++) {
		for(unsigned col = 0; col < Cols; col++) {
			for(unsigned d = 0; d < Dim; d++) {
				R[row][col] += A[row][d] * B[d][col];
			}
		}
	}
	return R;
}

// operator *= : cMatrix<Type, Dim, Dim> & (cMatrix<Type, Dim, Dim> &, const cMatrix<Type, Dim, Dim> &)
template<class Type, unsigned Dim>
inline cMatrix<Type, Dim, Dim> & operator *= (cMatrix<Type, Dim, Dim> &A, const cMatrix<Type, Dim, Dim> &B) {
	return A = Mul(A, B);
}

// operator + : cMatrix<Type, Rows, Cols>(const cMatrix<Type, Rows, Cols> &, const cMatrix<Type, Rows, Cols> &)
template<class Type, unsigned Rows, unsigned Cols>
inline cMatrix<Type, Rows, Cols> operator + (const cMatrix<Type, Rows, Cols> &A, const cMatrix<Type, Rows, Cols> &B) {
	cMatrix<Type, Rows, Cols> T;
	for(unsigned row = 0; row < Rows; row++) {
		for(unsigned col = 0; col < Cols; col++) {
			T[row][col] = A[row][col] + B[row][col];
		}
	}
	return T;
}

// operator - : cMatrix<Type, Rows, Cols>(const cMatrix<Type, Rows, Cols> &, const cMatrix<Type, Rows, Cols> &)
template<class Type, unsigned Rows, unsigned Cols>
inline cMatrix<Type, Rows, Cols> operator - (const cMatrix<Type, Rows, Cols> &A, const cMatrix<Type, Rows, Cols> &B) {
	cMatrix<Type, Rows, Cols> T;
	for(unsigned row = 0; row < Rows; row++) {
		for(unsigned col = 0; col < Cols; col++) {
			T[row][col] = A[row][col] - B[row][col];
		}
	}
	return T;
}

// operator * : cMatrix<Type, Rows, Cols>(const cMatrix<Type, Rows, Cols> &, const Scalar)
template<class Type, unsigned Rows, unsigned Cols, class Scalar>
inline cMatrix<Type, Rows, Cols> operator * (const cMatrix<Type, Rows, Cols> &M, const Scalar s) {
	cMatrix<Type, Rows, Cols> T(M);
	return T *= s;
}

// operator * : cMatrix<Type, Rows, Cols>(const Scalar, const cMatrix<Type, Rows, Cols> &)
template<class Type, unsigned Rows, unsigned Cols, class Scalar>
inline cMatrix<Type, Rows, Cols> operator * (const Scalar s, const cMatrix<Type, Rows, Cols> &M) {
	cMatrix<Type, Rows, Cols> T(M);
	return T *= s;
}

// operator * : cMatrix<Type, Rows, Cols>(const cMatrix<Type, Rows, Dim> &, const cMatrix<Type, Dim, Cols> &)
template<class Type, unsigned Rows, unsigned Dim, unsigned Cols>
inline cMatrix<Type, Rows, Cols> operator * (const cMatrix<Type, Rows, Dim> &A, const cMatrix<Type, Dim, Cols> &B) {
	return Mul(A, B);
}

// Mul : cVector<Type, Cols>(const cVector<Type, Rows> &, const cMatrix<Type, Rows, Cols> &)
template<class Type, unsigned Rows, unsigned Cols>
inline cVector<Type, Cols> Mul(const cVector<Type, Rows> &u, const cMatrix<Type, Rows, Cols> &M) {
	cVector<Type, Cols> t;
	for(unsigned col = 0; col < Cols; col++) {
		for(unsigned row = 0; row < Rows; row++) {
			t[col] += u[row] * M[row][col];
		}
	}
	return t;
}

// operator * : cVector<Type, Cols>(const cVector<Type, Rows> &, const cMatrix<Type, Rows, Cols> &)
template<class Type, unsigned Rows, unsigned Cols>
inline cVector<Type, Cols> operator * (const cVector<Type, Rows> &u, const cMatrix<Type, Rows, Cols> &M) {
	return Mul(u, M);
}

// operator *= : cVector<Type, Dim> & (cVector<Type, Dim> &, const cMatrix<Type, Dim, Dim> &)
template<class Type, unsigned Dim>
inline cVector<Type, Dim> & operator *= (cVector<Type, Dim> &u, const cMatrix<Type, Dim, Dim> &M) {
	return u = Mul(u, M);
}

// Mul : cVector<Type, Rows>(const cMatrix<Type, Rows, Cols> &, const cVector<Type, Cols> &)
template<class Type, unsigned Rows, unsigned Cols>
inline cVector<Type, Rows> Mul(const cMatrix<Type, Rows, Cols> &M, const cVector<Type, Cols> &u) {
	cVector<Type, Rows> t;
	for(unsigned row = 0; row < Rows; row++) {
		for(unsigned col = 0; col < Cols; col++) {
			t[row] += M[row][col] * u[col];
		}
	}
	return t;
}

// operator * : cVector<Type, Rows>(const cMatrix<Type, Rows, Cols> &, const cVector<Type, Cols> &)
template<class Type, unsigned Rows, unsigned Cols>
inline cVector<Type, Rows> operator * (const cMatrix<Type, Rows, Cols> &M, const cVector<Type, Cols> &u) {
	return Mul(M, u);
}

// operator == : bool(const cMatrix<Type, Rows, Cols> &, const cMatrix<Type, Rows, Cols> &)
template<class Type, unsigned Rows, unsigned Cols>
inline bool operator == (const cMatrix<Type, Rows, Cols> &A, const cMatrix<Type, Rows, Cols> &B) {
	for(unsigned row = 0; row < Rows; row++) {
		for(unsigned col = 0; col < Cols; col++) {
			if(A[row][col] != B[row][col]) {
				return false;
			}
		}
	}
	return true;
}

// operator != : bool(const cMatrix<Type, Rows, Cols> &, const cMatrix<Type, Rows, Cols> &)
template<class Type, unsigned Rows, unsigned Cols>
inline bool operator != (const cMatrix<Type, Rows, Cols> &A, const cMatrix<Type, Rows, Cols> &B) {
	return !(A == B);
}

// Equals : bool(const cMatrix<Type, Rows, Cols> &, const cMatrix<Type, Rows, Cols> &, const Type)
template<class Type, unsigned Rows, unsigned Cols>
inline bool Equals(const cMatrix<Type, Rows, Cols> &A, const cMatrix<Type, Rows, Cols> &B, const Type Epsilon = (Type)0.001) {
	assert(Epsilon >= (Type)0);
	for(unsigned row = 0; row < Rows; row++) {
		for(unsigned col = 0; col < Cols; col++) {
			if(fabs(A[row][col] - B[row][col]) > Epsilon) return false;
		}
	}
	return true;
}

// Zero : cMatrix<Type, Rows, Cols> & (cMatrix<Type, Rows, Cols> &)
template<class Type, unsigned Rows, unsigned Cols>
inline cMatrix<Type, Rows, Cols> & Zero(cMatrix<Type, Rows, Cols> &M) {
	for(unsigned row = 0; row < Rows; row++) {
		for(unsigned col = 0; col < Cols; col++) {
			M[row][col] = (Type)0;
		}
	}
	return M;
}

// Identity : cMatrix<Type, Rows, Cols> & (cMatrix<Type, Rows, Cols> &)
template<class Type, unsigned Rows, unsigned Cols>
inline cMatrix<Type, Rows, Cols> & Identity(cMatrix<Type, Rows, Cols> &M) {
	for(unsigned row = 0; row < Rows; row++) {
		for(unsigned col = 0; col < Cols; col++) {
			M[row][col] = (Type)0;
		}
	}
	for(unsigned d = 0; d < Min(Cols, Rows); d++) {
		M[d][d] = (Type)1;
	}
	return M;
}

// Transpose : cMatrix<Type, Dim, Dim> & (cMatrix<Type, Dim, Dim> &)
template<class Type, unsigned Dim>
inline cMatrix<Type, Dim, Dim> & Transpose(cMatrix<Type, Dim, Dim> &M) {
	for(unsigned col = 0; col < Dim; col++) {
		for(unsigned row = col + 1; row < Dim; row++) {
			Swap(M[row][col], M[col][row]);
		}
	}
	return M;
}

// MakeTranspose : cMatrix<Type, Rows, Cols>(const cMatrix<Type, Cols, Rows> &) 
template<class Type, unsigned Rows, unsigned Cols>
inline cMatrix<Type, Rows, Cols> MakeTranspose(const cMatrix<Type, Cols, Rows> &M) {
	cMatrix<Type, Rows, Cols> R;
	for(unsigned row = 0; row < Rows; row++) {
		for(unsigned col = 0; col < Cols; col++) {
			R[i][j] = M[j][i];
		}
	}
	return R;
}

// Det : Type(const cMatrix<Type, 2, 2> &)
template<class Type>
inline Type Det(const cMatrix<Type, 2, 2> &M) {
	return M[0][0] * M[1][1] - M[0][1] * M[1][0];
}

// Det : Type(const cMatrix<Type, 3, 3> &)
template<class Type>
inline Type Det(const cMatrix<Type, 3, 3> &M) {
	Type d2_12_01 = M[1][0] * M[2][1] - M[1][1] * M[2][0];
	Type d2_12_02 = M[1][0] * M[2][2] - M[1][2] * M[2][0];
	Type d2_12_12 = M[1][1] * M[2][2] - M[1][2] * M[2][1];
	return M[0][0] * d2_12_12 - M[0][1] * d2_12_02 + M[0][2] * d2_12_01;
}

// Det : Type(const cMatrix<Type, 4, 4> &)
template<class Type>
inline Type Det(const cMatrix<Type, 4, 4> &M) {
	Type d2_01_01 = M[0][0] * M[1][1] - M[0][1] * M[1][0];
	Type d2_01_02 = M[0][0] * M[1][2] - M[0][2] * M[1][0];
	Type d2_01_03 = M[0][0] * M[1][3] - M[0][3] * M[1][0];
	Type d2_01_12 = M[0][1] * M[1][2] - M[0][2] * M[1][1];
	Type d2_01_13 = M[0][1] * M[1][3] - M[0][3] * M[1][1];
	Type d2_01_23 = M[0][2] * M[1][3] - M[0][3] * M[1][2];

	Type d3_012_012 = M[2][0] * d2_01_12 - M[2][1] * d2_01_02 + M[2][2] * d2_01_01;
	Type d3_012_013 = M[2][0] * d2_01_13 - M[2][1] * d2_01_03 + M[2][3] * d2_01_01;
	Type d3_012_023 = M[2][0] * d2_01_23 - M[2][2] * d2_01_03 + M[2][3] * d2_01_02;
	Type d3_012_123 = M[2][1] * d2_01_23 - M[2][2] * d2_01_13 + M[2][3] * d2_01_12;

	return - M[3][0] * d3_012_123 + M[3][1] * d3_012_023 - M[3][2] * d3_012_013 + M[3][3] * d3_012_012;
}

// Invert : bool(cMatrix<Type, 2, 2> &)
template<class Type>
inline bool Invert(cMatrix<Type, 2, 2> &M) {
	double Det = m[0][0] * m[1][1] - m[0][1] * m[1][0];

	if(fabs(Det) < Type(1e-14)) return false;
	double InvDet = 1.0 / Det;

	Type M00 = M[0][0];
	M[0][0] = M[1][1] * InvDet;
	M[0][1] = - M[0][1] * InvDet;
	M[1][0] = - M[1][0] * InvDet;
	M[1][1] = M00 * InvDet;

	return true;
}

// Invert : bool(cMatrix<Type, 3, 3> &)
template<class Type>
inline bool Invert(cMatrix<Type, 3, 3> &M) {
	Type d2_12_01 = M[1][0] * M[2][1] - M[1][1] * M[2][0];
	Type d2_12_02 = M[1][0] * M[2][2] - M[1][2] * M[2][0];
	Type d2_12_12 = M[1][1] * M[2][2] - M[1][2] * M[2][1];

	double Det =  M[0][0] * d2_12_12 - M[0][1] * d2_12_02 + M[0][2] * d2_12_01;

	if(fabs(Det) < 1e-14) return false;
	double InvDet = 1.0 / Det;

	Type d2_01_01 = M[0][0] * M[1][1] - M[0][1] * M[1][0];
	Type d2_01_02 = M[0][0] * M[1][2] - M[0][2] * M[1][0];
	Type d2_01_12 = M[0][1] * M[1][2] - M[0][2] * M[1][1];

	Type d2_02_01 = M[0][0] * M[2][1] - M[0][1] * M[2][0];
	Type d2_02_02 = M[0][0] * M[2][2] - M[0][2] * M[2][0];
	Type d2_02_12 = M[0][1] * M[2][2] - M[0][2] * M[2][1];

	M[0][0] = + d2_12_12 * InvDet;
	M[1][0] = - d2_12_02 * InvDet;
	M[2][0] = + d2_12_01 * InvDet;

	M[0][1] = - d2_02_12 * InvDet;
	M[1][1] = + d2_02_02 * InvDet;
	M[2][1] = - d2_02_01 * InvDet;

	M[0][2] = + d2_01_12 * InvDet;
	M[1][2] = - d2_01_02 * InvDet;
	M[2][2] = + d2_01_01 * InvDet;

	return true;
}

// Invert : bool(cMatrix<Type, 4, 4> &)
template<class Type>
inline bool Invert(cMatrix<Type, 4, 4> &M) {
	Type d2_01_01 = M[0][0] * M[1][1] - M[0][1] * M[1][0];
	Type d2_01_02 = M[0][0] * M[1][2] - M[0][2] * M[1][0];
	Type d2_01_03 = M[0][0] * M[1][3] - M[0][3] * M[1][0];
	Type d2_01_12 = M[0][1] * M[1][2] - M[0][2] * M[1][1];
	Type d2_01_13 = M[0][1] * M[1][3] - M[0][3] * M[1][1];
	Type d2_01_23 = M[0][2] * M[1][3] - M[0][3] * M[1][2];

	Type d3_012_012 = M[2][0] * d2_01_12 - M[2][1] * d2_01_02 + M[2][2] * d2_01_01;
	Type d3_012_013 = M[2][0] * d2_01_13 - M[2][1] * d2_01_03 + M[2][3] * d2_01_01;
	Type d3_012_023 = M[2][0] * d2_01_23 - M[2][2] * d2_01_03 + M[2][3] * d2_01_02;
	Type d3_012_123 = M[2][1] * d2_01_23 - M[2][2] * d2_01_13 + M[2][3] * d2_01_12;

	Type Det =  - M[3][0] * d3_012_123 + M[3][1] * d3_012_023 - M[3][2] * d3_012_013 + M[3][3] * d3_012_012;

	if(fabs(Det) < 1e-6) return false;
	Type InvDet = Type(1) / Det;

	Type d2_03_01 = M[0][0] * M[3][1] - M[0][1] * M[3][0];
	Type d2_03_02 = M[0][0] * M[3][2] - M[0][2] * M[3][0];
	Type d2_03_03 = M[0][0] * M[3][3] - M[0][3] * M[3][0];
	Type d2_03_12 = M[0][1] * M[3][2] - M[0][2] * M[3][1];
	Type d2_03_13 = M[0][1] * M[3][3] - M[0][3] * M[3][1];
	Type d2_03_23 = M[0][2] * M[3][3] - M[0][3] * M[3][2];

	Type d2_13_01 = M[1][0] * M[3][1] - M[1][1] * M[3][0];
	Type d2_13_02 = M[1][0] * M[3][2] - M[1][2] * M[3][0];
	Type d2_13_03 = M[1][0] * M[3][3] - M[1][3] * M[3][0];
	Type d2_13_12 = M[1][1] * M[3][2] - M[1][2] * M[3][1];
	Type d2_13_13 = M[1][1] * M[3][3] - M[1][3] * M[3][1];
	Type d2_13_23 = M[1][2] * M[3][3] - M[1][3] * M[3][2];

	Type d3_023_012 = M[2][0] * d2_03_12 - M[2][1] * d2_03_02 + M[2][2] * d2_03_01;
	Type d3_023_013 = M[2][0] * d2_03_13 - M[2][1] * d2_03_03 + M[2][3] * d2_03_01;
	Type d3_023_023 = M[2][0] * d2_03_23 - M[2][2] * d2_03_03 + M[2][3] * d2_03_02;
	Type d3_023_123 = M[2][1] * d2_03_23 - M[2][2] * d2_03_13 + M[2][3] * d2_03_12;

	Type d3_123_012 = M[2][0] * d2_13_12 - M[2][1] * d2_13_02 + M[2][2] * d2_13_01;
	Type d3_123_013 = M[2][0] * d2_13_13 - M[2][1] * d2_13_03 + M[2][3] * d2_13_01;
	Type d3_123_023 = M[2][0] * d2_13_23 - M[2][2] * d2_13_03 + M[2][3] * d2_13_02;
	Type d3_123_123 = M[2][1] * d2_13_23 - M[2][2] * d2_13_13 + M[2][3] * d2_13_12;

	Type d3_013_012 = M[3][0] * d2_01_12 - M[3][1] * d2_01_02 + M[3][2] * d2_01_01;
	Type d3_013_013 = M[3][0] * d2_01_13 - M[3][1] * d2_01_03 + M[3][3] * d2_01_01;
	Type d3_013_023 = M[3][0] * d2_01_23 - M[3][2] * d2_01_03 + M[3][3] * d2_01_02;
	Type d3_013_123 = M[3][1] * d2_01_23 - M[3][2] * d2_01_13 + M[3][3] * d2_01_12;

	M[0][0] = - d3_123_123 * InvDet;
	M[1][0] = + d3_123_023 * InvDet;
	M[2][0] = - d3_123_013 * InvDet;
	M[3][0] = + d3_123_012 * InvDet;

	M[0][1] = + d3_023_123 * InvDet;
	M[1][1] = - d3_023_023 * InvDet;
	M[2][1] = + d3_023_013 * InvDet;
	M[3][1] = - d3_023_012 * InvDet;

	M[0][2] = + d3_013_123 * InvDet;
	M[1][2] = - d3_013_023 * InvDet;
	M[2][2] = + d3_013_013 * InvDet;
	M[3][2] = - d3_013_012 * InvDet;

	M[0][3] = - d3_012_123 * InvDet;
	M[1][3] = + d3_012_023 * InvDet;
	M[2][3] = - d3_012_013 * InvDet;
	M[3][3] = + d3_012_012 * InvDet;

	return true;
}

// SetTransl : VECTOR & (VECTOR &, const MATRIX &)
template<class VECTOR_TYPE, unsigned VECTOR_SIZE, class MATRIX_TYPE, unsigned MATRIX_ROWS, unsigned MATRIX_COLS>
inline cVector<VECTOR_TYPE, VECTOR_SIZE> & SetTransl(cVector<VECTOR_TYPE, VECTOR_SIZE> &u, const cMatrix<MATRIX_TYPE, MATRIX_ROWS, MATRIX_COLS> &M) {
	assert((MATRIX_ROWS == MATRIX_COLS && (VECTOR_SIZE == MATRIX_COLS - 1 || VECTOR_SIZE == MATRIX_COLS)) ||
		(MATRIX_ROWS == MATRIX_COLS + 1 && (VECTOR_SIZE == MATRIX_COLS || VECTOR_SIZE == MATRIX_COLS + 1)));

	if((MATRIX_ROWS == MATRIX_COLS && VECTOR_SIZE == MATRIX_COLS) ||
		(MATRIX_ROWS == MATRIX_COLS + 1 && VECTOR_SIZE == MATRIX_COLS + 1)) {
			u[VECTOR_SIZE - 1] = (VECTOR_TYPE)1;
	}
	for(unsigned i = 0; i < MATRIX_ROWS - 1; i++) {
		u[i] = (VECTOR_TYPE)M[MATRIX_ROWS - 1][i];
	}
	return u;
}

// SetTransl : MATRIX & (MATRIX &, const VECTOR &)
template<class MATRIX_TYPE, unsigned MATRIX_ROWS, unsigned MATRIX_COLS, class VECTOR_TYPE, unsigned VECTOR_SIZE>
inline cMatrix<MATRIX_TYPE, MATRIX_ROWS, MATRIX_COLS> & SetTransl(cMatrix<MATRIX_TYPE, MATRIX_ROWS, MATRIX_COLS> &M, const cVector<VECTOR_TYPE, VECTOR_SIZE> &u) {
	assert((MATRIX_ROWS == MATRIX_COLS && (VECTOR_SIZE == MATRIX_COLS - 1 || VECTOR_SIZE == MATRIX_COLS)) ||
		(MATRIX_ROWS == MATRIX_COLS + 1 && (VECTOR_SIZE == MATRIX_COLS || VECTOR_SIZE == MATRIX_COLS + 1)));

	if((MATRIX_ROWS == MATRIX_COLS && VECTOR_SIZE == MATRIX_COLS) ||
		(MATRIX_ROWS == MATRIX_COLS + 1 && VECTOR_SIZE == MATRIX_COLS + 1)) {
			VECTOR_TYPE w = u[VECTOR_SIZE - 1];
			for(unsigned i = 0; i < MATRIX_ROWS - 1; i++) {
				M[MATRIX_ROWS - 1][i] = (MATRIX_TYPE)(u[i] / w);
			}
		} else {
			for(unsigned i = 0; i < MATRIX_ROWS - 1; i++) {
				M[MATRIX_ROWS - 1][i] = (MATRIX_TYPE)u[i];
			}
		}
	return M;
}

// SetScale : MATRIX & (MATRIX &, const VECTOR &)
template<class MATRIX_TYPE, unsigned MATRIX_ROWS, unsigned MATRIX_COLS, class VECTOR_TYPE, unsigned VECTOR_SIZE>
inline cMatrix<MATRIX_TYPE, MATRIX_ROWS, MATRIX_COLS> & SetScale(cMatrix<MATRIX_TYPE, MATRIX_ROWS, MATRIX_COLS> &M, const cVector<VECTOR_TYPE, VECTOR_SIZE> &u) {
	assert(((VECTOR_SIZE == MATRIX_ROWS - 1 && VECTOR_SIZE == MATRIX_COLS - 1) ||
		(VECTOR_SIZE == MATRIX_ROWS - 1 && VECTOR_SIZE == MATRIX_COLS) ||
		(VECTOR_SIZE == MATRIX_COLS - 1 && VECTOR_SIZE == MATRIX_ROWS)));

	for(unsigned d = 0; d < VECTOR_SIZE; d++) {
		M[d][d] = (MATRIX_TYPE)u[d];
	}
	return M;
}

// SetScale : MATRIX & (MATRIX &, const SCALAR)
template<class MATRIX_TYPE, unsigned MATRIX_ROWS, unsigned MATRIX_COLS, class SCALAR_TYPE>
inline cMatrix<MATRIX_TYPE, MATRIX_ROWS, MATRIX_COLS> & SetScale(cMatrix<MATRIX_TYPE, MATRIX_ROWS, MATRIX_COLS> &M, const SCALAR_TYPE s) {
	for(unsigned d = 0; d < Min(MATRIX_ROWS, MATRIX_COLS, Max(MATRIX_ROWS, MATRIX_COLS) - 1); d++) {
		M[d][d] = (MATRIX_TYPE)s;
	}
	return M;
}

// SetXRot : MATRIX & (MATRIX &, const SCALAR)
template<class MATRIX_TYPE, unsigned MATRIX_ROWS, unsigned MATRIX_COLS, class SCALAR_TYPE>
inline cMatrix<MATRIX_TYPE, MATRIX_ROWS, MATRIX_COLS> & SetXRot(cMatrix<MATRIX_TYPE, MATRIX_ROWS, MATRIX_COLS> &M, const SCALAR_TYPE Pitch) {
	assert(MATRIX_ROWS >= 3 && MATRIX_COLS >= 3 && MATRIX_ROWS <= 4 && MATRIX_COLS <= 4);

	Identity(M);
	MATRIX_TYPE C = cos((MATRIX_TYPE)Pitch), S = sin((MATRIX_TYPE)Pitch);
	// yAxis:
	M[1][1] = C;
	M[2][1] = - S;
	// zAxis:
	M[1][2] = S;
	M[2][2] = C;

	return M;
}

// SetYRot : MATRIX & (MATRIX &, const SCALAR)
template<class MATRIX_TYPE, unsigned MATRIX_ROWS, unsigned MATRIX_COLS, class SCALAR_TYPE>
inline cMatrix<MATRIX_TYPE, MATRIX_ROWS, MATRIX_COLS> & SetYRot(cMatrix<MATRIX_TYPE, MATRIX_ROWS, MATRIX_COLS> &M, const SCALAR_TYPE Yaw) {
	assert(MATRIX_ROWS >= 3 && MATRIX_COLS >= 3 && MATRIX_ROWS <= 4 && MATRIX_COLS <= 4);

	Identity(M);
	MATRIX_TYPE C = cos((MATRIX_TYPE)Yaw), S = sin((MATRIX_TYPE)Yaw);
	// xAxis:
	M[0][0] = C;
	M[2][0] = S;
	// zAxis:
	M[0][2] = - S;
	M[2][2] = C;

	return M;
}

// SetZRot : MATRIX & (MATRIX &, const SCALAR)
template<class MATRIX_TYPE, unsigned MATRIX_ROWS, unsigned MATRIX_COLS, class SCALAR_TYPE>
inline cMatrix<MATRIX_TYPE, MATRIX_ROWS, MATRIX_COLS> & SetZRot(cMatrix<MATRIX_TYPE, MATRIX_ROWS, MATRIX_COLS> &M, const SCALAR_TYPE Roll) {
	assert(MATRIX_ROWS >= 2 && MATRIX_COLS >= 2 && MATRIX_ROWS <= 4 && MATRIX_COLS <= 4);

	Identity(M);
	MATRIX_TYPE C = cos((MATRIX_TYPE)Roll), S = sin((MATRIX_TYPE)Roll);
	// xAxis:
	M[0][0] = C;
	M[1][0] = - S;
	// yAxis:
	M[0][1] = S;
	M[1][1] = C;

	return M;
}

// SetRot : MATRIX & (MATRIX &, const VECTOR3 &, const SCALAR)
template<class MATRIX_TYPE, unsigned MATRIX_ROWS, unsigned MATRIX_COLS, class VECTOR_TYPE, class SCALAR_TYPE>
inline cMatrix<MATRIX_TYPE, MATRIX_ROWS, MATRIX_COLS> & SetRot(cMatrix<MATRIX_TYPE, MATRIX_ROWS, MATRIX_COLS> &M, const cVector<VECTOR_TYPE, 3> &Axis, const SCALAR_TYPE Angle) {
	assert(MATRIX_ROWS >= 3 && MATRIX_COLS >= 3 && MATRIX_ROWS <= 4 && MATRIX_COLS <= 4);
	cVector<VECTOR_TYPE, 3> l(Axis);
	assert(l.Normalize() > (VECTOR_TYPE)0.001);

	MATRIX_TYPE S = sin((MATRIX_TYPE)Angle);
	MATRIX_TYPE C = cos((MATRIX_TYPE)Angle);
	MATRIX_TYPE t = MATRIX_TYPE(1) - C;
	MATRIX_TYPE x = l[Xelt], y = l[Yelt], z = l[Zelt];

	Identity(M);
	// xAxis:
	M[0][0] = (t * x * x) + C;
	M[1][0] = (t * x * y) - (S * z);
	M[2][0] = (t * x * z) + (S * y);
	// yAxis:
	M[0][1] = (t * x * y) + (S * z);
	M[1][1] = (t * y * y) + C;
	M[2][1] = (t * y * z) - (S * x);
	// zAxis:
	M[0][2] = (t * x * z) - (S * y);
	M[1][2] = (t * y * z) + (S * x);
	M[2][2] = (t * z * z) + C;

	return M;
}

/*

template<class Type>
inline cMatrix<Type, 4, 4> & SetLookAt(cMatrix<Type, 4, 4> &M, const cVector<Type, 3> &At, Type Azim, Type Elev, Type toEye) {
	cVector<Type, 3> Ref = MakeCartesian(Type(1), Azim, Elev);
	cVector<Type, 3> Up = MakeCartesian(Type(1), Azim, Elev - Rad((Type)45)) - Ref;
	cVector<Type, 3> zAxis = Normal(Ref);
	cVector<Type, 3> Eye = At + zAxis * toEye;
	cVector<Type, 3> xAxis = Normal(Cross(Up, zAxis));
	cVector<Type, 3> yAxis = Cross(zAxis, xAxis);

	inline float4x4 lookat(const float3 &At, float Azim, float Elev, float toEye) {
	float3 Ref = cartesian(1.0f, Azim, Elev);
	float3 Up = cartesian(1.0f, Azim, Elev - radians(45.0f)) - Ref;
	float3 zAxis = normalize(Ref);
	float3 Eye = At + zAxis * toEye;
	float3 xAxis = normalize(cross(Up, zAxis));
	float3 yAxis = cross(zAxis, xAxis);
	float4x4 L;
	L._m00 = xAxis.x,			L._m01 = yAxis.x,			L._m02 = zAxis.x,			L._m03 = 0.0f;
	L._m10 = xAxis.y,			L._m11 = yAxis.y,			L._m12 = zAxis.y,			L._m13 = 0.0f;
	L._m20 = xAxis.z,			L._m21 = yAxis.z,			L._m22 = zAxis.z,			L._m23 = 0.0f;
	L._m30 = - dot(xAxis, Eye),	L._m31 = - dot(yAxis, Eye),	L._m32 = - dot(zAxis, Eye),	L._m33 = 1.0f;
	return L;
}*/
/*
// SetOrtho : cMatrix<T, 4, 4> & (cMatrix<T, 4, 4> &, T Left, T Right, T Bottom, T Top, T zNear, T zFar)
template<class T>
inline cMatrix<T, 4, 4> & SetOrtho(cMatrix<T, 4, 4> &M, T Left, T Right, T Bottom, T Top, T zNear, T zFar) {
	Zero(M);
	T irl = T(1) / (Right - Left);
	T itp = T(1) / (Top - Bottom);
	T ifn = T(1) / (zFar - zNear);

	M[0][0] = T(2) * irl;
	M[1][1] = T(2) * itb;
	M[2][2] = - T(2) * ifn;

	M[3][0] = - (Right + Left) * irl;
	M[3][1] = - (Top + Bottom) * itb;
	M[3][2] = - (zFar + zNear) * ifn;
	M[3][3] = - (T)1;

	return M;
}

// SetFrustum : cMatrix<T, 4, 4> & (cMatrix<T, 4, 4> &, T Left, T Right, T Bottom, T Top, T zNear, T zFar)
template<class T>
inline cMatrix<T, 4, 4> & SetFrustum(cMatrix<T, 4, 4> &M, T Left, T Right, T Bottom, T Top, T zNear, T zFar) {
	Zero(M);
	M[0][0] = (T(2) * zNear) / (Right - Left);
	M[1][1] = (T(2) * zNear) / (Top - Bottom);
	M[2][0] = (Right + Left) / (Right - Left);
	M[2][1] = (Top + Bottom) / (Top - Bottom);
	M[2][2] = - (zFar + zNear) / (zFar - zNear);
	M[2][3] = - T(1);
	M[3][2] = - (T(2) * zFar * zNear) / (zFar - zNear);
	return M;
}

// SetPerspective : cMatrix<T, 4, 4> & (cMatrix<T, 4, 4> &, T Fov, T AspectWtoH, T zNear, T zFar)
template<class T>
inline cMatrix<T, 4, 4> & SetPerspective(cMatrix<T, 4, 4> &M, T Fov, T AspectWtoH, T zNear, T zFar) {
	T h = tan(T(0.5) * Rad(Fov));
	T Top = h * zNear;
	T Right = Top * AspectWtoH;
	return SetFrustum(M, - Right, Right, - Top, Top, zNear, zFar);
}
*/

} // comms