/*****************************************************************************/
/*    File:    c2mExporter.h
/*    Desc:    CossacksII generic model export stuff
/*    Author:    Ruslan Shestopalyuk
/*    Date:    07.11.2002
/*****************************************************************************/
#ifndef __C2MEXPORTER_H__
#define __C2MEXPORTER_H__

#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include "windows.h"
#include "assert.h"
#include "kHash.hpp"


#include "gmDefines.h"
#include "mMath3D.h"

const int c_MaxNameLen            = 128;
const int c_MaxIndent            = 24;
const int c_MaxSceneTextures    = 256;
const int c_MaxSceneMaterials    = 256;
const int c_MaxBonesPerVertex    = 4;

typedef char PATH[_MAX_PATH];

enum GSortAttribute
{
    gsaUnknown        = 0,
    gsaObject        = 1,
    gsaMaterial        = 2,
    gsaIdxProximity = 3
}; // enum GVAttribute

/*****************************************************************************/
/*    Class:    GVertex
/*    Desc:    Generic vertex description
/*****************************************************************************/
class GVertex
{
public:
    GVertex() : x(0.0f), y(0.0f), z(0.0f),
                nx(0.0f), ny(0.0f), nz(0.0f),
                u(0.0f), v(0.0f), color(0xFFFFFFFF),
                matID(0xFFFF), smGroup(0xFFFFFFFF), objID(0xFFFF), nWeights(0) 
                {
                    memset( weight, 0, c_MaxBonesPerVertex * sizeof( weight[0] ) ); 
                    memset( boneRef, 0, c_MaxBonesPerVertex * sizeof( boneRef[0] ) ); 
                }

    //  hash function ;)
    unsigned int hash() const
    {
        unsigned char* s = (unsigned char*) this;
        unsigned int h = 0;
        int nBytes = sizeof( GVertex ) - sizeof( smGroup );
        for (int i = 0; i < nBytes; i++)
        {
            h = (h * 729 + (*s) * 37) ^ (h >> 1);
            s++;
        }
        return h;
    } // hash

    bool equal( const GVertex& el )
    {
        return (memcmp( this, &el, sizeof( GVertex ) - sizeof( smGroup ) ) == 0)
                && (
                    ((smGroup & el.smGroup) != 0) /*|| 
                    (smGroup == el.smGroup)*/
                );
    } // equal

    void copy( const GVertex& el )
    {
        memcpy( this, &el, sizeof( GVertex ) );
    } // copy

    void* GetVertex( VertexFormat vf );

    DWORD PackBoneRef( int nBones );
    bool  AddWeight( int idx, float w );
    void  LeaveBiggerWeight( int idx, float w );

    float    x, y, z;        //  position
    float    nx, ny, nz;        //  normal
    float    u, v;            //  tex coords
    float    u2, v2;            //  second tex coords
    WORD    matID;            //  material ID

    DWORD    color;            //  vertex color
    WORD    objID;            //  object ID
    float    weight[c_MaxBonesPerVertex];    //  skinning weight
    WORD    boneRef[c_MaxBonesPerVertex];    //  influence bone index
    BYTE    nWeights;        //  number of skinning weights
    
    DWORD    smGroup;        //  smoothing group

    static int __cdecl cmpObject( const void *e1, const void *e2 )
    {
        return ((GVertex*)e1)->objID - ((GVertex*)e2)->objID;
    }

    static int __cdecl cmpMaterial( const void *e1, const void *e2 )
    {
        return ((GVertex*)e1)->matID - ((GVertex*)e2)->matID;
    }

    void dump( FILE* fp )
    {
        fprintf( fp, "matID: %d; objID: %d; smGroup: %d; nWeights: %d; ", 
                    matID, objID, smGroup, nWeights );
        if (nWeights > 0)
        {
            for (int i = 0; i < nWeights; i++)
            {
                fprintf( fp, "w%d=%1.4f; b%d=%d;  ", i, weight[i], i, boneRef[i] );
            }
        }
        fprintf( fp, "\n" );
    }
}; // class GVertex

class GShader
{
    int                            id;
    std::vector<SNode*>        attr;

public:
    int        GetNAttributes() const 
    {
        return attr.size();
    }

    SNode*    GetAttribute( int idx )
    {
        if (idx < 0 || idx >= attr.size()) return NULL;
        return attr[idx];
    }

    void AddAttribute( SNode* pAttr )
    {
        attr.push_back( pAttr );
    }

    void AddAttribute( int pos, SNode* pAttr )
    {
        attr.insert( attr.begin() + pos, pAttr );
    }

    void ClearAttributes()
    {
        attr.clear();
    }

    int GetID() const { return id;}
    void SetID( int _id ) { id = _id;}

    bool equal( const GShader& sh )
    {
        if (attr.size() != sh.attr.size())
        {
            return false;
        }

        for (int i = 0; i < attr.size(); i++)
        {
            if (attr[i] != sh.attr[i]) return false;
        }
        return true;
    }
}; // class GShader

const int c_MaxTextures = 8;

enum MtlFlag
{
    mfBumpMap        = 1,
    mfEnvMap        = 2,
    mfAnimated        = 3
}; // enum MtlFlags

typedef Hash<GVertex> VertHash;

struct Poly
{
    int            v1;
    int            v2;
    int            v3;
}; // struct Poly

enum DialogResult
{
    drUnknown        = 0,
    drYes            = 1,
    drNo            = 2,
    drOK            = 3,
    drCancel        = 4
}; // enum DialogResult

const int c_MinPolyNum = 65536;

#define NO_MATERIAL 0xFFFF

/*****************************************************************************/
/*    Class:    c2mExporter
/*    Desc:    Generic export class, contains stuff independent of 
/*                Max/Maya/Whatever specifics
/*****************************************************************************/
class c2mExporter
{
public:
    c2mExporter();

    virtual void            StartExporting() = 0;
    
    void                    ShowExportDialog();
    void                    Msg( const char* format, ... );
    void                    SetProgress( double progrVal );
    void                    StepProgressNode();

    void                    MsgIndent();
    void                    MsgUnindent();

    void                    ClearTextureTable();


    Texture*            GetTexNode( const char* texFile, int stage );
    void                    SetPluginFileName( const char* fileName );

    void                    InitInternal();
    void                    ShutInternal();

    bool                    WriteModel( SNode* pModel );

    //  model-related
    void                    CleanModel();
    int                        AddPolygon( int v1, int v2, int v3 );
    void                    ResetGeometryStorage();

    TransformNode*        CreateMeshNode();

    void                    UpOneDirTreeLevel( char* dir );
    static char*            BrowseForFolder();
    
    static HINSTANCE        hInstance;
    VertHash                vertHash;

protected:
    //  options of export
    int        ___beginPersistent;
    bool    bStaticVertexColors;
    bool    bCopyTextures;        
    bool    bKeyframe;            
    bool    bExportAnimation;    
    bool    bAnimationOnly;
    bool    bExportSkeletal;    
    bool    bOverwriteTextures;        
    bool    bExportNormals;        
    bool    bExportColors;        
    bool    bIgnoreSmoothing;    
    bool    bNoTex;
    bool    bSuppressLog;
    bool    bReduceKeys;
    int        nKPS;
    int        nWeightsPerBone;
    char    texPath            [_MAX_PATH];
    int        ___endPersistent;


    char    modelFileName    [_MAX_PATH];
    char    modelName        [_MAX_PATH];
    char    pluginFileName    [_MAX_PATH];
    
    
    char    curNodeName        [c_MaxNameLen];
    
    double    progress;
    double    progressPerNode;
    int        nTransformNodes;

    std::vector<GVertex>                curNodeVertices;
    std::vector<Poly>                    curNodePolys;

    int        nIndent;
    double    radius2;

    //  generic window-related 
    bool    IsChecked    ( HWND hWnd );
    void    SetCheck    ( HWND hWnd, bool checked );
    bool    GetInt        ( HWND hWnd, int& val );
    void    EnableAnimGroup( bool enable ); 

    DialogResult AskYesNo( const char* msg );


    bool    checkPolySanity( const Poly& p );

    static  c2mExporter*    me;
    static bool                exportDone;
    
    bool                    haveBlendedVertices;
    bool                    abortProcess;

    Poly*                    poly;
    int                        nPoly;
    int                        maxPoly;
    int                        nPolyTotal;
    int                        nVertTotal;

    SNode*                m_pModel;
    SNode*                m_pCurSNode;
    AnimationBlock*        m_pAnimGroup;
    Animation*            m_pParentAnim;

    //  keyframe reducing biases
    float                    m_ScaleTolerance;
    float                    m_RotTolerance;
    float                    m_PosTolerance;

    //  keyframe sampling interval
    float                    m_SamplingInterval;

    //  current animation time
    float                    m_CurTime;

    void                    SortPoly( int first, int num, GSortAttribute attr );
    SurfaceMaterial*                    CreateDefaultMaterial();

    int                        GetPolyClusterSize( int firstPoly, GSortAttribute attr );

    void                    DumpPolys( FILE* fp );
    void                    OnOpenExportDlg();

    void                    CreateTextureDir    ();
    void                    CopySceneTextures    ();

    VertexFormat            GetCurVertexFormat();

    int                        GetTotalPolyNum() const;
    int                        GetTotalVertNum() const;
    int                        GetTotalTexNum() const;

    GShader*                GetNodeShader( int idx );
    GShader*                GetNodeShaderByID( int id );

    int                        GetNodeShaderID( int mtlID );
    int                        GetNNodeShaders() const;
    int                        GetNNodeShadersReal() const;
    void                    ClearNodeShaders(); 
    int                        AddNodeShader( GShader& sh ); 


    Texture*                AddTextureNode( Texture* SNode );
    Shader*                    AddDssNode( Shader* SNode );
    SurfaceMaterial*                    AddMtlNode( SurfaceMaterial* SNode );

    void                    PostprocessScene();
    virtual void            ProcessAnimation(){}


private:

    static HWND        hPanelWnd;
    static HWND        hLogWnd;    
    static HWND        hProgressWnd;
    static HWND        hGoBtn;
    static HWND        hTexPathWnd;
    static HWND        hChPathWnd;
    static HWND        hCopyTexWnd;
    static HWND        hWeightsPerBoneWnd;
    
    static HWND        hAnimOnly;
    static HWND        hExportAnimWnd;
    static HWND        hCopyTCap;
    
    static HWND        hAnGroup;
    static HWND        hNormWnd;
    static HWND        hColWnd;
    static HWND        hIgnoreSm;
    static HWND        hSkinning;
    static HWND        h2ndTexPair;
    static HWND        hNoTex;
    static HWND        hSuppressLog;
    static HWND        hReduceKeys;
    
    static HWND        hOverwriteTextures;

    static BOOL CALLBACK OptionsDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
    static void        EnableControls();
    static void        DisableControls();

    void            Dialog2Options();
    void            Options2Dialog();


    std::vector<GShader>                nodeShaders;
    int                                 nNodeShadersReal;

    std::vector<Texture*>            texTable;
    std::vector<Shader*>            dssTable;
    std::vector<SurfaceMaterial*>                mtlTable;

    static char            folderPath[_MAX_PATH];

}; // class c2mExporter

IReflected* CreateOrthoCamera( float vVol, float aspect, float zn, float zf );
IReflected* CreatePerspCamera( float fovx, float aspect, float zn, float zf );

#endif // __C2MEXPORTER_H__