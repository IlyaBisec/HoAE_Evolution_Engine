/*****************************************************************************/
/*    File:    uiSystemExplorer.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    30 Dec 2004
/*****************************************************************************/
#ifndef __UISystemExplorer_H__
#define __UISystemExplorer_H__

#include "uiObjectInspector.h"
#include "uiNodeTree.h"

/*****************************************************************************/
/*    Class:    SystemExplorer
/*    Desc:    
/*****************************************************************************/    
class SystemExplorer : public Window
{
    ObjectInspector         m_Props;
    NodeTree                m_NodeTree;

public:
                            SystemExplorer();

    virtual void            Render      ();
    virtual void            Expose      ( PropertyMap& pm );

    DECLARE_CLASS(SystemExplorer);

}; // class SystemExplorer

#endif // __UISYSTEMEXPLORER_H__