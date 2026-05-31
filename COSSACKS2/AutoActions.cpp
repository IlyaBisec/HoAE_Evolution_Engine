#include "stdheader.h"
#include "WeaponSystem.h"
#include "AutoActions.h"
void RegisterAutoActions(){
#include "AutoActionsregistration.h"
}
//
#include "cvi_MainMenu.h"
#include "VictoryConditions.h"
#include "CurrentMapOptions.h"
//
extern word NPlayers;
extern word COMPSTART[8];
//

//-------------- cva_GS_Player --------------//
byte gsPlayerNI=0;
PlayerInfo* gsPlayerI=NULL;
Nation* gsNT=NULL;
bool gsBuilding=false;
bool gsAI=false;
//
void cva_GS_Player::SetFrameState(SimpleDialog* SD){
	gsPlayerNI=0;
	gsAI=false;	
	byte color[2];
	memset(color,0xFF,sizeof color);
	int c=0;
	for(int pid=0;pid<7;pid++){		
		if(pid<NPlayers){
			gsPlayerI=GSets.CGame.PL_INFO+pid;
			gsPlayerNI=GSets.CGame.cgi_NatRefTBL[gsPlayerI->ColorID];
		}else
		if(COMPSTART[pid]){ 
			gsPlayerI=GSets.CGame.PL_INFO+pid;			
			gsPlayerNI=COMPSTART[pid]&15;
			gsAI=true;
		}else{
			break;
		}
		//
		bool add=true;
		for(int j=0;j<c;j++){
			if(color[j]==gsPlayerNI){
				add=false;
				break;				
			}
		}
		if(add&&c<2){
			color[c]=gsPlayerNI;
			if(SD->ID==c){
				break;
			}
			c++;
		}
	}
	gsNT=NATIONS+gsPlayerNI;
}
//---------- End of: cva_GS_Player ----------//


//-------------- cva_GS_Resource --------------//
int r[6]={0,3,2,1,4,5};
void cva_GS_Resource::SetFrameState(SimpleDialog* SD){
	int* Res=NULL;
	switch(SD->ID){
		case 0:		Res=gsNT->ResTotal;			break;
		case 1:		Res=gsNT->ResOnUpgrade;		break;
		case 2:		Res=gsNT->ResOnMines;		break;
		case 3:		Res=gsNT->ResOnUnits;		break;
		case 4:		Res=gsNT->ResOnBuildings;	break;
		case 5:		Res=gsNT->ResBuy;			break;
		case 6:		Res=gsNT->ResSell;			break;
		case 7:		Res=gsNT->ResOnLife;		break;
	};
	if(Res&&SD->DSS.GetAmount()>6){
		for(int i=0;i<6;i++){
			TextButton* TB=dynamic_cast<TextButton*>(SD->DSS[i+1]);
			if(TB){
				_str static txt;
				txt.Clear();
				txt=Res[r[i]];
				TB->SetMessage(txt.pchar());
			}
		}
	}
}
//---------- End of: cva_GS_Resource ----------//


//-------------- cva_GS_FormationDesk --------------//
int FormH=100;
void cva_GS_FormationDesk::SetFrameState(SimpleDialog* SD){
    if(SD->DSS.GetAmount()<2) return;
	int n=0;
	for(int i=0;i<gsNT->NMon;i++){
		if(gsNT->NBrProduced[i]||gsNT->NBrLost[i]){ //gsNT->NBrErased[i]||
			n++;
		}
	}
	//
	FormH=SD->DSS[0]->GetHeight()+SD->DSS[1]->GetHeight()*n;
	SD->SetHeight(FormH);
	FormH+=SD->Gety();
	//	
	for(int i=SD->DSS.GetAmount()-1;i<n;i++){
		DialogsDesk* DD=new DialogsDesk;
		SD->DSS.Add(DD);
		SD->DSS[1]->Copy(DD);
		DD->ID=i;
		DD->Sety(SD->DSS[1]->Gety()+SD->DSS[1]->GetHeight()*i);
	}	
}
//---------- End of: cva_GS_FormationDesk ----------//


//-------------- cva_GS_UnitsDesk --------------//
void cva_GS_UnitsDesk::SetFrameState(SimpleDialog* SD){
	if(SD->DSS.GetAmount()<2) return;	
	int n=0;
	for(int i=0;i<gsNT->NMon;i++){
		if(gsNT->NProduced[i]||gsNT->NKilled[i]){ //gsNT->NErased[i]||
			if(gsNT->Mon[i]->newMons->Building==Building){
				n++;
			}			
		}
	}
	gsBuilding=Building;	
	//
	int H=SD->DSS[0]->GetHeight()+SD->DSS[1]->GetHeight()*n;	
	SD->Sety(FormH);
	SD->SetHeight(H);
	if(!Building) FormH+=H;
	//	
	for(int i=SD->DSS.GetAmount()-1;i<n;i++){
		DialogsDesk* DD=new DialogsDesk;
		SD->DSS.Add(DD);
		SD->DSS[1]->Copy(DD);
		DD->ID=i;
		DD->Sety(SD->DSS[1]->Gety()+SD->DSS[1]->GetHeight()*i);
	}	
}
//---------- End of: cva_GS_UnitsDesk ----------//


//-------------- cva_GS_Formation --------------//
void cva_GS_Formation::SetFrameState(SimpleDialog* SD){
	if(SD->DSS.GetAmount()<4) return;
    word NIndex=0xFFFF;
	int id=0;
	for(int i=0;i<gsNT->NMon;i++){
		if(gsNT->NBrProduced[i]||gsNT->NBrLost[i]){ //gsNT->NBrErased[i]||
			if(SD->ID==id){
				NIndex=i;
				break;
			}
			id++;
		}
	}
	SD->Visible=false;
	if(NIndex!=0xFFFF){
		SD->Visible=true;
		TextButton* TB;
		_str txt;
		TB=dynamic_cast<TextButton*>(SD->DSS[0]);
		txt=gsNT->Mon[NIndex]->Message;
		TB->SetMessage(txt.pchar());
		TB=dynamic_cast<TextButton*>(SD->DSS[1]);
		txt=gsNT->NBrProduced[NIndex];
		TB->SetMessage(txt.pchar());
		TB=dynamic_cast<TextButton*>(SD->DSS[2]);
		txt=gsNT->NBrErased[NIndex];
		TB->SetMessage(txt.pchar());
		TB=dynamic_cast<TextButton*>(SD->DSS[3]);
		txt=gsNT->NBrLost[NIndex];
		TB->SetMessage(txt.pchar());	
	}
}
//---------- End of: cva_GS_Formation ----------//


//-------------- cva_GS_Unit --------------//
void cva_GS_Unit::SetFrameState(SimpleDialog* SD){
	if(SD->DSS.GetAmount()<4) return;
	word NIndex=0xFFFF;
	int id=0;
	for(int i=0;i<gsNT->NMon;i++){
		if(gsNT->NProduced[i]||gsNT->NKilled[i]){ //gsNT->NErased[i]||
			if(gsNT->Mon[i]->newMons->Building==gsBuilding){
				if(SD->ID==id){
					NIndex=i;
					break;
				}
				id++;
			}
		}
	}
	SD->Visible=false;
	if(NIndex!=0xFFFF){
		SD->Visible=true;
		TextButton* TB;
		_str txt;
		TB=dynamic_cast<TextButton*>(SD->DSS[0]);
		txt=gsNT->Mon[NIndex]->Message;
		TB->SetMessage(txt.pchar());
		TB=dynamic_cast<TextButton*>(SD->DSS[1]);
		txt=gsNT->NProduced[NIndex];
		TB->SetMessage(txt.pchar());
		TB=dynamic_cast<TextButton*>(SD->DSS[2]);
		txt=gsNT->NErased[NIndex];
		TB->SetMessage(txt.pchar());
		TB=dynamic_cast<TextButton*>(SD->DSS[3]);
		txt=gsNT->NKilled[NIndex];
		TB->SetMessage(txt.pchar());	
	}
}
//---------- End of: cva_GS_Unit ----------//


//-------------- cva_GS_PlayerName --------------//
void cva_GS_PlayerName::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB&&gsPlayerI){
		static _str txt;
		txt.Clear();
		char* state=NULL;
		if(NATIONS[gsPlayerNI].VictState==2){
			state=GetTextByID("#Winner");
		}else
		if(NATIONS[gsPlayerNI].VictState==1){
			state=GetTextByID("#Looser");
		}
		// find companion
		if(!gsAI){
			byte NI=GSets.CGame.cgi_NatRefTBL[gsPlayerI->ColorID];
			for(int i=0;i<NPlayers;i++){
				PlayerInfo* I=GSets.CGame.PL_INFO+i;
				if(I!=gsPlayerI&&NI==GSets.CGame.cgi_NatRefTBL[I->ColorID]){
					txt.print("%s, ",I->name);
					break;
				}
			}
		}
		//
		if(gsAI||!state){
			txt.print("%s",gsPlayerI->name);
		}else{
			txt.print("%s (%s)",gsPlayerI->name,state);
		}		
		//GlobalAI.Diff[CB->CurLine].Name		
		void LimitString(char* str,lpRLCFont FONT,int L);
		LimitString(txt.pchar(),TB->PassiveFont,395);
		TB->SetMessage(txt.pchar());
	}    
}
//---------- End of: cva_GS_PlayerName ----------//


//-------------- cva_GS_PlayerRace --------------//
void cva_GS_PlayerRace::SetFrameState(SimpleDialog* SD){
	TextButton* TB=dynamic_cast<TextButton*>(SD);
	if(TB&&gsPlayerI){
		//static _str txt;
		//txt.Clear();
		TB->SetMessage(GlobalAI.Ai[gsPlayerI->NationID].Message);
	}
}
//---------- End of: cva_GS_PlayerRace ----------//


//-------------- cva_GS_Desk --------------//
void cva_GS_Desk::SetFrameState(SimpleDialog* SD){
	VC_DestroyTownHalls* VC=dynamic_cast<VC_DestroyTownHalls*>(MOptions.VictoryRule.Get());
	if(VC){ //vGameMode==gmSin||vGameMode==gmMul
		SD->Visible=true;
	}else{
		SD->Visible=false;
		v_MainMenu.CloseDS();
	}
}
//---------- End of: cva_GS_Desk ----------//


//-------------- cva_OnMove --------------//
bool	cva_OnMove::LeftClick(SimpleDialog* SD){
    //insert there left click handler, it should return true if click handled
	if ( FindeObject(SD) ){
		mx0 = mouseX;
		my0 = mouseY;
	};
	return true;
};
extern bool realLpressed;
void	cva_OnMove::SetFrameState(SimpleDialog* SD){
	if (Move){
		ShiftDialog( m_pOBJ, mouseX-mx0, mouseY-my0 );
		mx0 = mouseX;
		my0 = mouseY;
	};
};
void	cva_OnMove::ShiftDialog(ParentFrame* pPF,int dx,int dy){
	if (pPF!=NULL) {
		pPF->ShiftDialog(dx,dy);
		int N=pPF->DSS.GetAmount();
		while (N--) {
			pPF->DSS[N]->ShiftDialog(dx,dy);
//			ShiftDialog(pPF->DSS[N],dx,dy);
		};
	};
};
void	cva_OnMove::OnDraw(SimpleDialog* SD){
    //insert there your own drawing function
	Move = (m_pOBJ!=NULL && SD->MouseOver && realLpressed);
	if ( !Move )	m_pOBJ=NULL;
};
bool	cva_OnMove::FindeObject(SimpleDialog* SD){
	// Finde global parent
	m_pOBJ = NULL;
	BaseClass*		pGPARENT = dynamic_cast<BaseClass*>(SD);
	do {
		m_pOBJ = dynamic_cast<SimpleDialog*>(pGPARENT);
        if ( m_pOBJ!=NULL ) {
			if ( UseName && ( m_name.pchar()==NULL || m_pOBJ->Name.pchar()==NULL || strcmp(m_name.pchar(),m_pOBJ->Name.pchar())!=0 ) ){
				m_pOBJ = NULL;
			};
			if ( !UseName && m_pOBJ->ID!=m_id ) {
				m_pOBJ = NULL;
			};
        };
		pGPARENT = pGPARENT->GetParent();
	} while( pGPARENT!=NULL && m_pOBJ==NULL );
	
	return (bool)( m_pOBJ != NULL );
};// finde object by NAME or ID ( use for LeftClick )
//---------- End of: cva_OnMove ----------//


//-------------- cva_CursorGP --------------//
void cva_CursorGP::OnDraw(SimpleDialog* SD){
	//insert there your own drawing function
	if ( SD->Active ){
		TempWindow TW;
		PushWindow(&TW);
		IRS->SetViewPort(Rct(0,0,RealLx,RealLy));
			GPS.FlushBatches();
			GPS.ShowGP(mouseX+10,mouseY-16,FileID,SpriteID,0);
		PopWindow(&TW);
	};
}
//---------- End of: cva_CursorGP ----------//


