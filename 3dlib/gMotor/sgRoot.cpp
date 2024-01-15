/*****************************************************************************/
/*    File:    sgRoot.cpp
/*    Author:    Ruslan Shestopalyuk
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "sgDummy.h"
#include "sgRoot.h"

#include "sgCursor.h"
#include "sgFog.h"

#include "mHeightmap.h"

#include "sgStatistics.h"
#include "sgReflection.h"
#include "IMediaManager.h"
#include "sgAnimationBlock.h"
#include "ITerrain.h"

#include "sgSpriteManager.h"
#include "sgEffect.h"

#include "uiControl.h"
#include "uiEffectEditor.h"
#include "uiPhysicsEditor.h"

#include "kFilePath.h"

IMPLEMENT_CLASS( Root );

void RenderRoot()
{
    Root::instance()->Render();
}

void InitRoot()
{
    Root::instance()->CreateGuts();
}



class HVector3D : public Vector3D
{
public:
    HVector3D(){}
    HVector3D( const Vector3D& v ) { x = v.x; y = v.y; z = v.z; }
    unsigned int    hash    () const
    {
        unsigned int h = 15731 + int( x ) + int( y )*37 + int( z )*729;
        h = (h * 729) ^ (h >> 1);
        return h;
    }
    bool            equal    ( const HVector3D& v ) { return isEqual( v ); }
    void            copy    ( const HVector3D& v ) { x = v.x; y = v.y; z = v.z; }
}; // class HashedVector

typedef Hash<HVector3D>    VectorHash;

struct MapDirection
{
    std::string             m_CameraName;
    std::string             m_Texture;
    Matrix4D                m_UVTM;
    VectorHash              m_Vertices;
    std::vector<WORD>       m_Indices;
    int                     m_NAddedVertices;

    MapDirection()
    {
        m_NAddedVertices = 0;
    }

    ~MapDirection()
    {
    }

    void AddVertex( const Vector3D& pos )
    {
        HVector3D hv( pos );
        if (FindVertex( pos ) == -1) m_Vertices.add( hv );
    }

    void AddTriangle( const Vector3D& a, const Vector3D& b, const Vector3D& c )
    {
        int nA = FindVertex( a );
        int nB = FindVertex( b );
        int nC = FindVertex( c );
        int nV = m_Vertices.numElem();
        if (nA == -1) 
        { 
            nA = m_Vertices.numElem(); 
            m_Vertices.add( a );
        }
        if (nB == -1) 
        { 
            nB = m_Vertices.numElem(); 
            m_Vertices.add( b );
        }
        if (nC == -1) 
        { 
            nC = m_Vertices.numElem(); 
            m_Vertices.add( c );
        }
        WORD wA = nA;
        WORD wB = nB;
        WORD wC = nC;
        m_Indices.push_back( wA );
        m_Indices.push_back( wB );
        m_Indices.push_back( wC );
    }

    int  FindVertex( const Vector3D& pos )
    {
        return m_Vertices.find( pos );
    }

    void CreateMesh( BaseMesh& bm )
    {
        int nV = m_Vertices.numElem();
        int nI = m_Indices.size();
        if (nV == 0 || nI == 0) return;
        bm.create( nV, nI, vfVertex2t );
        bm.setNVert( nV );
        bm.setNInd ( nI );
        bm.setNPri ( nI/3 );
        memcpy( bm.getIndices(), &m_Indices[0], nI*2 );
        Vertex2t* v = (Vertex2t*)bm.getVertexData();
        for (int i = 0; i < nV; i++)
        {
            Vector4D pos( m_Vertices.elem( i ) ); pos.w = 1.0f;
            v[i] = m_Vertices.elem( i );
            v[i].diffuse = 0xFFFFFFFF;
            pos *= m_UVTM;
            v[i].u = pos.x;
            v[i].v = pos.y;
        }
    }
}; // struct MapDirection

struct MappedModel
{
    std::vector<MapDirection>   m_Directions;
    MappedModel()
    {
    }

    bool LoadFromFile( const char* fname )
    {
        FILE* fp = fopen( fname, "rt" );
        if (!fp) return false;
        m_Directions.clear();
        int nV   = 0;
        int nTri = 0;
        int nCam = 0;
        char buf[1024];
        int  idx;
        fscanf( fp, "%s%s%d%s%s%d%s%s%d", 
            buf, buf, &nV, 
            buf, buf, &nTri,
            buf, buf, &nCam );

        for (int i = 0; i < nCam; i++)
        {
            char tex[1024];
            char cname[1024];
            Matrix4D m;
            fscanf( fp, "%s%d%s%s%d%s%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f", 
                buf, &idx, cname, buf, &idx, tex, 
                &m.e00, &m.e01, &m.e02, &m.e03,
                &m.e10, &m.e11, &m.e12, &m.e13,
                &m.e20, &m.e21, &m.e22, &m.e23,
                &m.e30, &m.e31, &m.e32, &m.e33 );

            m_Directions.push_back( MapDirection() );
            MapDirection& md = m_Directions.back();
            md.m_Texture        = tex;
            md.m_CameraName     = cname;
            md.m_UVTM           = m;
        }

        std::vector<Vector3D> vert;
        vert.reserve( nV );
        for (int i = 0; i < nV; i++) 
        {
            Vector3D v;
            fscanf( fp, "%f%f%f", &v.x, &v.y, &v.z );
            vert.push_back( v );
        }

        for (int i = 0; i < nTri; i++)
        {
            int n, a, b, c, meshIdx;
            fscanf( fp, "%d%d%d%d%d", &n, &a, &b, &c, &meshIdx );
            m_Directions[meshIdx].AddTriangle( vert[a], vert[b], vert[c] );
        }

        fclose( fp );
        return true;
    }
}; // struct MappedModel

/*****************************************************************************/
/*    Root implementation
/*****************************************************************************/
Root::Root() : SNode()
{
    AddRef();
    SetImmortal();
    SetName( "Root" );

    bool res = Vector3D( 4.0f, 0.0f, 0.0f ).isColinear( Vector3D::oX );
    res = Vector3D( 4.0f, 1.0f, 0.0f ).isColinear( Vector3D::oX );

    AABoundBox aabb( Vector3D::null, 100.0f );
    for (int i = 0; i < 1000; i++)
    {
        Plane pl;
        pl.normal() = Vector3D::GetRandomDir();
        pl.d = rndValuef( -100.0f, 100.0f );
        
        Triangle tri;
        Vector3D pt[2];
        tri.Random( aabb, 10.0f, 100.0f );
        int nClip = tri.Clip( pl, pt );
        
        bool res1 = pl.Contains( pt[0] );
        bool res2 = pl.Contains( pt[1] );

        float dt1 = pt[0].dot( pl.normal() ) + pl.d;
        float dt2 = pt[1].dot( pl.normal() ) + pl.d;

    }    
} // Root constructor

Root::~Root()
{
}

void LinkSG();
void LinkWidgets();
void LinkEffects();

void Root::CreateGuts()
{
    LinkSG();
    LinkEffects();
    LinkWidgets();

    AddChild( CreateServices()            );            
    AddChild( CreateGameSceneSetup()    );    
    AddChild( CreateEditorSceneSetup()    );    
    AddChild( CreateFrameContainer()    );    
    AddChild( CreateEditors()            );

#ifdef _TRACE
    Log.Info( "Root::CreateGuts" );
#endif // _TRACE

} // Root::CreateGuts

void Root::Render()
{

    if (GetNChildren() == 0) CreateGuts();
    IRS->DisableLights();
    //IRS->SetFog( NULL );
    IRS->ResetWorldTM();

    Animation::SetupTimeDelta();

    IRS->ClearDevice( 0xFF0000FF, true, true );
    SNode::Render();
} // Root::Render

Group* Root::CreateEditorSceneSetup()
{
    if (!IRS) return NULL;

    Group* pG = new Group();
    pG->SetName( "EditorSetup" );

    //  default material
    SurfaceMaterial* pMtl = new SurfaceMaterial();
    pMtl->SetDiffuse    ( 0xFFFFFFFF );
    pMtl->SetAmbient    ( 0xFFFFFFFF );
    pMtl->SetSpecular    ( 0xFFE5E5E5 );
    pMtl->SetShininess    ( 30.0f         );

    pMtl->SetName        ( "DefaultMaterial" );
    pG->AddChild        ( pMtl );

    //  default light
    LightSource* pLight = new DirectionalLight();
    pLight->SetName        ( "DefaultLight"    );
    pLight->SetDiffuse    ( 0xFFFFFFFF        );
    pLight->SetAmbient    ( 0xFF505050        );
    pLight->SetSpecular    ( 0xFFFFFFFF        );
    
    Vector3D ldir;
    ldir.set( -10.0f, -5.0f, -5.0f );
    ldir.normalize();

    pLight->SetDir        ( ldir );
    pLight->SetPos        ( Vector3D( -170.0f, 95.0f, 120.0f ) );
    pG->AddChild( pLight );

    //  additional light
    LightSource* pLight1 = new DirectionalLight();
    pLight1->SetName    ( "AddLight"        );
    pLight1->SetDiffuse    ( 0xFFEEEEFF        );
    pLight1->SetAmbient    ( 0xFFC0C0C0        );
    pLight1->SetSpecular( 0xFFFFFFFF        );
    pLight1->SetInvisible();

    ldir.set( 5.0f, 5.0f, -5.0f );
    ldir.normalize();
    
    pLight1->SetDir        ( ldir );
    pLight1->SetPos        ( Vector3D( -170.0f, 95.0f, 120.0f ) );
    pG->AddChild( pLight1 );

    //  default texture
    Texture* pTex0 = new Texture( "white.tga" );
    Texture* pTex1 = new Texture( "white.tga" );
    pTex1->SetStage( 1 );
    pG->AddChild( pTex0 );
    pG->AddChild( pTex1 );

    return pG;
} // Root::CreateEditorSceneSetup

Group* Root::CreateGameSceneSetup()
{
    if (!IRS) return NULL;

    Group* pG = new Group();
    pG->SetName( "GameSetup" );
    pG->SetInvisible();

    Background* svp = new Background();
    svp->SetExtents( 0.0f, 0.0f, 1024.0f, 768.0f );
    svp->SetBgColor( 0x00000000 );

    svp->SetName( "GameViewport" );
    //svp->SetViewportFlag( Background::vfClearColor );
    //svp->SetViewportFlag( Background::vfClearDepth );
    pG->AddChild( svp );
    
    //  default material
    SurfaceMaterial* pMtl = new SurfaceMaterial();
    pMtl->SetDiffuse    ( 0xFFFFFFFF );
    pMtl->SetAmbient    ( 0xFF454545 );
    pMtl->SetSpecular    ( 0xFFE5E5E5 );
    pMtl->SetShininess    ( 70.0f         );

    pMtl->SetName        ( "GameMaterial" );
    pG->AddChild        ( pMtl );

    //  default light
    LightSource* pLight = new DirectionalLight();
    pLight->SetName        ( "GameLight"    );
    pLight->SetDiffuse    ( 0xFFFFFFFF    );
    pLight->SetAmbient    ( 0xFFFFFFFF    );
    pLight->SetSpecular    ( 0xFFFFFFFF    );

    Vector3D ldir;
    ldir.set( -10.0f, -5.0f, -5.0f );
    ldir.normalize();

    pLight->SetDir        ( ldir );
    pLight->SetPos        ( Vector3D( -170.0f, 95.0f, 120.0f ) );
    pG->AddChild( pLight );

    return pG;
} // Root::CreateGameSceneSetup

Group* Root::CreateFrameContainer()    
{
    Group* pG = new Group();
    pG->SetName( "Frame" );
    
    SpriteManager* pSpriteManager = (SpriteManager*)GetSpriteManager();
    pG->AddChild( pSpriteManager );
    if (pSpriteManager) pSpriteManager->Init();

    
    ReflectionMap* pMap = pG->AddChild<ReflectionMap>( "ReflectionMap" );
    pMap->Init();
    IRMap = pMap;
    
    PEffectManager* pEffMgr = (PEffectManager*)IEffMgr; 
    pG->AddChild( pEffMgr );
    

    SNode* pPhys = (SNode*)ObjectFactory::instance().Create( "Physics" );
    if (pPhys)
    {
        pPhys->SetName( "Physics" );
        pG->AddChild( pPhys );
    }
    return pG;
} // Root::CreateFrameContainer

Group* Root::CreateServices()
{
    Group* pG = new Group();
    pG->SetInvisible();
    pG->SetName( "Services" );

    pG->AddChild( TextureManager::instance() );
    
    return pG;
} // Root::CreateServices

Group* Root::CreateEditors()
{
    Group* pG = new Group();
    pG->SetName( "Editors" );

    Background* pVP = new Background();
    pVP->SetExtents( 0.0f, 0.0f, 1024.0f, 768.0f );
    pVP->SetBgColor( 0x00000000 );

    pVP->SetName( "EditorsViewPort" );
    pG->AddChild( pVP );

    return pG;
} //  Root::CreateEditors

