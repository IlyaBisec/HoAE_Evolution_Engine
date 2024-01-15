//-----------------------------------------------------------------------------
// File: D3DSettings.h
//
// Desc: Settings class and change-settings dialog class for the Direct3D 
//       samples framework library.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
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

    D3DAdapterInfo*         pWindowed_AdapterInfo;
    D3DDeviceInfo*          pWindowed_DeviceInfo;
    D3DDeviceCombo*         pWindowed_DeviceCombo;

    D3DDISPLAYMODE          Windowed_DisplayMode; // not changable by the user
    D3DFORMAT               Windowed_DepthStencilBufferFormat;
	D3DMULTISAMPLE_TYPE     Windowed_WorstMultisampleType;
	D3DMULTISAMPLE_TYPE		Windowed_BestMultisampleType;
	D3DMULTISAMPLE_TYPE		Windowed_MultisampleType;
	DWORD                   Windowed_MultisampleQuality;
    VertexProcessingType    Windowed_VertexProcessingType;
    UINT                    Windowed_PresentInterval;
    bool                    bDeviceClip;
    int                     Windowed_Width;
    int                     Windowed_Height;

    D3DAdapterInfo*         pFullscreen_AdapterInfo;
    D3DDeviceInfo*          pFullscreen_DeviceInfo;
    D3DDeviceCombo*         pFullscreen_DeviceCombo;

    D3DDISPLAYMODE          Fullscreen_DisplayMode;             // changable by the user
    D3DFORMAT               Fullscreen_DepthStencilBufferFormat;
    D3DMULTISAMPLE_TYPE     Fullscreen_WorstMultisampleType;
	D3DMULTISAMPLE_TYPE		Fullscreen_BestMultisampleType;
	D3DMULTISAMPLE_TYPE		Fullscreen_MultisampleType;
    DWORD                   Fullscreen_MultisampleQuality;
    VertexProcessingType    Fullscreen_VertexProcessingType;
    UINT                    Fullscreen_PresentInterval;

    D3DAdapterInfo*         PAdapterInfo() const { return m_bIsWindowed ? pWindowed_AdapterInfo : pFullscreen_AdapterInfo; }
    D3DDeviceInfo*          PDeviceInfo () const { return m_bIsWindowed ? pWindowed_DeviceInfo  : pFullscreen_DeviceInfo; }
    D3DDeviceCombo*         PDeviceCombo() const { return m_bIsWindowed ? pWindowed_DeviceCombo : pFullscreen_DeviceCombo; }

    int                     Ordinal     () { return PDeviceCombo()->m_Ordinal; }
    D3DDEVTYPE              DevType     () { return PDeviceCombo()->m_DevType; }

	bool					m_BackBufferForceTrueColor;
    D3DFORMAT               BackBufferFormat() {
		if(m_BackBufferForceTrueColor) {
			return D3DFMT_X8R8G8B8;
		}
		else {
			return PDeviceCombo()->m_BackBufferFormat;
		}
	}
	void					BackBufferForceTrueColorEnable() { m_BackBufferForceTrueColor = true; }
	void					BackBufferForceTrueColorDisable() { m_BackBufferForceTrueColor = false; }

    D3DDISPLAYMODE          DisplayMode () { return m_bIsWindowed ? Windowed_DisplayMode : Fullscreen_DisplayMode; }
    void SetDisplayMode(D3DDISPLAYMODE value) { if (m_bIsWindowed) Windowed_DisplayMode = value; else Fullscreen_DisplayMode = value; }

    D3DFORMAT DepthStencilBufferFormat() { return m_bIsWindowed ? Windowed_DepthStencilBufferFormat : Fullscreen_DepthStencilBufferFormat; }
    void SetDepthStencilBufferFormat(D3DFORMAT value) { if (m_bIsWindowed) Windowed_DepthStencilBufferFormat = value; else Fullscreen_DepthStencilBufferFormat = value; }

	D3DMULTISAMPLE_TYPE MultisampleType() { return m_bIsWindowed ? Windowed_MultisampleType : Fullscreen_MultisampleType; }

	void SetMultisampleTypeToBest() {
		Windowed_MultisampleType = Windowed_BestMultisampleType;
		Fullscreen_MultisampleType = Fullscreen_BestMultisampleType;
	}
	
	void SetMultisampleTypeToWorst() {
		Windowed_MultisampleType = Windowed_WorstMultisampleType;
		Fullscreen_MultisampleType = Fullscreen_WorstMultisampleType;
	}

	D3DMULTISAMPLE_TYPE MultisampleTypeBest() {
		return m_bIsWindowed ? Windowed_BestMultisampleType : Fullscreen_BestMultisampleType;
	}

	D3DMULTISAMPLE_TYPE MultisampleTypeWorst() {
		return m_bIsWindowed ? Windowed_WorstMultisampleType : Fullscreen_WorstMultisampleType;
	}

    void SetMultisampleType(D3DMULTISAMPLE_TYPE value) {
		if (m_bIsWindowed) Windowed_MultisampleType = value; else Fullscreen_MultisampleType = value;
	}

    DWORD MultisampleQuality() { return m_bIsWindowed ? Windowed_MultisampleQuality : Fullscreen_MultisampleQuality; }
    void SetMultisampleQuality(DWORD value) { if (m_bIsWindowed) Windowed_MultisampleQuality = value; else Fullscreen_MultisampleQuality = value; }

    VertexProcessingType GetVertexProcessingType() { return m_bIsWindowed ? Windowed_VertexProcessingType : Fullscreen_VertexProcessingType; }
    void SetVertexProcessingType(VertexProcessingType value) { if (m_bIsWindowed) Windowed_VertexProcessingType = value; else Fullscreen_VertexProcessingType = value; }

    UINT PresentInterval() { return m_bIsWindowed ? Windowed_PresentInterval : Fullscreen_PresentInterval; }
    void SetPresentInterval(UINT value) { if (m_bIsWindowed) Windowed_PresentInterval = value; else Fullscreen_PresentInterval = value; }

    bool DeviceClip() { return bDeviceClip; }
    void SetDeviceClip( bool bClip ) { bDeviceClip = bClip; }
}; // class D3DSettings

//-----------------------------------------------------------------------------
// Name: class CD3DSettingsDialog
// Desc: Dialog box to allow the user to change the D3D settings
//-----------------------------------------------------------------------------
class CD3DSettingsDialog
{
private:
    HWND                    m_hDlg;
    D3DEnumeration*         m_pEnumeration;
    D3DSettings             m_d3dSettings;

    // ComboBox helper functions
    void    ComboBoxAdd                 ( int id, const void* pData, LPCTSTR pstrDesc );
    void    ComboBoxSelect              ( int id, const void* pData );
    void*   ComboBoxSelected            ( int id );
    bool    ComboBoxSomethingSelected   ( int id );
    UINT    ComboBoxCount               ( int id );
    void    ComboBoxSelectIndex         ( int id, int index );
    void    ComboBoxClear               ( int id );
    bool    ComboBoxContainsText        ( int id, LPCTSTR pstrText );

    void    AdapterChanged              ();
    void    DeviceChanged               ();
    void    WindowedFullscreenChanged   ();
    void    AdapterFormatChanged        ();
    void    ResolutionChanged           ();
    void    RefreshRateChanged          ();
    void    BackBufferFormatChanged     ();
    void    DSBufferFormatChanged       ();
    void    MultisampleTypeChanged      ();
    void    MultisampleQualityChanged   ();
    void    VertexProcessingChanged     ();
    void    PresentIntervalChanged      ();
    void    DeviceClipChanged           ();

public:
            CD3DSettingsDialog          ( D3DEnumeration* pEnumeration, D3DSettings* pSettings);
    INT_PTR ShowDialog                  ( HWND hwndParent );
    INT_PTR DialogProc                  ( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );
    void    GetFinalSettings            ( D3DSettings* pSettings ) { *pSettings = m_d3dSettings; }
}; // class CD3DSettingsDialog

#endif



