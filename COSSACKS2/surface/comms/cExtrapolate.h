#pragma once

template<class Type>
class cExtrapolate {
public:
	typedef enum {
		ET_NONE					= 0x01,
		ET_LINEAR				= 0x02,
		ET_LINEAR_NOSTOP		= 0x42,
		ET_ACCELLINEAR			= 0x04,
		ET_ACCELLINEAR_NOSTOP	= 0x44,
		ET_DECELLINEAR			= 0x08,
		ET_DECELLINEAR_NOSTOP	= 0x48,
		ET_ACCELSINE			= 0x10,
		ET_ACCELSINE_NOSTOP		= 0x50,
		ET_DECELSINE			= 0x20,
		ET_DECELSINE_NOSTOP		= 0x60,
		ET_NOSTOP				= 0x40
	} eExtrapolationType;

	cExtrapolate();
	void Init(const float StartTime, const float Duration, const Type &StartValue,
		const Type &BaseSpeed, const Type &Speed, const eExtrapolationType ExtrapolationType);
	
	const Type GetCurrentValue(const float Time) const;
	const Type GetCurrentSpeed(const float Time) const;

	bool IsDone(const float Time) const { return !(m_ExtrapolationType & ET_NOSTOP) && Time >= m_StartTime + m_Duration; }
	
	void SetStartTime(const float Time) { m_StartTime = Time; m_CurrentTime = -1; }
	void SetStartValue(const Type &StartValue) { m_StartValue = StartValue; m_CurrentTime = -1; }
	
	float GetStartTime() const { return m_StartTime; }
	float GetEndTime() const { !(m_ExtrapolationType & ET_NOSTOP) && m_Duration > 0 ? m_StartTime + m_Duration : 0; }
	float GetDuration() const { return m_Duration; }
	const Type & GetStartValue() const { return m_StartValue; }
	const Type & GetBaseSpeed() const { return m_BaseSpeed; }
	const Type & GetSpeed() const { return m_Speed; }
	const eExtrapolationType GetExtrapolationType() const { return m_ExtrapolationType; }

private:
	eExtrapolationType m_ExtrapolationType;
	float m_StartTime;
	float m_Duration;
	Type m_StartValue;
	Type m_BaseSpeed;
	Type m_Speed;
	mutable float m_CurrentTime;
	mutable Type m_CurrentValue;
};

typedef cExtrapolate<float> cExtrapolate1;
typedef cExtrapolate<cVec2> cExtrapolate2;
typedef cExtrapolate<cVec3> cExtrapolate3;
typedef cExtrapolate<cAngles> cExtrapolateA;

//-----------------------------------------------------------------------------
// cExtrapolate<Type>::ctor
//-----------------------------------------------------------------------------
template<class Type>
inline cExtrapolate<Type>::cExtrapolate() {
	m_ExtrapolationType = ET_NONE;
	m_StartTime = m_Duration = 0.0f;
	ZeroMemory(&m_StartValue, sizeof(m_StartValue));
	m_BaseSpeed = m_Speed = m_CurrentValue = m_StartValue;
	m_CurrentTime = -1;
}

//--------------------------------------------------------------------------------------------------------------------------
// cExtrapolate<Type>::Init
//--------------------------------------------------------------------------------------------------------------------------
template<class Type>
inline void cExtrapolate<Type>::Init(const float StartTime, const float Duration, const Type &StartValue,
									 const Type &BaseSpeed, const Type &Speed, const eExtrapolationType ExtrapolationType) {
	m_StartTime = StartTime;
	m_Duration = Duration;
	m_StartValue = m_CurrentValue = StartValue;
	m_BaseSpeed = BaseSpeed;
	m_Speed = Speed;
	m_ExtrapolationType = ExtrapolationType;
	m_CurrentTime = -1;
}

//-----------------------------------------------------------------------------
// cExtrapolate<Type>::GetCurrentValue
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cExtrapolate<Type>::GetCurrentValue(const float Time) const {
	if(Time == m_CurrentTime) {
		return m_CurrentValue;
	}
	
	m_CurrentTime = Time;

	if(Time < m_StartTime) {
		return m_StartValue;
	}
	
	float CurTime = Time;
	if(!(m_ExtrapolationType & ET_NOSTOP) && CurTime > m_StartTime + m_Duration) {
		CurTime = m_StartTime + m_Duration;
	}

	float dt, s;
	switch(m_ExtrapolationType & ~ET_NOSTOP) {
		case ET_NONE:
			dt = (CurTime - m_StartTime) * 0.001f;
			m_CurrentValue = m_StartValue + dt * m_BaseSpeed;
			break;
		case ET_LINEAR:
			dt = (CurTime - m_StartTime) * 0.001f;
			m_CurrentValue = m_StartValue + dt * (m_BaseSpeed + m_Speed);
			break;
		case ET_ACCELLINEAR:
			if(!m_Duration) {
				m_CurrentValue = m_StartValue;
			} else {
				dt = (CurTime - m_StartTime) / m_Duration;
				s = (0.5f * dt * dt) * (m_Duration * 0.001f);
				m_CurrentValue = m_StartValue + dt * m_BaseSpeed + s * m_Speed;
			}
			break;
		case ET_DECELLINEAR:
			if(!m_Duration) {
				m_CurrentValue = m_StartValue;
			} else {
				dt = (CurTime - m_StartTime) / m_Duration;
				s = (dt - 0.5f * dt * dt) * (m_Duration * 0.001f);
				m_CurrentValue = m_StartValue + dt * m_BaseSpeed + s * m_Speed;
			}
			break;
		case ET_ACCELSINE:
			if(!m_Duration) {
				m_CurrentValue = m_StartValue;
			} else {
				dt = (CurTime - m_StartTime) / m_Duration;
				s = (1.0f - cMath::Cos(dt * cMath::HalfPi)) * m_Duration * 0.001f * cMath::Sqrt1Over2;
				m_CurrentValue = m_StartValue + dt * m_BaseSpeed + s * m_Speed;
			}
			break;
		case ET_DECELSINE:
			if(!m_Duration) {
				m_CurrentValue = m_StartValue;
			} else {
				dt = (CurTime - m_StartTime) / m_Duration;
				s = cMath::Sin(dt * cMath::HalfPi) * m_Duration * 0.001f * cMath::Sqrt1Over2;
				m_CurrentValue = m_StartValue + dt * m_BaseSpeed + s * m_Speed;
			}
			break;
	}
	return m_CurrentValue;
}

//-----------------------------------------------------------------------------
// cExtrapolate<Type>::GetCurrentSpeed
//-----------------------------------------------------------------------------
template<class Type>
inline const Type cExtrapolate<Type>::GetCurrentSpeed(const float Time) const {
	if(Time < m_StartTime || !m_Duration) {
		return m_StartValue - m_StartValue;
	}

	if(!(m_ExtrapolationType & ET_NOSTOP) && Time > m_StartTime + m_Duration) {
		return m_StartValue - m_StartValue;
	}

	float dt, s;
	switch(m_ExtrapolationType & ~ET_NOSTOP) {
		case ET_NONE:
			return m_BaseSpeed;
		case ET_LINEAR:
			return m_BaseSpeed + m_Speed;
		case ET_ACCELLINEAR:
			dt = (Time - m_StartTime) / m_Duration;
			s = dt;
			return m_BaseSpeed + s * m_Speed;
		case ET_DECELLINEAR:
			dt = (Time - m_StartTime) / m_Duration;
			s = 1.0f - dt;
			return m_BaseSpeed + s * m_Speed;
		case ET_ACCELSINE:
			dt = (Time - m_StartTime) / m_Duration;
			s = cMath::Sin(dt * cMath::HalfPi);
			return m_BaseSpeed + s * m_Speed;
		case ET_DECELSINE:
			dt = (Time - m_StartTime) / m_Duration;
			s = cMath::Cos(dt * cMath::HalfPi);
			return m_BaseSpeed + s * m_Speed;
		default:
			return m_BaseSpeed;
	}
}