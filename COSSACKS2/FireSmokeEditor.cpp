#include "stdheader.h"
#include "ClassEditorsRegistration.h"

void ReplaceStringInMD(char* filename,char* command,char* replacewith);
void ShowBuildingPreview(int BuildingID,int xscr,int yscr);
extern bool g_bPerspCamera;

class BasicBuildingEditor:public BaseClass{
public:
	int  pUnitType;
	int  UnitType;
	int  dx;
	int  dy;
	NewMonster* NM;
	BasicBuildingEditor(){
		pUnitType=-1;
	}
	SAVE(BasicBuildingEditor);
		//REG_ENUM(_index,UnitType,ALL_UNITS);		
		REG_MEMBER(_UnitType,UnitType);
	ENDSAVE;
	virtual void OnLPress(int dx,int dy){
	}
	virtual void OnRPress(int dx,int dy){
	}
	virtual bool GetCmdToReplace(_str& command,_str& com_value){
		return false;
	}
	virtual void Process(){}
	virtual void Draw(int xs,int ys){}
	virtual void OnSelectNewUnit(){};
	void ChangeMD(){
		_str cmd;
		_str value;
		if(GetCmdToReplace(cmd,value)){
			char* n  = NM->MD_File;
			char fnm[128];
			sprintf(fnm,"%s.md",n);
			ReplaceStringInMD(fnm,cmd.pchar(),value.pchar());
		}
	}
	void Process(int Options){
		if(Options==1)g_bPerspCamera=false;		
		if(Options==2){			
			NM=NATIONS->Mon[UnitType]->newMons;
			if(pUnitType!=UnitType)OnSelectNewUnit();
			pUnitType=UnitType;
			Process();
			int x0=RealLx*2/5+dx;
			int y0=RealLy*2/3+dy;
			int xs=x0+NM->PicDx;
			int ys=y0+NM->PicDy;
			IRS->ClearDevice();
			ShowBuildingPreview(UnitType,x0,y0);		
			GPS.FlushBatches();
			Draw(xs,ys);
			GPS.FlushBatches();
			SetupCamera();
			int mdx=mouseX-xs;
			int mdy=mouseY-ys;
			if(Lpressed&&mouseX<RealLx-300){			
				OnLPress(mdx,mdy);
				ChangeMD();
				Lpressed=false;				
			}
			if(Rpressed){
				Rpressed=false;
				OnRPress(mdx,mdy);	
				ChangeMD();
			}		
			if( GetKeyState(VK_LEFT)  & 0x8000 )dx--;
			if( GetKeyState(VK_RIGHT) & 0x8000 )dx++;
			if( GetKeyState(VK_UP)    & 0x8000 )dy--;
			if( GetKeyState(VK_DOWN)  & 0x8000 )dy++;
			if( GetKeyState(VK_HOME)  & 0x8000 ){
				dy=0;
				dx=0;
			}			
		}
		if(Options==3||Options==4)g_bPerspCamera=true;        
	}	
};
class FireSmokeEditor:public BasicBuildingEditor{
public:
	int  FiresOrSmoke;

	short*  Nelm;
	short** elmX;
	short** elmY;	

	SAVE(FireSmokeEditor);
		REG_PARENT(BasicBuildingEditor);
		REG_ENUM(_index,FiresOrSmoke,FIRE_OR_SMOKE);
	ENDSAVE;	

	virtual void OnLPress(int dx,int dy){
		for(int i=0;i<*Nelm;i++)if(Norma((*elmX)[i]-dx,(*elmY)[i]-dy)<2)return;

		*elmX=(short*)realloc(*elmX,(*Nelm) *2+2);
		*elmY=(short*)realloc(*elmY,(*Nelm) *2+2);			
		(*elmX)[*Nelm]=dx;
		(*elmY)[*Nelm]=dy;
		(*Nelm)++;
	}
	virtual void OnRPress(int dx,int dy){
		for(int i=0;i<*Nelm;i++){
			if(Norma((*elmX)[i]-dx,(*elmY)[i]-dy)<6){
				if(i<*Nelm){
					memcpy( (*elmX)+i,(*elmX)+i+1,( (*Nelm) -i-1)*2 );
					memcpy( (*elmY)+i,(*elmY)+i+1,( (*Nelm) -i-1)*2 );
				}
				(*Nelm)--;
				break;
			}
		}
	}
	virtual bool GetCmdToReplace(_str& command,_str& com_value){
		command=FiresOrSmoke?"SMOKE":"FIRES";
		com_value.Add(*Nelm);
		com_value+=" ";
		for(int i=0;i<*Nelm;i++){
			com_value.print(" %d %d  ",(*elmX)[i],(*elmY)[i]);
		}		
		return true;
	}
	virtual void Process(){
		Nelm=&NM->NFires [FiresOrSmoke];
		elmX=&NM->FireX  [FiresOrSmoke];
		elmY=&NM->FireY  [FiresOrSmoke];
	}
	virtual void Draw(int xs,int ys){	
		if(GetKeyState(VK_CONTROL)&0x8000){
			for(int i=0;i<2;i++){
				for(int q=0;q<NM->NFires[i];q++){
					int x=NM->FireX[i][q];
					int y=NM->FireY[i][q];
					if(i==0){
						GPS.DrawFillRect(xs+x-1,ys+y-1,3,3,0xFFFF0000);
					}else{
						GPS.DrawLine(xs+x-3,ys+y,xs+x+4,ys+y,0xFFFFFFFF);
						GPS.DrawLine(xs+x,ys+y-3,xs+x,ys+y+4,0xFFFFFFFF);
					}
				}
			}
		}else{
			for(int i=0;i<*Nelm;i++){
				GPS.DrawFillRect(xs+(*elmX)[i]-1,ys+(*elmY)[i]-1,3,3,0xFFFF0000);
			}
		}
	}
};
FireSmokeEditor FS_editor;

void ShowBuildingPreview(int BuildingID,int xscr,int yscr);
void SetupCamera();

void ReplaceStringInMD(char* filename,char* command,char* replacewith){
	if(filename&&command&&replacewith)
	{
		DString mdf;
		mdf.ReadFromFile(filename);
		if(mdf.pchar())
		{
			char* com=NULL;
			DString newstr;
			newstr.Add(command);
			newstr.Add(" ");
			newstr.Add(replacewith);
			if(com=strstr(mdf.pchar(),command))
			{
				char* comend=NULL;
				if(comend=strstr(com,"\n"))
				{
					int len=comend-com;
					char* tmp=new char[len+1];
					strncat(tmp,com,len);
					tmp[len]=0;
					mdf.Replace(tmp,newstr.pchar());
					delete[] tmp;
				}
			}
			else
			{
				if(com=strstr(mdf.pchar(),"[END]"))
				{
					newstr.Add("\n[END]");
					mdf.Replace("[END]",newstr.pchar());
				}
			}
			mdf.WriteToFile(filename);
		}
	}
}

bool Process_BLD_Editor(ClassEditor* CE,BaseClass* BC,int Options){//Options=1-init, 2-process, 3-ok pressed, 4-cancel pressed 
	BasicBuildingEditor* B=dynamic_cast<BasicBuildingEditor*>(BC);
	if(B)B->Process(Options);	
	return false;
}

//----------------------- 3 points aligning ------------------------//

class Edit3Points:public BasicBuildingEditor{
public:
	int CurrentPoint;
	union{
		struct{
			int  Point1x;
			int  Point1y;
			int  Point1z;

			int  Point2x;
			int  Point2y;
			int  Point2z;

			int  Point3x;
			int  Point3y;
			int  Point3z;
		};
		int pts[9];
	};

	SAVE(Edit3Points);		
		REG_PARENT(BasicBuildingEditor);
		//REG_MEMBER(_int,CurrentPoint);
		/*
		REG_MEMBER(_int,Point1x);
		REG_MEMBER(_int,Point1y);
		REG_MEMBER(_int,Point1z);

		REG_MEMBER(_int,Point2x);
		REG_MEMBER(_int,Point2y);
		REG_MEMBER(_int,Point2z);

		REG_MEMBER(_int,Point3x);
		REG_MEMBER(_int,Point3y);
		REG_MEMBER(_int,Point3z);
		*/
	ENDSAVE;
	virtual void OnLPress(int dx,int dy){
		if(CurrentPoint>=0&&CurrentPoint<3){
            pts[CurrentPoint*3  ]=dx;
			pts[CurrentPoint*3+1]=dy;
		}
	}
	virtual void OnRPress(int dx,int dy){
		if(CurrentPoint>=0&&CurrentPoint<3){
			pts[CurrentPoint*3+2]=dy-pts[CurrentPoint*3+1];			
		}		
	}
	virtual bool GetCmdToReplace(_str& command,_str& com_value){
		if(NM->Building){
			command="ALIGN_WITH_3POINTS";
			com_value="";
			com_value.print(" %d %d %d   %d %d %d   %d %d %d" 
				,Point1x,Point1y,Point1z
				,Point2x,Point2y,Point2z
				,Point3x,Point3y,Point3z);
			NM->Use3pAlign = Point1x!=Point2x || Point2x!=Point3x || Point1x!=Point3x;
			return true;
		}return false;
	}
	virtual void Process(){
		extern short WheelDelta;
		if(WheelDelta&&mouseX<RealLx-300){
			CurrentPoint=(300+CurrentPoint+WheelDelta/120)%3;
			WheelDelta=0;
		}
		NM->AlignPt1x=Point1x;
		NM->AlignPt1y=Point1y;
		NM->AlignPt1z=Point1z;
		NM->AlignPt2x=Point2x;
		NM->AlignPt2y=Point2y;
		NM->AlignPt2z=Point2z;
		NM->AlignPt3x=Point3x;
		NM->AlignPt3y=Point3y;
		NM->AlignPt3z=Point3z;
	}
	virtual void Draw(int xs,int ys){	
		for(int i=0;i<3;i++)				{
			int x=pts[i*3  ]+xs;
			int y=pts[i*3+1]+ys;
			int z=pts[i*3+2];
			DWORD C = i==CurrentPoint?0xFFFFFF00:0xA0FFFFFF;
			GPS.DrawFillRect(x-1,y,3,z,C);
			GPS.DrawFillRect(x-2,y-2,5,5,C);			
			GPS.DrawFillRect(x-4,y+z-1,9,3,C);
		}
	}
	virtual void OnSelectNewUnit(){
        Point1x=NM->AlignPt1x;
		Point1y=NM->AlignPt1y;
		Point1z=NM->AlignPt1z;
		Point2x=NM->AlignPt2x;
		Point2y=NM->AlignPt2y;
		Point2z=NM->AlignPt2z;
		Point3x=NM->AlignPt3x;
		Point3y=NM->AlignPt3y;
		Point3z=NM->AlignPt3z;
	}
};

Edit3Points E3P;

void RegBLD_Editors(){
    return;
	AddStdEditor("Building Aligning editor",&E3P,""
		,RCE_RIGHT_POSITION|RCE_EXITONESCAPE|RCE_HIDEOKCANCEL|RCE_HIDEHEADER
		,Process_BLD_Editor);
	AddStdEditor("Fires & Smoke",&FS_editor,""
		,RCE_RIGHT_POSITION|RCE_EXITONESCAPE|RCE_HIDEOKCANCEL|RCE_HIDEHEADER
		,Process_BLD_Editor);
}