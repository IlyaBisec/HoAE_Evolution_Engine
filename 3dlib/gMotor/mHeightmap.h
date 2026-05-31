/*****************************************************************************/
/*    File:    mHeightmap.h
/*    Desc:    Grid of height values
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11-12-2003
/*****************************************************************************/
#ifndef __MHEIGHTMAP_H__
#define __MHEIGHTMAP_H__

/*********************************************************************************************/
/*  Class:    Lattice
/*  Desc:    General 2D array
/*********************************************************************************************/
template <class TVal> class Lattice
{
    TVal*                m_Val;                //  heights array
    TVal                m_DefaultValue;
    int                    m_SideNodes;        //  number of side nodes
public:
    Lattice() : m_Val(NULL), m_SideNodes(0) {}
    ~Lattice()
    {
        delete []m_Val;
        m_Val = NULL;
        m_SideNodes = 0;
    }
    
    TVal*    GetArrayPtr() { return m_Val; }
    int        GetNSideNodes() const { return m_SideNodes; }
    void    SetDefaultValue( const TVal& val ) { m_DefaultValue = val; }
    TVal    GetDefaultValue() const { return m_DefaultValue; }

    void SetNSideNodes( int nNodes )
    {
        int hMapSz = GetSize();
        m_SideNodes = nNodes;

        int newHMapSz = m_SideNodes * m_SideNodes;
        if (newHMapSz != hMapSz)
        {
            delete []m_Val;
            m_Val = new TVal [newHMapSz];
            memset( m_Val, 0, newHMapSz * sizeof( TVal ) );
        }
    }

    TVal* GetNodePos( int nx, int ny )
    {
        assert( m_Val && nx < m_SideNodes && ny < m_SideNodes );
        return &m_Val[nx + ny * m_SideNodes];
    }

    int    GetSize() const 
    { 
        return m_SideNodes * m_SideNodes; 
    }

    void Unserialize( InStream& is )
    {
        int nSideNodes = 0;
        is >> nSideNodes;
        SetNSideNodes( nSideNodes );
        is.Read( m_Val, GetSize() * sizeof( m_Val ) );
    }

    void Serialize( OutStream& os ) const
    {
        os << m_SideNodes;
        os.Write( m_Val, GetSize() * sizeof( m_Val ) );
    }
    
    const TVal& GetValue( int x, int y ) const
    {
        if (!m_Val ) return m_DefaultValue;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x >= m_SideNodes) 
        {
            x = m_SideNodes - 1;
        }
        if (y >= m_SideNodes) 
        {
            y = m_SideNodes - 1;
        }
        return m_Val[x + y * m_SideNodes];
    }

    TVal& operator()( int x, int y ) const
    {
        return GetValue( x, y );
    }

    void SetValue( int idx, const TVal& val )
    {
        if (!m_Val) return;
        if (idx < 0 || idx >= GetSize()) return;
        m_Val[idx] = val;
    }

    TVal GetValue( int idx )
    {
        if (!m_Val ) return m_DefaultValue;
        if  (idx < 0 || idx >= GetSize()) return m_DefaultValue;
        return m_Val[idx];
    }

    void SetValue( const TVal& val )
    {
        int sz = GetSize();
        for (int i = 0; i < sz; i++) m_Val[i] = val;
    }

    bool SetValue( int x, int y, const TVal& val )
    {
        if (!m_Val) return false;
        if (x < 0 || x >= m_SideNodes || y < 0 || y >= m_SideNodes) return false;
        m_Val[x + y * m_SideNodes] = val;
        return true;
    }

    bool IsOutside( int x, int y ) const
    {
        return (x < 0) || (y < 0) || (x >= m_SideNodes) || (y >= m_SideNodes); 
    }

    void Swap( Lattice& lattice )
    {
        assert( lattice.GetSize() == GetSize() );
        ::Swap( m_Val, lattice.m_Val );
    }

}; // class Lattice

template <class TVal>
InStream& operator >>( InStream& is, Lattice<TVal>& hmap )
{
    hmap.Unserialize( is );
    return is;
} // operator >>

template <class TVal>
OutStream& operator <<( OutStream& os, const  Lattice<TVal>& hmap )
{
    hmap.Serialize( os );
    return os;
} // operator <<

/*********************************************************************************************/
/*  Class:    HeightMap
/*  Desc:    Grid of height values
/*********************************************************************************************/
class HeightMap : public Lattice<float>
{
    float                m_GridStep;            //  step of the grid in the world space
    AABoundBox            m_AABB;                //  bounding box in the world space

public:
    void                DisturbPerlin        ( float dx, float dy, float sx, float sy, float dh, float hs );
    _inl float            GetHeight            ( float x, float y    ) const;
    _inl float            GetHeight            ( int x, int y ) const;

    _inl void            SetHeight            ( int x, int y, float val );
    _inl void            SetHeight            ( float x, float y, float val );

    void                SetHeight            ( float val    );
    void                Scale                ( float val    );

    _inl void            SetGridStep            ( float step ) { m_GridStep = step; }
    _inl float            GetGridStep            () const { return m_GridStep; }
    _inl void            GetNormal            ( float x, float y, Vector3D& normal ) const;
    _inl Vector3D        GetNormal            ( int x, int y ) const;
    _inl Vector3D        GetNormal            ( float x, float y ) const;
    _inl Vector3D       GetNormal           ( float x, float y, float radius ) const;
    _inl void            ToWorldSpace        ( float& x, float& y ) const;
    _inl bool            LocateNode            ( float x, float y, int& nx, int& ny ) const;
    bool                Pick                ( const Ray3D& ray, Vector3D& pt, const Rct* pArea = NULL );
    XStatus                Intersect            ( const Sphere& sphere,        const Rct* pArea = NULL );
    XStatus                Intersect            ( const AABoundBox& aabb,    const Rct* pArea = NULL );
    XStatus                Intersect            ( const Segment3D& seg,        const Rct* pArea = NULL );

    const AABoundBox&    CalculateAABB        ();
    const AABoundBox&    GetAABB                () const { return m_AABB; }
    void                SetAABB                ( const AABoundBox& aabb ) { m_AABB = aabb; }

    friend InStream&    operator >>            ( InStream& is, HeightMap& hmap );
    friend OutStream&    operator <<            ( OutStream& os, const HeightMap& hmap );
protected:

}; // class HeightMap

InStream&    operator >> ( InStream& is,            HeightMap& hmap );
OutStream&    operator << ( OutStream& os, const  HeightMap& hmap );

#ifdef _INLINES
#include "mHeightmap.inl"
#endif // !_INLINES

#endif // __MHEIGHTMAP_H__
