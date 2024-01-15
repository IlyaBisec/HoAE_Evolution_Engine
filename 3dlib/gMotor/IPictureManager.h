/*****************************************************************************/
/*    File:    IPictureManager.h
/*    Desc:    Image files management interface
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-03-2004
/*****************************************************************************/
#ifndef __IPICTUREMANAGER_H__
#define __IPICTUREMANAGER_H__

enum PictureFileFormat
{
    pfBMP        = 0,
    pfJPG        = 1,
    pfTGA        = 2,
    pfPNG        = 3,
    pfDDS        = 4,
    pfPPM        = 5,
    pfDIB        = 6,
    pfUnknown    = 255
}; // enum PictureFileFormat

/*****************************************************************************/
/*    Class:    IPictureManager
/*    Desc:    Image files management interface
/*****************************************************************************/
class IPictureManager
{
public:
    virtual int                 GetImageID          ( const char* fileName ) = 0;
    virtual bool                LoadImage           ( int imgID ) = 0;
    virtual bool                UnloadImage         ( int imgID ) = 0;
    virtual void                Purge               ()            = 0;
    virtual BYTE*               GetPixels           ( int imgID ) = 0;
    virtual int                 GetWidth            ( int imgID ) = 0;
    virtual int                 GetHeight           ( int imgID ) = 0;
    virtual bool                IsValid             ( int imgID ) = 0;
    virtual bool                IsLoaded            ( int imgID ) = 0;
    virtual const char*         GetFileName         ( int imgID ) = 0;
    virtual PictureFileFormat   GetFileFormat       ( int imgID ) = 0;
    virtual ColorFormat         GetColorFormat      ( int imgID ) = 0;
    virtual bool                DrawImage           ( int imgID, float x, float y, float z = 0.0f ) = 0;
    virtual bool                DrawImage           ( int imgID, const Rct& ext, float z = 0.0f ) = 0;
    virtual void                SetDiffuse          ( DWORD color ) = 0;
    virtual void                SetFiltering        ( bool bFilter = true ) = 0;
    
    virtual void                SetTransform        ( const Matrix4D& tm ) = 0;
    virtual const Matrix4D&     GetTransform        () const = 0;
    virtual void                EnableTransform     ( bool bEnable = true ) = 0;
    virtual bool                TransformEnabled    () const = 0;

}; // class IPictureManager

extern DIALOGS_API IPictureManager* IPM;
void InitPictureManager();
IPictureManager* GetPictureManager();

#endif // __IPICTUREMANAGER_H__