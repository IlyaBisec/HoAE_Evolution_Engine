#include "stdheader.h"
//#include "VideoLayer.h"
//#include "theovorb\theovorb.h"
#include "TVPlugin\TVPlugin.h"
#define NCOLR 4
VideoLayer::VideoLayer(){
	Texture=0;
	BM=NULL;
	Lx=0;
	Ly=0;
}
VideoLayer::~VideoLayer(){

}

#define TXSH 9
#define TXSZ (1<<TXSH)
#define FTXSZ float(1<<TXSH)
#define TXSZ1 (TXSZ-1)

const float c_TexelBias = -0.5f;
#define SetupVT(X,Y,U,V){\
	VT->u=float(U)/FTXSZ;\
	VT->v=float(V)/FTXSZ;\
	VT->x=X+c_TexelBias;\
	VT->y=Y+c_TexelBias;\
	VT->w=1.0;\
	VT->diffuse=Diffuse;\
	VT++;\
};
void RectCopy(byte* src,int sx,int sy,int sLx,int sLy,int sPitch,byte* dst,int dx,int dy,int dPitch){
	byte* sofs=src+sx+sy*sPitch;
	byte* dofs=dst+dx+dy*dPitch;
	for(int i=0;i<sLy;i++){
		memcpy(dofs,sofs,sLx);
		dofs+=dPitch;
		sofs+=sPitch;
	}
}
void VideoLayer::ShowRGB_Buffer(byte* src,int lx,int ly,int x,int y,int SizeX,DWORD Diffuse,float sx,float sy){
	if(!Texture){
		Texture=IRS->CreateTexture("BinkTempTexture",TXSZ,TXSZ,cfXRGB8888,1,tmpManaged);
	}
	if(true){//lx>127+256||ly>256-64){
		if(BM)delete(BM);
		BM=new BaseMesh;
		int Nx=(lx>>TXSH)+1;
		int Ny=(ly>>TXSH)+1;
		BM->create(4,6,vfVertexTnL);
		word* ids=BM->getIndices();
        ids[0]=0;
		ids[1]=1;
		ids[2]=2;
		ids[3]=1;
		ids[4]=2;
		ids[5]=3;
		BM->setNInd(6);
		BM->setNPri(2);
		BM->setNVert(4);
		BM->setTexture(Texture);
        
        static shVideo  = IRS->GetShaderID( "video" );
        static shHud    = IRS->GetShaderID( "hud" );
		if (sx > 1.05) BM->setShader( shVideo ); else BM->setShader( shHud );
		
        VertexTnL* V=(VertexTnL*)BM->getVertexData();
		for(int ix=0;ix<Nx;ix++){
			for(int iy=0;iy<Ny;iy++){
				VertexTnL* VT=V;
				int LX=ix==Nx-1?lx&TXSZ1:TXSZ;
				int LY=iy==Ny-1?ly&TXSZ1:TXSZ;
				if(LX||LY){
					SetupVT(x+float(ix*TXSZ)*sx,y+float(iy*TXSZ)*sy,0,0);
					SetupVT(x+float(ix*TXSZ+LX)*sx,y+float(iy*TXSZ)*sy,LX,0);
					SetupVT(x+float(ix*TXSZ)*sx,y+float(iy*TXSZ+LY)*sy,0,LY);
					SetupVT(x+float(ix*TXSZ+LX)*sx,y+float(iy*TXSZ+LY)*sy,LX,LY);
					int pitch;
					BYTE* bits=IRS->LockTexBits(Texture,pitch);
                    RectCopy(src,ix*TXSZ*NCOLR,iy*TXSZ,LX*NCOLR,LY,SizeX*NCOLR,bits,0,0,pitch);
					IRS->UnlockTexBits(Texture);
					//IRS->SaveTexture(Texture,"1.DDS");
					DrawBM(*BM);
				}
			}
		}
		delete(BM);
		BM=NULL;
		return;
	}
	if(lx!=Lx||ly!=Ly){
		delete(BM);
		BM=NULL;
		Lx=lx;
		Ly=ly;
	}
	
	if(!BM){
		BM=new BaseMesh;
		BM->create(12,18,vfVertexTnL);
		word* idxs=BM->getIndices();
		int t4=0;
		int t6=0;
		for(int t=0;t<3;t++){
            idxs[t6  ]=t4;
			idxs[t6+1]=t4+1;
			idxs[t6+2]=t4+2;
			idxs[t6+3]=t4+2;
			idxs[t6+4]=t4+1;
			idxs[t6+5]=t4+3;
			t6+=6;
			t4+=4;
		}
		BM->setNInd(18);
		BM->setNPri(6);
		BM->setNVert(12);
		BM->setTexture(Texture);
		BM->setShader(IRS->GetShaderID("hud"));
	}
	VertexTnL* VT=(VertexTnL*)BM->getVertexData();
	int hly=ly/2;
	int DX=lx>256?256:lx;

	SetupVT(x,y,0,0);
	SetupVT(x+DX,y,DX,0);
	SetupVT(x,y+ly,0,ly);
	SetupVT(x+DX,y+ly,DX,ly);
	if(lx>256){

		SetupVT(x+256,y,0,171);
		SetupVT(x+lx,y,lx-256,171);
		SetupVT(x+256,y+hly,0,170+hly);
		SetupVT(x+lx,y+hly,lx-256,170+hly);

		SetupVT(x+256,y+hly,128,171);
		SetupVT(x+lx,y+hly,128+lx-256,171);
		SetupVT(x+256,y+ly,128,170+hly);
		SetupVT(x+lx,y+ly,128+lx-256,170+hly);
		BM->setNInd(18);
		BM->setNPri(6);
		BM->setNVert(12);
	}else{
		BM->setNInd(6);
		BM->setNPri(2);
		BM->setNVert(4);
	}

	//copying data to texture
	int pitch;
	BYTE* bits=IRS->LockTexBits(Texture,pitch);
	const int nbytes=NCOLR;
	if(bits){
		RectCopy(src,  0,  0,512       ,   ly ,lx*2,bits,0  ,0  ,256*nbytes);
		if(lx>256){
			RectCopy(src,256*nbytes,  0,(lx-256)*nbytes,   hly,lx*nbytes,bits,0  ,170,256*nbytes);
			RectCopy(src,256*nbytes,hly,(lx-256)*nbytes,ly-hly,lx*nbytes,bits,256,170,256*nbytes);
		}
		IRS->UnlockTexBits(Texture);
		//IRS->SaveTexture(Texture,"tmp1.bmp");
	}
	DrawBM(*BM);
}
VideoLayer VL[2];
void ProcessVideo(){
	return;
}

extern City CITY[8];
DLLEXPORT int GetResource(byte Nat,byte ID);
DLLEXPORT int GetMyNation();
extern ActiveObjectIndexses  ActiveIndexses;
void EstimateFormationSituation(byte NI,word BrigID,FormSituation* S){
	if(BrigID>=CITY[NI].NBrigs)return;
	Brigade* BR=CITY[NI].Brigs+BrigID;
	memset(S,0,sizeof *S);
	word* IDS=BR->Memb;
	word* SNS=BR->MembSN;
	int N=BR->NMemb;
	S->FormID=BR->WarType-1;
	S->UnitID=BR->MembID;
	S->Panic=EngSettings.MoraleType==0&&BR->Morale<31*10000?10000:0;
	//S->Tired=BR->IsTired?10000:0;
	int NTotal=0;
	for(int i=NBPERSONAL;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
            OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				NewMonster* NM=OB->newMons;
				NTotal++;
				if(OB->Sdoxlo)S->NDying++;
				else if(OB->Attack&&OB->NewAnm->Code==anm_Attack+1)S->NShooters++;
				else if(OB->Attack&&OB->NewAnm->Code==anm_Attack)S->NMelee++;
				else if(OB->InMotion){
					if(OB->GroundState)S->NAttMoving++;
					else S->NMoving++;
				}else{ 
					if(OB->newMons->SlowRecharge){
						if(OB->NewAnm->Code==anm_PAttack||OB->NewAnm->Code==anm_PAttack+1||OB->NewAnm->Code==anm_PAttack+2)S->NPAttack++;
						else if(OB->NewAnm->Code==anm_UAttack||OB->NewAnm->Code==anm_UAttack+1||OB->NewAnm->Code==anm_UAttack+2)S->NUAttack++;
						else if(OB->NewState==4)S->NRecharging++;
						else if(OB->GroundState)S->NAttStanding++;
						else S->NStanding++;
					}else if(OB->GroundState)S->NAttStanding++;
					else S->NStanding++;
				}
				if((OB->Usage==StrelokID||OB->Usage==MentID)&&OB->delay==0)
					S->NFireReady++;

				S->Tired+=OB->GetTired;
			}
		}
	}
	S->IsStandGround=BR->InStandGround;
	S->Arsenal=GetResource(GetMyNation(),5);//COAL
	S->Morale=BR->Morale;//GEC
	S->Food=GetResource(GetMyNation(),1);//GEC
	Brigade* Enm = BR->GetNearesEnmBrigade();
	if(Enm)
	{
		int xx,yy;
		byte ddr;
		BR->GetCenter(&xx,&yy,&ddr);
		int xx1,yy1;
		byte ddr1;
		Enm->GetCenter(&xx1,&yy1,&ddr1);
		S->DistToMainTarget=Norma(xx1-xx,yy1-yy);
	}
	if(NTotal){
		for(i=0;i<maxPar;i++){
			S->Direct[i]=(S->Direct[i]*100)/NTotal;
		}
		S->Tired/=NTotal;
		S->Tired/=1000;
	}
}
int GetStrHash(char* id,int prefix){
    int L=strlen(id);
	int S=0;
	for(int i=0;i<L;i++){
		S+=int(id[i])*int(id[i+1]+1);
	}
	S=(prefix<<24)+(S&0xFFFFFF);
	return S;
}
char CurrentPlayingBik[256]="";
int GetFormConditionID(char* ID,int& param){
	char id[128];
	param=0;
	char* s;
	if(s=strchr(ID,'(')){
        param=atoi(s+1);
		s[0]=0;
		strcpy(id,ID);
	}else strcpy(id,ID);
	if(strstr(id,"ORDER:")){
		static char* OrdList[]={
			"ORDER:MOVE","ORDER:ATTMOVE","ORDER:RIFLE","ORDER:NORIFLE","ORDER:MELEE",
			"ORDER:GRENADE","ORDER:L1","ORDER:L2","ORDER:L3","ORDER:DL1",
			"ORDER:DL2","ORDER:DL3","ORDER:TOLINE","ORDER:TOSQUARE","ORDER:TOKARE",
			"ORDER:TOMOTION","ORDER:FIILFORM","ORDER:STANDGROUND",""
		};
        for(int q=0;OrdList[q][0];q++)if(!strcmp(OrdList[q],ID))return 8192+q;
        return -1;
	}else
	if(id[0]=='@'){
		return GetStrHash(id+1,0x11);
	}else
	if(id[0]=='*'){
		return GetStrHash(id+1,0x10);
	}else
	if(id[0]=='#'){
		//formation ID condition
		int cidx=-1;
		for(int p=0;p<NEOrders&&cidx==-1;p++){
			if(!strcmp(ElementaryOrders[p].ID,id))cidx=p;
		}
		if(cidx==-1)return -3;//unknown formation ID
		return 1024+cidx;
	}else if(!strcmp(id,"SHOOT"))return 0;
	else if(!strcmp(id,"MELEE"))return 1;
	else if(!strcmp(id,"DIE"))return 2;
	else if(!strcmp(id,"MOVE"))return 3;
	else if(!strcmp(id,"STAND"))return 4;
	else if(!strcmp(id,"ATTMOVE"))return 5;
	else if(!strcmp(id,"ATTSTAND"))return 6;
	else if(!strcmp(id,"PANIC"))return 7;
	else if(!strcmp(id,"TIRED"))return 8;
	else if(!strcmp(id,"RECHARGE"))return 9;
	else if(!strcmp(id,"PATTACK"))return 10;
	else if(!strcmp(id,"UATTACK"))return 11;
	else return -1;
}
bool CheckFormCondition(int ID,int param,void* Param){
    FormSituation* FS=(FormSituation*)Param;
	int pref=ID>>24;
	if(pref==0x11){
		int S=GetStrHash(CurrentPlayingBik,0x11);
		return S==ID;
	}else
	if(pref==0x10){
		char* id=NATIONS->Mon[FS->UnitID]->MonsterID;
		int S=GetStrHash(id,0x10);
		return S==ID;
	}else
	if(ID>8192){
		return FS->FinalStateOrder==ID-8192;
	}else
	if(ID>=1024){
		return FS->FormID==(ID-1024);
	}else{
		return FS->Direct[ID]>param;
	}
}
#include "Parser.h"
void LoadErr(char* sz,...);
OneTrigger VideoSeq;
OneTrigger VidOfSeq;
//==============================================================================================================//
#include "SuperEditor.h"
OneScript VideoSeqScript;
OneScript VidOfSeqScript;
//
OneScript SoundEvents;
//
void LoadVideoSeqScript(OneScript* OS, char* FileName)
{
	xmlQuote Xq;
	if(Xq.ReadFromFile(FileName))
	{
		ErrorPager Err;
		OS->reset_class(OS);
		if(OS->Load(Xq,OS,&Err))
		{
			int ff=0;
		}
	}
}
//==============================================================================================================//
void LoadVideoSeq(OneTrigger* OT, char* FileName){
	OT->~OneTrigger();
	ResFile F=RReset(FileName);
	if(F!=INVALID_HANDLE_VALUE){
		int sz=RFileSize(F);
		char* Data=znew(char,sz+1);
		RBlockRead(F,Data,sz);
		Data[sz]=0;
		int Line=1;
		int err=OT->ParseOneScope(Data,&GetFormConditionID,Line);
		if(err<0){
            char cc[128];
			LoadErr("%s, line %d : Parsing error : %s",FileName,Line,VideoSeq.ErrMessage);
		}
		free(Data); 
		RClose(F);
	}
}
char OneTrigger::ErrMessage[256];
#define MaxBiks 2
char CurrentBik[MaxBiks][256]={"",""};
int BikPlIndex[MaxBiks];
bool NoBreakBik[MaxBiks]={0,0};
bool GetSubCommand(char* src,int pos,char* dst){
	char* s=src;
	for(int i=0;i<pos;i++){
		if(i)s++;
		s=strchr(s,'&');
		if(!s)return false;
	}
	strcpy(dst,src);
	s=strchr(dst,'&');
	if(s)s[0]=0;
	s=strchr(dst,'+');
	if(s)s[0]=0;
}
void SmartLog(char* mask,...){	
	char cc[512];
	va_list args;
	va_start(args, mask);
	vsprintf(cc, mask, args);		
	va_end(args);
    static char* log=NULL;
	static int lsize=0;
	static int lmsize=0;
	int L=strlen(cc);
	if(log==NULL||!strstr(log,cc)){
        FILE* F=fopen("log.txt","a");
		if(F){
			SYSTEMTIME ST;
			GetLocalTime(&ST);
			fprintf(F,"%02d:%02d:%02d | %s\n",ST.wHour,ST.wMinute,ST.wSecond,cc);
			fclose(F);
			if(lsize+L>lmsize){
				lmsize+=L+4000;
				log=(char*)realloc(log,lmsize);
				log[lsize]=0;
				lsize+=L;                				
				strcat(log,cc);
			}
		}
	}
}
// �� ��������� :)
byte* Data=NULL;
int Lx,Ly;
bool TryShowBrigBinkFrame(int PlayerID){
	char dst[256];
	bool CurFinished=false;	
	Data=GetNextBinkFrame(Lx,Ly,PlayerID);
	CurFinished=false;
	if(!Data){
		CurFinished=true;
		if(GetSubCommand(CurrentBik[PlayerID],BikPlIndex[PlayerID]+1,dst)){
			PlayBink(dst,PlayerID);
			strcpy(CurrentPlayingBik,dst);
			Data=GetNextBinkFrame(Lx,Ly,PlayerID);
			if(Data){
				BikPlIndex[PlayerID]++;
				return false;
			}
		}
	}
	//
	char* ComList[64];
	int N=0;//=OT->GetCommand(&CheckFormCondition,&SelectedFormationSituation,&ComList,32);
	if(!N)
	{
		int GetVideoCommand(OneScript* OS,char** ComList,char* CurrentBik);
		if(PlayerID==0)
			N=GetVideoCommand(&VideoSeqScript,ComList,CurrentBik[0]);
		else
			if(PlayerID==1)
				N=GetVideoCommand(&VidOfSeqScript,ComList,CurrentBik[1]);
	}
	if(N){
		bool Break=1;
		if(Data){
			for(int i=0;i<N;i++)if(!strcmp(ComList[i],CurrentBik[PlayerID]))Break=0;
		}
		if(Break){
			int level=0;
			for(int i=0;i<N;i++){
				char* com=ComList[i];
				int L=0;
				char* s=strstr(com,"+level");
				if(s){
					L=atoi(s+6);
				}
				if(L>level)level=L;
			}
			int p=0;
			for(i=0;i<N;i++){
				char* com=ComList[i];
				int L=0;
				char* s=strstr(com,"+level");
				if(s)L=atoi(s+6);
				if(L>=level){
					ComList[p]=ComList[i];
					p++;
				}
			}
			N=p;
			if(N){
				N=rand()%N;
				char* com=ComList[N];
				if((!strstr(com,"+break"))&&(!CurFinished)&&NoBreakBik[PlayerID])goto SHOW;
				NoBreakBik[PlayerID]=strstr(com,"+nobreak")!=NULL;
				BikPlIndex[PlayerID]=0;
				char bik[64];				
				strcpy(CurrentBik[PlayerID],ComList[N]);
				strcpy(bik,ComList[N]);
				char* s0=bik;
				char* s=NULL;
				s=strchr(s0,'&');
				if(s)s[0]=0;
				s=strchr(s0,'+');
				if(s)s[0]=0;
				//GetSubCommand(CurrentBik[PlayerID],0,dst);				
				bool playr=PlayBink(bik,PlayerID);
				if(!playr)SmartLog("Video Player Error | Unable to open file <%s>",CurrentBik[PlayerID]);
				strcpy(CurrentPlayingBik,CurrentBik[PlayerID]);
				Data=GetNextBinkFrame(Lx,Ly,PlayerID);
			}
		}
	};
SHOW:	
	if(Data){		
		return true;
	}	
	return false;
}
void ShowBrigBinkFrame(int VideoX, int VideoY, int PlayerID){
	DWORD Color=0xFFFFFFFF;
	//GPS.FlushBatches();
	if(Data){
		VL[PlayerID].ShowRGB_Buffer(Data,Lx,Ly,VideoX,VideoY,Lx,Color);
	}	
	//GPS.ShowGP(VideoX+vdx,VideoY+vdy,BackGP,BackSprite,7);
}
//
FormSituation SelectedFormationSituation;
void ProcessVideoForBrigade(OneTrigger* OT,byte NI,word BrigID,int Action,int BackGP,int BackSprite,int VideoX,int VideoY, int vdx, int vdy,int PlayerID){
	//
	//EstimateFormationSituation(NI,BrigID,&SelectedFormationSituation);
	EstimateFormationSituation(NI,BrigID,&ActiveIndexses.FormationSituation);	
	//
	return;
	//	
	ActiveIndexses.FormationSituation.FinalStateOrder=Action;
	//SelectedFormationSituation.FinalStateOrder=Action;
	if(GSets.DisableVideo) return;
	if(!TryShowBrigBinkFrame(PlayerID)) return;
	ShowBrigBinkFrame(VideoX, VideoY, PlayerID);	
}

BinkPlayer::BinkPlayer(){
	Ready=0;
	Bink=0;
	OutBuffer=NULL;
	Lx=0;
	Ly=0;
}
BinkPlayer::~BinkPlayer(){
	if(Bink){
		Close();
	}
}
void BinkPlayer::Close(){
	if(Bink)BinkClose(Bink);
	Bink=0;
}
void BinkPlayer::SetupBuffer(int wx,int wy){
	if(wx!=Lx||wy!=Ly){
		Lx=wx;
		Ly=wy;
		OutBuffer=(byte*)realloc(OutBuffer,Lx*Ly*NCOLR);
	}
}
bool BinkPlayer::Open(char* Name){
	if(Bink)BinkClose(Bink);
	Bink=0;
	Ready=0;
	BinkSoundUseDirectSound(0);
	unsigned long P[2];
	P[0]=0;
	P[1]=LocSettings.LanguageSetForBikVideo;
	BinkSetSoundTrack(P[1]?2:1,&P[0]);
    Bink=BinkOpen(Name,BINKNOSKIP|BINKSNDTRACK);
	if(Bink){
		SetupBuffer(Bink->Width,Bink->Height);
        strcpy(FileName,Name);
		Ready=1;
		int V=((GSets.SVOpt.SoundVolume*GSets.SVOpt.SoundVolume)/100)*32768/100;
		BinkSetVolume(Bink,P[0],V);
		if(P[1])BinkSetVolume(Bink,P[1],V);
		return true;
	}
	return false;
}
bool BinkPlayer::GetCurrentFrame(){
	if(Ready){
		if ( Bink->FrameNum>= Bink->Frames-2)return false;
		if(!BinkWait(Bink)){
			s32 res0=BinkDoFrame( Bink );
			s32 res=BinkCopyToBuffer( Bink,OutBuffer,Lx*NCOLR,Ly,0,0,BINKSURFACE32);
			BinkNextFrame(Bink);
		}
		return true;
	}
	return false;
}
#define NBPlayer 2
BinkPlayer BPlayer[NBPlayer];
bool PlayBink(char* Name,int PlayerID){
	if(PlayerID>=NBPlayer) return false;
	return BPlayer[PlayerID].Open(Name);
}
byte* GetNextBinkFrame(int& Lx,int& Ly,int PlayerID){
	BinkPlayer* BP=BPlayer+PlayerID;
	if(BP->Ready){
		Lx=BP->Lx;
		Ly=BP->Ly;
		if(BP->GetCurrentFrame()){
			return BP->OutBuffer;
		}	else return NULL;
	}else return NULL;
}
//
void SetCDVolumeEx(int Vol);
int GetCDVolume();
CEXPORT int GetCurPtr();
DialogsSystem* GetShowAboutDS();
//
extern bool KeyPressed;
extern bool Lpressed;
bool IsInVideo=false;
bool FullscreenVideoCredits=false;

#include "comms\comms.h"
#include "HTML\html.h"

// ManOwaR :: Special "dummy" function for StarForce
__declspec(dllexport) int PrepareVideoPlay( const char * pcszPlayFile , char * pszVideoFile , char * pszAudioFile )
{
	char szTemp[ MAX_PATH ] , *pszTemp;
	FILE *pfh = NULL;

	strcpy( pszVideoFile , pcszPlayFile );

	if ( ( pszTemp = strrchr( pszVideoFile , '\\' ) ) )
		*( pszTemp + 1 ) = '\0';
	else
		pszVideoFile[0] = '\0';
	
	strcpy( pszAudioFile , pszVideoFile );

	pfh = fopen( pcszPlayFile , "rb" );

	if ( ! pfh )
		return FALSE;

	if ( ! fgets( szTemp , MAX_PATH , pfh ) ){
		fclose( pfh );
		return FALSE;
	}

	if ( ( pszTemp = strchr( szTemp , '\r' ) ) )
		*pszTemp = '\0';

	if ( ( pszTemp = strchr( szTemp , '\n' ) ) )
		*pszTemp = '\0';

	strcat( pszVideoFile , szTemp );

	if ( ! fgets( szTemp , MAX_PATH , pfh ) ){
		fclose( pfh );
		return FALSE;
	}

	if ( ( pszTemp = strchr( szTemp , '\r' ) ) )
		*pszTemp = '\0';

	if ( ( pszTemp = strchr( szTemp , '\n' ) ) )
		*pszTemp = '\0';

	strcat( pszAudioFile , szTemp );
	
	fclose( pfh );

	return TRUE;
}

void StopPlayingMP3();
void PlayRandomTrack();

void PlayFullscreenVideo( char* name , float px , float py , bool Skipable = true )
{
	extern byte PlayGameMode; // ugly

	char szVideoName[ MAX_PATH ] , szAudioName[ MAX_PATH ];
	int curs , vol;
	TV_AH _ah = NULL;
	TV_VH _vh = NULL;
	DWORD dwKey = 0;

	if( ! PrepareVideoPlay( name , szVideoName , szAudioName ) )
		return;

	int mPlay=GSets.SVOpt.MusicPlayMode;
	GSets.SVOpt.MusicPlayMode=0;
	StopPlayingMP3();

	bool play=true;

	_ah = tv_StreamOpen( szAudioName );
	if( ! _ah )
		play=false;
		//return;

	_vh = tv_VideoOpen( szVideoName );
	if ( ! _vh ){
		//tv_StreamClose( _ah );
		play=false;
		//return;
	}	

	int v=GSets.SVOpt.SoundVolume*GSets.SVOpt.RelativeVideoToSoundVolume;
	tv_StreamSetVolume( _ah , v );

	IsInVideo = true;
	curs = GetCurPtr();
	SetCurPtr( 15 );

	IRS->ClearDevice();
	FlipPages();

	// To skip if inactive BEFORE playing:
	extern HWND g_hWnd;
	if( g_hWnd != GetForegroundWindow() ){
		play=false;
		//return;
	}

	PlayGameMode = 2;

	if(play){

		tv_StreamPlay( _ah );
		tv_VideoPlay( _vh , FALSE , NULL );

		Lpressed = false;
		KeyPressed = false;

		do {
			//ProcessMessages();

			// To skip if inactive DURING playing:
			if(g_hWnd != GetForegroundWindow()) {
				break;
			}

			if ( tv_VideoFinished( _vh ) && tv_StreamFinished( _ah ) )
				break;
			else {
				dwKey = ( /*( GetAsyncKeyState( VK_LBUTTON )	& 0x8000 ) || 
						  ( GetAsyncKeyState( VK_RBUTTON )	& 0x8000 ) ||*/ 
						  ( GetAsyncKeyState( VK_ESCAPE )		& 0x8000 ) /*|| 
						  ( GetAsyncKeyState( VK_SPACE )		& 0x8000 )*/ );
			}
			Sleep( 100 );

		} while( ! ( dwKey && Skipable ) );
	}

	if(_ah)
		tv_StreamClose( _ah );
	if(_vh)
		tv_VideoClose( _vh );

	IRS->ClearDevice();
	FlipPages();

	PlayGameMode = 0;

	//SetCDVolumeEx( vol );
	if(mPlay){
		GSets.SVOpt.MusicPlayMode=mPlay;
		StopPlayingMP3();
		PlayRandomTrack();
	}

	SetCurPtr( curs );
	IsInVideo = false;
	FullscreenVideoCredits = false;
	LastKey = 0;
	KeyPressed = false;
	void UnPress();
	UnPress();
}

CEXPORT void ShowRGBBuff(byte* Buf,int x,int y,int Lx,int Ly,int SizeX,int xs,int ys){
	static VideoLayer VL;
	VL.ShowRGB_Buffer(Buf+x*3+y*SizeX*3,Lx,Ly,xs,ys,SizeX,0xFFFFFFFF,1.0f,1.0f);
}

// ----------------------------------------------------------------------------------------------------------------------