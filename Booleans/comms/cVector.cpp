#include "cVector.h"
#include "cMatrix.h"
#include "cStr.h"

namespace comms {

const cVector cVector::MinValue(DoubleMin, DoubleMin, DoubleMin);
const cVector cVector::MaxValue(DoubleMax, DoubleMax, DoubleMax);
const cVector cVector::Origin(0., 0., 0.);
const cVector cVector::Origin1D(0.);
const cVector cVector::Origin2D(0., 0.);
const cVector cVector::xAxis(1., 0., 0.);
const cVector cVector::xAxis2D(1., 0.);
const cVector cVector::yAxis(0., 1., 0.);
const cVector cVector::yAxis2D(0., 1.);
const cVector cVector::zAxis(0., 0., 1.);
const cVector cVector::Unit(1., 1., 1.);

//-----------------------------------------------------------------------------
// operator << : cStr & (cStr &, const cVector &)
//-----------------------------------------------------------------------------
cStr & operator << (cStr &Str, const cVector &u) {
	Str += "[";
	for(int i = 0; i < u.Dim(); i++) {
		if(i) Str += ", ";
		Str += u[i];
	}
	Str += "]";
	return Str;
} // operator << : cStr & (cStr &, const cVector &)

} // comms