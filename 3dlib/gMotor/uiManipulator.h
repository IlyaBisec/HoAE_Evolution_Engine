/*****************************************************************************/
/*    File:    uiManipulator.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    18.06.2004
/*****************************************************************************/
#ifndef __UIMANIPULATOR_H__
#define __UIMANIPULATOR_H__
#include "IInput.h"

#include "sgShader.h"
#include "sgDummy.h"
#include "sgGeometry.h"
#include "uiWidget.h"

enum DragMode
{
    dmNone          = 0,
    dmDragX         = 1,
    dmDragY         = 2,
    dmDragZ         = 3,
    dmDragCenter    = 4
}; // enum DragMode

//-----------------------------------------------------------------------------
// uiMoveTool
//-----------------------------------------------------------------------------
class uiMoveTool : public Widget {
public:
	uiMoveTool();
	void Expose(PropertyMap &pm);
	void BindTM(Matrix4D *pSrc);
	void Render();
	bool OnMouseLBDown(int xMouse, int yMouse);
	bool OnMouseLBUp(int xMouse, int yMouse);
	bool OnMouseMove(int xMouse, int yMouse, DWORD Keys);

	DECLARE_SCLASS(uiMoveTool, Widget, UIMT);
private:
	float CalcAxes(Vector3D &xAxis, Vector3D &yAxis, Vector3D &zAxis);
	float CalcTracePos(Vector3D &TraceX, Vector3D &TraceY, Vector3D &TraceZ);

	DragMode m_DragMode;
	DWORD m_XColor;
	DWORD m_YColor;
	DWORD m_ZColor;
	DWORD m_CenterColor;
	DWORD m_SelColor;
	DWORD m_HeightColor;
	DWORD m_TraceColor;
	float m_ArrowLen; // Fraction of viewport side.
	float m_HeadLen; // Fraction of arrow len.
	float m_HeadRadius; // Fraction of arrow len.
	float m_CenterSide; // In pixels.
	float m_MaxSelDist; // Square fraction of arrow len.
	float m_HeightSide; // In pixels.
	float m_TraceSide; // In pixels.

	Vector3D m_CurPos;
	Vector3D m_StartPos;
	int m_StartMX, m_StartMY;
	Matrix4D *m_pTM;
};

/*****************************************************************************/
/*    Class:    TranslateTool
/*    Desc:    Translating manipulator
/*****************************************************************************/
class TranslateTool : public Widget
{
    DragMode            m_DragMode;

    DWORD               m_XColor;
    DWORD               m_YColor;
    DWORD               m_ZColor;
    DWORD               m_PColor;       //  color of screen plane moving gizmo
    DWORD               m_SelColor;     //  selected gizmo element color

    float               m_ArrowLen;     //  length of the gizmo arrow, fraction of viewport side
    float               m_MinSelDist;   
    DWORD               m_HMarkerColor;

    float               m_HeadLen;      //  length of the arrow head, fraction of arrow length
    float               m_HeadR;        //  radius of the arrow head, fraction of arrow length
    float               m_SGizmoSide;   //  side of screen space drag gizmo, in pixels

    Vector3D            m_StartPos;
    int                 m_StartMX, m_StartMY;

    Vector3D            m_InitPos;      
    Vector3D            m_CurPos;

    Matrix4D*           m_pTM;

    float               GetWorldFrame       ( Vector3D&x, Vector3D& y, Vector3D& z );

public:
                        TranslateTool       ();
    virtual void        Render                ();
    virtual void        Expose                ( PropertyMap& pm );
    virtual bool         OnMouseLBDown        ( int mX, int mY );
    virtual bool         OnMouseMove            ( int mX, int mY, DWORD keys );
    virtual bool         OnMouseLBUp            ( int mX, int mY );

    void                SetPosition         ( const Vector3D& pos );
    const Vector3D&     GetPosition         () const { return m_CurPos; }

    void                BindTM              ( Matrix4D* pTM );
    void                UnbindNode          ();

    DECLARE_SCLASS(TranslateTool,Widget,TRTL)
}; // class TranslateTool

/*****************************************************************************/
/*    Class:    ScaleTool
/*    Desc:    Translating manipulator
/*****************************************************************************/
class ScaleTool : public Widget
{
    DragMode            m_DragMode;

    DWORD               m_XColor;
    DWORD               m_YColor;
    DWORD               m_ZColor;
    DWORD               m_PColor;       //  color of screen plane moving gizmo
    DWORD               m_SelColor;     //  selected gizmo element color

    float               m_ArrowLen;    
    float               m_MinSelDist;   
    DWORD               m_HMarkerColor;

    float               m_HeadLen;      //  length of the arrow head, fraction of arrow length
    float               m_SGizmoSide;   //  side of screen space drag gizmo, in pixels

    int                 m_StartMX;
    int                 m_StartMY;

    bool                m_bLockUniform;

    Vector3D            m_StartScale;
    Vector3D            m_InitScale;      
    Vector3D            m_CurScale;

    Matrix4D*           m_pTM;

    float               GetWorldFrame       ( Vector3D&x, Vector3D& y, Vector3D& z );

public:
                        ScaleTool           ();
    virtual void        Render                ();
    virtual void        Expose                ( PropertyMap& pm );
    virtual bool         OnMouseLBDown        ( int mX, int mY );
    virtual bool         OnMouseMove            ( int mX, int mY, DWORD keys );
    virtual bool         OnMouseLBUp            ( int mX, int mY );

    void                SetScale            ( const Vector3D& s );
    const Vector3D&     GetPosition         () const { return m_CurScale; }

    void                BindTM              ( Matrix4D* pTM );
    void                UnbindNode          ();

    DECLARE_SCLASS(ScaleTool,Widget,SCTL)
}; // class ScaleTool

/*****************************************************************************/
/*    Class:    RotateTool
/*    Desc:    Rotating manipulator
/*****************************************************************************/
class RotateTool : public SNode, public IInputDispatcher
{
    DragMode            m_DragMode;

    DWORD               m_XColor;
    DWORD               m_YColor;
    DWORD               m_ZColor;
    DWORD               m_PColor;       //  color of screen plane moving gizmo
    DWORD               m_SelColor;     //  selected gizmo element color

    float               m_ArrowLen;     //  length of the gizmo arrow, fraction of viewport side
    float               m_MinSelDist;   
    float               m_HeadLen;      //  length of the arrow head, fraction of arrow length

    Vector3D            m_StartPos;
    int                 m_StartMX, m_StartMY;

    Vector3D            m_InitPos;      
    Vector3D            m_CurPos;

    TransformNode*      m_pNode;

    float               GetWorldFrame       ( Vector3D&x, Vector3D& y, Vector3D& z );

public:
                        RotateTool           ();
    virtual void        Render                ();
    virtual void        Expose                ( PropertyMap& pm );
    virtual bool         OnMouseLBDown    ( int mX, int mY );
    virtual bool         OnMouseMove            ( int mX, int mY, DWORD keys );
    virtual bool         OnMouseLBUp    ( int mX, int mY );

    void                SetPosition         ( const Vector3D& pos );
    const Vector3D&     GetPosition         () const { return m_CurPos; }

    void                BindNode            ( TransformNode* pNode );
    void                UnbindNode          ();

    DECLARE_SCLASS(RotateTool,SNode,ROTL)
}; // class RotateTool



#endif // __UIMANIPULATOR_H__
