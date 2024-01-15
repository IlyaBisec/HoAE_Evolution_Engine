/*****************************************************************************/
/*	File:	edNodeMaker.cpp
/*	Desc:	Stub file for registering editor scene node types
/*	Author:	Ruslan Shestopalyuk
/*	Date:	06-26-2003
/*****************************************************************************/
#include "stdafx.h"
#include "kTemplates.hpp"
#include "kHash.hpp"

#include "sg.h"
#include "sgSprite.h"
#include "uiControl.h"
#include "sgTerraEdit.h"
#include "uiControl.h"
#include "dbDebugger.h"
#include "sgPhysics.h"
#include "edGluer.h"
#include "edUtilities.h"

#include "dbCurrent.h"
#include "dbLodder.h"
#include "dbImpostor.h"
#include "dbAnimation.h"


IMPLEMENT_CLASS( TerrainEditor		);

//  misc

IMPLEMENT_CLASS( Gluer				);
IMPLEMENT_CLASS( Debugger		    );

IMPLEMENT_CLASS( Utilities          );

Group* CreatePhysicsTemplates()
{
	Group* pG = new Group();
	pG->SetName( "Physics" );
	//pG->AddChild<Body>						( "Body"			);

	Group* pJoints = pG->AddChild<Group>	( "Joints"			);
	pJoints->AddChild<JointCluster>			( "JointCluster"	);

	pJoints->AddChild<BallSocketJoint>		( "BallSocket"		);
	pJoints->AddChild<SliderJoint>			( "Slider"			);
	pJoints->AddChild<HingeJoint>			( "Hinge"			);
	pJoints->AddChild<Hinge2Joint>			( "Hinge2"			);
	pJoints->AddChild<UniversalJoint>		( "Universal"		);
	pJoints->AddChild<FixedJoint>			( "Fixed"			);
	pJoints->AddChild<ContactJoint>			( "Contact"			);
	pJoints->AddChild<AMotorJoint>			( "AngularMotor"	);
	pJoints->AddChild<SkeletalJoint>		( "SkeletalJoint"	);

	/*Group* pGeoms = pG->AddChild<Group>	( "Geoms" );
	pGeoms->AddChild<CollideBox>			( "Box"				);
	pGeoms->AddChild<CollideSphere>			( "Sphere"			);
	pGeoms->AddChild<CollideCylinder>		( "Cylinder"		);
	pGeoms->AddChild<CollideCapsule>		( "Capsule"			);
	pGeoms->AddChild<CollidePlane>			( "Plane"			);
	pGeoms->AddChild<CollideRay>			( "Ray"				);
	pGeoms->AddChild<CollideTriMesh>		( "TriMesh"			);

	Group* pSpaces = pG->AddChild<Group>    ( "Spaces" );
	pSpaces->AddChild<CollideSimpleSpace>	( "Simple"	);
	pSpaces->AddChild<CollideHashSpace>	    ( "Hash"	);
	pSpaces->AddChild<CollideQuadSpace>	    ( "Quad"	);*/

	Group* pMisc = pG->AddChild<Group>( "Misc" );
	pMisc->AddChild<PushForce>			    ( "PushForce" );

	return pG;
} // CreatePhysicsTemplates
bool GecOptimCollision;

