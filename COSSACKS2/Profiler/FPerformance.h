#ifndef _FPERFORMANCE_H_
#define _FPERFORMANCE_H_

#include <windows.h>
#include <Mmsystem.h>
#include <atlbase.h>
#include <tlhelp32.h>
#include <conio.h>
#include <math.h>
#include <assert.h>

class FPerformance {
	static double CPUFreq;
	static bool isFirst;
	unsigned __int64 StartTicks;
	unsigned __int64 StopTicks;

public:
	inline void Start(void)
	{
		unsigned t1, t2;
		__asm {
			rdtsc
			mov t1,edx
			mov t2,eax
		}
		StartTicks = ((__int64)t1<<32)|(__int64)t2;
	}

	inline void Stop(void)
	{
		unsigned t1, t2;
		__asm {
			rdtsc
			mov t1,edx
			mov t2,eax
		}
		StopTicks = ((__int64)t1<<32)|(__int64)t2;
	}

	double GetDeltaTime(void)
	{
		assert(CPUFreq > 1.0);
		return ((double)(StopTicks-StartTicks)/CPUFreq)/1000.0;
	}

	double GetCurrTime(void)
	{
		assert(CPUFreq > 1.0);
		unsigned t1, t2;
		__asm {
			rdtsc
			mov t1,edx
			mov t2,eax
		}
		return ((double)(((__int64)t1<<32)|(__int64)t2)/CPUFreq)/1000.0;
	}

	double GetCPUFreq(void);

	FPerformance(void)
	{
		if(isFirst)
		{
			CPUFreq = GetCPUFreq();
			isFirst = false;
		}
		StartTicks = StopTicks = 0;
	}
	~FPerformance(void)
	{

	}
};

#endif