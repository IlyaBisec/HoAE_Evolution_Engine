///////////////////////////////////////////////////////////
// CDIRSND.H -- Header file for the CDirSound class.
///////////////////////////////////////////////////////////

#ifndef __CDIRSND_H
#define __CDIRSND_H

//#include <afxwin.h>
#include <windows.h>
#include "cwave.h"
#include "dsound.h"
#include <mmsystem.h>

#define MAXSND 4096 
#define MAXSND1 (MAXSND+1)
class CDirSound
{
public:
    LPDIRECTSOUND m_pDirectSoundObj;
    HWND m_hWindow;
	char* m_SoundNames[MAXSND1];	
	DWORD SoundFreq[MAXSND1];
    LPDIRECTSOUNDBUFFER m_bufferPointers[MAXSND1];	
    DWORD m_bufferSizes[MAXSND1];
	int RiseUpTime[MAXSND1];
	int FallDnTime[MAXSND1];
	int LastHitTime[MAXSND1];
	bool LoopedSound[MAXSND1];
	byte SoundCtg[MAXSND1];
	byte SoundForCtg[MAXSND1];
	byte IndexInGroup[MAXSND1];
	int SoundCtgFreq[256];
	int CurrSoundCtgFreq[256];
	WORD  CtgSoundID[256][16];
	byte  CtgNSounds[16];
	byte  StartGroupFreq[256];
	byte  FinalGroupFreq[256];
	int LastDecTime;

	short Volume[MAXSND1];
    short Pan[MAXSND1];
	short SrcX[MAXSND1];
	short SrcY[MAXSND1];
	byte  BufIsRun[MAXSND1];
	UINT m_currentBufferNum;
	void CDirSound::CreateDirSound(HWND hWnd);
    ~CDirSound();
	void ValidateSoundBuffer(UINT bufferNum);
	UINT AddNewSound(const char* name);
    void CreateSoundBuffer(CWave* pWave,UINT bufferNum);
	void DuplicateSoundBuffer(UINT bufferNum,UINT srcBuffer);
    BOOL DirectSoundOK();
	void SetLastVolume(short Vol){
		Volume[m_currentBufferNum]=Vol;
	};
    BOOL CopyWaveToBuffer(CWave* pWave, UINT bufferNum);
	void SetVolume(UINT bufferNum,int vol);
	void SetPan(UINT bufferNum,int pan);
    BOOL PlaySound(UINT bufferNum,DWORD Flags);
    BOOL StopSound(UINT bufferNum);
	BOOL PlayCoorSound(UINT bufferNum,int x,int vx);
	void MarkSoundLikePlaying(UINT bufferNum,int x);
	void ControlPan(UINT bufferNum);
	bool IsPlaying(UINT bufferNum);
	int GetPos(UINT bufferNum);
	void ProcessSoundSystem();
	void HitSound(int SoundID);
	//---------------advanced: group sounds------------------//
	void ClearSoundCategories(){
		memset(SoundCtgFreq,0,sizeof SoundCtgFreq);
		memset(CurrSoundCtgFreq,0,sizeof SoundCtgFreq);
		memset(&CtgSoundID,0,sizeof CtgSoundID);
		memset(CtgNSounds,0,sizeof CtgNSounds);
		memset(StartGroupFreq,0,sizeof StartGroupFreq);
		memset(FinalGroupFreq,0,sizeof FinalGroupFreq);
		memset(SoundForCtg,0,sizeof SoundForCtg);
		memset(IndexInGroup,0,sizeof IndexInGroup);
		memset(SoundCtg,0,sizeof SoundCtg);
		LastDecTime=GetTickCount();
	};
	void SetSoundCategory(WORD SoundID,byte ctg,byte forctg,int idx){
		if(SoundID<MAXSND1){
			SoundCtg[SoundID]=ctg;
			SoundForCtg[SoundID]=forctg;
			IndexInGroup[SoundID]=idx;
		};
	};
	void ClearGroupSound(byte ctg){
		CtgNSounds[ctg]=0;
	};
	void AddGroupSound(byte ctg,WORD SoundID){
		if(CtgNSounds[ctg]<16){
			CtgSoundID[ctg][CtgNSounds[ctg]]=SoundID;
			CtgNSounds[ctg]++;
		};
	};
	void SetGroupOptions(byte ctg,int StartFreq,int EndFreq){
		StartGroupFreq[ctg]=StartFreq;
		FinalGroupFreq[ctg]=EndFreq;
	};
	void StopCtgSounds(){
		memset(SoundCtgFreq,0,sizeof SoundCtgFreq);
	};
protected:
    void ReleaseAll();
};

#endif

#define MaxSnd MAXSND
#define DECFACTOR 200