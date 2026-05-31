#include "comms.h"

const dVector dVector::MinValue(cMath::DoubleMinValue, cMath::DoubleMinValue, cMath::DoubleMinValue);
const dVector dVector::MaxValue(cMath::DoubleMaxValue, cMath::DoubleMaxValue, cMath::DoubleMaxValue);
const dVector dVector::Origin(0., 0., 0.);
const dVector dVector::Origin1D(0.);
const dVector dVector::Origin2D(0., 0.);
const dVector dVector::xAxis(1., 0., 0.);
const dVector dVector::xAxis2D(1., 0.);
const dVector dVector::yAxis(0., 1., 0.);
const dVector dVector::yAxis2D(0., 1.);
const dVector dVector::zAxis(0., 0., 1.);
const dVector dVector::Unit(1., 1., 1.);

//-----------------------------------------------------------------------------
// dVector::UpdateBoundBox : void(dVector &, dVector &) const
//-----------------------------------------------------------------------------
void dVector::UpdateBoundBox(dVector &bbMin, dVector &bbMax) const {
	bbMin.x = cMath::Min(bbMin.x, x);
	bbMin.y = cMath::Min(bbMin.y, y);
	bbMin.z = cMath::Min(bbMin.z, z);
	bbMax.x = cMath::Max(bbMax.x, x);
	bbMax.y = cMath::Max(bbMax.y, y);
	bbMax.z = cMath::Max(bbMax.z, z);
}

//-----------------------------------------------------------------------------
// operator << : cStr & (cStr &, const dVector &)
//-----------------------------------------------------------------------------
cStr & operator << (cStr &Str, const dVector &u) {
	Str += "[";
	for(int i = 0; i < u.Dim(); i++) {
		if(i) Str += ", ";
		Str += u[i];
	}
	Str += "]";
	return Str;
} // operator << : cStr & (cStr &, const dVector &)