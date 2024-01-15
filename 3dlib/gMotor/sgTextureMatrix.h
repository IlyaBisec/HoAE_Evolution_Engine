/***********************************************************************************/
/*  File:   sgTextureMatrix.h
/*  Date:   14.11.2005
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __SGTEXTUREMATRIX_H__
#define __SGTEXTUREMATRIX_H__

#include "sgTransformNode.h"

/*****************************************************************************/
/*    Class:    TextureMatrix
/*    Desc:    UV transformation matrix node
/*****************************************************************************/
class TextureMatrix : public TransformNode
{
    int                 m_Stage;

public:
                        TextureMatrix();
    virtual void        Render            ();
    virtual void        Serialize        ( OutStream& os ) const;
    virtual void        Unserialize        ( InStream& is    );
    virtual void        Expose            ( PropertyMap& pm );

    void                SetStage        ( int stage ) { m_Stage = stage; }
    void                SetTextureTM    ( const Matrix4D& m );


    DECLARE_SCLASS(TextureMatrix,TransformNode,TMTR);
}; // class TextureMatrix


#endif //__SGTEXTUREMATRIX_H__