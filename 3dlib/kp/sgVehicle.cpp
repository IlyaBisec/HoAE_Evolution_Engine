/*****************************************************************************/
/*	File:	sgVehicle.h
/*	Desc:	Vehicle controller
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-22-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgController.h"
#include "sg.h"
#include "sgPhysics.h"
#include "sgVehicle.h"



/*****************************************************************************/
/*	VehicleController implementation
/*****************************************************************************/
VehicleController::VehicleController()
{
	m_Steer			= 0.0f;
	m_Speed			= 0.0f;
	m_MinSpeed		= -10.f;
	m_MaxSpeed		= 10.f;
	m_MinSteerAngle	= -c_PId6;
	m_MaxSteerAngle	= c_PId6;
	m_SteerDelta	= 0.01f;
	m_SpeedDelta	= 0.005f;

	m_pJointLF		= NULL;
	m_pJointRF		= NULL;
	m_pJointLB		= NULL;
	m_pJointRB		= NULL;
	m_pSteeringWheel= NULL;
	m_pChassi		= NULL;
} // VehicleController::VehicleController

VehicleController::~VehicleController()
{
}

bool VehicleController::OnKeyDown( DWORD keyCode, DWORD flags )
{
	return false;
}

bool VehicleController::OnChar( DWORD charCode, DWORD flags )
{
	return false;
} // VehicleController::OnChar

bool VehicleController::OnKeyUp( DWORD keyCode, DWORD flags )
{
	return false;
}

void VehicleController::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_SpeedDelta << m_SteerDelta;
}

void VehicleController::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_SpeedDelta >> m_SteerDelta;
}

void VehicleController::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "VehicleController", this );
	pm.f( "MinSpeed", 			m_MinSpeed 		);
	pm.f( "MaxSpeed", 			m_MaxSpeed 		);
	pm.f( "SpeedDelta",			m_SpeedDelta	);
	pm.f( "MinSteerAngle", 		m_MinSteerAngle );
	pm.f( "MaxSteerAngle", 		m_MaxSteerAngle );
	pm.f( "SteerDelta",			m_SteerDelta	);

	pm.f( "Steer", 				m_Steer, NULL, true );
	pm.f( "Speed", 				m_Speed, NULL, true );

	pm.f( "LookAt.x",				m_LookAt.x 		);
	pm.f( "LookAt.y",				m_LookAt.y 		);
	pm.f( "LookAt.z",				m_LookAt.z 		);
	pm.f( "m_PosDistance",		m_PosDistance	);		
	pm.f( "m_PosHeight",			m_PosHeight		);		
	pm.f( "m_AngularRelax",		m_AngularRelax  );
	pm.m( "Init",				Init );
} // VehicleController::Expose

void VehicleController::Init()
{
	
}

void VehicleController::Render()
{
	m_pJointLF			= (Hinge2Joint*)GetInput( 0 );
	m_pJointRF			= (Hinge2Joint*)GetInput( 1 );
	m_pJointLB			= (Hinge2Joint*)GetInput( 2 );
	m_pJointRB			= (Hinge2Joint*)GetInput( 3 );
	m_pSteeringWheel	= (TransformNode*)GetInput( 4 );
	
	dBodyID wheelLB = m_pJointLB ? dJointGetBody( m_pJointLB->GetJointID(), 1 ) : NULL;
	dBodyID wheelLF = m_pJointLF ? dJointGetBody( m_pJointLF->GetJointID(), 1 ) : NULL;
	dBodyID wheelRB = m_pJointRB ? dJointGetBody( m_pJointRB->GetJointID(), 1 ) : NULL;
	dBodyID wheelRF = m_pJointRF ? dJointGetBody( m_pJointRF->GetJointID(), 1 ) : NULL;

	if (!wheelLB || !wheelRB || !wheelLF || !wheelRF) return; 

	dBodySetFiniteRotationMode( wheelLB, 1 );
	dBodySetFiniteRotationMode( wheelRB, 1 );
	dBodySetFiniteRotationMode( wheelLF, 1 );
	dBodySetFiniteRotationMode( wheelRF, 1 );

	if (!m_pJointLF || !m_pJointRF || !m_pJointLB || !m_pJointRB) return;
	
	if (GetKeyState( VK_UP ) < 0)
	{
		/*dJointSetHinge2Param( m_pJointLB->GetJointID(), dParamVel2, -m_SpeedDelta );
		dJointSetHinge2Param( m_pJointRB->GetJointID(), dParamVel2, m_SpeedDelta );

		dJointSetHinge2Param( m_pJointLF->GetJointID(), dParamVel2, -m_SpeedDelta );
		dJointSetHinge2Param( m_pJointRF->GetJointID(), dParamVel2, m_SpeedDelta );*/
		dBodyAddRelTorque( wheelRF, m_SpeedDelta, 0.0f, 0.0f );
		dBodyAddRelTorque( wheelLF, m_SpeedDelta, 0.0f, 0.0f );
		dBodyAddRelTorque( wheelRB, m_SpeedDelta, 0.0f, 0.0f );
		dBodyAddRelTorque( wheelLB, m_SpeedDelta, 0.0f, 0.0f );
	}

	if (GetKeyState( VK_DOWN ) < 0)
	{
		//dJointSetHinge2Param( m_pJointLB->GetJointID(), dParamVel2, m_SpeedDelta );
		//dJointSetHinge2Param( m_pJointRB->GetJointID(), dParamVel2, -m_SpeedDelta );

		//dJointSetHinge2Param( m_pJointLF->GetJointID(), dParamVel2, m_SpeedDelta );
		//dJointSetHinge2Param( m_pJointRF->GetJointID(), dParamVel2, -m_SpeedDelta );
		dBodyAddRelTorque( wheelRF, -m_SpeedDelta, 0.0f, 0.0f );
		dBodyAddRelTorque( wheelLF, -m_SpeedDelta, 0.0f, 0.0f );
		dBodyAddRelTorque( wheelRB, -m_SpeedDelta, 0.0f, 0.0f );
		dBodyAddRelTorque( wheelLB, -m_SpeedDelta, 0.0f, 0.0f );
	}

	if (GetKeyState( VK_RIGHT ) < 0)
	{
		float angL = dJointGetHinge2Angle1( m_pJointLF->GetJointID() );
		float angR = dJointGetHinge2Angle1( m_pJointRF->GetJointID() );
		
		angL -= m_SteerDelta;
		angR -= m_SteerDelta;
		
		dJointSetHinge2Param( m_pJointLF->GetJointID(), dParamVel, m_SteerDelta );
		dJointSetHinge2Param( m_pJointRF->GetJointID(), dParamVel, m_SteerDelta );
	}

	if (GetKeyState( VK_LEFT ) < 0)
	{
		float angL = dJointGetHinge2Angle1( m_pJointLF->GetJointID() );
		float angR = dJointGetHinge2Angle1( m_pJointRF->GetJointID() );

		angL += m_SteerDelta;
		angR += m_SteerDelta;
		
		dJointSetHinge2Param( m_pJointLF->GetJointID(), dParamVel, -m_SteerDelta );
		dJointSetHinge2Param( m_pJointRF->GetJointID(), dParamVel, -m_SteerDelta );
	}

	if (GetKeyState( VK_SHIFT ) < 0)
	{
		dJointSetHinge2Param( m_pJointRB->GetJointID(), dParamVel2, 0 );
		dJointSetHinge2Param( m_pJointLB->GetJointID(), dParamVel2, 0 );
		dJointSetHinge2Param( m_pJointRF->GetJointID(), dParamVel2, 0 );
		dJointSetHinge2Param( m_pJointLF->GetJointID(), dParamVel2, 0 );

		dJointSetHinge2Param( m_pJointRB->GetJointID(), dParamVel, 0 );
		dJointSetHinge2Param( m_pJointLB->GetJointID(), dParamVel, 0 );
		dJointSetHinge2Param( m_pJointRF->GetJointID(), dParamVel, 0 );
		dJointSetHinge2Param( m_pJointLF->GetJointID(), dParamVel, 0 );
	}

	//  steering wheel
	if (m_pSteeringWheel)
	{
	
	}
} // VehicleController::Render


