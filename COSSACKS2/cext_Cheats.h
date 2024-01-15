#pragma once
//////////////////////////////////////////////////////////////////////////
CEXPORT	void StartAIEx(byte Nat,char* Name,int Land,int Money,int ResOnMap,int Difficulty);
//////////////////////////////////////////////////////////////////////////
class cext_Cheat_StartAI: public GameExtension{
public:
	virtual bool OnCheatEntering(const char* Cheat){
		if(strstr(Cheat,"startAI ")){
			_str name;
			name=Cheat;
			name.Replace("startAI ","AI\\");
			name.Replace(".ai","");
			name.Replace(".xml","");
			name+=".ai.xml";			
			StartAIEx(MyNation,name.pchar(),0,0,0,0);
			return true;
		}
		return false;
	}
};
