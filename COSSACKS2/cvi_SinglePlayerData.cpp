#include "stdheader.h"
#include ".\cvi_singleplayerdata.h"
#include ".\cvi_MainMenu.h"
//////////////////////////////////////////////////////////////////////////
cvi_SinglePlayerData vmSinglePlayerData;
const char* vmSinglePlayerDataXML="dialogs\\SinglePlayerData.xml";
cvi_PlayerData vmPlayerData;
//////////////////////////////////////////////////////////////////////////
SinglePlayerData_HeroAbilityInfo::SinglePlayerData_HeroAbilityInfo()
{
	Level=1;
	VirtualUp=false;
	AbIndex=-1;
}

SinglePlayerData_HeroInfo::SinglePlayerData_HeroInfo()
{
	//HeroType=-1;
	Level=1;
	Expa=0;
	WaitForAbilUp=false;
}
SinglePlayerData_HeroesInfoList::SinglePlayerData_HeroesInfoList()
{
}
SinglePlayerData_CampaignHeroInfo::SinglePlayerData_CampaignHeroInfo()
{
}

cvi_SinglePlayerData::cvi_SinglePlayerData(void)
{
}

cvi_SinglePlayerData::~cvi_SinglePlayerData(void)
{
}

void vmPlayerDataWrite(){
	vmPlayerData.Write();
};
void cvi_PlayerData::Write(){
	WriteCurAka();
	WriteToFile(vmSinglePlayerDataXML);
}

bool cvi_PlayerData::Read(){
	PlayerNick=GetTextByID("#NewPlayerName");
	/*
	if(ReadFromFile(vmSinglePlayerDataXML)){
		SetCurAka(CurAkaID);
		return true;
	}
	return false;
	*/
	ReadFromFile(vmSinglePlayerDataXML);
	SetCurAka(CurAkaID);
	return true;
}
//
void NormProfileName(char* s);
//
bool cvi_PlayerData::AddNewAka(const char* Name){
	cvi_SinglePlayerData* SPD=new cvi_SinglePlayerData;
	_str name;
	name=Name;
	NormProfileName(name.pchar());
	SPD->PlayerName=name.pchar();
	SPD->Difficulty=0;
	int n=vmCampaigns.GetAmount();
	for(int i=0;i<n;i++){
		SPD->NDoneMission.Add(0);
	}
	//
	SetCurAka(Aka.Add(SPD));
	return true;
};
bool cvi_PlayerData::DelAka(int ID){	
	return Aka.DelElement(ID);
};
int cvi_PlayerData::SetCurAka(int ID){
	if(ID>=0&&ID<Aka.GetAmount()){
		CurAkaID=ID;
	}else{
		if(Aka.GetAmount()==0){
			//AddNewAka(GetTextByID("#NewPlayerName"));
			return CurAkaID=-1;			
		}else{
			if(ID<0){
				CurAkaID=0;
			}else{
				CurAkaID=Aka.GetAmount()-1;
			}
		}
	}
	Aka[CurAkaID]->Copy(&vmSinglePlayerData,false);
	return CurAkaID;
};

void cvi_PlayerData::WriteCurAka(){
	if(CurAkaID<0||CurAkaID>=Aka.GetAmount()){		
		if(Aka.GetAmount()==0){
			return;
			/*
			cvi_SinglePlayerData* SPD=new cvi_SinglePlayerData;
			SPD->PlayerName=GetTextByID("#NewPlayerName");			
			Aka.Add(SPD);
			*/
		}
		CurAkaID=0;
	}

	// refresh hero dara
	void RefreshHeroCampaignData();
	RefreshHeroCampaignData();

	// copy data
	vmSinglePlayerData.Copy(Aka[CurAkaID],false);
};
// cva_SPD_DelAka
int curAka=-1;
bool cva_SPD_DelAka::LeftClick(SimpleDialog* SD){
	vmPlayerData.DelAka(curAka);
	return true;
}
// cva_SPD_AkaList
void cva_SPD_AkaList::Init(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD){
		LD->DSS.Clear();
		for(int i=0;i<vmPlayerData.Aka.GetAmount();i++){
			LD->AddElement(vmPlayerData.Aka[i]->PlayerName);
		}
		LD->CurrentElement=vmPlayerData.CurAkaID;
	}
}
void cva_SPD_AkaList::Accept(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD){
		LD->CurrentElement=vmPlayerData.SetCurAka(LD->CurrentElement);
	}
}
void cva_SPD_AkaList::SetFrameState(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD){
		int i=-1;
		int n=vmPlayerData.Aka.GetAmount();
		if(n>0){
			i=LD->CurrentElement;
			if(i<0) LD->CurrentElement=0;
			if(i>=n) LD->CurrentElement=n-1;
		}else{
			LD->CurrentElement=i;
		}
		curAka=LD->CurrentElement;
	}
}
// cva_SPD_AddAka
void cva_SPD_AddAka::SetFrameState(SimpleDialog* SD){	
	if(vmPlayerData.Aka.GetAmount()==0){
		SD->v_Actions.LeftClick(SD);
		/*
		if(vmPlayerData.SetCurAka(curAka)==-1){
			v_MainMenu.ModalDesk=BackModalDesk;
		}
		*/
	}
}	
cvi_PlayerData tempPD;
cvi_SinglePlayerData tempSPD;
bool cva_SPD_ChangeAka::LeftClick(SimpleDialog* SD){
	vmPlayerData.Copy(&tempPD,false);
	vmSinglePlayerData.Copy(&tempSPD,false);
	return true;
}
//cva_SPD_CancelAka
bool cva_SPD_CancelAka::LeftClick(SimpleDialog* SD){
	tempPD.Copy(&vmPlayerData,false);
	tempSPD.Copy(&vmSinglePlayerData,false);
	return true;
}
// cva_SPD_ToMainIfNotAka
bool cva_SPD_ToMainIfNotAka::LeftClick(SimpleDialog* SD){
	if(vmPlayerData.Aka.GetAmount()==0){
		v_MainMenu.ModalDesk=MainModalDesk;
	}
	return true;
}
// cva_SPD_MovieList
void cva_SPD_MovieList::Init(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	if(LD){
		LD->DSS.Clear();
		for(int i=0;i<vmSinglePlayerData.Movies.GetAmount();i++){
			cvi_mfMovie* M=vmSinglePlayerData.Movies[i]->Get();
			if(M) LD->AddElement(GetTextByID(M->Name.pchar()));
		}
		LD->CurrentElement=0;
	}
}
void cva_SPD_MovieList::Accept(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	int n=vmSinglePlayerData.Movies.GetAmount();
	if(LD&&LD->CurrentElement>=0&&LD->CurrentElement<n){
		cvi_mfMovie* M=vmSinglePlayerData.Movies[LD->CurrentElement]->Get();
		if(M) M->Play();		
	}
}
void cva_SPD_MovieList::SetFrameState(SimpleDialog* SD){
	ListDesk* LD=dynamic_cast<ListDesk*>(SD);
	SD->Enabled=LD&&LD->CurrentElement!=-1;
}
