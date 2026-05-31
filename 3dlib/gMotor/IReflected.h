/*****************************************************************************/
/*    File:    IReflected.h
/*    Desc:    Interface for type reflection
/*    Author:    Ruslan Shestopalyuk
/*    Date:    29.10.2003
/*****************************************************************************/
#ifndef __IREFLECTED_H__
#define __IREFLECTED_H__

#include "kTemplates.hpp"

class PropertyMap;
/*****************************************************************************/
/*    Class:    IReflected
/*    Desc:    Interface which exposed classes should implement
/*****************************************************************************/
class IReflected
{
public:
    virtual void            Expose      ( PropertyMap& pm ) = 0;
    virtual const char*     ClassName   () const { return "Unknown"; }
    virtual DWORD           ClassID     () const { return 0; }
    virtual const char*     GetName     () const { return "Unknown"; }
    virtual DWORD           GetColor    () const { return 0xFFFFFFFF; }
    virtual int             GetGlyphID  () const { return 0; }

    virtual IReflected*     Parent      () const                        { return NULL;  }
    virtual int             NumChildren () const                        { return 0;     }
    virtual IReflected*     Child       ( int idx ) const               { return NULL;  }
    virtual bool            AddChild    ( IReflected* pChild )          { return false; }
    virtual IReflected*     Clone       () const { return NULL; }

    virtual int             ChildIdx    ( IReflected* pChild ) const    { return -1;  }
    virtual bool            SetChild    ( int idx, IReflected* pChild ) { return false; }
    virtual bool            DelChild    ( int idx )            { return false; }

    bool DelChild( IReflected* pNode )  
    { 
        int nCh = NumChildren();
        for (int i = nCh - 1; i >= 0; i--)
        {
            if (pNode == Child( i )) 
            {
                return DelChild( i );
            }
        }
        return false; 
    } // DelChild

    bool ReleaseChildren()  
    { 
        int nCh = NumChildren();
        bool ok = true;
        for (int i = nCh - 1; i >= 0; i--)
        {
            ok &= DelChild( i );
        }
        return ok; 
    } // ReleaseChildren

    typedef TreeIterator<IReflected>  Iterator;
    class NameFilter
    {
        typedef bool (*FilterCallback)( const IReflected* pNode );
        static const int c_MaxNodeNameLen = 256;
    public:
        NameFilter( const char* name )
        {
            assert( strlen( name ) < c_MaxNodeNameLen );
            if (name) strcpy( m_Name, name ); m_Name[0] = 0;
        }
        operator FilterCallback() const { return filter; }

    protected:
        static char m_Name[c_MaxNodeNameLen];

        static bool filter( const IReflected* pNode )
        {
            if (!pNode) return false;
            if (!strcmp( pNode->GetName(), m_Name )) return true;
            return false;
        }
    }; // class NameFilter

}; // class IReflected

#endif // __IREFLECTED_H__