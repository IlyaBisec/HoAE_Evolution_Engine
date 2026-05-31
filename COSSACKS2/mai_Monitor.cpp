#include "stdheader.h"
#include ".\mai_monitor.h"

mai_Monitor vaiMonitor;

mai_Monitor::mai_Monitor(void)
{
}

mai_Monitor::~mai_Monitor(void)
{
}
//
extern word FULLWAY[128];
extern int NWPTS;
//
bool v_maiActive=false;
//
CEXPORT void maiSetGroupID(word ID, byte Type){
	//vaiMonitor.AC_Group.Way.FastClear();
	v_maiActive=false;
	//
	if(Type==3){
		if(vaiMonitor.AC_Group.GroupID==0xFFFF){
			v_maiActive=true;
			vaiMonitor.AC_Group.GroupID=0xFFFE;
			vaiMonitor.AC_Group.Type=1;
		}
	}else
	if(ID<SCENINF.UGRP.GetAmount()){
		vaiMonitor.AC_Group.GroupID=ID;
		UnitsGroup* UG=&SCENINF.UGRP[ID];
		for(int i=0;i<UG->N;i++){
			OneObject* OB=Group[UG->IDS[i]];
			if(OB&&OB->Serial==UG->SNS[i]&&OB->Selected&(1<<MyNation)){
				if(vaiMonitor.AC_Group.GroupID!=ID){
					vaiMonitor.AC_Group.GroupID=ID;
					//vaiMonitor.AC_Group.Way.FastClear();
				}
				v_maiActive=true;
				vaiMonitor.AC_Group.Type=Type;
				break;
			}
		}
	}else{
		vaiMonitor.AC_Group.GroupID=0xFFFF;		
	}
};
CEXPORT void maiSetTargets(word* Targets){
	if(v_maiActive){
		memcpy(vaiMonitor.AC_Group.Targets,Targets,sizeof vaiMonitor.AC_Group.Targets);
	}	
}
CEXPORT void maiSetDangerMap(int* DangerMap){
	if(v_maiActive){
		memcpy(vaiMonitor.AC_Group.DangerMap,DangerMap,sizeof vaiMonitor.AC_Group.DangerMap);
	}
}
CEXPORT void maiSetWayArray(){	
	if(v_maiActive){
		vaiMonitor.AC_Group.Way.FastClear();
		for(int i=NWPTS-1;i>=0;i--){
			vaiMonitor.AC_Group.Way.Add(FULLWAY[i]);
		}
	}
}
CEXPORT int maiGetDangIteration(){
	return vaiMonitor.AC_Group.dangIteration;
}
CEXPORT int maiGetDangFear(){
	return vaiMonitor.AC_Group.dangFear;
}
//
DLLEXPORT bool GetTopZoneCoor(int Zone,int* x,int* y);
DLLEXPORT int GetTopZone(int x,int y);
void DrawWorldLine(float x0,float y0,float x1,float y1,DWORD Diffuse);
DLLEXPORT int FindNextZoneOnTheSafeWay(int Start,int Fin,short* DangerMap,int* MaxDanger,int DangSteps);
DLLEXPORT int FindNextZoneOnTheSafeWayEx(int Start,int Fin,int* DangerMap,int* MaxDanger,int DangSteps, byte TopType=0);
extern int vMx,vMy;
extern int vMapx,vMapy;
//
void mai_ACGroup::DrawOnMap(){
	if(Visualization&&GroupID!=0xFFFF){
		//
		int i=Way.GetAmount()-2;
		while(i>=0){
            int x0,y0;
			int x1,y1;
			if(GetTopZoneCoor(Way[i],&x0,&y0)&&GetTopZoneCoor(Way[i+1],&x1,&y1)){
				DrawWorldLine(x0<<6,y0<<6,x1<<6,y1<<6,0xFF2020FF);
				//GPS.DrawLine(V0.x,V0.y,V1.x,V1.y,0xFF2020BB);	
				//GetTotalHeight
			}
			i--;
		}
		//
		_str str;
		i=GetNAreas();
		while(i>0){
			i--;
			int x,y;
			if((DangerMap[i]||Targets[i]!=0xFFFF)&&GetTopZoneCoor(i,&x,&y)){
				x<<=6;
				y<<=6;	
				Vector3D v = SkewPt(x,y,GetTotalHeight(x,y));
				WorldToScreenSpace(v);
				//!IMM->IsPointVisible(v)
				if(v.x>-100&&v.y>-100&&v.x<RealLx+100&&v.y<RealLy+100){
					if(DangerMap[i]){
						str=DangerMap[i];
						ShowString(v.x,v.y,str.pchar(),&WhiteFont);
					};
					if(Targets[i]!=0xFFFF){
						str.Clear();
						str.print("TargetID = %d",Targets[i]);
						DWORD c=WhiteFont.GetColor();
						WhiteFont.SetColor(0xFF20FF20);
						ShowString(v.x,v.y-16,str.pchar(),&WhiteFont);
						WhiteFont.SetColor(c);						
					};
				}
			}
		}
		// Test
		if((GetKeyState(VK_MENU)&0x8000)&&Way.GetAmount()>0){
			DWORD col=0xFFFF2020;
			int fin=GetTopZone(vMapx,vMapy);
			if(fin!=0xFFFF){
				int L=0;
				FindNextZoneOnTheSafeWayEx((int)Way[0],fin,DangerMap,&L,100);
				int i=NWPTS-2;
				while(i>=0){					
					int x0,y0;
					int x1,y1;
					if(GetTopZoneCoor(FULLWAY[i],&x0,&y0)&&GetTopZoneCoor(FULLWAY[i+1],&x1,&y1)){
						DrawWorldLine(x0<<6,y0<<6,x1<<6,y1<<6,col);
						//GPS.DrawLine(V0.x,V0.y,V1.x,V1.y,0xFF2020BB);	
						//GetTotalHeight
					}		
					i--;
				}
				//
				Vector3D v = SkewPt(vMapx,vMapy,GetTotalHeight(vMapy,vMapy));
				WorldToScreenSpace(v);
				_str str;
				str=L;
				DWORD c=WhiteFont.GetColor();
				WhiteFont.SetColor(col);
				ShowString(v.x,v.y-16,str.pchar(),&WhiteFont);
				WhiteFont.SetColor(c);
			}
		}
	}
};
void mai_ACGroup::DrawOnMiniMap(int x,int y,int Lx,int Ly){
};

CEXPORT bool GetVit2Alex(){ 
	return EngSettings.EnableVit2Alex&&vGameMode!=gmCamp;
}