#include "windows.h"
#include "stdheader.h"
// DeviceCD.cpp : implementation file
//

//#include "stdafx.h"
#pragma pack(1)
#include "DeviceCD.h"
#include "TMixer.h"
#include <stdio.h>
#include "ResFile.h"
#include "gFile.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define USEMP3
/////////////////////////////////////////////////////////////////////////////
// CDeviceCD
int StartTrack=2;
int NTracks=19;
byte TracksMask[32]={2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
CDeviceCD::CDeviceCD()
{
#ifdef USEMP3
	return;
#endif
	GFILE* f=Gopen("Tracks.cd","r");
	if(f){
		Gscanf(f,"%d%d",&StartTrack,&NTracks);
		for(int i=0;i<NTracks;i++)Gscanf(f,"%d",TracksMask+i);
		Gclose(f);
	};
	Open();
}

CDeviceCD::~CDeviceCD()
{
#ifdef USEMP3
	return;
#endif
	Close();
}


// CDeviceCD message handlers
MCIDEVICEID glFDeviceID;
bool CDeviceCD::Open()
{
#ifdef USEMP3
	return true;
#endif

	MCI_OPEN_PARMS OPEN_PARAMS;

	OPEN_PARAMS.dwCallback=0;
	OPEN_PARAMS.lpstrDeviceType="CDAudio";

	FError=mciSendCommand(0,MCI_OPEN,MCI_OPEN_TYPE,(DWORD) (LPMCI_OPEN_PARMS) &OPEN_PARAMS);

	if(! FError) 
	{
		FDeviceID=OPEN_PARAMS.wDeviceID;
		glFDeviceID=FDeviceID;
		FOpened=TRUE;

		MCI_SET_PARMS SET_PARAMS;

		SET_PARAMS.dwCallback=0;
		SET_PARAMS.dwTimeFormat=MCI_FORMAT_TMSF;

		FError=mciSendCommand(FDeviceID,MCI_SET,MCI_SET_TIME_FORMAT,(DWORD) (LPMCI_SET_PARMS) &SET_PARAMS);

		if(! FError){;};

		return TRUE;

	}
	else
	{
		FDeviceID=-1;
		FOpened=FALSE;
		return FALSE;
	}
}

bool CDeviceCD::Close()
{
#ifdef USEMP3
	return true;
#endif
	if(FOpened)
	{
		MCI_GENERIC_PARMS CLOSE_PARAMS;

		CLOSE_PARAMS.dwCallback=0;

		FError=mciSendCommand(FDeviceID,MCI_CLOSE,0,(DWORD) (LPMCI_GENERIC_PARMS) &CLOSE_PARAMS);

		if(! FError)
		{
			FOpened=FALSE;
			FDeviceID=-1;
			return TRUE;
		}
		else
			return FALSE;
	}
	else
		return FALSE;
}

bool CDeviceCD::Pause()
{
#ifdef USEMP3
	return true;
#endif
	if(FOpened)
	{
		MCI_GENERIC_PARMS PAUSE_PARAMS;

		PAUSE_PARAMS.dwCallback=0;

		FError=mciSendCommand(FDeviceID,MCI_PAUSE,0,(DWORD) (LPMCI_GENERIC_PARMS) &PAUSE_PARAMS);

		if(! FError)
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}

bool CDeviceCD::Resume()
{
#ifdef USEMP3
	return true;
#endif
 	if(FOpened)
	{
		MCI_GENERIC_PARMS RESUME_PARAMS;

		RESUME_PARAMS.dwCallback=0;

		FError=mciSendCommand(FDeviceID,MCI_RESUME,0,(DWORD) (LPMCI_GENERIC_PARMS) &RESUME_PARAMS);

		if(! FError)
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}

bool CDeviceCD::Stop()
{
#ifdef USEMP3
	return true;
#endif
	if(FOpened)
	{
		MCI_GENERIC_PARMS STOP_PARAMS;

		STOP_PARAMS.dwCallback=0;

		FError=mciSendCommand(FDeviceID,MCI_STOP,0,(DWORD) (LPMCI_GENERIC_PARMS) &STOP_PARAMS);

		if(! FError)
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}

DWORD CDeviceCD::GetVolume()
{
#ifdef USEMP3
	return 0;
#endif
	CMixer Mixer(MIXERLINE_COMPONENTTYPE_DST_SPEAKERS,
	             MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC,
	             MIXERCONTROL_CONTROLTYPE_VOLUME);


	return Mixer.GetControlValue();
}

bool CDeviceCD::SetVolume(DWORD Volume)
{
#ifdef USEMP3
	return false;
#endif
	CMixer Mixer(MIXERLINE_COMPONENTTYPE_DST_SPEAKERS,
	             MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC,
	             MIXERCONTROL_CONTROLTYPE_VOLUME);


	Mixer.SetControlValue(Volume);

	return 1;
}
extern HWND hwnd;
bool CDeviceCD::Play(DWORD Track)
{
#ifdef USEMP3
	return true;
#endif
	if(FOpened)
	{
		MCI_PLAY_PARMS PLAY_PARAMS;

		PLAY_PARAMS.dwCallback=(DWORD)hwnd;
		PLAY_PARAMS.dwFrom=Track;
		PLAY_PARAMS.dwTo=Track+1;

		FError=mciSendCommand(FDeviceID,MCI_PLAY,MCI_NOTIFY | MCI_FROM | MCI_TO,(DWORD) (LPMCI_PLAY_PARMS) &PLAY_PARAMS);

		if(! FError){
			FError=mciSendCommand(FDeviceID,MCI_PLAY,MCI_NOTIFY | MCI_FROM ,(DWORD) (LPMCI_PLAY_PARMS) &PLAY_PARAMS);
			return TRUE;
		}else{
			return FALSE;
		};
	}
	else
		return FALSE;
}
int PrevTrack3=-1;
int PrevTrack2=-1;
int PrevTrack1=-1;
int NextCommand=-1;
void PlayCDTrack(int Id);
extern int srando();
void PlayRandomTrack();
LRESULT CD_MCINotify(WPARAM wFlags, LONG lDevId)
{
	if(MCIDEVICEID(lDevId)==glFDeviceID && wFlags==MCI_NOTIFY_SUCCESSFUL)
	{
		//insert here
		if(NextCommand==-1){
			PlayRandomTrack();
		}else if(NextCommand>=1000){
			PlayCDTrack(NextCommand-1000);
			PrevTrack1=NextCommand-1000;
			NextCommand=-1;
		}else{
			PlayCDTrack(NextCommand);
			PrevTrack1=NextCommand;
		};
		return 1;
	}
	else
		return 0;

}
CDeviceCD CDPLAY;
void PlayCDTrack(int Id){
#ifdef USEMP3
	return;
#endif
	CDPLAY.Play(Id);
};
extern int CurrentNation;
extern int PlayMode;
void PlayRandomMP3();
void PlayRandomTrack(){
#ifdef USEMP3
	PlayRandomMP3();
	return;
#endif
	if(GSets.SVOpt.MusicPlayMode==1&&CurrentNation!=-1){
		PlayCDTrack(TracksMask[CurrentNation]);
		return;
	};
	int Track=-1;
	do{
		Track=(((GetTickCount()&4095)*NTracks)>>12)+StartTrack;
		if(Track==PrevTrack1||Track==PrevTrack2||Track==PrevTrack3)Track=-1;
	}while(Track==-1);
	PrevTrack3=PrevTrack2;
	PrevTrack2=PrevTrack1;
	PrevTrack1=Track;
	PlayCDTrack(Track);
};
#define CEXPORT __declspec(dllexport)
void StopPlayingMP3();
CEXPORT
void StopPlayCD(){
#ifdef USEMP3
	StopPlayingMP3();
	return;
#endif
	CDPLAY.Stop();
};
void SetMP3Volume(int vol);
int GetMP3Volume();
CEXPORT
int GetCDVolume(){
#ifdef USEMP3
	return GetMP3Volume();
#endif
	return (int(CDPLAY.GetVolume())*100)>>16;
};
CEXPORT
void SetCDVolume(int Vol){
#ifdef USEMP3
	//if(Vol<2)Vol=2;
	SetMP3Volume(Vol);
	return;
#endif
	CDPLAY.SetVolume(((Vol)*65535)/100);
};
void SetCDVolumeEx(int Vol){
	extern byte PlayGameMode;
	if(PlayGameMode==0){
		GSets.SVOpt.MusicVolume=Vol;
		SetCDVolume(Vol);
	}
}