/*****************************************************************************/
/*    File:    kUuid.h
/*    Desc:    Unique universal identifier wrapper
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-04-2004
/*****************************************************************************/
#ifndef __KUUID_H__
#define __KUUID_H__
#pragma comment (lib, "rpcrt4" )

/*****************************************************************************/
/*    Class:    uuid
/*    Desc:    Wrapper for unique universal identifier value
/*****************************************************************************/
class uuid
{
public:
                    uuid        ();
    DWORD            hash        ();
    bool            create        ();
    const char*        asString    () const;
    bool            fromString    ( const char* str );

private:
    BYTE            _uuid[16];
}; // class uuid

#endif // __KUUID_H__