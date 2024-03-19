#include "stdheader.h"
#include "Extensions.h"
#include "vui_Actions.h"
//
void addmr2(int v,char* s,int L);
#define addmr(v) addmr2(v,__FILE__,__LINE__)

#include "ExMapGenerator.h"
int mrand();

extern TreesScope TSC;
GetAngleInPoint(int x,int y);
bool GetGradPoint(int &x,int &y,int r);
bool GetIsoPoint(int &x,int &y,int r);
//extern int MinMapX;
//extern int MaxMapX;
//extern int MinMapY;
//extern int MaxMapY;
char* PredefinedMapStyle=NULL;
int Condition::GetCheck(int x,int y,int Height,int Angle,int Tex,int Fact,int FactW){
	int WT=0;
	if(Height>=StartHeight&&Height<FinHeight)WT=255;
	else{
		int dh=Height<StartHeight?StartHeight-Height:Height-FinHeight;
		if(MaxHeightDiff<1)MaxHeightDiff=1;
		WT=255*(MaxHeightDiff-dh)/MaxHeightDiff;
		if(WT<0)WT=0;
	}
	int WA=0;
	if(Angle>=StartAngle&&Angle<FinAngle)WA=255;
	else{
		int dh=Angle<StartAngle?StartAngle-Angle:Angle-FinAngle;
		if(MaxAngleDiff<1)MaxAngleDiff=1;
		WA=255*(MaxAngleDiff-dh)/MaxAngleDiff;
		if(WA<0)WA=0;
	}
	int W0=WA*WT/255;
	if(TextureControl==1 && TexFactControl.Texture!=Tex)//this
		W0=0;
	if(TextureControl==2 && TexFactControl.Texture==Tex)//not this
		W0=0;
	if(FactureControl==1 && TexFactControl.Facture!=Fact)//this
		W0=0;
	if(FactureControl==1 && TexFactControl.Facture==Fact && !FactW)//this
		W0=0;
	if(FactureControl==2 && TexFactControl.Facture==Fact && FactW)//not this
		W0=0;
	return W0;
}
int Condition::GetResult(int x,int y,int Height,int Angle,int Tex,int Fact,int FactW){
	int W0=255;
	if(CheckDistance==0){
        W0=GetCheck(x,y,Height,Angle,Tex,Fact,FactW);
	}else{
		static int dxy[12]={-2,-1,0,-2,2,-1,2,1,0,2,-2,1};
		int RR=CheckDistance>>1;
		if(DistanceCheckType==0){//gradient
			int xb=x;
			int yb=y;
			int minr=100000;
			for(int i=0;i<6;i++){
				int x1=x+dxy[i+i]*RR;
				int y1=y+dxy[i+i+1]*RR;
				int x10=x1;
				int y10=y1;
				if(GetGradPoint(x1,y1,RR*2)){
					int r=Norma(x1-x,y1-y);
					if(r<minr){
                        minr=r;
						xb=x10;
						yb=y10;
					}
				}
			}
			W0=GetCheck(xb,yb,GetHeight(xb,yb),GetAngleInPoint(xb,yb),Tex,Fact,FactW);
		}else if(DistanceCheckType==1){//radius
			int best;
			int maxw=0;
			for(int i=0;i<6;i++){
				int x1=x+dxy[i+i]*RR;
				int y1=y+dxy[i+i+1]*RR;
				W0=GetCheck(x1,y1,GetHeight(x1,y1),GetAngleInPoint(x1,y1),Tex,Fact,FactW);
				if(W0>=maxw){
					best=W0;
					maxw=W0;
				}
			}
			W0=best;
		}else if(DistanceCheckType==2){//isoline
			int xb=x;
			int yb=y;
			int xbi=x;
			int ybi=y;
			int minr=100000;
			for(int i=0;i<6;i++){
				int x1=x+dxy[i+i]*RR;
				int y1=y+dxy[i+i+1]*RR;
				int x10=x1;
				int y10=y1;
				if(GetIsoPoint(x1,y1,RR*2)){
					int r=Norma(x10+x1-x,y10+y1-y);
					if(r<minr){
						minr=r;
						xb=x10;
						yb=y10;
						int x1=x-dxy[i+i]*RR;
						int y1=y-dxy[i+i+1]*RR;
					}
				}
			}
			int W1;
			int W2;
			W1=GetCheck(xb,yb,GetHeight(xb,yb),GetAngleInPoint(xb,yb),Tex,Fact,FactW);
			W2=GetCheck(xbi,ybi,GetHeight(xbi,ybi),GetAngleInPoint(xbi,ybi),Tex,Fact,FactW);
			if(W1>W2){
                W0=W1;
			}else{
				W0=W2;
			}
		}
		if(ExcludeCentralPoint){
			int W1;
			W1=GetCheck(x,y,Height,Angle,Tex,Fact,FactW);
			int wmax=max(W0,W1);
			if(wmax){
				W0=(wmax-W1)*W0/wmax;
			}
		}
	}
	return W0;
}
bool Action::GetResult(int x,int y,int W0,ExTexResult& result){
	float fw=WeightVariation.GetFValue(x,y);
	W0=int(float(W0)*WholeWeight*fw);
	int NT=Textures.GetAmount();
	if(NT>0){
		int TW=TexturesMixing.GetFValue(x,y)*255;
		int T0=(TW*(NT-1))>>8;
		int T1;
		if(NT==1) 
			T1=T0;
		else
			T1=T0+1;
		int W=255-TW;
		if(W<128){
			swap(T0,T1);
			W=255-W;
		}
		result.t1=Textures[T0]->Texture;
		result.t2=Textures[T1]->Texture;
		result.f1=Textures[T0]->Facture;
		result.f2=Textures[T1]->Facture;
		result.w1=W0*W/255;
		result.w2=W0*(255-W)/255;
		result.tdz1=Textures[T0]->TexFactDz;
		result.tdz2=Textures[T1]->TexFactDz;
	}else{
		result.w1=0;
		result.w2=0;
		result.t1=0;
		result.t2=0;
		result.f1=0;
		result.f2=0;
		result.tdz1=0;
		result.tdz2=0;
	}
	result.fWeight1=FacturesWeight*FactWeightVariaton.GetFValue(x,y)*2.0f;
	result.fWeight2=result.fWeight1;
	result.LockMask=int(Locking.Lock0)+(int(Locking.Lock1)<<1)+(int(Locking.Lock2)<<2)+(int(Locking.Lock3)<<3);
	if(Trees.GetAmount()>0){
		addmr(Trees.GetAmount()+1);
		float Weights[64];
		float sw=0;
		for(int i=0;i<Trees.GetAmount();i++){
			assert(i<64);
			Weights[i]=Trees[i]->WholeDensity*Trees[0]->Density.GetFValue(x,y);
			float r=Trees[i]->MinDistance;
			sw+=Weights[i]*r;
		}
		float p=sw*float(mrand())/32767.0;
		sw=0;
		int bidx=0;
		for(int i=0;i<Trees.GetAmount();i++){
			float r=Trees[i]->MinDistance;
			sw+=Weights[i]*r;
			if(p<=sw){
				bidx=i;
				break;
			}
		}
		addmr(555);
		result.TreesGrp=Trees[bidx]->Trees;
		result.TreesDensity=fw*Weights[bidx];
		result.TreesDist=Trees[bidx]->MinDistance;
		addmr(result.TreesGrp);
	}else{
		addmr(444);
		result.TreesGrp=-1;
		result.TreesDensity=0;
		result.TreesDist=0;
	}
	if(Mask.Enabled){
		int ms=240*32<<ADDSH;
		float xx=float(x)/ms;
		float yy=float(y)/ms;
		BasePaintMap* BPM=MLayers->Maps[Mask.Map.TopLayerIndex];
        float M=BPM->GetGreyPrecise(xx,yy);
		result.w1=result.w1*M/256;
		result.w2=result.w2*M/256;
	}
	addmr(result.TreesGrp);
	return 1;
}
bool ExLandsScope::GetResult(int x,int y,int Height,int Angle,int Tex,int Fact,int FactW,ExTexResult& result){
	_clearfp();
	_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);
	addmr(x);
	addmr(y);
	addmr(Height);
	addmr(Angle);
	addmr(Tex);
	addmr(Fact);
	addmr(FactW);

	int Weights [64];
	int WEIGHTS[64];
	int Factures[64];
	int TexFactDz[64];
	int Trees[64];
	byte Smooth[64];
	float TWeights[64];
	float MinDistance[64];
	float FW[64];
	byte  LockMask[64];
	memset(Weights ,0,sizeof Weights );
	memset(WEIGHTS ,0,sizeof WEIGHTS );
	memset(Factures ,0,sizeof Factures );
	memset(TexFactDz ,0,sizeof TexFactDz );
	memset(Trees ,0,sizeof Trees );
	memset(Smooth ,0,sizeof Smooth );
	memset(TWeights ,0,sizeof TWeights );
	memset(MinDistance ,0,sizeof MinDistance );
	memset(FW ,0,sizeof FW );
	memset(LockMask ,0,sizeof LockMask );

	int WS=0;
	int WMAX=0;
	int WMAXNEXT=0;
	float x0=float(3840<<ADDSH);
	float y0=float(3840<<ADDSH);
	for(int j=0;j<Terr.GetAmount();j++)if(Terr[j]->Enabled){
		float _fx[]={0,0,-1,1, 0,0,-1, 1,-1,1};
		float _fy[]={0,0, 0,0,-1,1,-1,-1, 1,1};
		float _fc[]={0,2, 0,0, 0,0, 0, 0, 0,0};
		float fx=_fx[Terr[j]->WeightPreferring];
		float fy=_fy[Terr[j]->WeightPreferring];
		float fc=_fc[Terr[j]->WeightPreferring];
		float w0=Terr[j]->GlobalWeight;
		float w=Terr[j]->WeightVariation.GetFValue(x,y);
		addmr(w*10000);
        w=256*w0*(w+(x-x0)*fx/x0+(y-y0)*fy/y0+(1-sqrt((x-x0)*(x-x0)+(y-y0)*(y-y0))/x0)*fc);
		addmr(w*10000);
		if(Terr[j]->Mask.Enabled){
			int ms=240*32<<ADDSH;
			float xx=float(x)/ms;
			float yy=float(y)/ms;
			BasePaintMap* BPM=MLayers->Maps[Terr[j]->Mask.Map.TopLayerIndex];
			float M=BPM->GetGreyPrecise(xx,yy);
			w=w*M/256;
		}
		addmr(w*10000);
		assert(j<64);
		WEIGHTS[j]=int(w);
		addmr(j);
		addmr(WEIGHTS[j]);
		if(WEIGHTS[j]<1)WEIGHTS[j]=1;
		WS+=WEIGHTS[j];
		if(WEIGHTS[j]>WMAX){
			WMAXNEXT=WMAX;
			WMAX=WEIGHTS[j];
		}else{
			if(WEIGHTS[j]>WMAXNEXT){
				WMAXNEXT=WEIGHTS[j];
			}
		}
	};
	for(int j=0;j<Terr.GetAmount();j++)if(Terr[j]->Enabled){
		int W0=WEIGHTS[j];
		int W=W0*W0*W0/WS;
		if(W<4)W=4;
		if(abs(W0-WMAX)<=WMAX/4){
			ExTerrain* TP=Terr[j];
			int WC=0;
			bool smooth=true;
			ExTexResult tr;
			ExTexResult tr1;
			tr.w1=0;
			for(int i=0;i<TP->Rules.GetAmount();i++)if(TP->Rules[i]->Enabled){
				WC=TP->Rules[i]->Condition.GetResult(x,y,Height,Angle,Tex,Fact,FactW);
				if(tr.w1>0){
					TP->Rules[i]->Action.GetResult(x,y,WC,tr1);
					if(tr1.w1>tr.w1){
						tr.tdz2=tr.tdz1;
						tr.t2=tr.t1;
						tr.w2=tr.w1;
						tr.f2=tr.f1;
						tr.fWeight2=tr1.fWeight1;
						tr.tdz1=tr1.tdz1;
						tr.t1=tr1.t1;
						tr.w1=tr1.w1;
						tr.f1=tr1.f1;
						tr.fWeight1=tr1.fWeight1;
						tr.TreesGrp=tr1.TreesGrp;	
						tr.TreesDensity=tr1.TreesDensity;
						tr.TreesDist=tr1.TreesDist;
						tr.LockMask=tr1.LockMask;
						smooth=TP->Rules[i]->Action.Smoothing;
					}else if(tr1.w1>tr.w2){
						tr.tdz2=tr1.tdz1;
						tr.t2=tr1.t1;
						tr.w2=tr1.w1;
						tr.f2=tr1.f1;
						tr.fWeight2=tr1.fWeight1;
					}
					if(tr1.w2>tr.w2){
						tr.tdz2=tr1.tdz2;
						tr.t2=tr1.t2;
						tr.w2=tr1.w2;
						tr.f2=tr1.f2;
						tr.fWeight2=tr1.fWeight2;
					}
				}else{
					TP->Rules[i]->Action.GetResult(x,y,WC,tr);
					smooth=TP->Rules[i]->Action.Smoothing;
				}
			}
			tr.w1=(tr.w1*W)>>8;
			tr.w2=(tr.w2*W)>>8;
			if(tr.t1>=0&&tr.t1<255&&tr.w1>0){
				if(tr.w1>Weights[tr.t1]){
					Factures[tr.t1]=tr.f1;
					TexFactDz[tr.t1]=tr.tdz1;
					if(WEIGHTS[j]==WMAX){
						Smooth[tr.t1]=smooth;
					}
					if(tr.TreesGrp){
						Trees[tr.t1]=tr.TreesGrp;
						TWeights[tr.t1]=short(tr.TreesDensity*256.0f);
						MinDistance[tr.t1]=tr.TreesDist;
					}else{
						Trees[tr.t1]=-1;
						TWeights[tr.t1]=0;
						MinDistance[tr.t1]=0;
					}
					FW[tr.t1]=tr.fWeight1;
					LockMask[tr.t1]=tr.LockMask;
				}
				Weights[tr.t1]+=tr.w1;
				if(tr.t2>=0&&tr.t2<256){
					if(tr.w2>Weights[tr.t2]){
						Factures[tr.t2]=tr.f2;
						TexFactDz[tr.t2]=tr.tdz2;
						FW[tr.t2]=tr.fWeight2;
					}
				}
				Weights[tr.t2]+=tr.w2;
			}
		}
	}
	bool sm=0;
	int maxw=0;
	int btex=0;
	int bfac=0;
	for(int i=0;i<64;i++){
		addmr(Weights[i]);
		if(Weights[i]>maxw){
			maxw=Weights[i];
			btex=i;
			bfac=Factures[i];
		}
	}
	addmr(maxw);
	if(maxw){
		addmr(789);
		Weights[btex]=0;
		sm=Smooth[btex];
		result.tdz1=TexFactDz[btex];
		result.TreesGrp=Trees[btex];
	    result.TreesDensity=TWeights[btex];
	    result.TreesDist=MinDistance[btex];
	}else{
		addmr(987);
		result.tdz1=0;
		result.TreesGrp=-1;
		result.TreesDensity=0;
		result.TreesDist=0;
	}
	addmr(result.TreesGrp);
	result.t1=btex;
	result.f1=bfac;
	result.w1=maxw;
	result.fWeight1=FW[btex];
	result.fWeight2=FW[btex];
	result.LockMask=LockMask[btex];
	maxw=0;
	for(int i=0;i<64;i++){
		if(Weights[i]>maxw){
			maxw=Weights[i];
			btex=i;
			bfac=Factures[i];
		}
	}
	if(maxw){
		Weights[btex]=0;
		result.tdz2=TexFactDz[btex];
	}else{
		result.tdz2=0;
	}
	result.t2=btex;
	result.f2=bfac;
	result.w2=maxw;
	maxw=0;
	return sm||(WMAXNEXT>WMAX*3/4);
}
///////////////////////////////////////////////////////////
extern int RealLx;
extern int RealLy;

/////////////////////////////////Draw circle
void SetTexFactInPoint(int x,int y,int R1,int R2,int ter){
	
}
////////////////////////////////////////////////////////////////////////////////////////////
void SetCurrentProgress(char* Message,int Stage,int maxStage);
bool MMItemChoose(SimpleDialog* SD);
//CEXPORT int ItemChoose;
bool ProcessLoadingFile(char* Mask,char* DestName,int Header);
bool ProcessSavingFile(char* Mask,char* DestName,int Header,bool clear);
void Convert(ExLandsScope* TP);

RandomMapDescription RMDESC;


bool rce_ProcessRandomMapEditor(ClassEditor* CE,BaseClass* BC,int Options){
	if(Options==1){
		if(!MLayers){
			MLayers=new MapsScope;
			MLayers->SetDefaults();
		}
	}
	return false;
}
void AddRandomMapEditor(){
	AddStdEditor("RandomMapParams",&RMDESC,"RandomMapDesc.xml",RCE_DEFAULT,rce_ProcessRandomMapEditor);
}
void ExLandsScope::Generate(){
	
}
ExLandsScope* TP=NULL;

void SaveMLayersToFileInCompactForm(char* filename);
bool LoadMLayersFromFileInCompactForm(char* filename);

void ExPaintMapWith(){
	mkdir("XmlMaps");
	if(!MLayers)MLayers=new MapsScope;
	xmlQuote xml3;

	//RegGeneratorClass();

	if(!MLayers->Maps.GetAmount()){
		if(!LoadMLayersFromFileInCompactForm("Surface\\Maps.dat")){
			if(xml3.ReadFromFile("Surface\\Maps.xml")){
				ErrorPager Error(0);
				MLayers->Load(xml3,MLayers,&Error);
			}else{
				MLayers->SetDefaults();
			}
		}
	}

	if(!TP)TP=new ExLandsScope;
	TP->RegisterMembers();
	TP->reset_class(TP);
	xmlQuote xml;
	if(xml.ReadFromFile("ExLandscape.xml")){
		ErrorPager Error(0);
		TP->reset_class(TP);
		TP->Load(xml,TP,&Error);
		//xmlQuote xml2;
		//TP->Save(xml2,TP);
		//xml2.WriteToFile("Landscape2.xml");
	}
	DialogsSystem GDS;
	ClassEditor CE;	
	GDS.CloseDialogs();
	int CED_DLG_LX=540;
	int CED_DLG_LY=750;
	DialogsDesk* DD=GDS.AddDialogsDesk(RealLx/2-CED_DLG_LX/2,RealLy/2-CED_DLG_LY/2,CED_DLG_LX,CED_DLG_LY,"StandartBorderNotScrollable");
	DialogsDesk* DD1=GDS.AddDialogsDesk(RealLx/2-CED_DLG_LX/2,RealLy/2-CED_DLG_LY/2,CED_DLG_LX,CED_DLG_LY,"NullBorder");
	ColoredBar* CB3=DD->addColoredBar(0,CED_DLG_LY-50,CED_DLG_LX,50,0xFF);
	CB3->Diffuse=CED_BackColor;
	CE.ClearAll();
	CE.Base=NULL;
	CE.Frame=NULL;
	CE.CreateFromClass(DD,0,0,CED_DLG_LX,CED_DLG_LY-50,(BaseClass*)TP,3,"StandartBorder");
	void ProcessScreen();
	extern int LastKey;
	LastKey=0;
	extern byte MI_Mode;
    int NBTN=9;
	int BTDY=30;
	RLCFont* AF=&WhiteFont;
	RLCFont* PF=&BlackFont;
	const char* msgs[]={"Load","Save","Generate","PaintOnly","ReliefOnly","Cancel","Convert","ClearMaps",""};
	TextButton* TBS[7];
	int Tl=0;
	for(int i=1;i<NBTN;i++){
		Tl+=GetRLCStrWidth((char*)msgs[i-1],AF);
	}
	Tl=CED_DLG_LX-Tl;
	Tl/=NBTN;
	int xx=Tl;
	for(int i=1;i<NBTN;i++){
		TextButton* TB=DD1->addTextButton(NULL,xx,CED_DLG_LY-BTDY+4,(char*)msgs[i-1],AF,PF,PF,0);
		xx+=Tl+GetRLCStrWidth((char*)msgs[i-1],AF);
		TB->OnUserClick=&MMItemChoose;
        TB->UserParam=i;
	}
	ItemChoose=-1;
	do{
		if((GetKeyState(VK_CONTROL)&0x8000)&&(GetKeyState(VK_MENU)&0x8000)&&(GetKeyState('N')&0x8000)){
			if(MLayers){
				int NM=MLayers->Maps.GetAmount();
				for(int i=0;i<NM;i++){
					MLayers->Maps[i]->Clear();
				}
			}
		}
		//ProcessScreen();
		ProcessMessages();
		if(LastKey==27||LastKey==13)break;
		CE.Process();
		GDS.ProcessDialogs();
        GDS.RefreshView();
		if(MLayers->Maps.GetAmount()){
			for(int i=0;i<MLayers->Maps.GetAmount();i++)MLayers->Maps[i]->Desc.Clear();
			int NR=TP->Relief.Rules.GetAmount();
			for(int i=0;i<NR;i++){
				_str* s=&MLayers->Maps[TP->Relief.Rules[i]->Layer.TopLayerIndex]->Desc;
				*s+=TP->Relief.Rules[i]->Description;
				*s+=" ";
			}
			for(int i=0;i<TP->Terr.GetAmount();i++){
				for(int j=0;j<TP->Terr[i]->Rules.GetAmount();j++){
					if(TP->Terr[i]->Rules[j]->Action.Mask.Enabled){
						_str* s=&MLayers->Maps[TP->Terr[i]->Rules[j]->Action.Mask.Map.TopLayerIndex]->Desc;
						*s+=TP->Terr[i]->Rules[j]->Description;
						*s+=" ";
					}
				}
			}
			for(int i=0;i<MLayers->Maps.GetAmount();i++){
				if(i<TP->PredefinedNamesOfLayers.GetAmount()){
					MLayers->Maps[i]->Desc=*TP->PredefinedNamesOfLayers[i];
				}else{
					_str* s=&MLayers->Maps[i]->Desc;
					if(s&&s->pchar()){
						void Translit(char* s);
						Translit(s->pchar());
					}
				}				
			}
		}
		switch(ItemChoose){
			case 1://Load
				{
					char dst[256];
					if(ProcessLoadingFile("XmlMaps\\*.xml",dst,4)){
						xmlQuote xml3;
						_str s;
						s="XmlMaps\\";
						s+=dst;
						if(xml3.ReadFromFile(s.pchar())){
							ErrorPager Error(0);
							MLayers->Clear();
							MLayers->Load(xml3,MLayers,&Error);
                            CE.ExpInfChanged=1;
						}
					}
				}
				break;
			case 2://Save
				char dst[256];
				if(ProcessSavingFile("XmlMaps\\*.xml",dst,3,1)){
					xmlQuote xml3;
					MLayers->Save(xml3,MLayers);
					_str sd;
					sd="XmlMaps\\";
					sd+=dst;
					if(!strstr(sd.pchar(),".xml"))sd+=".xml";
                    xml3.WriteToFile(sd.pchar());
				}
				break;
			case 3://Generate
				TP->GenerateRelief=1;
				TP->GenerateTextures=1;
				LastKey=13;
				break;
			case 4://PaintOnly
				TP->GenerateRelief=0;
				TP->GenerateTextures=1;
				LastKey=13;
				break;
			case 5://ReliefOnly
				TP->GenerateRelief=1;
				TP->GenerateTextures=0;
				LastKey=13;
				break;
			case 6://cancel
				LastKey=27;
				break;
			case 7:				
				Convert(TP);
				break;
			case 8:
				for(int i=0;i<MLayers->Maps.GetAmount();i++){
					MLayers->Maps[i]->Clear();
				}
				break;
		}
		ItemChoose=-1;
	}while(true);
	xmlQuote xml2;
	TP->Save(xml2,TP);
	void RearrangeSprites();
	if(LastKey==13){
		xmlQuote xml4;
		if(!(GetKeyState(VK_CONTROL)&0x8000)){
			/*
			SetCurrentProgress("Saving",0,10);
			MLayers->Save(xml4,MLayers);
			SetCurrentProgress("Saving",5,10);
			xml4.WriteToFile("Maps.xml");
			*/
			SaveMLayersToFileInCompactForm("Surface\\Maps.dat");
			SetCurrentProgress("Saving",8,10);			
			void DeleteAllSprites();
			DeleteAllSprites();
			xml2.WriteToFile("ExLandscape.xml");
			SetCurrentProgress("Saving",9,10);
			void CreateFractal();
			CreateFractal();
			SetCurrentProgress("Saving",10,10);
		}else{
			SetCurrentProgress("Saving",1,10);
			xml2.WriteToFile("ExLandscape.xml");
			SetCurrentProgress("Saving",6,10);
			void CreateFractal();
			CreateFractal();
			SetCurrentProgress("Saving",10,10);
		}
		TP->Generate();
	};
	bool CheckFactureWeightInPoint(int V);
	for(int i=0;i<MaxPointIndex;i++)CheckFactureWeightInPoint(i);
	void MakeAllDirtyGBUF();
	MakeAllDirtyGBUF();
	void ResetWaterHash();
	ResetWaterHash();
	void ResetGroundCache();
	ResetGroundCache();
	void CreateMiniMap();
	CreateMiniMap();
	LastKey=0;
	//delete(TP);
	//SetTexFactInPoint(4000,3200,800,1000,1);
}
FILE* FLG=NULL;
bool AllowMR=false;
void addmr1(int v,char* s,int L){
	return;
    extern int mrpos;
	mrpos+=v;
	mrpos&=8191;
	if(AllowMR){
		if(FLG&&!(GetKeyState(' ')&0x8000)){
			char* s1=s+strlen(s);
			while(s1>s&&s1[0]!='\\')s1--;
			fprintf(FLG,"%d %d %s %d\n",v,mrpos,s1,L);
		}
	}
}
void addmr2(int v,char* s,int L){				
	return;
	extern int mrpos;
	mrpos+=v;
	mrpos&=8191;
	if(FLG&&!(GetKeyState(' ')&0x8000)){
		char* s1=s+strlen(s);
		while(s1>s&&s1[0]!='\\')s1--;
		fprintf(FLG,"%d %d %s %d\n",v,mrpos,s1,L);
	}
}
void addmr1(int v,char* s,int L);
void initmr(){
	FLG=fopen("rmlog.log","w");
}
void closemr(){
	if(FLG)fclose(FLG);
	FLG=NULL;
}
bool OneRandomLayer::CheckConditions(int x,int y){
	bool c=true;
	int N=Conditions.GetAmount();
	for(int q=0;q<N;q++)c&=Conditions[q]->Check(x,y);
	return c;
}
void OneRandomLayer::Perform(){	
	addmr(17);
	if(AffectedLayer>=0){
		addmr(18);
		BasePaintMap* GM=MLayers->Maps[AffectedLayer];
		addmr(19);
		GM->Clear();
		addmr(20);
		int Lx,Ly;
		GM->GetSize(Lx,Ly);
		addmr(21);
		int N=Conditions.GetAmount();
		addmr(22);
		for(int j=0;j<Lx;j++){
			for(int i=0;i<Lx;i++)if(CheckConditions(i,j)){
				addmr(i);
				addmr(j);
				AllowMR=true;
				float fv=PiecesShape.GetFValue(i*16,j*16);
				AllowMR=false;
				addmr(12);
				GM->SetValue(i,j,255,fv*255);
				addmr(fv*255);
			}
		}
		//GM->CreateTexturePart(0,0,512,512);
	}
}
void OneRandomLandscapeType::Perform(int Code,float MaxWaterPercent){
	addmr(66);
	extern int mrpos;
	mrpos=Code&8191;
	int mrand();
	const RECT rc = MOptions.LimitCamArgs.GetRect();
//	float x0=(MaxMapX+MinMapX)*8;
	float x0=(rc.right+rc.left)*8;
	float coef=0.0004;
	float coef2=coef*coef;
	int i=Lands.GetAmount()*mrand()/32768;
	int n=Lands[i]->Samples.GetAmount();
//	float xcoef=float(MaxMapX-MinMapX)/480.0f;
//	float ycoef=float(MaxMapY-MinMapY)/480.0f;
	float xcoef=float(rc.right-rc.left)/480.0f;
	float ycoef=float(rc.bottom-rc.top)/480.0f;
	bool Used[64];
	memset(&Used,0,sizeof Used);
	addmr(67);
	for(int j=0;j<n;j++){
		addmr(77);
		ShowLoadProgress("CreatingLayers",j,n);
		_clearfp();
		_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);
		OneRandomLayer* RL=Lands[i]->Samples[j];
		int afL=RL->AffectedLayer;
		if(!Used[afL]){
			int NL=0;
			int LIDX[8];
			for(int q=j;q<n&&NL<8;q++){
				if(Lands[i]->Samples[q]->AffectedLayer==afL){
					LIDX[NL]=q;
                    NL++;
				}
			}
			if(NL>1){
				RL=Lands[i]->Samples[LIDX[mrand()%NL]];
			}
			Used[afL]=1;
			_fractalLayer* F=&RL->PiecesShape;						
			F->phaseX=mrand();
			F->phaseY=mrand();
			addmr(F->phaseX);
			addmr(F->phaseY);
			F->CenterX=x0;
			F->CenterY=x0;
			F->Constant=0;//((RL->Constant_start+(RL->Constant_final-RL->Constant_start)*mrand()/32768)-0.5);
			F->LinearX=((RL->LinearX_start+(RL->LinearX_final-RL->LinearX_start)*mrand()/32768)-0.5)*coef;
			F->LinearY=((RL->LinearY_start+(RL->LinearY_final-RL->LinearY_start)*mrand()/32768)-0.5)*coef;
			F->SquareX=((RL->SquareX_start+(RL->SquareX_final-RL->SquareX_start)*mrand()/32768)-0.5)*coef2;
			F->SquareY=((RL->SquareY_start+(RL->SquareY_final-RL->SquareY_start)*mrand()/32768)-0.5)*coef2;
			//tuning filling percent
			short ptx[512];
			short pty[512];
			int npt=0;
			for(int q=0;q<2048&&npt<512;q++){
				int x=xcoef*(mrand()&511);
				int y=ycoef*(mrand()&511);
				addmr(x);
				addmr(y);
				if(RL->CheckConditions(x,y)){
					ptx[npt]=x;
					pty[npt]=y;
					addmr(x);
					addmr(y);
					npt++;
				}
			}
			addmr(88);
			float FP=RL->FillingPercent;
			if(afL==4){
				if(FP>MaxWaterPercent)FP=MaxWaterPercent;
			}
			if(npt){				
				int dir=0;
				int nitr=0;
				addmr(npt);
				addmr(RL->PiecesShape.Constant*100);
				do{
					int ns=0;
					addmr(npt);
					for(int q=0;q<npt;q++){
						addmr(RL->PiecesShape.phaseX);
						addmr(RL->PiecesShape.phaseY);
						addmr(ptx[q]);
						addmr(pty[q]);
						AllowMR=true;
						float v=RL->PiecesShape.GetFValue(ptx[q]*16,pty[q]*16);
						AllowMR=false;
						addmr(v*1000);
						if(v>0.5f)ns++;
					}
					addmr(33);
					float pp=float(ns)/npt;
					if(pp>FP){
						if(dir==0)dir=1;
						if(dir==-1)break;
						RL->PiecesShape.Constant-=0.01f;
						addmr(RL->PiecesShape.Constant*1000);
					}else
					if(pp<FP){
						if(dir==0)dir=-1;
						if(dir==1)break;
						RL->PiecesShape.Constant+=0.01f;
						addmr(RL->PiecesShape.Constant*1000);
					}
					nitr++;
					addmr(nitr);
				}while(nitr<300);
			}
			addmr(99);
			RL->Perform();
		}
	}
}
void OneRandomLandscapeType::Resample(int LayerType){
	const RECT rc = MOptions.LimitCamArgs.GetRect();
//	float x0=(MaxMapX+MinMapX)*8;
	float x0=(rc.right+rc.left)*8;
	float coef=0.0004;
	float coef2=coef*coef;
	int i=Lands.GetAmount()*mrand()/32768;
	int n=Lands[i]->Samples.GetAmount();
//	float xcoef=float(MaxMapX-MinMapX)/480.0f;
//	float ycoef=float(MaxMapY-MinMapY)/480.0f;
	float xcoef=float(rc.right-rc.left)/480.0f;
	float ycoef=float(rc.bottom-rc.top)/480.0f;
	bool Used[64];
	memset(&Used,0,sizeof Used);
	for(int j=0;j<n;j++){		
		OneRandomLayer* RL=Lands[i]->Samples[j];
		int afL=RL->AffectedLayer;
		if(afL==LayerType){			
			RL->Perform();
			break;
		}
	}
}
bool _IsOutsideTheLayer::Check(int x,int y){
	BasePaintMap* GM=MLayers->Maps[IsOutsideTheLayer];
	int id,w,ni;
	GM->GetValue(x,y,id,w,ni);
	return w<128;
}
bool _IsInsideTheLayer::Check(int x,int y){
	BasePaintMap* GM=MLayers->Maps[IsInsideTheLayer];
	int id,w,ni;
	GM->GetValue(x,y,id,w,ni);
	return w>128;
}
#define BoundSize 18
#define SqBound (BoundSize*7/10)
bool _IsOnBoundOfTheLayer::Check(int x,int y){
	BasePaintMap* GM=MLayers->Maps[IsOnBoundOfTheLayer];
	int id,w1,w2,w3,w4,ni;
	GM->GetValue(x-SqBound,y-SqBound,id,w1,ni);
	GM->GetValue(x+SqBound,y-SqBound,id,w2,ni);
	GM->GetValue(x-SqBound,y+SqBound,id,w3,ni);
	GM->GetValue(x+SqBound,y+SqBound,id,w4,ni);
	int wa=(w1+w2+w3+w4)/4;
	GM->GetValue(x-BoundSize,y,id,w1,ni);
	GM->GetValue(x+BoundSize,y,id,w2,ni);
	GM->GetValue(x,y-BoundSize,id,w3,ni);
	GM->GetValue(x,y+BoundSize,id,w4,ni);
	wa=(wa+(w1+w2+w3+w4)/4)/2;
	return wa!=0&&wa!=255;
}
bool _FarOutsideTheLayer::Check(int x,int y){
	BasePaintMap* GM=MLayers->Maps[FarOutsideTheLayer];
	int id,ni,w;
	GM->GetValue(x,y,id,w,ni);
	if(w>128)return false;
	GM->GetValue(x-SqBound,y-SqBound,id,w,ni);
	if(w>128)return false;
	GM->GetValue(x+SqBound,y-SqBound,id,w,ni);
	if(w>128)return false;
	GM->GetValue(x-SqBound,y+SqBound,id,w,ni);
	if(w>128)return false;
	GM->GetValue(x+SqBound,y+SqBound,id,w,ni);
	if(w>128)return false;	
	GM->GetValue(x-BoundSize,y,id,w,ni);
	if(w>128)return false;	
	GM->GetValue(x+BoundSize,y,id,w,ni);
	if(w>128)return false;	
	GM->GetValue(x,y-BoundSize,id,w,ni);
	if(w>128)return false;	
	GM->GetValue(x,y+BoundSize,id,w,ni);
	if(w>128)return false;		
	return true;
}
bool _DeepInsideTheLayer::Check(int x,int y){
	BasePaintMap* GM=MLayers->Maps[DeepInsideTheLayer];
	int id,ni,w;
	GM->GetValue(x,y,id,w,ni);
	if(w<240)return false;
	GM->GetValue(x-SqBound,y-SqBound,id,w,ni);
	if(w<240)return false;
	GM->GetValue(x+SqBound,y-SqBound,id,w,ni);
	if(w<240)return false;
	GM->GetValue(x-SqBound,y+SqBound,id,w,ni);
	if(w<240)return false;
	GM->GetValue(x+SqBound,y+SqBound,id,w,ni);
	if(w<240)return false;	
	GM->GetValue(x-BoundSize,y,id,w,ni);
	if(w<240)return false;	
	GM->GetValue(x+BoundSize,y,id,w,ni);
	if(w<240)return false;	
	GM->GetValue(x,y-BoundSize,id,w,ni);
	if(w<240)return false;	
	GM->GetValue(x,y+BoundSize,id,w,ni);
	if(w<240)return false;		
	return true;
}
bool _OR_conditions::Check(int x,int y){
	if(!OR.GetAmount())return false;
	bool sc=false;
	for(int i=0;i<OR.GetAmount();i++)sc|=OR[i]->Check(x,y);
	return sc;
}
bool _NOT_condition::Check(int x,int y){
	OneLayerCondition* OLC=NOT.Get();
	if(OLC)return !OLC->Check(x,y);
	return true;
}
int mrand();
bool OneStartPositionType::SetPositions(StartPositionTempStruct* temp,char* NatList,bool CheckHeight,bool DivideByWater){
	const RECT rc = MOptions.LimitCamArgs.GetRect();
//	int Lx=MaxMapX/2;
//	int Ly=MaxMapY/2;
	int Lx=rc.right/2;
	int Ly=rc.bottom/2;
	temp->Zone.SetSize(Lx,Ly,0xFFFF);
	temp->Start.SetSize(Lx,Ly,0xFFFF);
	temp->Pieces.SetSize(Lx,Ly,0xFFFF);	
	temp->Voronoi.SetSize(Lx,Ly,0xFFFF);
	int NPlayers=0;
	int L=strlen(NatList);
	int Presence[8]={0,0,0,0,0,0,0,0};
	for(int i=0;i<L&&i<8;i++)if(NatList[i]!='0'){
		Presence[NPlayers]=i;
		NPlayers++;	
	}
	for(int i=8;i<Lx-8;i++){
		for(int j=8;j<Ly-8;j++){
            int xm=i*512/240;
			int ym=j*512/240;
			int xr=i*64;
			int yr=j*64;
			//if(GetHeight(xr,yr)>10||(!CheckHeight)){
				int N=AccessConditions.GetAmount();
				bool res=true;
				for(int q=0;q<N&&res;q++){
					res&=AccessConditions[q]->Check(xm,ym);
				}
				if(res){
					temp->Zone.Set(i,j,0xFFFE);
					int N=StartPlaceConditions.GetAmount();
					bool res=true;
					for(int q=0;q<N&&res;q++){
						res&=StartPlaceConditions[q]->Check(xm,ym);
					}
					if(res)temp->Start.Set(i,j,0xFFFE);
				}
				N=PiecesConditions.GetAmount();
				res=true;
				for(int q=0;q<N&&res;q++){
					res&=PiecesConditions[q]->Check(xm,ym);
				}
				if(res)temp->Pieces.Set(i,j,0);
			//}
		}
	}
	int max=temp->Zone.FillWithTopValues(0xFFFE,0xFFFF);
	DynArray<int> W;
	W.Add(0,max+1);
	for(int i=0;i<Lx;i++){
		for(int j=0;j<Ly;j++){
			word V=temp->Zone.Get(i,j);
			if(V!=0xFFFF&&V!=0xFFFE){
				W[V]++;
			}
		}
	}
	int maxV=0;
	int maxP=-1;
	for(int i=0;i<=max;i++){
		if(W[i]>maxV){
			maxV=W[i];
			maxP=i;
		}
	}
	if(maxP!=-1){
		DynArray<DWORD> xy;		
		for(int i=3;i<Lx-3;i++){
			for(int j=3;j<Ly-3;j++){
				word V=temp->Zone.Get(i,j);
				if(V!=maxP){
					if(temp->Start.Get(i,j)!=0xFFFF)temp->xy_bad.Add(i+(j<<16));
                    temp->Zone.Set(i,j,0xFFFF);
					temp->Start.Set(i,j,0xFFFF);                    
				}else{
					if(temp->Start.Get(i,j)!=0xFFFF){
						temp->Start.Set(i,j,0);
						xy.Add(i+(j<<16));
					}
				}
			}
		}
		if(xy.GetAmount()){
			//solving distance problem			
			int bestV=0;
			for(int i=0;i<15000;i++){
                int CurX[32];
				int CurY[32];
				for(int i=0;i<NPlayers;i++){					
					DWORD V=xy[mrand()%xy.GetAmount()];
					CurX[i]=V&0xFFFF;
					CurY[i]=V>>16;
				}
				int minr=10000;
				int sr=0;
				for(int q=1;q<NPlayers;q++){
					for(int p=0;p<q;p++){
                        int r=Norma(CurX[p]-CurX[q],CurY[p]-CurY[q]);
						if(r<minr)minr=r;
					}					
				}
				for(int q=0;q<NPlayers;q++){
					int L=Norma(CurX[q]-Lx/2,CurY[q]-Ly/2);
					if(L>Lx/4)L=Lx/4;
					sr+=L/8;
				}
				if(minr>Lx*3/4)minr=Lx*3/4;
				minr+=sr;
				if(minr>bestV){
					memcpy(temp->BestX,CurX,NPlayers<<2);
					memcpy(temp->BestY,CurY,NPlayers<<2);
					bestV=minr;
				}
			}
			temp->bestV=bestV;
			int rm=bestV/2;
			int rm2=rm*3/4;
			int scv=64;			
			temp->Voronoi.CreateVoronoiDiagramm(temp->BestX,temp->BestY,NPlayers);
			if(DivideByWater){
				for(int ix=0;ix<Lx;ix++){
					for(int iy=0;iy<Ly;iy++){
                        word V1=temp->Voronoi.Get(ix-1,iy-1,0xFFFF);
						word V2=temp->Voronoi.Get(ix+1,iy-1,0xFFFF);
						word V3=temp->Voronoi.Get(ix-1,iy+1,0xFFFF);
						word V4=temp->Voronoi.Get(ix+1,iy+1,0xFFFF);
						word V=0xFFFF;

						if(V1!=0xFFFF) V=V1;
						if(V2!=0xFFFF) V=V2;
						if(V3!=0xFFFF) V=V3;
						if(V4!=0xFFFF) V=V4;

						if(V1==0xFFFF) V1=V;
						if(V2==0xFFFF) V2=V;
						if(V3==0xFFFF) V3=V;
						if(V4==0xFFFF) V4=V;

						if(!(V1==V2&&V1==V3&&V1==V4)){
							//setting water spot
							int xx=ix*512/240;//+(mrand()&7)-3;
							int yy=iy*512/240;//+(mrand()&7)-3;
                            MLayers->Maps[4]->SetValueInRound(xx,yy,4,3,255);
							MLayers->Maps[0]->SetValueInRound(xx,yy,4,3,0);
							//if(mrand()<8000){
                                //MLayers->Maps[6]->SetValueInRound(xx,yy,5,2,255);
							//}
						}
					}
				}
			}			
			return true;
		}
	}
	return false;
}
void delTrees(int x,int y1,int r);
bool OneStartPositionType::SetPieces(StartPositionTempStruct* temp,char* NatList,bool CheckHeight,bool DivideByWater){
	const RECT rc = MOptions.LimitCamArgs.GetRect();
//	int Lx=MaxMapX/2;
//	int Ly=MaxMapY/2;
	int Lx=rc.right/2;
	int Ly=rc.bottom/2;
	int NPlayers=0;
	int L=strlen(NatList);
	int Presence[8]={0,0,0,0,0,0,0,0};
	for(int i=0;i<L&&i<8;i++)if(NatList[i]!='0'){
		Presence[NPlayers]=i;
		NPlayers++;	
	}
	for(int i=0;i<Lx;i++){
		for(int j=0;j<Ly;j++){
			int xm=i*512/240;
			int ym=j*512/240;
			int xr=i*64;
			int yr=j*64;

			int rr=600;

			int H=GetHeight(xr,yr);
			int HL=GetHeight(xr-rr,yr);
			int HR=GetHeight(xr+rr,yr);
			int HU=GetHeight(xr,yr-rr);
			int HD=GetHeight(xr,yr+rr);

			int HL1=GetHeight(xr-rr,yr-rr);
			int HR1=GetHeight(xr+rr,yr-rr);
			int HU1=GetHeight(xr-rr,yr+rr);
			int HD1=GetHeight(xr+rr,yr+rr);

			int HMin1=min(min(min(HL1,HR1),min(HU1,HD1)),H);
			int HMax1=max(max(max(HL1,HR1),max(HU1,HD1)),H);

			int HMin=min(min(min(HL,HR),min(HU,HD)),HMin1);
			int HMax=max(max(max(HL,HR),max(HU,HD)),HMax1);

			if(HMin<=20||HMax>=80){
				for(int px=-2;px<=2;px++)
					for(int py=-2;py<=2;py++){
						temp->Start.Set(i+px,j+py,0xFFFF);
						temp->Zone.Set(i+px,j+py,0xFFFF);
						temp->Pieces.Set(i+px,j+py,0xFFFF);
					}
			}
		}
	}
	int rm=temp->bestV/2;
	int rm2=rm*3/4;
	int scv=64;	
	int nc=MyNation;
	int st_sz=4;
	/*
	FILE* F =fopen("debug.txt","w");
	FILE* FV=fopen("debugV.txt","w");
	FILE* FA=fopen("debugA.txt","w");
	if(F){
		for(int i=0;i<240;i+=3){
			for(int j=0;j<240;j+=3){
				char v='*';
				if(temp->Pieces.Get(j,i)==0){
					int V=temp->Voronoi.Get(j,i);
					if(V!=0xFFFF){
                        v='0'+V;
					}
				}
				fprintf(F,"%c",v);
				v='*';
				if(temp->Pieces.Get(j,i)==0){					
					v='X';
				}
				fprintf(FA,"%c",v);
				v='*';
				int V=temp->Voronoi.Get(j,i);
				if(V!=0xFFFF){					
					v='0'+V;
				}
				fprintf(FV,"%c",v);
			}
			fprintf(F ,"\n");
			fprintf(FA,"\n");
			fprintf(FV,"\n");
		}
		fclose(F);
		fclose(FV);
		fclose(FA);
	}
	*/
	for(int i=0;i<NPlayers;i++){
		SetMyNation(Presence[i]);
		for(int j=0;j<StartPieces.GetAmount();j++){
			int r0=StartPieces[j]->MinRadiusOfNearZone/scv;
			int r1=StartPieces[j]->MaxRadiusOfNearZone/scv;
			if(r0>rm2)r0=rm2;
			if(r1>rm2)r1=rm2;			
			for(int p=0;p<StartPieces[j]->AmountInNearZone;p++){
				SetMyNation(Presence[i]);
				int xx=temp->BestX[i];
				int yy=temp->BestY[i];
				if(temp->Pieces.FindRandomPos(xx,yy,r0,r1,0,temp->Voronoi,i)){
					char* s=StartPieces[j]->Samples[mrand()%StartPieces[j]->Samples.GetAmount()]->Name.pchar();
					extern bool ImmVis;
					ImmVis=true;
					byte cc=MyNation;
					void RM_Load(char* Name,int x,int y);																			
					if(s){
                        if(StartPieces[j]->Neytral)SetMyNation(7);
						delTrees(xx*scv,yy*scv,160);
						RM_Load(s,xx*scv,yy*scv);
						for(int q=-st_sz;q<=st_sz;q++)
							for(int p=-st_sz;p<=st_sz;p++)
								temp->Pieces.Set(xx+q,yy+p,0xFFFF);
					}
					SetMyNation(cc);
					ImmVis=false;
				}
			}
			extern bool ImmVis;
			void RM_Load(char* Name,int x,int y);							
			for(int p=0;p<StartPieces[j]->AmountInFarZone;p++){
				int xx=temp->BestX[i];
				int yy=temp->BestY[i];
				if(temp->Pieces.FindRandomPos(xx,yy,r1,rm,0,temp->Voronoi,i)){
					char* s=StartPieces[j]->Samples[mrand()%StartPieces[j]->Samples.GetAmount()]->Name.pchar();							
					ImmVis=true;
					if(s){
						byte cc=MyNation;
                        if(StartPieces[j]->Neytral)SetMyNation(7);
						delTrees(xx*scv,yy*scv,160);
						RM_Load(s,xx*scv,yy*scv);
						SetMyNation(cc);
						for(int q=-st_sz;q<=st_sz;q++)
							for(int p=-st_sz;p<=st_sz;p++)
								temp->Pieces.Set(xx+q,yy+p,0xFFFF);
					}
					ImmVis=false;
				}
			}
			if(temp->xy_bad.GetAmount()){
				SetMyNation(7);
				for(int i=0;i<FarPieces.GetAmount();i++){
					if(FarPieces[i]->Name.pchar()){
						DWORD p=temp->xy_bad[mrand()%temp->xy_bad.GetAmount()];
						int x=p&0xFFFF;
						int y=p>>16;
						if(temp->Pieces.Get(x,y)==0){
							ImmVis=true;
							//assert(GetHeight(x*scv,y*scv)>20);
							delTrees(x*scv,y*scv,160);
							RM_Load(FarPieces[i]->Name.pchar(),x*scv,y*scv);
							for(int q=-st_sz;q<=st_sz;q++)
								for(int p=-st_sz;p<=st_sz;p++)
									temp->Pieces.Set(x+q,y+p,0xFFFF);
							ImmVis=false;                                
						}
					}
				}
			}					
		}
	}
	SetMyNation(nc);
	return true;
}
void GenerateRandomMap(int Code,int NPlayers){
}
//MapName syntax
//RN0 <%X-randomValue> <%X-options> <Nations>.m3d
//example:
//RN0 1234 0 11110000.m3d
void SetFractalPhase(BaseClass* BC){
	_fractal* F=dynamic_cast<_fractal*>(BC);
	if(F){
		F->phaseX=mrand();
		F->phaseY=mrand();
	}
	int N=BC->GetAmountOfElements();
	for(int i=0;i<N;i++){
		void* ptr=BC->GetElementPtr(i,BC);
		BaseClass* B=BC->GetElementClass(i);
		if(B->CheckDirectCasting()){
			B=(BaseClass*)ptr;
			SetFractalPhase(B);
		}
	}
}
char* VicTypes[]={"VC_KillTownCenter","VC_Score","VC_Mines","VC_AnnihilateHero",""};
int TimeLimits[]={1000,30,45,60};
bool GenerateRandomLayers(char* MapName){		
	//RegGeneratorClass();
	if(!(MapName[0]=='R'&&MapName[1]=='N'&&MapName[2]=='0'))return false;

	/*void DeleteAllSprites();
	DeleteAllSprites();
	void DeleteAllUnits();
	DeleteAllUnits();*/

	ADDSH=1;
	void FreeArrays();
	void SetupArrays();
	//FreeArrays();
	//SetupArrays();
	extern bool mInitRNDMapName;
	mInitRNDMapName=false;
	char nats[32];
	char temp[32];
	extern word XXP;
	XXP=0;
	DWORD V1;
	DWORD Code;
	int z=sscanf(MapName,"%s%X%X%s",temp,&Code,&V1,nats);
	if(z!=4)return false;
	int StartPos=V1&15;
	bool WaterMap=((V1>>4)&15)!=0;
	int StartRes=(V1>>8)&15;
	int VictCond=(V1>>12)&15;
	int TimeLimit=(V1>>16)&15;
	int MapSize=(V1>>20)&15;
	int FogOff=(V1>>24)&15;
	int np=0;
	int L=strlen(nats);
	for(int i=0;i<L;i++)np+=nats[i]!='0'?1:0;
	if(np>3)MapSize=0;
	if(MapSize){
//		MaxMapX=480*10/15;
//		MaxMapY=MaxMapX;
	}
	extern int FogMode;	
	Code&=8191;
	extern int mrpos;	
	mrpos=Code;
	XXP=0;
	//if(MLayers)delete MLayers;
	//if(!MLayers){
	//ShowLoadProgress("SetupMLayers",0,1);
	_clearfp();
	_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);
	//MLayers=new MapsScope;
	//MLayers->SetDefaults();
	//}
	//ShowLoadProgress("SetupExLandscape",0,1);
	_clearfp();
	_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);
	ExLandsScope* TP=new ExLandsScope;
	TP->SafeReadFromFile("ExLandscape.xml");	
	_clearfp();
	_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);

	RMDESC.Init();

	mrpos=Code;
	XXP=0;

	SetFractalPhase(TP);
	SetFractalPhase(&RMDESC);

	int p=RMDESC.Landscapes.GetAmount();	
	initmr();	

	mrpos=Code;
	XXP=0;

	addmr(StartPos);
	addmr(WaterMap);
	addmr(StartRes);
	addmr(VictCond);
	addmr(TimeLimit);
	addmr(MapSize);
	addmr(FogOff);
	addmr(np);
	addmr(Code);
	addmr(V1);

	_clearfp();
	_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);

	if(p){
		OneRandomLandscapeType* ORL=RMDESC.Landscapes[mrand()%p];
		//ShowLoadProgress("CreatingLayers",0,1);
		_clearfp();
		_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);
		float maxfp=2.0f/np/np;
		if(MapSize)maxfp/=3;
		addmr(11);
		ORL->Perform(Code,maxfp);
		addmr(22);
		StartPositionTempStruct temp;
		if(StartPos<RMDESC.StartPositions.GetAmount())
			addmr(33);
		RMDESC.StartPositions[StartPos]->SetPositions(&temp,nats,false,WaterMap);		
		addmr(44);
		if(WaterMap){
			addmr(55);
			ORL->Resample(6);
			ORL->Resample(8);
			addmr(66);			
		}
	}
}
bool GenerateRandomMap(char* MapName){		
	//RegGeneratorClass();
	if(!(MapName[0]=='R'&&MapName[1]=='N'&&MapName[2]=='0'))return false;

	void DeleteAllSprites();
	DeleteAllSprites();
	void DeleteAllUnits();
	DeleteAllUnits();

	ADDSH=1;
	void FreeArrays();
	void SetupArrays();
	FreeArrays();
	SetupArrays();
	extern bool mInitRNDMapName;
	mInitRNDMapName=false;
	char nats[32];
	char temp[32];
	extern word XXP;
	XXP=0;
	DWORD V1;
	DWORD Code;
	int z=sscanf(MapName,"%s%X%X%s",temp,&Code,&V1,nats);
	if(z!=4)return false;
	int StartPos=V1&15;
	bool WaterMap=((V1>>4)&15)!=0;
	int StartRes=(V1>>8)&15;
	int VictCond=(V1>>12)&15;
	int TimeLimit=(V1>>16)&15;
	int MapSize=(V1>>20)&15;
	int FogOff=(V1>>24)&15;
	int ULimit=(V1>>28)&15;
	int np=0;
	int L=strlen(nats);
	for(int i=0;i<L;i++)np+=nats[i]!='0'?1:0;
	if(np>3)MapSize=0;
	if(MapSize){
//		MaxMapX=480*10/15;
//		MaxMapY=MaxMapX;
	}
	extern int FogMode;	
	Code&=8191;
	extern int mrpos;	
	mrpos=Code;
	XXP=0;
	if(MLayers)delete MLayers;
	//if(!MLayers){
		ShowLoadProgress("SetupMLayers",0,1);
		_clearfp();
		_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);
		MLayers=new MapsScope;
		MLayers->SetDefaults();
	//}
	ShowLoadProgress("SetupExLandscape",0,1);
	_clearfp();
	_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);
	ExLandsScope* TP=new ExLandsScope;
	TP->SafeReadFromFile(PredefinedMapStyle?PredefinedMapStyle:"ExLandscape.xml");	
	_clearfp();
	_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);

	RMDESC.Init();

	mrpos=Code;
	XXP=0;

	SetFractalPhase(TP);
    SetFractalPhase(&RMDESC);

	int p=RMDESC.Landscapes.GetAmount();	
	initmr();	

	mrpos=Code;
	XXP=0;

	addmr(StartPos);
	addmr(WaterMap);
	addmr(StartRes);
	addmr(VictCond);
	addmr(TimeLimit);
	addmr(MapSize);
	addmr(FogOff);
	addmr(np);
	addmr(Code);
	addmr(V1);

	_clearfp();
	_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);

	if(p){
		OneRandomLandscapeType* ORL=RMDESC.Landscapes[mrand()%p];
		ShowLoadProgress("CreatingLayers",0,1);
		_clearfp();
		_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);
		float maxfp=2.0f/np/np;
		if(MapSize)maxfp/=3;
		addmr(11);
		ORL->Perform(Code,maxfp);
		addmr(22);
		StartPositionTempStruct temp;
		if(StartPos<RMDESC.StartPositions.GetAmount())
			addmr(33);
			RMDESC.StartPositions[StartPos]->SetPositions(&temp,nats,false,WaterMap);		
			addmr(44);
		if(WaterMap){
			addmr(55);
			ORL->Resample(6);
			ORL->Resample(8);
			addmr(66);
			//ORL->Resample(9);
		}
		TP->GenerateRelief=true;
		TP->GenerateTextures=true;
		addmr(77);
		_clearfp();
		_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);
		TP->Generate();				
		addmr(88);
		_clearfp();
		_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);
		if(StartPos<RMDESC.StartPositions.GetAmount())
			RMDESC.StartPositions[StartPos]->SetPieces(&temp,nats,true,false);		
		ShowLoadProgress("CreateLightMap",0,1);
		_clearfp();
		_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);
		void CreateLightMap();
		addmr(99);
		CreateLightMap();
		EraseAreas();	
		void CreateTotalLocking();
		ShowLoadProgress("CreateTotalLocking",0,1);
		_clearfp();
		_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);
		CreateTotalLocking();						
		void ResearchIslands();
		ResearchIslands();
		ShowLoadProgress("ResetGroundCache",0,1);
		_clearfp();
		_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);
		void MakeAllDirtyGBUF();
		MakeAllDirtyGBUF();
		void ResetWaterHash();
		ResetWaterHash();
		void ResetGroundCache();
		ResetGroundCache();		
		void CreateCostPlaces();
		CreateCostPlaces();
		void CreateMiniMap();
		//ShowLoadProgress("CreateMiniMap",0,1);
		CreateMiniMap();
		ShowLoadProgress("CreateMiniMap",1,1);
		_clearfp();
		_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);
		for(int i=0;i<8;i++){
			for(int j=0;j<8;j++){
                SetXRESRC(i,j,5000);
			}
		}
		if(StartRes<RMDESC.StartRes.GetAmount()){
			StartResourcesEx* SR=RMDESC.StartRes[StartRes];
			for(int i=0;i<8;i++){
				SetXRESRC(i,0,SR->Wood);
				SetXRESRC(i,1,SR->Gold);
				SetXRESRC(i,2,SR->Stone);
				SetXRESRC(i,3,SR->Food);
				SetXRESRC(i,4,SR->Iron);
				SetXRESRC(i,5,SR->Coal);
			}
		}
	}
	delete(TP);
	delete(MLayers);
	MLayers=NULL;
	closemr();
	VictoryConditionRoot* VR=NULL;
	switch(VictCond) {
	case 1:
		VR=new VC_ScoreGame;
		((VC_ScoreGame*)VR)->TimeLimit=TimeLimits[TimeLimit];
		break;
	case 2:
		VR=new VC_CaptureGame;
		((VC_CaptureGame*)VR)->TimeLimit=TimeLimits[TimeLimit];
		break;		
	case 3:
		VR=new VC_AnnihilateHero;
		//((VC_AnnihilateHero*)VR)->TimeLimit=TimeLimits[TimeLimit];
		break;
	default:
        VR=new VC_DestroyTownHalls;
	}
	switch(ULimit){
		case 0:
			GSets.CGame.UnitLimit=64000;
			break;
		case 1:
			GSets.CGame.UnitLimit=700;
			break;
		case 2:
			GSets.CGame.UnitLimit=1200;
			break;
		case 3:
			GSets.CGame.UnitLimit=2000;
			break;
	}
	MOptions.VictoryRule.Set(VR);
	FogMode=!FogOff;
	ext_OnInitAfterMapLoading();
	_controlfp(_PC_64|_RC_NEAR|_DN_SAVE,_MCW_PC|_MCW_RC|_MCW_DN);
}
int GetAmountOfExtraOptions(){
    return 8;
}
const char* GetExtraOptionName(int idx){
	switch(idx){
		case 0:
			return GetTextByID("RM_StartPos");
		case 1:
			return GetTextByID("INTF_CROOM_O1");
		case 2:
			return GetTextByID("RM_StartRes");
		case 3:
			return GetTextByID("RM_VictRule");
		case 4:
			return GetTextByID("RM_TimeLimit");
		case 5:
			return GetTextByID("RM_MapSize");
		case 6:
			return GetTextByID("RM_FogOfWar");		
		case 7:
			return GetTextByID("RM_UnitLimit");
	}
	return "???";
}
int GetExtraOptionAmountOfElements(int idx){
	RMDESC.Init();
	switch(idx){
		case 0:
			return RMDESC.StartPositions.GetAmount();
		case 1:
			return 2;
		case 2:
			return RMDESC.StartRes.GetAmount();
		case 3:
			return 4;
		case 4:
			return 4;
		case 5:
			return 2;
		case 6:
			return 2;	
		case 7:
			return 4;
	}
	return 0;
}

char* _ULIMIT[]={"RM_Unlimited","700","1200","2000"};

const char* GetExtraOptionElementName(int idx,int eidx){
	RMDESC.Init();
	switch(idx){
		case 0:
			return GetTextByID(RMDESC.StartPositions[eidx]->Name.pchar());
		case 1:
			if(eidx)return GetTextByID("MS_Islands");
			else return GetTextByID("MS_Land");
		case 2:
			return GetTextByID(RMDESC.StartRes[eidx]->Name.pchar());
		case 3:
			return GetTextByID(VicTypes[eidx]);
		case 4:
			if(eidx>0){
				static char tmp[16];
				sprintf(tmp,"%d %s",TimeLimits[eidx],GetTextByID("#min"));
				return tmp;
			}else return GetTextByID("VC_NoTimeLimit");            
		case 5:
			if(eidx)return GetTextByID("MS_Small");
			else return GetTextByID("MS_Normal");
		case 6:
			if(eidx)return GetTextByID("MS_FogOff");
			else return GetTextByID("MS_FogOn");		
		case 7:
			return eidx?_ULIMIT[eidx]:GetTextByID(_ULIMIT[eidx]);
	}
	return "???";
}
//returns true if option available
//idx  - option number
//eidx - option value
//OptionsValuesArray - array of current options values
bool GetExtraOptionsCompartability(int idx,int& eidx,int* OptionsValuesArray){
	int ei0=eidx;
	eidx=-1;
#ifdef _DEMO_
	if(idx==2){
		eidx=1;
	}else{
		eidx=0;
	}
	if(idx==4){//time limit
		int vc=OptionsValuesArray[3]; //victory conditions
		if(!(vc==1||vc==2)){ //time limited game only when vc==VC_Score||VC_Mines
			eidx=0;
			return false;
		}			
	}
#else
	if(idx==4){//time limit
        int vc=OptionsValuesArray[3]; //victory conditions
		if(!(vc==1||vc==2)){ //time limited game only when vc==VC_Score||VC_Mines
			eidx=0;
			return false;
		}else{
			if(ei0==0)eidx=3;
		}
	}

	extern word NPlayers;
	int n=NPlayers;
	extern word COMPSTART[8];
	for(int i=NPlayers;i<7;i++)if(COMPSTART[i])n++;

	if(idx==5){//small map		
		if(n>3){
			eidx=0;
			return false;
		}
	}
	if(idx==1){//Land/Sea
		if(n>4){
			eidx=0;
			return false;
		}
	}
#endif _DEMO_
	return true;
}
void CollectRules(_str& dest,int x,int y){
	if(TP){		
		int H=GetHeight(x,y);
		int A=GetAngleInPoint(x,y);
		dest.print("H=%d A=%d  ",H,A);
		for(int i=0;i<TP->Terr.GetAmount();i++){
			if(TP->Terr[i]->Enabled){
				ExTerrain* EXT=TP->Terr[i];
				for(int j=0;j<EXT->Rules.GetAmount();j++){
					ObjectRules* ORL=EXT->Rules[j];
					if(ORL->Enabled){
                        int R=ORL->Condition.GetResult(x,y,H,A,0,0,0);
						if(R>8&&ORL->Description.pchar()){
							char cc[64];
							sprintf(cc,"[%s: %d%%] ",ORL->Description.pchar(),R*100/255);
							dest+=cc;
						}
					}
				}
			}
		}
	}    
}
void UnPress();
extern bool realLpressed;
void UserFriendlyMapGenerator(){
	mkdir("XmlMaps");
	if(!MLayers)MLayers=new MapsScope;
	xmlQuote xml3;
	if(!MLayers->Maps.GetAmount()){
		if(xml3.ReadFromFile("UserLandscape.xml")){
			ErrorPager Error(0);
			MLayers->Load(xml3,MLayers,&Error);
		}else{
			MLayers->SetDefaults();
		}
	}

	if(!TP)TP=new ExLandsScope;
	TP->RegisterMembers();
	TP->reset_class(TP);
	xmlQuote xml;
	if(xml.ReadFromFile("ExLandscape.xml")){
		ErrorPager Error(0);
		TP->reset_class(TP);
		TP->Load(xml,TP,&Error);			
	}
	DialogsSystem GDS;
	char CurPreset[256]="ExLandScape.xml";
	if(GDS.SafeReadFromFile("Dialogs\\RandomMapGenerator.DialogsSystem.xml")){
		int x0,y0,x1,y1;
		GDS.GetDialogsFrame(x0,y0,x1,y1);
		GDS.x=(RealLx-x1+x0)/2;
		GDS.y=(RealLy-y1+y0)/2;
		ItemChoose=-1;
		SimpleDialog* SD;
		SD=GDS.Find("#STARTRES");
		InputBox* IB=dynamic_cast<InputBox*>(SD);
		SD=GDS.Find("#VICTCOND");
		ComboBox* VICT=dynamic_cast<ComboBox*>(SD);
		SD=GDS.Find("#MAPSIZE");
		ComboBox* MAPSIZE=dynamic_cast<ComboBox*>(SD);
		SD=GDS.Find("#NPLAYERS");
		ComboBox* NPLAY=dynamic_cast<ComboBox*>(SD);
		SD=GDS.Find("#TAB_RANDOM");
		TabButton* TB_RANDOM=dynamic_cast<TabButton*>(SD);
		SD=GDS.Find("#TAB_DESIGNED");
		TabButton* TB_DESIGN=dynamic_cast<TabButton*>(SD);
		SimpleDialog* RND_PREVIEW=GDS.Find("#RANDOM_PREVIEW");
		SD=GDS.Find("#SMALLRECT");
		ColoredBar* SMALLRECT=dynamic_cast<ColoredBar*>(SD);
		SD=GDS.Find("#RND_VALUE");
		InputBox* RND_VALUE=dynamic_cast<InputBox*>(SD);
		SD=GDS.Find("#LAND_TYPE");
		ComboBox* LANDTYPE=dynamic_cast<ComboBox*>(SD);
		SD=GDS.Find("#PRESETS");
		ComboBox* PRESET=dynamic_cast<ComboBox*>(SD);
		SD=GDS.Find("#PRESETNAME");
		InputBox* PRESETNAME=dynamic_cast<InputBox*>(SD);
		SimpleDialog* AREYOUSURE=GDS.Find("#AREYOUSURE");
		SimpleDialog* DESIGNEDCANVAS=GDS.Find("#DESIGNEDCANVAS");

		char cc[512]="5000";
		char NewPresName[512];
		sprintf(NewPresName,"NewPreset%04X",GetTickCount()&0xFFFF);
		if(PRESETNAME)PRESETNAME->Str=NewPresName;
		
		if(LANDTYPE){
			LANDTYPE->AddLine("Land");
			LANDTYPE->AddLine("Island");
		}
		char RND[32];
		sprintf(RND,"%d",(rand()+GetTickCount())&32767);
		if(RND_VALUE){
			RND_VALUE->Str=RND;
			RND_VALUE->StrMaxLen=20;
		}

		
		if(IB){
			IB->Str=cc;
			IB->StrMaxLen=32;
		}
		char* VC_IDS[64];
		int NVC=0;
		if(VICT){
			for(int i=0;i<3;i++){
				VICT->AddLine(GetTextByID(VicTypes[i]));
			}
		}
		if(MAPSIZE){
            MAPSIZE->AddComplexLine("Normal|Small");
		}
		if(NPLAY){
			for(int i=2;i<7;i++){
				char cc[32];
				sprintf(cc,"%d",i);
				NPLAY->AddLine(cc);
			}
		}		
		void FindAllPresets(ClassArray<_str>& Collector);
		ClassArray<_str> CL;
		FindAllPresets(CL);
		
		if(VICT&&MAPSIZE&&NPLAY&&LANDTYPE&&PRESET){
			FILE* F=fopen("UserTerrain\\Presets\\settings.dat","r");
			if(F){
				int res=5000;
				fscanf(F,"%d%d%d%d%d%d",&VICT->CurLine,&MAPSIZE->CurLine,&NPLAY->CurLine,&LANDTYPE->CurLine,&PRESET->CurLine,&res);
				sprintf(cc,"%d",res);
				fclose(F);
			}
		}

		do{
			if(TB_RANDOM){
				if(TB_RANDOM->State){
					if(PRESET){
						PRESET->CurLine=0;
						PRESET->Enabled=false;
					}
				}else{
					if(PRESET){
						PRESET->Enabled=true;
					}
				}
			}
			if(MLayers->Maps.GetAmount()){
				for(int i=0;i<MLayers->Maps.GetAmount();i++)MLayers->Maps[i]->Desc.Clear();
				int NR=TP->Relief.Rules.GetAmount();
				for(int i=0;i<NR;i++){
					_str* s=&MLayers->Maps[TP->Relief.Rules[i]->Layer.TopLayerIndex]->Desc;
					*s+=TP->Relief.Rules[i]->Description;
					*s+=" ";
				}
				for(int i=0;i<TP->Terr.GetAmount();i++){
					for(int j=0;j<TP->Terr[i]->Rules.GetAmount();j++){
						if(TP->Terr[i]->Rules[j]->Action.Mask.Enabled){
							_str* s=&MLayers->Maps[TP->Terr[i]->Rules[j]->Action.Mask.Map.TopLayerIndex]->Desc;
							*s+=TP->Terr[i]->Rules[j]->Description;
							*s+=" ";
						}
					}
				}
				for(int i=0;i<MLayers->Maps.GetAmount();i++){
					if(i<TP->PredefinedNamesOfLayers.GetAmount()){
						MLayers->Maps[i]->Desc=*TP->PredefinedNamesOfLayers[i];
					}else{
						if(i<TP->PredefinedNamesOfLayers.GetAmount()){
							MLayers->Maps[i]->Desc=*TP->PredefinedNamesOfLayers[i];
						}else{
							_str* s=&MLayers->Maps[i]->Desc;
							if(s&&s->pchar()){
								void Translit(char* s);
								Translit(s->pchar());
							}
						}						
					}
				}
			}
			if(PRESET){				
				if(PRESET->CurLine>=0&&PRESET->CurLine<CL.GetAmount()){
					char* n=CL[PRESET->CurLine]->pchar();
					if(n){
						char cc2[128];
						sprintf(cc2,"UserTerrain\\Presets\\%s",n);
						if(strcmp(cc2,CurPreset)){
							strcpy(CurPreset,cc2);
							xmlQuote xml;
							if(xml.ReadFromFile(CurPreset)){
								ErrorPager Error(0);
								TP->reset_class(TP);
								TP->Load(xml,TP,&Error);			
							}
						}						
					}
				}
			}
			GSets.CGame.ViewMask=0;
			void ProcessScreen();
			ProcessScreen();
			GSets.CGame.ViewMask=255;
			ProcessMessages();			
			GDS.ProcessDialogs();
			if(SMALLRECT&&TB_RANDOM){
				SMALLRECT->Visible=MAPSIZE->CurLine&&TB_RANDOM->State==false;
				SMALLRECT->Style=1;
			}
			GDS.RefreshView();
			if(ItemChoose==2){
				sprintf(RND,"%d",(rand()+GetTickCount())&32767);
				ItemChoose=-1;
			}
			if(ItemChoose==3){
				char NAME[32];
				sprintf(NAME,"RN0 %x %d0 ",atoi(RND),LANDTYPE->CurLine);
				int NP=NPLAY->CurLine+2;
				for(int i=0;i<NP;i++){
					strcat(NAME,"1");
				}
				for(int i=NP;i<8;i++){
					strcat(NAME,"0");
				}
				strcat(NAME,".m3d");
				GenerateRandomLayers(NAME);
				for(int i=0;i<MLayers->Maps.GetAmount();i++){
					BasePaintMap* M=MLayers->Maps[i];
					int tlx,tly;
					M->GetSize(tlx,tly);
					M->CreateTexturePart(0,0,tlx,tly);
				}
				ItemChoose=-1;
			}
			if(ItemChoose==10){//new preset
				//vital should insert dialog "Please input name for preset"
				char pname[128];
				strcpy(pname,NewPresName);
				if(!strstr(pname,".xml"))strcat(pname,".xml");
				//int id=GetTickCount()&65535;
				//sprintf(pname,"User_preset_%04X.xml",id);
				char pname2[128];				
				sprintf(pname2,"UserTerrain\\Presets\\%s",pname);
				
				IRM->CheckFileAccess(pname2);
				CopyFile(CurPreset,pname2,false);
				
				CL.reset_class(&CL);
				FindAllPresets(CL);
				if(PRESET){
					PRESET->Clear();
					for(int i=0;i<CL.GetAmount();i++){
                        PRESET->AddLine(CL[i]->pchar());
						if(!strcmp(CL[i]->pchar(),pname)){
							PRESET->CurLine=i;
						}
					}
				}
				ItemChoose=-1;
			}
			if(ItemChoose==11){//edit preset
				SimpleEditClass(CurPreset,TP,CurPreset,false);
				ItemChoose=-1;                
			}
			if(ItemChoose==12&&PRESET&&PRESET->NLines>1){//delete preset
                if(AREYOUSURE)AREYOUSURE->Visible=true;
                if(DESIGNEDCANVAS)DESIGNEDCANVAS->Enabled=false;
				ItemChoose=-1;
				UnPress();
				realLpressed=false;
			}
			if(ItemChoose==32){//cancel delete preset
				if(AREYOUSURE)AREYOUSURE->Visible=false;
				if(DESIGNEDCANVAS)DESIGNEDCANVAS->Enabled=true;
				ItemChoose=-1;
				UnPress();
				realLpressed=false;
			}
			if(ItemChoose==31&&PRESET&&PRESET->NLines>1){//delete preset after confirmation
				if(AREYOUSURE)AREYOUSURE->Visible=false;
				if(DESIGNEDCANVAS)DESIGNEDCANVAS->Enabled=true;
				realLpressed=false;
				//vital should insert dialog "Are you sure?"
				DeleteFile(CurPreset);
				CL.reset_class(&CL);
				FindAllPresets(CL);
				if(PRESET){
					PRESET->Clear();
					for(int i=0;i<CL.GetAmount();i++){
						PRESET->AddLine(CL[i]->pchar());						
					}
				}
				ItemChoose=-1;                
			}
			if(ItemChoose==13){
				for(int i=0;i<MLayers->Maps.GetAmount();i++){
					MLayers->Maps[i]->Clear();
				}
				ItemChoose=-1;
			}
		}while(ItemChoose==-1);
		if(VICT&&MAPSIZE&&NPLAY&&LANDTYPE&&PRESET){
			FILE* F=fopen("UserTerrain\\Presets\\settings.dat","w");
			if(F){
				fprintf(F,"%d %d %d %d %d %d",VICT->CurLine,MAPSIZE->CurLine,NPLAY->CurLine,LANDTYPE->CurLine,PRESET->CurLine,atoi(cc));				
				fclose(F);
			}
		}
		if(ItemChoose==17){//texturize
			if(TB_RANDOM&&TB_RANDOM->State==false&&NPLAY&&LANDTYPE){
				MLayers->WriteToFile("UserLandscape.xml");
				if(MAPSIZE){
					if(MAPSIZE->CurLine){
//						MaxMapX=480*10/15;
//						MaxMapY=MaxMapX;
					}else{
//						MaxMapX=480;
//						MaxMapY=MaxMapX;
					}                
				}
				TP->GenerateRelief=false;
				TP->GenerateTextures=true;
				void DeleteAllSprites();
				DeleteAllSprites();
				void DeleteAllUnits();
				DeleteAllUnits();
				TP->Generate();//generating designed map
			}
		}
		if(ItemChoose==1){//OK
			if(TB_RANDOM&&TB_RANDOM->State&&NPLAY&&LANDTYPE){
                char NAME[32];
				sprintf(NAME,"RN0 %x %d0 ",atoi(RND),LANDTYPE->CurLine);
				int NP=NPLAY->CurLine+2;
				for(int i=0;i<NP;i++){
					strcat(NAME,"1");
				}
				for(int i=NP;i<8;i++){
					strcat(NAME,"0");
				}
				PredefinedMapStyle=CurPreset;				
				void Load3DMap(char* Map);
				Load3DMap(NAME);
				PredefinedMapStyle=NULL;
			}else{
				MLayers->WriteToFile("UserLandscape.xml");
				if(MAPSIZE){
					if(MAPSIZE->CurLine){
//						MaxMapX=480*10/15;
//						MaxMapY=MaxMapX;
					}else{
//						MaxMapX=480;
//						MaxMapY=MaxMapX;
					}                
				}
				TP->GenerateRelief=true;
				TP->GenerateTextures=true;
				void DeleteAllSprites();
				DeleteAllSprites();
				void DeleteAllUnits();
				DeleteAllUnits();
				ActivateSurfTool("Generate");//TP->Generate();//generating designed map
			}
			int R=atoi(cc);
			for(int i=0;i<8;i++){//start resources
				MOptions.Players.Player[i].StartRes.Wood=R;
				MOptions.Players.Player[i].StartRes.Gold=R;
				MOptions.Players.Player[i].StartRes.Stone=R;
				MOptions.Players.Player[i].StartRes.Food=R;
				MOptions.Players.Player[i].StartRes.Iron=R;
				MOptions.Players.Player[i].StartRes.Coal=R;
			}
			//Victory conditions
			if(VICT){
				VictoryConditionRoot* VR=NULL;
				switch(VICT->CurLine+1) {
				case 1:
					VR=new VC_ScoreGame;
					((VC_ScoreGame*)VR)->TimeLimit=TimeLimits[0];
					break;
				case 2:
					VR=new VC_CaptureGame;
					((VC_CaptureGame*)VR)->TimeLimit=TimeLimits[0];
					break;		
				case 3:
					VR=new VC_AnnihilateHero;
					//((VC_AnnihilateHero*)VR)->TimeLimit=TimeLimits[TimeLimit];
					break;
				default:
					VR=new VC_DestroyTownHalls;
				}
				MOptions.VictoryRule.Set(VR);
			}
			//NPlayers
			if(NPLAY){
				MOptions.MaxPlayers=NPLAY->CurLine+2;
				MOptions.RandomizePlayersPositions=true;
			}
			void CreateLightMap();
			CreateLightMap();
			EraseAreas();	
			void CreateTotalLocking();
			CreateTotalLocking();						
			void ResearchIslands();
			ResearchIslands();
			void MakeAllDirtyGBUF();
			MakeAllDirtyGBUF();
			void ResetWaterHash();
			ResetWaterHash();
			void ResetGroundCache();
			ResetGroundCache();
			void CreateMiniMap();
			CreateMiniMap();
		}
	}
}
//Actions for map generator
MapRef MREF;
class ShowMLayers:public vui_Action{
public:
	ShowMLayers(){
		MR=NULL;
	}
	MapRef_editor* MR;	
	virtual void OnDraw(SimpleDialog* SD){
		if(SD->Enabled&&!SD->EditMode){
			GPS.SetCurrentDiffuse(0xFFFFFFFF);
			if(!MR){			
				MR=new MapRef_editor;
				ControlParams CParam;		
				CParam.LeftColumnWidth=4;
				CParam.AbsMinLeftWidth=0;
				MR->cTopIndex=0;
				MR->CreateControl(SD,0,0,SD->x1-SD->x,SD->y1-SD->y-20,&MREF,&MREF,"-nosquare",&CParam);
				MR->cTopIndex=0;
			}
			xmlQuote xml;
			MR->Get(&xml);			
		}
		if(MR)MR->Palet->Enabled=SD->Enabled;
	}
	SAVE(ShowMLayers);
		REG_PARENT(vui_Action);
	ENDSAVE;
};
class ShowMLayersNotEditable:public vui_Action{
public:
	ShowMLayers(){
		MR=NULL;
	}
	MapRef_editor* MR;	
	virtual void OnDraw(SimpleDialog* SD){
		if(SD->Enabled&&!SD->EditMode){
			GPS.SetCurrentDiffuse(0xFFFFFFFF);
			if(!MR){			
				MR=new MapRef_editor;
				ControlParams CParam;		
				CParam.LeftColumnWidth=4;
				CParam.AbsMinLeftWidth=0;
				MR->cTopIndex=0;
				MR->CreateControl(SD,0,0,SD->x1-SD->x,SD->y1-SD->y,&MREF,&MREF,"-nosquare -noedit defpalet 4 -nopalette",&CParam);
				MR->cTopIndex=0;
			}
			xmlQuote xml;
			MR->Get(&xml);
		}
	}
	SAVE(ShowMLayersNotEditable);
	REG_PARENT(vui_Action);
	ENDSAVE;
};
class SelectPenPadius:public vui_Action{
public:
	virtual bool isLeftClick(){ return true; };
	virtual bool LeftClick(SimpleDialog* SD){
		BaseClass* P=SD->GetParent();
		if(P){
			ClassArray<SimpleDialog>* SDP=dynamic_cast<ClassArray<SimpleDialog>*>(P);
			if(SDP){
				for(int i=0;i<SDP->GetAmount();i++){
					SimpleDialog* SDC=(*SDP)[i];
					if(SDC){
                        SDC->ID=0;
					}
				}
			}
		}
		SD->ID=1;
		MREF.PenRadius=min((SD->x1-SD->x)/2,(SD->y1-SD->y)/2)/64.0f;
		MREF.PenWidth=MREF.PenRadius/3.0f;
		return true;
	};
    virtual void OnDraw(SimpleDialog* SD){
		GPS.SetCurrentDiffuse(0xFFFFFFFF);
		DWORD C=SD->ID?0xFFFF0000:0xFF00FF00;
		void DrawCircle( float x, float y, float radius, DWORD fillColor, DWORD linesColor, int nSegments );
		int R=min((SD->x1-SD->x)/2,(SD->y1-SD->y)/2);
		for(float r=R;r>0&&r>R-4;r-=0.5)DrawCircle((SD->x+SD->x1)/2,(SD->y+SD->y1)/2,r,C,C,32);
		
	};
	SAVE(SelectPenPadius);
		REG_PARENT(vui_Action);
	ENDSAVE;
};
void FindAllPresets(ClassArray<_str>& Collector){
	WIN32_FIND_DATA FD;
	HANDLE H=FindFirstFile("UserTerrain\\Presets\\*.xml",&FD);
	if(H&&H!=INVALID_HANDLE_VALUE){
		do{
			_str* s=new _str;
			*s=FD.cFileName;
			Collector.Add(s);			
		}while(FindNextFile(H,&FD));
		FindClose(H);
	};
}
class ListOfPresets:public vui_Action{
public:
	virtual void OnDraw(SimpleDialog* SD){
		ComboBox* CB=dynamic_cast<ComboBox*>(SD);
		if(CB){
			if(CB->TestMode){
				CB->Clear();
				ClassArray<_str> CL;
                FindAllPresets(CL);
				for(int i=0;i<CL.GetAmount();i++){
					CB->AddLine(GetTextByID(CL[i]->pchar()));
				}
			}
		}
	}
	SAVE(ListOfPresets);
		REG_PARENT(vui_Action);
	ENDSAVE;
};
extern bool OptHidden;
class HideIfRightPanelActive:public vui_Action{
public:
	virtual void SetFrameState(SimpleDialog* SD){
		SD->Visible&=OptHidden;
	}
	SAVE(HideIfRightPanelActive);
		REG_PARENT(vui_Action);
	ENDSAVE;
};
void RegGeneratorClass(){	
	REG_CLASS(BasePaintMap);
	REG_CLASS(GreyMap);
	REG_CLASS(OneRandomLandscapeSample);
	REG_CLASS(OneRandomLandscapeType);
	REG_CLASS(OneRandomLayer);

	REG_CLASS(OneLayerCondition);
	REG_CLASS(_IsInsideTheLayer);
	REG_CLASS(_DeepInsideTheLayer);
	REG_CLASS(_IsOutsideTheLayer);
	REG_CLASS(_FarOutsideTheLayer);
	REG_CLASS(_IsOnBoundOfTheLayer);	
	REG_CLASS(_OR_conditions);
	REG_CLASS(_NOT_condition);

	REG_CLASS(OneGroundSample);
	REG_CLASS(OneReqPiece);
	REG_CLASS(OneStartPositionType);
	REG_CLASS(StartResourcesEx);	

	REG_CLASS_EX(ShowMLayers,"MapGenerator");
	REG_CLASS_EX(ShowMLayersNotEditable,"MapGenerator");	
	REG_CLASS_EX(SelectPenPadius,"MapGenerator");
	REG_CLASS_EX(ListOfPresets,"MapGenerator");
	REG_CLASS_EX(HideIfRightPanelActive,"Tools");
}
ExLandsScope* CurPreset=NULL;
void SetCurrentPreset(char* Name){
	char cc[128];
	sprintf(cc,"UserTerrain\\Presets\\%s",Name);
	if(CurPreset)delete(CurPreset);
	CurPreset=new ExLandsScope;
	if(!CurPreset->SafeReadFromFile(cc)){
		delete(CurPreset);
		CurPreset=NULL;
	}    
}
void SetTextureInPointUsingPreset(int Vertex,int Texture){
	
}
bool CheckIfPresetActive(){
	return CurPreset;
}