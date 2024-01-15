/*****************************************************************************/
/*	File:	sgDeviceSettings.cpp
/*	Desc:	DeviceSettings management implementation
/*	Author:	Ruslan Shestopalyuk
/*	Date:	11-24-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "kInput.h"
#include "sgDeviceSettings.h"

BEGIN_NAMESPACE(sg)
/*****************************************************************************/
/*	DeviceSettings implementation
/*****************************************************************************/
DeviceSettings::DeviceSettings()
{
	SetName( "DeviceSettings" );
	m_ScreenResolution			= sr1024x768;
	m_BackBufferColorFormat		= cfRGB565;
	m_DepthStencilFormat		= dsfD16;	
	m_bFullScreen				= false;
}

void DeviceSettings::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "DeviceSettings", this );
	pm.f( "ScreenResolution",		 	m_ScreenResolution			);
	pm.f( "BackBufferColorFormat", 	m_BackBufferColorFormat		);
	pm.f( "DepthStencilFormat",	 	m_DepthStencilFormat		);
	pm.f( "FullScreen",			 	m_bFullScreen				);

	pm.m( "ApplySettings", ApplySettings );
} // DeviceSettings::Expose

void DeviceSettings::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
}

void DeviceSettings::Unserialize( InStream& is	)
{
	Parent::Unserialize( is );
}

void DeviceSettings::ApplySettings()
{

}

END_NAMESPACE(sg)
