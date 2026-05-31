#include "stdheader.h"
#include "CollidingCamera.h"
#include "vCamera.h"
#include "TankController.h"
#include "..\CurrentMapOptions.h"

Surface::CollidingCamera g_CollidingCamera;
extern int FogMode;

namespace Surface {
struct CollisionArgs {
	cVec3 eRadius;
	// in R3 space:
	cVec3 R3Velocity;
	cVec3 R3Position;
	// in eSpace:
	cVec3 velocity;
	cVec3 normalizedVelocity;
	cVec3 basePoint;

	bool foundCollision;
	double nearestDistance;
	cVec3 intersectionPoint;
};

CollisionArgs Args;

// Assumes: p1, p2 and p3 are given in ellipsoid space:
void checkTriangle(CollisionArgs *colPackage, const cVec3 &p1, const cVec3 &p2, const cVec3 &p3) {
	// Make the plane containing this triangle:
	cPlane trianglePlane(p1, p2, p3);

	// Is triangle front - facing to the velocity vector?
	// We only check front - facing triangles
	if(trianglePlane.IsFrontFacingTo(colPackage->normalizedVelocity)) {
		// Get interval of plane intersection:
		double t0, t1;
		bool embeddedInPlane = false;

		// Calculate the signed distance from sphere
		// position to triangle plane
		double signedDistToTrianglePlane = trianglePlane.SignedDistanceToPoint(colPackage->basePoint);

		// Cache this as we're going to use it a few times below:
		float normalDotVelocity = cVec3::Dot(trianglePlane.GetNormal(), colPackage->velocity);

		// if sphere is travelling parallel to the plane:
		if(normalDotVelocity == 0.0f) {
			if(cMath::Abs(signedDistToTrianglePlane) >= 1.0f) {
				// Sphere is not embedded in plane.
				// No collision possible.
				return;
			} else {
				// Sphere is embedded in plane.
				// It intersects in the whole range [0..1]
				embeddedInPlane = true;
				t0 = 0.0;
				t1 = 1.0;
			}
		} else {
			// dot(N, D) is not 0. Calculate intersection interval:
			t0 = (-1.0 - signedDistToTrianglePlane) / normalDotVelocity;
			t1 = (1.0 - signedDistToTrianglePlane) / normalDotVelocity;

			// Swap so t0 < t1
			if(t0 > t1) {
				cMath::Swap(t0, t1);
			}

			// Check that at least one result is within range:
			if(t0 > 1.0f || t1 < 0.0f) {
				// Both t values are outside values [0, 1]
				// No collision possible:
				return;
			}

			// Clamp to [0, 1]
			t0 = cMath::Clamp(t0, 0.0, 1.0);
			t1 = cMath::Clamp(t1, 0.0, 1.0);
		}

		// At this point we have two time values t0 and t1
		// between which the swept sphere intersects with the
		// triangle plane. If any collision is to occur it must
		// happen within this interval.
		cVec3 collisionPoint;
		bool foundCollision = false;
		float t = 1.0;

		// First we check for the easy case - collision inside
		// the triangle. If this happens it must be at time t0
		// as this is when the sphere rests on the front side
		// of the triangle plane. Note, this can only happen if
		// the sphere is not embedded in the triangle plane.
		if(!embeddedInPlane) {
			cVec3 planeIntersectionPoint = (colPackage->basePoint - trianglePlane.GetNormal()) +
				t0 * colPackage->velocity;
			if(true) {//planeIntersectionPoint.IsInsideTri(p1, p2, p3)) {
//			cVec3 T[] = {p1, p2, p3};
//			if(cVec3::IsInsideConvexPolygon(planeIntersectionPoint, T, 3)) {
				foundCollision = true;
				t = t0;
				collisionPoint = planeIntersectionPoint;
			}
		}

		// if we haven't found a collision already we'll have to
		// sweep sphere against points and edges of the triangle.
		// Note: A collision inside the triangle (the check above)
		// will always happen before a vertex or edge collision!
		// This is why we can skip the swept test if the above
		// gives a collision!
		if(foundCollision == false) {
			// some commonly used terms:
			cVec3 velocity = colPackage->velocity;
			cVec3 base = colPackage->basePoint;
			float velocitySquaredLength = velocity.LengthSq();
			float a, b, c; // Params for equation
			float newT;

			// For each vertex or edge a quadratic equation have to be solved.
			// We parameterize this equation as
			// a * t^2 + b * t + c = 0 and below we calculate the
			// the parameters a, b and c for each test.

			// Check against points:
			a = velocitySquaredLength;

			// P1
			b = 2.0 * (cVec3::Dot(velocity, base - p1));
			c = (p1 - base).LengthSq() - 1.0;
			if(cSolver::GetLowestPositiveRoot2(a, b, c, t, newT)) {
				t = newT;
				foundCollision = true;
				collisionPoint = p1;
			}

			// P2:
			b = 2.0 * (cVec3::Dot(velocity, base - p2));
			c = (p2 - base).LengthSq() - 1.0;
			if(cSolver::GetLowestPositiveRoot2(a, b, c, t, newT)) {
				t = newT;
				foundCollision = true;
				collisionPoint = p2;
			}

			// P3:
			b = 2.0 * (cVec3::Dot(velocity, base - p3));
			c = (p3 - base).LengthSq() - 1.0;
			if(cSolver::GetLowestPositiveRoot2(a, b, c, t, newT)) {
				t = newT;
				foundCollision = true;
				collisionPoint = p3;
			}

			// Check against edges:

			// p1 -> p2:
			cVec3 edge = p2 - p1;
			cVec3 baseToVertex = p1 - base;
			float edgeSquaredLength = edge.LengthSq();
			float edgeDotVelocity = cVec3::Dot(edge, velocity);
			float edgeDotBaseToVertex = cVec3::Dot(edge, baseToVertex);

			// Calculate parameters for equation:
			a = edgeSquaredLength * -velocitySquaredLength +
				edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2 * cVec3::Dot(velocity, baseToVertex)) -
				2 * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1 - baseToVertex.LengthSq()) +
				edgeDotBaseToVertex * edgeDotBaseToVertex;
			
			// Does the swept sphere collide against infinite edge?
			if(cSolver::GetLowestPositiveRoot2(a, b, c, t, newT)) {
				// Check if intersection is within line segment:
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) / edgeSquaredLength;
				if(f >= 0.0 && f <= 1.0) {
					// intersection took plane within segment.
					t = newT;
					foundCollision = true;
					collisionPoint = p1 + f * edge;
				}
			}

			// p2 -> p3
			edge = p3 - p2;
			baseToVertex = p2 - base;
			edgeSquaredLength = edge.LengthSq();
			edgeDotVelocity = cVec3::Dot(edge, velocity);
			edgeDotBaseToVertex = cVec3::Dot(edge, baseToVertex);

			a = edgeSquaredLength * -velocitySquaredLength +
				edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2 * cVec3::Dot(velocity, baseToVertex)) -
				2.0 * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1 - baseToVertex.LengthSq()) +
				edgeDotBaseToVertex * edgeDotBaseToVertex;

			if(cSolver::GetLowestPositiveRoot2(a, b, c, t, newT)) {
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex)/
					edgeSquaredLength;
				if(f >= 0.0 && f <= 1.0) {
					t = newT;
					foundCollision = true;
					collisionPoint = p2 + f * edge;
				}
			}

			// p3 -> p1:
			edge = p1 - p3;
			baseToVertex = p3 - base;
			edgeSquaredLength = edge.LengthSq();
			edgeDotVelocity = cVec3::Dot(edge, velocity);
			edgeDotBaseToVertex = cVec3::Dot(edge, baseToVertex);

			a = edgeSquaredLength * -velocitySquaredLength +
				edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2 * cVec3::Dot(velocity, baseToVertex)) -
				2.0 * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1 - baseToVertex.LengthSq()) +
				edgeDotBaseToVertex * edgeDotBaseToVertex;

			if(cSolver::GetLowestPositiveRoot2(a, b, c, t, newT)) {
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) /
					edgeSquaredLength;
				if(f >= 0.0 && f <= 1.0) {
					t = newT;
					foundCollision = true;
					collisionPoint = p3 + f * edge;
				}
			}
		}

		// Set result:
		if(foundCollision == true) {
			// distance to collision: 't' is time of collision
			float distToCollision = t * colPackage->velocity.Length();

			// Does this trianlge qualify for the closest hit?
			// it does if it's the first hit is the closest
			if(colPackage->foundCollision == false ||
				distToCollision < colPackage->nearestDistance) {
					// Collision information necessary for sliding:
					colPackage->nearestDistance = distToCollision;
					colPackage->intersectionPoint = collisionPoint;
					colPackage->foundCollision = true;
				}
		}
	} // if not backface
}

SurfMesh World;
cVec3 g_Pos;

void WorldCheckCollision(CollisionArgs *pArgs) {
	SurfVertex u[3];
	cVec3 t0, t1, t2;
	for(int nTri = 0; nTri < World.NTris(); nTri++) {
		World.GetTri(nTri, u);
		t0 = u[0].ToVector3() / pArgs->eRadius;
		t1 = u[1].ToVector3() / pArgs->eRadius;
		t2 = u[2].ToVector3() / pArgs->eRadius;
		checkTriangle(pArgs, t0, t1, t2);
	}
}

int collisionRecursionDepth = 0;

// Set this to match application scale..
const float unitsPerMeter = 100.0f;

cVec3 CollideWithWorld(const cVec3 &pos, const cVec3 &vel) {
	// All hard - coded distances in this function is
	// scaled to fit the setting above...
	float unitScale = unitsPerMeter / 100.0f;
	float veryCloseDistance = 0.005f * unitScale;

	// do we need to worry?
	if(collisionRecursionDepth > 5) return pos;

	// Ok, we need to worry:
	Args.velocity = vel;
	Args.normalizedVelocity = cVec3::Normalize(vel);
	Args.basePoint = pos;
	Args.foundCollision = false;

	// Check for collision
	WorldCheckCollision(&Args);

	// If no collision we just move along the velocity
	if(Args.foundCollision == false) {
		return pos + vel;
	}

	// Collision occured!

	// The original destination point
	cVec3 destinationPoint = pos + vel;
	cVec3 newBasePoint = pos;

	// only update if we are not already very close
	// and if so we only move very close to intersection not
	// to the exact spot.
	if(Args.nearestDistance >= veryCloseDistance) {
		cVec3 V = vel;
		V = cVec3::Normalize(V) * (Args.nearestDistance - veryCloseDistance);
		newBasePoint = Args.basePoint + V;

		// Adjust polygon intersection point (so sliding
		// plane will be unaffected by the fact that we
		// move slightly less than collision tells us)
		V.Normalize();
		Args.intersectionPoint -= veryCloseDistance * V;
	}
	// Determine the sliding plane
	cVec3 slidePlaneOrigin = Args.intersectionPoint;
	cVec3 slidePlaneNormal = newBasePoint - Args.intersectionPoint;
	slidePlaneNormal.Normalize();

	cPlane slidingPlane(slidePlaneOrigin, slidePlaneNormal);

	cVec3 newDestinationPoint = destinationPoint -
		slidingPlane.SignedDistanceToPoint(destinationPoint) * slidePlaneNormal;

	// Generate the slide vector, which will become our new
	// velocity vector for the next iteration
	cVec3 newVelocityVector = newDestinationPoint - Args.intersectionPoint;

	// Recurse:

	// dont recurse if the new velocity is very small
	if(newVelocityVector.Length() < veryCloseDistance) {
		return newBasePoint;
	}

	collisionRecursionDepth++;
	return CollideWithWorld(newBasePoint, newVelocityVector);
}

void CollideAndSlide(const cVec3 &vel, const cVec3 &gravity) {
	Args.R3Position = g_Pos;
	Args.R3Velocity = vel;
	Args.eRadius.Set(100, 100, 100);

	// calculate position and velocity in eSpace
	cVec3 eSpacePosition = Args.R3Position / Args.eRadius;
	cVec3 eSpaceVelocity = Args.R3Velocity / Args.eRadius;

	// Iterate until we have our final position.
	collisionRecursionDepth = 0;

	cVec3 finalPosition = CollideWithWorld(eSpacePosition, eSpaceVelocity);

	// Add gravity pull:

	// Set the new R3 position (convert back from eSpace to R3)
	Args.R3Position = finalPosition * Args.eRadius;
	Args.R3Velocity = gravity;
	
	eSpaceVelocity = gravity / Args.eRadius;
	collisionRecursionDepth = 0;

	finalPosition = CollideWithWorld(finalPosition, eSpaceVelocity);

	// Convert final result back to R3:
	finalPosition = finalPosition * Args.eRadius;

	// Move the entity
	g_Pos = finalPosition;
}

//-----------------------------------------------------------------------------
// CollidingCamera::Simulate
//-----------------------------------------------------------------------------
void CollidingCamera::Simulate(const float tStep, const int nSteps) {
	Vector3D d = ICam->GetDir();
	cVec3 Dir(d.x, d.y, 0.0f);
	
	float T = tStep * nSteps;

	float FullStep = EngSettings.CollidingCamera.MaxVelocity * T;
	Nodes N;
	N.Gather(m_Args.Pos, EngSettings.CollidingCamera.Radius, FullStep);

	for(int s = 0; s < nSteps; s++) {
		cVec3 CollisionForce = N.CalcCollisionForce(m_Args.Pos,
			EngSettings.CollidingCamera.Radius, EngSettings.CollidingCamera.Bump);
		cVec3 GravityForce = - cVec3::AxisZ *
			EngSettings.CollidingCamera.Mass * EngSettings.CollidingCamera.Gravity;
		cVec3 ControlDir = cVec3::Truncate(m_Args.ControlMoveForceSign * Dir +
			m_Args.ControlJumpForceSign * cVec3::AxisZ +
			m_Args.ControlStrafeForceSign * cVec3(- Dir.y, Dir.x, 0), 1.0f);
		cVec3 ControlForce = ControlDir * cVec3(EngSettings.CollidingCamera.MoveForce,
			EngSettings.CollidingCamera.MoveForce, EngSettings.CollidingCamera.JumpForce);

		cVec3 F = CollisionForce + GravityForce + ControlForce;
		cVec3 A = F / EngSettings.CollidingCamera.Mass;

		// Floor:
		if(m_Args.Pos.z > MOptions.FogArgs.final_FogFarPlane / 2 && A.z > 0) {
			A.z = 0;
		}

		m_Args.Vel += A * tStep;

		m_Args.Vel -= m_Args.Vel * EngSettings.CollidingCamera.AirFriction;

		if(cVec3::Length(CollisionForce) > 1e-6) {
			m_Args.Vel -= m_Args.Vel * cVec3::Normalize(CollisionForce) * EngSettings.CollidingCamera.ContactFriction;
		}
		
		m_Args.Vel = cVec3::Truncate(m_Args.Vel, EngSettings.CollidingCamera.MaxVelocity);
		m_Args.Pos += m_Args.Vel * tStep;
//		g_Pos = m_Args.Pos; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//		m_Args.Vel = 20 * ControlDir; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//		CollideAndSlide(m_Args.Vel * tStep, - cVec3::zAxis); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//		m_Args.Pos = g_Pos; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	}
	m_Args.ControlMoveForceSign = 0.0f;
	m_Args.ControlJumpForceSign = 0.0f;
	m_Args.ControlStrafeForceSign = 0.0f;
}

//--------------------------------------------------------------------------------------------------------------
// CollidingCamera::Nodes::Collides
//--------------------------------------------------------------------------------------------------------------
bool CollidingCamera::Nodes::Collides(const cVec3 &u, const cVec3 &Center, const float Radius) {
	cVec3 d = u - Center;
	return d.Length() < Radius;
}

//--------------------------------------------------------------------------------------------------------------
// CollidingCamera::Nodes::Gather
//--------------------------------------------------------------------------------------------------------------
bool CollidingCamera::Nodes::Gather(const cVec3 &StepCenter, const float Radius, const float FullStep) {
	assert(Radius > 0);
	Free();
	Node n;
	float R = Radius + FullStep;

	SurfRef Ref;
//	g_UniversalMap.QUADS.GetRefAroundCenter(Ref, SurfVertex((short)StepCenter.x, (short)StepCenter.y, (short)StepCenter.z), R);
	g_UniversalMap.GetRefSphere(Ref, SurfVertex((short)StepCenter.x, (short)StepCenter.y, (short)StepCenter.z), R, SR_TRIS);

//	Ref.ToSurfMesh(World); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//	return false; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//	cSphere S(StepCenter, R); // <<<<<<<<<<<<<<<<<<<<<<<<

//	if(Ref.idQuads.Count()) {
//		Ref.Complete(SR_TRIS, SR_QUADS);
		if(Ref.idTris.Count()) {
			SurfVertex r0, r1, r2;
			cVec3 t0, t1, t2;
			cPlane P;
			for(int iTri = 0; iTri < Ref.idTris.Count(); iTri++) {
				g_UniversalMap.GetTri(Ref.idTris[iTri], r0, r1, r2);
				t0.Set(r0.x, r0.y, r0.z);
				t1.Set(r1.x, r1.y, r1.z);
				t2.Set(r2.x, r2.y, r2.z);
//				if(S.TriIntersection(t0, t1, t2, &n.p, &n.N)) { // <<<<<<<<<<<<<<<<<<<<<<<<<<<<
//					Add(n); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//				} // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
				n.p = (t0 + t1 + t2) / 3.0f;
				if(Collides(n.p, StepCenter, R)) {
					P.SetFromPoints(t0, t1, t2);
					if(P.GetNormal().IsNormalized()) {
						n.N = P.GetNormal();
						Add(n);
					}
				}
			}
		}
//	}
	// Consider water:
	if(StepCenter.z < R) {
		n.p = cVec3(StepCenter.ToVec2(), 0);
		n.N = cVec3::AxisZ * EngSettings.CollidingCamera.Floatage;
		Add(n);
	}
	
	return Count() > 0;
}

//-------------------------------------------------------------------------------------------
// CollidingCamera::Nodes::CalcCollisionForce
//-------------------------------------------------------------------------------------------
cVec3 CollidingCamera::Nodes::CalcCollisionForce(const cVec3 &SubStepCenter,
														 const float Radius, const float k) {
	assert(Radius > 0);
	cVec3 Force(0);
	for(int i = 0; i < Count(); i++) {
		const Node &n = GetAt(i);
		cVec3 d = n.p - SubStepCenter;
		float l = d.Length();
		if(l < Radius) {
			float x = Radius - l;
			Force += n.N * k * x;
		}
	}
	return Force;
}

//-----------------------------------------------------------------------------
// Activate
//-----------------------------------------------------------------------------
void CollidingCamera::Activate() {
	if(m_fActive) return;
	if(g_TankController.IsActive()) return;

	Vector3D pos = ICam->GetPosition();
	Vector3D dir = ICam->GetDirection();
	dVector Dir(dir.x, dir.z, dir.y);
	Dir.Normalize();
	assert(cMath::Equals(Dir.Length(), 1.0));
	double r, Phi, Psi;
	Dir.Spherical(r, Phi, Psi);
	m_Args.MouseAzim = m_Args.Azim = Phi;
	m_Args.MouseElev = m_Args.Elev = Psi;
	m_Args.Pos.Set(pos.x, pos.y, pos.z);
	m_Args.Vel = cVec3::Zero;

	m_fActive = true;

	m_xCenter = RealLx / 2;
	m_yCenter = RealLy / 2;
	SetCursorPos(m_xCenter, m_yCenter);
	while(ShowCursor(FALSE) >= 0);

	m_fUIWasHidden = EngSettings.CollidingCamera.HideUI;
	g_SurfEditor.Update();
	if(m_fUIWasHidden) {
		g_SurfEditor.HideUI = true;
	}
	g_SurfEditor.FreeCamera = false;
	g_SurfEditor.Update();

	m_fCameraWasForced = EngSettings.CollidingCamera.ForceCameraParams;
	if(EngSettings.CollidingCamera.ForceCameraParams) {
		m_PrevFogArgsUseFromMap = MOptions.FogArgs.UseFromMap;
		extern float CameraMovingStage;
		m_pFinalFogNearPlane = MOptions.FogArgs.final_FogNearPlane;
		m_pFinalFogFarPlane = MOptions.FogArgs.final_FogFarPlane;
		m_pCameraMovingStage = CameraMovingStage;
		CameraMovingStage = EngSettings.CollidingCamera.CameraMovingStage;
		MOptions.FogArgs.final_FogFarPlane = EngSettings.CollidingCamera.final_FogFarPlane;
		MOptions.FogArgs.final_FogNearPlane = EngSettings.CollidingCamera.final_FogNearPlane;
		MOptions.FogArgs.UseFromMap = true;
		m_PrevSkyShift = MOptions.FogArgs.SkyShift;
		MOptions.FogArgs.SkyShift = EngSettings.CollidingCamera.SkyShift;
	}

	m_Args.Timer.Begin();
	m_Args.TimeAccum = 0.0;
	
	m_FogMode = FogMode;
	FogMode = 0;
}

//-----------------------------------------------------------------------------
// CollidingCamera::Free
//-----------------------------------------------------------------------------
void CollidingCamera::Free() {
	if(!m_fActive) return;
	m_fActive = false;
	while(ShowCursor(TRUE) <= 0);
	m_Args.Timer.End();
	if(m_fUIWasHidden) {
		g_SurfEditor.Update();
		g_SurfEditor.HideUI = false;
		g_SurfEditor.Update();
	}
	if(m_fCameraWasForced) {
		extern float CameraMovingStage;
		CameraMovingStage = m_pCameraMovingStage;
		MOptions.FogArgs.final_FogNearPlane = m_pFinalFogNearPlane;
		MOptions.FogArgs.final_FogFarPlane = m_pFinalFogFarPlane;
		MOptions.FogArgs.UseFromMap = m_PrevFogArgsUseFromMap;
		MOptions.FogArgs.SkyShift = m_PrevSkyShift;
	}

	FogMode = m_FogMode;
}

//--------------------------------------------------------------------------------------------------
// CalcOrientation
//--------------------------------------------------------------------------------------------------
void CollidingCamera::CalcOrientation(dVector &Right, dVector &Up, dVector &Dir) const {
	dVector c0 = dVector::Cartesian(1.0, m_Args.Azim, m_Args.Elev);
	dVector c1 = dVector::Cartesian(1.0, m_Args.Azim, m_Args.Elev - cMath::Rad(45));
	dVector r0(c0.x, c0.z, c0.y);
	dVector r1(c1.x, c1.z, c1.y);
	dVector Ref = r1 - r0;
	Ref.Normalize();

	Dir = r0;
	Dir.Normalize();

	Right = Ref.Cross(Dir);
	Right.Normalize();

	Up = Dir.Cross(Right);
	Up.Normalize();
}

//-----------------------------------------------------------------------------
// SetupCamera
//-----------------------------------------------------------------------------
void CollidingCamera::SetupCamera() {
	if(!m_fActive) return;
//	static cTimer T;

	m_Args.Timer.End();
	m_Args.TimeAccum += EngSettings.CollidingCamera.TimeScale * m_Args.Timer.msMeanTime;
	if(m_Args.TimeAccum > EngSettings.CollidingCamera.TimeScale * EngSettings.CollidingCamera.TimeOut) m_Args.TimeAccum = 0.0;
	int nSteps = int(m_Args.TimeAccum / EngSettings.CollidingCamera.TimeStep);

	if(nSteps > 0) {
		m_Args.Azim = m_Args.MouseAzim;
		m_Args.Elev = m_Args.MouseElev;

		if(GetKeyState(VK_UP) < 0 || (GetKeyState('E') < 0 && EngSettings.CollidingCamera.ESDF) ||
			(GetKeyState('W') < 0 && EngSettings.CollidingCamera.WASD)) Forward();
		if(GetKeyState(VK_DOWN) < 0 || (GetKeyState('D') < 0 && EngSettings.CollidingCamera.ESDF) ||
			(GetKeyState('S') < 0 && EngSettings.CollidingCamera.WASD)) Backward();
		if(GetKeyState(VK_RIGHT) < 0 || (GetKeyState('F') < 0 && EngSettings.CollidingCamera.ESDF) ||
			(GetKeyState('D') < 0 && EngSettings.CollidingCamera.WASD)) Right();
		if(GetKeyState(VK_LEFT) < 0 || (GetKeyState('S') < 0 && EngSettings.CollidingCamera.ESDF) ||
			(GetKeyState('A') < 0 && EngSettings.CollidingCamera.WASD)) Left();
		if(GetKeyState(VK_SPACE) < 0) Jump();

//		T.Begin(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		Simulate(EngSettings.CollidingCamera.TimeStep, nSteps);
//		if(T.End(255)) { // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//			cStr Stat; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//			Stat += "Mean Time (ms): "; //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//			Stat += T.m_msMeanTime; // <<<<<<<<<<<<<<<<<<<<<
//			Log.Message(Stat.ToChar()); // <<<<<<<<<<<<<<<<<<<<<<
//		} // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		m_Args.TimeAccum -= nSteps * EngSettings.CollidingCamera.TimeStep;

//		cStr Info; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//		Info += nSteps; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//		Log.Message(Info.asChar()); // <<<<<<<<<<<<<<<<<<<<<<<<
	}

	ICam->SetPosition(Vector3D(m_Args.Pos.x, m_Args.Pos.y, m_Args.Pos.z));
	dVector Right, Up, Dir;
	CalcOrientation(Right, Up, Dir);
	ICam->SetOrientation(Vector3D(Right.x, Right.y, Right.z),
		Vector3D(Up.x, Up.y, Up.z), Vector3D(Dir.x, Dir.y, Dir.z));

	m_Args.Timer.Begin();
}

//-----------------------------------------------------------------------------
// Toggle
//-----------------------------------------------------------------------------
void CollidingCamera::Toggle() {
	if(IsActive()) {
		Free();
	} else {
		Activate();
	}
}

//--------------------------------------------------------------------------------------
// CameraMessage : bool(HWND, UINT, WPARAM, LPARAM)
//--------------------------------------------------------------------------------------
bool CollidingCamera::CameraMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	if(!m_fActive) return false;
	if(GetForegroundWindow() != g_hWnd) return false;

	static POINTS pts;
	int dx, dy;
	switch(Msg) {
	case WM_SETCURSOR:
		while(ShowCursor(FALSE) >= 0);
		return true;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		return true;
	case WM_MOUSEMOVE:
		pts = MAKEPOINTS(lParam);
		dx = pts.x - m_xCenter;
		dy = pts.y - m_yCenter;
		if(dx || dy) {
			SetCursorPos(m_xCenter, m_yCenter);
//			if(abs(dx) < 300 && abs(dy) < 300) {
//				m_Args.Azim -= EngSettings.CollidingCamera.MouseSpeed * float(dx);
				m_Args.MouseAzim -= EngSettings.CollidingCamera.MouseSpeed * float(dx);
//				m_Args.Elev += EngSettings.CollidingCamera.MouseSpeed * float(dy);
				m_Args.MouseElev += EngSettings.CollidingCamera.MouseSpeed * float(dy);
//				m_Args.Elev = cMath::clamp(m_Args.Elev, 0, (float)cMath::cPi);
				m_Args.MouseElev = cMath::Clamp(m_Args.MouseElev, 0.0f, cMath::Pi);
				m_Args.MouseAzim = cMath::AngleNormalizeTwoPi(m_Args.MouseAzim);
//			}
		}
		return true;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		return true;

	case WM_KEYDOWN:
		if(VK_HOME == wParam || VK_ESCAPE == wParam) {
			Free();
			return true;
		}
		if(VK_SPACE == wParam) {
			Jump();
			return true;
		}
	case WM_KEYUP:
	case WM_CHAR:
		if(EngSettings.CollidingCamera.WASD && ('W' == wParam || 'A' == wParam || 'S' == wParam || 'D' == wParam)) return true;
		if(EngSettings.CollidingCamera.ESDF && ('E' == wParam || 'S' == wParam || 'D' == wParam || 'F' == wParam)) return true;
		if(VK_NEXT == wParam || VK_PRIOR == wParam) return false;
		return true;
    case WM_ACTIVATE:
		switch(wParam) {
			case WA_ACTIVE:
				SetCursorPos(m_xCenter, m_yCenter);
				ShowCursor(FALSE);
				break;
			default:
				break;
		}
		return true;
	default:
		return false;
	}
}

}; // Surface