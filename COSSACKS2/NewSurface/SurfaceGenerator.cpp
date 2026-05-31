#include "SurfaceGenerator.h"
#include "ITerrain.h"
#include "..\WeaponSystem.h"
#include "..\UnitAbility.h"

extern UniversalMap g_UniversalMap;
extern SurfaceGenerator SURF;
void InitLayersSystem(){
	bool LoadMLayersFromFileInCompactForm(char* filename);
	LoadMLayersFromFileInCompactForm("Surface\\SurfaceShape.dat");
	SURF.reset_class(&SURF);
	SURF.SafeReadFromFile("Surface\\Surface3D.xml");
}

void GetTexString(char* dest,ClonesArray<OneTexturePair>& tex){
	int N=tex.GetAmount();
	for(int i=0;i<N;i++){
		DWORD C1,C2;
		int GetFacture(int);
		int tn=GetFacture(tex[i]->Facture);
		DWORD C=tex[i]->Color;
		C=MulDWORD(C,512);
		C1=C|0xFF000000;
		C2=C&0xFFFFFF;
		if(i==0){
			if(N==1)C2=C1;
			sprintf(dest,"{tid %d 0 0 64 32  0 0 64 32  %X %X %X %X}",tn,C1,C2,C1,C2);
			dest+=strlen(dest);			
		}else
			if(i==N-1){			
				sprintf(dest,"{tid %d -64 0 64 32  0 0 64 32  %X %X %X %X 0}",tn,C2,C1,C2,C1);
				dest+=strlen(dest);			
			}else{
				sprintf(dest,"{tid %d -64 0 64 32  0 0 64 32  %X %X %X %X 0}",tn,C2,C1,C2,C1);
				dest+=strlen(dest);
				sprintf(dest,"{tid %d 0 0 64 32  0 0 64 32  %X %X %X %X}",tn,C1,C2,C1,C2);
				dest+=strlen(dest);
			}
	}
}

extern UniversalMap g_UniversalMap;

void RegisterSurfaceClasses(){
	REG_CLASS(SurfaceGenerator);
	REG_CLASS(OneTexturingStyle);
	REG_CLASS(OneSurfaceType);
	REG_CLASS(OneSurfaceRule);
	REG_CLASS(OneTexturingRule);
	REG_CLASS(OneTexturePair);
	REG_CLASS(OneGenCondition);
	REG_CLASS(OneTexturingCondition);
	REG_CLASS(CheckHeightAndAngle);
	REG_CLASS(CheckHeight);
	REG_CLASS(CheckVariatedHeightAndAngle);
	REG_CLASS(CheckSpecialLayer);
	REG_CLASS(CheckIfNotInLayer);
	REG_CLASS(CheckIfNearLayer);
	REG_CLASS(OneLayerColor);
	REG_CLASS(CheckSpecialSurfaceType);
}

void SaveNewSurface(ResFile F){
	//x-2 y-2 z-2 Color-3 Shadow-1 Facture-1 nx-1 ny-1 nz-1 stype-1 total: 15
	int sz=4+4+4+g_UniversalMap.Vert.GetAmount()*15+g_UniversalMap.Idxs.GetAmount()*4;
	int c='2HSM';
	RBlockWrite(F,&c,4);
	RBlockWrite(F,&sz,4);
	c=g_UniversalMap.Vert.GetAmount();
	RBlockWrite(F,&c,4);	
	c=g_UniversalMap.Idxs.GetAmount();
	RBlockWrite(F,&c,4);
	int N=g_UniversalMap.Vert.GetAmount();
	OneMapVertex* V=&g_UniversalMap.Vert[0];
	for(int i=0;i<N;i++,V++){		
		RBlockWrite(F,&V->x,2);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<N;i++,V++){		
		RBlockWrite(F,&V->y,2);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<N;i++,V++){		
		RBlockWrite(F,&V->z,2);
	}		
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<N;i++,V++){		
		RBlockWrite(F,&V->Color,1);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<N;i++,V++){		
		RBlockWrite(F,((byte*)(&V->Color))+1,1);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<N;i++,V++){		
		RBlockWrite(F,((byte*)(&V->Color))+2,1);
	}	
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<g_UniversalMap.Vert.GetAmount();i++,V++){		
		RBlockWrite(F,&V->Shadow,1);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<N;i++,V++){		
		RBlockWrite(F,&V->nFacture,1);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<N;i++,V++){		
		RBlockWrite(F,&V->nx,1);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<N;i++,V++){		
		RBlockWrite(F,&V->ny,1);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<N;i++,V++){		
		RBlockWrite(F,&V->nz,1);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<N;i++,V++){		
		RBlockWrite(F,&V->SurfaceType,1);
	}		
	RBlockWrite(F,&g_UniversalMap.Idxs[0],g_UniversalMap.Idxs.GetAmount()*4);
}
void CreateHeightmap(){
	int N=VertInLine*MaxTH;
	memset(THMap,0xFF,N*2);
	VertexBasedMeshRef MR;
	g_UniversalMap.GetQuadsRefAll(MR);
	MR.FillMapOfHeights();
/*	for(int i=1;i<N-1;i++){
		if(THMap[i]==-1){
			int H=0;
			int N2=0;
			if(THMap[i-1]!=-1){
				H+=int(THMap[i-1]);
				N2++;
			}
			if(THMap[i+1]!=-1){
				H+=int(THMap[i+1]);
				N2++;
			}
			if(N2)H/=N2;
			if(N2)THMap[i]=H;
			else THMap[i]=1;
		}
	}*/
	int NL=MLayers->Maps.GetAmount();
	for(int ix=1;ix<MaxTH;ix++){
		for(int iy=1;iy<MaxTH-1;iy++){
			int v=ix+iy*VertInLine;
			int x=ix<<5;
			int y=(iy<<5)-(ix&1)*16;
			int H=THMap[v];
			if(H<0){
				for(int i=0;i<NL;i++)if(LAYERSLIST.Layers[i]->DontGenerateWater){
					if(MLayers->Maps[i]->GetGreyPrecise(x/15360.0,y/15360.0)>0){
						THMap[v]=10;
					}
				}			
			}
		}    
	}    
}
void SelectSurfaceType(bool New);
void LoadNewSurface(ResFile F){
	SelectSurfaceType(true);
	int Nv=0;
	int Ni=0;
	RBlockRead(F,&Nv,4);
	RBlockRead(F,&Ni,4);
	g_UniversalMap.Release();
	g_UniversalMap.Idxs.Add(0,Ni);
	OneMapVertex V;
	g_UniversalMap.Vert.Add(V,Nv);
	for(int i=0;i<g_UniversalMap.Vert.GetAmount();i++){
		OneMapVertex* V=&g_UniversalMap.Vert[i];
		RBlockRead(F,&V->x,2);
		RBlockRead(F,&V->y,2);
		RBlockRead(F,&V->z,2);
		RBlockRead(F,&V->Color,4);
		RBlockRead(F,&V->Shadow,1);
		RBlockRead(F,&V->nFacture,1);
		RBlockRead(F,&V->nx,1);
		RBlockRead(F,&V->ny,1);
		RBlockRead(F,&V->nz,1);
		V->SurfaceType=0;
		int c;
		RBlockRead(F,&c,1);
	}
	RBlockRead(F,&g_UniversalMap.Idxs[0],g_UniversalMap.Idxs.GetAmount()*4);
	//g_UniversalMap.InitUniversalMapQuad(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	g_UniversalMap.QuadsSystem_Init();
//	g_UniversalMap.FillMapOfHeights();
	CreateHeightmap();
}
void LoadNewSurface2(ResFile F){
	SelectSurfaceType(true);
	int Nv=0;
	int Ni=0;
	RBlockRead(F,&Nv,4);
	RBlockRead(F,&Ni,4);
	g_UniversalMap.Release();
	g_UniversalMap.Idxs.Add(0,Ni);
	OneMapVertex VT;
	g_UniversalMap.Vert.Add(VT,Nv);

	OneMapVertex* V=&g_UniversalMap.Vert[0];
	for(int i=0;i<Nv;i++,V++){		
		RBlockRead(F,&V->x,2);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<Nv;i++,V++){		
		RBlockRead(F,&V->y,2);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<Nv;i++,V++){		
		RBlockRead(F,&V->z,2);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<Nv;i++,V++){		
		RBlockRead(F,&V->Color,1);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<Nv;i++,V++){		
		RBlockRead(F,((byte*)&V->Color)+1,1);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<Nv;i++,V++){		
		RBlockRead(F,((byte*)&V->Color)+2,1);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<Nv;i++,V++){		
		RBlockRead(F,&V->Shadow,1);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<Nv;i++,V++){		
		RBlockRead(F,&V->nFacture,1);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<Nv;i++,V++){		
		RBlockRead(F,&V->nx,1);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<Nv;i++,V++){		
		RBlockRead(F,&V->ny,1);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<Nv;i++,V++){		
		RBlockRead(F,&V->nz,1);
	}
	V=&g_UniversalMap.Vert[0];
	for(int i=0;i<Nv;i++,V++){		
		RBlockRead(F,&V->SurfaceType,1);
	}
	RBlockRead(F,&g_UniversalMap.Idxs[0],g_UniversalMap.Idxs.GetAmount()*4);
	//g_UniversalMap.InitUniversalMapQuad(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	g_UniversalMap.QuadsSystem_Init();
	//	g_UniversalMap.FillMapOfHeights();
	CreateHeightmap();
}

LayersList LAYERSLIST;
void TestLayersColors(){
	LAYERSLIST.reset_class(&LAYERSLIST);
	LAYERSLIST.SafeReadFromFile("Surface\\Layers.xml");
	if(LAYERSLIST.Layers.GetAmount()&&MLayers){
		for(int i=0;i<MLayers->Maps.GetAmount();i++){
			if(i<LAYERSLIST.Layers.GetAmount()){
				((GreyMap*)MLayers->Maps[i])->Color=LAYERSLIST.Layers[i]->Color;
				*MLayers->MapsNames[i]=LAYERSLIST.Layers[i]->Name;
			}
		}
		for(int i=MLayers->Maps.GetAmount();i<LAYERSLIST.Layers.GetAmount();i++){
			MLayers->Maps.Add(new GreyMap(LAYERSLIST.Layers[i]->Color));
			_str* s=new _str;
			*s=LAYERSLIST.Layers[i]->Name;
			MLayers->MapsNames.Add(s);
		}
		Enumerator* E1=ENUM.Get("LAYERSLIST");
		E1->Clear();
		for(int i=0;i<MLayers->Maps.GetAmount();i++){
			char* s=MLayers->MapsNames[i]->str;
			E1->Add(s?s:"???");
		}
	}
}
void AddLayersEditor(){
	AddStdEditor("MapLayersEditor",&LAYERSLIST,"Surface\\Layers.xml",RCE_DEFAULT);
}

float CheckIfNearLayer::GetConditionWeight(PrecompiledVertex* V){	
	int x=V->Vertex->x;
	int y=V->Vertex->y;
	float w=DistField::Get(Layer,float(x)*surfScale,float(y)*surfScale);
	int D=fabs(w*16);	
	if(StartDistance!=FinalDistance){
        float ww=(FinalDistance-D)*255/(FinalDistance-StartDistance);
		if(ww>255)ww=255;
		if(ww<0)ww=0;
		return ww/255.0f;
	}	
	return 0.0f;
}

SurfaceGenerator SURF;

float CheckIfNotInLayer::GetConditionWeight(PrecompiledVertex* V){
	BasePaintMap* BPM=MLayers->Maps[Layer];
	int x=V->Vertex->x;
	int y=V->Vertex->y;
	SURF.ShapeOfBound.ShiftXY(x,y);
	return (255-BPM->GetGreyPrecise(x*surfScale,y*surfScale))/255.0f;
}

float CheckSpecialLayer::GetConditionWeight(PrecompiledVertex* V){
	BasePaintMap* BPM=MLayers->Maps[Layer];
	int x=V->Vertex->x;
	int y=V->Vertex->y;
	SURF.ShapeOfBound.ShiftXY(x,y);

	float v1=BPM->GetGreyPrecise((x-80)*surfScale,y*surfScale);
	float v2=BPM->GetGreyPrecise((x+80)*surfScale,y*surfScale);
	float v3=BPM->GetGreyPrecise(x*surfScale,(y+80)*surfScale);
	float v4=BPM->GetGreyPrecise(x*surfScale,(y-80)*surfScale);

	return (v1+v2+v3+v4)/4/255.0f;
}
float CheckSpecialSurfaceType::GetConditionWeight(PrecompiledVertex* V){
	return float(V->Vertex->SurfaceType==SurfaceType);
}


float CheckVariatedHeightAndAngle::GetConditionWeight(PrecompiledVertex* V){
	float af,hf;
	int DH=VariatedAddHeight*VariationLow.GetFValue(V->Vertex->x,V->Vertex->y);
	int H0=StartHeight+DH;
	int H1=FinalHeight+DH;
	if(V->Height>=H0&&V->Height<=H1)hf=1.0f;
	else{
		if(HeightDiff){
			if(V->Height<H1&&V->Height>H0-HeightDiff){
				hf=1-float(H0-V->Height)/HeightDiff;
			}else
				if(V->Height>H1&&V->Height<H1+HeightDiff){
					hf=1-float(V->Height-H1)/HeightDiff;
				}else hf=0.0f;
		}else hf=0.0f;
	}
	if(V->Angle>=StartAngle&&V->Angle<=FinalAngle)af=1.0f;
	else{
		if(AngleDiff){
			if(V->Angle<StartAngle&&V->Angle>StartAngle-AngleDiff){
				af=1-float(StartAngle-V->Angle)/AngleDiff;
			}else
				if(V->Angle>FinalAngle&&V->Angle<FinalAngle+AngleDiff){
					af=1-float(V->Angle-FinalAngle)/AngleDiff;
				}else af=0.0f;
		}else af=0.0f;
	}
	return hf*af;
}

float CheckHeight::GetConditionWeight(PrecompiledVertex* V){
	float hf;
	if(V->Height>=StartHeight&&V->Height<=FinalHeight)hf=1.0f;
	else{
		if(HeightDiff){
			if(V->Height<StartHeight&&V->Height>StartHeight-HeightDiff){
				hf=1-float(StartHeight-V->Height)/HeightDiff;
			}else
				if(V->Height>FinalHeight&&V->Height<FinalHeight+HeightDiff){
					hf=1-float(V->Height-FinalHeight)/HeightDiff;
				}else hf=0.0f;
		}else hf=0.0f;
	}
	return hf;
}

float CheckHeightAndAngle::GetConditionWeight(PrecompiledVertex* V){
	float af,hf;
	if(V->Height>=StartHeight&&V->Height<=FinalHeight)hf=1.0f;
	else{
		if(HeightDiff){
			if(V->Height<StartHeight&&V->Height>StartHeight-HeightDiff){
				hf=1-float(StartHeight-V->Height)/HeightDiff;
			}else
				if(V->Height>FinalHeight&&V->Height<FinalHeight+HeightDiff){
					hf=1-float(V->Height-FinalHeight)/HeightDiff;
				}else hf=0.0f;
		}else hf=0.0f;
	}
	if(V->Angle>=StartAngle&&V->Angle<=FinalAngle)af=1.0f;
	else{
		if(AngleDiff){
			if(V->Angle<StartAngle&&V->Angle>StartAngle-AngleDiff){
				af=1-float(StartAngle-V->Angle)/AngleDiff;
			}else
				if(V->Angle>FinalAngle&&V->Angle<FinalAngle+AngleDiff){
					af=1-float(V->Angle-FinalAngle)/AngleDiff;
				}else af=0.0f;
		}else af=0.0f;
	}
	return hf*af;
}

float DistField::Get(int Index,float x,float y){
	if(!map[Index]){
		map[Index]=new SquareMap<float>;
		SquareMap<float>* FL=map[Index];
		FL->SetSize(512,512,0);
		BasePaintMap* BPM=MLayers->Maps[Index];	
		for(int ix=0;ix<512;ix++){
			for(int iy=0;iy<512;iy++){
				float v=BPM->GetGreyPrecise(float(ix)/512.0f,float(iy)/512.0f);
				if(v<128)FL->Set(ix,iy,-10000);
				else FL->Set(ix,iy,10000);
			}
		}
		DynArray<DWORD> Edges;
		Edges.Check(4096);
		for(int ix=0;ix<512;ix++){
			for(int iy=0;iy<512;iy++){
				float v=FL->Get(ix,iy);
				float vL=FL->Get(ix-1,iy);
				float vR=FL->Get(ix+1,iy);
				float vU=FL->Get(ix,iy-1);
				float vD=FL->Get(ix,iy+1);
				bool add=false;
				if(v>5000){
					if(vL<-5000||vR<-5000||vU<-5000||vD<-5000)add=true;
				}else{
					if(vL>5000||vR>5000||vU>5000||vD>5000)add=true;
				}
				if(add){
					DWORD XY=ix+(iy<<16);
					Edges.Add(XY);
				}
			}
		}
		for(int i=0;i<Edges.GetAmount();i++){
			int x=Edges[i]&0xFFFF;
			int y=Edges[i]>>16;
			float v=FL->Get(x,y);
			if(v<0)FL->Set(x,y,-1);
			else FL->Set(x,y,1);
		}
		DynArray<DWORD> NextEdges;
		NextEdges.Check(4096);
		bool change=false;
		float curV=3;
		do{
			change=false;
			for(int i=0;i<Edges.GetAmount();i++){
				int x=Edges[i]&0xFFFF;
				int y=Edges[i]>>16;
				float vL=FL->Get(x-1,y);
				float vR=FL->Get(x+1,y);
				float vU=FL->Get(x,y-1);
				float vD=FL->Get(x,y+1);				
				if(vL<-5000){
					NextEdges.Add((x-1)+(y<<16));
					FL->Set(x-1,y,-curV);
					change=true;
				}
				if(vL>5000){
					NextEdges.Add((x-1)+(y<<16));
					FL->Set(x-1,y,curV);
					change=true;
				}
				if(vR<-5000){
					NextEdges.Add((x+1)+(y<<16));
					FL->Set(x+1,y,-curV);
					change=true;
				}
				if(vR>5000){
					NextEdges.Add((x+1)+(y<<16));
					FL->Set(x+1,y,curV);
					change=true;
				}
				if(vU<-5000){
					NextEdges.Add(x+((y-1)<<16));
					FL->Set(x,y-1,-curV);
					change=true;
				}
				if(vU>5000){
					NextEdges.Add(x+((y-1)<<16));
					FL->Set(x,y-1,curV);
					change=true;
				}
				if(vD<-5000){
					NextEdges.Add(x+((y+1)<<16));
					FL->Set(x,y+1,-curV);
					change=true;
				}
				if(vD>5000){
					NextEdges.Add(x+((y+1)<<16));
					FL->Set(x,y+1,curV);
					change=true;
				}
			}
			if(change){
				Edges.FastClear();
				for(int i=0;i<NextEdges.GetAmount();i++){
					Edges.Add(NextEdges[i]);
				}
				NextEdges.FastClear();
			}
			curV+=2;
		}while(change);
		//smoothing
		SquareMap<float> temp;
		FL->CopyTo(temp);
		for(int i=0;i<2;i++){		
			for(int ix=1;ix<511;ix++){
				for(int iy=1;iy<511;iy++){
					float va=(FL->Get(ix-1,iy)+FL->Get(ix+1,iy)+FL->Get(ix,iy-1)+FL->Get(ix,iy+1))/4;
					temp.Set(ix,iy,va);
				}			
			}
			temp.CopyTo(*FL);
		}
	}
	//map  created, now we can get value
	SquareMap<float>* FL=map[Index];
	int lx=512;
	int ly=512;	
	x*=lx;
	y*=ly;
	int xx=int(x);
	int yy=int(y);
	float f1=FL->Get(xx,yy);
	float f2=FL->Get(xx+1,yy);
	float f3=FL->Get(xx,yy+1);
	float f4=FL->Get(xx+1,yy+1);
	float dx=x-xx;
	float dy=y-yy;
	return f1+dx*(f2-f1)+dy*(f3-f1)+dx*dy*(f4+f1-f3-f2);    
}

SquareMap<float>* DistField::map[MaxDistSet];

float OneSurfaceRule::GetHeight(int x,int y,float& Weight,int z){
	/////
	if(AllowRandomShift){
		float wL=DistField::Get(Map,float(x-64)*surfScale,float(y)*surfScale);
		float wR=DistField::Get(Map,float(x+64)*surfScale,float(y)*surfScale);
		float wU=DistField::Get(Map,float(x)*surfScale,float(y-64)*surfScale);
		float wD=DistField::Get(Map,float(x)*surfScale,float(y+64)*surfScale);

		float nx=(wR-wL)/4;
		float ny=(wD-wU)/4;

		float nn=sqrt(nx*nx+ny*ny);
		if(nn>0){
			nx/=nn;
			ny/=nn;
		}
		float dr=(RandomShiftFractal.GetFValue(x+y,x-y)-0.5f)*DegreeOfRandomShift*RandomShiftModulator;		
		extern int zcur;
		x+=nx*dr*zcur;
		y+=ny*dr*zcur;
	}	
	/////
	Weight=0;
	if(!Enabled)return 0;	
	int dx=x;
	int dy=y;
	if(AllowShapeCurving){
		ShapeCurving.ShiftXY(dx,dy);
		dx-=x;
		dy-=y;
	}else{
		dx=0;
		dy=0;
	}
	float bw=0.0f;
	float cBoundWidth=BoundWidth;
	if(VariateWidth){		
		bw=WidthVariationLow.GetFValue(x,y);
		cBoundWidth=cBoundWidth*(1-bw)+bw*ChangedWidth;
		if(AllowShapeCurving){
			dx*=(ShapeCurvingModulator*bw+1-bw);
			dy*=(ShapeCurvingModulator*bw+1-bw);
		}		
	}
	float w=DistField::Get(Map,float(x+dx)*surfScale,float(y+dy)*surfScale);
	int H=TopHeight;
	if(AllowModulation&&!ModulateAfterTexturing){
		H*=ModulationFactor*ModulationVariation.GetFValue(x,y)*GlobalModulationVariation.GetFValue(x,y);
	}
	if(cBoundWidth<1)cBoundWidth=1;	
	Weight=0.5f+float(w)*16/cBoundWidth;
	if(Weight<0.0f)Weight=0.0f;
	if(Weight>1.0f)Weight=1.0f;
	Weight=Weight*Weight*(2-Weight)*(2-Weight);
	int N=ExtraConditions.GetAmount();
	if(N){
		PrecompiledVertex V;
		V.Angle=0;
		V.Azimut=0;
		OneMapVertex MV;
		MV.x=x;
		MV.y=y;
		MV.z=z;
		V.Vertex=&MV;
		V.VertexIndex=0;

		for(int i=0;i<N;i++){			
			V.Height=z;
			float WW=ExtraConditions[i]->GetConditionWeight(&V);
			Weight*=WW;
		}
	}
	return H;	
}

float OneSurfaceRule::PerformShift(Vector3D V,Vector3D& DV){//returns weight of vector shift
	int x=V.x;
	int y=V.y;
	DV=Vector3D(0,0,0);
	if(!(Enabled&&(AllowRandomShift||ModulateAfterTexturing)))return 0;	
	int dx=x;
	int dy=y;
	if(AllowShapeCurving){
		ShapeCurving.ShiftXY(dx,dy);
		dx-=x;
		dy-=y;
	}else{
		dx=0;
		dy=0;
	}
	float bw=0.0f;
	float cBoundWidth=BoundWidth;
	float cDegreeOfRandomShift=DegreeOfRandomShift;
	if(VariateWidth){		
		bw=WidthVariationLow.GetFValue(x,y);
		cBoundWidth=cBoundWidth*(1-bw)+bw*ChangedWidth;
		if(AllowShapeCurving){
			dx*=(ShapeCurvingModulator*bw+1-bw);
			dy*=(ShapeCurvingModulator*bw+1-bw);
		}
		if(AllowRandomShift){
			cDegreeOfRandomShift*=(RandomShiftModulator*bw+1-bw);
		}
	}

	float w=DistField::Get(Map,float(x+dx)*surfScale,float(y+dy)*surfScale);	

	float Weight=0.0f;
	if(cBoundWidth<1)cBoundWidth=1;	
	Weight=0.5f+float(w)*16/cBoundWidth;
	if(Weight<0.0f)Weight=0.0f;
	if(Weight>1.0f)Weight=1.0f;
	//Weight=Weight*Weight*(2-Weight)*(2-Weight);

	if(AllowModulation&&ModulateAfterTexturing){
		DV.z=V.z*Weight*(ModulationFactor*ModulationVariation.GetFValue(x,y)*GlobalModulationVariation.GetFValue(x,y)-1.0f);
	}
	/*
	if(AllowRandomShift){
		float wL=DistField::Get(Map,float(x+dx-64)*surfScale,float(y+dy)*surfScale);
		float wR=DistField::Get(Map,float(x+dx+64)*surfScale,float(y+dy)*surfScale);
		float wU=DistField::Get(Map,float(x+dx)*surfScale,float(y+dy-64)*surfScale);
		float wD=DistField::Get(Map,float(x+dx)*surfScale,float(y+dy+64)*surfScale);

		float nx=(wR-wL)/4;
		float ny=(wD-wU)/4;

		float nn=sqrt(nx*nx+ny*ny);
		if(nn>0){
			nx/=nn;
			ny/=nn;
		}

		float w1=1.0f-Weight;
		w1=w1*w1;

		float dr=RandomShiftFractal.GetFValue(x+y,x-y)-0.5f;

		DV.x+=Weight*w1*Weight*w1*dr*nx*Weight*cDegreeOfRandomShift*8000;
		DV.y+=Weight*w1*Weight*w1*dr*ny*Weight*cDegreeOfRandomShift*8000;
	}	
	*/
	return Weight;
}

float SurfaceGenerator::GetHeight(int x,int y){
	float SumH=0;
	float SumW=0;
	for(int i=0;i<Relief.GetAmount();i++)if(Relief[i]->Enabled){
		OneSurfaceType* OST=Relief[i];
		float SumHC=0;
		float SumWC=0;	
		int sr=OST->SurfaceRules.GetAmount();
		int ns=0;
		for(int j=0;j<sr;j++)if(OST->SurfaceRules[j]->Enabled)ns++;
		for(int j=0;j<sr;j++)if(OST->SurfaceRules[j]->Enabled){
			OneSurfaceRule* OSR=OST->SurfaceRules[j];
			float W=0;
			int zz=0;
			if(SumWC>0.01){
				zz=SumHC/SumWC;
			}
			float H=OSR->GetHeight(x,y,W,zz);
			SumHC+=H*W*ns+OST->BasicHeight*(1-W);
			SumWC++;           
		}
		if(SumWC<0.001){
			SumWC=1;
			SumHC=OST->BasicHeight;
		}
		if(OST->UseMap){
			BasePaintMap* BPM=MLayers->Maps[OST->Map];			
			float w=BPM->GetGreyPrecise(float(x)*surfScale,float(y)*surfScale);
			SumHC*=float(w)/255;
			SumWC*=float(w)/255;
		}
		SumH+=SumHC;
		SumW+=SumWC;
	}
	if(SumW<0.001)SumW=1;
	return SumH/SumW;
}

bool SurfaceGenerator::PerformShift(Vector3D& V){
	Vector3D SumV=Vector3D(0,0,0);
	float SumW=0;
	for(int i=0;i<Relief.GetAmount();i++)if(Relief[i]->Enabled){
		OneSurfaceType* OST=Relief[i];
		Vector3D SumVC=Vector3D(0,0,0);
		float SumWC=0;		
		for(int j=0;j<OST->SurfaceRules.GetAmount();j++)if(OST->SurfaceRules[j]->Enabled){
			OneSurfaceRule* OSR=OST->SurfaceRules[j];
			Vector3D DV=Vector3D(0,0,0);
			float W=OSR->PerformShift(V,DV);
			DV*=W;
			SumVC+=DV;
			SumWC+=W;           
		}
		if(SumWC<0.001){
			SumWC=1;			
		}
		if(OST->UseMap){
			BasePaintMap* BPM=MLayers->Maps[OST->Map];
			float w=BPM->GetGreyPrecise(V.x*surfScale,V.y*surfScale);
			SumVC*=float(w)/255;
			SumWC*=float(w)/255;
		}
		SumV+=SumVC;
		SumW+=SumWC;
	}
	if(SumW<0.001)SumW=1;
	SumV/=SumW;
	V+=SumV;
	return true;
}
int zcur;
void GeneratorSurfaceFunction::ModifyVertex(OneMapVertex& V)
{
	zcur=V.z;
	V.z=GPtr->GetHeight(V.x,V.y);
}

bool GeneratorSurfaceFunction::ShiftVertex(OneMapVertex& V){
	Vector3D VV=Vector3D(V.x,V.y,V.z);
	GPtr->PerformShift(VV);
	//assert(VV.x>-200);
	//assert(VV.y>-200);
	V.x=VV.x;		
	V.y=VV.y;
	V.z=VV.z;
	//assert(V.x>-200);
	//assert(V.y>-200);
	return true;
}

float sq_norma(float x,float y);
void PrecompiledVertex::CreateFromVertex(OneMapVertex& V){
	Height=V.z;
	Angle=atan2(sq_norma(V.nx,V.ny),float(V.nz))*180/3.1415;
	Vertex=&V;
}

void SurfaceGenerator::FillVertexTextures(PrecompiledVertex* V){
	int x=V->Vertex->x;
	int y=V->Vertex->y;
	float R=0;
	float G=0;
	float B=0;
	float SummW=0;
	int FacturesWeight[128];
	memset(&FacturesWeight,0,sizeof FacturesWeight);
	int MinFacture=127;
	int MaxFacture=0;
	for(int i=0;i<Texturing.GetAmount();i++)if(Texturing[i]->Enabled){
		float cr=1.0f;
		OneTexturingStyle* OTS=Texturing[i];
		float sw=1.0f;
		for(int q=0;q<OTS->Conditions.GetAmount();q++){
			sw*=OTS->Conditions[q]->GetConditionWeight(V);
		}		
		//if(OTS->UseMap){
		//	BasePaintMap* BPM=MLayers->Maps[OTS->Map];
		//	sw=BPM->GetGreyPrecise(x*surfScale,y*surfScale);
		//	sw=sw*sw;
		//}
		if(sw>0.05){
			for(int j=0;j<OTS->TexturingRules.GetAmount();j++){
				OneTexturingRule* OTR=OTS->TexturingRules[j];
				if(OTR->Enabled&&OTR->Textures.GetAmount()){
					float w0=1.0f;
					for(int q=0;q<OTR->Conditions.GetAmount();q++){
						w0*=OTR->Conditions[q]->GetConditionWeight(V);
						if(w0<0.01)break;
					}					
					if(w0>0.01){
						w0*=sw;
						if(OTR->UseGlobalWeight){
							w0*=OTR->GlobalWeight*OTR->GlobalWeightVariation.GetFValue(x,y);
						}
						if(w0>0.001){
							int T1,T2;
							DWORD C1,C2;
							float w1,w2;
							if(OTR->Textures.GetAmount()>1){
								T1=OTR->Textures[0]->Facture;
								C1=OTR->Textures[0]->Color;
								T2=OTR->Textures[1]->Facture;
								C2=OTR->Textures[1]->Color;								
								w1=OTR->TexturesMixing.GetFValue(x,y);
								w2=1.0f-w1;
							}else{
								//one texture
								T1=T2=OTR->Textures[0]->Facture;
								C1=C2=OTR->Textures[0]->Color;
								w1=0;
								w2=1.0f;
							}
							float CB=(C1&255)*w1+(C2&255)*w2;
							float CG=((C1>>8)&255)*w1+((C2>>8)&255)*w2;
							float CR=((C1>>16)&255)*w1+((C2>>16)&255)*w2;
							R+=CR*w0;
							G+=CG*w0;
							B+=CB*w0;
							SummW+=w0;
							assert(T1<128&&T2<128);
							FacturesWeight[T1]+=w0*w1*1000;
							FacturesWeight[T2]+=w0*w2*1000;
							if(T1<MinFacture)MinFacture=T1;
							if(T2<MinFacture)MinFacture=T2;
							if(T1>MaxFacture)MaxFacture=T1;
							if(T2>MaxFacture)MaxFacture=T2;
						}
					}
				}
			}
		}
	}
	if(SummW>0.001){
		R/=SummW;
		G/=SummW;
		B/=SummW;
		if(R>255)R=255;
		if(G>255)G=255;
		if(B>255)B=255;
		int fc=0;
		int maxw=0;
		for(int i=MinFacture;i<=MaxFacture;i++){
			if(FacturesWeight[i]>maxw){
				maxw=FacturesWeight[i];
				fc=i;
			}
		}
		V->Vertex->Color=0xFF000000+(int(R)<<16)+(int(G)<<8)+int(B);
		V->Vertex->nFacture=fc;
	}
}

void SurfaceGenerator::TexturizeSurface(VertexBasedMesh& M){
	for(int i=0;i<M.Vert.GetAmount();i++){
		OneMapVertex* V=&M.Vert[i];
		PrecompiledVertex PV;
		PV.CreateFromVertex(*V);
		FillVertexTextures(&PV);
	}
}
void SurfaceGenerator::TexturizeSurface(VertexBasedMesh& M,int xc,int yc,int R1,int R2){
	VertexBasedMeshRef MR0,MR;
	g_UniversalMap.GetQuadsRefCylinder(xc,yc,R2-4,MR0);
	MR0.GetRefCylinder(xc,yc,R2-4,MR);
	DWORDS Verts;
	cHashIndex h;
	MR.ExtractUniqueVerts(Verts,h);
	for(int i=0;i<Verts.Count();i++){
		OneMapVertex* V=&M.Vert[Verts[i]];
		OneMapVertex V0=*V;
		PrecompiledVertex PV;
		PV.CreateFromVertex(*V);
		FillVertexTextures(&PV);
		int r=sq_norma(V0.x-xc,V0.y-yc);
		if(r>R1){
			int ww=(r-R1)*255/(R2-R1);
			int GetFractalValEx(int x,int y,int Type);
			ww=(ww*GetFractalValEx(V->x*5,V->y*5,1)/256);
			if(ww>240)ww=255;
			if(ww<0)ww=0;			
			DWORD CC=MixDWORD(V0.Color,V->Color,ww,255-ww);
			V->Color=CC;
			if(r>(R1+R2)/2)V->nFacture=V0.nFacture;
		}
	}
	Rct rc(xc - R2-100, yc - R2-100, 2 * R2+200, 2 * R2+200);
	ITerra->InvalidateGeometry(&rc);
}
void ChangeTreesInRadiusWithEffect(int x,int y,int R,int LandType);
void SlowTexturizeSurface(int xc,int yc,int R1,int R2
						  ,int RiseTime,int GrowTime,int RemainTime,int EliminateTime,bool Forever
						  ,char* BirthWeapon,int BirthProbability
						  ,char* UndoWeapon ,int UndoProbability){	
	VertexBasedMeshRef MR0,MR;
	g_UniversalMap.GetQuadsRefCylinder(xc,yc,R2-4,MR0);
	MR0.GetRefCylinder(xc,yc,R2-4,MR);
	DWORDS Verts;
	cHashIndex h;
	MR.ExtractUniqueVerts(Verts,h);
	for(int i=0;i<Verts.Count();i++){
		OneMapVertex* V=g_UniversalMap.Vert+Verts[i];		
		int r=sq_norma(V->x-xc,V->y-yc);		
		int ww=255;
		if(r<R2){
			if(r>R1){
				ww=255-(r-R1)*255/(R2-R1);
				int GetFractalValEx(int x,int y,int Type);
				ww=(ww*GetFractalValEx(V->x*5,V->y*5,1)/256);
				if(ww>255)ww=255;
				if(ww<0)ww=0;	
			}
			int ww2=128+(ww-128)*4;
			if(ww2<0)ww2=0;
			if(ww2>255)ww2=255;
			if(ww2>0){
				EffectToBorn E1;
				E1.WeaponToBorn=BirthWeapon;
				E1.WeaponBornProbability=BirthProbability;
				EffectToBorn E2;
				E2.WeaponToBorn=UndoWeapon;
				E2.WeaponBornProbability=UndoProbability;
				TMORPH.AddTerraTypeDeformator(Verts[i],(GrowTime*(255-ww)/255+GrowTime*r/R2),RiseTime,RemainTime,EliminateTime,ww2,Forever,&E1,&E2);
			}
		}
	}
}
void ApplyLandType(char* From,char* To,int xc,int yc,int R1,int R2){
	float d=15360.0f/512.0f;
	for(int i=0;i<MLayers->MapsNames.GetAmount();i++)if(LAYERSLIST.Layers[i]->ThisLayerDeterminesRace){		
		GreyMap* GRM=(GreyMap*)MLayers->Maps[i];
		GRM->PreciseClearValueInRound(float(xc)/d,float(yc)/d,float(R1)/d,float(R2-R1)/d);		
	}
	for(int i=0;i<MLayers->MapsNames.GetAmount();i++){
		if(MLayers->MapsNames[i]->equal(To)){
			GreyMap* GRM=(GreyMap*)MLayers->Maps[i];
			GRM->PreciseSetValueInRound(float(xc)/d,float(yc)/d,float(R1)/d,float(R2-R1)/d);
			SURF.TexturizeSurface(g_UniversalMap,xc,yc,R1,R2);
			break;
		}
	}
}
void ApplyLandTypeSlow(char* From,char* To,int xc,int yc,int R1,int R2
					   ,int RiseTime,int GrowTime,int RemainTime,int EliminateTime,bool Forever
					   ,char* BirthWeapon,int BirthProbability
					   ,char* UndoWeapon ,int UndoProbability){
	float d=15360.0f/512.0f;
	for(int i=0;i<MLayers->MapsNames.GetAmount();i++)if(LAYERSLIST.Layers[i]->ThisLayerDeterminesRace){
		GreyMap* GRM=(GreyMap*)MLayers->Maps[i];
		GRM->PreciseClearValueInRound(float(xc)/d,float(yc)/d,float(R1)/d,float(R2-R1)/d);
	}	
	for(int i=0;i<MLayers->MapsNames.GetAmount();i++){
		if(MLayers->MapsNames[i]->equal(To)){
			ChangeTreesInRadiusWithEffect(xc,yc,R2,i);
			GreyMap* GRM=(GreyMap*)MLayers->Maps[i];
			GRM->PreciseSetValueInRound(float(xc)/d,float(yc)/d,float(R1)/d,float(R2-R1)/d);
			SlowTexturizeSurface(xc,yc,R1,R2,RiseTime,GrowTime,RemainTime,EliminateTime,Forever,BirthWeapon,BirthProbability,UndoWeapon,UndoProbability);
			break;
		}
	}
}
int RA_Values[17]={
	255,160,210,193,
	240,145,160,195,
	210,255,200,190,
	150,210,180,220,255};
int GetChasmModulator(byte Angle){
    int A1=Angle>>4;
	int A2=A1+1;
	int DA=Angle-(A1<<4);
	return RA_Values[A1]+(RA_Values[A2]-RA_Values[A1])*DA/16;
}
void ApplyLandChasm(int xc,int yc,int R,int Deep
					,int RiseTime,int GrowTime,int RemainTime,int EliminateTime){
	VertexBasedMeshRef MR0,MR;
	g_UniversalMap.GetQuadsRefCylinder(xc,yc,R,MR0);
	MR0.GetRefCylinder(xc,yc,R,MR);
	DWORDS Verts;
	cHashIndex h;
	MR.ExtractUniqueVerts(Verts,h);
	for(int i=0;i<Verts.Count();i++){
		OneMapVertex* V=g_UniversalMap.Vert+Verts[i];		
		int dx=V->x-xc;
		int dy=V->y-yc;
		word GetDir(int dx,int dy);
		word GetDir(int dx,int dy);
		byte A=GetDir(dx,dy)+xc;
		int RR=R*GetChasmModulator(A)/255;
		int r=sq_norma(dx,dy);
		int ww=255;
		int DW=60;
		int DH2=30;
		if(r<RR+DW){
			float DR=float(RR-r);
			int DX=0;
			int DY=0;
			int DH;
			Vector3D nr;
			if(DR>0){
				nr=Vector3D(-dx,-dy,0);
				DH=-Deep*(RR-r)/RR-DH2;
			}else{
                nr=Vector3D(0,0,1);
				DH=-(DW+DR)*DH2/DW;
			}
			nr.normalize();
			if(DR>0){
				int DD=(r-RR)*sqrt(RR-r)/sqrt(RR);
				DX=-3*dx*DD/RR;
				DY=-3*dy*DD/RR;
			}
			int ww=(RR-r)*255/RR;
			int DT=rando()%RiseTime/8+RiseTime*(255-ww)*7/255/8;
			TMORPH.AddTerraHeightDeformator(Verts[i],DT,RiseTime-DT,RemainTime-DT,EliminateTime,Vector3D(DX,DY,DH),nr);
		}
	}
}
extern int TrueTime;
void FallUnitLink(OneObject* OB){	
	int dt=TrueTime-OB->LocalOrder->info.BuildObj.ObjIndex;
	if(dt<0)return;	
	int sp=800+(OB->Index%30)*30;
    OB->OverEarth=OB->LocalOrder->info.BuildObj.ObjX-dt*dt/sp;
	if(OB->OverEarth<-1500){
		OB->Die();
		OB->OverEarth=-1500;
	}
}
void FallUnit(OneObject* OB,int Timeout){
    OB->UnlimitedMotion=false;
	Order1* OR=OB->CreateOrder(0);
	if(OR){
		OR->info.BuildObj.ObjIndex=TrueTime+Timeout;
		OR->info.BuildObj.ObjX=OB->OverEarth;
        OR->DoLink=&FallUnitLink;
	}
	OB->UnlimitedMotion=true;
}
typedef float fn_GetHeight(float x,float y,float xc,float yc,void* param);
_fractalLine FL_Chasm;
void ApplyArbitraryChasm(int xc,int yc,int R,float Deep,float SlopeScale,fn_GetHeight* getHeight,void* param
						,int RiseTime,int GrowTime,int RemainTime,int EliminateTime){
	static float F_scale=2.0;
    FL_Chasm.Scale=F_scale;
	VertexBasedMeshRef MR0,MR;
	g_UniversalMap.GetQuadsRefCylinder(xc,yc,R,MR0);
	MR0.GetRefCylinder(xc,yc,R,MR);
	DWORDS Verts;
	cHashIndex h;
	MR.ExtractUniqueVerts(Verts,h);
	static float Xscale=1.4*SlopeScale*R*R/300/300;
	for(int i=0;i<Verts.Count();i++){
		OneMapVertex* V=g_UniversalMap.Vert+Verts[i];		
		int xx=V->x;int yy=V->y;
		//FL_Chasm.ShiftXY(xx,yy);
		float H=getHeight(xx,yy,xc,yc,param);
		if(H>0){
            Vector3D V(
				getHeight(xx-16,yy,xc,yc,param)-getHeight(xx+16,yy,xc,yc,param)
				,getHeight(xx,yy-16,xc,yc,param)-getHeight(xx,yy+16,xc,yc,param),32);
			V.x*=Deep;
			V.y*=Deep;
			V.normalize();
			int ww=H*255;
			int DT=rando()%RiseTime/8+RiseTime*(255-ww)*7/255/8;
			static float clampV=4;
			float HClamp=H*clampV;
			if(HClamp>1)HClamp=1;
			float H2=H*(1-H)*HClamp*HClamp*Deep;			
			TMORPH.AddTerraHeightDeformator(Verts[i],DT,RiseTime-DT,RemainTime-DT,EliminateTime,Vector3D(H2*V.x*Xscale,H2*V.y*Xscale,-H*Deep),V);
		}
	}
	//falling units

	itr_UnitsInRadius.Create(xc,yc,R);
	while(OneObject* OB=itr_UnitsInRadius.Next()){
		HeroVariableStorage* DetectHero(OneObject* OB);
		if(!DetectHero(OB)){
			float H=getHeight(OB->RealX/16,OB->RealY/16,xc,yc,param);
			int ww=H*255;
			H*=Deep;
			int DT=30+RiseTime*(255-ww)/255;
			if(H>32)FallUnit(OB,DT);
		}else{
			for(int i=0;i<512;i++){
				byte A=rando()&255;
				int RR=R+rando()&127;
                int x=xc+(TCos[A]*R)/256;
				int y=yc+(TSin[A]*R)/256;
				if(!CheckBar(x/16-3,y/16-3,7,7)){
					void ChangeUnitCoor(OneObject* OB,int newX,int newY);
					ChangeUnitCoor(OB,x<<4,y<<4);
				}
			}
		}
	}
	itr_GetBuildingsInRadius.Create(xc,yc,R);
	while(OneObject* OB=itr_GetBuildingsInRadius.Next()){
		float H=getHeight(OB->RealX/16,OB->RealY/16,xc,yc,param);
		int ww=H*255;
		H*=Deep;
		int DT=30+RiseTime*(255-ww)/255;
		if(H>10)FallUnit(OB,DT);
	}
}

struct fn_GetHeightForRound_param{
	float R;
	float Deep;
};
float fn_GetHeightForRound(float x,float y,float xc,float yc,void* param){	
	fn_GetHeightForRound_param* P=(fn_GetHeightForRound_param*)param;
	word GetDir(int dx,int dy);
	byte A=GetDir(x-xc,y-yc)+xc;
	float R=sq_norma(x-xc,y-yc);
	R=R*255/GetChasmModulator(A);	
	if(R>P->R+64)return 0;
	R=P->R-R;
    //float H1=R*R/92;
	float H1=(R+64)/2;
	float H2=R*P->Deep/P->R;
	if(H2<0)H2=0;
	return max(H1,H2)/P->Deep;
}
void ApplyRoundChasm(int xc,int yc,int R,int Deep,int RiseTime,int GrowTime,int RemainTime,int EliminateTime){
    fn_GetHeightForRound_param P;
	P.R=R;
	P.Deep=Deep;
	ApplyArbitraryChasm(xc,yc,R,Deep,1,&fn_GetHeightForRound,&P,RiseTime,GrowTime,RemainTime,EliminateTime);
}
struct fn_GetHeightForRow_param{
	float R1;
	float R2;
	float minR;
	float SIN,COS;
	float Deep;
};
float fn_GetHeightForRow(float x,float y,float xc,float yc,void* param){	
	fn_GetHeightForRow_param* P=(fn_GetHeightForRow_param*)param;
	x-=xc;y-=yc;
	float xr=P->R1-fabs(x*P->COS+y*P->SIN);
	float yr=P->R2-fabs(-y*P->SIN+y*P->COS);
	if(xr<0 || yr<0)return 0;
	float RR=min(xr,yr);
	float H1=((RR*RR/64)*RR/64)*RR/64;
	//if(RR>64)H1=P->Deep;
	//float H2=RR*P->Deep/P->minR;
	float H2=P->Deep;
	//if(H2>P->Deep)H2=P->Deep;
	return min(H1,H2)/P->Deep;
}
float fn_GetHeightElliptic(float x,float y,float xc,float yc,void* param){	
	fn_GetHeightForRow_param* P=(fn_GetHeightForRow_param*)param;
	x-=xc;y-=yc;
	float xr=P->R1-fabs(x*P->COS+y*P->SIN);
	float yr=P->R1-fabs(-y*P->SIN+y*P->COS)*P->R1/P->R2;
	if(xr<0 || yr<0)return 0;
	float RR=sq_norma(xr,yr);
	float H1=(RR*RR/64);
	//if(RR>64)H1=P->Deep;
	//float H2=RR*P->Deep/P->minR;
	float H2=P->Deep;
	//if(H2>P->Deep)H2=P->Deep;
	return min(H1,H2)/P->Deep;
}
void ApplyRowChasm(int xc,int yc,int R1,int R2,byte Dir,int Deep,int RiseTime,int GrowTime,int RemainTime,int EliminateTime){
	fn_GetHeightForRow_param P;
	P.R1=R1;
	P.R2=R2;
	P.Deep=Deep;
	P.minR=min(R1,R2);
	P.SIN=TSin[Dir]/255.0f;
	P.COS=TCos[Dir]/255.0f;
	float d=min(R1,R2)/float(max(R1,R2));
	ApplyArbitraryChasm(xc,yc,max(R1,R2),Deep,d*d,&fn_GetHeightForRow,&P,RiseTime,GrowTime,RemainTime,EliminateTime);
}

					
void ApplyStandartChasm(int x,int y,int R,int Time){
	ApplyRoundChasm(x,y,R,450,400,500,Time,500);
}
void TestRuleApply(int x,int y,int r){
	//SURF.TexturizeSurface(g_UniversalMap,x,y,r,r+200);
	//if(GetKeyState('A'))ApplyLandTypeSlow("IceDruids","Necro",x,y,r,r+400,200,5000,5000,5000,false,"",0,"",0);
	//else ApplyLandTypeSlow("NecroDruids","Ice",x,y,r,r+400,200,5000,5000,3000,false,"Blizard",1,"",0);
	//ApplyLandChasm(x,y,r+100,450,400,500,5000,500);
	//ApplyRoundChasm(x,y,r+100,450,400,500,5000,500);
	static int xp=0;
	static int yp=0;
	if(x!=xp && y!=yp){
		ApplyRoundChasm(x,y,r+100,450,400,500,5000,500);
		//ApplyLandChasm(x,y,r+100,450,400,500,5000,500);
	}
	xp=x;
	yp=y;
}
void DistField::ClearMaps(){
	for(int i=0;i<MaxDistSet;i++)if(map[i]){
		delete(map[i]);
		map[i]=NULL;
	}
}

void SurfaceGenerator::PerformTest(){
	DistField::ClearMaps();
	GeneratorSurfaceFunction SF;
	SF.GPtr=this;
	UniversalMap VBMesh;
	Vector3D v3dSun(2, 1, 1);
//	VBMesh.CreateSurface(150,150,32,&SF);
//	VBMesh.VertexBasedMesh::ModifySurface(2,&SF);
	VBMesh.CalcNormals();
	SURF.TexturizeSurface(VBMesh);
	for(int i = 0; i < VBMesh.Vert.GetAmount(); i++)
		if(VBMesh.Vert[i].nFacture > 100) VBMesh.Vert[i].nFacture = 100;

	//	VBMesh.CalcShadow(v3dSun, 0.4, 80, 255);
	//VBMesh.InitUniversalMapQuad(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	VBMesh.QuadsSystem_Init();

	OneSplitMesh OSMesh;
	VertexBasedMeshRef VBMRef;
	VBMesh.GetMeshRefSphere(Vector3D(0,0,0),5000,&VBMRef);
	//OSMesh.AdvSplitMesh(&VBMRef);
	OSMesh.Split(VBMRef);
	DistField::ClearMaps();
	float A = 0;

	// ----------------------------------------
	extern UniversalMap g_UniversalMap;
	g_UniversalMap.GetMeshRefBox(0, 0, 10000, 20000, &VBMRef);
	//OSMesh.AdvSplitMesh(&VBMRef);
	OSMesh.Split(VBMRef);
	do{
		A += 0.08;
		//processing
		//MESH.TestDraw();	
		//NvertexOut(MESH);
		/*		v3dSun.x = cos(A),
		v3dSun.y = 1,
		v3dSun.z = sin(A);
		MESH.CalcShadow(v3dSun, 0.4, 80, 255);
		SPM.AdvSplitMesh(&MR);*/
		OSMesh.Draw();
		//VBMesh.TestDraw();

		FlipPages();
		ProcessMessages();
		IRS->ClearDevice( 0, true, true );
		extern float fMapX;
		extern float fMapY;
		float fstep=0.5;
		if(GetKeyState(VK_LEFT)&0x8000){			
			fMapX-=fstep;
		}
		if(GetKeyState(VK_RIGHT)&0x8000){			
			fMapX+=fstep;;
		}
		if(GetKeyState(VK_UP)&0x8000){			
			fMapY-=fstep;
		}
		if(GetKeyState(VK_DOWN)&0x8000){			
			fMapY+=fstep;
		}
		void SetupCamera();
		SetupCamera();
	}while(!(GetKeyState(VK_SPACE)&0x8000));	
}

extern UniversalMap g_UniversalMap;
bool LoadMLayersFromFileInCompactForm(char* filename);
void SaveMLayersToFileInCompactForm(char* filename);
//Options=1-init, 2-process, 3-ok pressed, 4-cancel pressed 
bool rce_SurfaceCallback(ClassEditor* CE,BaseClass* BC,int Options){
	if(Options==1){
		LastKey=0;
		KeyPressed=false;
		TestLayersColors();	
		if(!LoadMLayersFromFileInCompactForm("Surface\\SurfaceShape.dat")){
			bool CheckIfFileExists(char* Name);
			if(CheckIfFileExists("Surface\\SurfaceShape.xml")){
				MLayers->reset_class(MLayers);
				MLayers->SafeReadFromFile("Surface\\SurfaceShape.xml");
				TestLayersColors();	
			}
		}
	}
	if(Options==2){
		if(MLayers->Maps.GetAmount()){
			Enumerator* E=ENUM.Get("LAYERSLIST");
			for(int i=0;i<MLayers->Maps.GetAmount();i++){
				MLayers->Maps[i]->Desc.Clear();
				MLayers->Maps[i]->Desc=E->Get(i);
				MLayers->Maps[i]->Desc+=" : ";
			}
			int NR=SURF.Relief.GetAmount();
			for(int i=0;i<NR;i++){
				_str* s=&MLayers->Maps[SURF.Relief[i]->Map]->Desc;				
				_str* ads=&SURF.Relief[i]->Description;
				if(ads->str&&ads->str[0]){
					*s+=*ads;
					*s+=" ";
				}
				int NR=SURF.Relief[i]->SurfaceRules.GetAmount();
				for(int j=0;j<NR;j++){
					_str* s=&MLayers->Maps[SURF.Relief[i]->SurfaceRules[j]->Map]->Desc;
					_str* ads=&SURF.Relief[i]->SurfaceRules[j]->Description;
					if(ads->str&&ads->str[0]){
						*s+=*ads;
						*s+=" ";
					}
				}

			}
			for(int i=0;i<SURF.Texturing.GetAmount();i++){
				/*if(SURF.Texturing[i]->UseMap){
				_str* s=&MLayers->Maps[SURF.Texturing[i]->Map]->Desc;
				_str* ads=&SURF.Texturing[i]->Description;
				if(ads->str&&ads->str[0]){
				*s+=*ads;
				*s+=" ";
				}                    
				}*/
				for(int j=0;j<SURF.Texturing[i]->TexturingRules.GetAmount();j++){
					OneTexturingRule* TR=SURF.Texturing[i]->TexturingRules[j];
					for(int q=0;q<TR->Conditions.GetAmount();q++){
						CheckSpecialLayer* L=dynamic_cast<CheckSpecialLayer*>(TR->Conditions[q]);
						if(L){
							_str* s=&MLayers->Maps[L->Layer]->Desc;
							_str* ads=&TR->Description;
							if(ads->str&&ads->str[0]){
								*s+=*ads;
								*s+=" ";
							}
						}
					}					
				}
			}
			for(int i=0;i<MLayers->Maps.GetAmount();i++){
				_str* s=&MLayers->Maps[i]->Desc;
				if(s&&s->str){
					void Translit(char* s);
					Translit(s->str);
				}
			}
		}
	}
	if(Options==3){//Ok
		//MLayers->WriteToFile("SurfaceShape.xml");
		SaveMLayersToFileInCompactForm("Surface\\SurfaceShape.dat");
		if(!g_UniversalMap.Idxs.GetAmount())SURF.DontGenerateRelief=false;
		if(!SURF.DontGenerateRelief){
			g_UniversalMap.Release();
		}
		DistField::ClearMaps();
		GeneratorSurfaceFunction SF;
		SF.GPtr=&SURF;		
		SF.ShapeOfBound=&SURF.ShapeOfBound;		
		Vector3D a;
		int cellsize=32;
		int NX=SURF.Size/cellsize;
		if(NX<40)NX=40;
		if(NX>512)NX=512;

		SF.MapLx=NX*cellsize;
		SF.MapLy=NX*cellsize;

		if(!SURF.DontGenerateRelief){
			/*
			g_UniversalMap.CreateSurface(NX,NX, 32, &SF);
			g_UniversalMap.VertexBasedMesh::ModifySurface(NX<300,&SF);
			g_UniversalMap.Smooth(0.5,2);
			*/
			g_UniversalMap.ApplyMCubes(32,NX,NX,&SF);
			VertexBasedMeshRef Ref;
			g_UniversalMap.GetRefAll(Ref);
			Ref.Relax();
			//g_UniversalMap.Smooth(0.5,2);
			//g_UniversalMap.InvertNormals();
		}
		g_UniversalMap.CalcNormals();
		a.x=2;
		a.y=1;
		a.z=1;
		a.normalize();
		SURF.TexturizeSurface(g_UniversalMap);
//		g_UniversalMap.SmoothColors(0,0,20000);
//		g_UniversalMap.SmoothColors(0,0,20000);
//		g_UniversalMap.SmoothColors(0,0,20000);
		VertexBasedMeshRef Ref;
		g_UniversalMap.GetQuadsRefAll(Ref);
//		Ref.SmoothColors(0, 0, 20000);
//		Ref.SmoothColors(0, 0, 20000);
//		Ref.SmoothColors(0, 0, 20000);
		for(int k = 0; k < 3; k++) {
			Ref.SmoothColors();
		}
		int N = 0, maxNFacture = 0;
		int GetNFactures();
		maxNFacture = GetNFactures() - 1;
		N = g_UniversalMap.Vert.GetAmount();
		for(int i = 0; i < N; i++)
			if(g_UniversalMap.Vert[i].nFacture > maxNFacture)
				g_UniversalMap.Vert[i].nFacture = maxNFacture;
		//g_UniversalMap.CalcShadow(a, 0.2, 40, 255);
		//g_UniversalMap.CreateShadow(a, 40, 255);
		//g_UniversalMap.InitUniversalMapQuad(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		g_UniversalMap.QuadsSystem_Init();
		//g_UniversalMap.FillMapOfHeights();
//		VertexBasedMeshRef Ref;
//		g_UniversalMap.GetQuadsRefAll(Ref);
		Ref.FillMapOfHeights();
		//float CreateNaturalShadow(VertexBasedMesh *, int);
		//CreateNaturalShadow(&g_UniversalMap, 2048);
		//		float CreateNaturalShadow(UniversalMap &);
		//		CreateNaturalShadow(g_UniversalMap);
//		extern CSun g_Sun;
//		g_UniversalMap.Shade(true, g_UniversalMap.m_pSun);
//		g_UniversalMap.Shade(true, g_UniversalMap.m_Shader);
		//g_UniversalMap.Shade(g_UniversalMap.m_Shader, SSDirectIllumination);
		g_UniversalMap.Shade(g_UniversalMap.m_Shader, SSLocalIllumination);
		DistField::ClearMaps();
		void ResetGroundCache();
		ResetGroundCache();
		// BEGIN: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		char Buffer[256], Str[4096];
		sprintf(Buffer, "QuadX0 = %d\tQuadY0 = %d\r\n",
			g_UniversalMap.QuadX0, g_UniversalMap.QuadY0);
		strcpy(Str, Buffer);
		int UMLx = 0, UMLy = 0;
		UMLx = g_UniversalMap.QuadLx * g_UniversalMap.QuadsNx,
			UMLy = g_UniversalMap.QuadLy * g_UniversalMap.QuadsNy;
		sprintf(Buffer, "UMLx = %d\tUMLy = %d\r\n", UMLx, UMLy);
		strcat(Str, Buffer);
		sprintf(Buffer, "g_UniversalMap.Vert.GetAmount() == %d", g_UniversalMap.Vert.GetAmount());
		strcat(Str, Buffer);
		sprintf(Buffer, "\r\ng_UnviersalMap.Idxs.GetAmount() == %d", g_UniversalMap.Idxs.GetAmount());
		strcat(Str, Buffer);
		bool FacturesInUse[SPLITMESH_MAXNFACTURES];
		ZeroMemory(FacturesInUse, sizeof(FacturesInUse));
		N = g_UniversalMap.Vert.GetAmount();
		for(i = 0; i < N; i++) FacturesInUse[g_UniversalMap.Vert[i].nFacture] = true;
		maxNFacture = GetNFactures() - 1;
		strcat(Str, "\r\nNumbers of factures in use: ");
		for(i = 0; i <= maxNFacture; i++)
			if(FacturesInUse[i] == true)
			{
				sprintf(Buffer, " %d", i);
				strcat(Str, Buffer);
			}
			MessageBox(NULL, Str, "rce_SurfaceCallback stats:", 0);
			// END: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	}
	return false;
}

void RegisterSurfEditor(){
	if(!MLayers){
		MLayers=new MapsScope;
		MLayers->SetDefaults();
		void TestLayersColors();
		TestLayersColors();
	}
	//SURF.Test.SetParent(&SURF);	
	AddStdEditor("SurfaceEditor",&SURF,"Surface\\Surface3D.xml",RCE_DEFAULT,rce_SurfaceCallback,NULL,'S');        
	if(GetKeyState(VK_CONTROL)&0x8000){
		void SelectEditor();
		SelectEditor();
	}
}
//////////////////////////////////////////////////////////////////////////
//OneVertexTempDeformator
//////////////////////////////////////////////////////////////////////////

void OneVertexTempDeformator::BlendVertsCoor(OneMapVertex& V1,OneMapVertex& V2,int W1,int W2,OneMapVertex& Dest){
	int DW=(W1+W2)/2;
	Dest.x=(int(V1.x)*W1+int(V2.x)*W2+DW)/255;
	Dest.y=(int(V1.y)*W1+int(V2.y)*W2+DW)/255;
	Dest.z=(int(V1.z)*W1+int(V2.z)*W2+DW)/255;

	Dest.nx=(int(V1.nx)*W1+int(V2.nx)*W2+DW)/255;
	Dest.ny=(int(V1.ny)*W1+int(V2.ny)*W2+DW)/255;
	Dest.nz=(int(V1.nz)*W1+int(V2.nz)*W2+DW)/255;
}
void OneVertexTempDeformator::BlendVertsColors(OneMapVertex& V1,OneMapVertex& V2,int W1,int W2,OneMapVertex& Dest){
	int DW=(W1+W2)/2;
    Dest.Shadow = (int(V1.Shadow)*W1+int(V2.Shadow)*W2+DW)/255;
	Dest.Color  = MixDWORD(V1.Color,V2.Color,W1,W2);
	Dest.nFacture=W1>128?V1.nFacture:V2.nFacture;
}
void OneVertexTempDeformator::BlendVerts(OneMapVertex& V1,OneMapVertex& V2,int W1,int W2,OneMapVertex& Dest){
    BlendVertsCoor(V1,V2,W1,W2,Dest);
	BlendVertsColors(V1,V2,W1,W2,Dest);
}
int ScaledGameTime=0;
bool OneVertexTempDeformator::PerformDeformation(){
	int t=ScaledGameTime-TimeToWait-StartTime;
	if(t>0){
		int w=0;
		if(t<TimeToTransformToFinalState){
            w=t*255/TimeToTransformToFinalState;
		}else{
			t-=TimeToTransformToFinalState;
			if(t<TimeToStayInFinalState){
                w=255;                
			}else{				
				t-=TimeToStayInFinalState;
				if(t<TimeToReturnInStartState){
					if(!ForeverChange){
						w=255-t*255/TimeToReturnInStartState;
					}else w=255;
				}else if(ForeverChange)w=255;
			}
		}
		if(w<0)w=0;
		if(w>MaxFinalWeight)w=MaxFinalWeight;
		BlendVerts(DestVertex,StartVertex,w,255-w,CurVertex);
		return true;
	}
	return false;    
}
void ChangeTerraType::CalcDestVertex(){	
	OneMapVertex* V=g_UniversalMap.Vert+VertexIndex;	
	OneMapVertex V0=*V;
	PrecompiledVertex PV;
	PV.CreateFromVertex(*V);
	SURF.FillVertexTextures(&PV);
	DestVertex=*V;
	*V=V0;
}
void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, AdditionalWeaponParams* AddParams);
void ChangeTerraType::OnStart(){
	if(StartEffect.WeaponToBorn.pchar()[0]&&rando()<StartEffect.WeaponBornProbability*328){
        CreateNewActiveWeapon(StartEffect.WeaponToBorn.pchar(),0xFFFF,CurVertex.x,CurVertex.y,CurVertex.z,0xFFFF,CurVertex.x,CurVertex.y,CurVertex.z+1000,NULL);
	}
}	
void ChangeTerraType::OnEndStay(){
	if(FinalEffect.WeaponToBorn.pchar()[0] && rando()<FinalEffect.WeaponBornProbability*328){
		CreateNewActiveWeapon(FinalEffect.WeaponToBorn.pchar(),0xFFFF,CurVertex.x,CurVertex.y,CurVertex.z,0xFFFF,CurVertex.x,CurVertex.y,CurVertex.z+1000,NULL);
	}
}
void TempTerraMorph::AddDeformator(OneVertexTempDeformator* D){
	OneVertexTempDeformator* df;
	for(int i=0;i<Deformations.GetAmount();i++)if( (df=Deformations[i])->VertexIndex==D->VertexIndex){
		if(df->ForeverChange)D->StartVertex=df->CurVertex;
		else D->StartVertex=df->StartVertex;		
		delete(df);
		Deformations[i]=D;
        return;
	}
	Deformations.Add(D);
}
void OneVertexTempDeformator::Init(int VertIndex){
	OneMapVertex* V=g_UniversalMap.Vert+VertIndex;	
	StartVertex=*V;
	CurVertex=*V;
	StartTime=PrevTime=ScaledGameTime;
	DestVertexDone=false;
	TimeToWait=0;
	TimeToTransformToFinalState=1000;
	TimeToStayInFinalState=2000;
	TimeToReturnInStartState=1000;
	VertexIndex=VertIndex;
	MaxFinalWeight=255;
	ForeverChange=false;
}
void TempTerraMorph::AddTerraTypeDeformator(int VertexIndex,int WaitTime,int ChangeTime,int StayTime,int UndoTime,int MaxWeight,bool Forever,EffectToBorn* start,EffectToBorn* final){
	OneMapVertex* V=g_UniversalMap.Vert+VertexIndex;
	ChangeTerraType* ctt=new ChangeTerraType;
	ctt->Init(VertexIndex);
	ctt->TimeToWait=WaitTime;
	ctt->TimeToTransformToFinalState=ChangeTime;
	ctt->TimeToStayInFinalState=StayTime;
	ctt->TimeToReturnInStartState=UndoTime;
	ctt->MaxFinalWeight=MaxWeight;
	ctt->ForeverChange=Forever;
	if(start){
        start->copyto(ctt->StartEffect);
	}
	if(final){
		final->copyto(ctt->FinalEffect);
	}
	AddDeformator(ctt);
}
void TempTerraMorph::AddTerraHeightDeformator(int VertexIndex,int WaitTime,int ChangeTime,int StayTime,int UndoTime,
											  Vector3D DCoor,Vector3D normal){
	OneMapVertex* V=g_UniversalMap.Vert+VertexIndex;
	ChangeTerraHeight* ctt=new ChangeTerraHeight;
	ctt->Init(VertexIndex);
	ctt->TimeToWait=WaitTime;
	ctt->TimeToTransformToFinalState=ChangeTime;
	ctt->TimeToStayInFinalState=StayTime;
	ctt->TimeToReturnInStartState=UndoTime;
	ctt->MaxFinalWeight=0xFF;
	ctt->ForeverChange=false;
	ctt->DCoor=DCoor;
	ctt->Normal=normal;
	AddDeformator(ctt);
}
void ChangeTerraHeight::CalcDestVertex(){
	OneMapVertex* V=g_UniversalMap.Vert+VertexIndex;	
	OneMapVertex V0=*V;	
	if(V->z>5){
		V->x+=DCoor.x;
		V->y+=DCoor.y;
		V->z+=DCoor.z;
	}
	V->nx=Normal.x*127;
	V->ny=Normal.y*127;
	V->nz=Normal.z*127;

	PrecompiledVertex PV;
	PV.CreateFromVertex(*V);
	SURF.FillVertexTextures(&PV);
	DestVertex=*V;
	*V=V0;    
}
int GetScaledGameTime();
void TempTerraMorph::Process(){
    ScaledGameTime=GetScaledGameTime();
	for(int i=0;i<Deformations.GetAmount();i++){
		OneVertexTempDeformator* d=Deformations[i];
		OneMapVertex* V=g_UniversalMap.Vert+d->VertexIndex;
		int dt=ScaledGameTime-d->StartTime;
		if(dt>d->TimeToWait){
			int dtp=d->PrevTime-d->StartTime;
			int tt=d->TimeToWait;
			if(dtp<=d->TimeToWait){
				d->OnStart();	
			}
            tt+=d->TimeToTransformToFinalState;			
			if(dtp<=tt && dt>tt){
				d->OnBeginToStay();
			}
			tt+=d->TimeToStayInFinalState;
			if(dtp<=tt && dt>tt){
				d->OnEndStay();
			}
			tt+=d->TimeToReturnInStartState;
			if(dtp<=tt && dt>tt){
				d->OnEnd();
			}			
			if(!d->DestVertexDone){
				d->CalcDestVertex();
				d->DestVertexDone=true;
			}
			d->PerformDeformation();
			if(memcmp(&d->CurVertex,V,sizeof OneMapVertex)){
				*V=d->CurVertex;
				Rct r(V->x-64,V->y-64,128,128);
                ITerra->InvalidateGeometry(&r);
			}
			if(dt>tt){
				delete(d);
				Deformations[i]=NULL;
				Deformations.Del(i,1);
				i--;
				d=NULL;
			}
		}
		if(d)d->PrevTime=ScaledGameTime;
	}
}
TempTerraMorph TMORPH;
void ProcessTerramorph(){
	TMORPH.Process();
}
DWORD fnRecreateLandsEnumerator(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	if(PrevCallHash!=LAYERSLIST.Layers.GetAmount()){
		for(int i=0;i<LAYERSLIST.Layers.GetAmount();i++)if(LAYERSLIST.Layers[i]->ThisLayerDeterminesRace){
			out->add(LAYERSLIST.Layers[i]->Name.pchar(),i,NULL);
		}
	}
	return LAYERSLIST.Layers.GetAmount();
}
void CreateLandLayersEnumerator(){
    RefreshableEnumerator* RE=new RefreshableEnumerator(fnRecreateLandsEnumerator);
    Enumerator* E=ENUM.Get("LANDLAYERS");
	E->SetProcEnum(RE);
}

//////////////////////////////////////////////////////////////////////////
/////////////////////     TREES TRANSFORMATION     ///////////////////////
//////////////////////////////////////////////////////////////////////////
class OneTreeInSequence:public BaseClass{
public:
	int TreeType;
	SAVE(OneTreeInSequence);
		REG_ENUM(_index,TreeType,TREESTYPES);
	ENDSAVE;
	virtual bool ForceSimplification(){return true;}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="%$TreeType$%";
		return true;
	}
};
class xTreesList:public ClonesArray<OneTreeInSequence>{
public:
	virtual bool ForceSimplification(){return true;}
};
class TreesSequence:public BaseClass{
public:
	int Nation;
	xTreesList Trees;
	SAVE(TreesSequence);
		REG_CLASS(OneTreeInSequence);
		REG_ENUM(_index,Nation,LANDLAYERS);
		REG_AUTO(Trees);
	ENDSAVE;
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="%$Nation$% : %$Trees,,$%";
		return true;
	}
};
class TreesExchangeList:public BaseClass{
public:
	ClonesArray<TreesSequence> TreesGroup;
	SAVE(TreesExchangeList);
		REG_CLASS(TreesSequence);
		REG_AUTO(TreesGroup);
	ENDSAVE;
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="%$TreesGroup,/$%";
		return true;
	}
};
class OneWeaponToChangeTree:public BaseClass{
public:
	int LandType;
    _str WeaponToChangeTree;
	Delimiter D;
	SAVE(OneWeaponToChangeTree);
		REG_ENUM(_index,LandType,LANDLAYERS);
		REG_ENUM(_strindex,WeaponToChangeTree,WeaponModificatorEnum);
		REG_AUTO(D);
	ENDSAVE;
	virtual bool ForceSimplification(){return true;}
};
class TreesExchangeInfo:public BaseClass{
public:
	ClonesArray<OneWeaponToChangeTree> WeaponsToChangeTrees;
	ClonesArray<TreesExchangeList> TreesExchange;
	SAVE(TreesExchangeInfo);
		REG_AUTO(WeaponsToChangeTrees);
		REG_AUTO(TreesExchange);
		REG_CLASS(OneWeaponToChangeTree);
	ENDSAVE;
};
TreesExchangeInfo TreesExchangeScope;
bool ChangeTreeType(OneSprite* OS,int LandType){
    int Type=OS->SGIndex;
	for(int i=0;i<TreesExchangeScope.TreesExchange.GetAmount();i++){
		TreesExchangeList* TX=TreesExchangeScope.TreesExchange[i];
		for(int j=0;j<TX->TreesGroup.GetAmount();j++){
			TreesSequence* TS=TX->TreesGroup[j];
			for(int k=0;k<TS->Trees.GetAmount();k++){
				if(TS->Trees[k]->TreeType==Type){
					if(LandType!=TS->Nation){
						for(int q=0;q<TX->TreesGroup.GetAmount();q++){
							if(TX->TreesGroup[q]->Nation==LandType){
								TreesSequence* TS=TX->TreesGroup[q];
								if(TS->Trees.GetAmount()){
									int id=TS->Trees[rando()%TS->Trees.GetAmount()]->TreeType;
									OS->SGIndex=id;
									OS->OC=OS->SG->Objects[id];
									return true;
								}
							}
						}
					}
					return false;
				}
			}
		}
	}
	return false;
}
void ChangeTreesInRadius(int x,int y,int R,int LandType){
	itr_GetSpritesInRadius.Create(x,y,R+1);
	while(OneSprite* OS=itr_GetSpritesInRadius.Next()){
		ChangeTreeType(OS,LandType);
	}
}
void ChangeTreesInRadiusWithEffect(int x,int y,int R,int LandType){
	itr_GetSpritesInRadius.Create(x,y,R+1);
	char* name=NULL;
	for(int i=0;i<TreesExchangeScope.WeaponsToChangeTrees.GetAmount();i++){
		if(TreesExchangeScope.WeaponsToChangeTrees[i]->LandType==LandType){
			name=TreesExchangeScope.WeaponsToChangeTrees[i]->WeaponToChangeTree.pchar();
			break;
		}
	}
	while(OneSprite* OS=itr_GetSpritesInRadius.Next()){
		if(name)CreateNewActiveWeapon(name,0xFFFF,OS->x,OS->y,OS->z,0xFFFF,OS->x,OS->y,OS->z+100,NULL);
		else ChangeTreeType(OS,LandType);
	}
}
void RegTreesExchangeEditor(){
	REG_CLASS(TreesExchangeList);
	TreesExchangeScope.SafeReadFromFile("Surface\\TreesExchange.xml");
	AddStdEditor("TreesExchangeEditor",&TreesExchangeScope,"Surface\\TreesExchange.xml",RCE_DEFAULT);
}

class SurfFuncForMC:public CubeTriangulator{
public:
	BaseSurfaceFunction* BF;
	int CellSize;
	virtual float GetValueF(float x,float y,float z){
		OneMapVertex MV;
		x*=CellSize;
		y*=CellSize;
		z*=CellSize;
		MV.x=x;
		MV.y=y;
		MV.z=z;
        BF->ModifyVertex(MV);		
		float v=MV.z-z;
		GeneratorSurfaceFunction* GS=(GeneratorSurfaceFunction*)BF;
		if(x<1024 || y<1024 || x>GS->MapLx-1024 || y>GS->MapLy-1024){			
			int xx=x;
			int yy=y;
            GS->ShapeOfBound->ShiftXY(xx,yy);
			static float sq=-1600;
			float dz=z/4+z*z/sq;
			float v1=128-xx-dz;
			float v2=128-yy-dz;
			float v3=xx-GS->MapLx+200-dz;
			float v4=yy-GS->MapLy+200-dz;
            v1=min(-v1,-v2);
			v3=min(-v3,-v4);
			v1=min(v1,v3);
			v=min(v,v1);
		}
		return v;
	}
};
void VertexBasedMesh::ApplyMCubes(int CellSize,int Nx,int Ny,BaseSurfaceFunction* Func){
	WholeTriangulation* WT=new WholeTriangulation;
	SurfFuncForMC* MC=new SurfFuncForMC;
	MC->CellSize=CellSize;
	MC->BF=Func;
	float sq6=sqrt(6.0f);
	float sq2=sqrt(2.0f);
	float sq3=sqrt(3.0f);
	Matrix3D M(2/sq6,0,1/sq3,-1/sq6,1/sq2,1/sq3,-1/sq6,-1/sq2,1/sq3);
	Matrix4D M4=M;
	MC->SetTransform(M4,false);
	MC->GetFullTriangilationRecursive(5,5,0 , -15,-15,-12 , Nx+5,Ny+5,35 , *WT);
	Free();
	for(int i=0;i<WT->HVert.numElem();i++){
		OneMapVertex MV;
		tagVector& tv=WT->HVert.elem(i);
		MV.x=tv.vec.x*CellSize;
		MV.y=tv.vec.y*CellSize;
		MV.z=tv.vec.z*CellSize;
		MV.Color=0xFF808080;
		MV.nFacture=0;
		AddVertex(MV);
	}
	for(int i=0;i<WT->Idxs.size();i++){
		AddIndex(WT->Idxs[i]);
	}
	for(int i=0;i<Vert.GetAmount();i++)
	{
		Func->ShiftVertex(Vert[i]);
	}
}