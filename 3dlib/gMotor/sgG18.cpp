/*****************************************************************************/
/*    File:    sgG18.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    03.03.2003
/*****************************************************************************/
#include "stdafx.h"

#include "kHash.hpp"
#include "kResource.h"
#include "sgSpriteManager.h"
#include "sgG18.h"


/*****************************************************************************/
/*    G18Creator implementation
/*****************************************************************************/
G18Creator::G18Creator()
{
    SpritePackage::RegisterCreator( this );
}

SpritePackage* G18Creator::CreatePackage( char* fileName, const BYTE* data )
{
    return NULL;
} // G18Creator::Load

const char*    G18Creator::Description() const
{
    return "G18 Sprite Loader";
} // G18Creator::Description




    
