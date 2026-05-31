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
	static void SaveSurfaceRect(cVec2 _min,cVec2 _max,bool SaveSurface,bool SaveLayers);
	static void DropRefToUndo(Surface::SurfRef& ref);
	static void DropWholeMap();
	static void DropLayers(Surface::SurfRef& ref);
	virtual DWORD GetMagic(){return 'SURF';}
	virtual void  Undo(BinStream& Stream);
};