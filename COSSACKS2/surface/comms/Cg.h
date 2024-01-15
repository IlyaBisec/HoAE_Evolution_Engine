#ifndef __Cg__
#define __Cg__

#include "cUtils.h"

namespace comms {

float clamp(const float a, const float b, const float s);
float lerp(const float, const float, const float);
float sign(const float);

// clamp : float(const float, const float, const float)
inline float clamp(const float s, const float Lo, const float Hi) {
	return Min(Max(s, Lo), Hi);
}

// sign : float(const float)
inline float sign(const float s) {
	return s > 0 ? 1.0f : (s < 0 ? -1.0f : 0.0f);
}

// lerp : float(const float, const float, const float)
inline float lerp(const float a, const float b, const float s) {
	return a + s * (b - a);
}

struct float3 {
	float x;
	float y;
	float z;

	float3();
	float3(const float);
	float3(const float *);
	float3(const float3 &);
	float3(const float, const float, const float);

	void operator += (const float);
	void operator += (const float3 &);
	void operator -= (const float);
	void operator -= (const float3 &);
	void operator *= (const float);
	void operator *= (const float3 &);
	void operator /= (const float);
	void operator /= (const float3 &);
};

float3 operator + (const float3 &);
float3 operator - (const float3 &);

float3 operator + (const float3 &, const float);
float3 operator + (const float, const float3 &);
float3 operator + (const float3 &, const float3 &);

float3 operator - (const float3 &, const float);
float3 operator - (const float, const float3 &);
float3 operator - (const float3 &, const float3 &);

float3 operator * (const float3 &, const float);
float3 operator * (const float, const float3 &);
float3 operator * (const float3 &, const float3 &);

float3 operator / (const float3 &, const float);
float3 operator / (const float, const float3 &);
float3 operator / (const float3 &, const float3 &);

float3	abs(const float3 &);
float3	clamp(const float3 &, const float3 &, const float3 &);
float3	cross(const float3 &, const float3 &);
float	distance(const float3 &, const float3 &);
float	dot(const float3 &, const float3 &);
float3	faceforward(const float3 &, const float3 &, const float3 &);
float	length(const float3 &);
float3	lerp(const float3 &, const float3 &, const float);
float3	normalize(const float3 &);
float3	reflect(const float3 &, const float3 &);
float3	refrect(const float3 &, const float3 &, const float);
float3	sign(const float3 &);

// float3.ctor : ()
inline float3::float3()
: x(0.0f), y(0.0f), z(0.0f) {}

// float3.ctor : (const float)
inline float3::float3(const float S)
: x(S), y(S), z(S) {}

// float3.ctor : (const float *)
inline float3::float3(const float *p) {
	x = p[0];
	y = p[1];
	z = p[2];
}

// float3.ctor : (const float3 &)
inline float3::float3(const float3 &u) {
	x = u.x;
	y = u.y;
	z = u.z;
}

// float3.ctor : (const float, const float, const float)
inline float3::float3(const float a, const float b, const float c) {
	x = a;
	y = b;
	z = c;
}

// float3::operator += : void(const float)
inline void float3::operator += (const float s) {
	x += s;
	y += s;
	z += s;
}

// float3::operator += : void(const float3 &)
inline void float3::operator += (const float3 &u) {
	x += u.x;
	y += u.y;
	z += u.z;
}

// float3::operator -= : void(const float)
inline void float3::operator -= (const float s) {
	x -= s;
	y -= s;
	z -= s;
}

// float3::operator -= : void(const float3 &)
inline void float3::operator -= (const float3 &u) {
	x -= u.x;
	y -= u.y;
	z -= u.z;
}

// float3::operator *= : void(const float)
inline void float3::operator *= (const float s) {
	x *= s;
	y *= s;
	z *= s;
}

// float3::operator *= : void(const float3 &)
inline void float3::operator *= (const float3 &u) {
	x *= u.x;
	y *= u.y;
	z *= u.z;
}

// float3::operator /= : void(const float)
inline void float3::operator /= (const float s) {
	x /= s;
	y /= s;
	z /= s;
}

// float3::operator /= : void(const float3 &)
inline void float3::operator /= (const float3 &u) {
	x /= u.x;
	y /= u.y;
	z /= u.z;
}

// operator + : float3(const float3 &)
inline float3 operator + (const float3 &u) {
	return u;
}

// operator - : float3(const float3 &)
inline float3 operator - (const float3 &u) {
	return float3(-u.x, -u.y, -u.z);
}

// operator + : float3(const float3 &, const float)
inline float3 operator + (const float3 &u, const float s) {
	return float3(u.x + s, u.y + s, u.z + s);
}

// operator + : float3(const float, const float3 &)
inline float3 operator + (const float s, const float3 &u) {
	return float3(s + u.x, s + u.y, s + u.z);
}

// operator + : float3(const float3 &, const float3 &)
inline float3 operator + (const float3 &u, const float3 &v) {
	return float3(u.x + v.x, u.y + v.y, u.z + v.z);
}

// operator - : float3(const float3 &, const float)
inline float3 operator - (const float3 &u, const float s) {
	return float3(u.x - s, u.y - s, u.z - s);
}

// operator - : float3(const float, const float3 &)
inline float3 operator - (const float s, const float3 &u) {
	return float3(s - u.x, s - u.y, s - u.z);
}

// operator - : float3(const float3 &, const float3 &)
inline float3 operator - (const float3 &u, const float3 &v) {
	return float3(u.x - v.x, u.y - v.y, u.z - v.z);
}

// operator * : float3(const float3 &, const float)
inline float3 operator * (const float3 &u, const float s) {
	return float3(u.x * s, u.y * s, u.z * s);
}

// operator * : float3(const float, const float3 &)
inline float3 operator * (const float s, const float3 &u) {
	return float3(s * u.x, s * u.y, s * u.z);
}

// operator / : float3(const float3 &, const float)
inline float3 operator / (const float3 &u, const float s) {
	return float3(u.x / s, u.y / s, u.z / s);
}

// operator / : float3(const float, const float3 &)
inline float3 operator / (const float s, const float3 &u) {
	return float3(s / u.x, s / u.y, s / u.z);
}

// operator / : float3(const float3 &, const float3 &)
inline float3 operator / (const float3 &u, const float3 &v) {
	return float3(u.x / v.x, u.y / v.y, u.z / v.z);
}

// operator * : float3(const float3 &, const float3 &)
inline float3 operator * (const float3 &u, const float3 &v) {
	return float3(u.x * v.x, u.y * v.y, u.z * v.z);
}

// abs : float3(const float3 &)
inline float3 abs(const float3 &u) {
	return float3(::abs(u.x), ::abs(u.y), ::abs(u.z));
}

// clamp : float3(const float3 &, const float3 &, const float3 &)
inline float3 clamp(const float3 &u, const float3 &Lo, const float3 &Hi) {
	return float3(Min(Max(u.x, Lo.x), Hi.x), Min(Max(u.y, Lo.y), Hi.y), Min(Max(u.z, Lo.z), Hi.z));
}

// cross : float3(const float3 &, const float3 &)
inline float3 cross(const float3 &u, const float3 &v) {
	return float3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

// distance : float(const float3 &, const float3 &)
inline float distance(const float3 &p0, const float3 &p1) {
	return length(p0 - p1);
}

// dot : float(const float3 &, const float3 &)
inline float dot(const float3 &u, const float3 &v) {
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

// faceforward : float3(const float3 &, const float3 &, const float3 &)
inline float3 faceforward(const float3 &n, const float3 &i, const float3 &ng) {
	return - n * sign(dot(i, ng));
}

// lenght : float(const float3 &)
inline float length(const float3 &u) {
	return sqrtf(u.x * u.x + u.y * u.y + u.z * u.z);
}

// lerp : float3(const float3 &, const float3 &, const float)
inline float3 lerp(const float3 &u, const float3 &v, const float s) {
	return u + s * (v - u);
}

// cerp : float(const float, const float, const float, const float, const float)
inline float cerp(const float u0, const float u1, const float u2, const float u3, const float s) {
	float p = (u3 - u2) - (u0 - u1);
	float q = (u0 - u1) - p;
	float r = u2 - u0;
	return s * (s * (s * p + q) + r) + u1;
}


// cerp : float3(const float3 &, const float3 &, const float3 &, const float3 &, const float)
inline float3 cerp(const float3 &u0, const float3 &u1, const float3 &u2, const float3 &u3, const float s) {
	float3 p = (u3 - u2) - (u0 - u1);
	float3 q = (u0 - u1) - p;
	float3 r = u2 - u0;
	return s * (s * (s * p + q) + r) + u1;
}


// normalize : float3(const float3 &)
inline float3 normalize(const float3 &u) {
	return u / sqrtf(u.x * u.x + u.y * u.y + u.z * u.z);
}

// reflect : float3(const float3 &, const float3 &)
inline float3 reflect(const float3 &i, const float3 &n) {
	return i - 2 * dot(i, n) * n;
}

// refrect : float3(const float3 &, const float3 &, const float)
inline float3 refrect(const float3 &i, const float3 &n, const float eta) {
	float k = 1.0f - eta * eta * (1.0f - dot(n, i) * dot(n, i));
	if(k < 0.0f) return float3(0.0f, 0.0f, 0.0f);
	else return eta * i - (eta * dot(n, i) + sqrtf(k)) * n;
}

// sign : float3(const float3 &)
inline float3 sign(const float3 &u) {
	return float3(sign(u.x), sign(u.y), sign(u.z));
}

// truncate : float3(const float3 &, const float)
inline float3 truncate(const float3 &u, const float MaxLen) {
	float3 r(u);
	if(MaxLen == 0) return float3(0, 0, 0);
	if(MaxLen > 0) {
		float l = length(u);
		if(l > MaxLen) {
			float il = MaxLen / l;
			r.x = u.x * il;
			r.y = u.y * il;
			r.z = u.z * il;
		}
	}
	return r;
}

} // comms

#endif // __Cg__