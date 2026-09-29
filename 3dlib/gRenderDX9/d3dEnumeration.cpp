//-----------------------------------------------------------------------------
// File: D3DEnumeration.cpp
//
// Desc: Enumerates D3D adapters, devices, modes, etc.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#include "gRenderPch.h"


//-----------------------------------------------------------------------------
// Name: ColorChannelBits
// Desc: Returns the number of color channel bits in the specified D3DFORMAT
//-----------------------------------------------------------------------------

static UINT ColorChannelBits(D3DFORMAT fmt)
{
    switch (fmt)
    {
    case D3DFMT_R8G8B8:         return 8;
    case D3DFMT_A8R8G8B8:       return 8;
    case D3DFMT_X8R8G8B8:       return 8;

    case D3DFMT_R5G6B5:         return 5;
    case D3DFMT_X1R5G5B5:       return 5;
    case D3DFMT_A1R5G5B5:       return 5;

    case D3DFMT_A4R4G4B4:       return 4;

    case D3DFMT_R3G3B2:         return 2;
    case D3DFMT_A8R3G3B2:       return 2;
    case D3DFMT_X4R4G4B4:       return 4;

    case D3DFMT_A2B10G10R10:    return 10;
    case D3DFMT_A2R10G10B10:    return 10;

    default:
        return 0;
    }
} // ColorChannelBits


//-----------------------------------------------------------------------------
// Name: AlphaChannelBits
// Desc: Returns the number of alpha channel bits in the specified D3DFORMAT
//-----------------------------------------------------------------------------

static UINT AlphaChannelBits(D3DFORMAT fmt)
{
    switch (fmt)
    {
    case D3DFMT_R8G8B8:         return 0;
    case D3DFMT_A8R8G8B8:       return 8;
    case D3DFMT_X8R8G8B8:       return 0;

    case D3DFMT_R5G6B5:         return 0;
    case D3DFMT_X1R5G5B5:       return 0;
    case D3DFMT_A1R5G5B5:       return 1;

    case D3DFMT_A4R4G4B4:       return 4;

    case D3DFMT_R3G3B2:         return 0;
    case D3DFMT_A8R3G3B2:       return 8;
    case D3DFMT_X4R4G4B4:       return 0;

    case D3DFMT_A2B10G10R10:    return 2;
    case D3DFMT_A2R10G10B10:    return 2;

    default:
        return 0;
    }
} // AlphaChannelBits


//-----------------------------------------------------------------------------
// Name: DepthBits
// Desc: Returns the number of depth bits in the specified D3DFORMAT
//-----------------------------------------------------------------------------

static UINT DepthBits(D3DFORMAT fmt)
{
    switch (fmt)
    {
    case D3DFMT_D16:        return 16;
    case D3DFMT_D15S1:      return 15;
    case D3DFMT_D24X8:      return 24;
    case D3DFMT_D24S8:      return 24;
    case D3DFMT_D24X4S4:    return 24;
    case D3DFMT_D32:        return 32;

    default:
        return 0;
    }
} // DepthBits


//-----------------------------------------------------------------------------
// Name: StencilBits
// Desc: Returns the number of stencil bits in the specified D3DFORMAT
//-----------------------------------------------------------------------------

static UINT StencilBits(D3DFORMAT fmt)
{
    switch (fmt)
    {
    case D3DFMT_D16:        return 0;
    case D3DFMT_D15S1:      return 1;
    case D3DFMT_D24X8:      return 0;
    case D3DFMT_D24S8:      return 8;
    case D3DFMT_D24X4S4:    return 4;
    case D3DFMT_D32:        return 0;

    default:
        return 0;
    }
} // StencilBits


//-----------------------------------------------------------------------------
// Name: D3DEnumeration constructor
//-----------------------------------------------------------------------------

D3DEnumeration::D3DEnumeration()
{
    m_pD3D = NULL;
    ConfirmDevice = NULL;

    m_AppMinFullscreenWidth = 640;
    m_AppMinFullscreenHeight = 480;
    m_AppMinColorChannelBits = 5;
    m_AppMinAlphaChannelBits = 0;
    m_AppMinDepthBits = 15;
    m_AppMinStencilBits = 0;

    m_bUseZBuffer = false;
    m_bAppUsesMixedVP = false;
    m_bAppRequiresWindowed = false;
    m_bAppRequiresFullscreen = false;
} // D3DEnumeration::D3DEnumeration


//-----------------------------------------------------------------------------
// Name: SortModesCallback
// Desc: Used to sort D3DDISPLAYMODEs
//-----------------------------------------------------------------------------

static int __cdecl SortModesCallback(
    const void *arg1,
    const void *arg2)
{
    const D3DDISPLAYMODE *pdm1 =
        (const D3DDISPLAYMODE *)arg1;

    const D3DDISPLAYMODE *pdm2 =
        (const D3DDISPLAYMODE *)arg2;

    if (pdm1->Width > pdm2->Width)
        return 1;

    if (pdm1->Width < pdm2->Width)
        return -1;

    if (pdm1->Height > pdm2->Height)
        return 1;

    if (pdm1->Height < pdm2->Height)
        return -1;

    if (pdm1->Format > pdm2->Format)
        return 1;

    if (pdm1->Format < pdm2->Format)
        return -1;

    if (pdm1->RefreshRate > pdm2->RefreshRate)
        return 1;

    if (pdm1->RefreshRate < pdm2->RefreshRate)
        return -1;

    return 0;
} // SortModesCallback


//-----------------------------------------------------------------------------
// Name: Enumerate
// Desc: Enumerates available D3D adapters, devices, modes, etc.
//-----------------------------------------------------------------------------

HRESULT D3DEnumeration::Enumerate()
{
    HRESULT hr;
    std::vector<D3DFORMAT> adapterFormatList;

    if (m_pD3D == NULL)
        return E_FAIL;

    // Enumerate() can safely be called more than once.
    m_AdapterInfoList.clear();
    m_AllowedFormats.clear();

    m_AllowedFormats.push_back(D3DFMT_X8R8G8B8);
    m_AllowedFormats.push_back(D3DFMT_X1R5G5B5);
    m_AllowedFormats.push_back(D3DFMT_R5G6B5);
    m_AllowedFormats.push_back(D3DFMT_A2R10G10B10);

    UINT numAdapters = m_pD3D->GetAdapterCount();

    for (UINT m_Ordinal = 0;
        m_Ordinal < numAdapters;
        m_Ordinal++)
    {
        D3DAdapterInfo adapterInfo;

        adapterInfo.m_Ordinal = (int)m_Ordinal;

        hr = m_pD3D->GetAdapterIdentifier(
            m_Ordinal,
            0,
            &adapterInfo.m_Identifier
        );

        if (FAILED(hr))
            continue;

        // Get list of all display modes on this adapter.
        // Also build a temporary list of all display adapter formats.
        adapterFormatList.clear();

        for (size_t i = 0;
            i < m_AllowedFormats.size();
            i++)
        {
            D3DFORMAT allowedAdapterFormat =
                m_AllowedFormats[i];

            UINT numAdapterModes =
                m_pD3D->GetAdapterModeCount(
                    m_Ordinal,
                    allowedAdapterFormat
                );

            for (UINT mode = 0;
                mode < numAdapterModes;
                mode++)
            {
                D3DDISPLAYMODE displayMode;

                hr = m_pD3D->EnumAdapterModes(
                    m_Ordinal,
                    allowedAdapterFormat,
                    mode,
                    &displayMode
                );

                if (FAILED(hr))
                    continue;

                if (displayMode.Width < m_AppMinFullscreenWidth ||
                    displayMode.Height < m_AppMinFullscreenHeight ||
                    ColorChannelBits(displayMode.Format) <
                    m_AppMinColorChannelBits)
                {
                    continue;
                }

                adapterInfo.m_DisplayModes.push_back(displayMode);

                bool bPresent = false;

                for (size_t j = 0;
                    j < adapterFormatList.size();
                    j++)
                {
                    if (adapterFormatList[j] ==
                        displayMode.Format)
                    {
                        bPresent = true;
                        break;
                    }
                }

                if (!bPresent)
                    adapterFormatList.push_back(
                        displayMode.Format
                    );
            }
        }

        // Sort display mode list.
        //
        // IMPORTANT:
        // &vector[0] is not valid for an empty vector.
        if (!adapterInfo.m_DisplayModes.empty())
        {
            qsort(
                &adapterInfo.m_DisplayModes[0],
                adapterInfo.m_DisplayModes.size(),
                sizeof(D3DDISPLAYMODE),
                SortModesCallback
            );
        }

        // Get info for each device on this adapter.
        if (FAILED(
            hr = EnumerateDevices(
                &adapterInfo,
                adapterFormatList)))
        {
            return hr;
        }

        // If at least one device on this adapter is available
        // and compatible with the app, add the adapter.
        if (!adapterInfo.m_DevInfos.empty())
        {
            m_AdapterInfoList.push_back(adapterInfo);
        }
    }

    return S_OK;
} // D3DEnumeration::Enumerate


//-----------------------------------------------------------------------------
// Name: EnumerateDevices
//-----------------------------------------------------------------------------

HRESULT D3DEnumeration::EnumerateDevices(
    D3DAdapterInfo *pAdapterInfo,
    std::vector<D3DFORMAT> &adapterFormatList)
{
    if (pAdapterInfo == NULL)
        return E_INVALIDARG;

    const D3DDEVTYPE devTypeArray[] =
    {
        D3DDEVTYPE_HAL,
        D3DDEVTYPE_SW,
        D3DDEVTYPE_REF
    };

    const UINT devTypeArrayCount =
        sizeof(devTypeArray) / sizeof(devTypeArray[0]);

    HRESULT hr;

    for (UINT idt = 0;
        idt < devTypeArrayCount;
        idt++)
    {
        D3DDeviceInfo devInfo;

        devInfo.m_Ordinal = pAdapterInfo->m_Ordinal;
        devInfo.m_DevType = devTypeArray[idt];

        hr = m_pD3D->GetDeviceCaps(
            pAdapterInfo->m_Ordinal,
            devInfo.m_DevType,
            &devInfo.m_Caps
        );

        if (FAILED(hr))
            continue;

        if (FAILED(
            hr = EnumerateDeviceCombos(
                &devInfo,
                adapterFormatList)))
        {
            return hr;
        }

        if (devInfo.m_DevCombos.empty())
            continue;

        pAdapterInfo->m_DevInfos.push_back(devInfo);
    }

    return S_OK;
} // D3DEnumeration::EnumerateDevices


//-----------------------------------------------------------------------------
// Name: EnumerateDeviceCombos
//-----------------------------------------------------------------------------

HRESULT D3DEnumeration::EnumerateDeviceCombos(
    D3DDeviceInfo *pDeviceInfo,
    std::vector<D3DFORMAT> &adapterFormatList)
{
    if (pDeviceInfo == NULL)
        return E_INVALIDARG;

    const D3DFORMAT backBufferFormats[] =
    {
        D3DFMT_A8R8G8B8,
        D3DFMT_X8R8G8B8,
        D3DFMT_A2R10G10B10,
        D3DFMT_R5G6B5,
        D3DFMT_A1R5G5B5,
        D3DFMT_X1R5G5B5
    };

    const UINT nBackBufferFormats =
        sizeof(backBufferFormats) /
        sizeof(backBufferFormats[0]);

    bool bIsWindowed[] =
    {
        false,
        true
    };

    for (size_t iaf = 0;
        iaf < adapterFormatList.size();
        iaf++)
    {
        D3DFORMAT adapterFormat =
            adapterFormatList[iaf];

        for (UINT ibbf = 0;
            ibbf < nBackBufferFormats;
            ibbf++)
        {
            D3DFORMAT backBufferFormat =
                backBufferFormats[ibbf];

            if (AlphaChannelBits(backBufferFormat) <
                m_AppMinAlphaChannelBits)
            {
                continue;
            }

            for (UINT iiw = 0;
                iiw < 2;
                iiw++)
            {
                bool isWindowed =
                    bIsWindowed[iiw];

                if (!isWindowed &&
                    m_bAppRequiresWindowed)
                {
                    continue;
                }

                if (isWindowed &&
                    m_bAppRequiresFullscreen)
                {
                    continue;
                }

                if (FAILED(
                    m_pD3D->CheckDeviceType(
                        pDeviceInfo->m_Ordinal,
                        pDeviceInfo->m_DevType,
                        adapterFormat,
                        backBufferFormat,
                        isWindowed)))
                {
                    continue;
                }

                D3DDeviceCombo devCombo;

                devCombo.m_Ordinal =
                    pDeviceInfo->m_Ordinal;

                devCombo.m_DevType =
                    pDeviceInfo->m_DevType;

                devCombo.m_AdapterFormat =
                    adapterFormat;

                devCombo.m_BackBufferFormat =
                    backBufferFormat;

                devCombo.m_bIsWindowed =
                    isWindowed;

                if (m_bUseZBuffer)
                {
                    BuildDepthStencilFormatList(
                        &devCombo
                    );

                    if (devCombo.m_DepthStencilFormats.empty())
                        continue;
                }

                BuildMultiSampleTypeList(
                    &devCombo
                );

                if (devCombo.m_MSampleTypes.empty())
                    continue;

                BuildDSMSConflictList(
                    &devCombo
                );

                BuildVertexProcessingTypeList(
                    pDeviceInfo,
                    &devCombo
                );

                if (devCombo.m_VProcessTypes.empty())
                    continue;

                BuildPresentIntervalList(
                    pDeviceInfo,
                    &devCombo
                );

                if (devCombo.m_PresentIntervals.empty())
                    continue;

                pDeviceInfo->m_DevCombos.push_back(
                    devCombo
                );
            }
        }
    }

    return S_OK;
} // D3DEnumeration::EnumerateDeviceCombos


//-----------------------------------------------------------------------------
// Name: BuildDepthStencilFormatList
//-----------------------------------------------------------------------------

void D3DEnumeration::BuildDepthStencilFormatList(
    D3DDeviceCombo *pDeviceCombo)
{
    if (pDeviceCombo == NULL)
        return;

    const D3DFORMAT dsFormats[] =
    {
        D3DFMT_D16,
        D3DFMT_D15S1,
        D3DFMT_D24X8,
        D3DFMT_D24S8,
        D3DFMT_D24X4S4,
        D3DFMT_D32
    };

    const UINT nDsFormats =
        sizeof(dsFormats) / sizeof(dsFormats[0]);

    for (UINT idsf = 0;
        idsf < nDsFormats;
        idsf++)
    {
        D3DFORMAT depthStencilFmt =
            dsFormats[idsf];

        if (DepthBits(depthStencilFmt) <
            m_AppMinDepthBits)
        {
            continue;
        }

        if (StencilBits(depthStencilFmt) <
            m_AppMinStencilBits)
        {
            continue;
        }

        if (SUCCEEDED(
            m_pD3D->CheckDeviceFormat(
                pDeviceCombo->m_Ordinal,
                pDeviceCombo->m_DevType,
                pDeviceCombo->m_AdapterFormat,
                D3DUSAGE_DEPTHSTENCIL,
                D3DRTYPE_SURFACE,
                depthStencilFmt)))
        {
            if (SUCCEEDED(
                m_pD3D->CheckDepthStencilMatch(
                    pDeviceCombo->m_Ordinal,
                    pDeviceCombo->m_DevType,
                    pDeviceCombo->m_AdapterFormat,
                    pDeviceCombo->m_BackBufferFormat,
                    depthStencilFmt)))
            {
                pDeviceCombo->m_DepthStencilFormats.push_back(
                    depthStencilFmt
                );
            }
        }
    }
} // D3DEnumeration::BuildDepthStencilFormatList


//-----------------------------------------------------------------------------
// Name: BuildMultiSampleTypeList
//-----------------------------------------------------------------------------

void D3DEnumeration::BuildMultiSampleTypeList(
    D3DDeviceCombo *pDeviceCombo)
{
    if (pDeviceCombo == NULL)
        return;

    const D3DMULTISAMPLE_TYPE msTypeArray[] =
    {
        D3DMULTISAMPLE_NONE,
        D3DMULTISAMPLE_NONMASKABLE,
        D3DMULTISAMPLE_2_SAMPLES,
        D3DMULTISAMPLE_3_SAMPLES,
        D3DMULTISAMPLE_4_SAMPLES,
        D3DMULTISAMPLE_5_SAMPLES,
        D3DMULTISAMPLE_6_SAMPLES,
        D3DMULTISAMPLE_7_SAMPLES,
        D3DMULTISAMPLE_8_SAMPLES,
        D3DMULTISAMPLE_9_SAMPLES,
        D3DMULTISAMPLE_10_SAMPLES,
        D3DMULTISAMPLE_11_SAMPLES,
        D3DMULTISAMPLE_12_SAMPLES,
        D3DMULTISAMPLE_13_SAMPLES,
        D3DMULTISAMPLE_14_SAMPLES,
        D3DMULTISAMPLE_15_SAMPLES,
        D3DMULTISAMPLE_16_SAMPLES
    };

    const UINT msTypeArrayCount =
        sizeof(msTypeArray) /
        sizeof(msTypeArray[0]);

    for (UINT imst = 0;
        imst < msTypeArrayCount;
        imst++)
    {
        D3DMULTISAMPLE_TYPE msType =
            msTypeArray[imst];

        DWORD msQuality = 0;

        if (SUCCEEDED(
            m_pD3D->CheckDeviceMultiSampleType(
                pDeviceCombo->m_Ordinal,
                pDeviceCombo->m_DevType,
                pDeviceCombo->m_BackBufferFormat,
                pDeviceCombo->m_bIsWindowed,
                msType,
                &msQuality)))
        {
            pDeviceCombo->m_MSampleTypes.push_back(
                msType
            );

            pDeviceCombo->m_MSampleQualities.push_back(
                msQuality
            );
        }
    }
} // D3DEnumeration::BuildMultiSampleTypeList


//-----------------------------------------------------------------------------
// Name: BuildDSMSConflictList
//-----------------------------------------------------------------------------

void D3DEnumeration::BuildDSMSConflictList(
    D3DDeviceCombo *pDeviceCombo)
{
    if (pDeviceCombo == NULL)
        return;

    D3DDSMSConflict DSMSConflict;

    for (size_t i = 0;
        i < pDeviceCombo->m_DepthStencilFormats.size();
        i++)
    {
        D3DFORMAT dsFmt =
            pDeviceCombo->m_DepthStencilFormats[i];

        for (size_t j = 0;
            j < pDeviceCombo->m_MSampleTypes.size();
            j++)
        {
            D3DMULTISAMPLE_TYPE msType =
                pDeviceCombo->m_MSampleTypes[j];

            if (FAILED(
                m_pD3D->CheckDeviceMultiSampleType(
                    pDeviceCombo->m_Ordinal,
                    pDeviceCombo->m_DevType,
                    dsFmt,
                    pDeviceCombo->m_bIsWindowed,
                    msType,
                    NULL)))
            {
                DSMSConflict.m_DSFormat = dsFmt;
                DSMSConflict.m_MSType = msType;

                pDeviceCombo->m_DSMSConflicts.push_back(
                    DSMSConflict
                );
            }
        }
    }
} // BuildDSMSConflictList


//-----------------------------------------------------------------------------
// Name: BuildVertexProcessingTypeList
//-----------------------------------------------------------------------------

void D3DEnumeration::BuildVertexProcessingTypeList(
    D3DDeviceInfo *pDeviceInfo,
    D3DDeviceCombo *pDeviceCombo)
{
    if (pDeviceInfo == NULL ||
        pDeviceCombo == NULL)
    {
        return;
    }

    VertexProcessingType vpt;

    if ((pDeviceInfo->m_Caps.DevCaps &
        D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0)
    {
        if ((pDeviceInfo->m_Caps.DevCaps &
            D3DDEVCAPS_PUREDEVICE) != 0)
        {
            if (ConfirmDevice == NULL ||
                ConfirmDevice(
                    &pDeviceInfo->m_Caps,
                    vpPureHardware,
                    pDeviceCombo->m_AdapterFormat,
                    pDeviceCombo->m_BackBufferFormat))
            {
                vpt = vpPureHardware;

                pDeviceCombo->m_VProcessTypes.push_back(
                    vpt
                );
            }
        }

        if (ConfirmDevice == NULL ||
            ConfirmDevice(
                &pDeviceInfo->m_Caps,
                vpHardware,
                pDeviceCombo->m_AdapterFormat,
                pDeviceCombo->m_BackBufferFormat))
        {
            vpt = vpHardware;

            pDeviceCombo->m_VProcessTypes.push_back(
                vpt
            );
        }

        if (m_bAppUsesMixedVP &&
            (ConfirmDevice == NULL ||
                ConfirmDevice(
                    &pDeviceInfo->m_Caps,
                    vpMixed,
                    pDeviceCombo->m_AdapterFormat,
                    pDeviceCombo->m_BackBufferFormat)))
        {
            vpt = vpMixed;

            pDeviceCombo->m_VProcessTypes.push_back(
                vpt
            );
        }
    }

    if (ConfirmDevice == NULL ||
        ConfirmDevice(
            &pDeviceInfo->m_Caps,
            vpSoftware,
            pDeviceCombo->m_AdapterFormat,
            pDeviceCombo->m_BackBufferFormat))
    {
        vpt = vpSoftware;

        pDeviceCombo->m_VProcessTypes.push_back(
            vpt
        );
    }
} // BuildVertexProcessingTypeList


//-----------------------------------------------------------------------------
// Name: BuildPresentIntervalList
//-----------------------------------------------------------------------------

void D3DEnumeration::BuildPresentIntervalList(
    D3DDeviceInfo *pDeviceInfo,
    D3DDeviceCombo *pDeviceCombo)
{
    if (pDeviceInfo == NULL ||
        pDeviceCombo == NULL)
    {
        return;
    }

    const UINT piArray[] =
    {
        D3DPRESENT_INTERVAL_IMMEDIATE,
        D3DPRESENT_INTERVAL_DEFAULT,
        D3DPRESENT_INTERVAL_ONE,
        D3DPRESENT_INTERVAL_TWO,
        D3DPRESENT_INTERVAL_THREE,
        D3DPRESENT_INTERVAL_FOUR
    };

    const UINT piArrayCount =
        sizeof(piArray) / sizeof(piArray[0]);

    for (UINT ipi = 0;
        ipi < piArrayCount;
        ipi++)
    {
        UINT pi = piArray[ipi];

        if (pDeviceCombo->m_bIsWindowed)
        {
            if (pi == D3DPRESENT_INTERVAL_TWO ||
                pi == D3DPRESENT_INTERVAL_THREE ||
                pi == D3DPRESENT_INTERVAL_FOUR)
            {
                continue;
            }
        }

        // D3DPRESENT_INTERVAL_DEFAULT is zero and therefore
        // is always accepted here.
        if (pi == D3DPRESENT_INTERVAL_DEFAULT ||
            (pDeviceInfo->m_Caps.PresentationIntervals & pi))
        {
            pDeviceCombo->m_PresentIntervals.push_back(pi);
        }
    }
} // D3DEnumeration::BuildPresentIntervalList