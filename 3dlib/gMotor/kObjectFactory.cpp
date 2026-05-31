/*****************************************************************************/
/*    File:    kObjectFactory.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    29.10.2003
/*****************************************************************************/
#include "stdafx.h"
#include "IResourceManager.h"

#include "kFilePath.h"
#include "kObjectFactory.h"
#include "kHose.h"

bool LocateProperty( IReflected* pStart, const char* propName, 
                        IReflected* &pEnd, ClassMember* &pMem )
{
    IReflected* pRef = pStart;
    IReflected* pPrevRef = NULL;
    while (pRef)
    {
        IReflected::Iterator it( pRef );
        while (it)
        {
            IReflected* pChildRef = *it;
            if (pChildRef == pPrevRef) { ++it; continue; }
            PropertyMap* pm = ObjectFactory::instance().GetPropertyMap( pChildRef->ClassName() );                    
            if (!pm) { ++it; continue; }
            ClassMember* cm = pm->FindByName( propName );
            if (cm)
            {
                pEnd = pRef;
                pMem = cm;
                return true;
            }
            ++it;
        }
        pPrevRef = pRef;
        pRef = pRef->Parent();
    }
    return false;
} // LocateProperty

/*****************************************************************************/
/*    ObjectFactory implementation
/*****************************************************************************/
ObjectFactory::ObjectFactory()
{
}

ObjectFactory::~ObjectFactory()
{
    for (int i = 0; i < m_ClassReg.size(); i++) 
    {
        m_ClassReg[i].m_Properties.Clear();
        //delete m_ClassReg[i].m_pPattern;
    }
} // ObjectFactory::~ObjectFactory

ObjectFactory& ObjectFactory::instance()
{
    static ObjectFactory me;
    return me;
}

IReflected* ObjectFactory::CreateFromScript( const char* fname, IReflected* pRoot )
{
    int id = GetResourceManager()->FindResource( fname );
    if (id == -1) return NULL;
    int size = 0;
    char* pData = (char*)IRM->LockData( id, size );
    if (!pData) return NULL;    
    XMLNode root( pData );
    if (!pRoot) pRoot = Create( root.GetTag() );
    if (!pRoot) 
    {
        Log.Error( "Could not create object from script: %s", fname );
        return NULL;
    }
    
    //  first pass creates structure and fills in properties
    Create( pRoot, root );
    //  second pass maps signals to slots (which are unmapped at first pass)
    Create( pRoot, root );

    IResource* pRes = dynamic_cast<IResource*>( pRoot );
    if (pRes)
    {
        IRM->BindResource( id, pRes );
    }
    return pRoot;
} // ObjectFactory::CreateFromScript

bool ObjectFactory::Create( IReflected* pObject, const XMLNode& root )
{
    if (!pObject) return false;
    int cIdx = -1;
    for (int i = 0; i < m_ClassReg.size(); i++)
    {
        if (!stricmp( root.GetTag(), m_ClassReg[i].m_Name)) { cIdx = i; break;}
    }
    if (cIdx == -1) return false;
    ClassRec& cr = m_ClassReg[cIdx];
    if (!cr.m_Creator) return false;
    //  retrieve attributes
    PropertyMap& pm = cr.m_Properties;
    for (int i = 0; i < pm.GetNMembers(); i++)
    {
        ClassMember* pMem = pm.GetMember( i );
        XMLNode* pAttr = root.FindAttr( pMem->GetName() );  
        if (pAttr)
        {
            pMem->FromString( pObject, pAttr->GetValue() );
        }        
    }

    //  check for hose references
    const XMLNode* pAttr = root.FirstAttr();
    while (pAttr)
    {
        const char* pName = pAttr->GetValue();
        const char* pTag  = pAttr->GetTag();
        pAttr = pAttr->NextSibling();
        if (pName[0] != '<' || pName[1] != '=' ) continue;
        pName += 2;
        bool bTwoSide = false;
        if (pName[0] == '>') 
        { 
            bTwoSide = true; 
            pName++;
        }
        ClassMember* pMem = pm.FindByName( pTag );
        if (!pMem) continue;
        IReflected* pSrcObj = NULL;
        ClassMember* pSrcMem = NULL;
        bool bRes = LocateProperty( pObject, pName, pSrcObj, pSrcMem );
        if (!bRes || !pSrcObj || !pSrcMem) continue;
        HoseDepot::instance().AddHose( pSrcObj, pSrcMem, pObject, pMem );
        if (bTwoSide) HoseDepot::instance().AddHose( pObject, pMem, pSrcObj, pSrcMem );
    }

    //  create child objects
    IReflected* pTN = dynamic_cast<IReflected*>( pObject );
    if (pTN)
    {
        int nCh = root.GetNChildren();
        const XMLNode* pChild = root.FirstChild();
        for (int i = 0; i < nCh; i++)
        {
            IReflected* pChNode = NULL;
            XMLNode* pName = pChild->FindAttr( "name" );
            int nCh = pTN->NumChildren();
            if (pName) 
            {
                for (int i = 0; i < nCh; i++)
                {
                    if (!stricmp( pName->GetValue(), pTN->Child( i )->GetName() ))
                    {
                        pChNode = pTN->Child( i );
                        break;
                    }
                }
            }
            bool bNewChild = (pChNode == NULL);
            if (!pChNode) pChNode = (IReflected*)Create( pChild->GetTag() );
            
            if (bNewChild) pTN->AddChild( pChNode );
            Create( (IReflected*)pChNode, *pChild );
            pChild = pChild->NextSibling();
        }
    }
    return true;
} // ObjectFactory::Create

bool ObjectFactory::SaveToScript( const char* fname, IReflected* pRoot )
{
       
    return false;
}

ObjectFactory::CreateCallback ObjectFactory::GetCreator( const char* cName )
{
    for (int i = 0; i < m_ClassReg.size(); i++)
    {
        if (!stricmp( cName, m_ClassReg[i].m_Name)) return m_ClassReg[i].m_Creator;
    }
    return NULL;
} // ObjectFactory::GetCreator

IReflected* ObjectFactory::GetClassPattern(  const char* cName ) const
{
    for (int i = 0; i < m_ClassReg.size(); i++)
    {
        if (!stricmp( cName, m_ClassReg[i].m_Name)) return m_ClassReg[i].m_pPattern;
    }
    return NULL;
} // ObjectFactory::GetClassPattern

const char* ObjectFactory::GetClassName( DWORD id )
{
    for (int i = 0; i < m_ClassReg.size(); i++)
    {
        if (m_ClassReg[i].m_ID == id) return m_ClassReg[i].m_Name;
    }
    return NULL;
} // ObjectFactory::GetClassName

ObjectFactory::CreateCallback ObjectFactory::GetCreator( DWORD id )
{
    for (int i = 0; i < m_ClassReg.size(); i++)
    {
        if (m_ClassReg[i].m_ID == id) return m_ClassReg[i].m_Creator;
    }
    return NULL;
} // ObjectFactory::GetCreator

bool ObjectFactory::RegClass( const char* cName, ObjectFactory::CreateCallback creator )
{
    if (!creator) return false;
    if (GetCreator( cName ))
    { 
        Log.Error( "Class %s has already been registered.", cName );
        return false;
    }
    IReflected* pPattern = creator();
    assert( pPattern );
    DWORD id = pPattern->ClassID();
    if (GetCreator( id ) && id != 0)
    {
        Log.Error( "Class ID for class %s has been already used by class: %s", 
                    cName, GetClassName( id ) ); 
        return false;
    }

    m_ClassReg.push_back( ClassRec() );
    ClassRec& rec   = m_ClassReg.back();
    rec.m_Creator   = creator;
    rec.m_Name      = cName;
    rec.m_pPattern   = pPattern;
    rec.m_ID        = id;
    //  expose reflection footprint
    rec.m_pPattern->Expose( rec.m_Properties );

    return true;
} // ObjectFactory::RegisterWidgetClass

PropertyMap* ObjectFactory::GetPropertyMap( const char* className )
{
    int nC = m_ClassReg.size();
    for (int i = 0; i < nC; i++)
    {
        ClassRec& rec   = m_ClassReg[i];
        if (!stricmp( className, rec.m_Name )) return &rec.m_Properties;
    }
    return NULL;
} // ObjectFactory::GetPropertyMap

