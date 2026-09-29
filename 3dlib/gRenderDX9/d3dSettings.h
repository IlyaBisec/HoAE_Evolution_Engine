//-----------------------------------------------------------------------------
// File: D3DSettings.h
//
// Desc: Settings class and change-settings dialog class for the Direct3D
//       samples framework library.
//-----------------------------------------------------------------------------
#ifndef D3DSETTINGS_H
#define D3DSETTINGS_H

//-----------------------------------------------------------------------------
// Name: class D3DSettings
// Desc: Current D3D settings: adapter, device, mode, formats, etc.
//-----------------------------------------------------------------------------
class D3DSettings
{
public:
    bool                    m_bIsWindowed;

    D3DAdapterInfo *pWindowed_AdapterInfo;
    D3DDeviceInfo *pWindowed_DeviceInfo;
    D3DDeviceCombo *pWindowed_DeviceCombo;

    D3DDISPLAYMODE          Windowed_DisplayMode;
    D3DFORMAT               Windowed_DepthStencilBufferFormat;

    D3DMULTISAMPLE_TYPE     Windowed_WorstMultisampleType;
    D3DMULTISAMPLE_TYPE     Windowed_BestMultisampleType;
    D3DMULTISAMPLE_TYPE     Windowed_MultisampleType;
    DWORD                   Windowed_MultisampleQuality;

    VertexProcessingType    Windowed_VertexProcessingType;
    UINT                    Windowed_PresentInterval;

    bool                    bDeviceClip;
    int                     Windowed_Width;
    int                     Windowed_Height;

    D3DAdapterInfo *pFullscreen_AdapterInfo;
    D3DDeviceInfo *pFullscreen_DeviceInfo;
    D3DDeviceCombo *pFullscreen_DeviceCombo;

    D3DDISPLAYMODE          Fullscreen_DisplayMode;
    D3DFORMAT               Fullscreen_DepthStencilBufferFormat;

    D3DMULTISAMPLE_TYPE     Fullscreen_WorstMultisampleType;
    D3DMULTISAMPLE_TYPE     Fullscreen_BestMultisampleType;
    D3DMULTISAMPLE_TYPE     Fullscreen_MultisampleType;
    DWORD                   Fullscreen_MultisampleQuality;

    VertexProcessingType    Fullscreen_VertexProcessingType;
    UINT                    Fullscreen_PresentInterval;

    bool                    m_BackBufferForceTrueColor;

    //-------------------------------------------------------------------------
    // Constructor
    //-------------------------------------------------------------------------
    D3DSettings()
    {
        m_bIsWindowed = true;

        pWindowed_AdapterInfo = NULL;
        pWindowed_DeviceInfo = NULL;
        pWindowed_DeviceCombo = NULL;

        pFullscreen_AdapterInfo = NULL;
        pFullscreen_DeviceInfo = NULL;
        pFullscreen_DeviceCombo = NULL;

        ZeroMemory(&Windowed_DisplayMode, sizeof(Windowed_DisplayMode));
        ZeroMemory(&Fullscreen_DisplayMode, sizeof(Fullscreen_DisplayMode));

        Windowed_DisplayMode.Format = D3DFMT_X8R8G8B8;
        Windowed_DisplayMode.Width = 800;
        Windowed_DisplayMode.Height = 600;
        Windowed_DisplayMode.RefreshRate = 0;

        Fullscreen_DisplayMode.Format = D3DFMT_X8R8G8B8;
        Fullscreen_DisplayMode.Width = 800;
        Fullscreen_DisplayMode.Height = 600;
        Fullscreen_DisplayMode.RefreshRate = 0;

        Windowed_DepthStencilBufferFormat = D3DFMT_D24S8;
        Fullscreen_DepthStencilBufferFormat = D3DFMT_D24S8;

        Windowed_WorstMultisampleType = D3DMULTISAMPLE_NONE;
        Windowed_BestMultisampleType = D3DMULTISAMPLE_NONE;
        Windowed_MultisampleType = D3DMULTISAMPLE_NONE;
        Windowed_MultisampleQuality = 0;

        Fullscreen_WorstMultisampleType = D3DMULTISAMPLE_NONE;
        Fullscreen_BestMultisampleType = D3DMULTISAMPLE_NONE;
        Fullscreen_MultisampleType = D3DMULTISAMPLE_NONE;
        Fullscreen_MultisampleQuality = 0;

        Windowed_VertexProcessingType = vpHardware;
        Fullscreen_VertexProcessingType = vpHardware;

        Windowed_PresentInterval = D3DPRESENT_INTERVAL_DEFAULT;
        Fullscreen_PresentInterval = D3DPRESENT_INTERVAL_DEFAULT;

        bDeviceClip = true;

        Windowed_Width = 800;
        Windowed_Height = 600;

        m_BackBufferForceTrueColor = false;
    }

    D3DAdapterInfo *PAdapterInfo() const
    {
        return m_bIsWindowed
            ? pWindowed_AdapterInfo
            : pFullscreen_AdapterInfo;
    }

    D3DDeviceInfo *PDeviceInfo() const
    {
        return m_bIsWindowed
            ? pWindowed_DeviceInfo
            : pFullscreen_DeviceInfo;
    }

    D3DDeviceCombo *PDeviceCombo() const
    {
        return m_bIsWindowed
            ? pWindowed_DeviceCombo
            : pFullscreen_DeviceCombo;
    }

    int Ordinal() const
    {
        D3DDeviceCombo *pCombo = PDeviceCombo();
        return pCombo ? pCombo->m_Ordinal : -1;
    }

    D3DDEVTYPE DevType() const
    {
        D3DDeviceCombo *pCombo = PDeviceCombo();
        return pCombo ? pCombo->m_DevType : D3DDEVTYPE_HAL;
    }

    D3DFORMAT BackBufferFormat() const
    {
        if (m_BackBufferForceTrueColor)
            return D3DFMT_X8R8G8B8;

        D3DDeviceCombo *pCombo = PDeviceCombo();
        if (pCombo)
            return pCombo->m_BackBufferFormat;

        return D3DFMT_X8R8G8B8;
    }

    void BackBufferForceTrueColorEnable()
    {
        m_BackBufferForceTrueColor = true;
    }

    void BackBufferForceTrueColorDisable()
    {
        m_BackBufferForceTrueColor = false;
    }

    D3DDISPLAYMODE DisplayMode() const
    {
        return m_bIsWindowed
            ? Windowed_DisplayMode
            : Fullscreen_DisplayMode;
    }

    void SetDisplayMode(D3DDISPLAYMODE value)
    {
        if (m_bIsWindowed)
            Windowed_DisplayMode = value;
        else
            Fullscreen_DisplayMode = value;
    }

    D3DFORMAT DepthStencilBufferFormat() const
    {
        return m_bIsWindowed
            ? Windowed_DepthStencilBufferFormat
            : Fullscreen_DepthStencilBufferFormat;
    }

    void SetDepthStencilBufferFormat(D3DFORMAT value)
    {
        if (m_bIsWindowed)
            Windowed_DepthStencilBufferFormat = value;
        else
            Fullscreen_DepthStencilBufferFormat = value;
    }

    D3DMULTISAMPLE_TYPE MultisampleType() const
    {
        return m_bIsWindowed
            ? Windowed_MultisampleType
            : Fullscreen_MultisampleType;
    }

    void SetMultisampleTypeToBest()
    {
        Windowed_MultisampleType = Windowed_BestMultisampleType;
        Fullscreen_MultisampleType = Fullscreen_BestMultisampleType;
    }

    void SetMultisampleTypeToWorst()
    {
        Windowed_MultisampleType = Windowed_WorstMultisampleType;
        Fullscreen_MultisampleType = Fullscreen_WorstMultisampleType;
    }

    D3DMULTISAMPLE_TYPE MultisampleTypeBest() const
    {
        return m_bIsWindowed
            ? Windowed_BestMultisampleType
            : Fullscreen_BestMultisampleType;
    }

    D3DMULTISAMPLE_TYPE MultisampleTypeWorst() const
    {
        return m_bIsWindowed
            ? Windowed_WorstMultisampleType
            : Fullscreen_WorstMultisampleType;
    }

    void SetMultisampleType(D3DMULTISAMPLE_TYPE value)
    {
        if (m_bIsWindowed)
            Windowed_MultisampleType = value;
        else
            Fullscreen_MultisampleType = value;
    }

    DWORD MultisampleQuality() const
    {
        return m_bIsWindowed
            ? Windowed_MultisampleQuality
            : Fullscreen_MultisampleQuality;
    }

    void SetMultisampleQuality(DWORD value)
    {
        if (m_bIsWindowed)
            Windowed_MultisampleQuality = value;
        else
            Fullscreen_MultisampleQuality = value;
    }

    VertexProcessingType GetVertexProcessingType() const
    {
        return m_bIsWindowed
            ? Windowed_VertexProcessingType
            : Fullscreen_VertexProcessingType;
    }

    void SetVertexProcessingType(VertexProcessingType value)
    {
        if (m_bIsWindowed)
            Windowed_VertexProcessingType = value;
        else
            Fullscreen_VertexProcessingType = value;
    }

    UINT PresentInterval() const
    {
        return m_bIsWindowed
            ? Windowed_PresentInterval
            : Fullscreen_PresentInterval;
    }

    void SetPresentInterval(UINT value)
    {
        if (m_bIsWindowed)
            Windowed_PresentInterval = value;
        else
            Fullscreen_PresentInterval = value;
    }

    bool DeviceClip() const
    {
        return bDeviceClip;
    }

    void SetDeviceClip(bool bClip)
    {
        bDeviceClip = bClip;
    }
};


//-----------------------------------------------------------------------------
// Name: class CD3DSettingsDialog
// Desc: Dialog box to allow the user to change the D3D settings
//-----------------------------------------------------------------------------
class CD3DSettingsDialog
{
private:
    HWND                    m_hDlg;
    D3DEnumeration *m_pEnumeration;
    D3DSettings             m_d3dSettings;

    void    ComboBoxAdd(int id, const void *pData, LPCTSTR pstrDesc);
    void    ComboBoxSelect(int id, const void *pData);
    void *ComboBoxSelected(int id);
    bool    ComboBoxSomethingSelected(int id);
    UINT    ComboBoxCount(int id);
    void    ComboBoxSelectIndex(int id, int index);
    void    ComboBoxClear(int id);
    bool    ComboBoxContainsText(int id, LPCTSTR pstrText);

    void    AdapterChanged();
    void    DeviceChanged();
    void    WindowedFullscreenChanged();
    void    AdapterFormatChanged();
    void    ResolutionChanged();
    void    RefreshRateChanged();
    void    BackBufferFormatChanged();
    void    DSBufferFormatChanged();
    void    MultisampleTypeChanged();
    void    MultisampleQualityChanged();
    void    VertexProcessingChanged();
    void    PresentIntervalChanged();
    void    DeviceClipChanged();

public:
    CD3DSettingsDialog(D3DEnumeration *pEnumeration,
        D3DSettings *pSettings);

    ~CD3DSettingsDialog();

    INT_PTR ShowDialog(HWND hwndParent);

    INT_PTR DialogProc(HWND hDlg,
        UINT msg,
        WPARAM wParam,
        LPARAM lParam);

    void GetFinalSettings(D3DSettings *pSettings)
    {
        if (pSettings)
            *pSettings = m_d3dSettings;
    }
};

#endif