#include "stdheader.h"
extern IRenderSystem*	IRS;

class OneParticle{
	int ExistingTime;
	int PrevTime;
	int MaxA;
public:
	bool visible;
	BaseMesh* BM;
	OneParticle();
	~OneParticle();
	void CreatePatch(Vector3D rc,float Lx,float Ly,int N1,int N2,Vector3D axe,float angle);
	virtual bool ProcessVertices();
	virtual bool TryToCreateRandomParticle();	
	virtual void Draw();
	virtual OneParticle* New();
};
class ParticlesContainer{
	DynArray<OneParticle*>Samples;
    DynArray<int> Attempts;
	BaseMesh* GMesh;
public:
	DynArray<OneParticle*> Particles;
    virtual void Process();
	virtual void Draw();
    ParticlesContainer();
	~ParticlesContainer();
};
void OneParticle::CreatePatch(Vector3D rc,float Lx,float Ly,int N1,int N2,Vector3D axe,float angle){
	if(BM)delete(BM);
	BM=new BaseMesh;
	Vector3D lt(-Lx/2,-Ly/2,0);
	Vector3D rt(Lx/2,-Ly/2,0);
	Vector3D lb(-Lx/2,Ly/2,0);
	Matrix3D R;
	R.rotation(axe,angle);
	lt.mulLeft(R);
	rt.mulLeft(R);
	lb.mulLeft(R);		
	lt+=rc;
	rt+=rc;
	lb+=rc;
	BM->createPatch(N1,N2,lt,rt,lb);
}
void GetFlowSpeed(int x,int y,float& vx,float& vy);
#define EXIST_TIME 4000
#define RISE_TIME 700
bool OneParticle::ProcessVertices(){
	//floating over the water
	int T0=GetTickCount();
	int A=255;
	if(ExistingTime<RISE_TIME){
		A=ExistingTime*255/RISE_TIME;
	}
	if(ExistingTime>EXIST_TIME){
		A=255-(ExistingTime-EXIST_TIME)*255/RISE_TIME;
		if(A<0)return false;
	}
	A=A*MaxA/255;
	A=(A>>3)+4;
	A+=16;
	visible=1;//A>24;
	DWORD Diff=(DWORD(A)<<24)+0xFFFFFF;
	float T=float(T0-PrevTime)/9000.0;
	if(BM){
		ExistingTime+=T0-PrevTime;
		Vertex2t* V=(Vertex2t*)BM->getVertexData();
		int N=BM->getNVert();
		int Ni=BM->getNInd();
		word* idx=BM->getIndices();
		int maxr=128;
		for(int j=1;j<Ni;j++){
			int r=Norma(V[idx[j]].x-V[idx[j-1]].x,V[idx[j]].y-V[idx[j-1]].y);
			if(r>maxr)maxr=r;
		}
		Diff=(DWORD(A*128/maxr)<<24)+0xFFFFFF;
		for(int i=0;i<N;i++){
			float vx,vy;
			GetFlowSpeed(V->x,V->y,vx,vy);
			float dvx=float(rand()-16384)/36000.0f;
			float dvy=float(rand()-16384)/36000.0f;
			V->x-=vx*T*256.0+dvx;
			V->y-=vy*T*256.0+dvy;			
			V->u2=V->u;
			V->v2=V->v;
			V->diffuse=Diff;
			V++;
		}
	}
	PrevTime=T0;
	return true;
}
int GetHeight(int,int);
bool OneParticle::TryToCreateRandomParticle(){
	int x0=(mapx<<5)-128;
	int y0=(mapy<<5)-128;
	int Lx=(smaplx<<5)+256;
	int Ly=(smaply<<5)+256;
	int x=x0+((int(rand())*Lx)>>15);
	int y=y0+((int(rand())*Ly)>>15);
	int H=GetHeight(x,y);
	if(H<0){
		float vx,vy;
		GetFlowSpeed(x,y,vx,vy);
		float v=sqrt(vx*vx+vy*vy);
		int r=v*4000;
		if(r>rand()){
			float ang=atan2(-vy,-vx);
            CreatePatch(Vector3D(x,y,0.1f),32,160,2,2,Vector3D(0,0,6),ang+float(rand()%100-50)/300.0f);
			Vertex2t* V=(Vertex2t*)BM->getVertexData();
			int NV=BM->getNVert();
			for(int j=0;j<NV;j++){
				V->x+=(rand()%20)-10;
				V->y+=(rand()%20)-10;
				V++;
			}
			BM->setTexture(IRS->GetTextureID("Soplia.tga"));
			BM->setTexture(IRS->GetTextureID("Soplia.tga"),1);
			BM->setShader(IRS->GetShaderID("Sopli"));
			PrevTime=GetTickCount();
			MaxA=255-(rand()&180);
			ExistingTime=0;
			return true;
		}
	}
	return false;
}
void OneParticle::Draw(){
	if(BM){
		DrawBM(*BM);
	}
}
OneParticle::OneParticle(){
	BM=NULL;
}
OneParticle::~OneParticle(){
	if(BM)delete(BM);
}
OneParticle* OneParticle::New(){
	return new OneParticle;
}
ParticlesContainer::ParticlesContainer(){
	GMesh=NULL;
}
ParticlesContainer::~ParticlesContainer(){
	Samples.Clear();
	Attempts.Clear();
    Particles.Clear();
}

#define ADDCLASS(x,N) {\
	x* OP=new x;\
	Samples.Add(OP);\
	Attempts.Add(N);\
}
void ParticlesContainer::Process(){
	if(!Samples.GetAmount()){
		ADDCLASS(OneParticle,10);
	}
	for(int i=0;i<Samples.GetAmount();i++){
		int N=Attempts[i];
		for(int j=0;j<N;j++){
			if(Samples[i]->TryToCreateRandomParticle()){
				Particles.Add(Samples[i]);
				Samples[i]=Samples[i]->New();
			}
		}
	}
	for(int i=0;i<Particles.GetAmount();i++){
		if(!Particles[i]->ProcessVertices()){
			delete(Particles[i]);
			Particles.Del(i,1);
			i--;
		}
	}
}
void ParticlesContainer::Draw(){
	if(!GMesh){
		GMesh=new BaseMesh;
		GMesh->create(20000,60000);
	}
	GMesh->setNVert(0);
	GMesh->setNInd(0);
	GMesh->setNPri(0);

	for(int i=0;i<Particles.GetAmount();i++)if(Particles[i]->visible){
		BaseMesh* BM=Particles[i]->BM;
		int NV=BM->getNVert();
		int NI=BM->getNInd();
		if(GMesh->getNVert()+NV>=GMesh->getMaxVert()||GMesh->getNInd()+NI>=GMesh->getMaxInd()){
			DrawBM(*GMesh);
			GMesh->setNVert(0);
			GMesh->setNInd(0);
		}
		int NV0=GMesh->getNVert();
		int NI0=GMesh->getNInd();
		Vertex2t* V0=(Vertex2t*)GMesh->getVertexData();

		memcpy(V0+NV0,BM->getVertexData(),NV*sizeof Vertex2t);

		word* idx0=GMesh->getIndices();
		word* idx1=BM->getIndices();
		
		for(int j=0;j<NI;j++)idx0[j+NI0]=idx1[j]+NV0;

		GMesh->setNInd(GMesh->getNInd()+BM->getNInd());
		GMesh->setNVert(GMesh->getNVert()+NV);
		GMesh->setNPri( (GMesh->getNInd()+BM->getNInd())/3 );

		GMesh->setShader(BM->getShader());
		GMesh->setTexture(BM->getTexture());

	}
	DrawBM(*GMesh);
	GMesh->setNVert(0);
	GMesh->setNInd(0);
	GMesh->setNPri(0);
}
ParticlesContainer PCONTAINER;
void ProcessParticles(){
	PCONTAINER.Draw();
	PCONTAINER.Process();
}
void DrawParticles(){
	PCONTAINER.Draw();
}