#include "stdheader.h"
#include "FSampProfiler.h"
FSampProfiler FProf;
static bool isInit=0;
void InitProfiler(){	
	if(!isInit){
		void ShowProfInit();
		ShowProfInit();
		FProf.Initialize();
		isInit=true;
	}
}
int prof_dt=0;
void RunProfiler(){
	FILE* F=fopen("lags.txt","r");
	if(F){
        fscanf(F,"%d",&prof_dt);
	}
	InitProfiler();
    FProf.Start();
}
bool p_isRun=false;
void StopProfiler(){
	FProf.Stop();
	FProf.SaveToFile("Profile.xml");
	FProf.Reset();
}
void ProfilerOnFrame(){
	if(isInit){		
		bool rd=FProf.OnFrame(prof_dt);
		if(p_isRun){
			bool grn=GetKeyState(VK_NUMLOCK) & 1;
			GPS.DrawFillRect( 0,RealLy/2,10,10,grn?0xFF00FF00:(rd?0xFFFF0000:0xFFFFFF00) );
		}
	}
}