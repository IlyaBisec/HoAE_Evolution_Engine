#include "stdheader.h"
#include "unitability.h"
#include "UnitAbility2.h"
#include "GameExtension.h"
#include "UnitAbilityExtension.h"
#include "UnitTypeGroup.h"

#include "GameInterface.h"
#include "mesh\unihash.h"
#include "FCompressor.h"
#include "MassivePlayer.h"

const int CellSize=256;
const int CellsVisibleRadius=4;
const int NumNeytralUnits=2000;
const int MaxUnitsPerPlayer=1000;

CEXPORT DWORD GetNatColor( int natIdx );

int LastDT=0;

void LOG(char* sz,...)
{
	return;

	char ach[256];
	va_list va;

	FILE* F=fopen("!mlog.log","a");
	if(F){
		va_start( va, sz );
		vfprintf ( F, sz, va );   
		va_end( va );
		fclose(F);
	}
}
#define LOGIT LOG("Mpl F: %s L:%d\n",__FUNCTION__,__LINE__)

extern CCommCore IPCORE;
extern FCompressor FCOMP;

OneObject* LoadOneObjectState(SaveBuf* SB);
void SaveOneObjectState(OneObject* OB,SaveBuf* SB);
void LoadOneObjectFullState(SaveBuf* SB);
void SaveOneObjectFullState(OneObject* OB,SaveBuf* SB);
void PseudoEraseUnit(pUnit u){
	OneObject* OB=u.object();
	if(OB){
		if(OB->NewBuilding && !u.GetPickUpAbility()){
			OB->NoSearchVictim=true;
			OB->Serial=0xFFFF;
			OB->ClearOrders();			
		}else u.Erase();
	}
}

void MassiveRoom::RefreshPeersList(){
    int n=IPCORE.GetPeersCount();
    for(int i=Players.GetAmount();i<n;i++){        
        MassivePlayer* mp=new MassivePlayer;
		mp->CellX=-1;
		mp->CellY=-1;
        Players.Add(mp);        
        DWORD id=IPCORE.GetPeerID(i);
        mp->Name=IPCORE.GetUserName(id);
        mp->ID=id;
        if(id==IPCORE.GetPeerID()){            
            mp->Connected=true;
            mp->ColorID=MyNation;
            mp->ColorID=IPCORE.GetPeerID(0);
            MyPlayerIndex=i;
            StartMyUnitsIndex=NumNeytralUnits+MyPlayerIndex*MaxUnitsPerPlayer;
			FinalMyUnitsIndex=StartMyUnitsIndex+MaxUnitsPerPlayer-1;
            AddSelectedToControlList(MyNation);
			for(int i=0;i<MAXOBJECT;i++){
				OneObject* OB=Group[i];
				if(OB && !OB->newMons->Immortal){
					if(!(i>=StartMyUnitsIndex && i<FinalMyUnitsIndex)){
						pUnit un;
						un.Set(WORD(i));
                        PseudoEraseUnit(un);
					}
				}
			}
        }
        SendMyCenter(i);
    }
}
void MassiveRoom::SendMyCenter(int PlayerIndex){
	MassivePlayer* me=Players[MyPlayerIndex];	
    MassivePlayer* mp=Players[PlayerIndex];	
	byte m=200;
	xBlockWrite(&mp->ToSend,&m,1);
	xBlockWrite(&mp->ToSend,&me->CenterX,2);
	xBlockWrite(&mp->ToSend,&me->CenterY,2);	
	xBlockWrite(&mp->ToSend,&me->CreepsKilled,2);
	xBlockWrite(&mp->ToSend,&me->HeroLevel,1);
	xBlockWrite(&mp->ToSend,&me->Money,4);	
}
CEXPORT bool ProcessWindowMessages();
inline bool ProcessMessages2();
extern bool NETWORK_INIT;
bool CreateMultiplaterInterface();
extern bool LoadAsIs;
bool MassiveRoom::JoinServer(const char* servername,const char* UserName,int Color,int HeroType){	
	if(Color!=-1)SetMyNation(Color);
	CreateMultiplaterInterface();
	if(!NETWORK_INIT){
		IPCORE.InitNetwork();
		NETWORK_INIT=1;
	};    
	LPSERVER_ENTRY pServers=NULL;
	IPCORE.StartServerPoll(servername);
	int T0=GetTickCount();
	DWORD dwNS=0;
	while(GetTickCount()-T0<4000){
		ProcessWindowMessages();
		ProcessMessages2();        
		dwNS=IPCORE.EnumServers(pServers);
		if(dwNS)break;
	}
	
	char* MapName=NULL;
	if(pServers){
		for(int i=0;i<dwNS;i++,pServers++){
			if(pServers->m_bAvailable){
				MapName=pServers->m_szSessionName;
				break;
			}
		}
	}
	if(MapName){
		LoadAsIs=true;
		GSets.CGame.StartSinglePlayerGame(MapName,MyNation);
		SetMyNation(Color);
		LoadAsIs=false;
		for(int i=0;i<200;i++){
			void PreDrawGameProcess();
			PreDrawGameProcess();
			void PostDrawGameProcess();
			PostDrawGameProcess();
		}
		SelectRandomHero(MyNation,HeroType);		
		IPCORE.InitClient(servername,UserName);
		int n=0;
		int T0=GetTickCount();
		while(GetTickCount()-T0<4000){
			ProcessWindowMessages();
			ProcessMessages2();        
			n=IPCORE.GetPeersCount();
			if(n)break;
		}
		if(n>0){
			IsActive=true;
			OwnNeytral=false;
			CreateGroupsEnumerator();
			RefreshPeersList();
			ProcessCenterPosition();
			CreatePlayersMap();
			GSets.CGame.PlaySinglePlayerGame();
			GSets.CGame.PostSinglePlayerGame();
		}
		return n!=0;
	}else return false;
}
void MassiveRoom::SelectRandomHero(int Color,int HeroType){
	SetMyNation(Color);
	DynArray<OneObject*> StartUnits;	
	for(WORD i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB){
			if(OB->NNUM==MyNation && DetectHero(OB)){
				StartUnits.Add(OB);
			}
		}
	}
	if(StartUnits.GetAmount()){
		OneObject*  u=StartUnits[rand()%StartUnits.GetAmount()];
		iVector2D v(u->RealX>>4,u->RealY>>4);
		pUnit pu(u);		
		pUnit un;
		if(HeroType!=-1){
			pu.Erase();
			pUnitType ut(HeroType);			
			un=GameInterface::CreateUnit(v.x,v.y,MyNation,ut,true);
		}else un=pu;
		pSelected::units(MyNation)+=un;
		pImSelected::units(MyNation)+=un;
	}
}
bool MassiveRoom::CreateServer(const char* PlayerName,const char* MapName,int Color,int HeroType){
	if(Color!=-1)SetMyNation(Color);
	if(MapName){
        LoadAsIs=true;
        GSets.CGame.StartSinglePlayerGame(MapName,MyNation);
		LoadAsIs=false;
		for(int i=0;i<200;i++){
			void PreDrawGameProcess();
			PreDrawGameProcess();
			void PostDrawGameProcess();
			PostDrawGameProcess();
		}		
		SelectRandomHero(MyNation,HeroType);		
	}
	CreateMultiplaterInterface();
	if(!NETWORK_INIT){
		IPCORE.InitNetwork();
		NETWORK_INIT=1;
	};    
	IPCORE.InitServer(GSets.CGame.cgi_CurrentMap,PlayerName);
    IsActive=true;
    MyPlayerIndex=0;
    MassivePlayer* mp=new MassivePlayer;
    Players.Add(mp);
    mp->Name=PlayerName;
    mp->Connected=true;
    mp->ColorID=MyNation;
    mp->ColorID=IPCORE.GetPeerID(0);
    StartMyUnitsIndex=NumNeytralUnits+MyPlayerIndex*MaxUnitsPerPlayer;
	FinalMyUnitsIndex=StartMyUnitsIndex+MaxUnitsPerPlayer-1;
	CreateGroupsEnumerator();
	OwnNeytral=true;
	pUnit p;
	for(int i=0;i<MAXOBJECT;i++){
		p.Set(WORD(i));
		if(p.DetectHero()){
			OneObject* OB=p.object();
			if(OB){
				if(OB->NNUM!=MyNation || !OB->Selected){
					p.Erase();//erasing other heroes
				}
			}
		}
	}
    AddSelectedToControlList(MyNation);
	ProcessCenterPosition();
	GSets.CGame.PlaySinglePlayerGame();
	GSets.CGame.PostSinglePlayerGame();
    return true;    
}
//must be called before executing commands buffer
void MassiveRoom::Process(){        
	if(Players.GetAmount()){
		CreatePlayersMap();	
		RefreshControlList();
		int TC=GetTickCount();
		static int T0=TC;
		if(TC-T0>100){
			RefreshPeersList();
		}
		CreateVisiblePlayersList();
		GatherCommands();
		ProcessReceive();    
		RefreshUnitsState();
		ProcessCenterPosition();
		SendAll();
	}
}

void MassiveRoom::CreateOneUnitData(int ID,int SN,SaveBuf* SB){
	OneObject* OB=Group[ID];
	if(OB){
        SaveOneObjectFullState(OB,SB);
	}
}
void MassiveRoom::ProcessUnitsRequest(SaveBuf* SB){
        
}
void MassiveRoom::AddSelectedToControlList(DWORD NI){    
    ForEachUnit(pSelected::units(NI),
        {
            pUnit u=unit;
            MakeUnitMine(u);
        }
    );    	
}
bool MassiveRoom::IsOwn(pUnit& u){
    int idx=u.index();
	if(OwnNeytral && idx<NumNeytralUnits)return true;
    return idx>=StartMyUnitsIndex && idx<=FinalMyUnitsIndex;
}
bool MassiveRoom::IsNeytral(pUnit& u){
    int idx=u.index();
    return idx<NumNeytralUnits;
}
bool MassiveRoom::IsOther(pUnit& u){
    int idx=u.index();
    return idx>=NumNeytralUnits && (idx<StartMyUnitsIndex || idx>FinalMyUnitsIndex);
}
int GetFreeIndexForUnit(int Subset){
    if(MassiveRoom::IsActive){
        if(Subset==-1){
            for(int i=0;i<NumNeytralUnits;i++){
                if(!Group[i]) return i;
            }
            return -1;
        }else{
			if(MassiveRoom::ActiveSubset!=-1 && MassiveRoom::ActiveSubset!=MassiveRoom::MyPlayerIndex)return -1;
            int s1=NumNeytralUnits+Subset*MaxUnitsPerPlayer;
            int s2=s1+MaxUnitsPerPlayer;
            for(int i=s1;i<s2;i++){
                if(!Group[i]) return i;
            }
            return -1;
        }
    }else{
        extern int LastAddUnit;
        extern bool EditMapMode;
	    if(!EditMapMode)LastAddUnit=0;
	    else if(rando()<128)LastAddUnit=0;
	    for(int i=LastAddUnit;i<MaxObj;i++)if(!Group[i])return i;
        return -1;
    }
}
int GetFreeIndexForUnit(){
	return GetFreeIndexForUnit(MassiveRoom::ActiveSubset);   
}
void SetActiveProducer(OneObject* OB){
	MassiveRoom::ActiveSubset=-1;
	if(OB && MassiveRoom::IsActive){
		int id=OB->Index;
		if(id>=NumNeytralUnits){
			MassiveRoom::ActiveSubset=(id-NumNeytralUnits)/MaxUnitsPerPlayer;
		}
	}
}
void SetActiveProducer(){    
    SetActiveProducer(GlobalUnitsContext::getCaster());
}
void SetMyActiveSubset(){
	if(MassiveRoom::IsActive){
		MassiveRoom::ActiveSubset=(MassiveRoom::StartMyUnitsIndex-NumNeytralUnits)/MaxUnitsPerPlayer;
	}
}
void SetDefaultActiveSubset(){
	MassiveRoom::ActiveSubset=-1;
}
bool IsMassiveGame(){
	return MassiveRoom::IsActive;
}
void ChangeUnitIndex(OneObject* OB,int dstindex){
	if(OB){
		Group[dstindex]=OB;	
		int id0=OB->Index;
		OB->Index=dstindex;
		OB->ImSelected=0;
		OB->Selected=0;	
		Group[id0]=NULL;	
		int NI=OB->NNUM;
		int n=NtNUnits[NI];
		word* NL=NatList[NI];
		for(int i=0;i<n;i++)if(NL[i]==id0)NL[i]=dstindex;
		MAXOBJECT=max(MAXOBJECT,WORD(dstindex+1));
		void ChangeHeroIndex(int srcindex,int dstindex);
		ChangeHeroIndex(id0,dstindex);
		for(int i=0;i<OB->GRP_N;i++){
			ClassRef<be_CGROUP> _group;
			_group.CPG_Index=OB->pGRP_IDS[i];
			be_CGROUP* gp=_group.Get();
			if(gp){
				int NU = gp->GetTotalAmount();
				if(NU){
					OneObject* pU = NULL;
					float HP = 0.f;
					int NN = 0;
					while (NU--) {
						if(gp->Units[NU].ID==id0){
							gp->Units[NU].ID=dstindex;
						}
					}
				}
			}
			_group.CPG_Index=0xFFFFFFFF;
		}
	}
}
bool MassiveRoom::MakeUnitMine(pUnit& unit){
	int idx=unit.ID;
    if(!(idx>=StartMyUnitsIndex && idx<=FinalMyUnitsIndex)){
		ActiveSubset=MyPlayerIndex;
        int id=GetFreeIndexForUnit(MyPlayerIndex);
		ActiveSubset=-1;
        if(id!=-1){
			ChangeUnitIndex(unit.object(),id);
            return true;
        }
    }
    return false;
}
void DisableUnit(pUnit& u){
	OneObject* OB=u.object();
	if(OB)OB->Disabled=true;
}
bool IfDisabled(pUnit& u){
	OneObject* OB=u.object();
	if(OB)return OB->Disabled;
	else return false;
}
bool IfImmortal(pUnit& u){
	OneObject* OB=u.object();
	return OB && OB->newMons->Immortal;
}
bool NotImmortal(pUnit& u){	
	return !IfImmortal(u);
}
void MassiveRoom::RefreshControlList(){	
	//my own & other players units should be erased if they are outside the visible range    
	MassivePlayer& me=*Players[MyPlayerIndex];
	int xmin=(me.CellX-CellsVisibleRadius)*CellSize;
	int xmax=(me.CellX+(CellsVisibleRadius+1))*CellSize;
	int ymin=(me.CellY-CellsVisibleRadius)*CellSize;
	int ymax=(me.CellY+(CellsVisibleRadius+1))*CellSize;    
	pUnit unit;
	/*
	for(int i=0;i<NumNeytralUnits;i++)if(Group[i]){
		unit.Set(WORD(i));
		iVector2D v=unit.GetCoor();
		if(v.x>=xmin || v.x<xmax || v.y>=ymin || v.y<ymax){
			MakeUnitMine(unit);			
		}        
	}
	*/
	//int UStart=NumNeytralUnits;
	//if(OwnNeytral)UStart=0;
	DynArray<int> UOwner;
	for(int i=0;i<=MAXOBJECT;i++)if(Group[i]){
		unit.Set(WORD(i));
		if(NotImmortal(unit)){
			iVector2D v=unit.GetCoor();
			OneObject* OB=unit.object();
			if(v.x<xmin || v.x>xmax || v.y<ymin || v.y>ymax){				
				if(OB && OB->Serial!=0xFFFF){
					if( (i>=StartMyUnitsIndex && i<=FinalMyUnitsIndex) || (OwnNeytral && i<NumNeytralUnits) ){				
						//searching for future owner of the unit					
						bi_DWORD dw(v.x/CellSize,v.y/CellSize);
						DWORD* own=OwnHistoryMap.get_last(dw);
						DWORD nextown=0;
						if(own)nextown=*own;
						if(nextown!=MyPlayerIndex){					
							MassivePlayer* mp=Players[nextown];						
							int c=201;
							xBlockWrite(&mp->ToSend,&c,1);
							SaveOneObjectFullState(unit.object(),&mp->ToSend);					
							c=206;
							xBlockWrite(&mp->ToSend,&c,1);
							c=unit.index();
							xBlockWrite(&mp->ToSend,&c,2);
							//unit.Erase();						
							PseudoEraseUnit(unit);
							i=MAXOBJECT;												
						}			
					}else{
						PseudoEraseUnit(unit);
						//unit.Erase();
					}
				}
			}else{				
				if(OB && OB->NewBuilding && OB->Serial==0xFFFF){
                    unit.Erase();
				}
			}
		}
	}
}
void MassiveRoom::ProcessReceive(){
    static BYTE* RecvBuf=new BYTE[RECV_BUFFER_LENGTH];
    PEER_ID pid;
    //WORD sz=IPCORE.ReceiveData(RecvBuf,&pid);
	for(int i=0;i<RecBuffer.GetAmount();i++){
		int sz=RecBuffer[i].Size;
		ReportReceive(sz);		
		LOG("Received %d\n",sz);
		pid=RecBuffer[i].Peer;
		memcpy(RecvBuf,RecBuffer[i].Data,sz);
		delete[]RecBuffer[i].Data;
		RecBuffer.Del(i,1);
		i--;
		if(sz>0){//decompressing data		
			int PlayerIdx=-1;
			for(int i=0;i<Players.GetAmount();i++){
				if(Players[i]->ID==pid){
					PlayerIdx=i;
					break;
				}
			}
			if(PlayerIdx!=-1){
				BYTE* dd=NULL;
				unsigned dLen=0;
				FCOMP.DecompressBlock(&dd,&dLen,RecvBuf);		
				char cc[512]="";
				sprintf(cc,"recv:%d->%d ",sz,dLen);			
				BYTE PrevSign=0;
				LastDT=-1;
				if(dd){
					int pos=0;
					do{
						BYTE Sign=dd[pos];
						if(strlen(cc)<100)sprintf(cc+strlen(cc),"[%d]",Sign);
						switch(Sign){
							case 200://Center coordinate have been changed
								HandleCenterChange(PlayerIdx,dd,pos,dLen);
								break;
							case 201://unit state changed
								HandleUnitStateChange(PlayerIdx,dd,pos,dLen);
								break;
							case 202://command received                            
								HandleCommand(PlayerIdx,dd,pos,dLen);
								break;
							case 203://unit death occured
								HandleUnitDeath(PlayerIdx,dd,pos,dLen);
								break;
							case 204://units presence mask
								HandleUnitsPresenceMask(PlayerIdx,dd,pos,dLen);
								break;
							case 205://money
								HandleMoney(PlayerIdx,dd,pos,dLen);
								break;
							case 206:
								HandleReOwn(PlayerIdx,dd,pos,dLen);
								break;
							case 207:
								HandleUnitErase(PlayerIdx,dd,pos,dLen);
								break;
							case 208:
								HandleChat(PlayerIdx,dd,pos,dLen);
								break;
							case 209:
								HandleBuildingsInfo(PlayerIdx,dd,pos,dLen);
								break;
							default:
								pos=dLen;
						}
						PrevSign=Sign;
					}while(pos<dLen);
					delete[](dd);
					AssignHint1(cc,250);
				}
				if(LastDT!=-1){
					Players[PlayerIdx]->LastDT=LastDT;
				}
			}
		}
	}
}
//byte Sign=0
//word CenterX
//word CenterY
//wore CreepsKilled
//byte HeroLevel
//DWORD Money
void MassiveRoom::HandleCenterChange(int PlayerIndex,BYTE* buf,int& pos,int BufLen){
    WORD cx=((WORD*)(buf+pos+1))[0];
    WORD cy=((WORD*)(buf+pos+1))[1];
	WORD ck=((WORD*)(buf+pos+1))[2];
	BYTE hl=buf[pos+1+6];
	DWORD mo=((DWORD*)(buf+pos+1+7))[0];

    MassivePlayer& me=*Players[PlayerIndex];
    me.CenterX=cx;
    me.CenterY=cy;
    me.CellX=cx/CellSize;
    me.CellY=cy/CellSize;
	me.CreepsKilled=ck;
	me.Money=mo;
	me.HeroLevel=hl;
    pos+=12;
}
void MassiveRoom::HandleChat(int PlayerIndex,BYTE* buf,int& pos,int BufLen){
	WORD L=((WORD*)(buf+pos+1))[0];
	AssignHint1((char*)buf+pos+3,250);
	pos+=L+3;
}
//byte Sign=1
//BlockData[]
void MassiveRoom::HandleUnitStateChange(int PlayerIndex,BYTE* buf,int& pos,int BufLen){
    SaveBuf SB;
    int data_sz=BufLen-pos;
    SB.Buf=(byte*)malloc(data_sz);
    SB.Size=data_sz;
    memcpy(SB.Buf,buf+pos+1,data_sz);
	SB.Pos=0;
    LoadOneObjectFullState(&SB);    
    pos+=1+SB.Pos;
}
//byte Sign=2
//word Lenglth
//CommandsData[]
extern byte ExBuf[8192];
extern int EBPos;
void MassiveRoom::HandleCommand(int PlayerIndex,BYTE* buf,int& pos,int BufLen){
    int data_sz=((WORD*)(buf+pos+1))[0];
    if(pos+data_sz+3<=BufLen){
        if(EBPos+data_sz<8192){
            memcpy(ExBuf+EBPos,buf+pos+3,data_sz);
            EBPos+=data_sz;
        }
    }
    pos+=3+data_sz;
}
//byte Sign=3
//word ID
//word SN
void MassiveRoom::HandleUnitDeath(int PlayerIndex,BYTE* buf,int& pos,int BufLen){
    word ID=((WORD*)(buf+pos+1))[0];
    word SN=((WORD*)(buf+pos+1))[1];
    pUnit u(ID,SN);
	word ID2=((WORD*)(buf+pos+1))[2];
	word SN2=((WORD*)(buf+pos+1))[3];
	pUnit uk(ID2,SN2);
	if(u.Valid()){
		OneObject* OB=u.object();
		if(!OB->Sdoxlo){
			OneObject* OBK=uk.object();	
			InDie=true;
			u.Die();
			if(uk.Valid())ext_OnUnitDie(OB,OBK);
			InDie=false;
		}
	}
    pos+=9;
}
void MassiveRoom::HandleUnitErase(int PlayerIndex,BYTE* buf,int& pos,int BufLen){
	word ID=((WORD*)(buf+pos+1))[0];
	word SN=((WORD*)(buf+pos+1))[1];
	pUnit u(ID,SN);
	InDie=true;	
	OneObject* OB=u.object();
	if(OB){
		OB->delay=6000;
		u.Die();
		OB->Sdoxlo=6000;
	}
	InDie=false;
	pos+=9;
}
//byte Sign=4
//byte nOctets
//byte UnitPrecemceMask[nOctets]
void MassiveRoom::HandleUnitsPresenceMask(int PlayerIndex,BYTE* buf,int& pos,int BufLen){
    int st=NumNeytralUnits+PlayerIndex*MaxUnitsPerPlayer;
    int n=int(buf[pos+1])*8;    
    pUnit u;
    for(int i=0;i<n;i++){
        if(!(buf[(i>>3)+pos+2]&(1<<(i&7)))){
            u.Set(WORD(st+i));
			if(u.Valid()){
				u.Erase();
			}
        }
    }
	for(int i=n;i<MaxUnitsPerPlayer;i++){
		u.Set(WORD(st+i));
		if(u.Valid()){
			u.Erase();
		}
	}
    pos+=2+buf[pos+1];
}
//byte Sign=5;
//byte Nation
//DWORD ResValue
void MassiveRoom::HandleMoney(int PlayerIndex,BYTE* buf,int& pos,int BufLen){
    DWORD R=*((DWORD*)(buf+pos+2));
    SetXRESRC(buf[pos+1],6,R);
    pos+=6;
}
void MassiveRoom::HandleReOwn(int PlayerIndex,BYTE* buf,int& pos,int BufLen){
	WORD R=*((WORD*)(buf+pos+1));	
    pUnit u;
	u.Set(WORD(R));
	OneObject* OB=u.object();
	if(OB){
		//OB->AI_Guard=true;
		OB->NoSearchVictim=false;
	}
	MakeUnitMine(u);	
	pos+=3;
}
bool _overlap(int px0,int py0,int px1,int py1,int mx0,int my0,int mx1,int my1){
    int xc1=px0+px1;
    int yc1=py0+py1;
    int xc2=mx0+mx1;
    int yc2=my0+my1;
    int Rx=px1-px0+mx1-mx0;    
    int Ry=py1-py0+my1-my0;
    int dx=abs(xc2-xc1);
    int dy=abs(yc2-yc1);
    return dx<=Rx && dy<=Ry;
}
//visible players are that one, who can look at least one of my units
void MassiveRoom::CreateVisiblePlayersList(){
    VisiblePlayers.FastClear();
    WellVisiblePlayers.FastClear();
    MassivePlayer* me=Players[MyPlayerIndex];
    int MyCX=me->CenterX;
    int MyCY=me->CenterY;    
    int xmin=1000000;
    int xmax=-1000000;
    int ymin=1000000;
    int ymax=-1000000;
    pUnit u;
    for(int i=StartMyUnitsIndex;i<=FinalMyUnitsIndex;i++){
        u.Set(WORD(i));        
        if(u.Valid()){
            iVector2D v=u.GetCoor();
            if(v.x>xmax)xmax=v.x;
            if(v.y>ymax)ymax=v.y;
            if(v.x<xmin)xmin=v.x;
            if(v.y<ymin)ymin=v.y;
        }
    }
    int terrX0=(me->CellX-CellsVisibleRadius)*CellSize;
    int terrY0=(me->CellY-CellsVisibleRadius)*CellSize;
    int terrX1=(me->CellX+1+CellsVisibleRadius)*CellSize;
    int terrY1=(me->CellY+1+CellsVisibleRadius)*CellSize;
    int N=Players.GetAmount();
    for(int i=0;i<N;i++)if(i!=MyPlayerIndex){
        MassivePlayer* mp=Players[i];
        int pX0=(mp->CellX-CellsVisibleRadius)*CellSize;
        int pY0=(mp->CellY-CellsVisibleRadius)*CellSize;
        int pX1=(mp->CellX+1+CellsVisibleRadius)*CellSize;
        int pY1=(mp->CellY+1+CellsVisibleRadius)*CellSize;        
        if(_overlap(terrX0,terrY0,terrX1,terrY1,pX0,pY0,pX1,pY1)){
            VisiblePlayers.Add(i);
            if(_overlap(xmin,ymin,xmax,ymax,pX0,pY0,pX1,pY1)){
                WellVisiblePlayers.Add(i);
            }
        }
    }
}
void MassiveRoom::GatherCommands(){
	if(EBPos>30000){
		for(int i=0;i<WellVisiblePlayers.GetAmount();i++){
			MassivePlayer* mp=Players[WellVisiblePlayers[i]];
			word t=202;
			xBlockWrite(&mp->ToSend,&t,1);
			t=EBPos;
			xBlockWrite(&mp->ToSend,&t,2);
			xBlockWrite(&mp->ToSend,&ExBuf[0],EBPos);
		}
	}
}
void MassiveRoom::CreatePlayersMap(){
	PlayersMap.reset();	
	for(int i=0;i<Players.GetAmount();i++){
		MassivePlayer* mp=Players[i];
		if( mp->CellX!=-1 &&mp->CellY!=-1 ){
			int cx0=mp->CellX-CellsVisibleRadius;
			int cx1=mp->CellX+CellsVisibleRadius+1;
			int cy0=mp->CellY-CellsVisibleRadius;
			int cy1=mp->CellY+CellsVisibleRadius+1;
			for(int x=cx0;x<=cx1;x++){
				for(int y=cy0;y<=cy1;y++){
					bi_DWORD dw(x,y);
					PlayersMap.add(dw,_DWORD(i));
					OwnHistoryMap.del_elm(dw,_DWORD(i));					
					OwnHistoryMap.add(dw,_DWORD(i));					
				}
			}
		}
	}
}
void MassiveRoom::FillOwnersList(int x,int y,DynArray<int>& Owners){		
	bi_DWORD bi(x/CellSize,y/CellSize);
	scan_key(PlayersMap,bi,DWORD* pid){
		Owners.Add(*pid);		
	}scan_end;
}
void MassiveRoom::RefreshUnitsState(){    
    pUnit u;
    bool UnitsInfoWasSent=false;
    int T=GetTickCount();
	int StartUnit=StartMyUnitsIndex;
	if(OwnNeytral)StartUnit=0;
	DynArray<int> WasSent;
	DynArray<int> UOwners;
    for(int i=StartUnit;i<=FinalMyUnitsIndex;i++)if(Group[i]){
		if(i==NumNeytralUnits)i=StartMyUnitsIndex;
        //checking state change
        u.Set(WORD(i));		
		if(NotImmortal(u) && u.SN!=0xFFFF){
			iVector2D v=u.GetCoor();
			SendState newst;
			OneObject* OB=u.object();
			HeroVariableStorage* HV=DetectHero(OB);
			newst.LiveStatus=OB?OB->GetLiveState():ls_Erased;
			newst.PrevCoor=u.GetCoor();
			newst.PrevLife=u.GetLife();
			newst.LocalOrder=OB->LocalOrder;
			newst.Level=HV?HV->Level:0;		
			UOwners.FastClear();
			FillOwnersList(v.x,v.y,UOwners);
			for(int i=0;i<UOwners.GetAmount();i++){
				int pid=UOwners[i];
				if(pid!=MyPlayerIndex){
					MassivePlayer* mp=Players[pid];				
					tri_DWORD TD(pid,u.ID,u.SN);
					SendState* st=LastSendInfo.get(TD);
					bool shouldsend=false;                        
					int DR=Norma(newst.PrevCoor.x-mp->CenterX,newst.PrevCoor.y-mp->CenterY);			
					if(DR<CellSize)DR=CellSize;
					//DR>>=4;
					DR=200;
					if(st){
						int DT=T-st->PrevSendTime;			
						if(st->LocalOrder!=newst.LocalOrder)shouldsend=true;
						else
						if(st->Level!=newst.Level)shouldsend=true;
						else
						if((HV && DT>4000) || (HV==NULL && DT>8000))shouldsend=true;
						else
						if(newst.LiveStatus!=st->LiveStatus)shouldsend=true;
						else{
							int dr=Norma(newst.PrevCoor.x-st->PrevCoor.x,newst.PrevCoor.y-st->PrevCoor.y);
							if(dr>DR)shouldsend=true;
							else{
								if(abs(newst.PrevLife-st->PrevLife)>DR)shouldsend=true;
							}
						}
					}else{
						shouldsend=true;
					}
					if(shouldsend){
						newst.PrevSendTime=st?T:T-100000;
						LastSendInfo.add_once(TD,newst);                
						int c=201;
						xBlockWrite(&mp->ToSend,&c,1);
						SaveOneObjectFullState(u.object(),&mp->ToSend);
						if(!WasSent.find(pid))WasSent.Add(pid);
						UnitsInfoWasSent=true;
					}				
				}
			}		
		}
    }
    if(UnitsInfoWasSent){
        byte tmp[256];
        memset(tmp,0,256);
        pUnit u;
        int sz=0;
        for(int i=0,j=StartMyUnitsIndex;j<=FinalMyUnitsIndex;i++,j++){
            u.Set(WORD(j));
            if(u.Valid()){
                sz=i>>3;
                tmp[2+sz]|=1<<(i&7);                
            }
        }
        tmp[0]=204;
        tmp[1]=sz+1;
        for(int pid=0;pid<WasSent.GetAmount();pid++){
            //int pid=WellVisiblePlayers[j];
            MassivePlayer* mp=Players[pid];
			/*
            if(mp->ToSend.Size>16){
                xBlockWrite(&mp->ToSend,&tmp[0],sz+3);                
                int s=205;
                xBlockWrite(&mp->ToSend,&s,1);
                s=MyNation;
                xBlockWrite(&mp->ToSend,&s,1);
                s=XRESRC(MyNation,6);
                xBlockWrite(&mp->ToSend,&s,4);
            }
			*/
        }
    }
}
void MassiveRoom::ProcessCenterPosition(){
    MassivePlayer* me=Players[MyPlayerIndex];
    int sx=0;
    int sy=0;
    int N=0;
    pUnit u;
    pUnit Central;
    for(int i=StartMyUnitsIndex;i<=FinalMyUnitsIndex;i++)if(Group[i]){
        u.Set(WORD(i));  
        OneObject* OB=u.object();
        if(OB){
            eLiveState LS=OB->GetLiveState();
            if(LS==ls_LiveBody){                
                iVector2D v=u.GetCoor();
                sx+=v.x;
                sy+=v.y;
                N++;
				if(u.DetectHero()){
					Central=u;
					break;
				}
            }
        }
    }
    if(Central.Valid()){
        iVector2D v=Central.GetCoor();
        sx=v.x;
        sy=v.y;
		HeroVariableStorage* HV=Central.DetectHero();
		OneObject* OB=Central.object();
		if(HV && OB){
			me->HeroLevel=Central.GetHeroLevel();				
			me->CreepsKilled=OB->Kills;
			me->Money=XRESRC(MyNation,6);
		}
    }else{
        if(N){
            sx=sx/N;
            sy=sy/N;
        }else return;
    }
    int cx=sx/CellSize;
    int cy=sy/CellSize;    
    me->CentralUnit=Central;
    if(me->CellX!=cx || me->CellY!=cy){
        me->CenterX=sx;
        me->CenterY=sy;
        me->CellX=cx;
        me->CellY=cy;
		MemoryBinStream MBS;
		GatherMyBuildingsInfo(&MBS);
        for(int i=0;i<Players.GetAmount();i++)if(i!=MyPlayerIndex){
            SendMyCenter(i);
			MassivePlayer* MP=Players[i];
			xBlockWrite(&MP->ToSend,MBS.GetData(),MBS.GetWritePos());
        }
    }
}
void MassiveRoom::SendAll(){
    for(int i=0;i<Players.GetAmount();i++){
        MassivePlayer* mp=Players[i];
        if(mp->ToSend.Size){
            unsigned char* dd=NULL;
		    unsigned dLen=0;
		    FCOMP.type=FCOMPRESSOR_TYPE_LZO;
		    FCOMP.CompressBlock(&dd,&dLen,(unsigned char*)mp->ToSend.Buf,mp->ToSend.Size);
			BYTE* b=new BYTE[dLen+4];
			memcpy(b+4,dd,dLen);
			*((DWORD*)b)='MASS';
            IPCORE.SendToPeer(mp->ID,b,dLen+4,false);
			ReportSend(dLen+4);
			mp->ToSend.Clear();
			delete[]b;
            free(dd);
        }
    }
}
extern CEXPORT char PlName[64];
bool MassiveRoom::OnCheatEntering(const char* Cheat){
    if(!strncmp(Cheat,"mcreate ",8)){
        CreateServer(Cheat+8);
        return true;
    }else
    if(!strncmp(Cheat,"mjoin ",6)){
        char sn[128];
        char plname[128];
        sscanf(Cheat+6,"%s%s",sn,plname);
        JoinServer(sn,plname);
        return true;
    }else 
	if(!strncmp(Cheat,"mscreate",8)){
		CreateServer(PlName,"Maps\\Default.m3d",0,-1);
		return true;
	}else
	if(!strncmp(Cheat,"msjoin ",6)){
		char sn[128];		
		sscanf(Cheat+6,"%s",sn);
		JoinServer(sn,PlName,1,-1);
		return true;
	}else 
    if(!strcmp(Cheat,"copyun")){
        SaveBuf SB;
        if(pSelected::units(0)._getamount()){
            pUnit u=pSelected::units(0)[0];
            SaveOneObjectFullState(u.object(),&SB);
            SB.Pos=0;
            u.Erase();
            LoadOneObjectFullState(&SB);
        }
	}else
	if(!strncmp(Cheat,"chero ",6)){
		if(IsActive){
			ChangeMyHeroType(atoi(Cheat+6));
		}		
	}else{
		if(IsActive){
			for(int i=0;i<Players.GetAmount();i++){
				MassivePlayer* MP=Players[i];
				int sp=208;
				xBlockWrite(&MP->ToSend,&sp,1);
				sp=strlen(Cheat);
				xBlockWrite(&MP->ToSend,&sp,2);
				xBlockWrite(&MP->ToSend,(void*)Cheat,sp+1);
			}
		}
	}
    return false;
}
void MassiveRoom::OnUnloading(){
    IsActive=false;
    StartMyUnitsIndex=0;
    FinalMyUnitsIndex=0xFFFF;      
    ActiveSubset=-1;
	BytesSent=0;
	FirstSendTime=0;
	BytesReceived=0;
	FirstReceiveTime=0;
}
bool MassiveRoom::IsActive=false;
int MassiveRoom::StartMyUnitsIndex=0;
int MassiveRoom::FinalMyUnitsIndex=0xFFFF;  
int MassiveRoom::ActiveSubset;
int MassiveRoom::MyPlayerIndex=0;
MassiveRoom* MRoom=NULL;
void ProcessMRoom(){
	if(MRoom){
		MRoom->Process();
	}
}
void InstallMassiveRoom(){
	MRoom=new MassiveRoom;
    InstallExtension(MRoom,"MassRoom");
}

//transferring object state

bool SaveActiveAbilitiesToBinStream(OneObject* OB,BinStream* bs);
bool LoadActiveAbilitiesFromBinStream(OneObject* OB,BinStream* BS);
void SaveModifieadChar(OneObject* OB,BinStream* BS);
void LoadModifiedChar(OneObject* OB,BinStream* BS);
OneObject* LoadOneObjectState(BinStream* BS);
extern int AnimTime;

void LoadOneObjectFullState(SaveBuf* SB){
	MemoryBinStream MBS;
	MBS.Write(SB->Buf+SB->Pos,SB->Size-SB->Pos);
    OneObject* OB=LoadOneObjectState(&MBS);	

	BYTE X=MBS.ReadBYTE();
	if(X!=234){
		X=234;
	}

	int TRUETIME=MBS.ReadDWORD();
	int ANIMTIME=MBS.ReadDWORD();	
	LoadModifiedChar(OB,&MBS);

	X=MBS.ReadBYTE();
	if(X!=123){
		X=123;
	}
	
	LoadActiveAbilitiesFromBinStream(OB,&MBS);

	X=MBS.ReadBYTE();
	if(X!=111){
		X=111;
	}

	//restoring abil's file/sprite ids
    if(OB->ActiveAbility){
		OB->ActiveAbility->CheckChest=false;
        int n=OB->ActiveAbility->ActiveAbilities.GetAmount();
        for(int i=0;i<n;i++){
            ActiveUnitAbility* AA=OB->ActiveAbility->ActiveAbilities[i];
            UnitAbility* UA=AA->GetA();
            if(UA){
                AA->FileID=UA->FileID;
                AA->SpriteID=UA->SpriteID;
            }
			int DT=TrueTime-TRUETIME;
			int DA=AnimTime-ANIMTIME;
			LastDT=DT;			
			AA->DeleteTime+=DT;
			AA->CastingTime+=DT;
			AA->eAuraStartTime+=DT;
			AA->BirthTime+=DT;
			AA->LastImpulseWeaponTime+=DT;
			AA->LastLifeShotTime+=DT;
			AA->LastManaShotTime+=DT;
			AA->EffectTime+=DT;
			AA->DieTime+=DT;
			AA->DamageTime+=DT;
			AA->HealTime+=DT;						
			HeroVariableStorage* HV=dynamic_cast<HeroVariableStorage*>(AA);
			if(HV){
				//int DTA=DT*6;
				HV->LastUpLifeRegenerationTime+=DA;
				HV->LastUpManaRegenerationTime+=DA;
			}
        }
		OB->ActiveAbility->TransparencyStartTime+=TrueTime-TRUETIME;		
    }
	//adding to natlist
	int NNUM=OB->NNUM;
	int N=NtNUnits[NNUM];
	int ID=OB->Index;
	word* IDS=NatList[NNUM];
	for(int i=0;i<N;i++){
		if(IDS[i]==ID){
			N=-1;
			break;
		}
	}
	if(N>=0){
		if(NtNUnits[NNUM]==NtMaxUnits[NNUM]){
			NtMaxUnits[NNUM]+=1024;
			NatList[NNUM]=(word*)realloc(NatList[NNUM],2*NtMaxUnits[NNUM]);
		};
		NatList[NNUM][NtNUnits[NNUM]]=ID;
		NtNUnits[NNUM]++;
	}
	if(ID>=MAXOBJECT)MAXOBJECT=ID+1;
    OB->NoSearchVictim=true;	
	void CheckIfHeroInstalled(OneObject* OB);
	if(DetectHero(OB))CheckIfHeroInstalled(OB);

	SB->Pos+=MBS.GetReadPos();
}
int HLevl=0;
void RemoveHints(xmlQuote* xml){
	HLevl++;
	static Enumerator* EFL=ENUM.Get("ABLFIELDS");
	int N0=EFL->GetAmount();

    int n=xml->GetNSubQuotes();
    for(int i=0;i<n;i++){
        xmlQuote* sxml=xml->GetSubQuote(i);
		const char* qn=sxml->GetQuoteName();		
        if(
            (!strcmp(qn,"Hint")) ||
            (!strcmp(qn,"SpriteID")) ||
            (!strcmp(qn,"FileID"))  ){

            xml->DelSubQuote(i);
            i--;
            n--;
        }else{
			if(EFL->Get((char*)qn)==-1){
				EFL->Add((char*)qn);
			}
            RemoveHints(sxml);
        }
    }
	HLevl--;
	if(HLevl==0 && N0!=EFL->GetAmount()){
        N0=EFL->GetAmount();
		FILE* F=fopen("ablfields.txt","w");
		if(F){			
			for(int i=0;i<N0;i++){
                fprintf(F,"%s\n",EFL->GetStr(i));
			}
			fclose(F);
		}
	}
}
#define WRITE_DEF(sz,x,y) if(x!=y){BS->Write##sz(x);FLG=BS->GetData()+wp;FLG[_pos>>3]|=1<<(_pos&7);}_pos++;
void SaveOneObjectState(OneObject* OB,BinStream* BS){	
	BS->WriteWORD(OB->Index);
	BS->WriteWORD(OB->Serial);
	BS->WriteDWORD(OB->UniqID);
	BS->WriteDWORD(OB->RealX);
	BS->WriteDWORD(OB->RealY);
	BS->WriteWORD(OB->NIndex);
	BS->WriteBYTE(OB->NNUM);
	int wp=BS->GetWritePos();
	BYTE* FLG=BS->GetData()+wp;
	int _pos=0;
	BS->WriteDWORD(0);//64-bit presence flags
	BS->WriteDWORD(0);	
	WRITE_DEF(WORD,OB->OverEarth,0);//0
	WRITE_DEF(WORD,OB->RZ,50);//1			
	WRITE_DEF(BYTE,OB->Selected,0);//2
	BS->WriteWORD(OB->RealDirPrecise);
	BS->WriteWORD(OB->MaxLife);
	WRITE_DEF(WORD,OB->Life,OB->MaxLife);//3
	WRITE_DEF(BYTE,OB->AddShield,0);//4
	WRITE_DEF(BYTE,OB->AddDamage,0);//5
	WRITE_DEF(WORD,OB->Sdoxlo,0);//6
	BS->WriteWORD(OB->CurrentFrameLong);	
	WRITE_DEF(BYTE,OB->NewCurSprite,(OB->CurrentFrameLong>>8));//7

	BS->WriteBYTE(OB->NewCurSpritePrev);	
	
	WRITE_DEF(BYTE,OB->NMask,NATIONS[OB->NNUM].NMask);//8
	WRITE_DEF(BYTE,OB->PathDelay,0);//9
	WRITE_DEF(WORD,OB->Mana,0);//10
	WRITE_DEF(WORD,OB->MaxMana,0);//11
	WRITE_DEF(WORD,OB->DstX,0);//12
	WRITE_DEF(WORD,OB->DstY,0);//13
	WRITE_DEF(WORD,OB->delay,0);//14
	WRITE_DEF(WORD,OB->MaxDelay,0);//15
	WRITE_DEF(WORD,OB->Stage,0);//16
	WRITE_DEF(WORD,OB->NStages,0);//17
	BS->WriteDWORD(OB->TotalPath);
	BS->WriteBYTE(OB->MathMask);
	WRITE_DEF(BYTE,OB->UnitSpeed,64);//18
	WRITE_DEF(BYTE,OB->GroupSpeed,OB->newMons->MotionDist);//19
	WRITE_DEF(WORD,OB->BirthNIndex,OB->NIndex);//20
	WRITE_DEF(WORD,OB->Phase,0);//21
	WRITE_DEF(WORD,OB->Guard,0xFFFF);//22
	WRITE_DEF(WORD,OB->LockedInBuilding,0xFFFF);//23
	WRITE_DEF(BYTE,OB->AddFarms,0);//24
	WRITE_DEF(BYTE,OB->DestDirection,0);//25
	WRITE_DEF(BYTE,OB->SingleUpgLevel,0);//26
	WRITE_DEF(BYTE,OB->RStage,0);//27
	WRITE_DEF(BYTE,OB->RAmount,0);//28
	WRITE_DEF(BYTE,OB->RType,0);//29
	WRITE_DEF(WORD,OB->NearBase,0xFFFF);//30
	WRITE_DEF(BYTE,OB->NewState,0);//31
	WRITE_DEF(BYTE,OB->LocalNewState,OB->NewState);//0
	WRITE_DEF(DWORD,OB->DestX,-1);//1
	WRITE_DEF(DWORD,OB->DestY,-1);//2
    WRITE_DEF(WORD,OB->EnemyID,0xFFFF);//3
	WRITE_DEF(WORD,OB->EnemySN,0xFFFF);//4
	WRITE_DEF(BYTE,OB->NZalp,0);//5
	WRITE_DEF(BYTE,OB->NMisses,0);//6
	WRITE_DEF(DWORD,OB->AttachedEffectMask,1);//7
	WRITE_DEF(WORD,OB->KineticPower,0);//8
	WRITE_DEF(WORD,OB->Kills,0);//9
	WRITE_DEF(WORD,OB->ObjectScale,256);//10
    WRITE_DEF(WORD,OB->NInside,0);//11
	WRITE_DEF(DWORD,OB->RealVx,0);//12
	WRITE_DEF(DWORD,OB->RealVy,0);//13
	WRITE_DEF(WORD,OB->ForceX,0);//14
	WRITE_DEF(WORD,OB->ForceY,0);//15
	WRITE_DEF(WORD,OB->CPdestX,0xFFFF);//16
	WRITE_DEF(WORD,OB->CPdestY,0xFFFF);//17
	WRITE_DEF(WORD,OB->NIPoints,0);//18
	word Cod1=OB->HiLayer?OB->HiLayer->Code:0xFFFF;
	WRITE_DEF(WORD,Cod1,0xFFFF);//19
	word Cod2=OB->NewAnm?OB->NewAnm->Code:0xFFFF;
	WRITE_DEF(WORD,Cod2,0xFFFF);//20
	word Cod3=OB->LoLayer?OB->LoLayer->Code:0xFFFF;
	WRITE_DEF(WORD,Cod3,0xFFFF);//21
	BYTE F=0;
	if(OB->AI_Guard)F|=1;
	WRITE_DEF(BYTE,F,0);//22	
	int pp;
	if(OB->GroupIndex)pp=div(int(OB->GroupIndex)-int(SelSet),sizeof SelGroup).quot;
	else pp=255;

	WRITE_DEF(BYTE,pp,255);//19

	if(OB->NInside)BS->Write(OB->Inside,OB->NInside*2);
	if(OB->NIPoints){
		BS->Write(OB->PathX,OB->NIPoints*2);
		BS->Write(OB->PathY,OB->NIPoints*2);
	}
	//saving orders
	pp=0;
	Order1* ORDR=OB->LocalOrder;
	while(ORDR){
		pp++;
		ORDR=ORDR->NextOrder;
	};
	WRITE_DEF(WORD,pp,0);
	if(pp){
		//2.saving order information
		Order1 ORD1;
		ORDR=OB->LocalOrder;
		ORD1=*OB->LocalOrder;
		while(ORDR){
			ORD1.NextOrder=NULL;
			int GetOrderKind(ReportFn* RF);
			ORD1.DoLink=(ReportFn*)(GetOrderKind(ORDR->DoLink));
			//assert(int(ORD1.DoLink)!=-1);
			BS->Write(&ORD1,sizeof ORD1);
			ORDR=ORDR->NextOrder;
			if(ORDR)ORD1=*ORDR;
		}
	}
	//groups info
	static Enumerator* E=ENUM.Get("MAPGRPLIST");
	
	BS->WriteBYTE(135);

	byte N=OB->GRP_N;
	BS->WriteBYTE(N);
	for(int i=0;i<N;i++){
		int p=E->FindVal(OB->pGRP_IDS[i]);
		BS->WriteBYTE(p);
	}

	BS->WriteBYTE(153);
}

#define READ_DEF(sz,x,v)if(FLG[_pos>>3]&(1<<(_pos&7))){x=BS->Read##sz();}else x=v;_pos++;
void InitFromNM(OneObject* G,NewMonster* NM,GeneralObject* GO,AdvCharacter* ADC);

OneObject* LoadOneObjectState(BinStream* BS){	
	WORD IND=BS->ReadWORD();
	WORD SN=BS->ReadWORD();
	WORD uniq=BS->ReadDWORD();
	int  RX=BS->ReadDWORD();
	int  RY=BS->ReadDWORD();
	int NIndex=BS->ReadWORD();

	OneObject* OB=new OneObject;
	if(Group[IND]){
		OneObject* OB=Group[IND];
		OB->CloseObject();
		free(OB);
	}
	OB->NNUM=BS->ReadBYTE();
	Group[IND]=OB;	
	OB->Index=IND;
	OB->Serial=SN;	
	OB->NIndex=NIndex;
	OB->RealX=RX;
	OB->RealY=RY;
	OB->UniqID=uniq;
	OB->CObjIndex=0xFFFF;

	GeneralObject* GO=NATIONS[OB->NNUM].Mon[OB->NIndex];
	NewMonster* NM=GO->newMons;

	OB->Ref.General=GO;
	OB->newMons=NM;
	OB->MoreCharacter=GO->MoreCharacter;
	OB->Nat=NATIONS+OB->NNUM;
	InitFromNM(OB,NM,GO,GO->MoreCharacter);
	OB->Serial=SN;

	OB->x=(OB->RealX-(OB->Lx<<7))>>8;
	OB->y=(OB->RealY-(OB->Lx<<7))>>8;

	int wp=BS->GetReadPos();
	BYTE* FLG=BS->GetData()+wp;
	int _pos=0;
	BS->ReadDWORD();//64-bit presence flags
	BS->ReadDWORD();
	READ_DEF(WORD,OB->OverEarth,0);//0	
	READ_DEF(WORD,OB->RZ,50);//1
	READ_DEF(BYTE,OB->Selected,0);//2
	OB->RealDirPrecise=BS->ReadWORD();
	OB->MaxLife=BS->ReadWORD();
	READ_DEF(WORD,OB->Life,OB->MaxLife);//3	
	READ_DEF(BYTE,OB->AddShield,0);//4	
	READ_DEF(BYTE,OB->AddDamage,0);//5	
	READ_DEF(WORD,OB->Sdoxlo,0);//6	
	OB->CurrentFrameLong=BS->ReadWORD();
	READ_DEF(BYTE,OB->NewCurSprite,(OB->CurrentFrameLong>>8));//7	
	OB->NewCurSpritePrev=BS->ReadBYTE();

	READ_DEF(BYTE,OB->NMask,NATIONS[OB->NNUM].NMask);//8
	READ_DEF(BYTE,OB->PathDelay,0);//9
	READ_DEF(WORD,OB->Mana,0);//10
	READ_DEF(WORD,OB->MaxMana,0);//11
	READ_DEF(WORD,OB->DstX,0);//12
	READ_DEF(WORD,OB->DstY,0);//13
	READ_DEF(WORD,OB->delay,0);//14
	READ_DEF(WORD,OB->MaxDelay,0);//15
	READ_DEF(WORD,OB->Stage,0);//16
	READ_DEF(WORD,OB->NStages,0);//17
	OB->TotalPath=BS->ReadDWORD();
	OB->MathMask=BS->ReadBYTE();
	READ_DEF(BYTE,OB->UnitSpeed,64);//18
	READ_DEF(BYTE,OB->GroupSpeed,OB->newMons->MotionDist);//19
	READ_DEF(WORD,OB->BirthNIndex,OB->NIndex);//20
	READ_DEF(WORD,OB->Phase,0);//21
	READ_DEF(WORD,OB->Guard,0xFFFF);//22
	READ_DEF(WORD,OB->LockedInBuilding,0xFFFF);//23
	READ_DEF(BYTE,OB->AddFarms,0);//24
	READ_DEF(BYTE,OB->DestDirection,0);//25
	READ_DEF(BYTE,OB->SingleUpgLevel,0);//26
	READ_DEF(BYTE,OB->RStage,0);//27
	READ_DEF(BYTE,OB->RAmount,0);//28
	READ_DEF(BYTE,OB->RType,0);//29
	READ_DEF(WORD,OB->NearBase,0xFFFF);//30
	READ_DEF(BYTE,OB->NewState,0);//31
	READ_DEF(BYTE,OB->LocalNewState,OB->NewState);//0
	READ_DEF(DWORD,OB->DestX,-1);//1
	READ_DEF(DWORD,OB->DestY,-1);//2
	READ_DEF(WORD,OB->EnemyID,0xFFFF);//3
	READ_DEF(WORD,OB->EnemySN,0xFFFF);//4
	READ_DEF(BYTE,OB->NZalp,0);//5
	READ_DEF(BYTE,OB->NMisses,0);//6
	READ_DEF(DWORD,OB->AttachedEffectMask,1);//7
	READ_DEF(WORD,OB->KineticPower,0);//8
	READ_DEF(WORD,OB->Kills,0);//9
	READ_DEF(WORD,OB->ObjectScale,256);//10
	READ_DEF(WORD,OB->NInside,0);//11
	READ_DEF(DWORD,OB->RealVx,0);//12
	READ_DEF(DWORD,OB->RealVy,0);//13
	READ_DEF(WORD,OB->ForceX,0);//14
	READ_DEF(WORD,OB->ForceY,0);//15
	READ_DEF(WORD,OB->CPdestX,0xFFFF);//16
	READ_DEF(WORD,OB->CPdestY,0xFFFF);//17
	READ_DEF(WORD,OB->NIPoints,0);//18	
	word Cod1,Cod2,Cod3;
	READ_DEF(WORD,Cod1,0xFFFF);
	READ_DEF(WORD,Cod2,0xFFFF);
	READ_DEF(WORD,Cod3,0xFFFF);
	if(Cod1!=0xFFFF)OB->HiLayer=NM->Animations->Get(Cod1);
	if(Cod2!=0xFFFF)OB->NewAnm=NM->Animations->Get(Cod2);
	if(Cod3!=0xFFFF)OB->LoLayer=NM->Animations->Get(Cod3);
	BYTE F=0;
	READ_DEF(BYTE,F,0);
	OB->AI_Guard|=(F&1)!=0;

	OB->RealDir=OB->GraphDir=OB->RealDirPrecise>>8;

	int pp;
	if(OB->GroupIndex)pp=div(int(OB->GroupIndex)-int(SelSet),sizeof SelGroup).quot;
	else pp=255;
	READ_DEF(BYTE,pp,255);//19
	if(pp!=255)OB->GroupIndex=SelSet+pp;
	else OB->GroupIndex=NULL;

	if(OB->NInside){
		OB->Inside=znew(word,OB->NInside);
		BS->Read(OB->Inside,OB->NInside*2);
	}
	if(OB->NIPoints){
		OB->PathX=(short*)FastGetMem(OB->NIPoints*2);
		OB->PathY=(short*)FastGetMem(OB->NIPoints*2);
		BS->Read(OB->PathX,OB->NIPoints*2);
		BS->Read(OB->PathY,OB->NIPoints*2);
	}
	//loading orders
	pp=0;
	READ_DEF(WORD,pp,0);//20
	Order1* PRE=NULL;
	Order1* OR1;
	for(int j=0;j<pp;j++){
		OR1=GetOrdBlock();
		if(!j)OB->LocalOrder=OR1;
		BS->Read(OR1,sizeof Order1);
		ReportFn* GetOrderRef(int i);
		OR1->DoLink=GetOrderRef(int(OR1->DoLink));
		if(OR1->DoLink){
			if(PRE)PRE->NextOrder=OR1;
			PRE=OR1;
		}else{
			if(j==0)OB->LocalOrder=NULL;//leak ..:(
		}
	}	
	//groups info

	int X=BS->ReadBYTE();
	if(X!=135){
        X=135;
	}


	static Enumerator* E=ENUM.Get("MAPGRPLIST");
    byte N=BS->ReadBYTE();
	if(N){
		OB->pGRP_IDS=new DWORD[N];
		OB->GRP_N=0;
		for(int i=0;i<N;i++){
			byte ID=BS->ReadBYTE();
			if(ID!=0xFF){
				DWORD p=E->GetVal(ID);
				OB->pGRP_IDS[OB->GRP_N]=p;
				ClassRef<be_CGROUP> _group;
				_group.CPG_Index=OB->pGRP_IDS[OB->GRP_N];
				be_CGROUP* gp=_group.Get();
				if(gp){					
					int NU = gp->GetTotalAmount();
					bool found=false;
					if(NU){
						OneObject* pU = NULL;
						float HP = 0.f;
						int NN = 0;
						while (NU--) {
							if(gp->Units[NU].ID==OB->Index){
                                found=true;
								gp->Units[NU].SN=OB->Serial;
								break;
							}
						}
					}
					if(!found){
						gp->Units.Add( OneUS(OB->Index,OB->Serial) );
					}
				}
				_group.CPG_Index=0xFFFFFFFF;
				OB->GRP_N++;
			}
		}
	}

	X=BS->ReadBYTE();
	if(X!=153){
		X=153;
	}

	return OB;
}

void SaveOneObjectFullState(OneObject* OB,SaveBuf* SB){
	MemoryBinStream MBS;
    SaveOneObjectState(OB,&MBS);

	MBS.WriteBYTE(234);

	MBS.WriteDWORD(TrueTime);
	MBS.WriteDWORD(AnimTime);

    //saving modification params    

	SaveModifieadChar(OB,&MBS);

	MBS.WriteBYTE(123);
	
    //saving active abilities

	SaveActiveAbilitiesToBinStream(OB,&MBS);

	MBS.WriteBYTE(111);

	xBlockWrite(SB,MBS.GetData(),MBS.Size());	

    //unsigned char* dd=NULL;
	//unsigned dLen=0;
	//FCOMP.type=FCOMPRESSOR_TYPE_LZO;
	//FCOMP.CompressBlock(&dd,&dLen,(unsigned char*)SB->Buf,SB->Size);    
    //free(dd);

}
void MassiveRoom::OnDrawOnMapOverAll(){
	return;
	int y=60;
	char cc[256];
	for(int i=0;i<2;i++){        
		sprintf(cc,"[%d] : ",NSL[i]);
		for(int j=0;j<NSL[i]&&j<5;j++){
			sprintf(cc+strlen(cc),"%d ",Selm[i][j]);
		}
        ShowString(120,y,cc,&WhiteFont);
		y+=22;
	}
	for(int i=0;i<Players.GetAmount();i++){ 
		MassivePlayer* mp=Players[i];
		int N=0;
		int st=NumNeytralUnits+i*MaxUnitsPerPlayer;
		sprintf(cc,"[%s] : Hero Level:%d   Kills: %d   Money:%d  DT=%.2fs cx=%d cy=%d ",mp->Name.pchar(),mp->HeroLevel,mp->CreepsKilled,mp->Money,float(mp->LastDT)/1000.0f,mp->CellX,mp->CellY);
		for(int i=0;i<MaxUnitsPerPlayer;i++){
			pUnit u;
			u.Set(WORD(i+st));
			if(u.Valid()){
				N++;
				/*if(N==1){
					int L=strlen(cc);
					if(L<200){
						sprintf(cc+L,"[%d:%s] ",i+st,u.object()->Ref.General->Message);
					}
				}*/
			}
		}
		sprintf(cc+strlen(cc)," Amount : %d",N);
		ShowString(120,y,cc,&WhiteFont);
		y+=22;
	}	
	if(GetKeyState(VK_CAPITAL)){
		int N=16400/CellSize;
		int xs=600;
		int ys=100;
		int xs1=xs+N*4+30;
		GPS.DrawRect(xs,ys,N*4,N*4,0xFFFF00FF00);
		GPS.DrawRect(xs1,ys,N*4,N*4,0xFFFF00FF00);
		for(int x=0;x<N;x++){
			for(int y=0;y<N;y++){            
				bi_DWORD bd=bi_DWORD(x,y);
				for(int i=0;i<8;i++){
					DWORD* ow1=PlayersMap.get(bd,i);
					if(ow1)GPS.DrawFillRect(xs+x*4,ys+y*4,3,3,(GetNatColor(*ow1)&0xFFFFFF)+0x40000000);
				}
				DWORD* ow2=OwnHistoryMap.get_last(bd);
				if(ow2)GPS.DrawFillRect(xs1+x*4,ys+y*4,3,3,GetNatColor(*ow2));
			}
		}
	}
	sprintf(cc,"Sent: %d (%.01f k/s)",BytesSent/1000,float(BytesSent)/(GetTickCount()-FirstSendTime));	
	ShowString(120,y,cc,&WhiteFont);
	y+=22;
	sprintf(cc,"Recv: %d (%.01f k/s)",BytesReceived/1000,float(BytesReceived)/(GetTickCount()-FirstReceiveTime));
	ShowString(120,y,cc,&WhiteFont);
	y+=22;
}
DynArray<DataPack> MassiveRoom::RecBuffer;	
void MassiveRoom::AddRec(BYTE* data,int sz,DWORD ID){
	DataPack dp;
	dp.Data=new BYTE[sz];
	dp.Peer=ID;
	memcpy(dp.Data,data,sz);	
	dp.Size=sz;
	RecBuffer.Add(dp);
}
void AddMassRecord(BYTE* Data,int sz,DWORD ID){
	MassiveRoom::AddRec(Data,sz,ID);
}
void MassiveRoom::ReportSend(int Size){
    BytesSent+=Size;
	if(FirstSendTime==0)FirstSendTime=GetTickCount();
}
void MassiveRoom::ReportReceive(int Size){
	BytesReceived+=Size;
	if(FirstReceiveTime==0)FirstReceiveTime=GetTickCount();
}
/// loading/saving abilities in very brief form ///

void SaveAbilitiesBriefly(OneObject* OB,SaveBuf* SB){

}
bool MassiveRoom::OnUnitDie(OneObject* Dead,OneObject* Killer){
	if(!InDie){
		for(int i=0;i<WellVisiblePlayers.GetAmount();i++){
			MassivePlayer* MP=Players[WellVisiblePlayers[i]];
			int p=Dead->delay==6000 ? 207 : 203;
			xBlockWrite(&MP->ToSend,&p,1);
			xBlockWrite(&MP->ToSend,&Dead->Index,2);
			xBlockWrite(&MP->ToSend,&Dead->Serial,2);
			if(Killer){
				xBlockWrite(&MP->ToSend,&Killer->Index,2);
				xBlockWrite(&MP->ToSend,&Killer->Serial,2);
			}else{
				int p=0xFFFF;
				xBlockWrite(&MP->ToSend,&p,2);
				xBlockWrite(&MP->ToSend,&p,2);
			}
		}
	}
	return true;
}
void MassiveRoom::CreateGroupsEnumerator(){
    Enumerator* E=ENUM.Get("MAPGRPLIST");
	E->Clear();
	int N=pGroup::amount();
	for(int i=0;i<N;i++){
		pGroup PG=pGroup::getgroup(i);
		be_CGROUP* G=PG.group();
		if(G){
			E->Add(G->Name.pchar(),G->GetObjectGlobalID());
		}
	}
}
void MassiveRoom::GatherMyBuildingsInfo(BinStream* BS){
	int uStart=OwnNeytral?0:StartMyUnitsIndex;
	pUnit p;
	int n=0;
	for(word i=uStart;i<=FinalMyUnitsIndex;i++){
        p.Set(i);
        OneObject* OB=p.object();
		if(OB && OB->NewBuilding && !OB->newMons->Immortal)n++;
        if(i==NumNeytralUnits-1)i=StartMyUnitsIndex-1;
	}
	uStart=OwnNeytral?0:StartMyUnitsIndex;
	BS->WriteBYTE(209);
	BS->WriteWORD(n);
	n=0;
	for(word i=uStart;i<=FinalMyUnitsIndex;i++){
		p.Set(i);
		OneObject* OB=p.object();
		if(OB && OB->NewBuilding && !OB->newMons->Immortal){
            BS->WriteWORD(OB->RealX>>4);
			BS->WriteWORD(OB->RealY>>4);
			BS->WriteWORD(OB->Life);
			BS->WriteBYTE(OB->NNUM);
		}
		if(i==NumNeytralUnits-1)i=StartMyUnitsIndex-1;
	}
}
void MassiveRoom::HandleBuildingsInfo(int PlayerIndex,BYTE* buf,int& pos,int BufLen){
    MemoryBinStream BS;
	BS.Write(buf+pos+1,BufLen-pos-1);
	int n=BS.ReadWORD();
	for(int i=0;i<n;i++){
		int  x=BS.ReadWORD();
		int  y=BS.ReadWORD();
		int  Life=BS.ReadWORD();
		byte NI=BS.ReadBYTE();
		for(int j=0;j<MAXOBJECT;j++){
			OneObject* OB=Group[j];
			if(OB){
				if((OB->RealX>>4)==x && (OB->RealY>>4)==y){
					OB->Life=Life;
					OB->NNUM=NI;
				}
			}
		}
	}
    pos+=1+BS.GetReadPos();
}
void MassiveRoom::ChangeMyHeroType(int HeroTypeIndex){
	MassivePlayer* MP=Players[MyPlayerIndex];
	if(MP->CentralUnit.Valid()){
		Enumerator* E=ENUM.Get("AllHeroEnum");
		if(HeroTypeIndex<E->GetAmount()){
			int ht=E->GetVal(HeroTypeIndex);
			OneObject* OB=MP->CentralUnit.object();
			if(OB){
				GeneralObject* GO=NATIONS[OB->NNUM].Mon[ht];
				if(!OB->ActiveAbility){
					OB->ActiveAbility = new UnitActiveAbilityList(OB);	
				}
				OB->ActiveAbility->BornNIndex=GO->NIndex;
				OB->ActiveAbility->BornNM=GO->newMons;
				OB->ActiveAbility->BornAC=GO->MoreCharacter;
			}
		}
	}
}