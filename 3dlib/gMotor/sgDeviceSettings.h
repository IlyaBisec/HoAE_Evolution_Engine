/*****************************************************************************/
/*    File:    sgDeviceSettings.h
/*    Desc:    DeviceSettings commands management
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11-24-2003
/*****************************************************************************/
#ifndef __SGDEVICESETTINGS_H__
#define __SGDEVICESETTINGS_H__

/*****************************************************************************/
/*    Enum:    ScreenResolution
/*****************************************************************************/
namespace sg{
/*****************************************************************************/
/*    Class:    DeviceSettings
/*    Desc:    
/*****************************************************************************/
class DeviceSettings : public SNode, public PSingleton<DeviceSettings>
{
public:
                            DeviceSettings();

    virtual void            Expose            ( PropertyMap& pm );
    virtual void            Serialize        ( OutStream& os ) const;
    virtual void            Unserialize        ( InStream& is    );
    void                    ApplySettings    ();

    NODE(DeviceSettings,SNode,DEVS);

protected:
    ScreenResolution        m_ScreenResolution;
    ColorFormat                m_BackBufferColorFormat;
    DepthStencilFormat        m_DepthStencilFormat;
    bool                    m_bFullScreen;

}; // class DeviceSettings

}; // namespace sg

#endif // __SGDEVICESETTINGS_H__