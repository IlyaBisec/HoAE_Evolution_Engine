
#include "VopylClasses.h"

static HRESULT dx_stat;

// ---------------------------------------------------------------------------------------------------
// CFileSoundSource
// ---------------------------------------------------------------------------------------------------
CFileSoundSource::CFileSoundSource(VOID)
{
	ZeroMemory(&m_WaveFormat,sizeof(WAVEFORMATEX));
	m_pFile=NULL;
	ZeroMemory(m_szFileName,MAX_PATH);
	m_fStreamLength=0;
	m_fStreamPosition=0;
	m_bStreamFinished=TRUE;
	m_bStreamLooped=FALSE;
	m_dwPCMTotal=0;
}

CFileSoundSource::~CFileSoundSource(VOID)
{
	if(m_pFile)
		CloseStream();
}

VPL_STATUS CFileSoundSource::OpenStream(LPCSTR pszFileName)
{
	strcpy(m_szFileName,pszFileName);
	assert((m_pFile=fopen(m_szFileName,"rb"))!=NULL);
	m_fStreamPosition=0;
	m_bStreamFinished=FALSE;
	m_bStreamLooped=FALSE;
	m_dwPCMTotal=0;

	return VPL_OK;
}

VPL_STATUS CFileSoundSource::CloseStream(VOID)
{
	if(m_pFile)
		fclose(m_pFile);
	m_pFile=NULL;
	m_fStreamLength=0;
	m_fStreamPosition=0;
	m_bStreamFinished=TRUE;
	m_bStreamLooped=FALSE;
	m_dwPCMTotal=0;

	return VPL_OK;
}

VPL_STATUS CFileSoundSource::ResetStream(VOID)
{
	fseek(m_pFile,0,SEEK_SET);
	m_fStreamPosition=0;
	m_bStreamFinished=FALSE;

	return VPL_OK;
}

VPL_STATUS CFileSoundSource::SilenceBuffer(LPVOID lpBuffer, DWORD dwBytes)
{
	ZeroMemory(lpBuffer,dwBytes);

	return VPL_OK;
}

// ---------------------------------------------------------------------------------------------------
// COggFileSoundSource
// ---------------------------------------------------------------------------------------------------
COggFileSoundSource::COggFileSoundSource(VOID)
{
	m_pbTempStorageBuf=(LPBYTE)malloc(2*VPL_CHUNK_SIZE);
	ZeroMemory(&m_ovFile,sizeof(OggVorbis_File));
	m_dwTempStorageSize=0;
	m_pvi=NULL;
}

COggFileSoundSource::~COggFileSoundSource(VOID)
{
	free(m_pbTempStorageBuf);
	CloseStream();
}

VPL_STATUS COggFileSoundSource::OpenStream(LPCSTR pszFileName)
{
	CFileSoundSource::OpenStream(pszFileName);
	assert((ov_open(m_pFile,&m_ovFile,NULL,0))==0);
	m_pvi=ov_info(&m_ovFile,-1);

	ZeroMemory(&m_WaveFormat,sizeof(WAVEFORMATEX));
	m_WaveFormat.cbSize=sizeof(WAVEFORMATEX);
	m_WaveFormat.wFormatTag=WAVE_FORMAT_PCM;
	m_WaveFormat.nChannels=m_pvi->channels;
	m_WaveFormat.nSamplesPerSec=m_pvi->rate;
	m_WaveFormat.wBitsPerSample=16;
	m_WaveFormat.nBlockAlign=2*m_WaveFormat.nChannels;
	m_WaveFormat.nAvgBytesPerSec=m_WaveFormat.nSamplesPerSec*m_WaveFormat.nBlockAlign;

	m_fStreamLength=ov_time_total(&m_ovFile,-1);

	m_dwPCMTotal=2*m_WaveFormat.nChannels*(DWORD)ov_pcm_total(&m_ovFile,-1);

	return VPL_OK;
}

VPL_STATUS COggFileSoundSource::CloseStream(VOID)
{
	ov_clear(&m_ovFile);
	ZeroMemory(&m_ovFile,sizeof(OggVorbis_File));
	m_dwTempStorageSize=0;
	m_pvi=NULL;
//	CFileSoundSource::CloseStream();

	return VPL_OK;
}

VPL_STATUS COggFileSoundSource::ResetStream(VOID)
{
	CFileSoundSource::ResetStream();
	ov_pcm_seek(&m_ovFile,0);
	m_dwTempStorageSize=0;

	return VPL_OK;
}

VPL_STATUS COggFileSoundSource::ReadStream(LPVOID lpBuffer, DWORD dwBytes, LPDWORD pdwReadBytes)
{
	long bytes_read=0,from_temp=0,all_to_get=dwBytes;
	int current_section=-1;
	LPBYTE	dest_buffer=(LPBYTE)lpBuffer;

	if(pdwReadBytes)
		*pdwReadBytes=dwBytes;

	while(1){
		if(m_dwTempStorageSize){
			from_temp=all_to_get>(long)m_dwTempStorageSize?m_dwTempStorageSize:all_to_get;
			memcpy(dest_buffer,m_pbTempStorageBuf,from_temp);
			dest_buffer+=from_temp;
			all_to_get-=from_temp;
			m_dwTempStorageSize-=from_temp;
			if(m_dwTempStorageSize)
				memmove(m_pbTempStorageBuf,m_pbTempStorageBuf+from_temp,m_dwTempStorageSize);
		};

		if(all_to_get==0)
			break;

		bytes_read=ov_read(&m_ovFile,(LPSTR)(m_pbTempStorageBuf+m_dwTempStorageSize),VPL_CHUNK_SIZE,0,2,1,&current_section);
		if(bytes_read==0)
			if(m_bStreamLooped){
				assert(ov_pcm_seek(&m_ovFile,0)==0);
				bytes_read=ov_read(&m_ovFile,(LPSTR)(m_pbTempStorageBuf+m_dwTempStorageSize),VPL_CHUNK_SIZE,0,2,1,&current_section);
				assert(bytes_read!=0);
			}else{
				SilenceBuffer(m_pbTempStorageBuf+m_dwTempStorageSize,VPL_CHUNK_SIZE);
				bytes_read=VPL_CHUNK_SIZE;
				m_bStreamFinished=TRUE;
				if(pdwReadBytes)
					if(*pdwReadBytes==dwBytes)
						*pdwReadBytes=dwBytes-all_to_get;
			};

		m_dwTempStorageSize+=bytes_read;
	};

	m_fStreamPosition=ov_time_tell(&m_ovFile);

	return VPL_OK;
}

// ---------------------------------------------------------------------------------------------------
// CWavFileSoundSource
// ---------------------------------------------------------------------------------------------------
CWavFileSoundSource::CWavFileSoundSource(VOID)
{
    m_hmmio   = NULL;
    m_dwSize  = 0;
}

CWavFileSoundSource::~CWavFileSoundSource(VOID)
{
	CloseStream();
}


VPL_STATUS CWavFileSoundSource::OpenStream(LPCSTR pszFileName)
{
	CFileSoundSource::OpenStream(pszFileName);
    m_hmmio=mmioOpen((LPSTR)pszFileName,NULL,MMIO_ALLOCBUF|MMIO_READ);
	ReadMMIO();
	ResetStream();
	m_dwSize=m_ck.cksize;

	m_dwPCMTotal=m_ck.cksize;

	return VPL_OK;
}

HRESULT CWavFileSoundSource::ReadMMIO()
{
    MMCKINFO        ckIn;

    mmioDescend(m_hmmio,&m_ckRiff,NULL,0);
	ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
    mmioDescend(m_hmmio,&ckIn,&m_ckRiff,MMIO_FINDCHUNK );
	mmioRead(m_hmmio,(HPSTR)&m_WaveFormat,sizeof(WAVEFORMATEX));

	m_WaveFormat.cbSize=sizeof(WAVEFORMATEX);

	mmioAscend(m_hmmio,&ckIn,0);
	
	return S_OK;
}

VPL_STATUS CWavFileSoundSource::CloseStream(VOID)
{
    mmioClose(m_hmmio,0);
    m_hmmio=NULL;
//	CFileSoundSource::CloseStream();

	return VPL_OK;
}

VPL_STATUS CWavFileSoundSource::ResetStream(VOID)
{
	CFileSoundSource::ResetStream();
	mmioSeek(m_hmmio,m_ckRiff.dwDataOffset+sizeof(FOURCC),SEEK_SET);
	m_ck.ckid=mmioFOURCC('d','a','t','a');
	mmioDescend(m_hmmio,&m_ck,&m_ckRiff,MMIO_FINDCHUNK);

	return VPL_OK;
}

VPL_STATUS CWavFileSoundSource::ReadStream(LPVOID lpBuffer, DWORD dwBytes, LPDWORD pdwReadBytes)
{
	MMIOINFO mmioinfoIn;

	mmioGetInfo(m_hmmio,&mmioinfoIn,0);

	UINT cbDataIn=dwBytes;
	if(cbDataIn>m_ck.cksize)
		cbDataIn=m_ck.cksize;

	if(pdwReadBytes)
		*pdwReadBytes=cbDataIn;

	m_ck.cksize-=cbDataIn;

	for(DWORD cT=0;cT<cbDataIn;cT++){
		if(mmioinfoIn.pchNext==mmioinfoIn.pchEndRead)
			mmioAdvance(m_hmmio,&mmioinfoIn,MMIO_READ);

		*((BYTE*)lpBuffer+cT)=*((BYTE*)mmioinfoIn.pchNext);
		mmioinfoIn.pchNext++;
	}

	mmioSetInfo(m_hmmio,&mmioinfoIn,0);

	if(dwBytes>cbDataIn)
		SilenceBuffer((LPBYTE)lpBuffer+cbDataIn,dwBytes-cbDataIn);

	return VPL_OK;
}
// ---------------------------------------------------------------------------------------------------
// CSoundFileData
// ---------------------------------------------------------------------------------------------------
CSoundFileData::CSoundFileData(VOID)
{
	ZeroMemory(&m_WaveFormat,sizeof(WAVEFORMATEX));
	ZeroMemory(m_szFileName,MAX_PATH);
	m_pbWaveData=NULL;
	m_dwWaveSize=0;
	m_dwLastAccessTime=0;
}

CSoundFileData::~CSoundFileData(VOID)
{
	FreeData();
}

LPCSoundFileData CSoundFileData::LoadData(LPCSTR pszFileName)
{
	LPCFileSoundSource	pFSS;
	DWORD				dwBytesRead;

	FreeData();

	strcpy(m_szFileName,pszFileName);

	if(strstr(m_szFileName,".ogg")||strstr(m_szFileName,".OGG"))
		pFSS=new COggFileSoundSource;
	else
		if(strstr(m_szFileName,".wav")||strstr(m_szFileName,".WAV"))
			pFSS=new CWavFileSoundSource;
		else
			return NULL;

	pFSS->OpenStream(m_szFileName);
	m_dwWaveSize=pFSS->GetPCMLength();
	memcpy(&m_WaveFormat,pFSS->GetStreamFormat(),sizeof(WAVEFORMATEX));
	m_pbWaveData=(LPBYTE)malloc(m_dwWaveSize);
	pFSS->ReadStream(m_pbWaveData,m_dwWaveSize,&dwBytesRead);
	pFSS->CloseStream();
	delete pFSS;

	m_dwLastAccessTime=GetTickCount();
    
	return this;
}

VPL_STATUS CSoundFileData::FreeData(VOID)
{
	if(m_pbWaveData)
		free(m_pbWaveData);
	ZeroMemory(&m_WaveFormat,sizeof(WAVEFORMATEX));
	ZeroMemory(m_szFileName,MAX_PATH);
	m_pbWaveData=NULL;
	m_dwWaveSize=0;
	m_dwLastAccessTime=0;

	return VPL_OK;
}

LPCSoundFileData CSoundFileData::Touch(VOID)
{
	m_dwLastAccessTime=GetTickCount();
	return this;
}
// ---------------------------------------------------------------------------------------------------
// CCachedSoundSource
// ---------------------------------------------------------------------------------------------------
CCachedSoundSource::~CCachedSoundSource(VOID)
{
	str_dat_map_iter sdmi;
	LPCSoundFileData psfd;

	while((sdmi=m_map.begin())!=m_map.end()){
		psfd=sdmi->second;
		m_map.erase(sdmi);
		delete psfd;
	};
}

CCachedSoundSource::CCachedSoundSource(VOID)
{
	m_dwMaxEntries=VPL_DEFAULT_ENTRIES;
	#ifdef COLLECT_STATS
		m_dwHits=0;
		m_dwMisses=0;
	#endif // COLLECT_STATS
}

VOID CCachedSoundSource::SetMaxEntries(DWORD dwEntries)
{
	m_dwMaxEntries=dwEntries;
}

bool ltsfd(str_dat_pair e1, str_dat_pair e2)
{
	return e1.second->m_dwLastAccessTime<e2.second->m_dwLastAccessTime;
}

LPCSoundFileData CCachedSoundSource::GetSoundData(LPCSTR pszFileName)
{
	LPCSoundFileData psfd=NULL;

	str_dat_map_iter sm_it=m_map.find(pszFileName);

	if(sm_it==m_map.end()){
		#ifdef COLLECT_STATS
			m_dwMisses++;		
		#endif // COLLECT_STATS
		if(m_map.size()<m_dwMaxEntries){
			psfd=new CSoundFileData;
			psfd->LoadData(pszFileName);
			m_map.insert(str_dat_pair(psfd->m_szFileName,psfd));
			return psfd;
		}else{
			str_dat_map_iter sdmi=min_element(m_map.begin(),m_map.end(),ltsfd);
			return sdmi->second->LoadData(pszFileName);
		};
	}else{
		#ifdef COLLECT_STATS
			m_dwHits++;
		#endif // COLLECT_STATS
		return sm_it->second->Touch();
	};
}
// ---------------------------------------------------------------------------------------------------
// CSimpleSound
// ---------------------------------------------------------------------------------------------------
CSimpleSound::CSimpleSound()
{
	m_pDSBuffer=NULL;
	ZeroMemory(&m_DSBufferDesc,sizeof(DSBUFFERDESC));
	m_lPan=DSBPAN_CENTER;
	m_lVolume=DSBVOLUME_MAX;
	m_dwPlayFlags=0;
}

CSimpleSound::~CSimpleSound(VOID)
{
	if(m_pDSBuffer){
		m_pDSBuffer->Stop();
		// m_pDSBuffer->Release();
	};
}

// ---------------------------------------------------------------------------------------------------
// CStaticSound
// ---------------------------------------------------------------------------------------------------
HRESULT CStaticSound::Play(LPCSoundFileData pSoundFileData)
{
	m_dwPlayFlags=0;
	return DS_OK;
}

// ---------------------------------------------------------------------------------------------------
// CStreamingSound
// ---------------------------------------------------------------------------------------------------
CStreamingSound::CStreamingSound()
{
	m_pFileSoundSource=NULL;
	m_bLeftValid=FALSE;
	m_bRightValid=FALSE;
	m_bFinished=TRUE;
	m_bEmpty=FALSE;
	m_pMDS=NULL;
}

CStreamingSound::~CStreamingSound(VOID)
{
	if(m_pFileSoundSource)
		delete m_pFileSoundSource;
}

VOID CStreamingSound::HandleBuffer(VOID)
{
	LPVOID ppvAudioPtr1=NULL;
	DWORD dwAudioBytes1=0;
	DWORD dwPlayCursor;

	DX_CHECK(12,m_pDSBuffer->GetCurrentPosition(&dwPlayCursor,NULL));

	if((dwPlayCursor<m_DSBufferDesc.dwBufferBytes>>1)&&(m_bRightValid==FALSE)){
		// we are playing left part - filling right part

		if(m_bEmpty){
			m_bFinished=TRUE;
			DX_CHECK(6,m_pDSBuffer->Stop());
			return;
		};

		if(m_pFileSoundSource->IsStreamFinished())
			m_bEmpty=TRUE;

		m_pFileSoundSource->ReadStream(m_pMDS->m_bTempBuffer,m_DSBufferDesc.dwBufferBytes>>1,NULL);
		DX_CHECK(7,m_pDSBuffer->Lock(m_DSBufferDesc.dwBufferBytes>>1,m_DSBufferDesc.dwBufferBytes>>1,&ppvAudioPtr1,&dwAudioBytes1,NULL,NULL,0));
		memcpy(ppvAudioPtr1,m_pMDS->m_bTempBuffer,dwAudioBytes1);
		DX_CHECK(8,m_pDSBuffer->Unlock(ppvAudioPtr1,dwAudioBytes1,NULL,0));

		m_bLeftValid=FALSE;
		m_bRightValid=TRUE;
	}else
	if((dwPlayCursor>m_DSBufferDesc.dwBufferBytes>>1)&&(m_bLeftValid==FALSE)){
		// we are playing right part - filling left part

		if(m_bEmpty){
			m_bFinished=TRUE;
			DX_CHECK(9,m_pDSBuffer->Stop());
			return;
		};

		if(m_pFileSoundSource->IsStreamFinished())
			m_bEmpty=TRUE;

		m_pFileSoundSource->ReadStream(m_pMDS->m_bTempBuffer,m_DSBufferDesc.dwBufferBytes>>1,NULL);
		DX_CHECK(10,m_pDSBuffer->Lock(0,m_DSBufferDesc.dwBufferBytes>>1,&ppvAudioPtr1,&dwAudioBytes1,NULL,NULL,0));
		memcpy(ppvAudioPtr1,m_pMDS->m_bTempBuffer,dwAudioBytes1);
		DX_CHECK(11,m_pDSBuffer->Unlock(ppvAudioPtr1,dwAudioBytes1,NULL,0));

		m_bLeftValid=TRUE;
		m_bRightValid=FALSE;
	};
}

HRESULT CStreamingSound::Play(LPMYDIRECTSOUND pDirectSound, LPCSTR pszFileName)
{
	LPVOID ppvAudioPtr1=NULL;
	DWORD dwAudioBytes1=0;

	m_bFinished=TRUE;

	m_pMDS=pDirectSound;

	if(strstr(pszFileName,".ogg")||strstr(pszFileName,".OGG"))
		m_pFileSoundSource=new COggFileSoundSource;
	else
		if(strstr(pszFileName,".wav")||strstr(pszFileName,".WAV"))
			m_pFileSoundSource=new CWavFileSoundSource;
		else
			return DS_OK;

	m_pFileSoundSource->OpenStream(pszFileName);
	
	m_DSBufferDesc.dwSize=sizeof(DSBUFFERDESC);
	m_DSBufferDesc.dwFlags=DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;
	m_DSBufferDesc.dwReserved=0;
	m_DSBufferDesc.guid3DAlgorithm=DS3DALG_DEFAULT;
	m_DSBufferDesc.lpwfxFormat=m_pFileSoundSource->GetStreamFormat();
	m_DSBufferDesc.dwBufferBytes=STREAM_BUF_SIZE*m_DSBufferDesc.lpwfxFormat->nAvgBytesPerSec;

	DX_CHECK(1,pDirectSound->CreateSoundBuffer(&m_DSBufferDesc,&m_pDSBuffer,NULL));

	m_pFileSoundSource->ReadStream(m_pMDS->m_bTempBuffer,m_DSBufferDesc.dwBufferBytes>>1,NULL);
	DX_CHECK(2,m_pDSBuffer->Lock(0,m_DSBufferDesc.dwBufferBytes>>1,&ppvAudioPtr1,&dwAudioBytes1,NULL,NULL,0));
		memcpy(ppvAudioPtr1,m_pMDS->m_bTempBuffer,dwAudioBytes1);
	DX_CHECK(3,m_pDSBuffer->Unlock(ppvAudioPtr1,dwAudioBytes1,NULL,0));
	m_bLeftValid=TRUE;
	m_bRightValid=FALSE;
	m_dwPlayFlags=DSBPLAY_LOOPING;

	DX_CHECK(4,m_pDSBuffer->SetVolume(m_lVolume));
	DX_CHECK(5,m_pDSBuffer->Play(0,0,m_dwPlayFlags));


	if(dx_stat!=DS_OK){
		char szError[512];
		sprintf(szError,"%4.4X",dx_stat);
		MessageBox(NULL,szError,szError,MB_OK);
	}

	m_bFinished=FALSE;

	return DS_OK;
}

// ---------------------------------------------------------------------------------------------------
// CMyDirectSound
// ---------------------------------------------------------------------------------------------------

const WAVEFORMATEX wfmt_stereo	=	{1,2,44100,176400,4,16,18};	
const WAVEFORMATEX wfmt_mono	=	{1,1,44100,88200,2,16,18};	

const DWORD b_flags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;

const DSBUFFERDESC bdesc_stereo =	{
		sizeof(DSBUFFERDESC),
		b_flags,
		STREAM_BUF_SIZE*wfmt_stereo.nAvgBytesPerSec,
		0,
		(LPWAVEFORMATEX)&wfmt_stereo,
		DS3DALG_DEFAULT
};

const DSBUFFERDESC bdesc_mono =	{
	sizeof(DSBUFFERDESC),
		b_flags,
		STREAM_BUF_SIZE*wfmt_mono.nAvgBytesPerSec,
		0,
		(LPWAVEFORMATEX)&wfmt_mono,
		DS3DALG_DEFAULT
};


CMyDirectSound::CMyDirectSound(LPDIRECTSOUND pDS)
{
	m_pDS=pDS;

	for(int i=0;i<MY_DS_BUFFERS;i++)
		m_pBuffers[i]=NULL;

	for(int i=0;i<2;i++)
		pDS->CreateSoundBuffer(&bdesc_stereo,&m_pBuffers[i],NULL);
	for(int i=2;i<MY_DS_BUFFERS;i++)
		pDS->CreateSoundBuffer(&bdesc_mono,&m_pBuffers[i],NULL);

	m_bTempBuffer=malloc(bdesc_stereo.dwBufferBytes);

}

CMyDirectSound::~CMyDirectSound()
{
	for(int i=0;i<MY_DS_BUFFERS;i++)
		if(m_pBuffers[i]){
			m_pBuffers[i]->Stop();
			m_pBuffers[i]->Release();
			m_pBuffers[i]=NULL;
		}

	if(m_bTempBuffer)
		free(m_bTempBuffer);
}

HRESULT CMyDirectSound::CreateSoundBuffer(	
								LPCDSBUFFERDESC pcDSBufferDesc,
								LPDIRECTSOUNDBUFFER * ppDSBuffer,
								LPUNKNOWN pUnkOuter )
{
	DWORD dwStatus;
	int i;

	*ppDSBuffer=NULL;

	if(memcmp(pcDSBufferDesc->lpwfxFormat,bdesc_stereo.lpwfxFormat,sizeof(WAVEFORMATEX))==0){
		for(i=0;i<2;i++){
			m_pBuffers[i]->GetStatus(&dwStatus);
			if(!(dwStatus&DSBSTATUS_PLAYING))
				break;
		}
	}else
	if(memcmp(pcDSBufferDesc->lpwfxFormat,bdesc_mono.lpwfxFormat,sizeof(WAVEFORMATEX))==0){
		for(i=2;i<MY_DS_BUFFERS;i++){
			m_pBuffers[i]->GetStatus(&dwStatus);
			if(!(dwStatus&DSBSTATUS_PLAYING))
				break;
		}
	}else{
		return DSERR_GENERIC;
	}

	*ppDSBuffer=m_pBuffers[i];

	return DS_OK;
}

