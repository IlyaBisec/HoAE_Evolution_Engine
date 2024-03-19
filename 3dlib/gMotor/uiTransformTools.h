/*****************************************************************************/
/*    File:    uiTransformTools.h
/*    Desc:    Object visual transformation tools
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#ifndef __UITRANSFORMTOOLS_H__
#define __UITRANSFORMTOOLS_H__
#include "IInput.h"

/*****************************************************************************/
/*    Class:    TransformTool
/*****************************************************************************/
class TransformTool : public TransformNode, public IInputDispatcher
{
public:
                        TransformTool();
    virtual void        Expose                ( PropertyMap& pm );
    void                SetNodeToTransform    ( TransformNode* pNode ) { m_pNode = pNode; }

    DECLARE_SCLASS(TransformTool, TransformNode, TRAT);

protected:
    float                m_Size;
    TransformNode*        m_pNode;

}; // class TransformTool

/*****************************************************************************/
/*    Class:    SelectionTool
/*****************************************************************************/
class SelectionTool : public SNode, public IInputDispatcher
{
public:
                        SelectionTool        ();
    virtual void        Render                ();
    virtual bool         OnMouseMove            ( int mX, int mY, DWORD keys );
    virtual bool         OnMouseLBDown    ( int mX, int mY );    
    virtual bool         OnMouseLBUp    ( int mX, int mY );
    virtual void        Expose                ( PropertyMap& pm );
    
    void                SelectAll            ();
    void                UnselectAll            ();
    bool                IsNodeSelected        ( TransformNode* pNode );
    void                UnselectNode        ( TransformNode* pNode );
    void                SelectNode            ( TransformNode* pNode );
    int                    GetNSelectedNodes    ();
    TransformNode*        GetSelectedNode        ( int idx = 0 );

    DECLARE_SCLASS(SelectionTool, SNode, SELT);
protected:

    Shader*                m_pSelShader;
    ZBias*                        m_pSelZBias;
    Group*                        m_pSelection;
    Group*                        m_pSceneRoot;
    DWORD                        m_SelectionColor;

    c2::array<TransformNode*>    m_SelNodes;
}; // class SelectionTool

/*****************************************************************************/
/*    Class:    MoveTool
/*****************************************************************************/
class MoveTool : public TransformTool
{
    enum MoveDirection
    {
        mdNone    = 0,
        mdX        = 1, 
        mdY        = 2,
        mdZ        = 3
    };

public:
                        MoveTool();
    virtual void        Render();
    virtual bool         OnMouseMove            ( int mX, int mY, DWORD keys );
    virtual bool         OnMouseLBDown    ( int mX, int mY );    
    virtual bool         OnMouseLBUp    ( int mX, int mY );

    DECLARE_SCLASS(MoveTool, TransformTool, MOVT);

private:

    Matrix4D            GetXTM( float scale = 1.0f );
    Matrix4D            GetYTM( float scale = 1.0f );
    Matrix4D            GetZTM( float scale = 1.0f );

    Geometry*            m_Arrow;
    Shader*        m_pShader;
    
    MoveDirection        m_MoveDir;
    bool                m_bDragging;
    int                    m_Mx;
    int                    m_My;
}; // class MoveTool



#endif //  __UITRANSFORMTOOLS_H__
