/*****************************************************************************/
/*    File:    uiPicture.h
/*    Desc:    Picture UI element
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-20-2003
/*****************************************************************************/
#ifndef __UIPICTURE_H__
#define __UIPICTURE_H__

#include "IPictureManager.h"

ENUM( PictureFileFormat, "PictureFileFormat", 
         en_val( pfBMP,        "BMP"  ) <<
         en_val( pfJPG,        "JPG"  ) <<
         en_val( pfTGA,        "TGA"  ) <<
         en_val( pfPNG,        "PNG"  ) <<
         en_val( pfDDS,        "DDS"  ) <<
         en_val( pfPPM,        "PPM"  ) <<
         en_val( pfDIB,        "DIB"  ) << 
        en_val( pfUnknown,    "Unknown" ) );    


/*****************************************************************************/
/*    Class:    Picture
/*    Desc:    Background picture node
/*****************************************************************************/
class Picture : public SNode
{
    int                    m_ID;
    Rct                    m_Ext;
    bool                m_bStretch;
    float                m_Z;
    std::string            m_File;

    DWORD                m_Color;
    bool                m_bFilter;

public:
                        Picture            ();
    virtual                ~Picture        ();
    virtual void        Render            ();
    virtual void        Serialize        ( OutStream& os ) const;
    virtual void        Unserialize        ( InStream& is );
    virtual void        Expose            ( PropertyMap& pm );

    const char*            GetFile            () const { return m_File.c_str(); }
    void                SetFile            ( const char* file );

    int                    GetInitialWidth    () const;  
    int                    GetInitialHeight() const; 
    PictureFileFormat    GetFileFormat    () const;    
    ColorFormat            GetColorFormat    () const;
    bool                IsLoaded        () const;
    bool                IsValid            () const;

    void                UnloadImage        ();
    void                LoadImage        ();

    DECLARE_SCLASS(Picture,SNode,PICT);
}; // class Picture



#endif // __UIPICTURE_H__