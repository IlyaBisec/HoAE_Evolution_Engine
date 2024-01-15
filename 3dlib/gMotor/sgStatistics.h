/*****************************************************************************/
/*    File:    sgStatistics.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11-13-2003
/*****************************************************************************/
#ifndef __SGSTATISTICS_H__
#define __SGSTATISTICS_H__

#include "kArray.hpp"



const int c_MaxPointsFPS = 128;
/*****************************************************************************/
/*    Class:    StatManager
/*    Desc:    Statistics info manager
/*****************************************************************************/
class StatManager : public SNode, public IStatistics
{
    c2::circular_queue<int, c_MaxPointsFPS>    m_FPS;
public:
                         StatManager();
    virtual void         Render();


    virtual float        GetFPS         (){ return 0; }
    virtual float        GetAverageFPS  (){ return 0; }
    virtual float        GetMinFPS      (){ return 0; }
    virtual float        GetMaxFPS      (){ return 0; }

    DECLARE_SCLASS(StatManager,SNode,STAT);
}; // class StatManager



#endif // __SGSTATISTICS_H__
