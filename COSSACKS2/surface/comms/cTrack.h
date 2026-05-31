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
//		cBezierSeg3 Curve3;
//		float lStart, lEnd;
		
//		cBezierSeg1 Curve1;
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