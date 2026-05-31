#include "stdheader.h"

// register all childrens
void RegZG_Children(){
	REG_CLASS(GroupExInfo);
	REG_CLASS(ZoneExInfo);
	REG_CLASS(StartPoint);	
	// vital
	REG_CLASS(c_CapturableExInfo);
	//VITYA & GREY
	REG_CLASS(CZM_ARS_LV);
	REG_CLASS(CZM_DRS_LV);
	REG_CLASS(CAI_TOTAL_WAR);
	REG_CLASS(CAMP_LV);
	REG_CLASS(STRAT_ZONE); //grey [03.03.2004]
	REG_CLASS(TOWER_ZONE); //grey [04.08.2004]
	//
	REG_CLASS(AiImportantPoint);
	//-----
}

//////////////////////////////////////////////////////////////////////////
ClassArray<ActiveGroup> AGroups;
ClassArray<ActiveZone> AZones;
//////////////////////////////////////////////////////////////////////////

void DrawPlaneLine(int x0,int y0,int x1,int y1,byte c);
void DrawPlaneLine2(int x0,int y0,int x1,int y1,DWORD c);

void Draw3DRound(int x,int y,int r){
	int N=((r*6)>>5)+5;
	double Rd=r;
	int xp=x+r;
	int yp=y;
	for(int i=1;i<=N;i++){
		double Angle=i*2*3.1415926535/N;
		int x1=x+int(Rd*cos(Angle));
		int y1=y+int(Rd*sin(Angle));
		DrawPlaneLine2(xp,yp,x1,y1,0xFFFFFF22);
		xp=x1;
		yp=y1;
	};
};
extern int RealLx;
extern int RealLy;
void ActiveZone::Draw(){
	if(LMode)return;
	
	Vector3D pos=SkewPt( x, y, GetHeight(x,y) );
	WorldToScreenSpace( pos );
	int xx = pos.x;
	int yy = pos.y; //(y>>1)-(mapy<<4)-GetHeight(x,y);

	if(xx<-R||yy<-R||xx>RealLx+R||yy>RealLy+R)return;
	int L=GetRLCStrWidth(Name.pchar(),&BigYellowFont);
	int L2=L>>1;
	int H=GetRLCHeight(BigYellowFont.RLC,'W');
	Draw3DRound(x,y,R);

	DrawLine(xx-1,yy,xx-1,yy-48,0xFFFFFF00);
	DrawLine(xx,yy,xx,yy-48,0xFFFFFF00);
	DrawLine(xx+1,yy,xx+1,yy-48,0xFFFFFF00);

	DrawLine(xx-L2-8,yy-48,xx+L2+8,yy-48,0xFFFFFF00);
	DrawLine(xx-L2-8,yy-48-H-10,xx+L2+8,yy-48-H-10,0xFFFFFF00);
	DrawLine(xx-L2-8,yy-48,xx-L2-8,yy-48-H-10,0xFFFFFF00);
	DrawLine(xx+L2+8,yy-48,xx+L2+8,yy-48-H-10,0xFFFFFF00);
	
	ShowString(xx-L2+1,yy-H-55+1,Name.pchar(),&BigBlackFont);
	ShowString(xx-L2-1,yy-H-55+1,Name.pchar(),&BigBlackFont);
	ShowString(xx-L2+1,yy-H-55-1,Name.pchar(),&BigBlackFont);
	ShowString(xx-L2-1,yy-H-55-1,Name.pchar(),&BigBlackFont);
	ShowString(xx-L2,yy-H-55,Name.pchar(),&BigYellowFont);
};
void DrawAGroups();
extern bool NoText;
void DrawAZones(){
	if(NoText)return;
	for(int i=0;i<AZones.GetAmount();i++)AZones[i]->Draw();
	DrawAGroups();
    GPS.FlushBatches();
};
char ZoneName[64]="Zone1";
char GroupName[64]="Group1";
int CurrentZone=-1;
int ZActionType=0;
int CreateNewZone(char* Name,int x,int y,int R){
	ActiveZone* AZ=new ActiveZone;
	AZones.Add(AZ);
	AZ->x=x;
	AZ->y=y;
	AZ->Name=Name;	
	AZ->R=R;
	AZ->Dir=0;	
	return AZones.GetAmount()-1;
};
bool DeleteActiveGroup();
void DeleteAZone(int id){
	if(id<AZones.GetAmount()){
		ActiveZone* AZ=AZones[id];
		if(AZ->GetInfo(_ZONE_EX_INFO_CAPTURABLE_,0)) return;
		AZones.Del(id,1);
	};
};
bool DelCurrentAZone(){
	if(!DeleteActiveGroup()){
		if(CurrentZone!=-1){
			DeleteAZone(CurrentZone);
			CurrentZone=-1;
			return true;
		};
		return false;
	}else return true;
};
extern bool KeyPressed;
extern int LastKey;
extern bool realLpressed;
bool EnterStr(char * str,char* Message);
void ProcessAGroups(int x,int y);
void CreateNewActiveGroup(char* gName);
void ErrorBox(char* str);
int ZoneCmd=0;
bool WasUnpress=0;
void UnPress();
void ClearMStack();
void ProcessPropertyEditor(BaseClass* BC);
void ControlAZones(int x,int y){
	if(!(GetKeyState(VK_SCROLL)/*||(GetKeyState(VK_CONTROL)&0x8000)*/)){
		ZoneCmd=0;
		return;
	};
	ProcessAGroups(x,y);
	if((KeyPressed&&LastKey==13&&(GetKeyState(VK_CONTROL)&0x8000))||ZoneCmd){
		if(NSL[MyNation]&&ZoneCmd!=1){
			if(EnterStr(GroupName,"New Group")){	
				bool Err=false;
				for(int i=0;i<AGroups.GetAmount();i++)if(!strcmp(AGroups[i]->Name.pchar(),GroupName)){
					ErrorBox("This name already exsists.");
					Err=true;
				};
				if(!Err)CreateNewActiveGroup(GroupName);
			};
		};
		if(ZoneCmd!=2&&!NSL[MyNation]){
			if(EnterStr(ZoneName,"New Zone")){	
				bool Err=false;
				for(int i=0;i<AZones.GetAmount();i++)if(!strcmp(AZones[i]->Name.pchar(),ZoneName)){
					ErrorBox("This name already exsists.");
					Err=true;
				};
				Lpressed=0;
				UnPress();
				ClearMStack();
				if(!Err){
					WasUnpress=1;
					//if(ZoneCmd){
					//	CurrentZone=CreateNewZone(ZoneName,mouseX,mouseY,0);
					//}else{
						CurrentZone=CreateNewZone(ZoneName,x,y,0);
					//};
				};
				ZActionType=0;
			};
		};
		KeyPressed=false;
	};
	ZoneCmd=0;
	if(!realLpressed)WasUnpress=0;
	if(WasUnpress)return;
	if(realLpressed&&!WasUnpress){
		if(CurrentZone!=-1){
			switch(ZActionType){
			case 0:
				ZActionType=1;
				Lpressed=false;
				break;
			case 1:
				{
					int x0=AZones[CurrentZone]->x;
					int y0=AZones[CurrentZone]->y;
					AZones[CurrentZone]->R=int(sqrt((x-x0)*(x-x0)+(y-y0)*(y-y0)));
					Lpressed=false;
				};
				break;
			case 2:
				AZones[CurrentZone]->x=x;
				AZones[CurrentZone]->y=y;
				Lpressed=false;
				break;
			};
		}else{
			//search for zone
			for(int i=0;i<AZones.GetAmount();i++){
				ActiveZone* AZ=AZones[i];
				int r=int(sqrt((x-AZ->x)*(x-AZ->x)+(y-AZ->y)*(y-AZ->y)));
				if(abs(r-AZ->R)<16){
					CurrentZone=i;
					ZActionType=1;
					Lpressed=false;
				}else if(r<16){
					CurrentZone=i;
					ZActionType=2;
					Lpressed=false;
				};
			};
		};
	}else{
		if(CurrentZone!=-1){
			switch(ZActionType){
			case 0:
				AZones[CurrentZone]->x=x;
				AZones[CurrentZone]->y=y;
				break;
			case 1:
			case 2:
				CurrentZone=-1;
				break;
			};
		};
	};
	if(Rpressed&&!ImNSL[MyNation]){
		for(int i=0;i<AZones.GetAmount();i++){
			ActiveZone* AZ=AZones[i];
			int r=int(sqrt((x-AZ->x)*(x-AZ->x)+(y-AZ->y)*(y-AZ->y)));
			if(abs(r)<20){
				ProcessPropertyEditor(AZones[i]);
				Rpressed=0;
				UnPress();
				return;
			}
		}
	}	
};
int CurrentAGroup=-1;
void ActiveGroup::Draw(byte c){
	if(LMode)return;
	MinX=10000000;
	MinY=10000000;
	MaxX=0;
	MaxY=0;

	AminX=10000000;
	AminY=10000000;
	AmaxX=0;
	AmaxY=0;
	for(int i=0;i<Units.GetAmount();i++){
		word MID=Units[i].ID;
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==Units[i].SN){
				int x=OB->RealX>>4;
				int y=(OB->RealY>>5)-OB->RZ;
				if(x<MinX)MinX=x;
				if(x>MaxX)MaxX=x;
				if(y<MinY)MinY=y;
				if(y>MaxY)MaxY=y;
				x=OB->RealX>>10;
				y=OB->RealY>>10;
				if(x<AminX)AminX=x;
				if(x>AmaxX)AmaxX=x;
				if(y<AminY)AminY=y;
				if(y>AmaxY)AmaxY=y;
			};
		};
	};
	
	if(MinX<=MaxX)
	{
		MinX-=40;
		MaxX+=40;
		MinY-=40;
		MaxY+=40;
		int dx=mapx<<5;
		int dy=mapy<<4;
		
		Vector3D lt( MinX, MinY*2, 50 );
		Vector3D lb( MinX, MaxY*2, 50 );
		Vector3D rt( MaxX, MinY*2, 50 );
		Vector3D rb( MaxX, MaxY*2, 50 );

		GPS.DrawLine( lt, rt, 0xFFFFFF22 );
		GPS.DrawLine( rt, rb, 0xFFFFFF22 );
		GPS.DrawLine( rb, lb, 0xFFFFFF22 );
		GPS.DrawLine( lb, lt, 0xFFFFFF22 );

		int L=GetRLCStrWidth(Name.pchar(),&BigYellowFont);

		Vector3D c( (MinX + MaxX)/2, MinY*2 - 40, 50 );
		WorldToScreenSpace( c );
		int xc = c.x;
		int yc = c.y;

		if(xc>-64&&xc<RealLx+64&&yc>-64&&yc<RealLy+64)
		{
			ShowString(xc-(L>>1)+1,yc+1,Name.pchar(),&BigBlackFont);
			ShowString(xc-(L>>1)-1,yc-1,Name.pchar(),&BigBlackFont);
			ShowString(xc-(L>>1)-1,yc+1,Name.pchar(),&BigBlackFont);
			ShowString(xc-(L>>1)-1,yc-1,Name.pchar(),&BigBlackFont);
			ShowString(xc-(L>>1),yc,Name.pchar(),&BigYellowFont);
		};
		Visible=true;
	}else{
		Visible=false;
	}
};
void DrawAGroups(){
	for(int i=0;i<AGroups.GetAmount();i++){
		if(i==CurrentAGroup)AGroups[i]->Draw(clrWhite);
		else AGroups[i]->Draw(clrYello);
	};
	GPS.FlushLines();
};
void CreateNewActiveGroup(char* gName){	
	ActiveGroup* AG=new ActiveGroup;
    AGroups.Add(AG);
	int NU=NSL[MyNation];
	word* ids=Selm[MyNation];	
	AG->Name=gName;	
	for(int i=0;i<NU;i++){
		word MID=ids[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				OneUS U;
				U.ID=MID;
				U.SN=OB->Serial;
                AG->Units.Add(U);
			};
		};
	};
	AG->Visible=false;
	AG->Draw(0);
};
int CreateFreeActiveGroup(char* gName){
	ActiveGroup* AG=new ActiveGroup;
	AGroups.Add(AG);	
	AG->Name=znew(char,strlen(gName)+1);
	AG->Name=gName;
	return AGroups.GetAmount()-1;
};
void ProcessAGroups(int x,int y){
	CurrentAGroup=-1;
	int xs=x;
	int ys=(y>>1)-GetHeight(x,y);
	for(int i=0;i<AGroups.GetAmount();i++){
		ActiveGroup* AG=AGroups[i];
		if(AG->Visible&&xs>=AG->MinX&&xs<=AG->MaxX&&ys>=AG->MinY&&ys<=AG->MaxY){
			CurrentAGroup=i;
			if(Rpressed&&!ImNSL[MyNation]){
				ProcessPropertyEditor(AGroups[i]);
				Rpressed=0;
				UnPress();				
			}
		};
	};
	RefreshAGroups();
};
bool DeleteActiveGroup(){
	if(CurrentAGroup==-1)return false;
	AGroups.Del(CurrentAGroup,1);
	CurrentAGroup=-1;
	return true;
};
void DrawMinAZones(int x,int y){
	for(int i=0;i<AZones.GetAmount();i++){
		int dx=x+(AZones[i]->x>>6);
		int dy=y+(AZones[i]->y>>6);
		int R=AZones[i]->R>>6;
		if(R<1)R=1;
		GPS.DrawLine(dx-R,dy,dx+R,dy,0xFFFFFFFF);
		GPS.DrawLine(dx,dy-R,dx,dy+R,0xFFFFFFFF);
	};
	for(i=0;i<AGroups.GetAmount();i++){
		ActiveGroup* AG=AGroups[i];
		if(AG->Visible){
			GPS.DrawFillRect(x+AG->AminX-1,y+AG->AminY-1,AG->AmaxX-AG->AminX+4,AG->AmaxY-AG->AminY+4,0x80FFFF00);
		};
	};
};
CEXPORT DWORD GetNatColor(int i);
CEXPORT void Add_VI_Zone(int x, int y, DWORD color);
void DrawMinAZonesVis(int x,int y){
	for(int i=0;i<AZones.GetAmount();i++){
		DWORD col=0;
		if(AZones[i]->Dir==117){
			col=0xFFFFFFFF;
		}else{
			c_CapturableExInfo* zi=(c_CapturableExInfo*)AZones[i]->GetInfo(_ZONE_EX_INFO_CAPTURABLE_,0);
			if(zi){
				//col=(GetKeyState(VK_CONTROL)&0x8000)?0xFFFFFFFF:GetNatColor(zi->Owner);				
				bool blink=(zi->Owner>7);
				byte owner=zi->Owner&7;
				DWORD c=0xDF000000+(MulDWORD(GetNatColor(owner),300)&0xFFFFFF);
				if(blink){
					static int t=GetTickCount();
					float T=1000.0;					
					float mul=155+100*sin(2.0*3.1415*(float(GetTickCount()-t)/T));
					//c=MulDWORD(GetNatColor(zi->Owner-8),mul);
					//c = (mul<<24) + (MulDWORD(GetNatColor(zi->Owner-8),300)&0xFFFFFF);
					c = MixDWORD(c,0xDFFFFFFF,255-mul,mul);
				}
				Add_VI_Zone(AZones[i]->x, AZones[i]->y, c);
			}
		}
		if(col){
			int dx=x+(AZones[i]->x>>6);
			int dy=y+(AZones[i]->y>>6);
			int R=AZones[i]->R>>6;
			if(R<1)R=1;
			GPS.DrawLine(dx-R,dy,dx+R,dy,col);
			GPS.DrawLine(dx,dy-R,dx,dy+R,col);
		}
	};
};
void ClearAllZones(){
	AZones.Clear();
	AGroups.Clear();	
	CurrentZone=-1;
	CurrentAGroup=-1;
};
void RefreshGroup(ActiveGroup* AG){
	for(int i=0;i<AG->Units.GetAmount();i++){
		bool DelThis=false;
		word MID=AG->Units[i].ID;
		if(MID==0xFFFF)DelThis=true;
		else{
			OneObject* OB=Group[MID];
			if(!(OB&&(!OB->Sdoxlo)&&OB->Serial==AG->Units[i].SN))DelThis=true;
		}
		if(DelThis){
			AG->Units.Del(i,1);
			i--;
		}
	}
}
void RefreshAGroups(){
	for(int i=0;i<AGroups.GetAmount();i++){
		ActiveGroup* AG=AGroups[i];
		if(!AG->Visible){
			CurrentAGroup=i;
			DeleteActiveGroup();
			i--;
		}else{
			RefreshGroup(AG);
		};
	};
};
DIALOGS_API GroupExInfo* GetExGroupInfo(char* groupID,DWORD ID,int Index){
	for(int i=0;i<AGroups.GetAmount();i++)if(!strcmp(AGroups[i]->Name.pchar(),groupID)){
		return AGroups[i]->GetInfo(ID,Index);
	}
	return NULL;
}
DIALOGS_API ZoneExInfo* GetExZoneInfo(char* zoneID,DWORD ID,int Index){
	for(int i=0;i<AZones.GetAmount();i++)if(!strcmp(AZones[i]->Name.pchar(),zoneID)){
		return AZones[i]->GetInfo(ID,Index);
	}
	return NULL;
}
DIALOGS_API void* GetGroupHandle(char* groupID){
	for(int i=0;i<AGroups.GetAmount();i++)if(!strcmp(AGroups[i]->Name.pchar(),groupID))return AGroups[i];
	return NULL;
}
DIALOGS_API void* GetZoneHandle(char* zoneID){
	for(int i=0;i<AZones.GetAmount();i++)if(!strcmp(AZones[i]->Name.pchar(),zoneID))return AZones[i];
	return NULL;
}
DIALOGS_API GroupExInfo* GetExGroupInfo(void* groupID,DWORD ID,int Index){
	if(groupID){
		return ((ActiveGroup*)groupID)->GetInfo(ID,Index);
	}else return NULL;
}
DIALOGS_API ZoneExInfo* GetExZoneInfo(void* zoneID,DWORD ID,int Index){
	if(zoneID){
		return ((ActiveZone*)zoneID)->GetInfo(ID,Index);
	}else return NULL;
}
DIALOGS_API void* GetGroupHandle(int Index){
	if(Index<AGroups.GetAmount()){
		return AGroups[Index];
	}else return NULL;
}
DIALOGS_API void* GetZoneHandle(int Index){
	if(Index<AZones.GetAmount()){
		return AZones[Index];
	}else return NULL;
}

//////////////////////////////////////////////////////////////////////////

ActiveZone* _AZone::GetAZone(){
	if(AZone==NULL&&!Init){
		Init=true;
		int n=AZones.GetAmount();
		for(int i=0;i<n;i++){
			if(!strcmp(AZones[i]->Name.pchar(),pchar())){
				AZone=AZones[i];
				//break;
			}
		}
	}
	return AZone;
};	
void _AZone::GetVariants(ClonesArray<_str> &List){
	int n=AZones.GetAmount();
	for(int i=0;i<n;i++){
		_str* s=new _str;
		s->Add(AZones[i]->Name.pchar());
		List.Add(s);
	}		
};
//
class _AZone_editor:public OneElementEditor{
	TextButton* TB;
public:
	virtual const char* GetClassID(){ return "_AZone"; }
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
							BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam);		
	virtual OneElementEditor* _new(){
		return new _AZone_editor;
	}
	virtual bool Process(){
		return true;
	}
};
bool _AZone_find(SimpleDialog* SD){
	_AZone* V=dynamic_cast<_AZone*>((_variant*)SD->UserParam); //_AZone
	if(V){
		int n=AZones.GetAmount();
		for(int i=0;i<n;i++)if(AZones[i]->Name==V->pchar()){
			int x=AZones[i]->x;
			int y=AZones[i]->y;
			void SetScreenCenterToXY(int x, int y);
			SetScreenCenterToXY(x,y);
			/*
			x=(x-(RealLx>>1))>>5;
			y=(y-RealLy)>>5;
			if(x<1)x=1;
			if(y<1)y=1;
			if(x+smaplx>msx-1)x=msx-smaplx-1;
			if(y+smaply>msy-1)y=msy-smaply-1;
			mapx=x;
			mapy=y;
			*/
			break;
		}		

	}
	return true;
}
int _AZone_editor::CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
								BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
	//static int CBB_GPFILE=GPS.PreLoadGPImage("Interf3\\elements\\Combo");
	//Box=Base->addGP_ComboBoxDLX(NULL,x+27,y,x1-x+1-27,CBB_GPFILE,0,9,0,&CED_AFont,&CED_Font,NULL);	
	//VScrollBar* VS;
	//Box->AssignScroll((DialogsSystem*)Base,&VS,3,0,8);
	//Box->Enabled=false;	
	_AZone* V=dynamic_cast<_AZone*>((_variant*)DataPtr);
	//_variant* V=dynamic_cast<_variant*>(Class);
	if(V){
		int dw=CParam->LeftColumnWidth-CParam->MinLeftWidth;
		x-=dw;
		x1+=dw;
		CParam->LeftColumnWidth-=dw;
		CParam->RightColumnWidth+=dw;		
		TB=Base->addTextButton(NULL,x+27,y,V->pchar(),&CED_AFont,&CED_Font,&CED_Font,0);
		//Box->AddLine(V->pchar());
		static int bidx=GPS.PreLoadGPImage("Interf3\\btnz");
		GP_Button* BD=Base->addGP_Button(NULL,x,y,bidx,17,16);
		BD->OnUserClick=&_AZone_find;
		BD->UserParam=(int)DataPtr;
        //ClonesArray<_str> List;
		//V->GetVariants(List);
		//Box->AddLine("--empty--");
		//for(int i=0;i<List.GetAmount();i++){
		//	Box->AddLine(List[i]->pchar());
		//}
	}
	return y1;
}
extern DynArray<OneElementEditor*> type_list;
void RegizterActiveZoneEditors(){
	type_list.Add(new _AZone_editor);
}
