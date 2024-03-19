//==================================================================================================================//
#include "stdheader.h"
//==================================================================================================================//
extern int tmtmt;
//==================================================================================================================//
Musor::Musor()
{
	BrigadeID=0xFFFFFFFF;
	MusorID=0xFFFFFFFF;
	Data=NULL;
	DataSize=0;
	DataRetentionTime=0;
	StorageTime=0;
}
Musor::~Musor()
{
	if(Data)
	{
		free(Data);
	}
}
Musor::Musor(DWORD BID,const void* D, DWORD DS, DWORD MID)
{
	BrigadeID=BID;
	MusorID=MID;
	Data=NULL;
	DataSize=DS;
	if(D)
	{
		Data=(void*)malloc(DataSize);
		memcpy(Data,D,DataSize);
	}
	DataRetentionTime=tmtmt;
	StorageTime=10;
}
//==================================================================================================================//
Musorozbornik::Musorozbornik(void)
{
}

Musorozbornik::~Musorozbornik(void)
{
	Clear();
}
void Musorozbornik::AddBrigadeLinkInfo(Brigade* B)
{
	if(B&&B->BOrder)
	{
		DWORD BrigadeID=B->CT->NI<<16;
		BrigadeID+=B->ID;
		MusorStorage.Add(new Musor(BrigadeID,B->BOrder,B->BOrder->Size));
	}
}
void Musorozbornik::AddMusor(const void* D, DWORD DS, DWORD MID)
{
	if(D)
	{
		MusorStorage.Add(new Musor(0xFFFFFFFF,D,DS,MID));
	}
}
void Musorozbornik::ClearMusorProcess()
{
	int n=MusorStorage.GetAmount();
	for(int i=0;i<n;i++)
	{
		if(MusorStorage[i])
		{
			if((MusorStorage[i]->DataRetentionTime+MusorStorage[i]->StorageTime)<tmtmt)
			{
				delete(MusorStorage[i]);
				MusorStorage[i]=NULL;
				MusorStorage.Del(i,1);
				i--;
				n--;
			}
		}
	}
}
void* Musorozbornik::GetLastBrigadeLinkData(DWORD BID)
{
	int n=MusorStorage.GetAmount();
	if(n)
	{
		for(int i=n-1;i>-1;i--)
		{
			if(MusorStorage[i]->BrigadeID==BID)
				return MusorStorage[i]->Data;
		}
	}
	return NULL;
}
void* Musorozbornik::GetData(DWORD MID, DWORD &DS)
{
	int n=MusorStorage.GetAmount();
	if(n)
	{
		for(int i=n-1;i>-1;i--)
		{
			if(MusorStorage[i]->MusorID==MID)
			{
				DS=MusorStorage[i]->DataSize;
				return MusorStorage[i]->Data;
			}
		}
	}
	DS=0;
	return NULL;
}
void Musorozbornik::Clear()
{
	int n=MusorStorage.GetAmount();
	for(int i=0;i<n;i++)
	{
		if(MusorStorage[i])
		{
			delete (MusorStorage[i]);
		}
	}
	MusorStorage.Clear();
}
//==================================================================================================================//
Musorozbornik MusorStorage;
//==================================================================================================================//
