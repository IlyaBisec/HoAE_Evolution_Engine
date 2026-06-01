#pragma once
//#include "stdheader.h"
#ifndef __LOADSAVE_H__
#define __LOADSAVE_H__

class SaveBuf{
public:
	byte* Buf;
	int Pos;
	int Size;
	int RealSize;
	SaveBuf();
	void Init();
	~SaveBuf();
	void Clear();
	void SaveToFile(ResFile f1);
	void LoadFromFile(ResFile f1);
};

#ifndef DIALOGS_DIP_USER

void PreSaveGame(SaveBuf* SB,char* Messtr,int ID);
SFLB_DLLEXPORT SFLB_PreLoadGame(SaveBuf* SB,bool LoadNation);
SFLB_DLLEXPORT SFLB_LoadGame(char* fnm,bool LoadNation);
void SaveGame(char* Name,char* Messtr,int ID);
void xBlockRead(SaveBuf* SB,void* Data,int Size);
void xBlockWrite(SaveBuf* SB,void* Data,int Size);

#endif //DIALOGS_DIP_USER

/*
DLLEXPORT int GetDataForSave(byte** Data, byte AiNat);
DLLEXPORT void FreeSaveData(byte* Data);
DLLEXPORT void SetSaveData(byte* Data, int size, byte AiNat);
*/
#endif //_LOADSAVE_H__