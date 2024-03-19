#ifndef __cTimer__
#define __cTimer__

#include "cUtils.h"

namespace comms {

class cTimer {
public:
	cTimer();
	// profiling based on m_ticksMeanTime, m_msMeanTime as well as m_cps:
	void Begin();
	bool End(const int nCycle = 0xff);
	// profiling based on m_cps only:
	bool Call(const int nCycle = 0xff);
	
	double m_ticksMeanTime;
	double m_msMeanTime;
	double m_cps;
private:
	double Query() const;
	double Frequency() const;

	doubles m_TimeTable;
	double m_Begin, m_CycleBegin;
	int m_nCalls;

	static const int NotQueried = - 1;
};

// cTimer
inline cTimer::cTimer()
: m_ticksMeanTime(0), m_msMeanTime(0), m_cps(0), m_Begin(NotQueried), m_nCalls(0) {}

// cTimer::Query
inline double cTimer::Query() const {
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return (double)li.QuadPart;
}

// cTimer::Frequency
inline double cTimer::Frequency() const {
	LARGE_INTEGER liFrequency;
	QueryPerformanceFrequency(&liFrequency);
	return (double)liFrequency.QuadPart;
}

// cTimer::Begin
inline void cTimer::Begin() {
	assert(m_Begin == NotQueried);

	m_Begin = Query();
	if(m_TimeTable.Count() == 0) {
		m_CycleBegin = m_Begin;
	}
}

// cTimer::End
inline bool cTimer::End(const int nCycle) {
	assert(m_Begin != NotQueried);

	double tEnd = Query();
	m_TimeTable.Add(tEnd - m_Begin), m_Begin = NotQueried;

	int nCalls = m_TimeTable.Count();
	if(nCalls >= nCycle) {
		// Mean Time:
		// (ticks)
		m_ticksMeanTime = 0;
		for(int i = 0; i < nCalls; i++) {
			m_ticksMeanTime += m_TimeTable[i];
		}
		m_ticksMeanTime /= double(nCalls);
		// (ms)
		double TicksPerSecond = Frequency();
		m_msMeanTime = m_ticksMeanTime / (TicksPerSecond * 0.001);
		// cps:
		double sec = (tEnd - m_CycleBegin) / TicksPerSecond;
		m_cps = double(nCalls) / sec;
		// ----------------------------------------
		m_TimeTable.Free();
		return true;
	}
	return false;
}

// cTimer::Call
inline bool cTimer::Call(const int nCycle) {
	if(!m_nCalls) {
		m_CycleBegin = Query();
	}
	m_nCalls++;

	if(m_nCalls >= nCycle) {
		double tCur = Query();
		double TicksPerSecond = Frequency();
		// cps:
		double sec = (tCur - m_CycleBegin) / TicksPerSecond;
		m_cps = double(m_nCalls) / sec;
		// -------------------------------------------------
		m_nCalls = 0;
		return true;
	}
	return false;
}

} // comms

#endif // __cTimer__