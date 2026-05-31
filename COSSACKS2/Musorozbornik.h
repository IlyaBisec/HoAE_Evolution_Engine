#pragma once
class Musor
{
public:
	Musor();
	~Musor();
	Musor(DWORD BrigadeID,const void* Data, DWORD DataSize, DWORD MusorID=0xFFFFFFFF);
	
	//word Index;
	DWORD BrigadeID;
	DWORD MusorID;
	void* Data;
	DWORD DataSize;
	int DataRetentionTime;
	int StorageTime;
};
class Musorozbornik
{
public:
	Musorozbornik(void);
	~Musorozbornik(void);

	DynArray<Musor*> MusorStorage;

	void AddBrigadeLinkInfo(Brigade* B);
	void AddMusor(const void* Data, DWORD DataSize, DWORD MusorID);
	void ClearMusorProcess();
	void* GetLastBrigadeLinkData(DWORD BrigadeID);
	void* GetData(DWORD MusorID, DWORD &DataSize);
	void Clear();
};
