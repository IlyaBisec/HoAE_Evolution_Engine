#pragma once

template<class Type>
class cSpline {
public:
	virtual int AddValue(const float Time, const Type &Value);

	virtual void Free();
	virtual void Copy(const cSpline<Type> &Src);
protected:
	cList<float> m_Times;
	cList<Type> m_Values;

};

template<class Type>
class cBezierSpline {
public:
	enum eEnds {
		E_OPEN,
		E_CLOSED
	};
	
	struct Knot {
		Type Point;
		float Time;
	};
	
	void SetFromPoints(const Type *pPoints, const int nPoints, const typename cBezierSpline<Type>::eEnds Ends = E_OPEN);
	void SetFromKnots(const typename cBezierSpline<Type>::Knot *pKnots, const int nKnots, const typename cBezierSpline<Type>::eEnds Ends = E_OPEN);
	void SetUniformSampled(const cBezierSpline<Type> &Src, const int nKnots);
	void SetAdaptiveSampled(const cBezierSpline<Type> &Src, const float MaxErrSq);
	
	void Copy(const cBezierSpline<Type> &Src);
	
	struct Place {
		int nSegment;
		float LocalTime;
	};
	
	int GetNumSegments() const;
	const cBezierSeg<Type> & GetCurve(const int nSegment) const;
	cBezierSeg<Type> & GetMutableCurve(const int nSegment);
	
	struct Segment {
		cBezierSeg<Type> Curve;
		float AccumTime; // Spline time at segment end.
		
		cSphere BSphere;
		float AccumLength; // Spline length at segment end.
	};

	const typename cBezierSpline<Type>::Segment & GetSegment(const int nSegment) const;

	const cBounds & GetBounds() const;
	
	float GetTotalLength() const;
	
	void GetSegmentTimeRange(const int nSegment, float *tStart, float *tEnd) const;
	void GetSegmentLengthRange(const int nSegment, float *lStart, float *lEnd) const;
	
	const typename cBezierSpline<Type>::Place GetPlaceByTime(const float t) const;
	const typename cBezierSpline<Type>::Place GetPlaceByLength(const float l) const;
	const typename cBezierSpline<Type>::Place GetPlaceByLengthAccurate(const float l) const;
	
	float GetTimeFromPlace(const typename cBezierSpline<Type>::Place &P) const;
	float GetLengthFromPlace(const typename cBezierSpline<Type>::Place &P) const;
	
	const typename cBezierSpline<Type>::Place CalcClosestPointPlaceLinear(const Type &p) const;
	const typename cBezierSpline<Type>::Place CalcClosestPointPlaceAccurate(const Type &p, const float MaxErrSq) const;
	
	const Type GetValue(const float t) const;
	const Type GetValue(const typename cBezierSpline<Type>::Place &P) const;
	
	const Type GetDerivativeLocal(const float t) const;
	const Type GetDerivativeLocal(const typename cBezierSpline<Type>::Place &P) const;
	
	const Type GetDerivativeGlobal(const float t) const;
	const Type GetDerivativeGlobal(const typename cBezierSpline<Type>::Place &P) const;

	int IndexOfSegment1(const int nSegment0) const;
	int IndexOfSegment0(const int nSegment1) const;
	
	bool IsClosingEP(const int nSegment0, const int nSegment1) const;
	bool IsEmpty() const;

	void RemoveEP(const int nSegment0, const int nSegment1);
	void AddEP(const typename cBezierSpline<Type>::Place &P);

	void TranslateAll(const Type &);
	void TranslateEP0(const int nSegment1, const Type &t);
	void TranslateEP1(const int nSegment0, const Type &t);
	void TranslateCV0(const int nSegment1, const Type &t);
	void TranslateCV1(const int nSegment0, const Type &t);
	
	const typename cBezierSpline<Type>::eEnds GetEnds() const;
	bool IsOpen() const;
	bool IsClosed() const;
	void Open();
	void Close();

private:
	void Update();

	static int Index(const int i, const int nSize, const typename cBezierSpline<Type>::eEnds Ends);
	
	void AdjustCV0(const int nSegment1);
	void AdjustCV1(const int nSegment0);
	
	void SetAdaptiveSampled_RecursiveAdd(const cBezierSeg<Type> &C, const float t0, const float t1, const float MaxErrSq);
	
	void ParameterizeByArcLength();
	
	cList<Segment> m_Segments;
	cBounds m_Bounds;

	eEnds m_Ends;
};

typedef cBezierSpline<cVec2> cBezierSpline2;
typedef cBezierSpline<cVec3> cBezierSpline3;

//--------------------------------------------------------------------------------------------------------------------------------------
// cBezierSpline<Type>::SetFromPoints
//--------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::SetFromPoints(const Type *pPoints, const int nPoints, const typename cBezierSpline<Type>::eEnds Ends) {
	m_Ends = Ends;
	
	int nCurves = nPoints - 1;
	assert(nCurves > 0);

	m_Segments.Free();
	m_Segments.Lock(nCurves);
	
	for(int i = 0; i < nCurves; i++) {
		Type d0 = (pPoints[i + 1] - pPoints[Index(i - 1, nPoints, Ends)]) / 2.0f;
		Type d1 = (pPoints[Index(i + 2, nPoints, Ends)] - pPoints[i]) / 2.0f;
		
		m_Segments[i].Curve.SetFromEnds(pPoints[i], d0, pPoints[i + 1], d1);
		m_Segments[i].AccumTime = (i + 1) / float(nCurves);
	}

	Update();
}

//-------------------------------------------------------------------------------------------------------------------
// cBezierSpline<Type>::Index
//-------------------------------------------------------------------------------------------------------------------
template<class Type>
inline int cBezierSpline<Type>::Index(const int i, const int nSize, const typename cBezierSpline<Type>::eEnds Ends) {
	assert(i >= -1 && i <= nSize);

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
// cBezierSpline<Type>::SetFromKnots
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::SetFromKnots(const typename cBezierSpline<Type>::Knot *pKnots, const int nKnots, const typename cBezierSpline<Type>::eEnds Ends) {
	m_Ends = Ends;

	int nCurves = nKnots - 1;
	assert(nCurves > 0);

	m_Segments.Free();
	m_Segments.Lock(nCurves);
	
	assert(pKnots[0].Time == 0.0f);
	assert(pKnots[nKnots - 1].Time == 1.0f);
	
	for(int i = 0; i < nCurves; i++) {
		Type d0 = (pKnots[i + 1].Point - pKnots[Index(i - 1, nKnots, Ends)].Point) / 2.0f;
		Type d1 = (pKnots[Index(i + 2, nKnots, Ends)].Point - pKnots[i].Point) / 2.0f;
		
		float TimeScale = (pKnots[i + 1].Time - pKnots[i].Time) / float(nCurves);

		d0 *= TimeScale;
		d1 *= TimeScale;

		m_Segments[i].Curve.SetFromEnds(pKnots[i].Point, d0, pKnots[i + 1].Point, d1);
		m_Segments[i].AccumTime = pKnots[i + 1].Time;
	}

	Update();
}

//----------------------------------------------------------------------------------------------------
// cBezierSpline<Type>::SetUniformSampled
//----------------------------------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::SetUniformSampled(const cBezierSpline<Type> &Src, const int nKnots) {
	int nCurves = nKnots - 1;
	assert(nCurves > 0);

	float Len = Src.GetTotalLength();
	float LenPerSeg = Len / float(nCurves);
	float TimePerSeg = 1.0f / float(nCurves);
	
	m_Segments.Free();
	m_Segments.Lock(nCurves);

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
// cBezierSpline<Type>::SetAdaptiveSampled_RecursiveAdd
//---------------------------------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::SetAdaptiveSampled_RecursiveAdd(const cBezierSeg<Type> &C, const float t0, const float t1, const float MaxErrSq) {
	if(C.CalcLinearErrorBoundSq() <= MaxErrSq) {
		assert(m_Segments.Count() > 0 || t0 == 0.0f);
		assert(m_Segments.Count() == 0 || m_Segments.GetBack().AccumTime == t0);
		Segment S;
		S.Curve = C;
		S.AccumTime = t1;
		m_Segments.Add(S);
	} else {
		cBezierSeg<Type> C0, C1;
		C.SplitToSubCurves(&C0, &C1);
		float tMid = (t0 + t1) * 0.5f;

		SetAdaptiveSampled_RecursiveAdd(C0, t0, tMid, MaxErrSq);
		SetAdaptiveSampled_RecursiveAdd(C1, tMid, t1, MaxErrSq);
	}
}

//---------------------------------------------------------------------------------------------------------
// cBezierSpline<Type>::SetAdaptiveSampled
//---------------------------------------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::SetAdaptiveSampled(const cBezierSpline<Type> &Src, const float MaxErrSq) {
	m_Segments.Free();
	for(int i = 0; i < Src.GetNumSegments(); i++) {
		float tStart, tEnd;
		Src.GetSegmentTimeRange(i, &tStart, &tEnd);
		SetAdaptiveSampled_RecursiveAdd(Src.GetCurve(i), tStart, tEnd, MaxErrSq);
	}
	Update();
}

//-----------------------------------------------------------------------------
// cBezierSpline<Type>::Copy
//-----------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::Copy(const cBezierSpline<Type> &Src) {
	m_Segments.Copy(Src.m_Segments);
	m_BBox = Src.m_BBox;
	m_Ends = Src.m_Ends;
}

//-----------------------------------------------------------------------------
// cBezierSpline<Type>::GetNumSegments
//-----------------------------------------------------------------------------
template<class Type>
inline int cBezierSpline<Type>::GetNumSegments() const {
	return m_Segments.Count();
}

//-----------------------------------------------------------------------------------------
// cBezierSpline<Type>::GetCurve
//-----------------------------------------------------------------------------------------
template<class Type>
inline const cBezierSeg<Type> & cBezierSpline<Type>::GetCurve(const int nSegment) const {
	return m_Segments[nSegment].Curve;
}

//------------------------------------------------------------------------------------
// cBezierSpline<Type>::GetMutableCurve
//------------------------------------------------------------------------------------
template<class Type>
inline cBezierSeg<Type> & cBezierSpline<Type>::GetMutableCurve(const int nSegment) {
	return m_Segments[nSegment].Curve;
}

//--------------------------------------------------------------------------------------------------------------
// cBezierSpline<Type>::GetSegment
//--------------------------------------------------------------------------------------------------------------
template<class Type>
inline const typename cBezierSpline<Type>::Segment & cBezierSpline<Type>::GetSegment(const int nSegment) const {
	return m_Segments[nSegment];
}

//-----------------------------------------------------------------------------
// cBezierSpline<Type>::GetBounds
//-----------------------------------------------------------------------------
template<class Type>
inline const cBounds & cBezierSpline<Type>::GetBounds() const {
	return m_Bounds;
}

//-----------------------------------------------------------------------------
// cBezierSpline<Type>::GetTotalLength
//-----------------------------------------------------------------------------
template<class Type>
inline float cBezierSpline<Type>::GetTotalLength() const {
	if(!m_Segments.Count()) {
		return 0.0f;
	}
	return m_Segments.GetBack().AccumLength;
}

//----------------------------------------------------------------------------------------------------
// cBezierSpline<Type>::GetSegmentTimeRange
//----------------------------------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::GetSegmentTimeRange(int nSegment, float *tStart, float *tEnd) const {
	assert(nSegment >= 0 && nSegment < m_Segments.Count());
	
	if(nSegment == 0) {
		*tStart = 0.0f;
	} else {
		*tStart = m_Segments[nSegment - 1].AccumTime;
	}
	*tEnd = m_Segments[nSegment].AccumTime;
}

//------------------------------------------------------------------------------------------------------
// cBezierSpline<Type>::GetSegmentLengthRange
//------------------------------------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::GetSegmentLengthRange(int nSegment, float *lStart, float *lEnd) const {
	assert(nSegment >= 0 && nSegment < m_Segments.Count());
	
	if(nSegment == 0) {
		*lStart = 0.0f;
	} else {
		*lStart = m_Segments[nSegment - 1].AccumLength;
	}
	*lEnd = m_Segments[nSegment].AccumLength;
}

//---------------------------------------------------------------------------------------------------------
// cBezierSpline<Type>::GetPlaceByTime
//---------------------------------------------------------------------------------------------------------
template<class Type>
inline const typename cBezierSpline<Type>::Place cBezierSpline<Type>::GetPlaceByTime(const float t) const {
	assert(cMath::IsZeroToOneExact(t));
	
	Place P;
	ZeroMemory(&P, sizeof(P));

	if(m_Segments.Count()) {
		assert(m_Segments.GetBack().AccumTime == 1.0f);

		int nSegment = 0;
		while(t > m_Segments[nSegment].AccumTime) {
			nSegment++;
		}

		float tStart, tEnd;
		GetSegmentTimeRange(nSegment, &tStart, &tEnd);
		assert(t >= tStart && t <= tEnd);
		P.nSegment = nSegment;
		P.LocalTime = cMath::LerperClamp01(tStart, tEnd, t);
	}
	return P;
}

//-----------------------------------------------------------------------------------------------------------
// cBezierSpline<Type>::GetPlaceByLength
//-----------------------------------------------------------------------------------------------------------
template<class Type>
inline const typename cBezierSpline<Type>::Place cBezierSpline<Type>::GetPlaceByLength(const float l) const {
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
		assert(L >= lStart && L <= lEnd);
		P.nSegment = nSegment;
		P.LocalTime = cMath::LerperClamp01(lStart, lEnd, L);
	}
	return P;
}

//-------------------------------------------------------------------------------------------------------------------
// cBezierSpline<Type>::GetPlaceByLengthAccurate
//-------------------------------------------------------------------------------------------------------------------
template<class Type>
inline const typename cBezierSpline<Type>::Place cBezierSpline<Type>::GetPlaceByLengthAccurate(const float l) const {
	const float L = cMath::Clamp(l, 0.0f, GetTotalLength());
	
	Place P = GetPlaceByLength(L);
	float lStart, lEnd;
	GetSegmentLengthRange(P.nSegment, &lStart, &lEnd);
	assert(L >= lStart && L <= lEnd);
	P.LocalTime = m_Segments[P.nSegment].Curve.CalcParameterForLength(L - lStart);

	return P;
}

//------------------------------------------------------------------------------------------------------
// cBezierSpline<Type>::GetTimeFromPlace
//------------------------------------------------------------------------------------------------------
template<class Type>
inline float cBezierSpline<Type>::GetTimeFromPlace(const typename cBezierSpline<Type>::Place &P) const {
	if(!m_Segments.Count()) return 0.0f;

	float tStart, tEnd;
	GetSegmentTimeRange(P.nSegment, &tStart, &tEnd);

	return cMath::Lerp(tStart, tEnd, P.LocalTime);
}

//--------------------------------------------------------------------------------------------------------
// cBezierSpline<Type>::GetLengthFromPlace
//--------------------------------------------------------------------------------------------------------
template<class Type>
inline float cBezierSpline<Type>::GetLengthFromPlace(const typename cBezierSpline<Type>::Place &P) const {
	if(!m_Segments.Count()) return 0.0f;

	float lStart, lEnd;
	GetSegmentLengthRange(P.nSegment, &lStart, &lEnd);

	return lStart + cMath::Lerp(lStart, lEnd, P.LocalTime);
}

//----------------------------------------------------------------------------------------------------------------------
// cBezierSpline<Type>::CalcClosestPointPlaceLinear
//----------------------------------------------------------------------------------------------------------------------
template<class Type>
inline const typename cBezierSpline<Type>::Place cBezierSpline<Type>::CalcClosestPointPlaceLinear(const Type &p) const {
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
	
	assert(CurBestDistance2 != cMath::FloatMaxValue);
	return P;
}

//----------------------------------------------------------------------------------------------------------------------------------------------
// cBezierSpline<Type>::CalcClosestPointPlaceAccurate
//----------------------------------------------------------------------------------------------------------------------------------------------
struct cBezierSplineClosestPointRef {
	int nSegment;
	float MinDistance2;
	static int Cmp(const cBezierSplineClosestPointRef *r0, const cBezierSplineClosestPointRef *r1) {
		return (int)cMath::Sign(r0->MinDistance2 - r1->MinDistance2);
	}
};
template<class Type>
inline const typename cBezierSpline<Type>::Place cBezierSpline<Type>::CalcClosestPointPlaceAccurate(const Type &p, const float MaxErrSq) const {
	assert(m_Segments.Count() > 0);
	
	cList<cBezierSplineClosestPointRef> Refs;
	Refs.Lock(m_Segments.Count());
	
	Place CurBestPlace;
	CurBestPlace.LocalTime = 0.0f;
	CurBestPlace.nSegment = 0;
	
	float CurBestDistance2 = cMath::FloatMaxValue;

	for(int i = 0; i < m_Segments.Count(); i++) {
		Refs[i].nSegment = i;
		Refs[i].MinDistance2 = cMath::Square(m_Segments[i].BSphere.DistanceToPoint(p));
	}

	Refs.Sort(cBezierSplineClosestPointRef::Cmp);
//	assert(Refs[0].MinDistance2 <= Refs[1].MinDistance2);

	for(i = 0; i < m_Segments.Count(); i++) {
		if(Refs[i].MinDistance2 >= CurBestDistance2) {
			break;
		}

		int nSegment = Refs[i].nSegment;
		const Segment &S = m_Segments[nSegment];
		float d2;
		float Time = S.Curve.CalcClosestPointParameterAccurate(p, MaxErrSq, &d2);
		assert(Refs[i].MinDistance2 <= d2 + cMath::Epsilon);
		if(d2 < CurBestDistance2) {
			CurBestDistance2 = d2;
			CurBestPlace.LocalTime = Time;
			CurBestPlace.nSegment = nSegment;
		}
	}
	assert(CurBestDistance2 != cMath::FloatMaxValue);

	return CurBestPlace;
}

//-----------------------------------------------------------------------------
// cBezierSpline<Type>::GetValue
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cBezierSpline<Type>::GetValue(const float t) const {
	Place P = GetPlaceByTime(t);
	return GetCurve(P.nSegment).GetValue(P.LocalTime);
}

//---------------------------------------------------------------------------------------------------
// cBezierSpline<Type>::GetValue
//---------------------------------------------------------------------------------------------------
template<class Type>
inline const Type cBezierSpline<Type>::GetValue(const typename cBezierSpline<Type>::Place &P) const {
	return GetCurve(P.nSegment).GetValue(P.LocalTime);
}

//------------------------------------------------------------------------------
// cBezierSpline<Type>::GetDerivativeLocal
//------------------------------------------------------------------------------
template<class Type>
inline const Type cBezierSpline<Type>::GetDerivativeLocal(const float t) const {
	Place P = GetPlaceByTime(t);
	return GetCurve(P.nSegment).GetDerivative(P.LocalTime);
}

//-------------------------------------------------------------------------------------------------------------
// cBezierSpline<Type>::GetDerivativeLocal
//-------------------------------------------------------------------------------------------------------------
template<class Type>
inline const Type cBezierSpline<Type>::GetDerivativeLocal(const typename cBezierSpline<Type>::Place &P) const {
	return GetCurve(P.nSegment).GetDerivative(P.LocalTime);
}

//-------------------------------------------------------------------------------
// cBezierSpline<Type>::GetDerivativeGlobal
//-------------------------------------------------------------------------------
template<class Type>
inline const Type cBezierSpline<Type>::GetDerivativeGlobal(const float t) const {
	Place P = GetPlaceByTime(t);
	return GetDerivativeGlobal(P);
}

//--------------------------------------------------------------------------------------------------------------
// cBezierSpline<Type>::GetDerivativeGlobal
//--------------------------------------------------------------------------------------------------------------
template<class Type>
inline const Type cBezierSpline<Type>::GetDerivativeGlobal(const typename cBezierSpline<Type>::Place &P) const {
	float tStart, tEnd;
	GetSegmentTimeRange(P.nSegment, &tStart, &tEnd);
	float tRange = tEnd - tStart;
	assert(tRange <= 1.0f);
	Type DerivLocal = GetCurve(P.nSegment).GetDerivative(P.LocalTime);
	return DerivLocal * (1.0f / tRange);
}

//-----------------------------------------------------------------------------
// cBezierSpline<Type>::IndexOfSegment1
//-----------------------------------------------------------------------------
template<class Type>
inline int cBezierSpline<Type>::IndexOfSegment1(const int nSegment0) const {
	assert(nSegment0 >= 0 && nSegment0 < m_Segments.Count());

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
// cBezierSpline<Type>::IndexOfSegment0
//-----------------------------------------------------------------------------
template<class Type>
inline int cBezierSpline<Type>::IndexOfSegment0(const int nSegment1) const {
	assert(nSegment1 >= 0 && nSegment1 < m_Segments.Count());

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
// cBezierSpline<Type>::IsClosingEP
//--------------------------------------------------------------------------------------------
template<class Type>
inline bool cBezierSpline<Type>::IsClosingEP(const int nSegment0, const int nSegment1) const {
	return IsClosed() && 0 == nSegment1 && m_Segments.Count() - 1 == nSegment0;
}

//-----------------------------------------------------------------------------
// cBezierSpline<Type>::IsEmpty
//-----------------------------------------------------------------------------
template<class Type>
inline bool cBezierSpline<Type>::IsEmpty() const {
	return (IsOpen() && m_Segments.Count() == 0) || (IsClosed() && m_Segments.Count() <= 1);
}

//-----------------------------------------------------------------------------------
// cBezierSpline<Type>::RemoveEP
//-----------------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::RemoveEP(const int nSegment0, const int nSegment1) {
	bool Start = nSegment0 < 0 && nSegment1 >= 0;
	bool End = nSegment0 >= 0 && nSegment1 < 0;
	bool Between = nSegment0 >= 0 && nSegment1 >= 0;

	if(Start || End) {
		assert(E_OPEN == m_Ends);
		if(Start) {
			assert(0 == nSegment1);
			m_Segments.RemoveAt(0);
		} else if(End) {
			assert(m_Segments.Count() - 1 == nSegment0);
			m_Segments.RemoveBack();
		}
		if(m_Segments.Count()) {
			Update();
			ParameterizeByArcLength();
		}
	} else if(Between) {
		assert(IndexOfSegment0(nSegment1) == nSegment0);
		assert(IndexOfSegment1(nSegment0) == nSegment1);

		if(0 == nSegment1 && m_Segments.Count() - 1 == nSegment0) { // Removing closing point:
			assert(E_CLOSED == m_Ends);
			
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
			assert(nSegment1 > nSegment0);

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
// cBezierSpline<Type>::AddEP
//------------------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::AddEP(const typename cBezierSpline<Type>::Place &P) {
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
// cBezierSpline<Type>::TranslateAll
//-----------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::TranslateAll(const Type &t) {
	for(int i = 0; i < m_Segments.Count(); i++) {
		m_Segments[i].Curve.Translate(t);
		m_Segments[i].BSphere.Translate(t);
	}
	m_Bounds.Translate(t);
}

//---------------------------------------------------------------------------------
// cBezierSpline<Type>::TranslateEP0
//---------------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::TranslateEP0(const int nSegment1, const Type &t) {
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
// cBezierSpline<Type>::TranslateEP1
//---------------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::TranslateEP1(const int nSegment0, const Type &t) {
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
// cBezierSpline<Type>::TranslateCV0
//---------------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::TranslateCV0(const int nSegment1, const Type &t) {
	m_Segments[nSegment1].Curve.GetMutablePoint(1) += t; // CV0
	const int nSegment0 = IndexOfSegment0(nSegment1);
	if(nSegment0 >= 0) {
		AdjustCV1(nSegment0);
	}
	Update();
}

//---------------------------------------------------------------------------------
// cBezierSpline<Type>::TranslateCV1
//---------------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::TranslateCV1(const int nSegment0, const Type &t) {
	m_Segments[nSegment0].Curve.GetMutablePoint(2) += t; // CV1
	const int nSegment1 = IndexOfSegment1(nSegment0);
	if(nSegment1 >= 0) {
		AdjustCV0(nSegment1);
	}
	Update();
}

//-------------------------------------------------------------------------------------
// cBezierSpline<Type>::GetEnds
//-------------------------------------------------------------------------------------
template<class Type>
inline const typename cBezierSpline<Type>::eEnds cBezierSpline<Type>::GetEnds() const {
	return m_Ends;
}

//-----------------------------------------------------------------------------
// cBezierSpline<Type>::IsOpen
//-----------------------------------------------------------------------------
template<class Type>
inline bool cBezierSpline<Type>::IsOpen() const {
	return E_OPEN == m_Ends;
}

//-----------------------------------------------------------------------------
// cBezierSpline<Type>::IsClosed
//-----------------------------------------------------------------------------
template<class Type>
inline bool cBezierSpline<Type>::IsClosed() const {
	return E_CLOSED == m_Ends;
}

//-----------------------------------------------------------------------------
// cBezierSpline<Type>::Open
//-----------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::Open() {
	m_Ends = E_OPEN;
}

//-----------------------------------------------------------------------------
// cBezierSpline<Type>::Close
//-----------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::Close() {
	if(m_Segments.Count() < 2) return;

	m_Ends = E_CLOSED;
	
	m_Segments.GetBack().Curve.GetMutablePoint(3) = m_Segments[0].Curve.GetPoint(0);
	AdjustCV1(m_Segments.Count() - 1);
}

//-----------------------------------------------------------------------------
// cBezierSpline<Type>::Update
//-----------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::Update() {
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
// cBezierSpline<cQuat>::Update
//-----------------------------------------------------------------------------
inline void cBezierSpline<cQuat>::Update() {
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
// cBezierSpline<Type>::AdjustCV0
//-----------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::AdjustCV0(const int nSegment1) {
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
// cBezierSpline<Type>::AdjustCV1
//-----------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::AdjustCV1(const int nSegment0) {
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
// cBezierSpline<Type>::ParameterizeByArcLength
//-----------------------------------------------------------------------------
template<class Type>
inline void cBezierSpline<Type>::ParameterizeByArcLength() {
	if(!m_Segments.Count()) return;

	float Scale = 1.0f / GetTotalLength();

	for(int i = 0; i < m_Segments.Count(); i++) {
		m_Segments[i].AccumTime = m_Segments[i].AccumLength * Scale;
	}
	assert(cMath::IsOne(m_Segments.GetBack().AccumTime));
	m_Segments.GetBack().AccumTime = 1.0f;
}