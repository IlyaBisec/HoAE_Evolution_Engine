/*****************************************************************************/
/*	File:	sgPhysics.h
/*	Desc:	Physics wrapper
/*	Author:	Ruslan Shestopalyuk
/*	Date:	11-12-2003
/*****************************************************************************/
#ifndef __SGPHYSICS_H__
#define __SGPHYSICS_H__

#include "ode/ode.h"

enum StepMode
{
	smNormal	= 0,
	smStepFast	= 1,
	smQuickStep	= 2
}; // enum StepMode

ENUM(StepMode,"StepMode", 
				en_val(smNormal,	"Normal"	) << 
				en_val(smStepFast,	"StepFast"	) <<
				en_val(smQuickStep, "QuickStep"	) );


/*****************************************************************************/
/*	Class:	PhysicsSystem
/*	Desc:	Manages physics simulation
/*****************************************************************************/
class PhysicsSystem : public SNode
{
	dWorldID					m_PID;			//  ode's world handle
	Vector3D					m_Gravity;		//  global gravity vector
	float						m_ERP;			//  global error reduction parameter
	float						m_CFM;			//  global constraint force mixing
	float						m_MaxTimeStep;	//  minimal time step allowed

	DWORD						m_LastTime;		//  
	float						m_StepScale;
	float						m_WorldScale;	//  global scale applied to position values
	float						m_TimeScale;	//  global scale applied to delta time
	bool						m_bFixedStep;
	float						m_FixedStepSize;
	
	StepMode					m_StepMode;
	int							m_MaxStepFastIter;

public:
								PhysicsSystem	();
	virtual						~PhysicsSystem	();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );
	virtual void				Expose			( PropertyMap& pm );	

	float						GetCFM			() const { return m_CFM; }
	float						GetERP			() const { return m_ERP; }

	float						GetGravityX		() const { return m_Gravity.x;	}
	float						GetGravityY		() const { return m_Gravity.y;	}
	float						GetGravityZ		() const { return m_Gravity.z;	}
	const Vector3D&				GetGravity		() const { return m_Gravity;	}

	void						SetCFM			( float val );
	void						SetERP			( float val );

	void						CreateBody		();

	void						Step			();
	void						SetGravityX		( float val );  
	void						SetGravityY		( float val ); 
	void						SetGravityZ		( float val );  
	void						SetGravity		( const Vector3D& vec );

	void						SetEarthGravity	();

	bool						IsValid			() const { return m_PID != NULL; }

	dWorldID					GetWorldID		() { return m_PID; }
	float						GetWorldScale	() const { return m_WorldScale; }
	void						SetWorldScale	( float val );
	float						GetTimeScale	() const { return m_TimeScale; }	
	static PhysicsSystem*		GetCurSystem	() { return s_pCurPhysSystem; }

	DECLARE_SCLASS(PhysicsSystem,SNode,PHYS);

	static float 				s_JointHandleSize; 
	static DWORD 				s_JointAnchorColorF;
	static DWORD 				s_JointAnchorColor; 
	static DWORD 				s_JointAxis1ColorF; 
	static DWORD 				s_JointAxis1Color;  
	static DWORD 				s_JointAxis2ColorF; 
	static DWORD 				s_JointAxis2Color;  
	static DWORD 				s_GeomColor;		
	static DWORD 				s_GeomColorF;
	static DWORD 				s_ContactColor;
	static bool					s_bEnableGizmoZ;

protected:
	static PhysicsSystem*		s_pCurPhysSystem;

}; // class PhysicsSystem

/*****************************************************************************/
/*	Class:	Articulated
/*	Desc:	Physically simulated articulated object - e.g car, human etc.
/*****************************************************************************/
class Articulated : public SNode
{
public:
								Articulated		();
	virtual void				Expose			( PropertyMap& pm );		
	void						Reset			();
	void						ShowJoints		( bool bShow = true );
	void						ShowBodies		( bool bShow = true );
	void						ShowGeoms		( bool bShow = true );
	bool						IsShowJoints	() const { return m_bShowJoints; }
	bool						IsShowBodies	() const { return m_bShowBodies; }
	bool						IsShowGeoms		() const { return m_bShowGeoms; }

	void						Init			();


	DECLARE_SCLASS(Articulated,SNode,ARTJ);

private:
	bool						m_bShowJoints;
	bool						m_bShowBodies;
	bool						m_bShowGeoms;

}; // class Articulated

/*****************************************************************************/
/*	Class:	Body
/*	Desc:	Represents single rigid body, for which physics simulation is applied
/*****************************************************************************/
class Body : public Controller
{
	dBodyID						m_PID;
	Vector3D					m_Velocity;		//  velocity vector
	Vector3D					m_AngVelocity;	//  angular velocity vector

	float						m_Mass;			//  mass
	float						m_Density;		//  body density == mass/volume


	Matrix3D					m_Inertia;		//  inertia tensor (mass distribution)
	TransformNode*				m_pTMNode;

	Vector3D					m_ScaleVector;	//  body scaling vector
	Matrix4D					m_MassCenterTM;	//  transform of mass center in the space of node we are attached to
												//  (this remains constant all the time)
	
public:
								Body		();
	virtual						~Body		();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );
	virtual void				Expose			( PropertyMap& pm );	
	virtual void				Reset			();

	float						GetMass			() const { return m_Mass; }
	void						SetMass			( float val ); 

	float						GetDensity		() const { return m_Density; }
	void						SetDensity		( float val ); 

	void						SetMass			( const dMass& mass );
	dBodyID						GetBodyID		() const { return m_PID; }

	bool						IsValid			() const { return m_PID != NULL; }
	
	
	void						CreateBody		();
	void						InitBodyTM		();
	
	Matrix4D					GetTransform	();
	void						EnableBody		( bool bEnable = true );
	bool						IsBodyEnabled	() const;

	void						SetLocalCMX		( float val );
	void						SetLocalCMY		( float val );
	void						SetLocalCMZ		( float val );

	void						SetLocalCMRotX	( float val );
	void						SetLocalCMRotY	( float val );
	void						SetLocalCMRotZ	( float val );

	float						GetLocalCMX		() const;
	float						GetLocalCMY		() const;
	float						GetLocalCMZ		() const;

	float						GetLocalCMRotX	() const;
	float						GetLocalCMRotY	() const;
	float						GetLocalCMRotZ	() const;

	void						Rename			();

	DECLARE_SCLASS(Body,Controller,RIGB);
}; // class Body

class JointCluster;
/*****************************************************************************/
/*	Class:	Joint
/*	Desc:	Joint (constraint) applied to the rigid bodies
/*****************************************************************************/
class Joint : public SNode
{
protected:
	dJointID					m_PID;
	Body*						m_pBody1;
	Body*						m_pBody2;

public:
								Joint	();
	virtual						~Joint	();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );
	virtual void				Expose			( PropertyMap& pm );	
	bool						IsValid			() const { return m_PID != NULL; }
	void						InitJoint		();
	virtual void				Reset			();
	bool						NeedTwoBodies	() const { return false; }
	dJointID					GetJointID		() const { return m_PID; }
	Vector3D					GetDirection1	( const Vector3D& pos ) const;
	Vector3D					GetDirection2	( const Vector3D& pos ) const;

	DECLARE_SCLASS(Joint,SNode,PHYJ);

protected:
	JointCluster*				FindParentJointCluster();
	virtual dJointID			CreateJoint		( dJointGroupID groupID ) { return NULL; }
	virtual void				SetParams		(){}
	virtual void				GetParams		(){}
	virtual Matrix4D			GetTransform	();
	void						GetFromBodies	( Vector3D& anchor, Vector3D& dir1, Vector3D& dir2 );
	virtual void				PositionFromBodies() {}

}; // class Joint

/*****************************************************************************/
/*	Class:	StopMotorJoint
/*	Desc:	Joint with stop/motor properties
/*****************************************************************************/
class StopMotorJoint : public Joint
{
protected:
	//  stop parameters
	float 						m_LoStop;		//  low stop angle or position 
	float 						m_HiStop;		//	high stop angle or position
	float 						m_StopERP;		//  ERP used by the stops. 
	float 						m_StopCFM;		//  CFM value used by the stops

	//  motor parameters
	float 						m_MotorVel;		//  desired motor velocity (angular or linear) 
	float 						m_MotorFMax;	//  maximum force or torque of motor 

	float 						m_FudgeFactor;  //  stabilizing factor to avoid jumpiness at stops
	float 						m_Bounce;		//  the bouncyness of the stops
	float 						m_CFM;			//  CFM value used when not at a stop. 

public:
								StopMotorJoint	();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );
	virtual void				Expose			( PropertyMap& pm );	

	DECLARE_SCLASS(StopMotorJoint,Joint,SMJO);

protected:
}; // class Joint

/*****************************************************************************/
/*	Class:	JointCluster
/*	Desc:	Cluster of physical joints
/*****************************************************************************/
class JointCluster : public Group
{
	dJointGroupID				m_PID;
	int							m_MaxSize;

public:
								JointCluster	();
								~JointCluster	();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );
	virtual void				Expose			( PropertyMap& pm );	
	bool						IsValid			() const { return m_PID != NULL; }
	dJointGroupID				GetClusterID	() const { return m_PID; }
	void						RenameJoints	();

	DECLARE_SCLASS(JointCluster,Group,PHJC);
}; // class JointCluster

/*****************************************************************************/
/*	Class:	BallSocketJoint
/*	Desc:	Ball-socket joint
/*****************************************************************************/
class BallSocketJoint : public Joint
{
	Vector3D					m_Anchor;			//  ball anchor

public:
								BallSocketJoint	();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );
	virtual void				Expose			( PropertyMap& pm );	

	DECLARE_SCLASS(BallSocketJoint,Joint,JBSO);

protected:
	virtual dJointID			CreateJoint		( dJointGroupID groupID );
	virtual void				SetParams		();
	virtual void				PositionFromBodies();

}; // class BallSocketJoint

/*****************************************************************************/
/*	Class:	SliderJoint
/*	Desc:	Slider joint
/*****************************************************************************/
class SliderJoint : public StopMotorJoint
{
	Vector3D					m_Axis;			//  sliding axis

public:
								SliderJoint		();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );
	virtual void				Expose			( PropertyMap& pm );	

	float						GetSliderPosition() const;
	float						GetSliderPositionRate() const;
	Vector3D					GetAnchor		() const;

	DECLARE_SCLASS(SliderJoint,StopMotorJoint,JSLI);

protected:
	virtual dJointID			CreateJoint		( dJointGroupID groupID );
	virtual void				SetParams		();
	virtual void				PositionFromBodies();
}; // class SliderJoint

/*****************************************************************************/
/*	Class:	HingeJoint
/*	Desc:	Hinge joint
/*****************************************************************************/
class HingeJoint : public StopMotorJoint
{
	Vector3D					m_Anchor;			//  hinge anchor
	Vector3D					m_Axis;				//  hinge axis

public:
								HingeJoint		();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );
	virtual void				Expose			( PropertyMap& pm );	

	float						GetAngle		() const;
	float						GetAngleRate	() const;
	DECLARE_SCLASS(HingeJoint,StopMotorJoint,JHIN);

protected:
	virtual dJointID			CreateJoint		( dJointGroupID groupID );
	virtual void				SetParams		();
	virtual void				PositionFromBodies();

}; // class HingeJoint

/*****************************************************************************/
/*	Class:	Hinge2Joint
/*	Desc:	Hinge2 joint
/*****************************************************************************/
class Hinge2Joint : public StopMotorJoint
{
	float						m_SuspensionERP;	// suspension error reduction parameter 
	float						m_SuspensionCFM;	// suspension constraint force mixing value

	float 						m_MotorVel2;		//  desired motor velocity at axis2
	float 						m_MotorFMax2;		//  maximum torque of motor at axis2

	Vector3D					m_Anchor;			//  hinge2 anchor
	Vector3D					m_Axis1;			//  hinge2 first axis
	Vector3D					m_Axis2;			//  hinge2 second axis

public:
								Hinge2Joint		();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );
	virtual void				Expose			( PropertyMap& pm );	
	bool						NeedTwoBodies	() const { return true; }

	float						GetAngle1		() const;
	float						GetAngle1Rate	() const;
	float						GetAngle2Rate	() const;
	float						GetMaxSteerAngle() const { return m_HiStop; }
	float						GetMinSteerAngle() const { return m_LoStop; }
	
	void						PositionFromBodies();

	DECLARE_SCLASS(Hinge2Joint,StopMotorJoint,JHI2);

protected:
	virtual dJointID			CreateJoint		( dJointGroupID groupID );
	virtual void				SetParams		();
	virtual void				GetParams		();

}; // class Hinge2Joint

/*****************************************************************************/
/*	Class:	UniversalJoint
/*	Desc:	Universal joint
/*****************************************************************************/
class UniversalJoint : public StopMotorJoint
{
	Vector3D					m_Anchor;			//  anchor
	Vector3D					m_Axis1;			//  first axis
	Vector3D					m_Axis2;			//  second axis

public:
								UniversalJoint	();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );
	virtual void				Expose			( PropertyMap& pm );	
	virtual void				PositionFromBodies();

	DECLARE_SCLASS(UniversalJoint,StopMotorJoint,JUNI);

protected:
	virtual dJointID			CreateJoint		( dJointGroupID groupID );
	virtual void				SetParams		();
}; // class UniversalJoint

/*****************************************************************************/
/*	Class:	FixedJoint
/*	Desc:	Fixed joint
/*****************************************************************************/
class FixedJoint : public Joint
{
public:
								FixedJoint		();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );
	virtual void				Expose			( PropertyMap& pm );	

	DECLARE_SCLASS(FixedJoint,Joint,JFIX);

protected:
	virtual dJointID			CreateJoint		( dJointGroupID groupID );

}; // class FixedJoint

/*****************************************************************************/
/*	Class:	ContactJoint
/*	Desc:	Contact joint
/*****************************************************************************/
class ContactJoint : public Joint
{
public:
								ContactJoint		();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );
	virtual void				Expose			( PropertyMap& pm );	

	DECLARE_SCLASS(ContactJoint,Joint,JCON);

protected:
	virtual dJointID			CreateJoint		( dJointGroupID groupID );

}; // class ContactJoint

/*****************************************************************************/
/*	Enum:	AMotorMode
/*****************************************************************************/
enum AMotorMode
{
	ammUnknown	= 0,
	ammUser		= 1,
	ammEuler	= 2
}; // enum AMotorMode

/*****************************************************************************/
/*	Class:	AMotorJoint
/*	Desc:	Angular Motor joint
/*****************************************************************************/
class AMotorJoint : public StopMotorJoint
{
	Vector3D					m_Axis1;
	Vector3D					m_Axis2;

	//  second axis stop parameters
	float 						m_LoStop2;		//  low stop angle or position 
	float 						m_HiStop2;		//	high stop angle or position
	float 						m_StopERP2;		//  ERP used by the stops. 
	float 						m_StopCFM2;		//  CFM value used by the stops

	//  second axis motor parameters
	float 						m_MotorVel2;	//  desired motor velocity (angular or linear) 
	float 						m_MotorFMax2;	//  maximum force or torque of motor 

	float 						m_FudgeFactor2; //  stabilizing factor to avoid jumpiness at stops
	float 						m_Bounce2;		//  the bouncyness of the stops
	float 						m_CFM2;			//  CFM value used when not at a stop. 

	//  third axis stop parameters
	float 						m_LoStop3;		
	float 						m_HiStop3;		
	float 						m_StopERP3;		
	float 						m_StopCFM3;		

	//  third axis motor parameters
	float 						m_MotorVel3;	
	float 						m_MotorFMax3;	

	float 						m_FudgeFactor3; 
	float 						m_Bounce3;		
	float 						m_CFM3;			

public:
								AMotorJoint			();
	virtual void				Render				();
	virtual void				Serialize			( OutStream& os ) const;
	virtual void				Unserialize			( InStream& is  );
	virtual void				Expose				( PropertyMap& pm );	
	virtual void				PositionFromBodies	();

	DECLARE_SCLASS(AMotorJoint,StopMotorJoint,JAMO);

protected:
	virtual dJointID			CreateJoint		( dJointGroupID groupID );
	virtual void				SetParams		();

}; // class AMotorJoint


ENUM( AMotorMode,"AMotorMode",	en_val( ammUnknown, "Unknown"	) << 
									en_val( ammUnknown, "User"		) << 
									en_val( ammUnknown, "Euler"		) );

/*****************************************************************************/
/*	Class:	SkeletalJoint
/*	Desc:	Combination of ball-socket and AMotor joint
/*****************************************************************************/
class SkeletalJoint : public StopMotorJoint
{
public:
								SkeletalJoint		();
	virtual void				Render				();
	virtual void				Serialize			( OutStream& os ) const;
	virtual void				Unserialize			( InStream& is  );
	virtual void				Expose				( PropertyMap& pm );	

	DECLARE_SCLASS(SkeletalJoint,StopMotorJoint,SKEJ);

protected:
	virtual dJointID			CreateJoint		( dJointGroupID groupID );
	virtual void				SetParams		();

}; // class SkeletalJoint

class CldSpace;
/*****************************************************************************/
/*	Class:	CldGeom
/*	Desc:	Base class for collision primitives
/*			Primitive can be attached to the rigid body, or straight to the 
/*				geometry in case it is not subject to the dynamics solver
/*****************************************************************************/
class CldGeom : public SNode
{
	dGeomID						m_PID;
public:
								CldGeom			();
	virtual						~CldGeom		();
	virtual void				Expose			( PropertyMap& pm );	
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );

	bool						IsValid			() const { return m_PID != NULL; }
	dGeomID						GetPrimitiveID	() const { return m_PID; }
	void						InitGeomTM		();
	virtual void				Reset			();

	DECLARE_SCLASS(CldGeom,SNode,CLPR);

protected:
	virtual dGeomID				CreateGeom		( dSpaceID spaceID ) { return NULL; }
	virtual	void				FitGeom			( const Primitive& prim ){}
	virtual bool				IsPlaceable		() const { return true; }
	Matrix4D					GetTransform	();

	CldSpace*					FindParentSpace	();
	Body*						GetAttachedBody	();
}; // class CldGeom

/*****************************************************************************/
/*	Class:	CldBox
/*	Desc:	Box collision primitive
/*****************************************************************************/
class CldBox : public CldGeom
{
	Vector3D					m_Extents;

public:
								CldBox		();
	virtual void				Render		();
	virtual void				Serialize	( OutStream& os ) const;
	virtual void				Unserialize	( InStream& is  );

	virtual void				Expose		( PropertyMap& pm );	

	DECLARE_SCLASS(CldBox,CldGeom,COLB);

protected:
	virtual dGeomID				CreateGeom	( dSpaceID spaceID );
	virtual	void				FitGeom		( const Primitive& prim );
}; // class CldBox

/*****************************************************************************/
/*	Class:	CldSphere
/*	Desc:	Sphere collision primitive
/*****************************************************************************/
class CldSphere : public CldGeom
{
	float						m_Radius;

public:
								CldSphere	();
	virtual void				Render		();
	virtual void				Serialize	( OutStream& os ) const;
	virtual void				Unserialize	( InStream& is  );

	virtual void				Expose		( PropertyMap& pm );	

	DECLARE_SCLASS(CldSphere,CldGeom,COLS);

protected:
	virtual dGeomID				CreateGeom	( dSpaceID spaceID );
	virtual	void				FitGeom		( const Primitive& prim );
}; // class CldSphere

/*****************************************************************************/
/*	Class:	CldCylinder
/*	Desc:	Cylinder collision primitive
/*****************************************************************************/
class CldCylinder : public CldGeom
{
	float						m_Height;
	float						m_Radius;

public:
								CldCylinder	();
	virtual void				Render		();
	virtual void				Serialize	( OutStream& os ) const;
	virtual void				Unserialize	( InStream& is  );

	virtual void				Expose		( PropertyMap& pm );	

	DECLARE_SCLASS(CldCylinder,CldGeom,COLC);

protected:
	virtual dGeomID				CreateGeom	( dSpaceID spaceID );
	virtual	void				FitGeom		( const Primitive& prim );
}; // class CldCylinder

/*****************************************************************************/
/*	Class:	CldCapsule
/*	Desc:	Capped cylinder collision primitive
/*****************************************************************************/
class CldCapsule : public CldGeom
{
	float						m_Height;
	float						m_Radius;

public:
								CldCapsule			();
	virtual void				Render				();
	virtual void				Serialize			( OutStream& os ) const;
	virtual void				Unserialize			( InStream& is  );

	virtual void				Expose				( PropertyMap& pm );	

	DECLARE_SCLASS(CldCapsule,CldGeom,CLCC);

protected:
	virtual dGeomID				CreateGeom	( dSpaceID spaceID );
	virtual	void				FitGeom		( const Primitive& prim );
}; // class CldCapsule

/*****************************************************************************/
/*	Class:	CldPlane
/*	Desc:	Plane collision primitive
/*****************************************************************************/
class CldPlane : public CldGeom
{
	Plane						m_Plane;

public:
								CldPlane	();
	virtual void				Render		();
	virtual void				Serialize	( OutStream& os ) const;
	virtual void				Unserialize	( InStream& is  );
	virtual void				Expose		( PropertyMap& pm );	

	DECLARE_SCLASS(CldPlane,CldGeom,CLPL);

protected:
	virtual dGeomID				CreateGeom	( dSpaceID spaceID );
	virtual	void				FitGeom		( const Primitive& prim );
	virtual bool				IsPlaceable	() const { return false; }

}; // class CldPlane

/*****************************************************************************/
/*	Class:	CldRay
/*	Desc:	Ray collision primitive
/*****************************************************************************/
class CldRay : public CldGeom
{
	float						m_Length;
public:
								CldRay		();
	virtual void				Render		();
	virtual void				Serialize	( OutStream& os ) const;
	virtual void				Unserialize	( InStream& is  );

	virtual void				Expose		( PropertyMap& pm );	

	DECLARE_SCLASS(CldRay,CldGeom,CLRA);

protected:
	virtual dGeomID				CreateGeom	( dSpaceID spaceID );
}; // class CldRay

/*****************************************************************************/
/*	Class:	CldTriMesh
/*	Desc:	TriMesh collision primitive
/*****************************************************************************/
class CldTriMesh : public CldGeom
{
	dTriMeshDataID				m_TriDataID;
	Geometry*					m_pMeshNode;
	Matrix4D					m_MeshTM;

	Primitive					m_CollisionMesh;
	std::vector<int>			m_Indices;

public:

								CldTriMesh	();
	virtual 					~CldTriMesh	();
	virtual void				Render		();
	virtual void				Serialize	( OutStream& os ) const;
	virtual void				Unserialize	( InStream& is  );
	virtual void				Expose		( PropertyMap& pm );	
	virtual bool				IsTriDataValid() const { return m_TriDataID != NULL; }
	virtual void				Reset			();


	DECLARE_SCLASS(CldTriMesh,CldGeom,CLTR);

protected:
	virtual dGeomID				CreateGeom			( dSpaceID spaceID );
	Geometry*					GetMeshNode			();
	void						InitMeshStructure	( Primitive& pri, 
														const Matrix4D& tm = Matrix4D::identity );
}; // class CldTriMesh

/*****************************************************************************/
/*	Class:	CldSpace
/*	Desc:	Base class for groups of collision objects
/*****************************************************************************/
class CldSpace : public SNode
{
	dSpaceID					m_PID;
	dJointGroupID				m_ContactGroupID;
	dContact*					m_Contacts;
	int							m_MaxContacts; 
	bool						m_bDrawContacts;
	int							m_NContacts;
	int							m_NCollidedBodies;
	int							m_MaxPerBodyContacts;

	//  contact reaction parameters
	float						m_Mu; 
	float						m_Mu2; 
	float						m_Bounce; 
	float						m_BounceVel; 
	float						m_SoftERP; 
	float						m_SoftCFM; 
	float						m_Motion1; 
	float						m_Motion2;
	float						m_Slip1;
	float						m_Slip2; 

	bool						m_bFDir1; 
	bool						m_bApprox1; 
	bool						m_bApprox2; 

public:
								CldSpace		();
	virtual						~CldSpace		();
	virtual void				Expose			( PropertyMap& pm );
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );
	virtual void				Render			();
	bool						IsValid			() const { return m_PID != NULL; }
	dSpaceID					GetSpaceID		() const { return m_PID; }
	dJointGroupID				GetContactGroupID() const { return m_ContactGroupID; }

	void						SetSpring		( float spring );
	void						SetDamping		( float damping );
	float						GetSpring		() const;
	float						GetDamping		() const;

	void						RenameGeoms		();

	DECLARE_SCLASS(CldSpace,SNode,CLSP);

protected:
	CldSpace*					FindParentSpace	();
	virtual dSpaceID			CreateSpace		( dSpaceID parentSpaceID ) { return NULL; }
	void						ProcessCollision( void *data, dGeomID o1, dGeomID o2 );

	static void					OnCollide		( void *data, dGeomID o1, dGeomID o2 );

	static CldSpace*			s_pCurSpace;

}; // class CldSpace

/*****************************************************************************/
/*	Class:	CldSimpleSpace
/*	Desc:	Simple collision space
/*****************************************************************************/
class CldSimpleSpace : public CldSpace
{
public:
								CldSimpleSpace	() {}
	virtual						~CldSimpleSpace	();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );

	virtual void				Expose			( PropertyMap& pm );	

	DECLARE_SCLASS(CldSimpleSpace,CldSpace,CLSS);

protected:
	virtual dSpaceID			CreateSpace		( dSpaceID parentSpaceID );
}; // class CldSimpleSpace

/*****************************************************************************/
/*	Class:	CldHashSpace
/*	Desc:	Hash collision space
/*****************************************************************************/
class CldHashSpace : public CldSpace
{
public:
								CldHashSpace	() {}
	virtual						~CldHashSpace	();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );

	virtual void				Expose			( PropertyMap& pm );	

	DECLARE_SCLASS(CldHashSpace,CldSpace,CLSH);

protected:
	virtual dSpaceID			CreateSpace		( dSpaceID parentSpaceID );
}; // class CldHashSpace

/*****************************************************************************/
/*	Class:	CldQuadSpace
/*	Desc:	Quad collision space
/*****************************************************************************/
class CldQuadSpace : public CldSpace
{
	Vector3D					m_Center;		//  quadtree center
	Vector3D					m_Extents;		//  quadtree extents
	int							m_Depth;		//  quadtree depth

public:
								CldQuadSpace	();
	virtual						~CldQuadSpace	();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );

	virtual void				Expose			( PropertyMap& pm );	

	DECLARE_SCLASS(CldQuadSpace,CldSpace,CLSQ);

protected:
	virtual dSpaceID			CreateSpace		( dSpaceID parentSpaceID );
}; // class CldQuadSpace

/*****************************************************************************/
/*	Class:	PushForce
/*	Desc:	Gives impulse to the body
/*****************************************************************************/
class PushForce : public TransformNode
{
	float			m_Amount;
	float			m_ForceCone;
	float			m_ApplyPointRadius;
	Body*			m_pBody;
	bool			m_bBodyRelative;

public:
					PushForce	();
	void			Apply		();

	virtual void	Render		();
	virtual void	Serialize	( OutStream& os ) const;
	virtual void	Unserialize	( InStream& is  );
	virtual void	Expose		( PropertyMap& pm );	

	DECLARE_SCLASS(PushForce,TransformNode,PUFO);
}; // class PushForce



#endif // __SGPHYSICS_H__
