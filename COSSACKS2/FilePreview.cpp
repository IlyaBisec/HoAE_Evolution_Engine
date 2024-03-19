#include "stdheader.h"
#include "FilePreview.h"
#include "kContext.h"
#include "ai_scripts.h"
#include "uiModelView.h"

class G17_Preview:public FilePreview{	
	int gprev;
	int ptime;
	float scale;
	int dx,dy;
public:
	virtual ~G17_Preview(){};
	G17_Preview(){
		gprev=-1;
		ptime=0;
	};
	virtual int EstimateSize(int TotalYSize){
		return TotalYSize/2;		
	}
	virtual void DrawPreview(int x,int y,int x1,int y1,const char* File){
		char NAME[256];
		strcpy(NAME,File);
		char* s=strstr(NAME,".G17");
		if(s)s[0]=0;
		s=strstr(NAME,".G16");
		if(s)s[0]=0;
		s=strstr(NAME,".GP2");
		if(s)s[0]=0;
		s=strstr(NAME,".G2D");
		if(s)s[0]=0;
		int G=GPS.PreLoadGPImage(NAME);		
		if(G>0&&G<65535){
			if(G!=gprev){
				gprev=G;
				///if(GPS.LoadGP(G)){
					int N=GPS.GPNFrames(G);
					int minx=10000;
					int miny=10000;
					int maxx=-10000;
					int maxy=-10000;
					for(int i=0;i<N&&i<2500;i++){			
						Rct rct;					
						GPS.GetGPBoundFrame(G,i,rct);
						if(rct.x<minx)minx=rct.x;
						if(rct.y<miny)miny=rct.y;
						if(rct.x+rct.w>maxx)maxx=rct.x+rct.w;
						if(rct.y+rct.h>maxy)maxy=rct.y+rct.h;
					}
					scale=1.0;
					if(maxx>minx&&maxy>miny){					
						scale=min(float(x1-x)/float(maxx-minx),float(y1-y)/float(maxy-miny));
						if(scale>1.0)scale=1.0;
					}
					dx=int((x1-x-float(maxx-minx)*scale)/2-float(minx)*scale);
					dy=int((y1-y-float(maxy-miny)*scale)/2-float(miny)*scale);
					ptime=GetTickCount();
				//}
			}
			int N=GPS.GPNFrames(G);
			if(N>0){
				GPS.FlushBatches();
				int fr=((GetTickCount()-ptime)/500)%N;
				GPS.SetScale(scale);
				GPS.EnableZBuffer(0);
				GPS.ShowGP(x+dx,y+dy,G,fr,0);
				GPS.SetScale(1.0f);
			}
		}
	}
	virtual bool CheckAbilityToPreview(const char* Ext){
		if(!stricmp(Ext,".g17"))return true;
		if(!stricmp(Ext,".g16"))return true;
		if(!stricmp(Ext,".g2d"))return true;
		if(!stricmp(Ext,".gp2"))return true;
		else return false;
	}
	virtual FilePreview* new_element(){
		return new G17_Preview;
	}
};

//void DrawThumbnail( DWORD mdlID, const Rct& vp, 
//                   DWORD animID, float animTime, 
//                   float rotAngle,DWORD bgColor,float scale, float shift);

class C2M_Preview:public FilePreview{	
	int gprev;
	int ptime;
	float scale;
	int dx,dy;
    ///ModelView view;
public:
	virtual ~C2M_Preview(){};
	C2M_Preview(){
		gprev=-1;
		ptime=0;
	};
	virtual int EstimateSize(int TotalYSize){
		return TotalYSize/2;		
	}
	virtual void DrawPreview(int x,int y,int x1,int y1,const char* File){
        DrawThumbnail( IMM->GetModelID(File), Rct(x,y,x1-x,y1-y) );
	}
	virtual bool CheckAbilityToPreview(const char* Ext){
		if(!stricmp(Ext,".c2m"))return true;
		else return false;
	}
	virtual FilePreview* new_element(){
		return new C2M_Preview;
	}
};

class Dialog_Preview:public FilePreview{
	int px,py,px1,py1;
	char pFile[256];
	DialogsSystem DSS;
	int Y0;
	int DY;
	int CY;
	int dir;
public:
	virtual ~Dialog_Preview(){};
	Dialog_Preview(){
		px=py=px1=py1=-1;
		pFile[0]=0;DY=0;
	}
	virtual int EstimateSize(int TotalYSize){
		return TotalYSize/2;		
	}
	virtual void DrawPreview(int x,int y,int x1,int y1,const char* File){		
		if(x!=px||y!=py||x1!=px1||y1!=py1||strcmp(pFile,File)){
			strcpy(pFile,File);
			px=x;
			py=y;
			px1=x1;
			py1=y1;
            DSS.CloseDialogs();
			DSS.x=x;
			DSS.y=y+10;
			SimpleDialog* SD=NULL;
			xmlQuote xml;
			if(xml.ReadFromFile((char*)File)){
				OneClassStorage* OCS=CGARB.GetClass(xml.GetQuoteName());
				if(OCS){
					SD=(SimpleDialog*)OCS->OneMemb;
					if(SD){
						SD=(SimpleDialog*)SD->new_element();
						ErrorPager EP(0);
						SD->Load(xml,SD,&EP);
						SD->SetEditMode(true);
					}
				}
			}
			if(SD){
				DSS.DSS.Add(SD);
				int MinX=100000;
				int MaxX=-100000;
				int MinY=100000;
				int MaxY=-100000;
				for(int i=0;i<DSS.DSS.GetAmount();i++){
					if(DSS.DSS[i]->x<MinX)MinX=DSS.DSS[i]->x;
					if(DSS.DSS[i]->x1>MaxX)MaxX=DSS.DSS[i]->x1;
					if(DSS.DSS[i]->y<MinY)MinY=DSS.DSS[i]->y;
					if(DSS.DSS[i]->y1>MaxY)MaxY=DSS.DSS[i]->y1;
				}
				DSS.x=x+(x1-x-MaxX+MinX)/2;
				DSS.y=y+(y1-y-MaxY+MinY)/2;
				if(DSS.x+MinX<x)DSS.x=x-MinX;
				if(DSS.y+MinY<y)DSS.y=y-MinY;
				Y0=DSS.y;
				DY=MaxY-MinY-y1+y;
				if(DY<0)DY=0;
				CY=0;
				dir=1;
			}
		}
		if(DY){
			CY+=dir;
			if(CY>DY)dir=-1;
			if(CY<0)dir=1;
			DSS.y=Y0+CY;
		}
		DSS.ProcessDialogs();

	}
	virtual bool CheckAbilityToPreview(const char* Ext){
		if(strstr(Ext,".Dialogs.xml"))return true;
		else return false;
	}
	virtual FilePreview* new_element(){
		return new Dialog_Preview;
	}
};
class dss_Preview:public FilePreview{
	int px,py,px1,py1;
	char pFile[256];
	DialogsSystem DSS;
public:
	virtual ~dss_Preview(){};
	dss_Preview(){
		px=py=px1=py1=-1;
		pFile[0]=0;
	}
	virtual int EstimateSize(int TotalYSize){
		return TotalYSize/2;		
	}
	virtual void DrawPreview(int x,int y,int x1,int y1,const char* File){		
		if(x!=px||y!=py||x1!=px1||y1!=py1||strcmp(pFile,File)){
			strcpy(pFile,File);
			px=x;
			py=y;
			px1=x1;
			py1=y1;
			DSS.CloseDialogs();
			DSS.x=x;
			DSS.y=y+10;
			xmlQuote xml;
			if(xml.ReadFromFile((char*)File)){
				ErrorPager EP(0);
				DSS.DSS.Load(xml,&DSS.DSS,&EP);				
			}
			int MinX=100000;
			int MaxX=-100000;
			int MinY=100000;
			int MaxY=-100000;
			for(int i=0;i<DSS.DSS.GetAmount();i++){
				if(DSS.DSS[i]->x<MinX)MinX=DSS.DSS[i]->x;
				if(DSS.DSS[i]->x1>MaxX)MaxX=DSS.DSS[i]->x1;
				if(DSS.DSS[i]->y<MinY)MinY=DSS.DSS[i]->y;
				if(DSS.DSS[i]->y1>MaxY)MaxY=DSS.DSS[i]->y1;
			}
			DSS.x=x+(x1-x-MaxX+MinX)/2;
			DSS.y=y+(y1-y-MaxY+MinY)/2;
			if(DSS.x+MinX<x)DSS.x=x-MinX;
			if(DSS.y+MinY<y)DSS.y=y-MinX;
		}
		DSS.ProcessDialogs();
	}
	virtual bool CheckAbilityToPreview(const char* Ext){
		if(strstr(Ext,".dss.xml"))return true;
		else return false;
	}
	virtual FilePreview* new_element(){
		return new dss_Preview;
	}
};
class DialogsSystem_Preview:public FilePreview{
	int px,py,px1,py1;
	char pFile[256];
	DialogsSystem DSS;
public:
	virtual ~DialogsSystem_Preview(){};
	DialogsSystem_Preview(){
		px=py=px1=py1=-1;
		pFile[0]=0;
	}
	virtual int EstimateSize(int TotalYSize){
		return TotalYSize/2;		
	}
	virtual void DrawPreview(int x,int y,int x1,int y1,const char* File){		
		if(x!=px||y!=py||x1!=px1||y1!=py1||strcmp(pFile,File)){
			px=x;
			py=y;
			px1=x1;
			py1=y1;
			strcpy(pFile,File);
			DSS.CloseDialogs();
			DSS.x=x;
			DSS.y=y+10;
			xmlQuote xml;
			if(xml.ReadFromFile((char*)File)){
				ErrorPager EP(0);
				DSS.Load(xml,&DSS,&EP);
				//DSS.SetEditMode(true);
			}
			int MinX=100000;
			int MaxX=-100000;
			int MinY=100000;
			int MaxY=-100000;
			for(int i=0;i<DSS.DSS.GetAmount();i++){
				if(DSS.DSS[i]->x<MinX)MinX=DSS.DSS[i]->x;
				if(DSS.DSS[i]->x1>MaxX)MaxX=DSS.DSS[i]->x1;
				if(DSS.DSS[i]->y<MinY)MinY=DSS.DSS[i]->y;
				if(DSS.DSS[i]->y1>MaxY)MaxY=DSS.DSS[i]->y1;
			}
			DSS.x=x+(x1-x-MaxX+MinX)/2-MinX;
			DSS.y=y+(y1-y-MaxY+MinY)/2-MinY;
			if(DSS.x+MinX<x)DSS.x=x-MinX;
			if(DSS.y+MinY<y)DSS.y=y-MinX;
		}
		DSS.ProcessDialogs();
	}
	virtual bool CheckAbilityToPreview(const char* Ext){
		if(strstr(Ext,".DialogsSystem.xml"))return true;
		else return false;
	}
	virtual FilePreview* new_element(){
		return new DialogsSystem_Preview;
	}
};
class UnitDesc_Preview:public FilePreview{
	int px,py,px1,py1;
	const char* pFile;
	UnitDesc UD;
public:
	virtual ~UnitDesc_Preview(){};
	UnitDesc_Preview(){
		px=py=px1=py1=-1;
		pFile="";
	}
	virtual int EstimateSize(int TotalYSize){
		return TotalYSize/2;		
	}
	virtual void DrawPreview(int x,int y,int x1,int y1,const char* File){		
		if(x!=px||y!=py||x1!=px1||y1!=py1||strcmp(pFile,File)){
			pFile=File;
			UD.DS.CloseDialogs();
			UD.DS.x=x;
			UD.DS.y=y+10;
			xmlQuote xml;
			if(xml.ReadFromFile((char*)File)){
				ErrorPager EP(0);
				UD.Load(xml,&UD,&EP);
			}
			int MinX=100000;
			int MaxX=-100000;
			int MinY=100000;
			int MaxY=-100000;
			for(int i=0;i<UD.DS.DSS.GetAmount();i++){
				if(UD.DS.DSS[i]->x<MinX)MinX=UD.DS.DSS[i]->x;
				if(UD.DS.DSS[i]->x1>MaxX)MaxX=UD.DS.DSS[i]->x1;
				if(UD.DS.DSS[i]->y<MinY)MinY=UD.DS.DSS[i]->y;
				if(UD.DS.DSS[i]->y1>MaxY)MaxY=UD.DS.DSS[i]->y1;
			}
			UD.DS.x=x;//+(x1-x/*-MaxX+MinX*/)/2;
			UD.DS.y=y;//+(y1-y/*-MaxY+MinY*/)/2;
		}
		UD.DS.ProcessDialogs();
	}
	virtual bool CheckAbilityToPreview(const char* Ext){
		if(Ext[strlen(Ext)-1]=='.')return true;
		else return false;
	}
	virtual FilePreview* new_element(){
		return new UnitDesc_Preview;
	}
};
ClassArray<FilePreview> PreviewGarbage;
void RegisterPreviewers(){
	PreviewGarbage.Add(new G17_Preview);
	PreviewGarbage.Add(new Dialog_Preview);
	PreviewGarbage.Add(new dss_Preview);
	PreviewGarbage.Add(new DialogsSystem_Preview);
	PreviewGarbage.Add(new UnitDesc_Preview);
	PreviewGarbage.Add(new C2M_Preview);
}
FilePreview* GetPreviewer(const char* Ext){	
	for(int i=0;i<PreviewGarbage.GetAmount();i++)if(PreviewGarbage[i]->CheckAbilityToPreview(Ext)){
		return PreviewGarbage[i]->new_element();
	}
	return NULL;
}
