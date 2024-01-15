/*****************************************************************************/
/*	File:	sgPhysics.cpp
/*	Desc:	Physics wrapper
/*	Author:	Ruslan Shestopalyuk
/*	Date:	11-12-2003
/*****************************************************************************/
#include "gPhysics_pch.h"
#include "ode/ode.h"
#include "sgPhysics.h"

IMPLEMENT_CLASS( PhysicsSystem		);
IMPLEMENT_CLASS( PhysicsObject		);
IMPLEMENT_CLASS( Body				);
IMPLEMENT_CLASS( Joint				);
IMPLEMENT_CLASS( StopMotorJoint		);
IMPLEMENT_CLASS( JointCluster		);
IMPLEMENT_CLASS( BallSocketJoint	);
IMPLEMENT_CLASS( SliderJoint		);
IMPLEMENT_CLASS( HingeJoint			);
IMPLEMENT_CLASS( Hinge2Joint		);
IMPLEMENT_CLASS( UniversalJoint		);
IMPLEMENT_CLASS( FixedJoint			);
IMPLEMENT_CLASS( ContactJoint		);
IMPLEMENT_CLASS( AMotorJoint		);
IMPLEMENT_CLASS( SkeletalJoint		);

IMPLEMENT_CLASS( CollideGeom		);
IMPLEMENT_CLASS( CollideBox			);
IMPLEMENT_CLASS( CollideSphere		);
IMPLEMENT_CLASS( CollideCylinder	);
IMPLEMENT_CLASS( CollideCapsule		);
IMPLEMENT_CLASS( CollidePlane		);
IMPLEMENT_CLASS( CollideRay			);
IMPLEMENT_CLASS( CollideTriMesh		);

IMPLEMENT_CLASS( CollideSpace		);
IMPLEMENT_CLASS( CollideSimpleSpace	);
IMPLEMENT_CLASS( CollideHashSpace	);
IMPLEMENT_CLASS( CollideQuadSpace	);
IMPLEMENT_CLASS( PushForce			);

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
	return PhysicsSystem::inst().GetWorldScale();
}

_inl float GetCurTimeScale()
{
    return PhysicsSystem::inst().GetTimeScale();
}

_inl dWorldID GetCurWorldID()
{
    return (dWorldID)PhysicsSystem::inst().GetWorldID();
}

/*****************************************************************************/
/*	PhysicsSystem implementation
/*****************************************************************************/
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

PhysicsSystem& PhysicsSystem::inst()
{
    static PhysicsSystem me;
    return me;
}

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
    m_bInited           = false;

	m_PID				= dWorldCreate();

	if (m_PID)
	{
		SetERP( m_ERP );
		SetCFM( m_CFM );
		SetGravity( m_Gravity );
	}

} // PhysicsSystem::PhysicsSystem

void PhysicsSystem::Init()
{
    if (m_bInited) return;

    LINK_CLASS( PhysicsSystem	);
    LINK_CLASS( PhysicsObject		);
    LINK_CLASS( Body			);
    LINK_CLASS( Joint			);
    LINK_CLASS( StopMotorJoint	);
    LINK_CLASS( JointCluster	);
    LINK_CLASS( BallSocketJoint	);
    LINK_CLASS( SliderJoint		);
    LINK_CLASS( HingeJoint	    );
    LINK_CLASS( Hinge2Joint		);
    LINK_CLASS( UniversalJoint	);
    LINK_CLASS( FixedJoint		);
    LINK_CLASS( ContactJoint	);
    LINK_CLASS( AMotorJoint		);
    LINK_CLASS( SkeletalJoint	);
    
    LINK_CLASS( CollideGeom			);
    LINK_CLASS( CollideBox			);
    LINK_CLASS( CollideSphere		);
    LINK_CLASS( CollideCylinder		);
    LINK_CLASS( CollideCapsule		);
    LINK_CLASS( CollidePlane		);
    LINK_CLASS( CollideRay			);
    LINK_CLASS( CollideTriMesh		);
    
    LINK_CLASS( CollideSpace		);
    LINK_CLASS( CollideSimpleSpace	);
    LINK_CLASS( CollideHashSpace	);
    LINK_CLASS( CollideQuadSpace	);
    LINK_CLASS( PushForce		);

    m_bInited = true;
} // PhysicsSystem::Init

PhysicsSystem::~PhysicsSystem()
{
	if (m_PID) dWorldDestroy( (dWorldID)m_PID );
	dCloseODE();
}

void PhysicsSystem::Render()
{
    Init();
	if (IsDisabled()) return;
	Step();
} // PhysicsSystem::Render

void PhysicsSystem::SetWorldScale( float val )
{
	m_WorldScale = val;
	m_TimeScale = sqrtf( m_WorldScale );
}

void PhysicsSystem::SetTimeScale( float scale ) 
{ 
    m_TimeScale = scale; 
    m_WorldScale = scale*scale;
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
		dWorldStepFast1( (dWorldID)m_PID, dt, m_MaxStepFastIter );
	}
	else if (m_StepMode == smNormal)
	{
		dWorldStep( (dWorldID)m_PID, dt );
	}
	else if (m_StepMode == smQuickStep)
	{
		dWorldQuickStep( (dWorldID)m_PID, dt );
	}

} // PhysicsSystem::Step

void PhysicsSystem::Serialize( OutStream& os ) const
{
	SNode::Serialize( os );
	os << m_ERP	<< m_CFM << m_Gravity << m_StepScale << m_WorldScale << m_MaxTimeStep << 
		Enum2Byte(m_StepMode) << m_MaxStepFastIter;
}

void PhysicsSystem::Unserialize( InStream& is  )
{
	SNode::Unserialize( is );
	is >> m_ERP	>> m_CFM >> m_Gravity >> m_StepScale >> m_WorldScale >> m_MaxTimeStep >> 
		Enum2Byte(m_StepMode) >> m_MaxStepFastIter;
}

void PhysicsSystem::Expose( PropertyMap& pm )
{
	pm.start<SNode>( "PhysicsSystem", this );
	pm.p( "Valid", &PhysicsSystem::IsValid );
	pm.p( "ERP", &PhysicsSystem::GetERP, &PhysicsSystem::SetERP );
	pm.p( "CFM", &PhysicsSystem::GetCFM, &PhysicsSystem::SetCFM );
	pm.f( "StepScale", m_StepScale );
	pm.f( "MaxTimeStep", m_MaxTimeStep );
	pm.p( "WorldScale", &PhysicsSystem::GetWorldScale, &PhysicsSystem::SetWorldScale );
	pm.p( "GravityX", &PhysicsSystem::GetGravityX, &PhysicsSystem::SetGravityX );
	pm.p( "GravityY", &PhysicsSystem::GetGravityY, &PhysicsSystem::SetGravityY );
	pm.p( "GravityZ", &PhysicsSystem::GetGravityZ, &PhysicsSystem::SetGravityZ );
	pm.f( "FixedStep", m_bFixedStep );
	pm.f( "FixedStepSize", m_FixedStepSize );
	pm.f( "StepMode", m_StepMode );
	pm.f( "MaxStepFastIter", m_MaxStepFastIter );
	pm.m( "SetEarthGravity", &PhysicsSystem::SetEarthGravity );
	pm.m( "Step", &PhysicsSystem::Step );
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
	if (m_PID) dWorldSetCFM((dWorldID) m_PID, val );
}

void PhysicsSystem::SetERP( float val )
{
	m_ERP = val;
	if (m_PID) dWorldSetERP( (dWorldID)m_PID, val );
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
	if (m_PID) dWorldSetGravity( (dWorldID)m_PID, gr.x, gr.y, gr.z );
} // PhysicsSystem::SetGravity

/*****************************************************************************/
/*	Body implementation
/*****************************************************************************/
void PhysicsObject::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "PhysicsObject", this );
   
	pm.p( "ShowBodies", &PhysicsObject::IsShowBodies, &PhysicsObject::ShowBodies );
	pm.p( "ShowJoints", &PhysicsObject::IsShowJoints,&PhysicsObject::ShowJoints );
	pm.p( "ShowGeoms", &PhysicsObject::IsShowGeoms, &PhysicsObject::ShowGeoms	 );
    pm.m( "Reset", &PhysicsObject::Reset		);
    pm.m( "Init", &PhysicsObject::Init		);
} // PhysicsObject::Expose

void PhysicsObject::Init()
{
	ReleaseChildren();
	AddChild<Group>			    ( "Bodies"		);
	AddChild<JointCluster>	    ( "Joints"		);
	AddChild<CollideHashSpace>	( "Collidable"	);
} // PhysicsObject::Init

PhysicsObject::PhysicsObject()
{
	m_bShowJoints = false;
	m_bShowBodies = false;
	m_bShowGeoms  = false;
}

void PhysicsObject::ShowJoints( bool bShow )
{
	Iterator it( this, Joint::FnFilter );
	while (it)
	{
		((SNode*)*it)->SetDrawGizmo( bShow );
		++it;
	}
	m_bShowJoints = bShow;
} // PhysicsObject::ShowJoints

void PhysicsObject::ShowGeoms( bool bShow )
{
	Iterator it( this, CollideGeom::FnFilter );
	while (it)
	{
		((CollideGeom*)*it)->SetDrawGizmo( bShow );
		++it;
	}
	m_bShowGeoms  = bShow;
} // PhysicsObject::ShowGeoms

void PhysicsObject::ShowBodies( bool bShow )
{
	Iterator it( this, Body::FnFilter );
	while (it)
	{
		((Body*)*it)->SetDrawGizmo( bShow );
		++it;
	}
	m_bShowBodies = bShow;
} // PhysicsObject::ShowBodies

void PhysicsObject::Reset()
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
	Iterator gIt( this, CollideGeom::FnFilter );
	while (gIt)
	{
		((CollideGeom*)*gIt)->Reset();
		++gIt;
	}
} // PhysicsObject::Reset

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
	m_pBone		    = NULL;
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
	if (m_PID) dBodyDestroy( (dBodyID)m_PID );
	m_PID = NULL;
}

void Body::SetVelocity( const Vector3D& v )
{
    m_Velocity = v;
    dBodySetLinearVel( (dBodyID)m_PID, v.x, v.y, v.z );
}

void Body::SetAngVelocity( const Vector3D& v )
{
    m_AngVelocity = v;
    dBodySetAngularVel( (dBodyID)m_PID, v.x, v.y, v.z );
}

void Body::Render()
{
	if (!m_PID) CreateBody();
	if (!m_PID || !m_pBone) return;

	//  update values
	const dVector3* pos = (const dVector3*)dBodyGetPosition( (dBodyID)m_PID );
	const dQuaternion* rot = (const dQuaternion*)dBodyGetQuaternion( (dBodyID)m_PID );
	Vector3D cpos;
	Quaternion quat;
	Convert( cpos, *pos );
	Convert( quat, *rot );
	cpos /= GetCurWorldScale();
	Matrix3D mrot;
	mrot.rotation( quat );

	Matrix4D tm = Matrix4D( m_ScaleVector, mrot, cpos );
	Matrix4D pTM;
	pTM.inverse( m_pBone->GetParentWorldTM() );

	Matrix4D m; m.inverse( m_MassCenterTM );
	tm *= pTM;
	tm.mulLeft( m );
	m_pBone->SetTransform( tm );
} // Body::Render

void Body::CreateBody()
{
	if (!m_pBone) return;
	m_PID = dBodyCreate( GetCurWorldID() );
	if (m_PID)
	{
		InitBodyTM();
		SetMass( m_Mass );
	}
}  //  Body::CreateBody

Matrix4D Body::GetTransform()
{
	if (!m_pBone) return Matrix4D::identity;
	Matrix4D m = m_pBone->GetWorldTM();
	m.mulLeft( m_MassCenterTM );
	return m;
}

void Body::InitBodyTM()
{
	if (!m_pBone || !m_PID) return;
	
	Matrix4D& tm = GetTransform();
	
	Matrix3D rotTM;
	Vector3D pos;
	tm.Factorize( m_ScaleVector, rotTM, pos );
	pos *= GetCurWorldScale();

	dBodySetPosition( (dBodyID)m_PID, pos.x, pos.y, pos.z );
	dQuaternion dq;
	Quaternion quat;
	quat.FromMatrix( rotTM );
	quat.normalize();
	Convert( dq, quat );
	dBodySetQuaternion( (dBodyID)m_PID, dq );
} // Body::InitBodyTM

void Body::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Mass << m_Velocity << m_AngVelocity << m_Inertia << m_MassCenterTM << m_BoneName;
}

void Body::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Mass >> m_Velocity >> m_AngVelocity >> m_Inertia >> m_MassCenterTM >> m_BoneName;
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
	pm.m( "Rename", &Body::Rename );
	pm.p( "Valid", &Body::IsValid );
	pm.p( "Enabled", &Body::IsBodyEnabled, &Body::EnableBody );
	pm.p( "Mass", &Body::GetMass, &Body::SetMass );
	pm.p( "DX", &Body::GetLocalCMX, &Body::SetLocalCMX );
	pm.p( "DY", &Body::GetLocalCMY, &Body::SetLocalCMY );
	pm.p( "DZ", &Body::GetLocalCMZ, &Body::SetLocalCMZ );
	pm.p( "RotX", &Body::GetLocalCMRotX, &Body::SetLocalCMRotX );
	pm.p( "RotY", &Body::GetLocalCMRotY, &Body::SetLocalCMRotY );
	pm.p( "RotZ", &Body::GetLocalCMRotZ, &Body::SetLocalCMRotZ );

	pm.f( "VelocityX", m_Velocity.x, NULL, true );
	pm.f( "VelocityY", m_Velocity.y, NULL, true );
	pm.f( "VelocityZ", m_Velocity.z, NULL, true );
	pm.f( "AngVelocityX", m_AngVelocity.x, NULL, true );
	pm.f( "AngVelocityY", m_AngVelocity.y, NULL, true );
	pm.f( "AngVelocityZ", m_AngVelocity.z, NULL, true );

} // Body::Expose

void Body::Rename()
{
	m_pBone = (TransformNode*)GetInput( 0 );
	if (m_pBone) SetName( m_pBone->GetName() );
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
		dBodySetMass( (dBodyID)m_PID, &mass );
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
		dBodySetMass( (dBodyID)m_PID, &mass );
	}
} // Body::SetDensity

void Body::SetMass( const dMass& mass )
{
	dBodySetMass( (dBodyID)m_PID, &mass );
} // Body::SetMass

void Body::EnableBody( bool bEnable )
{
	if (!m_PID) return;
	if (bEnable)
	{
		dBodyEnable( (dBodyID)m_PID );
	}
	else
	{
		dBodyDisable( (dBodyID)m_PID );
	}
} // Body::EnableBody

bool Body::IsBodyEnabled() const
{
	return m_PID && dBodyIsEnabled( (dBodyID)m_PID ) == 1;
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
	if (m_PID) dJointDestroy( (dJointID)m_PID );
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
	pm.p( "Valid", &Joint::IsValid );
	pm.m( "InitJoint", &Joint::InitJoint		);
	pm.m( "PositionFromBodies", &Joint::PositionFromBodies );
} // Joint::Expose

void Joint::InitJoint()
{
	if (m_PID) 
	{
		dJointDestroy( (dJointID)m_PID );
		m_PID = NULL;
	}

	JointCluster* pCluster = FindParentJointCluster();
	if (!pCluster) return;
	if (!m_pBody1) m_pBody1 = (Body*)GetInput( 0 );
	if (!m_pBody2) m_pBody2 = (Body*)GetInput( 1 );
	dBodyID id1 = m_pBody1 ? (dBodyID)m_pBody1->GetBodyID() : NULL;
	dBodyID id2 = m_pBody2 ? (dBodyID)m_pBody2->GetBodyID() : NULL;
	
	m_PID = CreateJoint( pCluster->GetClusterID() );
	if (!m_PID) return;
	
	if ((!id1 || !id2) && NeedTwoBodies()) return;
	//  attach joint to bodies
	dJointAttach( (dJointID)m_PID, id1, id2 );
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
	SNode* pNode = this;
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
	pm.p( "Valid", &StopMotorJoint::IsValid );
	pm.m( "InitJoint", &StopMotorJoint::InitJoint		);
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
	if (m_PID) dJointGroupDestroy( (dJointGroupID)m_PID );
}

void JointCluster::Render()
{
	if (!m_PID)
	{
		m_PID = dJointGroupCreate( 0 );
	}
	SNode::Render();
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
	pm.p( "Valid", &JointCluster::IsValid );
	pm.f( "MaxSize", m_MaxSize );
	pm.m( "RenameJoints", &JointCluster::RenameJoints );
} // JointCluster::Expose

void JointCluster::RenameJoints()
{
	for (int i = 0; i < GetNChildren(); i++)
	{
		Joint* pJoint = (Joint*)GetChild( i );
		if (!pJoint->IsA<Joint>()) continue;
		SNode* pB1 = pJoint->GetInput( 0 );
		SNode* pB2 = pJoint->GetInput( 1 );
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
		dJointGetBallAnchor( (dJointID)m_PID, dAnchor );
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

OdeID BallSocketJoint::CreateJoint( OdeID groupID )
{
	return (dJointID)dJointCreateBall( GetCurWorldID(), (dJointGroupID)groupID );
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
	dJointSetBallAnchor( (dJointID)m_PID, anchor.x, anchor.y, anchor.z );
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
	pm.p( "SliderPosition", &SliderJoint::GetSliderPosition );
	pm.p( "SliderPositionRate", &SliderJoint::GetSliderPositionRate );
	pm.f( "AxisX", m_Axis.x );
	pm.f( "AxisY", m_Axis.y );
	pm.f( "AxisZ", m_Axis.z );
} // SliderJoint::Expose

OdeID SliderJoint::CreateJoint( OdeID groupID )
{
	return (dJointID)dJointCreateBall( (dWorldID)GetCurWorldID(), (dJointGroupID)groupID );
}

float SliderJoint::GetSliderPosition() const
{
	return m_PID ? dJointGetSliderPosition( (dJointID)m_PID ) : 0.0f;
}

float SliderJoint::GetSliderPositionRate() const
{
	return m_PID ? dJointGetSliderPositionRate( (dJointID)m_PID ) : 0.0f;
}

void SliderJoint::SetParams()
{
	if (!m_PID) return;

	Matrix4D tm = GetTransform();
	m_Axis = tm.getV2();

	dJointSetSliderParam( (dJointID)m_PID, dParamLoStop,			m_LoStop		);
	dJointSetSliderParam( (dJointID)m_PID, dParamHiStop,			m_HiStop		);
	dJointSetSliderParam( (dJointID)m_PID, dParamStopERP,			m_StopERP		);
	dJointSetSliderParam( (dJointID)m_PID, dParamStopCFM,			m_StopCFM		);
	dJointSetSliderParam( (dJointID)m_PID, dParamVel,				m_MotorVel		);
	dJointSetSliderParam( (dJointID)m_PID, dParamFMax,			m_MotorFMax		);
	dJointSetSliderParam( (dJointID)m_PID, dParamFudgeFactor,		m_FudgeFactor	);
	dJointSetSliderParam( (dJointID)m_PID, dParamBounce,			m_Bounce		);
	dJointSetSliderParam( (dJointID)m_PID, dParamCFM,				m_CFM			);

	dJointSetSliderAxis ( (dJointID)m_PID, m_Axis.x, m_Axis.y, m_Axis.z );
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
		dJointGetHingeAxis( (dJointID)m_PID, dAxis );
		dJointGetHingeAnchor( (dJointID)m_PID, dAnchor );
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

OdeID HingeJoint::CreateJoint( OdeID groupID )
{
	return dJointCreateHinge( GetCurWorldID(), (dJointGroupID)groupID );
}

float HingeJoint::GetAngle() const
{
	return m_PID ? dJointGetHingeAngle( (dJointID)m_PID ) : 0.0f;
}

float HingeJoint::GetAngleRate() const
{
	return m_PID ? dJointGetHingeAngleRate( (dJointID)m_PID ) : 0.0f;
}

void HingeJoint::SetParams()
{
	if (!m_PID) return;

	dJointSetHingeParam( (dJointID)m_PID, dParamLoStop,			m_LoStop		);
	dJointSetHingeParam( (dJointID)m_PID, dParamHiStop,			m_HiStop		);
	dJointSetHingeParam( (dJointID)m_PID, dParamStopERP,			m_StopERP		);
	dJointSetHingeParam( (dJointID)m_PID, dParamStopCFM,			m_StopCFM		);
	dJointSetHingeParam( (dJointID)m_PID, dParamVel,				m_MotorVel		);
	dJointSetHingeParam( (dJointID)m_PID, dParamFMax,				m_MotorFMax		);
	dJointSetHingeParam( (dJointID)m_PID, dParamFudgeFactor,		m_FudgeFactor	);
	dJointSetHingeParam( (dJointID)m_PID, dParamBounce,			m_Bounce		);
	dJointSetHingeParam( (dJointID)m_PID, dParamCFM,				m_CFM			);

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
	
	dJointSetHingeAnchor( (dJointID)m_PID, anchor.x, anchor.y, anchor.z );
	dJointSetHingeAxis  ( (dJointID)m_PID, axis.x, axis.y, axis.z );
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
		dJointGetHinge2Anchor( (dJointID)m_PID, danchor );
		dJointGetHinge2Axis1 ( (dJointID)m_PID, daxis1 );
		dJointGetHinge2Axis2 ( (dJointID)m_PID, daxis2 );

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
	pm.p( "Angle1", &Hinge2Joint::GetAngle1 );
	pm.p( "Angle1Rate", &Hinge2Joint::GetAngle1Rate );
	pm.p( "Angle2Rate", &Hinge2Joint::GetAngle2Rate );
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

OdeID Hinge2Joint::CreateJoint( OdeID groupID )
{
	return (OdeID)dJointCreateHinge2( GetCurWorldID(), (dJointGroupID)groupID );
}

float Hinge2Joint::GetAngle1() const
{
	return m_PID ? dJointGetHinge2Angle1( (dJointID)m_PID ) : 0.0f;
}

float Hinge2Joint::GetAngle1Rate() const
{
	return m_PID ? dJointGetHinge2Angle1Rate( (dJointID)m_PID ) : 0.0f;
}

float Hinge2Joint::GetAngle2Rate() const
{
	return m_PID ? dJointGetHinge2Angle2Rate( (dJointID)m_PID ) : 0.0f;
}

void Hinge2Joint::SetParams()
{
	if (!m_PID) return;
	dJointSetHinge2Param( (dJointID)m_PID, dParamSuspensionERP,	m_SuspensionERP	);
	dJointSetHinge2Param( (dJointID)m_PID, dParamSuspensionCFM,	m_SuspensionCFM	);
	dJointSetHinge2Param( (dJointID)m_PID, dParamLoStop,			m_LoStop		);
	dJointSetHinge2Param( (dJointID)m_PID, dParamHiStop,			m_HiStop		);
	dJointSetHinge2Param( (dJointID)m_PID, dParamStopERP,			m_StopERP		);
	dJointSetHinge2Param( (dJointID)m_PID, dParamStopCFM,			m_StopCFM		);
	dJointSetHinge2Param( (dJointID)m_PID, dParamVel,				m_MotorVel		);
	dJointSetHinge2Param( (dJointID)m_PID, dParamFMax,			m_MotorFMax		);
	dJointSetHinge2Param( (dJointID)m_PID, dParamFudgeFactor,		m_FudgeFactor	);
	dJointSetHinge2Param( (dJointID)m_PID, dParamBounce,			m_Bounce		);
	dJointSetHinge2Param( (dJointID)m_PID, dParamCFM,				m_CFM			);

	dJointSetHinge2Param( (dJointID)m_PID, dParamVel2,			m_MotorVel		);
	dJointSetHinge2Param( (dJointID)m_PID, dParamFMax2,			m_MotorFMax		);


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
	
	dJointSetHinge2Anchor( (dJointID)m_PID, anchor.x, anchor.y, anchor.z );
	dJointSetHinge2Axis1 ( (dJointID)m_PID, axis1.x, axis1.y, axis1.z );
	dJointSetHinge2Axis2 ( (dJointID)m_PID, axis2.x, axis2.y, axis2.z );
} // Hinge2Joint::SetParams

void Hinge2Joint::GetParams()
{
	if (!m_PID) return;
	dVector3 anchor, axis1, axis2;
	dJointGetHinge2Anchor( (dJointID)m_PID, anchor );
	dJointGetHinge2Axis1 ( (dJointID)m_PID, axis1 );
	dJointGetHinge2Axis2 ( (dJointID)m_PID, axis2 );
	
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
		dJointGetUniversalAnchor( (dJointID)m_PID, dAnchor );
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

OdeID UniversalJoint::CreateJoint( OdeID groupID )
{
	return dJointCreateUniversal( GetCurWorldID(), (dJointGroupID)groupID );
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
	dJointSetUniversalParam( (dJointID)m_PID, dParamLoStop,			m_LoStop		);
	dJointSetUniversalParam( (dJointID)m_PID, dParamHiStop,			m_HiStop		);
	dJointSetUniversalParam( (dJointID)m_PID, dParamStopERP,			m_StopERP		);
	dJointSetUniversalParam( (dJointID)m_PID, dParamStopCFM,			m_StopCFM		);
	dJointSetUniversalParam( (dJointID)m_PID, dParamVel,				m_MotorVel		);
	dJointSetUniversalParam( (dJointID)m_PID, dParamFMax,				m_MotorFMax		);
	dJointSetUniversalParam( (dJointID)m_PID, dParamFudgeFactor,		m_FudgeFactor	);
	dJointSetUniversalParam( (dJointID)m_PID, dParamBounce,			m_Bounce		);
	dJointSetUniversalParam( (dJointID)m_PID, dParamCFM,				m_CFM			);

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
	
	dJointSetUniversalAnchor( (dJointID)m_PID, anchor.x, anchor.y, anchor.z );
	dJointSetUniversalAxis1 ( (dJointID)m_PID, axis1.x, axis1.y, axis1.z );
	dJointSetUniversalAxis2 ( (dJointID)m_PID, axis2.x, axis2.y, axis2.z );
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

OdeID FixedJoint::CreateJoint( OdeID groupID )
{
	return dJointCreateFixed( GetCurWorldID(), (dJointGroupID)groupID );
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

OdeID ContactJoint::CreateJoint( OdeID groupID )
{
	dContact contact;
	return dJointCreateContact( GetCurWorldID(), 
								(dJointGroupID)groupID, &contact );
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

OdeID AMotorJoint::CreateJoint( OdeID groupID )
{
	return dJointCreateAMotor( GetCurWorldID(), (dJointGroupID)groupID );
}

void AMotorJoint::SetParams()
{
	if (!m_PID) return;
	dJointSetAMotorParam( (dJointID)m_PID, dParamLoStop,			m_LoStop		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamHiStop,			m_HiStop		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamStopERP,			m_StopERP		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamStopCFM,			m_StopCFM		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamVel,				m_MotorVel		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamFMax,			m_MotorFMax		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamFudgeFactor,		m_FudgeFactor	);
	dJointSetAMotorParam( (dJointID)m_PID, dParamBounce,			m_Bounce		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamCFM,				m_CFM			);

	dJointSetAMotorParam( (dJointID)m_PID, dParamLoStop2,			m_LoStop2		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamHiStop2,			m_HiStop2		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamStopERP2,		m_StopERP2		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamStopCFM2,		m_StopCFM2		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamVel2,			m_MotorVel2		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamFMax2,			m_MotorFMax2	);
	dJointSetAMotorParam( (dJointID)m_PID, dParamFudgeFactor2,	m_FudgeFactor2	);
	dJointSetAMotorParam( (dJointID)m_PID, dParamBounce2,			m_Bounce2		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamCFM2,			m_CFM2			);

	dJointSetAMotorParam( (dJointID)m_PID, dParamLoStop3,			m_LoStop3		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamHiStop3,			m_HiStop3		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamStopERP3,		m_StopERP3		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamStopCFM3,		m_StopCFM3		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamVel3,			m_MotorVel3		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamFMax3,			m_MotorFMax3	);
	dJointSetAMotorParam( (dJointID)m_PID, dParamFudgeFactor3,	m_FudgeFactor3	);
	dJointSetAMotorParam( (dJointID)m_PID, dParamBounce3,			m_Bounce3		);
	dJointSetAMotorParam( (dJointID)m_PID, dParamCFM3,			m_CFM3			);

	dJointSetAMotorNumAxes( (dJointID)m_PID, 3 );
	dJointSetAMotorMode( (dJointID)m_PID, dAMotorEuler );

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
	dJointSetAMotorAxis( (dJointID)m_PID, 0, 1, axis1.x, axis1.y, axis1.z );
	dJointSetAMotorAxis( (dJointID)m_PID, 2, 2, axis2.x, axis2.y, axis2.z );

} // AMotorJoint::SetParams

/*****************************************************************************/
/*	CollideSkeletalJoint implementation
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

OdeID SkeletalJoint::CreateJoint( OdeID groupID )
{
	return NULL;
}

void SkeletalJoint::SetParams()
{

}

/*****************************************************************************/
/*	CollideGeom implementation
/*****************************************************************************/
CollideGeom::CollideGeom() : m_PID(NULL)
{
    m_bHasGeomTM = false;
    m_GeomTM = Matrix4D::identity;
    m_pBody  = NULL;
    m_pTransform = NULL;
} // CollideGeom::CollideGeom

CollideGeom::~CollideGeom()
{
	//if (m_PID) dGeomDestroy( m_PID );
}

void CollideGeom::SetLocalTM( const Matrix4D& tm )
{
    m_bHasGeomTM = !tm.equal( Matrix4D::identity );
    m_GeomTM = tm;
} // CollideGeom::SetLocalTM

void CollideGeom::Reset()
{
	if (m_PID) dGeomDestroy( (dGeomID)m_PID );
	m_PID = NULL;
} // CollideGeom::Reset

Matrix4D CollideGeom::GetWorldTM() const
{
    Matrix4D tm = m_GeomTM;
    const Body* pBody = GetAttachedBody();
    if (pBody)
    {
        dGeomID geomID = m_bHasGeomTM ? (dGeomID)m_GeomTMID : (dGeomID)m_PID;
        dBodyID bodyID = dGeomGetBody( geomID );
        const dVector3* pos = (const dVector3*)dBodyGetPosition( bodyID );
        const dQuaternion* rot = (const dQuaternion*)dBodyGetQuaternion( bodyID );
        Vector3D cpos;
        Quaternion quat;
        Convert( cpos, *pos );
        Convert( quat, *rot );
        cpos /= GetCurWorldScale();
        Matrix3D mrot;
        mrot.rotation( quat );

        tm *= Matrix4D( Vector3D::one, mrot, cpos );
    }
    else if (m_pTransform) 
    {
        tm *= m_pTransform->GetWorldTM();
    }
    return tm;
} // CollideGeom::GetWorldTM

void CollideGeom::Render()
{
	if (m_PID) return;
	
	CollideSpace* pSpace = FindParentSpace();
	if (!pSpace || !pSpace->GetSpaceID()) return;
    Body* pBody = GetAttachedBody();

    if (m_bHasGeomTM)
    {
        m_PID = CreateGeom( NULL );
        m_GeomTMID = dCreateGeomTransform( (dSpaceID)pSpace->GetSpaceID() );
        if (!m_GeomTMID) return;
        dGeomTransformSetGeom( (dGeomID)m_GeomTMID, (dGeomID)m_PID );
        dGeomEnable( (dGeomID)m_GeomTMID );
        SetGeomTM( m_GeomTMID, GetTransform() );
        SetGeomTM( m_PID, m_GeomTM );
        if (pBody) dGeomSetBody( (dGeomID)m_GeomTMID, (dBodyID)pBody->GetBodyID() );
    }
    else
    {
        m_PID = CreateGeom( pSpace->GetSpaceID() );
        if (!m_PID) return;
        dGeomEnable( (dGeomID)m_PID );
        if (pBody) 
	    {
	    	dGeomSetBody( (dGeomID)m_PID, (dBodyID)pBody->GetBodyID() );
	    }
	    else if (IsPlaceable())
	    {
	    	SetGeomTM( m_PID, GetTransform() ); 
	    }
    }
} // CollideGeom::Render

void CollideGeom::SetGeomTM( OdeID geomID, const Matrix4D& tm )
{
    Matrix3D rotTM;
    Vector3D pos, scale;
    tm.Factorize( scale, rotTM, pos );
    pos *= GetCurWorldScale();

    dGeomSetPosition( (dGeomID)m_PID, pos.x, pos.y, pos.z );
    dQuaternion dq;
    Quaternion quat;
    quat.FromMatrix( rotTM );
    quat.normalize();
    Convert( dq, quat );
    dGeomSetQuaternion( (dGeomID)m_PID, dq );
} // CollideGeom::SetGeomTM

void CollideGeom::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
    os << m_GeomTM;
} // CollideGeom::Serialize

void CollideGeom::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
    is >> m_GeomTM;
    SetLocalTM( m_GeomTM );
} // CollideGeom::Unserialize

void CollideGeom::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CollideGeom", this );
	pm.p( "Valid", &CollideGeom::IsValid );
}

Matrix4D CollideGeom::GetTransform()
{
	if (m_pTransform) return m_pTransform->GetWorldTM();
	return Matrix4D::identity;
} // CollideGeom::GetTransform

CollideSpace* CollideGeom::FindParentSpace() 
{
	SNode* pNode = this;
	pNode = pNode->GetParent();
	while (pNode)
	{
		if ( pNode->IsA<CollideSpace>() ) return (CollideSpace*)pNode;
		pNode = pNode->GetParent();
	}
	return NULL;
} // CollideGeom::FindParentSpace

/*****************************************************************************/
/*	CollideBox implementation
/*****************************************************************************/
CollideBox::CollideBox()
{
	m_Extents.x = 100.0f;
	m_Extents.y = 100.0f;
	m_Extents.z = 100.0f;
}

void CollideBox::Render()
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
} // CollideBox::Render

void CollideBox::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Extents;
}

void CollideBox::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Extents;
}

void CollideBox::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CollideBox", this );
	pm.f( "ExtX", m_Extents.x );
	pm.f( "ExtY", m_Extents.y );
	pm.f( "ExtZ", m_Extents.z );
}

OdeID CollideBox::CreateGeom( OdeID spaceID )
{
	Vector3D ext( m_Extents );
	ext *= GetCurWorldScale();

	const Matrix4D tm = GetTransform();
	Matrix3D rotTM;
	Vector3D pos, scale;
	tm.Factorize( scale, rotTM, pos );
	ext *= scale;

	dGeomID id = dCreateBox( (dSpaceID)spaceID, ext.x, ext.y, ext.z );
	Body* pBody = GetAttachedBody();
	if (pBody)
	{
		dMass mass;
		dMassSetBoxTotal( &mass, pBody->GetMass(), ext.x, ext.y, ext.z );
		pBody->SetMass( mass );
	}
	return id;
} // CollideBox::CreateGeom

void CollideBox::FitGeom( const Primitive& prim )
{

}
/*****************************************************************************/
/*	CollideSphere implementation
/*****************************************************************************/
CollideSphere::CollideSphere()
{
	m_Radius = 100.0f;
}

void CollideSphere::Render()
{
	Parent::Render();
	if (DoDrawGizmo())
	{
		rsEnableZ( PhysicsSystem::s_bEnableGizmoZ );
		
        IRS->SetWorldTM( GetWorldTM() );

		DrawSphere( Sphere( Vector3D::null, m_Radius ), PhysicsSystem::s_GeomColorF, PhysicsSystem::s_GeomColor, 32 );
		rsFlush();
	}
} // CollideSphere::Render

void CollideSphere::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Radius;
}

void CollideSphere::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Radius;
}

void CollideSphere::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CollideSphere", this );
	pm.f( "Radius", m_Radius );
}

OdeID	CollideSphere::CreateGeom( OdeID spaceID )
{
	const Matrix4D tm = GetTransform();
	Matrix3D rotTM;
	Vector3D pos, scale;
	tm.Factorize( scale, rotTM, pos );
	float r = scale.x*m_Radius*GetCurWorldScale();
	dGeomID id = dCreateSphere( (dSpaceID)spaceID, r );
	Body* pBody = GetAttachedBody();
	if (pBody)
	{
		dMass mass;
		dMassSetSphereTotal( &mass, pBody->GetMass(), r );
		pBody->SetMass( mass );
	}
	return id;
} // CollideSphere::CreateGeom

void CollideSphere::FitGeom( const Primitive& prim )
{
}

/*****************************************************************************/
/*	CollideCylinder implementation
/*****************************************************************************/
CollideCylinder::CollideCylinder()
{
	m_Height = 200.0f;
	m_Radius = 50.0f;
}

void CollideCylinder::Render()
{
	Parent::Render();
}

void CollideCylinder::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Radius << m_Height;
}

void CollideCylinder::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Radius >> m_Height;
}

void CollideCylinder::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CollideCylinder", this );
	pm.f( "Height", m_Height );
	pm.f( "Radius", m_Radius );
}

OdeID	CollideCylinder::CreateGeom( OdeID spaceID )
{
	return NULL;
}

void CollideCylinder::FitGeom( const Primitive& prim )
{

}

/*****************************************************************************/
/*	CollideCapsule implementation
/*****************************************************************************/
CollideCapsule::CollideCapsule()
{
	m_Height = 60.0f;
	m_Radius = 20.0f;
}

void CollideCapsule::Render()
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
} // CollideCapsule::Render

void CollideCapsule::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Radius << m_Height;
}

void CollideCapsule::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Radius >> m_Height;
}

void CollideCapsule::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CollideCapsule", this );
	pm.f( "Height", m_Height );
	pm.f( "Radius", m_Radius );
}

OdeID	CollideCapsule::CreateGeom( OdeID spaceID )
{
	const Matrix4D tm = GetTransform();
	Matrix3D rotTM;
	Vector3D pos, scale;
	tm.Factorize( scale, rotTM, pos );

	float r = scale.x*m_Radius*GetCurWorldScale();
	float h = scale.x*m_Height*GetCurWorldScale();
	dGeomID id = dCreateCCylinder( (dSpaceID)spaceID, r, h );
	Body* pBody = GetAttachedBody();
	if (pBody)
	{
		dMass mass;
		dMassSetCappedCylinderTotal( &mass, pBody->GetMass(), 3, r, h );
		pBody->SetMass( mass );
	}
	return id;
} // CollideCapsule::CreateGeom

void CollideCapsule::FitGeom( const Primitive& prim )
{
}

/*****************************************************************************/
/*	CollideRay implementation
/*****************************************************************************/
CollideRay::CollideRay()
{
	m_Length = 1000.0f;
}

void CollideRay::Render()
{
	Parent::Render();
	if (DoDrawGizmo())
	{
		const Matrix4D& tm = GetTransform();
		DrawRay( Ray3D( tm.getTranslation(), tm.getV2() ), PhysicsSystem::s_GeomColor, m_Length );
	}
} // CollideRay::Render

void CollideRay::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Length;
}

void CollideRay::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Length;
}

void CollideRay::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CollideRay", this );
	pm.f( "Length", m_Length );
}

OdeID	CollideRay::CreateGeom( OdeID spaceID )
{
	return dCreateRay( (dSpaceID)spaceID, m_Length*GetCurWorldScale() );
}

/*****************************************************************************/
/*	CollidePlane implementation
/*****************************************************************************/
CollidePlane::CollidePlane()
{
	m_Plane = Plane::xOy;
}

void CollidePlane::Render()
{
	Parent::Render();
	if (DoDrawGizmo())
	{
		rsEnableZ( PhysicsSystem::s_bEnableGizmoZ );
		IRS->SetWorldTM( Matrix4D::identity );
		DrawPlane( m_Plane, PhysicsSystem::s_GeomColorF, PhysicsSystem::s_GeomColor, 
            &Vector3D::null, 1000.0f );
		rsFlushPoly3D();
		rsFlushLines3D();
	}
} //  CollidePlane::Render

void CollidePlane::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Plane;
}

void CollidePlane::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Plane;
}

void CollidePlane::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CollidePlane", this );
	pm.f( "nx", m_Plane.a );
	pm.f( "ny", m_Plane.b );
	pm.f( "nz", m_Plane.c );
	pm.f( "d",  m_Plane.d );
}

OdeID	CollidePlane::CreateGeom( OdeID spaceID )
{
	return dCreatePlane( (dSpaceID)spaceID, m_Plane.a, m_Plane.b, m_Plane.c, -m_Plane.d*GetCurWorldScale() );
}

void CollidePlane::FitGeom( const Primitive& prim )
{
}

/*****************************************************************************/
/*	CollideTriMesh implementation
/*****************************************************************************/
CollideTriMesh::CollideTriMesh()
{
	m_TriDataID = NULL;
	m_pMeshNode = NULL;
	m_MeshTM.setIdentity();
}

CollideTriMesh::~CollideTriMesh()
{
	//dGeomTriMeshDataDestroy( m_TriDataID );
}

void CollideTriMesh::Render()
{
	Parent::Render();
	if (!m_pMeshNode)
	{
		m_pMeshNode = GetMeshNode();
		if (m_pMeshNode)
		{
            InitMeshStructure( m_pMeshNode->GetMesh(), ::GetWorldTM( m_pMeshNode ) );
		}
	}

	if (DoDrawGizmo() && m_pMeshNode)
	{
		static int shID = IRS->GetShaderID( "wire" );
		IRS->SetShader( shID );
		IRS->SetWorldTM( m_MeshTM );
		DrawPrimBM( m_pMeshNode->GetMesh() );
	}

} // CollideTriMesh::Render

void CollideTriMesh::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
}

void CollideTriMesh::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
}

Geometry* CollideTriMesh::GetMeshNode()
{
	Iterator it( this, Geometry::FnFilter );
	if (it == false) return false;
	Geometry* pGeom = (Geometry*)*it;
	return pGeom;
} // CollideTriMesh::GetMeshNode

void CollideTriMesh::Reset()
{
	Parent::Reset();
	if (m_TriDataID) dGeomTriMeshDataDestroy( (dTriMeshDataID)m_TriDataID ); 
	m_TriDataID = NULL;
	m_pMeshNode = NULL;
} // CollideTriMesh::Reset

void CollideTriMesh::InitMeshStructure( Primitive& pri, const Matrix4D& tm )
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

	dGeomTriMeshDataBuildSingle( (dTriMeshDataID)m_TriDataID, 
								 m_CollisionMesh.getVertexData(), 
								 m_CollisionMesh.getVertexStride(), 
								 m_CollisionMesh.getNVert(),
								 &m_Indices[0], nIdx, 3 * sizeof(int) );
} // CollideTriMesh::InitMeshStructure

void CollideTriMesh::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CollideTriMesh", this );
	pm.p( "MeshValid", &CollideTriMesh::IsTriDataValid );
}

OdeID	CollideTriMesh::CreateGeom( OdeID spaceID )
{
	m_TriDataID = dGeomTriMeshDataCreate();
	if (!m_TriDataID) return NULL;
	return dCreateTriMesh( (dSpaceID)spaceID, (dTriMeshDataID)m_TriDataID, NULL, NULL, NULL );
} // CollideTriMesh::CreateGeom

/*****************************************************************************/
/*	CollideSpace implementation
/*****************************************************************************/
CollideSpace* CollideSpace::s_pCurSpace = NULL;

CollideSpace::CollideSpace() : m_PID(NULL)
{
	m_ContactGroupID 	    = NULL;
	m_MaxContacts	 	    = 128;
	m_bDrawContacts  	    = false;
	m_NContacts			    = 0;
	m_NCollidedBodies	    = 0;

	m_Mu				    = 0.2f; 
	m_Mu2				    = 0.2f;
	m_bFDir1			    = false; 
	m_Bounce			    = 0.3f; 
	m_BounceVel			    = 0.0f;
	m_SoftERP			    = 0.1f; 
	m_SoftCFM			    = 1e-3f; 
	m_Motion1			    = 0.0f; 
	m_Motion2			    = 0.0f;
	m_Slip1				    = 0.01f;
	m_Slip2				    = 0.01f; 
	m_bApprox1			    = true;
	m_bApprox2			    = true;	
	m_Contacts			    = NULL;
	m_MaxPerBodyContacts    = 5;
} // CollideSpace::CollideSpace

CollideSpace::~CollideSpace()
{
	delete []m_Contacts;
}

void CollideSpace::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Mu << m_Mu2 << m_bFDir1 << m_Bounce << m_BounceVel << m_SoftERP << m_SoftCFM << m_Motion1 <<		
		  m_Motion2 << m_Slip1 << m_Slip2 << m_bApprox1 << m_bApprox2;		
}

void CollideSpace::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Mu >> m_Mu2 >> m_bFDir1 >> m_Bounce >> m_BounceVel >> m_SoftERP >> m_SoftCFM >> m_Motion1 >>		
		m_Motion2 >> m_Slip1 >> m_Slip2 >> m_bApprox1 >> m_bApprox2;
}

void CollideSpace::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CollideSpace", this );
	pm.p( "Valid", &CollideSpace::IsValid );
	pm.m( "RenameGeoms", &CollideSpace::RenameGeoms );
	pm.f( "DrawContacts", m_bDrawContacts );
	pm.f( "MaxPerBodyContacts", m_MaxPerBodyContacts );
	pm.f( "Mu", 		m_Mu		);		
	pm.f( "Mu2", 		m_Mu2		);		
	pm.f( "Bounce",	m_Bounce	);	
	pm.f( "BounceVel",m_BounceVel	);	
	pm.f( "SoftERP",	m_SoftERP	);	
	pm.f( "SoftCFM",	m_SoftCFM	);	
	pm.f( "Motion1",	m_Motion1	);	
	pm.f( "Motion2",	m_Motion2	);	
	pm.f( "Slip1",	m_Slip1		);		
	pm.f( "Slip2",	m_Slip2		);		
	
	pm.f( "FDir1",	m_bFDir1	);			
	pm.f( "Approx1",	m_bApprox1	);	
	pm.f( "Approx2",	m_bApprox2	);	
	pm.f( "NContacts", m_NContacts, NULL, true );
	pm.f( "NCollided", m_NCollidedBodies, NULL, true );
} // CollideSpace::Expose

void CollideSpace::RenameGeoms()
{
	for (int i = 0; i < GetNChildren(); i++)
	{
		CollideGeom* pGeom = (CollideGeom*)GetChild( i );
		if (!pGeom->IsA<CollideGeom>()) continue;
		SNode* pB = pGeom->GetInput( 0 );
		if (pB) pGeom->SetName( pB->GetName() );
	}
} // CollideSpace::RenameGeoms

CollideSpace* CollideSpace::FindParentSpace() 
{
	SNode* pNode = this;
	pNode = pNode->GetParent();
	while (pNode)
	{
		if ( pNode->IsA<CollideSpace>() ) return (CollideSpace*)pNode;
		pNode = pNode->GetParent();
	}
	return NULL;
} // CollideSpace::FindParentSpace

void OnCollide( void *data, dGeomID o1, dGeomID o2 )
{
    CollideSpace* pSpace = CollideSpace::s_pCurSpace;
    if (pSpace) pSpace->ProcessCollision( data, (OdeID)o1, (OdeID)o2 );
}

void CollideSpace::Render()
{
	if (!m_PID) 
	{
		CollideSpace* pParentSpace = FindParentSpace();
		m_PID = CreateSpace( pParentSpace ? pParentSpace->GetSpaceID() : NULL );
		m_ContactGroupID = dJointGroupCreate( 0 );
	}

	if (!m_Contacts) m_Contacts = new dContact[m_MaxContacts];

	SNode::Render();
	if (!m_PID) return;

	CollideSpace* pSpace = s_pCurSpace;
	s_pCurSpace = this;
	dJointGroupEmpty( (dJointGroupID)m_ContactGroupID );
	m_NContacts = 0;
	m_NCollidedBodies = 0;
	dSpaceCollide( (dSpaceID)m_PID, 0, OnCollide );
	s_pCurSpace = pSpace;
} // CollideSpace::Render

void CollideSpace::ProcessCollision( void *data, OdeID o1, OdeID o2 )
{
	assert( m_Contacts );
    dContact* pContacts = (dContact*)m_Contacts;

	//  skip connected bodies
	dBodyID bodyID1 = dGeomGetBody( (dGeomID)o1 );
	dBodyID bodyID2 = dGeomGetBody( (dGeomID)o2 );
	if (bodyID1 && bodyID2 && dAreConnected( bodyID1, bodyID2 )) return;
	if (!bodyID1 && !bodyID2) return;
	
	//  do collision
	int nContacts = dCollide( (dGeomID)o1, (dGeomID)o2, m_MaxContacts, &pContacts[0].geom, sizeof( dContact ) );
	if (nContacts == 0) return;

	m_NCollidedBodies++;

	if (m_bDrawContacts) { IRS->SetWorldTM( Matrix4D::identity ); rsEnableZ( PhysicsSystem::s_bEnableGizmoZ ); }
	nContacts = tmin( nContacts, m_MaxContacts - m_NContacts, m_MaxPerBodyContacts );
	for (int i = 0; i < nContacts; i++) 
	{
		DWORD mode = dContactBounce | dContactSoftERP | dContactSoftCFM;

		if (m_bFDir1)	mode |= dContactFDir1;
		if (m_bApprox1) mode |= dContactApprox1_1;
		if (m_bApprox2) mode |= dContactApprox1_2;

		pContacts[i].surface.mode			= mode;
		pContacts[i].surface.mu			    = m_Mu;	
		pContacts[i].surface.mu2			= m_Mu2;	
		pContacts[i].surface.bounce		    = m_Bounce;
		pContacts[i].surface.bounce_vel	    = m_BounceVel;
		pContacts[i].surface.soft_erp		= m_SoftERP;
		pContacts[i].surface.soft_cfm		= m_SoftCFM;
		pContacts[i].surface.motion1		= m_Motion1; 
		pContacts[i].surface.motion2		= m_Motion2;
		pContacts[i].surface.slip1			= m_Slip1;	
		pContacts[i].surface.slip2			= m_Slip2;	

		m_NContacts++;

		dJointID jID = dJointCreateContact( GetCurWorldID(), (dJointGroupID)GetContactGroupID(), &pContacts[i] );
		//  here should be firing event of (Contact, Material, BodyID1, BodyID2, Position, Normal, whatever...)

		dJointAttach( jID, bodyID1, bodyID2 ); 
		if (m_bDrawContacts)
		{
			DrawRay( Ray3D( Vector3D(	pContacts[i].geom.pos[0]/GetCurWorldScale(),
										pContacts[i].geom.pos[1]/GetCurWorldScale(),
										pContacts[i].geom.pos[2]/GetCurWorldScale() ),
							Vector3D(	pContacts[i].geom.normal[0], 
										pContacts[i].geom.normal[1], 
										pContacts[i].geom.normal[2] ) ), PhysicsSystem::s_ContactColor, 30.0f ); 
		}
	}
	if (m_bDrawContacts) rsFlushLines3D();
} // CollideSpace::ProcessCollision

/*****************************************************************************/
/*	CollideSimpleSpace implementation
/*****************************************************************************/
CollideSimpleSpace::~CollideSimpleSpace()
{
}

void CollideSimpleSpace::Render()
{
	Parent::Render();
}

void CollideSimpleSpace::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
}

void CollideSimpleSpace::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
}

void CollideSimpleSpace::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CollideSimpleSpace", this );
}

OdeID CollideSimpleSpace::CreateSpace( OdeID parentSpaceID )
{
	return (dSpaceID)dSimpleSpaceCreate( (dSpaceID)parentSpaceID );
}

/*****************************************************************************/
/*	CollideHashSpace implementation
/*****************************************************************************/
CollideHashSpace::~CollideHashSpace()
{
}

void CollideHashSpace::Render()
{
	Parent::Render();
}

void CollideHashSpace::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
}

void CollideHashSpace::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
}

void CollideHashSpace::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CollideHashSpace", this );
}

OdeID CollideHashSpace::CreateSpace( OdeID parentSpaceID )
{
	return (dSpaceID)dHashSpaceCreate( (dSpaceID)parentSpaceID );
}

/*****************************************************************************/
/*	CollideQuadSpace implementation
/*****************************************************************************/
CollideQuadSpace::CollideQuadSpace() 
{
	m_Center	= Vector3D::null;	
	m_Extents	= Vector3D( 5000.0f, 5000.0f, 5000.0f );	
	m_Depth		= 4;	
}

CollideQuadSpace::~CollideQuadSpace()
{
}

void CollideQuadSpace::Render()
{
	Parent::Render();
}

void CollideQuadSpace::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
}

void CollideQuadSpace::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
}

void CollideQuadSpace::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CollideQuadSpace", this );
	pm.f( "Depth",	m_Depth );
	pm.f( "CenterX",	m_Center.x );
	pm.f( "CenterY",	m_Center.y );
	pm.f( "CenterZ",	m_Center.z );
	pm.f( "ExtX",		m_Extents.x );
	pm.f( "ExtY",		m_Extents.y );
	pm.f( "ExtZ",		m_Extents.z );
} // CollideQuadSpace::Expose

OdeID CollideQuadSpace::CreateSpace( OdeID parentSpaceID )
{
	dVector3 dc, dext;
	Convert( dc, m_Center );
	Convert( dext, m_Extents );
	return (dSpaceID)dQuadTreeSpaceCreate( (dSpaceID)parentSpaceID, dc, dext, m_Depth );
} // CollideQuadSpace::CreateSpace

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
		dBodyAddRelForceAtRelPos( (dBodyID)m_pBody->GetBodyID(), f.x, f.y, f.z, p.x, p.y, p.z );
	}
	else
	{
		dBodyAddForceAtRelPos( (dBodyID)m_pBody->GetBodyID(), f.x, f.y, f.z, p.x, p.y, p.z );
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
	pm.m( "Apply", &PushForce::Apply );
}


