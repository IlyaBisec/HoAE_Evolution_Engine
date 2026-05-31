/*****************************************************************************/
/*	File:	rsDXErr.cpp
/*	Desc:	DirectX error handling implementation
/*	Author:	Ruslan Shestopalyuk
/*	Date:	30.01.2003
/*****************************************************************************/
#include "stdafx.h"
#include "rsDX.h"
#include "rsDXErr.h"

const char* GetD3DErrorDesc( HRESULT hresult )
{
	switch (hresult)
	{
	case D3DERR_CONFLICTINGRENDERSTATE: 
		return "The currently set render states cannot be used together."; 
	case D3DERR_CONFLICTINGTEXTUREFILTER: 
		return "The current texture filters cannot be used together.";
	case D3DERR_CONFLICTINGTEXTUREPALETTE: 
		return "The current textures cannot be used simultaneously.";
	case D3DERR_DEVICELOST: 
		return "The device is lost and cannot be restored at the current time."
				"Rendering is not possible."; 
	case D3DERR_DEVICENOTRESET: 
		return "The device cannot be reset."; 
	case D3DERR_DRIVERINTERNALERROR: 
		return "Internal driver error."; 
	case D3DERR_INVALIDCALL: 
		return "The method call is invalid. " 
				"Method's parameter may have an invalid value."; 
	case D3DERR_INVALIDDEVICE: 
		return "The requested device type is not valid."; 
	case D3DERR_MOREDATA: 
		return "There is more data available than the specified buffer size can hold."; 
	case D3DERR_NOTAVAILABLE: 
		return "This device does not support the queried technique."; 
	case D3DERR_NOTFOUND: 
		return "The requested item was not found.";
	case D3DERR_OUTOFVIDEOMEMORY: 
		return "Direct3D does not have enough display memory to perform the operation."; 
	case D3DERR_TOOMANYOPERATIONS: 
		return "The application is requesting more texture-filtering operations "
				"than the device supports."; 
	case D3DERR_UNSUPPORTEDALPHAARG: 
		return "The device does not support a specified texture-blending argument "
				"for the alpha channel.";
	case D3DERR_UNSUPPORTEDALPHAOPERATION: 
		return "The device does not support a specified texture-blending operation " 
				"for the alpha channel."; 
	case D3DERR_UNSUPPORTEDCOLORARG: 
		return "The device does not support a specified texture-blending argument "
				"for color values."; 
	case D3DERR_UNSUPPORTEDCOLOROPERATION: 
		return "The device does not support a specified texture-blending operation "
				"for color values."; 
	case D3DERR_UNSUPPORTEDFACTORVALUE: 
		return "The device does not support the specified texture factor value."; 
	case D3DERR_UNSUPPORTEDTEXTUREFILTER: 
		return "The device does not support the specified texture filter."; 
	case D3DERR_WRONGTEXTUREFORMAT: 
		return "The pixel format of the texture surface is not valid."; 
	case E_FAIL: 
		return "An undetermined error occurred inside the Direct3D subsystem."; 
	case E_INVALIDARG: 
		return "An invalid parameter was passed to the returning function."; 
	case E_OUTOFMEMORY: 
		return "Direct3D could not allocate sufficient memory to complete the call.";
	default: return "";
	}
	return "";
} // GetD3DErrorDesc

const char* GetDispChangeErrorDesc( LONG result )
{
	switch (result)
	{
	case DISP_CHANGE_SUCCESSFUL:
		return "The display settings change was successful.";
	case DISP_CHANGE_RESTART:
		return "The computer must be restarted in order for the graphics mode to work."; 
	case DISP_CHANGE_BADFLAGS:
		return "An invalid set of flags was passed in ChangeDisplayMode."; 
	case DISP_CHANGE_BADPARAM:
		return "An invalid flag, combination of flags, or parameter passed in ChangeDisplayMode."; 
	case DISP_CHANGE_FAILED:
		return "The display driver failed the specified graphics mode."; 
	case DISP_CHANGE_BADMODE:
		return "The graphics mode is not supported."; 
	case DISP_CHANGE_NOTUPDATED:
		return "Unable to write settings to the registry."; 
	default: return "Unexpected error while ChangeDisplaySettings.";
	} 
} // GetDispChangeErrorDesc