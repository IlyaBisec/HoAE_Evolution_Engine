class _WaveFileName:public _str{
public:
	AUTONEW(_WaveFileName);
	virtual const char* GetClassName(){
		return "_WaveFileName";
	}
};
class _WaveFileName_editor:public OneElementEditor{
public:
	char str[256];
	virtual const char* GetClassID(){return "_WaveFileName";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
			InputBox* IB=Base->addInputBox(NULL,x,y,str,120,x1-x+1,y1-y+1,&CED_Font,&CED_AFont);
			IB->Enabled=0;
			SimpleDialog* SD=Base->addViewPort(x,y,x1-x+1,y1-y+1);
			SD->OnUserClick=&LS_gp_UserClick;
			LS_gp_param* p=new LS_gp_param;
			p->destname=str;
			p->ext="*.wav";
			p->startdir="Sound\\";
			p->CE=CParam->CE;
			SD->AllocPtr=(char*)p;
			return y1;
		}
		virtual bool Assign(xmlQuote* xml){
			if(xml->Get_string())strcpy(str,xml->Get_string());
			else str[0]=0;
			return true;
		}
		virtual int Get(xmlQuote* xml){
			xml->Assign_string(str);
			return true;
		}
		virtual OneElementEditor* _new(){
			return (OneElementEditor*)new _WaveFileName_editor;
		}
};
DWORD playWAVEFile(HWND hWndNotify, LPSTR lpszWAVEFileName);
class OneSound:public BaseClass{	
public:
	int SoundID;
	//
	_WaveFileName WaveFile;
	float Volume;	
	bool Looped;
	float StartSharpness;
	float EndSharpness;
	bool SlightlyChangeFrequency;
	class PlayWave:public BaseFunction{
	public:
		void EvaluateFunction(){
			OneSound* OS=get_parent<OneSound>();
			if(OS){
				playWAVEFile(hwnd,OS->WaveFile.pchar());
			}
		}
	};
	PlayWave Play;
	OneSound();
	SAVE(OneSound);
	SAVE_SECTION(1);
	REG_AUTO(WaveFile);
	REG_AUTO(Play);
	REG_MEMBER(_float01,Volume);
	REG_MEMBER(_bool,Looped);
	SAVE_SECTION(2);
	REG_MEMBER(_float01,StartSharpness);
	REG_MEMBER(_float01,EndSharpness);	

	ENDSAVE;
	DWORD GetClassMask(){
		if(Looped)return 3;
		else return 1;
	}
};
class GroupSoundRef:public BaseClass{
	int GroupIndex;
public:
	_str GroupSoundName;
	int StartIntensivity;
	int EndIntensivity;
	SAVE(GroupSoundRef);
	REG_AUTO(GroupSoundName);
	REG_MEMBER(_int,StartIntensivity);
	REG_MEMBER(_int,EndIntensivity);
	ENDSAVE;
};
class WSLIST:public ClonesArray<OneSound>{
	const char* GetEditableElementView(int Index,void* DataPtr,const char* LocalName,DWORD Option);
};
class SoundsGroup:public BaseClass{
public:
	_str  GroupName;
	float Volume;
	int   MinPause;
	int   PauseVariation;
	bool  DangerousSound;
	bool  DontPlayInDangerousSituation;
	float PanningDegree;
	float DistanceFadeDegree;
	int   Usage;
	int   GroupSoundID;	
	int   GroupSoundStartFrequency;
	int   GroupSoundEndFrequency;
    _str GroupSoundName;	
	WSLIST List;
	__constructor(SoundsGroup){	
		Volume=1.0f;
		Usage=0;
		DistanceFadeDegree=0.5;
		PanningDegree=0.5;
	}
	SAVE(SoundsGroup);
	SAVE_SECTION(1);
	REG_AUTO(GroupName);
	REG_MEMBER(_float01,Volume);
	REG_MEMBER_EX(_int,MinPause,MinimalPause(sec));
	REG_MEMBER_EX(_int,PauseVariation,PauseVariation(sec));
	REG_MEMBER(_bool,DangerousSound);
	REG_MEMBER(_bool,DontPlayInDangerousSituation);
	REG_MEMBER(_float01,PanningDegree);
	REG_MEMBER(_float01,DistanceFadeDegree);
	REG_ENUM(_index,Usage,SoundUsage);
	SAVE_SECTION(2);
	REG_ENUM(_index,GroupSoundID,SoundGroup);

	SAVE_SECTION(4);
	REG_MEMBER(_int,GroupSoundStartFrequency);
	REG_MEMBER(_int,GroupSoundEndFrequency);
	REG_ENUM(_index,Usage,SoundUsage);

	SAVE_SECTION(1);
	REG_AUTO(List);

	ENDSAVE;

	DWORD GetClassMask(){
		if(Usage==1)return 1+2;
		if(Usage==2)return 1+2+4;
		return 1;
	}
};
class VSLIST:public ClonesArray<SoundsGroup>{
	const char* GetEditableElementView(int Index,void* DataPtr,const char* LocalName,DWORD Option);
};
class AllSounds:public BaseClass{
public:
	VSLIST SoundsList;
	SAVE(AllSounds);
	//-classes registration-
	REG_CLASS(GroupSoundRef);
	REG_CLASS(OneSound);
	REG_CLASS(SoundsGroup);
	//members registration
	REG_AUTO(SoundsList);
	ENDSAVE;
	bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra);	
};
extern AllSounds VisualSound;
//
int GetSound(char* Name);
extern char* SoundID[MaxSnd];
extern word NSounds;
//
class SoundsEnumerator: public ProcEnumerator{
public:
	virtual DWORD GetValue(const char* ID){
		if(!strcmp(ID,"NONE")) return 0xFFFFFFFF;
		return GetSound((char*)ID);
	}
	virtual char* GetValue(DWORD ID){
		if(ID==0xFFFFFFFF) return "NONE";
		if(ID<NSounds){
			return SoundID[ID];
		}
		return "NONE";
	}
	virtual int GetAmount(){
		return NSounds+1;
	}
	virtual char* GetIndexedString(int idx){		
		return GetValue(idx-1);
	}
	virtual DWORD GetIndexedValue (int idx){
		return idx-1;
	}	
};
 