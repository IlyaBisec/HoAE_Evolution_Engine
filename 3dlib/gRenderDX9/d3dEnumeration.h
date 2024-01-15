//-----------------------------------------------------------------------------
// File: D3DEnumeration.h
// Desc: Enumerates D3D adapters, devices, modes, etc.
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef __D3DENUM_H__
#define __D3DENUM_H__

#include <vector>

//-----------------------------------------------------------------------------
// Enum: VertexProcessingType
// Desc: Enumeration of all possible D3D vertex processing types.
//-----------------------------------------------------------------------------
enum VertexProcessingType
{
    vpSoftware      = 0,
    vpMixed         = 1,
    vpHardware      = 2,
    vpPureHardware  = 3,
}; // enum VertexProcessingType


//-----------------------------------------------------------------------------
// Name: struct D3DDSMSConflict
// Desc: A depth/stencil buffer format that is incompatible with a
//       multisample type.
//-----------------------------------------------------------------------------
struct D3DDSMSConflict
{
    D3DFORMAT                       m_DSFormat;
    D3DMULTISAMPLE_TYPE             m_MSType;
}; // struct D3DDSMSConflict

//-----------------------------------------------------------------------------
// Name: struct D3DDeviceCombo
// Desc: A combination of adapter format, back buffer format, and windowed/fullscreen 
//       that is compatible with a particular D3D device (and the app).
//-----------------------------------------------------------------------------
struct D3DDeviceCombo
{
    int                                 m_Ordinal;
    D3DDEVTYPE                          m_DevType;
    D3DFORMAT                           m_AdapterFormat;
    D3DFORMAT                           m_BackBufferFormat;
    bool                                m_bIsWindowed;
    std::vector<D3DFORMAT>              m_DepthStencilFormats;
    std::vector<D3DMULTISAMPLE_TYPE>    m_MSampleTypes;
    std::vector<DWORD>                  m_MSampleQualities; 
    std::vector<D3DDSMSConflict>        m_DSMSConflicts;
    std::vector<VertexProcessingType>   m_VProcessTypes;
    std::vector<DWORD>                  m_PresentIntervals;
}; // struct D3DDeviceCombo

//-----------------------------------------------------------------------------
// Name: struct D3DDeviceInfo
// Desc: Info about a D3D device, including a list of D3DDeviceCombos (see below) 
//       that work with the device.
//-----------------------------------------------------------------------------
struct D3DDeviceInfo
{
    int                             m_Ordinal;
    D3DDEVTYPE                      m_DevType;
    D3DCAPS9                        m_Caps;
    std::vector<D3DDeviceCombo>     m_DevCombos; 
}; // struct D3DDeviceInfo

//-----------------------------------------------------------------------------
// Name: struct D3DAdapterInfo
// Desc: Info about a display adapter.
//-----------------------------------------------------------------------------
struct D3DAdapterInfo
{
    int                             m_Ordinal;
    D3DADAPTER_IDENTIFIER9          m_Identifier;
    std::vector<D3DDISPLAYMODE>     m_DisplayModes; 
    std::vector<D3DDeviceInfo>      m_DevInfos; 
}; // struct D3DAdapterInfo

typedef bool (*ConfirmDeviceCallback)(  D3DCAPS9* pCaps, 
                                        VertexProcessingType vertexProcessingType, 
									    D3DFORMAT adapterFormat, 
                                        D3DFORMAT backBufferFormat );
//-----------------------------------------------------------------------------
// Name: class D3DEnumeration
// Desc: Enumerates available D3D adapters, devices, modes, etc.
//-----------------------------------------------------------------------------
class D3DEnumeration
{
private:
    IDirect3D9*     m_pD3D;

    HRESULT         EnumerateDevices                ( D3DAdapterInfo* pAdapterInfo, std::vector<D3DFORMAT>& adapterFormatList );
    HRESULT         EnumerateDeviceCombos           ( D3DDeviceInfo* pDeviceInfo, std::vector<D3DFORMAT>& adapterFormatList );
    void            BuildDepthStencilFormatList     ( D3DDeviceCombo* pDeviceCombo );
    void            BuildMultiSampleTypeList        ( D3DDeviceCombo* pDeviceCombo );
    void            BuildDSMSConflictList           ( D3DDeviceCombo* pDeviceCombo );
    void            BuildVertexProcessingTypeList   ( D3DDeviceInfo* pDeviceInfo, D3DDeviceCombo* pDeviceCombo );
    void            BuildPresentIntervalList        ( D3DDeviceInfo* pDeviceInfo, D3DDeviceCombo* pDeviceCombo );

public:
    std::vector<D3DAdapterInfo> m_AdapterInfoList;

    // The following variables can be used to limit what modes, formats, 
    // etc. are enumerated.  Set them to the values you want before calling
    // Enumerate().
    ConfirmDeviceCallback       ConfirmDevice;
    UINT                        m_AppMinFullscreenWidth;
    UINT                        m_AppMinFullscreenHeight;
    UINT                        m_AppMinColorChannelBits;     // min color bits per channel in adapter format
    UINT                        m_AppMinAlphaChannelBits;     // min alpha bits per pixel in back buffer format
    UINT                        m_AppMinDepthBits;
    UINT                        m_AppMinStencilBits;
    bool                        m_bUseZBuffer;
    bool                        m_bAppUsesMixedVP;            // whether app can take advantage of mixed vp mode
    bool                        m_bAppRequiresWindowed;        
    bool                        m_bAppRequiresFullscreen;
    std::vector<D3DFORMAT>      m_AllowedFormats;         

                                D3DEnumeration     ();
    void                        SetD3D             ( IDirect3D9* pD3D ) { m_pD3D = pD3D; }
    IDirect3D9*                 GetD3D             () { if (m_pD3D) m_pD3D->AddRef(); return m_pD3D; }
    HRESULT                     Enumerate          ();
}; // class D3DEnumeration

#endif // __D3DENUM_H__
