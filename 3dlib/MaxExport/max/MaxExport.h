/*****************************************************************************/
/*    File:    MaxExport.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    07.11.2002
/*****************************************************************************/
#ifndef __MAXEXPORT__H
#define __MAXEXPORT__H

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "phyexp.h"
#include "iskin.h"

extern TCHAR *GetString(int id);

#define C2MEXPORTER_CLASS_ID    Class_ID(0x2a71a045, 0x340d6436)
const int c_ProgressRange        = 100;
const int c_MaxNodeSubMtls        = 128;
const int c_MaxBonesPerMesh        = 512;
const int c_MaxSkinMeshes        = 64;

/*****************************************************************************/
/*    Class:    HashedNode
/*    Desc:    hashed INode pointer 
/*****************************************************************************/
class HashedNode
{
public:
    INode*        maxNode;
    SNode*    pNode;

    HashedNode() : maxNode(0), pNode(0) {}
    HashedNode( INode* _maxNode, SNode* _pNode ) : maxNode(_maxNode), pNode(_pNode) {}
    
    unsigned int hash() const
    {
        unsigned char* s = (unsigned char*) this;
        unsigned int h = 0;
        int nBytes = sizeof( maxNode );
        for (int i = 0; i < nBytes; i++)
        {
            h = (h * 729 + (*s) * 37) ^ (h >> 1);
            s++;
        }
        return h;
    } // hash

    bool equal( const HashedNode& el )
    {
        return maxNode == el.maxNode;
    } // equal

    void copy( const HashedNode& el )
    {
        maxNode = el.maxNode;
        pNode    = el.pNode;
    } // copy
}; // class HashedNode

struct SkinInfo
{
    Skin*                       pGeom;
    INode*                      bones[c_MaxBonesPerMesh];
    Matrix4D                    offs[c_MaxBonesPerMesh];

    int                            nBones;

    SkinInfo() : nBones(0), pGeom(NULL) {}

    int        AddBone( INode* pNode )
    {
        if (!pNode) return nBones - 1;
        for (int i = 0; i < nBones; i++)
        {
            if (bones[i] == pNode) return i;
        }
        assert( nBones < c_MaxBonesPerMesh - 1);
        bones[nBones++] = pNode;
        const char* name = pNode->GetName();
        return nBones - 1;
    }

    bool    IsBonePresent( INode* pNode )
    {
        for (int i = 0; i < nBones; i++)
        {
            if (bones[i] == pNode) return true;
        }
        return false;
    }    

    void    SetBoneOffset( int idx, const Matrix4D& m )
    {
        assert( idx >= 0 && idx < nBones );
        offs[idx] = m;
    }

    const Matrix4D& GetBoneOffset( int idx )
    {
        assert( idx >= 0 && idx < nBones );
        return offs[idx];
    }

    bool operator==( const SkinInfo& info ) const
    {
        return memcmp( (void*)this, (void*)&info, sizeof( SkinInfo ) ) == 0;
    }

}; // struct SkinInfo

/*****************************************************************************/
/*    Class:    maxExporter
/*    Desc:    Main exporter class    o
/*****************************************************************************/
class maxExporter : public SceneExport, public c2mExporter 
{
public:
    static HWND hParams;

    int                ExtCount();                    // Number of extensions supported
    const TCHAR *    Ext(int n);                    // Extension #n (i.e. "3DS")
    const TCHAR *    LongDesc();                    // Long ASCII description (i.e. "Autodesk 3D Studio File")
    const TCHAR *    ShortDesc();                // Short ASCII description (i.e. "3D Studio")
    const TCHAR *    AuthorName();                // ASCII Author name
    const TCHAR *    CopyrightMessage();            // ASCII Copyright message
    const TCHAR *    OtherMessage1();            // Other message #1
    const TCHAR *    OtherMessage2();            // Other message #2
    unsigned int    Version();                    // Version number * 100 (i.e. v3.01 = 301)
    void            ShowAbout(HWND hWnd);        // Show DLL's "About..." box

    BOOL            SupportsOptions(int ext, DWORD options);
    int                DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

    maxExporter();
    ~maxExporter();        

    void            StartExporting();

protected:
    
    Interface*                    iMax;
    INode*                        curNode;
    ExpInterface*                iExport;
    TimeValue                    curTime;
    Matrix4D                    flipTM;
    Matrix4D                    camTweakTM;
    Matrix4D                    localNodeTweakTM;

    int                            subMtl[c_MaxNodeSubMtls];
    int                            nSubMtls;

    TransformNode*                ProcessNode            ( INode* node, SNode* pParent );
    TransformNode*                ProcessNodeSubTree    ( INode* node, SNode* pParent );
    
    bool                        ProcessNodeShaders    ( INode* node );
    GShader                        ProcessStdMtl        ( Mtl* mtl, bool bAssignDss = true, int startStage = 1 );
    void                        ProcessShellMtl        ( Mtl* mtl );

    bool                        ExtractDiffuseMap    ( Mtl* mtl, GShader& sh, int stage = 0 );
    bool                        ExtractBumpMap        ( Mtl* mtl, GShader& sh, int stage = 1 );
    bool                        ExtractEnvMap        ( Mtl* mtl, GShader& sh, int stage = 1 );

    bool                        ExtractPositionController( INode* node, PRSAnimation* pAnm );
    bool                        ExtractRotationController( INode* node, PRSAnimation* pAnm );
    bool                        ExtractScalingController( INode* node, PRSAnimation* pAnm );
    bool                        ExtractBipedController    ( INode* node, PRSAnimation* pAnm );

    bool                        ExtractFloatTrack( Control* pC, FloatCurve& curve );
    
    //  skinning
    bool                        FetchPhysiqueVertex        ( GVertex& vert, int vertIdx );
    bool                        FetchSkinVertex         ( GVertex& vert, int vertIdx );

    bool                        InitNodeSkinning    ( INode* node );
    Modifier*                    FindModifier( INode* node, Class_ID modID );

    TransformNode*              GetCameraNode    ( INode* node );
    TransformNode*              GetLocatorNode    ( INode* node );
    TransformNode*              GetLightNode    ( INode* node );
    TransformNode*              GetBoneNode        ( INode* node );
    TransformNode*              GetMeshNode        ( INode* node );
    TransformNode*              GetTransformNode( INode* node );
    SNode*                      GetPRSAnimation ( INode* node );
    UVAnimation*                GetUVAnimation    ( Texmap* pTexMap );

    void                        PostprocessSkinning();
    Matrix4D                    CalcBoneOffset( INode* boneNode );
    
    Matrix4D                    GetNodeTransform( INode* node, TimeValue* pTime = NULL );

    bool                        ShouldSkipNode( INode* node );

    IPhyContextExport*            m_pPhysique;
    
    ISkin*                      m_pSkin;
    ISkinContextData*           m_pSkinData;

    bool                        bHaveWeights;

    Object*                        curObject;

    void                        PreprocessScene    ();
    void                        PreTraverseSubTree( INode* node );
    
    virtual void                ProcessAnimation();

    Hash<HashedNode>            nodeHash;
    
    LinearMap<SkinInfo, c_MaxSkinMeshes> skins;
    SkinInfo*                    curSkin;
    SkinInfo*                    NewSkin( SkinInfo* pOrigSkin = NULL );

}; // class maxExporter

/*****************************************************************************/
/*    Class:    maxExporterClassDesc
/*    Desc:    3ds Max exporter class glue
/*****************************************************************************/
class maxExporterClassDesc : public ClassDesc2 
{
public:
    int             IsPublic() { return TRUE; }
    void *            Create(BOOL loading = FALSE) { return new maxExporter(); }
    const TCHAR *    ClassName() { return GetString(IDS_CLASS_NAME); }
    SClass_ID        SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
    Class_ID        ClassID() { return C2MEXPORTER_CLASS_ID; }
    const TCHAR*     Category() { return GetString(IDS_CATEGORY); }
    virtual TCHAR* GetRsrcString(INT_PTR id) { return _T("c2mExporter"); }


    const TCHAR*    InternalName() { return _T("c2mExporter"); }    // returns fixed parsable name (scripter-visible name)
    HINSTANCE        HInstance() { return c2mExporter::hInstance; }    // returns owning module handle
}; // class maxExporterClassDesc 

#endif // __MAXEXPORT__H
