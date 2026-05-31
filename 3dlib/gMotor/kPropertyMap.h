/*****************************************************************************/
/*    File:    kPropertyMap.h
/*    Desc:    Code for the string interfacing the class members
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-13-2003
/*    Remark:    Welcome to the Big Ball Of Mud... ;)
/*****************************************************************************/
#ifndef __KPROPERTYMAP_H__
#define __KPROPERTYMAP_H__
#include "IReflected.h"

#include "kSignal.h"
#include "kTypeTraits.h"

/*****************************************************************************/
/*    Class:    ClassMember
/*    Desc:    Base abstract class for the property map member. All concrete  
/*                types (properties, methods, fields etc.) must implement this 
/*                kinda crappy interface
/*****************************************************************************/
class ClassMember
{
public:
    //  property attributes
    bool IsReadonly() const { return m_bReadonly; }
    bool IsDisabled() const { return m_bDisabled; }
    bool IsHidden  () const { return m_bHidden;   }

    //  returns name of the property
    const char*    GetName() const { return m_Name.c_str(); }
    
    //  returns property type (in string format)
    const char*    GetTypeName() const { return m_Type.c_str(); }

    //  gets property value by the string property name
    template <class TRes> bool Get( void* pObject, TRes& res ) { return _Get( pObject, &res ); }

    virtual bool ToString   ( void* pThis, char* buf, int bufSize ) const { return false; }
    virtual bool FromString ( void* pThis, const char* val ) { return false; }
    virtual bool NextValue  ( void* pThis ) { return false; }
    virtual bool PrevValue  ( void* pThis ) { return false; }


    //  sets property value by the string property name
    template <class TVal> bool Set( void* pObject, const TVal& val ) { return _Set( pObject, &val ); }
    bool Call( void* pObject ) { return _Set( pObject ); }
    void Invoke( void* pObject ) { _Set( pObject ); }

protected:
    //  constructor is protected, because only Property map has to manage
    //  instances of the property descriptions
    ClassMember( const char* name ) : 
        m_Name        ( name        ), 
        m_Type        ( ""        ),
        m_bReadonly    ( false        ),
        m_bDisabled    ( false        ),
        m_bHidden    ( false        ) {}
    
    //  this methods concrete member types must implement
    virtual bool _Get( void* pThis, void* pVal ) const = 0;
    virtual bool _Set( void* pThis, const void* pVal = NULL ) = 0;
    
    std::string            m_Name;        //  name of the property
    std::string            m_Type;        //  type name
    std::string            m_Host;        //  host type name

    int                    m_TypeID;    //  unique type identifier
    //  attribute flags
    bool                m_bDisabled;
    bool                m_bHidden;
    bool                m_bReadonly;
    
    friend class PropertyMap;
}; // class ClassMember

/*****************************************************************************/
/*    Class:    Property
/*    Desc:    Property type - imaginary field value which is read 
/*             (and optionally written) through class member getters/setters
/*****************************************************************************/
template <class BaseT, class PropT>
class Property : public ClassMember
{
    typedef PropT (BaseT::*FnGetter) () const;
    typedef void  (BaseT::*FnSetter)( PropT val );

    FnGetter        m_fnGetter;    //  getter member function pointer
    FnSetter        m_fnSetter; //  setter member function pointer

public:
    Property( const char* name, const char* typeName, FnGetter get, FnSetter set = NULL ) : 
        ClassMember( name ), 
        m_fnGetter( get  ), 
        m_fnSetter( set  )
        { 
            m_bReadonly = (set == NULL); 
            if (typeName != NULL) m_Type = typeName;
            else 
            {
                m_Type = TypeTraits<PropT>::TypeName();
            }
            m_TypeID = TypeTraits<PropT>::TypeID();
        }

        virtual bool _Get( void* pThis, void* pVal ) const
        {    
             if (!m_fnGetter || !pThis || !pVal) return false;
            *((PropT*)pVal) = ((reinterpret_cast<BaseT*>( pThis )->*m_fnGetter)());
            return true;
        }

        virtual bool _Set( void* pThis, const void* pVal )
        {    
            if (!m_fnSetter || !pThis || !pVal) return false;
            (reinterpret_cast<BaseT*>( pThis )->*m_fnSetter)( *((const PropT*)pVal) );
            return true;
        }

        virtual bool ToString( void* pThis, char* buf, int bufSize ) const
        {
            PropT tmpVal;
            bool res = _Get( pThis, &tmpVal );
            if (res) res = TypeTraits<PropT>::ToString( tmpVal, buf, bufSize );
            return res;
        }

        virtual bool FromString( void* pThis, const char* val )
        {
            PropT tmpVal;
            bool res = TypeTraits<PropT>::FromString( tmpVal, val );
            if (res) _Set( pThis, &tmpVal );
            return res;
        }

        virtual bool NextValue( void* pThis ) 
        { 
            if (!m_fnSetter || !m_fnGetter || !pThis) return false;
            PropT tmpVal = ((reinterpret_cast<BaseT*>( pThis )->*m_fnGetter)());
            if (!TypeTraits<PropT>::NextValue( tmpVal )) return false;
            (reinterpret_cast<BaseT*>( pThis )->*m_fnSetter)( tmpVal );
            return true; 
        }

        virtual bool PrevValue( void* pThis ) 
        { 
            if (!m_fnSetter || !m_fnGetter || !pThis) return false;
            PropT tmpVal = ((reinterpret_cast<BaseT*>( pThis )->*m_fnGetter)());
            if (!TypeTraits<PropT>::PrevValue( tmpVal )) return false;
            (reinterpret_cast<BaseT*>( pThis )->*m_fnSetter)( tmpVal );
            return true;  
        }

}; // class Property

/*****************************************************************************/
/*    Class:    FunctorMember
/*    Desc:    FunctorMember type - imaginary field value which is read 
/*             (and optionally written) through class member getters/setters
/*****************************************************************************/
template <class FuncT>
class FunctorMember : public ClassMember
{
    FuncT           m_Functor;

public:
    FunctorMember( const char* name, const FuncT& func, const char* typeName = NULL ) : 
      ClassMember( name ), m_Functor( func )
      { 
          if (typeName != NULL) m_Type = typeName;
          else 
          {
              m_Type = TypeTraits<FuncT::PropertyType>::TypeName();
          }
          m_TypeID = TypeTraits<FuncT::PropertyType>::TypeID();
      }

      virtual bool _Get( void* pThis, void* pVal ) const
      {    
          if (!pThis || !pVal) return false;
          return m_Functor.Get( *(FuncT::PropertyType*)pVal );
      }

      virtual bool _Set( void* pThis, const void* pVal )
      {    
          if (!pThis || !pVal) return false;
          return m_Functor.Set( *(FuncT::PropertyType*)pVal );
      }

      virtual bool ToString( void* pThis, char* buf, int bufSize ) const
      {
          FuncT::PropertyType tmpVal;
          bool res = _Get( pThis, &tmpVal );
          if (res) res = TypeTraits<FuncT::PropertyType>::ToString( tmpVal, buf, bufSize );
          return res;
      }

      virtual bool FromString( void* pThis, const char* val )
      {
          FuncT::PropertyType tmpVal;
          bool res = TypeTraits<FuncT::PropertyType>::FromString( tmpVal, val );
          if (res) _Set( pThis, &tmpVal );
          return res;
      }

      virtual bool NextValue( void* pThis ) 
      { 
          if (!pThis) return false;
          FuncT::PropertyType tmpVal;
          bool res = _Get( pThis, &tmpVal );
          if (!res) return false;
          if (!TypeTraits<FuncT::PropertyType>::NextValue( tmpVal )) return false;
          return _Set( pThis, &tmpVal );
      }

      virtual bool PrevValue( void* pThis ) 
      { 
          if (!pThis) return false;
          FuncT::PropertyType tmpVal;
          bool res = _Get( pThis, &tmpVal );
          if (!res) return false;
          if (!TypeTraits<FuncT::PropertyType>::PrevValue( tmpVal )) return false;
          return _Set( pThis, &tmpVal );
      }

}; // class FunctorMember

/*****************************************************************************/
/*    Class:    Method
/*    Desc:    Member function type - nothing takes, nothing returns, just works
/*****************************************************************************/
template <class BaseT>
class Method : public ClassMember
{
    typedef void (BaseT::*FnProcess)();

    FnProcess        m_fnProcess;

public:
    Method( const char* name, FnProcess call ) : 
        ClassMember    ( name ), 
        m_fnProcess    ( call ) 
        {
            m_TypeID = stMethod;
            m_Type = "method";
        }

    virtual bool _Get( void* pThis, void* pVal ) const
    {    
        return false;
    }
    virtual bool _Set( void* pThis, const void* pVal )
    {    
        if (!m_fnProcess) return false;
        (reinterpret_cast<BaseT*>( pThis )->*m_fnProcess)();
        return true;
    }
}; // class Method

struct MethodFunctor
{
    ClassMember*    m_pMember;
    void*           m_pExposed;

    MethodFunctor( void* pExposed, ClassMember* pMember ) : 
    m_pExposed( pExposed ), m_pMember( pMember ) {}
    void operator()() const
    {
        m_pMember->Call( m_pExposed );
    }
}; // struct MethodFunctor

/*****************************************************************************/
/*    Class:    SignalMember
/*    Desc:    Member function type - nothing takes, nothing returns, just works
/*****************************************************************************/
class SignalMember : public ClassMember
{
    int             m_Offset;
public:
    SignalMember( const char* name, void* pBase, Signal& signal );
    virtual bool FromString( void* pThis, const char* val ); 
    virtual bool _Get( void* pThis, void* pVal ) const;
    virtual bool _Set( void* pThis, const void* pVal );
}; // class SignalMember

/*****************************************************************************/
/*    Class:    Field
/*    Desc:    Class field member type with the straight access. 
/*****************************************************************************/
template <class FieldT>
class Field : public ClassMember
{
    int        m_Offset;

public:
    Field( const char* name, const char* typeName, void* pBase, 
            FieldT& field, bool bReadonly = false ) : ClassMember( name )
    {
        assert( pBase );
        m_Offset = (unsigned char*)(&field) - (unsigned char*)pBase;
        m_bReadonly = bReadonly;
        if (typeName != NULL) m_Type = typeName;
        else 
        {
            m_Type = TypeTraits<FieldT>::TypeName();
        }
        m_TypeID = TypeTraits<FieldT>::TypeID();
    }

    virtual bool _Get( void* pThis, void* pVal ) const
    {    
          if (!pThis || !pVal) return false;
          *((FieldT*)pVal) = FieldRef( pThis );
          return true;
    }

    virtual bool _Set( void* pThis, const void* pVal )
    {    
        if (m_bReadonly || !pVal || !pThis) return false;
        FieldRef( pThis ) = *((const FieldT*)pVal);
        return true;
    }

    virtual bool ToString( void* pThis, char* buf, int bufSize ) const
    {
        if (!pThis) return false;
        return TypeTraits<FieldT>::ToString( FieldRef( pThis ), buf, bufSize );
    }

    virtual bool FromString( void* pThis, const char* val )
    {
        if (m_bReadonly || !pThis) return false;
        return TypeTraits<FieldT>::FromString( FieldRef( pThis ), val );
    }
    
    virtual bool NextValue( void* pThis ) 
    { 
        if (m_bReadonly || !pThis) return false;
        return TypeTraits<FieldT>::NextValue( FieldRef( pThis ) );
    }

    virtual bool PrevValue( void* pThis ) 
    { 
        if (m_bReadonly || !pThis) return false;
        return TypeTraits<FieldT>::PrevValue( FieldRef( pThis ) );
    }

protected:
    FieldT&    FieldRef( void* pThis )
    {
        return *((FieldT*)((unsigned char*)pThis + m_Offset));
    }

    const FieldT& FieldRef( void* pThis ) const 
    {
        return *((FieldT*)((unsigned char*)pThis + m_Offset));
    }
}; // class Field

/*****************************************************************************/
/*    Class:    PropertyMap
/*    Desc:    Exposes object properties through string interface
/*****************************************************************************/
class PropertyMap
{
    void*                            m_pObject;        //  pointer to the object we are mapping to
    std::string                        m_ClassName;    //  name of the mapped object's class
    std::vector<ClassMember*>        m_Members;        //  class members

public:
                    PropertyMap() : m_pObject(NULL) {}
                    ~PropertyMap() {}
    void            SetObject( void* pObject ) { m_pObject = pObject; }
    void*           GetObject() const { return m_pObject; }
    int             GetNMembers() const { return m_Members.size(); }
    ClassMember*    GetMember( int idx ) { if (idx < 0 || idx >= m_Members.size()) return NULL; return m_Members[idx]; }
    void            Clear() 
    { 
        for (int i = 0; i < m_Members.size(); i++) delete m_Members[i]; 
        m_Members.clear(); 
    }

    //  adding field
    template <class FieldT> void f( const char* name, FieldT& field, 
        const char* typeName = NULL, bool readonly = false )
    {
        m_Members.push_back( new Field<FieldT>( name, typeName, m_pObject, field, readonly ) );
    }

    //  adding property
    template <class BaseT, class PropT>
        void p( const char* name, 
                PropT (BaseT::*get)() const, 
                void  (BaseT::*set)( PropT val ) = NULL, 
                const char* typeName = NULL )
    {
        m_Members.push_back( new Property<BaseT, PropT>( name, typeName, get, set ) );
    }

    //  adding method
    template <class BaseT> void m( const char* name, void (BaseT::*call)() )
    {
        m_Members.push_back( new Method<BaseT>( name, call ) );
    }

    //  adding functor
    template <class TFunctor> void r( const char* name, const TFunctor& fr, const char* typeName = NULL )
    {
        m_Members.push_back( new FunctorMember<TFunctor>( name, fr, typeName ) );
    }

    //  adding signal
    void s( const char* name, Signal& signal )
    {
        m_Members.push_back( new SignalMember( name, m_pObject, signal ) );
    }

    //  get value of the property/field or method result
    template <class TRes> bool get( const char* propName, TRes& res )
    {
        if (m_pObject == NULL) return false; 
        ClassMember* pProp = FindByName( propName );
        if (!pProp) return false;
        return pProp->Get( m_pObject, res );
    }

    //  get string representation of the member value
    bool get( const char* propName, char* buf, int bufSize )
    {
        if (m_pObject == NULL) return false; 
        ClassMember* pProp = FindByName( propName );
        if (!pProp) return false;
        return pProp->ToString( m_pObject, buf, bufSize );
    }

    //  get value of the property/field or method input parameter
    template <class TVal> bool set( const char* propName, const TVal& val )
    {
        if (m_pObject == NULL) return false; 
        ClassMember* pProp = FindByName( propName );
        if (!pProp) return false;
        return pProp->Set( m_pObject, val );
    }

    bool set( const char* propName, const char* val )
    {
        if (m_pObject == NULL) return false; 
        ClassMember* pProp = FindByName( propName );
        if (!pProp) return false;
        return pProp->FromString( m_pObject, val );
    }
    
    template <class TBase, class T> void start( const char* className, T* pObject, DWORD flags = 0 )
    {
        pObject->TBase::Expose( *this );
        SetObject( pObject );
        m_ClassName = std::string( className );
    }

    template <class T> void start( const char* className, T* pObject, DWORD flags = 0 )
    {
        SetObject( pObject );
        m_ClassName = std::string( className );
    }

    //  runs method with given name
    bool run( const char* propName )
    {
        if (m_pObject == NULL) return false; 
        ClassMember* pProp = FindByName( propName );
        if (!pProp) return false;
        return pProp->Call( m_pObject );
    }

    ClassMember* FindByName( const char* name )
    {
        for (int j = 0; j < m_Members.size(); j++)
        {
            if (!stricmp( m_Members[j]->GetName(), name )) return m_Members[j];
        }
        return NULL;
    }

}; // class PropertyMap

#endif // __KPROPERTYMAP_H__
