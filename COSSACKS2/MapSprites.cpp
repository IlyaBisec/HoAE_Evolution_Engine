#include "stdheader.h"
#include "DrawModels.h"
#include "Surface\Surface.h"
#include "TreesMesh.h"
int GetTopology(int x,int y, byte LockType=0);
bool CheckSpritesInArea(int x,int y,int r,int GrpIndex);
int MAXSPR;
word GetDir(int dx,int dy);
void UnregisterSprite(int N);
void CheckGrassPresence(int x,int y,int R);
int Prop43(int y);

extern SprGroup TREES;
extern SprGroup STONES;
extern SprGroup HOLES;
extern SprGroup SPECIAL;
extern SprGroup ANMSPR;
extern SprGroup COMPLEX;
extern SprGroup WALLS;

#ifdef _USE3D
extern SprGroup MODELS3D;
#endif
//--------------Local 3D heights-------------//
int GetUnitHeight(int x,int y){
	return GetTotalHeight(x,y);
};
//-------------------------------------------//

int GetRXY(int V1,int V2,int x,int y){
	return (V1+(((V2-V1)*(x*171+y*313)&255)>>8));
}
byte* NSpri;
int** SpRefs;
OneSprite* Sprites=NULL;
word OneObject::FindNearestBase(){
	OneObject* OB;
	byte ResID=RType;
	word msk=1<<ResID;
	int dist=10000000;
	word OID=0xFFFF;
	for(int i=0;i<MAXOBJECT;i++){
		OB=Group[i];
		if(OB&&!OB->Sdoxlo&&(OB->Ready||OB->LocalOrder)&&(OB->AbRes&msk)&&OB->NNUM==NNUM){
			int dist1=Norma(RealX-OB->RealX,RealY-OB->RealY);
			if(dist1<dist){
				OID=i;
				dist=dist1;
			};
		};
	};
	return OID;
};
int LastAddSpr=0;
void ClearAllGSprites();
void ClearAllGS();
void CreateInfoMap();
void DeleteAllSprites(){
	NMines=0;
	OneSprite::ClearActiveSprites();
	ClearAllGSprites();
#ifdef _USE3D
	ClearAllGS();
#endif
	try{
		for(int i=0;i<MaxSprt;i++){
            if(Sprites[i].M4)delete(Sprites[i].M4);
			Sprites[i].Enabled=false;
		};
	}catch(...){};
	try{memset(NSpri,0,VAL_SPRSIZE);}catch(...){};
	int nn=VAL_SPRSIZE;
	try{
	for(int i=0;i<nn;i++){
		if(SpRefs[i]){
			free(SpRefs[i]);
			SpRefs[i]=NULL;
		};
	};
	}catch(...){};
	MAXSPR=0;
	LastAddSpr=0;
	if(Sprites)free(Sprites);
	Sprites=NULL;
	MaxSprt=0;
	ObjTimer.NMembers=0;
	CreateInfoMap();
};
void CHKS();
void RegisterSprite(int N){
	//assert(_CrtCheckMemory());//????
	OneSprite* OSP=&Sprites[N];
	if(OSP->x<0)OSP->x=0;
	if(OSP->y<0)OSP->y=0;
	int nn=(OSP->x>>7)+((OSP->y>>7)<<SprShf);
	assert(nn<VAL_SPRSIZE);
	if(SpRefs[nn]){
		//search for existing reference
		int nsp=NSpri[nn];
		int* SPR=SpRefs[nn];
		for(int i=0;i<nsp;i++)if(SPR[i]==N)return;
		if((nsp&0xFFFFF0)==nsp){
			int* spp=znew(int,nsp+16);
			memcpy(spp,SPR,nsp<<2);
			free(SPR);
			SPR=spp;
			SpRefs[nn]=spp;
		};
		SPR[nsp]=N;
		NSpri[nn]++;
	}else{
		SpRefs[nn]=znew(int,16);
		SpRefs[nn][0]=N;
		NSpri[nn]++;
	};
	//assert(_CrtCheckMemory());//????
};
void EraseExtraHeightObject(int x,int y);
void UnregisterSprite(int N){
	OneSprite* OSP=&Sprites[N];	
	//EraseExtraHeightObject(OSP->x,OSP->y);
	void MakeDirtyGBUF(int mx,int my,int sx,int sy);
	int scx=OSP->x>>(10-Shifter);
	int scy=OSP->y>>(10-Shifter);
	int NN=10>>(5-Shifter);
	if(NN<2)NN=2;
	///MakeDirtyGBUF(scx-NN,scy-NN-NN,NN*2,NN*3);
	int nn=(OSP->x>>7)+((OSP->y>>7)<<SprShf);
	//assert(nn<sprNx*sprNx);
	int xc=OSP->x;
	int yc=OSP->y;
	int gR=OSP->OC->GrassRadius;
	int gH=OSP->OC->GrassHeight;
	if(SpRefs[nn]){
		//search for existing reference
		int nsp=NSpri[nn];
		int* SPR=SpRefs[nn];
		for(int i=0;i<nsp;i++)if(SPR[i]==N){
			if(i<nsp-1){
				memcpy(SPR+i,SPR+i+1,(nsp-i-1)<<2);
			};
			NSpri[nn]--;
			if(!NSpri[nn]){
				free(SPR);
				SpRefs[nn]=NULL;
			};
			if(gH){
				CheckGrassPresence(xc,yc,gR);
			}
			return;
		};
		//assert(false);
	};
	if(gH){
		CheckGrassPresence(xc,yc,gR);
	}
};
SprGroup::SprGroup(){
	
};
SprGroup::~SprGroup(){
	
};
void InitSprites(){
	OneSprite::ClearActiveSprites();
	void ClearTreesGroups();
	ClearTreesGroups();
	Sprites=NULL;
	MaxSprt=0;
	//memset(Sprites,0,sizeof OneSprite);
	memset(NSpri,0,VAL_SPRSIZE);
	//memset(SpMap,0,sizeof SPRMap);
	TREES.LoadSprites("treelist");
	STONES.LoadSprites("stonlist");
	HOLES.LoadSprites("holelist");
	SPECIAL.LoadSprites("meat");
#ifdef CONQUEST
	//SPECIAL.LoadSprites("meat");
	ANMSPR.LoadSprites("Animations");
#endif
    COMPLEX.LoadSprites("complex");
	WALLS.LoadSprites("walls");
};
int GetMinDist(int x,int y){
	int md=10000;
	for(int i=0;i<MaxSprt;i++)if(Sprites[i].Enabled){
		int dst=abs(x-Sprites[i].x)+abs(y-Sprites[i].y);
		if(dst<md)md=dst;
	};
	return md;
};
bool CheckDist(int x,int y,word r){
    if(CheckBar(x>>4,y>>4,(r>>4)+3,(r>>4)+3))return false;
	for(int i=0;i<MaxSprt;i++){
		OneSprite* SPR=&Sprites[i];
		if(SPR->Enabled){
			int dst=abs(x-SPR->x)+abs(y-SPR->y);
			if(dst<r+SPR->Radius)return false;
		};
	};
	return true;
};
bool SpriteSuccess;
int LastSpriteIndex;
void CHKS();
class T_Curve{
public:
	int x0;
	int y0;
    ClonesArray<LockInfoPoint>* LPINF;
	//int N;
	//short* xi;
	//short* yi;
};
bool CheckTPointInside(int x,int y,T_Curve* TC){
	int ncr=0;
	for(int i=0;i<TC->LPINF->GetAmount();i++){
		int x1=TC->x0+(*TC->LPINF)[i]->x;
		int y1=TC->y0+(*TC->LPINF)[i]->y;
		int i1=(i+1)%TC->LPINF->GetAmount();
		int x2=TC->x0+(*TC->LPINF)[i1]->x;
		int y2=TC->y0+(*TC->LPINF)[i1]->y;
		if(x1>x2){
			x1+=x2;
			x2=x1-x2;
			x1-=x2;
			y1+=y2;
			y2=y1-y2;
			y1-=y2;
		};
		if(x1<=x&&x<x2){
			int y0=y1+((y2-y1)*(x-x1))/(x2-x1);
			if(y0>y)ncr++;
		};
	};
	return ncr&1;
};
bool AddLockPts(int x0,int y0,ClonesArray<LockInfoPoint>& LockInfo,byte add){
	T_Curve TC;
	TC.x0=x0;
	TC.y0=y0;
	TC.LPINF=&LockInfo;
	int minx=100000;
	int miny=100000;
	int maxx=-100000;
	int maxy=-100000;
	for(int i=0;i<LockInfo.GetAmount();i++){
		int x=x0+LockInfo[i]->x;
		int y=y0+LockInfo[i]->y;
		if(x<minx)minx=x;
		if(y<miny)miny=y;
		if(x>maxx)maxx=x;
		if(y>maxy)maxy=y;
	};
	minx=(minx>>4)-4;
	miny=(miny>>3)-4;
	maxx=(maxx>>4)+4;
	maxy=(maxy>>3)+4;
	for(int dx=minx;dx<=maxx;dx++){
		for(int dy=miny;dy<=maxy;dy++){
			if(CheckTPointInside((dx<<4)+8,(dy<<3)+4,&TC)){
				if(add==1){
					BSetPt(dx,dy);
				}else
				if(add==0){
					BClrPt(dx,dy);
				}else{
					if(CheckPt(dx,dy))return true;
				};
			};
		};
	};
	return false;
};

void AddGPiece(int x,int y,word Type);
void addSpriteAnyway(int x,int y,SprGroup* SG,word id);

void ObjCharacter::GetMatrix4D(Matrix4D& M4,int x,int y,int z) const
{	
	//if(x&1){
		M4.srt(Scale,Vector3D::oZ,RotFI,Vector3D(x,y,z));
	//}else{
	//	M4.srt(Scale,Vector3D::oZ,RotFI+c_PI,Vector3D(x,y,z));
	//	Matrix4D MI=Matrix4D::identity;
	//	MI.e00=-1;
	//	M4.mulLeft(MI);
	//}
}

int GetTotalHeight(int,int);
int GetModIndexByLandType(OneSprite* OS,int LandType){
	ObjCharacter* OC=OS->OC;	
	for(int i=0;i<OC->TreeMods.GetAmount();i++){
		if(LandType==OC->TreeMods[i]->LandType){
			return i;
		}
	}
	return OC->TreeMods.GetAmount()?0:-1;
}
int GetLandTypeInPoint(int x,int y);
void addSprite(int x,int y,SprGroup* SG,word id,int PrefRadius){
	if(SG->Objects[id]->NRandom){
		id+=rand()%SG->Objects[id]->NRandom;
	};
	//if(SG->Objects[id]->OnGround!=0xFFFF){
	//	AddGPiece(x,y,SG->Objects[id]->OnGround);
	//	return;
	//};
	int nb=SG->Objects[id]->BREF.GetAmount();
	
	SpriteSuccess=false;
	LastSpriteIndex=-1;
	int i=LastAddSpr;
	if(LastAddSpr+4>=MaxSprt){
		int psp=MaxSprt;
		MaxSprt+=32768;
		Sprites=(OneSprite*)realloc(Sprites,MaxSprt*sizeof OneSprite);
		memset(Sprites+psp,0,(MaxSprt-psp)*sizeof OneSprite);
	};
	//int i=0;
	while(Sprites[i].Enabled&&i<MaxSprt)i++;
	if(i>=MaxSprt){
		LastAddSpr=0;
		return;
	};
	LastAddSpr=i;
	OneSprite* OSP=&Sprites[i];
	if(i>MAXSPR)MAXSPR=i;
	int cx=x>>7;
	int cy=y>>7;
	if(cx>=VAL_SPRNX||cy>=VAL_SPRNX)return;
	OSP->OC=SG->Objects[id];
	int RR=int(OSP->OC->Radius)<<4;
	if(PrefRadius!=-1)RR=PrefRadius<<4;
	ObjCharacter* OC=SG->Objects[id];
	if(!CheckSpritesInArea(x<<4,y<<4,RR,OC->GroupIndex))return;	
	if(OC->LockInfo.GetAmount()){
		if(AddLockPts(x-OC->CenterX,(y>>1)-OC->CenterY,OC->LockInfo,2))return;
	};
	//if(GetMinDist(x,y)<48)return;
	int offs=cx+cy*VAL_SPRNX;
	//if(NSpri[offs]>=SprInCell)return;
	OSP->x=x;
	OSP->y=y;
	OSP->z=GetTotalHeight(x,y);
	if(OC->FixHeight>-1000){
        OSP->z=OC->FixHeight;
	}
	OSP->Enabled=true;
	//SPRMap[offs*SprInCell+NSpri[offs]]=i;
	//NSpri[offs]++;
	OSP->SG=SG;
	OSP->SGIndex=id;
	OSP->Index=i;
	RegisterSprite(i);
	OSP->WorkOver=0;
    OSP->TimePassed=0;
	OSP->Damage=0;
	OSP->Radius=OC->Radius;	
	OSP->Locking=0;
    if(OSP->OC->TimeAmount)ObjTimer.Add(i,0);
	SpriteSuccess=true;
	LastSpriteIndex=i;
	
	if(OC->LockInfo.GetAmount()){
		AddLockPts(OSP->x-OC->CenterX,(OSP->y>>1)-OC->CenterY,OC->LockInfo,1);
	};
	for(i=0;i<nb;i++){
		ObjCharacter* BOC=SG->Objects[id]->BREF[i]->ObjectToBorn.Get();
		if(BOC)addSpriteAnyway(x+SG->Objects[id]->BREF[i]->BDx,y+SG->Objects[id]->BREF[i]->BDy,SG,BOC->ParentIndex);
	};
	OSP->CreateMatrix();
	void AddExtraHeightObject(int x,int y,int id,Matrix4D* m4);	
	if(OC->ModelManagerID!=-1){		
		OSP->M4=new Matrix4D;					
		OC->GetMatrix4D(*OSP->M4,OSP->x,OSP->y,OSP->z);	
		///if(!OC->EditableModel){
			///AddExtraHeightObject(OSP->x,OSP->y,OC->ModelManagerID,OSP->M4);
		//}
	}
	if(OC->AutoDeep>-1000){
		void SetHiInRound(int x,int y,int r1,int H1,int r2,int H2);
		SetHiInRound(OSP->x,OSP->y,OC->AutoDeepR,OC->AutoDeep,OC->AutoDeepR1,-300);
	}

	OSP->Color=0;//MulDWORD(OSP->OC->TFactor,GetRXY(256-16,256+16,OSP->x,OSP->y));
	OSP->CurrentModIndex=GetModIndexByLandType(OSP,GetLandTypeInPoint(OSP->x,OSP->y));
	if(OSP->CurrentModIndex!=-1){
		OSP->Color=MulDWORD(OSP->OC->TreeMods[OSP->CurrentModIndex]->Color,GetRXY(256-16,256+16,OSP->x,OSP->y));
        OSP->M4->srt(OSP->OC->TreeMods[OSP->CurrentModIndex]->Scale,Vector3D::oZ,OSP->OC->RotFI+(OSP->SG!=&STONES ? float(rand()%((1+EngSettings.TreesDirectionVariation*2)-EngSettings.TreesDirectionVariation))*c_PI/128.0 : 0),Vector3D(x,y,OSP->z));
	}
	OSP->RiseStage=1.0f;
	OSP->DeathStage=1.0f;
	if(OSP->OC->GrassHeight){
		CheckGrassPresence(OSP->x,OSP->y,OSP->OC->GrassRadius);
	}
};
void AddSpriteScaled(int x,int y,SprGroup* SG,int id,int Radius,float Scale,float ExtraRotation,float exZ){
	LastSpriteIndex=-1;
	addSprite(x,y,SG,id,Radius);
	if(LastSpriteIndex!=-1){
		OneSprite* OS=Sprites+LastSpriteIndex;
		if(OS->M4){
			Matrix4D S;
			S.srt(Scale,Vector3D(1,0,0),ExtraRotation,Vector3D(0,0,exZ));
			OS->M4->mulLeft(S);
		}
	}
}
void addSprite(int x,int y,SprGroup* SG,word id){
	addSprite(x,y,SG,id,-1);
};
void CheckMemoryIntegrity();
void CheckGrassPresence(int x,int y,int R);
void addSpriteAnyway(int x,int y,SprGroup* SG,word id){
	SpriteSuccess=false;
	int n=SG->Objects.GetAmount();
	if(id>=n){
		if(n<=1)return;
		id=1+(x%(n-1));
	}
	//if(SG->Objects[id]->OnGround!=0xFFFF){
	//	AddGPiece(x,y,SG->Objects[id]->OnGround);
	//	return;
	//};	
	int i=LastAddSpr;
	if(LastAddSpr+4>=MaxSprt){
		int psp=MaxSprt;
		MaxSprt+=32768;
		Sprites=(OneSprite*)realloc(Sprites,MaxSprt*sizeof OneSprite);
		memset(Sprites+psp,0,(MaxSprt-psp)*sizeof OneSprite);
	};
	while(Sprites[i].Enabled&&i<MaxSprt)i++;
	if(i>=MaxSprt){
		LastAddSpr=0;
		return;
	};
	LastAddSpr=i;
	OneSprite* OSP=&Sprites[i];
	if(i>MAXSPR)MAXSPR=i;
	int cx=x>>7;
	int cy=y>>7;
	if(cx>=VAL_SPRNX||cy>=VAL_SPRNX)return;
	int offs=cx+cy*VAL_SPRNX;	
	ObjCharacter* OC=SG->Objects[id];
	OSP->OC=OC;
	OSP->x=x;
	OSP->y=y;		
	OSP->Enabled=true;
	OSP->SG=SG;
	OSP->SGIndex=id;
	OSP->Index=i;
	RegisterSprite(i);
	OSP->WorkOver=0;
    OSP->TimePassed=0;
	OSP->Damage=0;
	OSP->Radius=OSP->OC->Radius;
	OSP->OC=SG->Objects[id];
	if(OSP->OC->FixHeight>-1000){
		OSP->z=OSP->OC->FixHeight;
	}else OSP->z=GetHeight(x,y);
	if(OSP->z>1024)OSP->z-=2048;
    if(OSP->OC->TimeAmount||SG==&COMPLEX)ObjTimer.Add(i,0);
	OSP->Locking=0;
	SpriteSuccess=true;	
	if(OC->LockInfo.GetAmount()){
		AddLockPts(OSP->x-OC->CenterX,(OSP->y>>1)-OC->CenterY,OC->LockInfo,1);
	};
	OSP->CreateMatrix();

	OSP->Color=MulDWORD(OSP->OC->TFactor,GetRXY(256-16,256+16,OSP->x,OSP->y));
	OSP->CurrentModIndex=GetModIndexByLandType(OSP,GetLandTypeInPoint(OSP->x,OSP->y));
	if(OSP->CurrentModIndex==-1)OSP->Color=0;
	OSP->RiseStage=1.0f;
	OSP->DeathStage=1.0f;

	void AddExtraHeightObject(int x,int y,int id,Matrix4D* m4);
	
#ifdef _USE3D
	void MakeDirtyGBUF(int mx,int my,int sx,int sy);
	int scx=OSP->x>>(10-Shifter);
	int scy=OSP->y>>(10-Shifter);
	int NN=10>>(5-Shifter);
	if(NN<2)NN=2;
	if(OSP->OC->GrassHeight){
		CheckGrassPresence(OSP->x,OSP->y,OSP->OC->GrassRadius);
	}
	MakeDirtyGBUF(scx-NN,scy-NN-NN,NN*2,NN*3);
#endif //_USE3D
};
#define MSXH (240<<ADDSH)
int GetHeight(int x,int y){
	if(x<0)x=0;
	if(y<32)y=32;
	if(x>MSXH*32)x=MSXH*32;
	if(y>MSXH*32)y=MSXH*32;
	int nx=x>>5;
	if(nx&1){
		int dd=(x&31);
		int dy=dd>>1;
		int oy=15-dy;
		int y1=(y+oy)>>5;
		int dy1=(y+oy)&31;
		if(dy1>32-dd){
			int Vert2=nx+y1*VertInLine+1;
			int Vert3=Vert2+VertInLine;
			int Vert1=Vert3-1;
			int H1=THMap[Vert1];
			int H2=THMap[Vert2];
			int H3=THMap[Vert3];
			int x0=(nx<<5);
			int y0=(y1<<5)+16;
			int HH=H1+(((x-x0)*(((H2+H3)>>1)-H1))>>5)+(((y-y0)*(H3-H2))>>5);
			//if(HH<0)HH=0;
			//assert(HH>=0);
			return HH;
		}else{
			int Vert2=nx+y1*VertInLine;
			int Vert3=Vert2+VertInLine;
			int Vert1=Vert2+1;
			int H1=THMap[Vert1];
			int H2=THMap[Vert2];
			int H3=THMap[Vert3];
			int x0=(nx<<5)+32;
			int y0=(y1<<5);
			int HH=H1-(((x-x0)*(((H2+H3)>>1)-H1))>>5)+(((y-y0)*(H3-H2))>>5);
			//if(HH<0)HH=0;
			//assert(HH>=0);
			return HH;
		};
	}else{
		int dd=(x&31);
		int dy=dd>>1;
		int oy=15-dy;
		int y1=(y+dy)>>5;
		int dy1=(y+dy)&31;
		if(dy1<dd){
			int Vert1=nx+y1*VertInLine;
			int Vert2=Vert1+1;
			int Vert3=Vert2+VertInLine;
			int H1=THMap[Vert1];
			int H2=THMap[Vert2];
			int H3=THMap[Vert3];
			int x0=(nx<<5);
			int y0=(y1<<5);
			int HH=H1+(((x-x0)*(((H2+H3)>>1)-H1))>>5)+(((y-y0)*(H3-H2))>>5);
			//if(HH<0)HH=0;
			return HH;
		}else{
			int Vert2=nx+y1*VertInLine;
			int Vert3=Vert2+VertInLine;
			int Vert1=Vert3+1;
			int H1=THMap[Vert1];
			int H2=THMap[Vert2];
			int H3=THMap[Vert3];
			int x0=(nx<<5)+32;
			int y0=(y1<<5)+16;
			int HH=H1-(((x-x0)*(((H2+H3)>>1)-H1))>>5)+(((y-y0)*(H3-H2))>>5);
			//if(HH<0)HH=0;
			return HH;
		};
	};
};
extern short* DeepMap;
int GetDeep(int x,int y){
	if(x<0)x=0;
	if(y<32)y=32;
	if(x>MSXH*32)x=MSXH*32;
	if(y>MSXH*32)y=MSXH*32;
	int nx=x>>5;
	if(nx&1){
		int dd=(x&31);
		int dy=dd>>1;
		int oy=15-dy;
		int y1=(y+oy)>>5;
		int dy1=(y+oy)&31;
		if(dy1>32-dd){
			int Vert2=nx+y1*VertInLine+1;
			int Vert3=Vert2+VertInLine;
			int Vert1=Vert3-1;
			int H1=DeepMap[Vert1];
			int H2=DeepMap[Vert2];
			int H3=DeepMap[Vert3];
			int x0=(nx<<5);
			int y0=(y1<<5)+16;
			int HH=H1+(((x-x0)*(((H2+H3)>>1)-H1))>>5)+(((y-y0)*(H3-H2))>>5);
			//if(HH<0)HH=0;
			//assert(HH>=0);
			return HH;
		}else{
			int Vert2=nx+y1*VertInLine;
			int Vert3=Vert2+VertInLine;
			int Vert1=Vert2+1;
			int H1=DeepMap[Vert1];
			int H2=DeepMap[Vert2];
			int H3=DeepMap[Vert3];
			int x0=(nx<<5)+32;
			int y0=(y1<<5);
			int HH=H1-(((x-x0)*(((H2+H3)>>1)-H1))>>5)+(((y-y0)*(H3-H2))>>5);
			//if(HH<0)HH=0;
			//assert(HH>=0);
			return HH;
		};
	}else{
		int dd=(x&31);
		int dy=dd>>1;
		int oy=15-dy;
		int y1=(y+dy)>>5;
		int dy1=(y+dy)&31;
		if(dy1<dd){
			int Vert1=nx+y1*VertInLine;
			int Vert2=Vert1+1;
			int Vert3=Vert2+VertInLine;
			int H1=DeepMap[Vert1];
			int H2=DeepMap[Vert2];
			int H3=DeepMap[Vert3];
			int x0=(nx<<5);
			int y0=(y1<<5);
			int HH=H1+(((x-x0)*(((H2+H3)>>1)-H1))>>5)+(((y-y0)*(H3-H2))>>5);
			//if(HH<0)HH=0;
			return HH;
		}else{
			int Vert2=nx+y1*VertInLine;
			int Vert3=Vert2+VertInLine;
			int Vert1=Vert3+1;
			int H1=DeepMap[Vert1];
			int H2=DeepMap[Vert2];
			int H3=DeepMap[Vert3];
			int x0=(nx<<5)+32;
			int y0=(y1<<5)+16;
			int HH=H1-(((x-x0)*(((H2+H3)>>1)-H1))>>5)+(((y-y0)*(H3-H2))>>5);
			//if(HH<0)HH=0;
			return HH;
		};
	};
};
#define CLB (64<<4)
#define CLB1 ((64<<4)-1)
#define CLB2 (64<<3)
#define CLB21 ((64<<3)-1)
int GetPrecHeight(int x,int y){
	int nx=x>>(6+4);
	if(nx&1){
		int dd=(x&CLB1);
		int dy=dd>>1;
		int oy=CLB21-dy;
		int y1=(y+oy)>>(6+4);
		int dy1=(y+oy)&CLB1;
		if(dy1>CLB-dd){
			int Vert2=nx+y1*VertInLine+1;
			int Vert3=Vert2+VertInLine;
			int Vert1=Vert3-1;
			int H1=THMap[Vert1]<<4;
			int H2=THMap[Vert2]<<4;
			int H3=THMap[Vert3]<<4;
			int x0=(nx<<(6+4));
			int y0=(y1<<(6+4))+CLB2;
			return H1+(((x-x0)*(((H2+H3)>>1)-H1))>>10)+(((y-y0)*(H3-H2))>>10);
		}else{
			int Vert2=nx+y1*VertInLine;
			int Vert3=Vert2+VertInLine;
			int Vert1=Vert2+1;
			int H1=THMap[Vert1]<<4;
			int H2=THMap[Vert2]<<4;
			int H3=THMap[Vert3]<<4;
			int x0=(nx<<10)+CLB;
			int y0=(y1<<10);
			return H1-(((x-x0)*(((H2+H3)>>1)-H1))>>10)+(((y-y0)*(H3-H2))>>10);
		};
	}else{
		int dd=(x&CLB1);
		int dy=dd>>1;
		int oy=CLB21-dy;
		int y1=(y+dy)>>10;
		int dy1=(y+dy)&CLB1;
		if(dy1<dd){
			int Vert1=nx+y1*VertInLine;
			int Vert2=Vert1+1;
			int Vert3=Vert2+VertInLine;
			int H1=THMap[Vert1]<<4;
			int H2=THMap[Vert2]<<4;
			int H3=THMap[Vert3]<<4;
			int x0=(nx<<10);
			int y0=(y1<<10);
			return H1+(((x-x0)*(((H2+H3)>>1)-H1))>>10)+(((y-y0)*(H3-H2))>>10);
		}else{
			int Vert2=nx+y1*VertInLine;
			int Vert3=Vert2+VertInLine;
			int Vert1=Vert3+1;
			int H1=THMap[Vert1]<<4;
			int H2=THMap[Vert2]<<4;
			int H3=THMap[Vert3]<<4;
			int x0=(nx<<10)+CLB;
			int y0=(y1<<10)+CLB2;
			return H1-(((x-x0)*(((H2+H3)>>1)-H1))>>6)+(((y-y0)*(H3-H2))>>6);
		};
	};
};
int mul3(int);
extern int tmtmt;
void HideFlags(){
	for(int i=0;i<MaxSprt;i++){
		OneSprite* OS=Sprites+i;
		if(OS->Enabled&&OS->SG==&TREES&&TREES.GetNSprites()==OS->SGIndex+1){
			OS->Enabled=false;
			ObjTimer.Del(OS->Index,0);
            UnregisterSprite(OS->Index);
		};
	};
};
bool AddSprRefl(int xg,int yg,int xc,int yc,int GP_File,int Sprite,int dy1,int dy2);
void DrawSpriteTrees();
#ifdef _USE3D
DWORD T_Diff=0xFF808080;
void ChangeTreeChannels(int idx,int d){
	int A=(T_Diff>>24);
	int R=(T_Diff>>16)&255;
	int G=(T_Diff>>8)&255;
	int B=(T_Diff)&255;
	int* v;
	switch(idx){
	case 0:
		v=&R;
		break;
	case 1:
		v=&G;
		break;
	case 2:
		v=&B;
		break;
	};
	*v+=d;
	if(*v<0)*v=0;
	if(*v>255)*v=255;
	T_Diff=(A<<24)+(R<<16)+(G<<8)+B;
};
void ProcessTChanels(){
	int d=0;
	if(GetKeyState(VK_ADD)&0x8000)d=1;
	if(GetKeyState(VK_SUBTRACT)&0x8000)d=-1;
	if(d){
		if(GetKeyState('1')&0x8000)ChangeTreeChannels(0,d);
		if(GetKeyState('2')&0x8000)ChangeTreeChannels(1,d);
		if(GetKeyState('3')&0x8000)ChangeTreeChannels(2,d);
	};

};
#endif
extern IRenderSystem*	IRS;
#define maxobj 8192
class FastSprites{
public:
	int NObj;
	FastSprites(){
		BM=NULL;
		PrevX=0;
		PrevY=0;
		NObj=0;
	}
	~FastSprites(){
		if(BM)delete(BM);
	}
	int PrevX;
	int PrevY;
	BaseMesh* BM;
	void init(){
		if(BM)return;
		BM=new BaseMesh;
		BM->create(maxobj*4,maxobj*6);
		BM->setShader(IRS->GetShaderID("sprite_grass"));
		BM->setTexture(IRS->GetTextureID("fastsprites.tga"));

		word* idxs=BM->getIndices();
		int i6=0;
		int i4=0;
		for(int i=0;i<maxobj;i++){
			idxs[i6  ]=i4  ;
			idxs[i6+1]=i4+1;
			idxs[i6+2]=i4+2;
			idxs[i6+3]=i4+1;
			idxs[i6+4]=i4+3;
			idxs[i6+5]=i4+2;
			i4+=4;
			i6+=6;
		}
	}
	void clear(){
		NObj=0;
	};
	void add(Vector3D& pos,Vector2D& tc,Vector2D& left,Vector2D& size,float scale,float angle){
		init();
		//if(PrevX!=mouseX||PrevY!=mouseY){
		if(NObj<maxobj-1){
			Vertex2t* V=((Vertex2t*)BM->getVertexData())+NObj*4;
			V[0].x=(left.x-tc.x)*scale;
			V[0].y=(left.y-tc.y)*scale;
			V[0].z=pos.z;

			V[1].x=(left.x+size.x-tc.x)*scale;
			V[1].y=(left.y-tc.y)*scale;
			V[1].z=pos.z;

			V[2].x=(left.x-tc.x)*scale;
			V[2].y=(left.y+size.y-tc.y)*scale;
			V[2].z=pos.z;

			V[3].x=(left.x+size.x-tc.x)*scale;
			V[3].y=(left.y+size.y-tc.y)*scale;
			V[3].z=pos.z;

			V[0].u=left.x;
			V[0].v=left.y;
			V[0].u2=0;
			V[0].v2=0;

			V[1].u=left.x+size.x;
			V[1].v=left.y;
			V[1].u2=0;
			V[1].v2=0;

			V[2].u=left.x;
			V[2].v=left.y+size.y;
			V[2].u2=0;
			V[2].v2=0;

			V[3].u=left.x+size.x;
			V[3].v=left.y+size.y;
			V[3].u2=0;
			V[3].v2=0;

			float fcos=cos(angle);
			float fsin=sin(angle);
			for(int i=0;i<4;i++){
				float fx=V[i].x*fcos-V[i].y*fsin;
				float fy=V[i].y*fcos+V[i].x*fsin;
				V[i].x=pos.x+fx;
				V[i].y=pos.y+fy/2.0f;
				V[i].z=pos.z-fy*0.866025f;
				V[i].diffuse=0xA0FFFFFF;
				V[i].u/=256.0f;
				V[i].v/=256.0f;
			}
			NObj++;
			BM->setNInd(NObj*6);
			BM->setNPri(NObj*2);
			BM->setNVert(NObj*4);

		}		
	}
	void Draw(){
		init();
		PrevX=mouseX;
		PrevY=mouseY;
		DrawBM(*BM);
		NObj=0;
	}
};
FastSprites FS;

void DrawFastSprites()
{
	FS.Draw();
}

void ClearFastSprites()
{
    FS.clear();
}
void UnLoadFS(){
	ClearFastSprites();
	if(FS.BM)delete(FS.BM);
	FS.BM=NULL;
}
void AddFastSprite(const OneSprite& OS)
{
	const ObjCharacter& OC = *OS.OC;
	float ang=0.02*cos(float(GetTickCount())/float((400+OS.x%23+OS.y%17)));
	FS.add( Vector3D( OS.x, OS.y, OS.z ),
            Vector2D( OC.uC, OC.vC ),
            Vector2D( OC.uL, OC.vL ),
            Vector2D( OC.uR, OC.vR ),
            1.0f, ang );
} // AddFastSprite

void addScrSprite(int x,int y,SprGroup* SG,word id){
	int yy=y;
	if(Mode3D)yy=ConvScrY(x,y);
	if(yy<0)return;
	yy=div24(yy<<5);
	addSprite(x,yy,SG,id);
};
int GetTreeItem();
extern int InteriorType;
void addTree(int x,int y){
	switch(InteriorType){
	case 0:addScrSprite(x,y,&TREES,GetTreeItem());
		break;
	case 1:addScrSprite(x,y,&STONES,GetTreeItem());
		break;
	case 2:addScrSprite(x,y,&ANMSPR,GetTreeItem());
		break;
	};
};
void addLogTree(int x,int y){
	switch(InteriorType){
	case 0:addSprite(x,y,&TREES,GetTreeItem());
		break;
	case 1:addSprite(x,y,&STONES,GetTreeItem());
		break;
	case 2:addSprite(x,y,&ANMSPR,GetTreeItem());
		break;
    case 3:addSprite(x,y,&COMPLEX,GetTreeItem());
        break;

	};
};
void addLogTree(int x,int y,int r){
	switch(InteriorType){
	case 0:addSprite(x,y,&TREES,GetTreeItem(),r<=16?-1:r<100?60:95);
		break;
	case 1:addSprite(x,y,&STONES,GetTreeItem());
		break;
	case 2:addSprite(x,y,&ANMSPR,GetTreeItem());
		break;
    case 3:addSprite(x,y,&WALLS,GetTreeItem());
		break;
	case 4:addSprite(x,y,&COMPLEX,GetTreeItem());
        break;

	};
};
void FindBestPositionForSprite(SprGroup* SG,int Index,int& x,int& y);
extern int LastMx;
extern int LastMy;
int LastAddSX=-1;
int LastAddSY=-1;
extern SprGroup* CURSG;
void addTrees(int x,int y1,int r){
	int y=Prop43(y1);
	if(InteriorType==3)r=1;
	int itm=GetTreeItem();
	if(CURSG->Objects[itm]->CONN.GetAmount()&&!(GetKeyState(VK_CONTROL)&0x8000)){
		if(Norma(LastMx-LastAddSX,LastMy-LastAddSY)>8){
			int itm=GetTreeItem();
			int x=LastMx;
			int y=LastMy;
			FindBestPositionForSprite(CURSG,itm,x,y);
			addSprite(x,y,CURSG,itm);
			LastAddSX=LastMx;
			LastAddSY=LastMy;
		}
		return;      
	}
	if(r<16){
		addLogTree(x,y,0);
	}else{
		int n=(r*r)>>8;
		int r2=r*r;
		for(int i=0;i<n;i++){
			int dx=((rando()-16384)*r)>>14;
			int dy=((rando()-16384)*r)>>14;
			if(dx*dx+dy*dy<=r2)addLogTree(x+dx,y+dy,r);
		};
	};
	if(CURSG==&WALLS){
		void UnPress();
		UnPress();
	}
};
void AddSpriteScaled(int x,int y,SprGroup* SG,int id,int Radius,float Scale,float ExtraRotation,float);
void addTreesEx(int x,int y1,int r,float Scale,float Theta,float ExtraZ){
	int y=Prop43(y1);
	if(InteriorType==3)r=1;
	int itm=GetTreeItem();
	if(CURSG->Objects[itm]->CONN.GetAmount()&&!(GetKeyState(VK_CONTROL)&0x8000)){
		if(Norma(LastMx-LastAddSX,LastMy-LastAddSY)>8){
			int itm=GetTreeItem();
			int x=LastMx;
			int y=LastMy;
			FindBestPositionForSprite(CURSG,itm,x,y);
			//addSprite(x,y,CURSG,itm);
			AddSpriteScaled(x,y,CURSG,itm,6,Scale,Theta,ExtraZ);
			LastAddSX=LastMx;
			LastAddSY=LastMy;
		}
		return;      
	}
	if(r<16){
		//addLogTree(x,y,0);
		AddSpriteScaled(x,y,CURSG,GetTreeItem(),6,Scale,Theta,ExtraZ);
	}else{
		int n=(r*r)>>8;
		int r2=r*r;
		for(int i=0;i<n;i++){
			int dx=((rando()-16384)*r)>>14;
			int dy=((rando()-16384)*r)>>14;
			if(dx*dx+dy*dy<=r2){
				//addLogTree(x+dx,y+dy,r);
				float R=r;
				clamp(R,24.0f,85.0f);
				AddSpriteScaled(x+dx,y+dy,CURSG,GetTreeItem(),R,Scale*((1.0+(rand()%(2*EngSettings.TreesSizeVariation+1)-EngSettings.TreesSizeVariation)/100.0f)),float(rand()-16384)*Theta/16384.0f,ExtraZ);
			}
		};
	};
	if(CURSG==&WALLS){
		void UnPress();
		UnPress();
	}
};
void delTrees(int x,int y1,int r){
	int y=Prop43(y1);
	int r2=r*r;
	for(int i=0;i<MaxSprt;i++){
		OneSprite* OS=&Sprites[i];
		if(OS->Enabled){
			int ox=OS->x-x;
			int oy=OS->y-y;
            if(ox*ox+oy*oy<=r2){
                OS->Enabled=false;
                UnregisterSprite(OS->Index);
                ObjTimer.Del(OS->Index,0);
            };
		};
	};
};
extern MotionField UnitsField;
//searces place for peasant near the resource
//px,py-coordinates of peasant(unit: 16*unit=1pix)
//tx,ty-coordintes of resource(unit : pixel)
//r-best work distance (unit : pixel)
bool FindPlaceForPeasant(int* px,int* py,
						int tx,int ty,int r){
	int pix=((*px))>>8;
	int piy=((*py))>>8;
	int treex=(tx>>4)-1;
	int treey=(ty>>4)-1;
	int rad=r>>4;
	int radx=(r-(r>>2))>>4;
	int minr=10000;
	int bx,by,rr;
	int x1=treex+rad;
	int y1=treey;
	if(!CheckBar(x1,y1,3,3)){
		rr=Norma(x1-pix,y1-piy);
		minr=rr;
		bx=x1+1;
		by=y1+1;
	};
	y1=treey+rad;
	if(!CheckBar(x1,y1,3,3)){
		rr=Norma(x1-pix,y1-piy);
		if(rr<minr){
			minr=rr;
			bx=x1+1;
			by=y1+1;
		};
	};
	y1=treey-rad;
	if(!CheckBar(x1,y1,3,3)){
		rr=Norma(x1-pix,y1-piy);
		if(rr<minr){
			minr=rr;
			bx=x1+1;
			by=y1+1;
		};
	};
	x1=treex;
	if(!CheckBar(x1,y1,3,3)){
		rr=Norma(x1-pix,y1-piy);
		if(rr<minr){
			minr=rr;
			bx=x1+1;
			by=y1+1;
		};
	};
	y1=treey+rad;
	if(!CheckBar(x1,y1,3,3)){
		rr=Norma(x1-pix,y1-piy);
		if(rr<minr){
			minr=rr;
			bx=x1+1;
			by=y1+1;
		};
	};
	x1=treex-rad;
	if(!CheckBar(x1,y1,3,3)){
		rr=Norma(x1-pix,y1-piy);
		if(rr<minr){
			minr=rr;
			bx=x1+1;
			by=y1+1;
		};
	};
	y1=treey;
	if(!CheckBar(x1,y1,3,3)){
		rr=Norma(x1-pix,y1-piy);
		if(rr<minr){
			minr=rr;
			bx=x1+1;
			by=y1+1;
		};
	};
	y1=treey-rad;
	if(!CheckBar(x1,y1,3,3)){
		rr=Norma(x1-pix,y1-piy);
		if(rr<minr){
			minr=rr;
			bx=x1+1;
			by=y1+1;
		};
	};
	if(minr<1000){
		*px=bx<<4;
		*py=by<<4;
		return true;
	}else return false;
};
//-------------SEARCH FOR RESOURCES ROUTINES----------
//searcing for the resource in the cell.
//New Resource - resource,that was not use before;
//Old resource - already partially used resource
class CellSearch{
public:
	int xd,yd;//unit : pixels
	int BestNew;
	int BestOld;
	int NewDist;//unit : pix
	int OldDist;//unit : pix
	byte ResType;
	void InitCSR();
};
void CellSearch::InitCSR(){
	NewDist=10000;
	OldDist=10000;
	BestNew=INITBEST;
	BestOld=INITBEST;
};
void FindResInCell(CellSearch* CSR,int cell,int Top ){
	if(cell<0||cell>=VAL_SPRSIZE)return;
	int* CEL=SpRefs[cell];
	int   NCEL=NSpri[cell];
	int TOP1=Top*GetNAreas();
	if(!(CEL&&NCEL))return;
	CSR->NewDist=10000;
	CSR->OldDist=10000;
	int dist;
	for(int i=0;i<NCEL;i++){
		OneSprite* OS=&Sprites[CEL[i]];
		//assert(OS->Enabled);
		ObjCharacter* OC=OS->OC;
		if(OC->ResType==CSR->ResType&&!OS->Surrounded){
			int tx=OS->x>>6;
			int ty=OS->y>>6;
			//int STop=SafeTopRef(tx,ty);
			//if(STop<0xFFFE&&(Top==STop||GetLinksDist(TOP1+STop)<90)){
				if(OC->ResPerWork){
					//old resource
					dist=Norma(OS->x-CSR->xd,OS->y-CSR->yd);
					if(dist<CSR->OldDist){
						CSR->OldDist=dist;
						CSR->BestOld=CEL[i];
					};
				}else{
					//new resource
					dist=Norma(OS->x-CSR->xd,OS->y-CSR->yd);
					if(dist<CSR->NewDist){
						CSR->NewDist=dist;
						CSR->BestNew=CEL[i];
					};
				};
			//};
		};
	};
};
bool CheckSpritesInCell(int cell,int x,int y,int r,int GrpIndex){
    if(cell<0||cell>=VAL_SPRSIZE)return true;
	int* CEL=SpRefs[cell];
	int   NCEL=NSpri[cell];
	if(!(CEL&&NCEL))return true;
	for(int i=0;i<NCEL;i++){
		OneSprite* OS=&Sprites[CEL[i]];
		//assert(OS->Enabled);
		ObjCharacter* OC=OS->OC;
		if(OC->GroupIndex==GrpIndex||GrpIndex==-1){
			if(OC->ResType!=0xFE){
				int dr=OS->Radius;
				if(OC->IntResType>8)dr=0;
    			if(Norma(OS->x-x,OS->y-y)<(r+dr))return false;
			}
		}
    }
    return true;
};
//x: 16units=1 pix
//y: 16units=1 pix
bool CheckSpritesInArea(int x,int y,int r,int GrpIndex=-1){
    x>>=4;
    y>>=4;
    r>>=4;
    int nr=(r>>7)+1;
    int nr1=nr+nr+1;
    int cell=(x>>7)-nr+(((y>>7)-nr)<<SprShf);
    for(int ix=0;ix<nr1;ix++)
        for(int iy=0;iy<nr1;iy++)
            if(!CheckSpritesInCell(cell+ix+(iy<<SprShf),x,y,r,GrpIndex))return false;
    return true;
};
bool CheckSpritesInAreaFast(int x,int y,int r,int GrpIndex=-1){
    x>>=4;
    y>>=4;
    r>>=4;
    int nr=1;
    int nr1=nr+nr+1;
    int cell=(x>>7)-nr+(((y>>7)-nr)<<SprShf);
    for(int ix=0;ix<nr1;ix++)
        for(int iy=0;iy<nr1;iy++)
            if(!CheckSpritesInCell(cell+ix+(iy<<SprShf),x,y,r,GrpIndex))return false;
    return true;
};
bool CheckSpritesInCellNew(int cell,int x,int y,int r){
    if(cell<0||cell>=VAL_SPRSIZE)return true;
	int* CEL=SpRefs[cell];
	int   NCEL=NSpri[cell];
	if(!(CEL&&NCEL))return true;
	for(int i=0;i<NCEL;i++){
		OneSprite* OS=&Sprites[CEL[i]];
		//assert(OS->Enabled);
		ObjCharacter* OC=OS->OC;
		if(OC->ResType!=0xFE&&OS->SG!=&TREES&&OS->SG!=&WALLS&&OS->SG!=&SPECIAL&&OS->SG!=&ANMSPR&&!(OS->SG==&STONES&&OS->OC->IntResType>8)){
			int dr=OS->Radius;
			if(OC->IntResType>8)dr=0;
			if(OS->SG==&COMPLEX)dr-=15<<4;
    		if(Norma(OS->x-x,OS->y-y)<(r+dr))return false;
        };
    };
    return true;
};
void EraseSprite(int Index){
	OneSprite* OS=&Sprites[Index];
	if(OS&&OS->Enabled){
		int id=OS->SGIndex;
		ObjCharacter* OC=OS->SG->Objects[id];
		if(OC->LockInfo.GetAmount()){
			AddLockPts(OS->x-OC->CenterX,(OS->y>>1)-OC->CenterY,OC->LockInfo,0);
		};
		OS->Enabled=0;
		ObjTimer.Del(Index,0);
		UnregisterSprite(Index);
	};
};
void EraseTreesInPoint(int x,int y){
	int cell=(x>>3)+((y>>3)<<SprShf);
	if(cell<0||cell>=VAL_SPRSIZE)return;
	int* CEL=SpRefs[cell];
	int   NCEL=NSpri[cell];
	if(!(CEL&&NCEL))return;
	for(int i=0;i<NCEL;i++){
		OneSprite* OS=&Sprites[CEL[i]];
		//assert(OS->Enabled);
		ObjCharacter* OC=OS->OC;
		if(OC->ResType==0xFE||OS->SG==&TREES||OS->SG==&WALLS){
			if(x==(OS->x>>4)&&y==(OS->y>>4)){
				OS->Enabled=false;
				ObjTimer.Del(OS->Index,0);
				UnregisterSprite(OS->Index);
			};
        };
    };
};
bool CheckSpritesInAreaNew(int x,int y,int r,bool Erase){
    x>>=4;
    y>>=4;
    r>>=4;
    int nr=(r>>7)+3;
    int nr1=nr+nr+1;
    int cell=(x>>7)-nr+(((y>>7)-nr)<<SprShf);
	if(Erase){
		for(int ix=0;ix<nr1;ix++)
			for(int iy=0;iy<nr1;iy++)
				if(!CheckSpritesInCellNew(cell+ix+(iy<<SprShf),x,y,r))return false;
	}else{
	    for(int ix=0;ix<nr1;ix++)
			for(int iy=0;iy<nr1;iy++)
				if(!CheckSpritesInCell(cell+ix+(iy<<SprShf),x,y,r,-1))return false;
	};
    return true;
};
int MINESPRITEID=-1;
word GetOneBld(int cell,int pos=0);
int CheckSpritesInCellForMine(int cell,int* xi,int* yi,int r,byte rmask){
    if(cell<0||cell>=VAL_SPRSIZE)return true;
	int x=*xi;
	int y=*yi;
	int* CEL=SpRefs[cell];
	int   NCEL=NSpri[cell];
	if(!(CEL&&NCEL))return -2;
    byte rm;
    int ID=-1;
	for(int i=0;i<NCEL;i++){
		OneSprite* OS=&Sprites[CEL[i]];
		//assert(OS->Enabled);
		ObjCharacter* OC=OS->OC;
        if(OS->Enabled&&OC->IntResType<8){
            rm=1<<OC->IntResType;
			if(rm&rmask){//&&(Norma(OS->x-x,OS->y-y)>>2)<OC->IntWorkRadius){				
				word BMD=GetOneBld(cell+1+VAL_MAXCX);
				//if(BMD==0xFFFF){
					ID=CEL[i];
					MINESPRITEID=ID;
					*xi=OS->x;
					*yi=OS->y;
				//}
            };
        };//else{
    		//if(OC->ResType!=0xFE){
        	//	if(Norma(OS->x-x,OS->y-y)<r)return -1;
            //};
        //};
    };
    if(ID==-1)return -2;
    return ID;
};
//x: 16units=1 pix
//y: 16units=1 pix
int GetUnitModifiedBySprite(int _MineID,OneSprite* OS);
int CheckMinePosition(int id,int* xi,int* yi,int r,bool Manual){
	NewMonster* NM=NATIONS->Mon[id]->newMons;
    int x=(*xi)>>4;
    int y=(*yi)>>4;
	int x0=x;
	int y0=y;
    r>>=4;MINESPRITEID=0;
	byte RMask=NM->ProdType?Manual?0xFF:NM->ProdType:0;
    int nr=(r>>7)+1;
    int nr1=nr+nr+1;
    int cell=((x-nr)>>7)+(((y-nr)>>7)<<SprShf);
    int ID=-1;
    int ID1=-1;
	int minr=10000;
	int bestsp=0;
    for(int ix=0;ix<nr1;ix++)
        for(int iy=0;iy<nr1;iy++){
			x=x0;
			y=y0;
            ID1=CheckSpritesInCellForMine(cell+ix+(iy<<SprShf),&x,&y,r,RMask);
            if(ID1==-1)return -1;
            if(ID1>=0){
				OneSprite* OS=Sprites+MINESPRITEID;
				int r=Norma(x0-OS->x,y0-OS->y);
				if(r<minr){					
					int t=GetUnitModifiedBySprite(id,OS);
					if(t>=0){
						bestsp=MINESPRITEID;
						minr=r;
						*xi=x<<4;
						*yi=y<<4;
						ID=ID1;
					}
				}
			};
        };
	MINESPRITEID=bestsp;
    return ID;
};
int GetCell(int xs,int ys){
	return (xs>>7)+((ys>>7)<<SprShf);
};
//searches for resource object. 
//xd,yd-coordinates of the researching point(pix)
//returns required coordinates of peasant
//xp,yp-coordinates of peasant(min units)
//RType-resource type
//-----Notes-------
int FindResourceObject(int* xd,int* yd,int xp,int yp,byte RType){
	addrand(*xd);
	addrand(*yd);
	addrand(xp);
	addrand(yp);
	addrand(RType);

	CellSearch CSR1;
	CSR1.xd=*xd;
	CSR1.yd=*yd;
	CSR1.ResType=RType;
	CSR1.InitCSR();
	int tx=(*xd)>>6;
	int ty=(*yd)>>6;
	if(tx<=0||ty<=0)return INITBEST;
	int DstTop=SafeTopRef(tx,ty);
	if(DstTop>=0xFFFE)return INITBEST;
	int pix,piy;
	int STCL=GetCell(CSR1.xd,CSR1.yd);
	bool ResFound=false;
	bool EndCyc=false;
	int rr=0;
	int subcsr=0;
	int x0=CSR1.xd>>7;
	int y0=CSR1.yd>>7;
	do{
		do{
			char* xi=Rarr[rr].xi;
			char* yi=Rarr[rr].yi;
			int N=Rarr[rr].N;
			for(int i=0;i<N;i++){
				int xx=x0+xi[i];
				int yy=y0+yi[i];
				if(xx>=0&&yy>=0&&xx<VAL_MAXCX&&yy<VAL_MAXCX){
					FindResInCell(&CSR1,xx+(yy<<SprShf),DstTop);
				}
			}
			if(CSR1.BestOld!=INITBEST&&CSR1.BestNew!=INITBEST){
				if(CSR1.NewDist<CSR1.OldDist+CSR1.OldDist){
					pix=xp;
					piy=yp;
					OneSprite* OSP=&Sprites[CSR1.BestNew];
					if(FindPlaceForPeasant(&pix,&piy,OSP->x,OSP->y,OSP->OC[OSP->SGIndex].WorkRadius)){
						*xd=pix;
						*yd=piy;
						addrand(CSR1.BestNew);
						return CSR1.BestNew;
					}else{
						OSP->Surrounded=true;
						OSP=&Sprites[CSR1.BestOld];
						if(FindPlaceForPeasant(&pix,&piy,OSP->x,OSP->y,OSP->OC[OSP->SGIndex].WorkRadius)){
							*xd=pix;
							*yd=piy;
							addrand(CSR1.BestOld);
							return CSR1.BestOld;
						}else{
							OSP->Surrounded=true;
							EndCyc=false;
							CSR1.InitCSR();
						};
					};
				}else{
					pix=xp;
					piy=yp;
					OneSprite* OSP=&Sprites[CSR1.BestOld];
					if(FindPlaceForPeasant(&pix,&piy,OSP->x,OSP->y,OSP->OC[OSP->SGIndex].WorkRadius)){
						*xd=pix;
						*yd=piy;
						addrand(CSR1.BestOld);
						return CSR1.BestOld;
					}else{
						OSP->Surrounded=true;
						OSP=&Sprites[CSR1.BestNew];
						if(FindPlaceForPeasant(&pix,&piy,OSP->x,OSP->y,OSP->OC[OSP->SGIndex].WorkRadius)){
							*xd=pix;
							*yd=piy;
							addrand(CSR1.BestNew);
							return CSR1.BestNew;
						}else{
							OSP->Surrounded=true;
							EndCyc=false;
							CSR1.InitCSR();
						};
					};
				};
			}else{
				EndCyc=true;
				if(CSR1.BestOld!=INITBEST){
					pix=xp;
					piy=yp;
					OneSprite* OSP=&Sprites[CSR1.BestOld];
					if(FindPlaceForPeasant(&pix,&piy,OSP->x,OSP->y,OSP->OC->WorkRadius)){
						*xd=pix;
						*yd=piy;
						addrand(CSR1.BestOld);
						return CSR1.BestOld;
					}else{
						OSP->Surrounded=true;
						EndCyc=false;
						CSR1.InitCSR();
					};
				};
				if(CSR1.BestNew!=INITBEST){
					pix=xp;
					piy=yp;
					OneSprite* OSP=&Sprites[CSR1.BestNew];
					if(FindPlaceForPeasant(&pix,&piy,OSP->x,OSP->y,OSP->OC->WorkRadius)){
						*xd=pix;
						*yd=piy;
						addrand(CSR1.BestNew);
						return CSR1.BestNew;
					}else{
						OSP->Surrounded=true;
						EndCyc=false;
						CSR1.InitCSR();
					};
				};
			};
		}while(!EndCyc);
		EndCyc=false;
		rr++;		
		CSR1.InitCSR();
	}while(rr<5);
	rando();
	return INITBEST;
};
void FindLimResInCell(CellSearch* CSR,int cell,word Lim,int Top){
	if(cell<0||cell>=VAL_SPRSIZE)return;
	int* CEL=SpRefs[cell];
	int   NCEL=NSpri[cell];
	if(!(CEL&&NCEL))return;
	CSR->NewDist=10000;
	CSR->OldDist=10000;
	int TOP1=Top*GetNAreas();
	int dist;
	for(int i=0;i<NCEL;i++){
		int RS=CEL[i];
		if(RS!=Lim){
			OneSprite* OS=&Sprites[CEL[i]];
			//assert(OS->Enabled);
			ObjCharacter* OC=OS->OC;
			if(OC->ResType==CSR->ResType&&!OS->Surrounded){
				int tx=OS->x>>6;
				int ty=OS->y>>6;
				int FTop=SafeTopRef(tx,ty);
				if(FTop<0xFFFE&&(GetLinksDist(FTop+TOP1)<20||FTop==Top)){
					if(OC->ResPerWork){
						//old resource
						dist=Norma(OS->x-CSR->xd,OS->y-CSR->yd);
						if(dist<CSR->OldDist){
							CSR->OldDist=dist;
							CSR->BestOld=CEL[i];
						};
					}else{
						//new resource
						dist=Norma(OS->x-CSR->xd,OS->y-CSR->yd);
						if(dist<CSR->NewDist){
							CSR->NewDist=dist;
							CSR->BestNew=CEL[i];
						};
					};
				};
			};
		};
	};
};

int FindLimResourceObject(int* xd,int* yd,int xp,int yp,byte RType,word Lim){
	addrand(*xd);
	addrand(*yd);
	addrand(xp);
	addrand(yp);
	int SrcTop=GetTopology(xp>>4,yp>>4);
	int DstTop=GetTopology(*xd,*yd);
	if(DstTop>=0xFFFE)return INITBEST;
	CellSearch CSR1;
	CSR1.xd=*xd;
	CSR1.yd=*yd;
	CSR1.ResType=RType;
	CSR1.InitCSR();
	int pix,piy;
	int STCL=(CSR1.xd>>7)+((CSR1.yd>>7)<<SprShf);
	bool ResFound=false;
	bool EndCyc=false;
	int rr=0;
	int subcsr=0;
	do{
		do{
			if(!rr)FindLimResInCell(&CSR1,STCL,Lim,DstTop);
			else{
				int nn=rr+rr+1;
				for(int i=0;i<nn;i++)
					FindLimResInCell(&CSR1,STCL-subcsr-rr+i,Lim,DstTop);
				for(int i=0;i<nn;i++)
					FindLimResInCell(&CSR1,STCL+subcsr-rr+i,Lim,DstTop);
				nn-=2;
				int subb=VAL_SPRNX-subcsr-rr;
				for(int i=0;i<nn;i++){
					FindLimResInCell(&CSR1,STCL+subb,Lim,DstTop);
					subb+=VAL_SPRNX;
				};
				subb=VAL_SPRNX-subcsr+rr;
				for(int i=0;i<nn;i++){
					FindLimResInCell(&CSR1,STCL+subb,Lim,DstTop);
					subb+=VAL_SPRNX;
				};
			};
			if(CSR1.BestOld!=INITBEST&&CSR1.BestNew!=INITBEST){
				if(CSR1.NewDist<CSR1.OldDist+CSR1.OldDist){
					pix=xp;
					piy=yp;
					OneSprite* OSP=&Sprites[CSR1.BestNew];
					if(FindPlaceForPeasant(&pix,&piy,OSP->x,OSP->y,OSP->OC->WorkRadius)){
						*xd=pix;
						*yd=piy;
						return CSR1.BestNew;
					}else{
						OSP->Surrounded=true;
						OSP=&Sprites[CSR1.BestOld];
						if(FindPlaceForPeasant(&pix,&piy,OSP->x,OSP->y,OSP->OC->WorkRadius)){
							*xd=pix;
							*yd=piy;
							return CSR1.BestOld;
						}else{
							OSP->Surrounded=true;
							EndCyc=false;
							CSR1.InitCSR();
						};
					};
				}else{
					pix=xp;
					piy=yp;
					OneSprite* OSP=&Sprites[CSR1.BestOld];
					if(FindPlaceForPeasant(&pix,&piy,OSP->x,OSP->y,OSP->OC->WorkRadius)){
						*xd=pix;
						*yd=piy;
						return CSR1.BestOld;
					}else{
						OSP->Surrounded=true;
						OSP=&Sprites[CSR1.BestNew];
						if(FindPlaceForPeasant(&pix,&piy,OSP->x,OSP->y,OSP->OC->WorkRadius)){
							*xd=pix;
							*yd=piy;
							return CSR1.BestNew;
						}else{
							OSP->Surrounded=true;
							EndCyc=false;
							CSR1.InitCSR();
						};
					};
				};
			}else{
				EndCyc=true;
				if(CSR1.BestOld!=INITBEST){
					pix=xp;
					piy=yp;
					OneSprite* OSP=&Sprites[CSR1.BestOld];
					if(FindPlaceForPeasant(&pix,&piy,OSP->x,OSP->y,OSP->OC->WorkRadius)){
						*xd=pix;
						*yd=piy;
						return CSR1.BestOld;
					}else{
						OSP->Surrounded=true;
						EndCyc=false;
						CSR1.InitCSR();
					};
				};
				if(CSR1.BestNew!=INITBEST){
					pix=xp;
					piy=yp;
					OneSprite* OSP=&Sprites[CSR1.BestNew];
					if(FindPlaceForPeasant(&pix,&piy,OSP->x,OSP->y,OSP->OC->WorkRadius)){
						*xd=pix;
						*yd=piy;
						return CSR1.BestNew;
					}else{
						OSP->Surrounded=true;
						EndCyc=false;
						CSR1.InitCSR();
					};
				};
			};
		}while(!EndCyc);
		EndCyc=false;
		rr++;
		subcsr+=VAL_SPRNX;
		CSR1.InitCSR();
	}while(rr<4);
	return INITBEST;
};

enum AlignType
{
	atGround	= 1,
	atVertical	= 2,
}; // enum AlignType

Matrix4D Plane2WorldTM( const Vector2D& VS1, const Vector2D& VS2, const Vector2D& VS3,
                                         const Vector3D&  VW1, const Vector3D&  VW2, const Vector3D&  VW3 );

void OneSprite::CreateMatrix()
{
	if (OC->Aligning == atVertical)
	{	
			if(!M4)M4=new Matrix4D;
			Matrix4D GetAlignLineTransformWithScape( const Vector3D& Center,const Vector3D& pivot, int x1, int y1, int x2, int y2 );
			*M4 = GetAlignLineTransformWithScape( Vector3D(x,y,0 ),
											Vector3D( OC->CenterX, OC->CenterY, 0.0f ), 
											OC->va_x1, OC->va_y1, 
											OC->va_x2, OC->va_y2 );
			//M4->translate( SkewPt( x, y, z ) );
			//float Bias=0;//.6f;
			//float BiasY=Bias*0.866025f;
			//float BiasZ=Bias*0.5f;
			//float TBias=0;//-16.0f;
			//float TBiasY=TBias*0.866025f;
			//float TBiasZ=TBias*0.5f;

			//OC->P1.xp=OC->va_x1;
			//OC->P1.yp=OC->va_y1;
			//OC->P1.wx=OC->va_x1-OC->CenterX;
			//OC->P1.wy=(OC->va_y1-OC->CenterY)*2;
			//OC->P1.wz=0;

			//OC->P2.xp=OC->va_x2;
			//OC->P2.yp=OC->va_y2;
			//OC->P2.wx=OC->va_x2-OC->CenterX;
			//OC->P2.wy=(OC->va_y2-OC->CenterY)*2;
			//OC->P2.wz=0;

			//if(OC->va_y2<OC->va_y1){
			//	OC->P1.wy-=BiasY;
			//	OC->P1.wz-=BiasZ;
			//	OC->P2.wy+=BiasY;
			//	OC->P2.wz+=BiasZ;
			//}else{
			//	OC->P1.wy+=BiasY;
			//	OC->P1.wz+=BiasZ;
			//	OC->P2.wy-=BiasY;
			//	OC->P2.wz-=BiasZ;
			//}
			//int dz=128;
			//OC->P3.xp=(OC->va_x1+OC->va_x2)/2;
			//OC->P3.yp=(OC->va_y1+OC->va_y2)/2-dz;
			//OC->P3.wx=(OC->P1.wx+OC->P2.wx)/2;
			//OC->P3.wy=(OC->P1.wy+OC->P2.wy-dz)/2;
			//OC->P3.wz=dz*866/1000;
			//for(int i=0;i<3;i++){
			//	(&OC->P1)[i].wy+=BiasY*10;
			//	(&OC->P1)[i].wz+=BiasZ*10;
			//}
			//OC->P1.wy-=TBiasY;
			//OC->P1.wz-=TBiasZ;
			//OC->P2.wy-=TBiasY;
			//OC->P2.wz-=TBiasZ;
			//OC->P3.wy-=TBiasY;
			//OC->P3.wz-=TBiasZ;
	}else if (OC->Aligning == atGround)
	{	
		//horizontal aligning
			float TBias=-16.0f;
			float TBiasY=TBias*0.866025f;
			float TBiasZ=TBias*0.5f;

			float Bias=-EngSettings.TerrainZBias*2/3;
			int xc=OC->CenterX;
			int yc=OC->CenterY;
			OC->P1.xp=xc-20;
			OC->P1.yp=yc-5;
			OC->P1.wx=-20;
			OC->P1.wy=-10;
			OC->P1.wz=Bias;

			OC->P2.xp=xc+20;
			OC->P2.yp=yc-5;
			OC->P2.wx=20;
			OC->P2.wy=-10;
			OC->P2.wz=Bias;

			OC->P3.xp=xc;
			OC->P3.yp=yc+10;
			OC->P3.wx=0;
			OC->P3.wy=20;
			OC->P3.wz=Bias;
			OC->P1.wy-=TBiasY;
			OC->P1.wz-=TBiasZ;
			OC->P2.wy-=TBiasY;
			OC->P2.wz-=TBiasZ;
			OC->P3.wy-=TBiasY;
			OC->P3.wz-=TBiasZ;
			Vector2D PP1,PP2,PP3;
			
			PP1 = Vector2D(OC->P1.xp,OC->P1.yp);
			PP2 = Vector2D(OC->P2.xp,OC->P2.yp);
			PP3 = Vector2D(OC->P3.xp,OC->P3.yp);
			Vector3D W1,W2,W3;
			int H1=GetHeight(x+OC->P1.wx,y+OC->P1.wy);
			int H2=GetHeight(x+OC->P2.wx,y+OC->P2.wy);
			int H3=GetHeight(x+OC->P3.wx,y+OC->P3.wy);
			W1.set(x+OC->P1.wx,y+OC->P1.wy-H1/2,OC->P1.wz+H1*866/1000);
			W2.set(x+OC->P2.wx,y+OC->P2.wy-H2/2,OC->P2.wz+H2*866/1000);
			W3.set(x+OC->P3.wx,y+OC->P3.wy-H3/2,OC->P3.wz+H3*866/1000);
			if(!M4)M4=new Matrix4D;
			*M4 = Plane2WorldTM(PP1,PP2,PP3,W1,W2,W3);
	}
} // OneSprite::CreateMatrix

int OneSprite::PerformWork(word Effect){
	ObjCharacter* OCR=OC;
	WorkOver++;	
	int Wamount=(int(OCR->WorkAmount)*Effect)/100;
	if(Wamount<=WorkOver){
		//transformation to the next stage
        ObjCharacter* OCN=OC->NextWorkObj.Get();
		if(OCN){
			OC=OCN;
			int OldSG=SGIndex;
			SGIndex=OCN->ParentIndex;
		    WorkOver=0;
            TimePassed=0;
			void MakeDirtyPiece(int x,int y);
			if(SG!=&COMPLEX&&OldSG!=SGIndex)MakeDirtyPiece(x,y);
        }else{
            //Enabled=false;
            //ObjTimer.Del(Index,0);
            //UnregisterSprite(Index);
        };
	};
	return OCR->ResPerWork;
};
int OneSprite::PerformIntWork(int work){
	ObjCharacter* OCR=OC;
	WorkOver+=work;	
	int r=OCR->IntResPerWork;
	if(OCR->WorkAmount<=WorkOver){
		//transformation to the next stage		
		ObjCharacter* OCN=OC->NextWorkObj.Get();
		if(OCN){
			OC=OCN;
			SGIndex=OCN->ParentIndex;
			WorkOver=0;
			TimePassed=0;
		}
	};
	return work*r;
};
void OneSprite::PerformDamage(int Dam){
	ObjCharacter* OCR=OC;
	if(Dam>1)Dam=Dam>>1;
	Damage+=Dam;	
	if(OCR->DamageAmount<=Damage){
		//transformation to the next stage
		SGIndex=OCR->DNextObj;
		OC=SG->Objects[SGIndex];
		Damage=0;
	};
};
//word FindResBase
void TakeResLink(OneObject* OBJ);
#define OBJDELTA 5000000
int OneObject::TakeResource(int x,int y,byte ResID,int Prio,byte OrdType){
	if(UnlimitedMotion)return false;
	if(CheckOrderAbility())return false;
	if(Prio<PrioryLevel)return false;
	if(UnlimitedMotion)return false;
	if(!ext_OnAttemptToTakeResource(this,x,y,ResID))return INITBEST;
	//checking ability
	NewMonster* NM=newMons;
	if(!(NM->CompxUnit&&NM->CompxUnit->CanTakeExRes)){
		switch(ResID){
		case 0://wood
			if(!NM->GetAnimation(anm_WorkTree)->Enabled)return INITBEST;
			break;
		case 1://gold
		case 2://stone
			if(!NM->GetAnimation(anm_WorkStone)->Enabled)return INITBEST;
			break;
		case 3:
			if(!NM->GetAnimation(anm_WorkField)->Enabled)return INITBEST;
			break;
		default:
			return  INITBEST;
		};
		if(RType!=ResID){
			RAmount=0;
			RType=0xFF;
		}
	}else{
#ifndef COSSACKS2
		if(!NM->CompxUnit->TakeResStage[ResID])return INITBEST;
		if(RType!=ResID||RType!=NM->CompxUnit->TransformTo[ResID]){
			RAmount=0;
			RType=0xFF;
		};
#endif //COSSACKS2	
	};
	int xx=x;
	int yy=y;
#ifdef COSSACKS2
	int DObj;
	if(x==-1){
		DObj=y+OBJDELTA;
	}else{
		DObj=FindResourceObject(&xx,&yy,RealX,RealY,ResID);
		if(DObj==INITBEST&&OrdType!=1)return INITBEST;		
	};
#else //COSSACKS2
	int DObj=FindResourceObject(&xx,&yy,RealX,RealY,ResID);
	if(DObj==INITBEST&&OrdType!=1)return INITBEST;
#endif //COSSACKS2
	RStage=0;
	Order1* Or1=CreateOrder(OrdType);
	if(!int(Or1))return INITBEST;
	Or1->PrioryLevel=Prio&127;
	Or1->OrderType=238;//Take resource
	Or1->OrderTime=0;
	Or1->DoLink=&TakeResLink;
	Or1->info.TakeRes.x=xx;
	Or1->info.TakeRes.y=yy;
	Or1->info.TakeRes.ResID=ResID;
	Or1->info.TakeRes.SprObj=DObj;
	//PrioryLevel=Prio&127;
	NothingTime=0;
	NearBase=0xFFFF;
	PrioryLevel=LocalOrder->PrioryLevel;
	MakeOrderSound(this,2);
	return DObj;
};
extern bool FastMode;	
int GOINDEX=0;
int CheckNtIntegrity(byte NI){
	if(NI>=0&&NI<8){
		int N=NtNUnits[NI];
		word* IDS=NatList[NI];
		int S=0;
		for(int p=0;p<N;p++){
			word MID=IDS[p];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB)S+=OB->RealDir;
			};
		}
		return S;
	};
	return 1;
};
void TakeResLink(OneObject* OBJ){
	int DObj=OBJ->LocalOrder->info.TakeRes.SprObj;
	if(DObj>=MaxSprt){
		OBJ->DeleteLastOrder();
		return;
	}
	int c=OBJ->NewAnm->Code;
	if(c>=anm_Attack && c<anm_Attack+32){
		int af=OBJ->NewAnm->ActiveFrame;
		if(OBJ->NewCurSprite>=af && OBJ->NewCurSpritePrev<=af){
			OneSprite* OS=&Sprites[DObj];
			if(OS->M4 && !OS->CheckActionPresence('PUSH')){
				if(rando()<1024 && OS->CurrentModIndex!=-1 && !OS->CheckActionPresence('LAND')){
                    int L=GetLandTypeInPoint(OS->x,OS->y);
					ChangeTreeLandType* CLT=new ChangeTreeLandType;
					CLT->Create(L);
					OS->AddSpriteAction(CLT);
				}else{
					PushSprite3D* P=new PushSprite3D;
					P->Create(OS,OBJ->RealX>>4,OBJ->RealY>>4,0.05,800,4);
					OS->AddSpriteAction(P);
				}
			}
		}
	}
	if(!OBJ->FrameFinished){
		if(OBJ->NewAnm->CanBeBrokenAfterFrame>OBJ->NewCurSprite)return;
	}
	rando();
	//addrand(CheckNtIntegrity(OBJ->NNUM));
	GOINDEX=OBJ->Index;
	OBJ->PrioryLevel=OBJ->LocalOrder->PrioryLevel;
	OBJ->UnBlockUnit();
	NewMonster* NM=OBJ->newMons;
	
	int xx=OBJ->LocalOrder->info.TakeRes.x;
	int yy=OBJ->LocalOrder->info.TakeRes.y;
	byte ResType=OBJ->LocalOrder->info.TakeRes.ResID;
	addrand(DObj);
	addrand(xx);
	addrand(yy);
	byte ExResType=ResType;
	if(NM->CompxUnit){
		ResType=NM->CompxUnit->TransformTo[ResType];
	};
	if(ResType>8){
		OBJ->DeleteLastOrder();
		//addrand(CheckNtIntegrity(OBJ->NNUM));
		return;
	};
	bool GOIN=DObj>=OBJDELTA?(OBJ->RAmount):(OBJ->RAmount>=NM->MaxResPortion[ResType]);
	if(ResType!=0xFF&&GOIN){
FindNow:
		if(OBJ->NearBase==0xFFFF){
			OBJ->RType=ExResType;
			OBJ->NearBase=OBJ->FindNearestBase();
		}
		if(OBJ->NearBase==0xFFFF){
			if(OBJ->NNUM==MyNation){
				void fes_neSklad();
				fes_neSklad();
			}
			Order1* OR1=OBJ->LocalOrder;
			OBJ->LocalOrder=OR1->NextOrder;
			OBJ->FreeOrdBlock(OR1);
			OBJ->UnBlockUnit();
			OBJ->UnlimitedMotion=0;
			//addrand(CheckNtIntegrity(OBJ->NNUM));
			return;
		};
		OneObject* OB=Group[OBJ->NearBase];
		if(!OB||OB->Sdoxlo){
			OBJ->NearBase=0xFFFF;
			goto FindNow;
		};
		if(!OB->AbRes){
			if(OBJ->NearBase==0xFFFF){
				OBJ->NearBase=OBJ->FindNearestBase();
			};
			if(OBJ->NearBase==0xFFFF){
				Order1* OR1=OBJ->LocalOrder;
				OBJ->LocalOrder=OR1->NextOrder;
				OBJ->FreeOrdBlock(OR1);
				OBJ->UnBlockUnit();
				OBJ->UnlimitedMotion=0;
				//addrand(CheckNtIntegrity(OBJ->NNUM));
				return;
			};
		};
		NewMonster* NM1=OB->newMons;
		short dsx=0;
        short dsy=0;
        if(NM1->NewParams.TakeResToBuildpoints && NM1->BuildPtX.GetAmount()){
            int N=NM1->BuildPtX.GetAmount();
            int PicSX=NM1->PicDx<<4;
            int PicSY=(NM1->PicDy)<<5;
            int x0=((OB->RealX+PicSX)>>8);
            int y0=((OB->RealY+PicSY)>>8);
            int rm=100000000;
            for(int i=0;i<N;i++){
                int xx=x0+NM1->BuildPtX[i];
                int yy=y0+NM1->BuildPtY[i];
                int R=OBJ->DistTo(xx,yy);
				if(CheckPt(xx,yy))R+=10000;
                if(R<rm){
                    rm=R;
                    dsx=xx;
                    dsy=yy;
                }
            }
            if(rm>3){
                OBJ->NewMonsterSendTo(dsx<<8,dsy<<8,128+16,1);
                return;
            }
        }else
		if(NM1->BornPtX.GetAmount()){
			if(OBJ->UnlimitedMotion){
				//addrand(CheckNtIntegrity(OBJ->NNUM));
					AdvCharacter* ADC=OB->MoreCharacter;
				int ramn=div(int(OBJ->RAmount)*ADC->ResEff,100).quot;
				if(ResType==TreeID){
					ramn=(ramn*int(NATIONS[OBJ->NNUM].WoodEff))/100;
					OBJ->Nat->CITY->WoodAmount+=ramn;
				}else
				if(ResType==StoneID){
					ramn=(ramn*int(NATIONS[OBJ->NNUM].StoneEff))/100;
					OBJ->Nat->CITY->StoneAmount+=ramn;
				}else
				if(ResType==FoodID){
					int eff=(NATIONS[OBJ->NNUM].FoodEff)%100;
					ramn=(ramn*eff)/10;
					OBJ->Nat->CITY->FoodAmount+=ramn;
				};
				addrand(XRESRC(OBJ->NNUM,ResType));
				AddXRESRC(OBJ->NNUM,ResType,ramn);
				addrand(XRESRC(OBJ->NNUM,ResType));
				NATIONS[OBJ->NNUM].AddResource(ResType,ramn);
				NATIONS[OBJ->NNUM].ResTotal[ResType]+=ramn;
				OBJ->RAmount=0;
				OBJ->RType=0xFF;
				OBJ->NewState=0;
				OBJ->GroundState=0;
				addrand(OB->GroundState+1);

				int PicSX=NM1->PicDx<<4;
				int PicSY=(NM1->PicDy)<<5;
				int x0=((OB->RealX+PicSX)>>8);
				int y0=((OB->RealY+PicSY)>>8);
				int np=NM1->BornPtX.GetAmount();
				OBJ->UnlimitedMotion=0;
				OBJ->ClearOrderedUnlimitedMotion(1);
				//addrand(CheckNtIntegrity(OBJ->NNUM));
				for(int j=np-1;j>=0;j--){
					OBJ->NewMonsterPreciseSendTo3D(((x0<<4)+NM1->BornPtX[j])<<4,((y0<<4)+NM1->BornPtY[j])<<4,j>0?NM1->BornPtZ[j-1]:NM1->BornPtZ[j],NM1->BornPtZ[j],128+127,1+128);
				};
				OBJ->UnlimitedMotion=1;
				//addrand(CheckNtIntegrity(OBJ->NNUM));
				return;
			}else{
				int PicSX=NM1->PicDx<<4;
				int PicSY=(NM1->PicDy)<<5;
				int x0=((OB->RealX+PicSX)>>8);
				int y0=((OB->RealY+PicSY)>>8);
				int np=NM1->BornPtX.GetAmount();
				int dsx0=x0+(NM1->ConcPtX[0]>>4);
				int dsy0=y0+(NM1->ConcPtY[0]>>4);
				int NRM=Norma(dsx0-OBJ->x,dsy0-OBJ->y);
				if(NRM<5){
					//OBJ->ClearOrderedUnlimitedMotion(1);
					OBJ->UnlimitedMotion=0;
					np=NM1->ConcPtX.GetAmount();
					for(int j=np-1;j>=0;j--){
						OBJ->NewMonsterPreciseSendTo3D(((x0<<4)+NM1->ConcPtX[j])<<4,((y0<<4)+NM1->ConcPtY[j])<<4,j>0?NM1->ConcPtZ[j-1]:NM1->ConcPtZ[j],NM1->ConcPtZ[j],128+127,1);
					};
					OBJ->UnlimitedMotion=1;
					//addrand(CheckNtIntegrity(OBJ->NNUM));
					return;
				}else{
					OBJ->NewState=OBJ->GroundState;
					OBJ->CreatePath(dsx0,dsy0);	
					//addrand(CheckNtIntegrity(OBJ->NNUM));
					return;
				};
			};

		}else
			if(NM1->ConcPtX.GetAmount()){
			int PicSX=NM1->PicDx<<4;
			int PicSY=(NM1->PicDy)<<5;
			int x0=((OB->RealX+PicSX)>>8);
			int y0=((OB->RealY+PicSY)>>8);
			int np=NM1->ConcPtX.GetAmount();
			int Nr=1000000;
			for(int j=0;j<np;j++){
				int dsx0=x0+(NM1->ConcPtX[j]>>4);
				int dsy0=y0+(NM1->ConcPtY[j]>>4);
				int NRM=Norma(dsx0-OBJ->x,dsy0-OBJ->y);
				if(NRM<Nr){
					dsx=dsx0;
					dsy=dsy0;
					Nr=NRM;
				};
			};
		}else{
			dsx=OB->RealX>>8;
			dsy=OB->RealY>>8;
		};
		if(Norma(OBJ->x-dsx,OBJ->y-dsy)<6){
			AdvCharacter* ADC=OB->MoreCharacter;
			int ramn=div(int(OBJ->RAmount)*ADC->ResEff,100).quot;
			if(ResType==TreeID){
				ramn=(ramn*int(NATIONS[OBJ->NNUM].WoodEff))/100;
				OBJ->Nat->CITY->WoodAmount+=ramn;
			}else
			if(ResType==StoneID){
				ramn=(ramn*int(NATIONS[OBJ->NNUM].StoneEff))/100;
				OBJ->Nat->CITY->StoneAmount+=ramn;
			}else
			if(ResType==FoodID){
				int eff=(NATIONS[OBJ->NNUM].FoodEff)%100;
				ramn=(ramn*eff)/10;
				OBJ->Nat->CITY->FoodAmount+=ramn;
			};
			addrand(XRESRC(OBJ->NNUM,ResType));
			AddXRESRC(OBJ->NNUM,ResType,ramn);
			addrand(XRESRC(OBJ->NNUM,ResType));
			NATIONS[OBJ->NNUM].AddResource(ResType,ramn);
			NATIONS[OBJ->NNUM].ResTotal[ResType]+=ramn;
			OBJ->RAmount=0;
			OBJ->RType=0xFF;
			OBJ->NewState=0;
		}else{
			if(OBJ->NothingTime>8){
				if(OBJ->CheckBlocking()){
					OBJ->DeleteBlocking();
					return;
				}else{
					OBJ->CreatePath(dsx,dsy);
				};
			}else{
				OBJ->CreatePath(dsx,dsy);
			};
			return;
		};
	};
	if(DObj==INITBEST){
		//Resource still not found!
		DObj=FindResourceObject(&xx,&yy,OBJ->RealX,OBJ->RealY,ExResType);
		addrand(DObj);
		if(DObj==INITBEST){
			//Resource does not exists
			Order1* OR1=OBJ->LocalOrder;
			OBJ->LocalOrder=OR1->NextOrder;
			OBJ->FreeOrdBlock(OR1);
			OBJ->UnBlockUnit();
			return;
		};
		OBJ->LocalOrder->info.TakeRes.SprObj=DObj;
		OBJ->LocalOrder->info.TakeRes.x=xx;
		OBJ->LocalOrder->info.TakeRes.y=yy;
	};
#ifdef COSSACKS2
	if(DObj>=OBJDELTA){
		word MID=DObj-OBJDELTA;
		if(MID<0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&OB->NNUM==OBJ->NNUM&&OB->newMons->ProdType){
				OBJ->DeleteLastOrder();
				OBJ->GoToMine(OB->Index,128+16,1);
				return;
			};
		};
		OBJ->DeleteLastOrder();
		return;
	}else{
#endif //COSSACKS2
		OneSprite* OS=&Sprites[DObj];
		if(!OS->Enabled){
			OBJ->LocalOrder->info.TakeRes.SprObj=INITBEST;
			return;
		};
		ObjCharacter* OC=OS->OC;
		if(OBJ->NothingTime>10){
			OBJ->UnBlockUnit();
			DObj=FindLimResourceObject(&xx,&yy,OBJ->RealX,OBJ->RealY,ExResType,DObj);
			addrand(DObj);
			if(DObj!=INITBEST){
				OBJ->LocalOrder->info.TakeRes.SprObj=DObj;
				OBJ->LocalOrder->info.TakeRes.x=xx;
				OBJ->LocalOrder->info.TakeRes.y=yy;
				OS=&Sprites[DObj];
				OBJ->NothingTime=0;
				OC=OS->OC;
			};
		};
		if(!(OC->ResType==ExResType&&OS->Enabled)){
			OBJ->UnBlockUnit();
			DObj=FindResourceObject(&xx,&yy,OBJ->RealX,OBJ->RealY,ExResType);
			if(DObj==INITBEST){
				//Resource does not exists
				Order1* OR1=OBJ->LocalOrder;
				OBJ->LocalOrder=OR1->NextOrder;
				OBJ->FreeOrdBlock(OR1);
				return;
			};
			OBJ->LocalOrder->info.TakeRes.SprObj=DObj;
			OBJ->LocalOrder->info.TakeRes.x=xx;
			OBJ->LocalOrder->info.TakeRes.y=yy;
			OS=&Sprites[DObj];
			OC=OS->OC;
		};
		OC=OS->OC;
		int odx=(OBJ->RealX>>4)-OS->x;
		int ody=(OBJ->RealY>>4)-OS->y;
		int Rdist=Norma(odx,ody);
		if(Rdist<OC->WorkRadius){
			OBJ->DeletePath();
			OBJ->RealVx=0;
			OBJ->RealVy=0;
			OBJ->DestX=-1;
			char dirc=char(GetDir(-odx,-ody));
			char ddir=dirc-OBJ->RealDir;
			if(ddir<16){
				OBJ->RealDir=dirc;
				OBJ->GraphDir=dirc;
				OBJ->SetZeroFrame();
				if(NM->CompxUnit&&NM->CompxUnit->CanTakeExRes){
					byte ST=NM->CompxUnit->TakeResStage[ExResType];
					if(OBJ->LocalNewState==ST){
						OBJ->DeletePath();
						OBJ->InMotion=false;
						OBJ->BackMotion=false;
						OBJ->RType=ExResType;	
						if(ResType==FoodID&&OS->SG==&COMPLEX){
							OBJ->RAmount+=OS->PerformWork(int(NATIONS[OBJ->NNUM].FoodEff/100)*10);
						}else OBJ->RAmount+=OS->PerformWork(100);
						OBJ->BlockUnit();
						OBJ->NothingTime=0;
						OBJ->NearBase=0xFFFF;
						OBJ->NewAnm=NM->GetAnimation(anm_Attack+OBJ->LocalNewState-1);
						OBJ->SetZeroFrame();
					}else{
						OBJ->NewState=ST;
						TryToStand(OBJ,false);
					};
				}else{
					switch(ResType){
					case 0://wood
						OBJ->NewAnm=NM->GetAnimation(anm_WorkTree);
						break;
					case 1://stone
					case 2://gold
						OBJ->NewAnm=NM->GetAnimation(anm_WorkStone);
						break;
					case 3:
						OBJ->NewAnm=NM->GetAnimation(anm_WorkField);
					};
					rando();
					OBJ->DeletePath();
					OBJ->InMotion=false;
					OBJ->BackMotion=false;
						OBJ->RType=ExResType;
					if(ResType==FoodID&&OS->SG==&COMPLEX){
						OBJ->RAmount+=OS->PerformWork(int(NATIONS[OBJ->NNUM].FoodEff/100)*10);
					}else OBJ->RAmount+=OS->PerformWork(100);
					OBJ->BlockUnit();
					OBJ->NothingTime=0;
					OBJ->NearBase=0xFFFF;
				};
			}else{
				if(ddir>0)OBJ->RealDir+=16;
				else OBJ->RealDir-=16;
				OBJ->GraphDir=OBJ->RealDir;

			};
		}else{
			if(Rdist<128){
				//checking for place to stand
				if(CheckBar((xx>>4)-1,(yy>>4)-1,3,3)){
					//Place for peasant is oocupied by other!
					//Let us find new place
					if(FindPlaceForPeasant(&xx,&yy,OS->x,OS->y,OC->WorkRadius)){
						OBJ->LocalOrder->info.TakeRes.x=xx;
						OBJ->LocalOrder->info.TakeRes.y=yy;
						OBJ->CreatePath(OS->x>>4,OS->y>>4);
						//OBJ->CreatePath(xx>>4,yy>>4);
					}else{
						OBJ->UnBlockUnit();
						DObj=FindResourceObject(&xx,&yy,OBJ->RealX,OBJ->RealY,ResType);
						if(DObj==INITBEST){
							//Resource does not exists
							Order1* OR1=OBJ->LocalOrder;
							OBJ->LocalOrder=OR1->NextOrder;
							OBJ->FreeOrdBlock(OR1);
							return;
						};
						OBJ->LocalOrder->info.TakeRes.SprObj=DObj;
						OBJ->LocalOrder->info.TakeRes.x=xx;
						OBJ->LocalOrder->info.TakeRes.y=yy;
						TryToStand(OBJ,false);
					};
					return;
				}else{
					if(OBJ->NothingTime>8){
						if(OBJ->CheckBlocking())OBJ->DeleteBlocking();
						else{
							OBJ->CreatePath(OS->x>>4,OS->y>>4);
							//OBJ->CreatePath(xx>>4,yy>>4);
						};
					}else{
						OBJ->CreatePath(OS->x>>4,OS->y>>4);
						//OBJ->CreatePath(xx>>4,yy>>4);
					};
				};
			}else{
				if(OBJ->NothingTime>8){
					if(OBJ->CheckBlocking())OBJ->DeleteBlocking();
					else{
						OBJ->CreatePath(OS->x>>4,OS->y>>4);
						//OBJ->CreatePath(xx>>4,yy>>4);
					};
				}else{
					OBJ->CreatePath(OS->x>>4,OS->y>>4);
					//OBJ->CreatePath(xx>>4,yy>>4);
				};
				//OBJ->DestX=xx<<4;
				//OBJ->DestY=yy<<4;
			};
		};
#ifdef COSSACKS2
	};
#endif //COSSACKS2
};
byte FindAnyResInCell(int x,int y,int cell,int* Dist,byte Res){
	if(cell<0||cell>=VAL_SPRSIZE)return Res;
	int* CEL=SpRefs[cell];
	int   NCEL=NSpri[cell];
	if(!(CEL&&NCEL))return Res;
	int dst;
	int dist=*Dist;
	byte rsr=Res;
	for(int i=0;i<NCEL;i++){
		OneSprite* OS=&Sprites[CEL[i]];
		//assert(OS->Enabled);
		if(OS){
			dst=Norma((x-OS->x)*2,y-OS->y);
			if(dst<dist){
				ObjCharacter* OC=OS->OC;
				if(OC->ResType<0xFE){
					dist=dst;
					rsr=OC->ResType;
				}
			}
		}
	}
	*Dist=dist;
	return rsr;
};
byte DetermineResource(int x,int y){
	int cell=(x>>7)+((y>>7)<<SprShf);
	int DistR=10000;
	byte res=0xFF;
	for(int dx=-2;dx<=2;dx++){
		for(int dy=-2;dy<=2;dy++){
			res=FindAnyResInCell(x,y,cell+dx+dy*VAL_SPRNX,&DistR,res);
		}
	}
	if(DistR<160)return res;
	else return 0xFF;
};
extern int tmtmt;
void ProcessTreesChange();
void ProcessSprites(){
	if(!(tmtmt&31)){
		for(int i=0;i<MaxSprt;i++)Sprites[i].Surrounded=false;
	};
	ProcessTreesChange();
	OneSprite::ProcessActiveSprites();	
};
bool OneObject::CheckLocking(int dx,int dy){
	return CheckBar(x+dx-1,y+dy-1,Lx+2,Lx+2);
};
void OneObject::SetDestCoor(int x,int y){
	DestX=(x<<8)+((Lx)<<7);
	DestY=(y<<8)+((Lx)<<7);
	addrand(DestX);
	addrand(DestY);
};
void OneObject::EscapeLocking(){
	//round 1
	if(!CheckLocking(-1,0)){
		DeletePath();
		SetDestCoor(x-1,y);
		return;
	};
	if(!CheckLocking(1,0)){
		DeletePath();
		SetDestCoor(x+1,y);
		return;
	};
	if(!CheckLocking(0,-1)){
		DeletePath();
		SetDestCoor(x,y-1);
		return;
	};
	if(!CheckLocking(0,1)){
		DeletePath();
		SetDestCoor(x,y+1);
		return;
	};
	if(!CheckLocking(-1,-1)){
		DeletePath();
		SetDestCoor(x-1,y-1);
		return;
	};
	if(!CheckLocking(1,1)){
		DeletePath();
		SetDestCoor(x+1,y+1);
		return;
	};
	if(!CheckLocking(1,-1)){
		DeletePath();
		SetDestCoor(x+1,y-1);
		return;
	};
	if(!CheckLocking(-1,1)){
		DeletePath();
		SetDestCoor(x-1,y+1);
		return;
	};
	if(!CheckLocking(-2,0)){
		DeletePath();
		SetDestCoor(x-2,y);
		return;
	};
	if(!CheckLocking(2,0)){
		DeletePath();
		SetDestCoor(x+2,y);
		return;
	};
	if(!CheckLocking(0,2)){
		DeletePath();
		SetDestCoor(x,y+2);
		return;
	};
	if(!CheckLocking(0,-2)){
		DeletePath();
		SetDestCoor(x,y-2);
		return;
	};
};
//-----------------Time handler-------------------//

TimeReq::TimeReq(){
    NMembers=0;
    MaxMembers=0;
    IDS=NULL;
    Kinds=NULL;
};
TimeReq::~TimeReq(){
    if(IDS)free(IDS);
    if(Kinds)free(Kinds);
    IDS=NULL;
    Kinds=NULL;
    NMembers=0;
    Kinds=0;
	MaxMembers=0;
};
void TimeReq::Add(int ID,byte kind){
    if(!MaxMembers){
        MaxMembers=32;
        IDS=znew(int,MaxMembers);
        Kinds=znew(byte,MaxMembers);
    }else{;
        if(NMembers>=MaxMembers){
            MaxMembers+=32;
            IDS=(int*)realloc((void*)IDS,MaxMembers<<2);
            Kinds=(byte*)realloc((void*)Kinds,MaxMembers);
        };
    };
    IDS[NMembers]=ID;
    Kinds[NMembers]=kind;
    NMembers++;
};
void TimeReq::Del(int ID,byte Kind){
    for(int i=0;i<NMembers;i++){
        if(IDS[i]==ID&&Kinds[i]==Kind){
            if(i<NMembers-1){
                memcpy(IDS+i,IDS+i+1,(NMembers-i-1)<<2);
                memcpy(Kinds+i,Kinds+i+1,NMembers-i-1);
            };
            NMembers--;
        };
    };
};
void TimeReq::Handle(){
    for(int i=0;i<NMembers;i++){
        int ID=IDS[i];
        byte Kind=Kinds[i];
        if(!Kind){
            OneSprite* OS=&Sprites[ID];
            ObjCharacter* OC=OS->OC;
            SprGroup* SG=OS->SG;
            OS->TimePassed+=GameSpeed;
            if(OC->TimeAmount&&OS->TimePassed>=(OC->TimeAmount<<8)){
				ObjCharacter* NOC=OC->NextTimeObj.Get();
                if(NOC){
                    if(NOC!=OC)OS->WorkOver=0;
                    OS->SGIndex=NOC->ParentIndex;
		            OS->OC=NOC;
					if(OS->OC->GrassHeight){
						CheckGrassPresence(OS->x,OS->y,OS->OC->GrassRadius);
					}
                }else{
                    OS->Enabled=false;
                    Del(OS->Index,0);
                    UnregisterSprite(OS->Index);
                };
                OS->TimePassed=0;
            };
        };
    };
};
extern TimeReq ObjTimer;
void KillTimeReq(){
    ObjTimer.NMembers=0;
}
int MaxSprt;
int GetSpriteEdges(SprGroup* SG,int SprIndex,int* xy,int* pType,int& NOut){
	NOut=0;
	ObjCharacter* OC=SG->Objects[SprIndex];
	int NA=OC->CONN.GetAmount();
	if(!NA)return 0;
	for(int i=0;i<NA;i++){
		xy[i+i]=OC->CONN[i]->x-OC->CenterX;
		xy[i+i+1]=(OC->CONN[i]->y-OC->CenterY)*2;
		pType[i]=short(OC->CONN[i]->ConnType);
	}
	NOut=OC->NOutConn;
	return NA;
}
void FindBestPositionForSprite(SprGroup* SG,int Index,int& x,int& y){
	int ppXY[64];
	int ctp[32];
	int nout;
	int nc=GetSpriteEdges(SG,Index,ppXY,ctp,nout);
	if(!nc)return;
    int MinR=100000;
	int bestX=-1;
	int bestY=-1;
	int cx0=x>>7;
	int cy0=y>>7;
	for(int dx=-2;dx<=2;dx++){
		for(int dy=-2;dy<=2;dy++){
			int cell=(cx0+dx)+((cy0+dy)<<SprShf);
			if(cell>=0&&cell<VAL_SPRSIZE){
				int* CEL=SpRefs[cell];
				int   NCEL=NSpri[cell];
				if(CEL&&NCEL){
					for(int i=0;i<NCEL;i++){
						OneSprite* OS=&Sprites[CEL[i]];
						if(OS->Enabled){
							ObjCharacter* OC=OS->OC;
							int cs_ppXY[64];
							int cs_ctp[32];
							int cs_nout;
							int cs_nc=GetSpriteEdges(OS->SG,OS->SGIndex,cs_ppXY,cs_ctp,cs_nout);
							if(cs_nc){
								for(int p=0;p<cs_nc;p++){
									for(int q=0;q<nout;q++)if(cs_ctp[p]&&cs_ctp[p]==-ctp[q]){
										int bx=OS->x+cs_ppXY[p+p  ]-ppXY[q+q];
										int by=OS->y+cs_ppXY[p+p+1]-ppXY[q+q+1];
										int R=Norma(x-bx,y-by);
										if(R<MinR&&R<200){
											//if(CheckSpritesInArea(bx<<4,by<<4,32)){
												MinR=R;
												bestX=bx;
												bestY=by;
											//}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if(bestX!=-1){
		x=bestX;
		y=bestY;
	}
}
/*
class V3Dint{
public:
	V3Dint(){
		x=0;y=0;z=0;
	}
	V3Dint(int X,int Y,int Z){
        x=X;y=Y;z=Z;
	}
	int x,y,z;
};
class OneExtraPlane{
	int a,b,c;//z=(a(x-x1)+b(y-y1)+c)/256;
public:
	union{
		struct{
			V3Dint P1;	
			V3Dint P2;
			V3Dint P3;
		};
		struct{
            V3Dint PTS[3];
		};
	};
	int OwnerID;
	byte OwnerType;
	OneExtraPlane(){
	}
	OneExtraPlane(V3Dint& p1,V3Dint& p2,V3Dint& p3){
        c=p1.z;
		int D=(P2.x-P1.x)*(P3.y-P1.y)-(P2.y-P1.y)*(P3.x-P1.x);
		if(D!=0){
            a=((P3.y-P1.y)*(P2.z-P1.z)+(P2.y-P1.y)*(P3.z-P1.z))*256/D;
			b=-((P3.x-P1.x)*(P2.z-P1.z)+(P2.x-P1.x)*(P3.z-P1.z))*256/D;
		}else{
			a=0;
			b=0;
		}
	}
	int CheckIfPointInside(int x,int y){
		int ncr=0;
		for(int i=0;i<3;i++){
			int i1=(i+1)%3;
			int x0=PTS[i ].x;
			int y0=PTS[i ].y;
			int x1=PTS[i1].x;
			int y1=PTS[i1].y;
			if((x>=x0&&x<x1)||(x>=x1&&x<x0)){
                int yy=y0+(x-x0)*(y1-y0)/(x1-x0);
				if(yy<y)ncr++;
			}
		}
		return ncr&1;
	}
	int GetPlaneHeihgt(int x,int y){
		return ((a*(x-P1.x)+b*(y-P1.yp))>>8)+P1.z;
	}
};
class OneCellWithPlane{
	DynArray<OneExtraPlane> Planes;
};
class PS_Key{
public:
	int x,y;	
	unsigned int	hash	   () const {return x+(y*1779);}
	bool			equal	   ( const VKey& el ){return x==el.x&&y==el.y;}
	bool			operator ==( const VKey& el ){return x==el.x&&y==el.y;}
	void			copy	   ( const VKey& el ){x=el.x;y=el.y;}
};
class PlanesScope{
	PonterHash<OneCellWithPlane,PS_Key> PHash;
public:
	int GetPlanesHeight(int x,int y){
		PS_Key pk;
		pk.x=x>>7;
		pk.y=y>>7;
		int idx=PHash.find(pk);
		if(idx!=NO_ELEMENT){
            OneCellWithPlane* OCP=PHash.get(idx);
			int mv=-10000;
			if(OCP){
				int N=OCP->Planes.GetAmount();
				for(int i=0;i<N;i++){
					if(OCP->Planes[i].CheckIfPointInside(x,y)){
						int H=OCP->Planes[i].GetPlaneHeihgt(x,y);
						if(H>mv)mv=H;
					}
				}
			}
			return mv;
		}else return -10000;
	}
	void AddPlane(OneExtraPlane* OP,int OwnerID,byte OwnerType){
		int minx=100000;
		int maxx=-100000;
		int miny=100000;
		int maxy=-100000;
		for(int i=0;i<3;i++){
			int x=OP->PTS[i].x>>7;
			int y=OP->PTS[i].y>>7;
            if(x<minx)minx=x;
			if(x>maxx)maxx=x;
			if(y<miny)miny=y;
			if(y>maxy)maxy=y;
		}
		for(int ix=minx;ix<=maxx;ix++){
			for(int iy=miny;iy<=maxy;iy++){
				PS_Key pk;
				pk.x=ix;
				pk.y=iy;
				int idx=PHash.find(pk);
				if(idx!=NO_ELEMENT){
					OneCellWithPlane* OCP=PHash.get(idx);
					int mv=-10000;
					if(OCP){

					}
				}else{

				}
			}
		}
	}
};
*/

void ObjCharacter::SetModelDefaults(){
	Scale=1.0f;
	RotX=0.0f;
	RotY=0.0f;
	ModelDZ=0.0;
	RotFI=0.0f;
}
const char* ObjCharRef::GetThisElementView(const char* Local){
	if(Get())return Get()->Name.pchar();
	else return Local;
}
extern ModelsScope RenderModels;
typedef bool cbCheckSprite(OneSprite* OS, void* Param);
int GetSpritesInRadius(int x, int y, int Radius, cbCheckSprite* cbF, void* Param)
{
	int NR=(Radius>>7)+1;
	int cx0=x>>7;
	int cy0=y>>7;
	int NN=0;
	for(int i=0;i<NR;i++)
	{
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++)
		{
			int xx=cx0+xi[j];
			int yy=cy0+yi[j];
			if(xx>=0&&yy>=0&&xx<VAL_MAXCX&&yy<VAL_MAXCX)
			{
				int cell=xx+(yy<<SprShf);
				if(cell>=0&&cell<VAL_SPRSIZE)
				{
					int* CEL=SpRefs[cell];
					int  NCEL=NSpri[cell];
					if(CEL&&NCEL)
					{
						for(int i=0;i<NCEL;i++)
						{
							OneSprite* OS=&Sprites[CEL[i]];
							if(OS->Enabled)
							{
								if(Norma(OS->x-x,OS->y-y)<Radius)
								{
									if(cbF(OS,Param))NN++;
								}
							}
						}
					}
				}
			}
		}
	}			
	return NN;
}
void AddSG2Stream(int sindex,SprGroup* SG,EnumeratorOutputStream* out){
	for(int i=0;i<SG->Sections.GetAmount();i++){
		OneSprSection* SS=SG->Sections[i];
		for(int j=0;j<SS->ObjectsList.GetAmount();j++){
			ObjCharacter* OC=SS->ObjectsList[j]->Get();
			if(OC){
				out->add(OC->Name.pchar(),(sindex<<16)+OC->SpriteIndex,SS->SectionName.pchar());
				OC->GroupIndex=i+1;
			}
		}
	}
	for(int i=0;i<SG->Objects.GetAmount();i++){
		ObjCharacter* OC=SG->Objects[i];
		if(OC->GroupIndex==0){
			out->add(OC->Name.pchar(),(sindex<<16)+OC->SpriteIndex,"other...");
		}
	}
}
DWORD fnRecreateTreesEnumerator(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	int n=0;
	if(PrevCallHash!=1234){
        AddSG2Stream(0,&TREES,out);		
	}
	return 1234;
}
void CreateTreesEnumerator(){
	RefreshableEnumerator* RE=new RefreshableEnumerator(fnRecreateTreesEnumerator);
	Enumerator* E=ENUM.Get("TREESTYPES");
	E->SetProcEnum(RE);
}
DynArray<int> OneSprite::ActiveSprites;
void OneSprite::ClearActiveSprites(){
	for(int i=0;i<ActiveSprites.GetAmount();i++){
		OneSprite* OS=Sprites+ActiveSprites[i];
		for(int j=0;j<OS->SpriteActions.GetAmount();j++){
			delete(OS->SpriteActions[j]);
		}
		OS->SpriteActions.DynArray<ActionOverSprite*>::Clear();
	}
	ActiveSprites.Clear();
}
void OneSprite::ProcessActiveSprites(){
	for(int i=0;i<ActiveSprites.GetAmount();i++){
        OneSprite* OS=Sprites+ActiveSprites[i];
		for(int j=0;j<OS->SpriteActions.GetAmount();j++){
			if(!OS->SpriteActions[j]->Process(OS)){
				delete(OS->SpriteActions[j]);
				OS->SpriteActions.Del(j--,1);
			}
		}
		if(!OS->SpriteActions.GetAmount()){
			ActiveSprites.Del(i--,1);
		}
	}
}
void OneSprite::AddSpriteAction(ActionOverSprite* Action){
	if(!SpriteActions.GetAmount())ActiveSprites.Add(Index);
    SpriteActions.Add(Action);
}
bool OneSprite::CheckActionPresence(DWORD Code){
	for(int i=0;i<SpriteActions.GetAmount();i++){
		if(SpriteActions[i]->GetActionID()==Code)return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
bool PushSprite3D::Create(OneSprite* OS,int PusherX,int PusherY,float PushAngle,int ReturnBackTime,float NOscillations){
	if(OS->M4){
		Vector3D axe(PusherY-OS->y,-PusherX+OS->x,0);
		StartMatrix=*OS->M4;
		FinalMatrix=StartMatrix;
		Matrix4D M;
		M.rotation(axe,PushAngle);
		StartMatrix.mulLeft(M);
		CurrentTime=0;
		TitalTime=ReturnBackTime;
		NOscl=NOscillations;
		return true;
	}
	return false;
}
bool PushSprite3D::Process(OneSprite* OS){
	CurrentTime+=GameSpeed*40/256;
	if(CurrentTime<TitalTime){
		float w=float(TitalTime-CurrentTime)/TitalTime;
		float blend=w*w*sin((1-w)*c_PI*NOscl);
		*OS->M4=StartMatrix;
		(*OS->M4)*=blend;
		OS->M4->addWeighted(FinalMatrix,1-blend);
		return true;
	}
	*OS->M4=FinalMatrix;
	return false;
}
void ChangeTreeLandType::Create(int DestLand){
	SrcLandType=-1;
	TimePassed=0;
	DestTreeLandType=DestLand;
}
bool IgnoreTreesProcessing=false;
bool ChangeTreeLandType::Process(OneSprite* OS){
	if(IgnoreTreesProcessing)return false;
	if(OS->CurrentModIndex!=-1){		
		if(SrcLandType==-1){
            SrcLandType=OS->CurrentModIndex;
			DestTreeLandType=GetModIndexByLandType(OS,DestTreeLandType);
			//if(SrcLandType==DestTreeLandType)return false;
		}
		ObjCharacter* OC=OS->OC;
		TreeModificationParam* M1=OC->TreeMods[SrcLandType];
		TreeModificationParam* M2=OC->TreeMods[DestTreeLandType];
		int DT=M1->DeathTime*(16+OS->Index%32)/32;
		int RT=M2->RiseTime*(16+OS->Index%32)/32;
        int time=DT+RT+2;
		TimePassed+=GameSpeed*40/256;
		if(TimePassed<time){
			DWORD A=0xFF;
			if(TimePassed<DT+1){
                OS->RiseStage=1.0f;
				OS->DeathStage=1-float(TimePassed)/float(DT+1);
				OS->CurrentModIndex=SrcLandType;
				float x=1-OS->DeathStage;
				A=255*(1-x*x*x*x);
			}else{
				OS->DeathStage=1.0f;
				OS->RiseStage=float(TimePassed-DT)/float(RT+1);
				if(OS->CurrentModIndex!=DestTreeLandType){
					Matrix4D M;
					M.scaling(M2->Scale/M1->Scale);
					if(OS->M4)OS->M4->mulLeft(M);
				}
				OS->CurrentModIndex=DestTreeLandType;
			}
			int v=TimePassed*255/time;
			OS->Color=MulDWORD(MixDWORD(M2->Color,M1->Color,v,255-v),GetRXY(256-16,256+16,OS->x,OS->y));
			OS->Color=(A<<24)+(OS->Color&0xFFFFFF);
			return true;
		}else{
			OS->RiseStage=1.0f;
			OS->DeathStage=1.0f;
			OS->CurrentModIndex=DestTreeLandType;
			OS->Color=MulDWORD(M2->Color,GetRXY(256-16,256+16,OS->x,OS->y));
		}
	}
	return false;
}
void ProcessTreesChange(){
	if(IgnoreTreesProcessing)return;
	if(MaxSprt && MAXSPR){
		for(int i=0;i<30;i++){
			int p=(rando()+i)%MAXSPR;
			OneSprite* OS=Sprites+p;
			if(OS && OS->Enabled && OS->CurrentModIndex!=-1 && !OS->CheckActionPresence('LAND')){
                int L=GetLandTypeInPoint(OS->x,OS->y);
				if(L!=OS->OC->TreeMods[OS->CurrentModIndex]->LandType){
					ChangeTreeLandType* CLT=new ChangeTreeLandType;
					CLT->Create(L);
					OS->AddSpriteAction(CLT);
				}
			}
		}
	}
}
void SetCorrectTreesTypees(){
	for(int i=0;i<MAXSPR;i++){
		OneSprite* OS=Sprites+i;
		if(OS && OS->Enabled && OS->CurrentModIndex!=-1){
			int L=GetLandTypeInPoint(OS->x,OS->y);
			if(L!=OS->OC->TreeMods[OS->CurrentModIndex]->LandType){
				OS->RiseStage=1.0f;
				OS->DeathStage=1.0f;
				L=GetModIndexByLandType(OS,L);
				OS->CurrentModIndex=L;
				TreeModificationParam* M2=OS->OC->TreeMods[L];
				OS->Color=MulDWORD(M2->Color,GetRXY(256-16,256+16,OS->x,OS->y));
			}
		}
	}
}
void PutSpritesOnEarth(){
	for(int i=0;i<MAXSPR;i++){
		OneSprite* OS=Sprites+i;
		if(OS->Enabled){
			if(!OS->OC->EditableModel){
				if(OS->M4){
					OS->z=OS->M4->e32=GetTotalHeight(OS->x,OS->y);
				}
			}
		}
	}
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i]; 
		if(OB && !OB->Sdoxlo){
			OB->RZ=GetTotalHeight(OB->RealX>>4,OB->RealY>>4);
		}
	}
    TreesMesh.Clear();
    TreesMesh.Free();
}
void RecalcTrees(){
    for(int i=0;i<MAXSPR;i++){
        OneSprite* OSP=Sprites+i;
        if(OSP->Enabled && OSP->SG==&TREES){
            if(OSP->CurrentModIndex!=-1){
                OSP->Color=MulDWORD(OSP->OC->TreeMods[OSP->CurrentModIndex]->Color,GetRXY(256-16,256+16,OSP->x,OSP->y));
            }
            itr_GetSpritesInRadius.Create(OSP->x,OSP->y,64);
            while(OneSprite* OS=itr_GetSpritesInRadius.Next()){
                if(OS!=OSP){
                    if(abs(OS->y-OSP->y)<2){
                        OS->y-=4;
                    }
                }
            }
        }
    }
}