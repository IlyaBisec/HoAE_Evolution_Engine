/*****************************************************************************/
/*    File:    sgGeometry.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "sgTransformNode.h"
#include "sgGeometry.h"
#include "sgDummy.h"

#include "kIOHelpers.h"
#include "IFontManager.h"
#include "mSpatial.h"
#include "vSkin.h"
#ifndef _INLINES
#include "sgGeometry.inl"
#endif // !_INLINES
#include "sgAttachedEffect.h"

IMPLEMENT_CLASS( Geometry           );
IMPLEMENT_CLASS( GeometryRef        );
/*****************************************************************************/
/*    Geometry    implementation
/*****************************************************************************/
void Geometry::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Mesh;
} // Geometry::Serialize

void Geometry::Unserialize( InStream& is ) 
{
    Parent::Unserialize( is );
    is >> m_Mesh;
    m_AABB = m_Mesh.GetAABB();
} // Geometry::Serialize

void Geometry::SetIsStatic( bool val ) 
{ 
    m_Mesh.setIsStatic( val ); 
    m_Mesh.setDevHandle( NULL ); 
}

void Geometry::Create( int nVert, int nIdx, VertexFormat vertFormat, PrimitiveType ptype )
{
    m_Mesh.create( nVert, nIdx, vertFormat, ptype );
}

void Geometry::PostRender()
{
    IRS->ResetWorldTM();
    if (m_bShowMeshNormals)
    {
        VertexIterator it;
        it << GetMesh();
        while (it)
        {
            Vector3D norm = it.normal();
            Vector3D pos  = it.pos();
            norm *= m_NormalLen;
            norm += pos;
            rsLine( pos, norm, 0xFFFF0000, 0xFFFF0000 );
            ++it;
        }
        rsFlushLines3D();
    }

    if (DoDrawGizmo())
    {
        static int shWire = IRS->GetShaderID( "wire" );
        IRS->SetShader( shWire );
        DrawPrimBM( m_Mesh );
        IRS->Flush();
    }
} // Geometry::PostRender

int Geometry::s_VBuffer = -1;
int Geometry::s_IBuffer = -1;

void Geometry::Render()
{
    SNode::Render();
    IRS->SetWorldTM( TransformNode::TMStackTop() );
    
    BaseMesh& bm = GetMesh();

    if (s_IBuffer == -1) s_IBuffer = IRS->GetIBufferID( "SharedStatic" );
    if (s_VBuffer == -1) s_VBuffer = IRS->GetVBufferID( "SharedStatic" );

    int nV      = bm.getNVert();
    int nI      = bm.getNInd();
    int nBytes  = nV*bm.getVertexStride();
    int vType   = (int)bm.getVertexFormat();

    IRS->SetVB( s_VBuffer, vType );

    if (!IRS->IsIBStampValid( s_IBuffer, m_ICacheStamp ))
    {
        BYTE* pIdx  = IRS->LockAppendIB( s_IBuffer, nI, m_IBufferPos, m_ICacheStamp );
        if (!pIdx) return;
        memcpy( pIdx, bm.getIndices(), nI*sizeof(WORD) );
        IRS->UnlockIB( s_IBuffer );
    }

    if (!IRS->IsVBStampValid( s_VBuffer, m_VCacheStamp ))
    {
        BYTE* pOut  = IRS->LockAppendVB( s_VBuffer, nV, m_VBufferPos, m_VCacheStamp );
        if (!pOut) return;
        memcpy( pOut, bm.getVertexData(), nV*bm.getVertexStride() );
        IRS->UnlockVB( s_VBuffer );
    }

    RenderTask& rt = IRS->AddTask();
    rt.m_ShaderID       = IRS->GetShader();
    rt.m_TexID[0]       = IRS->GetTexture( 0 );
    rt.m_TexID[1]       = IRS->GetTexture( 1 );
    rt.m_TexID[2]       = -1;
    rt.m_bTransparent   = true;
    rt.m_VType          = vType;
    rt.m_NVert          = nV;
    rt.m_NIdx           = nI;
    rt.m_PriType        = bm.getPriType();

    rt.m_VBufID         = s_VBuffer;
    rt.m_FirstVert      = m_VBufferPos;
    rt.m_IBufID         = s_IBuffer;
    rt.m_FirstIdx       = m_IBufferPos;
    rt.m_Source         = GetName();
    rt.m_bHasTM         = true; 
    rt.m_TM             = TransformNode::TMStackTop();     
    
    IRS->Flush();
    PostRender();
} // Geometry::Render

void Geometry::GetVertexIterator( VertexIterator& it )
{
    it << m_Mesh;
} // Geometry::GetVertexIterator

DWORD Geometry::GetDiffuse() const
{
    if (m_Mesh.getNVert() == 0) return 0;
    BaseMesh* pri = const_cast<BaseMesh*>( &m_Mesh );
    VertexIterator vit;
    vit << *pri;
    int nV = 0;
    float a, r, g, b;
    float ta = 0.0f;
    float tr = 0.0f;
    float tg = 0.0f;
    float tb = 0.0f;

    while (vit)
    {
        ColorValue::FromARGB( vit.diffuse(), a, r, g, b );
        ta += a;
        tr += r;
        tg += g;
        tb += b;

        nV++;
        ++vit;
    }
    
    float fnv = float( nV );
    return ColorValue::ToARGB( ta/fnv, tr/fnv, tg/fnv, tb/fnv );
} // Geometry::GetDiffuse
 
void Geometry::SetDiffuse( DWORD color )
{
    GetMesh().setDiffuseColor( color );
} // Geometry::SetDiffuse

void Geometry::Expose( PropertyMap&  pm )
{
    pm.start<Parent>( "Geometry", this );
    pm.p( "Polygons", &Geometry::GetNPoly );
    pm.p( "Vertices", &Geometry::GetNVert );
    pm.p( "Diffuse", &Geometry::GetDiffuse, &Geometry::SetDiffuse, "color" );
    pm.p( "Static", &Geometry::IsStatic, &Geometry::SetIsStatic );
    pm.p( "PrimitiveType", &Geometry::GetPriType        );
    pm.p( "VertexFormat", &Geometry::GetVertexFormat    );
    pm.f( "AABB xyz",        m_AABB.minv, "direction", true );
    pm.f( "AABB XYZ",        m_AABB.maxv, "direction", true );

    pm.f( "ShowMeshNormals",m_bShowMeshNormals    );
    pm.f( "MeshNormalLen",    m_NormalLen            );
    pm.m( "CalculateNormals", &Geometry::CalculateNormals );
    pm.m( "FlipNormals", &Geometry::FlipNormals );
    pm.m( "DumpToCPP", &Geometry::DumpToCPP );
    pm.m( "AttachEffect", &Geometry::AttachEffect );
} // Geometry::Expose

void Geometry::CalculateNormals()
{
    m_Mesh.calcNormals();
}

void Geometry::FlipNormals()
{
    VertexIterator vit;
    vit << m_Mesh;
    while (vit)
    {
        vit.normal().reverse();
        ++vit;
    }
} // Geometry::FlipNormals

int    Geometry::Pick( const Vector3D& org, const Vector3D& dir, Vector3D& pt )
{
    return -1;
}

Sphere Geometry::GetBoundSphere()
{
    Vector3D c;
    float r = 0.0f;
    m_Mesh.GetCentroid( c );

    VertexIterator it;
    it << m_Mesh;
    while (it)
    {
        float d2 = c.distance2( it.pos() );
        if (d2 > r) r = d2;
        ++it;
    }
    return Sphere( c, sqrtf( r ) );
} // Geometry::GetBoundSphere

void Geometry::DumpToCPP()
{
    FILE* fp = fopen( "c:\\dumps\\geomdump.cpp", "wt" );
    if (!fp) return;
    DumpToCPP( fp );
    fclose( fp );
}

void Geometry::DumpToCPP( FILE* fp )
{
    VertexIterator v;
    v << m_Mesh;
    fprintf( fp, "const float c_%sV[%d][5] = {\n", GetName(), m_Mesh.getNVert() );
    while (v)
    {
        Vector3D vec = v.pos();
        fprintf( fp, "\t{%.5f, %.5f, %.5f, %.5f, %.5f},\n", vec.x, vec.y, vec.z, v.uv(0).u, v.uv(0).v );
        ++v;
    }
    fprintf( fp, "};\n\n" );
    
    WORD* idx = m_Mesh.getIndices();
    int nPri = m_Mesh.getNPri();
    fprintf( fp, "const WORD c_%sI[%d][3] = {\n", GetName(), nPri );
    for (int i = 0; i < nPri; i++)
    {
        fprintf( fp, "\t{%d, %d, %d},\n", idx[i*3], idx[i*3+1], idx[i*3+2] );
    }
} // Geometry::DumpToCPP

/*****************************************************************************/
/*    GeometryRef    implementation
/*****************************************************************************/
void GeometryRef::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );

    os << firstInd << nInd << firstVert << nVert;

    DWORD poolID = c_BadID;
    if (pool)
    {
        NodePtrMap::iterator it = s_NodeMap.find( (SNode*)pool );
        if (it != s_NodeMap.end())
        {
            poolID = (*it).second;
        }
    }

    os << poolID;
} // GeometryRef::Serialize

void GeometryRef::Unserialize( InStream& is ) 
{
    Parent::Unserialize( is );

    DWORD poolID;
    is >> firstInd >> nInd >> firstVert >> nVert >> poolID;
} // GeometryRef::Serialize


void FlattenStaticHierarchy( SNode* pRootNode )
{
    if (!pRootNode) return;
    SNode::Iterator it( pRootNode );
    
    Matrix4D rootTM = GetWorldTM( pRootNode );
    Matrix4D invRootTM;
    invRootTM.inverse( rootTM );

    Group* pResult = new Group();

    Geometry* pBase = NULL;
    
    while (it)
    {
        SNode* pNode = (SNode*)*it;
        if (pNode->IsInvisible()) it.Up();
        if (pNode->HasFn( Geometry::Magic() ))
        {
            Geometry* pGeom = (Geometry*)pNode;
            Matrix4D geomTM( GetWorldTM( pNode ) );
            geomTM *= invRootTM;
            pGeom->GetMesh().transform( geomTM );
            //pResult->AddChild( pGeom );

            if (pBase)
            {
                pBase->GetMesh() += pGeom->GetMesh();
            }
            else
            {
                pBase = pGeom;
                pResult->AddChild( pBase );
            }
        }
        ++it;
    }

    if (pResult->GetNChildren() > 0) 
    {
        pRootNode->ReleaseChildren();
        pRootNode->AddChild( pResult );
    }
    else
    {
        delete pResult;
    }
} // FlattenStaticHierarchy

static BYTE* s_VertArray;
static int     s_VertStride;
static Ray3D s_Ray;

int DistCmp( const void* p1, const void* p2 )
{
    WPoly* pPoly1 = (WPoly*)p1;
    WPoly* pPoly2 = (WPoly*)p2;
    
    Triangle tri1;
    pPoly1->GetTriangle( tri1, s_VertArray, s_VertStride );

    Triangle tri2;
    pPoly2->GetTriangle( tri2, s_VertArray, s_VertStride );
    
    float d1 = tri1.Distance( s_Ray );
    float d2 = tri2.Distance( s_Ray );
    
    return d1 < d2;
} // DistCmp

void SortPolys( const Ray3D& ray, BaseMesh& bm )
{
    /*BSPTree<VertexN, WPoly> bsp;
    bsp.Create( (VertexN*)bm.getVertexData(), bm.getNVert(), 
                (WPoly*)bm.getIndices(), bm.getNPri() );*/
    
    s_VertStride = bm.getVertexStride();
    s_VertArray  = (BYTE*)bm.getVertexData();
    s_Ray = ray;
    qsort( bm.getIndices(), bm.getNPri(), sizeof( WPoly ), DistCmp );
    
} // SortPolys

int CountPolygons( SNode* pNode )
{
    int nP = 0;
    SNode::Iterator it( pNode, Geometry::FnFilter );
    while (it) 
    {
        Geometry* pGeom = (Geometry*)*it;
        BaseMesh& bm = pGeom->GetMesh();
        nP += bm.getNPri();
        ++it;
    }
    return nP;
} // CountPolygons

int CountVertices( SNode* pNode )
{
    int nV = 0;
    SNode::Iterator it( pNode, Geometry::FnFilter );
    while (it) 
    {
        Geometry* pGeom = (Geometry*)*it;
        BaseMesh& bm = pGeom->GetMesh();
        nV += bm.getNVert();
        ++it;
    }
    return nV;
} // CountVertices

Sphere GetStaticBoundSphere( SNode* pNode )
{
    Sphere sphere( Vector3D::null, 0.0f );
    if (!pNode) return sphere; 

    SNode::Iterator it( pNode, Geometry::FnFilter );
    
    bool bFirst = true;

    while (it)
    {
        Geometry* pGeom = (Geometry*)*it;
        Sphere locSphere = pGeom->GetBoundSphere();
        locSphere.Transform( GetWorldTM( pGeom ) );
        if (bFirst)
        {
            bFirst = false;
            sphere = locSphere;
            ++it;
        }
        sphere += locSphere;
        ++it;
    }
    return sphere;
} // GetStaticBoundSphere

AABoundBox    CalculateAABB( SNode* pNode )
{
    TransformNode::ResetTMStack();
    pNode->Render();

    AABoundBox aabb( Vector3D::null, 0.0f );
    if (!pNode) 
    {
        return aabb;
    }

    SNode::Iterator it( pNode );
    bool bFirst = true;

    while (it)
    {   
		Skin* pSkin = dynamic_cast<Skin*>( (SNode*)*it );
		if(pSkin){			
			AABoundBox cbox = pSkin->GetBaseMesh().GetAABB();
			cbox.Transform( GetWorldTM( pSkin ) );
			pSkin->SetAABB( cbox );
			if (bFirst)
			{
				bFirst = false;
				aabb = cbox;
				continue;
			}
			aabb.Union( cbox );			
			++it;
		}else{
			Geometry* pGeom = dynamic_cast<Geometry*>( (SNode*)*it );
			++it;
			if (!pGeom) continue;
			AABoundBox cbox = pGeom->GetMesh().GetAABB();
			cbox.Transform( GetWorldTM( pGeom ) );
			pGeom->SetAABB( cbox );
			if (bFirst)
			{
				bFirst = false;
				aabb = cbox;
				continue;
			}
			aabb.Union( cbox );
		}
    }
    return aabb;
} // CalculateAABB

SNode* PickNode( const Ray3D& ray, SNode* pNode, Vector3D& pt )
{
    SNode* pSelNode = NULL;
    if (!pNode) return false;
    float minDist = 0.0f;
    SNode::Iterator it( pNode, Geometry::FnFilter );
    while (it)
    {
        Geometry* pGeom = (Geometry*)*it;
        BaseMesh& bm = pGeom->IsA<Skin>() ? 
                    ((Skin*)pGeom)->GetBaseMesh() : pGeom->GetMesh();

        Matrix4D tm = GetWorldTM( pGeom );
        Matrix4D invTM;
        invTM.inverse( tm );
        Ray3D tray( ray ); 
        invTM.transformPt( tray.Orig() );
        invTM.transformVec( tray.Dir() );
        int triIdx = -1;
        float dist = bm.PickPoly( tray, triIdx );
        if (triIdx >= 0)
        {
            pt = tray.getPoint( dist );
            tm.transformPt( pt );
            dist = pt.distance( ray.getOrig() );
            if (dist < minDist || !pSelNode)
            {
                pSelNode = pGeom;
                minDist = dist;
            }
        }
        ++it;
    }
    return pSelNode;
} // PickNode


bool Split( const Primitive& mesh, const Plane& plane, Primitive& posMesh, Primitive& negMesh )
{
    if (mesh.getPriType() != ptTriangleList) return false;
    WPoly* poly = (WPoly*)mesh.getIndices();
    int nP = mesh.getNPri();

    Triangle tri;
    int stride = mesh.getVertexStride();
    for (int i = 0; i < nP; i++)
    {
        poly->GetTriangle( tri, mesh.getVertexData(), stride );
        XStatus xs = tri.Intersect( plane );
        //if (xs == xsInside) posMesh.Add( tri );
        //else if (xs == xsOutside) negMesh.Add( tri );
        //else
        ////  split triangle
        //{
        //
        //}
    }
    return true;
} // Split


DIALOGS_API void ScanHeightmap( const Primitive& pri, const Matrix4D& tm, 
                   float stepx, float stepy, SetHeightCallback put )
{
    assert( put );
    AABoundBox aabb, AABB;
    aabb = pri.GetAABB();
    AABB = aabb;
    AABB.Transform( tm );

    Matrix4D inv;
    inv.inverse( tm );

    float bbdx=AABB.maxv.x-AABB.minv.x;
    float bbdy=AABB.maxv.y-AABB.minv.y;

    AABB.minv.x-=bbdx;
    AABB.minv.y-=bbdy;
    AABB.maxv.x+=bbdx;
    AABB.maxv.y+=bbdy;

    int      begnx = floorf( AABB.minv.x/stepx );
    float begx  = floorf( AABB.minv.x/stepx ) * stepx;

    int      begny = floorf( AABB.minv.y/stepy );
    float begy  = floorf( AABB.minv.y/stepy ) * stepy;

    int ny = begny;
    int triIdx = -1;
    //Vector3D ldir(0,-0.5,0.866);
	static float spd=0.998;
    Vector3D ldir(0,0,spd);
    inv.transformVec(ldir);
    Vector3D ldir1;
    ldir1=inv.getV2();
    Ray3D ray( tm.getTranslation(), ldir );

    Vector3D org( 0, 0, 0 );        
    
    
    for (float y = begy; y <= AABB.maxv.y; y += stepy)
    {
        int nx = begnx;
        for (float x = begx; x <= AABB.maxv.x; x += stepx)
        {
            org.set( x, y, 0.0f );
            inv.transformPt( org );
            ray.setOrig( org );
            float H = pri.PickPoly( ray, triIdx )/* + aabb.minv.z*/;
            //H*= l;
            if (triIdx != -1)
            {
                put( nx, ny, H );
            }
            nx++;
        }
        ny++;
    }        
} // ScanHeightmap


DIALOGS_API void ScanHeightmap( const Primitive& pri, const Matrix4D& tm, 
                   float stepx, float stepy, VisitHeightCallback put )
{
    assert( put );
    AABoundBox aabb, AABB;
    aabb = pri.GetAABB();
    AABB = aabb;
    AABB.Transform( tm );

    Matrix4D inv;
    inv.inverse( tm );

    int      begnx = floorf( AABB.minv.x/stepx );
    float begx  = floorf( AABB.minv.x/stepx ) * stepx;

    int      begny = floorf( AABB.minv.y/stepy );
    float begy  = floorf( AABB.minv.y/stepy ) * stepy;

    int ny = begny;
    int triIdx = -1;

    Vector3D ldir( inv.getV2() );
    Ray3D ray( tm.getTranslation(), ldir );
    Vector3D org( 0, 0, 0 );

    for (float y = begy; y <= AABB.maxv.y; y += stepy)
    {
        int nx = begnx;
        for (float x = begx; x <= AABB.maxv.x; x += stepx)
        {
            org.set( x, y, aabb.minv.z );
            inv.transformPt( org );
            ray.setOrig( org );
            float H = pri.PickPoly( ray, triIdx ) + aabb.minv.z;
            if (triIdx != -1)
            {
                put( nx, ny );
            }
            nx++;
        }
        ny++;
    }        
} // ScanHeightmap


static Vector3D s_Axis;
static int        s_VStride;
static BYTE*    s_Vert;

int __cdecl PolyCmp( const void *t1, const void *t2 )
{
    WORD* pT1 = (WORD*)t1;
    WORD* pT2 = (WORD*)t2;
    Vector3D* v10 = (Vector3D*)(s_Vert + s_VStride*pT1[0]);
    Vector3D* v11 = (Vector3D*)(s_Vert + s_VStride*pT1[1]);
    Vector3D* v12 = (Vector3D*)(s_Vert + s_VStride*pT1[2]);

    Vector3D* v20 = (Vector3D*)(s_Vert + s_VStride*pT2[0]);
    Vector3D* v21 = (Vector3D*)(s_Vert + s_VStride*pT2[1]);
    Vector3D* v22 = (Vector3D*)(s_Vert + s_VStride*pT2[2]);
    
    float d1 = tmax( s_Axis.dot( *v10 ), s_Axis.dot( *v11 ), s_Axis.dot( *v12 ) );
    float d2 = tmax( s_Axis.dot( *v20 ), s_Axis.dot( *v21 ), s_Axis.dot( *v22 ) );
    if (d1 < d2) return -1;
    if (d1 > d2) return  1;
    return 0;
}

void SortPolygons( Primitive& pri, const Vector3D& axis )
{
    WORD* idx    = pri.getIndices();
    int nTri    = pri.getNPri();
    s_Axis        = axis;
    s_VStride    = pri.getVertexStride();
    s_Vert        = (BYTE*)pri.getVertexData();
    qsort( idx, nTri, sizeof(WORD)*3, PolyCmp );
} // SortPolygons

void DrawPolygonNumbers( Primitive& pri, int fontID )
{
    WORD* idx    = pri.getIndices();
    int nTri    = pri.getNPri();
    int stride    = pri.getVertexStride();
    BYTE* vert    = (BYTE*)pri.getVertexData();

    for (int i = 0; i < nTri; i++)
    {
        Vector3D* v0 = (Vector3D*)(vert + stride*idx[i*3 + 0]);
        Vector3D* v1 = (Vector3D*)(vert + stride*idx[i*3 + 1]);
        Vector3D* v2 = (Vector3D*)(vert + stride*idx[i*3 + 2]);
        Vector3D v( *v0 );
        v += *v1;
        v += *v2;
        v *= 1.0f/3.0f;
        char str[256];
        sprintf( str, "%d", i );
        IWM->DrawStringW( fontID, str, v );
    }

} // DrawPolygonNumbers

Vector3D GetRandomPoint( const Primitive& p, MeshPointSample sample )
{
    if (sample == mpsVertex)
    {
        int nV = p.getNVert();
        int vIdx = rndValue( 0, nV - 1 );
        float* v = (float*)(p.getVertexData() + vIdx*p.getVertexStride());
        return Vector3D( v[0], v[1], v[2] );
    }
    assert( false );
    return Vector3D::null;
} // GetRandomPoint

void Geometry::AttachEffect(){
    AttachedEffect* ef = new AttachedEffect;
    ef->SetName( "Effect" );
    AddChild( ef );
}

StaticMesh::StaticMesh(){
    m_Shader=-1;
    for(int i=0;i<4;i++)m_Texture[i]=-1;
    m_VertexSize=0;
    m_VBID=-1;
    m_IBID=-1;
    m_maxVerts=0;
    m_maxIdxs=0;
    m_numIdxs=0;
    m_numVerts=0;
    m_numPri=0;
    m_VB_ptr=NULL;
    m_IB_ptr=NULL;
}
StaticMesh::~StaticMesh(){
    reset();
}
void  StaticMesh::reset(){
	unlock();
	if(m_VBID!=-1)IRS->DeleteVB(m_VBID);
	if(m_IBID!=-1)IRS->DeleteIB(m_IBID);
	m_VBID=-1;
	m_IBID=-1;
}
BYTE* StaticMesh::getVertexData(){
    if(!m_VB_ptr){        
        DWORD Stamp=0;
        m_VB_ptr=IRS->LockVB(m_VBID,0,m_maxVerts,Stamp);        
    }
    return m_VB_ptr;
}
WORD* StaticMesh::getIndices(){
    if(!m_IB_ptr){
        DWORD Stamp=0;
        m_IB_ptr=(WORD*)IRS->LockIB(m_IBID,0,m_maxIdxs,Stamp);        
    }
    return m_IB_ptr;
}
int StaticMesh::getNVert(){
    return m_numVerts;
}
int StaticMesh::getMaxVert(){
    return m_maxVerts;
}
int StaticMesh::getMaxInd(){
    return m_maxIdxs;
}
int StaticMesh::getNInd(){
    return m_numIdxs;
}
int StaticMesh::getNPri(){
    return m_numPri;
}
int StaticMesh::getShader(){
    return m_Shader;
}
int StaticMesh::getTexture( int stage ){
    return m_Texture[ stage ];
}
void StaticMesh::setShader( int shader ){
    m_Shader=shader;
}
void StaticMesh::setTexture( int tex, int stage ){
    m_Texture[ stage ]=tex;
}
void StaticMesh::setNVert( int n ){
    m_numVerts=n;
}
void StaticMesh::setNPri( int n ){
    m_numPri=n;
}
void StaticMesh::setNInd( int n ){
    m_numIdxs=n;
}
void StaticMesh::ToMesh(BaseMesh& bm) const{
	bm.create(m_numVerts,m_numIdxs,m_vf);
	StaticMesh* sm=(StaticMesh*)this;
	memcpy(bm.getVertexData(),sm->getVertexData(),m_numVerts*m_VertexSize);
	memcpy(bm.getIndices(),sm->getIndices(),m_numIdxs*2);
	bm.setNInd(sm->getNInd());
	bm.setNVert(sm->getNVert());
	bm.setNPri(sm->getNPri());
	sm->unlock();
}
void StaticMesh::FromMesh(BaseMesh& bm){
	reset();
	create(bm.getNVert(),bm.getNInd(),bm.getVertexFormat());
	memcpy(getVertexData(),bm.getVertexData(),bm.getNVert()*m_VertexSize);
	memcpy(getIndices(),bm.getIndices(),bm.getNInd()*2);
	setNInd(bm.getNInd());
	setNVert(bm.getNVert());
	setNPri(bm.getNPri());
	unlock();
}
void StaticMesh::create( int numVert, int numIdxs, VertexFormat vf){
    m_vf=vf;
    m_maxVerts=numVert;
    m_maxIdxs=numIdxs;
    m_VertexSize=CreateVertexDeclaration(vf).m_VertexSize;
    static int ID=0;
    ID++;
    char ccV[32];
    sprintf(ccV,"StaticMeshVB%02d",ID);    
    VertexDeclaration m_VDecl=CreateVertexDeclaration( vf );
    m_VType=m_VDecl.m_TypeID = IRS->RegisterVType( m_VDecl );    
    m_VBID=IRS->CreateVB(ccV,m_maxVerts*m_VertexSize,m_VType);
    sprintf(ccV,"StaticMeshIB%02d",ID++);
    m_IBID=IRS->CreateIB(ccV,m_maxIdxs*2,isWORD);    
    m_numIdxs=0;
    m_numVerts=0;
    m_numPri=0;
    m_VB_ptr=NULL;
    m_IB_ptr=NULL;
}
void StaticMesh::unlock(){
    if(m_VB_ptr)IRS->UnlockVB(m_VBID);
    if(m_IB_ptr)IRS->UnlockIB(m_IBID);
    m_VB_ptr=NULL;
    m_IB_ptr=NULL;
}
void StaticMesh::render(){
    RenderTask& rt=IRS->AddTask();
    rt.m_bHasTM         = false;
    rt.m_ShaderID       = IRS->GetShader();//m_Shader;
    rt.m_TexID[0]       = m_Texture[0];
    rt.m_TexID[1]       = m_Texture[1];
    rt.m_TexID[2]       = m_Texture[2];
    rt.m_TexID[3]       = m_Texture[3];
    rt.m_TexID[4]       = -1;
    rt.m_TexID[5]       = -1;
    rt.m_TexID[6]       = -1;
    rt.m_TexID[7]       = -1;
    rt.m_bTransparent   = false;
    rt.m_VBufID         = m_VBID;
    rt.m_FirstVert      = 0;
    rt.m_NVert          = m_numVerts;
    rt.m_IBufID         = m_IBID;
    rt.m_FirstIdx       = 0;
    rt.m_NIdx           = m_numIdxs;
    rt.m_VType          = m_VType;
    rt.m_bHasTM         = false;
    rt.m_Source         = "StaticMesh";    
}