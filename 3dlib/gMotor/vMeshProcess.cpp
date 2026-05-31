/*****************************************************************************/
/*	File:	vMeshProcess.cpp
/*	Desc:	Different mesh processing functions
/*	Author:	Ruslan Shestopalyuk
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "mGeom3D.h"
#include "IMesh.h"
#include "vMesh.h"

using namespace sg;
int GetNodeLevel( Node* pNode )
{
	int level = -1;
	while (pNode) 
	{
		pNode = pNode->GetParent();
		level++;
	}
	return level;
} // GetNodeLevel

int CmpBones( const void* pTM1, const void* pTM2 )
{
	TransformNode* node1 = *((TransformNode**)pTM1);
	TransformNode* node2 = *((TransformNode**)pTM2);
	int lev1 = GetNodeLevel( node1 );
	int lev2 = GetNodeLevel( node2 );
	if (lev1 != lev2) return lev1 - lev2;
	return strcmp( node1->GetParent()->GetName(), node2->GetParent()->GetName() );
} // CmpBones

template <class TVert>
void ExtractSkinningInfo( MeshFactory& mf, void* pVert )
{
    TVert* v = (TVert*)pVert;
    for (int i = 0; i < v->GetNBlendI(); i++) mf.AddBlendI( v->GetBlendI( i ), i );
    for (int i = 0; i < v->GetNBlendW(); i++) mf.AddBlendW( v->GetBlendW( i ), i );
} // ExtractSkinningInfo

IMesh*  ConvertModel( DWORD mdlID )
{
	Node* pMdl = NodePool::GetNode( mdlID );
    if (!pMdl) return NULL;
	
	//  create mesh factory
	MeshFactory mf;

	//  extract bone list
	std::vector<TransformNode*>	bones;
	Node::Iterator bit( pMdl );
	while (bit)
	{
		Node* pNode = (Node*)bit;
		++bit;
		if (!pNode->IsA<TransformNode>()) continue;
		TransformNode* pTM = (TransformNode*)pNode;
		bool bNoBone = true;
		for (int i = 0; i < bones.size(); i++) 
		{ 
			if (bones[i] == pTM) 
			{
				bNoBone = false; 
				break; 
			} 
		}
		if (bNoBone) bones.push_back( pTM );
	}
	
	int nBones = bones.size();
	///if (nBones > 0) qsort( &bones[0], bones.size(), sizeof(TransformNode*), CmpBones );
	for (int i = 0; i < nBones; i++)
	{
		mf.AddBone( bones[i]->GetName(), bones[i]->GetTransform() );	
	}

    TransformNode* pCurTM = NULL;
    Node::Iterator it( pMdl );
    while (it)
    {
        Node* pNode = (Node*)it;
        if (pNode->IsA<Geometry>())
        {
			int cV = mf.GetNVerts();
            Geometry* pGeom = (Geometry*)pNode;
            BaseMesh& bm = pGeom->GetPrimitive();
            VertexFormat vf = bm.getVertexFormat();
            int nV = bm.getNVert();
            VertexIterator vit;
            vit << bm;
            for (int i = 0; i < nV; i++) 
            {
                Vector3D pos = vit.pos( i ); 
                mf.AddPos( pos );
            }
            if (vit.HasNormal())
            {
                for (int i = 0; i < nV; i++) 
                {
                    Vector3D normal = vit.n( i );
                    mf.AddNormal( normal );
                }
            }

            if (vit.HasUV())
            {
                for (int i = 0; i < nV; i++) mf.AddUV( vit.u( i ), vit.v( i ), 0 );
            }
            if (vit.HasUV2())
            {
                for (int i = 0; i < nV; i++) mf.AddUV( vit.u( i ), vit.v( i ), 1 );
            }
            if (vit.HasDiffuse())
            {
                for (int i = 0; i < nV; i++) mf.AddDiffuse( vit.diffuse( i ) );
            }

            if (vit.HasSpecular())
            {
                for (int i = 0; i < nV; i++) mf.AddSpecular( vit.specular( i ) );
            }

            if (vit.HasBlendI())
            {
                for (int i = 0; i < nV; i++)
                {
                    if (vf == vfNMP1)       ExtractSkinningInfo<VertexNMP1>( mf, &vit.pos( i ) );
                    else if (vf == vfNMP2)  ExtractSkinningInfo<VertexNMP2>( mf, &vit.pos( i ) );
                    else if (vf == vfNMP3)  ExtractSkinningInfo<VertexNMP3>( mf, &vit.pos( i ) );
                    else if (vf == vfNMP4)  ExtractSkinningInfo<VertexNMP4>( mf, &vit.pos( i ) );
                }
            }

            int nT = bm.getNPri();
            WORD* idx = bm.getIndices();
			for (int i = 0; i < nT; i++) { mf.AddTriangle( cV + idx[i*3 + 0], cV + idx[i*3 + 1], cV + idx[i*3 + 2] ); }

            if (pNode->IsA<SkinnedGeometry>())
            //  extract bone references
            {
                
            }
        }
        ++it;
    }

    IMesh* mesh = mf.CreateMesh();
    return mesh;
} // ConvertModel
