/*****************************************************************
/*  File:   IRenderSystem.h                                      *
/*  Desc:   3D rendering device abstract interface               *
/*  Date:   Feb 2002                                             *
/*****************************************************************/
#ifndef __RENDERSYSTEM_H__ 
#define __RENDERSYSTEM_H__
#pragma    once

#include "ITexture.h"
#include "IIndexBuffer.h"
#include "IVertexBuffer.h"
#include "IShader.h"

#include "windows.h"
#include "gmDefines.h"
#include "kColorValue.h"

const int  c_MaxTextureStages    = 8;

class Rct;
class Matrix4D;
class Frustum;
class Plane;
class Sphere;
class Cylinder;
class Capsule;
class AABoundBox;
class BaseMesh;

/*****************************************************************************/
/*    Enum:    VertexFormat
/*    Desc:    Pre-defined vertex format types
/*****************************************************************************/
enum VertexFormat
{
    vfUnknown        = 0,
    vfVertexTnL      = 1,
    vfVertex2t       = 2,
    vfVertexN        = 3,
    vfVertexTnL2     = 4,
    vfVertexT        = 5,
    vfVertexMP1      = 6,
    vfVertexW1       = 7,
    vfVertexTnL2S    = 8,
    vfVertexW2       = 9,
    vfVertexW3       = 10,
    vfVertexW4       = 11,
    vfVertexN2T      = 12,
    vfVertexXYZD     = 13,
    vfVertexXYZW     = 14,
    vfVertexTS       = 15,

    vfVertex1W       = 16,
    vfVertex2W       = 17,
    vfVertex3W       = 18,
    vfVertex4W       = 19,

    vfVertexBump     = 20,
    vfVertex2F       = 21,
	vfVertexUV3C	 = 22,
	vfVertex5C		 = 23,

    vfLAST           = 24
};  // enum VertexFormat

/*****************************************************************************/
/*    Enum:    BufferFormat
/*    Desc:    Type of used resource buffer    
/*****************************************************************************/
enum BufferFormat
{
    bfUnknown        = 0,
    bfStatic        = 1,
    bfDynamic        = 2
};    //  enum BufferFormat

const int c_NumVertexTypes = (int)vfLAST;

class Matrix4D;
class Vector4D;
class Matrix3D;
class Vector3D;
class BaseMesh;
class DirectionalLight;
class PointLight;
class SpotLight;

class SurfaceMaterial;
class Fog;

class RenderStateBlock;
class TextureStateBlock;
class StateBlock;

/*****************************************************************************/
/*    Enum:    MemoryPool
/*    Desc:    Memory location of resource
/*****************************************************************************/
enum MemoryPool
{
    mpUnknown        = 0,
    mpSysMem         = 1,
    mpVRAM           = 2,
    mpManaged        = 3
};  // enum MemoryPool

/*****************************************************************************/
/*    Enum:    PrimitiveType
/*    Desc:    Types of the drawn primitives
/*****************************************************************************/
enum PrimitiveType
{
    ptUnknown       = 0,
    ptPointList     = 1,
    ptLineList      = 2,
    ptLineStrip     = 3,
    ptTriangleList  = 4,
    ptTriangleStrip = 5,
    ptTriangleFan   = 6,
    ptQuadList      = 7
};  // enum PrimitiveType

inline int GetNumPrimitives( PrimitiveType pType, int indNum )
{
    switch (pType)
    {
    case ptUnknown:             return 0;
    case ptTriangleList:        return indNum / 3;     
    case ptTriangleStrip:       return indNum - 2;
    case ptLineList:            return indNum >> 1;
    case ptLineStrip:           return indNum - 1;
    case ptPointList:           return indNum; 
    case ptTriangleFan:         return indNum - 2;
    case ptQuadList:            return indNum/6;
    }    
    return 0;
} // GetNumPrimitives

inline int GetNumIndices( PrimitiveType pType, int priNum )
{
    switch (pType)
    {
    case ptUnknown:         return 0;
    case ptTriangleList:    return priNum * 3;     
    case ptTriangleStrip:   return priNum + 2;
    case ptLineList:        return priNum << 1;
    case ptLineStrip:       return priNum + 1;
    case ptPointList:       return priNum; 
    case ptTriangleFan:     return priNum + 2;
    case ptQuadList:        return priNum*6;
    }    
    return 0;
} // GetNumIndices

/*****************************************************************************/
/*    Enum: ScreenResolution
/*****************************************************************************/
enum ScreenResolution
{
    srUnknown            = 0,
    sr640x480            = 1,
    sr800x600            = 2,
    sr1024x768           = 3,
    sr1280x1024          = 4,
    sr1600x1200          = 5
}; // ScreenResolution

/*****************************************************************************/
/*    Enum: ScreenBitDepth
/*****************************************************************************/
enum ScreenBitDepth
{
    bdUnknown           = 0,
    bd16                = 1,
    bd32                = 2
}; // enum ScreenBitDepth

/*****************************************************************************/
/*    Class:    ScreenProp
/*    Desc:    Describes screen properties
/*****************************************************************************/
class ScreenProp
{
public:
    int                 m_Width;
    int                 m_Height;

    ColorFormat         m_ColorFormat;
    bool                m_bFullScreen;
    bool                m_bCoverDesktop;
    int                 m_RefreshRate;

    ScreenProp()
    {
        m_Width         = 1024;
        m_Height        = 768;
        m_ColorFormat   = cfRGB565;
        m_bFullScreen   = false;
        m_bCoverDesktop = false;
        m_RefreshRate   = 75;
    } 

    bool equal( const ScreenProp& prop ) const
    {
        return      (m_Width        == prop.m_Width         ) &&
                    (m_Height       == prop.m_Height        ) &&
                    (m_ColorFormat  == prop.m_ColorFormat   ) &&
                    (m_bFullScreen  == prop.m_bFullScreen   ) &&
                    (m_bCoverDesktop== prop.m_bCoverDesktop );
    }

}; // class ScreenProp

/*****************************************************************************/
/*  Class:  RenderTask
/*  Desc:   Atomic polygons rendering task
/*****************************************************************************/
class RenderTask
{
public:
    int                     m_Pass;         //  index of the render pass
    int                     m_ShaderID;     //  index of the used shader instance
    int                     m_TexID[c_MaxTextureStages];

    int                     m_VBufID;       //  hardware vertex buffer ID
    int                     m_VType;        //  geometry vertex type ID
    int                     m_FirstVert;    //  index of the first vertex at the vertex buffer
    int                     m_NVert;        //  number of the vertices in the geometry
    DWORD                   m_VBufStamp;    //  hardware vertex buffer caching watermark

    DWORD                   m_TFactor;      //  texture factor

    int                     m_IBufID;       //  hardware index buffer ID  
    int                     m_FirstIdx;     //  index of the first index at the index buffer
    int                     m_NIdx;         //  number of the indices in the geometry
    DWORD                   m_IBufStamp;    //  hardware index buffer caching watermark

    PrimitiveType           m_PriType;      //  type of the rendered primitives

    bool                    m_bHasTM;       //  whether geometry should be transformed before rendering
    bool                    m_bTransparent; //  whether geometry has transparent texture parts
	bool					m_UseMeshConst;
	Vector4D				m_MeshConst;
	int						m_ConstIdx;
    Matrix4D                m_TM;           //  world transform to apply before rendering
    const char*             m_Source;       //  geometry string ID - for debug purposes   

    static const RenderTask  c_Invalid;

    RenderTask() :          m_Pass           ( 0  ),
        m_ShaderID       ( -1 ),
        m_VBufID         ( -1 ),
        m_FirstVert      ( -1 ),
        m_NVert          ( -1 ),
        m_IBufID         ( -1 ),
        m_FirstIdx       ( -1 ),
        m_NIdx           ( -1 ),
        m_VType          ( -1 ),
        m_bHasTM         ( false ),
        m_bTransparent   ( false ),
		m_UseMeshConst	 ( false ),
        m_PriType        ( ptTriangleList ),
        m_IBufStamp      ( 0 ),
        m_VBufStamp      ( 0 ),
        m_TFactor        ( 0 ),
        m_Source         ( "" )
    {
        m_TexID[0] = m_TexID[1] = m_TexID[2] = m_TexID[3] = 
            m_TexID[4] = m_TexID[5] = m_TexID[6] = m_TexID[7] = -1; 
    }

    bool operator <( const RenderTask& rt ) const
    {
        if (m_bTransparent && !rt.m_bTransparent) return true;
        if (m_Pass      < rt.m_Pass)        return true;
        //if (m_TFactor   < rt.m_TFactor )    return true;
        if (m_ShaderID  < rt.m_ShaderID)    return true;
        if (m_TexID[0]  < rt.m_TexID[0])    return true;
        //if (m_TexID[1]  < rt.m_TexID[1])    return true;
        //if (m_TexID[2]  < rt.m_TexID[2])    return true;
        //if (m_TexID[3]  < rt.m_TexID[3])    return true;
        //if (m_TexID[4]  < rt.m_TexID[4])    return true;
        //if (m_TexID[5]  < rt.m_TexID[5])    return true;
        //if (m_TexID[6]  < rt.m_TexID[6])    return true;
        //if (m_TexID[7]  < rt.m_TexID[7])    return true;
        //if (m_VBufID    < rt.m_VBufID)      return true;
        if (m_VType     < rt.m_VType)       return true;
        //if (m_IBufID    < rt.m_IBufID)      return true;
        if (m_bHasTM && !rt.m_bHasTM)       return true;

        return false;
    } // operator <

    bool operator ==( const RenderTask& rt ) const
    {
        if (m_bTransparent  != rt.m_bTransparent) return false;
        if (m_Pass          != rt.m_Pass)        return false;
        if (m_TFactor       != rt.m_TFactor)     return false;
        if (m_ShaderID      != rt.m_ShaderID)    return false;
        if (m_TexID[0]      != rt.m_TexID[0])    return false;
        if (m_TexID[1]      != rt.m_TexID[1])    return false;
        if (m_TexID[2]      != rt.m_TexID[2])    return false;
        if (m_TexID[3]      != rt.m_TexID[3])    return false;
        if (m_TexID[4]      != rt.m_TexID[4])    return false;
        if (m_TexID[5]      != rt.m_TexID[5])    return false;
        if (m_TexID[6]      != rt.m_TexID[6])    return false;
        if (m_TexID[7]      != rt.m_TexID[7])    return false;
        if (m_VBufID        != rt.m_VBufID)      return false;
        if (m_VType         != rt.m_VType)       return false;
        if (m_IBufID        != rt.m_IBufID)      return false;
        if (m_FirstIdx      != rt.m_FirstIdx)    return false;
        if (m_FirstVert     != rt.m_FirstVert)   return false;
        if (m_bHasTM        != rt.m_bHasTM)      return false;
        return true;
    } // operator ==

}; // struct RenderTask

class IDeviceClient;
/*************************************************************************************/
/*  Class:  IRenderSystem, abstract interface                                  
/*  Desc:   Incapsulates all rendering commands    passed to hardware API                 
/*************************************************************************************/
class  IRenderSystem  
{
public:

    //  general
    virtual void                Init                ( HINSTANCE hInst, HWND hWnd = NULL ) = 0;
    virtual void                ShutDown            () = 0;
    virtual bool                SetScreenProp       ( const ScreenProp& prop ) = 0;
    virtual ScreenProp          GetScreenProp       () = 0;
    virtual int                 GetNDisplayModes    () = 0;
    virtual void                GetDisplayMode      ( int idx, int& width, int& height ) = 0;
    virtual void                GetClientSize       ( int& width, int& height ) = 0;
    virtual void                ClearDevice         ( DWORD color = 0, bool bColor = true, bool bDepth = false, bool bStencil = false )  = 0;
    virtual DWORD               GetCurFrame         () const = 0;
    virtual void                AddClient           ( IDeviceClient* iNotify ) = 0;
    virtual float               GetFPS              () const = 0;
	virtual void				EnablePostEffects	(bool Enable,float AutoBrightDegree,float AutoBrightSpeed,float ,float MotionBlurAlpha) = 0;
	virtual void				PreparePosteffects	() = 0;
	virtual void				RenderPosteffects	() = 0;


    //  transforms
    virtual void                SetViewTM           ( const Matrix4D& vmatr ) = 0;
    virtual void                SetProjTM           ( const Matrix4D& pmatr ) = 0;
    virtual void                SetWorldTM          ( const Matrix4D& wmatr ) = 0;
	virtual void                SetWorldViewProjTM  ( const Matrix4D& wmatr ) = 0;
    virtual void                ResetWorldTM        () = 0;
    virtual void                SetTextureTM        ( const Matrix4D& tmatr, int stage = 0 ) = 0;
    virtual void                SetBumpTM           ( const Matrix3D& bmatr, int stage = 0 ) = 0;
    virtual const Matrix4D&     GetViewTM           () const = 0;
    virtual const Matrix4D&     GetProjTM           () const = 0;
    virtual const Matrix4D&     GetWorldTM          () const = 0;
	virtual const Matrix4D&     GetWorldViewProjTM  () const = 0;
    virtual const Matrix4D&     GetTextureTM        ( int stage = 0 ) const = 0;
    virtual const Matrix3D&     GetBumpTM           ( int stage = 0 ) const = 0;

    //  cursor
    virtual bool                SetCursor           ( int texID, const Rct& rctOnTex, int hotspotX = 0, int hotspotY = 0 ) = 0;
    virtual bool                UpdateCursor        ( int x, int y, bool drawNow = false ) = 0;
    virtual void                ShowCursor          ( bool bShow = true ) = 0;
    
    //  vertex buffers
    virtual int                 RegisterVType       ( const VertexDeclaration& vdecl ) = 0;
    virtual BYTE*               LockVB              ( int vbID, int firstV, int numV, DWORD& stamp ) = 0;
    virtual BYTE*               LockAppendVB        ( int vbID, int size, int& offset, DWORD& stamp ) = 0;
    virtual void                UnlockVB            ( int vbID ) = 0;
    virtual bool                IsVBStampValid      ( int vbID, DWORD stamp ) = 0; 
    virtual int                 GetVBufferID        ( const char* vbName ) = 0;
    virtual bool                DiscardVB           ( int vbID ) = 0;
    virtual int                 CreateVB            ( const char* name, int size, int vType, bool bDynamic = false ) = 0;
    virtual bool                DeleteVB            ( int vbID ) = 0;
    virtual bool                SetVB               ( int vbID, int vType = -1, int stream = 0, int frequency = 1 ) = 0;

    //  index buffers
    virtual BYTE*               LockIB              ( int ibID, int firstIdx, int numIdx, DWORD& stamp ) = 0;
    virtual BYTE*               LockAppendIB        ( int ibID, int size, int& offset, DWORD& stamp ) = 0;
    virtual bool                IsIBStampValid      ( int ibID, DWORD stamp ) = 0; 
    virtual void                UnlockIB            ( int ibID ) = 0;
    virtual int                 GetIBufferID        ( const char* ibName ) = 0;
    virtual bool                DiscardIB           ( int ibID ) = 0;
    virtual int                 CreateIB            ( const char* name, int size, IndexSize idxSize, bool bDynamic = false ) = 0;
    virtual bool                DeleteIB            ( int ibID ) = 0;
    virtual bool                SetIB               ( int ibID ) = 0;

    //  drawing
    virtual bool                StartFrame          () = 0;
    virtual void                EndFrame            () = 0;
    virtual void                Draw                ( int firstVert, int nVert, int firstIdx, int nIdx, 
                                                        PrimitiveType priType = ptTriangleList ) = 0;

    //  textures
    virtual bool                ReloadTextures      () = 0;
    virtual void                SetTextureOverride  ( int TextureID,int Stage ) = 0;
    virtual int                 GetTextureOverride  ( int Stage ) = 0;
    virtual void                SetTransparentTexOverride( int Stage ) = 0;
    virtual void                SetTexture          ( int texID, int stage = 0, bool bCache = false ) = 0;
    virtual int                 GetTexture          ( int stage = 0 ) = 0;
    virtual const char*         GetTexturePath      ( int texID ) const = 0;
    virtual int                 GetShader           () = 0;
    virtual bool                SaveTexture         ( int texID, const char* fname ) = 0;
    virtual void                CopyTexture         ( int destID, int srcID, const Rct* rct = NULL, int nRect = 1 ) = 0;
    virtual void                CreateMipLevels     ( int texID ) = 0;
    virtual int                 GetTextureID        ( const char* texName ) = 0;
	virtual bool				CopyRenderTarget	(int RenderTargetID,int SysMemTexID) = 0;

	typedef void				OnGetTextureID(const char *TexFileName);
	virtual void				RegCallbackOnGetTextureID(OnGetTextureID *Fn) = 0; // Tex was precached.

	typedef void				OnLoadTexture(const char *TexFileName);
	virtual void				RegCallbackOnLoadTexture(OnLoadTexture *Fn) = 0; // This is effect of GetTextureID call in case of loading tex file from disk.

    virtual int                 CreateTexture       ( const char* texName, 
                                                        int width, int height, 
                                                        ColorFormat clrFormat, 
                                                        int nMips = 0, 
                                                        TextureMemoryPool memPool = tmpManaged, 
                                                        bool bRenderTarget   = false, 
                                                        DepthStencilFormat dsFormat = dsfNone,
                                                        bool bDynamic        = false ) = 0;
    virtual int                 CreateNormalMap     ( int texID, float amplitude = 10.0f ) = 0;
    virtual bool                DeleteTexture       ( int texID ) = 0;
    virtual BYTE*               LockTexBits         ( int texID, int& pitch, int level = 0 ) = 0;
    virtual BYTE*               LockTexBits         ( int texID, const Rct& rect, int& pitch, int level = 0    ) = 0;
    virtual void                UnlockTexBits       ( int texID, int level = 0 ) = 0;

    virtual const char*         GetTextureName      ( int texID ) = 0;
    virtual ColorFormat         GetTextureFormat    ( int texID ) = 0;

    virtual int                 GetNTextures        () const = 0;
    virtual int                 GetTextureSize      ( int texID ) const = 0;
    virtual TextureMemoryPool   GetTexturePool      ( int texID ) const = 0;
    
    virtual int                 GetTextureWidth     ( int texID ) const = 0;
    virtual int                 GetTextureHeight    ( int texID ) const = 0;
    virtual int                 GetTextureNMips     ( int texID ) const = 0;
    virtual int                 GetTexMemorySize    () const = 0;

    //  shaders
    virtual int                 GetShaderID         ( const char* shaderName ) = 0;
    virtual IShader*            GetShader           ( int shID ) const = 0;
    virtual const char*         GetShaderName       ( int shID ) const = 0;
    virtual bool                SetShaderTech       ( int shID, int techID ) = 0;
    virtual int                 GetNShaderVars      ( int shID ) const= 0;
    virtual int                 GetNShaderPasses    ( int shID ) const= 0;
    virtual int                 GetShaderVarID      ( int shID, const char* constantName ) = 0;
    virtual bool                SetShaderVar        ( int shID, int cID, bool val ) = 0;
    virtual bool                SetShaderVar        ( int shID, int cID, float val ) = 0;
    virtual bool                SetShaderVar        ( int shID, int cID, int val ) = 0;
    virtual bool                SetShaderVar        ( int shID, int cID, const Matrix4D& val ) = 0;
    virtual bool                SetShaderVar        ( int shID, int cID, const Vector4D& val ) = 0;
	virtual bool                SetShaderVar        ( int shID, int cID, const Vector4D* val,int count ) = 0;
    virtual bool                SetShaderVar        ( int shID, int cID, const Vector3D& val ) = 0;
    virtual bool                ReloadShaders       () = 0;
    virtual void                SetShader           ( int shaderID, int passID = 0 ) = 0;
    virtual void                SetShaderAutoVars   () = 0;
    virtual bool                IsShaderValid       ( int shID, int techID = 0 ) = 0;
    virtual void                SetShadersQuality   ( int Level ) = 0;//0 - maximal 1 - low
    virtual int                 GetShadersQuality   () = 0;
	virtual void				SetBumpEnable		( bool State ) = 0;
	virtual	bool				GetBumpEnable		() = 0;
	virtual void				SetShaderConst		(int ConstIndex,float Value) = 0;//ConstIndex=0..3 - you can refer them like ShaderConst0 .. ShaderConst3
	virtual float				GetShaderConst		(int ConstIndex ) = 0;

    virtual bool                SetClipPlane        ( DWORD idx, const Plane& plane ) = 0;
    virtual bool                PushRenderTarget    ( int texID, int dsID = -1 ) = 0;
    virtual bool                PopRenderTarget     () = 0;
    virtual void                SetViewPort         ( const Rct& vp, float zn = 0.0f, float zf = 1.0f, bool bClip = true ) = 0;
    virtual Rct                 GetViewPort         () const = 0;

    //  render states 
    virtual void                SetTextureFactor    ( DWORD tfactor          ) = 0;
    virtual DWORD               GetTextureFactor    () const = 0;
    virtual void                SetAlphaRef         ( BYTE alphaRef          ) = 0;
    virtual void                SetZEnable          ( bool bEnable = true ) = 0;
    virtual void                SetZWriteEnable     ( bool bEnable = true ) = 0;
    virtual void                SetDitherEnable     ( bool bEnable = true ) = 0;
    virtual void                SetTexFilterEnable  ( bool bEnable = true ) = 0;
    virtual void                SetWireframe        ( bool bEnable = true ) = 0;
    virtual void                SetColorConst       ( DWORD Color ) = 0; // this variable can be referenced in vertex shader like ColorConst
    virtual DWORD               GetColorConst       () = 0;

    virtual void                Dump                ( const char* fname = 0 ) = 0;
    virtual bool                ApplyStateBlock     ( DWORD id ) = 0;
    virtual bool                DeleteStateBlock    ( DWORD id ) = 0;   
    virtual void                SetRSBlock          ( RenderStateBlock*    pBlock               ) = 0;
    virtual void                SetTSBlock          ( TextureStateBlock* pBlock, int stage ) = 0;
    virtual DWORD               CreateStateBlock    ( StateBlock*         pBlock               ) = 0;

    //  lighting
    virtual void                SetDirLight         ( DirectionalLight*    pLight, int& index ) = 0;
    virtual void                SetPointLight       ( PointLight*        pLight, int& index ) = 0;
    virtual void                SetSpotLight        ( SpotLight*        pLight, int& index ) = 0;
	virtual void				SetFog				( DWORD FogColor,float FogStart,float FogEnd,float FogDensity,int FogMode ) = 0;
	virtual void				ApplyFogStateBlock  () = 0;
    virtual void                DisableLights       () = 0;
    virtual void                SetMaterial         ( DWORD ambient, DWORD diffuse, DWORD specular, DWORD emissive, float power ) = 0;

    //  fog
    virtual float               GetFogDensity       () const = 0;
    virtual void                SetFogDensity       ( float fog ) = 0;

    //  font
    virtual int                 GetFontID           ( const char* name ) = 0;
    virtual void                DestroyFont         ( int fontID ) = 0;
    virtual int                 CreateFont          ( const char* name, int height, DWORD charset = DEFAULT_CHARSET, bool bBold = false, bool bItalic = false )                            = 0;
    virtual int                 CreateFont          ( const char* texName, int charW, int charH ) = 0;
    virtual int                 GetStringWidth      ( int fontID, const char* str, int spacing = 1 ) = 0;
    virtual int                 GetCharWidth        ( int fontID, BYTE ch ) = 0;
    virtual int                 GetCharHeight       ( int fontID, BYTE ch ) = 0;
    virtual void                SetCurrentFont      ( int fontID ) = 0;

    virtual bool                DrawString          ( const char* str, const Vector3D& pos, DWORD color = 0xFFFFFFFF, int spacing = 1 ) = 0;
    virtual bool                DrawString3D        ( const char* str, const Vector3D& pos, DWORD color = 0xFFFFFFFF, int spacing = 1 ) = 0;
    virtual bool                DrawChar            ( const Vector3D& pos, BYTE ch, DWORD color = 0xFFFFFFFF ) = 0;
    virtual bool                DrawChar            ( const Vector3D& pos, const Rct& uv, DWORD color = 0xFFFFFFFF ) = 0;
    virtual bool                DrawChar            ( const Vector3D& pos, const Rct& uv, float w, float h, DWORD color = 0xFFFFFFFF ) = 0;
    virtual void                FlushText           () = 0;    

    //  primitives
    virtual void                DrawLine            ( float x1, float y1, float x2, float y2, float z, DWORD color1, DWORD color2 ) = 0;
    virtual void                DrawLine            ( const Vector3D& a, const Vector3D& b, DWORD color1, DWORD color2 ) = 0;
    virtual void                DrawRect            ( const Rct& rct, const Rct& uv, float z, DWORD ca, DWORD cb, DWORD cc, DWORD cd ) = 0;
    virtual void                DrawPoly            ( float ax, float ay, float bx, float by, float cx, float cy,
                                                       float au, float av, float bu, float bv, float cu, float cv ) = 0;
    virtual void                DrawPoly            ( const Vector3D& a, const Vector3D& b, const Vector3D& c, DWORD acol, DWORD bcol, DWORD ccol,
                                                       float au, float av, float bu, float bv, float cu, float cv ) = 0;
    virtual void                FlushPrim           ( bool bShaded = true ) = 0;
    virtual void                PurgeStaticBuffers  () = 0;
    virtual RenderTask&         AddTask             () = 0;
    virtual void                Flush               () = 0;

	virtual void				ScreenShotBMP		(const char *pBMPFileName) = 0;
	virtual void				ScreenShotJPG		(const char *pJPGFileName) = 0;

	virtual void				TimeOverrideEnable		(const float secNewTime) = 0;
	virtual void				TimeOverrideDisable		() = 0;
	virtual bool				TimeOverrideIsEnabled	(float *pSecTime = NULL) = 0;

	virtual void				AntialiasingEnable		() = 0;
	virtual void				AntialiasingDisable		() = 0;
	virtual bool				AntialiasingIsEnabled	() = 0;

	virtual void				TrueColorEnable			() = 0;
	virtual void				TrueColorDisable		() = 0;
	virtual bool				TrueColorIsEnabled		() = 0;

	virtual bool				GetDeviceDisplayMode	(int *pWidth, int *pHeight, int *pBpp, int *pRefreshRate) = 0;
	
	virtual void				RefreshRateOverrideEnable		(const int Hz) = 0;
	virtual void				RefreshRateOverrideDisable		() = 0;
	virtual bool				RefreshRateOverrideIsEnabled	(int *pHz = NULL) = 0;
	virtual HWND				GetHWND							() = 0;

	virtual void				AddScreenResolution		(const int XRes, const int YRes, const int RR) = 0;

}; // class IRenderSystem

extern DIALOGS_API IRenderSystem* IRS;
IRenderSystem* GetRenderSystem();

/*****************************************************************
/*  Class:  IDeviceClient
/*  Desc:   Notified when render system is destroyed, for 
/*                example, when screen resolution is being changed
/*****************************************************************/
class IDeviceClient
{
public:
    virtual void    OnDestroyRS   () = 0;
    virtual void    OnCreateRS    () = 0;

}; // class IDeviceClient


#endif // __RENDERSYSTEM_H__