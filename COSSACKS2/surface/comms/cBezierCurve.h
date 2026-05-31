#pragma once

template<class Type>
class cBezierCurve {
public:
	void SetFromPoints(const Type &P0, const Type &P1, const Type &P2, const Type &P3);
	void SetFromEnds(const Type &P0, const Type &D0, const Type &P1, const Type &D1);

	const Type * GetPoints() const;
	const Type & GetPoint(const int index) const;
	Type & GetMutablePoint(const int index);

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

	void SplitToSubCurves(cBezierCurve<Type> *pSubCurve0, cBezierCurve<Type> *pSubCurve1,
		const float tSplitAt = 0.5f) const;

	float CalcLinearErrorBoundSq() const;
	float CalcClosestPointParameterLinear(const Type &p, float *pDistanceSq = NULL) const;
	float CalcClosestPointParameterAccurate(const Type &p, const float MaxErrSq, float *pDistanceSq = NULL) const;

private:
	Type m_Points[4];
};

typedef cBezierCurve<float> cBezierCurve1;
typedef cBezierCurve<cVec2> cBezierCurve2;
typedef cBezierCurve<cVec3> cBezierCurve3;

//-------------------------------------------------------------------------------------------------------------
// cBezierCurve<Type>::SetFromPoints
//-------------------------------------------------------------------------------------------------------------
template<class Type>
inline void cBezierCurve<Type>::SetFromPoints(const Type &P0, const Type &P1, const Type &P2, const Type &P3) {
	m_Points[0] = P0;
	m_Points[1] = P1;
	m_Points[2] = P2;
	m_Points[3] = P3;
}
											 
//-----------------------------------------------------------------------------------------------------------
// cBezierCurve<Type>::SetFromEnds
//-----------------------------------------------------------------------------------------------------------
template<class Type>
inline void cBezierCurve<Type>::SetFromEnds(const Type &P0, const Type &D0, const Type &P1, const Type &D1) {
	m_Points[0] = P0;
	m_Points[1] = D0 * 1.0f / 3.0f + P0;
	m_Points[2] = P1 - D1 * 1.0f / 3.0f;
	m_Points[3] = P1;
}

//-----------------------------------------------------------------------------
// cBezierCurve<Type>::GetPoints
//-----------------------------------------------------------------------------
template<class Type>
inline const Type * cBezierCurve<Type>::GetPoints() const {
	return m_Points;
}

//-----------------------------------------------------------------------------
// cBezierCurve<Type>::GetPoint
//-----------------------------------------------------------------------------
template<class Type>
inline const Type & cBezierCurve<Type>::GetPoint(const int index) const {
	assert(index >= 0 && index < 4);
	return m_Points[index];
}

//-----------------------------------------------------------------------------
// cBezierCurve<Type>::GetMutablePoint
//-----------------------------------------------------------------------------
template<class Type>
inline Type & cBezierCurve<Type>::GetMutablePoint(const int index) {
	assert(index >= 0 && index < 4);
	return m_Points[index];
}

//-----------------------------------------------------------------------------
// cBezierCurve<Type>::GetValue
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cBezierCurve<Type>::GetValue(const float t) const {
	float s = 1.0f - t;
	float B0 = s * s * s;
	float B1 = 3.0f * t * s * s;
	float B2 = 3.0f * s * t * t;
	float B3 = t * t * t;
	
	return B0 * m_Points[0] + B1 * m_Points[1] + B2 * m_Points[2] + B3 * m_Points[3];
}

//-----------------------------------------------------------------------------
// cBezierCurve<Type>::GetValue0
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cBezierCurve<Type>::GetValue0() const {
	return m_Points[0];
}

//-----------------------------------------------------------------------------
// cBezierCurve<Type>::GetValue1
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cBezierCurve<Type>::GetValue1() const {
	return m_Points[3];
}

//-----------------------------------------------------------------------------
// cBezierCurve<Type>::Get1stDerivative
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cBezierCurve<Type>::Get1stDerivative(const float t) const {
	float t2 = t * t;
	float B0 = - 3.0f + 6.0f * t - 3.0f * t2;
	float B1 = 3.0f - 12.0f * t + 9.0f * t2;
	float B2 = 6.0f * t - 9.0f * t2;
	float B3 = 3.0f * t2;
	
	return B0 * m_Points[0] + B1 * m_Points[1] + B2 * m_Points[2] + B3 * m_Points[3];
}

//-----------------------------------------------------------------------------
// cBezierCurve<Type>::Get1stDerivative0
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cBezierCurve<Type>::Get1stDerivative0() const {
	return - 3.0f * m_Points[0] + 3.0f * m_Points[1];
}

//-----------------------------------------------------------------------------
// cBezierCurve<Type>::Get1stDerivative1
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cBezierCurve<Type>::Get1stDerivative1() const {
	return - 3.0f * m_Points[2] + 3.0f * m_Points[3];
}

//-----------------------------------------------------------------------------
// cBezierCurve<Type>::Get2ndDerivative
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cBezierCurve<Type>::Get2ndDerivative(const float t) const {
	float B0 = 6.0f - 6.0f * t;
	float B1 = - 12.0f + 18.0f * t;
	float B2 = 6.0f - 18.0f * t;
	float B3 = 6.0f * t;
	
	return B0 * m_Points[0] + B1 * m_Points[1] + B2 * m_Points[2] + B3 * m_Points[3];
}

//-----------------------------------------------------------------------------
// cBezierCurve<Type>::Get2ndDerivative0
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cBezierCurve<Type>::Get2ndDerivative0() const {
	return 6.0f * m_Points[0] - 12.0f * m_Points[1] + 6.0f * m_Points[2];
}

//-----------------------------------------------------------------------------
// cBezierCurve<Type>::Get2ndDerivative1
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cBezierCurve<Type>::Get2ndDerivative1() const {
	return 6.0f * m_Points[1] - 12.0f * m_Points[2] + 6.0f * m_Points[3];
}

//-----------------------------------------------------------------------------
// cBezierCurve<Type>::Get3rdDerivative
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cBezierCurve<Type>::Get3rdDerivative() const {
	return - 6.0f * m_Points[0] + 18.0f * m_Points[1] - 18.0f * m_Points[2] + 6.0f * m_Points[3];
}

//-----------------------------------------------------------------------------
// cBezierCurve<Type>::GetBounds
//-----------------------------------------------------------------------------
template<class Type>
inline const cBounds cBezierCurve<Type>::GetBounds() const {
	return cBounds::FromPoints(m_Points, 4);
}

//-----------------------------------------------------------------------------
// cBezierCurve<Type>::GetBoundSphere
//-----------------------------------------------------------------------------
template<class Type>
inline const cSphere cBezierCurve<Type>::GetBoundingSphere() const {
	return cSphere::FromPoints(m_Points, 4);
}

//-----------------------------------------------------------------------------------------------------
// cBezierCurve<Type>::CalcLength
//-----------------------------------------------------------------------------------------------------
template<class Type>
inline float cBezierCurve<Type>::CalcLength(const float tFrom, const float tTo, const float dt) const {
	assert(cMath::IsZeroToOneEps(tFrom));
	assert(cMath::IsZeroToOneEps(tTo));
	
	Type p[2];
	p[0] = GetValue(tFrom);
	float Length = 0.0f;
	int index = 1;

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
// cBezierCurve<Type>::CalcLength
//-----------------------------------------------------------------------------
template<class Type>
inline float cBezierCurve<Type>::CalcLength(const float dt) const {
	return CalcLength(0.0f, 1.0f, dt);
}

//-------------------------------------------------------------------------------------------------
// cBezierCurve<Dim>::CalcParameterForLength
//-------------------------------------------------------------------------------------------------
template<class Type>
inline float cBezierCurve<Type>::CalcParameterForLength(const float Length, const float dt) const {
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

//-------------------------------------------------------------------------------------------------------------
// cBezierCurve<Type>::SplitToSubCurves
//-------------------------------------------------------------------------------------------------------------
template<class Type>
inline void cBezierCurve<Type>::SplitToSubCurves(cBezierCurve<Type> *pSubCurve0, cBezierCurve<Type> *pSubCurve1,
												 const float tSplitAt) const {
	Type b01 = Type::Lerp(m_Points[0], m_Points[1], tSplitAt);
	Type b12 = Type::Lerp(m_Points[1], m_Points[2], tSplitAt);
	Type b23 = Type::Lerp(m_Points[2], m_Points[3], tSplitAt);
	Type b012 = Type::Lerp(b01, b12, tSplitAt);
	Type b123 = Type::Lerp(b12, b23, tSplitAt);
	Type b0123 = Type::Lerp(b012, b123, tSplitAt);
	
	pSubCurve0->SetFromPoints(m_Points[0], b01, b012, b0123);
	pSubCurve1->SetFromPoints(b0123, b123, b23, m_Points[3]);
}

//-----------------------------------------------------------------------------
// cBezierCurve<Type>::CalcLinearErrorBoundSq
//-----------------------------------------------------------------------------
template<class Type>
inline float cBezierCurve<Type>::CalcLinearErrorBoundSq() const {
	const Type P1 = Type::Lerp(m_Points[0], m_Points[3], 1.0f / 3.0f);
	const Type P2 = Type::Lerp(m_Points[0], m_Points[3], 2.0f / 3.0f);
	return cMath::Max(Type::DistanceSq(P1, m_Points[1]), Type::DistanceSq(P2, m_Points[2]));
}

//---------------------------------------------------------------------------------------------------------
// cBezierCurve<Type>::CalcClosestPointParameterLinear
//---------------------------------------------------------------------------------------------------------
template<class Type>
inline float cBezierCurve<Type>::CalcClosestPointParameterLinear(const Type &p, float *pDistanceSq) const {
	float Scale;
	float d2 = p.DistanceToLineSegSq(GetValue0(), GetValue1(), &Scale);
	if(pDistanceSq) {
		*pDistanceSq = d2;
	}
	return Scale;
}

//---------------------------------------------------------------------------------------------------------------------------------
// cBezierCurve<Type>::CalcClosestPointParameterAccurate
//---------------------------------------------------------------------------------------------------------------------------------
template<class Type>
struct cBezierCurveClosestPointRecurser {
	cBezierCurve<Type> SubCurve;
	float tStart, tEnd;
};
template<class Type>
inline float cBezierCurve<Type>::CalcClosestPointParameterAccurate(const Type &p, const float MaxErrSq, float *pDistanceSq) const {
	cList<cBezierCurveClosestPointRecurser<Type> > Stack;
	
	float CurBestParameter = 0.0f;
	float CurBestDistance2 = Type::DistanceSq(p, m_Points[0]);

	cBezierCurveClosestPointRecurser<Type> R;
	R.SubCurve = *this;
	R.tStart = 0.0f;
	R.tEnd = 1.0f;
	Stack.Add(R);

	while(Stack.Count() > 0) {
		const cBezierCurveClosestPointRecurser<Type> &Cur = Stack.GetBack();
		
		cBounds bb = Cur.SubCurve.GetBounds();
		float dToBB2 = bb.DistanceToPointSq(p);

		if(dToBB2 >= CurBestDistance2) {
			Stack.RemoveBack();
			continue;
		}

		const float dHeadSq = Type::DistanceSq(p, Cur.SubCurve.GetValue0());
		const float dTailSq = Type::DistanceSq(p, Cur.SubCurve.GetValue1());
		
		float ErrSq = Cur.SubCurve.CalcLinearErrorBoundSq();
		if(ErrSq <= MaxErrSq) {
			float Scale;
			float dToLineSeg2 = p.DistanceToLineSegSq(Cur.SubCurve.GetValue0(), Cur.SubCurve.GetValue1(), &Scale);
			assert(dToBB2 <= dToLineSeg2 + cMath::SpaceEpsilon);

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

		R.SubCurve.SplitToSubCurves(&Stack[st0].SubCurve, &Stack[st1].SubCurve, 0.5f);

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