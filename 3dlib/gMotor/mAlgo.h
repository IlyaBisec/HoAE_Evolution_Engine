/*****************************************************************************/
/*  File:   mAlgo.h
/*  Desc:   Helper algorithms
/*  Author: Ruslan Shestopalyuk
/*  Date:   09-26-2003
/*****************************************************************************/
#ifndef __MALGO_H__
#define __MALGO_H__

template <class TVal, class TNode>
_inl int BinarySearch( const TVal& val, const TNode* pNodes, int nNodes )
{
    if (!pNodes || nNodes == 0) return -1;
    int first = 0, last = nNodes - 1;

    if (!(pNodes[0] < val)) 
    {
        if (pNodes[0] == val) return 0; 
        return -1;
    }
    if (pNodes[last] < val) return last;

    while (last - first > 1)
    {
        int current = (last + first) / 2;
        if (!(pNodes[current] < val)) last = current;
        else first = current;
    }
    return pNodes[last] == val ? last : first;
} // BinarySearch

template <class T>
_inl void Swap( T& a, T& b )
{
    T tmp;
    tmp = a;
    a = b;
    b = tmp;
} // Swap

template <class T>
class LessCmp
{
public:
    _inl static bool Less( const T& a, const T& b )
    {
        return (a < b);
    }
}; // class LessCmp

template < class Cmp, class T >
_inl void Sort( T& a, T& b, T& c )
{
    if (Cmp::Less( b, a )) Swap( a, b );
    if (Cmp::Less( c, b )) Swap( b, c );
    if (Cmp::Less( b, a )) Swap( a, b );
} // Sort

BYTE            QuantizeUpperHemisphereNormal   ( const Vector3D& normal );
const Vector3D& GetUpperHemisphereNormal        ( BYTE idx );

int         GetNumRingPoints    ( int level );
bool        GetRingPoint        ( int level, int idx, int& nx, int& ny );
int         GetMaxRingLevel     ();
    
#endif // __MALGO_H__