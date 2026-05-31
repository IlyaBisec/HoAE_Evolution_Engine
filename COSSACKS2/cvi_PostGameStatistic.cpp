#include "stdheader.h"
#include "WeaponSystem.h"
#include ".\cvi_postgamestatistic.h"
#include ".\cvi_MainMenu.h"
#include "VictoryConditions.h"
#include "CurrentMapOptions.h"
//
#include "Dialogs\MISSIONSCRIPT\MissionScript.h"
#include "Dialogs\MISSIONSCRIPT\EndMissionData.h"
//#include "Dialogs\BigMapQuest\CQuest.h"				//Grey		[24.03.2004]
#include "Dialogs\ClasDeclaration.h"				//Grey		[06.08.2004]
#include "Dialogs\CPlayerSave.hpp"					//Grey.
//////////////////////////////////////////////////////////////////////////
cvi_PostGameStatistic vi_PGS[2];
//////////////////////////////////////////////////////////////////////////

cvi_PostGameStatistic::cvi_PostGameStatistic(void)
{
}

cvi_PostGameStatistic::~cvi_PostGameStatistic(void)
{
}

void cvi_PostGameStatistic::Create(char* PlayerName, Nation* Nat){
	Player=PlayerName;
	NT=Nat;
	Units.Clear();
	UnitsNIndex.Clear();
	for(int i=0;i<2048;i++){
		if(NT->NErased[i]||NT->NKilled[i]||NT->NProduced[i]){
			vsInfoArmy u;
			u.Killed=NT->NErased[i];
			u.Lost=NT->NKilled[i];
			u.Active=NT->NProduced[i];
			Units.Add(u);
			UnitsNIndex.Add(i);
		}
	}
};

//

void cva_DA_StatUnits::SetUnit(int ID, SimpleDialog* SD){
};
int cva_DA_StatUnits::GetNUnit(){
	return 0;
};

///////////////////////////////////////////
// EW2 Campaign Statistics
///////////////////////////////////////////

bool cva_CampStat_Show::LeftClick(SimpleDialog* SD){
	VC_DestroyTownHalls* VC=dynamic_cast<VC_DestroyTownHalls*>(MOptions.VictoryRule.Get());
	if(VC){
		//bool Draw_EW2_CampaignStats(SimpleDialog* SD);
		//Draw_EW2_CampaignStats(SD);
		v_MainMenu.StartDS(v_ISys.EW2_CampaignStats.pchar());
	}	
	return true;
}

//
extern CPlayerSAVE_STRUCT* CurPlayer;
//
int mode=0;
LinearArray<int,_int>* GetStatsArray(int P){
	if(CurPlayer&&CurPlayer->STATS.GetAmount()){ //P>0
		CStatsSAVE* Player=CurPlayer->STATS[P];
		if(Player){
			switch(mode){
				case 0: return &Player->SecNum;
				case 1: return &Player->ResSup;
				case 2: return &Player->ResAmo;
				case 3: return &Player->RecrtN;
				case 4: return &Player->ArmPow;
				case 5: return &Player->NatPow;
				case 6: return &Player->GenExp;
				case 7: return &Player->FKill;
				case 8: return &Player->FLost;
			};
		}
	}
	return NULL;
}
int GetMaxStats(){
	int m=0;
	for(int i=0;i<MAX_PLAYERS;i++){
		LinearArray<int,_int>* LA=GetStatsArray(i);
		if(LA&&LA->GetAmount()){
			if(mode){
				for(int j=0;j<LA->GetAmount();j++){
					if(m<(*LA)[j]){
						m=(*LA)[j];
					}
				}
			}else{
				m+=(*LA)[0];
			}
		}
	}
	//m=
	return m*11/10;
}
void cva_CampStat_Mode::SetFrameState(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){
		if(LastKey==VK_UP){
			CB->CurLine--;
			if(CB->CurLine<0)CB->CurLine=0;
			LastKey=0;
		}else
		if(LastKey==VK_DOWN){
			CB->CurLine++;
			if(CB->CurLine>=CB->NLines)CB->CurLine=CB->NLines-1;
			LastKey=0;
		}
		mode=CB->CurLine;
	}
}
void cva_CampStat_Mode::Init(SimpleDialog* SD){
	ComboBox* CB=dynamic_cast<ComboBox*>(SD);
	if(CB){
		CB->Clear();
		CB->AddLine(GetTextByID("#EW2_sectors_num"));
		CB->AddLine(GetTextByID("#EW2_supply"));
		CB->AddLine(GetTextByID("#EW2_resources_amount"));
		CB->AddLine(GetTextByID("#EW2_recruits_number"));
		CB->AddLine(GetTextByID("#EW2_power_of_army"));
		CB->AddLine(GetTextByID("#EW2_power_of_nation"));
		CB->AddLine(GetTextByID("#EW2_commander_exp_amount"));
		CB->AddLine(GetTextByID("#EW2_formation_killed"));
		CB->AddLine(GetTextByID("#EW2_formation_lost"));
		CB->CurLine=0;
	}
}
//
int	GetBigMapNationID(int NID,DWORD &MapColor);
DWORD GetNatColor( int natIdx );
//
int player=0;
//float dy=1.0;
int dy=1;
//
void cva_CampStat_Graphs::SetFrameState(SimpleDialog* SD){
	Canvas* C=dynamic_cast<Canvas*>(SD);
	if(!C) return;
	C->Clear();
	int Max=GetMaxStats();
	if(Max&&CurPlayer){
		if((Max%7)>0){
			Max=(Max/7+1)*7;
		}
		dy=Max/7;
		int W=C->GetWidth();
		int H=C->GetHeight();
		for(int j=0;j<MAX_PLAYERS;j++){
			DWORD mc;
			int c=GetBigMapNationID(j,mc);
			LinearArray<int,_int>* LA=GetStatsArray(c);
			if(LA){				
				DWORD col=mc|0xFF000000;
				int dx=23;
				if(dx*LA->GetAmount()>800){
					dx=800/LA->GetAmount();
				}
				int* xy_array=new int[LA->GetAmount()*2];
				int* xy=xy_array;
				for(int i=0;i<LA->GetAmount();i++){
					//C->AddLine((i-1)*dx, H-(*LA)[i-1]*H/Max, i*dx, H-(*LA)[i]*H/Max, col);
					xy[0]=i*dx;
					xy[1]=H-(*LA)[i]*H/Max;
					xy+=2;
				}
				C->AddThickLines(xy_array,LA->GetAmount(),5,col);
				delete(xy_array);
			}
		}
	}
}
void cva_CampStat_Oy::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB){
		static _str txt;
		if(dy<10.0){
			txt.Clear();
			//txt.print("%.1f",float(SD->ID+1)*dy);
			//txt.print("%d",float(SD->ID+1)*dy);
			txt.print("%d",(SD->ID+1)*dy);
		}else{
			int d=dy;
			if(dy>1000.0){
				d=d/50*50;
			}else
			if(dy>100.0){
				d=d/5*5;
			}
			txt=(SD->ID+1)*d;
		}		
		TB->SetMessage(txt.pchar());
	}
}
void cva_CampStat_Ox::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);	
	if(TB){
		LinearArray<int,_int>* LA=GetStatsArray(0);
		static _str txt;
		if(LA){
			int dx=23;
			if(dx*LA->GetAmount()>800){
				dx=800/LA->GetAmount();
			}
			int x=(SD->ID+1)*46/dx;
			txt=x;
		}else{
			txt=0;
		}
		TB->SetMessage(txt.pchar());
	}
}
void cva_CampStat_Player::SetFrameState(SimpleDialog* SD){
	player=SD->ID;
}
void cva_CampStat_Color::SetFrameState(SimpleDialog* SD){
	DWORD mc=0;
	GetBigMapNationID(player,mc);
	SD->Diffuse=mc|0xFF000000;
}
void cva_CampStat_PlFlag::SetFrameState(SimpleDialog* SD){	
	GPPicture* GP=dynamic_cast<GPPicture*>(SD);
	if(CurPlayer&&GP){
		DWORD mc;
		int nid=GetBigMapNationID(player,mc);
		GP->SetSpriteID(GlobalAI.Ai[player/*nid*/].NWaterAI);
	}
}
void cva_CampStat_PlName::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(CurPlayer&&TB){
		DWORD mc;
		TB->SetMessage(GlobalAI.Ai[player].Message);
		//TB->SetMessage(GlobalAI.Ai[GetBigMapNationID(player,mc)].Message);
	}
}
void cva_CampStat_PlScore::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(CurPlayer&&TB){
		DWORD mc;
		int nid=GetBigMapNationID(player,mc);
		LinearArray<int,_int>* LA=GetStatsArray(nid);
		if(LA&&LA->GetAmount()){
			static _str txt;
			txt=(*LA)[LA->GetAmount()-1];
			TB->SetMessage(txt.pchar());
		}else{
			TB->SetMessage("0");
		}
	}
}
