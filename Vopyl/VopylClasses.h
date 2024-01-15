#pragma once

#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <mmsystem.h>
#include <dsound.h>

#include "include\codec.h"
#include "include\vorbisfile.h"

#include "NiceVopyl.h"

#define STREAM_BUF_SIZE 2

// ---------------------------------------------------------------------------------------------------
// CMyDirectSound
// ---------------------------------------------------------------------------------------------------
#define MY_DS_BUFFERS 5

typedef class CMyDirectSound
{
protected:
	LPDIRECTSOUND m_pDS;
	LPDIRECTSOUNDBUFFER	m_pBuffers[MY_DS_BUFFERS];
	DSBUFFERDESC m_Desc[MY_DS_BUFFERS];
public:
	LPVOID m_bTempBuffer;
	CMyDirectSound(LPDIRECTSOUND pDS);
	~CMyDirectSound();
	HRESULT CreateSoundBuffer(	LPCDSBUFFERDESC pcDSBufferDesc,
								LPDIRECTSOUNDBUFFER * ppDSBuffer,
								LPUNKNOWN pUnkOuter );

}*LPMYDIRECTSOUND;

// ---------------------------------------------------------------------------------------------------
// CFileSoundSource
// ---------------------------------------------------------------------------------------------------
typedef class CFileSoundSource
{
protected:
	WAVEFORMATEX	m_WaveFormat;
	FILE			*m_pFile;
	char			m_szFileName[MAX_PATH];
	double			m_fStreamLength;
	double			m_fStreamPosition;
	bool			m_bStreamFinished;
	bool			m_bStreamLooped;
	DWORD			m_dwPCMTotal;
public:
	CFileSoundSource(VOID);
	virtual ~CFileSoundSource(VOID);
	virtual VPL_STATUS OpenStream(LPCSTR pszFileName);
	VPL_STATUS CloseStream(VOID);
	virtual VPL_STATUS ResetStream(VOID);
	virtual VPL_STATUS ReadStream(LPVOID lpBuffer, DWORD dwBytes, LPDWORD pdwReadBytes)=0;
	VOID SetLoopMode(bool bEnableLoop)
		{m_bStreamLooped=bEnableLoop;};
	VPL_STATUS SilenceBuffer(LPVOID lpBuffer, DWORD dwBytes);
	LPWAVEFORMATEX GetStreamFormat(VOID)
		{return &m_WaveFormat;};
	VPL_POS GetStreamLength(VOID)
		{return m_fStreamLength;};
	VPL_POS GetStreamPosition(VOID)
		{return m_fStreamPosition;};
	DWORD GetPCMLength(VOID)
		{return m_dwPCMTotal;};
	bool IsStreamFinished(VOID)
		{return m_bStreamFinished;};
}*LPCFileSoundSource;
// ---------------------------------------------------------------------------------------------------
// COggFileSoundSource
// ---------------------------------------------------------------------------------------------------
typedef class COggFileSoundSource : public CFileSoundSource
{
protected:
	LPBYTE			m_pbTempStorageBuf;
	DWORD			m_dwTempStorageSize;
	OggVorbis_File	m_ovFile;
	vorbis_info		*m_pvi;
	int				m_Current;
public:
	COggFileSoundSource(VOID);
	virtual ~COggFileSoundSource(VOID);
	virtual VPL_STATUS OpenStream(LPCSTR pszFileName);
	VPL_STATUS CloseStream(VOID);
	virtual VPL_STATUS ResetStream(VOID);
	virtual VPL_STATUS ReadStream(LPVOID lpBuffer, DWORD dwBytes, LPDWORD pdwReadBytes);
}*LPCOggFileSoundSource;
// ---------------------------------------------------------------------------------------------------
// CWavFileSoundSource
// ---------------------------------------------------------------------------------------------------
typedef class CWavFileSoundSource : public CFileSoundSource
{
protected:
    HMMIO         m_hmmio;       
    MMCKINFO      m_ck;          
    MMCKINFO      m_ckRiff;      
    DWORD         m_dwSize;      
    HRESULT ReadMMIO();
public:
	CWavFileSoundSource(VOID);
	virtual ~CWavFileSoundSource(VOID);
	virtual VPL_STATUS OpenStream(LPCSTR pszFileName);
	VPL_STATUS CloseStream(VOID);
	virtual VPL_STATUS ResetStream(VOID);
	virtual VPL_STATUS ReadStream(LPVOID lpBuffer, DWORD dwBytes, LPDWORD pdwReadBytes);
}*LPCWavFileSoundSource;
// ---------------------------------------------------------------------------------------------------
// CSoundFileData
// ---------------------------------------------------------------------------------------------------
typedef class CSoundFileData
{
public:
	WAVEFORMATEX	m_WaveFormat;
	CHAR			m_szFileName[MAX_PATH];
	LPBYTE			m_pbWaveData;
	DWORD			m_dwWaveSize;
	DWORD			m_dwLastAccessTime;
	CSoundFileData(VOID);
	virtual ~CSoundFileData(VOID);
	CSoundFileData* LoadData(LPCSTR pszFileName);
	VPL_STATUS FreeData(VOID);
	CSoundFileData* Touch(VOID);
}*LPCSoundFileData;
// ---------------------------------------------------------------------------------------------------
// CCachedSoundSource
// ---------------------------------------------------------------------------------------------------
struct ltstr
{
	bool operator()(LPCSTR s1, LPCSTR s2) const
	{
		return strcmp(s1,s2)<0;
	}
};
// ---------------------------------------------------------------------------------------------------
typedef map <LPCSTR,LPCSoundFileData,ltstr> str_dat_map;
typedef pair <LPCSTR,LPCSoundFileData> str_dat_pair;
typedef str_dat_map::iterator str_dat_map_iter;
// ---------------------------------------------------------------------------------------------------
typedef class CCachedSoundSource
{
protected:
	str_dat_map m_map;
	DWORD m_dwMaxEntries;
	#ifdef COLLECT_STATS
		DWORD	m_dwHits;
		DWORD	m_dwMisses;
	#endif // COLLECT_STATS
public:
	CCachedSoundSource(VOID);
	VOID SetMaxEntries(DWORD dwEntries);
	virtual ~CCachedSoundSource(VOID);
	LPCSoundFileData GetSoundData(LPCSTR pszFileName);
}*LPCCachedSoundSource;
// ---------------------------------------------------------------------------------------------------
// CSimpleSound
// ---------------------------------------------------------------------------------------------------
typedef class CSimpleSound
{ 
protected:
	LPDIRECTSOUNDBUFFER m_pDSBuffer;
	DSBUFFERDESC m_DSBufferDesc;
	LONG m_lPan;
	LONG m_lVolume;
	DWORD m_dwPlayFlags;
public:
	CSimpleSound();
	virtual ~CSimpleSound(VOID);
	HRESULT GetPan(LPLONG plPan)
		{return m_pDSBuffer->GetPan(plPan);};
	HRESULT SetPan(LONG lPan)
		{return m_pDSBuffer->SetPan(lPan);};
	HRESULT GetVolume(LPLONG plVolume)
		{return m_pDSBuffer->GetVolume(plVolume);};
	HRESULT SetVolume(LONG lVolume)
		{return m_pDSBuffer->SetVolume(lVolume);};
	VOID SetStartVolume(LONG lVolume)
		{m_lVolume=lVolume;};
	virtual VOID HandleBuffer(VOID)
		{;};
	HRESULT Stop(VOID)
		{return m_pDSBuffer->Stop();};
	HRESULT Resume(VOID)
		{return m_pDSBuffer->Play(0,0,m_dwPlayFlags);};
}*LPCSimpleSound;
// ---------------------------------------------------------------------------------------------------
// CStaticSound
// ---------------------------------------------------------------------------------------------------
typedef class CStaticSound : public CSimpleSound
{
protected:
	LPCSoundFileData m_pSoundFileData;
public:
	HRESULT Play(LPCSoundFileData pSoundFileData);
}*LPCStaticSound;
// ---------------------------------------------------------------------------------------------------
// CStreamingSound
// ---------------------------------------------------------------------------------------------------
typedef class CStreamingSound : public CSimpleSound
{
protected:	
	LPCFileSoundSource m_pFileSoundSource;
	BOOL m_bLeftValid;
	BOOL m_bRightValid;
	BOOL m_bFinished;
	BOOL m_bEmpty;
	LPMYDIRECTSOUND m_pMDS;
public:
	CStreamingSound();
	virtual ~CStreamingSound(VOID);
	HRESULT Play(LPMYDIRECTSOUND pDirectSound, LPCSTR pszFileName);
	virtual VOID HandleBuffer(VOID);
	LPCFileSoundSource GetSoundSource()
		{return m_pFileSoundSource;};
	BOOL IsFinished(){return m_bFinished;};
}*LPCStreamingSound;
// ---------------------------------------------------------------------------------------------------
// CSoundManager
// ---------------------------------------------------------------------------------------------------
typedef class CSoundManager
{
typedef list <LPCStreamingSound> m_pss_list;
typedef m_pss_list::iterator m_pss_list_iter;
protected:
//	LPCCachedSoundSource m_pCachedSoundSource;
	LPDIRECTSOUND m_pDirectSound;
	HWND m_hWnd;
	m_pss_list m_list;
public:
	CSoundManager(VOID);
	~CSoundManager(VOID);
	LPCStreamingSound PlayStreaming(LPCSTR pszFileName);
	VOID CloseStreaming(LPCStreamingSound);
	VOID HandleBuffers(VOID);
}*LPCSoundManager;
