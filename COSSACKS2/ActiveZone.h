#pragma once
#ifndef ActiveZone_header
#define ActiveZone_header
//////////////////////////////////////////////////////////////////////////

#include "ZonesAndGroupsExtraInfo.h"
//#include "ClassEditor.h"

class ActiveZone:public BaseClass{
public:
	int x,y,R;
	_str Name;
	byte Dir;
	ActiveZone(){
		x=0;
		x=0;
		R=0;
		Dir=0;
	}
	ClassArray<ZoneExInfo> AdditionalInfo;
//--------------------//
	void Draw();
	_inline ZoneExInfo* GetInfo(DWORD ID,int Index){
		int N=AdditionalInfo.GetAmount();
		for(int i=0;i<N;i++)if(AdditionalInfo[i]->InfID==ID){
			if(!Index)return AdditionalInfo[i];
			Index--;
		}            
		return NULL;
	}
	_inline int GetInfoAmount(DWORD ID){
		int N1=0;
		int N=AdditionalInfo.GetAmount();
		for(int i=0;i<N;i++)if(AdditionalInfo[i]->InfID==ID)N1++;
		return N1;
	}
	SAVE(ActiveZone)
    REG_MEMBER(_int,x);
	REG_MEMBER(_int,y);
	REG_MEMBER(_int,R);
	REG_AUTO(Name);	
	REG_AUTO(AdditionalInfo);
	ENDSAVE
};

struct OneUS{
	OneUS()	{ ID=SN=0xFFFF; };
	OneUS(WORD _ID,WORD _SN) { ID=_ID; SN=_SN; };
	WORD ID;
	WORD SN;
    //  Silver, 27.07.04:
    bool operator ==( const OneUS& val ) const { return ((ID == val.ID)&&(SN == val.SN)); }
};
class _OneUS:public BaseClass{
public:
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra=NULL){
		OneUS* V=(OneUS*)ClassPtr;
		char c[26];
		if(V->ID!=0xFFFF){
			OneObject* OB=Group[V->ID];
			if(OB&&OB->Serial==V->SN){
			//	sprintf(c,"%d %d %d",OB->RealX>>4,OB->RealY>>4,OB->Index);
				sprintf(c,"%u",OB->UniqID);
				xml.Assign_string(c);
				return;
			}
		}
		xml.Assign_string("0xFFFFFFFF");
	}
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL){
		OneUS* V=(OneUS*)ClassPtr;
		const char* c=xml.Get_string();
		if(c){
			V->ID=0xFFFF;
			V->SN=0xFFFF;
			DWORD uid = 0xFFFFFFFF;
		//	int z=0;
		//	int x,y;
		//	z=sscanf(c,"%d%d",&x,&y);
			sscanf(c,"%u",&uid);
		//	if(z!=2){
			if(uid==0xFFFFFFFF){
				Error->xml_print(xml,"unable to read _OneUS from XML: %s\n",xml.Get_string());
				return false;
			}
		//	int FindUnitByCoor(int x,int y);
		//	int id=FindUnitByCoor(x<<4,y<<4);
			int N = ULIMIT;
			int id = -1;
			while ( (id==-1) && (--N>=0)) {
				if ( Group[N]!=NULL && Group[N]->UniqID == uid ) id = N;
			};
			if(id!=-1){
				V->ID=id;
				V->SN=Group[id]->Serial;
			}
		}
		return true;
	}
	virtual const char* GetClassName(){
		return "_OneUS";
	}
	AUTONEW(_OneUS);
};

class ActiveGroup:public ReferableBaseClass{
public:
	LinearArray<OneUS,_OneUS> Units;
	//_str Name;
	ClassArray<GroupExInfo> AdditionalInfo;
	int MinX,MinY,MaxX,MaxY;
	int AminX,AminY,AmaxX,AmaxY;
	bool Visible;

	void Draw(byte c);
	_inline GroupExInfo* GetInfo(DWORD ID,int Index){
		int N=AdditionalInfo.GetAmount();
		for(int i=0;i<N;i++)if(AdditionalInfo[i]->InfID==ID){
			if(!Index)return AdditionalInfo[i];
			Index--;
		}            
		return NULL;
	}
	_inline int GetInfoAmount(DWORD ID){
		int N1=0;
		int N=AdditionalInfo.GetAmount();
		for(int i=0;i<N;i++)if(AdditionalInfo[i]->InfID==ID)N1++;
		return N1;
	}
	SAVE(ActiveGroup)
	REG_AUTO(Name);
    REG_AUTO(Units);
	REG_AUTO(AdditionalInfo);
	ENDSAVE
};
extern ClassArray<ActiveGroup> AGroups;
extern ClassArray<ActiveZone>  AZones;
//
// new base types by vital
//
class DIALOGS_API c_CapturableExInfo:public ZoneExInfo{
public:
	byte Owner;
	//
	c_CapturableExInfo(){
		InfID=_ZONE_EX_INFO_CAPTURABLE_;
	}
	SAVE(c_CapturableExInfo)
		REG_PARENT(ZoneExInfo);
		REG_MEMBER(_BYTE, Owner);
	ENDSAVE;	
};
//
class DIALOGS_API _AZone:public _variant{
	bool Init;
	ActiveZone* AZone;
public:
	_AZone() { AZone=NULL; Init=false; }
	ActiveZone* GetAZone();	
	virtual void GetVariants(ClonesArray<_str> &List);
	SAVE(_AZone)
		REG_PARENT(_variant);
	ENDSAVE;
};
//
void ClearAllZones();
void RefreshAGroups();

#endif