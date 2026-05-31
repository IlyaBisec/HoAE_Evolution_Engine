#include "stdheader.h"

int ncalls=0;
void __log(int line){
	ncalls++;
	FILE* F=fopen("plog.log","w");
	if(F){
		fprintf(F,"%d (%d)",line,ncalls);
		fclose(F);
	}
}
#define _LOG
//__log(__LINE__);

bool ReadAttribute(char* str,char* ID,char* Dest){
	char* sa=strstr(str,ID);
	if(sa){
		sa+=2+strlen(ID);
		int i=0;
		while(sa[i]!='"' && sa[i]!=0){
			Dest[i]=sa[i];
			Dest[i+1]=0;
			i++;
		}
		return true;
	}
	return false;
}
class ProfView;
class ProfList:public ClassArray<ProfView>{
public:
	int GetExpansionRules(){
		return 0;
	}	
};
class ProfView:public BaseClass{
public:	
	bool Native;
	_str FuncName;
	int  NSamples;
	int  NOwnSamples;
	int BaseAddr;
	ProfList Node;
	ProfView* PrevLevel;
	static int MaxSamples;
	int Level;
	SAVE(ProfView);
	REG_AUTO(Node);
	ENDSAVE;
	ProfView(){
		PrevLevel=NULL;
		Level=0;
		NSamples=0;
		NOwnSamples=0;
		Native=1;
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra){

	}
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
		char* s=(char*)xml.GetQuoteName();
		char fn[512]="";
		if(ReadAttribute(s,"Name",fn)){
            FuncName=fn;
		}
		if(ReadAttribute(s,"sWorkTime",fn)){
			FuncName="Info";
			Native=0;
			ProfView* pv=new ProfView;
			pv->Native=0;
			pv->FuncName.print("WorkTime=%s",fn);
			Node.Add(pv);
		}
		if(ReadAttribute(s,"AllSamplingsCount",fn)){
			ProfView* pv=new ProfView;
			pv->Native=0;
			pv->FuncName.print("AllSamplingsCount=%s",fn);
			Node.Add(pv);
			MaxSamples=atoi(fn);
		}
		if(ReadAttribute(s,"msOneSamplingTime",fn)){
			ProfView* pv=new ProfView;
			pv->Native=0;
			pv->FuncName.print("msOneSamplingTime=%s",fn);
			Node.Add(pv);
		}
		if(ReadAttribute(s,"CpuUsage",fn)){
			ProfView* pv=new ProfView;
			pv->Native=0;
			pv->FuncName.print("CpuUsage=%s",fn);
			Node.Add(pv);
		}
		if(strstr(s,"UnknownSamples")){
			if(ReadAttribute(s," Samples",fn)){
				Native=0;
				FuncName="UnknownSamples";
				ProfView* pv=new ProfView;
				pv->Native=0;
				pv->FuncName.print("UnknownSamples=%s",fn);
                Node.Add(pv);
			}
		}
		if(ReadAttribute(s,"Samples",fn)){
			NSamples=atoi(fn);
		}
		if(ReadAttribute(s,"OwnSamples",fn)){
			NOwnSamples=atoi(fn);
		}
		if(ReadAttribute(s,"BaseAddr",fn)){
			sscanf(fn,"%x",&BaseAddr);
		}
		//if(Level&&NSamples>MaxSamples){
		//	if(FuncName.pchar()&&!strcmp(FuncName.pchar(),"WinMain"))MaxSamples=NSamples;
		//}
		for(int i=0;i<xml.GetNSubQuotes();i++){
			ProfView* pv=new ProfView;
			Node.Add(pv);
			pv->Level=Level+1;			
			pv->Load(*xml.GetSubQuote(i),&pv,Error,NULL);
			pv->PrevLevel=this;			
		}
		bool ch;
		do{
			ch=0;
			for(int i=1;i<Node.GetAmount();i++){
				if(Node[i-1]->NSamples<Node[i]->NSamples){
					swap(Node[i-1],Node[i]);
					ch=1;
				}
			}
		}while(ch);
		return true;
	}
	virtual const char* GetElementView(int Index,const char*){
		return FuncName.pchar()?FuncName.pchar():"Root";
	}
	bool ForceSimplification(){
		return true;
	}
	//int GetExpansionRules(){
	//	return 0;
	//}
};
int ProfView::MaxSamples=0;
class ProfView_editor:public OneElementEditor{
public:	
	void CreateBar(ParentFrame* Base,int x,int y,int Lx,int Ly,char* Header,float stage,bool HDR,int Level){
		Canvas* CN=Base->AddCanvas(x,y,Lx,Ly);
		if(!HDR){			
			if(Level&1){
				CN->AddBar(1,1,Lx-3,Ly-3,0xFF202020);
				CN->AddRect(0,0,Lx-1,Ly-1,0xFFAFAFAF);
			}else{
				CN->AddBar(1,1,Lx-3,Ly-3,0xFF505050);
				CN->AddRect(0,0,Lx-1,Ly-1,0xFFFFFFF);
			}
			DWORD C=0xFF800000;//MixDWORD(0xFF008000,0xFF800000,int((100-stage)*256/100),int(stage*256/100));
			CN->AddBar(1,1,(Lx-2)*stage/100,Ly-2,C);
			char cc[32];
			sprintf(cc,"%.1f%%",stage);
			CN->AddCText(Lx/2,Ly/2-9,cc,&YellowFont);
		}else CN->AddCText(Lx/2,Ly/2-12,Header,&YellowFont);
	}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
		int Ly=y1-y-2;
		int Lx=60;
		int dr=(Lx+4)*3-CParam->RightColumnWidth;
		x-=dr;
		ProfView PT;
		int dof=int(&PT.Node)-int(&PT);
		ProfView* PV=(ProfView*)(int(DataPtr)-dof);		
		if(!PV->Native){
			CParam->LeftColumnWidth+=CParam->RightColumnWidth;
			CParam->RightColumnWidth=0;
			return y1;
		}
		bool HDR=PV->FuncName.pchar()==NULL;
		CreateBar(Base,x+2,y+1,Lx,Ly,"Smp",PV->PrevLevel&&PV->PrevLevel->NSamples?float(PV->NSamples)*100.0f/PV->PrevLevel->NSamples:0,HDR,PV->Level);
		x+=Lx+4;
		CreateBar(Base,x+2,y+1,Lx,Ly,"Smp/All",PV->MaxSamples?float(PV->NSamples)*100.0f/PV->MaxSamples:0,HDR,PV->Level);
		x+=Lx+4;
		CreateBar(Base,x+2,y+1,Lx,Ly,"Own/Smp",PV->NSamples?float(PV->NOwnSamples)*100.0f/PV->NSamples:0,HDR,PV->Level);		
		CParam->RightColumnWidth+=dr;
		CParam->LeftColumnWidth-=dr;
		return y1;
	}
	virtual bool Process(){
		return true;
	}
	virtual const char* GetClassID(){return "ClassArray.ProfView";}
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new ProfView_editor;
	}
};
//===================================================================================================================//
void AddProf(ProfView* Who, ProfView* To)
{
	if(Who&&To)
	{
		int n=To->Node.GetAmount();
		bool find=false;
		for(int i=0;i<n;i++)
		{
			if(To->Node[i]->BaseAddr==Who->BaseAddr)
			{
				To->Node[i]->NSamples+=Who->NOwnSamples;
				//To->Node[i]->NSamples+=Who->BaseAddr;
				//To->Node[i]->NOwnSamples+=Who->NOwnSamples;
				AddProf(Who->PrevLevel,To->Node[i]);
				find=true;
			}
		}
		if(!find)
		{
			ProfView* NP = new ProfView();
			NP->FuncName.Add(Who->FuncName);
			NP->BaseAddr=Who->BaseAddr;
			//NP->NSamples=Who->NSamples;
			//NP->NOwnSamples=Who->NOwnSamples;
			NP->NSamples=Who->NOwnSamples;
			NP->PrevLevel=To;
			To->Node.Add(NP);
			AddProf(Who->PrevLevel,NP);
		}
	}
}
void FindOwnSamples(ProfView* Samp, ProfView* To)
{
	if(Samp&&To)
	{
		if(Samp->NOwnSamples>0)
		{
			AddProf(Samp,To);
		}
		int n=Samp->Node.GetAmount();
		for(int i=0;i<n;i++)
		{
			FindOwnSamples(Samp->Node[i],To);
		}
	}
}
int Compare(const void* v1, const void* v2)
{
	int rez=0;
	ProfView** p1=(ProfView**)v1;
	ProfView** p2=(ProfView**)v2;
	if((*p1)&&(*p2))
	{
		rez=(*p2)->NSamples-(*p1)->NSamples;
	}
	return rez;
}
void ReversProfView(ProfView* From, ProfView* Result)
{
	FindOwnSamples(From,Result);
	qsort((void*)Result->Node.GetValues(),Result->Node.GetAmount(),sizeof(ProfView*),&Compare);
}
//===================================================================================================================//
//CEXPORT int ItemChoose;
bool MMItemChoose(SimpleDialog* SD);
extern DynArray<OneElementEditor*> type_list;
void ShowProfInfo(){
	static int ini=1;
	if(ini){
		type_list.Add(new ProfView_editor);
		ini=0;
	}
	ProfView PV;
	PV.MaxSamples=0;
	if(PV.ReadFromFile("Profile.xml")){
		
		ProfView* Revers = new ProfView();
		Revers->FuncName.Add("TopFunctions");
		ReversProfView(&PV,Revers);
		PV.Node.Add(Revers);
		
		xmlQuote xml;
		ItemChoose=-1;
		if(xml.ReadFromFile("Dialogs\\Profiler.DialogsSystem.xml")){
			DialogsSystem DSS;
			ErrorPager EP;
			DSS.Load(xml,&DSS,&EP);
			SimpleDialog* Desk=DSS.Find("OptionsDesk");
			SimpleDialog* OK=DSS.Find("OK");
			SimpleDialog* CANCEL=DSS.Find("CANCEL");
			if(Desk&&OK&&CANCEL){
				int x0,y0,x1,y1;
				DSS.GetDialogsFrame(x0,y0,x1,y1);
				if(x1>x0){
					DSS.x=(RealLx-x1+x0)/2;
					DSS.y=(RealLy-y1+y0)/2;
					OK->OnUserClick=&MMItemChoose;
					OK->UserParam=1;
					CANCEL->OnUserClick=&MMItemChoose;
					CANCEL->UserParam=1;
					ClassEditor CE;
					CE.CreateFromClass(Desk,0,0,Desk->x1-Desk->x,Desk->y1-Desk->y,&PV,3,"EmptyBorder");
					do{
						ProcessMessages();					
						DSS.ProcessDialogs();
						CE.Process();
						DSS.RefreshView();
					}while(ItemChoose==-1);
				}
			}
		}
	}
}
