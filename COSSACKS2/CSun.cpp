#include "CSun.h"

const float CSun::c_defk = 0.4f;

// Direction to the sun:
#define xSunDir	2.0f
#define ySunDir	1.0f
#define zSunDir	0.8f

// Radius:
#define r		0.04f

// Performing calculations:
#define lSunDir	sqrt(xSunDir * xSunDir + ySunDir * ySunDir + zSunDir * zSunDir)
#define xDir	(xSunDir / lSunDir)
#define yDir	(ySunDir / lSunDir)
#define zDir	(zSunDir / lSunDir)

// Variations:
#define vx0		0.0f
#define vy0		0.0f
#define vx1		r
#define vy1		0.0f
#define vx2		(0.707f * r)
#define vy2		(0.707f * r)
#define vx3		0.0f
#define vy3		r
#define vx4		(-0.707f * r)
#define vy4		(0.707f * r)
#define vx5		-r
#define vy5		0.0f
#define vx6		(-0.707f * r)
#define	vy6		(-0.707f * r)
#define	vx7		0.0f
#define	vy7		-r
#define vx8		(0.707f * r)
#define	vy8		(-0.707f * r)

// (xXAxis, yXAxis, zXAxis), (xYAxis, yYAxis, zYAxis), (xZAxis, yZAxis, zZAxis)
#define xZAxis	xDir
#define yZAxis	yDir
#define zZAxis	zDir
#define xCross(ux, uy, uz, vx, vy, vz)	(uy * vz - uz * vy)
#define yCross(ux, uy, uz, vx, vy, vz)	(uz * vx - ux * vz)
#define zCross(ux, uy, uz, vx, vy, vz)	(ux * vy - uy * vx)
#define xXAxis	xCross(xZAxis, yZAxis, zZAxis, 0.0f, 0.0f, 1.0f)
#define yXAxis	yCross(xZAxis, yZAxis, zZAxis, 0.0f, 0.0f, 1.0f)
#define zXAxis	zCross(xZAxis, yZAxis, zZAxis, 0.0f, 0.0f, 1.0f)
#define xYAxis	xCross(xZAxis, yZAxis, zZAxis, xXAxis, yXAxis, zXAxis)
#define yYAxis	yCross(xZAxis, yZAxis, zZAxis, xXAxis, yXAxis, zXAxis)
#define zYAxis	zCross(xZAxis, yZAxis, zZAxis, xXAxis, yXAxis, zXAxis)

// e00(xXAxis)  e01(xYAxis)  e02(xZAxis)
// e10(yXAxis)  e11(yYAxis)  e12(yZAxis)
// e20(zXAxis)  e21(zYAxis)  e22(zZAxis)
#define e00	xXAxis
#define e01	xYAxis
#define e02	xZAxis
#define e10	yXAxis
#define e11 yYAxis
#define e12 yZAxis
#define e20	zXAxis
#define e21	zYAxis
#define e22 zZAxis

// i00	i01	i02
// i10	i11	i12
// i20	i21	i22
#define D	(e00 * (e11 * e22 - e12 * e21) - e01 * (e10 * e22 - e12 * e20) + e02 * (e10 * e21 - e11 * e20))
#define	i00	((e11 * e22 - e12 * e21) / D)
#define	i01	((e02 * e21 - e01 * e22) / D)
#define	i02	((e01 * e12 - e02 * e11) / D)
#define i10	((e12 * e20 - e10 * e22) / D)
#define i11	((e00 * e22 - e02 * e20) / D)
#define i12	((e02 * e10 - e00 * e12) / D)
#define i20	((e10 * e21 - e11 * e20) / D)
#define i21	((e01 * e20 - e00 * e21) / D)
#define i22	((e00 * e11 - e01 * e10) / D)

// Dir0, Dir1, Dir2, Dir3, Dir4, Dir5, Dir6, Dir7, Dir8
#define	Dot(ux, uy, uz, vx, vy, vz)	(ux * vx + uy * vy + uz * vz)
#define	sx0	(Dot(vx0, vy0, 0.0f, i00, i10, i20) + xDir)
#define	sy0	(Dot(vx0, vy0, 0.0f, i01, i11, i21) + yDir)
#define	sz0	(Dot(vx0, vy0, 0.0f, i02, i12, i22) + zDir)

#define	sx1	(Dot(vx1, vy1, 0.0f, i00, i10, i20) + xDir)
#define	sy1	(Dot(vx1, vy1, 0.0f, i01, i11, i21) + yDir)
#define	sz1	(Dot(vx1, vy1, 0.0f, i02, i12, i22) + zDir)

#define	sx2	(Dot(vx2, vy2, 0.0f, i00, i10, i20) + xDir)
#define	sy2	(Dot(vx2, vy2, 0.0f, i01, i11, i21) + yDir)
#define	sz2	(Dot(vx2, vy2, 0.0f, i02, i12, i22) + zDir)

#define	sx3	(Dot(vx3, vy3, 0.0f, i00, i10, i20) + xDir)
#define	sy3	(Dot(vx3, vy3, 0.0f, i01, i11, i21) + yDir)
#define	sz3	(Dot(vx3, vy3, 0.0f, i02, i12, i22) + zDir)

#define	sx4	(Dot(vx4, vy4, 0.0f, i00, i10, i20) + xDir)
#define	sy4	(Dot(vx4, vy4, 0.0f, i01, i11, i21) + yDir)
#define	sz4	(Dot(vx4, vy4, 0.0f, i02, i12, i22) + zDir)

#define	sx5	(Dot(vx5, vy5, 0.0f, i00, i10, i20) + xDir)
#define	sy5	(Dot(vx5, vy5, 0.0f, i01, i11, i21) + yDir)
#define	sz5	(Dot(vx5, vy5, 0.0f, i02, i12, i22) + zDir)

#define	sx6	(Dot(vx6, vy6, 0.0f, i00, i10, i20) + xDir)
#define	sy6	(Dot(vx6, vy6, 0.0f, i01, i11, i21) + yDir)
#define	sz6	(Dot(vx6, vy6, 0.0f, i02, i12, i22) + zDir)

#define	sx7	(Dot(vx7, vy7, 0.0f, i00, i10, i20) + xDir)
#define	sy7	(Dot(vx7, vy7, 0.0f, i01, i11, i21) + yDir)
#define	sz7	(Dot(vx7, vy7, 0.0f, i02, i12, i22) + zDir)

#define	sx8	(Dot(vx8, vy8, 0.0f, i00, i10, i20) + xDir)
#define	sy8	(Dot(vx8, vy8, 0.0f, i01, i11, i21) + yDir)
#define	sz8	(Dot(vx8, vy8, 0.0f, i02, i12, i22) + zDir)

#define	l0	sqrt(sx0 * sx0 + sy0 * sy0 + sz0 * sz0)
#define	l1	sqrt(sx1 * sx1 + sy1 * sy1 + sz1 * sz1)
#define	l2	sqrt(sx2 * sx2 + sy2 * sy2 + sz2 * sz2)
#define	l3	sqrt(sx3 * sx3 + sy3 * sy3 + sz3 * sz3)
#define	l4	sqrt(sx4 * sx4 + sy4 * sy4 + sz4 * sz4)
#define	l5	sqrt(sx5 * sx5 + sy5 * sy5 + sz5 * sz5)
#define	l6	sqrt(sx6 * sx6 + sy6 * sy6 + sz6 * sz6)
#define	l7	sqrt(sx7 * sx7 + sy7 * sy7 + sz7 * sz7)
#define	l8	sqrt(sx8 * sx8 + sy8 * sy8 + sz8 * sz8)

#define	Dir0	Vector3D(sx0 / l0, sy0 / l0, sz0 / l0)
#define	Dir1	Vector3D(sx1 / l1, sy1 / l1, sz1 / l1)
#define	Dir2	Vector3D(sx2 / l2, sy2 / l2, sz2 / l2)
#define	Dir3	Vector3D(sx3 / l3, sy3 / l3, sz3 / l3)
#define	Dir4	Vector3D(sx4 / l4, sy4 / l4, sz4 / l4)
#define	Dir5	Vector3D(sx5 / l5, sy5 / l5, sz5 / l5)
#define	Dir6	Vector3D(sx6 / l6, sy6 / l6, sz6 / l6)
#define	Dir7	Vector3D(sx7 / l7, sy7 / l7, sz7 / l7)
#define	Dir8	Vector3D(sx8 / l8, sy8 / l8, sz8 / l8)

const Vector3D CSun::c_defSources[] = {Dir0, Dir1, Dir2, Dir3, Dir4, Dir5, Dir6, Dir7, Dir8};

#undef	xSunDir
#undef	ySunDir
#undef	zSunDir
#undef	r

#undef	vx0
#undef	vy0
#undef	vx1
#undef	vy1
#undef	vx2
#undef	vy2
#undef	vx3
#undef	vy3
#undef	vx4
#undef	vy4
#undef	vx5
#undef	vy5
#undef	vx6
#undef	vy6
#undef	vx7
#undef	vy7
#undef	vx8
#undef	vy8

#undef	lSunDir
#undef	xDir
#undef	yDir
#undef	zDir
#undef	xCross
#undef	yCross
#undef	zCross
#undef	xXAxis
#undef	yXAxis
#undef	zXAxis
#undef	xYAxis
#undef	yYAxis
#undef	zYAxis
#undef	xZAxis
#undef	yZAxis
#undef	zZAxis
#undef	e00
#undef	e01
#undef	e02
#undef	e10
#undef	e11
#undef	e12
#undef	e20
#undef	e21
#undef	e22
#undef	D
#undef	i00
#undef	i01
#undef	i02
#undef	i10
#undef	i11
#undef	i12
#undef	i20
#undef	i21
#undef	i22
#undef	Dot
#undef	sx0
#undef	sy0
#undef	sz0
#undef	sx1
#undef	sy1
#undef	sz1
#undef	sx2
#undef	sy2
#undef	sz2
#undef	sx3
#undef	sy3
#undef	sz3
#undef	sx4
#undef	sy4
#undef	sz4
#undef	sx5
#undef	sy5
#undef	sz5
#undef	sx6
#undef	sy6
#undef	sz6
#undef	sx7
#undef	sy7
#undef	sz7
#undef	sx8
#undef	sy8
#undef	sz8
#undef	l0
#undef	l1
#undef	l2
#undef	l3
#undef	l4
#undef	l5
#undef	l6
#undef	l7
#undef	l8
#undef	Dir0
#undef	Dir1
#undef	Dir2
#undef	Dir3
#undef	Dir4
#undef	Dir5
#undef	Dir6
#undef	Dir7
#undef	Dir8