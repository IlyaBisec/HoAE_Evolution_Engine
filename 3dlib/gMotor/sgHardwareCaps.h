/*****************************************************************************/
/*    File:    sgHardwareCaps.h
/*    Desc:    Device capabilities description node
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11-24-2003
/*****************************************************************************/
#ifndef __SGHARDWARECAPS_H__
#define __SGHARDWARECAPS_H__


/*****************************************************************************/
/*    Class:    HardwareCaps
/*    Desc:    
/*****************************************************************************/
class HardwareCaps : public SNode, public PSingleton<HardwareCaps>
{

public:
    HardwareCaps();

    DECLARE_SCLASS(HardwareCaps,SNode,HARC);
}; // class HardwareCaps



#endif // __SGHARDWARECAPS_H__