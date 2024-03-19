#ifndef __cUtils__
#define __cUtils__

#include "cList.h"
#include "math.h"

namespace comms {

typedef cList<int> ints;
typedef cList<float> floats;
typedef cList<double> doubles;
typedef cList<DWORD> DWORDS;
typedef cList<bool> bools;

const double cPi = 3.1415926535897932384626433832795;
const double c2Pi = 6.283185307179586476925286766559;
const double cPi2 = 1.5707963267948966192313216916398;
const double cPi180 = 0.017453292519943295769236907684886;
const double c180Pi = 57.295779513082320876798154814105;

inline double Rad(double Deg) { return Deg * cPi180; }
inline double Deg(double Rad) { return Rad * c180Pi; }

inline void AngleNormalize2Pi(double &Angle) { // 0 <= Angle < 2Pi
	if(Angle >= c2Pi) Angle -= c2Pi;
	if(Angle < 0.) Angle += c2Pi;
}

inline void AngleNormalizePi(double &Angle) { // - Pi < Angle <= Pi
	AngleNormalize2Pi(Angle);
	if(Angle > cPi) Angle -= c2Pi;
}

const double DoubleEpsilon = double(0x1); // 4.9406564584124654e-324
const double DoubleMin = double(-1.7976931348623157e+308);
const double DoubleMax = double(1.7976931348623157e+308);
const double DoubleNaN = double(0xFFF8000000000000); // -1.#IND
const double DoubleNegativeInfinity = double(0xFFF0000000000000); // -1.#INF
const double DoublePositiveInfinity = double(0x7FF0000000000000); // 1.#INF

const int IntMin = int(0x80000000);
const int IntMax = int(0x7FFFFFFF);

const short ShortMin = short(0x8000);
const short ShortMax = short(0x7FFF);

const COLORREF cBlack = 0xff000000;
const COLORREF cGray75 = 0xff404040;
const COLORREF cGray50 = 0xff808080;
const COLORREF cGray25 = 0xffc0c0c0;
const COLORREF cWhite = 0xffffffff;
const COLORREF cRed = 0xff0000ff;
const COLORREF cGreen = 0xff00ff00;
const COLORREF cBlue = 0xffff0000;
const COLORREF cCyan = 0xffffff00;
const COLORREF cMagenta = 0xffff00ff;
const COLORREF cYellow = 0xff00ffff;

const DWORD argbRed = 0xffff0000;
const DWORD argbGreen = 0xff00ff00;
const DWORD argbYellow = 0xffffff00;
const DWORD argbBlue = 0xff0000ff;
const DWORD argbCyan = 0xff00ffff;
const DWORD argbMagenta = 0xffff00ff;
const DWORD argbWhite = 0xffffffff;
const DWORD argbGray = 0xff808080;
const DWORD argbBlack = 0xff000000;

const DWORD argbColorPalette[] = {argbRed, argbGreen, argbYellow, argbBlue, argbCyan,
argbMagenta, argbWhite, argbGray, argbBlack};

template <class T> inline void Swap(T &x, T &y) { T z = x; x = y; y = z; }

template <class T> inline T Square(T x) { return x * x; }
template <class T> inline T Cube(T x) { return x * x * x; }

template <class T> inline T Sign(T x) { return x > 0 ? 1 : (x < 0 ? -1 : 0); }

template <class T> inline T Max(T x, T y) { return x > y ? x : y; }
template <class T> inline T Min(T x, T y) { return x < y ? x : y; }
template <class T> inline int MaxIndex(T x, T y) { return x > y ? 0 : 1; }
template <class T> inline int MinIndex(T x, T y) { return x < y ? 0 : 1; }

template <class T> inline T Max(T x, T y, T z) { return x > y ? (x > z ? x : z) : (y > z ? y : z); }
template <class T> inline T Min(T x, T y, T z) { return x < y ? (x < z ? x : z) : (y < z ? y : z); }
template <class T> inline int MaxIndex(T x, T y, T z) { return x > y ? (x > z ? 0 : 2) : (y > z ? 1 : 2); }
template <class T> inline int MinIndex(T x, T y, T z) { return x < y ? (x < z ? 0 : 2) : (y < z ? 1 : 2); }

template <class T> inline bool IsAmong(T v, T x, T y) { return v == x || v == y; }
template <class T> inline bool IsAmong(T v, T x, T y, T z) { return v == x || v == y || v == z; }

template <class T> inline bool RangeCheck(T value, T Lo, T Hi) {
	if(Lo > Hi) Swap(Lo, Hi);
	return value >= Lo && value <= Hi;
}

template <class T> inline T Clamp(T &value, T Lo, T Hi) {
	assert(Lo < Hi);
	if(value < Lo) return value = Lo;
	if(value > Hi) return value = Hi;
	return value;
}

template <class T> inline T Clamp(const T &value, T Lo, T Hi) {
	assert(Lo < Hi);
	if(value < Lo) return Lo;
	if(value > Hi) return Hi;
	return value;
}

const double kEpsilon = 1e-6;

class cUtils {
public:
	static bool IsZero(double value, double Epsilon = kEpsilon);
	static bool NotZero(double value, double Epsilon = kEpsilon);
	static bool AreEquals(double x, double y, double Epsilon = kEpsilon);
	static int CompareZero(double value, double Epsilon = kEpsilon);
	static int Compare(double x, double y, double Epsilon = kEpsilon);
};

// cUtils::IsZero
inline bool cUtils::IsZero(double value, double Epsilon) {
	return value > - Epsilon && value < Epsilon;
} // cUtils::IsZero

// cUtils::NotZero
inline bool cUtils::NotZero(double value, double Epsilon) {
	return !IsZero(value, Epsilon);
} // cUtils::NotZero

// cUtils::AreEquals
inline bool cUtils::AreEquals(double x, double y, double Epsilon) {
	return IsZero(x - y, Epsilon);
} // cUtils::AreEquals

// cUtils::CompareZero
inline int cUtils::CompareZero(double value, double Epsilon) {
	return IsZero(value, Epsilon) ? 0 : (value > 0.0 ? 1 : - 1);
} // cUtisl::CompareZero

// cUtils::Compare
inline int cUtils::Compare(double x, double y, double Epsilon) {
	return IsZero(x - y, Epsilon) ? 0 : (x > y ? 1 : -1);
} // cUtils::Compare

// Is value Val within epsilon neighborhood about interval (Lo - Hi)?
inline bool IsWithin(double Val, double Lo, double Hi, double Eps = kEpsilon) {
	double L = Lo, H = Hi;
	if(L > H) Swap(L, H);
	L -= Eps;
	H += Eps;
	return Val > L && Val < H;
}

/*inline bool BoundCheck(int Num, int Lo, int Hi) {
	int L = Lo, H = Hi;
	if(L > H) Swap(L, H);
	return Num > L && Num < H;
}*/


// Is value x a power of two?
inline bool IsPowerOfTwo(int x) {
	return ( x & ( x - 1 ) ) == 0 && x > 0;
}

} // comms

#endif // __cUtils__