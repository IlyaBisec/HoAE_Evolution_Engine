/*****************************************************************************/
/*    File:    sgTexture.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#ifndef __SGTEXTURE_H__
#define __SGTEXTURE_H__

#include <stack>

class Texture;
/*****************************************************************************/
/*    Class:    RenderTarget
/*****************************************************************************/
class RenderTarget : public SNode
{
public:
                                RenderTarget();
                                ~RenderTarget();

    virtual void                Render           ();
    void                        SetTarget        ( Texture* pTex, Texture* pDepth = NULL );
    void                        SetClearColor    ( DWORD color ) { m_ClearColor = color; }
    void                        EnableClear      ( bool bClear = true ) { m_bClear = bClear; }
    virtual void                Serialize        ( OutStream& os ) const;
    virtual void                Unserialize      ( InStream&  is );
    virtual void                Expose           ( PropertyMap& pm );
    void                        SetViewPort      ( float x, float y, float w, float h );

    
    DECLARE_SCLASS(RenderTarget, SNode, RTAR);

protected:
    
    Texture*                    m_pTexture;
    Texture*                    m_pDepthBuffer;
    Rct                         m_ViewPort;

    DWORD                       m_ClearColor;
    bool                        m_bClear;

}; // class RenderTarget

/*****************************************************************************/
/*    Class:    Texture
/*    Desc:    Texture attribute
/*****************************************************************************/
class Texture : public SNode
{
public:
    static const int            c_MaxStages = 16;

    _inl                        Texture         ();
    _inl                        Texture         ( const char* name, int stage = 0 );

    virtual void                Render          ();
    virtual void                Serialize       ( OutStream& os ) const;
    virtual void                Unserialize     ( InStream&  is );
    virtual void                VisitAttributes ();

    _inl void                   SetStage        ( int stage ) { m_Stage = stage; }
    _inl int                    GetStage        () const { return m_Stage; }

    _inl int                    GetWidth        () const;
    _inl int                    GetHeight       () const;
    _inl ColorFormat            GetColorFormat  () const;
    _inl DepthStencilFormat     GetDepthStencilFormat () const;

    _inl bool                   IsProcedural    () const;
    _inl bool                   IsRT            () const;
    _inl TextureMemoryPool      GetMemoryPool   () const;
    _inl TextureUsage           GetUsage        () const;
    _inl int                    GetNMips        () const;

    virtual void                Expose          ( PropertyMap& pm );
    virtual bool                IsEqual         ( const SNode* node ) const;


    const char*                 GetTextureFile  () const { return GetName(); }
    void                        SetTextureFile  ( const char* file );

    std::string                 GetFileName     () const;

    _inl void                   SetWidth       ( int            val        ); 
    _inl void                   SetHeight      ( int            val        );         
    _inl void                   SetColorFormat ( ColorFormat    format    );
    _inl void                   SetDepthStencilFormat ( DepthStencilFormat format );

    _inl void                   SetIsProcedural( bool            val        );
    _inl void                   SetIsRT        ( bool            val        );
    _inl void                   SetMemoryPool  ( TextureMemoryPool    pool    );
    _inl void                   SetNMips       ( int            val        );
    _inl void                   SetUsage       ( TextureUsage  usage    );

    static _inl void            Freeze         () { s_bFrozen = true; }
    static _inl void            Unfreeze       () { s_bFrozen = false; }

    void                        SetTexID       ( int id ) { m_TexID = id; }
    int                         GetTexID       () const { return m_TexID; }

    static _inl Texture*        GetCurTexture  ( int stage );
    bool                        CreateTexture  ();
    void                        CreateMipLevels();

    BYTE*                       LockBits       ( int& pitch, int level = 0 );
    void                        UnlockBits     ( int level = 0 );
    bool                        Save           ( const char* fname );

    static std::stack<int>      s_Overrides[c_MaxStages];

    static void                 PushOverride   ( int stage, int texID );
    static void                 PopOverride    ( int stage );

    DECLARE_SCLASS( Texture, SNode, TXRE );

private:
    WORD                        m_Stage;
    mutable DWORD               m_TexID;
    static Texture*             s_pCurTexture[c_MaxStages];
    static bool                 s_bFrozen;
}; // class Texture

/*****************************************************************************/
/*    Class:    TextureManager
/*    Desc:    Manages textures
/*****************************************************************************/
class TextureManager : public SNode, public PSingleton<TextureManager>
{
public:
    TextureManager() { SetName( "TextureManager" ); }

    DECLARE_SCLASS(TextureManager,SNode,TMGR);
}; // TextureManager



#ifdef _INLINES
#include "sgTexture.inl"
#endif // _INLINES

#endif // __SGTEXTURE_H__
