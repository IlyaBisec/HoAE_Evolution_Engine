#include "stdheader.h"
#include "kContext.h"
#include "unitability.h"
#include "CurrentMapOptions.h"

byte CurDrawNation=0;
class ZElement{
public:
	bool ShowAnimation;
	short XL,YL;
	OneObject* OB;
	bool IsDrawn;
	DWORD Diffuse;
	DWORD Handle; // Silver, 17.05.2004 - Unique entity handle
	float scale;
	float x,y;
	float z;
	bool Targeted;
	Vector3D Target;
	union{
		struct{
			word FileID;
			word SpriteID;
			word FileIDEx;
			word SpriteIDEx;

			int Param1,Param2;
			byte Nat;
			bool WorldPoint;		

			float Rotation;
			float ScaleX;
			float ScaleY;
			bool  AllowRotation;

			int pivotX;
			int pivotY;
		};
		struct{
            NewAnimation* ANM;			
			float Dir;
			int Frame;
			float fiDir;
			float fiOrt;
		};
	};
};
extern bool CINFMOD;
#define MAXZELM 16000
ZElement ZCashe[MAXZELM+32];
int NZElm;
#define NYLines 2000
#define DYZBuf 256
#define ZSHF  6
#define ZBFNX (1<<ZSHF)
byte ZBFCounter[NYLines];
word ZBUF [NYLines*ZBFNX];
word LOLAYER[4096];
int NLOITEMS;
word HILAYER[1024];
int NHIITEMS;
short ZBUFX[NYLines*ZBFNX];
//debug only!
word ZBUF_OLD [NYLines*ZBFNX];
word ZBUFX_OLD [NYLines*ZBFNX];
//-----------
word BRSel[256];
int NBSel;
void ClearZBuffer(){
	memset(ZBFCounter,0,NYLines);
	NZElm=0;
	NLOITEMS=0;
	NHIITEMS=0;
	NBSel=0;
};

extern int RealLy;
extern int mapy;
DWORD CurZDiffuse=0xFF808080;

float GetCurrentScale()
{
	return 1.0 / float(1<<(5-Shifter));
}

extern int TrueTime;

#define zoomsh (5-Shifter)
float GetVisibleSphereRadius(Vector3D Center,float Radius);
void AddAnimation(float x,float y,float z,NewAnimation* ANM,int Frame,float Dir,DWORD Diffuse,OneObject* OB,float Scale,float fiDir,float fiOrt,DWORD Handle){
	Scale=Scale*MOptions.GlobalMapScale/256;
	if(OB){
		NewMonster* NM=OB->newMons;
		if(NM->NewParams.LOD1_Scale>0){			
			float R=GetVisibleSphereRadius(Vector3D(x,y,z),NM->UnitCylinderHeight);
			if(R>NM->NewParams.LOD1_Scale){
				if(R>NM->NewParams.LOD2_Scale && NM->NewParams.LOD2_Scale>0){
					ANM=NM->GetAnimation(ANM->Code+10000);
				}else ANM=NM->GetAnimation(ANM->Code+5000);
			}
		}
	}
	if(ANM->Code==anm_RotateAtPlace) Dir=0;
	if(OB && OB->ActiveAbility){
		int Tr=OB->ActiveAbility->Transparency;
		int dT=TrueTime-OB->ActiveAbility->TransparencyStartTime;
		if( dT>=0 && dT<=255 ){
			int TrLast=OB->ActiveAbility->TransparencyLast;
			Tr=TrLast-(TrLast-Tr)*dT/255;
		}
		if(Tr<=0)
			return;
		DWORD D=(Tr<<24)+0xFFFFFF;
		Diffuse=ModDWORD(Diffuse,D);
	}
	if(NZElm>=MAXZELM)return;
	int XL=(int(x)>>zoomsh)-(mapx<<(5-zoomsh));	
	int YL=(int(y)>>(1+zoomsh))-(mapy<<(4-zoomsh))+DYZBuf;
	if(YL<0){
		YL=(YL+100000)%NYLines;
	}
	if(YL>=NYLines)YL=YL%NYLines;
	int zb=ZBFCounter[YL];
	if(zb>=ZBFNX)return;
	ZElement* ZEL=ZCashe+NZElm;
	ZEL->Targeted=false;
	ZEL->ShowAnimation=1;
	int zz=(YL<<ZSHF)+zb;
	ZBUF[zz]=NZElm;
	ZBUFX[zz]=XL;
	ZBFCounter[YL]++;
	NZElm++;

	ZEL->ANM=ANM;
	ZEL->Dir=Dir;
	ZEL->scale=Scale;
	ZEL->fiDir=fiDir;
	ZEL->fiOrt=fiOrt;
	ZEL->WorldPoint=0;
	ZEL->x=x;
	ZEL->y=y;
	ZEL->z=Mode3D?z:0;
	ZEL->Diffuse=Diffuse;
	ZEL->OB=OB;
	ZEL->IsDrawn=0;
	ZEL->Frame=Frame;	
	ZEL->Handle=Handle;
}
void AddAnmTarget(Vector3D Dest){
	if(NZElm>0&&NZElm<MAXZELM){
		ZElement* ZEL=ZCashe+NZElm-1;
		ZEL->Targeted=true;
		ZEL->Target=Dest;
	}
}
void AddHiPoint(short x,short y,OneObject* OB,word FileID,word SpriteID,int Param1,int Param2){
	if(NHIITEMS>=1024||NZElm>MAXZELM)return;
	//assert(SpriteID!=0xFFFF);
	ZElement* ZEL=ZCashe+NZElm;
	ZEL->ShowAnimation=0;
	ZEL->WorldPoint=0;
	ZEL->x=x;
	ZEL->y=y;
	ZEL->FileID=FileID;
	ZEL->FileIDEx=0xFFFF;
	ZEL->SpriteID=SpriteID;
	ZEL->Param1=Param1;
	ZEL->Param2=Param2;
	ZEL->IsDrawn=false;	
	ZEL->Nat=CurDrawNation;
	ZEL->Diffuse=CurZDiffuse;

	HILAYER[NHIITEMS]=NZElm;
	NHIITEMS++;
	NZElm++;
};
void AddSuperLoPoint(short x,short y,OneObject* OB,word FileID,word SpriteID,int Param1,int Param2){
	if(NLOITEMS>=4096||NZElm>MAXZELM)return;
	//assert(SpriteID!=0xFFFF);
	ZElement* ZEL=ZCashe+NZElm;
	ZEL->ShowAnimation=0;
	ZEL->WorldPoint=0;
	ZEL->x=x;
	ZEL->y=y;
	ZEL->FileID=FileID;
	ZEL->FileIDEx=0xFFFF;
	ZEL->SpriteID=SpriteID;
	ZEL->Param1=Param1;
	ZEL->Param2=Param2;
	ZEL->OB=OB;
	ZEL->Nat=CurDrawNation;
	ZEL->IsDrawn=true;	
	ZEL->Diffuse=CurZDiffuse;

	LOLAYER[NLOITEMS]=NZElm;
	NLOITEMS++;
	NZElm++;
};
void AddLoPoint(short x,short y,OneObject* OB,word FileID,word SpriteID,int Param1,int Param2){
	if(NLOITEMS>=4096)return;
	//assert(SpriteID!=0xFFFF);
	ZElement* ZEL=ZCashe+NZElm;
	ZEL->ShowAnimation=0;
	ZEL->WorldPoint=0;
	ZEL->x=x;
	ZEL->y=y;
	ZEL->FileID=FileID;
	ZEL->FileIDEx=0xFFFF;
	ZEL->SpriteID=SpriteID;
	ZEL->Param1=Param1;
	ZEL->Param2=Param2;
	ZEL->OB=OB;
	ZEL->Nat=CurDrawNation;
	ZEL->IsDrawn=false;	
	ZEL->Diffuse=CurZDiffuse;

	LOLAYER[NLOITEMS]=NZElm;
	NLOITEMS++;
	NZElm++;
};
int ZPointBias=0;
void SetZBias(int b){
	ZPointBias=b;
};


void AddWorldPoint( int xc, int yc, int zc,
                    int dx, int dy,
                    OneObject* OB,
                    word FileID, word SpriteID,
                    DWORD Diffuse,
                    bool AllowRotation,
                    float ScaleX, float ScaleY, float RotateAngle )
{
	if (NZElm>=MAXZELM) return;
	int XL=(xc>>zoomsh)-(mapx<<(5-zoomsh));	
    int YL=(yc>>(1+zoomsh))-(mapy<<(4-zoomsh))+DYZBuf;

	if(YL<0)YL=0;
	if(YL>=NYLines)YL=NYLines-1;
	int zb=ZBFCounter[YL];
	if(zb>=ZBFNX)return;
	ZElement* ZEL=ZCashe+NZElm;
	ZEL->ShowAnimation=0;
	int zz=(YL<<ZSHF)+zb;
	ZBUF[zz]=NZElm;
	ZBUFX[zz]=XL;
	ZBFCounter[YL]++;
	NZElm++;

    ZEL->WorldPoint=1;
	ZEL->pivotX=dx;
	ZEL->pivotY=dy;
	ZEL->x=xc;
	ZEL->y=yc;
	ZEL->z=Mode3D?zc:0;
	ZEL->Rotation=RotateAngle;
	ZEL->scale=1.0f/float(1<<(5-Shifter));
	ZEL->OB=OB;
	ZEL->Diffuse=Diffuse;	
	ZEL->YL=YL;
	ZEL->XL=XL;
	ZEL->SpriteID=SpriteID;
	ZEL->FileID=FileID;
	ZEL->AllowRotation=AllowRotation;
	ZEL->ScaleX=ScaleX;
	ZEL->ScaleY=ScaleY;
}
void AddPoint(short XL,short YL,short x,short y,OneObject* OB,word FileID,word SpriteID,int Param1,int Param2,int Rotation){
	if(NZElm>=MAXZELM)return;
	//assert(FileID<1024);
	//assert(SpriteID!=0xFFFF);
	YL+=DYZBuf;
	if(YL<0)YL=0;
	if(YL>=NYLines)YL=NYLines-1;	
	int zb=ZBFCounter[YL];
	if(zb>=ZBFNX)return;
	ZElement* ZEL=ZCashe+NZElm;
	ZEL->ShowAnimation=0;
	ZEL->WorldPoint=0;
	ZEL->Rotation=Rotation;
	if(Rotation){
		ZEL->pivotX=Param1;
		ZEL->pivotY=Param2;
		Param1=0;
		Param2=0;
	}
	ZEL->x=x;
	ZEL->y=y;
	ZEL->XL=XL;
	ZEL->YL=YL;
	ZEL->FileID=FileID;
	ZEL->FileIDEx=0xFFFF;
	ZEL->SpriteID=SpriteID;
	ZEL->Param1=Param1;
	ZEL->Param2=Param2;
	ZEL->IsDrawn=false;
	ZEL->OB=OB;
	ZEL->Nat=CurDrawNation;
	ZEL->Diffuse=CurZDiffuse;
	
	float rx, ry, rz;
	static const float c_Cos30 = cosf( c_PI / 6.0f );
	
	int sh = 5 - Shifter;
	float zoomScale = 1.0f;
	while (sh > 0) 
	{
		zoomScale /= 2.0f;
		sh >>= 1;
	}

	if (OB) 
	{
		rx = ((float)OB->RealX) / 16.0f;
		ry = ((float)OB->RealY) / 16.0f;
		rz = ((float)OB->RZ);
	}
	else 
	{	
		ZEL->z=0.0;
		int zz=(YL<<ZSHF)+zb;
		ZBUF[zz]=NZElm;
		ZBUFX[zz]=XL;
		ZBFCounter[YL]++;
		NZElm++;
		ZEL->scale=zoomScale;
		return;
		rx = 0.0f;
		ry = 0.0f;		
	}

	
	Vector4D pos( rx, ry - rz*0.5f, rz*c_Cos30, 1.0f );
	WorldToScreenSpace( pos );

	float pivotX = float( XL ) - float( x );
	float pivotY = (float( YL ) - float( DYZBuf ) - rz*zoomScale) - float( y );

	ZEL->x = pos.x - pivotX;
	ZEL->y = pos.y - pivotY;
	ZEL->z = pos.z - (ZPointBias)/GetZRange();
	ZEL->scale = zoomScale;

	
	int zz=(YL<<ZSHF)+zb;
	ZBUF[zz]=NZElm;
	ZBUFX[zz]=XL;
	ZBFCounter[YL]++;
	NZElm++;
	
};
void AddPointEx(short XL,short YL,short x,short y,OneObject* OB,word FileID,word SpriteID,word FileIDex,word SpriteIDex,int Param1,int Param2){
	if(NZElm>=MAXZELM)return;
	//assert(FileID<1024);
	//assert(SpriteID!=0xFFFF);
	YL+=DYZBuf;
	if(YL<0)YL=0;
	if(YL>=NYLines)YL=NYLines-1;
	int zb=ZBFCounter[YL];
	if(zb>=ZBFNX)return;
	ZElement* ZEL=ZCashe+NZElm;
	ZEL->ShowAnimation=0;
	ZEL->WorldPoint=0;
	ZEL->x=x;
	ZEL->y=y;
	ZEL->XL=XL;
	ZEL->YL=YL;

	ZEL->FileID=FileID;
	ZEL->SpriteID=SpriteID;
	ZEL->FileIDEx=FileIDex;
	ZEL->SpriteIDEx=SpriteIDex;

	ZEL->Param1=Param1;
	ZEL->Param2=Param2;
	ZEL->IsDrawn=false;
	ZEL->OB=OB;
	ZEL->Nat=CurDrawNation;
	ZEL->Diffuse=CurZDiffuse;

	//adding to Z-Buffer
	int zz=(YL<<ZSHF)+zb;
	ZBUF[zz]=NZElm;
	ZBUFX[zz]=XL;
	ZBFCounter[YL]++;
	NZElm++;
};
//Param1 values:
//bits 0..2-visibility options
//0-simple
//1-pulsing(Param2-pointer to pulsing data)
//2-transparent(Param2-pointer to transparency data)
//3-encoded with palette(Param2-pointer to palette)
//4-shading with mask(Param2-pointer to gradient data)
void AddOptPoint(byte Method,short XL,short YL,short x,short y,OneObject* OB,word FileID,word Sprite,int Options){

	AddPoint(XL,YL,x,y,OB,FileID,Sprite,0,0,0);
	return;
};
void AddLine(short X1,short Y1,short X2,short Y2,short x,short y,OneObject* OB,word FileID,word SpriteID,int Param1,int Param2);
void AddOptLine(short X1,short Y1,short X2,short Y2,short x,short y,OneObject* OB,word FileID,word SpriteID,int Options){	
};
void AddLine(short X1,short Y1,short X2,short Y2,short x,short y,OneObject* OB,word FileID,word SpriteID,int Param1,int Param2){	
};
void SortWords(int NWords,word* Data,short* Factor){	
};
void SortZBuffer(){
	int pos=0;
	short TEMPLINE[64];
	int tofs=int(TEMPLINE);
	for(int i=0;i<NYLines;i++){
		int N=ZBFCounter[i];
		if(N>1){
			word* ZPOS=ZBUF+pos;
			SortWords(N,ZBUF+pos,ZBUFX+pos);
		};
		pos+=ZBFNX;
	};
};
extern bool TransMode;
void DrawMarker(OneObject* OB);
void DrawImMarker(OneObject* OB);
void PtLine(int x,int y,int x1,int y1,byte c){
	int DD=int(sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1)));
	if(!DD)return;
	int NP=(DD>>5)+1;
	for(int i=0;i<=NP;i++){
		GPS.DrawFillRect(x+div((x1-x)*i,NP).quot-1,y+div((y1-y)*i,NP).quot-1,3,3,0xFFFF00FF);
	};
};
void DrawColorMarker(OneObject* OB,byte cl);
void DrawBorder(Brigade* BR);
void DrawHealth(OneObject* OB);
extern bool HealthMode;
void DrawGrassNearUnit(int x,int y);
void RegisterVisibleGP(OneObject* OB,int gpID,int sprID,int x,int y, float scale);


DWORD ModDWORD2X(DWORD C1,DWORD C2){
	DWORD R=0;
	for(int i=0;i<4;i++){
		int CH1=C1&255;
		int CH2=C2&255;
		C1>>=8;
		C2>>=8;
		int CC=(CH1*CH2*2)>>8;
		if(CC>255)CC=255;
		R|=(CC<<(i<<3));
	}
	return R;
}
DWORD ModDWORD2XRGB(DWORD C1,DWORD C2){
	DWORD R=0;
	for(int i=0;i<4;i++){
		int CH1=C1&255;
		int CH2=C2&255;
		C1>>=8;
		C2>>=8;
		int CC = i==3 ? CH1 : ((CH1*CH2*2)>>8);
		if(CC>255)CC=255;
		R|=(CC<<(i<<3));
	}
	return R;
}
extern bool IsEffectTargeted;
extern Vector3D GlobalEffectTarget;
extern bool CYLINDER_test_mode=false;
void ShowZElement(ZElement* ZEL)
{
	PROF;
	GPS.SetCurrentDiffuse(ZEL->Diffuse);
	if(ZEL->ShowAnimation){
		DWORD GetTexDiffuse(int x,int y);
		DWORD C=ZEL->Diffuse;//0xFF808080;
		if(ZEL->ANM->Enabled){
			PROF;
			PushEntityContext( ZEL->Handle );
			if(IsEffectTargeted=ZEL->Targeted){
                GlobalEffectTarget=ZEL->Target;
			}
			ZEL->ANM->DrawAt(ZEL->Frame,ZEL->OB?ZEL->OB->NNUM:0,ZEL->x,ZEL->y,ZEL->z,ZEL->Dir,ZEL->scale,C,ZEL->fiDir,ZEL->fiOrt,ZEL->OB);
			PopEntityContext();
			ENDPROF;
		}
		ENDPROF;
		OneObject* OB=ZEL->OB;
		if(OB){
			if(CYLINDER_test_mode){
				int H,R,H0;
				OB->GetCylinder(R,H,H0);
				DrawCylinder(Capsule(Vector3D(OB->RealX/16,OB->RealY/16,H0),Vector3D(0,0,H),R),0x20FF0000,0xFFFFFFFF,true);
			}
			if(CINFMOD){				
				void DrawDebugBuildingInfo( const OneObject* OB );
				DrawDebugBuildingInfo( OB );
				GPS.FlushBatches();
			}
		}
        return;
	}

	NEXTPROF;
	
	if(ZEL->WorldPoint){
		DWORD GetTexDiffuse(int x,int y);
		DWORD Diff=0xFF808080;
		GPS.SetCurrentDiffuse(ModDWORD2X(Diff,ZEL->Diffuse));		
		GPS.SetScale( ZEL->scale );
		if(ZEL->AllowRotation)
		{
			Vector3D pivot( ZEL->pivotX, ZEL->pivotY, 0.0f );
			Matrix4D tm = GetRolledBillboardTransform( pivot, 0.0f );				
			tm.translate( SkewPt( ZEL->x, ZEL->y, ZEL->z ) );
			float d=ZEL->Rotation;
			tm.e10+=d;
			tm.e30-=tm.e10*ZEL->pivotY;
			GPS.DrawWSprite( ZEL->FileID, ZEL->SpriteID, tm, 0 );
		}
		else
		{
			if (IsPerspCameraMode())
			{
				Vector3D pivot( ZEL->pivotX, ZEL->pivotY, 0.0f );
				Matrix4D tm = GetRolledBillboardTransform( pivot, 0.0f );
				tm.translate( SkewPt( ZEL->x, ZEL->y, ZEL->z ) );
				GPS.DrawWSprite( ZEL->FileID, ZEL->SpriteID, tm, 0 );
			}
			else
			{
				int fdx = ZEL->SpriteID >= 4096 ? -ZEL->pivotX : ZEL->pivotX;

				Vector4D wPos( ZEL->x, ZEL->y, ZEL->z, 1.0f );
				WorldToScreenSpace( wPos );
				extern float GlobalScale;
				GPS.ShowGP( wPos.x - fdx*GlobalScale, wPos.y - ZEL->pivotY*GlobalScale, wPos.z, 
							ZEL->FileID, ZEL->SpriteID, ZEL->OB ? ZEL->OB->NNUM : 0 );
			}
		}
	}

	ENDPROF;	

	GPS.SetCurrentDiffuse(0xFFFFFFFF);
}; // ShowZElement

int mul3(int);
extern int RealLx;
extern int RealLy;
void PROSESS_WAVES();
void DRAW_WAVES();
#define ZMASK 8191
typedef DynArray<ZElement*> ZArray;
ZArray ZLIST[8];
int SelColor=0;
void ArrangeZBuffer(){
	static int T0=GetTickCount();
	SelColor=int(200.0+50.0*sin(float(GetTickCount()-T0)/120));
	for(int q=0;q<8;q++)ZLIST[q].NValues=0;
	PROF;
	for (int i = 0; i < NYLines; i++)
	{	
		PROF;
		int NZ=ZBFCounter[i];
		word* ZW=ZBUF+i*ZBFNX;
		for (int j = 0; j < NZ; j++)
		{
			WORD zelIdx = *ZW;
			ZW++;
			ZElement* ZEL = &ZCashe[zelIdx];
			if(ZEL->FileID<0xFFFF){				
				if(ZEL->ShowAnimation){
					if(ZEL->ANM->AnimationType&&!(ZEL->OB&&ZEL->OB->NewBuilding)){						
						ZLIST[5].Add(ZEL);						
					}else{
						if(ZEL->OB&&ZEL->OB->NewBuilding){
							if((ZEL->Diffuse>>24)>200){
								ZLIST[3].Add(ZEL);
							}else{
								ZLIST[6].Add(ZEL);
							}
						}else ZLIST[1].Add(ZEL);
					}
				}else{
					//if(ZEL->AllowRotation&&ZEL->WorldPoint)ZLIST[4].Add(ZEL);					
					if(ZEL->WorldPoint)ZLIST[4].Add(ZEL);					
				}
			}
		}
		ENDPROF;
	}
	ENDPROF;
}
void SetCollectMode(int mode);
void ClearCollection();
void ShowCollection();
extern float GlobalScale;

void FlushSprites( byte subsetIdx = 0 )
{	
	PROF;
	GlobalScale=1.0f/float(1<<(5-Shifter));
	if(subsetIdx==1)
	{
		SetCollectMode(1);		
	}else SetCollectMode(0);
	
	if(subsetIdx==2)
	{
		subsetIdx=1;
		ShowCollection();
		ClearCollection();
		GPS.FlushBatches();
		return;
	}
	NEXTPROF;
	ZArray& Z=ZLIST[subsetIdx];
	int N=Z.GetAmount();
	
	if(subsetIdx==4||subsetIdx==1)
	{
		for(int i=N-1;i>=0;i--){
			ShowZElement(Z[i]);
		}
	}
	else
	{
		for(int i=0;i<N;i++)
		{
			ShowZElement(Z[i]);
		}
	}
	NEXTPROF;
	if(N)GPS.FlushBatches();
	ENDPROF;	

} // FlushSprites

void zb_memset();
void ShowZBuffer(){	
	//PROSESS_WAVES();
	if(CINFMOD)zb_memset();
	//DRAW_WAVES();	
	FlushSprites();
	if(CINFMOD){
		for(int i=DYZBuf;i<1600;i++){
			if(ZBFCounter[i]){
				int n=ZBFCounter[i];
				int pos=i<<ZSHF;
				for(int j=0;j<n;j++){
					word ZT=ZBUF_OLD[pos+j]&0x6000;
					int xx=ZBUFX_OLD[pos+j];
					int yy=i-DYZBuf;
					switch(ZT){
					case 0:
						Xbar(xx-1,yy-1,3,3,0xD0);
						break;
					case 0x2000:
						Xbar(xx-1,yy-1,3,3,0xD0);//clrBlue);
						break;
					case 0x4000:
						Xbar(xx-1,yy-1,3,3,0xD0);
						break;
					};
				};
			};
		};
	};
	void DrawAllGModels();
	DrawAllGModels();
	void TestGModels();
	TestGModels();
};
void zb_memset(){
	memcpy(ZBUF_OLD,ZBUF,sizeof(ZBUF));
	memcpy(ZBUFX_OLD,ZBUFX,sizeof(ZBUFX));
}

//!!