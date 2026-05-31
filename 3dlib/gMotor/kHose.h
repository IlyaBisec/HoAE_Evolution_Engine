/*****************************************************************************/
/*    File:    kHose.h
/*    Desc:     
/*    Author:    Ruslan Shestopalyuk
/*    Date:    01-02-2005
/*****************************************************************************/
#ifndef __KHOSE_H__
#define __KHOSE_H__

#include "IReflected.h"
#include "kTypeTraits.h"
#include "kPropertyMap.h"
#include "kHash.hpp"
#include "kStaticArray.hpp"

const int c_MaxHoseStrlen   = 256;
const int c_MaxHoses        = 1024;
/*****************************************************************************/
/*    Class:    Hose
/*    Desc:    Fat, but useful one-direction connection between two exposed 
/*              properties of reflectable objects
/*****************************************************************************/
class Hose
{
    ClassMember*    m_pSrcProp;                 //  source property description
    ClassMember*    m_pDstProp;                 //  destination property description

    IReflected*     m_pSrcObj;                  //  source object
    IReflected*     m_pDstObj;                  //  destination object

    char            m_Value[c_MaxHoseStrlen];   //  last value

protected:
    Hose*           m_pNextIn;                  //  next in linked list of inputs  
    Hose*           m_pNextOut;                 //  next in linked list of outputs   
    
    friend class    HoseDepot;
    friend class    HosedObject;

public:
    Hose() :    m_pSrcProp(NULL), m_pDstProp(NULL), 
                m_pSrcObj (NULL), m_pDstObj (NULL), 
                m_pNextIn (NULL), m_pNextOut(NULL) { m_Value[0] = 0; }

    void        Disconnect() {  m_pSrcProp = NULL; m_pDstProp = NULL; 
                                m_pSrcObj  = NULL; m_pDstObj  = NULL;
                                m_pNextIn  = NULL; m_pNextOut = NULL;
                                m_Value[0] = 0;
                             }
    bool        IsConnected() const { return (m_pSrcProp != NULL); }
    
    Hose( IReflected* pSrcObj, ClassMember* pSrcProp, IReflected* pDstObj, ClassMember* pDstProp ) 
        :   m_pSrcObj   (pSrcObj), 
            m_pSrcProp  (pSrcProp), 
            m_pDstObj   (pDstObj), 
            m_pDstProp  (pDstProp), 
            m_pNextIn   (NULL), 
            m_pNextOut  (NULL) { m_Value[0] = 0; }

    bool operator ==( const Hose& hose ) const
    {
        return  (m_pSrcProp == hose.m_pSrcProp) &&
                (m_pDstProp == hose.m_pDstProp) &&
                (m_pSrcObj == hose.m_pSrcObj) &&
                (m_pDstObj == hose.m_pDstObj);
    }

    bool Update()
    {
        if (!m_pSrcProp || !m_pDstProp || !m_pSrcObj || !m_pDstObj) return false;
        char buf[c_MaxHoseStrlen];
        //  get from source new string representation
        if (!m_pSrcProp->ToString( m_pSrcObj, buf, c_MaxHoseStrlen )) return false;
        //  check whether representation was changed from last update
        if (!strnicmp( buf, m_Value, c_MaxHoseStrlen )) return true;
        //  write to the destination
        if (!m_pDstProp->FromString( m_pDstObj, buf )) return false;
        strncpy( m_Value, buf, c_MaxHoseStrlen );
        return true;
    }
}; // class Hose

/*****************************************************************************/
/*    Class:    HoseKey
/*    Desc:    Used for hashing pointers-to-object
/*****************************************************************************/
struct HoseKey
{
    IReflected*     m_pObj;         // pointer to the hosed reflectable object

                    HoseKey() : m_pObj( NULL ) {}
                    HoseKey( IReflected* pObj ) : m_pObj( pObj ) {}

    unsigned int    hash    () const
    {
        DWORD h = 541*DWORD( m_pObj );
        h = (h << 13) ^ h;
        h += 15731;
        return h;
    }
    bool            operator ==( const HoseKey& el ) const { return (m_pObj == el.m_pObj); }
    void            copy    ( const HoseKey& el ) { m_pObj = el.m_pObj; }
}; // struct HoseKey

/*****************************************************************************/
/*    Class:    HosedObject
/*    Desc:    Representation of the hosed object in the hose depot
/*****************************************************************************/
struct HosedObject
{
    IReflected*         m_pObject;      //  pointer to the hosed object
    Hose*               m_pInputs;      //  head of the input hoses linked list
    Hose*               m_pOutputs;     //  head of the output hoses linked list

    HosedObject() : m_pObject(NULL), m_pInputs(NULL), m_pOutputs(NULL) {}
    void Reset() { m_pObject = NULL; m_pInputs = NULL; m_pOutputs = NULL; }

    Hose* FindHose( const Hose& hose ) const
    {
        Hose* pRes = m_pInputs;
        while (pRes)
        {
            if (*pRes == hose) return pRes;
            pRes = pRes->m_pNextIn;
        }
        pRes = m_pOutputs;
        while (pRes)
        {
            if (*pRes == hose) return pRes;
            pRes = pRes->m_pNextOut;
        }
        return NULL;
    } // FindHose

    bool AddOutput( Hose& hose )
    {
        hose.m_pNextOut = m_pOutputs;
        m_pOutputs = &hose;
        return true;
    }

    bool AddInput( Hose& hose )
    { 
        hose.m_pNextIn = m_pInputs;
        m_pInputs = &hose;
        return true;
    }

    bool DelInput( Hose* pHose )
    {
        if (!pHose) return false;
        if (m_pInputs == pHose)
        {
            m_pInputs = pHose->m_pNextIn;
            pHose->m_pNextIn = NULL;
            return true;
        }
        Hose* cHose = m_pInputs;
        while (cHose)
        {
            if (cHose->m_pNextIn == pHose)
            {
                cHose->m_pNextIn = pHose->m_pNextIn;
                pHose->m_pNextIn = NULL;
                return true;
            }
            cHose = cHose->m_pNextIn;
        }
        return false;
    } // DelInput

    bool DelOutput( Hose* pHose )
    {
        if (!pHose) return false;
        if (m_pOutputs == pHose)
        {
            m_pOutputs = pHose->m_pNextOut;
            pHose->m_pNextOut = NULL;
            return true;
        }
        Hose* cHose = m_pOutputs;
        while (cHose)
        {
            if (cHose->m_pNextOut == pHose)
            {
                cHose->m_pNextOut = pHose->m_pNextOut;
                pHose->m_pNextOut = NULL;
                return true;
            }
            cHose = cHose->m_pNextOut;
        }
        return false; 
    } // DelOutput

}; // struct HosedObject

typedef StaticHash<HosedObject, HoseKey>   HoseHash;
/*****************************************************************************/
/*    Class:    HoseDepot
/*    Desc:    Manager of the objects' hoses
/*****************************************************************************/
class HoseDepot
{
    HoseHash                        m_ObjReg;  //  registry of the currently alive and hosed objects
    static_array<Hose, c_MaxHoses>  m_HosePool;

    HoseDepot(){}
public:
    
    bool AddHose(   IReflected* pSrcObj, const char* srcProp, 
                    IReflected* pDstObj, const char* dstProp )
    {
        PropertyMap* spm = ObjectFactory::instance().GetPropertyMap( pSrcObj->ClassName() ); 
        PropertyMap* dpm = ObjectFactory::instance().GetPropertyMap( pDstObj->ClassName() ); 
        if (!spm || !dpm) return false;
        
        ClassMember* pSMem = spm->FindByName( srcProp );
        ClassMember* pDMem = dpm->FindByName( dstProp );
        if (!pSMem || !pDMem) return false;

        return AddHose( pSrcObj, pSMem, pDstObj, pDMem );
    } // AddHose

    bool AddHose(   IReflected* pSrcObj, ClassMember* pSMem, 
                    IReflected* pDstObj, ClassMember* pDMem )
    {
        if (!pSMem || !pDMem) return false;

        int sID = m_ObjReg.add( HoseKey( pSrcObj ) );
        int dID = m_ObjReg.add( HoseKey( pDstObj ) );
        HosedObject& sObj = m_ObjReg.elem( sID );
        HosedObject& dObj = m_ObjReg.elem( dID );

        //  search if there is already such connection
        Hose h( pSrcObj, pSMem, pDstObj, pDMem );
        if (sObj.FindHose( h )) return false;
        if (dObj.FindHose( h )) return false;
        Hose& hose = AllocateHose();
        hose = h;
        sObj.AddOutput( hose );
        dObj.AddInput ( hose );
        return true;
    } // AddHose

    Hose& AllocateHose()
    {
        int nH = m_HosePool.size();
        for (int i = 0; i < nH; i++) if (!m_HosePool[i].IsConnected()) return m_HosePool[i];
        return m_HosePool.push_back( Hose() );
    }

    bool Update( IReflected* pObj )
    {
        int elID = m_ObjReg.find( HoseKey( pObj ) );
        if (elID == NO_ELEMENT) return false;
        const HosedObject& hobj = m_ObjReg.elem( elID );
        Hose* pHose = hobj.m_pInputs;
        while (pHose)
        {
            pHose->Update();
            pHose = pHose->m_pNextIn;
        }
        
        pHose = hobj.m_pOutputs;
        while (pHose)
        {
            pHose->Update();
            pHose = pHose->m_pNextOut;
        }
        return true;
    } // Update

    bool UnregisterObject( IReflected* pObj )
    {
        int elID = m_ObjReg.find( HoseKey( pObj ) );
        if (elID == NO_ELEMENT) return false;
        HosedObject& hObj = m_ObjReg.elem( elID );
        //  clear all input connections
        Hose* pHose = hObj.m_pInputs;
        while (pHose)
        {
            int outID = m_ObjReg.find( HoseKey( pHose->m_pSrcObj ) );
            m_ObjReg.elem( outID ).DelOutput( pHose );
            pHose->Disconnect();
            pHose = pHose->m_pNextIn;
        }
        //  clear all output connections
        pHose = hObj.m_pOutputs;
        while (pHose)
        {
            int inID = m_ObjReg.find( HoseKey( pHose->m_pDstObj ) );
            m_ObjReg.elem( inID ).DelInput( pHose );
            pHose->Disconnect();
            pHose = pHose->m_pNextOut;
        }
        //  remove from hosed objects registry
        hObj.Reset();
        m_ObjReg.delElem( elID );
        return true;
    } // UnregisterObject

    static HoseDepot& instance()
    {
        static HoseDepot me;
        return me;
    }
    
}; // class HoseDepot

#endif // __KHOSE_H__
