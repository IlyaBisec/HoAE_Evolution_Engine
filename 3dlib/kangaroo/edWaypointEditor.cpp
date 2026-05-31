/*****************************************************************************/
/*	File:	edWaypointEditor.cpp
/*	Desc:	Ragdoll controller
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-22-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "uiControl.h"
#include "edWaypointEditor.h"


/*****************************************************************************/
/*	WaypointEditor implementation
/*****************************************************************************/
WaypointEditor::WaypointEditor()
{
	m_pModel		= NULL;
	m_pSystem		= NULL;
	m_CurBeg		= Vector3D::null;
	m_CurEnd		= Vector3D::null;
	m_CurWidth		= 50.0f;

	m_bCreating		= false;
	m_pCurSegment	= NULL;
	m_bDragging		= false;
	m_Bias			= 0.5f;
	m_WeldRadius	= 10.0f;

	m_CoreColor		= 0xFF2BC42F;
	m_Color			= 0x4B2BC42F;

	m_SystemName	= "NaviSystem";

	m_CurSeg		= 0;
	m_CurMX = m_CurMX = 0;
} // WaypointEditor::WaypointEditor

void WaypointEditor::Weld( Vector3D& pt )
{
	float r2 = m_WeldRadius*m_WeldRadius;
	Iterator it( m_pSystem, SegmentNode::FnFilter );
	while (it)
	{
		SegmentNode* pSeg = (SegmentNode*)*it;
		if (pSeg->GetBeg().distance2( pt ) < r2)
		{
			pt = pSeg->GetBeg();
			return;
		}
		if (pSeg->GetEnd().distance2( pt ) < r2)
		{
			pt = pSeg->GetEnd();
			return;
		}
		++it;
	}
	pt.z += m_Bias;
}

void WaypointEditor::AddSegment( const Vector3D& beg, const Vector3D& end, float width )
{
	if (!m_pSystem) return;
	char name[256];
	sprintf( name, "seg%d", m_CurSeg++ );
	m_pCurSegment = m_pSystem->AddChild<SegmentNode>( name );
	m_pCurSegment->SetWidth	( width );

	m_pCurSegment->SetBeg	( beg	);
	m_pCurSegment->SetEnd	( end	);
	m_pCurSegment->SetNormal( Vector3D::oZ );
} // WaypointEditor::AddSegment

bool WaypointEditor::MouseToWorld( int mX, int mY, Vector3D& pt )
{
	ICamera* pCam = GetCamera();
    if (!pCam || !m_pModel) return false;
	Ray3D pickRay = pCam->GetPickRay( mX, mY );
	return (PickNode( pickRay, m_pModel, pt ) != NULL);
} // WaypointEditor::MouseToWorld

float WaypointEditor::GetCurWidth()
{
	if (!m_pSystem) return m_CurWidth;
	for (int i = 0; i < m_pSystem->GetNChildren(); i++)
	{
		SegmentNode* pNode = (SegmentNode*)m_pSystem->GetChild( i );
		if (!pNode->IsA<SegmentNode>()) continue;
		m_CurWidth = pNode->GetWidth();
	}
	return m_CurWidth;
}

bool WaypointEditor::OnMouseLBDown( int mX, int mY )
{
	if (IsInvisible()) return false;
	if (GetKeyState( VK_LSHIFT ) >= 0) return false;
	if (GetKeyState( VK_MENU ) < 0 || GetKeyState( VK_CONTROL ) < 0 ) return false;
	if (m_bCreating)
	{
		AddSegment( m_CurBeg, m_CurEnd, GetCurWidth() );
		m_bCreating = false;
	}
	else
	{
		GetCurWidth();
		if (MouseToWorld( mX, mY, m_CurBeg ))
		{
			m_bCreating = true;
			Weld( m_CurBeg );
		}
	}
	return false;
} // WaypointEditor::OnMouseLBDown

bool WaypointEditor::OnMouseMove( int mX, int mY, DWORD keys )
{
	if (IsInvisible()) return false;

	m_CurMX = mX;
	m_CurMY = mY;

	if (MouseToWorld( mX, mY, m_CurEnd ))
	{
		Weld( m_CurEnd );
	}
	return false;
}

bool WaypointEditor::OnMouseLBUp( int mX, int mY )
{
	return false;
}

bool WaypointEditor::OnMouseMBDown( int mX, int mY )
{
	return false;
}

bool WaypointEditor::OnMouseMBUp( int mX, int mY )
{
	return false;
}

bool WaypointEditor::OnKeyDown( DWORD keyCode, DWORD flags )
{
	return false;
}

void WaypointEditor::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "WaypointEditor", this );
	pm.f( "SystemName", m_SystemName );
	pm.f( "Color", m_Color, "color" );
	pm.f( "CoreColor", m_CoreColor, "color" );
}

SNode* WaypointEditor::FindModel()
{
	SNode* pScene = Root::instance()->FindChild<Group>( "Scene" );
	if (!pScene) return NULL;
	return pScene->GetChild( 0 );
} // WaypointEditor::FindModel

void WaypointEditor::Render()
{
	if (m_bCreating)
	{
		DrawFatSegment( m_CurBeg, m_CurEnd, Vector3D::oZ, m_CurWidth, false, m_Color, 0 );
	}

	m_pModel = FindModel();
	if (!m_pModel) return;
	if (!m_pSystem || m_pSystem->GetParent() != m_pModel)
	{
		delete m_pSystem;
		m_pSystem->SetInvisible();
	}

	m_pSystem->SetCoreColor ( m_CoreColor	);
	m_pSystem->SetColor		( m_Color		);
	m_pSystem->Render();
} // WaypointEditor::Render



