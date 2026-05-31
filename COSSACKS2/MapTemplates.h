#ifndef __MAPTEMPLATES_H__
#define __MAPTEMPLATES_H__
//square array of <Type>, sizeof cell  is 1<<Sh
#ifndef DIALOGS_API
#ifndef DIALOGS_USER
#define DIALOGS_API __declspec(dllexport)
#else //DIALOGS_USER
#define DIALOGS_API __declspec(dllimport)
//#define BaseMesh void
class BaseMesh;
#endif //DIALOGS_USER
#endif //DIALOGS_API
#include "smart_assert.h"
#include <kHash.hpp>
#ifndef NEW
#define NEW(t,s) (t*)malloc((s)*sizeof(t)) 
#endif

#define OP_ADD 0
#define OP_SUB 1
#define OP_MAX 2
#define OP_MIN 3
#define OP_DISSOLVE 4

#define FT_EXP   0
#define FT_EXP2  1
#define FT_INVX2 2
#define FT_LINE  3

template <class NType,int Sh,int MinV=0,int MaxV=0> class SquareNet{
	typedef NType SqFunc(NType src,int dx,int dy,void* param);
	typedef NType SqFunc2(NType src,int xc,int yc,int x,int y,void* param);
	NType* Data;
	int SizeX;
	int SizeY;
	int Shift;
public:
	SquareNet(){
		Data=NULL;
		SizeX=0;
		SizeY=0;
	}
	~SquareNet(){
		Free();
	}
	void Setup(){
		SizeX=(8192<<ADDSH)>>Sh;
		SizeY=SizeX;
		Shift=13+ADDSH-Sh;
		Data=NEW(NType,SizeX*SizeY);
		Fill(0);
	}
	void Free(){
		if(Data)free(Data);
		Data=NULL;
		SizeX=0;
		SizeY=0;
	}
	void Fill(NType V){
		if(sizeof(NType)==1){
			memset(Data,V,SizeX*SizeY);
		}else{
			int sz=SizeY<<Shift;
			for(int i=0;i<sz;i++)Data[i]=V;
		}
	}
	__forceinline void SetValue(int cx,int cy,NType V){
		if(cx>=0&&cx<SizeX&&cy>=0&&cy<SizeY){
			Data[cx+(cy<<Shift)]=V;
		}
	}
	__forceinline NType GetValue(int cx,int cy){
		if(cx<0)cx=0;
		if(cy<0)cy=0;
		if(cx>=SizeX)cx=SizeX-1;
		if(cy>=SizeY)cy=SizeY-1;
		return Data[cx+(cy<<Shift)];
	}
	
	__forceinline int GetAproxValue(int x,int y){
		int cx0=x>>Sh;
		int cy0=y>>Sh;
		int dx=x&((1<<Sh)-1);
		int dy=y&((1<<Sh)-1);
		int V0=(NType)GetValue(cx0,cy0);
		int V1=(NType)GetValue(cx0+1,cy0);
		int V2=(NType)GetValue(cx0,cy0+1);
		int V3=(NType)GetValue(cx0+1,cy0+1);
		int V=(V0+(((V1-V0)*dx)>>Sh)+(((V2-V0)*dy)>>Sh)+(((V0+V3-V1-V2)*dx*dy)>>(Sh+Sh)));
		if(MinV||MaxV){
			if(V<MinV)V=MinV;
			if(V>MaxV)V=MaxV;
		}
		return V;
	}
	int GetBufferSizeToSave(){
		return 4+SizeX*SizeX*sizeof(NType);
	}
	int SaveDataTo(char* buf){
		memcpy(buf,&SizeX,4);
		memcpy(buf+4,Data,SizeX*SizeY*sizeof(NType));
		return 4+SizeX*SizeX*sizeof(NType);
	}
	bool LoadDataFrom(char* buf){
		if(!Data)Setup();
		int sz0=*((NType*)buf);
		if(sz0!=SizeX){
			Free();
			Setup();
		}
        memcpy(Data,buf+4,SizeX*SizeY*sizeof(NType));
		return true;
	}
	void ApplyFn(SqFunc2* fn,int xc,int yc,void* param){
		int cx=xc>>Sh;
		int cy=yc>>Sh;
		int r0=0;
		NType max;
		do{
			max=0;
			int N=Rarr[r0].N;
			char* xi=Rarr[r0].xi;
			char* yi=Rarr[r0].yi;
			for(int v=0;v<N;v++){
				int CX=cx+xi[v];
				int CY=cy+yi[v];
				NType src=GetValue(CX,CY);
				NType dst=fn(src,xc,yc,(CX<<Sh),(CY<<Sh),param);
				NType D=abs(src-dst);
				if(D>max)max=D;
				SetValue(CX,CY,dst);
			}
			r0++;
		}while(max>1&&r0<40);
	}
	void ApplyDefFunction(int xc,int yc,float CenterValue,float EdgeValue,
		int InternalRadius,int ExternalRadius,
		int OP_TYPE,int FUNC_TYPE,
		float MinTypeValue,float MaxTypeValue,
		SqFunc* fn=NULL,void* param=NULL){

		int cx=xc>>Sh;
		int cy=yc>>Sh;
		int r0=0;
		NType max;
		do{
			max=0;
			int N=Rarr[r0].N;
			char* xi=Rarr[r0].xi;
			char* yi=Rarr[r0].yi;
			for(int v=0;v<N;v++){
				int CX=cx+xi[v];
				int CY=cy+yi[v];
				NType src=GetValue(CX,CY);
				int dxr=xc-(CX<<Sh);
				int dyr=yc-(CY<<Sh);
				float Vf;
				int RR=sqrt(dxr*dxr+dyr*dyr);
				if(RR<InternalRadius)Vf=1.0;
				else{
					float Dr=float(RR-InternalRadius)/float(ExternalRadius-InternalRadius);
					switch(FUNC_TYPE){
						case FT_EXP:
							Vf=exp(-Dr);
							break;
						case FT_EXP2:
							Vf=exp(-Dr*Dr);
							break;
						case FT_INVX2:
							Vf=1.0f/(Dr+1.0f)/(Dr+1.0f);
							break;
						case FT_LINE:
							if(Dr<1.0)Vf=1.0-Dr;
							else Vf=0.0f;
							break;
						default:
							Vf=0.0f;
					}       
				};
				if(fn)Vf=fn(Vf*MaxTypeValue/2,CX<<Sh,CY<<Sh,param)*2/MaxTypeValue;
				float fdst;
				switch(OP_TYPE){
					case OP_ADD:
						fdst=float(src)+CenterValue*Vf;
						break;
					case OP_SUB:
						fdst=float(src)-float(CenterValue)*Vf;
						break;
					case OP_MAX:
						fdst=float(src)>EdgeValue+(CenterValue-EdgeValue)*Vf?
								float(src):EdgeValue+(CenterValue-EdgeValue)*Vf;
						break;
					case OP_DISSOLVE:
					default:
						fdst=float(src)*(1.0f-Vf)+CenterValue*Vf;
						break;
				}		
				NType dst=NType(fdst<MinTypeValue?MinTypeValue:(fdst>MaxTypeValue?MaxTypeValue:fdst));
				NType D=abs(src-dst);
				if(D>max)max=D;				
				SetValue(CX,CY,dst);
			}
			r0++;
			if(r0<10)max=100;
		}while(max>1&&r0<40);
	}
};
#include "DynArray.h"
#include <StrHash.h>
#define STATHINT(x,y)\
{\
	static const char* y=GetTextByID(#y);\
	x->Hint=(char*)y;\
}
#define DYNHINT(x,y)\
{\
	x->Hint=NEW(char,strlen(y)+1);\
	strcpy(x->Hint,y);\
	x->AllocHint=1;\
}
template <class NType,int Sh=4,int SubDivSh=7,int OptimalVal=0,int MinV=0,int MaxV=0> class ESquareNet{
	typedef NType SqFunc(NType src,int dx,int dy,void* param);
	typedef NType SqFunc2(NType src,int xc,int yc,int x,int y,void* param);
	struct OneNetCell{
		word NVals;
		NType Vals[1<<((SubDivSh-Sh)*2)];
	};
	OneNetCell** Data;
	int SizeX;
	int SizeY;
	int Nx;
	int Ny;
public:
	ESquareNet(){
		Data=NULL;
		SizeX=0;
		SizeY=0;
		Nx=0;
		Ny=0;
	}
	~ESquareNet(){
		Free();
	}
	void Setup(){
		SizeX=(8192<<ADDSH)>>Sh;
		SizeY=SizeX;
		Nx=(8192<<ADDSH)>>SubDivSh;
		Ny=Nx;
		Data=(OneNetCell**)malloc(Nx*Ny*4);
		memset(Data,0,Nx*Ny*4);
	}
	void Free(){
		if(Data)free(Data);
		Data=NULL;
		SizeX=0;
		SizeY=0;
		Nx=0;
		Ny=0;
	}
	void Clear(){
		int sz=Nx*Ny;
		for(int i=0;i<sz;i++){
			if(Data[i])free(Data[i]);			
			Data[i]=NULL;
		}		
	}
	void Fill(NType V){
		Clear();
	}
	__forceinline void SetValue(int cx,int cy,NType V){
		if(cx>=0&&cx<SizeX&&cy>=0&&cy<SizeY){
			int cpx=cx>>(SubDivSh-Sh);
			int cpy=cy>>(SubDivSh-Sh);
			int ofs=cpx+cpy*Nx;
			if(V==OptimalVal&&!Data[ofs])return;
			if(!Data[ofs]){
				Data[ofs]=new OneNetCell;
				OneNetCell* ONC=Data[ofs];
				int N=1<<((SubDivSh-Sh)*2);
				ONC->NVals=0;
				for(int i=0;i<N;i++){
					ONC->Vals[i]=OptimalVal;
				}
			}
			OneNetCell* ONC=Data[ofs];
			int dx=cx&((1<<(SubDivSh-Sh))-1);
			int dy=cy&((1<<(SubDivSh-Sh))-1);
			int of1=dx+(dy<<(SubDivSh-Sh));
			if(V==OptimalVal){
				if(ONC->Vals[of1]!=OptimalVal)ONC->NVals--;
			}else{
				if(ONC->Vals[of1]==OptimalVal)ONC->NVals++;
			}
			ONC->Vals[of1]=V;
			if(!ONC->NVals){
				delete(ONC);
				Data[ofs]=NULL;
			}
		}
	}
	virtual NType GetValue(int cx,int cy){
		if(cx<0)cx=0;
		if(cy<0)cy=0;
		if(cx>=SizeX)cx=SizeX-1;
		if(cy>=SizeY)cy=SizeY-1;
		int cpx=cx>>(SubDivSh-Sh);
		int cpy=cy>>(SubDivSh-Sh);
		int ofs=cpx+cpy*Nx;
		OneNetCell* ONC=Data[ofs];
		if(!ONC)return OptimalVal;
		int dx=cx&((1<<(SubDivSh-Sh))-1);
		int dy=cy&((1<<(SubDivSh-Sh))-1);
		int of1=dx+(dy<<(SubDivSh-Sh));
		return ONC->Vals[of1];
	}
	bool TestCoor(int x,int y){
		int cx=x>>=Sh;
		int cy=y>>=Sh;
		if(cx<0)return false;
		if(cy<0)return false;
		if(cx>=SizeX)return false;
		if(cy>=SizeY)return false;
		int cpx=cx>>(SubDivSh-Sh);
		int cpy=cy>>(SubDivSh-Sh);
		int ofs=cpx+cpy*Nx;
		OneNetCell* ONC=Data[ofs];
		return ONC!=NULL;
	}
	__forceinline int GetAproxValue(int x,int y){
		int cx0=x>>Sh;
		int cy0=y>>Sh;
		int dx=x&((1<<Sh)-1);
		int dy=y&((1<<Sh)-1);
		int V0=(NType)GetValue(cx0,cy0);
		int V1=(NType)GetValue(cx0+1,cy0);
		int V2=(NType)GetValue(cx0,cy0+1);
		int V3=(NType)GetValue(cx0+1,cy0+1);
		int V=(V0+(((V1-V0)*dx)>>Sh)+(((V2-V0)*dy)>>Sh)+(((V0+V3-V1-V2)*dx*dy)>>(Sh+Sh)));
		if(MinV||MaxV){
			if(V<MinV)V=MinV;
			if(V>MaxV)V=MaxV;
		}
		return V;
	}
	__forceinline float GetAproxValue(float x,float y){
		int st=(1<<Sh);
		int cx0=x/st;
		int cy0=y/st;
		float dx=x-cx0*st;
		float dy=y-cy0*st;
		float V0=(NType)GetValue(cx0,cy0);
		float V1=(NType)GetValue(cx0+1,cy0);
		float V2=(NType)GetValue(cx0,cy0+1);
		float V3=(NType)GetValue(cx0+1,cy0+1);
		float V=(V0+((V1-V0)*dx)/st+((V2-V0)*dy)/st+((V0+V3-V1-V2)*dx*dy)/st/st);
		if(MinV||MaxV){
			if(V<MinV)V=MinV;
			if(V>MaxV)V=MaxV;
		}
		return V;
	}
	int GetBufferSizeToSave(){
		return 0;//temporarely
	}
	int SaveDataTo(char* buf){
		memcpy(buf,&SizeX,4);
		memcpy(buf+4,Data,SizeX*SizeY*sizeof(NType));
		return 4+SizeX*SizeX*sizeof(NType);
	}
	bool LoadDataFrom(char* buf){		
		return true;
	}
	void ApplyFn(SqFunc2* fn,int xc,int yc,void* param){
		int cx=xc>>Sh;
		int cy=yc>>Sh;
		int r0=0;
		NType max;
		do{
			max=0;
			int N=Rarr[r0].N;
			char* xi=Rarr[r0].xi;
			char* yi=Rarr[r0].yi;
			for(int v=0;v<N;v++){
				int CX=cx+xi[v];
				int CY=cy+yi[v];
				NType src=GetValue(CX,CY);
				NType dst=fn(src,xc,yc,(CX<<Sh),(CY<<Sh),param);
				NType D=abs(src-dst);
				if(D>max)max=D;
				SetValue(CX,CY,dst);
			}
			r0++;
		}while(max>1&&r0<40);
	}
	void ApplyDefFunction(int xc,int yc,float CenterValue,float EdgeValue,
		int InternalRadius,int ExternalRadius,
		int OP_TYPE,int FUNC_TYPE,
		float MinTypeValue,float MaxTypeValue,
		SqFunc* fn=NULL,void* param=NULL){
			int cx=xc>>Sh;
			int cy=yc>>Sh;
			int r0=0;
			NType max;
			do{
				max=0;
				int N=Rarr[r0].N;
				char* xi=Rarr[r0].xi;
				char* yi=Rarr[r0].yi;
				for(int v=0;v<N;v++){
					int CX=cx+xi[v];
					int CY=cy+yi[v];
					NType src=GetValue(CX,CY);
					int dxr=xc-(CX<<Sh);
					int dyr=yc-(CY<<Sh);
					float Vf;
					int RR=sqrt(dxr*dxr+dyr*dyr);
					if(RR<InternalRadius)Vf=1.0;
					else{
						float Dr=float(RR-InternalRadius)/float(ExternalRadius-InternalRadius);
						switch(FUNC_TYPE){
						case FT_EXP:
							Vf=exp(-Dr);
							break;
						case FT_EXP2:
							Vf=exp(-Dr*Dr);
							break;
						case FT_INVX2:
							Vf=1.0f/(Dr+1.0f)/(Dr+1.0f);
							break;
						case FT_LINE:
							if(Dr<1.0)Vf=1.0-Dr;
							else Vf=0.0f;
							break;
						default:
							Vf=0.0f;
						}       
					};
					if(fn)Vf=fn(Vf*MaxTypeValue/2,CX<<Sh,CY<<Sh,param)*2/MaxTypeValue;
					float fdst;
					switch(OP_TYPE){
					case OP_ADD:
						fdst=float(src)+CenterValue*Vf;
						break;
					case OP_SUB:
						fdst=float(src)-float(CenterValue)*Vf;
						break;
					case OP_MAX:
						fdst=max(float(src),EdgeValue+(CenterValue-EdgeValue)*Vf);
						break;
					case OP_DISSOLVE:
					default:
						fdst=float(src)*(1.0f-Vf)+CenterValue*Vf;
						break;
					}
					NType dst=NType(fdst<MinTypeValue?MinTypeValue:(fdst>MaxTypeValue?MaxTypeValue:fdst));
					NType D=abs(src-dst);
					if(D>max)max=D;
					SetValue(CX,CY,dst);
				}
				r0++;
				if(r0<10)max=100;
			}while(max>1&&r0<40);
		}
};

#endif //__MAPTEMPLATES_H__