/*****************************************************************************/
/*	File:	sgPhysics.h
/*	Desc:	Physics wrapper
/*	Author:	Ruslan Shestopalyuk
/*	Date:	11-12-2003
/*****************************************************************************/
#ifndef __SGPHYSICS_H__
#define __SGPHYSICS_H__

typedef void* OdeID;
enum StepMode
{
    smNormal	= 0,
    smStepFast	= 1,
    smQuickStep	= 2
}; // enum StepMode
/*****************************************************************************/
/*	Class:	PhysicsSystem
/*	Desc:	Manages physics simulation
/*****************************************************************************/
class PhysicsSystem : public SNode
{
	OdeID					    m_PID;			//  ode's world handle
	Vector3D					m_Gravity;		//  global gravity vector
	float						m_ERP;			//  global error reduction parameter
	float						m_CFM;			//  global constraint force mixing
	float						m_MaxTimeStep;	//  minimal time step allowed

	DWORD						m_LastTime;		//  
	float						m_StepScale;
	float						m_WorldScale;	//  global scale applied to position values
	float						m_TimeScale;	//  global scale applied to delta time
	bool						m_bFixedStep;
    bool                        m_bInited;
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

    void                        Init            ();

	float						GetCFM			() const { return m_CFM; }
	float						GetERP			() const { return m_ERP; }

	float						GetGravityX		() const { return m_Gravity.x;	}
	float						GetGravityY		() const { return m_Gravity.y;	}
	float						GetGravityZ		() const { return m_Gravity.z;	}
	const Vector3D&				GetGravity		() const { return m_Gravity;	}

	void						SetCFM			( float val );
	void						SetERP			( float val );

	void						Step			();
	void						SetGravityX		( float val );  
	void						SetGravityY		( float val ); 
	void						SetGravityZ		( float val );  
	void						SetGravity		( const Vector3D& vec );
    void                        SetFixedStep    ( float step ) { m_FixedStepSize = step; m_bFixedStep = true; }
    void                        SetStepMode     ( StepMode mode ) { m_StepMode = mode; }

	void						SetEarthGravity	();

	bool						IsValid			() const { return m_PID != NULL; }

	OdeID					    GetWorldID		() { return m_PID; }
	float						GetWorldScale	() const { return m_WorldScale; }
	void						SetWorldScale	( float val );
    void                        SetTimeScale	( float scale );
    float						GetTimeScale	() const { return m_TimeScale; }

    
    static PhysicsSystem&       inst(); 
    
	DECLARE_CLASS(PhysicsSystem);

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
}; // class PhysicsSystem

class dMass;
/*****************************************************************************/
/*	Class:	PhysicsObject
/*	Desc:	Physically simulated PhysicsObject object - e.g car, human etc.
/*****************************************************************************/
class PhysicsObject : public SNode
{
public:
								PhysicsObject		();
	virtual void				Expose			( PropertyMap& pm );		
	void						Reset			();
	void						ShowJoints		( bool bShow = true );
	void						ShowBodies		( bool bShow = true );
	void						ShowGeoms		( bool bShow = true );
	bool						IsShowJoints	() const { return m_bShowJoints; }
	bool						IsShowBodies	() const { return m_bShowBodies; }
	bool						IsShowGeoms		() const { return m_bShowGeoms; }

	void						Init			();


	DECLARE_SCLASS(PhysicsObject,SNode,ARTJ);

private:
	bool						m_bShowJoints;
	bool						m_bShowBodies;
	bool						m_bShowGeoms;

}; // class PhysicsObject

/*****************************************************************************/
/*	Class:	Body
/*	Desc:	Represents single rigid body, for which physics simulation is applied
/*****************************************************************************/
class Body : public Controller
{
	OdeID						m_PID;
	Vector3D					m_Velocity;		//  velocity vector
	Vector3D					m_AngVelocity;	//  angular velocity vector

	float						m_Mass;			//  mass
	float						m_Density;		//  body density == mass/volume


	Matrix3D					m_Inertia;		//  inertia tensor (mass distribution)
	
    std::string                 m_BoneName;
    TransformNode*				m_pBone;

	Vector3D					m_ScaleVector;	//  body scaling vector
	Matrix4D					m_MassCenterTM;	//  transform of mass center in the space of node we are attached to
												//  (this remains constant all the time)
	
public:
								Body		    ();
	virtual						~Body		    ();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );
	virtual void				Expose			( PropertyMap& pm );	
	virtual void				Reset			();

	float						GetMass			() const { return m_Mass; }
	void						SetMass			( float val ); 

    const char*                 GetBoneName     () const { return m_BoneName.c_str(); }
    void                        SetBoneName     ( const char* name ) { m_BoneName = name; }

    TransformNode*              GetBone         () const { return m_pBone; }
    void                        SetBone         ( TransformNode* pNode ) { m_pBone = pNode; }

	float						GetDensity		() const { return m_Density; }
	void						SetDensity		( float val ); 

    void                        SetVelocity     ( const Vector3D& v );
    void                        SetAngVelocity  ( const Vector3D& v );

	void						SetMass			( const dMass& mass );
	OdeID						GetBodyID		() const { return m_PID; }

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
	OdeID					    m_PID;
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
	OdeID					GetJointID		() const { return m_PID; }
	Vector3D					GetDirection1	( const Vector3D& pos ) const;
	Vector3D					GetDirection2	( const Vector3D& pos ) const;

	DECLARE_SCLASS(Joint,SNode,PHYJ);

protected:
	JointCluster*				FindParentJointCluster();
	virtual OdeID			CreateJoint		( OdeID groupID ) { return NULL; }
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
	OdeID				        m_PID;
	int							m_MaxSize;

public:
								JointCluster	();
								~JointCluster	();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );
	virtual void				Expose			( PropertyMap& pm );	
	bool						IsValid			() const { return m_PID != NULL; }
	OdeID				        GetClusterID	() const { return m_PID; }
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
	virtual OdeID			    CreateJoint		( OdeID groupID );
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
	virtual OdeID			CreateJoint		( OdeID groupID );
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
	virtual OdeID			CreateJoint		( OdeID groupID );
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
	virtual OdeID			    CreateJoint		( OdeID groupID );
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
	virtual OdeID			CreateJoint		( OdeID groupID );
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
	virtual OdeID			CreateJoint		( OdeID groupID );

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
	virtual OdeID			CreateJoint		( OdeID groupID );

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
	virtual OdeID			CreateJoint		( OdeID groupID );
	virtual void				SetParams		();

}; // class AMotorJoint


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
	virtual OdeID			    CreateJoint		( OdeID groupID );
	virtual void				SetParams		();

}; // class SkeletalJoint

class CollideSpace;
/*****************************************************************************/
/*	Class:	CollideGeom
/*	Desc:	Base class for collision primitives
/*			Primitive can be attached to the rigid body, or straight to the 
/*				geometry in case it is not subject to the dynamics solver
/*****************************************************************************/
class CollideGeom : public SNode
{
	OdeID						m_PID;              //  object geom id
    OdeID                       m_GeomTMID;  
    bool                        m_bHasGeomTM;
    Matrix4D                    m_GeomTM;

    Body*                       m_pBody;
    TransformNode*              m_pTransform;

public:
								CollideGeom		();
	virtual						~CollideGeom	();
	virtual void				Expose			( PropertyMap& pm );	
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );

    void                        AttachToBody    ( Body* pBody ) { m_pBody = pBody; }
    
    void                        SetLocalTM      ( const Matrix4D& tm );
    Matrix4D                    GetWorldTM      () const;

	bool						IsValid			() const { return m_PID != NULL; }
	OdeID						GetPrimitiveID	() const { return m_PID; }
	virtual void				Reset			();

	DECLARE_SCLASS(CollideGeom,SNode,CLPR);

protected:
	virtual OdeID				CreateGeom		( OdeID spaceID ) { return NULL; }
	virtual	void				FitGeom			( const Primitive& prim ){}
	virtual bool				IsPlaceable		() const { return true; }
	Matrix4D					GetTransform	();

	CollideSpace*				FindParentSpace	();
    Body*						GetAttachedBody	() const { return m_pBody; }
    void						SetGeomTM		( OdeID geomID, const Matrix4D& tm );

}; // class CollideGeom

/*****************************************************************************/
/*	Class:	CollideBox
/*	Desc:	Box collision primitive
/*****************************************************************************/
class CollideBox : public CollideGeom
{
	Vector3D					m_Extents;

public:
								CollideBox	();
	virtual void				Render		();
	virtual void				Serialize	( OutStream& os ) const;
	virtual void				Unserialize	( InStream& is  );
    void                        SetExtents  ( const Vector3D& v ) { m_Extents = v; }

	virtual void				Expose		( PropertyMap& pm );	

	DECLARE_SCLASS(CollideBox,CollideGeom,COLB);

protected:
	virtual OdeID				CreateGeom  ( OdeID spaceID );
	virtual	void				FitGeom		( const Primitive& prim );
}; // class CollideBox

/*****************************************************************************/
/*	Class:	CollideSphere
/*	Desc:	Sphere collision primitive
/*****************************************************************************/
class CollideSphere : public CollideGeom
{
	float						m_Radius;

public:
								CollideSphere	();
	virtual void				Render		();
	virtual void				Serialize	( OutStream& os ) const;
    virtual void				Unserialize	( InStream& is  );

	virtual void				Expose		( PropertyMap& pm );	

    void                        SetRadius   ( float rad ) { m_Radius = rad; }

	DECLARE_SCLASS(CollideSphere,CollideGeom,COLS);

protected:
	virtual OdeID				CreateGeom	( OdeID spaceID );
	virtual	void				FitGeom		( const Primitive& prim );
}; // class CollideSphere

/*****************************************************************************/
/*	Class:	CollideCylinder
/*	Desc:	Cylinder collision primitive
/*****************************************************************************/
class CollideCylinder : public CollideGeom
{
	float						m_Height;
	float						m_Radius;

public:
								CollideCylinder	();
	virtual void				Render		();
	virtual void				Serialize	( OutStream& os ) const;
	virtual void				Unserialize	( InStream& is  );

	virtual void				Expose		( PropertyMap& pm );	

	DECLARE_SCLASS(CollideCylinder,CollideGeom,COLC);

protected:
	virtual OdeID				CreateGeom	( OdeID spaceID );
	virtual	void				FitGeom		( const Primitive& prim );
}; // class CollideCylinder

/*****************************************************************************/
/*	Class:	CollideCapsule
/*	Desc:	Capped cylinder collision primitive
/*****************************************************************************/
class CollideCapsule : public CollideGeom
{
	float						m_Height;
	float						m_Radius;

public:
								CollideCapsule			();
	virtual void				Render				();
	virtual void				Serialize			( OutStream& os ) const;
	virtual void				Unserialize			( InStream& is  );
    void                        SetHeight           ( float height ) { m_Height = height; }
    void                        SetRadius           ( float radius ) { m_Radius = radius; }

	virtual void				Expose				( PropertyMap& pm );	

	DECLARE_SCLASS(CollideCapsule,CollideGeom,CLCC);

protected:
	virtual OdeID				CreateGeom	( OdeID spaceID );
	virtual	void				FitGeom		( const Primitive& prim );
}; // class CollideCapsule

/*****************************************************************************/
/*	Class:	CollidePlane
/*	Desc:	Plane collision primitive
/*****************************************************************************/
class CollidePlane : public CollideGeom
{
	Plane						m_Plane;

public:
								CollidePlane	();
	virtual void				Render		();
	virtual void				Serialize	( OutStream& os ) const;
	virtual void				Unserialize	( InStream& is  );
	virtual void				Expose		( PropertyMap& pm );	
    void                        SetPlane    ( const Plane& p ) { m_Plane = p; }

	DECLARE_SCLASS(CollidePlane,CollideGeom,CLPL);

protected:
	virtual OdeID				CreateGeom	( OdeID spaceID );
	virtual	void				FitGeom		( const Primitive& prim );
	virtual bool				IsPlaceable	() const { return false; }

}; // class CollidePlane

/*****************************************************************************/
/*	Class:	CollideRay
/*	Desc:	Ray collision primitive
/*****************************************************************************/
class CollideRay : public CollideGeom
{
	float						m_Length;
public:
								CollideRay		();
	virtual void				Render		();
	virtual void				Serialize	( OutStream& os ) const;
	virtual void				Unserialize	( InStream& is  );
    void                        SetLength   ( float len ) { m_Length = len; }

	virtual void				Expose		( PropertyMap& pm );	

	DECLARE_SCLASS(CollideRay,CollideGeom,CLRA);

protected:
	virtual OdeID				CreateGeom	( OdeID spaceID );
}; // class CollideRay

/*****************************************************************************/
/*	Class:	CollideTriMesh
/*	Desc:	TriMesh collision primitive
/*****************************************************************************/
class CollideTriMesh : public CollideGeom
{
	OdeID				        m_TriDataID;
	Geometry*					m_pMeshNode;
	Matrix4D					m_MeshTM;

	Primitive					m_CollisionMesh;
	std::vector<int>			m_Indices;

public:

								CollideTriMesh	();
	virtual 					~CollideTriMesh	();
	virtual void				Render		();
	virtual void				Serialize	( OutStream& os ) const;
	virtual void				Unserialize	( InStream& is  );
	virtual void				Expose		( PropertyMap& pm );	
	virtual bool				IsTriDataValid() const { return m_TriDataID != NULL; }
	virtual void				Reset			();


	DECLARE_SCLASS(CollideTriMesh,CollideGeom,CLTR);

protected:
	virtual OdeID				CreateGeom		    ( OdeID spaceID );
	Geometry*					GetMeshNode			();
	void						InitMeshStructure	( Primitive& pri, 
														const Matrix4D& tm = Matrix4D::identity );
}; // class CollideTriMesh

/*****************************************************************************/
/*	Class:	CollideSpace
/*	Desc:	Base class for groups of collision objects
/*****************************************************************************/
class CollideSpace : public SNode
{
	OdeID				m_PID;
	OdeID				m_ContactGroupID;
	void*			    m_Contacts;
	int					m_MaxContacts; 
	bool				m_bDrawContacts;
	int					m_NContacts;
	int					m_NCollidedBodies;
	int					m_MaxPerBodyContacts;

	//  contact reaction parameters
	float				m_Mu; 
	float				m_Mu2; 
	float				m_Bounce; 
	float				m_BounceVel; 
	float				m_SoftERP; 
	float				m_SoftCFM; 
	float				m_Motion1; 
	float				m_Motion2;
	float				m_Slip1;
	float				m_Slip2; 

	bool				m_bFDir1; 
	bool				m_bApprox1; 
	bool				m_bApprox2; 

public:
								CollideSpace	();
	virtual						~CollideSpace	();
	virtual void				Expose			( PropertyMap& pm );
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );
	virtual void				Render			();
	bool						IsValid			() const { return m_PID != NULL; }
	OdeID					    GetSpaceID		() const { return m_PID; }
	OdeID				        GetContactGroupID() const { return m_ContactGroupID; }

    //  Coloumb friction coefficient [0..inf], 0 - frictionless
    void                        SetFriction     ( float val ) { m_Mu = val; } 
    //  surface bounciness coeff [0..1], 0 - surface is not bouncy
    void                        SetBounce       ( float val ) { m_Bounce = val; } 
    void                        SetSoftness     ( float val ) { m_SoftCFM = val; }

	void						RenameGeoms		();
    void						ProcessCollision( void *data, OdeID o1, OdeID o2 );

    static CollideSpace*	    s_pCurSpace;

	DECLARE_SCLASS(CollideSpace,SNode,CLSP);

protected:
	CollideSpace*				FindParentSpace	();
	virtual OdeID			    CreateSpace		( OdeID parentSpaceID ) { return NULL; }

}; // class CollideSpace

/*****************************************************************************/
/*	Class:	CollideSimpleSpace
/*	Desc:	Simple collision space
/*****************************************************************************/
class CollideSimpleSpace : public CollideSpace
{
public:
								CollideSimpleSpace	() {}
	virtual						~CollideSimpleSpace	();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );

	virtual void				Expose			( PropertyMap& pm );	

	DECLARE_SCLASS(CollideSimpleSpace,CollideSpace,CLSS);

protected:
	virtual OdeID			CreateSpace		( OdeID parentSpaceID );
}; // class CollideSimpleSpace

/*****************************************************************************/
/*	Class:	CollideHashSpace
/*	Desc:	Hash collision space
/*****************************************************************************/
class CollideHashSpace : public CollideSpace
{
public:
								CollideHashSpace	() {}
	virtual						~CollideHashSpace	();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );

	virtual void				Expose			( PropertyMap& pm );	

	DECLARE_SCLASS(CollideHashSpace,CollideSpace,CLSH);

protected:
	virtual OdeID			CreateSpace		( OdeID parentSpaceID );
}; // class CollideHashSpace

/*****************************************************************************/
/*	Class:	CollideQuadSpace
/*	Desc:	Quad collision space
/*****************************************************************************/
class CollideQuadSpace : public CollideSpace
{
	Vector3D					m_Center;		//  quadtree center
	Vector3D					m_Extents;		//  quadtree extents
	int							m_Depth;		//  quadtree depth

public:
								CollideQuadSpace	();
	virtual						~CollideQuadSpace	();
	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is  );

	virtual void				Expose			( PropertyMap& pm );	

	DECLARE_SCLASS(CollideQuadSpace,CollideSpace,CLSQ);

protected:
	virtual OdeID			CreateSpace		( OdeID parentSpaceID );
}; // class CollideQuadSpace

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

ENUM(StepMode,"StepMode", 
     en_val(smNormal,	"Normal"	) << 
     en_val(smStepFast,	"StepFast"	) <<
     en_val(smQuickStep, "QuickStep"	) );

ENUM( AMotorMode,"AMotorMode",	en_val( ammUnknown, "Unknown"	) << 
     en_val( ammUnknown, "User"		) << 
     en_val( ammUnknown, "Euler"		) );


#endif // __SGPHYSICS_H__
