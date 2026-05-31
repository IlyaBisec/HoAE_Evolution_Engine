/*****************************************************************************/
/*    File:    vTreesRenderer.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11-12-2003
/*****************************************************************************/
#ifndef __VTREESRENDERER_H__
#define __VTREESRENDERER_H__

#include "kHash.hpp"
#include "kStaticArray.hpp"

/*****************************************************************************/
/*  Struct: TreeObject
/*  Desc:   Tree object description
/*****************************************************************************/
struct TreeObject
{
    int         m_ModelID;  //  tree model ID
    Matrix4D    m_TM;    
    TreeObject( int mid, const Matrix4D& tm ) : m_ModelID(mid), m_TM(tm) {}
    TreeObject() : m_ModelID(-1){} 
}; // struct TreeObject

/*****************************************************************************/
/*  Struct: TreesBlock
/*  Desc:   Cached trees geometry block
/*****************************************************************************/
struct TreesBlock
{
    struct Key
    {
        int         m_LOD;
        Rct         m_Ext;

        Key(){}
        Key( const Rct& ext, int lod ) : m_Ext(ext), m_LOD(lod) {}
        DWORD hash() const 
        {
            DWORD h = m_LOD*729 + m_Ext.x*37 + m_Ext.y*17 + m_Ext.w;
            return h;
        }

        bool operator == ( const Key& v ) const
        {
            return (m_LOD == v.m_LOD) && (m_Ext == v.m_Ext); 
        }
    }; // struct Key
    
    Key                             m_Key;
    bool                            m_bEmpty;
    int                             m_NObj;
    std::vector<RenderTask>          m_RenderBits;

    TreesBlock() : m_bEmpty(false), m_NObj(0) {}

}; // class TreesBlock

typedef StaticHash<TreesBlock, TreesBlock::Key>    TreesBlockHash;
/*****************************************************************************/
/*    Class:    TreesRenderer
/*    Desc:    
/*****************************************************************************/
class TreesRenderer
{
    TreesBlockHash  m_BlockHash;
    int             m_NTreesRendered;

public:
                    TreesRenderer       ();
    virtual void    RenderBlock         ( const Rct& ext, int LOD );
    virtual int     GetNTreesRendered   () const { return m_NTreesRendered; }
    virtual void    Reset               () { m_NTreesRendered = 0; }

protected:
    bool            CreateTreesBlock    ( TreesBlock& block );
    bool            BakeTreesBillboards ( TreesBlock& block, const TreeObject* trees, int nTrees );
    bool            BakeTreesGeometry   ( TreesBlock& block, const TreeObject* trees, int nTrees );

}; // class TreesRenderer 

const int c_MaxTreesInArea = 16384;
typedef static_array<TreeObject, c_MaxTreesInArea> TreesArray;
/*****************************************************************************/
/*  Class:  ITreesDataBase
/*  Desc:   Interface to the vegetation database
/*****************************************************************************/
class ITreesDataBase
{
public:
    virtual bool GetTreesInArea( const Rct& area, TreesArray& trees, int LOD = 0 ) = 0;
}; // class ITreesDataBase

/*****************************************************************************/
/*  Class:  EcotopeTreesDB
/*  Desc:   Simplistic procedurally trees database, based on the ecotopes
/*****************************************************************************/
class EcotopeTreesDB : public IReflected, public ITreesDataBase
{
public:
                    EcotopeTreesDB();
    virtual bool    GetTreesInArea( const Rct& area, TreesArray& trees, int LOD = 0 );
    virtual void    Expose( PropertyMap& pm );

}; // class ITreesDataBase

extern TreesRenderer*       ITrees;
extern ITreesDataBase*      ITreesDB;

#endif // __VTREESRENDERER_H__
