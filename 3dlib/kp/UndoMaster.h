#pragma once

class BasicUndoMaker{
public:
	virtual DWORD GetMagic(){return 0;}
	virtual void  Undo(BinStream& Stream){};
};
namespace Surface{
	class SurfRef;
};
class UndoMaster{
	static int UndoSize;
	static int UndoLevel;
	static DynArray<BasicUndoMaker*> Makers;
public:
	static void RegisterUndoMaker(BasicUndoMaker* M);
	static void Undo();
	static void SaveUndoInfo(BinStream& Stream);
	static void RemoveAllFromDisk();
	static void CheckFreeDiskSpace(int NMegabytes);
	static void Init();
};
//surface undo
class SurfaceUndo : public BasicUndoMaker{
	static int LastUndoTime;
public:
	static void DropSurfaceUndo();
	virtual DWORD GetMagic(){return 'MESH';}
	virtual void  Undo(BinStream& Stream);
};
class FreezeUndo : public BasicUndoMaker{
public:
    static void DropFreezeUndo();
    virtual DWORD GetMagic(){return 'FRIZ';}
	virtual void  Undo(BinStream& Stream);
};