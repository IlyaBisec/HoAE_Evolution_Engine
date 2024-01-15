#pragma once

template<class Type>
class cSegBezier {
public:
	void SetFromPoints(const Type &p0, const Type &p1, const Type &p2, const Type &p3);
	void SetFromEnds(const Type &p0, const Type &t0, const Type &p1, const Type &t1);

	const Type * GetPoints() const;
	
	const Type GetValue(const float t) const;
	const Type GetValue0() const;
	const Type GetValue1() const;

	const Type Get1stDerivative(const float t) const;
	const Type Get1stDerivative0() const;
	const Type Get1stDerivative1() const;

	const Type Get2ndDerivative(const float t) const;
	const Type Get2ndDerivative0() const;
	const Type Get2ndDerivative1() const;
	
	const Type Get3rdDerivative() const;

	const cBounds GetBounds() const;
	const cSphere GetBoundingSphere() const;
	
	float CalcLength(const float tFrom, const float tTo, const float dt = 0.01f) const;
	float CalcLength(const float dt = 0.01f) const;
	float CalcParameterForLength(const float Length, const float dt = 0.01f) const;

	void SplitToSubSegs(cSegBezier<Type> *pSubSeg0, cSegBezier<Type> *pSubSeg1, const float tSplitAt) const;
	void SplitToSubSegs05(cSegBezier<Type> *pSubSeg0, cSegBezier<Type> *pSubSeg1) const;
	
	float CalcLinearErrorBoundSq() const;
	float CalcClosestPointParameterLinear(const Type &p, float *pDistanceSq = NULL) const;
	float CalcClosestPointParameterAccurate(const Type &p, const float MaxErrSq, float *pDistanceSq = NULL) const;

private:
	Type m_Points[4];
};

typedef cSegBezier<float> cSegBezier1;
typedef cSegBezier<cVec2> cSegBezier2;
typedef cSegBezier<cVec3> cSegBezier3;
typedef cSegBezier<cAngles> cSegBezierA;

//-------------------------------------------------------------------------------------------------------------
// cSegBezier<Type>::SetFromPoints
//-------------------------------------------------------------------------------------------------------------
template<class Type>
inline void cSegBezier<Type>::SetFromPoints(const Type &p0, const Type &p1, const Type &p2, const Type &p3) {
	m_Points[0] = p0;
	m_Points[1] = p1;
	m_Points[2] = p2;
	m_Points[3] = p3;
}
											 
//-----------------------------------------------------------------------------------------------------------
// cSegBezier<Type>::SetFromEnds
//-----------------------------------------------------------------------------------------------------------
template<class Type>
inline void cSegBezier<Type>::SetFromEnds(const Type &p0, const Type &t0, const Type &p1, const Type &t1) {
	m_Points[0] = p0;
	m_Points[1] = t0 * 1.0f / 3.0f + p0;
	m_Points[2] = p1 - t1 * 1.0f / 3.0f;
	m_Points[3] = p1;
}

//-----------------------------------------------------------------------------
// cSegBezier<Type>::GetPoints
//-----------------------------------------------------------------------------
template<class Type>
inline const Type * cSegBezier<Type>::GetPoints() const {
	return m_Points;
}

//-----------------------------------------------------------------------------
// cSegBezier<Type>::GetValue
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cSegBezier<Type>::GetValue(const float t) const {
	float s = 1.0f - t;
	float B0 = s * s * s;
	float B1 = 3.0f * t * s * s;
	float B2 = 3.0f * s * t * t;
	float B3 = t * t * t;
	
	return B0 * m_Points[0] + B1 * m_Points[1] + B2 * m_Points[2] + B3 * m_Points[3];
}

//-----------------------------------------------------------------------------
// cSegBezier<Type>::GetValue0
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cSegBezier<Type>::GetValue0() const {
	return m_Points[0];
}

//-----------------------------------------------------------------------------
// cSegBezier<Type>::GetValue1
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cSegBezier<Type>::GetValue1() const {
	return m_Points[3];
}

//-----------------------------------------------------------------------------
// cSegBezier<Type>::Get1stDerivative
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cSegBezier<Type>::Get1stDerivative(const float t) const {
	float t2 = t * t;
	float B0 = - 3.0f + 6.0f * t - 3.0f * t2;
	float B1 = 3.0f - 12.0f * t + 9.0f * t2;
	float B2 = 6.0f * t - 9.0f * t2;
	float B3 = 3.0f * t2;
	
	return B0 * m_Points[0] + B1 * m_Points[1] + B2 * m_Points[2] + B3 * m_Points[3];
}

//-----------------------------------------------------------------------------
// cSegBezier<Type>::Get1stDerivative0
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cSegBezier<Type>::Get1stDerivative0() const {
	return - 3.0f * m_Points[0] + 3.0f * m_Points[1];
}

//-----------------------------------------------------------------------------
// cSegBezier<Type>::Get1stDerivative1
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cSegBezier<Type>::Get1stDerivative1() const {
	return - 3.0f * m_Points[2] + 3.0f * m_Points[3];
}

//-----------------------------------------------------------------------------
// cSegBezier<Type>::Get2ndDerivative
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cSegBezier<Type>::Get2ndDerivative(const float t) const {
	float B0 = 6.0f - 6.0f * t;
	float B1 = - 12.0f + 18.0f * t;
	float B2 = 6.0f - 18.0f * t;
	float B3 = 6.0f * t;
	
	return B0 * m_Points[0] + B1 * m_Points[1] + B2 * m_Points[2] + B3 * m_Points[3];
}

//-----------------------------------------------------------------------------
// cSegBezier<Type>::Get2ndDerivative0
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cSegBezier<Type>::Get2ndDerivative0() const {
	return 6.0f * m_Points[0] - 12.0f * m_Points[1] + 6.0f * m_Points[2];
}

//-----------------------------------------------------------------------------
// cSegBezier<Type>::Get2ndDerivative1
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cSegBezier<Type>::Get2ndDerivative1() const {
	return 6.0f * m_Points[1] - 12.0f * m_Points[2] + 6.0f * m_Points[3];
}

//-----------------------------------------------------------------------------
// cSegBezier<Type>::Get3rdDerivative
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cSegBezier<Type>::Get3rdDerivative() const {
	return - 6.0f * m_Points[0] + 18.0f * m_Points[1] - 18.0f * m_Points[2] + 6.0f * m_Points[3];
}

//-----------------------------------------------------------------------------
// cSegBezier<Type>::GetBounds
//-----------------------------------------------------------------------------
template<class Type>
inline const cBounds cSegBezier<Type>::GetBounds() const {
	return cBounds::FromPoints(m_Points, 4);
}

//-----------------------------------------------------------------------------
// cSegBezier<Type>::GetBoundSphere
//-----------------------------------------------------------------------------
template<class Type>
inline const cSphere cSegBezier<Type>::GetBoundingSphere() const {
	return cSphere::FromPoints(m_Points, 4);
}

//-----------------------------------------------------------------------------------------------------
// cSegBezier<Type>::CalcLength
//-----------------------------------------------------------------------------------------------------
template<class Type>
inline float cSegBezier<Type>::CalcLength(const float tFrom, const float tTo, const float dt) const {
	cAssert(cMath::IsZeroToOneEps(tFrom));
	cAssert(cMath::IsZeroToOneEps(tTo));
	
	Type p[2];
	p[0] = GetValue(tFrom);
	float Length = 0.0f;
	int index = 1;
	float t;
	for(float t = tFrom + dt; t <= tTo; t += dt) {
		p[index] = GetValue(t);
		Length += Type::Distance(p[0], p[1]);
		index ^= 1;
	}
	if(t < tTo) {
		p[index] = GetValue(tTo);
		Length += Type::Distance(p[0], p[1]);
	}
	return Length;
}

//-----------------------------------------------------------------------------
// cSegBezier<Type>::CalcLength
//-----------------------------------------------------------------------------
template<class Type>
inline float cSegBezier<Type>::CalcLength(const float dt) const {
	return CalcLength(0.0f, 1.0f, dt);
}

//-------------------------------------------------------------------------------------------------
// cSegBezier<Dim>::CalcParameterForLength
//-------------------------------------------------------------------------------------------------
template<class Type>
inline float cSegBezier<Type>::CalcParameterForLength(const float Length, const float dt) const {
	if(Length <= 0.0f) {
		return 0.0f;
	}

	Type p[2];
	int index = 1;
	p[0] = GetValue(0.0f);
	float PrevLength = 0.0f;

	for(float t = 0.0f; t <= 1.0f; t += dt) {
		p[index] = GetValue(t + dt);
		float NextLength = PrevLength + Type::Distance(p[0], p[1]);
		if(Length <= NextLength) {
			float l = cMath::LerperClamp01(PrevLength, NextLength, Length);
			return cMath::Lerp(t, t + dt, l);
		}
		PrevLength = NextLength;
		index ^= 1;
	}
	return 1.0f;
}

//------------------------------------------------------------------------------------------------------------
// cSegBezier<Type>::SplitToSubSegs05
//------------------------------------------------------------------------------------------------------------
template<class Type>
inline void cSegBezier<Type>::SplitToSubSegs05(cSegBezier<Type> *pSubSeg0, cSegBezier<Type> *pSubSeg1) const {
	Type b01 = Type::Lerp05(m_Points[0], m_Points[1]);
	Type b12 = Type::Lerp05(m_Points[1], m_Points[2]);
	Type b23 = Type::Lerp05(m_Points[2], m_Points[3]);
	Type b012 = Type::Lerp05(b01, b12);
	Type b123 = Type::Lerp05(b12, b23);
	Type b0123 = Type::Lerp05(b012, b123);

	pSubSeg0->SetFromPoints(m_Points[0], b01, b012, b0123);
	pSubSeg1->SetFromPoints(b0123, b123, b23, m_Points[3]);
}

//--------------------------------------------------------------------------------------------------------------------------------
// cSegBezier<Type>::SplitToSubSegs
//--------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void cSegBezier<Type>::SplitToSubSegs(cSegBezier<Type> *pSubSeg0, cSegBezier<Type> *pSubSeg1, const float tSplitAt) const {
	Type b01 = Type::Lerp(m_Points[0], m_Points[1], tSplitAt);
	Type b12 = Type::Lerp(m_Points[1], m_Points[2], tSplitAt);
	Type b23 = Type::Lerp(m_Points[2], m_Points[3], tSplitAt);
	Type b012 = Type::Lerp(b01, b12, tSplitAt);
	Type b123 = Type::Lerp(b12, b23, tSplitAt);
	Type b0123 = Type::Lerp(b012, b123, tSplitAt);
	
	pSubSeg0->SetFromPoints(m_Points[0], b01, b012, b0123);
	pSubSeg1->SetFromPoints(b0123, b123, b23, m_Points[3]);
}

//-----------------------------------------------------------------------------
// cSegBezier<Type>::CalcLinearErrorBoundSq
//-----------------------------------------------------------------------------
template<class Type>
inline float cSegBezier<Type>::CalcLinearErrorBoundSq() const {
	const Type p1 = Type::Lerp(m_Points[0], m_Points[3], 1.0f / 3.0f);
	const Type p2 = Type::Lerp(m_Points[0], m_Points[3], 2.0f / 3.0f);
	return cMath::Max(Type::DistanceSq(p1, m_Points[1]), Type::DistanceSq(p2, m_Points[2]));
}

//---------------------------------------------------------------------------------------------------------
// cSegBezier<Type>::CalcClosestPointParameterLinear
//---------------------------------------------------------------------------------------------------------
template<class Type>
inline float cSegBezier<Type>::CalcClosestPointParameterLinear(const Type &p, float *pDistanceSq) const {
	float Scale;
	cSeg Seg(cSeg::EndsCtor, GetValue0(), GetValue1());
	const cSeg::Result r = Seg.ProjectPoint(cSeg::Seg, p);
	if(pDistanceSq) {
		*pDistanceSq = Type::DistanceSq(p, r.CollidedPoint);
	}
	return r.Time;
}

//---------------------------------------------------------------------------------------------------------------------------------
// cSegBezier<Type>::CalcClosestPointParameterAccurate
//---------------------------------------------------------------------------------------------------------------------------------
template<class Type>
struct cSegBezierClosestPointRecurser {
	cSegBezier<Type> SubSeg;
	float tStart, tEnd;
};
template<class Type>
inline float cSegBezier<Type>::CalcClosestPointParameterAccurate(const Type &p, const float MaxErrSq, float *pDistanceSq) const {
	cList<cSegBezierClosestPointRecurser<Type> > Stack;
	
	float CurBestParameter = 0.0f;
	float CurBestDistance2 = Type::DistanceSq(p, m_Points[0]);

	cSegBezierClosestPointRecurser<Type> R;
	R.SubSeg = *this;
	R.tStart = 0.0f;
	R.tEnd = 1.0f;
	Stack.Add(R);

	while(Stack.Count() > 0) {
		const cSegBezierClosestPointRecurser<Type> &Cur = Stack.GetBack();
		
		cBounds bb = Cur.SubSeg.GetBounds();
		float dToBB2 = bb.DistanceToPointSq(p);

		if(dToBB2 >= CurBestDistance2) {
			Stack.RemoveBack();
			continue;
		}

		const float dHeadSq = Type::DistanceSq(p, Cur.SubSeg.GetValue0());
		const float dTailSq = Type::DistanceSq(p, Cur.SubSeg.GetValue1());
		
		float ErrSq = Cur.SubSeg.CalcLinearErrorBoundSq();
		if(ErrSq <= MaxErrSq) {
			cSeg Seg(cSeg::EndsCtor, Cur.SubSeg.GetValue0(), Cur.SubSeg.GetValue1());
			const cSeg::Result r = Seg.ProjectPoint(cSeg::Seg, p);
			const float Scale = r.Time;
			const float dToLineSeg2 = Type::DistanceSq(p, r.CollidedPoint);
			cAssert(dToBB2 <= dToLineSeg2 + cMath::SpaceEpsilon);

			if(dToLineSeg2 < CurBestDistance2) {
				CurBestDistance2 = dToLineSeg2;
				CurBestParameter = cMath::Lerp(Cur.tStart, Cur.tEnd, Scale);
			}
			Stack.RemoveBack();
			continue;
		}

		R = Stack.GetBack();
		Stack.RemoveBack();

		Stack.Add(R, 2);
		int st0 = Stack.Count() - 2;
		int st1 = Stack.Count() - 1;

		if(dHeadSq < dTailSq) {
			cMath::Swap(st0, st1);
		}

		R.SubSeg.SplitToSubSegs(&Stack[st0].SubSeg, &Stack[st1].SubSeg, 0.5f);

		Stack[st0].tStart = R.tStart;
		Stack[st0].tEnd = 0.5f * (R.tStart + R.tEnd);
		Stack[st1].tStart = Stack[st0].tEnd;
		Stack[st1].tEnd = R.tEnd;
	}
	
	if(pDistanceSq) {
		*pDistanceSq = CurBestDistance2;
	}

	return CurBestParameter;
}