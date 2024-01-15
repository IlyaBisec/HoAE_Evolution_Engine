/*****************************************************************************/
/*	File:	sgVehicle.h
/*	Desc:	Vehicle controller
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-22-2003
/*****************************************************************************/
#ifndef __SGVEHICLE_H__
#define __SGVEHICLE_H__


class JointHinge2;

/*****************************************************************************/
/*	Class:	Wheel
/*	Desc:	Vehicle wheel
/*****************************************************************************/
class Wheel : public Body
{
	Hinge2Joint*			m_pJoint;
public:	
	
	void					SetSteerAngle		( float angle );
	void					SetAngleVelocity	( float vel );
	void					HandBrake			();

	DECLARE_SCLASS(Wheel,Body,WHEL);
}; // class Wheel

/*****************************************************************************/
/*	Class:	Axle
/*	Desc:	Vehicle axle
/*****************************************************************************/
class Axle : public SNode
{
	Wheel*					m_pLeftWheel;
	Wheel*					m_pRightWheel;
public:
	DECLARE_SCLASS(Axle,SNode,AXLE);
}; // class Axle

/*****************************************************************************/
/*	Class:	Vehicle
/*	Desc:	Vehicle PhysicsObject dynamic object
/*****************************************************************************/
class Vehicle : public PhysicsObject
{
	Axle*			m_pFrontAxle;
	Axle*			m_pRearAxle;

public:
	DECLARE_SCLASS(Vehicle,PhysicsObject,VEHL);
}; // class Vehicle

/*****************************************************************************/
/*	Class:	VehicleController
/*	Desc:	Vehicle controller
/*****************************************************************************/
class VehicleController : public Controller, public IInputDispatcher
{
	Hinge2Joint*			m_pJointLF;
	Hinge2Joint*			m_pJointRF;
	Hinge2Joint*			m_pJointLB;
	Hinge2Joint*			m_pJointRB;

	TransformNode*			m_pSteeringWheel;
	TransformNode*			m_pChassi;

	float					m_Steer;
	float					m_Speed;
	float					m_MinSpeed, m_MaxSpeed;
	float					m_MinSteerAngle, m_MaxSteerAngle;
	float					m_SteerDelta;	
	float					m_SpeedDelta;	

	//  camera control parameters
	Vector3D				m_LookAt;			//  lookat point, relative to chassis center of mass
	float					m_PosDistance;		//  distance from camera position to cassis center of mass
	float					m_PosHeight;		//  camera position height above center of mass
	float					m_AngularRelax;		//  rate to match desired camera direction

public:
					VehicleController	();
					~VehicleController	();

	virtual bool	OnKeyDown			( DWORD keyCode, DWORD flags );	
	virtual bool	OnChar				( DWORD charCode, DWORD flags );	
	virtual bool	OnKeyUp				( DWORD keyCode, DWORD flags );
	virtual void	Render				();

	virtual void	Serialize			( OutStream& os ) const;
	virtual void	Unserialize			( InStream& is  );
	virtual void	Expose				( PropertyMap& pm );	

	void			Init				();

	DECLARE_SCLASS(VehicleController, Controller, VEHI);
}; // class VehicleController



#endif // __SGVEHICLE_H__
