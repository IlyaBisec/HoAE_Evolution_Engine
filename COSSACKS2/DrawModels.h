#include "IMediaManager.h"
#include "IShadowManager.h"
#include "kContext.h"

class OneModelToDraw{
public:
	int sortY;
	Matrix4D M4;
	int ModelID;
	int ContextID;
	int ShaderID;
	int ShaderVar1;
	float Var1_value;
	int ShaderVar2;
	float Var2_value;
	DWORD CFactor;
};
struct SortElm{
public:
	int Y;
	int pos;
};
class ModelsScope{
	DynArray<OneModelToDraw> MDraw;
	DynArray<SortElm> Sort;
public:
	~ModelsScope(){
		MDraw.Clear();
		Sort.GetAmount();
	}
	void Add(int ContextID,int ModelID,Matrix4D* M4,int sortY,DWORD CF
				,int ShaderID=-1,int ShaderVar1=-1,float Var1_value=0
				,int ShaderVar2=-1,float Var2_value=0){
		SortElm SE;
		SE.pos=MDraw.GetAmount();
		SE.Y=sortY;
		Sort.Add(SE);
        OneModelToDraw MD;
		MD.ContextID=ContextID;
		MD.sortY=sortY;
		MD.M4=*M4;
		MD.ModelID=ModelID;
		MD.CFactor=CF;
		MD.ShaderID=ShaderID;
		MD.ShaderVar1=ShaderVar1;
		MD.Var1_value=Var1_value;
		MD.ShaderVar2=ShaderVar2;
		MD.Var2_value=Var2_value;
		MDraw.Add(MD);	
	}
	void Clear(){
		MDraw.Clear();
		Sort.Clear();
	}
	static int __cdecl compare(const void* el1,const void* el2){
		return ((SortElm*)el1)->Y>((SortElm*)el2)->Y?1:-1;
	}
	void Draw(){
        extern bool g_bRenderShadowsPass;
        qsort(Sort.GetValues(),Sort.GetAmount(),sizeof SortElm,&compare);
		//for rocks
		Matrix4D tm;
		ICamera* iCam = GetCamera();
		tm = iCam ? iCam->GetWorldTM() : Matrix4D::identity;

		Matrix4D fm=tm;
		Matrix4D m2;
		m2.rotation(Vector3D::oX,0.8f);//float(GetTickCount())/2000.0f);
		tm*=m2;
		m2.scaling(1.0f/256.0f,0.9f/256.0f,1.0f);
		tm*=m2;	
		//for fog
		m2.rotation(Vector3D::oX,0.6);//float(GetTickCount())/2000.0f);
		fm*=m2;
		m2.scaling(1.0f/456.0f,0.9f/456.0f,1.0f);
		fm*=m2;	
		float t=GetTickCount()/24000.0;
		GPS.SetCurrentDiffuse(0xFF808080);
		static int hud2=IRS->GetShaderID("hud2");
		static int hud=IRS->GetShaderID("hud");
		GPS.SetShader(hud2);
        IRS->SetShader( hud2 );
		Vector3D vd=ICam->GetDirection();		
		for(int i=0;i<Sort.GetAmount();i++){			
            OneModelToDraw* OM=&MDraw[Sort[i].pos];
            int x=int(OM->M4.e30);
			int y=int(OM->M4.e31);
			PushEntityContext(OM->ContextID);
			IRS->SetTextureFactor(OM->CFactor);			
			//if(OM->ShaderID!=-1){
			//	if(OM->ShaderVar1!=-1){
			//		Vector4D V(OM->Var1_value,OM->Var2_value,0,0);
			//		IRS->SetShaderVar(OM->ShaderID,OM->ShaderVar1,V);
			//	}				
			//}
			IMM->StartModel(OM->ModelID,OM->M4,int(OM));
            IMM->DrawModel();            
            //if (g_bRenderShadowsPass)
            //{
                IShadowMgr->AddCaster( OM->ModelID, OM->M4 );
                IMM->RenderShadow( OM->ModelID, &OM->M4 );
            //}
			if(GSets.SVOpt.DrawReflectedModels){
				Vector3D vd=ICam->GetDirection();
				if(GetHeight(OM->M4.e30-vd.x*128,OM->M4.e31-vd.y*128)<0)
					IRMap->AddObject( OM->ModelID, &OM->M4 );
			}

			PopEntityContext();			
		}
		GPS.FlushBatches();
		GPS.SetShader(hud);
		if(!g_bRenderShadowsPass)Clear();
	}
};
extern ModelsScope RenderModels;