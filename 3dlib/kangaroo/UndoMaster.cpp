#include "StdAfx.h" #include "common.h" 
#include "UndoMaster.h"

int UndoMaster::UndoLevel=0;
int UndoMaster::UndoSize=0;
DynArray<BasicUndoMaker*> UndoMaster::Makers;

void UndoMaster::RegisterUndoMaker(BasicUndoMaker* M){
	for(int i=0;i<Makers.GetAmount();i++){
		if(Makers[i]->GetMagic()==M->GetMagic()){
			delete(M);
			return;
		}
	}
	Makers.Add(M);
}
void UndoMaster::Undo(){
	if(UndoLevel>0){
		_chdir(IRM->GetHomeDirectory());
		char name[256];
		sprintf(name,"Undo\\%08d",--UndoLevel);
		MemoryBinStream MBS;
		MBS.ReadFromFile(name);
		UndoSize-=MBS.Size();
		DeleteFile(name);
		if(MBS.Size()>4){
			DWORD M=MBS.ReadDWORD();
			for(int i=0;i<Makers.GetAmount();i++){
				if(M==Makers[i]->GetMagic()){
					Makers[i]->Undo(MBS);
				}
			}
		}
	}
}
void UndoMaster::SaveUndoInfo(BinStream& Stream){
	_chdir(IRM->GetHomeDirectory());
	CheckFreeDiskSpace(200);
	char name[256];
	sprintf(name,"Undo\\%08d",UndoLevel++);
	Stream.WriteToFile(name);
	UndoSize+=Stream.Size();
}
void CreateSortedFileList(const char* Directory,const char* Mask,FileList& result,bool subdir);
void UndoMaster::RemoveAllFromDisk(){
	_chdir(IRM->GetHomeDirectory());
    CreateDirectory("Undo",0);
	FileList FL;
	CreateSortedFileList("Undo\\","*.*",FL,false);
	for(int i=0;i<FL.GetAmount();i++){
		DeleteFile(FL[i]->pchar());
	}
	UndoSize=0;
	UndoLevel=0;
}
UINT64 GETFREE(char* lpszPath);
int GetFreeMegabytes(){
	char CRDIR[256];
	GetCurrentDirectory(200,CRDIR);
	CRDIR[3]=0;
	return GETFREE(CRDIR)/1000000;
}
const int UndoMaxSize=300000000;
void UndoMaster::CheckFreeDiskSpace(int NMegabytes){
	if(GetFreeMegabytes()<NMegabytes || UndoSize>UndoMaxSize){
		FileList FL;
		CreateSortedFileList("Undo\\","*.*",FL,false);
		for(int i=0;i<FL.GetAmount();i++){
			char c[256];
			sprintf(c,"Undo\\%s",FL[i]->pchar());
            ResFile F=RReset(c);
			if(F!=INVALID_HANDLE_VALUE){
				UndoSize-=RFileSize(F);
				RClose(F);
			}
			DeleteFile(c);
			if(GetFreeMegabytes()>NMegabytes && UndoSize<UndoMaxSize)return;
		}
	}    
}
void UndoMaster::Init(){
	RemoveAllFromDisk();
	RegisterUndoMaker(new SurfaceUndo);
    RegisterUndoMaker(new FreezeUndo);
}
//surface undo
int SurfaceUndo::LastUndoTime=0;
void SurfaceUndo::DropSurfaceUndo(){
    OneBackupItem BI;
    BI.M=&PMS;
    if(BI.M->FacesHistory.size()){        
        scan(BI.M->FacesHistory,OneFaceBackup** B,bi_DWORD* FaceL){            
            BI.Backup.Add(**B);
        }scan_end;        
        MemoryBinStream MBS;
        MBS.WriteDWORD('MESH');
        BI.Save(&MBS);        
        BI.M->FacesHistory.reset();
        UndoMaster::SaveUndoInfo(MBS);
    }
}
void SurfaceUndo::DropSurfaceUndoWithoutReset(){
	OneBackupItem BI;
	BI.M=&PMS;
	if(BI.M->FacesHistory.size()){
		scan(BI.M->FacesHistory,OneFaceBackup** B,bi_DWORD* Face){
			BI.Backup.Add(**B);
		}scan_end;
		MemoryBinStream MBS;
		MBS.WriteDWORD('MESH');
		BI.Save(&MBS);
		UndoMaster::SaveUndoInfo(MBS);
	}
}
void SurfaceUndo::Undo(BinStream& Stream){
    OneBackupItem BI;
    BI.M=&PMS;
    BI.Load(&Stream);
    BI.Undo();
}
void FreezeUndo::DropFreezeUndo(){
    MemoryBinStream mbs;
    mbs.WriteDWORD('FRIZ');
    int n=PMS.SubSurfaceSize*PMS.SubSurfaceSize;
    for(int i=0;i<PMS.FacesInfo.GetAmount();i++){
        SurfPoint3D* s=PMS.FacesInfo[i].Points;
        for(int j=0;j<n;j++){
            int F=lrintf(s->Freeze*255);
            if(F>255)F=255;
            if(F<0)F=0;
            mbs.WriteBYTE(F);
            s++;
        }
    }
    UndoMaster::SaveUndoInfo(mbs);
}
void FreezeUndo::Undo(BinStream& Stream){
    int n=PMS.SubSurfaceSize*PMS.SubSurfaceSize;
    for(int i=0;i<PMS.FacesInfo.GetAmount();i++){
        SurfPoint3D* s=PMS.FacesInfo[i].Points;
        for(int j=0;j<n;j++){
            int F=Stream.ReadBYTE();
            s->Freeze=F/255.0f;
            s++;
        }
    }
    PMS.SetAllDirty();
    PMS.UpdateDirtyFaces(true);
}