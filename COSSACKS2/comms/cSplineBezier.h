#pragma once

template<class Type>
class cSplineBezier {
public:
	enum eEnds {
		E_OPEN,
		E_CLOSED
	};
	
	struct Knot {
		Type Point;
		float Time;
	};
	
	void SetFromPoints(const Type *pPoints, const int nPoints, const typename cSplineBezier<Type>::eEnds Ends = E_OPEN);
	void SetFromKnots(const typename cSplineBezier<Type>::Knot *pKnots, const int nKnots, const typename cSplineBezier<Type>::eEnds Ends = E_OPEN);
	void SetUniformSampled(const cSplineBezier<Type> &Src, const int nKnots);
	void SetAdaptiveSampled(const cSplineBezier<Type> &Src, const float MaxErrSq);
	
	void Copy(const cSplineBezier<Type> &Src);
	
	struct Place {
		int nSegment;
		float LocalTime;
	};
	
	int GetNumSegments() const;
	const cSegBezier<Type> & GetCurve(const int nSegment) const;
	cSegBezier<Type> & GetMutableCurve(const int nSegment);
	
	struct Segment {
		cSegBezier<Type> Curve;
		float AccumTime; // Spline time at segment end.
		
		cSphere BSphere;
		float AccumLength; // Spline length at segment end.
	};

	const typename cSplineBezier<Type>::Segment & GetSegment(const int nSegment) const;

	const cBounds & GetBounds() const;
	
	float GetTotalLength() const;
	
	void GetSegmentTimeRange(const int nSegment, float *tStart, float *tEnd) const;
	void GetSegmentLengthRange(const int nSegment, float *lStart, float *lEnd) const;
	
	const typename cSplineBezier<Type>::Place GetPlaceByTime(const float t) const;
	const typename cSplineBezier<Type>::Place GetPlaceByLength(const float l) const;
	const typename cSplineBezier<Type>::Place GetPlaceByLengthAccurate(const float l) const;
	
	float GetTimeFromPlace(const typename cSplineBezier<Type>::Place &P) const;
	float GetLengthFromPlace(const typename cSplineBezier<Type>::Place &P) const;
	
	const typename cSplineBezier<Type>::Place CalcClosestPointPlaceLinear(const Type &p) const;
	const typename cSplineBezier<Type>::Place CalcClosestPointPlaceAccurate(const Type &p, const float MaxErrSq) const;
	
	const Type GetValue(const float t) const;
	const Type GetValue(const typename cSplineBezier<Type>::Place &P) const;
	
	const Type GetDerivativeLocal(const float t) const;
	const Type GetDerivativeLocal(const typename cSplineBezier<Type>::Place &P) const;
	
	const Type GetDerivativeGlobal(const float t) const;
	const Type GetDerivativeGlobal(const typename cSplineBezier<Type>::Place &P) const;

	int IndexOfSegment1(const int nSegment0) const;
	int IndexOfSegment0(const int nSegment1) const;
	
	bool IsClosingEP(const int nSegment0, const int nSegment1) const;
	bool IsEmpty() const;

	void RemoveEP(const int nSegment0, const int nSegment1);
	void AddEP(const typename cSplineBezier<Type>::Place &P);

	void TranslateAll(const Type &);
	void TranslateEP0(const int nSegment1, const Type &t);
	void TranslateEP1(const int nSegment0, const Type &t);
	void TranslateCV0(const int nSegment1, const Type &t);
	void TranslateCV1(const int nSegment0, const Type &t);
	
	const typename cSplineBezier<Type>::eEnds GetEnds() const;
	bool IsOpen() const;
	bool IsClosed() const;
	void Open();
	void Close();

private:
	void Update();

	static int Index(const int i, const int nSize, const typename cSplineBezier<Type>::eEnds Ends);
	
	void AdjustCV0(const int nSegment1);
	void AdjustCV1(const int nSegment0);
	
	void SetAdaptiveSampled_RecursiveAdd(const cSegBezier<Type> &C, const float t0, const float t1, const float MaxErrSq);
	
	void ParameterizeByArcLength();
	
	cList<Segment> m_Segments;
	cBounds m_Bounds;

	eEnds m_Ends;
};

typedef cSplineBezier<cVec2> cSplineBezier2;
typedef cSplineBezier<cVec3> cSplineBezier3;

//--------------------------------------------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::SetFromPoints
//--------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::SetFromPoints(const Type *pPoints, const int nPoints, const typename cSplineBezier<Type>::eEnds Ends) {
	m_Ends = Ends;
	
	int nCurves = nPoints - 1;
	cAssert(nCurves > 0);

	m_Segments.Free();
	m_Segments.SetCount(nCurves);
	
	for(int i = 0; i < nCurves; i++) {
		Type d0 = (pPoints[i + 1] - pPoints[Index(i - 1, nPoints, Ends)]) / 2.0f;
		Type d1 = (pPoints[Index(i + 2, nPoints, Ends)] - pPoints[i]) / 2.0f;
		
		m_Segments[i].Curve.SetFromEnds(pPoints[i], d0, pPoints[i + 1], d1);
		m_Segments[i].AccumTime = (i + 1) / float(nCurves);
	}

	Update();
}

//-------------------------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::Index
//-------------------------------------------------------------------------------------------------------------------
template<class Type>
inline int cSplineBezier<Type>::Index(const int i, const int nSize, const typename cSplineBezier<Type>::eEnds Ends) {
	cAssert(i >= -1 && i <= nSize);

	if(i < 0) {
		if(E_OPEN == Ends) {
			return 0;
		} else {
			return nSize - 2;
		}
	} else if(i >= nSize) {
		if(E_OPEN == Ends) {
			return nSize - 1;
		} else {
			return 1;
		}
	} else {
		return i;
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::SetFromKnots
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::SetFromKnots(const typename cSplineBezier<Type>::Knot *pKnots, const int nKnots, const typename cSplineBezier<Type>::eEnds Ends) {
	m_Ends = Ends;

	int nCurves = nKnots - 1;
	cAssert(nCurves > 0);

	m_Segments.Free();
	m_Segments.SetCount(nCurves);
	
	cAssert(pKnots[0].Time == 0.0f);
	cAssert(pKnots[nKnots - 1].Time == 1.0f);
	
	for(int i = 0; i < nCurves; i++) {
		Type d0 = (pKnots[i + 1].Point - pKnots[Index(i - 1, nKnots, Ends)].Point) / 2.0f;
		Type d1 = (pKnots[Index(i + 2, nKnots, Ends)].Point - pKnots[i].Point) / 2.0f;
		
		//float TimeScale = (pKnots[i + 1].Time - pKnots[i].Time) / float(nCurves);

		//d0 *= TimeScale;
		//d1 *= TimeScale;

		m_Segments[i].Curve.SetFromEnds(pKnots[i].Point, d0, pKnots[i + 1].Point, d1);
		m_Segments[i].AccumTime = pKnots[i + 1].Time;
	}

	Update();
}

//----------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::SetUniformSampled
//----------------------------------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::SetUniformSampled(const cSplineBezier<Type> &Src, const int nKnots) {
	int nCurves = nKnots - 1;
	cAssert(nCurves > 0);

	float Len = Src.GetTotalLength();
	float LenPerSeg = Len / float(nCurves);
	float TimePerSeg = 1.0f / float(nCurves);
	
	m_Segments.Free();
	m_Segments.SetCount(nCurves);

	int index = 1;
	Type p[2], d[2];
	p[0] = Src.GetValue(0.0f);
	d[0] = Src.GetDerivativeGlobal(0.0f);

	for(int i = 0; i < nCurves; i++) {
		float AccumLength = (i + 1) * LenPerSeg;
		Place P = Src.GetPlaceByLengthAccurate(AccumLength);
		p[index] = Src.GetValue(P);
		d[index] = Src.GetDerivativeGlobal(P);

		const Type &p0 = p[index ^ 1];
		const Type &p1 = p[index];

		const Type d0 = d[index ^ 1] * TimePerSeg;
		const Type d1 = d[index] * TimePerSeg;

		m_Segments[i].Curve.SetFromEnds(p0, d0, p1, d1);
		m_Segments[i].AccumTime = (i + 1) * TimePerSeg;

		index ^= 1;
	}

	Update();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::SetAdaptiveSampled_RecursiveAdd
//---------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::SetAdaptiveSampled_RecursiveAdd(const cSegBezier<Type> &C, const float t0, const float t1, const float MaxErrSq) {
	if(C.CalcLinearErrorBoundSq() <= MaxErrSq) {
		cAssert(m_Segments.Count() > 0 || t0 == 0.0f);
		cAssert(m_Segments.Count() == 0 || m_Segments.GetBack().AccumTime == t0);
		Segment S;
		S.Curve = C;
		S.AccumTime = t1;
		m_Segments.Add(S);
	} else {
		cSegBezier<Type> C0, C1;
		C.SplitToSubCurves(&C0, &C1);
		float tMid = (t0 + t1) * 0.5f;

		SetAdaptiveSampled_RecursiveAdd(C0, t0, tMid, MaxErrSq);
		SetAdaptiveSampled_RecursiveAdd(C1, tMid, t1, MaxErrSq);
	}
}

//---------------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::SetAdaptiveSampled
//---------------------------------------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::SetAdaptiveSampled(const cSplineBezier<Type> &Src, const float MaxErrSq) {
	m_Segments.Free();
	for(int i = 0; i < Src.GetNumSegments(); i++) {
		float tStart, tEnd;
		Src.GetSegmentTimeRange(i, &tStart, &tEnd);
		SetAdaptiveSampled_RecursiveAdd(Src.GetCurve(i), tStart, tEnd, MaxErrSq);
	}
	Update();
}

//-----------------------------------------------------------------------------
// cSplineBezier<Type>::Copy
//-----------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::Copy(const cSplineBezier<Type> &Src) {
	m_Segments.Copy(Src.m_Segments);
	m_BBox = Src.m_BBox;
	m_Ends = Src.m_Ends;
}

//-----------------------------------------------------------------------------
// cSplineBezier<Type>::GetNumSegments
//-----------------------------------------------------------------------------
template<class Type>
inline int cSplineBezier<Type>::GetNumSegments() const {
	return m_Segments.Count();
}

//-----------------------------------------------------------------------------------------
// cSplineBezier<Type>::GetCurve
//-----------------------------------------------------------------------------------------
template<class Type>
inline const cSegBezier<Type> & cSplineBezier<Type>::GetCurve(const int nSegment) const {
	return m_Segments[nSegment].Curve;
}

//------------------------------------------------------------------------------------
// cSplineBezier<Type>::GetMutableCurve
//------------------------------------------------------------------------------------
template<class Type>
inline cSegBezier<Type> & cSplineBezier<Type>::GetMutableCurve(const int nSegment) {
	return m_Segments[nSegment].Curve;
}

//--------------------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::GetSegment
//--------------------------------------------------------------------------------------------------------------
template<class Type>
inline const typename cSplineBezier<Type>::Segment & cSplineBezier<Type>::GetSegment(const int nSegment) const {
	return m_Segments[nSegment];
}

//-----------------------------------------------------------------------------
// cSplineBezier<Type>::GetBounds
//-----------------------------------------------------------------------------
template<class Type>
inline const cBounds & cSplineBezier<Type>::GetBounds() const {
	return m_Bounds;
}

//-----------------------------------------------------------------------------
// cSplineBezier<Type>::GetTotalLength
//-----------------------------------------------------------------------------
template<class Type>
inline float cSplineBezier<Type>::GetTotalLength() const {
	if(!m_Segments.Count()) {
		return 0.0f;
	}
	return m_Segments.GetBack().AccumLength;
}

//----------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::GetSegmentTimeRange
//----------------------------------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::GetSegmentTimeRange(int nSegment, float *tStart, float *tEnd) const {
	cAssert(nSegment >= 0 && nSegment < m_Segments.Count());
	
	if(nSegment == 0) {
		*tStart = 0.0f;
	} else {
		*tStart = m_Segments[nSegment - 1].AccumTime;
	}
	*tEnd = m_Segments[nSegment].AccumTime;
}

//------------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::GetSegmentLengthRange
//------------------------------------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::GetSegmentLengthRange(int nSegment, float *lStart, float *lEnd) const {
	cAssert(nSegment >= 0 && nSegment < m_Segments.Count());
	
	if(nSegment == 0) {
		*lStart = 0.0f;
	} else {
		*lStart = m_Segments[nSegment - 1].AccumLength;
	}
	*lEnd = m_Segments[nSegment].AccumLength;
}

//---------------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::GetPlaceByTime
//---------------------------------------------------------------------------------------------------------
template<class Type>
inline const typename cSplineBezier<Type>::Place cSplineBezier<Type>::GetPlaceByTime(const float t) const {
	cAssert(cMath::IsZeroToOneExact(t));
	
	Place P;
	ZeroMemory(&P, sizeof(P));

	if(m_Segments.Count()) {
		cAssert(m_Segments.GetBack().AccumTime == 1.0f);

		int nSegment = 0;
		while(t > m_Segments[nSegment].AccumTime) {
			nSegment++;
		}

		float tStart, tEnd;
		GetSegmentTimeRange(nSegment, &tStart, &tEnd);
		cAssert(t >= tStart && t <= tEnd);
		P.nSegment = nSegment;
		P.LocalTime = cMath::LerperClamp01(tStart, tEnd, t);
	}
	return P;
}

//-----------------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::GetPlaceByLength
//-----------------------------------------------------------------------------------------------------------
template<class Type>
inline const typename cSplineBezier<Type>::Place cSplineBezier<Type>::GetPlaceByLength(const float l) const {
	const float L = cMath::Clamp(l, 0.0f, GetTotalLength());

	Place P;
	ZeroMemory(&P, sizeof(P));
	
	if(m_Segments.Count()) {
		int nSegment = 0;

		while(L > m_Segments[nSegment].AccumLength) {
			nSegment++;
		}

		float lStart, lEnd;
		GetSegmentLengthRange(nSegment, &lStart, &lEnd);
		cAssert(L >= lStart && L <= lEnd);
		P.nSegment = nSegment;
		P.LocalTime = cMath::LerperClamp01(lStart, lEnd, L);
	}
	return P;
}

//-------------------------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::GetPlaceByLengthAccurate
//-------------------------------------------------------------------------------------------------------------------
template<class Type>
inline const typename cSplineBezier<Type>::Place cSplineBezier<Type>::GetPlaceByLengthAccurate(const float l) const {
	const float L = cMath::Clamp(l, 0.0f, GetTotalLength());
	
	Place P = GetPlaceByLength(L);
	float lStart, lEnd;
	GetSegmentLengthRange(P.nSegment, &lStart, &lEnd);
	cAssert(L >= lStart && L <= lEnd);
	P.LocalTime = m_Segments[P.nSegment].Curve.CalcParameterForLength(L - lStart);

	return P;
}

//------------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::GetTimeFromPlace
//------------------------------------------------------------------------------------------------------
template<class Type>
inline float cSplineBezier<Type>::GetTimeFromPlace(const typename cSplineBezier<Type>::Place &P) const {
	if(!m_Segments.Count()) return 0.0f;

	float tStart, tEnd;
	GetSegmentTimeRange(P.nSegment, &tStart, &tEnd);

	return cMath::Lerp(tStart, tEnd, P.LocalTime);
}

//--------------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::GetLengthFromPlace
//--------------------------------------------------------------------------------------------------------
template<class Type>
inline float cSplineBezier<Type>::GetLengthFromPlace(const typename cSplineBezier<Type>::Place &P) const {
	if(!m_Segments.Count()) return 0.0f;

	float lStart, lEnd;
	GetSegmentLengthRange(P.nSegment, &lStart, &lEnd);

	return lStart + cMath::Lerp(lStart, lEnd, P.LocalTime);
}

//----------------------------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::CalcClosestPointPlaceLinear
//----------------------------------------------------------------------------------------------------------------------
template<class Type>
inline const typename cSplineBezier<Type>::Place cSplineBezier<Type>::CalcClosestPointPlaceLinear(const Type &p) const {
	float CurBestDistance2 = cMath::FloatMaxValue;
	
	Place P;
	ZeroMemory(&P, sizeof(P));

	for(int i = 0; i < m_Segments.Count(); i++) {
		const Segment &S = m_Segments[i];
		float d2;
		float Time = S.Curve.CalcClosestPointParameterLinear(p, &d2);
		if(d2 < CurBestDistance2) {
			CurBestDistance2 = d2;
			P.LocalTime = Time;
			P.nSegment = i;
		}
	}
	
	cAssert(CurBestDistance2 != cMath::FloatMaxValue);
	return P;
}

//----------------------------------------------------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::CalcClosestPointPlaceAccurate
//----------------------------------------------------------------------------------------------------------------------------------------------
struct cSplineBezierClosestPointRef {
	int nSegment;
	float MinDistance2;
	static int Cmp(const cSplineBezierClosestPointRef *r0, const cSplineBezierClosestPointRef *r1) {
		return (int)cMath::Sign(r0->MinDistance2 - r1->MinDistance2);
	}
};
template<class Type>
inline const typename cSplineBezier<Type>::Place cSplineBezier<Type>::CalcClosestPointPlaceAccurate(const Type &p, const float MaxErrSq) const {
	cAssert(m_Segments.Count() > 0);
	
	cList<cSplineBezierClosestPointRef> Refs;
	Refs.SetCount(m_Segments.Count());
	
	Place CurBestPlace;
	CurBestPlace.LocalTime = 0.0f;
	CurBestPlace.nSegment = 0;
	
	float CurBestDistance2 = cMath::FloatMaxValue;

	for(int i = 0; i < m_Segments.Count(); i++) {
		Refs[i].nSegment = i;
		Refs[i].MinDistance2 = cMath::Square(m_Segments[i].BSphere.DistanceToPoint(p));
	}

	Refs.Sort(cSplineBezierClosestPointRef::Cmp);
//	cAssert(Refs[0].MinDistance2 <= Refs[1].MinDistance2);

	for(int i = 0; i < m_Segments.Count(); i++) {
		if(Refs[i].MinDistance2 >= CurBestDistance2) {
			break;
		}

		int nSegment = Refs[i].nSegment;
		const Segment &S = m_Segments[nSegment];
		float d2;
		float Time = S.Curve.CalcClosestPointParameterAccurate(p, MaxErrSq, &d2);
		//cAssert(Refs[i].MinDistance2 <= d2 + cMath::Epsilon);
		if(d2 < CurBestDistance2) {
			CurBestDistance2 = d2;
			CurBestPlace.LocalTime = Time;
			CurBestPlace.nSegment = nSegment;
		}
	}
	cAssert(CurBestDistance2 != cMath::FloatMaxValue);

	return CurBestPlace;
}

//-----------------------------------------------------------------------------
// cSplineBezier<Type>::GetValue
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cSplineBezier<Type>::GetValue(const float t) const {
	Place P = GetPlaceByTime(t);
	return GetCurve(P.nSegment).GetValue(P.LocalTime);
}

//---------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::GetValue
//---------------------------------------------------------------------------------------------------
template<class Type>
inline const Type cSplineBezier<Type>::GetValue(const typename cSplineBezier<Type>::Place &P) const {
	return GetCurve(P.nSegment).GetValue(P.LocalTime);
}

//------------------------------------------------------------------------------
// cSplineBezier<Type>::GetDerivativeLocal
//------------------------------------------------------------------------------
template<class Type>
inline const Type cSplineBezier<Type>::GetDerivativeLocal(const float t) const {
	Place P = GetPlaceByTime(t);
	return GetCurve(P.nSegment).Get1stDerivative(P.LocalTime);
}

//-------------------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::GetDerivativeLocal
//-------------------------------------------------------------------------------------------------------------
template<class Type>
inline const Type cSplineBezier<Type>::GetDerivativeLocal(const typename cSplineBezier<Type>::Place &P) const {
	return GetCurve(P.nSegment).Get1stDerivative(P.LocalTime);
}

//-------------------------------------------------------------------------------
// cSplineBezier<Type>::GetDerivativeGlobal
//-------------------------------------------------------------------------------
template<class Type>
inline const Type cSplineBezier<Type>::GetDerivativeGlobal(const float t) const {
	Place P = GetPlaceByTime(t);
	return GetDerivativeGlobal(P);
}

//--------------------------------------------------------------------------------------------------------------
// cSplineBezier<Type>::GetDerivativeGlobal
//--------------------------------------------------------------------------------------------------------------
template<class Type>
inline const Type cSplineBezier<Type>::GetDerivativeGlobal(const typename cSplineBezier<Type>::Place &P) const {
	float tStart, tEnd;
	GetSegmentTimeRange(P.nSegment, &tStart, &tEnd);
	float tRange = tEnd - tStart;
	cAssert(tRange <= 1.0f);
	Type DerivLocal = GetCurve(P.nSegment).Get1stDerivative(P.LocalTime);
	return DerivLocal * (1.0f / tRange);
}

//-----------------------------------------------------------------------------
// cSplineBezier<Type>::IndexOfSegment1
//-----------------------------------------------------------------------------
template<class Type>
inline int cSplineBezier<Type>::IndexOfSegment1(const int nSegment0) const {
	cAssert(nSegment0 >= 0 && nSegment0 < m_Segments.Count());

	int nSegment1 = nSegment0 + 1;
	if(nSegment1 >= m_Segments.Count()) {
		if(E_CLOSED == m_Ends) {
			nSegment1 = 0;
		} else {
			nSegment1 = -1;
		}
	}

	return nSegment1;
}

//-----------------------------------------------------------------------------
// cSplineBezier<Type>::IndexOfSegment0
//-----------------------------------------------------------------------------
template<class Type>
inline int cSplineBezier<Type>::IndexOfSegment0(const int nSegment1) const {
	cAssert(nSegment1 >= 0 && nSegment1 < m_Segments.Count());

	int nSegment0 = nSegment1 - 1;
	if(nSegment0 < 0) {
		if(E_CLOSED == m_Ends) {
			nSegment0 = m_Segments.Count() - 1;
		} else {
			nSegment0 = -1;
		}
	}

	return nSegment0;
}

//--------------------------------------------------------------------------------------------
// cSplineBezier<Type>::IsClosingEP
//--------------------------------------------------------------------------------------------
template<class Type>
inline bool cSplineBezier<Type>::IsClosingEP(const int nSegment0, const int nSegment1) const {
	return IsClosed() && 0 == nSegment1 && m_Segments.Count() - 1 == nSegment0;
}

//-----------------------------------------------------------------------------
// cSplineBezier<Type>::IsEmpty
//-----------------------------------------------------------------------------
template<class Type>
inline bool cSplineBezier<Type>::IsEmpty() const {
	return (IsOpen() && m_Segments.Count() == 0) || (IsClosed() && m_Segments.Count() <= 1);
}

//-----------------------------------------------------------------------------------
// cSplineBezier<Type>::RemoveEP
//-----------------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::RemoveEP(const int nSegment0, const int nSegment1) {
	bool Start = nSegment0 < 0 && nSegment1 >= 0;
	bool End = nSegment0 >= 0 && nSegment1 < 0;
	bool Between = nSegment0 >= 0 && nSegment1 >= 0;

	if(Start || End) {
		cAssert(E_OPEN == m_Ends);
		if(Start) {
			cAssert(0 == nSegment1);
			m_Segments.RemoveAt(0);
		} else if(End) {
			cAssert(m_Segments.Count() - 1 == nSegment0);
			m_Segments.RemoveBack();
		}
		if(m_Segments.Count()) {
			Update();
			ParameterizeByArcLength();
		}
	} else if(Between) {
		cAssert(IndexOfSegment0(nSegment1) == nSegment0);
		cAssert(IndexOfSegment1(nSegment0) == nSegment1);

		if(0 == nSegment1 && m_Segments.Count() - 1 == nSegment0) { // Removing closing point:
			cAssert(E_CLOSED == m_Ends);
			
			const Segment &S0 = m_Segments.GetBack();
			Segment &S1 = m_Segments[0];

			Segment S;
			S.Curve.SetFromEnds(S0.Curve.GetPoint(0), S0.Curve.GetDerivative0(), S1.Curve.GetPoint(3), S1.Curve.GetDerivative1());

			S1 = S;
			m_Segments.RemoveAt(nSegment0);

			if(m_Segments.Count()) {
				Update();
				ParameterizeByArcLength();
			}
		} else {
			cAssert(nSegment1 > nSegment0);

			Segment &S0 = m_Segments[nSegment0];
			const Segment &S1 = m_Segments[nSegment1];
			
			Segment S;
			S.AccumTime = S1.AccumTime;
			S.Curve.SetFromEnds(S0.Curve.GetPoint(0), S0.Curve.GetDerivative0(), S1.Curve.GetPoint(3), S1.Curve.GetDerivative1());
			
			S0 = S;
			m_Segments.RemoveAt(nSegment1);

			if(m_Segments.Count()) {
				Update();
			}
		}
	}
}

//------------------------------------------------------------------------------------
// cSplineBezier<Type>::AddEP
//------------------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::AddEP(const typename cSplineBezier<Type>::Place &P) {
	Segment &S = m_Segments[P.nSegment];

	Segment S0, S1;
	S.Curve.SplitToSubCurves(&S0.Curve, &S1.Curve, P.LocalTime);

	const float t0 = P.nSegment > 0 ? m_Segments[P.nSegment - 1].AccumTime : 0.0f;
	S1.AccumTime = S.AccumTime;

	const float L0 = S0.Curve.CalcLength();
	const float L1 = S1.Curve.CalcLength();
	S0.AccumTime = cMath::Lerp(t0, S1.AccumTime, L0 / (L0 + L1));
	S = S0;
	m_Segments.InsertAt(P.nSegment + 1, S1);

	Update();
}

//-----------------------------------------------------------------------------
// cSplineBezier<Type>::TranslateAll
//-----------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::TranslateAll(const Type &t) {
	for(int i = 0; i < m_Segments.Count(); i++) {
		m_Segments[i].Curve.Translate(t);
		m_Segments[i].BSphere.Translate(t);
	}
	m_Bounds.Translate(t);
}

//---------------------------------------------------------------------------------
// cSplineBezier<Type>::TranslateEP0
//---------------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::TranslateEP0(const int nSegment1, const Type &t) {
	m_Segments[nSegment1].Curve.GetMutablePoint(0) += t; // EP0
	m_Segments[nSegment1].Curve.GetMutablePoint(1) += t; // CV0
	const int nSegment0 = IndexOfSegment0(nSegment1);
	if(nSegment0 >= 0) {
		m_Segments[nSegment0].Curve.GetMutablePoint(3) += t; // EP1
		m_Segments[nSegment0].Curve.GetMutablePoint(2) += t; // CV1
	}
	Update();
}

//---------------------------------------------------------------------------------
// cSplineBezier<Type>::TranslateEP1
//---------------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::TranslateEP1(const int nSegment0, const Type &t) {
	m_Segments[nSegment0].Curve.GetMutablePoint(3) += t; // EP1
	m_Segments[nSegment0].Curve.GetMutablePoint(2) += t; // CV1
	const int nSegment1 = IndexOfSegment1(nSegment0);
	if(nSegment1 >= 0) {
		m_Segments[nSegment1].Curve.GetMutablePoint(0) += t; // EP0
		m_Segments[nSegment1].Curve.GetMutablePoint(1) += t; // CV0
	}
	Update();
}

//---------------------------------------------------------------------------------
// cSplineBezier<Type>::TranslateCV0
//---------------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::TranslateCV0(const int nSegment1, const Type &t) {
	m_Segments[nSegment1].Curve.GetMutablePoint(1) += t; // CV0
	const int nSegment0 = IndexOfSegment0(nSegment1);
	if(nSegment0 >= 0) {
		AdjustCV1(nSegment0);
	}
	Update();
}

//---------------------------------------------------------------------------------
// cSplineBezier<Type>::TranslateCV1
//---------------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::TranslateCV1(const int nSegment0, const Type &t) {
	m_Segments[nSegment0].Curve.GetMutablePoint(2) += t; // CV1
	const int nSegment1 = IndexOfSegment1(nSegment0);
	if(nSegment1 >= 0) {
		AdjustCV0(nSegment1);
	}
	Update();
}

//-------------------------------------------------------------------------------------
// cSplineBezier<Type>::GetEnds
//-------------------------------------------------------------------------------------
template<class Type>
inline const typename cSplineBezier<Type>::eEnds cSplineBezier<Type>::GetEnds() const {
	return m_Ends;
}

//-----------------------------------------------------------------------------
// cSplineBezier<Type>::IsOpen
//-----------------------------------------------------------------------------
template<class Type>
inline bool cSplineBezier<Type>::IsOpen() const {
	return E_OPEN == m_Ends;
}

//-----------------------------------------------------------------------------
// cSplineBezier<Type>::IsClosed
//-----------------------------------------------------------------------------
template<class Type>
inline bool cSplineBezier<Type>::IsClosed() const {
	return E_CLOSED == m_Ends;
}

//-----------------------------------------------------------------------------
// cSplineBezier<Type>::Open
//-----------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::Open() {
	m_Ends = E_OPEN;
}

//-----------------------------------------------------------------------------
// cSplineBezier<Type>::Close
//-----------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::Close() {
	if(m_Segments.Count() < 2) return;

	m_Ends = E_CLOSED;
	
	m_Segments.GetBack().Curve.GetMutablePoint(3) = m_Segments[0].Curve.GetPoint(0);
	AdjustCV1(m_Segments.Count() - 1);
}

//-----------------------------------------------------------------------------
// cSplineBezier<Type>::Update
//-----------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::Update() {
	if(!m_Segments.Count()) return;
	
	m_Bounds.Clear();
	
	float AccumLength = 0.0f;
	for(int i = 0; i < m_Segments.Count(); i++) {
		Segment &S = m_Segments[i];
		float L = S.Curve.CalcLength();
		AccumLength += L;
		S.AccumLength = AccumLength;
		S.BSphere = S.Curve.GetBoundingSphere();
		m_Bounds.AddBounds(S.Curve.GetBounds());
	}
}

//-----------------------------------------------------------------------------
// cSplineBezier<cQuat>::Update
//-----------------------------------------------------------------------------
inline void cSplineBezier<cQuat>::Update() {
	if(!m_Segments.Count()) return;
	
	m_Bounds.Clear();
	
	float AccumLength = 0.0f;
	for(int i = 0; i < m_Segments.Count(); i++) {
		Segment &S = m_Segments[i];
		S.AccumLength = 0.0f;
		S.BSphere.Clear();
	}
}

//-----------------------------------------------------------------------------
// cSplineBezier<Type>::AdjustCV0
//-----------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::AdjustCV0(const int nSegment1) {
	const int nSegment0 = IndexOfSegment0(nSegment1);
	if(nSegment0 < 0) return;
	const Type &CV1 = m_Segments[nSegment0].Curve.GetPoint(2);
	const Type &EP = m_Segments[nSegment1].Curve.GetPoint(0);
	Type &CV0 = m_Segments[nSegment1].Curve.GetMutablePoint(1);
	Type u = Type::Normalize(CV1 - EP);
	if(u.IsNormalized()) {
		CV0 = EP - u * Type::Distance(EP, CV0);
	}
}

//-----------------------------------------------------------------------------
// cSplineBezier<Type>::AdjustCV1
//-----------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::AdjustCV1(const int nSegment0) {
	const int nSegment1 = IndexOfSegment1(nSegment0);
	if(nSegment1 < 0) return;
	Type &CV1 = m_Segments[nSegment0].Curve.GetMutablePoint(2);
	const Type &EP = m_Segments[nSegment1].Curve.GetPoint(0);
	const Type &CV0 = m_Segments[nSegment1].Curve.GetPoint(1);
	Type u = Type::Normalize(CV0 - EP);
	if(u.IsNormalized()) {
		CV1 = EP - u * Type::Distance(EP, CV1);
	}
}

//-----------------------------------------------------------------------------
// cSplineBezier<Type>::ParameterizeByArcLength
//-----------------------------------------------------------------------------
template<class Type>
inline void cSplineBezier<Type>::ParameterizeByArcLength() {
	if(!m_Segments.Count()) return;

	float Scale = 1.0f / GetTotalLength();

	for(int i = 0; i < m_Segments.Count(); i++) {
		m_Segments[i].AccumTime = m_Segments[i].AccumLength * Scale;
	}
	cAssert(cMath::IsOne(m_Segments.GetBack().AccumTime));
	m_Segments.GetBack().AccumTime = 1.0f;
}