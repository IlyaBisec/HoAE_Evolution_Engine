#ifndef _FSAMPPROFILER_H_
#define _FSAMPPROFILER_H_

#include <windows.h>
#include <Mmsystem.h>
#include <atlbase.h>
#include <tlhelp32.h>
#include <conio.h>
#include <math.h>
#include "dia2.h"
#include "diacreate.h"
#include "cvconst.h"
#include "Psapi.h"
#include "FSampProfilerAlloc.h"
#include "FStreams\\FBaseStream.h"
#include "FPerformance.h"

/*
	В результате работы в ItemsStream находятся указатели на объекты FSample.
	Каждый объект содержит массив адресов возвратов, начиная с коренной функции и счетчик попаданий.
	В последней записи массива содержится адрес функции, в которой произошло прерывание.
*/

//#define FSAMP_PROFILER_LOG

class FSampProfiler {
	#define ROOT_FUNCTION_NAME "WinMain"
	enum { 
		MaxCallStackDepth = 64,
		MaxThreadsNumber = 1,
		TimeDistributionInterval = 30,
		MaxBreaksOnFrame = 4096
	};
	
	struct FFunction {
		unsigned Base, Len;
	};

	struct FSample {
		unsigned Count;
		unsigned CallStack[MaxCallStackDepth];
		unsigned CallStackDepth;
		FSample* Next;
	};

	class FTernTreeNode {
	public:
		FTernTreeNode* Left;
		FTernTreeNode* Middle;
		FTernTreeNode* Right;
		unsigned ch;
		FSample* Sample;
	};

	static FSPManager<FSample> FSPSamplesManager;
	#define newFSample() (FSPSamplesManager.Allocate())
	#define deleteSample(s) (FSPSamplesManager.Free(s))
	
	static FSPManager<FTernTreeNode> FSPTernTreeNodesManager;
	#define newFTernTreeNode() (FSPTernTreeNodesManager.Allocate())
	#define deleteFTernTreeNode(s) (FSPTernTreeNodesManager.Free(s))

	static FSPManager<FFunction> FSPFunctionsManager;
	#define newFFunction() (FSPFunctionsManager.Allocate())
	#define deleteFFunction(s) (FSPFunctionsManager.Free(s))

	static FTernTreeNode* Insert(FSample* Sample)
	{   
		FTernTreeNode** pt = &Root;
		FTernTreeNode* t = *pt;

		unsigned* Data = Sample->CallStack;
		int i = 0, Depth = Sample->CallStackDepth;
		while(i < Depth)
		{
			unsigned ch1 = Data[i];

			if(*pt == NULL)
			{
				*pt = newFTernTreeNode();
				(*pt)->ch = ch1;
				(*pt)->Left = (*pt)->Middle = (*pt)->Right = NULL;
				(*pt)->Sample = NULL;
			}
			unsigned ch2 = (*pt)->ch;

			if(ch1 == ch2) 
			{
				t = *pt;
				pt = &((*pt)->Middle);
				i++;
			}
			else
			if(ch1 < ch2)
			{
				t = *pt;
				pt = &((*pt)->Left);
			}
			else
			{
				t = *pt;
				pt = &((*pt)->Right);
			}
		}
		if(t->Sample == NULL)
		{
			t->Sample = newFSample();
			for(unsigned i = 0; i < Sample->CallStackDepth; i++)
				t->Sample->CallStack[i] = Sample->CallStack[i];
			t->Sample->CallStackDepth = Sample->CallStackDepth;
			t->Sample->Count = 0;
			t->Sample->Next = NULL;

			if(FirstSample == NULL)
			{
				assert(LastSample == NULL);
            FirstSample = LastSample = t->Sample;
			}
			else
			{
				LastSample->Next = t->Sample;
				LastSample = t->Sample;
			}
			SamplesNumber++;
		}
		return t;
	}
	static int LastFrameTime;
    static FSample LastFrameSamples[MaxBreaksOnFrame];
	static int LastFrameSamplesAmount;
	static FTernTreeNode *Root;
	static FSample *FirstSample;
	static FSample *LastSample;
	static int SamplesNumber;
	static HANDLE hThreads[MaxThreadsNumber];
	static HANDLE hMainThrd;
	static FFunction** Functions;
	static int FunctionsNumber;
	static bool	isDIAInitialized;
	static CComPtr<IDiaDataSource> pSource;
	static CComPtr<IDiaSession> pDiaSession; 
	static CRITICAL_SECTION cs;
	static int RootFunctionIdx;
	static unsigned RootFunctionBase;
	static unsigned MissedSamples;
	static bool isInit;
	static double AverTimeBetweenSamplings;
	static double MinTimeBetweenSamplings;
	static double MaxTimeBetweenSamplings;
	static double LastSamplingTime;
	static double SamplingsTime;
	static unsigned SamplingsCount;
	static unsigned TimeDistribution[TimeDistributionInterval+1];
	static f_FOutStream ModulesStream;
	#ifdef FSAMP_PROFILER_LOG
		static f_FOutStream TimesStream;
	#endif

	static DWORD WINAPI ThreadFunc(LPVOID lpParam);
	static int FFunction_compare(const void *arg1, const void *arg2);
	static int FSample_compare(const void *arg1, const void *arg2);
	bool InitDIA(void);
	char* GetFunctionByRVA(unsigned rva);
	bool DumpFunctions(char* RootFunctionName);
	void Fatal(const char *msg);
	static void RegisterMissedSample(unsigned Address);
	static void* GetCurrentModuleBase(void);
	static int GetCallStackByEBP(unsigned* CallStack, int MaxDepth, unsigned _ebp = 0);
	static int FindFunctionByVA(unsigned va);
	static double GetCPUFreq(void);
	static void EnumerateLoadedModules(void);
public:
	bool Initialize(char* RootFunctionName = ROOT_FUNCTION_NAME);
	bool Start(void);
	bool Stop(void);
	void SaveToFile(char* FileName);
	bool Reset(void);
	bool Release(void);
	bool OnFrame(int MinTimeDiff);

	FPerformance fpWork;

	FSampProfiler(void)
	{
		if(isInit) 
			throw("Release the existing FSampProfiler object first!");

		hMainThrd = 0;
		Functions = NULL;
		Root = NULL;
		FirstSample = NULL;
		LastSample = NULL;
		FunctionsNumber = 0;
		RootFunctionIdx = -1;
		SamplesNumber = 0;
	}
	~FSampProfiler(void)
	{
		Release();
	}
};

#endif