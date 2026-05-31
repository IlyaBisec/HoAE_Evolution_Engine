/*****************************************************************************/
/*	File:	edUtilities.h
/*	Author:	Ruslan Shestopalyuk
/*	Desc:	Glues sequence of pictures onto single picture
/*	Date:	15.04.2004
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include <direct.h>


/*****************************************************************************/
/*	Class:	Utilities
/*	Desc:	Glues sequence of pictures onto single picture
/*****************************************************************************/
class Utilities : public SNode
{
	
public:
						Utilities		();
	void				Process		    ();
	virtual void		Expose		    ( PropertyMap& pm );

    //  deletes unused textures from game folder, puts textures into respective subfolders
    void                ArrangeTextures ();

	DECLARE_SCLASS(Utilities, SNode, EUTL);
}; // class Utilities



