/*****************************************************************************/
/*    File:    sgG17.h
/*    Desc:    .g17 format loader
/*    Author:    Ruslan Shestopalyuk
/*    Date:    27.02.2003
/*****************************************************************************/
#ifndef __SGG17_H__
#define __SGG17_H__



/*****************************************************************************/
/*    Class:    G17Creator
/*    Desc:    Loader of the G17 sprite package
/*****************************************************************************/
class G17Creator : public IPackageCreator
{
public:
                            G17Creator        ();
    virtual SpritePackage*    CreatePackage    ( char* fileName, const BYTE* data );
    virtual const char*        Description        () const;

}; // class G17Creator



#endif // __SGG17_H__