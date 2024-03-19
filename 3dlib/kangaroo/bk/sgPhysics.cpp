/*****************************************************************************/
/*	File:	sgPhysics.cpp
/*	Desc:	Physics wrapper
/*	Author:	Ruslan Shestopalyuk
/*	Date:	11-12-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgPhysics.h"


/*****************************************************************************/
/*	Utilities
/*****************************************************************************/
//  to ODE
void Convert( dMatrix3& dm, const Matrix4D& m )
{
	dm[0] = m.e00; dm[4] = m.e01; dm[8]  = m.e02;
	dm[1] = m.e10; dm[5] = m.e11; dm[9]  = m.e12;
	dm[2] = m.e20; dm[6] = m.e21; dm[10] = m.e22;
	dm[3] = m.e30; dm[7] = m.e31; dm[11] = m.e32;
}

void Convert( dMatrix3& dm, const Matrix3D& m )
{
	dm[0] = m.e00; dm[4] = m.e01; dm[8]  = m.e02;
	dm[1] = m.e10; dm[5] = m.e11; dm[9]  = m.e12;
	dm[2] = m.e20; dm[6] = m.e21; dm[10] = m.e22;
	dm[3] = 0;	   dm[7] = 0;	  dm[11] = 0;
}

void Convert( dVector4& dv, const Vector4D& v )
{
	dv[0] = v.x; dv[1] = v.y; dv[2] = v.z; dv[3] = v.w;
}

void Convert( dVector3& dv, const Vector3D& v )
{
	dv[0] = v.x; dv[1] = v.y; dv[2] = v.z; 
}

void Convert( dQuaternion& dq, const Quaternion& q )
{
	dq[0] = q.s; 
	dq[1] = q.v.x;
	dq[2] = q.v.y;
	dq[3] = q.v.z;
}

//  from ODE
void Convert( Matrix4D& m, const dMatrix3& dm )
{
	m.e00 = dm[0]; m.e01 = dm[4]; m.e02 = dm[8] ; m.e03 = 0.0f;
	m.e10 = dm[1]; m.e11 = dm[5]; m.e12 = dm[9] ; m.e13 = 0.0f;
	m.e20 = dm[2]; m.e21 = dm[6]; m.e22 = dm[10]; m.e23 = 0.0f;
	m.e30 = dm[3]; m.e31 = dm[7]; m.e32 = dm[11]; m.e33 = 1.0f;
}

void Convert( Matrix3D& m, const dMatrix3& dm )
{
	m.e00 = dm[0]; m.e01 = dm[4]; m.e02 = dm[8] ;
	m.e10 = dm[1]; m.e11 = dm[5]; m.e12 = dm[9] ;
	m.e20 = dm[2]; m.e21 = dm[6]; m.e22 = dm[10];
}

void Convert( Vector4D& v, const dVector4& dv )
{
	v.x = dv[0]; v.y = dv[1]; v.z = dv[2]; v.w = dv[3];
}

void Convert( Vector3D& v, const dVector3& dv )
{
	v.x = dv[0]; v.y = dv[1]; v.z = dv[2]; 
}

void Convert( Quaternion& q, const dQuaternion& dq )
{
	q.s		= dq[0]; 
	q.v.x	= dq[1];
	q.v.y	= dq[2];
	q.v.z	= dq[3];
}

_inl float GetCurWorldScale()
{
	return PhysicsSystem::GetCurSystem() ? 
			PhysicsSystem::GetCurSystem()->GetWorldScale() : 1.0f;
}

_inl float GetCurTimeScale()
{
	return PhysicsSystem::GetCurSystem() ? 
		PhysicsSystem::GetCurSystem()->GetTimeScale() : 1.0f;
}

_inl dWorldID GetCurWorldID()
{
	return PhysicsSystem::GetCurSystem() ? 
			PhysicsSystem::GetCurSystem()->GetWorldID() : NULL;
}

/*****************************************************************************/
/*	PhysicsSystem implementation
/*****************************************************************************/
PhysicsSystem*	PhysicsSystem::s_pCurPhysSystem = NULL;

float 			PhysicsSystem::s_JointHandleSize	= 10.0f;
DWORD 			PhysicsSystem::s_JointAnchorColorF	= 0x55CCCC66;
DWORD 			PhysicsSystem::s_JointAnchorColor	= 0xFFCCCC66;
DWORD 			PhysicsSystem::s_JointAxis1ColorF	= 0x556666CC;
DWORD 			PhysicsSystem::s_JointAxis1Color	= 0xFF6666CC;
DWORD 			PhysicsSystem::s_JointAxis2ColorF	= 0x5566CC66;
DWORD 			PhysicsSystem::s_JointAxis2Color	= 0xFF66CC66;  
DWORD 			PhysicsSystem::s_GeomColor			= 0xFF66A8C6;  
DWORD 			PhysicsSystem::s_GeomColorF			= 0x6666A8C6;  
DWORD 			PhysicsSystem::s_ContactColor		= 0x66FFA8C6;  
bool			PhysicsSystem::s_bEnableGizmoZ		= false;

PhysicsSystem::PhysicsSystem()
{
	m_ERP				= 0.9f;
	m_CFM				= 0.001f;
	m_Gravity.x 		= 0.0f;
	m_Gravity.y 		= 0.0f;
	m_Gravity.z 		= -9.81f;
	m_LastTime			= 0;
	m_StepMode			= smNormal;
	m_MaxStepFastIter	= 10;

	m_StepScale 		= 5.0f;
	m_WorldScale		= 0.01f;
	m_TimeScale			= sqrtf( m_WorldScale );
	m_MaxTimeStep		= 0.05f;
	m_bFixedStep		= false;
	m_FixedStepSize		= 0.01;

	m_PID				= dWorldCreate();
	s_pCurPhysSystem	= this;

	if (m_PID)
	{
		SetERP( m_ERP );
		SetCFM( m_CFM );
		SetGravity( m_Gravity );
	}
} // PhysicsSystem::PhysicsSystem

PhysicsSystem::~PhysicsSystem()
{
	if (m_PID) dWorldDestroy( m_PID );
	dCloseODE();
}

void PhysicsSystem::Render()
{
	if (IsDisabled()) return;
	Step();
} // PhysicsSystem::Render

void PhysicsSystem::SetWorldScale( float val )
{
	m_WorldScale = val;
	m_TimeScale = sqrtf( m_WorldScale );
}

void PhysicsSystem::Step()
{
	if (m_PID == NULL) return;
	if (m_LastTime == 0)
	{
		m_LastTime = GetTickCount();
		return;
	}

	DWORD cTime = GetTickCount();
	float dt = cTime - m_LastTime;	
	dt *= 0.001;
	if (dt > m_MaxTimeStep) dt = m_MaxTimeStep;
	
	if (m_bFixedStep) dt = m_FixedStepSize;
	dt *= m_StepScale;
	if (dt <= 0.0) return;
	m_LastTime = cTime;

	if (m_StepMode == smStepFast)
	{
		dWorldStepFast1( m_PID, dt, m_MaxStepFastIter );
	}
	else if (m_StepMode == smNormal)
	{
		dWorldStep( m_PID, dt );
	}
	else if (m_StepMode == smQuickStep)
	{
		dWorldQuickStep( m_PID, dt );
	}

} // PhysicsSystem::Step

void PhysicsSystem::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_ERP	<< m_CFM << m_Gravity << m_StepScale << m_WorldScale << m_MaxTimeStep << 
		Enum2Byte(m_StepMode) << m_MaxStepFastIter;
}

void PhysicsSystem::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_ERP	>> m_CFM >> m_Gravity >> m_StepScale >> m_WorldScale >> m_MaxTimeStep >> 
		Enum2Byte(m_StepMode) >> m_MaxStepFastIter;
}

void PhysicsSystem::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "PhysicsSystem", this );
	pm.p( "Valid", IsValid );
	pm.p( "ERP", GetERP, SetERP );
	pm.p( "CFM", GetCFM, SetCFM );
	pm.f( "StepScale", m_StepScale );
	pm.f( "MaxTimeStep", m_MaxTimeStep );
	pm.p( "WorldScale", GetWorldScale, SetWorldScale );
	pm.p( "GravityX", GetGravityX, SetGravityX );
	pm.p( "GravityY", GetGravityY, SetGravityY );
	pm.p( "GravityZ", GetGravityZ, SetGravityZ );
	pm.f( "FixedStep", m_bFixedStep );
	pm.f( "FixedStepSize", m_FixedStepSize );
	pm.f( "StepMode", m_StepMode );
	pm.f( "MaxStepFastIter", m_MaxStepFastIter );
	pm.m( "SetEarthGravity", SetEarthGravity );
	pm.m( "Step", Step );
} // PhysicsSystem::Expose

void PhysicsSystem::SetEarthGravity()
{
	m_Gravity.x = 0.0;
	m_Gravity.y = 0.0;
	m_Gravity.z = -9.81f;
	SetGravity( m_Gravity );
}

void PhysicsSystem::SetCFM( float val )
{
	m_CFM = val;
	if (m_PID) dWorldSetCFM( m_PID, val );
}

void PhysicsSystem::SetERP( float val )
{
	m_ERP = val;
	if (m_PID) dWorldSetERP( m_PID, val );
}

void PhysicsSystem::SetGravityX( float val )
{
	m_Gravity.x = val;
	SetGravity( m_Gravity );
}

void PhysicsSystem::SetGravityY( float val )
{
	m_Gravity.y = val;
	SetGravity( m_Gravity );
}

void PhysicsSystem::SetGravityZ( float val )
{
	m_Gravity.z = val;
	SetGravity( m_Gravity );
}

void PhysicsSystem::SetGravity( const Vector3D& vec )
{
	m_Gravity = vec;
	Vector3D gr( vec );
	gr *= GetCurWorldScale();
	if (m_PID) dWorldSetGravity( m_PID, gr.x, gr.y, gr.z );
} // PhysicsSystem::SetGravity

/*****************************************************************************/
/*	Body implementation
/*****************************************************************************/
void Articulated::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "Articulated", this );
	pm.m( "Reset",		Reset		);
	pm.p( "ShowBodies",	IsShowBodies, ShowBodies );
	pm.p( "ShowJoints",	IsShowJoints, ShowJoints );
	pm.p( "ShowGeoms",		IsShowGeoms,  ShowGeoms	 );
	pm.m( "Init",		Init		);
} // Articulated::Expose

void Articulated::Init()
{
	RemoveChildren();
	AddChild<Group>			( "Bodies"		);
	AddChild<JointCluster>	( "Joints"		);
	AddChild<CldHashSpace>	( "Collidable"	);
} // Articulated::Init

Articulated::Articulated()
{
	m_bShowJoints = false;
	m_bShowBodies = false;
	m_bShowGeoms  = false;
}

void Articulated::ShowJoints( bool bShow )
{
	Iterator it( this, Joint::FnFilter );
	while (it)
	{
		((Node*)*it)->SetDrawGizmo( bShow );
		++it;
	}
	m_bShowJoints = bShow;
} // Articulated::ShowJoints

void Articulated::ShowGeoms( bool bShow )
{
	Iterator it( this, CldGeom::FnFilter );
	while (it)
	{
		((CldGeom*)*it)->SetDrawGizmo( bShow );
		++it;
	}
	m_bShowGeoms  = bShow;
} // Articulated::ShowGeoms

void Articulated::ShowBodies( bool bShow )
{
	Iterator it( this, Body::FnFilter );
	while (it)
	{
		((Body*)*it)->SetDrawGizmo( bShow );
		++it;
	}
	m_bShowBodies = bShow;
} // Articulated::ShowBodies

void Articulated::Reset()
{
	//  reset transforms
	Iterator trIt( this, TransformNode::FnFilter );
	while (trIt)
	{
		((TransformNode*)*trIt)->SetToInitial();
		++trIt;
	}

	//  clear rigid bodies
	Iterator bIt( this, Body::FnFilter );
	while (bIt)
	{
		((Body*)*bIt)->Reset();
		++bIt;
	}
	//  clear joints
	Iterator jIt( this, Joint::FnFilter );
	while (jIt)
	{
		((Joint*)*jIt)->Reset();
		++jIt;
	}
	//  clear geoms
	Iterator gIt( this, CldGeom::FnFilter );
	while (gIt)
	{
		((CldGeom*)*gIt)->Reset();
		++gIt;
	}
} // Articulated::Reset

/*****************************************************************************/
/*	Body implementation
/*****************************************************************************/
Body::Body()
{
	m_Mass			= 1.0f;
	m_Density		= 1.0f;
	m_PID			= NULL;
	m_Velocity		= Vector3D::null;
	m_AngVelocity	= Vector3D::null;
	m_pTMNode		= NULL;
	m_ScaleVector	= Vector3D::one;
	m_MassCenterTM	= Matrix4D::identity;
	m_Inertia.setIdentity();
}

Body::~Body()
{
	//  FIXME. ODE crashes in there 
	//if (m_PID) dBodyDestroy( m_PID );
}

void Body::Reset()
{
	if (m_PID) dBodyDestroy( m_PID );
	m_PID = NULL;
}

void Body::Render()
{
	m_pTMNode = (TransformNode*)GetInput( 0 );
	if (!m_PID) CreateBody();
	if (!m_PID || !m_pTMNode) return;

	//  update values
	const dVector3* pos = (const dVector3*)dBodyGetPosition( m_PID );
	const dQuaternion* rot = (const dQuaternion*)dBodyGetQuaternion( m_PID );
	Vector3D cpos;
	Quaternion quat;
	Convert( cpos, *pos );
	Convert( quat, *rot );
	cpos /= GetCurWorldScale();
	Matrix3D mrot;
	mrot.rotation( quat );

	Matrix4D tm = Matrix4D( m_ScaleVector, mrot, cpos );
	Matrix4D pTM;
	pTM.inverse( m_pTMNode->GetParentWorldTM() );

	Matrix4D m; m.inverse( m_MassCenterTM );
	tm *= pTM;
	tm.mulLeft( m );
	m_pTMNode->SetTransform( tm );
} // Body::Render



void Body::CreateBody()
{
	if (!m_pTMNode) return;
	m_PID = dBodyCreate( GetCurWorldID() );
	if (m_PID)
	{
		InitBodyTM();
		SetMass( m_Mass );
	}
}  //  Body::CreateBody

Matrix4D Body::GetTransform()
{
	if (!m_pTMNode) return Matrix4D::identity;
	Matrix4D m = m_pTMNode->GetWorldTM();
	m.mulLeft( m_MassCenterTM );
	return m;
}

void Body::InitBodyTM()
{
	if (!m_pTMNode || !m_PID) return;
	
	Matrix4D& tm = GetTransform();
	
	Matrix3D rotTM;
	Vector3D pos;
	tm.Factorize( m_ScaleVector, rotTM, pos );
	pos *= GetCurWorldScale();

	dBodySetPosition( m_PID, pos.x, pos.y, pos.z );
	dQuaternion dq;
	Quaternion quat;
	quat.FromMatrix( rotTM );
	quat.normalize();
	Convert( dq, quat );
	dBodySetQuaternion( m_PID, dq );
} // Body::InitBodyTM

void Body::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Mass << m_Velocity << m_AngVelocity << m_Inertia << m_MassCenterTM;
}

void Body::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Mass >> m_Velocity >> m_AngVelocity >> m_Inertia >> m_MassCenterTM;
}

void Body::SetLocalCMX( float val )
{
	m_MassCenterTM.e30 = val;
	InitBodyTM();
}

void Body::SetLocalCMY( float val )
{
	m_MassCenterTM.e31 = val;
	InitBodyTM();
}

void Body::SetLocalCMZ( float val )
{
	m_MassCenterTM.e32 = val;
	InitBodyTM();
}

float Body::GetLocalCMRotX() const
{
	Matrix3D rot = m_MassCenterTM;
	return RadToDeg( rot.EulerXYZ().x );
} // Body::GetLocalCMRotX

float Body::GetLocalCMRotY() const
{
	Matrix3D rot = m_MassCenterTM;
	return RadToDeg( rot.EulerXYZ().y );
} // Body::GetLocalCMRotY

float Body::GetLocalCMRotZ() const
{
	Matrix3D rot = m_MassCenterTM;
	return RadToDeg( rot.EulerXYZ().z );
} // Body::GetLocalCMRotZ

void Body::SetLocalCMRotX( float val )
{
	clamp( val, -180.0f, 180.0f );
	float sx = m_MassCenterTM.getV0().norm();
	float sy = m_MassCenterTM.getV1().norm();
	float sz = m_MassCenterTM.getV2().norm();

	Matrix3D rot;
	rot.rotation( DegToRad( val ), DegToRad( GetLocalCMRotY() ), DegToRad( GetLocalCMRotZ() ) );
	m_MassCenterTM.setRotation( rot );

	m_MassCenterTM.getV0() *= sx;
	m_MassCenterTM.getV1() *= sy;
	m_MassCenterTM.getV2() *= sz;

	InitBodyTM();
} // Body::SetLocalCMRotX

void Body::SetLocalCMRotY( float val )
{
	clamp( val, -180.0f, 180.0f );
	float sx = m_MassCenterTM.getV0().norm();
	float sy = m_MassCenterTM.getV1().norm();
	float sz = m_MassCenterTM.getV2().norm();

	Matrix3D rot;
	rot.rotation( DegToRad( GetLocalCMRotX() ), DegToRad( val ), DegToRad( GetLocalCMRotZ() ) );
	m_MassCenterTM.setRotation( rot );

	m_MassCenterTM.getV0() *= sx;
	m_MassCenterTM.getV1() *= sy;
	m_MassCenterTM.getV2() *= sz;

	InitBodyTM();
} // Body::SetLocalCMRotY

void Body::SetLocalCMRotZ( float val )
{
	clamp( val, -180.0f, 180.0f );
	float sx = m_MassCenterTM.getV0().norm();
	float sy = m_MassCenterTM.getV1().norm();
	float sz = m_MassCenterTM.getV2().norm();

	Matrix3D rot;
	rot.rotation( DegToRad( GetLocalCMRotX() ), DegToRad( GetLocalCMRotY() ), DegToRad( val ) );
	m_MassCenterTM.setRotation( rot );

	m_MassCenterTM.getV0() *= sx;
	m_MassCenterTM.getV1() *= sy;
	m_MassCenterTM.getV2() *= sz;

	InitBodyTM();
} // Body::SetLocalCMRotZ

float Body::GetLocalCMX() const
{
	return m_MassCenterTM.e30;
}

float Body::GetLocalCMY() const
{
	return m_MassCenterTM.e31;
}	

float Body::GetLocalCMZ() const
{
	return m_MassCenterTM.e32;
}

void Body::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "Body", this );
	pm.m( "Rename", Rename );
	pm.p( "Valid", IsValid );
	pm.p( "Enabled", IsBodyEnabled, EnableBody );
	pm.p( "Mass", GetMass, SetMass );
	pm.p( "DX", GetLocalCMX, SetLocalCMX );
	pm.p( "DY", GetLocalCMY, SetLocalCMY );
	pm.p( "DZ", GetLocalCMZ, SetLocalCMZ );
	pm.p( "RotX", GetLocalCMRotX, SetLocalCMRotX );
	pm.p( "RotY", GetLocalCMRotY, SetLocalCMRotY );
	pm.p( "RotZ", GetLocalCMRotZ, SetLocalCMRotZ );

	pm.f( "VelocityX", m_Velocity.x, NULL, true );
	pm.f( "VelocityY", m_Velocity.y, NULL, true );
	pm.f( "VelocityZ", m_Velocity.z, NULL, true );
	pm.f( "AngVelocityX", m_AngVelocity.x, NULL, true );
	pm.f( "AngVelocityY", m_AngVelocity.y, NULL, true );
	pm.f( "AngVelocityZ", m_AngVelocity.z, NULL, true );

} // Body::Expose

void Body::Rename()
{
	m_pTMNode = (TransformNode*)GetInput( 0 );
	if (m_pTMNode) SetName( m_pTMNode->GetName() );
}

void Body::SetMass( float val )
{
	m_Mass = val;
	if (m_PID)
	{
		dMass mass;
		mass.mass = m_Mass;
		Convert( mass.I, m_Inertia );
		Convert( mass.c, Vector4D( 0, 0, 0, 1 ) );
		dBodySetMass( m_PID, &mass );
	}
} // Body::SetMass

void Body::SetDensity( float val )
{
	m_Density = val;
	if (m_PID)
	{
		dMass mass;
		mass.mass = m_Mass;
		Convert( mass.I, m_Inertia );
		Convert( mass.c, Vector4D( 0, 0, 0, 1 ) );
		dBodySetMass( m_PID, &mass );
	}
} // Body::SetDensity

void Body::SetMass( const dMass& mass )
{
	dBodySetMass( m_PID, &mass );
} // Body::SetMass

void Body::EnableBody( bool bEnable )
{
	if (!m_PID) return;
	if (bEnable)
	{
		dBodyEnable( m_PID );
	}
	else
	{
		dBodyDisable( m_PID );
	}
} // Body::EnableBody

bool Body::IsBodyEnabled() const
{
	return m_PID && dBodyIsEnabled( m_PID ) == 1;
} // Body::IsBodyEnabled

/*****************************************************************************/
/*	Joint implementation
/*****************************************************************************/
Joint::Joint()
{
	m_PID	 		= NULL;
	m_pBody1 		= NULL;
	m_pBody2 		= NULL;
} // Joint::Joint

Joint::~Joint()
{
	//if (m_PID) dJointDestroy( m_PID );
}

void Joint::Reset()
{
	if (m_PID) dJointDestroy( m_PID );
	m_PID = NULL;
}

void Joint::Render()
{
	if (m_PID == NULL) InitJoint();
}

void Joint::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
} // Joint::Serialize

void Joint::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
} // Joint::Unserialize

void Joint::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "Joint", this );
	pm.p( "Valid", IsValid );
	pm.m( "InitJoint",	InitJoint		);
	pm.m( "PositionFromBodies", PositionFromBodies );
} // Joint::Expose

void Joint::InitJoint()
{
	if (m_PID) 
	{
		dJointDestroy( m_PID );
		m_PID = NULL;
	}

	JointCluster* pCluster = FindParentJointCluster();
	if (!pCluster) return;
	if (!m_pBody1) m_pBody1 = (Body*)GetInput( 0 );
	if (!m_pBody2) m_pBody2 = (Body*)GetInput( 1 );
	dBodyID id1 = m_pBody1 ? m_pBody1->GetBodyID() : NULL;
	dBodyID id2 = m_pBody2 ? m_pBody2->GetBodyID() : NULL;
	
	m_PID = CreateJoint( pCluster->GetClusterID() );
	if (!m_PID) return;
	
	if ((!id1 || !id2) && NeedTwoBodies()) return;
	//  attach joint to bodies
	dJointAttach( m_PID, id1, id2 );
	SetParams();

} // Joint::CreateJoint

void Joint::GetFromBodies( Vector3D& anchor, Vector3D& dir1, Vector3D& dir2 )
{
	if (!m_pBody1) m_pBody1 = (Body*)GetInput( 0 );
	if (!m_pBody2) m_pBody2 = (Body*)GetInput( 1 );
	Matrix4D tm1 = m_pBody1 ? m_pBody1->GetTransform() : Matrix4D::identity;
	Matrix4D tm2 = m_pBody2 ? m_pBody2->GetTransform() : Matrix4D::identity;
	
	anchor.add( tm1.getTranslation(), tm2.getTranslation() );
	anchor *= 0.5f;
	dir1.sub( tm1.getTranslation(), anchor );
	dir2.sub( tm2.getTranslation(), anchor );
} // Joint::GetFromBodies

Matrix4D Joint::GetTransform()
{
	TransformNode* pNode = NULL;
	for (int i = 0; i < GetNChildren(); i++)
	{
		if (GetChild( i )->IsA<TransformNode>()) return ((TransformNode*)GetChild( i ))->GetWorldTM();
	}
	return Matrix4D::identity;
} // Joint::GetTransform

JointCluster* Joint::FindParentJointCluster()
{
	Node* pNode = this;
	pNode = pNode->GetParent();
	while (pNode)
	{
		if ( pNode->IsA<JointCluster>() ) return (JointCluster*)pNode;
		pNode = pNode->GetParent();
	}
	return NULL;
} // Joint::FindParentJointCluster

Vector3D Joint::GetDirection1( const Vector3D& pos ) const
{
	if (m_pBody1)
	{
		Vector3D dir;
		dir.sub( m_pBody1->GetTransform().getTranslation(), pos );
		dir.normalize();
		return dir;
	}
	return Vector3D( 1, 0, 0 );
} // Joint::GetDirection1

Vector3D Joint::GetDirection2( const Vector3D& pos ) const
{
	if (m_pBody2)
	{
		Vector3D dir;
		dir.sub( m_pBody2->GetTransform().getTranslation(), pos );
		dir.normalize();
		return dir;
	}
	return Vector3D( -1, 0, 0 );
} // Joint::GetDirection2

/*****************************************************************************/
/*	StopMotorJoint implementation
/*****************************************************************************/
StopMotorJoint::StopMotorJoint()
{
	m_LoStop		= -0.75f;		
	m_HiStop		= 0.75f;		
	m_MotorVel		= 0.0f;			
	m_MotorFMax		= 0.1f;				
	m_FudgeFactor	= 0.5f; 
	m_Bounce		= 0.001f;		
	m_CFM			= 0.001f;			
	m_StopERP		= 0.9f;		
	m_StopCFM		= 0.001f;		
} // StopMotorJoint::StopMotorJoint

void StopMotorJoint::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_LoStop << m_HiStop << m_MotorVel << m_MotorFMax << m_FudgeFactor 
		<< m_Bounce << m_CFM << m_StopERP << m_StopCFM; 
} // StopMotorJoint::Serialize

void StopMotorJoint::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_LoStop >> m_HiStop >> m_MotorVel >> m_MotorFMax >> m_FudgeFactor 
		>> m_Bounce >> m_CFM >> m_StopERP >> m_StopCFM; 
} // StopMotorJoint::Unserialize

void StopMotorJoint::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "StopMotorJoint", this );
	pm.p( "Valid", IsValid );
	pm.m( "InitJoint",	InitJoint		);
	pm.f( "LoStop",		m_LoStop		);
	pm.f( "HiStop",		m_HiStop		);
	pm.f( "StopERP",		m_StopERP		);
	pm.f( "StopCFM",		m_StopCFM		);

	pm.f( "MotorVel",		m_MotorVel		);
	pm.f( "MotorFMax",	m_MotorFMax		);

	pm.f( "Bounce",		m_Bounce		);
	pm.f( "CFM",			m_CFM			);
	pm.f( "FudgeFactor",	m_FudgeFactor	);
} // StopMotorJoint::Expose

/*****************************************************************************/
/*	JointCluster implementation
/*****************************************************************************/
JointCluster::JointCluster()
{
	m_PID		= NULL;
	m_MaxSize	= 8;
}

JointCluster::~JointCluster()
{
	if (m_PID) dJointGroupDestroy( m_PID );
}

void JointCluster::Render()
{
	if (!m_PID)
	{
		m_PID = dJointGroupCreate( 0 );
	}
	Node::Render();
} // JointCluster::Render

void JointCluster::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_MaxSize;
}

void JointCluster::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_MaxSize;
}

void JointCluster::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "JointCluster", this );
	pm.p( "Valid", IsValid );
	pm.f( "MaxSize", m_MaxSize );
	pm.m( "RenameJoints", RenameJoints );
} // JointCluster::Expose

void JointCluster::RenameJoints()
{
	for (int i = 0; i < GetNChildren(); i++)
	{
		Joint* pJoint = (Joint*)GetChild( i );
		if (!pJoint->IsA<Joint>()) continue;
		Node* pB1 = pJoint->GetInput( 0 );
		Node* pB2 = pJoint->GetInput( 1 );
		char cbuf[256]; cbuf[0] = 0;
		if (pB1) strcat( cbuf, pB1->GetName() );
		strcat( cbuf, "." );
		if (pB2) strcat( cbuf, pB2->GetName() );
		pJoint->SetName( cbuf );
	}
} // JointCluster::RenameJoints

/*****************************************************************************/
/*	BallSocketJoint implementation
/*****************************************************************************/
BallSocketJoint::BallSocketJoint()
{
	m_Anchor = Vector3D::null;
}

void BallSocketJoint::PositionFromBodies()
{
	Vector3D d1, d2;
	GetFromBodies( m_Anchor, d1, d2 );

	Body* pBody = (Body*)GetInput( 0 );
	if (pBody)
	{
		Matrix4D tm = pBody->GetTransform();
		tm.inverse();
		tm.transformPt( m_Anchor );
	}
} // BallSocketJoint::PositionFromBodies

void BallSocketJoint::Render()
{
	Parent::Render();
	if (DoDrawGizmo())
	{
		rsEnableZ( PhysicsSystem::s_bEnableGizmoZ );
		IRS->SetWorldTM( Matrix4D::identity );
		rsEnableZ( PhysicsSystem::s_bEnableGizmoZ );
		
		float jW = PhysicsSystem::s_JointHandleSize*0.1f;
		float jL = PhysicsSystem::s_JointHandleSize*0.3f;

		dVector3 dAnchor;
		dJointGetBallAnchor( m_PID, dAnchor );
		Vector3D anchor;
		Convert( anchor, dAnchor );
		anchor /= GetCurWorldScale();

		DrawSphere( Sphere( anchor, jL ), 0, PhysicsSystem::s_JointAnchorColor, 20 );
		Vector3D d = GetDirection1( anchor );
		d *= jL;
		Vector3D a( anchor ), b( anchor );
		a += d; b.addWeighted( d, 2.0f ); b -= a;
		DrawCylinder( Cylinder( Segment3D( a, b ), jW ), 
								PhysicsSystem::s_JointAxis1ColorF, PhysicsSystem::s_JointAxis1Color, false );

		d = GetDirection2( anchor );
		d *= jL;
		a = anchor; b = anchor;
		a += d; b.addWeighted( d, 2.0f ); b -= a;
		DrawCylinder( Cylinder( Segment3D( a, b ), jW ), 
						PhysicsSystem::s_JointAxis2ColorF, PhysicsSystem::s_JointAxis2Color, false );

		rsFlushLines3D();

		SetParams();
	}
} // BallSocketJoint::Render

void BallSocketJoint::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Anchor;
}

void BallSocketJoint::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Anchor;
}

void BallSocketJoint::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "BallSocketJoint", this );
	pm.f( "AnchorX", m_Anchor.x );
	pm.f( "AnchorY", m_Anchor.y );
	pm.f( "AnchorZ", m_Anchor.z );
}

dJointID BallSocketJoint::CreateJoint( dJointGroupID groupID )
{
	return dJointCreateBall( GetCurWorldID(), groupID );
}

void BallSocketJoint::SetParams()
{
	if (!m_PID) return;
	Vector3D anchor = m_Anchor; 
	Body* pBody = (Body*)GetInput( 0 );
	if (pBody)
	{
		pBody->GetTransform().transformPt( anchor );
	}
	anchor *= GetCurWorldScale();
	dJointSetBallAnchor( m_PID, anchor.x, anchor.y, anchor.z );
} // BallSocketJoint::SetParams

/*****************************************************************************/
/*	SliderJoint implementation
/*****************************************************************************/
SliderJoint::SliderJoint()
{
	m_Axis = Vector3D::oX;
}

void SliderJoint::PositionFromBodies()
{
	Vector3D anchor, d1, d2;
	GetFromBodies( anchor, d1, d2 );
	m_Axis.sub( d2, d1 );
	m_Axis.normalize();
}

Vector3D SliderJoint::GetAnchor() const
{
	return Vector3D::null;
}

void SliderJoint::Render()
{
	Parent::Render();
	if (DoDrawGizmo())
	{
		rsEnableZ( PhysicsSystem::s_bEnableGizmoZ );
		IRS->SetWorldTM( Matrix4D::identity );
		rsEnableZ( PhysicsSystem::s_bEnableGizmoZ );
		Vector3D dir1 = m_Axis;
		Vector3D dir2 = m_Axis;

		dir1 *= PhysicsSystem::s_JointHandleSize*1.5f;
		dir2 *= -PhysicsSystem::s_JointHandleSize*2.0f;

		Segment3D seg1( GetAnchor(), dir1 );
		Vector3D org( GetAnchor() ); org.addWeighted( m_Axis, PhysicsSystem::s_JointHandleSize );
		Segment3D seg2( org, dir2 );

		DrawCylinder( Cylinder( seg1, PhysicsSystem::s_JointHandleSize*0.4f ), PhysicsSystem::s_JointAxis1ColorF, PhysicsSystem::s_JointAxis1Color, false );
		DrawCylinder( Cylinder( seg2, PhysicsSystem::s_JointHandleSize*0.3f ), PhysicsSystem::s_JointAxis2ColorF, PhysicsSystem::s_JointAxis2Color, false );
		rsFlushLines3D();

		SetParams();
	}
} // SliderJoint::Render

void SliderJoint::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Axis;
}

void SliderJoint::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Axis;
}

void SliderJoint::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "SliderJoint", this );
	pm.p( "SliderPosition", GetSliderPosition );
	pm.p( "SliderPositionRate", GetSliderPositionRate );
	pm.f( "AxisX", m_Axis.x );
	pm.f( "AxisY", m_Axis.y );
	pm.f( "AxisZ", m_Axis.z );
} // SliderJoint::Expose

dJointID SliderJoint::CreateJoint( dJointGroupID groupID )
{
	return dJointCreateBall( GetCurWorldID(), groupID );
}

float SliderJoint::GetSliderPosition() const
{
	return m_PID ? dJointGetSliderPosition( m_PID ) : 0.0f;
}

float SliderJoint::GetSliderPositionRate() const
{
	return m_PID ? dJointGetSliderPositionRate( m_PID ) : 0.0f;
}

void SliderJoint::SetParams()
{
	if (!m_PID) return;

	Matrix4D tm = GetTransform();
	m_Axis = tm.getV2();

	dJointSetSliderParam( m_PID, dParamLoStop,			m_LoStop		);
	dJointSetSliderParam( m_PID, dParamHiStop,			m_HiStop		);
	dJointSetSliderParam( m_PID, dParamStopERP,			m_StopERP		);
	dJointSetSliderParam( m_PID, dParamStopCFM,			m_StopCFM		);
	dJointSetSliderParam( m_PID, dParamVel,				m_MotorVel		);
	dJointSetSliderParam( m_PID, dParamFMax,			m_MotorFMax		);
	dJointSetSliderParam( m_PID, dParamFudgeFactor,		m_FudgeFactor	);
	dJointSetSliderParam( m_PID, dParamBounce,			m_Bounce		);
	dJointSetSliderParam( m_PID, dParamCFM,				m_CFM			);

	dJointSetSliderAxis ( m_PID, m_Axis.x, m_Axis.y, m_Axis.z );
} // SliderJoint::SetParams

/*****************************************************************************/
/*	HingeJoint implementation
/*****************************************************************************/
HingeJoint::HingeJoint()
{
	m_Anchor = Vector3D::null;
	m_Axis = Vector3D::oZ;
}

void HingeJoint::PositionFromBodies()
{
	Vector3D d1, d2;
	GetFromBodies( m_Anchor, d1, d2 );
	d1.normalize();
	d2.normalize();
	m_Axis.cross( d1, d2 );
	if (fabs( m_Axis.normalize() ) < c_Epsilon)
	{
		d1 = Vector3D::oX;
		m_Axis.cross( d1, d2 );
		m_Axis.normalize();
	}

	Body* pBody = (Body*)GetInput( 0 );
	if (pBody)
	{
		Matrix4D tm = pBody->GetTransform();
		tm.inverse();
		tm.transformPt( m_Anchor );
		tm.transformVec( m_Axis );
	}
} // HingeJoint::PositionFromBodies

void HingeJoint::Render()
{
	Parent::Render();
	if (DoDrawGizmo())
	{
		rsEnableZ( PhysicsSystem::s_bEnableGizmoZ );
		IRS->SetWorldTM( Matrix4D::identity );
		rsEnableZ( PhysicsSystem::s_bEnableGizmoZ );
		Vector3D a, b, c, d;
		
		dVector3 dAxis, dAnchor;
		dJointGetHingeAxis( m_PID, dAxis );
		dJointGetHingeAnchor( m_PID, dAnchor );
		Vector3D anchor, axis;
		Convert( anchor, dAnchor );
		Convert( axis, dAxis );
		anchor /= GetCurWorldScale();

		float jW = PhysicsSystem::s_JointHandleSize * 0.3f;
		float jH = PhysicsSystem::s_JointHandleSize * 0.5f;

		//  left
		Vector3D dir1 = GetDirection1( anchor );
		a = anchor; a.addWeighted( dir1, jH );
		a.addWeighted( axis, jW );
		b = anchor; b.addWeighted( dir1, jH );
		b.addWeighted( axis, -jW );
		c = anchor; c.addWeighted( axis, jW );
		d = anchor; d.addWeighted( axis, -jW );
		rsQuad( a, b, c, d, PhysicsSystem::s_JointAxis1ColorF );
		rsQuad( b, a, d, c, PhysicsSystem::s_JointAxis1ColorF );

		//  right
		Vector3D dir2 = GetDirection2( anchor );
		a = anchor; a.addWeighted( dir2, jH );
		a.addWeighted( axis, jW );
		b = anchor; b.addWeighted( dir2, jH );
		b.addWeighted( axis, -jW );
		c = anchor; c.addWeighted( axis, jW );
		d = anchor; d.addWeighted( axis, -jW );
		rsQuad( a, b, c, d, PhysicsSystem::s_JointAxis2ColorF );
		rsQuad( b, a, d, c, PhysicsSystem::s_JointAxis2ColorF );

		//  anchor
		a = anchor; a.addWeighted( axis, -jW );
		d = axis; d *= jW*2.0f;
		DrawCylinder( Cylinder( Segment3D( a, d ), jH*0.2f ), 
						PhysicsSystem::s_JointAnchorColorF, PhysicsSystem::s_JointAnchorColor, false );
		rsFlushLines3D();
		rsFlushPoly3D();

		SetParams();
	}
} // HingeJoint::Render

void HingeJoint::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Axis << m_Anchor;
}

void HingeJoint::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Axis >> m_Anchor;
}

void HingeJoint::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "HingeJoint", this );
	pm.f( "AnchorX", m_Anchor.x );
	pm.f( "AnchorY", m_Anchor.y );
	pm.f( "AnchorZ", m_Anchor.z );
	pm.f( "AxisX", m_Axis.x );
	pm.f( "AxisY", m_Axis.y );
	pm.f( "AxisZ", m_Axis.z );
}

dJointID HingeJoint::CreateJoint( dJointGroupID groupID )
{
	return dJointCreateHinge( GetCurWorldID(), groupID );
}

float HingeJoint::GetAngle() const
{
	return m_PID ? dJointGetHingeAngle( m_PID ) : 0.0f;
}

float HingeJoint::GetAngleRate() const
{
	return m_PID ? dJointGetHingeAngleRate( m_PID ) : 0.0f;
}

void HingeJoint::SetParams()
{
	if (!m_PID) return;

	dJointSetHingeParam( m_PID, dParamLoStop,			m_LoStop		);
	dJointSetHingeParam( m_PID, dParamHiStop,			m_HiStop		);
	dJointSetHingeParam( m_PID, dParamStopERP,			m_StopERP		);
	dJointSetHingeParam( m_PID, dParamStopCFM,			m_StopCFM		);
	dJointSetHingeParam( m_PID, dParamVel,				m_MotorVel		);
	dJointSetHingeParam( m_PID, dParamFMax,				m_MotorFMax		);
	dJointSetHingeParam( m_PID, dParamFudgeFactor,		m_FudgeFactor	);
	dJointSetHingeParam( m_PID, dParamBounce,			m_Bounce		);
	dJointSetHingeParam( m_PID, dParamCFM,				m_CFM			);

	Vector3D anchor = m_Anchor; 
	Vector3D axis = m_Axis; 
	Body* pBody = (Body*)GetInput( 0 );
	if (pBody)
	{
		Matrix4D tm = pBody->GetTransform();
		tm.transformPt( anchor );
		tm.transformVec( axis );
	}
	anchor *= GetCurWorldScale();
	
	dJointSetHingeAnchor( m_PID, anchor.x, anchor.y, anchor.z );
	dJointSetHingeAxis  ( m_PID, axis.x, axis.y, axis.z );
} // HingeJoint::SetParams

/*****************************************************************************/
/*	Hinge2Joint implementation
/*****************************************************************************/
Hinge2Joint::Hinge2Joint()
{
	m_SuspensionERP	= 0.4;
	m_SuspensionCFM	= 0.8;

	m_MotorVel2		= 0.0f;
	m_MotorFMax2	= 0.0f;		

	m_Anchor		= Vector3D::null;		
	m_Axis1			= Vector3D::oX;		
	m_Axis2			= Vector3D::oZ;		
} // Hinge2Joint::Hinge2Joint

void Hinge2Joint::PositionFromBodies()
{
	Vector3D d1, d2;
	GetFromBodies( m_Anchor, d1, d2 );
	d1.normalize();
	d2.normalize();
	m_Axis1.cross( d1, d2 );
	if (fabs( m_Axis1.normalize() ) < c_Epsilon)
	{
		d1 = Vector3D::oX;
		m_Axis1.cross( d1, d2 );
		m_Axis1.normalize();
	}
	m_Axis2.cross( d1, m_Axis1 );
	m_Axis2.normalize();

	Body* pBody = (Body*)GetInput( 0 );
	if (pBody)
	{
		Matrix4D tm = pBody->GetTransform();
		tm.inverse();
		tm.transformPt( m_Anchor );
		tm.transformVec( m_Axis1 );
		tm.transformVec( m_Axis2 );
	}
} // Hinge2Joint::PositionFromBodies

void Hinge2Joint::Render()
{
	Parent::Render();
	if (DoDrawGizmo())
	{
		if (!m_PID) return;
		dVector3 danchor, daxis1, daxis2;
		Vector3D anchor, axis1, axis2;
		dJointGetHinge2Anchor( m_PID, danchor );
		dJointGetHinge2Axis1 ( m_PID, daxis1 );
		dJointGetHinge2Axis2 ( m_PID, daxis2 );

		Convert( anchor, danchor );
		Convert( axis1,  daxis1  );
		Convert( axis2,  daxis2  );
		anchor /= GetCurWorldScale();

		IRS->SetWorldTM( Matrix4D::identity );
		rsEnableZ( PhysicsSystem::s_bEnableGizmoZ );
		Vector3D dir1 = axis2;
		Vector3D dir2 = axis1;
		Vector3D dir3 = axis2;

		dir1 *= PhysicsSystem::s_JointHandleSize*0.8f;
		dir2 *= PhysicsSystem::s_JointHandleSize*2.0f;
		dir3 *= PhysicsSystem::s_JointHandleSize*2.0f;

		Vector3D org1( anchor ); org1.addWeighted( axis2, -PhysicsSystem::s_JointHandleSize*0.4f );
		Segment3D seg1( org1, dir1 );
		
		Vector3D org2( anchor ); org2.addWeighted( axis1, PhysicsSystem::s_JointHandleSize*0.5f );
		Segment3D seg2( org2, dir2 );

		Vector3D org3( anchor ); org3.addWeighted( axis2, -PhysicsSystem::s_JointHandleSize*0.4f );
		Segment3D seg3( org3, dir3 );

		DrawCylinder( Cylinder( seg1, PhysicsSystem::s_JointHandleSize*0.5f ), PhysicsSystem::s_JointAnchorColorF, PhysicsSystem::s_JointAnchorColor, false );
		DrawCylinder( Cylinder( seg2, PhysicsSystem::s_JointHandleSize*0.2f ), PhysicsSystem::s_JointAxis1ColorF,  PhysicsSystem::s_JointAxis1Color,  false );
		DrawCylinder( Cylinder( seg3, PhysicsSystem::s_JointHandleSize*0.2f ), PhysicsSystem::s_JointAxis2ColorF,  PhysicsSystem::s_JointAxis2Color,  false );

		rsFlushLines3D();
		rsFlushPoly3D();

		SetParams();
	}
} //  Hinge2Joint::Render

void Hinge2Joint::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_SuspensionERP << m_SuspensionCFM << m_MotorVel2	<< m_MotorFMax2 << 
		m_Anchor << m_Axis1 << m_Axis2;
}

void Hinge2Joint::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_SuspensionERP >> m_SuspensionCFM >> m_MotorVel2	>> m_MotorFMax2 >>
		m_Anchor >> m_Axis1 >> m_Axis2;
}

void Hinge2Joint::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "Hinge2Joint", this );
	pm.f( "SuspensionERP", m_SuspensionERP );
	pm.f( "SuspensionCFM", m_SuspensionCFM );
	pm.f( "MotorVel2", m_MotorVel2 );
	pm.f( "FMax2", m_MotorFMax2 );
	pm.p( "Angle1", GetAngle1 );
	pm.p( "Angle1Rate", GetAngle1Rate );
	pm.p( "Angle2Rate", GetAngle2Rate );
	pm.f( "AnchorX", m_Anchor.x );
	pm.f( "AnchorY", m_Anchor.y );
	pm.f( "AnchorZ", m_Anchor.z );
	pm.f( "Axis1X", m_Axis1.x );
	pm.f( "Axis1Y", m_Axis1.y );
	pm.f( "Axis1Z", m_Axis1.z );
	pm.f( "Axis2X", m_Axis2.x );
	pm.f( "Axis2Y", m_Axis2.y );
	pm.f( "Axis2Z", m_Axis2.z );
} // Hinge2Joint::Expose

dJointID Hinge2Joint::CreateJoint( dJointGroupID groupID )
{
	return dJointCreateHinge2( GetCurWorldID(), groupID );
}

float Hinge2Joint::GetAngle1() const
{
	return m_PID ? dJointGetHinge2Angle1( m_PID ) : 0.0f;
}

float Hinge2Joint::GetAngle1Rate() const
{
	return m_PID ? dJointGetHinge2Angle1Rate( m_PID ) : 0.0f;
}

float Hinge2Joint::GetAngle2Rate() const
{
	return m_PID ? dJointGetHinge2Angle2Rate( m_PID ) : 0.0f;
}

void Hinge2Joint::SetParams()
{
	if (!m_PID) return;
	dJointSetHinge2Param( m_PID, dParamSuspensionERP,	m_SuspensionERP	);
	dJointSetHinge2Param( m_PID, dParamSuspensionCFM,	m_SuspensionCFM	);
	dJointSetHinge2Param( m_PID, dParamLoStop,			m_LoStop		);
	dJointSetHinge2Param( m_PID, dParamHiStop,			m_HiStop		);
	dJointSetHinge2Param( m_PID, dParamStopERP,			m_StopERP		);
	dJointSetHinge2Param( m_PID, dParamStopCFM,			m_StopCFM		);
	dJointSetHinge2Param( m_PID, dParamVel,				m_MotorVel		);
	dJointSetHinge2Param( m_PID, dParamFMax,			m_MotorFMax		);
	dJointSetHinge2Param( m_PID, dParamFudgeFactor,		m_FudgeFactor	);
	dJointSetHinge2Param( m_PID, dParamBounce,			m_Bounce		);
	dJointSetHinge2Param( m_PID, dParamCFM,				m_CFM			);

	dJointSetHinge2Param( m_PID, dParamVel2,			m_MotorVel		);
	dJointSetHinge2Param( m_PID, dParamFMax2,			m_MotorFMax		);


	Vector3D axis1  = m_Axis1;
	Vector3D axis2  = m_Axis2;
	Vector3D anchor = m_Anchor;

	Body* pBody = (Body*)GetInput( 0 );
	if (pBody)
	{
		Matrix4D tm = pBody->GetTransform();
		tm.transformVec( axis1 );
		tm.transformVec( axis2 );
		tm.transformPt( anchor );
	}
	axis1.normalize();
	axis2.normalize();	
	anchor *= GetCurWorldScale();
	
	dJointSetHinge2Anchor( m_PID, anchor.x, anchor.y, anchor.z );
	dJointSetHinge2Axis1 ( m_PID, axis1.x, axis1.y, axis1.z );
	dJointSetHinge2Axis2 ( m_PID, axis2.x, axis2.y, axis2.z );
} // Hinge2Joint::SetParams

void Hinge2Joint::GetParams()
{
	if (!m_PID) return;
	dVector3 anchor, axis1, axis2;
	dJointGetHinge2Anchor( m_PID, anchor );
	dJointGetHinge2Axis1 ( m_PID, axis1 );
	dJointGetHinge2Axis2 ( m_PID, axis2 );
	
	Convert( m_Anchor, anchor );
	Convert( m_Axis1,  axis1  );
	Convert( m_Axis2,  axis2  );
	m_Anchor /= GetCurWorldScale();
} // Hinge2Joint::GetParams

/*****************************************************************************/
/*	UniversalJoint implementation
/*****************************************************************************/
UniversalJoint::UniversalJoint()
{
}

void UniversalJoint::Render()
{
	Parent::Render();
	if (DoDrawGizmo())
	{
		rsEnableZ( PhysicsSystem::s_bEnableGizmoZ );
		IRS->SetWorldTM( Matrix4D::identity );
		rsEnableZ( PhysicsSystem::s_bEnableGizmoZ );
		
		float jW = PhysicsSystem::s_JointHandleSize*0.1f;
		float jL = PhysicsSystem::s_JointHandleSize*0.3f;

		dVector3 dAnchor;
		dJointGetUniversalAnchor( m_PID, dAnchor );
		Vector3D anchor;
		Convert( anchor, dAnchor );
		anchor /= GetCurWorldScale();

		DrawAABB( AABoundBox( anchor, jL ), PhysicsSystem::s_JointAnchorColorF, PhysicsSystem::s_JointAnchorColor );
		Vector3D d = GetDirection1( anchor );
		d *= jL;
		Vector3D a( anchor ), b( anchor );
		a += d; b.addWeighted( d, 2.0f ); b -= a;
		DrawCylinder( Cylinder( Segment3D( a, b ), jW ), 
								PhysicsSystem::s_JointAxis1ColorF, PhysicsSystem::s_JointAxis1Color, false );

		d = GetDirection2( anchor );
		d *= jL;
		a = anchor; b = anchor;
		a += d; b.addWeighted( d, 2.0f ); b -= a;
		DrawCylinder( Cylinder( Segment3D( a, b ), jW ), 
						PhysicsSystem::s_JointAxis2ColorF, PhysicsSystem::s_JointAxis2Color, false );

		rsFlushLines3D();

		SetParams();
	}
} // UniversalJoint::Render

void UniversalJoint::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Axis1 << m_Axis2 << m_Anchor;
}

void UniversalJoint::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Axis1 >> m_Axis2 >> m_Anchor;
}

void UniversalJoint::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "UniversalJoint", this );
	pm.f( "AnchorX", m_Anchor.x );
	pm.f( "AnchorY", m_Anchor.y );
	pm.f( "AnchorZ", m_Anchor.z );
	pm.f( "Axis1X", m_Axis1.x );
	pm.f( "Axis1Y", m_Axis1.y );
	pm.f( "Axis1Z", m_Axis1.z );
	pm.f( "Axis2X", m_Axis2.x );
	pm.f( "Axis2Y", m_Axis2.y );
	pm.f( "Axis2Z", m_Axis2.z );
}

dJointID UniversalJoint::CreateJoint( dJointGroupID groupID )
{
	return dJointCreateUniversal( GetCurWorldID(), groupID );
}

void UniversalJoint::PositionFromBodies()
{
	Vector3D d1, d2;
	GetFromBodies( m_Anchor, d1, d2 );
	d1.normalize();
	d2.normalize();
	m_Axis1.cross( d1, d2 );
	if (fabs( m_Axis1.normalize() ) < c_Epsilon)
	{
		d1 = Vector3D::oX;
		m_Axis1.cross( d1, d2 );
		m_Axis1.normalize();
	}
	m_Axis2.cross( d1, m_Axis1 );
	m_Axis2.normalize();

	Body* pBody = (Body*)GetInput( 0 );
	if (pBody)
	{
		Matrix4D tm = pBody->GetTransform();
		tm.inverse();
		tm.transformPt( m_Anchor );
		tm.transformVec( m_Axis1 );
		tm.transformVec( m_Axis2 );
	}
} // UniversalJoint::PositionFromBodies

void UniversalJoint::SetParams()
{
	if (!m_PID) return;
	dJointSetUniversalParam( m_PID, dParamLoStop,			m_LoStop		);
	dJointSetUniversalParam( m_PID, dParamHiStop,			m_HiStop		);
	dJointSetUniversalParam( m_PID, dParamStopERP,			m_StopERP		);
	dJointSetUniversalParam( m_PID, dParamStopCFM,			m_StopCFM		);
	dJointSetUniversalParam( m_PID, dParamVel,				m_MotorVel		);
	dJointSetUniversalParam( m_PID, dParamFMax,				m_MotorFMax		);
	dJointSetUniversalParam( m_PID, dParamFudgeFactor,		m_FudgeFactor	);
	dJointSetUniversalParam( m_PID, dParamBounce,			m_Bounce		);
	dJointSetUniversalParam( m_PID, dParamCFM,				m_CFM			);

	Vector3D anchor = m_Anchor; 
	Vector3D axis1  = m_Axis1;
	Vector3D axis2  = m_Axis2;

	Body* pBody = (Body*)GetInput( 0 );
	if (pBody)
	{
		Matrix4D tm = pBody->GetTransform();
		tm.transformPt( anchor );
		tm.transformVec( axis1 );
		tm.transformVec( axis2 );
	}
	anchor *= GetCurWorldScale();
	
	dJointSetUniversalAnchor( m_PID, anchor.x, anchor.y, anchor.z );
	dJointSetUniversalAxis1 ( m_PID, axis1.x, axis1.y, axis1.z );
	dJointSetUniversalAxis2 ( m_PID, axis2.x, axis2.y, axis2.z );
} // UniversalJoint::SetParams

/*****************************************************************************/
/*	FixedJoint implementation
/*****************************************************************************/
FixedJoint::FixedJoint()
{
}

void FixedJoint::Render()
{
	Parent::Render();
}

void FixedJoint::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
}

void FixedJoint::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
}

void FixedJoint::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "FixedJoint", this );
}

dJointID FixedJoint::CreateJoint( dJointGroupID groupID )
{
	return dJointCreateFixed( GetCurWorldID(), groupID );
}

/*****************************************************************************/
/*	ContactJoint implementation
/*****************************************************************************/
ContactJoint::ContactJoint()
{
}

void ContactJoint::Render()
{
	Parent::Render();
}

void ContactJoint::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
}

void ContactJoint::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
}

void ContactJoint::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "ContactJoint", this );
}

dJointID ContactJoint::CreateJoint( dJointGroupID groupID )
{
	dContact contact;
	return dJointCreateContact( GetCurWorldID(), 
								groupID, &contact );
}

/*****************************************************************************/
/*	AMotorJoint implementation
/*****************************************************************************/
AMotorJoint::AMotorJoint()
{
	m_Axis1 = Vector3D::oX;
	m_Axis2 = Vector3D::oY;

	m_LoStop2		= -0.75f;		
	m_HiStop2		= 0.75f;		
	m_MotorVel2		= 0.0f;			
	m_MotorFMax2	= 0.1f;				
	m_FudgeFactor2	= 0.5f; 
	m_Bounce2		= 0.001f;		
	m_CFM2			= 0.001f;			
	m_StopERP2		= 0.9f;		
	m_StopCFM2		= 0.001f;		

	m_LoStop3		= -0.75f;		
	m_HiStop3		= 0.75f;		
	m_MotorVel3		= 0.0f;			
	m_MotorFMax3	= 0.1f;				
	m_FudgeFactor3	= 0.5f; 
	m_Bounce3		= 0.001f;		
	m_CFM3			= 0.001f;			
	m_StopERP3		= 0.9f;		
	m_StopCFM3		= 0.001f;		
} // AMotorJoint::AMotorJoint

void AMotorJoint::PositionFromBodies()
{
	Vector3D d1, d2, anchor;
	GetFromBodies( anchor, d1, d2 );
	d1.normalize();
	d2.normalize();
	m_Axis1.cross( d1, d2 );
	if (fabs( m_Axis1.normalize() ) < c_Epsilon)
	{
		d1 = Vector3D::oX;
		m_Axis1.cross( d1, d2 );
		m_Axis1.normalize();
	}
	m_Axis2.cross( d1, m_Axis1 );
	m_Axis2.normalize();

	Body* pBody = (Body*)GetInput( 0 );
	if (pBody)
	{
		Matrix4D tm = pBody->GetTransform();
		tm.inverse();
		tm.transformVec( m_Axis1 );
		tm.transformVec( m_Axis2 );
	}
} // AMotorJoint::PositionFromBodies

void AMotorJoint::Render()
{
	Parent::Render();
} // AMotorJoint::Render

void AMotorJoint::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Axis1 << m_Axis2 << 
		m_LoStop2 << m_HiStop2 << m_MotorVel2 << m_MotorFMax2 << m_FudgeFactor2 << 
		m_Bounce2 << m_CFM2 << m_StopERP2 << m_StopCFM2 << 
		m_LoStop3 << m_HiStop3 << m_MotorVel3 << m_MotorFMax3 << m_FudgeFactor3 << 
		m_Bounce3 << m_CFM3 << m_StopERP3 << m_StopCFM3;
} // AMotorJoint::Serialize

void AMotorJoint::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Axis1 >> m_Axis2 >> 
		m_LoStop2 >> m_HiStop2 >> m_MotorVel2 >> m_MotorFMax2 >> m_FudgeFactor2 >> 
		m_Bounce2 >> m_CFM2 >> m_StopERP2 >> m_StopCFM2 >> 
		m_LoStop3 >> m_HiStop3 >> m_MotorVel3 >> m_MotorFMax3 >> m_FudgeFactor3 >> 
		m_Bounce3 >> m_CFM3 >> m_StopERP3 >> m_StopCFM3;
} // AMotorJoint::Unserialize

void AMotorJoint::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "AMotorJoint", this );

	pm.f( "LoStop2",		m_LoStop2		);
	pm.f( "HiStop2",		m_HiStop2		);
	pm.f( "StopERP2",		m_StopERP2		);
	pm.f( "StopCFM2",		m_StopCFM2		);

	pm.f( "MotorVel2",	m_MotorVel2		);
	pm.f( "MotorFMax2",	m_MotorFMax2	);

	pm.f( "Bounce2",		m_Bounce2		);
	pm.f( "CFM2",			m_CFM2			);
	pm.f( "FudgeFactor2",	m_FudgeFactor2	);

	pm.f( "LoStop3",		m_LoStop3		);
	pm.f( "HiStop3",		m_HiStop3		);
	pm.f( "StopERP3",		m_StopERP3		);
	pm.f( "StopCFM3",		m_StopCFM3		);

	pm.f( "MotorVel3",	m_MotorVel3		);
	pm.f( "MotorFMax3",	m_MotorFMax3	);

	pm.f( "Bounce3",		m_Bounce3		);
	pm.f( "CFM3",			m_CFM3			);
	pm.f( "FudgeFactor3",	m_FudgeFactor3	);

	pm.f( "Axis1.x",	m_Axis1.x );
	pm.f( "Axis1.y",	m_Axis1.y );
	pm.f( "Axis1.z",	m_Axis1.z );
	pm.f( "Axis2.x",	m_Axis2.x );
	pm.f( "Axis2.y",	m_Axis2.y );
	pm.f( "Axis2.z",	m_Axis2.z );

} // AMotorJoint::Expose

dJointID AMotorJoint::CreateJoint( dJointGroupID groupID )
{
	return dJointCreateAMotor( GetCurWorldID(), groupID );
}

void AMotorJoint::SetParams()
{
	if (!m_PID) return;
	dJointSetAMotorParam( m_PID, dParamLoStop,			m_LoStop		);
	dJointSetAMotorParam( m_PID, dParamHiStop,			m_HiStop		);
	dJointSetAMotorParam( m_PID, dParamStopERP,			m_StopERP		);
	dJointSetAMotorParam( m_PID, dParamStopCFM,			m_StopCFM		);
	dJointSetAMotorParam( m_PID, dParamVel,				m_MotorVel		);
	dJointSetAMotorParam( m_PID, dParamFMax,			m_MotorFMax		);
	dJointSetAMotorParam( m_PID, dParamFudgeFactor,		m_FudgeFactor	);
	dJointSetAMotorParam( m_PID, dParamBounce,			m_Bounce		);
	dJointSetAMotorParam( m_PID, dParamCFM,				m_CFM			);

	dJointSetAMotorParam( m_PID, dParamLoStop2,			m_LoStop2		);
	dJointSetAMotorParam( m_PID, dParamHiStop2,			m_HiStop2		);
	dJointSetAMotorParam( m_PID, dParamStopERP2,		m_StopERP2		);
	dJointSetAMotorParam( m_PID, dParamStopCFM2,		m_StopCFM2		);
	dJointSetAMotorParam( m_PID, dParamVel2,			m_MotorVel2		);
	dJointSetAMotorParam( m_PID, dParamFMax2,			m_MotorFMax2	);
	dJointSetAMotorParam( m_PID, dParamFudgeFactor2,	m_FudgeFactor2	);
	dJointSetAMotorParam( m_PID, dParamBounce2,			m_Bounce2		);
	dJointSetAMotorParam( m_PID, dParamCFM2,			m_CFM2			);

	dJointSetAMotorParam( m_PID, dParamLoStop3,			m_LoStop3		);
	dJointSetAMotorParam( m_PID, dParamHiStop3,			m_HiStop3		);
	dJointSetAMotorParam( m_PID, dParamStopERP3,		m_StopERP3		);
	dJointSetAMotorParam( m_PID, dParamStopCFM3,		m_StopCFM3		);
	dJointSetAMotorParam( m_PID, dParamVel3,			m_MotorVel3		);
	dJointSetAMotorParam( m_PID, dParamFMax3,			m_MotorFMax3	);
	dJointSetAMotorParam( m_PID, dParamFudgeFactor3,	m_FudgeFactor3	);
	dJointSetAMotorParam( m_PID, dParamBounce3,			m_Bounce3		);
	dJointSetAMotorParam( m_PID, dParamCFM3,			m_CFM3			);

	dJointSetAMotorNumAxes( m_PID, 3 );
	dJointSetAMotorMode( m_PID, dAMotorEuler );

	Vector3D axis1  = m_Axis1;
	Vector3D axis2  = m_Axis2;

	Body* pBody = (Body*)GetInput( 0 );
	if (pBody)
	{
		Matrix4D tm = pBody->GetTransform();
		tm.transformVec( axis1 );
		tm.transformVec( axis2 );
	}
	axis1.normalize();
	axis2.normalize();
	dJointSetAMotorAxis( m_PID, 0, 1, axis1.x, axis1.y, axis1.z );
	dJointSetAMotorAxis( m_PID, 2, 2, axis2.x, axis2.y, axis2.z );

} // AMotorJoint::SetParams

/*****************************************************************************/
/*	CldSkeletalJoint implementation
/*****************************************************************************/
SkeletalJoint::SkeletalJoint()
{

}

void SkeletalJoint::Render()
{

}

void SkeletalJoint::Serialize( OutStream& os ) const
{

}

void SkeletalJoint::Unserialize( InStream& is  )
{

}

void SkeletalJoint::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "SkeletalJoint", this );
}

dJointID SkeletalJoint::CreateJoint( dJointGroupID groupID )
{
	return NULL;
}

void SkeletalJoint::SetParams()
{

}

/*****************************************************************************/
/*	CldGeom implementation
/*****************************************************************************/
CldGeom::CldGeom()  : m_PID(NULL)
{
}

CldGeom::~CldGeom()
{
	//if (m_PID) dGeomDestroy( m_PID );
}

void CldGeom::Reset()
{
	if (m_PID) dGeomDestroy( m_PID );
	m_PID = NULL;
} // CldGeom::Reset

void CldGeom::Render()
{
	if (!m_PID)
	{
		CldSpace* pSpace = FindParentSpace();
		if (!pSpace || !pSpace->GetSpaceID()) return;
		m_PID = CreateGeom( pSpace->GetSpaceID() );
		if (!m_PID) return;

		dGeomEnable( m_PID );
		Body* pBody = GetAttachedBody();
		if (pBody) 
		{
			dGeomSetBody( m_PID, pBody->GetBodyID() );
		}
		else if (IsPlaceable())
		{
			InitGeomTM(); 
		}
	}
} // CldGeom::Render

void CldGeom::InitGeomTM()
{
	const Matrix4D tm = GetTransform();
	Matrix3D rotTM;
	Vector3D pos, scale;
	tm.Factorize( scale, rotTM, pos );
	pos *= GetCurWorldScale();

	dGeomSetPosition( m_PID, pos.x, pos.y, pos.z );
	dQuaternion dq;
	Quaternion quat;
	quat.FromMatrix( rotTM );
	quat.normalize();
	Convert( dq, quat );
	dGeomSetQuaternion( m_PID, dq );
} // CldGeom::InitGeomTM

void CldGeom::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
}

void CldGeom::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
}

void CldGeom::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CldGeom", this );
	pm.p( "Valid", IsValid );
	pm.m( "InitTransform", InitGeomTM );
}

Matrix4D CldGeom::GetTransform()
{
	Node* pNode = GetInput( 0 );
	if (!pNode) return Matrix4D::identity;
	if (pNode->IsA<TransformNode>()) return ((TransformNode*)pNode)->GetWorldTM();
	if (pNode->IsA<Body>())
	{
		return ((Body*)pNode)->GetTransform();
	}
	return Matrix4D::identity;
} // CldGeom::GetTransform

CldSpace* CldGeom::FindParentSpace() 
{
	Node* pNode = this;
	pNode = pNode->GetParent();
	while (pNode)
	{
		if ( pNode->IsA<CldSpace>() ) return (CldSpace*)pNode;
		pNode = pNode->GetParent();
	}
	return NULL;
} // CldGeom::FindParentSpace

Body* CldGeom::GetAttachedBody() 
{
	Body* pBody = (Body*)GetInput( 0 );
	if (!pBody || !pBody->IsA<Body>()) return NULL;
	return pBody;
} // CldGeom::GetAttachedBody

/*****************************************************************************/
/*	CldBox implementation
/*****************************************************************************/
CldBox::CldBox()
{
	m_Extents.x = 100.0f;
	m_Extents.y = 100.0f;
	m_Extents.z = 100.0f;
}

void CldBox::Render()
{
	Parent::Render();
	if (DoDrawGizmo())
	{
		rsEnableZ( PhysicsSystem::s_bEnableGizmoZ );
		AABoundBox aabb( Vector3D::null, m_Extents.x*0.5f, m_Extents.y*0.5f, m_Extents.z*0.5f );
		IRS->SetWorldTM( GetTransform() );
		DrawAABB( aabb, PhysicsSystem::s_GeomColorF, PhysicsSystem::s_GeomColor );
		rsFlushLines3D();
		rsFlushPoly3D();
	}
} // CldBox::Render

void CldBox::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Extents;
}

void CldBox::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Extents;
}

void CldBox::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CldBox", this );
	pm.f( "ExtX", m_Extents.x );
	pm.f( "ExtY", m_Extents.y );
	pm.f( "ExtZ", m_Extents.z );
}

dGeomID	CldBox::CreateGeom( dSpaceID spaceID )
{
	Vector3D ext( m_Extents );
	ext *= GetCurWorldScale();

	const Matrix4D tm = GetTransform();
	Matrix3D rotTM;
	Vector3D pos, scale;
	tm.Factorize( scale, rotTM, pos );
	ext *= scale;

	dGeomID id = dCreateBox( spaceID, ext.x, ext.y, ext.z );
	Body* pBody = GetAttachedBody();
	if (pBody)
	{
		dMass mass;
		dMassSetBoxTotal( &mass, pBody->GetMass(), ext.x, ext.y, ext.z );
		pBody->SetMass( mass );
	}
	return id;
} // CldBox::CreateGeom

void CldBox::FitGeom( const Primitive& prim )
{

}
/*****************************************************************************/
/*	CldSphere implementation
/*****************************************************************************/
CldSphere::CldSphere()
{
	m_Radius = 100.0f;
}

void CldSphere::Render()
{
	Parent::Render();
	if (DoDrawGizmo())
	{
		rsEnableZ( PhysicsSystem::s_bEnableGizmoZ );
		IRS->SetWorldTM( GetTransform() );
		DrawSphere( Sphere( Vector3D::null, m_Radius ), PhysicsSystem::s_GeomColorF, PhysicsSystem::s_GeomColor, 32 );
		rsFlushLines3D();
		rsFlushPoly3D();
	}
} // CldSphere::Render

void CldSphere::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Radius;
}

void CldSphere::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Radius;
}

void CldSphere::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CldSphere", this );
	pm.f( "Radius", m_Radius );
}

dGeomID	CldSphere::CreateGeom( dSpaceID spaceID )
{
	const Matrix4D tm = GetTransform();
	Matrix3D rotTM;
	Vector3D pos, scale;
	tm.Factorize( scale, rotTM, pos );
	float r = scale.x*m_Radius*GetCurWorldScale();
	dGeomID id = dCreateSphere( spaceID, r );
	Body* pBody = GetAttachedBody();
	if (pBody)
	{
		dMass mass;
		dMassSetSphereTotal( &mass, pBody->GetMass(), r );
		pBody->SetMass( mass );
	}
	return id;
} // CldSphere::CreateGeom

void CldSphere::FitGeom( const Primitive& prim )
{
}

/*****************************************************************************/
/*	CldCylinder implementation
/*****************************************************************************/
CldCylinder::CldCylinder()
{
	m_Height = 200.0f;
	m_Radius = 50.0f;
}

void CldCylinder::Render()
{
	Parent::Render();
}

void CldCylinder::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Radius << m_Height;
}

void CldCylinder::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Radius >> m_Height;
}

void CldCylinder::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CldCylinder", this );
	pm.f( "Height", m_Height );
	pm.f( "Radius", m_Radius );
}

dGeomID	CldCylinder::CreateGeom( dSpaceID spaceID )
{
	return NULL;
}

void CldCylinder::FitGeom( const Primitive& prim )
{

}

/*****************************************************************************/
/*	CldCapsule implementation
/*****************************************************************************/
CldCapsule::CldCapsule()
{
	m_Height = 60.0f;
	m_Radius = 20.0f;
}

void CldCapsule::Render()
{
	Parent::Render();
	if (DoDrawGizmo())
	{
		rsEnableZ( PhysicsSystem::s_bEnableGizmoZ );
		Capsule cap = Capsule( Vector3D::null, Vector3D( 0, 0, m_Height ), m_Radius );
		IRS->SetWorldTM( GetTransform() );
		DrawCapsule( cap, PhysicsSystem::s_GeomColorF, PhysicsSystem::s_GeomColor, 16 );
		rsFlushLines3D();
		rsFlushPoly3D();
	}
} // CldCapsule::Render

void CldCapsule::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Radius << m_Height;
}

void CldCapsule::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Radius >> m_Height;
}

void CldCapsule::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CldCapsule", this );
	pm.f( "Height", m_Height );
	pm.f( "Radius", m_Radius );
}

dGeomID	CldCapsule::CreateGeom( dSpaceID spaceID )
{
	const Matrix4D tm = GetTransform();
	Matrix3D rotTM;
	Vector3D pos, scale;
	tm.Factorize( scale, rotTM, pos );

	float r = scale.x*m_Radius*GetCurWorldScale();
	float h = scale.x*m_Height*GetCurWorldScale();
	dGeomID id = dCreateCCylinder( spaceID, r, h );
	Body* pBody = GetAttachedBody();
	if (pBody)
	{
		dMass mass;
		dMassSetCappedCylinderTotal( &mass, pBody->GetMass(), 3, r, h );
		pBody->SetMass( mass );
	}
	return id;
} // CldCapsule::CreateGeom

void CldCapsule::FitGeom( const Primitive& prim )
{
}

/*****************************************************************************/
/*	CldRay implementation
/*****************************************************************************/
CldRay::CldRay()
{
	m_Length = 1000.0f;
}

void CldRay::Render()
{
	Parent::Render();
	if (DoDrawGizmo())
	{
		const Matrix4D& tm = GetTransform();
		DrawRay( Ray3D( tm.getTranslation(), tm.getV2() ), PhysicsSystem::s_GeomColor, m_Length );
	}
} // CldRay::Render

void CldRay::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Length;
}

void CldRay::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Length;
}

void CldRay::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CldRay", this );
	pm.f( "Length", m_Length );
}

dGeomID	CldRay::CreateGeom( dSpaceID spaceID )
{
	return dCreateRay( spaceID, m_Length*GetCurWorldScale() );
}

/*****************************************************************************/
/*	CldPlane implementation
/*****************************************************************************/
CldPlane::CldPlane()
{
	m_Plane = Plane::xOy;
}

void CldPlane::Render()
{
	Parent::Render();
	if (DoDrawGizmo())
	{
		rsEnableZ( PhysicsSystem::s_bEnableGizmoZ );
		IRS->SetWorldTM( Matrix4D::identity );
		DrawPlane( m_Plane, PhysicsSystem::s_GeomColorF, PhysicsSystem::s_GeomColor );
		rsFlushPoly3D();
		rsFlushLines3D();
	}
} //  CldPlane::Render

void CldPlane::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Plane;
}

void CldPlane::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Plane;
}

void CldPlane::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CldPlane", this );
	pm.f( "nx", m_Plane.a );
	pm.f( "ny", m_Plane.b );
	pm.f( "nz", m_Plane.c );
	pm.f( "d",  m_Plane.d );
}

dGeomID	CldPlane::CreateGeom( dSpaceID spaceID )
{
	return dCreatePlane( spaceID, m_Plane.a, m_Plane.b, m_Plane.c, -m_Plane.d*GetCurWorldScale() );
}

void CldPlane::FitGeom( const Primitive& prim )
{
}

/*****************************************************************************/
/*	CldTriMesh implementation
/*****************************************************************************/
CldTriMesh::CldTriMesh()
{
	m_TriDataID = NULL;
	m_pMeshNode = NULL;
	m_MeshTM.setIdentity();
}

CldTriMesh::~CldTriMesh()
{
	//dGeomTriMeshDataDestroy( m_TriDataID );
}

void CldTriMesh::Render()
{
	Parent::Render();
	if (!m_pMeshNode)
	{
		m_pMeshNode = GetMeshNode();
		if (m_pMeshNode)
		{
			InitMeshStructure( m_pMeshNode->GetMesh(), GetWorldTM( m_pMeshNode ) );
		}
	}

	if (DoDrawGizmo() && m_pMeshNode)
	{
		static int shID = IRS->GetShaderID( "wire" );
		IRS->SetShader( shID );
		IRS->SetWorldTM( m_MeshTM );
		DrawPrimBM( m_pMeshNode->GetMesh() );
	}

} // CldTriMesh::Render

void CldTriMesh::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
}

void CldTriMesh::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
}

Geometry* CldTriMesh::GetMeshNode()
{
	Iterator it( this, Geometry::FnFilter );
	if (it == false) return false;
	Geometry* pGeom = (Geometry*)*it;
	return pGeom;
} // CldTriMesh::GetMeshNode

void CldTriMesh::Reset()
{
	Parent::Reset();
	if (m_TriDataID) dGeomTriMeshDataDestroy( m_TriDataID ); 
	m_TriDataID = NULL;
	m_pMeshNode = NULL;
} // CldTriMesh::Reset

void CldTriMesh::InitMeshStructure( Primitive& pri, const Matrix4D& tm )
{
	m_MeshTM = tm;
	if (!m_TriDataID) return;
	m_CollisionMesh = pri;

	Matrix4D sc; sc.scaling( GetCurWorldScale() );
	Matrix4D meshTM( tm );
	meshTM *= sc;
	m_CollisionMesh.transform( meshTM );

	m_Indices.clear();
	int nIdx = pri.getNInd();
	WORD* idx = pri.getIndices();
	for (int i = 0; i < nIdx; i++)
	{
		m_Indices.push_back( idx[i] );
	}

	dGeomTriMeshDataBuildSingle( m_TriDataID, 
								 m_CollisionMesh.getVertexData(), 
								 m_CollisionMesh.getVertexStride(), 
								 m_CollisionMesh.getNVert(),
								 &m_Indices[0], nIdx, 3 * sizeof(int) );
} // CldTriMesh::InitMeshStructure

void CldTriMesh::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CldTriMesh", this );
	pm.p( "MeshValid", IsTriDataValid );
}

dGeomID	CldTriMesh::CreateGeom( dSpaceID spaceID )
{
	m_TriDataID = dGeomTriMeshDataCreate();
	if (!m_TriDataID) return NULL;
	return dCreateTriMesh( spaceID, m_TriDataID, NULL, NULL, NULL );
} // CldTriMesh::CreateGeom

/*****************************************************************************/
/*	CldSpace implementation
/*****************************************************************************/
CldSpace* CldSpace::s_pCurSpace = NULL;

CldSpace::CldSpace() : m_PID(NULL)
{
	m_ContactGroupID 	= NULL;
	m_MaxContacts	 	= 128;
	m_bDrawContacts  	= false;
	m_NContacts			= 0;
	m_NCollidedBodies	= 0;

	m_Mu				= 0.2f; 
	m_Mu2				= 0.2f;
	m_bFDir1			= false; 
	m_Bounce			= 0.3f; 
	m_BounceVel			= 0.0f;
	m_SoftERP			= 0.1f; 
	m_SoftCFM			= 0.0f; 
	m_Motion1			= 0.0f; 
	m_Motion2			= 0.0f;
	m_Slip1				= 0.01f;
	m_Slip2				= 0.01f; 
	m_bApprox1			= true;
	m_bApprox2			= true;	
	m_Contacts			= NULL;
	m_MaxPerBodyContacts =5;
} // CldSpace::CldSpace

CldSpace::~CldSpace()
{
	delete []m_Contacts;
}

void CldSpace::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Mu << m_Mu2 << m_bFDir1 << m_Bounce << m_BounceVel << m_SoftERP << m_SoftCFM << m_Motion1 <<		
		  m_Motion2 << m_Slip1 << m_Slip2 << m_bApprox1 << m_bApprox2;		
}

void CldSpace::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Mu >> m_Mu2 >> m_bFDir1 >> m_Bounce >> m_BounceVel >> m_SoftERP >> m_SoftCFM >> m_Motion1 >>		
		m_Motion2 >> m_Slip1 >> m_Slip2 >> m_bApprox1 >> m_bApprox2;
}

void CldSpace::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CldSpace", this );
	pm.p( "Valid", IsValid );
	pm.m( "RenameGeoms", RenameGeoms );
	pm.f( "DrawContacts", m_bDrawContacts );
	pm.f( "MaxPerBodyContacts", m_MaxPerBodyContacts );
	pm.f( "Mu", 		m_Mu		);		
	pm.f( "Mu2", 		m_Mu2		);		
	pm.f( "Bounce",	m_Bounce	);	
	pm.f( "BounceVel",m_BounceVel	);	
	pm.f( "SoftERP",	m_SoftERP	);	
	pm.f( "SoftCFM",	m_SoftCFM	);	
	pm.p( "@Spring", GetSpring, SetSpring );
	pm.p( "@Damping", GetDamping, SetDamping );
	pm.f( "Motion1",	m_Motion1	);	
	pm.f( "Motion2",	m_Motion2	);	
	pm.f( "Slip1",	m_Slip1		);		
	pm.f( "Slip2",	m_Slip2		);		
	
	pm.f( "FDir1",	m_bFDir1	);			
	pm.f( "Approx1",	m_bApprox1	);	
	pm.f( "Approx2",	m_bApprox2	);	
	pm.f( "NContacts", m_NContacts, NULL, true );
	pm.f( "NCollided", m_NCollidedBodies, NULL, true );
} // CldSpace::Expose

void CldSpace::RenameGeoms()
{
	for (int i = 0; i < GetNChildren(); i++)
	{
		CldGeom* pGeom = (CldGeom*)GetChild( i );
		if (!pGeom->IsA<CldGeom>()) continue;
		Node* pB = pGeom->GetInput( 0 );
		if (pB) pGeom->SetName( pB->GetName() );
	}
} // CldSpace::RenameGeoms

void  CldSpace::SetSpring( float spring )
{

}

void  CldSpace::SetDamping( float damping )
{

}

float CldSpace::GetSpring() const
{
	return 0.0f;
}

float CldSpace::GetDamping() const
{
	return 0.0f;
}

CldSpace* CldSpace::FindParentSpace() 
{
	Node* pNode = this;
	pNode = pNode->GetParent();
	while (pNode)
	{
		if ( pNode->IsA<CldSpace>() ) return (CldSpace*)pNode;
		pNode = pNode->GetParent();
	}
	return NULL;
} // CldSpace::FindParentSpace

void CldSpace::Render()
{
	if (!m_PID) 
	{
		CldSpace* pParentSpace = FindParentSpace();
		m_PID = CreateSpace( pParentSpace ? pParentSpace->GetSpaceID() : NULL );
		m_ContactGroupID = dJointGroupCreate( 0 );
	}

	if (!m_Contacts) m_Contacts = new dContact[m_MaxContacts];

	Node::Render();
	if (!m_PID) return;

	CldSpace* pSpace = s_pCurSpace;
	s_pCurSpace = this;
	dJointGroupEmpty( m_ContactGroupID );
	m_NContacts = 0;
	m_NCollidedBodies = 0;
	dSpaceCollide( m_PID, 0, OnCollide );
	s_pCurSpace = pSpace;
} // CldSpace::Render

void CldSpace::OnCollide( void *data, dGeomID o1, dGeomID o2 )
{
	CldSpace* pSpace = s_pCurSpace;
	if (pSpace) pSpace->ProcessCollision( data, o1, o2 );
} // CldSpace::OnCollide

void CldSpace::ProcessCollision( void *data, dGeomID o1, dGeomID o2 )
{
	assert( m_Contacts );
	
	//  skip connected bodies
	dBodyID bodyID1 = dGeomGetBody( o1 );
	dBodyID bodyID2 = dGeomGetBody( o2 );
	if (bodyID1 && bodyID2 && dAreConnected( bodyID1, bodyID2 )) return;
	if (!bodyID1 && !bodyID2) return;
	
	//  do collision
	int nContacts = dCollide( o1, o2, m_MaxContacts, &m_Contacts[0].geom, sizeof( dContact ) );
	if (nContacts == 0) return;

	m_NCollidedBodies++;

	if (m_bDrawContacts) { IRS->SetWorldTM( Matrix4D::identity ); rsEnableZ( PhysicsSystem::s_bEnableGizmoZ ); }
	nContacts = tmin( nContacts, m_MaxContacts - m_NContacts, m_MaxPerBodyContacts );
	for (int i = 0; i < nContacts; i++) 
	{
		DWORD mode =	dContactMu2 | dContactBounce | 
						dContactSlip1 | dContactSlip2 | 
						dContactSoftERP | dContactSoftCFM | 
						dContactMotion1	| dContactMotion2
						;

		if (m_bFDir1)	mode |= dContactFDir1;
		if (m_bApprox1) mode |= dContactApprox1_1;
		if (m_bApprox2) mode |= dContactApprox1_2;

		m_Contacts[i].surface.mode			= mode;
		m_Contacts[i].surface.mu			= m_Mu;	
		m_Contacts[i].surface.mu2			= m_Mu2;	
		m_Contacts[i].surface.bounce		= m_Bounce;
		m_Contacts[i].surface.bounce_vel	= m_BounceVel;
		m_Contacts[i].surface.soft_erp		= m_SoftERP;
		m_Contacts[i].surface.soft_cfm		= m_SoftCFM;
		m_Contacts[i].surface.motion1		= m_Motion1; 
		m_Contacts[i].surface.motion2		= m_Motion2;
		m_Contacts[i].surface.slip1			= m_Slip1;	
		m_Contacts[i].surface.slip2			= m_Slip2;	

		m_NContacts++;

		dJointID jID = dJointCreateContact( GetCurWorldID(), GetContactGroupID(), &m_Contacts[i] );
		//  here should be firing event of (Contact, Material, BodyID1, BodyID2, Position, Normal, whatever...)

		dJointAttach( jID, bodyID1, bodyID2 ); 
		if (m_bDrawContacts)
		{
			DrawRay( Ray3D( Vector3D(	m_Contacts[i].geom.pos[0]/GetCurWorldScale(),
										m_Contacts[i].geom.pos[1]/GetCurWorldScale(),
										m_Contacts[i].geom.pos[2]/GetCurWorldScale() ),
							Vector3D(	m_Contacts[i].geom.normal[0], 
										m_Contacts[i].geom.normal[1], 
										m_Contacts[i].geom.normal[2] ) ), PhysicsSystem::s_ContactColor, 30.0f ); 
		}
	}
	if (m_bDrawContacts) rsFlushLines3D();
} // CldSpace::ProcessCollision

/*****************************************************************************/
/*	CldSimpleSpace implementation
/*****************************************************************************/
CldSimpleSpace::~CldSimpleSpace()
{
}

void CldSimpleSpace::Render()
{
	Parent::Render();
}

void CldSimpleSpace::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
}

void CldSimpleSpace::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
}

void CldSimpleSpace::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CldSimpleSpace", this );
}

dSpaceID CldSimpleSpace::CreateSpace( dSpaceID parentSpaceID )
{
	return dSimpleSpaceCreate( parentSpaceID );
}

/*****************************************************************************/
/*	CldHashSpace implementation
/*****************************************************************************/
CldHashSpace::~CldHashSpace()
{
}

void CldHashSpace::Render()
{
	Parent::Render();
}

void CldHashSpace::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
}

void CldHashSpace::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
}

void CldHashSpace::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CldHashSpace", this );
}

dSpaceID CldHashSpace::CreateSpace( dSpaceID parentSpaceID )
{
	return dHashSpaceCreate( parentSpaceID );
}

/*****************************************************************************/
/*	CldQuadSpace implementation
/*****************************************************************************/
CldQuadSpace::CldQuadSpace() 
{
	m_Center	= Vector3D::null;	
	m_Extents	= Vector3D( 5000.0f, 5000.0f, 5000.0f );	
	m_Depth		= 4;	
}

CldQuadSpace::~CldQuadSpace()
{
}

void CldQuadSpace::Render()
{
	Parent::Render();
}

void CldQuadSpace::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
}

void CldQuadSpace::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
}

void CldQuadSpace::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CldQuadSpace", this );
	pm.f( "Depth",	m_Depth );
	pm.f( "CenterX",	m_Center.x );
	pm.f( "CenterY",	m_Center.y );
	pm.f( "CenterZ",	m_Center.z );
	pm.f( "ExtX",		m_Extents.x );
	pm.f( "ExtY",		m_Extents.y );
	pm.f( "ExtZ",		m_Extents.z );
} // CldQuadSpace::Expose

dSpaceID CldQuadSpace::CreateSpace( dSpaceID parentSpaceID )
{
	dVector3 dc, dext;
	Convert( dc, m_Center );
	Convert( dext, m_Extents );
	return dQuadTreeSpaceCreate( parentSpaceID, dc, dext, m_Depth );
} // CldQuadSpace::CreateSpace

/*****************************************************************************/
/*	PushForce implementation
/*****************************************************************************/
PushForce::PushForce()
{
	m_Amount			= 1.0f;
	m_ForceCone			= 0.1f;
	m_ApplyPointRadius	= 10.0f;
	m_pBody				= NULL;
	m_bBodyRelative		= false;
}

void PushForce::Apply()
{
	if (!m_pBody) return;
	Sphere sp( Vector3D::null, m_ApplyPointRadius );
	Vector3D p = sp.RandomPoint();
	Cone cone( Vector3D::oZ, Vector3D::null, m_ForceCone );
	Vector3D f = cone.RandomDir();
	m_LocalTM.transformVec( f );
	f *= m_Amount;
	p *= GetCurWorldScale();

	if (m_bBodyRelative)
	{
		dBodyAddRelForceAtRelPos( m_pBody->GetBodyID(), f.x, f.y, f.z, p.x, p.y, p.z );
	}
	else
	{
		dBodyAddForceAtRelPos( m_pBody->GetBodyID(), f.x, f.y, f.z, p.x, p.y, p.z );
	}
} // PushForce::Apply

void PushForce::Render()
{
	if (!m_pBody) 
	{
		m_pBody = (Body*)GetInput( 0 );
		if (!m_pBody->IsA<Body>()) m_pBody = NULL;
	}
} // PushForce::Render

void PushForce::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Amount << m_ForceCone << m_ApplyPointRadius;
}	

void PushForce::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	is >> m_Amount >> m_ForceCone >> m_ApplyPointRadius;
}

void PushForce::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "PushForce", this );
	pm.f( "Amount",	m_Amount			);
	pm.f( "Cone",		m_ForceCone			);
	pm.f( "Radius",	m_ApplyPointRadius	);
	pm.f( "BodyRelative", m_bBodyRelative );
	pm.m( "Apply",	Apply );
}


