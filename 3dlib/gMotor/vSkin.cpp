/*****************************************************************************/
/*    File:    vSkin.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgNode.h"
#include "sgTransformNode.h"
#include "sgGeometry.h"
#include "sgModel.h"
#include "mSkin.h"
#include "vSkin.h"
#include "sgShader.h"

IMPLEMENT_CLASS( Skin );
IMPLEMENT_CLASS( SkinGPU );
/*****************************************************************************/
/*    Skin    implementation
/*****************************************************************************/
Skin::Skin() : m_NWeights(0), m_BoneTM(NULL),  m_NBones(0), m_bBonesBound(false) 
{
    m_BoneTM = aligned_new<Matrix4D>( m_NBones, 1 );
}

Skin::~Skin()
{
    aligned_delete_nodestruct( m_BoneTM );
}

void Skin::AddInputBone( const char* bName )
{ 
    m_InputBoneNames.push_back( bName ); 
} // Skin::AddInputBone

const Matrix4D& Skin::GetMatrix( int idx )              
{ 
    if (idx < 0 || idx >= m_InputBones.size() || m_InputBones[idx] == NULL) 
    {
        return TransformNode::TMStackTop();
    }
    return m_InputBones[idx]->GetTopTM(); 
} // Skin::GetMatrix

void Skin::FlipNormals()
{
    VertexIterator vit;
    vit << m_BaseMesh;
    while (vit)
    {
        vit.normal().reverse();
        ++vit;
    }
} // Geometry::FlipNormals

void Skin::CalculateNormals()
{
    m_BaseMesh.calcNormals();
}

void Skin::Serialize( OutStream& os ) const
{
    SNode::Serialize( os );
    os << m_BaseMesh;
    os << m_BoneOffsets;
    os << m_InputBoneNames;
} // Skin::Serialize

void Skin::Unserialize( InStream& is )
{
    SNode::Unserialize( is );
    is >> m_BaseMesh;
    is >> m_BoneOffsets;
    is >> m_InputBoneNames;

    m_AABB = m_BaseMesh.GetAABB();
    m_Mesh.create       ( m_BaseMesh.getNVert() + 2, m_BaseMesh.getNInd(), vfVertexN );
    m_Mesh.setNVert     ( m_BaseMesh.getNVert() );
    m_Mesh.setNPri      ( m_BaseMesh.getNPri() );
    m_Mesh.setNInd      ( m_BaseMesh.getNInd() );
    m_Mesh.setIndices   ( m_BaseMesh.getIndices(), m_BaseMesh.getNInd() );

    VertexFormat vf = m_BaseMesh.getVertexFormat();
    switch (vf)
    {
    case vfVertexW1:    m_NWeights = 1; break;
    case vfVertexW2:    m_NWeights = 2; break;
    case vfVertexW3:    m_NWeights = 3; break;
    case vfVertexW4:    m_NWeights = 4; break;
    case vfVertex1W:    m_NWeights = 1; break;
    case vfVertex2W:    m_NWeights = 2; break;
    case vfVertex3W:    m_NWeights = 3; break;
    case vfVertex4W:    m_NWeights = 4; break;
    default:            m_NWeights = 0;
    }

    m_NBones = m_BoneOffsets.size();
    aligned_delete_nodestruct( m_BoneTM );
    m_BoneTM = aligned_new<Matrix4D>( m_NBones, 32 );

    m_bBonesBound = false;

    m_AABB = m_Mesh.GetAABB();
} // Skin::Unserialize
bool renderG=false;
void RenderVertexN_mesh_like_VertexBump(const BaseMesh& bm,DWORD& m_ICacheStamp,int& m_IBufferPos,DWORD& m_VCacheStamp,int& m_VBufferPos){
    static int s_SkinVBuffer = -1;
    if( s_SkinVBuffer==-1 ){
        s_SkinVBuffer = IRS->CreateVB("BumpVertexBuffer",sizeof(VertexBump)*65536,int(vfVertexBump));
    }
    int  nV = bm.getNVert();
    int  nI = bm.getNInd();
    WORD* idxs = bm.getIndices();
    if (!IRS->IsIBStampValid( Skin::s_SkinIBuffer, m_ICacheStamp )){
        BYTE* pIdx  = IRS->LockAppendIB( Skin::s_SkinIBuffer, nI, m_IBufferPos, m_ICacheStamp );
        if (!pIdx) return;
        memcpy( pIdx, idxs, nI*sizeof(WORD) );
        IRS->UnlockIB( Skin::s_SkinIBuffer );
    }
    IRS->SetVB( s_SkinVBuffer, (int)vfVertexBump );
    BYTE* pOut  = IRS->LockAppendVB( s_SkinVBuffer, nV, m_VBufferPos, m_VCacheStamp );
    if (!pOut) return;
    VertexBump* vb=(VertexBump*)pOut;
    VertexBump* vbc=vb;
    VertexN* vn=(VertexN*)bm.getVertexData();
    for(int i=0;i<nV;i++){
        vbc->x=vn->x;
        vbc->y=vn->y;
        vbc->z=vn->z;
        vbc->nx=vn->nx;
        vbc->ny=vn->ny;
        vbc->nz=vn->nz;
        vbc->u=vn->u;
        vbc->v=vn->v;
        vbc->tx=vbc->ty=vbc->tz=vbc->bx=vbc->by=vbc->bz=0;
        vbc++;
        vn++;
    }
    //calculating txyz,bxyz
    for(int i=0;i<nI;i+=3){
        VertexBump* v1=vb+idxs[i  ];
        VertexBump* v2=vb+idxs[i+1];
        VertexBump* v3=vb+idxs[i+2];
        float du = v2->u-v1->u;
        float dv = v2->v-v1->v;

        v1->tx+=(v2->x-v1->x)*du;
        v1->ty+=(v2->y-v1->y)*du;
        v1->tz+=(v2->z-v1->z)*du;

        v1->bx+=(v2->x-v1->x)*dv;
        v1->by+=(v2->y-v1->y)*dv;
        v1->bz+=(v2->z-v1->z)*dv;

        du = v3->u-v2->u;
        dv = v3->v-v2->v;

        v2->tx+=(v3->x-v2->x)*du;
        v2->ty+=(v3->y-v2->y)*du;
        v2->tz+=(v3->z-v2->z)*du;

        v2->bx+=(v3->x-v2->x)*dv;
        v2->by+=(v3->y-v2->y)*dv;
        v2->bz+=(v3->z-v2->z)*dv;

        du = v1->u-v3->u;
        dv = v1->v-v3->v;

        v3->tx+=(v1->x-v3->x)*du;
        v3->ty+=(v1->y-v3->y)*du;
        v3->tz+=(v1->z-v3->z)*du;

        v3->bx+=(v1->x-v3->x)*dv;
        v3->by+=(v1->y-v3->y)*dv;
        v3->bz+=(v1->z-v3->z)*dv;
    }
    IRS->UnlockVB( s_SkinVBuffer );
    RenderTask& rt = IRS->AddTask();
    rt.m_ShaderID       = IRS->GetShader();
    rt.m_TexID[0]       = IRS->GetTexture( 0 );
    rt.m_TexID[1]       = IRS->GetTexture( 1 );
    rt.m_TexID[2]       = -1;
    rt.m_bTransparent   = true;
    rt.m_VType          = int(vfVertexBump);
    rt.m_NVert          = nV;
    rt.m_NIdx           = nI;

    rt.m_VBufID         = s_SkinVBuffer;
    rt.m_FirstVert      = m_VBufferPos;
    rt.m_IBufID         = Skin::s_SkinIBuffer;
    rt.m_FirstIdx       = m_IBufferPos;
    rt.m_Source         = "";
    rt.m_bHasTM         = true; 
    rt.m_TM             = Matrix4D::identity;
    IRS->Flush();
}
void Skin::Render()
{    
    if(renderG){
        Geometry::Render();
        return;
    }

    ProcessGeometry();    

    const BaseMesh& skin    = GetMesh();
    const BaseMesh& bm      = GetBaseMesh();
    int             vType   = (int)vfVertexN;
    int             nV      = skin.getNVert();
    int             nI      = skin.getNInd();
    int             nBytes  = nV*sizeof(VertexOut);
    int             sh      = bm.getShader();
	if(IRS->GetBumpEnable() && IRS->GetShadersQuality()<=0 && strstr(IRS->GetShaderName(IRS->GetShader()),"bump")){
        RenderVertexN_mesh_like_VertexBump( skin, m_ICacheStamp, m_IBufferPos, m_VCacheStamp, m_VBufferPos );
        return;
    }
    if (nI == 0 || nV == 0) return;
    VertexOut*    dBuf = (VertexOut*)skin.getVertexData();
    if (!IRS->IsIBStampValid( s_SkinIBuffer, m_ICacheStamp ))
    {
        BYTE* pIdx  = IRS->LockAppendIB( s_SkinIBuffer, nI, m_IBufferPos, m_ICacheStamp );
        if (!pIdx) return;
        memcpy( pIdx, bm.getIndices(), nI*sizeof(WORD) );
        IRS->UnlockIB( s_SkinIBuffer );
    }

    IRS->SetVB( s_SkinVBuffer, (int)vfVertexN );
    BYTE* pOut  = IRS->LockAppendVB( s_SkinVBuffer, nV, m_VBufferPos, m_VCacheStamp );
    if (!pOut)
    {
        Log.Error( "Could not lock skinning vertex buffer." );
        return;
    }
    memcpy( pOut, dBuf, nV*sizeof(VertexOut) );
    IRS->UnlockVB( s_SkinVBuffer );

    RenderTask& rt = IRS->AddTask();
    rt.m_ShaderID       = IRS->GetShader();
    rt.m_TexID[0]       = IRS->GetTexture( 0 );
    rt.m_TexID[1]       = IRS->GetTexture( 1 );
    rt.m_TexID[2]       = -1;
    rt.m_bTransparent   = true;
    rt.m_VType          = vType;
    rt.m_NVert          = nV;
    rt.m_NIdx           = nI;

    rt.m_VBufID         = s_SkinVBuffer;
    rt.m_FirstVert      = m_VBufferPos;
    rt.m_IBufID         = s_SkinIBuffer;
    rt.m_FirstIdx       = m_IBufferPos;
    rt.m_Source         = GetName();
    rt.m_bHasTM         = true; 
    rt.m_TM             = Matrix4D::identity;

    //IRS->Flush();
    PostRender();
} // Skin::Render

bool Skin::BindBones()
{
    //  find host model 
    SNode* pParent = GetParent();
    while (pParent && !pParent->IsA<Model>()) pParent = pParent->GetParent();
    if (!pParent)
    {
        Log.Error( "Found unhosted skin." );
        return false;
    }
    bool bAllBound = true;
    const char* mName = pParent->GetName();
    
    int nBound = 0;

    for (int i = 0; i < m_InputBoneNames.size(); i++)
    {
        const char* bName = m_InputBoneNames[i].c_str();
        SNode::Iterator it( pParent );
        bool bBound = false;

        //OutputDebugString( "seeking: " );
        //OutputDebugString( bName );
        //OutputDebugString( "\n" );

        while (it)
        {
            TransformNode* pNode = dynamic_cast<TransformNode*>( (SNode*)*it );
            ++it;
            if (pNode)
            {
                //OutputDebugString( "    through: " );
                //OutputDebugString( pNode->GetName() );
                //OutputDebugString( "\n" );
            }
            if (pNode && !stricmp( bName, pNode->GetName() )) 
            {
                m_InputBones.push_back( pNode );
                bBound = true;
                //OutputDebugString( "    Found!\n" );
            }
        }
        if (!bBound) 
        {
            //OutputDebugString( "    NOT FOUND!\n" );
            m_InputBones.push_back( NULL );
            bAllBound = false;
        }
        else nBound++;
    }
    m_bBonesBound = true; 
    return bAllBound;
} // Skin::BindBones

int Skin::s_SkinVBuffer = -1;
int Skin::s_SkinIBuffer = -1;

void Skin::OnProcessGeometry()
{
    if (!m_bBonesBound) BindBones();

    //  setup bone transforms
    int nTM = m_InputBones.size();
    for (int i = 0; i < nTM; i++)
    {
        m_BoneTM[i] = GetMatrix( i );
        m_BoneTM[i].mulLeft( m_BoneOffsets[i] );
        
        /*char buf[256];
        sprintf( buf, "Bone %s: %f %f %f\n", 
                    m_InputBones[i] ? m_InputBones[i]->GetName() : "NO BONE",
                    m_BoneTM[i].e30, m_BoneTM[i].e31, m_BoneTM[i].e32 );   
        OutputDebugString( buf );*/
    }
    const Matrix4D* pBones = m_BoneTM;    

    //  
    BaseMesh& skin  = GetMesh();
    BaseMesh& bm    = GetBaseMesh();
    VertexOut*    dBuf = (VertexOut*)skin.getVertexData();

    if (s_SkinIBuffer == -1) s_SkinIBuffer = IRS->GetIBufferID( "SharedStatic" );
    if (s_SkinVBuffer == -1) s_SkinVBuffer = IRS->GetVBufferID( "SharedDynamic" );

    int vType = (int)vfVertexN;
    int nV = skin.getNVert();
    int nI = skin.getNInd();
    if (nI == 0 || nV == 0) return;
    switch (m_NWeights)
    {        
    case 1: 
        {
            Vertex1W*    sBuf = (Vertex1W*)m_BaseMesh.getVertexData();
            Skin1( sBuf, dBuf, nV, pBones );
        } break;
    case 2: 
        {    
            Vertex2W*    sBuf = (Vertex2W*)m_BaseMesh.getVertexData();
            Skin2( sBuf, dBuf, nV, pBones );    
        } break;
    case 3: 
        {
            Vertex3W*    sBuf = (Vertex3W*)m_BaseMesh.getVertexData();
            Skin3( sBuf, dBuf, nV, pBones );            
        } break;
    case 4: 
        {
            Vertex4W*    sBuf = (Vertex4W*)m_BaseMesh.getVertexData();
            Skin4( sBuf, dBuf, nV, pBones );            
        } break;
    }


} // Skin::ProcessSkin

void Skin::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "Skin", this );
    pm.p( "NumBones", &Skin::GetNBones );
} // Skin::Expose

//////////////////////////////////////////////////////////////////////////

std::vector<Vector3D> P_list;
std::vector<Vector3D> N_list;
std::vector<Vector3D> T_list;
std::vector<Vector3D> B_list;

void SkinGPU::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "SkinGPU", this );
	pm.p( "NumBones", &SkinGPU::GetNBones );
} // SkinGPU::Expose
SkinGPU::SkinGPU(){
	m_MatrixArrayInShader=-1;
}
SkinGPU::~SkinGPU(){
}
void SkinGPU::Render(){
	if (!m_bBonesBound) BindBones();

	//  setup bone transforms
	int nTM = m_InputBones.size();
	for (int i = 0; i < nTM; i++)
	{
		m_BoneTM[i] = GetMatrix( i );
		m_BoneTM[i].mulLeft( m_BoneOffsets[i] );
	}
	const Matrix4D* pBones = m_BoneTM;
	if(m_MatrixArrayInShader==-1)
		m_MatrixArrayInShader=IRS->GetShaderVarID(IRS->GetShader(),"BonesArray");
	Vector4D V[256];
	if(m_MatrixArrayInShader!=-1){		
		Vector4D* v=&V[0];
		for(int i=0;i<nTM;i++){
			Matrix4D& m=m_BoneTM[i];
			*(v)=Vector4D(m.e00,m.e10,m.e20,m.e30);v++;
            *(v)=Vector4D(m.e01,m.e11,m.e21,m.e31);v++;
			*(v)=Vector4D(m.e02,m.e12,m.e22,m.e32);v++;
		}
		IRS->SetShaderVar(IRS->GetShader(),m_MatrixArrayInShader,&V[0],nTM*3);
	}
	m_Mesh.setTexture(IRS->GetTexture());	
	m_Mesh.render();
	IRS->Flush();
	/*
	int n=P_list.size();
	float c=0.3;
	for(int i=0;i<n;i++){
		rsLine(P_list[i],P_list[i]+N_list[i]*c,0xFFFF0000);
		rsLine(P_list[i],P_list[i]+T_list[i]*c,0xFF00FF00);
		rsLine(P_list[i],P_list[i]+B_list[i]*c,0xFF0000FF);
	}
	*/
}
void SkinGPU::Serialize( OutStream& os ) const
{
	SNode::Serialize( os );
	BaseMesh bm;
	m_Mesh.ToMesh(bm);
	os << bm;
	os << m_BoneOffsets;
	os << m_InputBoneNames;	
} // Skin::Serialize

void SkinGPU::Unserialize( InStream& is )
{
	SNode::Unserialize( is );
	BaseMesh bm;	
	is >> bm;
	is >> m_BoneOffsets;
	is >> m_InputBoneNames;
	m_Mesh.FromMesh(bm);
	m_MatrixArrayInShader=IRS->GetShaderVarID(m_BaseMesh.getShader(),"BonesArray");
	m_AABB = m_BaseMesh.GetAABB();
	m_NWeights=3;
	m_NBones = m_BoneOffsets.size();
	aligned_delete_nodestruct( m_BoneTM );
	m_BoneTM = aligned_new<Matrix4D>( m_NBones, 32 );
	m_bBonesBound = false;
	//m_AABB = m_Mesh.GetAABB();
} // SkinGPU::Unserialize
#define vmod 3
DWORD ToDW(float x,int sh=0){
	int V=x*255;
	if(V<0)V=0;
	if(V>255)V=255;
	return DWORD(V)<<sh;
}
void Vertex4WtoUV3C(const Vertex4W* vs,VertexUV3C* vd){
    vd->u=vs->u;
	vd->v=vs->v;	

	DWORD m[4]={vs->m0,vs->m1,vs->m2,vs->m3};
	float w[4]={vs->w0,vs->w1,vs->w2,1-vs->w0-vs->w1-vs->w2};

	//sorting weights	
    bool c;
	do{
        c=false;
		for(int i=1;i<4;i++){
			if(w[i-1]<w[i]){
				swap(w[i-1],w[i]);
				swap(m[i-1],m[i]);
				c=true;
			}
		}
	}while(c);	
	float sw=w[0]+w[1]+w[2];
	if(sw>0){
		w[0]/=sw;
		w[1]/=sw;
		w[2]=1-w[0]-w[1];
		w[3]=0;
	}
	for(int i=1;i<4;i++){
		if(w[i]<0)w[i]=0;
		if(w[i]>1)w[i]=1;
	}
	vd->diffuse=DWORD(vs->normal.x*127+128)+(DWORD(vs->normal.y*127+128)<<8)+(DWORD(vs->normal.z*127+128)<<16)+ToDW(w[0],24);
	vd->specular=0;
	vd->color2=(m[0]*vmod)+((m[1]*vmod)<<8)+((m[2]*vmod)<<16)+ToDW(w[1],24);
}
void Vertex3WtoUV3C(const Vertex3W* vs,VertexUV3C* vd){
	vd->u=vs->u;
	vd->v=vs->v;
	vd->diffuse=DWORD(vs->normal.x*127+128)+(DWORD(vs->normal.y*127+128)<<8)+(DWORD(vs->normal.z*127+128)<<16)+ToDW(vs->w0,24);
	vd->specular=0;
	vd->color2=vs->m0*vmod+((vs->m1*vmod)<<8)+((vs->m2*vmod)<<16)+ToDW(vs->w1,24);	
}
void Vertex2WtoUV3C(const Vertex2W* vs,VertexUV3C* vd){
	vd->u=vs->u;
	vd->v=vs->v;
	vd->diffuse=DWORD(vs->normal.x*127+128)+(DWORD(vs->normal.y*127+128)<<8)+(DWORD(vs->normal.z*127+128)<<16)+ToDW(vs->w,24);
	vd->specular=0;
	vd->color2=vs->m0*vmod+((vs->m1*vmod)<<8)+ToDW(vs->w,24);
}
void Vertex1WtoUV3C(const Vertex1W* vs,VertexUV3C* vd){
	vd->u=vs->u;
	vd->v=vs->v;
	vd->diffuse=DWORD(vs->normal.x*127+128)+(DWORD(vs->normal.y*127+128)<<8)+(DWORD(vs->normal.z*127+128)<<16)+0xFF000000;
	vd->specular=0;
	vd->color2=vs->m;
}
void VertexNtoUV3C(const VertexN* vs,VertexUV3C* vd){
	vd->u=vs->u;
	vd->v=vs->v;
	vd->diffuse=DWORD(vs->nx*127+128)+(DWORD(vs->ny*127+128)<<8)+(DWORD(vs->nz*127+128)<<16)+0xFF000000;
	vd->specular=0;
	vd->color2=0;
}

void CalcBiNormal(VertexUV3C* Verts,int nV,WORD* idxs,int nI){	
	VertexBump* vb=new VertexBump[nV];
	VertexBump* vbc=vb;
	VertexUV3C* vn=Verts;
	for(int i=0;i<nV;i++){
		vbc->x=vn->x;
		vbc->y=vn->y;
		vbc->z=vn->z;
		vbc->nx=(float(vn->diffuse&0xFF)-128.0f)/127.0f;
		vbc->ny=(float((vn->diffuse>>8)&0xFF)-128.0f)/127.0f;
		vbc->nz=(float((vn->diffuse>>16)&0xFF)-128.0f)/127.0f;
		vbc->u=vn->u;
		vbc->v=vn->v;
		vbc->tx=vbc->ty=vbc->tz=vbc->bx=vbc->by=vbc->bz=0;
		vbc++;
		vn++;
	}
	Vector3D v;
	Vector3D vnrm;
	//calculating txyz,bxyz
	for(int i=0;i<nI;i+=3){
		VertexBump* v1=vb+idxs[i  ];
		VertexBump* v2=vb+idxs[i+1];
		VertexBump* v3=vb+idxs[i+2];

		Vector3D V1=Vector3D(v1->x,v1->y,v1->z);
		Vector3D V2=Vector3D(v2->x,v2->y,v2->z);
		Vector3D V3=Vector3D(v3->x,v3->y,v3->z);

		//Vector3D DU=(V2-V1)*(v2->u-v1->u)+(V3-V2)*(v3->u-v2->u)+(V1-V3)*(v1->u-v3->u);
		//Vector3D DV=(V2-V1)*(v2->v-v1->v)+(V3-V2)*(v3->v-v2->v)+(V1-V3)*(v1->v-v3->v);
		//DU.normalize();
		//DV.normalize();
		//Vector3D sq;
		//sq.cross(V1-V0,V2-V0);
		//float s=sq.norm();
		//DU*=s;
		//DV*=s;

		/*
		Vector3D e0(V1.x - V0.x, v1->u - v0->u, v1->v - v0->v);
		Vector3D e1(V2.x - V0.x, v2->u - v0->u, v2->v - v0->v);
		Vector3D c;
		Vector3D Tangent;
		Vector3D Binormal;
		c.cross(e0, e1);		

		Tangent.x = -c.y / c.x;
		Binormal.x = -c.z / c.x;	

		e0.x = V1.y - V0.y;
		e1.x = V2.y - V0.y;
		c.cross(e0, e1);		
		Tangent.y = - c.y / c.x;
		Binormal.y = - c.z / c.x;		

		e0.x = V1.z - V0.z;
		e1.x = V2.z - V0.z;
		c.cross(e0, e1);		
		Tangent.z = - c.y / c.x;
		Binormal.z = - c.z / c.x;

		Tangent.normalize();
		Binormal.normalize();
		Tangent*=c;
		Binormal*=c;
		*/
		
		float x1 = V2.x - V1.x;
		float x2 = V3.x - V1.x;
		float y1 = V2.y - V1.y;
		float y2 = V3.y - V1.y;
		float z1 = V2.z - V1.z;
		float z2 = V3.z - V1.z;

		float s1 = v2->u - v1->u;
		float s2 = v3->u - v1->u;
		float t1 = v2->v - v1->v;
		float t2 = v3->v - v1->v;

		float r = 1.0f / (s1 * t2 - s2 * t1);
		Vector3D Tangent((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r);
		Vector3D Binormal((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r);       
		

		float dT1=Tangent.dot(V2-V1)*(v2->u-v1->u);
		float dT2=Tangent.dot(V3-V1)*(v3->u-v1->u);		
		if(abs(dT2)>abs(dT1))dT1=dT2;
		if(dT1<0)Tangent*=-1;
		float dB1=Binormal.dot(V2-V1)*(v2->v-v1->v);
		float dB2=Binormal.dot(V3-V1)*(v3->v-v1->v);
		if(abs(dB2)>abs(dB1))dB1=dB2;
		if(dB1<0)Binormal*=-1;

		//Tangent.normalize();
		//Binormal.normalize();

		Vector3D sq;
		sq.cross(V2-V1,V3-V1);
		float s=sq.norm();

		//Tangent*=s;
		//Binormal*=s;

		v1->tx+=Tangent.x;
		v1->ty+=Tangent.y;
		v1->tz+=Tangent.z;
		v1->bx+=Binormal.x;
		v1->by+=Binormal.y;
		v1->bz+=Binormal.z;

		v2->tx+=Tangent.x;
		v2->ty+=Tangent.y;
		v2->tz+=Tangent.z;
		v2->bx+=Binormal.x;
		v2->by+=Binormal.y;
		v2->bz+=Binormal.z;

		v3->tx+=Tangent.x;
		v3->ty+=Tangent.y;
		v3->tz+=Tangent.z;
		v3->bx+=Binormal.x;
		v3->by+=Binormal.y;
		v3->bz+=Binormal.z;		
	}
	vn=Verts;
	vbc=vb;
	/*
	for(int i=0;i<nV;i++){
		Vector3D vnrm(vbc->nx,vbc->ny,vbc->nz);		
		vnrm.normalize();
		Vector3D vbin(vbc->bx,vbc->by,vbc->bz);
		vbin-=vnrm*vnrm.dot(vbin);
		vbin.normalize();		
		Vector3D vtri(vbc->tx,vbc->ty,vbc->tz);
		vtri-=vnrm*vnrm.dot(vtri);
		vtri.normalize();
		if(abs(vbin.dot(vtri))>0.99){
			vtri.cross(vbin,vnrm);
			vtri.normalize();			
		}
		Vector3D vn2;		
		vn2.cross(vbin,vtri);
		vn2.normalize();
		float sg=vnrm.dot(vn2);		
		vn->diffuse=(vn->diffuse&0xFF000000)+DWORD(128+vbin.z*127)+(DWORD(128+vbin.y*127)<<8)+(DWORD(128+vbin.x*127)<<16);
		vn->specular=DWORD(128+vtri.z*127)+(DWORD(128+vtri.y*127)<<8)+(DWORD(128+vtri.x*127)<<16)+(sg>0?0xFF000000:0);
		vn++;
		vbc++;
	}
	*/	
	for(int i=0;i<nV;i++){
		Vector3D vnrm(vbc->nx,vbc->ny,vbc->nz);		
		vnrm.normalize();
		Vector3D vtang(vbc->tx,vbc->ty,vbc->tz);
		vtang-=vnrm*vtang.dot(vnrm);
		vtang.normalize();		
		Vector3D vbin(vbc->bx,vbc->by,vbc->bz);
		vbin-=vnrm*vbin.dot(vnrm);
		vbin.normalize();
		Vector3D vn2;
		vn2.cross(vnrm,vbin);
		vn2.normalize();
		float sg=vtang.dot(vn2);
		vn->diffuse=(vn->diffuse&0xFF000000)+DWORD(128+vtang.z*127)+(DWORD(128+vtang.y*127)<<8)+(DWORD(128+vtang.x*127)<<16);
		vn->specular=DWORD(128+vnrm.z*127)+(DWORD(128+vnrm.y*127)<<8)+(DWORD(128+vnrm.x*127)<<16)+(sg>0?0xFF000000:0);

		/*
		P_list.push_back(Vector3D(vbc->x,vbc->y,vbc->z));
		N_list.push_back(vnrm);
		T_list.push_back(vtang);
		B_list.push_back(vbin);
		*/

		vn++;
		vbc++;
	}
	delete[]vb;
}
void ConvertGeomToSkinGPU(Geometry* skin,SkinGPU* sgpu){
	sgpu->SetName(skin->GetName());
	BaseMesh* bm=&skin->GetMesh();		
	int ni=bm->getNInd();
	int nv=bm->getNVert();
	StaticMesh* sm=sgpu->GetStaticMesh();
	sm->create(nv,ni,vfVertexUV3C);
	WORD* idxs=sm->getIndices();
	memcpy(idxs,bm->getIndices(),ni<<1);
	VertexUV3C* newv=(VertexUV3C*)sm->getVertexData();
	VertexFormat vf=bm->getVertexFormat();	
	for(int i=0;i<nv;i++){
		Vector3D V=bm->getVertexXYZ(i);
		newv->x=V.x;
		newv->y=V.y;
		newv->z=V.z;
		if(vf==vfVertexN){
			VertexNtoUV3C((VertexN*)bm->getVertexData(i),newv);
		}
		newv++;
	}
	CalcBiNormal((VertexUV3C*)sm->getVertexData(),nv,idxs,ni);
	sm->unlock();    
	sm->setNInd(ni);
	sm->setNVert(nv);
	sm->setNPri(ni/3);

	sgpu->m_NBones = 0;
	sgpu->m_BoneTM = NULL;
	sgpu->m_bBonesBound = true;
}
void ConvertSingleNodeToSkinGPU(Skin* skin,SkinGPU* sgpu){
	sgpu->SetName(skin->GetName());
	BaseMesh* bm=&skin->GetBaseMesh();		
	int ni=bm->getNInd();
	int nv=bm->getNVert();
	StaticMesh* sm=sgpu->GetStaticMesh();
	sm->create(nv,ni,vfVertexUV3C);
	WORD* idxs=sm->getIndices();
	memcpy(idxs,bm->getIndices(),ni<<1);
	VertexUV3C* newv=(VertexUV3C*)sm->getVertexData();
	VertexFormat vf=bm->getVertexFormat();	
	for(int i=0;i<nv;i++){
        Vector3D V=bm->getVertexXYZ(i);
		newv->x=V.x;
		newv->y=V.y;
		newv->z=V.z;
		switch(vf){
		case vfVertex4W:
			Vertex4WtoUV3C((Vertex4W*)bm->getVertexData(i),newv);
			break;
		case vfVertex3W:
			Vertex3WtoUV3C((Vertex3W*)bm->getVertexData(i),newv);
			break;
		case vfVertex2W:
			Vertex2WtoUV3C((Vertex2W*)bm->getVertexData(i),newv);
			break;
		case vfVertex1W:
			Vertex1WtoUV3C((Vertex1W*)bm->getVertexData(i),newv);
			break;
		}
		newv++;
	}
	CalcBiNormal((VertexUV3C*)sm->getVertexData(),nv,idxs,ni);
    sm->unlock();    
	sm->setNInd(ni);
	sm->setNVert(nv);
	sm->setNPri(ni/3);

	for(int i=0;i<skin->m_InputBoneNames.size();i++)
		sgpu->m_InputBoneNames.push_back(skin->m_InputBoneNames[i].c_str());
	for(int i=0;i<skin->m_BoneOffsets.size();i++)
		sgpu->m_BoneOffsets.push_back(skin->m_BoneOffsets[i]);

	sgpu->m_NBones = skin->m_BoneOffsets.size();	
	sgpu->m_BoneTM = aligned_new<Matrix4D>( sgpu->m_NBones, 32 );
	sgpu->m_bBonesBound = false;
}
AABoundBox    CalculateAABB( SNode* pNode );
SNode* ConvertNodesToSkinGPU(SNode* Node,bool &changed){
	Skin* skin=dynamic_cast<Skin*>(Node);
	if(skin){
		SkinGPU* g=dynamic_cast<SkinGPU*>(Node);
		if(!g){
			SkinGPU* sgpu=new SkinGPU;
			ConvertSingleNodeToSkinGPU(skin,sgpu);		
			Node=sgpu;
			changed=true;
		}
	}else{
		/*
		Geometry* G=dynamic_cast<Geometry*>(Node);
		if(G){
			SkinGPU* sgpu=new SkinGPU;
			ConvertGeomToSkinGPU(G,sgpu);
			Node=sgpu;
			changed=true;
		}
		*/
	}
	bool wasgeo=false;
	int n=Node->NumChildren();	
	for(int i=0;i<n;i++){
        SNode* sn=Node->GetChild(i);
		SNode* newnode=ConvertNodesToSkinGPU(sn,changed);
		if(newnode!=sn){
			newnode->SetParent(Node);
			Node->SetChild(i,newnode);
			sn->Release();			
			IRS->GetShaderVarID(IRS->GetShader(),"BonesArray");			
			wasgeo=true;
		}
	}	
	if(wasgeo){
		for(int i=0;i<n;i++){
			SNode* sn=Node->GetChild(i);
			Shader* sh=dynamic_cast<Shader*>(sn);
			if(sh){
				int sid=IRS->GetShaderID(sh->GetName());
				if(sid!=-1){
					int shv=IRS->GetShaderVarID(sid,"BonesArray");
					if(shv==-1){
						//sh->SetName("bump_normalmap");
						sh->SetName("bump_default");
					}
				}
			}
		}	
	}
	return Node;
}
bool ConvertNodesToSkinGPU(SNode* Node){
	bool ch=false;
	ConvertNodesToSkinGPU(Node,ch);		
	CalculateAABB(Node);
	return ch;
}
StaticMesh* SkinGPU::GetStaticMesh(){
	return &m_Mesh;
}