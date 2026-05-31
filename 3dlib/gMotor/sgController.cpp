/*****************************************************************************/
/*    File:    sgController.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "mAlgo.h"
#include "kIOHelpers.h"
#include "mAnimCurve.hpp"
#include "vSkin.h"

#ifndef _INLINES
#include "sgController.inl"
#endif // !_INLINES

IMPLEMENT_CLASS( Controller );


/*****************************************************************************/
/*    Controller implementation
/*****************************************************************************/
Controller::Controller()
{
}

Controller::~Controller()
{
}

void Controller::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
}

void Controller::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
}



