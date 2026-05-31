/***********************************************************************************/
/*  File:   sgMorphedGeometry.h
/*  Date:   14.11.2005
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __SGMORPHEDGEOMETRY_H__
#define __SGMORPHEDGEOMETRY_H__

#include "sgGeometry.h"
/*****************************************************************************/
/*  Class:   MorphedGeometry
/*  Desc:    Geometry with vertex position bound to another scene graph node
/*                transforms. 
/*****************************************************************************/
class MorphedGeometry : public Geometry
{
public:
                            MorphedGeometry () : m_bDisableMorphing( false ) {}
    virtual void            Expose              ( PropertyMap& pm );
    virtual void            Serialize           ( OutStream& os        ) const;
    virtual void            Unserialize         ( InStream& is        );

    static void             Freeze              () { s_bFrozen = true; }
    static void             Unfreeze            () { s_bFrozen = false; }
    virtual void            Render              ();

    void                    ProcessGeometry     ();
    void                    RenderMorphedGeometry();

    BaseMesh&               GetBaseMesh         () { return m_BaseMesh; }
    const BaseMesh&         GetBaseMesh         () const { return m_BaseMesh; }
    void                    ReplicateMesh       () { GetMesh().copy( m_BaseMesh ); }

    DECLARE_SCLASS(MorphedGeometry,Geometry,MGEO);

protected:

    //  callback, which is implemented by actual geometry morphers
    virtual void            OnProcessGeometry   () {}

    BaseMesh                m_BaseMesh;        
    bool                    m_bDisableMorphing;
    static bool             s_bFrozen;
}; // class MorphedGeometry

#endif //__SGMORPHEDGEOMETRY_H__