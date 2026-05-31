/*****************************************************************************/
/*    File:    vModelInstance.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11-06-2005
/*****************************************************************************/
#include "stdafx.h"
#include "sgTransformNode.h"
#include "sgGeometry.h"
#include "vSkin.h"
#include "sgModel.h"
#include "vModelInstance.h"

#include "uiWidgetEditor.h"
#include "uiFrameWindow.h"
#include "uiKangaroo.h"

/*****************************************************************************/
/*  ModelInstance implementation
/*****************************************************************************/
ModelInstance::ModelInstance() 
               :    m_pModel            ( NULL  ), 
                    m_LastFrame         ( 0     ), 
                    m_bBlending         ( false ), 
                    m_pPrevController   ( NULL  ),
                    m_bPreventBlending  ( true  )
{
    m_RootTM.setIdentity();
}

void ModelInstance::Create( SNode* pModel, const Matrix4D& rootTM )
{
    m_Skin.clear();
    m_Bones.clear();
    if (pModel->IsA<Model>()) ((Model*)pModel)->Load();
    SNode::Iterator it( pModel );
	int nc=0;
    while (it)
    {
        SNode* pNode = (SNode*)*it;
        if (pNode->IsA<Skin>())          m_Skin.push_back( SkinInstance( (Skin*)pNode ) );
        if (pNode->IsA<TransformNode>()) 
		{
			BoneInstance bone( (TransformNode*)pNode );
			Matrix4D mb = bone.m_pBone->GetTransform();
			mb *= GetWorldTM( bone.m_pBone->GetParent() );
			mb *= rootTM;
			bone.m_TM = mb;
			m_Bones.push_back( bone );
		}
        ++it;
		nc++;
    }
    m_pModel = pModel;

    //FOutStream fs("Models\\testdl.c2m");
    //pModel->SerializeSubtree(fs);

    m_bPreventBlending = true;
    m_RootTM = rootTM;
} // ModelInstance::Create  
//#define DRAW_BONES
bool _dbgDrawBonesMode = false;

void ModelInstance::Render()
{
    if (!m_pModel) return;
    int nB = m_Bones.size();
	extern bool GecOptimCollision;//GEC - GECOPTIM cheats , mogno udalit'
	if(GecOptimCollision){
		rsEnableZ(true);
	}
    if(_dbgDrawBonesMode){
        rsEnableZ(true);
    }
    IReflected* selNode=NULL;
    float mindist=0;
    for (int i = 0; i < nB; i++)
    {
        BoneInstance& bone = m_Bones[i];
        bone.m_pBone->SetTopTM( bone.m_TM );
        if(_dbgDrawBonesMode){
            const Matrix4D& m=bone.m_pBone->GetTransform();
            Matrix4D mW=bone.m_TM;
            Matrix4D mW0=mW;
            Matrix4D mL=m;
            mL.inverse();
            mW0.mulLeft(mL);
            Vector3D V0(mW0.e30,mW0.e31,mW0.e32);
            Vector3D VF(mW.e30,mW.e31,mW.e32);            
            
            rsLine(V0,VF,0x40FFFFFF,0xFFFF0000);
            float Mult=1.0f;
            NodeTree& nt=Kangaroo::instance().m_ModelEditor.m_TreeL;

            //ICamera* pCam = GetCamera();
            //if(pCam && GetKeyState(VK_SHIFT) < 0 && GetKeyState(VK_CONTROL) < 0){
            //    Ray3D ray = pCam->GetPickRay( mX, mY );
            //    floa
            //}            

            IReflected* IR=nt.GetSelectedNode();
            DWORD CF=0xFFFF0000;
            if(bone.m_pBone==IR){
                Mult=1.5+cos(float(GetTickCount()%200000)/200.0f)/2;
                CF=0xFFFFFF00;
            }
            for(int dx=-3;dx<=3;dx++){
                for(int dy=-3;dy<=3;dy++){
                    for(int dz=-3;dz<=3;dz++){
                        Vector3D V(dx,dy,dz);
                        if(V.norm()<3.5f){
                            V*=Mult;
                            mW.transformPt(V);
                            rsLine(V0,V,0x40FFFFFF,CF);
                            rsLine(V,VF,CF,CF);
                        }
                    }
                }
            }
			//DrawSphere(Sphere(Vector3D(bone.m_TM.e30,bone.m_TM.e31,bone.m_TM.e32),3),0x90FF0000,0x90FF0000,8);
        }
    }
    //if(_dbgDrawBonesMode){
		IRS->Flush();
        rsFlush();
        rsEnableZ(true);        
    //}
    
    if (nB == 0)
    {
        TransformNode::ResetTMStack( &m_RootTM );
        m_pModel->Render();
    }
    else
    {
		if(_dbgDrawBonesMode && GetKeyState(VK_NUMLOCK))return;
        TransformNode::SetCalcWorldTM( false );
        m_pModel->Render();
        TransformNode::SetCalcWorldTM( true );
    }
	IRS->Flush();
} // ModelInstance::Render

Matrix4D ModelInstance::GetNodeTransform( DWORD nodeID ){
    int nB = m_Bones.size();
    TransformNode* pNode = NodePool::GetNode<TransformNode>( nodeID );
    if(pNode){
        const char* name=pNode->GetName();
        if(name){
            for (int i = 0; i < nB; i++)
            {
                BoneInstance& bone = m_Bones[i];
                if( !strcmp(bone.m_pBone->GetName(),name) )
                {
                    Matrix4D mb = bone.m_pBone->GetTransform();
                    mb *= GetWorldTM( bone.m_pBone->GetParent() );
                    //mb *= rootTM;
                    return mb;
                }
            }    
        }
    }
    return Matrix4D::identity;
}
void ModelInstance::Animate( SNode* pController, const Matrix4D& rootTM, float blendInTime )
{
	if (!pController){
		int nB = m_Bones.size();
		for (int i = 0; i < nB; i++)
		{
			BoneInstance& bone = m_Bones[i];
			Matrix4D mb = bone.m_pBone->GetTransform();
			if(i>0){
				SNode* pNode = bone.m_pBone->GetParent();
				while (pNode && !pNode->m_bTransform){
					pNode = pNode->GetParent();
				}
				if(pNode)mb *= ((TransformNode*)pNode)->GetTopTM();//GetWorldTM( bone.m_pBone->GetParent() );
			}
			bone.m_pBone->SetTopTM( mb );
			mb *= rootTM;
			bone.m_TM = mb;            
		} 
		return;
	}

    int nB = m_Bones.size();
    if (pController != m_pPrevController && blendInTime > 0.0f)
    {
        if (m_bPreventBlending)
        {
            m_bPreventBlending = false;
            m_bBlending = false;
        }
        else
        {
            m_bBlending = true;
            m_BlendTimer.start( blendInTime );

            //  reset all bones to their rest positions
            for (int i = 0; i < nB; i++)
            {
                BoneInstance& bone = m_Bones[i];
                bone.m_pBone->SetToInitial();
            }
        }
        m_pPrevController = pController;
    }

    TransformNode::ResetTMStack();
    pController->Render();

    if (nB == 0) return;

    if (m_bBlending && m_BlendTimer.iteration() > 0) 
    {
        m_bBlending = false;
    }

    if (m_bBlending)
    {
        float blend = 1.0f - m_BlendTimer.seconds()/m_BlendTimer.period();
        for (int i = 0; i < nB; i++)
        {
            BoneInstance& bone = m_Bones[i];
            const Matrix4D& ma = bone.m_TM;
            Matrix4D mb = bone.m_pBone->GetTransform();
            if(i>0){
                SNode* pNode = bone.m_pBone->GetParent();
                while (pNode && !pNode->m_bTransform){
                    pNode = pNode->GetParent();
                }
                if(pNode)mb *= ((TransformNode*)pNode)->GetTopTM();//GetWorldTM( bone.m_pBone->GetParent() );
            }
            bone.m_pBone->SetTopTM( mb );
            //mb *= GetWorldTM( bone.m_pBone->GetParent() );
            mb *= rootTM;

            Matrix4D dm( mb );
            dm -= ma;
            
            Vector3D scA, scB, sc;
            Vector3D trA, trB, tr;
            Quaternion rotA, rotB, rot;

            ma.Factorize( scA, rotA, trA );
            mb.Factorize( scB, rotB, trB );

            float d=trA.distance2(trB);
            if(d>200.0f*200.0f)//blending threshold
            {
                trA = trB;
                scA = scB;
            }

            sc.addWeighted( scA, scB, blend, 1.0f - blend );
            tr.addWeighted( trA, trB, blend, 1.0f - blend );
            
            rotA.normalize();
            rotB.normalize();

            rot.Slerp( rotA, rotB, 1.0f - blend, true );
            Matrix4D m( sc, rot, tr );
            bone.m_TM = m;
        }
    }
    else
    {
        for (int i = 0; i < nB; i++)
        {
            BoneInstance& bone = m_Bones[i];
            Matrix4D mb = bone.m_pBone->GetTransform();
            if(i>0){
                SNode* pNode = bone.m_pBone->GetParent();
                while (pNode && !pNode->m_bTransform){
                    pNode = pNode->GetParent();
                }
                if(pNode)mb *= ((TransformNode*)pNode)->GetTopTM();//GetWorldTM( bone.m_pBone->GetParent() );
            }
            bone.m_pBone->SetTopTM( mb );
            mb *= rootTM;
            bone.m_TM = mb;            
        } 
    }
} // ModelInstance::Animate

void ModelInstance::OnUpdateNode( SNode* pNode )
{

}
