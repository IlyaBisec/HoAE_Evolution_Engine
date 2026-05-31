/*****************************************************************************/
/*    File:    kPropertyMap.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-22-2003
/*****************************************************************************/
#include "stdafx.h"
#include "kObjectFactory.h"
#include "kPropertyMap.h"

/*****************************************************************************/
/*  SignalMember implementation
/*****************************************************************************/
SignalMember::SignalMember( const char* name, void* pBase, Signal& signal ) : ClassMember( name ) 
{
    m_TypeID  = stSignal;
    m_Type    = "signal";
    m_Offset  = (unsigned char*)(&signal) - (unsigned char*)pBase;
}

bool SignalMember::FromString( void* pThis, const char* val ) 
{ 
    Signal* pSignal = (Signal*)(((unsigned char*)pThis) + m_Offset);
    if (!pSignal) return false;
    pSignal->DisconnectAll();
    
    IReflected* pRef  = NULL;
    ClassMember* pMem = NULL;
    IReflected* pDst  = (IReflected*)pThis;
    if (LocateProperty( pDst, val, pRef, pMem ))
    {
        MethodFunctor func( pRef, pMem );
        pSignal->Connect( func );
        return true;
    }
    return false; 
} // SignalMember::FromString

bool SignalMember::_Get( void* pThis, void* pVal ) const
{    
    return false;
}

bool SignalMember::_Set( void* pThis, const void* pVal )
{    
    Signal* pSignal = (Signal*)(((unsigned char*)pThis) + m_Offset);
    if (!pSignal) return false;
    (*pSignal)();
    return true;
}