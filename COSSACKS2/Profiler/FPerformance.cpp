#include "FPerformance.h"
//------------------------------------------------------------------------------------------------------
double FPerformance::CPUFreq = 0.0;
bool FPerformance::isFirst = true;
//------------------------------------------------------------------------------------------------------
double FPerformance::GetCPUFreq(void)
{
	unsigned tStart1, tStart2, tEnd1, tEnd2;
   timeBeginPeriod(1);
	unsigned time1 = timeGetTime();
	__asm {
		rdtsc
		mov tStart1,edx
		mov tStart2,eax
	}
	Sleep(1000);
	__asm {
		rdtsc
		mov tEnd1,edx
		mov tEnd2,eax
	}
	unsigned time2 = timeGetTime();
   timeEndPeriod(1);
	__int64 tDelta = (((__int64)tEnd1<<32)|(__int64)tEnd2) - (((__int64)tStart1<<32)|(__int64)tStart2);
	double fr = tDelta/(double)(time2-time1)/1000.0;
	return fr;
}
//------------------------------------------------------------------------------------------------------
