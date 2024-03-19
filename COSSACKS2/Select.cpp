#include "stdheader.h"

extern word SelCenter[8];
SelGroup SelSet[80];
SelGroup::SelGroup(){
	NMemb=0;
	Member=NULL;
	SerialN=NULL;
	CanMove=true;
	CanSearchVictim=true;
	CanHelpToFriend=true;
	Egoizm=false;
};
void SelGroup::CreateFromSelection(byte NI){
	if(int(Member))DeleteMembers(NI);
	Member=znew(word,NSL[NI]);
	SerialN=znew(word,NSL[NI]);
	word Nsel=NSL[NI];
	memcpy(Member,Selm[NI],Nsel<<1);
	memcpy(SerialN,SerN[NI],Nsel<<1);
	int k;
	word MID;
	OneObject* OB;
	for(k=0;k<Nsel;k++){
		MID=Member[k];
		if(k!=0xFFFF){
			OB=Group[k];
			if(OB){
				//if(OB->GroupIndex){
				//	NG1=OB->GroupIndex->NMemb;
				//	mem1=OB->GroupIndex->Member;
				//	for(int i=0;i<NG1;i++)
				//		if(mem1[i]==MID)mem1[i]=0xFFFF;
				//};
				//OB->Egoist=false;
				OB->GroupIndex=NULL;//this;
			};
		};
	};
	NMemb=Nsel;
};
void SelGroup::DeleteMembers(byte NI){
	word MID;
	OneObject* OB;
	//word Nsel=NSL[NI];
	//word * SMon=Selm[NI];
	for(int k=0;k<NMemb;k++){
		MID=Member[k];
		if(k!=0xFFFF){
			OB=Group[k];
			if(OB){
				//OB->Egoist=false;
				OB->GroupIndex=NULL;
			};
		};
	};
	if(int(Member))free(Member);
	if(int(SerialN))free(SerialN);
	SelGroup();
};
void SelGroup::SelectMembers(byte NI,bool Shift){
	SelCenter[NI]=0;
	word MID;
	OneObject* OB;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	word* ser=SerN[NI];
	word NR=0;
	byte NeedNI=GSets.CGame.cgi_NatRefTBL[NI];
	if(!Shift){
		for(int i=0;i<Nsel;i++){
			MID=SMon[i];
			if(MID!=0xFFFF){
				OB=Group[MID];
				if(OB&&OB->Serial==ser[i]&&!OB->Sdoxlo){
					//OB->Egoist=false;
					OB->Selected&=~GM(NI);
				};
			};
		};
		if(Nsel){
			free(SMon);
			free(ser);
		};
		SMon=znew(word,NMemb);
		ser=znew(word,NMemb);
	}else{
		NR=NSL[NI];
		int N1=NMemb+NR;
		SMon=(word*)realloc(SMon,N1<<1);
		ser=(word*)realloc(ser,N1<<1);
	};
	Selm[NI]=SMon;
	SerN[NI]=ser;
	for(int i=0;i<NMemb;i++){
		MID=Member[i];
		if(MID!=0xFFFF){
			OB=Group[MID];
			if(int(OB)&&OB->Serial==SerialN[i]&&OB->NNUM==NeedNI&&!OB->Sdoxlo){
				if(!(OB->Selected&GM(NI))){
					SMon[NR]=MID;
					ser[NR]=SerialN[i];
					OB->Selected|=GM(NI);
					NR++;
				};
			};
		};
	};
	NSL[NI]=NR;
	addrand(NSL[NI]+1);
};
void SelGroup::ImSelectMembers(byte NI,bool Shift){
	SelCenter[NI]=0;
	word MID;
	OneObject* OB;
	word Nsel=ImNSL[NI];
	word* SMon=ImSelm[NI];
	word* ser=ImSerN[NI];
	word NR=0;
	byte NeedNI=GSets.CGame.cgi_NatRefTBL[NI];
	if(!Shift){
		for(int i=0;i<Nsel;i++){
			MID=SMon[i];
			if(MID!=0xFFFF){
				OB=Group[MID];
				if(OB&&OB->Serial==ser[i]&&!OB->Sdoxlo){
					//OB->Egoist=false;
					OB->ImSelected&=~GM(NI);
				};
			};
		};
		if(Nsel){
			free(SMon);
			free(ser);
		};
		SMon=znew(word,NMemb);
		ser=znew(word,NMemb);
	}else{
		NR=ImNSL[NI];
		int N1=NMemb+NR;
		SMon=(word*)realloc(SMon,N1<<1);
		ser=(word*)realloc(ser,N1<<1);
	}
	ImSelm[NI]=SMon;
	ImSerN[NI]=ser;
	for(int i=0;i<NMemb;i++){
		MID=Member[i];
		if(MID!=0xFFFF){
			OB=Group[MID];
			if(OB&&OB->Serial==SerialN[i]&&OB->NNUM==NeedNI&&!OB->Sdoxlo){
				if(!(OB->ImSelected&GM(NI))){
					SMon[NR]=MID;
					ser[NR]=SerialN[i];
					OB->ImSelected|=GM(NI);
					NR++;
				};
			};
		};
	};
	ImNSL[NI]=NR;
};
inline bool PxInside(int x,int y,int x1,int y1,int xp,int yp){
	if(xp>=x&&xp<=x1&&yp>=y&&yp<=y1)return true;
	else return false;
};
