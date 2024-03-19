#include "comms.h"

namespace comms {

// cVector<Type, Dim>::ToStr : cStr(int) const
template<class Type, int Dim>
cStr cVector<Type, Dim>::ToStr(int nPrec) const {
	return cStr("[") + cStr::FromArray(ToPtr(), Dim, nPrec, ", ") + cStr("]");
}

// operator << : cStr & (cStr &, const cVector<Type, Dim> &)
template<class Type, int Dim>
cStr & operator << (cStr &Str, const cVector<Type, Dim> &u) {
	Str += u.ToStr();
	return Str;
}

} // comms