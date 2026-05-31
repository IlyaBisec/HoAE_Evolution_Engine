#include "comms.h"

namespace comms {

/*
// Det : Type(const cMatrix<Type, 3, 3> &)
template<class Type>
Type Det(const cMatrix<Type, 3, 3> &M) {
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
}*/


} // comms