//------------------------------------------------------------------------------------------------------
#include "FSampProfiler.h"
//------------------------------------------------------------------------------------------------------
HANDLE FSampProfiler::hThreads[FSampProfiler::MaxThreadsNumber];
HANDLE FSampProfiler::hMainThrd = 0;
CRITICAL_SECTION FSampProfiler::cs;
FSampProfiler::FFunction** FSampProfiler::Functions = NULL;
FSampProfiler::FTernTreeNode* FSampProfiler::Root = NULL;
FSampProfiler::FSample* FSampProfiler::FirstSample = NULL;
FSampProfiler::FSample* FSampProfiler::LastSample = NULL;
FSPManager<FSampProfiler::FSample> FSampProfiler::FSPSamplesManager;
FSPManager<FSampProfiler::FTernTreeNode> FSampProfiler::FSPTernTreeNodesManager;
FSPManager<FSampProfiler::FFunction> FSampProfiler::FSPFunctionsManager;
int FSampProfiler::FunctionsNumber = 0;
int FSampProfiler::RootFunctionIdx = -1;
unsigned FSampProfiler::RootFunctionBase = 0;
unsigned FSampProfiler::MissedSamples = 0;
int FSampProfiler::SamplesNumber = 0;
bool FSampProfiler::isDIAInitialized = false;
bool FSampProfiler::isInit = false;
double FSampProfiler::AverTimeBetweenSamplings = 0;
double FSampProfiler::MinTimeBetweenSamplings = 0;
double FSampProfiler::MaxTimeBetweenSamplings = 0;
double FSampProfiler::LastSamplingTime = 0;
double FSampProfiler::SamplingsTime = 0;
unsigned FSampProfiler::SamplingsCount = 0;
unsigned FSampProfiler::TimeDistribution[FSampProfiler::TimeDistributionInterval+1];
f_FOutStream FSampProfiler::ModulesStream;
CComPtr<IDiaDataSource> FSampProfiler::pSource;
CComPtr<IDiaSession> FSampProfiler::pDiaSession; 
#ifdef FSAMP_PROFILER_LOG
	f_FOutStream FSampProfiler::TimesStream;
#endif
//------------------------------------------------------------------------------------------------------
int FSampProfiler::FindFunctionByVA(unsigned va)
{
	int n1 = 0, n2 = FunctionsNumber/2, n3 = FunctionsNumber, num = 0;
	if(va >= Functions[n1]->Base && va < Functions[n3-1]->Base + Functions[n3-1]->Len)
	{
		while(1)
		{
			if(va < Functions[n2]->Base)
			{
				// - левее центра
				if(n1 == n2) break;
				n3 = n2;
			}
			else	
			if(va > Functions[n2]->Base)
			{
				// - правее центра
				if(n2 == n3) break;
				n1 = n2;
			}
			int _n2 = (n1 + n3)>>1;
			if(_n2 == n2) break;
			n2 = _n2;

			// - нашли текущую функцию
			if(va >= Functions[n2]->Base && va < Functions[n2]->Base + Functions[n2]->Len)
				return n2;
			assert(num++ < FunctionsNumber);
		}
	}
	return -1;
}
//------------------------------------------------------------------------------------------------------
DWORD WINAPI FSampProfiler::ThreadFunc(LPVOID lpParam)
{ 
	EnterCriticalSection(&cs);
	FPerformance fpCurr;
	while(1)
	{
		timeBeginPeriod(1);
		LeaveCriticalSection(&cs);
		Sleep(5);
		EnterCriticalSection(&cs);
		timeEndPeriod(1);

		// - check the time
		fpCurr.Start();
		double CurrTime = fpCurr.GetCurrTime();
		double DeltaTime = CurrTime - LastSamplingTime;
		// - get the maximum time between samplings
		if(MaxTimeBetweenSamplings < DeltaTime)
			MaxTimeBetweenSamplings = DeltaTime;
		// - get the minimum time between samplings
		if(MinTimeBetweenSamplings > DeltaTime)
			MinTimeBetweenSamplings = DeltaTime;
		// - get the summary time between samplings
		AverTimeBetweenSamplings += DeltaTime;
		
		int Idx = (int)(ceil(DeltaTime));
		if(Idx > TimeDistributionInterval) Idx = TimeDistributionInterval;
		TimeDistribution[Idx]++;
		LastSamplingTime = CurrTime;
		if(GetKeyState(VK_NUMLOCK) & 1)
		{
			// - get the thread context
			static CONTEXT cnt;
			cnt.ContextFlags = CONTEXT_CONTROL;
			SuspendThread(hMainThrd);
			GetThreadContext(hMainThrd, &cnt);

			// - получаем call-stack (CallStack[1] указывает на функцию, вызвавшую данную)
			static unsigned CallStack[MaxCallStackDepth+1];
			int Depth = GetCallStackByEBP(CallStack+1, MaxCallStackDepth, cnt.Ebp)+1;
			CallStack[0] = cnt.Eip;

			// - получим границы корневой функции
			unsigned RootFunctionBase = Functions[RootFunctionIdx]->Base;
			unsigned RootFunctionLen = Functions[RootFunctionIdx]->Len;
			
			// - индекс прерванной функции
			int CurrFunctionIdx = -1;

			// - найдем корень, должен быть в корневой функции
			int Idx = -1;
			for(int i = 0; i < Depth && i < MaxCallStackDepth; i++)
			{
				// - адрес внутри очередной функции
				unsigned Addr = CallStack[i];
				if(Addr >= RootFunctionBase && Addr < RootFunctionBase + RootFunctionLen)
				{
					assert(Idx == -1);
					Idx = i;
				}
				if(CurrFunctionIdx == -1)
					CurrFunctionIdx = FindFunctionByVA(Addr);
			}
			if(CurrFunctionIdx != -1)
			{
				// - занесем вместо адреса прерывания адрес прерванной функции
				CallStack[0] = Functions[CurrFunctionIdx]->Base;
				if(Idx != -1)
				{
					// - нашли путь
					FSample smp;
					smp.Count = 0;
					smp.CallStackDepth = 0;
					// - заносим call-stack, начиная с корневой функции
					for(int i = Idx; i >= 0; i--)
						smp.CallStack[smp.CallStackDepth++] = CallStack[i];
					FTernTreeNode* t = Insert(&smp);
					t->Sample->Count++;
				}
				else
				{
					// - нашли какую-то функцию, но без корневой
					RegisterMissedSample(cnt.Eip);
				}
			}
			else
			{
				// - промах
				RegisterMissedSample(cnt.Eip);
			}
			ResumeThread(hMainThrd);

			fpCurr.Stop();
			SamplingsTime += fpCurr.GetDeltaTime();
			SamplingsCount++;

			#ifdef FSAMP_PROFILER_LOG
				TimesStream.putint(fpCurr.GetCurrTime()*1000);
				TimesStream.putint(fpCurr.GetDeltaTime()*1000);
			#endif
		}
	}
	LeaveCriticalSection(&cs);
	return 0; 
} 
//------------------------------------------------------------------------------------------------------
bool FSampProfiler::Initialize(char* RootFunctionName)
{
	if(isInit) return true;
	isInit = true;

	if(!DumpFunctions(RootFunctionName)) return false;
	qsort(Functions, FunctionsNumber, sizeof(FFunction*), FFunction_compare);

	RootFunctionIdx = FindFunctionByVA(RootFunctionBase);
	if(RootFunctionIdx == -1)
		Fatal("RootFunctionIdx getting error: FindFunctionByVA");

	MissedSamples = 0;
	SamplingsTime = 0;
	SamplingsCount = 0;
	AverTimeBetweenSamplings = 0;
	MinTimeBetweenSamplings = 1000.0*60*60;
	MaxTimeBetweenSamplings = 0;
	memset(TimeDistribution, 0, sizeof(TimeDistribution[0])*TimeDistributionInterval);
	EnumerateLoadedModules();

	InitializeCriticalSection(&cs);

	HANDLE hThrd = GetCurrentThread();
	BOOL fSuccess = DuplicateHandle(
		GetCurrentProcess(), 
		hThrd, 
		GetCurrentProcess(), 
		&hMainThrd,
		0,
      FALSE,
      DUPLICATE_SAME_ACCESS);

	for(int i = 0; i < MaxThreadsNumber; i++)
	{
		DWORD dwThreadId;
//		hThreads[i] = CreateThread(NULL, 0, ThreadFunc, &hMainThrd, CREATE_SUSPENDED, &dwThreadId);
		hThreads[i] = CreateThread(NULL, 0, ThreadFunc, (void*)i, CREATE_SUSPENDED, &dwThreadId);
		if(hThreads[i] == NULL)
		   Fatal("CreateThread failed");
//		SetThreadPriority(hThreads[i], 0);
	}
	return true;
}
//------------------------------------------------------------------------------------------------------
bool FSampProfiler::Start(void)
{
	assert(isInit);
	EnterCriticalSection(&cs);
	// - сохраним начало старта
	fpWork.Start();
	// - обнулим счетчик семплингов
	SamplingsTime = 0.0;
	SamplingsCount = 0;
	FPerformance fp;
	LastSamplingTime = fp.GetCurrTime();
	#ifdef FSAMP_PROFILER_LOG
		TimesStream.reload(8*2*4096);
	#endif
	for(int i = 0; i < MaxThreadsNumber; i++)
		if(hThreads[i]) ResumeThread(hThreads[i]);
	LeaveCriticalSection(&cs);
	return true;
}
//------------------------------------------------------------------------------------------------------
bool FSampProfiler::Stop(void)
{
	assert(isInit);
	EnterCriticalSection(&cs);
	for(int i = 0; i < MaxThreadsNumber; i++)
	{
		if(hThreads[i]) 
		{
			if(SuspendThread(hThreads[i]) == -1)
				Fatal("SuspendThread");
		}
	}
	fpWork.Stop();
	LeaveCriticalSection(&cs);
	return true;
}
//------------------------------------------------------------------------------------------------------
bool FSampProfiler::Reset(void)
{
	assert(isInit);

	// - удаляем все семплы
	FSPSamplesManager.FreeAll();
	MissedSamples = 0;
	SamplesNumber = 0;
	SamplingsTime = 0;
	SamplingsCount = 0;
	FirstSample = LastSample = NULL;
	
	// - удаляем все узлы дерева
	FSPTernTreeNodesManager.FreeAll();
	Root = NULL;
	return true;
}
//------------------------------------------------------------------------------------------------------
bool FSampProfiler::Release(void)
{
	assert(isInit);

	// - сброс данных
	Reset();

	for(int i = 0; i < MaxThreadsNumber; i++)
	{
		if(hThreads[i]) 
		{
			CloseHandle(hThreads[i]);
			hThreads[i] = 0;
		}
	}
	DeleteCriticalSection(&cs);

	// - удаляем массив указателей на записи о функциях
	delete[] Functions;
	Functions = NULL;
	FunctionsNumber = 0;

	// -удаляем строки-имена модулей
	unsigned* Data = (unsigned*)ModulesStream.Data;
	int Number = ModulesStream.Size/(4*3);
	for(int i = 0; i < Number; i++)
	{
		char* Str = (char*)(Data[i*3+2]);
		delete[] Str;
	}
	isInit = false;
	return true;
}
//------------------------------------------------------------------------------------------------------

