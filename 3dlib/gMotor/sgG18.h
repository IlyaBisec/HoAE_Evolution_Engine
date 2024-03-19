/*****************************************************************************/
/*    File:    sgG18.h
/*    Desc:    .G18 format loader
/*    Author:    Ruslan Shestopalyuk
/*    Date:    27.02.2003
/*****************************************************************************/
#ifndef __SGG18_H__
#define __SGG18_H__



/*****************************************************************************/
/*    Class:    G18Creator
/*    Desc:    Loader of the G18 sprite package
/*****************************************************************************/
class G18Creator : public IPackageCreator
{
public:
                            G18Creator        ();
    virtual SpritePackage*    CreatePackage    ( char* fileName, const BYTE* data );
    virtual const char*        Description        () const;

}; // class G18Creator



#endif // __SGG18_H__