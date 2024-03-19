/*****************************************************************************/
/*    File:    ILight.h
/*    Author:  Ruslan Shestopalyuk
/*    Date:    10-15-2003
/*****************************************************************************/
#ifndef __ILIGHT_H__
#define __ILIGHT_H__

class Vector3D;

/*****************************************************************************/
/*    Class:    ILight
/*    Desc:    Interface for lights manipulation
/*****************************************************************************/
class ILight
{
public:
    virtual void            Render          () = 0;

    virtual Vector3D        GetPos          () const = 0;
    virtual Vector3D        GetDir          () const = 0;

    virtual DWORD           GetAmbient      () const = 0;
    virtual DWORD           GetDiffuse      () const = 0;
    virtual DWORD           GetSpecular     () const = 0;
    virtual float           GetRange        () const = 0;

    virtual DWORD           GetIndex        ()    const = 0;
    virtual void            SetIndex        ( DWORD index ) = 0;
    virtual void            SetPos          ( const Vector3D& pos ) = 0;
    virtual void            SetDir          ( const Vector3D& dir ) = 0;
    virtual void            SetDiffuse      ( DWORD diffuse  ) = 0;
    virtual void            SetAmbient      ( DWORD ambient  ) = 0;
    virtual void            SetSpecular     ( DWORD specular ) = 0;
    virtual void            SetRange        ( float range    ) = 0;

}; // class ILight

#endif // __ILight_H__