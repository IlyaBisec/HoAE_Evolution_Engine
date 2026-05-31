#include "stdheader.h"
#include "supereditor.h"
#include "mp3\oggvor.h"
//==================================================================================================================//
extern ActiveObjectIndexses  ActiveIndexses;
//==================================================================================================================//
DString ComListDS[64];
//#define FirstOgg_Stream 2
int FirstOgg_Stream=2;
//#define NOgg_Stream 4
#define NOgg_Stream 1
//==================================================================================================================//
DString Ogg_Stream_FileName[NOgg_Stream];
//==================================================================================================================//
void PlayOggSound(DynArray<DString*>* CommList)
{
	if(CommList)
	{
		int N = CommList->GetAmount();
		if(N)
		{
			//N=rand()%N;
			//bool played=false;
			{
				static DString History;
				for(int j=0;j<N&&N>1;j++){
					bool del=false;					
					if(History==*(*CommList)[j]){
						// in history
						del=true;
					}else{
						// now played
						for(int i=0;i<NOgg_Stream;i++){
							if(ov_StreamFinished(FirstOgg_Stream+i)){
								Ogg_Stream_FileName[i]="";
							}else{
								if(*(*CommList)[j]==Ogg_Stream_FileName[i]){
									del=true;
									break;
								}
							}
						}
					}
					if(del){
						CommList->Del(j,1);
						j--;
						N--;
					}
				}
				N = CommList->GetAmount();
				N = rand()%N;
				for(int i=0;i<NOgg_Stream;i++)
				{
					if(ov_StreamFinished(FirstOgg_Stream+i))
					{
						Ogg_Stream_FileName[i]=(*CommList)[N]->pchar();							
						static int volume=-1;
						//if(volume!=GSets.SVOpt.SoundVolume){
							volume=GSets.SVOpt.SoundVolume*EngSettings.vInterf.VolumeSoundEvents/100;
							ov_SetVolume(volume,FirstOgg_Stream+i);
						//}						
						ov_Play((*CommList)[N]->pchar(),FirstOgg_Stream+i);
						History=Ogg_Stream_FileName[i];
						break;
					}
				}
			}
		}
	}
}
int GetVideoCommand(OneScript* OS,char** ComList,char* CurrentBik)
{
	//if(ActiveIndexses.BrigadeID==-1)return 0;
	int N = ActiveIndexses.OggSoundComList.GetAmount();
	for(int i=0;i<N;i++)
		if(ActiveIndexses.OggSoundComList[i])
			delete (ActiveIndexses.OggSoundComList[i]);
	ActiveIndexses.OggSoundComList.Clear();

	ActiveIndexses.ComList=ComList;
	ActiveIndexses.NCom=0;
	ActiveIndexses.CurrentPlayingBik=CurrentBik;
	OS->Script.Execute();
	ActiveIndexses.BrigadeID=-1;
	ActiveIndexses.NI=0;

	FirstOgg_Stream=2;
	PlayOggSound(&ActiveIndexses.OggSoundComList);
	//
	return ActiveIndexses.NCom;
}
void ExecSoundEvents(){
	int N = ActiveIndexses.OggSoundComList.GetAmount();
	for(int i=0;i<N;i++)
		if(ActiveIndexses.OggSoundComList[i])
			delete (ActiveIndexses.OggSoundComList[i]);
	ActiveIndexses.OggSoundComList.Clear();
	//
	extern OneScript SoundEvents;
	SoundEvents.Script.Execute();
	//
	FirstOgg_Stream=2; //3;
	PlayOggSound(&ActiveIndexses.OggSoundComList);
}
//==================================================================================================================//
void PlayVideo::GetViewMask(DString& ST)
{
	ST.Add("PlayVideo(<0>)"); 
}
int PlayVideo::GetNArguments()
{
	return 1;
}
Operand* PlayVideo::GetArgument(int NIndex)
{
	Operand* rez=NULL;
	if(NIndex==0)
		rez = (Operand*)Name.Get();
	return rez;
}
void PlayVideo::SetArgument(int index, Operand* Op)
{
	if(index==0&&Op)
	{
		Name.Set((StringReturner*)Op);
	}
}
bool PlayVideo::Execute()
{
	bool rez=false;
	if(Name.Get())
	{
		StringType ST;
		if(Name.Get()->Get(&ST))
		{
			if(ActiveIndexses.NCom<64)
			{
				ComListDS[ActiveIndexses.NCom].Clear();
				ComListDS[ActiveIndexses.NCom].Add(ST.String.pchar());
				if(ActiveIndexses.ComList)
				{
					ActiveIndexses.ComList[ActiveIndexses.NCom]=ComListDS[ActiveIndexses.NCom].pchar();
					ActiveIndexses.NCom++;
				}
			}
			rez=true;
		}
	}
	return rez;
}
const char* PlayVideo::GetProcedureName()
{
	return "PlayVideo";
}
//==================================================================================================================//
int CPlaySound::GetNArguments()
{
	return 1;
}
Operand* CPlaySound::GetArgument(int NIndex)
{
	Operand* rez=NULL;
	if(NIndex==0)
		rez = (Operand*)Name.Get();
	return rez;
}
void CPlaySound::SetArgument(int index, Operand* Op)
{
	if(index==0&&Op)
	{
		Name.Set((StringReturner*)Op);
	}
}
bool CPlaySound::Execute()
{
	bool rez=false;
	if(Name.Get())
	{
		StringType ST;
		if(Name.Get()->Get(&ST))
		{
			DString* St = new DString();
			St->Add(ST.String.pchar());
			ActiveIndexses.OggSoundComList.Add(St);
			rez=true;
		}
	}
	return rez;
}
const char* CPlaySound::GetProcedureName()
{
	return "PlaySound";
}
//==================================================================================================================//
