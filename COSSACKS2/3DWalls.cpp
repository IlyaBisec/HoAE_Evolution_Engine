#include "stdheader.h"
#include "TreesScope.h"
#include "AI_Scripts.h"
#include "HashTop.h"
#include "Save_XML_ToMap.h"
#include "TestEditor.h"
#include "IShadowManager.h"
#include "kContext.h"
#define SnapToEdgeDist 180
DLLEXPORT int GetMyNation();
void UnPress();
class OneEdge:public BaseClass{
public:
	float dx;
	float dy;
	float dz;
	float Fi;
	float DFi;
	SAVE(OneEdge);
		REG_MEMBER(_float,dx);
		REG_MEMBER(_float,dy);
		REG_MEMBER(_float,dz);
		REG_MEMBER(_float,Fi);
		REG_MEMBER(_float,DFi);
	ENDSAVE;
};
class OneWallDestructionPart:public BaseClass{
public:
	_str  NodeName;
	float FallDeep;
	float FallStartTime;
	float RelaxTime;
	Vector3D Center;
	int  NodeID;
	int  AnimNodeID;
	byte Usage;	
	SAVE(OneWallDestructionPart);
		REG_AUTO(NodeName);
		REG_MEMBER(_float,FallDeep);
		REG_MEMBER(_float,FallStartTime);
		REG_MEMBER(_float,RelaxTime);		
		REG_MEMBER_EX(_float,Center.x,x);
		REG_MEMBER_EX(_float,Center.y,y);
		REG_MEMBER_EX(_float,Center.z,z);
	ENDSAVE;
	const char* GetThisElementView(const char* LocalName){
		if(NodeName.pchar())return NodeName.pchar();
		else return LocalName;
	}
};
class OneWallLes:public BaseClass{
public:
	OneWallLes(){
		AABB_scaleX=1.0f;
		AABB_scaleY=1.0f;
		AABB_scaleZ=1.0f;
		Scale=1.0f;
		AABB_Init=false;
	}
	int   Model;
	float AABB_scaleX;
	float AABB_scaleY;
	float AABB_scaleZ;
	float Scale;
	bool AABB_Init;
	AABoundBox AABB;
	SAVE(OneWallLes);
		REG_MEMBER(_ModelID,Model);
		REG_MEMBER(_float01,AABB_scaleX);
		REG_MEMBER(_float01,AABB_scaleY);
		REG_MEMBER(_float01,AABB_scaleZ);		
		REG_MEMBER(_float,Scale);
	ENDSAVE;    
};
class OneWallElement:public BaseClass{
public:
	class AutoCreateDestruct:public BaseFunction{
	public:
        void EvaluateFunction(){
			OneWallElement* WE=get_parent<OneWallElement>();
			if(WE){
				WE->DestructionParts.Clear();
                int N=IMM->GetNumSubNodes(WE->DestructedModelID);
				
                //  Silver, 7.09.2005:
                //  I have commented following piece of code because of:
                //  1. IMediaManager model rendering interface has changed
                //  2. I am not much into what's going on here
                //  3. It seems destructable walls are not used in HOAE

                
				IMM->StartModel(WE->DestructedModelID);
                for(int i=0;i<N;i++){                    
					OneWallDestructionPart* DP=new OneWallDestructionPart;
					DP->NodeName=IMM->GetSubNodeName(WE->DestructedModelID,i);					

					int mnod=IMM->GetNodeID(WE->DestructedModelID,DP->NodeName.pchar());
					int anod=IMM->GetNodeID(WE->DestructedAnimationID,DP->NodeName.pchar());				

                    if(anod>0){						
						DP->NodeID = mnod;
						DP->AnimNodeID = anod;
						WE->DestructionParts.Add(DP);
					}else delete(DP);
				}                
			}
		}
		SAVE(AutoCreateDestruct);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	};
	class GetModelNodes:public BaseFunction{
	public:
		void EvaluateFunction(){
			OneWallElement* WE=get_parent<OneWallElement>();
			if(WE){
				WE->DestructionParts.Clear();
				int N=IMM->GetNumSubNodes(WE->DestructedModelID);
				for(int i=0;i<N;i++){                    
					OneWallDestructionPart* DP=new OneWallDestructionPart;
					DP->NodeName=IMM->GetSubNodeName(WE->DestructedModelID,i);					
					WE->DestructionParts.Add(DP);					
				}
			}
		}
		SAVE(GetModelNodes);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	};
	int ModelID;
	int DestructedModelID;
	int DestructedAnimationID;
	int OpeningAnimationID;
	int OpenCenterDistance;
    int StandAnimation;
    int StandAnimationTime;
	float TimeScale;	
	ClonesArray<OneWallDestructionPart> DestructionParts;
	AutoCreateDestruct AutoCreateDestructionParts;
	GetModelNodes AutoGetModelNodes;	
	float Scale;
	int Rotation;
	int dz;
	int Usage;
	int AssociateWithUnit;

	ClonesArray<OneEdge> LeftEdges;
	ClonesArray<OneEdge> RightEdges;
    SAVE(OneWallElement);
	REG_CLASS(OneWallDestructionPart);
		REG_MEMBER(_ModelID,ModelID);
        REG_MEMBER(_ModelID,StandAnimation);
        REG_MEMBER(_int,StandAnimationTime);
		REG_MEMBER(_ModelID,DestructedModelID);
		REG_MEMBER(_ModelID,DestructedAnimationID);
		REG_MEMBER(_ModelID,OpeningAnimationID);        
		REG_MEMBER(_int,OpenCenterDistance);
		REG_MEMBER(_float,TimeScale);
		REG_AUTO(DestructionParts);
		REG_AUTO(AutoCreateDestructionParts);
		//REG_AUTO(AutoGetModelNodes);
		REG_MEMBER(_float,Scale);		
		REG_ENUM(_index,Usage,WallBlockUsage);
		REG_MEMBER(_int,Rotation);
		REG_MEMBER(_int,dz);
		REG_MEMBER(_UnitType,AssociateWithUnit);
		REG_AUTO(LeftEdges);
		REG_AUTO(RightEdges);
	ENDSAVE;
	OneWallElement(){
		Scale=1.0f;
		Rotation=0;
		dz=0;
		TimeScale=1.0f;
        StandAnimation=-1;
        StandAnimationTime=0;
	}
	virtual const char* GetThisElementView(const char* LocalName){
        static Enumerator* E=ENUM.Get("WallBlockUsage");
		return E->GetStr(Usage);
	}
};
class WallTreesParam:public BaseClass{
public:
	int   TreesGroup;
	float Density;
	float DensityVariation;
	bool  StandInFrontOfWall;
	bool  StandInBackOfWall;
	int   MinDistFromWall;
	int   MaxDistFromWall;
	int   MinDistFromTower;
	int   MaxDistFromTower;
	SAVE(WallTreesParam);
		REG_ENUM(_index,TreesGroup,TREESGROUPS);
		REG_MEMBER(_float01,Density);
		REG_MEMBER(_float01,DensityVariation);
		REG_MEMBER(_bool,StandInFrontOfWall);
		REG_MEMBER(_bool,StandInBackOfWall);
		REG_MEMBER(_int,MinDistFromWall);
		REG_MEMBER(_int,MaxDistFromWall);
		REG_MEMBER(_int,MinDistFromTower);
		REG_MEMBER(_int,MaxDistFromTower);
	ENDSAVE;
	WallTreesParam(){
		TreesGroup=0;
		Density=1.0f;
		DensityVariation=0.5f;
		StandInFrontOfWall=true;
		StandInBackOfWall=false;
		MinDistFromWall=80;
		MaxDistFromWall=100;
		MinDistFromTower=110;
		MaxDistFromTower=140;
	}
};
class WallDitchParam:public BaseClass{
public:
	bool InFront;
	bool InBack;
	int  DistanceFromWall;
	int  DistanceFromTower;
	int  InWidth;
	int  OutWidth;
	int  Depth;
	float DepthVariation;	
	int  Texture0;
	int  Facture0;
	float FactureMaxWeight0;
	int  Texture1;
	int  Facture1;
	float FactureMaxWeight1;
	float TexturedWidthScale;
	float TextureBlending;
	SAVE(WallDitchParam);
		REG_MEMBER(_bool,InFront);
		REG_MEMBER(_bool,InBack);
		REG_MEMBER(_int,DistanceFromWall);
		REG_MEMBER(_int,DistanceFromTower);
		REG_MEMBER(_int,InWidth);
		REG_MEMBER(_int,OutWidth);
		REG_MEMBER(_int,Depth);
		REG_MEMBER(_float01,DepthVariation);
		REG_MEMBER(_gtexture,Texture0);
		REG_MEMBER(_gfacture,Facture0);
		REG_MEMBER(_float01,FactureMaxWeight0);
		REG_MEMBER(_gtexture,Texture1);
		REG_MEMBER(_gfacture,Facture1);
		REG_MEMBER(_float01,FactureMaxWeight1);
		REG_MEMBER(_float01,TexturedWidthScale);
		REG_MEMBER(_float01,TextureBlending);

	ENDSAVE;
	WallDitchParam(){
		InFront=true;
		InBack=false;
		DistanceFromWall=100;
		DistanceFromTower=130;
		InWidth=40;
		OutWidth=80;
		Depth=60;
		DepthVariation=0.5f;
		TexturedWidthScale=0.5;
		TextureBlending=0.8;
	}

};
class WallGroundTexturingParam:public BaseClass{
public:
	int TerrainType;
	int MinRadius;
	int MaxRadius;
	int TowerMinRadius;
	int TowerMaxRadius;
	int VariationDistance;
	int Texture0;
	int Facture0;
	float FactrureMaxWeight0;
	float FactrureMinWeight0;
	int Texture1;
	int Facture1;
	float FactrureMaxWeight1;
	float FactrureMinWeight1;
	ClonesArray<WallTreesParam> SettingObjects;
	ClonesArray<WallDitchParam> Ditch;
	void Texturize(int x0,int y0,int x1,int y1);
	SAVE(WallGroundTexturingParam);
		REG_MEMBER(_int,TerrainType);
		REG_MEMBER(_int,MinRadius);
		REG_MEMBER(_int,MaxRadius);
		REG_MEMBER(_int,TowerMinRadius);
		REG_MEMBER(_int,TowerMaxRadius);
		REG_MEMBER(_int,VariationDistance);
		REG_MEMBER(_gtexture,Texture0);
		REG_MEMBER(_gfacture,Facture0);
		REG_MEMBER(_float0_10,FactrureMaxWeight0);		
		REG_MEMBER(_gtexture,Texture1);
		REG_MEMBER(_gfacture,Facture1);
		REG_MEMBER(_float0_10,FactrureMaxWeight1);		
		REG_AUTO(SettingObjects);
		REG_AUTO(Ditch);
	ENDSAVE;
	WallGroundTexturingParam(){
		TerrainType=0;
		MinRadius=50;
		MaxRadius=130;
		TowerMinRadius=70;
		TowerMaxRadius=160;
		VariationDistance=400;
		Texture0=14;
		Facture0=7;
		FactrureMaxWeight0=1.0f;
		Texture1=16;
		Facture1=8;
		FactrureMaxWeight1=1.0f;
	}
};
class WallTypeDescription:public BaseClass{
public:
	_str Name;
    float GlobalScale;
	ClonesArray<OneWallElement>            Elements;
	ClonesArray<OneWallLes>                Lesa;
	ClonesArray<WallGroundTexturingParam>  Texturing;	
	int MinWallLength;
	int MaxWallLength;
	int MinWallHeight;
	bool DontUseBastion;
	bool UseLikeSingleElements;
	bool AutoRotateFirstElement;
	SAVE(WallTypeDescription);
		REG_AUTO(Name);
		REG_MEMBER(_float,GlobalScale);
		REG_AUTO(Elements);
		REG_AUTO(Lesa);
		REG_AUTO(Texturing);
		REG_MEMBER(_int,MinWallLength);
		REG_MEMBER(_int,MaxWallLength);
		REG_MEMBER(_int,MinWallHeight);
		REG_MEMBER(_bool,DontUseBastion);
		REG_MEMBER(_bool,UseLikeSingleElements);
		REG_MEMBER(_bool,AutoRotateFirstElement);
	ENDSAVE;
	WallTypeDescription(){
		MinWallLength=350;
		MaxWallLength=500;
		MinWallHeight=0;
		DontUseBastion=0;
		UseLikeSingleElements=0;
		AutoRotateFirstElement=0;
	}
};
class WallsList:public BaseClass{
public:
	ClonesArray<WallTypeDescription> Walls;
	SAVE(WallsList);
		REG_AUTO(Walls);
	ENDSAVE;
};
#define MaxParts 64
class OneWallSection;
class OneWallPoint:public BaseClass{
public:
	OneWallElement* Type;
	Matrix4D M4;
	float x_in,y_in;
	float x_out,y_out;

	int ElmIndex;
	int DestructStage;
	int ConstructStage;
	int x,y,z;
	byte  Angle;
	float ScaleP;//parallel
	float ScaleO;//ortogonal
	float ScaleZ;//Z
	float Skos;
	bool Visible;
	DWORD DestructionMask;
	int  CrashTime[MaxParts];
	bool CrashStopped[MaxParts];
	void Draw(float anmstage,OneObject* OB,WallTypeDescription* WT,OneWallSection* OWS);
	SAVE(OneWallPoint);		
        REG_MEMBER(_int,ElmIndex);
		REG_MEMBER(_int,DestructStage);
		REG_MEMBER(_int,ConstructStage);
		REG_MEMBER(_int,x);
		REG_MEMBER(_int,y);
		REG_MEMBER(_int,z);
		REG_MEMBER(_float,ScaleP);
		REG_MEMBER(_float,ScaleO);
		REG_MEMBER(_float,ScaleZ);
		REG_MEMBER(_float,Skos);
		REG_MEMBER(_DWORD,Skos);
		REG_MEMBER(_bool,Visible);
	ENDSAVE;
	OneWallPoint(){
		DestructionMask=0xFFFFFFFF;//rand();		
		for(int i=0;i<MaxParts;i++){
			CrashTime[i]=0;
			CrashStopped[i]=0;
		}
		Visible=1;
	}
};
class OneWallEdge;
class OneWallsSystem;
class OneWallSection:public BaseClass{
public:
	int OwnerObjectID;
	int OwnerObjectSN;
	int OwnerObjectX;
	int OwnerObjectY;
	int LastBuildTime;
	OneObject* GetObj(){
		if(OwnerObjectID!=0xFFFF){
			OneObject* OB=Group[OwnerObjectID];
			if(OB&&OB->Serial==OwnerObjectSN){
				return OB;
			}
		}
		return NULL;
	}
	int SpecifiedObjectType;
	int WallType;
	bool TowerType;//0-tower,1-bastion
	bool Dead;
	bool ReCreateHeight;
	int OwnerNI;
	OneWallsSystem* Owner;
	ClonesArray<OneWallPoint> Points;
	virtual void SetColor();
	virtual void CreateLocking(){};
	OneWallSection(){
		OwnerObjectID=0xFFFF;
		OwnerObjectSN=0xFFFF;
		OwnerObjectX=0xFFFF;
		OwnerObjectY=0xFFFF;
		SpecifiedObjectType=0xFFFF;
		Owner=NULL;
		WallType=0;
		OwnerNI=0;
		TowerType=0;
		ReCreateHeight=0;
		Dead=false;
		LastBuildTime=0;
	}
	SAVE(OneWallSection);
		REG_MEMBER(_int,OwnerObjectID);
		REG_MEMBER(_int,OwnerObjectSN);
		REG_MEMBER(_int,OwnerObjectX);
		REG_MEMBER(_int,OwnerObjectY);
		REG_MEMBER(_int,SpecifiedObjectType);
		REG_MEMBER(_int,WallType);
		REG_MEMBER(_bool,TowerType);
		REG_MEMBER(_int,OwnerNI);
		REG_MEMBER(_bool,Dead);
		REG_AUTO(Points);
	ENDSAVE;
};
class OneWallLine:public OneWallSection{
public:	
	int StartEdge;
	OneWallEdge* Start;
	int FinalEdge;
	OneWallEdge* Final;	
	int DestructStage;
	int ConstructStage;
	bool Textured;
	virtual void CreateLocking();
	SAVE(OneWallLine);		
		REG_PARENT(OneWallSection);
		REG_MEMBER(_int,StartEdge);
		REG_MEMBER(_int,FinalEdge);
		REG_MEMBER(_int,DestructStage);
		REG_MEMBER(_int,ConstructStage);
		REG_MEMBER(_bool,Textured);		
	ENDSAVE;	
	OneWallLine(){
		Start=NULL;
		Final=NULL;
		ConstructStage=100;
		DestructStage=100;
		Textured=0;
	}
};
class OneWallEdge:public OneWallSection{
public:
	int EdgeID;
	//int WallType;
	int x,y,z;
	bool NeedToOpen;
	int AnimationStage;
	OneWallLine* In;
	OneWallLine* Out;
	virtual void SetColor();
	OneWallEdge(){
		NeedToOpen=0;
		AnimationStage=0;
	}
	//virtual void CreateLocking();
	SAVE(OneWallEdge);
		REG_PARENT(OneWallSection);
		REG_MEMBER(_int,EdgeID);
		REG_MEMBER(_int,x);
		REG_MEMBER(_int,y);
		REG_MEMBER(_int,z);
	ENDSAVE;
};
class OneWallsSystem:public BaseClass{
public:
	ClonesArray<OneWallEdge> Edges;
	ClonesArray<OneWallLine> Lines;
	byte TestMode;
	void ReCreate();	
	//fills all params of this class using partial:
	//x,y,z,DestructStage,ConstructStage of Edges 
	//and DestructStage, ConstructStage of Lines.
	void Draw();	
	void AddEdge(int x,int y,int ID,int Type,int NI,bool Bastion);
	void AddLine(int ID1,int ID2,int Type,int NI);
	OneWallEdge* GetNearEdge(int x,int y,int R);
	void FillTempFields(){
		for(int i=0;i<Edges.GetAmount();i++){
			Edges[i]->Owner=this;
		}
		for(int i=0;i<Lines.GetAmount();i++){
			Lines[i]->Owner=this;
			Lines[i]->Start=NULL;
			Lines[i]->Final=NULL;
			if(!Lines[i]->Dead){
				for(int j=0;j<Edges.GetAmount();j++){
					if(Lines[i]->FinalEdge==Edges[j]->EdgeID&&(!Edges[j]->Dead)){
						Lines[i]->Final=Edges[j];
						Edges[j]->In=Lines[i];
					}
					if((!Edges[j]->Dead)&&Lines[i]->StartEdge==Edges[j]->EdgeID){
						Lines[i]->Start=Edges[j];
						Edges[j]->Out=Lines[i];
					}
				}
			}
		}
	}
	SAVE(OneWallsSystem);
		REG_AUTO(Edges);
		REG_AUTO(Lines);
	ENDSAVE;
	OneWallsSystem(){
		TestMode=0;
	}
};
WallsList WList;
BaseClass* GetWallsClass(){
	return &WList;
}
//CEXPORT int ItemChoose;
bool MMItemChoose(SimpleDialog* SD);
void EditClass(BaseClass* BC,char* xmlName){
	BC->reset_class(BC);
	BC->SafeReadFromFile(xmlName);
	xmlQuote xml;
	ItemChoose=-1;		
	if(xml.ReadFromFile("Dialogs\\MapOptionsDialog.DialogsSystem.xml")){
		DialogsSystem DSS;
		ErrorPager EP;
		DSS.Load(xml,&DSS,&EP);
		SimpleDialog* Desk=DSS.Find("OptionsDesk");
		SimpleDialog* OK=DSS.Find("OK");
		SimpleDialog* CANCEL=DSS.Find("CANCEL");
		if(Desk&&OK&&CANCEL){
			int x0,y0,x1,y1;
			DSS.GetDialogsFrame(x0,y0,x1,y1);
			if(x1>x0){
				DSS.x=(RealLx-x1+x0)/2;
				DSS.y=(RealLy-y1+y0)/2;
				OK->OnUserClick=&MMItemChoose;
				OK->UserParam=1;
				CANCEL->OnUserClick=&MMItemChoose;
				CANCEL->UserParam=1;
				ClassEditor CE;
				CE.CreateFromClass(Desk,0,0,Desk->x1-Desk->x,Desk->y1-Desk->y,&WList,3,"EmptyBorder");
				do{
                    ProcessMessages();					
					DSS.ProcessDialogs();
					CE.Process();
					DSS.RefreshView();
				}while(ItemChoose==-1);
			}
		}
	}
	BC->WriteToFile(xmlName);
}

int WallsEditMode=0;
static int PrevClickX=0;
static int PrevClickY=0;
void ClearWallsEditMode(){
	WallsEditMode=0;
	PrevClickX=0;
	PrevClickY=0;
}
void EditWalls(){	
	EditClass(&WList,"Dialogs\\Walls.WallsList.xml");    
	WallsEditMode=1;	
}
float sq_norma(float x,float y){
	return sqrt(x*x+y*y);
}
word GetDir(int,int);
int CreateOrFindBuilding(byte NI,int x,int y,int Type){
	for(int ix=-1;ix<=1;ix++){
		for(int iy=-1;iy<=1;iy++){
			int BCL=(x>>11)+ix+(((y>>11)+iy)<<VAL_SHFCX)+VAL_MAXCX+1;		
			if(BCL>=0&&BCL<VAL_MAXCIOFS){
				word GetOneBld(int cell,int pos=0);
				word MID;
				for(int pos=0;(MID=GetOneBld(BCL,pos))!=0xFFFF;pos++){			
					OneObject* OB=Group[MID];
					if(OB&&(!OB->Sdoxlo)&&Norma(OB->RealX-x,OB->RealY-y)<32*32){
						return OB->Index;
					}
				}
			}
		}
	}
	return NATIONS[NI].CreateNewMonsterAt(x,y,Type,1,0);
}
int FindBuildingForWall(byte NI,int x,int y){
	for(int ix=-1;ix<=1;ix++){
		for(int iy=-1;iy<=1;iy++){
			int BCL=(x>>11)+ix+(((y>>11)+iy)<<VAL_SHFCX)+VAL_MAXCX+1;		
			if(BCL>=0&&BCL<VAL_MAXCIOFS){
				word GetOneBld(int cell,int pos=0);
				word MID;
				for(int pos=0;(MID=GetOneBld(BCL,pos))!=0xFFFF;pos++){			
					OneObject* OB=Group[MID];
					if(OB&&(!OB->Sdoxlo)&&Norma(OB->RealX-x,OB->RealY-y)<32*32){
						return OB->Index;
					}
				}
			}
		}
	}
	return 0xFFFF;
}
void OneWallsSystem::ReCreate(){
	//filling fast-access fields
	FillTempFields();
	bool ReCreateTop=0;
	int tx0=10000;
	int ty0=10000;
	int tx1=0;
	int ty1=0;
	word ObjsToBuild[8];
	int NObjsToBuild=0;
	//setting towers&bastions on them places
	for(int i=0;i<Edges.GetAmount();i++){
        OneWallEdge* OWE=Edges[i];
		if(OWE&&OWE->ReCreateHeight){
			for(int q=0;q<OWE->Points.GetAmount();q++){
				OneWallPoint* OWP=OWE->Points[q];
				void EraseExtraHeightObject(int x,int y);
				EraseExtraHeightObject(OWP->x,OWP->y);                
			}
			OWE->ReCreateHeight=0;
		}
		bool ReCreatePoint=!OWE->Dead;//true;
		/*if(OWE->Points.GetAmount()){
			if(OWE->Points[0]->DestructionMask!=0xFFFFFFFF)ReCreatePoint=false;
		}*/
		if(ReCreatePoint){
			OWE->Points.reset_class(&OWE->Points);
			OneWallPoint* OWP=new OneWallPoint;			
			OWP->Type=NULL;
			WallTypeDescription* WT=WList.Walls[OWE->WallType];
			int id=FindBuildingForWall(OWE->OwnerNI,OWE->x<<4,OWE->y<<4);
			if(id!=0xFFFF){
                OneObject* OB=Group[id];
				OWE->SpecifiedObjectType=OB->NIndex;
			}
			for(int j=0;j<WT->Elements.GetAmount();j++){
				int Usage=WT->Elements[j]->Usage;
				if(OWE->SpecifiedObjectType!=0xFFFF){
					if(OWE->SpecifiedObjectType==WT->Elements[j]->AssociateWithUnit){
						OWP->Type=WT->Elements[j];
						break;
					}
				}else
				if((Usage==3&&!OWE->TowerType)||(Usage==4&&OWE->TowerType)){//tower||bastion
					OWP->Type=WT->Elements[j];
					break;
				}
			}						
			if(OWP->Type){
				OWP->x=OWE->x;
				OWP->y=OWE->y;
				OWP->z=OWE->z;
				OWP->ScaleO=1.0f;
				OWP->ScaleP=1.0f;
				OWP->ScaleZ=1.0f;
				int InAngle=-1;
				int OutAngle=-1;
				if(OWE->In&&OWE->In->Start){
					InAngle=GetDir(OWE->In->Start->x-OWE->x,OWE->In->Start->y-OWE->y);
				}
				if(OWE->Out&&OWE->Out->Final){
					OutAngle=GetDir(OWE->Out->Final->x-OWE->x,OWE->Out->Final->y-OWE->y);
				}
				OWP->Angle=0;
				if(InAngle>=0&&OutAngle>=0){
					if(OutAngle<InAngle)OWP->Angle=byte(((OutAngle+InAngle)/2)&255);
					else OWP->Angle=byte(((OutAngle+InAngle+256)/2)&255);				
				}else if(InAngle>=0){
					OWP->Angle=byte((InAngle+128+64)&255);
				}else if(OutAngle>=0){
					OWP->Angle=byte((OutAngle+64)&255);
				}
				if(i==0)OWP->Angle=0;
				float COS=float(TCos[OWP->Angle])/256.0f;
				float SIN=float(TSin[OWP->Angle])/256.0f;
				float x_in_local=0;
				float y_in_local=0;
				if(OWP->Type->LeftEdges.GetAmount()){
					x_in_local=OWP->Type->LeftEdges[0]->dx*OWP->Type->Scale*WT->GlobalScale;
					y_in_local=OWP->Type->LeftEdges[0]->dy*OWP->Type->Scale*WT->GlobalScale;
				}
				float x_out_local=0;
				float y_out_local=0;
				if(OWP->Type->RightEdges.GetAmount()){
					x_out_local=OWP->Type->RightEdges[0]->dx*OWP->Type->Scale*WT->GlobalScale;
					y_out_local=OWP->Type->RightEdges[0]->dy*OWP->Type->Scale*WT->GlobalScale;
				}
				OWP->x_in=OWP->x+COS*x_in_local-SIN*y_in_local;
				OWP->y_in=OWP->y+COS*y_in_local+SIN*x_in_local;
				OWP->x_out=OWP->x+COS*x_out_local-SIN*y_out_local;
				OWP->y_out=OWP->y+COS*y_out_local+SIN*x_out_local;
				OWP->Angle+=OWP->Type->Rotation;
				OWP->z+=OWP->Type->dz;
				if(OWE->In==NULL&&OWE->Out&&WT->AutoRotateFirstElement){
					OWP->Angle+=128;
				}
				OWP->M4.srt(OWP->Type->Scale*WT->GlobalScale,Vector3D::oZ,OWP->Angle*3.1415925f/128.0f,Vector3D(OWP->x,OWP->y,OWP->z));
				//Matrix4D M4=GetSkewTM();				
				//OWP->M4*=M4;
				void AddExtraHeightObject(int x,int y,int id,Matrix4D* m4);	
				if(!(TestMode||OWE->Dead)){
					AddExtraHeightObject(OWP->x,OWP->y,OWP->Type->ModelID,&OWP->M4);				
				}
				if(!(TestMode||OWE->Dead)){
					//if(OWE->OwnerObjectID==0xFFFF){
						bool noobj=OWE->GetObj()==NULL;
						int idx=CreateOrFindBuilding(OWE->OwnerNI,OWP->x<<4,OWP->y<<4,OWP->Type->AssociateWithUnit);
						if(idx!=-1){
							OneObject* OB=Group[idx];
							if(OB){
								//if(OB->Stage<5){
								//	for(int i=OB->Stage;i<OB->NStages;i++)OB->NextStage();
								//	OB->Ready=1;
								//}
								OWE->OwnerObjectID=idx;
								OWE->OwnerObjectSN=OB->Serial;
								OWE->OwnerObjectX=OB->RealX;
								OWE->OwnerObjectY=OB->RealY;
								OB->RealDir=OWP->Angle;	
								if(noobj&&NObjsToBuild<8){
									ObjsToBuild[NObjsToBuild]=idx;
									NObjsToBuild++;
								}
							}
						}
					//}
				}
				OWE->Points.Add(OWP);
			}
		}
	}
	//setting walls on them places
	for(int i=0;i<Lines.GetAmount();i++){
        OneWallLine* OWL=Lines[i];
		if(!TestMode){
			if(OWL&&OWL->ReCreateHeight){
				for(int q=0;q<OWL->Points.GetAmount();q++){
					OneWallPoint* OWP=OWL->Points[q];
					void EraseExtraHeightObject(int x,int y);
					EraseExtraHeightObject(OWP->x,OWP->y);                
				}
				OWL->ReCreateHeight=0;
			}
		}
		bool ReCreatePoint=true;
		if(OWL->Points.GetAmount()){
			for(int w=0;w<OWL->Points.GetAmount();w++)if(OWL->Points[w]->DestructionMask!=0xFFFFFFFF)ReCreatePoint=false;
		}
		if(ReCreatePoint){
			OWL->Points.reset_class(&OWL->Points);
			if(OWL->Start&&OWL->Final&&OWL->Start->Points.GetAmount()&&OWL->Final->Points.GetAmount()){
				float x0=OWL->Start->Points[0]->x_out;
				float y0=OWL->Start->Points[0]->y_out;
				float x1=OWL->Final->Points[0]->x_in;
				float y1=OWL->Final->Points[0]->y_in;
				float z0=OWL->Start->Points[0]->z;
				float z1=OWL->Final->Points[0]->z;
				float Tdist=sq_norma(x1-x0,y1-y0);
				//finding element			
				OneWallElement* eType=NULL;
				OneWallElement* LeType=NULL;
				OneWallElement* ReType=NULL;
				WallTypeDescription* WT=WList.Walls[OWL->WallType];
				for(int j=0;j<WT->Elements.GetAmount();j++){
					int Usage=WT->Elements[j]->Usage;
					if(Usage==1){//central block
						if(OWL->SpecifiedObjectType!=0xFFFF){
							if(OWL->SpecifiedObjectType==WT->Elements[j]->AssociateWithUnit){
								eType=WT->Elements[j];
							}
						}else eType=WT->Elements[j];					
					}
					if(Usage==0){//left block
						if(OWL->SpecifiedObjectType!=0xFFFF){
							if(OWL->SpecifiedObjectType==WT->Elements[j]->AssociateWithUnit){
								LeType=WT->Elements[j];
							}
						}else LeType=WT->Elements[j];
					}
					if(Usage==2){//right block
						if(OWL->SpecifiedObjectType!=0xFFFF){
							if(OWL->SpecifiedObjectType==WT->Elements[j]->AssociateWithUnit){
								ReType=WT->Elements[j];
							}
						}else ReType=WT->Elements[j];
					}
				}
				if(eType&&eType->LeftEdges.GetAmount()&&eType->RightEdges.GetAmount()){
					float ex0=eType->LeftEdges [0]->dx;
					float ey0=eType->LeftEdges [0]->dy;
					float ex1=eType->RightEdges[0]->dx;
					float ey1=eType->RightEdges[0]->dy;
					extern bool EditMapMode;
					if(EditMapMode&&GetKeyState(VK_CONTROL)&0x8000){
						if(GetKeyState('9')&0x8000){
							eType->LeftEdges [0]->dy-=0.5;
							eType->RightEdges[0]->dy+=0.5;
						}
						if(GetKeyState('0')&0x8000){
							eType->LeftEdges [0]->dy+=0.5;
							eType->RightEdges[0]->dy-=0.5;
						}
					}
					float esize=sq_norma(ex1-ex0,ey1-ey0)*eType->Scale*WT->GlobalScale;				
					if(esize>0.0001){
						int ne=int(Tdist/esize+0.5f);
						if(ne){
							float hScale=(Tdist/ne/esize)*1.05f;
							for(int j=0;j<ne;j++){
								OneWallElement* celm;
								if(LeType&&ReType){
									switch(j%3){
										case 0:
											celm=LeType;
											break;
										case 1:
											celm=eType;
											break;
										case 2:
											celm=ReType;
											break;
									}
								}else celm=eType;
								OneWallPoint* OWP=new OneWallPoint;
								OWP->Type=celm;
								OWP->x=x0+(float(j)+0.5f)*(x1-x0)/ne;
								OWP->y=y0+(float(j)+0.5f)*(y1-y0)/ne;
								int zs=GetHeight(OWP->x,OWP->y);
								if(zs<WT->MinWallHeight)zs=WT->MinWallHeight;
								OWP->z=z0+(float(j)+0.5f)*(z1-z0)/ne;
								OWP->z=(OWP->z*2+zs)/3.0f;
								//extern short randoma[8192];
								//OWP->z+=(randoma[int(OWP->x+OWP->y*3.045f)&8191]&7)-3;
								OWP->ScaleO=1.0f;
								OWP->ScaleP=hScale;
								OWP->ScaleZ=1.0f;
								if(OWP->Type->Rotation==64||OWP->Type->Rotation==64+128)swap(OWP->ScaleO,OWP->ScaleP);
								OWP->Angle=atan2((y1-y0)/1000.0f,(x1-x0)/1000.0f)*128.0/3.1415925-64.0f;
								if(ne==1){
									OWP->M4.scaling(OWP->ScaleO,OWP->ScaleP,OWP->ScaleZ);
									if(OWP->Type->Rotation==64||OWP->Type->Rotation==64+128)OWP->M4.e02=(z1-z0)/sq_norma(x1-x0,y1-y0);
									else OWP->M4.e12=(z1-z0)/sq_norma(x1-x0,y1-y0);
									Matrix4D M4;
									OWP->Angle+=OWP->Type->Rotation;
									M4.srt(OWP->Type->Scale*WT->GlobalScale,Vector3D::oZ,OWP->Angle*3.1415925f/128.0f,Vector3D(OWP->x,OWP->y,OWP->z));
									OWP->M4*=M4;
									//M4=GetSkewTM();		
									//OWP->M4*=M4;
									void AddExtraHeightObject(int x,int y,int id,Matrix4D* m4);	
									if(!(TestMode||OWL->Dead)){
										AddExtraHeightObject(OWP->x,OWP->y,OWP->Type->ModelID,&OWP->M4);									
									}
								}
								OWL->Points.Add(OWP);
							}
						}
					}
				}
			}		
		}
	}
	for(int i=0;i<Lines.GetAmount();i++){
		OneWallLine* OWL=Lines[i];		
		if(OWL->Start&&OWL->Final&&OWL->Start->Points.GetAmount()&&OWL->Final->Points.GetAmount()){
			int N=OWL->Points.GetAmount();
			int xs=0;
			int ys=0;
            int Ns=0;
			WallTypeDescription* WT=WList.Walls[OWL->WallType];
			if(N){
				OneWallPoint* OWP;
				for(int j=0;j<N;j++){
					OWP=OWL->Points[j];
					if(N>1){					
						float xc=OWP->x;
						float yc=OWP->y;
						float zc=OWP->z;
						float zp,xp,yp;
						float zn,xn,yn;
						if(j>0){
							xp=OWL->Points[j-1]->x;
							yp=OWL->Points[j-1]->y;
							zp=OWL->Points[j-1]->z;
						}
						if(j<N-1){
							xn=OWL->Points[j+1]->x;
							yn=OWL->Points[j+1]->y;
							zn=OWL->Points[j+1]->z;
						}
						if(j==0){
							xp=xc*2.0f-xn;
							yp=yc*2.0f-yn;
							zp=zc*2.0f-zn;
						}
						if(j==N-1){
							xn=xc*2.0f-xp;
							yn=yc*2.0f-yp;
							zn=zc*2.0f-zp;
						}
						//OWP->z=(zp+zn+zc*2.0f)/4.0f;
						//OWP->x=(xp+xn+xc*2.0f)/4.0f;
						//OWP->y=(yp+yn+yc*2.0f)/4.0f;
						float d=sq_norma(xn-xp,yn-yp);
						OWP->M4.scaling(OWP->ScaleO,OWP->ScaleP,OWP->ScaleZ);
						//if(OWP->Type->Rotation==64||OWP->Type->Rotation==64+128)OWP->M4.e02=(zn-zp)/d;
						//else OWP->M4.e12=(zn-zp)/d;
						Matrix4D M4;
						OWP->Angle+=OWP->Type->Rotation;
						OWP->z+=OWP->Type->dz;
						M4.srt(OWP->Type->Scale*WT->GlobalScale,Vector3D::oZ,OWP->Angle*3.1415925f/128.0f,Vector3D(OWP->x,OWP->y,OWP->z));
						OWP->M4*=M4;
						//M4=GetSkewTM();		
						//OWP->M4*=M4;
						Ns++;
						xs+=OWP->x;
						ys+=OWP->y;
						void AddExtraHeightObject(int x,int y,int id,Matrix4D* m4);	
						if(!(TestMode||OWL->Dead)){
							AddExtraHeightObject(OWP->x,OWP->y,OWP->Type->ModelID,&OWP->M4);
						}
					}else{
						/*OWP->M4.scaling(1.0f,OWP->ScaleP,1.0f);						
						Matrix4D M4;
						M4.srt(OWP->Type->Scale*WT->GlobalScale,Vector3D::oZ,OWP->Angle*3.1415925f/128.0f,Vector3D(OWP->x,OWP->y,OWP->z));
						OWP->M4*=M4;*/
						Ns++;
						xs+=OWP->x;
						ys+=OWP->y;
					}

				}
				//texturing (if required)
				if(!TestMode){
					if(WT->Texturing.GetAmount()&&!OWL->Textured){
						WallGroundTexturingParam* WGTP=WT->Texturing[0];
						if(WGTP){
							WGTP->Texturize(OWL->Start->x,OWL->Start->y,OWL->Final->x,OWL->Final->y);
							OWL->Textured=1;
						}
					}
				}
				if(!(TestMode||OWL->Dead)){
					if(/*OWL->OwnerObjectID==0xFFFF&&*/Ns){
						int idx=CreateOrFindBuilding(OWL->OwnerNI,(xs/Ns)<<4,(ys/Ns)<<4,OWP->Type->AssociateWithUnit);
						if(idx!=-1){
							OneObject* OB=Group[idx];
							if(OB){
								//if(OB->Stage<5){
								//	for(int i=OB->Stage;i<OB->NStages;i++)OB->NextStage();
								//	OB->Ready=1;//temp
								//}
								OWL->OwnerObjectID=idx;
								OWL->OwnerObjectSN=OB->Serial;
								OWL->OwnerObjectX=(xs/Ns)<<4;
								OWL->OwnerObjectY=(ys/Ns)<<4;
								OWL->CreateLocking();
								ReCreateTop=1;
								int X=(OWL->Start->x)/64;
								int Y=(OWL->Start->y)/64;
								if(X>tx1)tx1=X;
								if(X<tx0)tx0=X;
								if(Y>ty1)ty1=Y;
								if(Y<ty0)ty0=Y;
								X=(OWL->Final->x)/64;
								Y=(OWL->Final->y)/64;
								if(X>tx1)tx1=X;
								if(X<tx0)tx0=X;
								if(Y>ty1)ty1=Y;
								if(Y<ty0)ty0=Y;
							}
						}
					}
				}
			}			
		}
		OWL->ReCreateHeight=0;
	}
	if(!TestMode){
		if(ReCreateTop){
			for(int i=0;i<NMFIELDS;i++){
				if(HashTable[i].NAreas)HashTable[i].ReCreateAreas(tx0-10,ty0-10,tx1+10,ty1+10);
			}
		}
	}
	void GetAwayAllUnitsFromLockpoints();
	void SetMonstersInCells();
	if(!TestMode){
		GetAwayAllUnitsFromLockpoints();
		SetMonstersInCells();
		for(int i=0;i<NObjsToBuild;i++){
			OneObject* OB=Group[ObjsToBuild[i]];
			void BuildWithSelected(byte NI,word ObjID,byte OrdType);
			BuildWithSelected(OB->NNUM,OB->Index,i==0?0:2);
		}
	}
}
#define vclamp(v)\
	if(v<0)v=0;\
	if(v>=MaxTH)v=MaxTH-1;
int GetPointToLineDist(int x,int y,int x1,int y1,int x2,int y2);
int GetFractalVal(int x,int y);
int GetPointToLineDist(int x,int y,int x1,int y1,int x2,int y2){
	int dx1=x-x1;
	int dy1=y-y1;
	int dx12=x2-x1;
	int dy12=y2-y1;
	int r1=Norma(dx1,dy1);
	int r12=Norma(dx12,dy12);
	if(r12){
		dx12=256*dx12/r12;
		dy12=256*dy12/r12;
		//r12=256;
		int sp=(dx1*dx12+dy1*dy12)/r12;
		if(sp<0)return r1;
		if(sp>256)return Norma(x2-x,y2-y);
		return abs(dy1*dx12-dx1*dy12)>>8;
	}else return r1;
}
int GetPointToLineDistEx(int x,int y,int x1,int y1,int x2,int y2,int& xn,int& yn,int& Weight){
	int dx1=x-x1;
	int dy1=y-y1;
	int dx12=x2-x1;
	int dy12=y2-y1;
	int r1=sq_norma(dx1,dy1);
	int r12=sq_norma(dx12,dy12);
	xn=x1;
	yn=y1;
	if(r12){
		dx12=256*dx12/r12;
		dy12=256*dy12/r12;
		int sp=(dx1*dx12+dy1*dy12)/r12;
		if(sp<0){
			Weight=r1?abs(dy1*dx12-dx1*dy12)/r1:0;
			return r1;
		}if(sp>256){
			xn=x2;
			yn=y2;
			int dx1=x-x2;
			int dy1=y-y2;
			int r1=sq_norma(dx1,dy1);
			Weight=r1?abs(dy1*dx12-dx1*dy12)/r1:0;
			return sq_norma(x2-x,y2-y);
		}
		xn=x1+(x2-x1)*sp/256;
		yn=y1+(y2-y1)*sp/256;
		Weight=255;
		return abs(dy1*dx12-dx1*dy12)>>8;
	}else return r1;
}
extern byte* FactureMap;
extern byte* FactureWeight;
void SetTexFactInPoint(int V,int Texture0,int Facture0,float FactrureMaxWeight0,int Texture1,int Facture1,float FactrureMaxWeight1,int TW,int W){
	
}
void WallGroundTexturingParam::Texturize(int x0,int y0,int x1,int y1){
	int vx0=(x0>>5);
	int vx1=(x1>>5);
	int vy0=(y0>>5);
	int vy1=(y1>>5);
	if(vx1<vx0)swap(vx1,vx0);
	if(vy1<vy0)swap(vy1,vy0);
	vx0-=(TowerMaxRadius>>4)+3;
	vx1+=(TowerMaxRadius>>4)+3;
	vy0-=(TowerMaxRadius>>4)+3;
	vy1+=(TowerMaxRadius>>4)+3;

	vclamp(vx0);
	vclamp(vx1);
	vclamp(vy0);
	vclamp(vy1);
	
	for(int vx=vx0;vx<=vx1;vx++){
		for(int vy=vy0;vy<=vy1;vy++){
            int x=vx<<5;
			int y=(vy<<5)-((vx&1)<<4);
			int V=vx+vy*VertInLine;
			int xn,yn,w;
			int R=GetPointToLineDistEx(x,y,x0,y0,x1,y1,xn,yn,w);
			int R0=Norma(x-x0,y-y0);
			int R1=Norma(x-x1,y-y1);
			bool FRONT=((x-x0)*(y1-y0)-(y-y0)*(x1-x0))<0;
			//if(R<min(R0,R1)){
				for(int t=0;t<Ditch.GetAmount();t++){
					WallDitchParam* WDP=Ditch[t];
					if(WDP&&((FRONT&&WDP->InFront)||(WDP->InBack&&!FRONT))){
						int dr=10000;
						int dw=R-WDP->DistanceFromWall;												
						if(dw>-WDP->InWidth&&dw<WDP->OutWidth){
							dr=dw;							
							int W=0;
							if(dr<0)W=(WDP->InWidth+dr)*256/WDP->InWidth;
							if(dr>0)W=(WDP->OutWidth-dr)*256/WDP->OutWidth;
							float f=float(GetFractalVal(x*3,y*3))/512.0;
							int FW=int((1.0f-f*WDP->DepthVariation)*256.0f);
							int DH=(WDP->Depth*FW)/256;
							W=(W*w)/256;
							int H1=GetHeight(xn,yn)-(DH*W/256);
							if(H1<0)H1=0;
							int H2=(H1*W+int(THMap[V])*(255-W))/256;
							if(DH>0)THMap[V]=min(int(THMap[V]),H2);
							else if(DH<0)THMap[V]=max(int(THMap[V]),H2);							
							int TW=GetFractalVal(x*1015/400,y*1015/400)>>1;
							if(TW>255)TW=255;
							if(TW<0)TW=0;
							dr=float(dr)/WDP->TexturedWidthScale;
							if(dr<0)W=(WDP->InWidth+dr)*256/WDP->InWidth;
							if(dr>0)W=(WDP->OutWidth-dr)*256/WDP->OutWidth;
							if(W<0)W=0;
							if(W>255)W=255;
							W=(W*w)/256;
							if(W)SetTexFactInPoint(V,WDP->Texture0,WDP->Facture0,WDP->FactureMaxWeight0,WDP->Texture1,WDP->Facture1,WDP->FactureMaxWeight1,float(TW)*WDP->TextureBlending,W);
						}					
					}
				}
			//}
			for(int t=0;t<SettingObjects.GetAmount();t++){
				WallTreesParam* WTP=SettingObjects[t];
				if(WTP){
					if((WTP->StandInFrontOfWall&&FRONT)||(WTP->StandInBackOfWall&&!FRONT)){
						bool STAND=0;
						bool Erase=1;
						if(R0>WTP->MaxDistFromTower&&R1>WTP->MaxDistFromTower){
							if(R>=WTP->MinDistFromWall&&R<=WTP->MaxDistFromWall)STAND=true;
							if(R<WTP->MinDistFromWall)Erase=1;
						}else{
							if(R0>WTP->MinDistFromTower&&R0<WTP->MaxDistFromTower)STAND=true;
							if(R1>WTP->MinDistFromTower&&R1<WTP->MaxDistFromTower)STAND=true;
							if(R0<WTP->MinDistFromTower||R1<WTP->MinDistFromTower)Erase=1;
						}
						void EraseTreesInPoint(int x,int y);
						if(Erase)EraseTreesInPoint(x,y);
						if(STAND){
							int FW=GetFractalVal(x*6,y*6);
							int W=(WTP->Density-float(FW)*WTP->DensityVariation/512.0)*32768;
							if(rando()<W){
								OneTreesGroup* OTG=TSC.TGroup[WTP->TreesGroup];
								int id=(int(rando())*OTG->Indices.GetAmount())>>15;
								SprGroup* GS=OTG->SGS[id];
								int idx=OTG->Indices[id];
								void addSprite(int x,int y,SprGroup* SG,word id,int PrefRadius);
								int DH=GetTotalHeight(x,y)-GetHeight(x,y);
								if(DH<4)addSprite(x,y,GS,idx,1);
							}
						}
					}
				}
			}
			int FW=GetFractalVal(x*520/VariationDistance,y*520/VariationDistance)>>1;
			if(FW>255)FW=255;
			if(FW<0)FW=0;			
            int LR=MinRadius+(MaxRadius-MinRadius)*FW/256;
			int TR=TowerMinRadius+(TowerMaxRadius-TowerMinRadius)*FW/256;
			LR*=2;
			TR*=2;
			int W=max(R<LR?(LR-R)*256/LR:0,max(R0<TR?(TR-R0)*256/TR:0,R1<TR?(TR-R1)*256/TR:0));			
			if(W>32){
				int TW=GetFractalVal(x*1024/VariationDistance,y*1024/VariationDistance)>>1;
				if(TW>255)TW=255;
				if(TW<0)TW=0;
				SetTexFactInPoint(V,Texture0,Facture0,FactrureMaxWeight0,Texture1,Facture1,FactrureMaxWeight1,FW,W);
			}
		}
	}
}
DWORD CurDiffLight=0;
DWORD ModDWORD(DWORD C1,DWORD C2);
void SetPulsedLight(DWORD ModColor){
	//return;
	static DWORD lightID = IMM->GetModelID( "GameLight" );
	int LX=IMM->GetLight(lightID)->GetDiffuse();
	if(LX)CurDiffLight=LX;
	if(ModColor){
		int W=128+int(126.0f*sin(float(GetTickCount())/200.0f));
		DWORD L=MixDWORD(ModColor,0xFF000000,W,255-W);
		IMM->GetLight(lightID)->SetDiffuse(L);
		IMM->GetLight(lightID)->Render();
	}
}
void SetStatLight(DWORD ModColor){
	return;
	static DWORD lightID = IMM->GetModelID( "GameLight" );
	int LX=IMM->GetLight(lightID)->GetDiffuse();
	if(LX)CurDiffLight=LX;
	if(ModColor){
		IMM->GetLight(lightID)->SetDiffuse(ModColor);
		IMM->GetLight(lightID)->Render();
	}
}
void RestoreLight(){
	return;
	static DWORD lightID = IMM->GetModelID( "GameLight" );
	if(CurDiffLight)IMM->GetLight(lightID)->SetDiffuse(CurDiffLight);
	IMM->GetLight(lightID)->Render();
}
void OneWallSection::SetColor(){
	void SetPulsedLight(DWORD ModColor);
	if(Owner){
		if(Owner->TestMode==1){
			//SetPulsedLight(0);
			IRS->SetTextureFactor(0xFF808080);
			return;
		}
		if(Owner->TestMode==2){
			//SetPulsedLight(0xFFFF0000);
			IRS->SetTextureFactor(0xFFFF0000);
			return;
		}
	}
	if(OwnerObjectID!=0xFFFF){
		OneObject* OB=Group[OwnerObjectID];
		if(OB&&OB->Serial==OwnerObjectSN){
			if(OB->ImSelected&(1<<MyNation)){
				//SetPulsedLight(0xFFFFFFFF);
				IRS->SetTextureFactor(0xFF808080);
				return;
			}
		}
	}
	IRS->SetTextureFactor(0xFF808080);
	//SetPulsedLight(0);
}
void OneWallEdge::SetColor(){
	/*if(NeedToOpen){
		SetPulsedLight(0xFF0000FF);
		return;
	}*/	
	OneWallSection::SetColor();	
}
void LogWalls(char* sz,...){
	char ach[256];
	va_list va;
	va_start( va, sz );
	vsprintf ( ach, sz, va );   
	va_end( va );
	FILE* f=fopen("walls.log","a");
	fprintf(f,ach);
	fclose(f);
}
float sq_norma(float x,float y);
void DrawLesa(OneObject* OB,AABoundBox& AABB,Matrix4D& M4,WallTypeDescription* WTD,float zHeight,float Angle){
	//if(OB->Stage<3)return;
    ClassArray<OneWallLes>& WLL=WTD->Lesa;
	int N=WLL.GetAmount();
	if(N){
		for(int j=0;j<N;j++)if(!WLL[j]->AABB_Init){
			WLL[j]->AABB=IMM->GetBoundBox(WLL[j]->Model);
			WLL[j]->AABB_Init=1;
		}
		float R=sq_norma(AABB.maxv.x-AABB.minv.x,AABB.maxv.y-AABB.minv.y);
		AABoundBox& AB1=WLL[0]->AABB;
        float R1=sq_norma(AB1.maxv.x-AB1.minv.x,AB1.maxv.y-AB1.minv.y);
		if(R1>0x0001&&WLL[0]->Scale){
			int N=R/R1/WLL[0]->Scale;
			if(!N)N=1;
			Matrix4D M=M4;
			float sc=WLL[0]->Scale;
			Matrix4D SC;
			SC.scaling(sc);
			M.mulLeft(SC);

            float dx=AB1.maxv.x-AB1.minv.x;
			float dy=AB1.maxv.y-AB1.minv.y;
			float dz=AB1.maxv.z-AB1.minv.z;

			dz*=WLL[0]->AABB_scaleZ;
			dx*=WLL[0]->AABB_scaleX;
			dy*=WLL[0]->AABB_scaleY;
			float DX=AABB.maxv.x-AABB.minv.x;
			float DY=AABB.maxv.y-AABB.minv.y;
			float DZ=AABB.maxv.z-AABB.minv.z;

			int da=(dx+dy)/2;

			int nx=(DX/da/sc)-0.5;
			int ny=(DY/da/sc)-0.5;
			if(nx<1)nx=1;
			if(ny<2)ny=2;
			
			Vector3D shX(dx,0,0);
			Vector3D shY(0,dy,0);
			Vector3D shZ(0,0,dz);
			M.transformVec(shX);
			M.transformVec(shY);
			M.transformVec(shZ);
			Matrix4D M0=M;
			Vector3D sh(-dx*(nx-1)/2,-dy*(ny-1)/2,0);
			M.transformVec(sh);
			M0.translate(sh);
			Matrix4D mr;
			mr.rotation(Vector3D(0,0,1),3.1415/2);
			int rp=OB?int(OB->RealX*135+OB->RealY*371)&4095:0;
			for(int ix=0;ix<nx;ix++){
				for(int iy=0;iy<ny;iy++){
					if(randoma[rp]&1){
						M0.mulLeft(mr);
					}
					M=M0;
					for(int q=0;q<ix;q++)M.translate(shX);
					for(int q=0;q<iy;q++)M.translate(shY);
					for(int i=0;i<3;i++)if(M.e32<zHeight){
						extern short randoma[8192];
						if(randoma[rp]&32){
							M.mulLeft(mr);
						}
						if(fabs(Angle)<0.0001){
							IMM->StartModel(WLL[0]->Model,M,ix);
							IShadowMgr->AddCaster(WLL[0]->Model,M);
                            IMM->DrawModel();
						}else{
							Matrix4D MR;
							float ang=Angle*randoma[rp+17]/24000;
							MR.rotation(Vector3D(randoma[rp+3]-16384,randoma[rp+39]-16384,0),ang);
							Vector3D tr(randoma[rp+29]-16384,randoma[rp+71]-16384,0);
                            tr*=ang/60.0f;
							MR.translate(tr);
							MR*=M;
							IMM->StartModel(WLL[0]->Model,MR,ix+1235);
							IShadowMgr->AddCaster(WLL[0]->Model,MR);
                            IMM->DrawModel();
						}
						M.translate(shZ);
						rp++;
					}
				}
			}
		}
	}
}
int LastWTaskT=0;
bool GetObjectVisibilityInFog(int x,int y,int z,OneObject* OB);
DWORD GetNatColor( int natIdx );
DWORD ModDWORD2X(DWORD C1,DWORD C2);
void OneWallPoint::Draw(float anmstage,OneObject* OB,WallTypeDescription* WT,OneWallSection* OWS){	
	if(OB){
		IRS->SetColorConst(GetNatColor(OB->NNUM));
		if(OB->newMons->TFactor)IRS->SetTextureFactor(ModDWORD2X(OB->newMons->TFactor,IRS->GetTextureFactor()));
		//else IRS->SetTextureFactor(0xFF808080);
	}//else IRS->SetTextureFactor(0xFF808080);

    int T=LastWTaskT;
	Vector3D V=Vector3D(x,y,z);
    WorldToScreenSpace(V);	

	if(V.x<-300||V.x>RealLx+300||V.y<-300||V.y>RealLy+350)return;
	if(OB || (OB && OB->NNUM!=MyNation)){
		if(!((OB && OB->TempFlag) || GetObjectVisibilityInFog(x,y,z,OB)))return;	
	}

	PushEntityContext(DWORD(OB)+DWORD(this)+DWORD(x)+DWORD(y*10000));

	if(CrashTime[0]!=0||DestructionMask!=0xFFFFFFFF
		&&Type->DestructedModelID>0&&Type->DestructedAnimationID>0){		


		int ndp=Type->DestructionParts.GetAmount();		
        IMM->StartModel(Type->DestructedModelID,M4,CUR_CONTEXT);
        IMM->AnimateModel(Type->DestructedAnimationID,0);
		bool SOMETHING=0;
		if(ndp==0){
			int dt=CrashTime[0]==0?0:GetTickCount()-CrashTime[0];		
			if(dt){
				if(dt<35000){
					float t=float(dt)*Type->TimeScale;
					IMM->AnimateModel(Type->DestructedAnimationID,t);											
					SOMETHING=1;
				}
			}
		}
		for(int i=0;i<ndp&&i<MaxParts;i++){
			int ii=i<32?i:31;
			OneWallDestructionPart* OWDP=Type->DestructionParts[i];
			if(OWDP){
				if(OWDP->NodeID<=0){
					OWDP->NodeID=IMM->GetNodeID(Type->DestructedModelID,OWDP->NodeName.pchar());
					OWDP->AnimNodeID=IMM->GetNodeID(Type->DestructedAnimationID,OWDP->NodeName.pchar());									
				}
				if(OWDP->NodeID<=0){
					LogWalls("Wrong node <%s> for model <%s>\n",OWDP->NodeName.pchar(),IMM->GetModelFileName(Type->DestructedModelID));
				}
				if(OWDP->AnimNodeID<=0){
					LogWalls("Wrong node <%s> for animation <%s>\n",OWDP->NodeName.pchar(),IMM->GetModelFileName(Type->DestructedAnimationID));
				}
				
				if(OWDP->NodeID>=0&&OWDP->AnimNodeID>=0){
					int dt=CrashTime[i]==0?0:GetTickCount()-CrashTime[i];
					IMM->SetVisible(OWDP->NodeID,1);
					if(dt){
						if(dt<35000){
							SOMETHING=1;
							float t=float(dt)*Type->TimeScale;
                            IMM->StartModel(OWDP->NodeID,M4,CUR_CONTEXT);
							IMM->AnimateModel(/*OWDP->NodeID,*/OWDP->AnimNodeID,t,0);
							/*
							Matrix4D M4X=IMM->GetNodeTransform(OWDP->NodeID);
							Vector3D V=OWDP->Center;											
							Matrix4D M40=M4X;
							M4X*=M4;
							M4X.transformPt(V);
							float sf=0;
							float at=IMM->GetAnimTime(Type->DestructedAnimationID);
							if(t>OWDP->FallStartTime&&at>0.01){
								float tdz=(t-OWDP->FallStartTime)*30.0f/(at-OWDP->FallStartTime);
								if(t<OWDP->RelaxTime){
									sf=(t-OWDP->FallStartTime)/(OWDP->RelaxTime-OWDP->FallStartTime);
								}else{
									sf=1.0f;													
								}
								float zsc=M4.e22;
								float zg=GetHeight(V.x,V.y)-M4.e32;
								if(zg<0)zg=0;
								float z0=OWDP->FallDeep*zsc;
								float dz=((z0-zg+tdz)*sf/zsc);
								if(dz<-0.1){
									//M40.e32-=dz;
									//IMM->SetNodeTransform(OWDP->NodeID,M40);
								}								
							}
							*/
						}else IMM->SetVisible(OWDP->NodeID,0);
					}else SOMETHING=1;
				}
			}
		}
		//if(SOMETHING){
            IMM->StartModel(Type->DestructedModelID,M4,CUR_CONTEXT/*i+98765*/);
			void RegisterVisibleGP( OneObject* OB, int modelID, const Matrix4D& tm );
			RegisterVisibleGP(OB,Type->DestructedModelID,M4);
			//IMM->StartModel(Type->DestructedModelID,M4);
			IShadowMgr->AddCaster(Type->DestructedModelID,M4);
            IMM->DrawModel();
		//}
	}else if(Visible){		
		float stage=OB?float(OB->Stage)/OB->NStages:1.0f;
		if(stage<0.05f)stage=0.05f;
		AABoundBox AB=IMM->GetBoundBox(Type->ModelID);			
		float Height=AB.maxv.z*1.3f;
		Plane PL;
		if(Height<=0){
			Vector3D P1(0,0,0);
			Vector3D P2(10,0,0);
			Vector3D P3(0,10,0);
			M4.transformPt(P1);
			M4.transformPt(P2);
			M4.transformPt(P3);	
			P1.z*=stage;
			P2.z*=stage;
			P3.z*=stage;
			PL.from3Points(P1,P2,P3);
			IRS->SetClipPlane(1,PL);
		}else{
			Vector3D P1(0,0,Height*stage);
			Vector3D P2(10,0,Height*stage);
			Vector3D P3(0,10,Height*stage);	
			M4.transformPt(P1);
			M4.transformPt(P2);
			M4.transformPt(P3);	
			Plane PL;
			PL.from3Points(P1,P2,P3);
			IRS->SetClipPlane(1,PL);
		}		

		//IRS->SetShaderConst( 0,AB.minv.z+(AB.maxv.z-AB.minv.z)*(0.2+stage*0.8) );
		IRS->SetShaderConst( 0,M4.e32+(AB.maxv.z-AB.minv.z)*M4.e22*(0.1+stage*0.9) );

		IMM->StartModel(Type->ModelID,M4,DWORD(this));
        if(Type->StandAnimation>0){
            float T=IMM->GetAnimTime( Type->StandAnimation );
            if(T>0 && Type->StandAnimationTime>0){
                int T2=GetTickCount() % Type->StandAnimationTime;
                IMM->AnimateModel(Type->StandAnimation,T2*T/Type->StandAnimationTime);
            }else{
                IMM->AnimateModel(Type->StandAnimation,0);
            }
        }
		if(Type&&Type->OpeningAnimationID>0){
			int T=IMM->GetAnimTime(Type->OpeningAnimationID)/2;
			if(T){
				IMM->AnimateModel(Type->OpeningAnimationID,T*anmstage);
			}
		}
		IShadowMgr->AddCaster(Type->ModelID,M4);
        IMM->DrawModel();
		IRS->SetShaderConst( 0,100000 );
        
        //  added by Silver, 25.06.04
        DWORD GetObjectsShadowQuality();
        //if (GetObjectsShadowQuality() == 0) IShadowMgr->AddCaster( Type->ModelID,M4 );
        void RegisterVisibleGP( OneObject* OB, int modelID, const Matrix4D& tm );
        RegisterVisibleGP( OB, Type->ModelID, M4 );
		
		void SetGlobalClipPlane(Vector3D& pos,Vector3D& norm);
		int dt=T-OWS->LastBuildTime;
		if(OB&&OB->Stage<OB->NStages||(OWS->LastBuildTime&&dt<10000)){
			float dzz=0;
			if(OB->Stage==OB->NStages){
				int prd=randoma[OB->RealX&8191]&63;
				dzz=float(dt)/120+float(dt/160.0f)*float(dt/160.0f);
				dzz=dzz*(128+64+prd)/256;
			}
			float DH=60;
			float zz=Height*stage+DH;
			Matrix4D M41=M4;
			if(dzz){
				Vector3D V=Vector3D(0,0,-dzz);
				M41.translate(V);
			}
			
			Vector3D P11(0,0,zz);
			Vector3D P21(10,0,zz);
			Vector3D P31(0,10,zz);	
			if(Height==0){
				P11.z*=stage;
				P21.z*=stage;
				P31.z*=stage;
				P11.z+=30;
				P21.z+=30;
				P31.z+=30;
				M41.transformPt(P11);
				M41.transformPt(P21);
				M41.transformPt(P31);	
				Plane PL1;
				PL1.from3Points(P11,P21,P31);
				IRS->SetClipPlane(1,PL1);
			}else{
				M41.transformPt(P11);
				M41.transformPt(P21);
				M41.transformPt(P31);	
				Plane PL1;
				PL1.from3Points(P11,P21,P31);
				IRS->SetClipPlane(1,PL1);
			}
			if(OB->Stage>=3||WT->DontUseBastion)DrawLesa(OB,AB,M41,WT,P11.z+40,dzz/70.0f);
			
			SetGlobalClipPlane(Vector3D(0,0,1000),Vector3D(0,0,-1));
		}
	}    
	PopEntityContext();
}
void OneWallsSystem::Draw(){	
	LastWTaskT=GetTickCount();
	for(int i=0;i<Edges.GetAmount();i++){
        OneWallEdge* OWE=Edges[i];
		if(OWE&&(OWE->GetObj()||OWE->Owner->TestMode)){			
			for(int j=0;j<OWE->Points.GetAmount();j++){
				OneWallPoint* OWP=OWE->Points[j];
				if(OWP){
					OWE->SetColor();
					OWP->Draw(float(OWE->AnimationStage)/256/25/2,OWE->GetObj(),WList.Walls[OWE->WallType],OWE);
				}
			}
			//RestoreLight();
			IRS->SetTextureFactor(0xFF808080);
		}
	}
	for(int i=0;i<Lines.GetAmount();i++){
		OneWallLine* OWL=Lines[i];
		if(OWL&&(OWL->GetObj()||OWL->Owner->TestMode)){			
			for(int j=0;j<OWL->Points.GetAmount();j++){
				OneWallPoint* OWP=OWL->Points[j];
				if(OWP){
					OWL->SetColor();
					OWP->Draw(0,OWL->GetObj(),WList.Walls[OWL->WallType],OWL);
				}
			}
			//RestoreLight();
			IRS->SetTextureFactor(0xFF808080);
		}
	}
	void SetGlobalClipPlane(Vector3D& pos,Vector3D& norm);
	SetGlobalClipPlane(Vector3D(0,0,1000),Vector3D(0,0,-1));
}
void OneWallsSystem::AddEdge(int x,int y,int ID,int Type,int NI,bool Bastion){
	OneWallEdge* OWE=new OneWallEdge;
	OWE->x=x;
	OWE->y=y;
	OWE->z=GetHeight(x,y);
	WallTypeDescription* WT=WList.Walls[Type];
	if(OWE->z<WT->MinWallHeight)OWE->z=WT->MinWallHeight;
	OWE->EdgeID=ID;
	OWE->WallType=Type;
	OWE->OwnerNI=NI;
	OWE->TowerType=Bastion;
	Edges.Add(OWE);
}
void OneWallsSystem::AddLine(int ID1,int ID2,int Type,int NI){
	OneWallLine* OWL=new OneWallLine;
	OWL->StartEdge=ID1;
	OWL->FinalEdge=ID2;
	OWL->WallType=Type;
	OWL->OwnerNI=NI;
	Lines.Add(OWL);
}
OneWallEdge* OneWallsSystem::GetNearEdge(int x,int y,int R){
	int MR=R;
    OneWallEdge* OWE=NULL;	
	for(int i=0;i<Edges.GetAmount();i++){
		OneWallEdge* OWE2=Edges[i];
		OneObject* OB=OWE2->GetObj();
		if(OB&&OWE2->TowerType==0&&!OB->Sdoxlo){
			int cR=Norma(Edges[i]->x-x,Edges[i]->y-y);
			if(cR<MR){
				MR=cR;
				OWE=Edges[i];
			}
		}
	}
	return OWE;
}
OneWallsSystem OWS;
OneWallsSystem OWS_Temp;
bool ApplyCost(byte NI,word NIndex,int x,int y);
bool CheckCost(byte NI,word NIndex,int x,int y);
void AddWallSegment(byte NI,int Type,int x0,int y0,int x1,int y1){
	WallTypeDescription* WTD=WList.Walls[Type];	
	for(int i=0;i<WTD->Elements.GetAmount();i++){
		if(WTD->Elements[i]->Usage==1){
			if(!ApplyCost(NI,WTD->Elements[i]->AssociateWithUnit,-1,-1))return;
			if(!CheckCost(NI,WTD->Elements[i]->AssociateWithUnit,-1,-1)){
				WallsEditMode=false;
			}
		}
	}
	int maxEdge=1;
	for(int i=0;i<OWS.Edges.GetAmount();i++){
		if(OWS.Edges[i]->EdgeID>maxEdge)maxEdge=OWS.Edges[i]->EdgeID;
	}
	maxEdge++;
	OneWallEdge* OWE0=OWS.GetNearEdge(x0,y0,min(SnapToEdgeDist,WTD->MinWallLength-20));
	OneWallEdge* OWE1=OWS.GetNearEdge(x1,y1,min(SnapToEdgeDist,WTD->MinWallLength-20));
	int ID1,ID2,ID3;
	if(OWE0)ID1=OWE0->EdgeID;
	else OWS.AddEdge(x0,y0,ID1=maxEdge++,Type,NI,0);
	if(OWE1)ID2=OWE1->EdgeID;
	else OWS.AddEdge(x1,y1,ID2=maxEdge++,Type,NI,0);
	if(WTD->DontUseBastion){
		OWS.AddLine(ID1,ID2,Type,NI);
	}else{
		OWS.AddEdge((x0+x1)/2,(y0+y1)/2,ID3=maxEdge++,Type,NI,1);
		OWS.AddLine(ID1,ID3,Type,NI);
		OWS.AddLine(ID3,ID2,Type,NI);
	}
	OWS.ReCreate();
}
void AddCheapWallSegment(byte NI,int Type,int x0,int y0,int x1,int y1){
	WallTypeDescription* WTD=WList.Walls[Type];
	int maxEdge=1;
	for(int i=0;i<OWS.Edges.GetAmount();i++){
		if(OWS.Edges[i]->EdgeID>maxEdge)maxEdge=OWS.Edges[i]->EdgeID;
	}
	maxEdge++;
	OneWallEdge* OWE0=OWS.GetNearEdge(x0,y0,min(SnapToEdgeDist,WTD->MinWallLength-20));
	OneWallEdge* OWE1=OWS.GetNearEdge(x1,y1,min(SnapToEdgeDist,WTD->MinWallLength-20));
	int ID1,ID2,ID3;
	if(OWE0)ID1=OWE0->EdgeID;
	else OWS.AddEdge(x0,y0,ID1=maxEdge++,Type,NI,0);
	if(OWE1)ID2=OWE1->EdgeID;
	else OWS.AddEdge(x1,y1,ID2=maxEdge++,Type,NI,0);
	if(WTD->DontUseBastion){
		OWS.AddLine(ID1,ID2,Type,NI);
	}else{
		OWS.AddEdge((x0+x1)/2,(y0+y1)/2,ID3=maxEdge++,Type,NI,1);
		OWS.AddLine(ID1,ID3,Type,NI);
		OWS.AddLine(ID3,ID2,Type,NI);
	}
	OWS.ReCreate();
}
void OneWallLine::CreateLocking(){
	if(Start&&Final&&!Dead){
		int x0,y0,x1,y1;
		x0=Start->x;
		y0=Start->y;
		x1=Final->x;
		y1=Final->y;
		int N=Norma(x1-x0,y1-y0)/32+1;
		for(int i=0;i<=N;i++){
			int x=x0+(x1-x0)*i/N;
			int y=y0+(y1-y0)*i/N;
			///BSetBar( (((x+32)>>6)<<2)-4 , (((y+32)>>6)<<2)-4 , 8 );
		}
	}
}
void InitWalls(){
	
}
void ReInitWalls(){
	REG_CLASS(WallDitchParam);
	REG_CLASS(WallTreesParam);
	REG_CLASS(WallGroundTexturingParam);
	REG_CLASS(OneEdge);
	REG_CLASS(OneWallElement);
	REG_CLASS(WallTypeDescription);
	REG_CLASS(OneWallPoint);
	REG_CLASS(OneWallEdge);
	REG_CLASS(OneWallLine);
	REG_CLASS(OneWallLes);
	WList.reset_class(&WList);
	WList.SafeReadFromFile("Dialogs\\Walls.WallsList.xml");
}
void TestWalls(){
	static bool IsInit=0;
	if(WList.Walls.GetAmount()){		
		if(WallsEditMode){		
			if(Rpressed){
				WallsEditMode=0;
				Rpressed=0;
				UnPress();
				PrevClickX=0;
				PrevClickY=0;
				return;
			}
			extern int LastMx;
			extern int LastMy;
			int x=LastMx;
			int y=LastMy;
			WallTypeDescription* WTD=WList.Walls[WallsEditMode-1];
			if(PrevClickX>0&&(x!=PrevClickX||y!=PrevClickY)){
				int R=Norma(x-PrevClickX,y-PrevClickY);				
				int R1=R;
				if(R1<WTD->MinWallLength)R1=WTD->MinWallLength;
				if(R1>WTD->MaxWallLength)R1=WTD->MaxWallLength;
				x=PrevClickX+(x-PrevClickX)*R1/R;
				y=PrevClickY+(y-PrevClickY)*R1/R;
			}
			OneWallEdge* OWE=OWS.GetNearEdge(x,y,min(SnapToEdgeDist,WTD->MinWallLength-20));
			bool P1Z=GetHeight(x,y)>0;
			bool P1R=true;
			bool P2Z=true;
			bool P2R=true;
			if(OWE){
				x=OWE->x;
				y=OWE->y;
				OneObject* OB=OWE->GetObj();
				P1R=(OB&&OB->Stage==OB->NStages)||OB==NULL;
			}
			if(PrevClickX==0){
				if(Lpressed){
                    PrevClickX=x;
					PrevClickY=y;
					Lpressed=0;
					UnPress();
				}else{
					if(WTD->DontUseBastion){
						OWS_Temp.reset_class(&OWS_Temp);										
						OWS_Temp.AddEdge(x-10-WTD->MinWallLength/2,y,1,WallsEditMode-1,MyNation,0);
						OWS_Temp.AddEdge(x+10+WTD->MinWallLength/2,y,2,WallsEditMode-1,MyNation,0);
						OWS_Temp.AddLine(1,2,WallsEditMode-1,MyNation);
						OWS_Temp.TestMode=1;
						OWS_Temp.ReCreate();
						OWS_Temp.Draw();
					}else{
						OWS_Temp.reset_class(&OWS_Temp);										
						OWS_Temp.AddEdge(x,y,1,WallsEditMode-1,MyNation,0);
						OWS_Temp.TestMode=1;
						OWS_Temp.ReCreate();
						OWS_Temp.Draw();
					}
				}
			}else{
                OWS_Temp.reset_class(&OWS_Temp);
				int R=Norma(x-PrevClickX,y-PrevClickY);
				WallTypeDescription* WTD=WList.Walls[WallsEditMode-1];
				if(WTD->DontUseBastion){
					OWS_Temp.AddEdge(PrevClickX,PrevClickY,1,WallsEditMode-1,MyNation,0);
					OWS_Temp.AddEdge(x,y,2,WallsEditMode-1,MyNation,0);					
					OWS_Temp.AddLine(1,2,WallsEditMode-1,MyNation);					
					OWS_Temp.Lines[0]->Textured=1;
					OWS_Temp.TestMode=1;
				}else{
					OWS_Temp.AddEdge(PrevClickX,PrevClickY,1,WallsEditMode-1,MyNation,0);
					OWS_Temp.AddEdge(x,y,2,WallsEditMode-1,MyNation,0);
					OWS_Temp.AddEdge((x+PrevClickX)/2,(y+PrevClickY)/2,3,WallsEditMode-1,MyNation,1);
					OWS_Temp.AddLine(1,3,WallsEditMode-1,MyNation);
					OWS_Temp.AddLine(3,2,WallsEditMode-1,MyNation);
					OWS_Temp.Lines[0]->Textured=1;
					OWS_Temp.Lines[1]->Textured=1;
					OWS_Temp.TestMode=1;
				}
				OneWallEdge* OWEP=OWS.GetNearEdge(PrevClickX,PrevClickY,min(SnapToEdgeDist,WTD->MinWallLength-20));
				P2Z=GetHeight(PrevClickX,PrevClickY)>0;
				if(OWEP){
					OneObject* OB=OWEP->GetObj();
					P2R=(OB&&OB->Stage==OB->NStages)||OB==NULL;
				}
				
				bool IsEnemyNearWall(int x0,int y0, int x1, int y1, int Radius, byte NI);
				bool IsBuildingNearWall(int x0,int y0, int x1, int y1, int Radius);
				bool en=true;
				if(IsEnemyNearWall(PrevClickX,PrevClickY,x,y,300,GetMyNation()))
				{
					en=false;
					OWS_Temp.TestMode=2;
				}
				bool TestDoubleWall(int x0,int y0, int x1, int y1);
				if(en&&(IsBuildingNearWall(PrevClickX,PrevClickY,x,y,100)
					||TestDoubleWall(PrevClickX,PrevClickY,x,y)/*||GetHeight(PrevClickX,PrevClickY)<-20||GetHeight(x,y)*/)){
					en=false;
					OWS_Temp.TestMode=2;
				}
				if(WTD->DontUseBastion){
					if( (!(P2Z||P2R)) || (!(P1Z||P1R)) ){
						en=false;
						OWS_Temp.TestMode=2;                    
					}
				}else{
					if( !(P1Z&&P2Z) ){
						en=false;
						OWS_Temp.TestMode=2;                    
					}
				}
				OWS_Temp.ReCreate();
				OWS_Temp.Draw();
				extern bool EditMapMode;
				if(Lpressed&&(en||(EditMapMode&&(GetKeyState(VK_CONTROL)&0x8000)))){
					Lpressed=0;
					UnPress();
					vui_IS_AddWallSection AWS;
					AWS.Data.NI=GSets.CGame.cgi_NatRefTBL[MyNation];
					AWS.Data.Type=WallsEditMode-1;
					AWS.Data.x0=PrevClickX;
					AWS.Data.y0=PrevClickY;
					AWS.Data.x1=x;
					AWS.Data.y1=y;
					AWS.InterfAction();
					PrevClickX=x;
					PrevClickY=y;
				}
			}
		}			
		//OWS.ReCreate(WList.Walls[0]);
		OWS.Draw();
	}
}
OneWallElement* GetWallElemetByObjectType(int NIndex){
	for(int i=0;i<WList.Walls.GetAmount();i++){
		WallTypeDescription* WTD=WList.Walls[i];
		if(WTD){
			for(int j=0;j<WTD->Elements.GetAmount();j++){
				OneWallElement* OWE=WTD->Elements[j];
				if(OWE&&OWE->AssociateWithUnit==NIndex)return OWE;
			}                        
		}		
	}
	return NULL;
}
int CheckIfObjectIsWall(int NIndex){
    char* uname=NATIONS->Mon[NIndex]->MonsterID;
	int L=strlen(uname)-4;
	if(L<=0)return 0;
	for(int i=0;i<WList.Walls.GetAmount();i++){
		WallTypeDescription* WTD=WList.Walls[i];
		if(WTD){
			for(int j=0;j<WTD->Elements.GetAmount();j++){
				OneWallElement* OWE=WTD->Elements[j];
				if(OWE){
					char* mname=NATIONS->Mon[OWE->AssociateWithUnit]->MonsterID;
					if(!strncmp(mname,uname,L)){
                        return i+1;
					}
				}
			}                        
		}		
	}
	return 0;
}
void SetWallBuildMode(int Mode){
	WallsEditMode=Mode;
}
OneWallSection* GetEdgeByObject(int ObjIndex){
	for(int i=0;i<OWS.Edges.GetAmount();i++)
		if(OWS.Edges[i]&&OWS.Edges[i]->OwnerObjectID==ObjIndex)
			return OWS.Edges[i];
	for(int i=0;i<OWS.Lines.GetAmount();i++)
		if(OWS.Lines[i]&&OWS.Lines[i]->OwnerObjectID==ObjIndex)
			return OWS.Lines[i];
	return NULL;
}
void UpgradeWallObject(OneObject* OB,int NewType){
	OneWallSection* OWE=GetEdgeByObject(OB->Index);
	if(OWE){
		OneWallElement* ELM=GetWallElemetByObjectType(NewType);
		if(ELM){
			OB->ClearOrders();

			OneObject* TransformUnitType(OneObject* OB,int DestType);
			TransformUnitType(OB,NewType);
			OWE->SpecifiedObjectType=NewType;			
			OWE->ReCreateHeight=1;
			for(int i=0;i<OWS.Lines.GetAmount();i++){
				if(OWS.Lines[i]->Start==OWE){
					OWS.Lines[i]->ReCreateHeight=1;
				}
				if(OWS.Lines[i]->Final==OWE){
					OWS.Lines[i]->ReCreateHeight=1;
				}
			}
			OWS.ReCreate();			
			for(int i=0;i<NMFIELDS;i++){
				HashTable[i].ReCreateAreas((OB->RealX>>10)-10,(OB->RealY>>10)-10,(OB->RealX>>10)+10,(OB->RealY>>10)+10);
			}
		}
	}
}
DWORD DestrMaskSet[]={
	1, 1+2,  1+2+4,  1+2+4+8,  1+2+4+8+16,  1+2+4+8+16+32,  1+2+4+8+16+32+64,1+2+4+8+16+32+128,
	1, 1+2,  1+2+4,  1+2+4+8,  1+2+4+8+32,  1+2+4+8+16+32,  1+2+4+8+16+32+64,1+2+4+8+16+32+128,	
	2, 2+4,  2+4+1,  2+4+1+8,  2+4+1+8+32,  2+4+1+8+16+32,  1+2+4+8+16+32+64,1+2+4+8+16+32+128,
	2, 2+4,  2+4+1,  2+4+1+16, 2+4+1+16+8,  2+4+1+16+8+32,  1+2+4+8+16+32+64,1+2+4+8+16+32+128
};

#define MaskSetRows 5
extern bool EditMapMode;
bool TestEnemyNearPoint(int x,int y,byte NMask);
void ProcessWalls(){	
	OWS.FillTempFields();
	bool ReLock=0;
	int lx0=10000;
	int lx1=0;
	int ly0=10000;
	int ly1=0;
	int T=GetTickCount();
	for(int i=0;i<OWS.Lines.GetAmount();i++){
        OneWallLine* OWL=OWS.Lines[i];
		if(OWL){
			bool del=1;
			if(OWL->OwnerObjectID!=0xFFFF){
				OneObject* OB=Group[OWL->OwnerObjectID];
				WallTypeDescription* WT=WList.Walls[OWL->WallType];
				if(OB&&OB->Serial==OWL->OwnerObjectSN){
					if(OB->Stage<OB->NStages){
						OWL->LastBuildTime=T;
						if(EditMapMode&&GetKeyState(VK_CONTROL)&0x8000)OB->NextStage();
						if((!WT->DontUseBastion)&&OB->Stage<10&&(!OB->newMons->Immortal)&&TestEnemyNearPoint(OB->RealX/16,OB->RealY/16,OB->NMask)){
                            OB->Die();
						}
					}
					OneObject* FIN=NULL;
					OneObject* FOU=NULL;
					if(OWL->Start)FIN=OWL->Start->GetObj();
					if(OWL->Final)FOU=OWL->Final->GetObj();					
					if(!WT->DontUseBastion){
						if((FIN==NULL||(FIN&&FIN->Stage==FIN->NStages))&&(FOU==NULL||(FOU&&FOU->Stage==FOU->NStages))){
							if(FIN){
								OB->Life=min(FIN->Life,OB->Life);
								if(FIN->Sdoxlo&&!OB->Sdoxlo)OB->Sdoxlo=1;
							}
							if(FOU){
								OB->Life=min(FOU->Life,OB->Life);
								if(FOU->Sdoxlo&&!OB->Sdoxlo)OB->Sdoxlo=1;
							}
						}
					}else{
						if(OB->NStages==OB->Stage){
							if(FIN&&FIN->Stage<FIN->NStages){
								for(int i=0;i<FIN->NStages;i++)FIN->NextStage();
							}
							if(FOU&&FOU->Stage<FOU->NStages){
								for(int i=0;i<FOU->NStages;i++)FOU->NextStage();
							}
						}
					}
					del=0;
					int LP=OB->Life*1024/OB->MaxLife;
					if(OB->Stage<OB->NStages)LP=1024;
					int N=OWL->Points.GetAmount();
					LP=LP*3/2;
					for(int j=0;j<N;j++){
						OneWallPoint* OWP=OWL->Points[j];
						if(OWP){
							int NS=OWP->Type->DestructionParts.GetAmount();
							if(NS){								
								int LP1=LP*(30+10*(j+j-N)*(j+j-N)/N/N)/30;
								if(LP1>1024)LP1=1024;
								int Stage=NS-NS*LP1/1024;							
								int st=Stage<32?Stage:31;
								DWORD ODD=OWP->DestructionMask;
								DWORD NDD;								
								NDD=0xFFFFFFFF-(0xFFFFFF>>(24-st));
								OWP->DestructionMask=NDD;
								for(int i=0;i<NS&&i<32;i++){
									DWORD M=1<<i;
									if((ODD&M)&&!(NDD&M)){
										OWP->CrashTime[i]=GetTickCount()+(rand()%2000);
										if(i==31){
											for(int q=32;q<NS;q++){
												OWP->CrashTime[q]=GetTickCount()+(rand()%2000);
											}
										}
									}
									if(!(ODD&M)&&(NDD&M)){
										OWP->CrashTime[i]=0;
									}
								}
							}
						}
					}				
				}else OB=NULL;
				int N=OWL->Points.GetAmount();
				for(int j=0;j<N;j++){
                    OneWallPoint* OWP=OWL->Points[j];
					if(OWP){
						if((!OB)||OB->Sdoxlo){
							if(OWP->Visible){
								void EraseExtraHeightObject(int x,int y);
								EraseExtraHeightObject(OWP->x,OWP->y);
								int tx=OWP->x/64;
								int ty=OWP->y/64;
                                if(tx<lx0)lx0=tx;
								if(tx>lx1)lx1=tx;
								if(ty<ly0)ly0=ty;
								if(ty>ly1)ly1=ty;
								ReLock=true;
								del=1;
								OWL->Dead=1;
								if(!OB)OWL->OwnerObjectID=0xFFFF;
							}
							OWP->Visible=0;
						}													
					}
				}				
			}			
		}
	}
	for(int i=0;i<OWS.Edges.GetAmount();i++){
		OneWallEdge* OWL=OWS.Edges[i];
		if(OWL){
			bool del=1;
			if(OWL->OwnerObjectID!=0xFFFF){
				OneObject* OB=Group[OWL->OwnerObjectID];				
				if(OB&&OB->Serial==OWL->OwnerObjectSN){
					WallTypeDescription* WT=WList.Walls[OWL->WallType];
					if(OB->Stage<OB->NStages){
						OWL->LastBuildTime=T;
						if(EditMapMode&&GetKeyState(VK_CONTROL)&0x8000)OB->NextStage();
						if((!WT->DontUseBastion)&&OB->Stage<10&&TestEnemyNearPoint(OB->RealX/16,OB->RealY/16,OB->NMask)){
							OB->Die();
						}
					}					
					bool stabil=OWL->TowerType==0||WT->DontUseBastion||
						((OWL->In&&OWL->In->GetObj()&&OWL->In->GetObj()->Sdoxlo==0)
						&&(OWL->Out&&OWL->Out->GetObj()&&OWL->Out->GetObj()->Sdoxlo==0));
					if(!stabil){
						if(OB->Life>20){
                            if((rando()&511)<GameSpeed)OB->Life-=20;
						}else{
							if(!OB->Sdoxlo)OB->Die();
						}
					}
					del=0;
					int LP=OB->Life*1024/OB->MaxLife;					
					if(OB->Stage<OB->NStages)LP=1024;
					int N=OWL->Points.GetAmount();
					for(int j=0;j<N;j++){
						OneWallPoint* OWP=OWL->Points[j];
						if(OWP&&OWP->Type){
							if(!OB->Sdoxlo){								
								if(OB->newMons->UseLikeGate){
                                    bool CheckCrossGate(int x,int y, int Radius, byte GateDir,byte Mask);
									if(OWL->NeedToOpen=CheckCrossGate(
										(OB->RealX>>4)-(OWP->Type->OpenCenterDistance*TCos[OB->RealDir])/256,
										(OB->RealY>>4)-(OWP->Type->OpenCenterDistance*TSin[OB->RealDir])/256,
										300,OB->RealDir+OWP->Type->Rotation,OB->NMask)){

                                        OWL->AnimationStage+=GameSpeed;
									}else{
										OWL->AnimationStage-=GameSpeed;
									}
									if(OWL->AnimationStage<0)OWL->AnimationStage=0;
									if(OWL->AnimationStage>256*25*2)OWL->AnimationStage=256*25*2;
								}
							}
							int NS=OWP->Type->DestructionParts.GetAmount();
							if(NS){
								int LP1=OB->Life*2048/OB->MaxLife;
								if(OB->Stage<OB->NStages)LP1=1024;
								if(LP1>1024)LP1=1024;
								int Stage=NS-NS*LP1/1024;							
								DWORD ODD=OWP->DestructionMask;
								DWORD NDD;
								if(Stage>28)NDD=0;
								else NDD=0xFFFFFFFF-(0xFFFFFFF>>(28-Stage));
								OWP->DestructionMask=NDD;
								for(int i=0;i<NS;i++){
									DWORD M=1<<i;
									if((ODD&M)&&!(NDD&M)){
										OWP->CrashTime[i]=GetTickCount()+(rand()%3000);
									}
									if(!(ODD&M)&&(NDD&M)){
										OWP->CrashTime[i]=0;
									}
								}
							}
						}
					}					
					//checking go away conditions
					void ProduceObjLink(OneObject* OB);
					if(LP<512||(OB->LocalOrder&&OB->LocalOrder->DoLink==&ProduceObjLink)){
						void LeaveMineLink(OneObject*);
						if(!(OB->LocalOrder&&OB->LocalOrder->DoLink==&LeaveMineLink)){
							word smType=0xFFFF;
							bool doSmt=false;
							for(int i=0;i<OB->NInside;i++){
								word ID=OB->Inside[i];							
								if(ID!=0xFFFF){
									OneObject* OIN=Group[ID];
									if(OIN){
										if(OIN->LocalOrder&&!OIN->Attack){
											doSmt=true;
											break;
										}
										smType=OIN->NIndex;
									}								
								}
							}						
							if(smType!=0xFFFF&&!doSmt){
								OB->LeaveMine(smType);
							}
						}
					}
				}else OB=NULL;
				int N=OWL->Points.GetAmount();
				for(int j=0;j<N;j++){
					OneWallPoint* OWP=OWL->Points[j];
					if(OWP){
						if((!OB)||OB->Sdoxlo){
							if(OWP->Visible){
								void EraseExtraHeightObject(int x,int y);
								EraseExtraHeightObject(OWP->x,OWP->y);
								int tx=OWP->x/64;
								int ty=OWP->y/64;
								if(tx<lx0)lx0=tx;
								if(tx>lx1)lx1=tx;
								if(ty<ly0)ly0=ty;
								if(ty>ly1)ly1=ty;
								ReLock=true;
								del=1;
								OWL->Dead=1;
								if(!OB)OWL->OwnerObjectID=0xFFFF;
								if(OWP->Type&&OWP->Type->DestructedAnimationID&&!OWP->Type->DestructionParts.GetAmount()){
									OWP->CrashTime[0]=GetTickCount();
									OWP->DestructionMask=0xFFFFFFFE;
								}
							}
							OWP->Visible=0;
						}
					}
				}
			}
			/*if(del){
				if(del){
					OWS.Edges.DelElement(i);
					i--;
				}
			}*/
		}
	}
	if(ReLock){
		for(int i=0;i<NMFIELDS;i++){
			HashTable[i].ReCreateAreas(lx0-15,ly0-15,lx1+15,ly1+15);
		}
	}
}
void OpenAllGates(byte NI,bool State){
    for(int i=0;i<OWS.Edges.GetAmount();i++){
        OneWallEdge* OWE=OWS.Edges[i];
        if(OWE&&!OWE->Dead){
            if(OWE->OwnerObjectID!=0xFFFF){
                OneObject* OB=Group[OWE->OwnerObjectID];                
                if(OB&&OB->Serial==OWE->OwnerObjectSN&&OB->newMons->UseLikeGate&&(NATIONS[NI].NMask&OB->NMask)/*OB->NNUM==NI*/&&!OB->Sdoxlo){
                    //Edit by TheBlackHunter - for ally
                    int dx=TCos[OB->RealDir];
                    int dy=TSin[OB->RealDir];
                    for(int q=-4;q<4;q++){
                        int xx=(OB->RealX+dx*q)>>8;
                        int yy=(OB->RealY+dy*q)>>8;
                        if(State)BClrBar(xx-2,yy-2,5);
                        else BSetBar(xx-2,yy-2,5);
                    }
                }
            }
        }
    }
}
void CreateNationDependentLinksForGates(){
	for(int i=0;i<OWS.Edges.GetAmount();i++){
		OneWallEdge* OWE=OWS.Edges[i];
		if(OWE){
			if(OWE->OwnerObjectID!=0xFFFF){
				OneObject* OB=Group[OWE->OwnerObjectID];				
				if(OB&&OB->Serial==OWE->OwnerObjectSN&&OB->newMons->UseLikeGate){
					int dx=TCos[OB->RealDir];
					int dy=TSin[OB->RealDir];
					for(int q=-8;q<8;q++){
						int x0=(OB->RealX+dx*8)>>10;
						int y0=(OB->RealY+dy*8)>>10;
						int x1=(OB->RealX-dx*8)>>10;
						int y1=(OB->RealY-dy*8)>>10;
						for(int j=0;j<NMFIELDS;j++)if(j!=1){
							int N=GetNAreas(j);
							int T0=GetTopRef(x0+(y0<<TopSH),j);
							int T1=GetTopRef(x1+(y1<<TopSH),j);
							if(T0!=T1&&T0<N&&T1<N){
                                HashTable[j].AddLink(T0,T1,OB->NMask);
								HashTable[j].AddLink(T1,T0,OB->NMask);
							}                            
						}
					}
				}
			}
		}
	}    
}
typedef bool tpUnitsCallback(OneObject* OB,void* param);
int PerformActionOverUnitsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);

bool CheckEnemNearLine(OneObject* OB,void* param)
{
	bool rez=false;
	if(OB && OB->NNUM!=7 && !OB->Sdoxlo)
	{
		int* pr=(int*)param;
		byte nmask=NATIONS[pr[5]].NMask;
		if(!(OB->NMask&nmask))
		{
			int xn,yn,Weight;
			int ds=GetPointToLineDistEx(OB->RealX>>4,OB->RealY>>4,pr[0],pr[1],pr[2],pr[3],xn,yn,Weight);
			if(ds<pr[4])
			{
				rez=true;
			}
		}
	}
	return rez;
}
// true - если на расстоянии Radius от отрезка x0 y0, x1 y1 есть вражеские юниты
bool IsEnemyNearWall(int x0,int y0, int x1, int y1, int Radius, byte NI)
{
	int ds=Norma(x0-x1,y0-y1);
	int pr[6];
	pr[0]=x0;
	pr[1]=y0;
	pr[2]=x1;
	pr[3]=y1;
	pr[4]=Radius;
	pr[5]=(int)NI;
	int rez=PerformActionOverUnitsInRadius(x0,y0,ds/2,&CheckEnemNearLine,(void*)pr);
	rez+=PerformActionOverUnitsInRadius(x1,y1,ds/2,&CheckEnemNearLine,(void*)pr);
	return rez>0;
}
bool TestDoubleWall(int x0,int y0, int x1, int y1){
    OneWallEdge* S=OWS.GetNearEdge(x0,y0,SnapToEdgeDist);
	OneWallEdge* F=OWS.GetNearEdge(x1,y1,SnapToEdgeDist);
	if(S&&F){
		if(S->In&&F->Out&&S->In->GetObj()&&S->In->GetObj()->Sdoxlo==0&&F->Out->GetObj()&&F->Out->GetObj()->Sdoxlo==0&&
			(S->In->Start==F->Out->Final
			||S->In->Final==F->Out->Start)){
                return true;
			}
		if(S->Out&&F->In&&S->Out->GetObj()&&S->Out->GetObj()->Sdoxlo==0&&F->In->GetObj()&&F->In->GetObj()->Sdoxlo==0&&
			(S->Out->Start==F->In->Final
			||S->Out->Final==F->In->Start)){
				return true;
			}

	}
	return false;
}
// true - если на расстоянии Radius от отрезка x0 y0, x1 y1 есть здания
bool IsBuildingNearWall(int x0,int y0, int x1, int y1, int Radius)
{
	bool rez=false;
	int cx=(x0+x1)/2;
	int cy=(y0+y1)/2;
	int ccx=(cx>>7)-8;
	int ccy=(cy>>7)-8;
	word MID=0xFFFF;
	for(int i=0;i<16;i++)
	{
		for(int j=0;j<16;j++)
		{
			int xx=ccx+i;
			int yy=ccy+j;
			if(xx>=0&&yy>=0&&xx<VAL_MAXCX-1&&yy<VAL_MAXCX-1)
			{
				int cell=xx+(yy<<VAL_SHFCX);
				word GetOneBld(int cell,int pos=0);
				MID=GetOneBld(cell);
				if(MID!=0xFFFF)
				{
					OneObject* OB=Group[MID];
					if(OB&&OB->newMons->BuildX0!=0)
					{
						int bx=OB->RealX>>4;
						int by=OB->RealY>>4;
						int d=Norma(bx-cx,by-cy);
						if(d<1200)
						{
							int bx0=bx+OB->newMons->BuildX0;
							int by0=by+OB->newMons->BuildY0;
							int bx1=bx+OB->newMons->BuildX1;
							int by1=by+OB->newMons->BuildY1;
							int bx2=(bx0+bx1+by0-by1)/2;
							int by2=(by0+by1+bx0-bx1)/2;
							int bx3=(bx0+bx1+by1-by0)/2;
							int by3=(by0+by1+bx1-bx0)/2;
							int xn,yn,Weight;

							if(GetPointToLineDistEx(bx,by,x0,y0,x1,y1,xn,yn,Weight)<Radius*2)return true;

							if(GetPointToLineDistEx(x0,y0,bx0,by0,bx2,by2,xn,yn,Weight)<Radius)return true;
							if(GetPointToLineDistEx(x0,y0,bx2,by2,bx1,by1,xn,yn,Weight)<Radius)return true;
							if(GetPointToLineDistEx(x0,y0,bx1,by1,bx3,by3,xn,yn,Weight)<Radius)return true;
							if(GetPointToLineDistEx(x0,y0,bx3,by3,bx0,by0,xn,yn,Weight)<Radius)return true;

							if(GetPointToLineDistEx(x1,y1,bx0,by0,bx2,by2,xn,yn,Weight)<Radius)return true;
							if(GetPointToLineDistEx(x1,y1,bx2,by2,bx1,by1,xn,yn,Weight)<Radius)return true;
							if(GetPointToLineDistEx(x1,y1,bx1,by1,bx3,by3,xn,yn,Weight)<Radius)return true;
							if(GetPointToLineDistEx(x1,y1,bx3,by3,bx0,by0,xn,yn,Weight)<Radius)return true;

							if(GetPointToLineDistEx(bx0,by0,x0,y0,x1,y1,xn,yn,Weight)<Radius)return true;
							if(GetPointToLineDistEx(bx1,by1,x0,y0,x1,y1,xn,yn,Weight)<Radius)return true;
							if(GetPointToLineDistEx(bx2,by2,x0,y0,x1,y1,xn,yn,Weight)<Radius)return true;
							if(GetPointToLineDistEx(bx3,by3,x0,y0,x1,y1,xn,yn,Weight)<Radius)return true;
						}
					}
				}
			}
		}
	}
	return rez;
}
bool UnitCheckCrossGate(OneObject* OB, void* param)
{
	bool rez=false;
	int* pr=(int*)param;
	byte nmask=(byte)pr[3];
	if(OB&&(!OB->Sdoxlo)&&OB->NMask&nmask&&OB->DestX!=-1)
	{
		//byte MDir=GetDir(OB->DestY-OB->RealX,OB->DestY-OB->RealY);
		byte GateDir=(byte)pr[2];
		byte Alf=GateDir;
		int f1=((((OB->RealX>>4)-pr[0])*TCos[Alf])>>8)+((((OB->RealY>>4)-pr[1])*TSin[Alf])>>8);
		int f2=((((OB->DestX>>4)-pr[0])*TCos[Alf])>>8)+((((OB->DestY>>4)-pr[1])*TSin[Alf])>>8);
		if(f1*f2<0 || Norma((OB->RealX>>4)-pr[0],(OB->RealY>>4)-pr[1])<80)rez=true;
			//rez=true;
		/*
		char dlt=abs(((char)MDir)-((char)GateDir));
		if(dlt<32)
		{
			rez=true;
		}
		*/
	}
	return rez;
}
// true - если есть юнит в Radius и он идет через точку  x, y
bool CheckCrossGate(int x,int y, int Radius, byte GateDir,byte Mask)
{
	int pr[4];
	pr[0]=x;
	pr[1]=y;
	pr[2]=GateDir;
	pr[3]=Mask;
	return PerformActionOverUnitsInRadius(x,y,Radius,&UnitCheckCrossGate,(void*)pr)>0;
}
class WallXML_Saver:public MapXmlSaver{
	virtual bool GetXmlToSaveInMap(xmlQuote& xml){
		OWS.Save(xml,&OWS);
		return true;
	}
	virtual bool GetXmlToSaveInSaveFile(xmlQuote& xml){
		return false;
	}

	virtual void SetXmlFromMap(xmlQuote& xml){
		ErrorPager ep(0);
		OWS.Load(xml,&OWS,&ep);
		for(int i=0;i<OWS.Edges.GetAmount();i++)if(OWS.Edges[i]){
            OWS.Edges[i]->SpecifiedObjectType=0xFFFF;
		}
		for(int i=0;i<OWS.Lines.GetAmount();i++)if(OWS.Lines[i]){
			OWS.Lines[i]->SpecifiedObjectType=0xFFFF;
		}
		OWS.ReCreate();
	}
	virtual void SetXmlFromSaveFile(xmlQuote& xml){
	}

	virtual void ResetMap(){
		OWS.reset_class(&OWS);
	};

	virtual DWORD GetSaverID(){return 'LLAW';};
};
void RegisterWallsSaver(){
	REG_XML_SAVER(WallXML_Saver);
}
void SetGlobalClipPlane(Vector3D& pos,Vector3D& norm){
	pos=Vector3D(pos.x,pos.y,pos.z);
	//norm=SkewPt(norm.x,norm.y,norm.z);
    Plane P(pos,norm);
	IRS->SetClipPlane(1,P);
}
OneObject* GetNewObjectToBuild(OneObject* PrevObject){
	int x=PrevObject->RealX;
	int y=PrevObject->RealY;
	int rm=100000;
	OneObject* BOB=NULL;
	for(int i=0;i<OWS.Edges.GetAmount();i++)if(OWS.Edges[i]){
        OneObject* OB=OWS.Edges[i]->GetObj();
		if(OB&&OB->Stage<OB->NStages){
			int ObjX=0;
			int ObjY=0;
			if(OB->FindPoint(&ObjX,&ObjY,FP_NEAREST_POINT|FP_FIND_WORKPOINT|FP_UNLOCKED_POINT)){
                int R=Norma(OB->RealX-x,OB->RealY-y);
				if(R<rm){
					rm=R;
					BOB=OB;
				}
			}
		}
	}
	for(int i=0;i<OWS.Lines.GetAmount();i++)if(OWS.Lines[i]){
		OneObject* OB=OWS.Lines[i]->GetObj();
		if(OB&&OB->Stage<OB->NStages){
			int ObjX=0;
			int ObjY=0;
			if(OB->FindPoint(&ObjX,&ObjY,FP_NEAREST_POINT|FP_FIND_WORKPOINT|FP_UNLOCKED_POINT)){
				int R=Norma(OB->RealX-x,OB->RealY-y);
				if(R<rm){
					rm=R;
					BOB=OB;
				}
			}
		}
	}
	return BOB;
}
void KillWallObject(OneObject* OB){
	
}
struct UGR_BridgesStruct{
	byte NI;
	int WallType;
	int CurBridgeStartX;
	int CurBridgeStartY;
	int CurBridgeFinalX;
	int CurBridgeFinalY;
	int CurBridgeNSegm;
	int CurBridgeNReady;
	int NAlreadyBuilt;
	int BuiltBStartX[64];
	int BuiltBStartY[64];
	int BuiltBFinalX[64];
	int BuiltBFinalY[64];
	word CurBridge_ids[64];
	word CurBridge_sns[64];
};
extern word NPlayers;
extern word* TopIslands;
void UGR_BuildBridgesLink(UnitsGroup* UGR){
	if(!NPlayers)return;
	UGR_BridgesStruct* UBS=(UGR_BridgesStruct*)UGR->GOrder->Data;
	if(UBS->NAlreadyBuilt>4){
		UGR->DeleteLastOrder();
		return;
	}
	if(UBS->CurBridgeStartX==-1){
        int curisl=CITY[UBS->NI].MyIsland;
		bool done=false;
		int na=0;
		DWORD Mask=NATIONS[UBS->NI].NMask;
		do{
            int p=rando()%7;
			byte ni=GSets.CGame.PL_INFO[p].ColorID;
			if(!(NATIONS[ni].NMask&Mask)){
				if(curisl!=CITY[ni].MyIsland){
					int enisl=CITY[ni].MyIsland;//island of enemy
                    int Zones1[2048];//array of my zones
					int Zones2[2048];//array of enemy zones
					int NZones1=0;
					int NZones2=0;
					//searching for island of my enemy
					for(int i=0;i<GetNAreas(0);i++){
						int id=TopIslands[i];
						if(id==curisl&&NZones1<2048){
                            Zones1[NZones1]=i;
							NZones1++;
						}
						if(id==enisl&&NZones2<2048){
							Zones2[NZones2]=i;
							NZones2++;
						}
					}
					int nz1=0;
					for(int i=0;i<NZones1;i++){
						Area* AR=GetTopMap(Zones1[i]);
						int xz=AR->x;
						int yz=AR->y;
						int z=GetHeight(xz*64,yz*16);
						if(z<70){
							Zones1[nz1]=Zones1[i];
							nz1++;
						}
					}
					NZones1=nz1;
					nz1=0;
					for(int i=0;i<NZones2;i++){
						Area* AR=GetTopMap(Zones2[i]);
						int xz=AR->x;
						int yz=AR->y;
						int z=GetHeight(xz*64,yz*16);
						if(z<70){
							Zones2[nz1]=Zones2[i];
							nz1++;
						}
					}
					NZones2=nz1;
					//cant build near old bridges
					for(int q=0;q<UBS->NAlreadyBuilt;q++){
						int x0=UBS->BuiltBStartX[q];
						int y0=UBS->BuiltBStartY[q];
						int x1=UBS->BuiltBStartX[q];
						int y1=UBS->BuiltBStartY[q];
						int nz1=0;
						for(int i=0;i<NZones1;i++){
							Area* AR=GetTopMap(Zones1[i]);
							int xz=AR->x;
							int yz=AR->y;
							if(Norma(xz-x0,yz-y0)>40&&Norma(xz-x1,yz-y1)>40){
								Zones1[nz1]=Zones1[i];
								nz1++;
							}
						}
						NZones1=nz1;
						nz1=0;
						for(int i=0;i<NZones2;i++){
							Area* AR=GetTopMap(Zones2[i]);
							int xz=AR->x;
							int yz=AR->y;
							if(Norma(xz-x0,yz-y0)>40&&Norma(xz-x1,yz-y1)>40){
								Zones2[nz1]=Zones2[i];
								nz1++;
							}
						}
						NZones2=nz1;
					}
					if(NZones1&&NZones2){
						int minr=100000;
						int z1=-1;
						int z2=-1;
						for(int i=0;i<NZones1;i++){
							Area* AR=GetTopMap(Zones1[i]);
							int xz=AR->x;
							int yz=AR->y;
							for(int j=0;j<NZones2;j++){
								Area* AR=GetTopMap(Zones2[j]);
                                int R=Norma(xz-AR->x,yz-AR->y);
								if(R<minr){
									z1=Zones1[i];
									z2=Zones2[j];
									minr=R;
								}
							}
						}
						if(z1!=-1){
							if(minr<50){
								Area* AR1=GetTopMap(z1);
								Area* AR2=GetTopMap(z2);
								done=true;
								UBS->CurBridgeStartX=AR1->x;
								UBS->CurBridgeStartY=AR1->y;
								UBS->CurBridgeFinalX=AR2->x;
								UBS->CurBridgeFinalY=AR2->y;
								UBS->CurBridgeNReady=0;
								UBS->CurBridgeNSegm=Norma(AR1->x-AR2->x,AR1->y-AR2->y)/6+1;
								UBS->CurBridgeNReady=0;
								memset(&UBS->CurBridge_ids,0xFF,sizeof UBS->CurBridge_ids);
								memset(&UBS->CurBridge_sns,0xFF,sizeof UBS->CurBridge_sns);
							}else na+=20;
						}
					}
				}
			}
			na++;
		}while(na<50&&!done);
		if(!done){
			UGR->DeleteLastOrder();
			return;
		}
	}else{
        //start and final point found, now we are ready to build
        //1. searching for currend segment to build
		bool succ=false;
		OneObject* OB=NULL;
		word id=UBS->CurBridge_ids[UBS->CurBridgeNReady];
		word sn=UBS->CurBridge_sns[UBS->CurBridgeNReady];
		if(id!=0xFFFF){
            OB=Group[id];
			if(OB&&OB->Serial==sn){
				if(OB->Stage==OB->NStages){
					UBS->CurBridgeNReady++;
					if(UBS->CurBridgeNReady==UBS->CurBridgeNSegm){
						int q=UBS->NAlreadyBuilt;
						UBS->BuiltBStartX[q]=UBS->CurBridgeStartX;
						UBS->BuiltBStartY[q]=UBS->CurBridgeStartY;
						UBS->BuiltBStartX[q]=UBS->CurBridgeFinalX;
						UBS->BuiltBStartY[q]=UBS->CurBridgeFinalY;                        
						UBS->NAlreadyBuilt++;
						UBS->CurBridgeStartX=-1;//bridge is ready, time to build next bridge
					}
					return;
				}else{
                    succ=true;
				}
			}
		}
		if(!succ){
			//we must add one additional segment
			int x0=UBS->CurBridgeStartX*64;
			int y0=UBS->CurBridgeStartY*64;
			int x1=UBS->CurBridgeFinalX*64;
			int y1=UBS->CurBridgeFinalY*64;

            int cx0=x0+(x1-x0)*UBS->CurBridgeNReady/UBS->CurBridgeNSegm;
			int cy0=y0+(y1-y0)*UBS->CurBridgeNReady/UBS->CurBridgeNSegm;
			int cx1=x0+(x1-x0)*(UBS->CurBridgeNReady+1)/UBS->CurBridgeNSegm;
			int cy1=y0+(y1-y0)*(UBS->CurBridgeNReady+1)/UBS->CurBridgeNSegm;

			AddCheapWallSegment(UBS->NI,UBS->WallType,cx0,cy0,cx1,cy1);
			int xc=(cx0+cx1)/2;
			int yc=(cy0+cy1)/2;
			OneObject* DEST=NULL;
			for(int i=0;i<MAXOBJECT;i++){
				OneObject* OB=Group[i];
				if(OB&&OB->NewBuilding/*&&OB->Stage<5&&!OB->Sdoxlo*/){
					if(Norma(xc-OB->RealX/16,yc-OB->RealY/16)<64){
						DEST=OB;//object successfully found
						break;
					}
				}
			}
			if(DEST){
                UBS->CurBridge_ids[UBS->CurBridgeNReady]=DEST->Index;
				UBS->CurBridge_sns[UBS->CurBridgeNReady]=DEST->Serial;
				OB=DEST;
				succ=true;
			}else{
				DEST=NULL;
			}
		}
		if(OB&&succ){
			UGR->RefreshUnits();
			for(int i=0;i<UGR->N;i++){
                OneObject* OB=Group[UGR->IDS[i]];
				if(!OB->LocalOrder)OB->BuildObj(OB->Index,16+128,0,0);
			}
		}
	}
}
bool UseGroupLikeBridgeBuilders(UnitsGroup* ugrp,int BridgeType,byte NI){
	if(ugrp->GOrder&&ugrp->GOrder->Link==&UGR_BuildBridgesLink)return false;
    GrpOrder* GO=ugrp->CreateOrder(0);    
	GO->Link=&UGR_BuildBridgesLink;
	UGR_BridgesStruct* UBS=new UGR_BridgesStruct;
	GO->Data=UBS;
	GO->DataSize=sizeof UGR_BridgesStruct;
	UBS->WallType=BridgeType;
	UBS->CurBridgeStartX=-1;
	UBS->CurBridgeStartY=-1;
	UBS->CurBridgeFinalX=-1;
	UBS->CurBridgeFinalY=-1;
	UBS->NAlreadyBuilt=0;	
	UBS->NI=NI;	
	ugrp->GOrder->Link(ugrp);
	return ugrp->GOrder!=NULL;
}
class WallSaverExtension:public GameExtension{
public:
	virtual bool OnGameSaving(xmlQuote& xml){
		OWS.Save(xml,&OWS);
		return true;
	}
	bool OnGameLoading(xmlQuote& xml){		
		ErrorPager ep(0);
		OWS.Load(xml,&OWS,&ep);
		for(int i=0;i<OWS.Lines.GetAmount();i++){
			if(OWS.Lines[i])OWS.Lines[i]->Textured=false;
		}
		OWS.ReCreate();
		return true;
	}
};
void InstallWallsSaver(){
	InstallExtension(new WallSaverExtension,"WallsSaver");
}
