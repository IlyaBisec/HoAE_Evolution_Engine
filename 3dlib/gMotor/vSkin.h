/*****************************************************************************/
/*    File:    vSkin.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#ifndef __VSKIN_H__
#define __VSKIN_H__

#include "sgMorphedGeometry.h"

/*****************************************************************************/
/*    Class:    Skin
/*    Desc:    Geometry with vertex position bound to another scene graph node
/*                transforms. 
/*****************************************************************************/
class Skin : public MorphedGeometry
{
public:
                            Skin            ();
    virtual                 ~Skin           ();

    virtual void            Serialize       ( OutStream& os        ) const;
    virtual void            Unserialize     ( InStream& is        );
    virtual void            Expose          ( PropertyMap& pm );
    void                    AddBoneOffset   ( const Matrix4D& matr ) { m_BoneOffsets.push_back( matr ); }
    const Matrix4D&         GetMatrix       ( int idx );              
    const Matrix4D&         GetBoneOffset   ( int idx ) const        { return m_BoneOffsets[idx]; }
    virtual void            Render          ();
    int                     GetNBones       () const { return m_InputBoneNames.size(); } 

    void                    SetIsStatic     ( bool val = true ) {}
    virtual void            FlipNormals     ();
    virtual void            CalculateNormals();
    int                     GetNWeights     () const { return m_NWeights; }
    void                    AddInputBone    ( const char* bName );

    DECLARE_SCLASS(Skin,MorphedGeometry,SKIN);

    static int                  s_SkinVBuffer;
    static int                  s_SkinIBuffer;

//protected:
    void                        OnProcessGeometry();
    bool                        BindBones();

    Matrix4D*                    m_BoneTM;
    int                         m_NBones;    

    std::vector<Matrix4D>        m_BoneOffsets;
    std::vector<std::string>    m_InputBoneNames;
    std::vector<TransformNode*> m_InputBones;

    bool                        m_bBonesBound;
    int                            m_NWeights;        //  number of skinning weights
}; // class Skin

class SkinGPU : public Skin{
protected:
	int						m_MatrixArrayInShader;
	StaticMesh				m_Mesh;
public:
	SkinGPU();
	~SkinGPU();
	virtual void            Serialize       ( OutStream& os        ) const;
	virtual void            Unserialize     ( InStream& is        );
	virtual void            Expose          ( PropertyMap& pm );
	virtual void            Render          ();
	StaticMesh*				GetStaticMesh	();

	DECLARE_SCLASS(SkinGPU,MorphedGeometry,SGPU);
};
bool ConvertNodesToSkinGPU(SNode* Node);
#endif // __VSKIN_H__