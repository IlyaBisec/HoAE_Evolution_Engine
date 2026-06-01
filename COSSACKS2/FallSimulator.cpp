#include "stdafx.h"
#include "IMediaManager.h"
#include "sg.h"
#include "sgAnimationBlock.h"
#include "sgPhysics.h"
#include "kHash.hpp"
#include "sgPRSAnimation.h" 
struct Dipole{
	Vector3D Start;
	Vector3D End;
};

const float flt_limit=0.0001f;
const float wire_radius=10.0f;
const float spring_factor=500;
const float Mu_factor=1.0f;
const float Reflect_coef=0.01;

float GetDistanceBetweenLines(Dipole Line1,Dipole Line2,Dipole& Nearest){
	Nearest.Start=Line1.Start;
	Nearest.End=Line2.Start;
	return Line1.Start.distance(Line2.Start);

	Vector3D D=Line2.Start;
	D-=Line1.Start;
	Vector3D V1=Line1.End;
	V1-=Line1.Start;
	Vector3D V2=Line2.End;
	V2-=Line2.Start;
	float v1x2=V1.norm2();
	float v2x2=V1.norm2();
	float v12 =V1.dot(V2);
	float det =v1x2*v2x2-v12*v12;
	float t1=0,t2=0;
	float v1D=V1.dot(D);
	float v2D=V2.dot(D);
	if(fabs(det)>flt_limit){
		t1=v1x2>flt_limit?t1=(v1D*v2x2-v12*v2D)/det:0;
		t2=v2x2>flt_limit?t2=(v12*v1D-v2D*v1x2)/det:0;
		if(t1<0||t1>1){
			clamp(t1,0.0f,1.0f);
			t2=v2x2>flt_limit?(v12*t1-v2D)/v2x2:0;
			clamp(t2,0.0f,1.0f);
		}else
		if(t2<0||t2>1){
			clamp(t2,0.0f,1.0f);
			t1=v1x2>flt_limit?(v12*t2+v1D)/v1x2:0;
			clamp(t1,0.0f,1.0f);
		}
	}else{
		float t1_0=v1x2>flt_limit?v1D/v1x2:0;
		float t1_1=v1x2>flt_limit?(v12+v1D)/v1x2:0;
		if(t1_0<0 && t1_1<0){
            t1=0;
			t2=t1_0>t1_1?0:1;
		}else 
		if(t1_0>1 && t1_1>1){
			t1=1;
			t2=t1_0<t1_1?0:1;
		}else{
			clamp(t1_0,0.0f,1.0f);
			clamp(t1_1,0.0f,1.0f);
			t1=(t1_0+t1_1)/2;
			t2=v2x2>flt_limit?(v12*t1-v2D)/v2x2:0;
			clamp(t2,0.0f,1.0f);
		}
	}
	Nearest.Start=Line1.End*t1;
	Nearest.Start+=Line1.Start;
	Nearest.End=Line2.End*t2;
	Nearest.End+=Line2.Start;
    return Nearest.Start.distance(Nearest.End);
}
class OneRigidBody;
class LinearPrimitive{
public:
	Dipole   Local;
	Dipole   Global;
	Vector3D Speed;
	OneRigidBody* Owner;	
	LinearPrimitive* Next;
};
struct LP_cell{
	int cx,cy,cz;
	LinearPrimitive* First;
	unsigned int	hash	() const {
		return cx + cy*1371 + cz*197651;
	}
	bool			equal	( const LP_cell& el ){
		return cx==el.cx && cy==el.cy && cz==el.cz;
	}
	void			copy	( const LP_cell& el ){
		cx=el.cx;
		cy=el.cy;
		cz=el.cz;
		First=el.First;
	}

};
#define HashCellSize 4
class PositionHash{
	Hash<LP_cell> p_Hash;
public:
	void Clear(){
		p_Hash.reset();
	}
	void AddPrimitive(LinearPrimitive* LP){
        int cx=LP->Global.Start.x/HashCellSize;
		int cy=LP->Global.Start.y/HashCellSize;
		int cz=LP->Global.Start.z/HashCellSize;
		LP_cell c;
		c.cx=cx;c.cy=cy;c.cz=cz;
		int idx=p_Hash.find(c);
		LP_cell* cur;
		if(idx==-1){
            c.First=LP;
			LP->Next=NULL;
            p_Hash.add(c);
		}else{
			cur=&p_Hash.elem(idx);
			LP->Next=cur->First;
			cur->First=LP; 
		}
	}
	LinearPrimitive* GetFirst(int cx,int cy,int cz){
		LP_cell c;
		c.cx=cx;c.cy=cy;c.cz=cz;
		int idx=p_Hash.find(c);
		if(idx!=-1){
			LP_cell* c=&p_Hash.elem(idx);
			return c->First;
		}else return NULL;
	}
};

class OneRigidBody{
public:
    static PositionHash ph;
	OneRigidBody(){
		mass=1;
		Inertia=1;
		Pos=Matrix4D::identity;
		AngleRotSpeed=Vector3D((rand()%10-5)/10.0f,(rand()%10-5)/10.0f,(rand()%10)/10.0f);
		Speed=Vector3D((rand()%10-5)/3.0f,(rand()%10-5)/3.0f,(rand()%5)/3.0f);
		SummF=Vector3D(0,0,0);
		SummM=Vector3D(0,0,0);
	}
	float    mass;
	float    Inertia;//simple, not accurate
	Matrix4D Pos;
	Vector3D AngleRotSpeed;
	Vector3D Speed;
	Vector3D SummF;
	Vector3D SummM;
	std::vector<LinearPrimitive> Prims;
	void CalcInertia(){
		Inertia=0;
		int np=Prims.size();
		if(np){
			float m0=mass/np;			
			for(int i=0;i<Prims.size();i++){
                Inertia+=m0*Prims[i].Local.Start.norm2()/3;
			}
		}
	}
	float CalcGlobals(float time){//returns max shift dist
		float d=0;
		for(int i=0;i<Prims.size();i++){
			Vector3D V0=Prims[i].Global.Start;
			Prims[i].Global.Start=Prims[i].Local.Start;
			Pos.transformPt(Prims[i].Global.Start);
			Prims[i].Global.End=Prims[i].Local.End;
			Pos.transformPt(Prims[i].Global.End);
			Prims[i].Speed=Prims[i].Global.Start;
			Prims[i].Speed-=V0;
			if(time==0)Prims[i].Speed*=time;
			else Prims[i].Speed/=time;			
			float dd=Prims[i].Global.Start.distance(V0);
			if(dd>d)d=dd;
		}
		//ph.Clear();
		for(int i=0;i<Prims.size();i++){
			ph.AddPrimitive(&Prims[i]);
		}
		return d;
	}
	float EstimateCalcProcess(float time){
		Vector3D dv=SummM;
		dv*=time;dv/=Inertia;
		Vector3D tmpAngleRotSpeed=AngleRotSpeed;
		tmpAngleRotSpeed+=dv;
		return tmpAngleRotSpeed.norm();
	}
	void CalcProcess(float time){
		Vector3D dv=SummF;
		dv*=time;dv/=mass;
		Speed+=dv;
		dv=SummM;
		dv*=time;dv/=Inertia;
		AngleRotSpeed+=dv;
		Matrix3D R;
		Vector3D W=AngleRotSpeed;
		W.normalize();
		R.rotation(W,AngleRotSpeed.norm()*time);
		Vector3D P0(Pos.e30,Pos.e31,Pos.e32);
		Pos.e30=Pos.e31=Pos.e32=0;
		Pos*=R;
		Pos.e30=P0.x;
		Pos.e31=P0.y;
		Pos.e32=P0.z;
		W=Speed;
		W*=time;
		Pos.translate(W);
	}
	void InitForces(){
		SummF=Vector3D(0,0,0);
		SummM=Vector3D(0,0,0);
	}	
	void CollideWith(){
		for(int i=0;i<Prims.size();i++){
			Vector3D v=Prims[i].Global.Start;
			int cx=v.x/HashCellSize;
			int cy=v.y/HashCellSize;
			int cz=v.z/HashCellSize;
			LinearPrimitive* CP=&Prims[i];
			for(int dx=-1;dx<=1;dx++){
				for(int dy=-1;dy<=1;dy++){
					for(int dz=-1;dz<=1;dz++){
						LinearPrimitive* LP=ph.GetFirst(cx+dx,cy+dy,cz+dz);
						while(LP){
							if(LP->Owner!=this){
								Dipole Contact;
								float R=GetDistanceBetweenLines(CP->Global,LP->Global,Contact);
								if(R<wire_radius){
									Vector3D pt=Contact.Start;
									pt+=Contact.End;
									pt/=2;
									float deep=wire_radius-R;
									Vector3D Force=Contact.Start;
									Force-=Contact.End;
									Force.normalize();								

									//tangent speed
									Vector3D speed=CP->Speed;
									speed-=LP->Speed;
									Vector3D tangent_speed=speed;
									float dpr=speed.dot(Force);
									Vector3D ff=Force;
									ff*=dpr;
									tangent_speed-=ff;

									//softness
									if(Force.dot(speed)>0)Force*=Reflect_coef*5;
									Force*=deep*spring_factor/2;						

									//friction
									float ts=tangent_speed.norm();
									if(ts>0.001)ts=0.001;
									tangent_speed.normalize();
									tangent_speed*=-Mu_factor*Force.norm()*ts/0.001;
									Force+=tangent_speed;

									SummF+=Force;

									Vector3D ptc=pt;
									ptc-=Vector3D(Pos.e30,Pos.e31,Pos.e32);
									Vector3D c;
									c.cross(ptc,Force);
									SummM+=c;									
								}								
							}
							LP=LP->Next;
						}
					}
				}
			}			
		}
	}
	void CollideWithZPlane(float z){
		for(int i=0;i<Prims.size();i++){
			LinearPrimitive* CP=&Prims[i];
			float z1=CP->Global.Start.z;
			float z2=CP->Global.End.z;
			float zmin=min(z1,z2);
			if(zmin<z){
                Vector3D pt=Prims[i].Global.Start;
				pt+=Prims[i].Global.End;
				pt/=2;
				Vector3D Force(0,0,(z-zmin)*spring_factor);

				Vector3D Fn=Force;
				Fn.normalize();								

				//tangent speed
				Vector3D speed=CP->Speed;
				Vector3D tangent_speed=speed;
				float dpr=speed.dot(Fn);
				Vector3D ff=Fn;
				ff*=dpr;
				tangent_speed-=ff;

				//softness
				if(Force.dot(speed)>0)Force*=Reflect_coef;			

				//friction
				float ts=tangent_speed.norm();
				if(ts>0.001)ts=0.001;
				tangent_speed.normalize();
				tangent_speed*=-Mu_factor*Force.norm()*ts/0.001;
				Force+=tangent_speed;

                SummF+=Force;
				Vector3D ptc=pt;
				ptc-=Vector3D(Pos.e30,Pos.e31,Pos.e32);
				Vector3D c;
				c.cross(ptc,Force);
				SummM+=c;
			}
		}
	}
	void Init(){

	}
};

PositionHash OneRigidBody::ph;

class BodySystem{
public:
	BodySystem(){
        g=9.8;
		z_plane_pos=0;
	}
	std::vector<OneRigidBody*> Bodies;
	float g;
	float z_plane_pos;
	void OneStep(float time){		
		static int nsteps=10;
		float maxr=0;
		for(int q=0;q<nsteps;q++){
			for(int i=0;i<Bodies.size();i++){
				Bodies[i]->InitForces();
			}
			for(int i=0;i<Bodies.size();i++){			
				Bodies[i]->CollideWith();
				Bodies[i]->CollideWithZPlane(z_plane_pos);
				Bodies[i]->SummF+=Vector3D(0,0,-g*Bodies[i]->mass);
			}
			OneRigidBody::ph.Clear();			
			for(int i=0;i<Bodies.size();i++){
				float r=Bodies[i]->EstimateCalcProcess(time);
				if(r>maxr)maxr=r;
				Bodies[i]->CalcProcess(time/nsteps);
				Bodies[i]->CalcGlobals(time/nsteps);
			}
		}
		nsteps=1+int(maxr/0.15);
	}
};
BodySystem TestBS;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
struct PT_hash_cell{
	int cx,cy,cz;	
	unsigned int	hash	() const {
		return cx + cy*1371 + cz*197651;
	}
	bool			equal	( const PT_hash_cell& el ){
		return cx==el.cx && cy==el.cy && cz==el.cz;
	}
	void			copy	( const PT_hash_cell& el ){
		cx=el.cx;
		cy=el.cy;
		cz=el.cz;
	}
};

Hash<PT_hash_cell> Pts;
bool CheckPtInVHash(Vector3D v){
	PT_hash_cell PH;
	PH.cx=v.x;
	PH.cy=v.y;
	PH.cz=v.z;
	int i=Pts.find(PH);
	if(i!=-1)return false;
	Pts.add(PH);
	return true;
}


class OneBody{
public:
	Body B;
	TransformNode TN;
	std::vector<CollideGeom*> Colliders;
	std::vector<Matrix4D> TransformHistory;
	static float TimeStep;
	~OneBody(){
		for(int i=0;i<Colliders.size();i++){\
			if(Colliders[i])delete(Colliders[i]);
		}
	}
	OneBody(){};
};
bool CheckIfFileExists(char* Name);

void SplitModel1(int id,int& dst_model,int& dst_anim,int NClusters,float RotCoef,float SpeedCoef,float PlaneZPos){
	char dir	[_MAX_PATH];
	char drive	[_MAX_PATH];
	char file	[_MAX_PATH];
	char ext	[_MAX_PATH];
	_splitpath( IMM->GetModelFileName(id), drive, dir, file, ext );
	char _model[512];
	char _anim [512];
	sprintf(_model,"%s%s%s_crash.c2m",drive,dir,file);
	sprintf(_anim ,"%s%s%s_crash_anim.c2m",drive,dir,file);

	if(CheckIfFileExists(_model) && CheckIfFileExists(_anim)){
		dst_model=IMM->GetModelID(_model);
		dst_anim=IMM->GetModelID(_anim);
		return;
	}
	Pts.reset();

	BaseMesh* BM=IMM->GetGeometry(id);
	
	SNode* pModel = NodePool::GetNode( id );
	if (!pModel) return;
	const char* textureName = "";
	SNode::Iterator it( pModel );
	while (it)
	{
		SNode* pNode = (SNode*)*it;
		if (pNode->IsA<Texture>()){
			textureName = pNode->GetName();
			break;
		}
		++it;
	}

	int* v=new int[BM->getNVert()];
	memset(v,0xFF,BM->getNVert()*4);
	int nv=BM->getNVert();
	int ni=BM->getNInd();
	WORD* idxs=BM->getIndices();
	VertexN* V=(VertexN*)BM->getVertexData();	
	bool change=false;
	int Nclust=0;
	do{
		change=false;
		for(int i=0;i<nv;i++){
			if(v[i]==-1){
				v[i]=Nclust;
				change=true;
				break;
			}
		}
		if(change){
			bool ch=false;
			do{
				ch=false;
				int nvr=0;
				for(int i=0;i<ni;i+=3){
					if(v[idxs[i]]==Nclust || v[idxs[i+1]]==Nclust || v[idxs[i+2]]==Nclust){
						if(v[idxs[i]]!=Nclust){
							v[idxs[i]]=Nclust;
							nvr++;ch=true;
						}
						if(v[idxs[i+1]]!=Nclust){
							v[idxs[i+1]]=Nclust;
							nvr++;ch=true;
						}
						if(v[idxs[i+2]]!=Nclust){
							v[idxs[i+2]]=Nclust;
							nvr++;ch=true;
						}						
					}
				}
			}while(ch);
			Nclust++;
		}
	}while(change);
	Vector3D* AABB_min=new Vector3D[Nclust];
	Vector3D* AABB_max=new Vector3D[Nclust];
	float*    AABB_vol=new float[Nclust];
	Vector3D* pos=new Vector3D[Nclust];
	Vector3D* norm=new Vector3D[Nclust];
	int*      deltbl=new int[Nclust];
	int* nvrt=new int[Nclust];
	float* ptRadius=new float[nv];	
	for(int i=0;i<Nclust;i++){
		pos[i]=Vector3D(0,0,0);
		norm[i]=Vector3D(0,0,0);
		nvrt[i]=0;
		AABB_max[i]=Vector3D(-FLT_MAX,-FLT_MAX,-FLT_MAX);
		AABB_min[i]=Vector3D(FLT_MAX,FLT_MAX,FLT_MAX);
		deltbl[i]=1;
	}	
	for(int i=0;i<nv;i++){
		int c=v[i];
		AABB_max[c].x=max(AABB_max[c].x,V[i].x);
		AABB_max[c].y=max(AABB_max[c].y,V[i].y);
		AABB_max[c].z=max(AABB_max[c].z,V[i].z);
		AABB_min[c].x=min(AABB_min[c].x,V[i].x);
		AABB_min[c].y=min(AABB_min[c].y,V[i].y);
		AABB_min[c].z=min(AABB_min[c].z,V[i].z);
	}
	for(int i=0;i<Nclust;i++){
		Vector3D V=AABB_max[i];
		V-=AABB_min[i];
		AABB_vol[i]=V.x*V.y*V.z;
	}
	//deleting small clusters. We will try to decrease amount of clusters to Ndesired
	int nResClust=Nclust;
	do{
		//searching for a smallest cluster
		float minV=FLT_MAX;
		int clust=-1;
		for(int i=0;i<Nclust;i++)if(deltbl[i] && AABB_vol[i]<minV){
			minV=AABB_vol[i];
			clust=i;
		}
		deltbl[clust]=0;
		AABB_vol[clust]=0;
		Vector3D cMin=AABB_min[clust];
		Vector3D cMax=AABB_max[clust];
		float bestP=FLT_MAX;
		int bestIdx=-1;
		for(int i=0;i<Nclust;i++)if(deltbl[i]){
			Vector3D ccMin=AABB_min[i];
			Vector3D ccMax=AABB_max[i];
			ccMax.x=max(ccMax.x,cMax.x);
			ccMax.y=max(ccMax.y,cMax.y);
			ccMax.z=max(ccMax.z,cMax.z);
			ccMin.x=min(ccMin.x,cMin.x);
			ccMin.y=min(ccMin.y,cMin.y);
			ccMin.z=min(ccMin.z,cMin.z);
			ccMax-=ccMin;
			float newV=ccMax.x*ccMax.y*ccMax.z/AABB_vol[i];
			if(newV<bestP){
				bestP=newV;
				bestIdx=i;
			}
		}
		if(bestIdx!=-1){
			int i;
			for(i=0;i<nv;i++)if(v[i]==clust){
				v[i]=bestIdx;
			}
			Vector3D& ccMin=AABB_min[bestIdx];
			Vector3D& ccMax=AABB_max[bestIdx];
			ccMax.x=max(ccMax.x,cMax.x);
			ccMax.y=max(ccMax.y,cMax.y);
			ccMax.z=max(ccMax.z,cMax.z);
			ccMin.x=min(ccMin.x,cMin.x);
			ccMin.y=min(ccMin.y,cMin.y);
			ccMin.z=min(ccMin.z,cMin.z);
			Vector3D V=ccMax;
			V-=ccMin;
			AABB_vol[i]=V.x*V.y*V.z;			
			nResClust--;
		}else break;
	}while(nResClust>NClusters);

	Vector3D Center(0,0,0);
	float top=0;
	for(int i=0;i<nv;i++){
		int c=v[i];
		nvrt[c]++;
		pos[c]+=Vector3D(V[i].x,V[i].y,V[i].z);
		norm[c]+=Vector3D(V[i].nx,V[i].ny,V[i].nz);
		Center+=Vector3D(V[i].x,V[i].y,V[i].z);
		if(V[i].z>top)top=V[i].z;
	}    
	Center/=nv;
	for(int i=0;i<Nclust;i++)if(nvrt[i]){
		pos[i]/=nvrt[i];
		norm[i].normalize();
	}
	//shrinking AABB to avoid intersections
	bool changeAABB;
	int niter=0;
	
	//physics init	
	OneBody** Parts=(OneBody**)malloc(Nclust*4);
	memset(Parts,0,4*Nclust);
	int NN=0;
	for(int i=0;i<Nclust;i++){
		if(nvrt[i]){
			OneBody* OBD=Parts[i]=new OneBody;
			OneRigidBody* ORBD=new OneRigidBody;
			ORBD->mass=nvrt[i];
			ORBD->Pos=Matrix4D::identity;
			ORBD->Pos.setTranslation(pos[i]);
			if(NN<25){
				for(int j=0;j<nv;j++){
					if(v[j]==i){
						VertexN* V1=V+j;
						Vector3D S(V1->x,V1->y,V1->z);
						if(CheckPtInVHash(S)){
							S-=pos[i];						
							LinearPrimitive LP;
							LP.Local.Start=S;
							LP.Local.End=S;
							LP.Speed=Vector3D(0,0,0);
							LP.Owner=ORBD;
							ORBD->Prims.push_back(LP);
						}
					}
				}
				ORBD->CalcInertia();
				ORBD->CalcGlobals(0);   
				TestBS.Bodies.push_back(ORBD);
				NN++;
			}
		}
	}
	int AnimTime=10000;
	int NS=int(4*AnimTime/1000/OneBody::TimeStep);
	int T0=GetTickCount();
	for(int i=0;i<NS;i++){	
		TestBS.OneStep(OneBody::TimeStep*4);
		for(int j=0;j<Nclust;j++)if(Parts[j] && j<TestBS.Bodies.size()){
			Parts[j]->TransformHistory.push_back(TestBS.Bodies[j]->Pos);			
		}		
	}
	/*
	for(int j=0;j<Nclust;j++)if(Parts[j]){
		for(int i=Parts[j]->TransformHistory.size()-1;i>10;i--){
			Matrix4D M1=Parts[j]->TransformHistory[i];
			Matrix4D M2=Parts[j]->TransformHistory[i-1];
			Vector3D dV=Vector3D(M1.e30-M2.e30,M1.e31-M2.e31,M1.e32-M2.e32);
			float dd=dV.norm();
			if(dd<0.01){
                Parts[j]->TransformHistory.pop_back();
			}
		}
	}
	*/

    //saving clusters to temp model
	{		
		FOutStream model_stream(_model);
		FOutStream anm_stream(_anim);
		Group* G=new Group;//model
		AnimationBlock* AB=new AnimationBlock;//animation
		AB->SetAnimationTime(AnimTime);
		G->SetName( "building" );
		AB->SetName( "building" );

		G->AddChild<Texture>(textureName);
		G->AddChild<Shader>("2d3d");
		int cidx=0;
		for(int i=0;i<Nclust;i++){
			//calc amount of verts in cluster
			int nvc=0;
			int nidxc=0;
			for(int j=0;j<nv;j++)if(v[j]==i)nvc++;
			for(int j=0;j<ni;j++)if(v[idxs[j]]==i)nidxc++;
			assert(nidxc%3==0);
			if(nvc){
				cidx++;
				char cc[32];
				sprintf(cc,"c%d",cidx);
				TransformNode* tn=G->AddChild<TransformNode>(cc);
				PRSAnimation* PRS=AB->AddChild<PRSAnimation>(cc);
				int NN=Parts[i]->TransformHistory.size();
				for(int q=0;q<NN;q+=5){
					Matrix4D MA=Parts[i]->TransformHistory[q];
					Matrix4D Tr;
					Matrix4D Tri;
					Tr.translation(pos[i]);
					Tri.inverse(Tr);
					//MA*=Tr;
					MA.mulLeft(Tri);
					PRS->AddKey(OneBody::TimeStep*q*1000/4,MA);
				}
				PRS->ReduceKeys();
				//////////////////////////////////////////////////////////////////////////				
				//tn->AddChild<Shader>("2d3d");
				sprintf(cc,"geo%d",cidx);
				Geometry* geo=tn->AddChild<Geometry>(cc);
				//////////////////////////////////////////////////////////////////////////
				//tn->AddChild<Shader>("lines3d");				
				//sprintf(cc,"aabb%d",cidx);
				//Geometry* aabb=tn->AddChild<Geometry>(cc);
				//BaseMesh& abm=aabb->GetMesh();
                //abm.createAABB(AABoundBox(AABB_min[i],AABB_max[i]));
				//////////////////////////////////////////////////////////////////////////				
				BaseMesh& bm=geo->GetMesh();				
				bm.create(nvc,nidxc,BM->getVertexFormat());
				VertexN* curV=(VertexN*)bm.getVertexData();
				WORD* curI=bm.getIndices();
                int* cod=new int[nv];
				int nvcl=0;
				for(int j=0;j<nv;j++)if(v[j]==i){
                    cod[j]=nvcl;
					curV[nvcl]=V[j];
                    nvcl++;
				}
				int nicl=0;
				for(int j=0;j<ni;j++)if(v[idxs[j]]==i){
                    curI[nicl++]=cod[idxs[j]];
				}
				bm.setNInd(nicl);
				bm.setNVert(nvcl);
				bm.setNPri(nicl/3);
			}
		}
		G->SerializeSubtree(model_stream);
		AB->SerializeSubtree(anm_stream);
		CopyFile(_model,"Models\\test.c2m",false);
		CopyFile(_anim,"Models\\test_anim.c2m",false);
		model_stream.CloseFile();
		anm_stream.CloseFile();
		dst_model=IMM->GetModelID(_model);
		dst_anim=IMM->GetModelID(_anim);
	}
	delete[]AABB_vol;
	delete[]pos;
	delete[]norm;
	delete[]deltbl;
	delete[]nvrt;
	delete[]ptRadius;	
	delete[]AABB_min;
	delete[]AABB_max;
	delete[]v;
	for(int i=0;i<Nclust;i++)if(Parts[i])delete(Parts[i]);
	free(Parts);
}

