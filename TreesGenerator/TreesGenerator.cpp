#include "stdafx.h"
#include "kIO.h"
#include "TreesGenerator.h"

#define VERTEX VertexBump
#define vfVERTEX vfVertexBump

void A(){
	cSplineTCB3 sp;	
	sp.AddValue(0,cVec3(0,0,0));
	sp.AddValue(1,cVec3(0,0,1));
	sp.GetCurrentValue(0.5);	
}
float GetRandomValueF(float V1,float V2,int& Seed);
//float FindCentralPoint(
void CreateFractalSpline(cSplineTCB3& sp,cVec3 Start,cVec3 End,float Amplitude,float Decrement,int StartDepth,int Seed){
    Amplitude*=0.125;
    sp.AddValue(0,Start,0.0,0.0);
	sp.AddValue(1,End,0.0,0.0);
    float R0=Start.Distance(Start,End);
    for(int i=1;i<=StartDepth;i++){
        float T=float(i)/(StartDepth+1);
        cVec3 cpos=sp.GetCurrentValue(T);
        cVec3 dpos(GetRandomValueF(-1,1,Seed),GetRandomValueF(-1,1,Seed),GetRandomValueF(-1,1,Seed));
        dpos.Normalize();
        dpos.z=0;
        dpos*=Amplitude*R0;
        dpos+=Start+(End-Start)*T;
        sp.AddValue(T,dpos,0.0,0.0);
    }
    int n=2+StartDepth;    
    cList<cVec3> vec;
    cList<float> Tm;
    for(int i=0;i<6 && Amplitude>0.05;i++){
        vec.Clear();
        Tm.Clear();
        for(int j=1;j<n;j++){
            cVec3 V1=sp.GetValue(j-1);
            cVec3 V2=sp.GetValue(j);
            float T1=sp.GetTime(j-1);
            float T2=sp.GetTime(j);
            float T=(T1+T2)/2;
            cVec3 Dir=sp.GetCurrent1stDerivative(T);
            Dir.Normalize();
            cVec3 cpos=sp.GetCurrentValue(T);
            cVec3 dpos(GetRandomValueF(-1,1,Seed),GetRandomValueF(-1,1,Seed),GetRandomValueF(-1,1,Seed));
            dpos.Normalize();
            dpos-=Dir*dpos.Dot(dpos,Dir);
            cpos+=dpos*Amplitude*R0;
            vec.Add(cpos);
            Tm.Add(T);            
        }
        for(int j=0;j<n-1;j++){
            sp.AddValue(Tm[j],vec[j],0.0,0.0);
        }
        n+=n-1;
        Amplitude*=Decrement;
    }
    //light smoothing
    vec.Clear();
    Tm.Clear();
    n=sp.GetValuesCount();
    for(int i=0;i<n;i++){
        vec.Add(sp.GetValue(i));
        Tm.Add(sp.GetTime(i));
    }
    for(int i=0;i<20;i++){
        for(int j=1;j<n-1;j++){
            vec[j]=(vec[j]+(vec[j-1]+vec[j+1])*0.05)/1.1;
        }
    }
    sp.Free();
    for(int i=0;i<n;i++){
        sp.AddValue(Tm[i],vec[i]);        
    }
}

Vector3D ParametricBranch::GetInterpPoint(float T){
    for(int i=1;i<NP;i++){
        if(T<Line[i].T){
            float T0=Line[i-1].T;
            float T1=Line[i].T;
            Vector3D V1=Line[i-1].Pos;
            Vector3D V2=Line[i].Pos;
            V2-=V1;
            T-=T0;
            T1-=T0;
            return V1+V2*(T/T1);
        }            
    }
    return Line[0].Pos;
}
float GetPointToLineDist3D(Vector3D P,Vector3D P1,Vector3D P2,float& Weight){
    P-=P1;
	P2-=P1;
    float r=P.norm();
	float r12=P2.norm();
	if(r12>0){		
		float sp=P2.dot(P)/r12/r12;
		if(sp<0){
			Weight=0;
			return r;
		}
		if(sp>1){
			Weight=1;
			return P.distance(P2);
		}
		Weight=sp;
		Vector3D PC=P2*sp;
		return PC.distance(P);
	}
	Weight=0.5;
	return r;
}
float ParametricBranch::GetDistToTube(Vector3D P,float& T){
	float minD=1e20;
	T=0;
	for(int i=1;i<Line.GetAmount();i++){
		float W;
		float D=GetPointToLineDist3D(P,Line[i-1].Pos,Line[i].Pos,W);
		if(D<minD){
			minD=D;
			T=Line[i-1].T+W*(Line[i].T-Line[i-1].T);
		}
	}
    return minD;
}
bool ParametricBranch::CheckIfPointInsideTube(Vector3D V){
    float T;
	float D=GetDistToTube(V,T);
	return (D<GetRadiusScale(T,0,L)*R*0.98f);
}


void ParametricBranch::Clear(){
    Line.Clear();
}
void ParametricBranch::CreateLine(float AngleThreshold,int StartSubDiv,float Length,float Radius,int NSegments){    
    Line.Clear();
    if(StartSubDiv==0)StartSubDiv=1;
    //create start division
    Init(Length);
    for(int i=0;i<=StartSubDiv;i++){
        branch_point p;
        p.T=float(i)/StartSubDiv;
        p.Pos=GetPoint(p.T,Length);
        p.Radius=GetRadiusScale(p.T,0,Length);
        Line.Add(p);
    }    
    NP=Line.GetAmount();
    R=Radius;
    L=Length;
    NS=NSegments;
    Line[Line.GetAmount()-1].Radius=0;
    //simplification
    float min_ang;
    float ang_threshold=1-cos(AngleThreshold);
    do{
        min_ang=1000;
        int bv=-1;
        for(int i=1;i<NP-1;i++){            
            branch_point& bpp=Line[i-1];
            branch_point& bpc=Line[i];
            branch_point& bpn=Line[i+1];
            Vector3D vn=bpn.Pos-bpc.Pos;
            vn.normalize();
            Vector3D vp=bpc.Pos-bpp.Pos;
            vp.normalize();
            float a=1-vp.dot(vn);
            Vector3D RP(bpp.T*L,bpp.Radius*Radius,0);
            Vector3D RC(bpc.T*L,bpc.Radius*Radius,0);
            Vector3D RN(bpn.T*L,bpn.Radius*Radius,0);
            vp=RC-RP;
            vp.normalize();
            vn=RN-RC;
            vn.normalize();
            float a1=(1-vp.dot(vn))*4;
            a=max(a1,a);
            if(a<min_ang){
                min_ang=a;
                bv=i;
            }            
        }
        if(bv!=-1){
            Line.Del(bv,1);
            NP--;
        }
    }while(min_ang<ang_threshold);
}
int  ParametricBranch::GetRequiredVerts(){
    return NP*(NS+1)+(NS>=6?NS*4:0);
}
int  ParametricBranch::GetRequiredIdxs(){
    return ((NP-1)*NS*2+NS)*3+(NS>=6?NS*18:0);
}
void ParametricBranch::CreateGeometry(BaseMesh& DestBM,Matrix4D& Transform,int NSegments,int NTexCycles,int TextureID){
    NS=NSegments;
    int nv=GetRequiredVerts();
    int ni=GetRequiredIdxs();
	DestBM.create(nv,ni,vfVERTEX);    
    VertexBump* NV=(VERTEX*)DestBM.getVertexData();
    word* idxs=DestBM.getIndices();
    int tLx=IRS->GetTextureWidth(TextureID);
    int tLy=IRS->GetTextureHeight(TextureID);
    if(tLx==0)tLx=1;
    if(tLy==0)tLy=1;
    float R0=GetRadiusScale(0,0,L)*R;
    float coef0=float(NTexCycles*L*tLx)/(tLy*2*c_PI);
    //verts generating
	float v0=0;
    Vector3D T0=Line[0].Pos;
    Vector3D T1=Line[NP-1].Pos;
    T1-=T0;
    T1.normalize();
    Vector3D T2;
    T2.cross(T1,Vector3D(1,0,0));
    if(T2.norm()<0.00001){
        T2.cross(T1,Vector3D(0,1,0));
    }
    T1=T2;
    Vector3D PrevZeroPt(0,0,0);
    int NSec=NS;
    int NSec0=NS;
    int NSec1=NS;

    if(CurrentLOD<2 && CurrentLevel==0){
        NSec0=NS*4;
        NSec1=NS*2;
    }
    for(int i=0;i<NP;i++){        
        if(i==0)NSec=NSec0;
        else
        if(i==1)NSec=NSec1;
        else NSec=NS;
        branch_point& pt=Line[i];
        float T=pt.T;
        Vector3D dir;
        if(i==0)dir=Line[i+1].Pos-pt.Pos;
        else if(i==NP-1)dir=pt.Pos-Line[i-1].Pos;
        else dir=Line[i+1].Pos-Line[i-1].Pos;
		float dist;

		if(i<NP-1)dist=(Line[i+1].Pos-pt.Pos).normalize();
        else dist=dir.normalize();

        Matrix3D mr;
        mr.rotation(dir,2*c_PI/NSec);        
        Matrix3D mrc=Matrix3D::identity;
        float RL=1;        
        Vector3D vv0;            
        vv0.cross(T1,dir);        
        vv0.normalize();
        for(int j=0;j<=NSec;j++){
            float A=float(j)/NSec;
            float RD=GetRadiusScale(T,A,L);
			RL=RD*R;            
            Vector3D v=vv0;
            v*=RL;            
            v*=mrc;
            mrc*=mr;
            Vector3D n=v;
            n.normalize();
            Transform.transformVec(n);
            v+=pt.Pos;
            Transform.transformPt(v);
            NV->x=v.x;
            NV->y=v.y;
            NV->z=v.z;
            NV->u=A*NTexCycles;
            NV->v=v0;//2*T*coef0;//(2*i+(i&1))*coef0;

            NV->nx=n.x;
            NV->ny=n.y;
            NV->nz=n.z;

			NV->tx=dir.x;
			NV->ty=dir.y;
			NV->tz=dir.z;

			Vector3D vb;
			vb.cross(n,dir);

			NV->bx=vb.x;
			NV->by=vb.y;
			NV->bz=vb.z;

            NV++;
        }	
		v0+=coef0*dist/RL/L;
    }
	//triangles generating
    int cv=0;
    for(int i=0;i<NP-1;i++){
        if(i<2 && NSec0!=NS){
            int NS2=(NS<<(2-i))+1;
            int NSC=NS<<(1-i);
            for(int j=0;j<NSC;j++){
                int j2=j<<1;                
                idxs[0]=cv+j2;
                idxs[2]=cv+(j2+1);
                idxs[1]=cv+j+NS2;
                idxs+=3;                
                idxs[0]=cv+(j2+1);
                idxs[2]=cv+(j2+2);
                idxs[1]=cv+NS2+j+1;
                idxs+=3;
                idxs[0]=cv+j+NS2;
                idxs[2]=cv+NS2+j+1;
                idxs[1]=cv+j2+1;
                idxs+=3;
            }
            cv+=NS2;
        }else{
            for(int j=0;j<NS;j++){
                idxs[0]=cv+j;
                idxs[2]=cv+(j+1);
                idxs[1]=cv+j+NS+1;
                idxs+=3;
                if(i<NP-2){
                    idxs[0]=cv+(j+1);
                    idxs[2]=cv+NS+1+(j+1);
                    idxs[1]=cv+j+NS+1;
                    idxs+=3;
                }
            }
            cv+=NS+1;
        }        
    }
    DestBM.setNVert(nv);
    DestBM.setNInd(ni);
    DestBM.setNPri(ni/3);
    DestBM.setShader(IRS->GetShaderID("tr"));
    DestBM.setTexture(TextureID);
}

void TestTree(){
	if(GetKeyState(VK_CONTROL)<0){
		ParametricBranch PB;
		
		PB.CreateLine(0.1,60,850,50,8);
		BaseMesh* BM=new BaseMesh;
		Matrix4D M;
		M.translation(1500,1500,50);
		PB.CreateGeometry(*BM,M,8,2,IRS->GetTextureID("RoadGOROD.tga"));    
		DrawBM(*BM);
		delete(BM);
	}else{
		ParametricBranchWithStartAngle PB;
		PB.BlendLength=180+80*cos(float(GetTickCount())/220.0f);	
		PB.StartDirection=Vector3D(0,0,1);
		PB.Transform.srt(1,Vector3D(1,0,0),1.5*cos(float(GetTickCount())/300.0),Vector3D(1000,1000,60));

		PB.CreateLine(0.1,60,850,50,8);
		BaseMesh* BM=new BaseMesh;
		Matrix4D M;
		M.translation(1500,1500,50);
		PB.CreateGeometry(*BM,M,8,2,IRS->GetTextureID("RoadGOROD.tga"));    
		DrawBM(*BM);
		delete(BM);
	}
}
//noise generator
float StartNoiseTable[8192];
bool nInitDone=false;

float GetSimpleNoise1D(float T){
	if(!nInitDone){
		for(int i=0;i<8192;i++){
			StartNoiseTable[i]=float(rand())/32768.0f;
		}
		nInitDone=true;
	}	
	float tt=floorf(T);
	int p=int(tt);
	float t=T-tt;
	float w1=(1-t)*(1-t);
	float w3=t*t;
	float w2=1-w1-w3;
	float f1=StartNoiseTable[p&8191];
	float f2=StartNoiseTable[(p+1)&8191];
	float f3=StartNoiseTable[(p+2)&8191];
	float f0=StartNoiseTable[(p+8191)&8191];
	return (f1+t*(f2-f0)/2)*w1+(f1+t*(f2-f1))*w2+(f2+(1-t)*(f1-f3)/2)*w3;
}
float GetFractalNoise1D(float T,float FractalSharpness,int Seed){
	float f=0;
	float coef=1;
	for(int i=0;i<5;i++){
		f+=GetSimpleNoise1D(T+Seed)*coef;
		coef*=FractalSharpness/2;
		Seed+=50;
		T*=2;
	}
	return f;
}
int GetRandomValue(int V1,int V2,int& Seed){
    Seed+=(Seed%9973)*7963+2693;
    return V1+((V2-V1)*(DWORD(Seed)%19339)/19338);
}
float GetRandomValueF(float V1,float V2,int& Seed){
    Seed+=(Seed%9973)*7963+2693;
    return V1+((V2-V1)*(DWORD(Seed)%19339)/19338.0f);
}

OneTreeBranch PB;
void TestT2(){    
    PB.BlendLength=180+80*cos(float(GetTickCount())/220.0f);	
    PB.StartDirection=Vector3D(0,0,1);
    PB.Transform.srt(1,Vector3D(0,1,0),0.2*cos(float(GetTickCount())/300.0),Vector3D(1000,1000,60));
    PB.CreateLine(0.1,60,400,150,8);
    BaseMesh* BM=new BaseMesh;
    Matrix4D M;
    M.translation(1500,1500,50);
    PB.CreateGeometry(*BM,M,8,2,IRS->GetTextureID("RedOakBark.tga"));    
    DrawBM(*BM);
    delete(BM);
}
void OneTreeBranch::Init(float Length){
    Spline.Free();
    CreateFractalSpline(Spline,cVec3(0,0,0),cVec3(0,0,Length),NoiseDegree,NoiseSharpness,NoiseFrequency,ShapeSeed);
}

Vector3D OneTreeBranch::GetOrgPoint(float T,float Length){
    cVec3 v=Spline.GetCurrentValue(T);    
    return Vector3D(v.x,v.y,v.z);        
}
float OneTreeBranch::GetRadiusScale(float T,float Angle,float Length){
    if(T>0.9999)return 0;
    T=T*0.85f;
    if(T>1)T=1;
    if(T<0)T=0;
    float rb=(1-T*T)*BranchToRootRelativeSize;
    if(T<RelativeRootLength){
        float v=1-T/RelativeRootLength;
		float A=Angle-floorf(Angle);
		rb+=v*v*(1-BranchToRootRelativeSize)*(1+RootCurvatureAmplitude*4*(GetSimpleNoise1D(A*RootCurvatureFrequency*4)+GetSimpleNoise1D((1-A)*RootCurvatureFrequency*4)-0.8));
    }
    rb*=RootRadiusRelativeToParent;
    return rb;
}

int WholeTree::GlobalSeed=0;

void WholeTree::GenerateMesh(BaseMesh& bm,BaseMesh& bbm,float RootRadius){
	DynArray<BaseMesh*> TempBM;//all branches meshes
	DynArray<BaseMesh*> TempBBM;//billboards meshes
	GlobalSeed=0;	
	//initial transform for the root branch	
    int tnv=0;//total vertices
    int tni=0;//total indices
	int bbtnv=0;//total billboard vertices
	int bbtni=0;//total billboard indices
	for(int i=0;i<Branches.GetAmount();i++){
		OneTreeBranch& TB=*Branches[i];
        TB.CurrentLevel=0;
        TB.CurrentLOD=LOD;
		if(TB.Enabled){
			BaseMesh* B=new BaseMesh;
			BaseMesh* BB=new BaseMesh;
			B->create(1,1,vfVERTEX);
			BB->create(1,1,vfVertex2t);
            TB.StartDirection=Vector3D(0,0,1);            
            TB.Transform=Matrix4D::identity;
            Matrix4D mi=Matrix4D::identity;
			//generating mesh tor the current branch
			TB.GenerateMesh(*B,*BB,mi,RootRadius,NULL);
            TempBM.Add(B);
			TempBBM.Add(BB);                
            tnv+=B->getNVert();
            tni+=B->getNInd();
			bbtnv+=BB->getNVert();
			bbtni+=BB->getNInd();            
		}
	}
	bm.create(tnv,tni,vfVERTEX);
	bbm.create(bbtnv,bbtni,vfVertex2t);
    for(int i=0;i<TempBM.GetAmount();i++){
        //branches geometry
        BaseMesh* B=TempBM[i];
		if(B->getNInd()){
			bm.setShader(B->getShader());
			bm.setTexture(B->getTexture());
			bm+=*B;
		}
        //billboards
		BaseMesh* BB=TempBBM[i];
		if(BB->getNInd()){
			bbm.setShader(BB->getShader());
			bbm.setTexture(BB->getTexture());
			bbm+=*BB;
		}
        //normalizing branches geometry
        int n=bbm.getNVert();
        if(n){
            Vertex2t* vt=(Vertex2t*)bbm.getVertexData();
            Vector3D _min(100000.0f,100000.0f,100000.0f);
            Vector3D _max(-100000.0f,-100000.0f,-100000.0f);
            Vector3D cm(0,0,0);
            for(int i=0;i<n;i++){                
                Vertex2t* vr=vt+i;
                Vector3D v(vr->x,vr->y,vr->z);
                _min.x=min(_min.x,v.x);
                _min.y=min(_min.x,v.y);
                _min.z=min(_min.x,v.z);
                _max.x=max(_max.x,v.x);
                _max.y=max(_max.x,v.y);
                _max.z=max(_max.x,v.z);
                cm+=v;
            }
            cm/=n;
            Vector3D L=_max-_min;
            for(int i=0;i<n;i++){
                Vertex2t* vr=vt+i;
                Vector3D v(vr->x,vr->y,vr->z);
                v-=cm;
                v.x/=L.x;
                v.y/=L.y;
                v.z/=L.z;
                v+=Vector3D(0.5,0.5,0.5);
                clamp(v.x,0.0f,1.0f);
                clamp(v.y,0.0f,1.0f);
                clamp(v.z,0.0f,1.0f);
                vr->diffuse=(vr->diffuse&0xFF000000)+(DWORD(v.x*255)<<16)+(DWORD(v.y*255)<<8)+DWORD(v.z*255);
            }                        
        }
		delete(TempBM[i]);
		delete(TempBBM[i]);
    }
}
void OneTreeBranch::GenerateMesh(BaseMesh& bm,BaseMesh& bbm,Matrix4D& Transform,float RootRadius,OneTreeBranch* NextB){
	//initialising seeds
    int __ShapeSeed=ShapeSeed+GetRandomValue(0,3796,WholeTree::GlobalSeed);
    int __AngleSeed=AngleSeed+GetRandomValue(0,3796,WholeTree::GlobalSeed);
    int __ProportionSeed=ProportionSeed+GetRandomValue(0,3796,WholeTree::GlobalSeed);
    int __DividingSeed=DividingSeed+GetRandomValue(0,3796,WholeTree::GlobalSeed);
    int __BillboardSeed=BillboardSeed+GetRandomValue(0,3796,WholeTree::GlobalSeed);

	//length of branch
    float L=RootRadius*GetRandomValue(MinProportion,MaxProportion,__ProportionSeed);
    BlendLength=RelativeRootLength*L;
	//initialising gravity
	Gravity=GravityFactor;
	
    //applying LOD-s 

    float CurvThreshold=CurvingThresholdForSimplification;
    //CurvThreshold*=pow(1.2,CurrentLOD);
    int CurSides=Sides;
    CurSides-=CurrentLOD;    
    if(CurSides<3)CurSides=3;

    //creating axe for the branch
    CreateLine(CurvThreshold*c_PI/180,80,L,RootRadius,Sides);
	if( (!(DisableBranchGeometry && EnableBillboards)) && (CurrentLOD<2 || CurrentLevel<2)){
	    CreateGeometry(bm,Transform,CurSides,1,BarkTexture);
	}    
	if(EnableBillboards){        
		//billboard center		
		Vector3D bp=Line[Line.GetAmount()-1].Pos;
		Transform.transformPt(bp);
		bbm.create(4,6,vfVertex2t);
		Vertex2t* vt=(Vertex2t*)bbm.getVertexData();
		word* idxs=bbm.getIndices();
		//billboard consists of 2 triangles
		idxs[0]=0;
		idxs[1]=1;
		idxs[2]=2;
		idxs[3]=1;
		idxs[4]=3;
		idxs[5]=2;
		float u0,v0,du,dv;
		if(NPictPerBillboardX && NPictPerBillboardY){
			u0=float(GetRandomValue(0,NPictPerBillboardX,__BillboardSeed))/NPictPerBillboardX;
			v0=float(GetRandomValue(0,NPictPerBillboardY,__BillboardSeed))/NPictPerBillboardX;
			du=1.0f/NPictPerBillboardX;
			dv=1.0f/NPictPerBillboardY;
		}else{
			u0=0;
			v0=0;
			du=1;
			dv=1;
		}
		float sz= GetRandomValueF(BillboardMinSize,BillboardMaxSize,__BillboardSeed)*BillboardScale;
		IRS->SetColorConst(BillboardColor);
		for(int dy=-1;dy<=1;dy+=2){
			for(int dx=-1;dx<=1;dx+=2){
				//center of billboard
				vt->x=bp.x;
				vt->y=bp.y;
				vt->z=bp.z;				
				vt->u=u0+du*(dx+1)/2;
				vt->v=v0+dv*(dy+1)/2;
				//u2,v2 means size of billboard in screen plane
				float a=GetRandomValueF(-0.2,0.2,__BillboardSeed);
				float _cos=cos(a);
				float _sin=sin(a);
				vt->u2=(dx*_cos+dy*_sin)*sz;
				vt->v2=(dy*_cos-dx*_sin)*sz;
				vt->diffuse=BillboardColor;
				vt++;
			}
		}
		bbm.setNVert(4);
		bbm.setNInd(6);
		bbm.setNPri(2);
		bbm.setShader(IRS->GetShaderID("trees_bb_leafs"));
		bbm.setTexture(BillboardTextureID);
	}
	//generating branches positions and directions for the next stage
	for(int i=0;i<SubBranches.GetAmount();i++){
		NextB=SubBranches[i];
		//creating branches
		int n=GetRandomValue(NextB->MinDivisions,NextB->MaxDivisions,__DividingSeed);        
		for(int i=0;i<=n && i<20;i++){
			//regular positions
			float pos;
			float de=NextB->DividingEndPosition;
			if(de>0.95)de=0.95;
            if(n!=0)pos=NextB->DividingStartPosition+(de-NextB->DividingStartPosition)*i/n;
            else pos=GetRandomValueF(DividingStartPosition,DividingEndPosition,__DividingSeed);
			Vector3D V1=GetInterpPoint(pos-0.0001);
			Vector3D V2=GetInterpPoint(pos+0.0001);
			V2-=V1;
			V2.normalize();//current direction of the branch
			Vector3D V=V2;
			Vector3D VD=V2;
			VD.z=0;
			if(VD.x==0 && VD.y==0){
				//the special case when vector is (0,0,1)
				VD.x=1;
			}
			float lxy=VD.norm();
			VD.normalize();//direction of branch in XY plane
			Vector3D VO;
			VO.cross(V2,VD);
			VO.normalize();//this vector is normal to current branch direction - we will use it like axe of rotation for next-level branch
			float ang0=atan2(lxy,V2.z);//start polar angle
			if(NextB->InversedAlignmentToParent)V*=-1;
			int na=GetRandomValue(NextB->MinBranchesOnEachDivision,NextB->MaxBranchesOnEachDivision,__DividingSeed);        
			for(int j=0;j<na&&j<10;j++){
				float da=c_PI*2/na;
				float angle1=j*da+GetRandomValueF(-da/2,da/2,__AngleSeed);//axial angle - almost regular and lays in i-th sector				
				float rs=GetRadiusScale(pos,angle1/2/c_PI,L);
				float angle2=ang0+GetRandomValueF(NextB->StartAngleToParent,NextB->FinalangleToParent,__AngleSeed)*c_PI;//polar angle
				Matrix4D MA1;
				MA1.rotation(V2,angle1);//axial
				Matrix4D MA2;
				MA2.rotation(VO,angle2);//polar
				MA2*=MA1;
				MA2.translate(V1);//translate
				MA2*=Transform;

				BaseMesh* B=new BaseMesh;
				BaseMesh* BB=new BaseMesh;
				B->create(1,1,vfVERTEX);
				BB->create(1,1,vfVertex2t);

				NextB->StartDirection=V;
				NextB->Transform=MA2;
                
				Matrix4D mi=Matrix4D::identity;
                NextB->CurrentLOD=CurrentLOD;
                NextB->CurrentLevel=CurrentLevel+1;
				NextB->GenerateMesh(*B,*BB,mi,rs*RootRadius,NextB);
				
				int nv=B->getNVert();
				char* tempV=new char[nv];
				memset(tempV,0,nv);
				VERTEX* vrt=(VERTEX*)B->getVertexData();
				OneTreeBranch* TBP=this;
				for(int q=0;q<nv;q++){
					Vector3D p(vrt->x,vrt->y,vrt->z);					
					if(!TBP->CheckIfPointInsideTube(p))tempV[q]=1;
					vrt++;
				}
				int ni=B->getNInd();
				int nt=ni/3;
				char* tempT=new char[nt];
				memset(tempT,0,nt);
				word* idxs=B->getIndices();
				int ntt=0;//number of visible triangles
				for(int q=0,idx=0;q<nt;q++,idx+=3){
					if(tempV[idxs[idx]]==1 || tempV[idxs[idx+1]]==1 || tempV[idxs[idx+2]]==1){
						tempT[q]=1;//triange is at least partially visible
						if(tempV[idxs[idx  ]]==0)tempV[idxs[idx  ]]=2;
						if(tempV[idxs[idx+1]]==0)tempV[idxs[idx+1]]=2;
						if(tempV[idxs[idx+2]]==0)tempV[idxs[idx+2]]=2;
						int nti=ntt*3;
						idxs[nti  ]=idxs[idx  ];
						idxs[nti+1]=idxs[idx+1];
						idxs[nti+2]=idxs[idx+2];
						ntt++;
					}
				}
				int* encod=new int[nv];
				memset(encod,0xFF,4*nv);
				int ntv=0;//number of remaining verts
				vrt=(VERTEX*)B->getVertexData();
				for(int q=0;q<nv;q++)if(tempV[q]){
					encod[q]=ntv;
					vrt[ntv++]=vrt[q];
				}
				//reindexing the triangles
				int nti=ntt*3;
				for(int q=0;q<nti;q++){
					idxs[q]=encod[idxs[q]];
					assert(idxs[q]>=0 && idxs[q]<ntv);
				}

				B->setNVert(ntv);
				B->setNInd(nti);
				B->setNPri(ntt);

				delete[]tempV;
				delete[]tempT;

				bm+=*B;
				bbm+=*BB;
				bbm.setTexture(BB->getTexture());
                
				delete B;
				delete BB;
			}
		}
	}	
}
WholeTree WT;
BaseMesh* B=NULL;
BaseMesh* BBM=NULL;
void DrawTreesEditor(){    
    if(GetKeyState(VK_SCROLL)){
        Matrix4D M;
        M.translation(2000,2000,50);
        IRS->SetWorldTM(M);
        if(B)DrawBM(*B);   
        if(BBM){
            IRS->SetShader( IRS->GetShaderID("trees_bb_leafs") );
            IRS->SetShaderAutoVars();
            IRS->SetTexture( BBM->getTexture( 0 ), 0, false );
            IRS->SetTexture( BBM->getTexture( 1 ), 1, false );
			BBM->setShader(IRS->GetShader());
            for(int i=0;i<2;i++){
                IRS->SetShader( IRS->GetShaderID("trees_bb_leafs"),i );
                IRS->SetShaderAutoVars();
                IRS->SetTexture( BBM->getTexture( 0 ), 0, false );
                IRS->SetTexture( BBM->getTexture( 1 ), 1, false );
                DrawPrimBM( *BBM );
            }
        }        
    }else{
        if(B)delete(B);
		if(BBM)delete(BBM);
        B=new BaseMesh;
		BBM=new BaseMesh;
        WT.GenerateMesh(*B,*BBM,30);
        Matrix4D M;
        M.translation(2000,2000,50);
        IRS->SetWorldTM(M);
        DrawBM(*B);
        for(int i=0;i<2;i++){
            IRS->SetShader( IRS->GetShaderID("trees_bb_leafs"),i );
            IRS->SetShaderAutoVars();
            IRS->SetTexture( BBM->getTexture( 0 ), 0, false );
            IRS->SetTexture( BBM->getTexture( 1 ), 1, false );
            DrawPrimBM( *BBM );
        }
    }
    if(B)GameInterface::PrintInfoMessage(100,"V=%d P=%d BB=%d",B->getNVert(),B->getNPri(),BBM?BBM->getNVert()/4:0);
    if( B && BBM && GetKeyState('S')<0 ){
        //saving geometry
        BaseMesh* BT[2];
        BT[0]=B;
        BT[1]=BBM;
        char cc[256];
        int p=0;
        CreateDirectory("models\\trees\\",0);
        CreateDirectory("models\\trees\\generate\\",0);
        do{
            sprintf(cc,"models\\trees\\generate\\tree%03d.c2m",p++);
            FILE* F=fopen(cc,"r");
            if(F){
                fclose(F);
            }else{
                IMM->SaveMeshes(cc,&BT[0],2);
                break;
            }
        }while(p<1000);
    }    
}
void OneTreeBranch::Randomize(){
    ShapeSeed=rand();
    RootCurvatureSeed=rand();
    BillboardSeed=rand();
    DividingSeed=rand();
    ProportionSeed=rand();
    AngleSeed=rand();
    for(int i=0;i<SubBranches.GetAmount();i++){
        SubBranches[i]->Randomize();
    }
}
void CallTreesEditor(){
    REG_CLASS(OneTreeBranch);
    WT.SafeReadFromFile("tree.xml");
    GameInterface::EditClass("WholeTree",&WT,"tree.xml",RCE_RIGHT_POSITION | RCE_EXITONENTER | RCE_EXITONESCAPE | RCE_HIDEHEADER | RCE_ALLOW_GAME_PROCESSING | RCE_SHOW_GAME_BACKGROUND | RCE_DOUBLEPANEL21,0,0);
    WT.WriteToFile("tree.xml");
    UNREG_CLASS("OneTreeBranch");
}
void WholeTree::AutoSaveToC2M(){
    BaseMesh* BT[2];
    BT[0]=B;
    BT[1]=BBM;
    char cc[256];
    int p=0;
    CreateDirectory("models\\trees\\",0);
    CreateDirectory("models\\trees\\generate\\",0);
    do{
        sprintf(cc,"models\\trees\\generate\\tree_%04d.c2m",p++);
        FILE* F=fopen(cc,"r");
        if(F){
            fclose(F);
        }else{
            IMM->SaveMeshes(cc,&BT[0],2);
            break;
        }
    }while(p<10000);
}
void WholeTree::ReCreateTree(){
    if(B)delete(B);
	if(BBM)delete(BBM);
    B=new BaseMesh;
	BBM=new BaseMesh;
    GenerateMesh(*B,*BBM,30);    
}
void WholeTree::GenerateRandom(){
    for(int i=0;i<Branches.GetAmount();i++){
        Branches[i]->Randomize();
    }
    ReCreateTree();
}
_str wtFileName;
void WholeTree::SaveParametersToXML(){
	if(GameInterface::ShowSaveFileDialog(wtFileName,"*.tree.xml","")){
		WriteToFile(wtFileName.cpchar());
	}
}
void WholeTree::LoadParametersFromXML(){
	if(GameInterface::ShowOpenFileDialog(wtFileName,"*.tree.xml","")){
		reset_class(this);
		SafeReadFromFile(wtFileName.cpchar());
	}    
}