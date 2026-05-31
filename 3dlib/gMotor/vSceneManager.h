/*****************************************************************************/
/*	File:	vSceneManager.h
/*	Desc:	ModelObject manager interface implementation
/*	Author:	Ruslan Shestopalyuk
/*  Date:   18.10.2004
/*****************************************************************************/
#ifndef __VSCENEMANAGER_H__
#define __VSCENEMANAGER_H__

#include "kStaticArray.hpp"
#include "IMesh.h"

const int c_MaxRenderBits = 16384;
typedef static_array<RenderBit*, c_MaxRenderBits> SortedTransactions;
/*****************************************************************************/
/*  Class:  SceneManager
/*  Desc:   Creates internal scene batch represenation, organizes and renders 
/*              batches
/*****************************************************************************/
class SceneManager
{
    RenderBit                       m_Tasks[c_MaxRenderBits];

    SortedTransactions              m_SortedTasks;
    int                             m_NTasks;
    bool                            m_bSorted;
    
    void                            SortTasks();

public:
                        SceneManager();
    RenderBit&  AddTask     ()
    {
        assert( m_NTasks < c_MaxRenderBits );
        return m_Tasks[m_NTasks++];
    }
    void                ResetScene  ();
    void                RenderScene ();

}; // class SceneManager

extern SceneManager* ISceneMgr;

#endif // __VSCENEMANAGER_H__