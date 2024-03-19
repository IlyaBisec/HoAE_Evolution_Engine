#include "stdheader.h"
///////////////////////////////////////////////////////////
// CDIRSND.CPP -- Implementation file for the CDirSound
//                class.
///////////////////////////////////////////////////////////

//#include <stdafx.h>
#include "cdirsnd.h"
#include "mp3\oggvor.h"
///////////////////////////////////////////////////////////
// CDirSound::CDirSound()
//
// This is the class's constructor.
///////////////////////////////////////////////////////////
void CDirSound::CreateDirSound(HWND hWnd)
{
    // Initialize class data members.
	ClearSoundCategories();
    m_hWindow = hWnd;
    m_pDirectSoundObj = NULL;
    m_currentBufferNum = 0;

    for (UINT x=0; x<MAXSND1; ++x)
    {
        m_bufferPointers[x] = NULL;
        m_bufferSizes[x] = 0;
    }

    // Create the main DirectSound object.
    HRESULT result =
        DirectSoundCreate(NULL, &m_pDirectSoundObj, NULL);
    if (result != DS_OK) return;
    
     // Set the priority level.
     result = m_pDirectSoundObj->
         SetCooperativeLevel(m_hWindow, DSSCL_PRIORITY);//NORMAL);
     if (result != DS_OK)
         m_pDirectSoundObj = NULL;
	    // Set up DSBUFFERDESC structure. 
	
	DSBUFFERDESC dsbdesc; 
	 memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); // Zero it out. 
	dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
	dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER; 
	// Buffer size is determined by sound hardware. 
	dsbdesc.dwBufferBytes = 0; 
	dsbdesc.lpwfxFormat = NULL; // Must be NULL for primary buffers. 
	LPDIRECTSOUNDBUFFER lpDsb=NULL;
	HRESULT hr = m_pDirectSoundObj->CreateSoundBuffer(&dsbdesc, &lpDsb, NULL); 
	/*
	int v;
	switch(hr){
	case DSERR_ALLOCATED:
		v=0;
		break;
	case DSERR_BADFORMAT:
		v=1;
		break;
	case DSERR_INVALIDPARAM:
		v=2;
		break;
	case DSERR_NOAGGREGATION: 
		v=3;
		break;
	case DSERR_OUTOFMEMORY:
		v=4;
		break;
	case DSERR_UNINITIALIZED:
		v=5;
		break;
	case DSERR_UNSUPPORTED:
		v=6;
		break;
	case DS_OK:
		v=7;
		break;
	};
	*/
	WAVEFORMATEX FR;
	DWORD rd;
	hr=lpDsb->GetFormat(&FR,sizeof FR,&rd);
	FR.wBitsPerSample=16;
	FR.nAvgBytesPerSec=88200;
	FR.nBlockAlign=4;
	FR.nChannels=2;
	FR.nSamplesPerSec=22050;
	hr=lpDsb->SetFormat(&FR);
	memset(BufIsRun,0,sizeof BufIsRun);
}

///////////////////////////////////////////////////////////
// CDirSound::~CDirSound()
//
// This is the class's destructor.
///////////////////////////////////////////////////////////
CDirSound::~CDirSound()
{
    if (m_pDirectSoundObj != NULL) ReleaseAll();
}

///////////////////////////////////////////////////////////
// CDirSound::ReleaseAll()
//
// This member function releases all the DirectSound
// objects created in the class.
///////////////////////////////////////////////////////////
void CDirSound::ReleaseAll()
{
    // Release all sound buffers.
	for (UINT x=1; x<=m_currentBufferNum; ++x){
		if(m_bufferPointers[x])m_bufferPointers[x]->Release();
		m_SoundNames[x]=NULL;
		RiseUpTime[x]=0;
		FallDnTime[x]=1000;
		LastHitTime[x]=GetTickCount()-100000;
		LoopedSound[x]=0;
	}

    // Release the DirectSound object.
    if (m_pDirectSoundObj != NULL)
        m_pDirectSoundObj->Release();
	memset(BufIsRun,0,sizeof BufIsRun);
	ClearSoundCategories();
}

void CDirSound::ValidateSoundBuffer(UINT bufferNum){
	if(!m_bufferPointers[bufferNum]){
        char* cname=m_SoundNames[bufferNum];
		if(cname){
			for(int q=bufferNum+1;q<MAXSND;q++){
				if(m_SoundNames[q]&&!strcmp(m_SoundNames[q],cname)){
					ValidateSoundBuffer(q);				
					DuplicateSoundBuffer(bufferNum,q);
					return;
				}
			}
			CWave wave(cname);
			if(wave.WaveOK()){
				CreateSoundBuffer(&wave,bufferNum);
				CopyWaveToBuffer(&wave,bufferNum);
			}else{
				Log.Warning("unable to load sound: %s",cname);
			}
		}
	}
}
UINT CDirSound::AddNewSound(const char* name){
	if (m_currentBufferNum == MAXSND)
        return 0;
    // Calculate the next available buffer number.
    ++m_currentBufferNum;
    m_SoundNames[m_currentBufferNum]=new char[strlen(name)+1];
	strcpy(m_SoundNames[m_currentBufferNum],name);
	m_bufferPointers[m_currentBufferNum]=NULL;
	return m_currentBufferNum;
}
///////////////////////////////////////////////////////////
// CDirSound::CreateSoundBuffer()
//
// This member function creates secondary sound buffers.
// The class can accommodate up to MAXSND such buffers.
///////////////////////////////////////////////////////////
void CDirSound::CreateSoundBuffer(CWave* pWave,UINT bufferNum)
{
    DSBUFFERDESC dsBufferDesc;
    HRESULT hResult;

    // Make sure there's room for another buffer.
    //if (m_currentBufferNum == MAXSND)
    //    return 0;

    // Calculate the next available buffer number.
    //++m_currentBufferNum;

    // Initialize the DSBUFFERDESC structure.
    LPWAVEFORMATEX pWaveFormat = pWave->GetWaveFormatPtr();
    memset(&dsBufferDesc, 0, sizeof(DSBUFFERDESC));
    dsBufferDesc.dwSize = sizeof(DSBUFFERDESC);
    dsBufferDesc.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
    dsBufferDesc.dwBufferBytes = pWave->GetWaveSize();
    dsBufferDesc.lpwfxFormat = (LPWAVEFORMATEX) pWaveFormat;

    // Create the secondary sound buffer.
    hResult = m_pDirectSoundObj->CreateSoundBuffer(&dsBufferDesc,
        &m_bufferPointers[bufferNum], NULL);
    if (hResult != DS_OK)
        return;

    // Save the buffer size.
    m_bufferSizes[bufferNum] = dsBufferDesc.dwBufferBytes;

    //return m_currentBufferNum;
}
///////////////////////////////////////////////////////////
// CDirSound::DuplicateSoundBuffer()
//
// This member function creates secondary sound buffers.
// The class can accommodate up to MAXSND such buffers.
///////////////////////////////////////////////////////////
void CDirSound::DuplicateSoundBuffer(UINT bufferNum,UINT srcBuffer)
{
    HRESULT hResult;

    // Make sure there's room for another buffer.
    //if (m_currentBufferNum == MAXSND)
    //    return 0;

    // Calculate the next available buffer number.
    //++m_currentBufferNum;
	hResult = m_pDirectSoundObj->DuplicateSoundBuffer(m_bufferPointers[srcBuffer],&m_bufferPointers[bufferNum]);
    if (hResult != DS_OK)return;
    m_bufferSizes[bufferNum] = m_bufferSizes[srcBuffer];

    //return m_currentBufferNum;
}
///////////////////////////////////////////////////////////
// CDirSound::CopyWaveToBuffer()
//
// This function copies wave data to a DirectSound buffer.
///////////////////////////////////////////////////////////
BOOL CDirSound::CopyWaveToBuffer(CWave* pWave, UINT bufferNum)
{
    LPVOID pSoundBlock1;
    LPVOID pSoundBlock2;
    DWORD bytesSoundBlock1;
    DWORD bytesSoundBlock2;
    HRESULT result;

    // Check for a valid buffer number.
    if ((bufferNum > m_currentBufferNum) || (bufferNum == 0))
        return FALSE;

    // Get a pointer to the requested buffer.
    LPDIRECTSOUNDBUFFER pSoundBuffer = 
        m_bufferPointers[bufferNum];

    // Lock the buffer.
    result = pSoundBuffer->Lock(0, m_bufferSizes[bufferNum],
        &pSoundBlock1, &bytesSoundBlock1,
        &pSoundBlock2, &bytesSoundBlock2, 0);
    if (result != DS_OK)
        return FALSE;

    // Copy the data into the buffer.
    char* pWaveData = pWave->GetWaveDataPtr();
    DWORD waveSize = pWave->GetWaveSize();
    memcpy((void*)pSoundBlock1, pWaveData, waveSize);

    // Unlock the buffer.
    pSoundBuffer->Unlock(pSoundBlock1, bytesSoundBlock1,
            pSoundBlock2, bytesSoundBlock2);
	pSoundBuffer->GetFrequency(SoundFreq+bufferNum);

    return TRUE;
}

///////////////////////////////////////////////////////////
// CDirSound::DirectSoundOK()
//
// This member function returns TRUE if the DirectSound
// object was created and initialized okay. Otherwise, it
// returns FALSE.
///////////////////////////////////////////////////////////
BOOL CDirSound::DirectSoundOK()
{
    if (m_pDirectSoundObj == NULL)
        return FALSE;

    return TRUE;
}

///////////////////////////////////////////////////////////
// CDirSound::PlaySound()
//
// This member function plays the sound stored in the given
// buffer.
///////////////////////////////////////////////////////////
BOOL CDirSound::PlaySound(UINT bufferNum,DWORD Flags)
{
	extern bool GecOptimCDirSound;
	if(GecOptimCDirSound) return FALSE;
	ValidateSoundBuffer(bufferNum);
	HRESULT result;
	//checking category of sound
	//MarkSoundLikePlaying(bufferNum,0);	
    // Check for a valid buffer number.
    if ((bufferNum > m_currentBufferNum) || (bufferNum == 0))
        return FALSE;
	if(LoopedSound[bufferNum]&&!(Flags&1))return true;

    // Get a pointer to the requested buffer.
    LPDIRECTSOUNDBUFFER pSoundBuffer = m_bufferPointers[bufferNum];
	if(!pSoundBuffer)return false;

    // Make sure the buffer is set to play from the beginning.
    result = pSoundBuffer->SetCurrentPosition(0);
    if (result != DS_OK)
        return FALSE;

    // Play the sound.
    result = pSoundBuffer->Play(0, 0, Flags);
    if (result != DS_OK)
        return FALSE;
	BufIsRun[bufferNum]=1;	
	int v[]={97,100,103};
    int V=v[rand()%3];
	pSoundBuffer->SetFrequency(SoundFreq[bufferNum]*V/100);
    return TRUE;
}
void CDirSound::MarkSoundLikePlaying(UINT bufferNum,int x){	
	extern bool GecOptimCDirSound;
	if(GecOptimCDirSound) return;
	ValidateSoundBuffer(bufferNum);
	byte ctg=SoundCtg[bufferNum];
	if(ctg){
		//CurrSoundCtgFreq[ctg]++;
		int fr=SoundCtgFreq[ctg]/11;
		if(fr>StartGroupFreq[ctg]){
            /*
			LPDIRECTSOUNDBUFFER pSoundBuffer = m_bufferPointers[bufferNum];
			LONG v=0;
			pSoundBuffer->GetVolume(&v);
			LONG v0=v;
			int D=FinalGroupFreq[ctg]-StartGroupFreq[ctg];
			if(D){
				if(fr<FinalGroupFreq[ctg])D=100*(fr-StartGroupFreq[ctg])/D;
				v-=D*50;
				pSoundBuffer->SetVolume(v);
			};
            */
			//seeking for a free group sound
			int NS=CtgNSounds[ctg];
			int LastPIdx=-1;
			if(NS){
				for(int idx=0;idx<NS;idx++){
					int bfid=CtgSoundID[ctg][idx];
					int DD=(FinalGroupFreq[ctg]-StartGroupFreq[ctg]);
					int freq=StartGroupFreq[ctg]+idx*DD;
					if(fr>freq&&fr<freq+DD*4){
						LastPIdx=bfid;
						if(!IsPlaying(bfid)){
							SetPan(bfid,0);
							SetVolume(bfid,-10000);
							PlaySound(bfid,DSBPLAY_LOOPING);
							//NATT=100;
							if(x){
								BufIsRun[bfid]=1;
								SrcX[bfid]=x;
								SrcY[bfid]=0;
							}
							//goto sp_end;
						}
					}
				}
				if(x&&LastPIdx!=-1){
					SrcX[LastPIdx]=(x+SrcX[LastPIdx]*15)>>4;
				};
			};
		};
		if(fr>FinalGroupFreq[ctg])goto sp_end;
	};
sp_end:;
	if(LoopedSound[bufferNum]){
		LastHitTime[bufferNum]=GetTickCount();
		SrcX[bufferNum]=(SrcX[bufferNum]*20+x)/21;
		SrcY[bufferNum]=0;
		return;
	}
};
BOOL CDirSound::PlayCoorSound(UINT bufferNum,int x,int vx)
{
    extern bool GecOptimCDirSound;
	if(GecOptimCDirSound) return FALSE;
	HRESULT result;
	ValidateSoundBuffer(bufferNum);
	//checking category of sound
	MarkSoundLikePlaying(bufferNum,x);
	if(LoopedSound[bufferNum])return true;
    // Check for a valid buffer number.
    if ((bufferNum > m_currentBufferNum) || (bufferNum == 0))
        return FALSE;

    // Get a pointer to the requested buffer.
    LPDIRECTSOUNDBUFFER pSoundBuffer = m_bufferPointers[bufferNum];
	if(!pSoundBuffer)return false;

    // Make sure the buffer is set to play from the beginning.
    result = pSoundBuffer->SetCurrentPosition(0);
    if (result != DS_OK)
        return FALSE;

    // Play the sound.
    result = pSoundBuffer->Play(0, 0, 0);
    if (result != DS_OK)
        return FALSE;
	BufIsRun[bufferNum]=1;
	SrcX[bufferNum]=x;
	SrcY[bufferNum]=vx;
    return TRUE;
}
extern int CenterX;
void CDirSound::ControlPan(UINT bufferNum){
	extern bool GecOptimCDirSound;
	if(GecOptimCDirSound) return;
	if(BufIsRun[bufferNum]){
		ValidateSoundBuffer(bufferNum);
		SrcX[bufferNum]+=SrcY[bufferNum];
        /*
		int pan=(SrcX[bufferNum]-CenterX)<<1;
		if(pan<-1700)pan=-1700;
		if(pan>1700)pan=1700;
		if(SrcX[bufferNum]==0)pan=0;
		SetPan(bufferNum,pan);
        */
		if(rand()<1024)IsPlaying(bufferNum);
	};
};
void CDirSound::SetVolume(UINT bufferNum,int vol){
	extern bool GecOptimCDirSound;
	if(GecOptimCDirSound) return;
	// Check for a valid buffer number.
    if ((bufferNum > m_currentBufferNum) || (bufferNum == 0))
        return;
	ValidateSoundBuffer(bufferNum);

    // Get a pointer to the requested buffer.
    LPDIRECTSOUNDBUFFER pSoundBuffer = m_bufferPointers[bufferNum];
	if(!pSoundBuffer)return;

    // Make sure the buffer is set to play from the beginning.
    if(pSoundBuffer)pSoundBuffer->SetVolume(vol);
};
void CDirSound::SetPan(UINT bufferNum,int pan){
	extern bool GecOptimCDirSound;
	if(GecOptimCDirSound) return;
	// Check for a valid buffer number.
    if ((bufferNum > m_currentBufferNum) || (bufferNum == 0))
        return;
	ValidateSoundBuffer(bufferNum);
    // Get a pointer to the requested buffer.
    LPDIRECTSOUNDBUFFER pSoundBuffer = m_bufferPointers[bufferNum];
	if(!pSoundBuffer)return;

    // Make sure the buffer is set to play from the beginning.
    if(Pan[bufferNum]!=pan)pSoundBuffer->SetPan(pan);
    Pan[bufferNum]=pan;
};
extern int TIME1;
int prevt0=0;
int NCCL=0;
extern int WarSound;
void CDirSound::ProcessSoundSystem(){
	extern bool GecOptimCDirSound;
	if(GecOptimCDirSound) return;
	int maxfr=0;
	int TC=GetTickCount();
	for(int i=0;i<MAXSND1;i++){
		if(BufIsRun[i]){
			ControlPan(i);
			if(SoundForCtg[i]){
				ValidateSoundBuffer(i);
				int ctg=SoundForCtg[i];
				int fr=SoundCtgFreq[ctg]/11;
				int idx=IndexInGroup[i];
				int DF=FinalGroupFreq[ctg]-StartGroupFreq[ctg];
                int start=StartGroupFreq[ctg]+idx*DF;
				//if(fr>maxfr)maxfr=fr;
				LPDIRECTSOUNDBUFFER pSoundBuffer = m_bufferPointers[i];
				if(pSoundBuffer){
					LONG v=0;
					pSoundBuffer->GetVolume(&v);
					int dv=abs(v)/20;
					dv=dv*GameSpeed/256;
					if(dv<10)dv=10;
					int vmax=0;
					short GetVolByFloat01(float Vol);
					float VOLF=float(fr-start)/DF;
					if(VOLF<0.1)VOLF=0.1;
					if(VOLF>1)VOLF=1;				
					vmax=-GetVolByFloat01(VOLF);				
					int v0=-(100-GSets.SVOpt.SoundVolume)*40;
					v0-=Volume[i];
					vmax+=v0;
					if(fr>start){
						dv*=30;
						if(v<-8000)v=-8000;
						if(v<vmax)v+=dv;
						else v=vmax;
					}else{
						if(fr==0)v-=dv*10;
						if(v>-10000)v-=dv;
					};
					if(v>vmax)v=vmax;				
					//if(v>v0)v=v0;
					if(v<=-8000){
						StopSound(i);
					}else SetVolume(i,v);
				}
			}else
			if(SoundCtg[i]&&!LoopedSound[i]){
				/*
				int ctg=SoundCtg[i];
				int fr=SoundCtgFreq[ctg]*2;
				if(fr>StartGroupFreq[ctg]){
					LPDIRECTSOUNDBUFFER pSoundBuffer = m_bufferPointers[i];
					LONG v=0;
					pSoundBuffer->GetVolume(&v);
					LONG v0=v;
					int D=FinalGroupFreq[ctg]-StartGroupFreq[ctg];
					if(D){
						if(fr<FinalGroupFreq[ctg])D=100*(fr-StartGroupFreq[ctg])/D;
						else D=100;
						v-=D*50;
						pSoundBuffer->SetVolume(v);
					}
				}
				*/
			}else
			if(LoopedSound[i]){
                int v0=-(100-GSets.SVOpt.SoundVolume)*40-Volume[i];
				LPDIRECTSOUNDBUFFER pSoundBuffer = m_bufferPointers[i];
				if(pSoundBuffer){
					LONG v=0;
					pSoundBuffer->GetVolume(&v);
					int DT=TC-LastHitTime[i];
					int DTM=(FallDnTime[i]*4000)>>16;
					if(DTM<2)DTM=2;
					if(DT<DTM/2){
						if(v<v0)v+=10000/((RiseUpTime[i]>>8)+1);
						if(v>v0)v=v0;
					}else{
						v0=v0-((8000+v0)*(DT-DTM/2)*2/DTM);
						if(v0<-10000)v0=-10000;
						if(v>v0)v-=100;
						if(v<v0)v=v0;
					}
					int ctg=SoundCtg[i];
					if(ctg){
						int fr=SoundCtgFreq[ctg]/11;
						if(fr>StartGroupFreq[ctg]){
							int D=FinalGroupFreq[ctg]-StartGroupFreq[ctg];
							if(D){
								if(fr<FinalGroupFreq[ctg])D=100*(fr-StartGroupFreq[ctg])/D;
								else D=100;
								v-=D*50;							
							}
						}
					}
					if(v<-7000){
						StopSound(i);
						BufIsRun[i]=0;
					}else{
						SetVolume(i,v);
					}
				}
			}
		}else if(LoopedSound[i]&&!SoundForCtg[i]){
			int DTM=(FallDnTime[i]*4000)>>16;
			if(DTM<2)DTM=2;
			if(TC-LastHitTime[i]<DTM){
				PlaySound(i,DSBPLAY_LOOPING);
				SetVolume(i,-5000);
			}
		}
	}
	for(i=0;i<256;i++){
		int fr=SoundCtgFreq[i]/11;
		if(fr>maxfr)maxfr=fr;
	};
	//processing group sounds
	/*
	int DT=GetTickCount()-LastDecTime;
	if(DT>200){
		if(DT>500)DT=500;
		DT=(DT*DECFACTOR)/8000;
		for(i=0;i<256;i++)if(SoundCtgFreq[i])SoundCtgFreq[i]-=(int(SoundCtgFreq[i])*DT)/1000;
		TIME1=SoundCtgFreq[1]/1000;
	};
	*/
	NCCL++;//=GameSpeed;
	for(int i=0;i<256;i++)if(CurrSoundCtgFreq[i]>SoundCtgFreq[i])SoundCtgFreq[i]=CurrSoundCtgFreq[i];
	if(NCCL>10){//256*15){
		NCCL=0;
		memcpy(SoundCtgFreq,CurrSoundCtgFreq,1024);
		memset(CurrSoundCtgFreq,0,1024);
	};
	TIME1=maxfr;
};
///////////////////////////////////////////////////////////
// CDirSound::StopSound()
//
// This member function stops the sound stored in the given
// buffer.
///////////////////////////////////////////////////////////
BOOL CDirSound::StopSound(UINT bufferNum)
{
    HRESULT result;

    // Check for a valid buffer number.
    if ((bufferNum > m_currentBufferNum) || (bufferNum == 0))
        return FALSE;

    // Get a pointer to the requested buffer.
    LPDIRECTSOUNDBUFFER pSoundBuffer = m_bufferPointers[bufferNum];
	if(!pSoundBuffer)return FALSE;

    // Make sure the buffer is set to play from the beginning.
    result = pSoundBuffer->Stop();
    if (result != DS_OK)
        return FALSE;

    return TRUE;
};
int CDirSound::GetPos(UINT bufferNum)
{
    HRESULT result;

    // Check for a valid buffer number.
    if ((bufferNum > m_currentBufferNum) || (bufferNum == 0))
        return 0;

    // Get a pointer to the requested buffer.
    LPDIRECTSOUNDBUFFER pSoundBuffer = m_bufferPointers[bufferNum];
	if(!pSoundBuffer)return 0;

    // Make sure the buffer is set to play from the beginning.
	DWORD pos;
    result = pSoundBuffer->GetCurrentPosition(&pos,NULL);
    return pos;

}
bool CDirSound::IsPlaying(UINT bufferNum)
{
    HRESULT result;

    // Check for a valid buffer number.
    if ((bufferNum > m_currentBufferNum) || (bufferNum == 0))
        return 0;

    // Get a pointer to the requested buffer.
    LPDIRECTSOUNDBUFFER pSoundBuffer = m_bufferPointers[bufferNum];
	if(!pSoundBuffer)return false;

    // Make sure the buffer is set to play from the beginning.
	LONG v;
	result = pSoundBuffer->GetVolume(&v);
	DWORD pos;
    result = pSoundBuffer->GetStatus(&pos);
	

	bool play=pos&DSBSTATUS_PLAYING;
	if(!play)BufIsRun[bufferNum]=0;
	return play;
}
extern CDirSound CDIRSND;
void StopLoopSounds(){
	CDIRSND.StopCtgSounds();
	for(int i=0;i<MAXSND1;i++)if(CDIRSND.BufIsRun[i]){
		//if(CDIRSND.SoundForCtg[i]){
			LPDIRECTSOUNDBUFFER pSoundBuffer = CDIRSND.m_bufferPointers[i];
			if(pSoundBuffer)pSoundBuffer->Stop();
		//};
	};
};
void PrintSoundInfo(int x,int y){
	char s[1024];
	s[0]=0;
	for(int i=0;i<10;i++){
		sprintf(s+strlen(s)," %d:%d [%d-%d]",i,CDIRSND.SoundCtgFreq[i]/11,CDIRSND.StartGroupFreq[i],CDIRSND.FinalGroupFreq[i]);		
	}
	ShowString(x,y,s,&SmallWhiteFont);
}
void CDirSound::HitSound(int SoundID){
	extern word SndTable[MaxSnd][16];
	if( SoundID>=0 && SoundID<MaxSnd ){
		int sid=SndTable[SoundID][0];
		if(sid>=0&&sid<MAXSND){
			byte ctg=SoundCtg[sid];
			if(ctg)CurrSoundCtgFreq[ctg]++;
		}
	}
}