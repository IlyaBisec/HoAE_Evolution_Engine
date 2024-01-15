#pragma once

template<class Type>
class cInterpolate {
public:
	cInterpolate();
	void Init(const float StartTime, const float Duration, const Type &StartValue, const Type &EndValue);

	const Type GetCurrentValue(const float Time) const;
	bool IsDone(const float Time) const { return Time >= m_StartTime + m_Duration; }
	
	void SetStartTime(const float StartTime) { m_StartTime = StartTime; }
	void SetDuration(const float Duration) { m_Duration = Duration; }
	void SetStartValue(const Type &StartValue) { m_StartValue = StartValue; }
	void SetEndValue(const Type &EndValue) { m_EndValue = EndValue; }
	
	float GetStartTime() const { return m_StartTime; }
	float GetEndTime() const { return m_StartTime + m_Duration; }
	float GetDuration() const { return m_Duration; }
	const Type & GetStartValue() const { return m_StartValue; }
	const Type & GetEndValue() const { return m_EndValue; }
private:
	float m_StartTime;
	float m_Duration;
	Type m_StartValue;
	Type m_EndValue;
	mutable float m_CurrentTime;
	mutable Type m_CurrentValue;
};

//-----------------------------------------------------------------------------
// cInterpolate.ctor
//-----------------------------------------------------------------------------
template<class Type>
inline cInterpolate<Type>::cInterpolate() {
	m_StartTime = m_Duration = m_CurrentTime = 0.0f;
	ZeroMemory(&m_StartValue, sizeof(m_StartValue));
	m_EndValue = m_CurrentValue = m_StartValue;
}

//----------------------------------------------------------------------------------
// cInterpolate<Type>::Init
//----------------------------------------------------------------------------------
template<class Type>
inline void cInterpolate<Type>::Init(const float StartTime, const float Duration,
									 const Type &StartValue, const Type &EndValue) {
	m_StartTime = StartTime;
	m_Duration = Duration;
	m_StartValue = StartValue;
	m_EndValue = EndValue;
	m_CurrentTime = StartTime - 1.0f;
	m_CurrentValue = StartValue;
}

//-----------------------------------------------------------------------------
// cInterpolate<Type>::GetCurrentValue
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cInterpolate<Type>::GetCurrentValue(const float Time) const {
	float dt = Time - m_StartTime;
	if(Time != m_CurrentTime) {
		m_CurrentTime = Time;
		if(dt <= 0) {
			m_CurrentValue = m_StartValue;
		} else if(dt >= m_Duration) {
			m_CurrentValue = m_EndValue;
		} else {
			m_CurrentValue = m_StartValue + (m_EndValue - m_StartValue) * (dt / m_Duration);
		}
	}
	return m_CurrentValue;
}

template<class Type>
class cInterpolateAccelDecelLinear {
public:
	cInterpolateAccelDecelLinear();
	void Init(const float StartTime, const float AccelTime, const float DecelTime, const float Duration,
		const Type &StartValue, const Type &EndValue);

	const Type GetCurrentValue(const float Time) const;
	const Type GetCurrentSpeed(const float Time) const;
	bool IsDone(const float Time) const { return Time >= m_StartTime + m_AccelTime + m_LinearTime + m_DecelTime; }

	void SetStartTime(const float StartTime) { m_StartTime = StartTime; Invalidate(); }
	void SetStartValue(const Type &StartValue) { m_StartValue = StartValue; Invalidate(); }
	void SetEndValue(const Type &EndValue) { m_EndValue = EndValue; Invalidate(); }

	float GetStartTime() const { return m_StartTime; }
	float GetEndTime() const { return m_StartTime + m_AccelTime + m_LinearTime + m_DecelTime; }
	float GetDuration() const { return m_AccelTime + m_LinearTime + m_DecelTime; }
	float GetAccelTime() const { return m_AccelTime; }
	float GetDecelTime() const { return m_DecelTime; }
	const Type & GetStartValue() const { return m_StartValue; }
	const Type & GetEndValue() const { return m_EndValue; }

private:
	float m_StartTime;
	float m_AccelTime;
	float m_LinearTime;
	float m_DecelTime;
	Type m_StartValue;
	Type m_EndValue;
	mutable cExtrapolate<Type> m_Extrapolate;
	
	const Type CalcSpeed() const;
	void Invalidate();
	void SetPhase(const float Time) const;
};

//-----------------------------------------------------------------------------
// cInterpolateAccelDecelLinear<Type>.ctor
//-----------------------------------------------------------------------------
template<class Type>
inline cInterpolateAccelDecelLinear<Type>::cInterpolateAccelDecelLinear() {
	m_StartTime = m_AccelTime = m_LinearTime = m_DecelTime = 0.0f;
	ZeroMemory(&m_StartValue, sizeof(m_StartValue));
	m_EndValue = m_StartValue;
}

//-----------------------------------------------------------------------------
// cInterpolateAccelDecelLinear<Type>::CalcSpeed
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cInterpolateAccelDecelLinear<Type>::CalcSpeed() const {
	return (m_EndValue - m_StartValue) * (1000.0f / (m_LinearTime + (m_AccelTime + m_DecelTime) * 0.5f));
}

//------------------------------------------------------------------------------------------------------------------------
// cInterpolateAccelDecelLinear<Type>::Init
//------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void cInterpolateAccelDecelLinear<Type>::Init(const float StartTime, const float AccelTime, const float DecelTime,
													 const float Duration, const Type &StartValue, const Type &EndValue) {
	m_StartTime = StartTime;
	m_AccelTime = AccelTime;
	m_DecelTime = DecelTime;
	m_StartValue = StartValue;
	m_EndValue = EndValue;

	if(Duration <= 0.0f) {
		return;
	}

	if(m_AccelTime + m_DecelTime > Duration) {
		m_AccelTime = m_AccelTime * Duration / (m_AccelTime + m_DecelTime);
		m_DecelTime = Duration - m_AccelTime;
	}

	m_LinearTime = Duration - m_AccelTime - m_DecelTime;
	Type Speed = CalcSpeed();
	
	if(m_AccelTime) {
		m_Extrapolate.Init(m_StartTime, m_AccelTime, m_StartValue, (m_StartValue - m_StartValue),
			Speed, cExtrapolate<Type>::ET_ACCELLINEAR);
	} else if(m_LinearTime) {
		m_Extrapolate.Init(m_StartTime, m_LinearTime, m_StartValue, (m_StartValue - m_StartValue),
			Speed, cExtrapolate<Type>::ET_LINEAR);
	} else {
		m_Extrapolate.Init(m_StartTime, m_DecelTime, m_StartValue, (m_StartValue - m_StartValue),
			Speed, cExtrapolate<Type>::ET_DECELLINEAR);
	}
}

//-----------------------------------------------------------------------------
// cInterpolateAccelDecelLinear<Type>::Invalidate
//-----------------------------------------------------------------------------
template<class Type>
inline void cInterpolateAccelDecelLinear<Type>::Invalidate() {
	m_Extrapolate.Init(0.0f, 0.0f, m_Extrapolate.GetStartValue(), m_Extrapolate.GetBaseSpeed(),
		CalcSpeed(), cExtrapolate<Type>::ET_NONE);
}

//--------------------------------------------------------------------------------
// cInterpolateAccelDecelLinear<Type>::SetPhase
//--------------------------------------------------------------------------------
template<class Type>
inline void cInterpolateAccelDecelLinear<Type>::SetPhase(const float Time) const {
	float dt = Time - m_StartTime;
	if(dt < m_AccelTime) {
		if(m_Extrapolate.GetExtrapolationType() != cExtrapolate<Type>::ET_ACCELLINEAR) {
			m_Extrapolate.Init(m_StartTime, m_AccelTime, m_StartValue, m_Extrapolate.GetBaseSpeed(),
				m_Extrapolate.GetSpeed(), cExtrapolate<Type>::ET_ACCELLINEAR);
		}
	} else if(dt < m_AccelTime + m_LinearTime) {
		if(m_Extrapolate.GetExtrapolationType() != cExtrapolate<Type>::ET_LINEAR) {
			m_Extrapolate.Init(m_StartTime + m_AccelTime, m_LinearTime,
				m_StartValue + m_Extrapolate.GetSpeed() * (m_AccelTime * 0.001f * 0.5f),
				m_Extrapolate.GetBaseSpeed(), m_Extrapolate.GetSpeed(), cExtrapolate<Type>::ET_LINEAR);
		}
	} else {
		if(m_Extrapolate.GetExtrapolationType() != cExtrapolate<Type>::ET_DECELLINEAR) {
			m_Extrapolate.Init(m_StartTime + m_AccelTime + m_LinearTime, m_DecelTime,
				m_EndValue - m_Extrapolate.GetSpeed() * (m_DecelTime * 0.001f * 0.5f),
				m_Extrapolate.GetBaseSpeed(), m_Extrapolate.GetSpeed(), cExtrapolate<Type>::ET_DECELLINEAR);
		}
	}
}

//---------------------------------------------------------------------------------------------
// cInterpolateAccelDecelLinear<Type>::GetCurrentValue
//---------------------------------------------------------------------------------------------
template<class Type>
inline const Type cInterpolateAccelDecelLinear<Type>::GetCurrentValue(const float Time) const {
	SetPhase(Time);
	return m_Extrapolate.GetCurrentValue(Time);
}

//---------------------------------------------------------------------------------------------
// cInterpolateAccelDecelLinear<Type>::GetCurrentSpeed
//---------------------------------------------------------------------------------------------
template<class Type>
inline const Type cInterpolateAccelDecelLinear<Type>::GetCurrentSpeed(const float Time) const {
	SetPhase(Time);
	return m_Extrapolate.GetCurrentSpeed(Time);
}

template<class Type>
class cInterpolateAccelDecelSine {
public:
	cInterpolateAccelDecelSine();
	void Init(const float StartTime, const float AccelTime, const float DecelTime, const float Duration,
		const Type &StartValue, const Type &EndValue);

	const Type GetCurrentValue(const float Time) const;
	const Type GetCurrentSpeed(const float Time) const;

	bool IsDone(const float Time) const { return Time >= m_StartTime + m_AccelTime + m_LinearTime + m_DecelTime; }

	void SetStartTime(const float StartTime) { m_StartTime = StartTime; Invalidate(); }
	void SetStartValue(const Type &StartValue) { m_StartValue = StartValue; Invalidate(); }
	void SetEndValue(const Type &EndValue) { m_EndValue = EndValue; Invalidate(); }

	float GetStartTime() const { return m_StartTime; }
	float GetEndTime() const { return m_StartTime + m_AccelTime + m_LinearTime + m_DecelTime; }
	float GetDuration() const { return m_AccelTime + m_LinearTime + m_DecelTime; }
	float GetAccelTime() const { return m_AccelTime; }
	float GetDecelTime() const { return m_DecelTime; }
	const Type & GetStartValue() const { return m_StartValue; }
	const Type & GetEndValue() const { return m_EndValue; }

private:
	float m_StartTime;
	float m_AccelTime;
	float m_LinearTime;
	float m_DecelTime;
	Type m_StartValue;
	Type m_EndValue;
	mutable cExtrapolate<Type> m_Extrapolate;
	
	const Type CalcSpeed() const;
	void Invalidate();
	void SetPhase(const float Time) const;
};

//-----------------------------------------------------------------------------
// cInterpolateAccelDecelSine<Type>.ctor
//-----------------------------------------------------------------------------
template<class Type>
inline cInterpolateAccelDecelSine<Type>::cInterpolateAccelDecelSine() {
	m_StartTime = m_AccelTime = m_LinearTime = m_DecelTime = 0.0f;
	ZeroMemory(&m_StartValue, sizeof(m_StartValue));
	m_EndValue = m_StartValue;
}

//-----------------------------------------------------------------------------
// cInterpolateAccelDecelSine<Type>::CalcSpeed
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cInterpolateAccelDecelSine<Type>::CalcSpeed() const {
	return (m_EndValue - m_StartValue) * (1000.0f / (m_LinearTime + (m_AccelTime + m_DecelTime) * cMath::Sqrt1Over2));
}

//----------------------------------------------------------------------------------------------------------------------
// cInterpolateAccelDecelSine<Type>::Init
//----------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void cInterpolateAccelDecelSine<Type>::Init(const float StartTime, const float AccelTime, const float DecelTime,
												   const float Duration, const Type &StartValue, const Type &EndValue) {
	m_StartTime = StartTime;
	m_AccelTime = AccelTime;
	m_DecelTime = DecelTime;
	m_StartValue = StartValue;
	m_EndValue = EndValue;

	if(Duration <= 0.0f) {
		return;
	}

	if(m_AccelTime + m_DecelTime > Duration) {
		m_AccelTime = m_AccelTime * Duration / (m_AccelTime + m_DecelTime);
		m_DecelTime = Duration - m_AccelTime;
	}

	m_LinearTime = Duration - m_AccelTime - m_DecelTime;
	Type Speed = CalcSpeed();
	
	if(m_AccelTime) {
		m_Extrapolate.Init(m_StartTime, m_AccelTime, m_StartValue, (m_StartValue - m_StartValue),
			Speed, cExtrapolate<Type>::ET_ACCELSINE);
	} else if(m_LinearTime) {
		m_Extrapolate.Init(m_StartTime, m_LinearTime, m_StartValue, (m_StartValue - m_StartValue),
			Speed, cExtrapolate<Type>::ET_LINEAR);
	} else {
		m_Extrapolate.Init(m_StartTime, m_DecelTime, m_StartValue, (m_StartValue - m_StartValue),
			Speed, cExtrapolate<Type>::ET_DECELSINE);
	}
}

//-----------------------------------------------------------------------------
// cInterpolateAccelDecelSine<Type>::Invalidate
//-----------------------------------------------------------------------------
template<class Type>
inline void cInterpolateAccelDecelSine<Type>::Invalidate() {
	m_Extrapolate.Init(0.0f, 0.0f, m_Extrapolate.GetStartValue(), m_Extrapolate.GetBaseSpeed(),
		CalcSpeed(), cExtrapolate<Type>::ET_NONE);
}

//--------------------------------------------------------------------------------
// cInterpolateAccelDecelSine<Type>::SetPhase
//--------------------------------------------------------------------------------
template<class Type>
inline void cInterpolateAccelDecelSine<Type>::SetPhase(const float Time) const {
	float dt = Time - m_StartTime;
	if(dt < m_AccelTime) {
		if(m_Extrapolate.GetExtrapolationType() != cExtrapolate<Type>::ET_ACCELSINE) {
			m_Extrapolate.Init(m_StartTime, m_AccelTime, m_StartValue, m_Extrapolate.GetBaseSpeed(),
				m_Extrapolate.GetSpeed(), cExtrapolate<Type>::ET_ACCELSINE);
		}
	} else if(dt < m_AccelTime + m_LinearTime) {
		if(m_Extrapolate.GetExtrapolationType() != cExtrapolate<Type>::ET_LINEAR) {
			m_Extrapolate.Init(m_StartTime + m_AccelTime, m_LinearTime,
				m_StartValue + m_Extrapolate.GetSpeed() * (m_AccelTime * 0.001f * cMath::Sqrt1Over2),
				m_Extrapolate.GetBaseSpeed(), m_Extrapolate.GetSpeed(), cExtrapolate<Type>::ET_LINEAR);
		}
	} else {
		if(m_Extrapolate.GetExtrapolationType() != cExtrapolate<Type>::ET_DECELSINE) {
			m_Extrapolate.Init(m_StartTime + m_AccelTime + m_LinearTime, m_DecelTime,
				m_EndValue - m_Extrapolate.GetSpeed() * (m_DecelTime * 0.001f * cMath::Sqrt1Over2),
				m_Extrapolate.GetBaseSpeed(), m_Extrapolate.GetSpeed(), cExtrapolate<Type>::ET_DECELSINE);
		}
	}
}

//-------------------------------------------------------------------------------------------
// cInterpolateAccelDecelSine<Type>::GetCurrentValue
//-------------------------------------------------------------------------------------------
template<class Type>
inline const Type cInterpolateAccelDecelSine<Type>::GetCurrentValue(const float Time) const {
	SetPhase(Time);
	return m_Extrapolate.GetCurrentValue(Time);
}

//-------------------------------------------------------------------------------------------
// cInterpolateAccelDecelSine<Type>::GetCurrentSpeed
//-------------------------------------------------------------------------------------------
template<class Type>
inline const Type cInterpolateAccelDecelSine<Type>::GetCurrentSpeed(const float Time) const {
	SetPhase(Time);
	return m_Extrapolate.GetCurrentSpeed(Time);
}