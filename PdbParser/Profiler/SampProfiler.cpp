#include "FSampProfiler.h"
FSampProfiler FProf;
void InitProfiler(){
	static bool isInit=0;
	if(!isInit){
		void ShowProfInit();
		ShowProfInit();
		FProf.Initialize();
		isInit=true;
	}
}
void RunProfiler(){
	InitProfiler();
    FProf.Start();
}
void StopProfiler(){
	FProf.Stop();
	FProf.SaveToFile("Profile.xml");
	FProf.Reset();
}