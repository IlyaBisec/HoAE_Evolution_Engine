#include "comms.h"

// cVec<Type, Dim>::ToStr : cStr(int) const
template<class Type, int Dim>
cStr cVec<Type, Dim>::ToStr(int nPrec) const {
	return cStr("[") + cStr::FromArray(ToPtr(), Dim, nPrec, ", ") + cStr("]");
}

// operator << : cStr & (cStr &, const cVec<Type, Dim> &)
template<class Type, int Dim>
cStr & operator << (cStr &Str, const cVec<Type, Dim> &u) {
	Str += u.ToStr();
	return Str;
}