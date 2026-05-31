#pragma once

template<class Type>
class tCurve {
public:
	tCurve();
	void Copy(const tCurve<Type> &Src) {
		m_Times.Copy(Src.m_Times);
		m_Values.Copy(Src.m_Values);
		m_CachedIndex = Src.m_CachedIndex;
	}
	virtual int AddValue(const float Time, const Type &Value);
	int GetValuesCount() const { return m_Values.Count(); }
	virtual void Free() { m_Times.Free(); m_Values.Free(); m_CachedIndex = -1; }

	virtual const Type GetCurrentValue(const float Time ) const;
	
	const Type GetValue(const int index) const { return m_Values[index]; }
	float GetTime(const int index) const { return m_Times[index]; }
	
	void ShiftTime(const float DeltaTime);
protected:
	cList<float> m_Times;
	cList<Type> m_Values;
	mutable int m_CachedIndex;
	int IndexForTime(const float Time) const;
	float TimeForIndex(const int index) const;
	Type ValueForIndex(const int index) const;
	float ClampedTime(const float Time) const;
};

template<class Type>
class cTCBCurve : public tCurve<Type> {
public:
	virtual void Copy(const cTCBCurve<Type> &Src) {
		m_Times.Copy(Src.m_Times);
		m_Values.Copy(Src.m_Values);
		m_Tension.Copy(Src.m_Tension);
		m_Bias.Copy(Src.m_Bias);
		m_Continuity.Copy(Src.m_Continuity);
	}
	virtual int AddValue(const float Time, const Type &Value, const float Tension = 0.0f, const float Bias = 0.0f, const float Continuity = 0.0f);
	virtual const Type GetCurrentValue(const float Time) const;
protected:
	cList<float> m_Tension;
	cList<float> m_Bias;
	cList<float> m_Continuity;
};

template<class Type>
class tBezierCurve : public tCurve<Type> {
public:
	const Type GetCurrentValue(const float Time) const;
protected:
	void Basis(const int nOrder, const float Time, float *BasisValues) const;
};

template<class Type>
class tNURBSCurve : public tCurve<Type> {
public:
	void Free() {
		m_Times.Free();
		m_Values.Free();
		m_Weights.Free();
		m_CachedIndex = -1;
	}
	void Copy(const tNURBSCurve<Type> &Src) {
		m_Times.Copy(Src.m_Times);
		m_Values.Copy(Src.m_Values);
		m_Weights.Copy(Src.m_Weights);
		m_CachedIndex = Src.m_CachedIndex;
	}
	int AddValue(const float Time, const Type &Value, const float Weight = 1.0f) {
		int index = IndexForTime(Time);
		m_Times.InsertAt(index, Time);
		m_Values.InsertAt(index, Value);
		m_Weights.InsertAt(index, Weight);
		return index;
	}
	const Type GetCurrentValue(const float Time) const;
protected:
	cList<float> m_Weights;
	float WeightForIndex(const int index) const;
	void Basis(const int index, const int order, const float t, float *bvals) const;
};

namespace LW {

typedef struct lwKey {
   struct lwKey *next;
   struct lwKey *prev;
   float  value;
   float  time;
   int    shape;
   float  tension;
   float  continuity;
   float  bias;
   float  param[ 4 ];
} Key;

typedef struct lwEnvelope {
   Key   *key;
   int    nkeys;
   int    behavior[ 2 ];
} Envelope;

}

template<class Type>
class cSpline {
public:
	cSpline();
	virtual void Free();
	virtual void Copy(const cSpline &Src);
	virtual int AddValue(const float Time, const Type &Value);
	int GetValuesCount() const { return m_Times.Count(); }
	virtual void RemoveAt(const int index);
	void ShiftTime(const float DeltaTime);
	
	const Type GetValue(const int index) const { return m_Values[index]; }
	void SetValue(const int index, const Type &Value) { m_Values[index] = Value; m_IsChanged = true; }
	float GetTime(const int index) const { return m_Times[index]; }

	float GetTimeStart() const { return m_Times[0]; }
	float GetTimeEnd() const { return m_IsClosed ? m_CloseTime : m_Times.GetBack(); }
	float GetDuration() const { return GetTimeEnd() - GetTimeStart(); }

	void Setup(const bool IsClosed, const float CloseTime) { m_IsClosed = IsClosed; m_CloseTime = CloseTime; m_IsChanged = true; }
	bool IsClosed() const { return m_IsClosed; }
	
	virtual const Type GetCurrentValue(const float Time) const;
	virtual const Type GetCurrent1stDerivative(const float Time) const;
	virtual const Type GetCurrent2ndDerivative(const float Time) const;
protected:
	mutable int m_CachedIndex;
	mutable bool m_IsChanged;
	bool m_IsClosed;
	float m_CloseTime;
	cList<float> m_Times;
	cList<Type> m_Values;
	int IndexForInsert(const float Time) const;
};

// cSpline<Type>.ctor
template<class Type>
inline cSpline<Type>::cSpline() {
	m_CachedIndex = -1;
	m_IsChanged = false;
	m_IsClosed = false;
	m_CloseTime = 0.0f;
}

// cSpline::Free
template<class Type>
inline void cSpline<Type>::Free() {
	m_Times.Free();
	m_Values.Free();
	m_CachedIndex = -1;
	m_CloseTime = 0.0f;
	m_IsChanged = true;
	m_IsClosed = false;
}

template<class Type>
inline void cSpline<Type>::Copy(const cSpline<Type> &Src) {
	m_Times.Copy(Src.m_Times);
	m_Values.Copy(Src.m_Values);
	m_CachedIndex = -1;
	m_IsClosed = Src.m_IsClosed;
	m_CloseTime = Src.m_CloseTime;
	m_IsChanged = true;
}

template<class Type>
inline int cSpline<Type>::AddValue(const float Time, const Type &Value) {
	int index = IndexForInsert(Time);
	m_Times.InsertAt(index, Time);
	m_Values.InsertAt(index, Value);
	m_IsChanged = true;
	return index;
}

template<class Type>
inline void cSpline<Type>::RemoveAt(const int index) {
	m_Times.RemoveAt(index);
	m_Values.RemoveAt(index);
	m_IsChanged = true;
}

template<class Type>
inline void cSpline<Type>::ShiftTime(const float DeltaTime) {
	for(int i = 0; i < m_Times.Count(); i++) {
		m_Times[i] += DeltaTime;
	}
	m_CloseTime += DeltaTime;
	m_IsChanged = true;
}

template<class Type>
inline const Type cSpline<Type>::GetCurrentValue(const float Time) const {
	int index = IndexForInsert(Time);
	if(index >= m_Values.Count()) {
		return m_Values[m_Values.Count() - 1];
	} else {
		return m_Values[index];
	}
}

template<class Type>
inline const Type cSpline<Type>::GetCurrent1stDerivative(const float Time) const {
	return m_Values[0] - m_Values[0];
}

template<class Type>
inline const Type cSpline<Type>::GetCurrent2ndDerivative(const float Time) const {
	return m_Values[0] - m_Values[0];
}

// cSpline<Type>::IndexForInsert
template<class Type>
inline int cSpline<Type>::IndexForInsert(const float Time) const {
	if(m_CachedIndex >= 0 && m_CachedIndex <= m_Times.Count()) {
		if(m_CachedIndex == 0) {
			if(Time <= m_Times[m_CachedIndex]) {
				return m_CachedIndex;
			}
		} else if(m_CachedIndex == m_Times.Count()) {
			if(Time > m_Times[m_CachedIndex - 1]) {
				return m_CachedIndex;
			}
		} else if(Time > m_Times[m_CachedIndex - 1] && Time <= m_Times[m_CachedIndex]) {
			return m_CachedIndex;
		} else if(Time > m_Times[m_CachedIndex] &&
			(m_CachedIndex + 1 == m_Times.Count() || Time <= m_Times[m_CachedIndex + 1])) {
			m_CachedIndex++;
			return m_CachedIndex;
		}
	}

	m_CachedIndex = cMath::IndexForInsert(Time, m_Times.ToPtr(), m_Times.Count());
	return m_CachedIndex;
}

template<class Type>
class cSplineTCB : public cSpline<Type> {
public:
	virtual void Free();
	virtual void Copy(const cSplineTCB<Type> &Src);
	virtual int AddValue(const float Time, const Type &Value, const float Tension = 0.0f, const float Continuity = 0.0f, const float Bias = 0.0f);
	virtual void RemoveAt(const int index);
	virtual const Type GetCurrentValue(const float Time, bool IsClosed = false, const float CloseTime = 0.0f) const;
	void SetConstantSpeed(const float TotalTime, const bool IsClosed);

	float GetTension(const int index) const { return m_Tension[index]; }
	float GetContinuity(const int index) const { return m_Continuity[index]; }
	float GetBias(const int index) const { return m_Bias[index]; }

	void SetTension(const int index, const float Tension) { m_Tension[index] = Tension; m_IsChanged = true; }
	void SetContinuity(const int index, const float Continuity) { m_Continuity[index] = Continuity; m_IsChanged = true; }
	void SetBias(const int index, const float Bias) { m_Bias[index] = Bias; m_IsChanged = true; }
private:
	cList<float> m_Tension;
	cList<float> m_Continuity;
	cList<float> m_Bias;

	struct CacheNode {
		float Time;
		Type Value;
		float Tension;
		float Continuity;
		float Bias;
		Type In;
		Type Out;
	};
	void PreCache() const;
	void AltCache() const;
	mutable cList<CacheNode> m_Cache;
};

typedef cSplineTCB<float> cSplineTCB1;
typedef cSplineTCB<cVec2> cSplineTCB2;
typedef cSplineTCB<cVec3> cSplineTCB3;
typedef cSplineTCB<cAngles> cSplineTCBA;

template<class Type>
inline void cSplineTCB<Type>::Free() {
	m_Times.Free();
	m_Values.Free();
	m_Tension.Free();
	m_Continuity.Free();
	m_Bias.Free();
	m_CachedIndex = -1;
	m_IsClosed = false;
	m_CloseTime = 0.0f;
	m_IsChanged = true;
}

template<class Type>
inline void cSplineTCB<Type>::Copy(const cSplineTCB<Type> &Src) {
	m_Times.Copy(Src.m_Times);
	m_Values.Copy(Src.m_Values);
	m_Tension.Copy(Src.m_Tension);
	m_Continuity.Copy(Src.m_Continuity);
	m_Bias.Copy(Src.m_Bias);
	m_CachedIndex = -1;
	m_IsClosed = Src.m_IsClosed;
	m_CloseTime = Src.m_CloseTime;
	m_IsChanged = true;
}


template<class Type>
inline int cSplineTCB<Type>::AddValue(const float Time, const Type &Value, const float Tension, const float Continuity, const float Bias) {
	int index = IndexForInsert(Time);
	m_Times.InsertAt(index, Time);
	m_Values.InsertAt(index, Value);
	m_Tension.InsertAt(index, Tension);
	m_Continuity.InsertAt(index, Continuity);
	m_Bias.InsertAt(index, Bias);
	m_IsChanged = true;
	return index;
}

template<class Type>
inline void cSplineTCB<Type>::RemoveAt(const int index) {
	m_Times.RemoveAt(index);
	m_Values.RemoveAt(index);
	m_Tension.RemoveAt(index);
	m_Continuity.RemoveAt(index);
	m_Bias.RemoveAt(index);
	m_IsChanged = true;
}

template<class Type>
inline void cSplineTCB<Type>::AltCache() const {
}

inline void cSplineTCB<cAngles>::AltCache() const {
	for(int i = 0; i < m_Cache.Count(); i++) {
		m_Cache[i].Value.Normalize180();
		if(i > 0) {
			for(int k = 0; k < m_Cache[0].Value.GetDimension(); k++) {
				float d = cMath::AngleNormalize180(m_Cache[i].Value[k] - m_Cache[i - 1].Value[k]);
				m_Cache[i].Value[k] = m_Cache[i - 1].Value[k] + d;
			}
		}
	}
}

template<class Type>
inline void cSplineTCB<Type>::PreCache() const {
	m_Cache.Free();

	if(m_Times.Count() < 1) {
		return;
	}

	CacheNode n;
	for(int i = 0; i < m_Times.Count(); i++) {
		n.Time = m_Times[i];
		n.Value = m_Values[i];
		n.Tension = m_Tension[i];
		n.Continuity = m_Continuity[i];
		n.Bias = m_Bias[i];
		m_Cache.Add(n);
	}
	
	if(m_IsClosed) {
		CacheNode NewLastByOne = m_Cache[0];
		CacheNode NewLast = m_Cache[1];
		CacheNode NewFirst = m_Cache.GetBack();
		NewFirst.Time = m_Cache[0].Time - (m_CloseTime - m_Cache.GetBack().Time);
		NewLastByOne.Time = m_CloseTime;
		NewLast.Time = m_CloseTime + (m_Cache[1].Time - m_Cache[0].Time);
		m_Cache.InsertAt(0, NewFirst);
		m_Cache.Add(NewLastByOne);
		m_Cache.Add(NewLast);
	}
	
	AltCache();

	if(m_Cache.Count() == 1) {
		return;
	}

	if(m_Cache.Count() == 2) {
		CacheNode &Start = m_Cache[0];
		CacheNode &End = m_Cache[1];
		Start.Out = End.In = cMath::_TCBSetupStartOutEndIn(Start.Value, End.Value);
		return;
	}

	for(int iCur = 0; iCur < m_Cache.Count(); iCur++) {
		int iPrev = cMath::Clamp(iCur - 1, 0, m_Cache.Count() - 1);
		int iNext = cMath::Clamp(iCur + 1, 0, m_Cache.Count() - 1);
		CacheNode &Cur = m_Cache[iCur];
		const CacheNode &Prev = m_Cache[iPrev];
		const CacheNode &Next = m_Cache[iNext];
		if(iPrev < iCur && iNext > iCur) {
			cMath::TCBSetupTimeAdj(Prev.Value, Cur.Value, Next.Value, Cur.In, Cur.Out, Prev.Time, Cur.Time, Next.Time, Cur.Tension, Cur.Continuity, Cur.Bias);
		} else if(iPrev == iCur) {
			assert(iNext > iCur);
			Cur.Out = cMath::TCBSetupStartOut(Cur.Value, Next.Value, Cur.Tension, Cur.Continuity, Cur.Bias);
		} else {
			assert(iNext == iCur);
			assert(iPrev < iCur);
			Cur.In = cMath::TCBSetupEndIn(Prev.Value, Cur.Value, Cur.Tension, Cur.Continuity, Cur.Bias);
		}
	}
}

template<class Type>
inline void cSplineTCB<Type>::SetConstantSpeed(const float TotalTime, const bool IsClosed) {
	if(m_Times.Count() <= 2) {
		return;
	}
	m_IsClosed = IsClosed;
	if(IsClosed) {
		m_CloseTime = TotalTime;
	}
	for(int i = 0; i < m_Times.Count(); i++) {
		m_Times[i] = float(i) * 0.01f;
	}
	PreCache();
	float TotalLength = 0.0f;
	cList<float> Ls;
	cSegBezier<Type> Seg;
	int iStart = IsClosed ? 1 : 0;
	int iEnd = IsClosed ? m_Cache.Count() - 2 : m_Cache.Count() - 1;
	for(int i = iStart; i < iEnd; i++) {
		Seg.SetFromEnds(m_Cache[i].Value, m_Cache[i].Out, m_Cache[i + 1].Value, m_Cache[i + 1].In);
		Ls.Add(Seg.CalcLength());
		TotalLength += Ls.GetBack();
	}
	m_Times[0] = 0.0f;
	float AccumLength = 0.0f;
	for(i = 1; i < m_Times.Count(); i++) {
		AccumLength += Ls[i - 1];
		m_Times[i] = AccumLength / TotalLength * TotalTime;
	}
	m_IsChanged = true;
}

template<class Type>
inline const Type cSplineTCB<Type>::GetCurrentValue(const float Time, bool IsClosed, const float CloseTime) const {
	if(m_IsChanged) {
		PreCache();
		m_IsChanged = false;
	}

	if(Time <= m_Times[0]) {
		return m_Values[0];
	}
	if(Time >= m_Cache.GetBack().Time) {
		return m_Cache.GetBack().Value;
	}
	
	int index = cMath::IndexForInsert(Time, &m_Cache[0].Time, m_Cache.Count(), sizeof(CacheNode));
	if(Time == m_Cache[index].Time) {
		return m_Cache[index].Value;
	}
	int i0 = index - 1;
	int i1 = index;
	
	float t = cMath::LerperClamp01(m_Cache[i0].Time, m_Cache[i1].Time, Time);
	cSegBezier<Type> Seg;
	Seg.SetFromEnds(m_Cache[i0].Value, m_Cache[i0].Out, m_Cache[i1].Value, m_Cache[i1].In);
	return Seg.GetValue(t);
}

class cTrack {
public:
	cTrack() {
		m_CachedIndex = -1;
		m_IsClosed = false;
	}
	void Free() {
		m_Keys.Free();
		m_CachedIndex = -1;
		for(int c = CX; c < CLAST; c++) {
			lwKeys[c].Free();
		}
	}
	void Copy(const cTrack &Src) {
		m_Keys.Copy(Src.m_Keys);
		m_CachedIndex = Src.m_CachedIndex;
		m_IsClosed = Src.m_IsClosed;
		Setup(m_IsClosed);
	}

//	enum eEnds {
//		E_OPEN,
//		E_CLOSED
//	};
	
	struct Key {
		Key() {}

		Key(const float Time, const cVec3 &Pos, const cAngles &Orient, const float Offset,
			const float Tension = 0.0f, const float Continuity = 0.0f, const float Bias = 0.0f) {
				Set(Time, Pos, Orient, Offset, Tension, Continuity, Bias);
			}

		void Set(const float Time, const cVec3 &Pos, const cAngles &Orient, const float Offset,
			const float Tension = 0.0f, const float Continuity = 0.0f, const float Bias = 0.0f) {
				this->Time = Time;
				this->Pos = Pos;
				this->Orient = Orient;
				this->Offset = Offset;
				this->Tension = Tension;
				this->Continuity = Continuity;
				this->Bias = Bias;
			}

//		Key(const float Time, const cVec3 &Pos, const cAngles &Orient, const float Offset, const int Count = 1) {
//			Set(Time, Pos, Orient, Offset, Count);
//		}
		
//		Key(const float Time, const cVec3 &Pos, const cAngles &Orient, const float Offset, const cVec3 &CV0, const cVec3 &CV1) {
//			Set(Time, Pos, Orient, Offset, CV0, CV1);
//		}

//		Key & Set(const float Time, const cVec3 &Pos, const cAngles &Orient, const float Offset, const int Count = 1);
//		Key & Set(const float Time, const cVec3 &Pos, const cAngles &Orient, const float Offset, const cVec3 &CV0, const cVec3 &CV1);
		
		float Time;
		cVec3 Pos;
		cAngles Orient;
		float Offset;

		float Tension;
		float Continuity;
		float Bias;

//		int Count;
	
//		cVec3 CV0;
//		cVec3 CV1;
//		bool CV0IsValid;
//		bool CV1IsValid;

//		void AdjustCV1();
//		void AdjustCV0();

//		float l;
	};

//	void Free();
	int AddKey(const cTrack::Key &);
	void RemoveKey(const int index);
	void CopyKeys(const cTrack::Key *pSrc, const int nCount);
	void ShiftTime(const float DeltaTime);
	const cTrack::Key GetCurrentKey(const float Time) const;
	int GetKeysCount() const { return m_Keys.Count(); }
	const cTrack::Key & GetKey(const int nKey) const { return m_Keys[nKey]; }

	float GetTimeStart() const { return m_Keys[0].Time; }
	float GetTimeEnd() const { return /*IsClosed() ? GetCloseTime() : */m_Keys.GetBack().Time; }
	float GetDuration() const { return GetTimeEnd() - GetTimeStart(); }
	
	void Setup(const bool IsClosed);
	
//	void MakeTimeUniform(const float TotalTime);
//	void ReCreateCVs();
//	void ReCreateSpeed();
	

//	void Close(const float Time);
//	float GetCloseTime() const { return m_Ends == E_CLOSED ? m_CloseTime : 0.0f; }

//	void TranslateAll(const cVec3 &Translation);
	void TranslateKey(const int nKey, const cVec3 &Translation);
//	void TranslateCV0(const int nKey, const cVec3 &Translation);
//	void TranslateCV1(const int nKey, const cVec3 &Translation);
	
//	const cVec3 GetCurrent2ndDerivative(const float Time) const;
	
//	bool IsClosed() const { return E_CLOSED == m_Ends; }
//	bool IsOpen() const { return E_OPEN == m_Ends; }

//	tBezierCurve<float> lFunc;
//	tNURBSCurve<float> lFunc;
private:
	bool m_IsClosed;
	cList<Key> m_Keys;
	mutable int m_CachedIndex;
	
	typedef enum { CX, CY, CZ, CO, CPITCH, CYAW, CROLL, CLAST } eChannels;
	cList<LW::lwKey> lwKeys[CLAST];
	mutable LW::lwEnvelope lwEnvelopes[CLAST];


//	eEnds m_Ends;
//	float m_CloseTime;
	
//	struct Segment {
//		cSegBezier3 Curve3;
//		float lStart, lEnd;
		
//		cSegBezier1 Curve1;
//		cQuat Q0, A, B, Q1;
//		float tStart, tEnd; // to use segments cached index
//	};
//	cList<Segment> m_Segments;

//	struct Place {
//		int nSegment;
//		float LocalTime;
//	};

//	const Place GetPlaceByTimeAccurate(const float Time) const;

//	float GetTotalLength() const;
//	const Place GetPlaceByLength(const float l) const;
//	const Place GetPlaceByLengthAccurate(const float l) const;

	int IndexForTime(const float Time) const;
//	int AdjustIndex(const int index) const;
//	float GetTotalLength() const;
//	void GetKeyTriplet(const int nKey, Key **pCur, Key **ppPrev, Key **ppNext);
	
//	void Update();
};


	
template<class Type>
inline tCurve<Type>::tCurve() {
	m_CachedIndex = -1;
}

template<class Type>
inline int tCurve<Type>::AddValue(const float Time, const Type &Value) {
	int index = IndexForTime(Time);
	m_Times.InsertAt(index, Time);
	m_Values.InsertAt(index, Value);

	return index;
}

template<class Type>
inline int cTCBCurve<Type>::AddValue(const float Time, const Type &Value, const float Tension, const float Bias, const float Continuity) {
	int index = IndexForTime(Time);
	m_Times.InsertAt(index, Time);
	m_Values.InsertAt(index, Value);
	m_Tension.InsertAt(index, Tension);
	m_Continuity.InsertAt(index, Continuity);

	return index;
}

template<class Type>
inline float tCurve<Type>::TimeForIndex(const int index) const {
	int n = m_Times.Count() - 1;
	
	if(index < 0) {
		return m_Times[0] + index * (m_Times[1] - m_Times[0]);
	} else if(index > n) {
		return m_Times[n] + (index - n) * (m_Times[n] - m_Times[n - 1]);
	}
	return m_Times[index];
}

template<class Type>
inline Type tCurve<Type>::ValueForIndex(const int index) const {
	int n = m_Values.Count() - 1;
	
	if(index < 0) {
		return m_Values[0] + index * (m_Values[1] - m_Values[0]);
	} else if(index > n) {
		return m_Values[n] + (index - n) * (m_Values[n] - m_Values[n - 1]);
	}
	return m_Values[index];
}

template<class Type>
inline const Type tCurve<Type>::GetCurrentValue(const float Time) const {
	int index = IndexForTime(Time);
	if(index >= m_Values.Count()) {
		return m_Values[m_Values.Count() - 1];
	} else {
		return m_Values[index];
	}
}

template<class Type>
inline void tCurve<Type>::ShiftTime(const float DeltaTime) {
	for(int i = 0; i < m_Times.Count(); i++) {
		m_Times[i] += DeltaTime;
	}
}

template<class Type>
inline int tCurve<Type>::IndexForTime(const float Time) const {
	if(m_CachedIndex >= 0 && m_CachedIndex <= m_Times.Count()) {
		if(m_CachedIndex == 0) {
			if(Time <= m_Times[m_CachedIndex]) {
				return m_CachedIndex;
			}
		} else if(m_CachedIndex == m_Times.Count()) {
			if(Time > m_Times[m_CachedIndex - 1]) {
				return m_CachedIndex;
			}
		} else if(Time > m_Times[m_CachedIndex - 1] && Time <= m_Times[m_CachedIndex]) {
			return m_CachedIndex;
		} else if(Time > m_Times[m_CachedIndex] &&
			(m_CachedIndex + 1 == m_Times.Count() || Time <= m_Times[m_CachedIndex + 1])) {
			m_CachedIndex++;
			return m_CachedIndex;
		}
	}

	int N = m_Times.Count();
	int Mid = N;
	int Offset = 0, Res = 0;
	while(Mid > 0) {
		Mid = N >> 1;
		if(Time == m_Times[Offset + Mid]) {
			return Offset + Mid;
		} else if(Time > m_Times[Offset + Mid]) {
			Offset += Mid;
			N -= Mid;
			Res = 1;
		} else {
			N -= Mid;
			Res = 0;
		}
	}
	m_CachedIndex = Offset + Res;
	return m_CachedIndex;
}



template<class Type>
inline const Type tBezierCurve<Type>::GetCurrentValue(const float Time) const {
	cList<float> BasisValues;
	BasisValues.Lock(m_Values.Count());

	Basis(m_Values.Count(), Time, BasisValues.ToPtr());
	Type u = BasisValues[0] * m_Values[0];

	for(int i = 1; i < m_Values.Count(); i++) {
		u += BasisValues[i] * m_Values[i];
	}
	return u;
}

template<class Type>
inline void tBezierCurve<Type>::Basis(const int nOrder, const float Time, float *BasisValues) const {
	int i, j, d;
	float c1, c2, s, o, ps, po;

	BasisValues[0] = 1.0f;
	d = nOrder - 1;
	if(d <= 0) {
		return;
	}

	cList<float> c;
	c.Lock(d + 1);
	s = (float)(Time - m_Times[0]) / (m_Times[m_Times.Count() - 1] - m_Times[0]);
	o = 1.0f - s;
	ps = s;
	po = o;
	
	for(i = 1; i < d; i++) {
		c[i] = 1.0f;
	}
	for(i = 1; i < d; i++) {
		c[i - 1] = 0.0f;
		c1 = c[i];
		c[i] = 1.0f;
		for(j = i + 1; j <= d; j++) {
			c2 = c[j];
			c[j] = c1 + c[j-1];
			c1 = c2;
		}
		BasisValues[i] = c[d] * ps;
		ps *= s;
	}
	for(i = d - 1; i >= 0; i--) {
		BasisValues[i] *= po;
		po *= o;
	}
	BasisValues[d] = ps;
}

template<class Type>
inline float tCurve<Type>::ClampedTime(const float Time) const {
	if(Time < m_Times[0]) {
		return m_Times[0];
	} else if(Time >= m_Times[m_Times.Count() - 1]) {
		return m_Times[m_Times.Count() - 1];
	}
	return Time;
}

template<class Type>
inline const Type tNURBSCurve<Type>::GetCurrentValue(const float Time) const {
	if(m_Times.Count() == 1) {
		return m_Values[0];
	}

	cList<float> bvals;
	const int Order = m_Times.Count();
	bvals.Lock(Order);
	
	float clampedTime = cMath::Clamp(Time, m_Times[0], m_Times.GetBack());
	int i = IndexForTime(clampedTime);
	Basis(i - 1, Order, clampedTime, bvals.ToPtr());
	Type v = m_Values[0] - m_Values[0];
	float w = 0.0f;
	for(int j = 0; j < Order; j++) {
		int k = i + j - (Order >> 1);
		float b = bvals[j] * WeightForIndex(k);
		w += b;
		v += b * ValueForIndex(k);
	}
	return v / w;
}

template<class Type>
inline void tNURBSCurve<Type>::Basis(const int index, const int order, const float t, float *bvals ) const {
	int r, s, i;
    float omega;

    bvals[order - 1] = 1.0f;
	for(r = 2; r <= order; r++) {
		i = index - r + 1;
		bvals[order - r] = 0.0f;
		for(s = order - r + 1; s < order; s++) {
			i++;
			omega = (float)(t - TimeForIndex(i)) / (TimeForIndex(i + r - 1) - TimeForIndex(i));
			bvals[s - 1] += (1.0f - omega) * bvals[s];
			bvals[s] *= omega;
		}
	}
}

template<class Type>
inline float tNURBSCurve<Type>::WeightForIndex(const int index) const {
	int n = m_Weights.Count() - 1;
	
	if(index < 0) {
//		if(boundaryType == BT_CLOSED ) {
//			return weights[ weights.Num() + index % weights.Num() ];
//		} else {
			return m_Weights[0] + index * (m_Weights[1] - m_Weights[0]);
//		}
	} else if( index > n) {
//		if ( boundaryType == BT_CLOSED ) {
//			return weights[ index % weights.Num() ];
//		} else {
			return m_Weights[n] + (index - n) * (m_Weights[n] - m_Weights[n - 1]);
//		}
	}
	return m_Weights[index];
}