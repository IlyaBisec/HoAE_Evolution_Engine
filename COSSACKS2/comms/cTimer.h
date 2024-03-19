#pragma once

class cTimer {
public:
	cTimer();
	// profiling based on ticksMeanTime, msMeanTime as well as "FramesPerSec":
	void Begin();
	bool End(const int nCycle = 1);
	// profiling based on "FramesPerSecond" only:
	bool Frame(const int nCycle = 0xff);
	
	float FramesPerSec;
	float msMeanTime;
	float ticksMeanTime;

	
	static double GetClockTicks();
	static double ClockTicksPerSecond();
	static double GetClockSeconds();
	static double GetClockMilliseconds();
private:

	doubles m_TimeTable;
	double m_Begin, m_CycleBegin;
	int m_nCalls;

	static const int NotQueried = - 1;
};

//-----------------------------------------------------------------------------
// cTimer::GetClockTicks : double()
//-----------------------------------------------------------------------------
inline double cTimer::GetClockTicks() {
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return (double)li.QuadPart;
}

//-----------------------------------------------------------------------------
// cTimer::GetClockSeconds : double()
//-----------------------------------------------------------------------------
inline double cTimer::GetClockSeconds() {
	return GetClockTicks() / ClockTicksPerSecond();
}

//-----------------------------------------------------------------------------
// cTimer::GetClockMilliseconds
//-----------------------------------------------------------------------------
inline double cTimer::GetClockMilliseconds() {
	return GetClockTicks() / (ClockTicksPerSecond() * 0.001);
}

//-----------------------------------------------------------------------------
// cTimer::ClockTicksPerSecond : double()
//-----------------------------------------------------------------------------
inline double cTimer::ClockTicksPerSecond() {
	LARGE_INTEGER liFrequency;
	QueryPerformanceFrequency(&liFrequency);
	return (double)liFrequency.QuadPart;
}

// cTimer
inline cTimer::cTimer()
: ticksMeanTime(0), msMeanTime(0), FramesPerSec(0), m_Begin(NotQueried), m_nCalls(0) {}

// cTimer::Begin
inline void cTimer::Begin() {
	cAssert(m_Begin == NotQueried);

	m_Begin = cTimer::GetClockTicks();
	if(m_TimeTable.Count() == 0) {
		m_CycleBegin = m_Begin;
	}
}

// cTimer::End
inline bool cTimer::End(const int nCycle) {
	cAssert(m_Begin != NotQueried);

	double tEnd = cTimer::GetClockTicks();
	m_TimeTable.Add(tEnd - m_Begin), m_Begin = NotQueried;

	int nCalls = m_TimeTable.Count();
	if(nCalls >= nCycle) {
		// Mean Time:
		// (ticks)
		double ticks = 0;
		for(int i = 0; i < nCalls; i++) {
			ticks += m_TimeTable[i];
		}
		ticks /= double(nCalls);
		ticksMeanTime = float(ticks);
		// (ms)
		double TicksPerSecond = cTimer::ClockTicksPerSecond();
		msMeanTime = float(ticks / (TicksPerSecond * 0.001));
		// FramesPerSec:
		double sec = (tEnd - m_CycleBegin) / TicksPerSecond;
		FramesPerSec = float(double(nCalls) / sec);
		// ----------------------------------------
		m_TimeTable.Free();
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// cTimer::Frame
//-----------------------------------------------------------------------------
inline bool cTimer::Frame(const int nCycle) {
	if(!m_nCalls) {
		m_CycleBegin = cTimer::GetClockTicks();
	}
	m_nCalls++;

	if(m_nCalls >= nCycle) {
		double tCur = cTimer::GetClockTicks();
		double TicksPerSecond = cTimer::ClockTicksPerSecond();
		// FramesPerSec:
		double sec = (tCur - m_CycleBegin) / TicksPerSecond;
		FramesPerSec = float(double(m_nCalls) / sec);
		// -------------------------------------------------
		m_nCalls = 0;
		return true;
	}
	return false;
}