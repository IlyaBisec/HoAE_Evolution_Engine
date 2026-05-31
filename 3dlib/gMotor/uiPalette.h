/*****************************************************************************/
/*    File:    uiPalette.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#ifndef __UIPALETTE_H__
#define __UIPALETTE_H__

#include "uiListBox.h"
#include "uiButton.h"

/*****************************************************************************/
/*    Class:    PaletteSample
/*    Desc:    
/*****************************************************************************/
class PaletteSample : public Widget
{
    std::string         m_SampleClass;
public:
                        PaletteSample   ();
    virtual void        Expose          ( PropertyMap& pm );
    virtual void        Render          ();

    const char*         GetSampleClass  () const { return m_SampleClass.c_str(); }
    void                SetSampleClass  ( const char* name ) { m_SampleClass = name; }

    DECLARE_CLASS(PaletteSample);
}; // class PaletteSample

/*****************************************************************************/
/*    Class:    FileBrowser
/*    Desc:    
/*****************************************************************************/
class FileBrowser : public ListWidget
{
public:
                        FileBrowser     ();
    virtual void        Render          ();
    virtual void        Expose          ( PropertyMap& pm );

    DECLARE_CLASS(FileBrowser);
}; // class FileBrowser

#endif // __UIPALETTE_H__