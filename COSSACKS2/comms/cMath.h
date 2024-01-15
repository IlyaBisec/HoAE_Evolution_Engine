#pragma once

class cMath {
public:
	static const float Pi;
	static const float TwoPi;
	static const float HalfPi;
	static const float QuarterPi;

	static const float RadsPerDeg;
	static const float DegsPerRad;

	static const double dPi;
	static const double dTwoPi;
	static const double dHalfPi;

	static const double dRadsPerDeg;
	static const double dDegsPerRad;
	
	static const float Epsilon;
	static const float EpsilonSq;
	static const float SpaceEpsilon;
	static const float Infinity;

	static const float MatrixEpsilon;
	static const float MatrixInvertEpsilon;

	static const double dEpsilon;

	static const float Sqrt1Over2;
	static const float Sqrt1Over3;

	static const float SecsPerMs;
	static const float MsPerSec;

	static const double DoubleEpsilon;
	static const double DoubleMinValue;
	static const double DoubleMaxValue;
	static const double DoubleNaN;
	static const double DoubleNegativeInfinity;
	static const double DoublePositiveInfinity;

	static const float FloatMinValue;
	static const float FloatMaxValue;

	static const int IntMinValue;
	static const int IntMaxValue;
	
	static float Rad(float Deg);
	static float Deg(float Rad);

	static float Sec(float Ms);
	static float Ms(float Sec);

	static bool IsZero(const float f, const float Eps = Epsilon);
	static bool IsOne(const float f, const float Eps = Epsilon);
	static bool IsMinusOne(const float f, const float Eps = Epsilon);
	static bool IsZeroToOneExact(const float f);
	static bool IsZeroToOneEps(const float f, const float Eps = Epsilon);
	static bool IsInRange(const int i, const int Lo, const int Hi);
	static bool IsInRangeExact(const float f, const float Lo, const float Hi);
	static bool IsInRangeEps(const float f, const float Lo, const float Hi, const float Eps = Epsilon);
	static bool Equals(const float x, const float y, const float Eps = Epsilon);
	static bool IsValid(const float f);

	static bool IsZero(const double d, const double Eps = dEpsilon);
	static bool Equals(const double x, const double y, const double Eps = dEpsilon);
	
	static float Clamp01(const float f);
	static float Lerp(const float a, const float b, const float s);
	static float Lerper(const float Fm, const float To, const float x);
	static float LerperClamp01(const float Lo, const float Hi, const float x);

	static int Abs(int n);
	static float Abs(float x);
	static double Abs(double x);
	static float Round(float x);
	static float Sqrt(float x);
	static float Sin(float a);
	static float Cos(float a);
	static void SinCos(float Angle, float &S, float &C);
	static float Tan(float a);
	static float ASin(float x);
	static float ACos(float x);
	static float ATan(float x);
	static float ATan(float y, float x);
	static float Pow(float x, float y);
	static float Exp(float x);
	static float Log(float x);

	static float Floor(float f);
	static float Ceil(float f);

	static double Floor(double d);
	static double Ceil(double d);

	static float Periodic(const float f, const float Lo, const float Hi, int *nPeriods = NULL);
	static int IndexForInsert(const float f, const float *Array, const int nCount, const size_t Stride = 0);
	
	static float AngleNormalizeTwoPi(float Angle); // 0 <= Angle < TwoPi
	static float AngleNormalizePi(float Angle); // - Pi < Angle <= Pi
	static float AngleNormalize360(float Angle);
	static float AngleNormalize180(float Angle);
	static float AngleDeltaRad(float Alpha, float Beta);
	static float AngleLerpRad(float Alpha, float Beta, float s);
	static float AngleDeltaDeg(float Alpha, float Beta);
	static float AngleLerpDeg(float Alpha, float Beta, float s);

	static int CalcPowerOfTwo(int x);
	static bool IsPowerOfTwo(int x);

	static void Randomize();
	static float Rand01();
	static float RandRange1();
	static float Rand(const float Lo, const float Hi);

	template<class Type> static void Swap(Type &x, Type &y);
	template<class Type> static void SwapChannels(Type *Array, const int Count, const int NChannels, const int Ch0, const int Ch1);
	
	template<class Type> static Type Square(const Type x);
	template<class Type> static Type Cube(const Type x);

	template<class Type> static Type Sign(const Type x);
	static int SignBitSet(const int i);
	static int SignBitNotSet(const int i);

	template<class Type> static Type Max(const Type x, const Type y);
	template<class Type> static Type Min(const Type x, const Type y);
	template<class Type> static int MaxIndex(const Type x, const Type y);
	template<class Type> static int MinIndex(const Type x, const Type y);
	
	template<class Type> static Type Max(const Type x, const Type y, const Type z);
	template<class Type> static Type Min(const Type x, const Type y, const Type z);
	template<class Type> static int MaxIndex(const Type x, const Type y, const Type z);
	template<class Type> static int MinIndex(const Type x, const Type y, const Type z);
	
	template<class Type> static Type Max(const Type x, const Type y, const Type z, const Type w);
	template<class Type> static Type Min(const Type x, const Type y, const Type z, const Type w);
	template<class Type> static int MaxIndex(const Type x, const Type y, const Type z, const Type w);
	template<class Type> static int MinIndex(const Type x, const Type y, const Type z, const Type w);
	
	template<class Type> static Type Clamp(const Type x, const Type Lo, const Type Hi);

	template<class Type> static const Type Hermite(const Type &p0, const Type &t0, const Type &p1, const Type &t1, const float s);
	template<class Type> static const Type Hermite1stDerivative(const Type &p0, const Type &t0, const Type &p1, const Type &t1, const float s);
	template<class Type> static const Type Hermite2ndDerivative(const Type &p0, const Type &t0, const Type &p1, const Type &t1, const float s);
	template<class Type> static const Type Hermite3rdDerivative(const Type &p0, const Type &t0, const Type &p1, const Type &t1, const float s);

	template<class Type> static void TCBSetup(const Type &Prev, const Type &Cur, const Type &Next, Type &CurIn, Type &CurOut,
		const float Tension = 0.0f, const float Continuity = 0.0f, const float Bias = 0.0f);
	template<class Type> static void TCBSetupTimeAdj(const Type &Prev, const Type &Cur, const Type &Next, Type &CurIn, Type &CurOut,
		const float tPrev, const float tCur, const float tNext,
		const float Tension = 0.0f, const float Continuity = 0.0f, const float Bias = 0.0f);
	template<class Type> static const Type TCBSetupStartOut(const Type &Start, const Type &Next,
		const float Tension = 0.0f, const float Continuity = 0.0f, const float Bias = 0.0f);
	template<class Type> static const Type TCBSetupEndIn(const Type &Prev, const Type &End,
		const float Tension = 0.0f, const float Continuity = 0.0f, const float Bias = 0.0f);

	template<class Type> static void _TCBSetup(const Type &Prev, const Type &Cur, const Type &Next, Type &CurIn, Type &CurOut,
		const float Tension = 0.0f, const float Continuity = 0.0f, const float Bias = 0.0f);
	template<class Type> static const Type _TCBSetupStartOut(const Type &Start, const Type &Next, const Type &NextIn, const float Tension = 0.0f);
	template<class Type> static const Type _TCBSetupEndIn(const Type &Prev, const Type &PrevOut, const Type &End, const float Tension = 0.0f);
	template<class Type> static const Type _TCBSetupStartOutEndIn(const Type &Start, const Type &End, const float Tension = 0.0f);
	
	static float TCBAdjInCoeff(const float tPrev, const float tCur, const float tNext);
	static float TCBAdjOutCoeff(const float tPrev, const float tCur, const float tNext);

	static DWORD Rgba(int Red, int Green, int Blue, int Alpha = 255);
	
	static bool RayTri(const cVec3 &RayOrig, const cVec3 &RayDir, const cVec3 &t0, const cVec3 &t1, const cVec3 &t2, float &u, float &v, float &t);
};

// cMath::Rad : float(float)
inline float cMath::Rad(float Deg) {
	return Deg * RadsPerDeg;
}

// cMath::Deg : float(float)
inline float cMath::Deg(float Rad) {
	return Rad * DegsPerRad;
}

// cMath::Sec : float(float)
inline float cMath::Sec(float Ms) {
	return Ms * SecsPerMs;
}

// cMath::Ms : float(float)
inline float cMath::Ms(float Sec) {
	return Sec * MsPerSec;
}

// cMath::IsZero : bool(const float, const float)
inline bool cMath::IsZero(const float f, const float Eps) {
	return Abs(f) <= Eps;
}

// cMath::IsOne : bool(const float, const float)
inline bool cMath::IsOne(const float f, const float Eps) {
	return Abs(f - 1.0f) <= Eps;
}

// cMath::IsMinusOne
inline bool cMath::IsMinusOne(const float f, const float Eps) {
	return Abs(f + 1.0f) <= Eps;
}

// cMath::IsZeroToOneExact
inline bool cMath::IsZeroToOneExact(const float f) {
	return f >= 0.0f && f <= 1.0f;
}

// cMath::IsZeroToOneEps
inline bool cMath::IsZeroToOneEps(const float f, const float Eps) {
	return f >= - Eps && f <= 1.0f + Eps;
}

// cMath::IsInRange : bool(const int, const int, const int)
inline bool cMath::IsInRange(const int i, const int Lo, const int Hi) {
	return i >= Lo && i <= Hi;
}

// cMath::IsInRangeExact
inline bool cMath::IsInRangeExact(const float f, const float Lo, const float Hi) {
	return f >= Lo && f <= Hi;
}

// cMath::IsInRangeEps
inline bool cMath::IsInRangeEps(const float f, const float Lo, const float Hi, const float Eps) {
	return f >= Lo - Eps && f <= Hi + Eps;
}

// cMath::Equals : bool(const float, const float, const float)
inline bool cMath::Equals(const float x, const float y, const float Eps) {
	return Abs(x - y) <= Eps;
}

// cMath::IsValid : bool(const float)
inline bool cMath::IsValid(const float f) {
	return f >= -FloatMaxValue && f <= FloatMaxValue;
}

// cMath::IsZero : bool(const double, const double)
inline bool cMath::IsZero(const double d, const double Eps) {
	return Abs(d) <= Eps;
}

// cMath::Equals : bool(const double, const double, const double)
inline bool cMath::Equals(const double x, const double y, const double Eps) {
	return Abs(x - y) <= Eps;
}

// cMath::Clamp01 : float(const float)
inline float cMath::Clamp01(const float s) {
	return s < 0.0f ? 0.0f : (s > 1.0f ? 1.0f : s);
}

// cMath::Lerp : float(const float, const float, const float)
inline float cMath::Lerp(const float a, const float b, const float s) {
	return a + s * (b - a);
}

// cMath::Lerper : float(const float, const float, const float)
inline float cMath::Lerper(const float Fm, const float To, const float x) {
	return (x - Fm) / (To - Fm);
}

// cMath::LerperClamp01 : float(const float, const float, const float)
inline float cMath::LerperClamp01(const float Lo, const float Hi, const float x) {
	cAssert(Hi > Lo);
	return Clamp01((x - Lo) / (Hi - Lo));
}

// cMath::Abs : int(int)
inline int cMath::Abs(int n) {
	int s = n >> 31;
	return (n ^ s) - s;
}

// cMath::Abs : float(float)
inline float cMath::Abs(float f) {
	int i = *reinterpret_cast<int *>(&f);
	i &= 0x7fffffff;
	return *reinterpret_cast<float *>(&i);
}

// cMath::Abs : double(double)
inline double cMath::Abs(double d) {
	return fabs(d);
}

// cMath::Round : float(float)
inline float cMath::Round(float x) {
	return floorf(x + (x > 0.0f ? 0.5f : -0.5f));
}

// cMath::Sqrt : float(float)
inline float cMath::Sqrt(float x) {
	cAssert(x >= 0.0f);
	return sqrtf(x);
}

// cMath::Sin : float(float)
inline float cMath::Sin(float a) {
	return sinf(a);
}

// cMath::Cos : float(float)
inline float cMath::Cos(float a) {
	return cosf(a);
}

// cMath::SinCos : void(float, float &, float &)
inline void cMath::SinCos(float Angle, float &S, float &C) {
	S = Sin(Angle);
	C = Cos(Angle);
}

// cMath::Tan : float(float)
inline float cMath::Tan(float a) {
	return tanf(a);
}

// cMath::ASin : float(float)
inline float cMath::ASin(float x) {
	if(x <= -1.0f) {
		return - HalfPi;
	}
	if(x >= 1.0f) {
		return HalfPi;
	}
	return asinf(x);
}

// cMath::ACos : float(float)
inline float cMath::ACos(float x) {
	if(x <= -1.0f) {
		return Pi;
	}
	if(x >= 1.0f) {
		return 0.0f;
	}
	return acosf(x);
}

// cMath::ATan : float(float)
inline float cMath::ATan(float a) {
	return atanf(a);
}

// cMath::ATan : float(float, float)
inline float cMath::ATan(float y, float x) {
	return atan2f(y, x);
}

// cMath::Pow : float(float, float)
inline float cMath::Pow(float x, float y) {
	return powf(x, y);
}

// cMath::Exp : float(float)
inline float cMath::Exp(float x) {
	return expf(x);
}

// cMath::Log : float(float)
inline float cMath::Log(float x) {
	return logf(x);
}

// cMath::Floor : float(float)
inline float cMath::Floor(float f) {
	return floorf(f);
}

// cMath::Ceil : float(float)
inline float cMath::Ceil(float f) {
	return ceilf(f);
}

// cMath::Floor : double(double)
inline double cMath::Floor(double d) {
	return floor(d);
}

// cMath::Ceil : double(double)
inline double cMath::Ceil(double d) {
	return ceil(d);
}

// cMath::Periodic : float(const float, const float, const float, int *)
inline float cMath::Periodic(const float f, const float Lo, const float Hi, int *nPeriods) {
	const float Range = Hi - Lo;
	if(Range == 0.0f) {
		if(nPeriods) {
			*nPeriods = 0;
			return Lo;
		}
	}

	float u = f - Range * (float)cMath::Floor((double)(f - Lo) / Range);
	if(nPeriods) {
		*nPeriods = -(int)((u - f) / Range + (u > f ? 0.5f : -0.5f));
	}
	return u;
}

//----------------------------------------------------------------------------------------------------------
// cMath::IndexForInsert
//----------------------------------------------------------------------------------------------------------
inline int cMath::IndexForInsert(const float f, const float *Array, const int nCount, const size_t Stride) {
	const size_t S = cMath::Max(sizeof(float), Stride);
	const char *Ptr = reinterpret_cast<const char *>(Array);
	int N = nCount;
	int Mid = N;
	int Offset = 0, Res = 0;
	while(Mid > 0) {
		Mid = N >> 1;
		if(f == *reinterpret_cast<const float *>(Ptr + S * (Offset + Mid))) {
			return Offset + Mid;
		} else if(f > *reinterpret_cast<const float *>(Ptr + S * (Offset + Mid))) {
			Offset += Mid;
			N -= Mid;
			Res = 1;
		} else {
			N -= Mid;
			Res = 0;
		}
	}
	return Offset + Res;
}

// cMath::AngleNormalizeTwoPi : float(float)
inline float cMath::AngleNormalizeTwoPi(float Angle) { // 0 <= Angle < TwoPi
	if(Angle >= TwoPi || Angle < 0.0f) {
		Angle -= floorf(Angle / TwoPi) * TwoPi;
	}
	return Angle;
}

// cMath::AngleNormalizePi : float(float)
inline float cMath::AngleNormalizePi(float Angle) { // - Pi < Angle <= Pi
	Angle = AngleNormalizeTwoPi(Angle);
	if(Angle > Pi) {
		Angle -= TwoPi;
	}
	return Angle;
}

// cMath::AngleNormalize360 : float(float)
inline float cMath::AngleNormalize360(float Angle) {
	if(Angle >= 360.0f || Angle < 0.0f) {
		Angle -= floorf(Angle / 360.0f) * 360.0f;
	}
	return Angle;
}

// cMath::AngleNormalize180 : float(float)
inline float cMath::AngleNormalize180(float Angle) {
	Angle = AngleNormalize360(Angle);
	if(Angle > 180.0f) {
		Angle -= 360.0f;
	}
	return Angle;
}

// cMath::AngleDeltaDeg : float(float, float)
inline float cMath::AngleDeltaDeg(float Alpha, float Beta) {
	return AngleNormalize180(Alpha - Beta);
}

// cMath::AngleLerpDeg : float(float, float, float)
inline float cMath::AngleLerpDeg(float Alpha, float Beta, float s) {
	while(Alpha > Beta + 180.0f) {
		Alpha += 360.0f;
	}
	while(Alpha < Beta - Pi) {
		Alpha -= 360.0f;
	}
	return Lerp(Alpha, Beta, s);
}


// cMath::AngleDeltaRad : float(float, float)
inline float cMath::AngleDeltaRad(float Alpha, float Beta) {
	return AngleNormalizePi(Alpha - Beta);
}

// cMath::AngleLerpRad : float(float, float, float)
inline float cMath::AngleLerpRad(float Alpha, float Beta, float s) {
	while(Alpha > Beta + Pi) {
		Alpha += TwoPi;
	}
	while(Alpha < Beta - Pi) {
		Alpha -= TwoPi;
	}
	return Lerp(Alpha, Beta, s);
}

// cMath::CalcPowerOfTwo : int(int)
inline int cMath::CalcPowerOfTwo(int x) {
	cAssert(x > 0);
	int p = 1;
	while(p < x) p += p;
	if(4 * x < 3 * p) p >>= 1;
	return p;
}
	
// cMath::IsPowerOfTwo : bool(int)
inline bool cMath::IsPowerOfTwo(int x) {
	return (x & (x - 1)) == 0 && x > 0;
}

// cMath::Randomize
inline void cMath::Randomize() {
	srand((unsigned)GetTickCount());
}

// cMath::Rand01 : float()
inline float cMath::Rand01() {
	return float(rand()) / float(RAND_MAX);
}

// cMath::RandRange1 : float()
inline float cMath::RandRange1() {
	return 2.0f * (Rand01() - 0.5f);
}

// cMath::Rand : float(const float, const float)
inline float cMath::Rand(const float Lo, const float Hi) {
	return Lo + Rand01() * (Hi - Lo);
}

// cMath::Swap<Type> : void(Type &, Type &)
template<class Type>
inline void cMath::Swap(Type &x, Type &y) {
	Type z = x;
	x = y;
	y = z;
}

// cMath::SwapChannels<Type> : void (Type *, const int, const int, const int, const int)
template<class Type>
inline void cMath::SwapChannels(Type *Array, const int Count, const int NChannels, const int Ch0, const int Ch1) {
	for(int i = 0; i < Count; i++) {
		Swap(Array[Ch0], Array[Ch1]);
		Array += NChannels;
	}
}

// cMath::Square<Type> : Type(const Type)
template<class Type>
inline Type cMath::Square(const Type x) {
	return x * x;
}

// cMath::Cube<Type> : Type(const Type)
template<class Type>
inline Type cMath::Cube(const Type x) {
	return x * x * x;
}

// cMath::Sign<Type> : Type(const Type)
template<class Type>
inline Type cMath::Sign(const Type x) {
	return x > (Type)0 ? (Type)1 : (x < (Type)0 ? -(Type)1 : (Type)0);
}

// cMath::SignBitSet
inline int cMath::SignBitSet(const int i) {
	return ((const unsigned long)(i)) >> 31;
}

// cMath::SignBitNotSet
inline int cMath::SignBitNotSet(const int i) {
	return (~((const unsigned long)(i))) >> 31;
}

// cMath::Max<Type> : Type(const Type, const Type)
template<class Type>
inline Type cMath::Max(const Type x, const Type y) {
	return x > y ? x : y;
}

// cMath::Min<Type> : Type(const Type, const Type)
template<class Type>
inline Type cMath::Min(const Type x, const Type y) {
	return x < y ? x : y;
}

// cMath::MaxIndex<Type> : int(const Type, const Type)
template<class Type>
inline int cMath::MaxIndex(const Type x, const Type y) {
	return x > y ? 0 : 1;
}

// cMath::MinIndex<Type> : int(const Type, const Type)
template<class Type>
inline int cMath::MinIndex(const Type x, const Type y) {
	return x < y ? 0 : 1;
}

// cMath::Max<Type> : Type(const Type, const Type, const Type)
template<class Type>
inline Type cMath::Max(const Type x, const Type y, const Type z) {
	return x > y ? (x > z ? x : z) : (y > z ? y : z);
}

// cMath::Min<Type> : Type(const Type, const Type, const Type)
template<class Type>
inline Type cMath::Min(const Type x, const Type y, const Type z) {
	return x < y ? (x < z ? x : z) : (y < z ? y : z);
}

// cMath::MaxIndex<Type> : int(const Type, const Type, const Type)
template<class Type>
inline int cMath::MaxIndex(const Type x, const Type y, const Type z) {
	return x > y ? (x > z ? 0 : 2) : (y > z ? 1 : 2);
}

// cMath::MinIndex<Type> : int(const Type, const Type, const Type)
template<class Type>
inline int cMath::MinIndex(const Type x, const Type y, const Type z) {
	return x < y ? (x < z ? 0 : 2) : (y < z ? 1 : 2);
}

// cMath::Max<Type> : Type (const Type, const Type, const Type, const Type)
template<class Type>
inline Type cMath::Max(const Type x, const Type y, const Type z, const Type w) {
	return x > y ? (x > z ? (x > w ? x : w) : (z > w ? z : w)) : (y > z ? (y > w ? y : w) : (z > w ? z : w));
}

// cMath::Min<Type> : Type (const Type, const Type, const Type, const Type)
template<class Type>
inline Type cMath::Min(const Type x, const Type y, const Type z, const Type w) {
	return x < y ? (x < z ? (x < w ? x : w) : (z < w ? z : w)) : (y < z ? (y < w ? y : w) : (z < w ? z : w));
}

// cMath::MaxIndex<Type> : int (const Type, const Type, const Type, const Type)
template<class Type>
inline int cMath::MaxIndex(const Type x, const Type y, const Type z, const Type w) {
	return x > y ? (x > z ? (x > w ? 0 : 3) : (z > w ? 2 : 3)) : (y > z ? (y > w ? 1 : 3) : (z > w ? 2 : 3));
}

// cMath::MinIndex<Type> : int (const Type, const Type, const Type, const Type)
template<class Type>
inline int cMath::MinIndex(const Type x, const Type y, const Type z, const Type w) {
	return x < y ? (x < z ? (x < w ? 0 : 3) : (z < w ? 2 : 3)) : (y < z ? (y < w ? 1 : 3) : (z < w ? 2 : 3));
}

// cMath::Clamp<Type> : Type(const Type, const Type, const Type)
template<class Type>
inline Type cMath::Clamp(const Type x, const Type Lo, const Type Hi) {
	return Min(Max(x, Lo), Hi);
}

// cMath::Hermite<Type> : const Type(const Type &, const Type &, const Type &, const Type &, const float)
template<class Type>
inline const Type cMath::Hermite(const Type &p0, const Type &t0, const Type &p1, const Type &t1, const float s) {
	float s2 = s * s;
	float s3 = s * s2;

	float c0 = 2.0f * s3 - 3.0f * s2 + 1.0f;
	float d0 = s3 - 2.0f * s2 + s;
	float c1 = - 2.0f * s3 + 3.0f * s2;
	float d1 = s3 - s2;

	return c0 * p0 + d0 * t0 + c1 * p1 + d1 * t1;
}

// cMath::Hermite1stDerivative<Type> : const Type(const Type &, const Type &, const Type &, const Type &, const float)
template<class Type>
inline const Type cMath::Hermite1stDerivative(const Type &p0, const Type &t0, const Type &p1, const Type &t1, const float s) {
	const float s2 = s * s;

	float c0 = 6.0f * s2 - 6.0f * s;
	float d0 = 3.0f * s2 - 4.0f * s + 1.0f;
	float c1 = - 6.0f * s2 + 6.0f * s;
	float d1 = 3.0f * s2 - 2.0f * s;
	
	return c0 * p0 + d0 * t0 + c1 * p1 + d1 * t1;
}

// cMath::Hermite2ndDerivative<Type> : const Type(const Type &, const Type &, const Type &, const Type &, const float)
template<class Type>
inline const Type cMath::Hermite2ndDerivative(const Type &p0, const Type &t0, const Type &p1, const Type &t1, const float s) {
	float c0 = 12.0f * s - 6.0f;
	float d0 = 6.0f * s - 4.0f;
	float c1 = - 12.0f * s + 6.0f;
	float d1 = 6.0f * s - 2.0f;

	return c0 * p0 + d0 * t0 + c1 * p1 + d1 * t1;
}

// cMath::Hermite3rdDerivative<Type> : const Type(const Type &, const Type &, const Type &, const Type &, const float)
template<class Type>
inline const Type cMath::Hermite3rdDerivative(const Type &p0, const Type &t0, const Type &p1, const Type &t1, const float s) {
	return 12.0f * p0 + 6.0f * t0 - 12.0f * p1 + 6.0f * t1;
}

//---------------------------------------------------------------------------------------------------------
// cMath::TCBSetup<Type>
//---------------------------------------------------------------------------------------------------------
template<class Type>
inline void cMath::_TCBSetup(const Type &Prev, const Type &Cur, const Type &Next, Type &CurIn, Type &CurOut,
							const float Tension, const float Continuity, const float Bias) {
	Type g0 = (Cur - Prev) * (1.0f + Bias);
	Type g1 = (Next - Cur) * (1.0f - Bias);
	Type dg = g1 - g0;

	CurIn = (1.0f - Tension) * (g0 + 0.5f * dg * (1.0f + Continuity));
	CurOut = (1.0f - Tension) * (g0 + 0.5f * dg * (1.0f - Continuity));
}

//-----------------------------------------------------------------------------------------------------------------------
// cMath::TCBSetupStartOut
//-----------------------------------------------------------------------------------------------------------------------
template<class Type>
inline const Type cMath::_TCBSetupStartOut(const Type &Start, const Type &Next, const Type &NextIn, const float Tension) {
	return (1.5f * (Next - Start) - 0.5f * NextIn) * (1.0f - Tension);
}

//-------------------------------------------------------------------------------------------------------------------
// cMath::TCBSetupEndIn
//-------------------------------------------------------------------------------------------------------------------
template<class Type>
inline const Type cMath::_TCBSetupEndIn(const Type &Prev, const Type &PrevOut, const Type &End, const float Tension) {
	return (1.5f * (End - Prev) - 0.5f * PrevOut) * (1.0f - Tension);
}

//-------------------------------------------------------------------------------------------------------
// cMath::TCBSetupStartOutEndIn
//-------------------------------------------------------------------------------------------------------
template<class Type>
inline const Type cMath::_TCBSetupStartOutEndIn(const Type &Start, const Type &End, const float Tension) {
	return (End - Start) * (1.0f - Tension);
}

// cMath::TCBAdjInCoeff
inline float cMath::TCBAdjInCoeff(const float tPrev, const float tCur, const float tNext) {
	return (tCur - tPrev) / (tNext - tPrev);
}

// cMath::TCBAdjOutCoeff
inline float cMath::TCBAdjOutCoeff(const float tPrev, const float tCur, const float tNext) {
	return (tNext - tCur) / (tNext - tPrev);
}




//---------------------------------------------------------------------------------------------------------
// cMath::TCBSetup<Type>
//---------------------------------------------------------------------------------------------------------
template<class Type>
inline void cMath::TCBSetup(const Type &Prev, const Type &Cur, const Type &Next, Type &CurIn, Type &CurOut,
							const float Tension, const float Continuity, const float Bias) {
	const float Ai = (1.0f - Tension) * (1.0f - Continuity) * (1.0f + Bias);
	const float Ao = (1.0f - Tension) * (1.0f + Continuity) * (1.0f + Bias);
	const float Bi = (1.0f - Tension) * (1.0f + Continuity) * (1.0f - Bias);
	const float Bo = (1.0f - Tension) * (1.0f - Continuity) * (1.0f - Bias);
	const Type Di = Cur - Prev;
	const Type Do = Next - Cur;
	CurIn = Bi * (Next - Cur) + Ai * Di;
	CurOut = Ao * (Cur - Prev) + Bo * Do;
}

//---------------------------------------------------------------------------------------------------------
// cMath::TCBSetup<Type>
//---------------------------------------------------------------------------------------------------------
template<class Type>
inline void cMath::TCBSetupTimeAdj(const Type &Prev, const Type &Cur, const Type &Next, Type &CurIn, Type &CurOut,
								   const float tPrev, const float tCur, const float tNext,
								   const float Tension, const float Continuity, const float Bias) {
	TCBSetup(Prev, Cur, Next, CurIn, CurOut, Tension, Continuity, Bias);
	CurIn *= TCBAdjInCoeff(tPrev, tCur, tNext);
	CurOut *= TCBAdjOutCoeff(tPrev, tCur, tNext);
}

//--------------------------------------------------------------------------------------------------------
// cMath::TCBSetupStartOut<Type>
//--------------------------------------------------------------------------------------------------------
template<class Type>
inline const Type cMath::TCBSetupStartOut(const Type &Start, const Type &Next,
										  const float Tension, const float Continuity, const float Bias) {
	return (1.0f - Tension) * (1.0f - Continuity) * (1.0f - Bias) * (Next - Start);
}

//-----------------------------------------------------------------------------------------------------
// cMath::TCBSetupEndIn<Type>
//-----------------------------------------------------------------------------------------------------
template<class Type>
inline const Type cMath::TCBSetupEndIn(const Type &Prev, const Type &End,
									   const float Tension, const float Continuity, const float Bias) {
	return (1.0f - Tension) * (1.0f - Continuity) * (1.0f + Bias) * (End - Prev);
}

//-----------------------------------------------------------------------------
// cMath::Rgba
//-----------------------------------------------------------------------------
inline DWORD cMath::Rgba(int Red, int Green, int Blue, int Alpha) {
	return (BYTE)Red | (WORD)(BYTE)Green << 8 | (DWORD)(BYTE)Blue << 16 | (DWORD)(BYTE)Alpha << 24;
} // cMath::Rgba
