#ifndef __cUtils__
#define __cUtils__

#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

#pragma pack(push)
#pragma pack(1)

namespace comms {

const double cPi = 3.1415926535897932384626433832795;
const double c2Pi = 6.283185307179586476925286766559;
const double cPi2 = 1.5707963267948966192313216916398;
const double cPi180 = 0.017453292519943295769236907684886;
const double c180Pi = 57.295779513082320876798154814105;

inline double Rad(double Deg) { return Deg * cPi180; }
inline double Deg(double Rad) { return Rad * c180Pi; }


inline double AngleNormalize2Pi(double Angle) { // 0 <= Angle < 2Pi
	if(Angle >= c2Pi) Angle -= c2Pi;
	if(Angle < 0.) Angle += c2Pi;
	return Angle;
}

inline double AngleNormalizePi(double Angle) { // - Pi < Angle <= Pi
	Angle = AngleNormalize2Pi(Angle);
	if(Angle > cPi) Angle -= c2Pi;
	return Angle;
}

namespace Double {
	const double Epsilon = double(0x1); // 4.9406564584124654e-324
	const double MinValue = double(-1.7976931348623157e+308);
	const double MaxValue = double(1.7976931348623157e+308);
	const double NaN = double(0xfff8000000000000); // -1.#IND
	const double NegativeInfinity = double(0xfff0000000000000); // -1.#INF
	const double PositiveInfinity = double(0x7ff0000000000000); // 1.#INF
	inline double Lerp(double d0, double d1, double t) { return d0 + t * (d1 - d0); }
}

namespace Float {
	const float MaxValue = 3.402823466e+38f;
	const float MinValue = 1.175494351e-38f;
	inline float Lerp(float f0, float f1, float t) { return f0 + t * (f1 - f0); }
}

namespace Short {
	const short MinValue = (short)0x8000;
	const short MaxValue = (short)0x7fff;
}

namespace Int {
	const int MinValue = int(0x80000000);
	const int MaxValue = int(0x7fffffff);
}

template <class T> inline void Swap(T &x, T &y) { T z = x; x = y; y = z; }

template <class T> inline T Square(T x) { return x * x; }
template <class T> inline T Cube(T x) { return x * x * x; }

template <class T> inline T Sign(T x) { return x > 0 ? (T)1 : (x < 0 ? -(T)1 : (T)0); }

template <class T> inline T Max(T x, T y) { return x > y ? x : y; }
template <class T> inline T Min(T x, T y) { return x < y ? x : y; }
template <class T> inline int MaxIndex(T x, T y) { return x > y ? 0 : 1; }
template <class T> inline int MinIndex(T x, T y) { return x < y ? 0 : 1; }

template <class T> inline T Max(T x, T y, T z) { return x > y ? (x > z ? x : z) : (y > z ? y : z); }
template <class T> inline T Min(T x, T y, T z) { return x < y ? (x < z ? x : z) : (y < z ? y : z); }
template <class T> inline int MaxIndex(T x, T y, T z) { return x > y ? (x > z ? 0 : 2) : (y > z ? 1 : 2); }
template <class T> inline int MinIndex(T x, T y, T z) { return x < y ? (x < z ? 0 : 2) : (y < z ? 1 : 2); }

template <class T> inline bool RangeCheck(T value, T Lo, T Hi) {
	if(Lo > Hi) Swap(Lo, Hi);
	return value >= Lo && value <= Hi;
}
template <class T> inline T Clamp(T &value, T Lo, T Hi) {
	assert(Lo <= Hi);
	if(value < Lo) return value = Lo;
	if(value > Hi) return value = Hi;
	return value;
}

template <class T> inline T Clamp(const T &value, T Lo, T Hi) {
	assert(Lo <= Hi);
	if(value < Lo) return Lo;
	if(value > Hi) return Hi;
	return value;
}

const double kEpsilon = 1.0e-6;

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

//namespace Double {
	
//}
//using Double::Lerp;

class cMath {
public:
	static const float Pi;
	static const float TwoPi;
	static const float HalfPi;
	static const float QuarterPi;

	static const float RadsPerDeg;
	static const float DegsPerRad;

	static const float Epsilon;
	static const float SpaceEpsilon;

	static float Rad(float Deg);
	static float Deg(float Rad);

	static bool IsZero(const float f, const float Eps = Epsilon);
	static bool IsOne(const float f, const float Eps = Epsilon);
	static bool Equals(const float x, const float y, const float Eps = Epsilon);
	static bool IsValid(const float f);

	static float Clamp(const float s, const float Lo, const float Hi);
	static float Lerp(const float a, const float b, const float s);

	static float Abs(float x);
	static float Sqrt(float x);
	static float Sin(float a);
	static float Cos(float a);
	static float Tan(float a);
	static float ASin(float x);
	static float ACos(float x);
	static float ATan(float x);
	static float Pow(float x, float y);
	static float Exp(float x);
	static float Log(float x);

	static float AngleNormalize2Pi(float Angle); // 0 <= Angle < TwoPi
	static float AngleNormalizePi(float Angle); // - Pi < Angle <= Pi
	static float AngleDelta(float Alpha, float Beta);

	static int CalcPowerOfTwo(int x);
	static bool IsPowerOfTwo(int x);
};

//-----------------------------------------------------------------------------
// cMath::Rad : float(float)
//-----------------------------------------------------------------------------
inline float cMath::Rad(float Deg) {
	return Deg * RadsPerDeg;
}

//-----------------------------------------------------------------------------
// cMath::Deg : float(float)
//-----------------------------------------------------------------------------
inline float cMath::Deg(float Rad) {
	return Rad * DegsPerRad;
}

//-----------------------------------------------------------------------------
// cMath::IsZero : bool(const float, const float)
//-----------------------------------------------------------------------------
inline bool cMath::IsZero(const float f, const float Eps) {
	return fabs(f) <= Eps;
}

//-----------------------------------------------------------------------------
// cMath::IsOne : bool(const float, const float)
//-----------------------------------------------------------------------------
inline bool cMath::IsOne(const float f, const float Eps) {
	return fabs(f - 1.0f) <= Eps;
}

//-----------------------------------------------------------------------------
// cMath::Equals : bool(const float, const float, const float)
//-----------------------------------------------------------------------------
inline bool cMath::Equals(const float x, const float y, const float Eps) {
	return fabs(x - y) <= Eps;
}

//-----------------------------------------------------------------------------
// cMath::IsValid : bool(const float)
//-----------------------------------------------------------------------------
inline bool cMath::IsValid(const float f) {
	return f >= -Float::MaxValue && f <= Float::MinValue;
}

//-----------------------------------------------------------------------------
// cMath::Clamp : float(const float, const float, const float)
//-----------------------------------------------------------------------------
inline float cMath::Clamp(const float s, const float Lo, const float Hi) {
	return Min(Max(s, Lo), Hi);
}

//-----------------------------------------------------------------------------
// cMath::Lerp : float(const float, const float, const float)
//-----------------------------------------------------------------------------
inline float cMath::Lerp(const float a, const float b, const float s) {
	return a + s * (b - a);
}

//-----------------------------------------------------------------------------
// cMath::Abs : float(float)
//-----------------------------------------------------------------------------
inline float cMath::Abs(float x) {
	return fabs(x);
}

//-----------------------------------------------------------------------------
// cMath::Sqrt : float(float)
//-----------------------------------------------------------------------------
inline float cMath::Sqrt(float x) {
	assert(x >= 0.0f);
	return sqrtf(x);
}

//-----------------------------------------------------------------------------
// cMath::Sin : float(float)
//-----------------------------------------------------------------------------
inline float cMath::Sin(float a) {
	return sinf(a);
}

//-----------------------------------------------------------------------------
// cMath::Cos : float(float)
//-----------------------------------------------------------------------------
inline float cMath::Cos(float a) {
	return cosf(a);
}

//-----------------------------------------------------------------------------
// cMath::Tan : float(float)
//-----------------------------------------------------------------------------
inline float cMath::Tan(float a) {
	return tanf(a);
}

//-----------------------------------------------------------------------------
// cMath::ASin : float(float)
//-----------------------------------------------------------------------------
inline float cMath::ASin(float x) {
	if(x <= -1.0f) {
		return - HalfPi;
	}
	if(x >= 1.0f) {
		return HalfPi;
	}
	return asinf(x);
}

//-----------------------------------------------------------------------------
// cMath::ACos : float(float)
//-----------------------------------------------------------------------------
inline float cMath::ACos(float x) {
	if(x <= -1.0f) {
		return Pi;
	}
	if(x >= 1.0f) {
		return 0.0f;
	}
	return acosf(x);
}

//-----------------------------------------------------------------------------
// cMath::ATan : float(float)
//-----------------------------------------------------------------------------
inline float cMath::ATan(float a) {
	return atanf(a);
}

//-----------------------------------------------------------------------------
// cMath::Pow : float(float, float)
//-----------------------------------------------------------------------------
inline float cMath::Pow(float x, float y) {
	return powf(x, y);
}

//-----------------------------------------------------------------------------
// cMath::Exp : float(float)
//-----------------------------------------------------------------------------
inline float cMath::Exp(float x) {
	return expf(x);
}

//-----------------------------------------------------------------------------
// cMath::Log : float(float)
//-----------------------------------------------------------------------------
inline float cMath::Log(float x) {
	return logf(x);
}

//-----------------------------------------------------------------------------
// cMath::AngleNormalize2Pi : float(float)
//-----------------------------------------------------------------------------
inline float cMath::AngleNormalize2Pi(float Angle) { // 0 <= Angle < TwoPi
	while(Angle >= TwoPi) Angle -= TwoPi;
	while(Angle < 0.0f) Angle += TwoPi;
	return Angle;
}

//-----------------------------------------------------------------------------
// cMath::AngleNormalizePi : float(float)
//-----------------------------------------------------------------------------
inline float cMath::AngleNormalizePi(float Angle) { // - Pi < Angle <= Pi
	Angle = AngleNormalize2Pi(Angle);
	if(Angle > Pi) Angle -= TwoPi;
	return Angle;
}

//-----------------------------------------------------------------------------
// cMath::AngleDelta : float(float, float)
//-----------------------------------------------------------------------------
inline float cMath::AngleDelta(float Alpha, float Beta) {
	return AngleNormalizePi(Alpha - Beta);
}

//-----------------------------------------------------------------------------
// cMath::CalcPowerOfTwo : int(int)
//-----------------------------------------------------------------------------
inline int cMath::CalcPowerOfTwo(int x) {
	assert(x > 0);
	int p = 1;
	while(p < x) p += p;
	if(4 * x < 3 * p) p >>= 1;
	return p;
}
	
//-----------------------------------------------------------------------------
// cMath::IsPowerOfTwo : bool(int)
//-----------------------------------------------------------------------------
inline bool cMath::IsPowerOfTwo(int x) {
	return (x & (x - 1)) == 0 && x > 0;
}

} // comms

#endif // __cUtils__