#include "stdheader.h"
#include "Surface\Surface.h"
#include "UndoMaster.h"
#include "ITerrain.h"

using Surface::SurfVertex;
using Surface::SurfRef;
using Surface::SurfMesh;

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
	CheckFreeDiskSpace(200);
	char name[256];
	sprintf(name,"Undo\\%08d",UndoLevel++);
	Stream.WriteToFile(name);
	UndoSize+=Stream.Size();
}
void CreateSortedFileList(const char* Directory,const char* Mask,FileList& result,bool subdir);
void UndoMaster::RemoveAllFromDisk(){
    CreateDirectory("Undo",0);
	// Fix for ROS -- EDIT BY XENO ---
	CreateDirectory("Temp",0);
	CreateDirectory("Crash",0);

	FileList FL;
	CreateSortedFileList("Undo\\","*.*",FL,false);
	for(int i=0;i<FL.GetAmount();i++){
		char c[256];
		sprintf(c,"Undo\\%s",FL[i]->pchar());
		DeleteFile(c);
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
}
//surface undo
int SurfaceUndo::LastUndoTime=0;
void SurfaceUndo::SaveSurfaceRect(cVec2 _min,cVec2 _max,bool SaveSurface,bool SaveLayers){
	MemoryBinStream MBS;
	MBS.WriteDWORD('SURF');
	MBS.WriteBYTE(SaveSurface);
	MBS.WriteBYTE(SaveLayers);
	if(SaveSurface){
		Surface::SurfRef QRef;	
		int VertBorder=128;
		g_UniversalMap.QUADS.GetRefRect(QRef,_min.x-VertBorder,_min.y-VertBorder,_max.x+VertBorder,_max.y+VertBorder);
		SurfMesh M;
		QRef.ToSurfMesh(M,Surface::SR_QUADS);
		MBS.WriteDWORD(QRef.idQuads.Count());
		for(int i=0;i<QRef.idQuads.Count();i++){
			MBS.WriteDWORD(QRef.idQuads[i]);
		}
		MBS.WriteDWORD(M.VERTS.Count());
		MBS.Write(M.VERTS.ToPtr(),M.VERTS.Count()*sizeof(SurfVertex));
		MBS.WriteDWORD(M.TRIS.Count());
		MBS.Write(M.TRIS.ToPtr(),M.TRIS.Count()*sizeof(DWORD));
	}
	if(SaveLayers){
		int xmin=_min.x*512/15360.0-15;
		int xmax=_max.x*512/15360.0-15;
		int ymin=_min.y*512/15360.0+15;
		int ymax=_max.y*512/15360.0+15;
		if(xmin<0)xmin=0;
		if(ymin<0)ymin=0;
		if(xmin>511)xmin=511;
		if(ymin>511)ymin=511;
		if(xmax<0)xmax=0;
		if(ymax<0)ymax=0;
		if(xmax>511)xmax=511;
		if(ymax>511)ymax=511;		
		MBS.WriteWORD(xmin);
		MBS.WriteWORD(ymin);
		MBS.WriteWORD(xmax);
		MBS.WriteWORD(ymax);
		int N=MLayers->Maps.GetAmount();
		int sz=(xmax-xmin+1)*(ymax-ymin+1)*N;
		int bufsz=1+sz/32;
		DWORD* BUF=new DWORD[bufsz];
		memset(BUF,0,bufsz<<2);
		MBS.WriteDWORD(bufsz);
		MBS.WriteWORD(N);
		int pos=0;
		for(int i=0;i<N;i++){
			GreyMap* GM=(GreyMap*)MLayers->Maps[i];
			for(int y=ymin;y<=ymax;y++){
				for(int x=xmin;x<=xmax;x++){
					if(GM->GetGreyScale(x,y)){					
						BUF[pos>>5]|=1<<(pos&31);						
					}
					pos++;
				}
			}
		}
		MBS.Write(BUF,bufsz<<2);
		delete[]BUF;
	}
	UndoMaster::SaveUndoInfo(MBS);		    
}

void SurfaceUndo::DropRefToUndo(Surface::SurfRef& ref){
	if(ref.idTris.Count()){		
		Surface::SurfRef QRef;
		SurfVertex sv_min;
		SurfVertex sv_max;
		ref.CalcBoundBox(Surface::SR_TRIS,sv_min,sv_max);
		SaveSurfaceRect(cVec2(sv_min.x,sv_min.y),cVec2(sv_max.x,sv_max.y),true,false);
	}
}
void SurfaceUndo::Undo(BinStream& MBS){	
	bool HaveSurf=MBS.ReadBYTE();
	bool HaveLayers=MBS.ReadBYTE();
	if(HaveSurf){
		SurfMesh M;		
		int N=MBS.ReadDWORD();
		cList<DWORD> QS;
		for(int i=0;i<N;i++){
			DWORD Q=MBS.ReadDWORD();			
			QS.Add(Q);	
			if(Q<g_UniversalMap.QUADS.Count()){
				g_UniversalMap.QUADS[Q].idTris.Free();
			}
		}
		g_UniversalMap.QUADS.Compile();
		int NV=MBS.ReadDWORD();	
		M.VERTS.SetCount(NV);
		MBS.Read(M.VERTS.ToPtr(),M.VERTS.Count()*sizeof(SurfVertex));
		int NI=MBS.ReadDWORD();
		M.TRIS.SetCount(NI);
		MBS.Read(M.TRIS.ToPtr(),M.TRIS.Count()*sizeof(DWORD));
		SurfRef Ref;
		M.GetRefAll(Ref, Surface::SR_TRIS);
		Ref.FillMapOfHeights();	
		if(g_UniversalMap.VERTS.Count()==0){
			g_UniversalMap.SurfMesh::Copy(M);
			g_UniversalMap.QUADS.Init();
		}else{
			g_UniversalMap.Sum(M);
			g_UniversalMap.QUADS.Compile();
			for(int i=0;i<N;i++){
				if(QS[i]<g_UniversalMap.QUADS.Count()){
					g_UniversalMap.QUADS[QS[i]].Update();
				}
			}		
		}		
	}
	if(HaveLayers){
		int xmin=MBS.ReadWORD();
		int ymin=MBS.ReadWORD();
		int xmax=MBS.ReadWORD();
		int ymax=MBS.ReadWORD();
		int bufsz=MBS.ReadDWORD();
		int N=MBS.ReadWORD();
		DWORD* BUF=new DWORD[bufsz];
		MBS.Read(BUF,bufsz<<2);
		int pos=0;
		for(int i=0;i<N;i++){
			GreyMap* GM=(GreyMap*)MLayers->Maps[i];
			for(int y=ymin;y<=ymax;y++){
				for(int x=xmin;x<=xmax;x++){
					int v=BUF[pos>>5] & (1<<(pos&31)) ? 1 : 0;
					GM->SetGreyScale(x,y,v?255:0);
					pos++;
				}
			}
		}
		delete[]BUF;
	}
	ITerra->InvalidateGeometry();
}
void SurfaceUndo::DropWholeMap(){
    SaveSurfaceRect(cVec2(-1000,-1000),cVec2(16000,16000),true,true);
}
void SurfaceUndo::DropLayers(Surface::SurfRef& ref){
	if(ref.idTris.Count()){		
		Surface::SurfRef QRef;
		SurfVertex sv_min;
		SurfVertex sv_max;
		ref.CalcBoundBox(Surface::SR_TRIS,sv_min,sv_max);
		SaveSurfaceRect(cVec2(sv_min.x,sv_min.y),cVec2(sv_max.x,sv_max.y),false,true);
	}
}