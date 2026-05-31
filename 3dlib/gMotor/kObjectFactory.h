/*****************************************************************************/
/*    File:    kObjectFactory.h
/*    Desc:    Interface for type reflection
/*    Author:    Ruslan Shestopalyuk
/*    Date:    29.10.2003
/*****************************************************************************/
#ifndef __KOBJECTFACTORY_H__
#define __KOBJECTFACTORY_H__

#include "IReflected.h"
#include "kPropertyMap.h"

class XMLNode;
/*****************************************************************************/
/*    Class:    ObjectFactory, singleton
/*    Desc:    Responsible for registering object types, reflection of the 
/*              object properties, creation objects by its type,
/*****************************************************************************/    
class ObjectFactory
{
public:
    typedef IReflected*      (*CreateCallback)();

    ///  registers widget type 
    bool                    RegClass        ( const char* cName, CreateCallback creator );
    ///  creates widget from the script file
    IReflected*             CreateFromScript( const char* fname, IReflected* pRoot = NULL );
    bool                    SaveToScript    ( const char* fname, IReflected* pRoot );
    IReflected*             GetClassPattern (  const char* cName ) const;

    ///  creates widget object by class name
    IReflected*             Create          ( const char* cName )
    {
        CreateCallback creator = GetCreator( cName );
        if (creator) return creator();
        return NULL;
    }

    IReflected*             Create          ( DWORD id )
    {
        CreateCallback creator = GetCreator( id );
        if (creator) return creator();
        return NULL;
    }

    PropertyMap*            GetPropertyMap  ( const char* className );

    static ObjectFactory&   instance();
    
private:
    struct ClassRec
    {
        const char*         m_Name;         //  name of the class
        DWORD               m_ID;           //  FOURCC for this class
        CreateCallback      m_Creator;      //  creator callback function
        PropertyMap         m_Properties;   //  class property map
        IReflected*         m_pPattern;     //  instance of the class object

        ClassRec() : m_Name(""), m_Creator(0), m_pPattern(NULL), m_ID(0) {}
    };

    typedef std::vector<ClassRec> ClassReg; 

    ClassReg                m_ClassReg;
    CreateCallback          GetCreator      ( const char* cName );
    CreateCallback          GetCreator      ( DWORD id );
    const char*             GetClassName    ( DWORD id );
    bool                    Create          ( IReflected* pObject, const XMLNode& root );

    ObjectFactory   ();
    ~ObjectFactory  ();
}; // class ObjectFactory

bool LocateProperty( IReflected* pStart, const char* propName, IReflected* &pEnd, ClassMember* &pMem );

#define DECLARE_CLASS(CName) virtual const char*     ClassName() const { return #CName; }\
                             static IReflected*      CreateInstance() { return new CName; }

#define DECLARE_SCLASS(CName,PName,Magic)    typedef PName Parent; \
                             static bool             FnFilter        ( const IReflected* pObj ) { return (dynamic_cast<const CName*>( pObj ) != NULL);} \
                             static IReflected*      CreateInstance  () { return new CName; } \
                             virtual DWORD           ClassID         () const { return *((DWORD*)(#Magic)); } \
                             virtual const char*     ClassName       () const { return #CName; } 

#define IMPLEMENT_CLASS(CName) bool __##CName##Registrator() { return ObjectFactory::instance().RegClass( #CName, CName::CreateInstance ); }
#define LINK_CLASS(CName) bool __##CName##Registrator(); printf( "%d", __##CName##Registrator() );

#endif // __KOBJECTFACTORY_H__