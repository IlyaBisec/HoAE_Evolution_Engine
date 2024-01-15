#include "stdheader.h"
#include "GameExtension.h"
#include "CurrentMapOptions.h"
#include "Pinger.h"
int GetCurrentPlayerVictStatus(){
	return NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].VictState;
}
//
extern word NPlayers;
extern EXBUFFER EBufs[MaxPL];
extern word COMPSTART[8];
//
bool CheckForAlonePlayer(){
    //this checks if player stayed alone with his team only
	if(NPlayers>1&&!MOptions.DontCheckForAlonePlayer){	
		//checking for human opponents
		byte mask=NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].NMask;
		for(int i=0;i<NPlayers;i++)if(NATIONS[GSets.CGame.PL_INFO[i].ColorID].VictState==0)
			if(EBufs[i].Enabled&&!(mask&NATIONS[GSets.CGame.PL_INFO[i].ColorID].NMask))
				return false;
		//checking for AI opponents
		for(int i=0;i<7;i++)if(COMPSTART[i]){
			byte NI=COMPSTART[i]&15;
			if(NATIONS[NI].VictState==0&&!(NATIONS[NI].NMask&mask)){			
				return false;
			}
		}
		//there is no opponents
		return true;
	}else return false;
}
//class EndGameExtension:public GameExtension{
//public:
//	virtual void OnManualExit(byte Nation){
//		if(NATIONS[Nation].VictState!=2){
//			//if not victory
//			CmdEndGame(Nation,1,0);
//		}
//	}
//	virtual void OnGameEnd(){
//		
//	}
//};