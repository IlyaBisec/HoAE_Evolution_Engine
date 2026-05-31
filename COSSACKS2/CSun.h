#ifndef	__CSUN__
#define	__CSUN__

#include "stdafx.h"

class CSun
{
public:
	CSun(float k, int ShadowMinLevel, int ShadowMaxLevel, Vector3D Source0, ...)
	{
		m_k = k, m_ShadowMinLevel = ShadowMinLevel, m_ShadowMaxLevel = ShadowMaxLevel;

		Vector3D s = Source0;
		va_list Sources2Set;
		va_start(Sources2Set, Source0);
		m_NSources = 0;
		while(AddSource(s)) s = va_arg(Sources2Set, Vector3D);
		va_end(Sources2Set);
	}
	bool AddSource(Vector3D Source)
	{
		if((m_NSources == c_MaxSources) || (!Source.x && !Source.y && !Source.z)) return false;
		Source.normalize();
		m_Sources[m_NSources] = Source;
		m_NSources++;
	}
	// Defaults:
	static const int c_MaxSources = 20, c_defNSources = 9, c_defShadowMinLevel = 80, c_defShadowMaxLevel = 160;
	static const float c_defk;
	static const Vector3D c_defSources[c_defNSources];
	// Values:
	int m_NSources;
	Vector3D m_Sources[c_MaxSources];
	int m_ShadowMinLevel, m_ShadowMaxLevel;
	float m_k;
};

#endif