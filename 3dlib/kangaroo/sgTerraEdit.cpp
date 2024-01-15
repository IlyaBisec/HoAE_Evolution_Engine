/*****************************************************************************/
/*	File:	sgTerraEdit.cpp
/*	Desc:	Terrain editing tools
/*	Author:	Ruslan Shestopalyuk
/*	Date:	11-12-2003
/*****************************************************************************/
#include "stdafx.h"
#include "uiControl.h"
#include "mTriangle.h"
#include "ITerrain.h"
#include "sgTerraEdit.h"


/*****************************************************************************/
/*	TerrainEditor implementation
/*****************************************************************************/
TerrainEditor::TerrainEditor()
{
	m_PerlinShiftX		= 17.0f;
	m_PerlinShiftY		= 80.0f;
	m_PerlinScaleX		= 0.001f;
	m_PerlinScaleY		= 0.001f;

	m_HeightShift		= 0.0f;
	m_HeightScale		= 300.0f;
	m_ZeroGroundLevel	= 0.0f;

	m_BrushRadius		= 500.0f;
	m_BrushColor		= 0xFFFF3333;
	m_BrushForm			= bfSquare;
	m_BrushDecay		= 0.01f;
	m_BrushRotation		= 0.0f;
	m_BrushPosition		= Vector3D::null;
	m_bDrawBrush		= false;
	m_HeightDelta		= 0.0f;

	m_pObjectTypes		= NULL;
	m_EditMode			= teTrees;

} // TerrainEditor::TerrainEditor

void TerrainEditor::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
} // TerrainEditor::Serialize

void TerrainEditor::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
} // TerrainEditor::Unserialize

void TerrainEditor::DisturbPerlin()
{
	if (!ITerra) return;
	ITerra->InvalidateAABB();
	ITerra->InvalidateGeometry();
	ITerra->InvalidateTexture();
} // TerrainEditor::DisturbPerlin

const int c_BrushSegments = 64.0f;
void TerrainEditor::DrawBrush()
{
	if (!ITerra || !m_bDrawBrush) return;

	int nSeg = c_BrushSegments;
	if (m_BrushForm == bfRound) 
	{
		float dAng = c_DoublePI / nSeg;
		float ang = m_BrushRotation;
		for (int i = 0; i < nSeg; i++)
		{
			Vector3D a( m_BrushPosition.x + m_BrushRadius * cosf( ang ), 
				m_BrushPosition.y + m_BrushRadius * sinf( ang ), 0.0f );
			Vector3D b( m_BrushPosition.x + m_BrushRadius * cosf( ang + dAng ), 
				m_BrushPosition.y + m_BrushRadius * sinf( ang + dAng ), 0.0f );
			a.z = ITerra->GetH( a.x, a.y ) + 5.0f;
			b.z = ITerra->GetH( b.x, b.y ) + 5.0f;
			rsLine( a, b, m_BrushColor, m_BrushColor );
			ang += dAng;
		}
	}
	else if (m_BrushForm == bfSquare) 
	{
		Matrix4D bTM( Vector3D::one, Vector3D::oZ, m_BrushRotation, m_BrushPosition ) ;
		float dS = 0.5f * m_BrushRadius / nSeg;
		int sideSeg = m_BrushRadius / dS;
		const float br = m_BrushRadius;
		for (float i = -m_BrushRadius; i < m_BrushRadius; i++)
		{
			Vector3D a( -br, i, 0.0f );
			Vector3D b( -br, tmin( i + dS, br ), 0.0f );
			bTM.transformPt( a );
			bTM.transformPt( b );
			a.z = ITerra->GetH( a.x, a.y ) + 5.0f;
			b.z = ITerra->GetH( b.x, b.y ) + 5.0f;
			rsLine( a, b, m_BrushColor, m_BrushColor );

			a.set( br, i, 0.0f );
			b.set( br, tmin( i + dS, br ), 0.0f );
			bTM.transformPt( a );
			bTM.transformPt( b );
			a.z = ITerra->GetH( a.x, a.y ) + 5.0f;
			b.z = ITerra->GetH( b.x, b.y ) + 5.0f;
			rsLine( a, b, m_BrushColor, m_BrushColor );

			a.set( i, br, 0.0f );
			b.set( tmin( i + dS, br ), br, 0.0f );
			bTM.transformPt( a );
			bTM.transformPt( b );
			a.z = ITerra->GetH( a.x, a.y ) + 5.0f;
			b.z = ITerra->GetH( b.x, b.y ) + 5.0f;
			rsLine( a, b, m_BrushColor, m_BrushColor );

			a.set( i, -br, 0.0f );
			b.set( tmin( i + dS, br ), -br, 0.0f );
			bTM.transformPt( a );
			bTM.transformPt( b );
			a.z = ITerra->GetH( a.x, a.y ) + 5.0f;
			b.z = ITerra->GetH( b.x, b.y ) + 5.0f;
			rsLine( a, b, m_BrushColor, m_BrushColor );
		}
	}

	IRS->SetWorldTM( Matrix4D::identity );
	rsFlushLines3D();
} // TerrainEditor::DrawBrush

bool TerrainEditor::OnMouseWheel( int delta )
{
	if (GetKeyState( VK_CONTROL ) < 0) 
	{
		m_BrushRotation += float( delta ) * 0.001f;
	}
	else
	{
		m_BrushRadius += delta/4;
	}
	return false;
} // TerrainEditor::OnMouseWheel

bool TerrainEditor::OnMouseRBDown( int mX, int mY )
{
	if (GetKeyState( VK_MENU ) < 0) return false;

	if (m_EditMode == teHeight)
	{
		m_HeightDelta = -10.0f;
	}

	return false;
} // TerrainEditor::OnMouseRBDown

bool TerrainEditor::OnMouseLBDown( int mX, int mY )
{
	if (GetKeyState( VK_MENU ) < 0) return false;

	if (m_EditMode == teObjects)
	{
		if (!m_pObjectTypes || m_pObjectTypes->GetNChildren() == 0 || !ITerra) return false;
		int nInst = m_BrushRadius*m_BrushRadius/10000;
		for (int i = 0; i < nInst; i++)
		{
			TerraObjectInstance inst;
			inst.id = rand() % m_pObjectTypes->GetNChildren();
			Vector3D pos( m_BrushPosition );
			pos.x += rndValuef( -m_BrushRadius, m_BrushRadius );
			pos.y += rndValuef( -m_BrushRadius, m_BrushRadius );
			pos.z = ITerra->GetH( pos.x, pos.y );
			inst.tm = Matrix4D( Vector3D::one, Vector3D::oZ, m_BrushRotation, pos );
			m_Objects.push_back( inst );
		}
	}
	else if (m_EditMode == teHeight)
	{
		m_HeightDelta = 10.0f;
	}
	else if (m_EditMode == teFacture)
	{
	}
	return false;
} // TerrainEditor::OnMouseLBDown

bool TerrainEditor::OnMouseRBUp( int mX, int mY )
{
	if (m_EditMode == teHeight)
	{
		m_HeightDelta = 0.0f;
	}
	return false;
} // TerrainEditor::OnMouseRBUp

bool TerrainEditor::OnMouseMove( int mX, int mY, DWORD keys )
{
	if (ITerra)
	{
		m_bDrawBrush = ITerra->Pick( mX, mY, m_BrushPosition );
	}
	return false;
} // TerrainEditor::OnMouseMove

bool TerrainEditor::OnMouseLBUp( int mX, int mY )
{
	if (m_EditMode == teHeight)
	{
		m_HeightDelta = 0.0f;
	}
	return false;
} // TerrainEditor::OnMouseLBUp

void TerrainEditor::Render()
{
	if (!m_pObjectTypes) m_pObjectTypes = GetChild<Group>( "ObjectTypes" );
	if (!ITerra) return;
	DrawBrush();

	/*if (fabs( m_HeightDelta ) > c_Epsilon)
	{
		if (m_BrushForm == bfRound)
		{
			float step = m_BrushRadius / 32.0f;
			int level = 0;
			int cx = 0, cy = 0;
			int nx, ny;
			ITerra->GetHmapNode( m_BrushPosition, cx, cy );
			for (float i = 0; i < m_BrushRadius; i += step)
			{
				int nElm = GetNumRingPoints( level );
				float h = m_HeightDelta * exp( -i * m_BrushDecay );
				for (int j = 0; j < nElm; j++)
				{
					GetRingPoint( level, j, nx, ny );
					float prevH = ITerra->GetH( cx + nx, cy + ny );
					ITerra->SetHeight( cx + nx, cy + ny, prevH + h );
				}
				level++;
				if (level >= GetMaxRingLevel()) break;
			}
		}
		else if (m_BrushForm == bfSquare)
		{
			int ox = 0, oy = 0;
			ITerra->GetHmapNode( m_BrushPosition, ox, oy );
			Matrix4D bTM( Vector3D::one, Vector3D::oZ, m_BrushRotation, m_BrushPosition ) ;
			const float br = m_BrushRadius;
			Vector3D a( -br, -br, 0.0f );
			Vector3D b(  br, -br, 0.0f );
			Vector3D c(  br,  br, 0.0f );
			Vector3D d( -br,  br, 0.0f );
			bTM.transformPt( a );
			bTM.transformPt( b );
			bTM.transformPt( c );
			bTM.transformPt( d );
			int ax, ay, bx, by, cx, cy, dx, dy;
			ITerra->GetHmapNode( a, ax, ay );
			ITerra->GetHmapNode( b, bx, by );
			ITerra->GetHmapNode( c, cx, cy );
			ITerra->GetHmapNode( d, dx, dy );

			float h = m_HeightDelta;

			Triangle2D::Rasterizer r1( ax, ay, bx, by, cx, cy );
			Triangle2D::Rasterizer r2( ax, ay, cx, cy, dx, dy );
			while (r1)
			{
				r1.Step();
				ITerra->SetHeight( r1.GetCurX(), r1.GetCurY(), 
					ITerra->GetH( r1.GetCurX(), r1.GetCurY() ) + h );
			}
			while (r2)
			{
				r2.Step();
				ITerra->SetHeight( r2.GetCurX(), r2.GetCurY(), 
					ITerra->GetH( r2.GetCurX(), r2.GetCurY() ) + h );
			}

		}
		Rct rct( m_BrushPosition.x - m_BrushRadius, 
				 m_BrushPosition.y - m_BrushRadius,
				 m_BrushRadius * 2, m_BrushRadius * 2 );
		ITerra->InvalidateAABB( &rct );
		ITerra->InvalidateGeometry( &rct );
	}*/

	int nObj = m_Objects.size();
	for (int i = 0; i < nObj; i++)
	{
		TerraObjectInstance& inst = m_Objects[i];
		TransformNode::ResetTMStack( &(inst.tm) );
		SNode* pObjectType = m_pObjectTypes->GetChild( inst.id );
		if (pObjectType) pObjectType->Render();
	}
	TransformNode::ResetTMStack();
} // TerrainEditor::Render

void TerrainEditor::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "TerrainEditor", this );
	pm.f( "PerlinShiftX",		m_PerlinShiftX		);
	pm.f( "PerlinShiftY",		m_PerlinShiftY		);
	pm.f( "PerlinScaleX",		m_PerlinScaleX		);
	pm.f( "PerlinScaleY",		m_PerlinScaleY		);
	pm.f( "HeightShift",		m_HeightShift		);
	pm.f( "HeightScale",		m_HeightScale		);
	pm.f( "ZeroGroundLevel",	m_ZeroGroundLevel	);
	pm.f( "BrushRadius",		m_BrushRadius		);
	pm.f( "BrushColor",		m_BrushColor, "color" );
	pm.f( "BrushForm",		m_BrushForm			);
	pm.f( "BrushDecay",		m_BrushDecay		);
	pm.f( "BrushRotation",	m_BrushRotation		);
	pm.f( "BrushPosX",		m_BrushPosition.x	);
	pm.f( "BrushPosY",		m_BrushPosition.y	);
	pm.f( "BrushPosZ",		m_BrushPosition.z	);
	pm.f( "EditMode",			m_EditMode			);
	pm.m( "DisturbPerlin",	DisturbPerlin		);
	pm.m( "ResetGround",		ResetGround			);
	pm.m( "FlattenValleys",	FlattenValleys		);
	pm.m( "AccentSlopes",	AccentSlopes		);
} // TerrainEditor::Expose

void TerrainEditor::ResetGround()
{
	if (!ITerra) return;
} // TerrainEditor::ResetGround

void TerrainEditor::AccentSlopes()
{
	if (!ITerra) return;
} // TerrainEditor::AccentSlopes

void TerrainEditor::FlattenValleys()
{
	if (!ITerra) return;
} // TerrainEditor::FlattenValleys

