#include "stdafx.h"
#include "IMediaManager.h"
#include "sg.h"
#include "sgAnimationBlock.h"
#include "sgPhysics.h"
#include "sgPRSAnimation.h"
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
float OneBody::TimeStep=0.005;
bool CheckIfFileExists(char* Name);
void SplitModel(int id,int& dst_model,int& dst_anim,int NClusters,float RotCoef,float SpeedCoef,float PlaneZPos){
	char dir	[_MAX_PATH];
	char drive	[_MAX_PATH];
	char file	[_MAX_PATH];
	char ext	[_MAX_PATH];
	_splitpath( IMM->GetModelFileName(id), drive, dir, file, ext );
	char _model[512];
	char _anim [512];
	sprintf(_model,"%s%s%s_crash.c2m",drive,dir,file);
	sprintf(_anim ,"%s%s%s_crash_anim.c2m",drive,dir,file);

	if(GetKeyState(VK_CONTROL) & 0x8000){
		if(CheckIfFileExists(_model) && CheckIfFileExists(_anim)){
			dst_model=IMM->GetModelID(_model);
			dst_anim=IMM->GetModelID(_anim);
			return;
		}
	}else{
		dst_model=IMM->GetModelID(_model);
		dst_anim=IMM->GetModelID(_anim);
		return;
	}



	BaseMesh* BM=IMM->GetGeometry(id);
	if(!BM)return;
	
	SNode* pModel = IMM->GetModelRoot( id );
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
			for(int i=0;i<nv;i++)if(v[i]==clust){
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
			AABB_vol[clust]=V.x*V.y*V.z;			
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
	do{
		changeAABB=false;
		for(int i=0;i<Nclust;i++)if(nvrt[i]){
			AABoundBox A(AABB_min[i],AABB_max[i]);
			for(int j=0;j<Nclust;j++)if(nvrt[j] && i!=j){
				AABoundBox B(AABB_min[j],AABB_max[j]);
				if(A.Overlap(B)){
					/*
					float* V1[]={&A.maxv.x,&A.maxv.y,&A.maxv.z,&A.minv.x,&A.minv.y,&A.minv.z};
					float* V2[]={&B.minv.x,&B.minv.y,&B.minv.z,&B.minv.x,&B.minv.y,&B.minv.z};
					float* V3[]={&B.maxv.x,&B.maxv.y,&B.maxv.z,&B.maxv.x,&B.maxv.y,&B.maxv.z};
					float d=FLT_MAX;
					float R=0;
					int p=-1;
					for(int w=0;w<6;w++){
						if(*V1[w]>*V2[w] && *V1[w]<*V3[w]){
							float R0=w<3 ? fabs(*V1[w]-*V2[w]) : fabs(*V1[w]-*V3[w]);
							float dd=R0/(*V3[w]-*V2[w]);
							if(dd<d){
								R=R0;
								d=dd;
								p=w;
							}
						}
					}
					if(p>=0){
						R*=0.49;
						if(R<0.1)R=-0.01;
						if(p<3){
							float cp=(*V1[p]+*V2[p])/2;
							*V1[p]=cp+R;
							*V2[p]=cp-R;
						}else{
							float cp=(*V1[p]+*V3[p])/2;
							*V1[p]=cp-R;
							*V3[p]=cp+R;
						}
						changeAABB=true;
					}
					*/
                    Vector3D L=A.maxv;
					L-=A.minv;
					Vector3D C=A.maxv;
					C+=A.minv;
					C/=2;
					L*=0.99/2;
					A.minv=C;A.minv-=L;
					A.maxv=C;A.maxv+=L;
					changeAABB=true;
				}
			}
			if(changeAABB){
				AABB_min[i]=A.minv;
				AABB_max[i]=A.maxv;
			}
		}
	}while( changeAABB && (niter++)<1000 );
	/*
    int NSph=2000;
	int step=nv/NSph;
	int NR=0;
	for(int i=0;i<nv;i+=step){
		Vector3D vc=Vector3D(V[i].x,V[i].y,V[i].z);
		float minRi=FLT_MAX;
		float minRo=FLT_MAX;
		for(int j=0;j<nv;j+=step)if(i!=j){
			Vector3D vn=Vector3D(V[j].x,V[j].y,V[j].z);
			float R=vc.distance(vn);
			if(v[i]==v[j]){
				if(R<minRi)minRi=R;
			}else{
				if(R<minRo)minRo=R;
			}
		}
		ptRadius[i]=min(2.5f,minRo/2);
		if(ptRadius[i]>2)NR++;
	}
	*/
	//physics init	
	PhysicsSystem& PS=PhysicsSystem::inst();
	PS.SetWorldScale(0.01f*100/top);
	PS.SetFixedStep(OneBody::TimeStep);
	CollidePlane Ground;
	Ground.SetPlane(Plane(Vector3D(0,0,PlaneZPos),Vector3D(0,0,1)));
	CollideHashSpace SP;
	SP.SetBounce(0.01);
	SP.SetFriction(20);
	SP.SetSoftness(0.0001);
	SP.AddChild(&Ground);
	
	OneBody** Parts=(OneBody**)malloc(Nclust*4);
	memset(Parts,0,4*Nclust);
	for(int i=0;i<Nclust;i++){
		if(nvrt[i]){
			OneBody* OBD=Parts[i]=new OneBody;
			//using boxes
			Vector3D SZ=AABB_max[i];
			SZ-=AABB_min[i];

			if(SZ.x<=0)SZ.x=0.1;
			if(SZ.y<=0)SZ.y=0.1;
			if(SZ.z<=0)SZ.z=0.1;

			CollideBox* CB=new CollideBox;			
			CB->SetExtents(SZ);
			OBD->Colliders.push_back(CB);
            OBD->B.SetMass(nvrt[i]);
			CB->AttachToBody(&OBD->B);
			SP.AddChild(CB);

			
			Vector3D cm=AABB_min[i];
			cm+=AABB_max[i];
			cm/=2;
			Matrix4D M=Matrix4D::identity;
			M.setTranslation(cm);
			OBD->TN.SetTransform(M);
			OBD->B.SetBone(&OBD->TN);
			Vector3D RV(rand()%100-50,rand()%100-50,rand()%50);
			RV.normalize();
			RV/=30;
			RV*=SpeedCoef;
			OBD->B.CreateBody();
			OBD->B.SetVelocity(RV);
			RV=Vector3D(rand()%100-50,rand()%100-50,rand()%50);
			RV.normalize();
			RV/=2;
			RV*=RotCoef;
			OBD->B.SetAngVelocity(RV);

			/*
			for(int p=0;p<nv;p+=step)if(v[p]==i && ptRadius[p]>2){
				CollideSphere* CS=new CollideSphere;
				CS->SetRadius(ptRadius[p]/1.01);
				Matrix4D TRM=Matrix4D::identity;
				Vector3D TR(V[p].x,V[p].y,V[p].z);
				TR-=cm;
                TRM.setTranslation(TR);                
				CS->SetLocalTM(TRM);
				CS->AttachToBody(&OBD->B);
				SP.AddChild(CS);
			}
			OBD->TN.SetTransform(M);
			OBD->B.SetBone(&OBD->TN);
			*/	
            
		}
	}
	int AnimTime=10000;
	int NS=int(AnimTime/1000/OneBody::TimeStep);
	int T0=GetTickCount();
	for(int i=0;i<NS;i++){	
		try{
			for(int j=0;j<Nclust;j++)if(Parts[j]){
				Parts[j]->B.Render();
			}
			SP.Render();
			PS.SetStepMode(smQuickStep);
			PS.Render();
			float maxd=0;
			for(int j=0;j<Nclust;j++)if(Parts[j]){
				Parts[j]->TransformHistory.push_back(Parts[j]->TN.GetTransform());			
			}		
		}catch (...) {
			break;
		}
		if(GetTickCount()-T0>20000)break;
	}
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
					PRS->AddKey(OneBody::TimeStep*q*1000,MA);
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
