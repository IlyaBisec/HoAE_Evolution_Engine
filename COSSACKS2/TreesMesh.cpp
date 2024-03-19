#include "stdafx.h"
#include "DynArray.h"
#include "sg.h"
#include "sgNodePool.h"
#include "kHash.hpp"
#include "IMediaManager.h"
#include "sgDummy.h"
#include "sgModel.h"
#include "TreesMesh.h"
#include "IShadowManager.h"
#include "Scape3D.h"

extern ICamera*	ICam;
void TreesMeshCreator::AddTree(DWORD ID,DWORD State,Vector3D pos,int ModelID,DWORD Color,Matrix4D& transform){
	SummHash+=ID*State;
	IShadowMgr->AddCastingBoundaryPoint(pos);
	TreeHashElem TE;
	TE.UniqID=ID;
	int idx=TreesHash.find(TE);
	if(idx==-1){
		TE.Create(ID,State,pos,ModelID,Color,transform);
		idx=TreesHash.add(TE);
	}
	TreeHashElem& T=TreesHash.elem(idx);
	if(T.StateHashValue!=State){
		T.Create(ID,State,pos,ModelID,Color,transform);
	}
	T.SortValue=ICam->GetDir().dot(pos);
	TaskToDraw.Add(&T);
}
int compare_TH(const void * p1, const void * p2){
	return ((TreeHashElem*)((DWORD*)p1)[0])->SortValue<((TreeHashElem*)((DWORD*)p2)[0])->SortValue?1:-1;
}
void DrawTreesShadowMesh(){
	IRS->SetClipPlane( 0, Plane::xOy );
	TreesMesh.DrawTreesPrims();	
}
void TreesMeshCreator::DrawTreesShadows(){
	IShadowMgr->AddProcCaster(&DrawTreesShadowMesh);
}
void TreesMeshCreator::ClearPrevHash(){	
	PrevSummHash=0xBAADBEEF;
}
void TreesMeshCreator::CreateMeshes(){
	DWORD PH=PrevSummHash;
	DWORD H=SummHash;
    PrevSummHash=SummHash;
	SummHash=0;	
	if(PH==H){
		TaskToDraw.FastClear();
		return;
	}
	Clear();
    int nvc=0;
	for(int q=0;q<num_common_tex;q++)CurMesh[q]=0;
	//sorting
    if(cmTexID[0]<=0){
        cmTexID[0]=IRS->GetTextureID("common_tex.dds");
        for(int i=1;i<num_common_tex;i++){
            char c[32];
            sprintf(c,"common_tex%d.dds",i);
            cmTexID[i]=IRS->GetTextureID(c);
        }
    }
	qsort(TaskToDraw.GetValues(),TaskToDraw.GetAmount(),4,compare_TH);
	static int idShPersp = IRS->GetShaderID("multiple_trees");
	static int idShOrtho = IRS->GetShaderID("multiple_trees_ortho");
	for(int i=0;i<TaskToDraw.GetAmount();i++){
		TreeHashElem* he=TaskToDraw[i];        
		BaseMesh* bm=he->BasicGeometry;
        int*pCurMesh;
		if(bm){
			int nv=bm->getNVert();
			int ni=bm->getNInd();
            DynArray<StaticMesh*>* pMeshes=NULL;
            int T=he->TexID;
            for(int q=0;q<num_common_tex;q++){
                if(T==cmTexID[q]){
                    pMeshes=&Meshes[q];					
                    pCurMesh=&CurMesh[q];					
                    break;
                }
            }
            if(pMeshes){
			    StaticMesh* curM=NULL;
			    if(pMeshes->GetAmount()==0 || (*pMeshes)[*pCurMesh]->getNVert()+nv>8*6*3*64 || (*pMeshes)[*pCurMesh]->getNInd()+ni>8*5*4*3*64){			
				    if(*pCurMesh<pMeshes->GetAmount()-1){\
						StaticMesh* bm=(*pMeshes)[*pCurMesh];
                        bm->unlock();
						bm->setShader(OrthoCameraMode::IsEnabled() ? idShOrtho : idShPersp);
					    (*pCurMesh)++;
					    curM=(*pMeshes)[*pCurMesh];				
				    }else{
                        if(pMeshes->GetAmount()>0){
                            StaticMesh* bm=(*pMeshes)[pMeshes->GetAmount()-1];
                            bm->unlock();
                        }
					    StaticMesh* bm=new StaticMesh;
					    bm->create(max(nv,8*6*3*64),max(ni,8*5*4*3*64),vfVertex2t);
//					    static int sh=IRS->GetShaderID("multiple_trees");
					    static int tex=IRS->GetTextureID("common_tex.dds");
					    bm->setTexture(tex);
						bm->setShader(OrthoCameraMode::IsEnabled() ? idShOrtho : idShPersp);//sh);				
					    curM=bm;
					    *pCurMesh=pMeshes->GetAmount();
					    pMeshes->Add(bm);
				    }
			    }else curM=(*pMeshes)[*pCurMesh];			
			    int nv0=curM->getNVert();			
			    Vertex2t* verts=(Vertex2t*)curM->getVertexData();			
				if(verts){
					verts+=nv0;
					memcpy(verts,he->CalculatedVerts,nv*sizeof Vertex2t);
					WORD* idxs=curM->getIndices();
					idxs+=curM->getNInd();
					WORD* s_idxs=bm->getIndices();
					for(int i=0;i<ni;i++)idxs[i]=s_idxs[i]+nv0;
					curM->setNVert(curM->getNVert()+nv);
					curM->setNInd(curM->getNInd()+ni);
					curM->setNPri(curM->getNInd()/3);		
				}
            }
		}
	}
    for(int q=0;q<num_common_tex;q++){
        DynArray<StaticMesh*>* pMeshes=&Meshes[q];    
        for(int i=0;i<pMeshes->GetAmount();i++)if((*pMeshes)[i]->getNVert()){
            StaticMesh& bm=*(Meshes[q][i]);
            bm.unlock();
			bm.setShader(OrthoCameraMode::IsEnabled() ? idShOrtho : idShPersp);
        }
    }
	TaskToDraw.FastClear();
}
void TreesMeshCreator::DrawTreesMeshes(){
    IRS->ResetWorldTM();    
	static int idShPersp = IRS->GetShaderID("multiple_trees");
	static int idShOrtho = IRS->GetShaderID("multiple_trees_ortho");
    IRS->SetShader(OrthoCameraMode::IsEnabled() ? idShOrtho : idShPersp);
//	static int sh=IRS->GetShaderID("multiple_trees");
    for(int q=0;q<num_common_tex;q++){
        DynArray<StaticMesh*>* pMeshes=&Meshes[q];    
	    for(int i=0;i<pMeshes->GetAmount();i++)if((*pMeshes)[i]->getNVert()){
		    StaticMesh& bm=*(Meshes[q][i]);
			//IRS->SetShader(OrthoCameraMode::IsEnabled() ? idShOrtho : idShPersp);// sh );
		    //IRS->SetShaderAutoVars();
		    //IRS->SetTexture( cmTexID[q], 0, false );
            //static int T1=IRS->GetTextureID("noise.dds");
		    //IRS->SetTexture( T1/*bm.getTexture( 1 )*/, 1, false );
		    //DrawPrimBM( bm );
            bm.setTexture(cmTexID[q]);
            bm.render();
	    }
    }
    IRS->Flush();
}
void TreesMeshCreator::DrawTreesPrims(){    
    IRS->ResetWorldTM();
	static int s_ShadowShader = IRS->GetShaderID( "projected_shadow_caster" );
	IRS->SetShader(s_ShadowShader);
    for(int q=0;q<num_common_tex;q++){
        DynArray<StaticMesh*>* pMeshes=&Meshes[q];  
	    for(int i=0;i<pMeshes->GetAmount();i++)if((*pMeshes)[i]->getNVert()){
		    StaticMesh& bm=*((*pMeshes)[i]);
		    //IRS->SetTexture( cmTexID[q], 0, false );		
		    //DrawPrimBM( bm );
            bm.render();
	    }
    }
    IRS->Flush();
}
int  TreesMeshCreator::GetMeshes(BaseMesh** OutBuf,int MaxSize){
	//for(int i=0;i<MaxSize && i<Meshes.GetAmount();i++)OutBuf[i]=Meshes[i];
	return 0;//Meshes.GetAmount();
}
void TreesMeshCreator::Clear(){	
    for(int q=0;q<num_common_tex;q++){
        DynArray<StaticMesh*>* pMeshes=&Meshes[q];  
	    for(int i=0;i<pMeshes->GetAmount();i++){
		    (*pMeshes)[i]->setNVert(0);
		    (*pMeshes)[i]->setNInd(0);
		    (*pMeshes)[i]->setNPri(0);
	    }
        CurMesh[q]=0;
    }
}
void TreesMeshCreator::Free(){
	for(int i=0;i<TreesHash.numElem();i++){
		TreeHashElem& th=TreesHash.elem(i);
		if(th.CalculatedVerts)free(th.CalculatedVerts);
		th.CalculatedVerts=NULL;		
	}
	TreesHash.reset();
    PrevSummHash=0xFF003456;
    SummHash=0xFF003455;
    TaskToDraw.Clear();
    /*
    for(int q=0;q<num_common_tex;q++){
        DynArray<StaticMesh*>* pMeshes=&Meshes[q];  
	    for(int i=0;i<pMeshes->GetAmount();i++)delete((*pMeshes)[i]);
        Meshes[q].Clear();
    } 
    */
}
void TreeHashElem::Create(DWORD ID,DWORD State,Vector3D pos,int ModelID,DWORD Color,Matrix4D& transform){
    UniqID=ID;
	StateHashValue=State;
	BaseMesh* bm=BasicGeometry;
    if(!bm){
        SNode* pG = IMM->GetModelRoot( ModelID );
        if (!pG) return;
        if (pG->IsA<Model>()) ((Model*)pG)->Load();
        int cGeom = 0;
        TexID=-1;
        SNode::Iterator it( pG );
        while (it)
        {
            Geometry* pGeom = (Geometry*)(*it);
            if(pGeom->IsA<Geometry>()){
                bm=&(pGeom->GetMesh());            
            }        
            if(pGeom->IsA<Texture>()){
                TexID=((Texture*)pGeom)->GetTexID();
            }
            ++it;
        }
        BasicGeometry=bm;
    }    
	if(bm){
		if(!CalculatedVerts)CalculatedVerts=(Vertex2t*)malloc(BasicGeometry->getNVert()*sizeof Vertex2t);
		Vertex2t* verts=CalculatedVerts;
		VertexN* s_verts=(VertexN*)bm->getVertexData();
		int nv=bm->getNVert();
		for(int i=0;i<nv;i++){
			Vector3D vPos(s_verts->x,s_verts->y,s_verts->z);
			transform.transformVec(vPos);
			float nx0=s_verts->nx;
			float ny0=s_verts->ny;
			float nx=nx0*transform.e00+ny0*transform.e10;
			float ny=nx0*transform.e01+ny0*transform.e11;
			verts->u2=pos.y;
			verts->v2=(512+pos.z)+floorf(pos.x+256)*2048.0f;
			verts->x=pos.x+vPos.x;
			verts->y=pos.y+vPos.y;
			verts->z=pos.z+vPos.z;			
			verts->u=s_verts->u;
			verts->v=s_verts->v;
			verts->diffuse=(Color & 0xFF0000FF)+(DWORD(128+nx*126)<<16)+(DWORD(128+ny*126)<<8);
			verts++;
			s_verts++;
		}
	}
}
TreesMeshCreator TreesMesh;