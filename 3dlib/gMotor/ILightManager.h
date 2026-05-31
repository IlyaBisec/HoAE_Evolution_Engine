/*****************************************************************************/
/*    File:    ILightManager.h
/*    Desc:    Interface for manipulating light environment
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-15-2003
/*****************************************************************************/
#ifndef __ILIGHTMANAGER_H__
#define __ILIGHTMANAGER_H__

/*****************************************************************************/
/*    Class:    ILightManager
/*    Desc:    Interface for lights manipulation
/*****************************************************************************/
class ILightManager
{
public:
    virtual void        Render        () = 0;

    virtual Vector3D    GetPos        () const = 0;
    virtual Vector3D    GetDir        () const = 0;

    virtual DWORD       GetAmbient    () const = 0;
    virtual DWORD       GetDiffuse    () const = 0;
    virtual DWORD       GetSpecular   () const = 0;
    virtual float       GetRange      () const = 0;

    virtual DWORD       GetIndex      ()    const = 0;
    virtual void        SetIndex      ( DWORD index ) = 0;
    virtual void        SetPos        ( const Vector3D& pos ) = 0;
    virtual void        SetDir        ( const Vector3D& dir ) = 0;
    virtual void        SetDiffuse    ( DWORD diffuse     ) = 0;
    virtual void        SetAmbient    ( DWORD ambient     ) = 0;
    virtual void        SetSpecular   ( DWORD specular ) = 0;
    virtual void        SetRange      ( float range     ) = 0;

}; // class ILightManager

extern ILightManager* ILM;

//
//std::vector<LightSource*> LightSource::s_ActiveLights;
//
//void LightSource::AddActiveLight( LightSource* pLight )
//{
//    int nLights = s_ActiveLights.size();
//    for (int i = 0; i < nLights; i++)
//    {
//        if (s_ActiveLights[i] == pLight) return;
//    }
//    s_ActiveLights.push_back( pLight );
//} // LightSource::AddActiveLight
//
//void LightSource::ClearActiveLights()
//{
//    s_ActiveLights.clear();
//} // LightSource::ClearActiveLights
//
//void LightSource::MirrorActiveLights( const Plane& plane )
//{
//    int nLights = s_ActiveLights.size();
//    IRS->DisableLights();
//    for (int i = 0; i < nLights; i++)
//    {
//        LightSource* pLight = s_ActiveLights[i];
//        Ray3D ray( pLight->GetPos(), pLight->GetDirZ() );
//        plane.Mirror( ray );
//        pLight->SetPos( ray.getOrig() );
//        pLight->SetDirZ( ray.getDir() );
//        pLight->Render();
//    }
//} // LightSource::MirrorActiveLights

#endif // __ILIGHTMANAGER_H__