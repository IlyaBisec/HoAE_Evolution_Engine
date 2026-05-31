#include "comms.h"

// cMesh.ctor
cMesh::cMesh() {
}

//-----------------------------------------------------------------------------------------------------------------
// cMesh.ctor
//-----------------------------------------------------------------------------------------------------------------
cMesh::cMesh(const char *Name, const eSetCtor, Vertex *Verts, const int NVerts, int *Indices, const int NIndices) {
	m_Name = Name;
	
	if(Verts != NULL && NVerts > 0) {
		m_Verts.Set(Verts, NVerts);
	}
	if(Indices != NULL && NIndices > 0) {
		m_Indices.Set(Indices, NIndices);
	}
	
	FillEdges();

	for(int i = 0; i < NVerts; i++) {
		m_Bounds.AddPoint(Verts[i].Pos);
	}
} // cMesh.ctor

//------------------------------------------------------------------------------------------------------------------------------
// cMesh.ctor
//------------------------------------------------------------------------------------------------------------------------------
cMesh::cMesh(const char *Name, const eCopyCtor, const Vertex *Verts, const int NVerts, const int *Indices, const int NIndices) {
	m_Name = Name;

	if(Verts != NULL && NVerts > 0) {
		m_Verts.Copy(Verts, NVerts);
	}
	if(Indices != NULL && NIndices > 0) {
		m_Indices.Copy(Indices, NIndices);
	}
	
	FillEdges();

	for(int i = 0; i < NVerts; i++) {
		m_Bounds.AddPoint(Verts[i].Pos);
	}
} // cMesh.ctor

// cMesh.dtor
cMesh::~cMesh() {
}

//-----------------------------------------------------------------------------
// cMesh::Transform
//-----------------------------------------------------------------------------
void cMesh::Transform(const cMat4 &T) const {
	cMat4 N(cMat4::IdentityCtor), I;
	if(cMat4::Invert(T, &I)) {
		N = cMat4::Transpose(I);
	}
	
	m_TransformedVerts.Clear();
	Vertex u;
	for(int i = 0; i < m_Verts.Count(); i++) {
		u = m_Verts[i];
		u.Pos.TransformCoordinate(T);
		u.Normal.TransformNormal(N);
		u.Tangent.TransformNormal(N);
		u.BiTangent.TransformNormal(N);
		m_TransformedVerts.Add(u);
	}
} // cMesh::Transform

//-----------------------------------------------------------------------------
// cMesh::CalcNormals
//-----------------------------------------------------------------------------
void cMesh::CalcNormals() {
	for(cList<Vertex>::IteratorMutable It = m_Verts.GetIteratorMutable(); !It.End(); ++It) {
		It->Normal.SetZero();
	}
	for(int i = 0; i < m_Indices.Count(); i += 3) {
		const int *Tri = m_Indices.ToPtr() + i;
		Vertex &t0 = m_Verts[Tri[0]];
		Vertex &t1 = m_Verts[Tri[1]];
		Vertex &t2 = m_Verts[Tri[2]];
		const cVec3 Normal = cVec3::Cross(t1.Pos - t0.Pos, t2.Pos - t0.Pos);
		t0.Normal += Normal;
		t1.Normal += Normal;
		t2.Normal += Normal;
	}
	for(cList<Vertex>::IteratorMutable It = m_Verts.GetIteratorMutable(); !It.End(); ++It) {
		It->Normal.Normalize();
	}
} // cMesh::CalcNormals

//-----------------------------------------------------------------------------
// cMesh::CalcTangents
//-----------------------------------------------------------------------------
void cMesh::CalcTangents(Vertex &t0, Vertex &t1, Vertex &t2) {
	cVec3 e0(t1.Pos.x - t0.Pos.x, t1.TexCoord.x - t0.TexCoord.x, t1.TexCoord.y - t0.TexCoord.y);
	cVec3 e1(t2.Pos.x - t0.Pos.x, t2.TexCoord.x - t0.TexCoord.x, t2.TexCoord.y - t0.TexCoord.y);
	cVec3 c = cVec3::Cross(e0, e1);
	if(!cMath::IsZero(c.x)) {
		t0.Tangent.x = -c.y / c.x;
		t0.BiTangent.x = -c.z / c.x;
	} else {
		t0.Tangent.x = 0.0f;
		t0.BiTangent.x = 0.0f;
	}

	e0.x = t1.Pos.y - t0.Pos.y;
	e1.x = t2.Pos.y - t0.Pos.y;
	c = cVec3::Cross(e0, e1);
	if(!cMath::IsZero(c.x)) {
		t0.Tangent.y = - c.y / c.x;
		t0.BiTangent.y = - c.z / c.x;
	} else {
		t0.Tangent.y = 0.0f;
		t0.BiTangent.y = 0.0f;
	}

	e0.x = t1.Pos.z - t0.Pos.z;
	e1.x = t2.Pos.z - t0.Pos.z;
	c = cVec3::Cross(e0, e1);
	if(!cMath::IsZero(c.x)) {
		t0.Tangent.z = - c.y / c.x;
		t0.BiTangent.z = - c.z / c.x;
	} else {
		t0.Tangent.z = 0.0f;
		t0.BiTangent.z = 0.0f;
	}

	if(cVec3::Dot(cVec3::Cross(t0.Tangent, t0.BiTangent), t0.Normal) < 0.0f) {
		t0.Tangent *= -1.0f;
	}
} // cMesh::CalcTangents


//-----------------------------------------------------------------------------
// cMesh::CalcTangents
//-----------------------------------------------------------------------------
void cMesh::CalcTangents() {
	for(int i = 0; i < m_Indices.Count(); i += 3) {
		const int *Tri = m_Indices.ToPtr() + i;
		CalcTangents(m_Verts[Tri[0]], m_Verts[Tri[1]], m_Verts[Tri[2]]);
	}
} // cMesh::CalcTangents


//-----------------------------------------------------------------------------
// cMesh::FillEdges
//-----------------------------------------------------------------------------
void cMesh::FillEdges() {
	m_Edges.Free();
	m_EdgeIndices.Free();
	
	if(!m_Indices.Count()) {
		return;
	}

	cList<int> VertexEdges(cList<int>::SetCountCtor, m_Verts.Count(), -1);
	cList<int> EdgeChain(cList<int>::SetCountCtor, m_Indices.Count());
	m_EdgeIndices.SetCount(m_Indices.Count());
	
	// The first edge is a dummy:
	Edge e[3];
	e[0].Verts[0] = e[0].Verts[1] = e[0].Tris[0] = e[0].Tris[1] = 0;
	m_Edges.Add(e[0]);

	for(int i = 0; i < m_Indices.Count(); i += 3) {
		const int *Tri = m_Indices.ToPtr() + i;
		int i0 = Tri[0], i1 = Tri[1], i2 = Tri[2];
		// Setup edges each with smallest vertex number first:
		int s = cMath::SignBitSet(i1 - i0);
		e[0].Verts[0] = Tri[s];
		e[0].Verts[1] = Tri[s ^ 1];
		s = cMath::SignBitSet(i2 - i1) + 1;
		e[1].Verts[0] = Tri[s];
		e[1].Verts[1] = Tri[s ^ 3];
		s = cMath::SignBitSet(i2 - i0) << 1;
		e[2].Verts[0] = Tri[s];
		e[2].Verts[1] = Tri[s ^ 2];
		// Find edges:
		for(int j = 0; j < 3; j++) {
			int v0 = e[j].Verts[0], v1 = e[j].Verts[1];
			int nEdge = VertexEdges[v0];
			for(; nEdge >= 0; nEdge = EdgeChain[nEdge]) {
				if(m_Edges[nEdge].Verts[1] == v1) {
					break;
				}
			}
			if(nEdge < 0) { // The edge does not yet exist:
				e[j].Tris[0] = e[j].Tris[1] = -1;
				nEdge = m_Edges.Add(e[j]);
				EdgeChain[nEdge] = VertexEdges[v0];
				VertexEdges[v0] = nEdge;
			}
			// Update edge index and edge triangle references:
			if(Tri[j] == v0) {
				cAssert(m_Edges[nEdge].Tris[0] == -1 && "Edge may not be shared by more than two triangles!");
				m_Edges[nEdge].Tris[0] = i;
				m_EdgeIndices[i + j] = nEdge;
			} else {
				cAssert(m_Edges[nEdge].Tris[1] == -1 && "Edge may not be shared by more than two triangles!");
				m_Edges[nEdge].Tris[1] = i;
				m_EdgeIndices[i + j] = -nEdge;
			}
		}
	}
} // cMesh::FillEdges

//-----------------------------------------------------------------------------
// cMesh::CalcColor
//-----------------------------------------------------------------------------
void cMesh::CalcColor(const cColor &LightAmbient) {
	// Selecting vertex buffer (src or transformed):
	Vertex *Verts = NULL;
	int Count = 0;
	if((Count = m_TransformedVerts.Count()) > 0) {
		Verts = m_TransformedVerts.ToPtr();
	} else {
		Count = m_Verts.Count();
		Verts = m_Verts.ToPtr();
	}
	// Filling vertex color with light / material ambient:
	for(int nVert = 0; nVert < Count; nVert++, Verts++) {
		Verts->Color = (m_Material.Ambient * LightAmbient).ToRgba();
	}
} // cMesh::CalcColor

//-----------------------------------------------------------------------------------------------------
// cMesh::CalcColor
//-----------------------------------------------------------------------------------------------------
void cMesh::CalcColor(const cVec3 &LightPos, const cColor &LightDiffuse, const cColor &LightSpecular) {
	// Selecting vertex buffer (source or transformed):
	cList<Vertex> *Verts = m_TransformedVerts.Count() > 0 ? &m_TransformedVerts : &m_Verts;
	// Filling vertex color with light / material diffuse / specular (w/o ambient):
	for(int nVert = 0; nVert < Verts->Count(); nVert++) {
		Vertex &r = Verts->GetAt(nVert);
		const cVec3 ToLight = LightPos - r.Pos;
		cVec3 tl(cVec3::Dot(ToLight, r.Tangent), cVec3::Dot(ToLight, r.BiTangent), cVec3::Dot(ToLight, r.Normal));
		tl.Normalize();

		const cVec3 ToEye = cRender::GetInstance()->GetViewer()->GetPos() - r.Pos;
		cVec3 te(cVec3::Dot(ToEye, r.Tangent), cVec3::Dot(ToEye, r.BiTangent), cVec3::Dot(ToEye, r.Normal));
		te.Normalize();
		
		const float s = cMath::Pow(cMath::Clamp01(cVec3::Dot(cVec3::Reflect(-tl, cVec3::AxisZ), te)), m_Material.Shininess);
		const cColor C = tl.z * m_Material.Diffuse * LightDiffuse + s * m_Material.Specular * LightSpecular;
		r.Color = C.ToRgba();
	}
} // cMesh::CalcColor

//-----------------------------------------------------------------------------
// cMesh::CreateShadowVolume
//-----------------------------------------------------------------------------
void cMesh::CreateShadowVolume(const cVec3 &LightPos) const {
	m_ShadowVolume.Clear();

	// Selecting vertex buffer (transformed or source):
	const cList<Vertex> *Verts = m_TransformedVerts.Count() > 0 ? &m_TransformedVerts : &m_Verts;
	
	// Projecting all vertices to infinity:
	cList<cVec4> Proj;
	for(int nVert = 0; nVert < Verts->Count(); nVert++) {
		const cVec3 &p = Verts->GetAt(nVert).Pos;
		Proj.Add(cVec4(p - LightPos, 0.0f));
	}
	
	// Classifying faces:
	cList<bool> IsFrontFacing;
	const int *Tri = m_Indices.ToPtr();
	for(int index = 0; index < m_Indices.Count(); index += 3, Tri += 3) {
		const cVec3 &p0 = Verts->GetAt(Tri[0]).Pos;
		const cVec3 &p1 = Verts->GetAt(Tri[1]).Pos;
		const cVec3 &p2 = Verts->GetAt(Tri[2]).Pos;
		const cVec3 c = 1.0f / 3.0f * (p0 + p1 + p2);
		const cVec3 Dir = cVec3::Cross(p1 - p0, p2 - p0);
		const float d = cVec3::Dot(LightPos - c, Dir);
		IsFrontFacing.Add(d > 0.0f);
	}
	
	// Classifying edges for shadow volume belonging:
	cList<bool> IsShadow;
	for(int nEdge = 0; nEdge < m_Edges.Count(); nEdge++) {
		IsShadow.Add(false);
		const cMesh::Edge &e = m_Edges[nEdge];
		if(e.Tris[0] < 0 || e.Tris[1] < 0) {
			const int iTri = e.Tris[0] < 0 ? e.Tris[1] / 3 : e.Tris[0] / 3;
			if(IsFrontFacing[iTri]) {
				IsShadow.GetBack() = true;
			}
		} else {
			if(IsFrontFacing[e.Tris[0] / 3] != IsFrontFacing[e.Tris[1] / 3]) {
				IsShadow.GetBack() = true;
			}
		}
	}
	
	// Creating shadow volume:
	for(int index = 0; index < m_EdgeIndices.Count(); index++) {
		const int iEdge = m_EdgeIndices[index];
		const int nEdge = cMath::Abs(iEdge);
		if(!IsShadow[nEdge]) {
			continue;
		}
		
		const cMesh::Edge &e = m_Edges[nEdge];
		int i0 = e.Verts[0];
		int i1 = e.Verts[1];
		if((IsFrontFacing[index / 3] && iEdge >= 0) || (!IsFrontFacing[index / 3] && iEdge < 0)) {
			cMath::Swap(i0, i1);
		}
		
		const cVec3 &p0 = Verts->GetAt(i0).Pos;
		const cVec3 &p1 = Verts->GetAt(i1).Pos;
		const cVec4 &Pr0 = Proj[i0];
		const cVec4 &Pr1 = Proj[i1];

		m_ShadowVolume.Add(cVec4(p0, 1.0f));
		m_ShadowVolume.Add(cVec4(p1, 1.0f));
		m_ShadowVolume.Add(Pr1);

		m_ShadowVolume.Add(cVec4(p0, 1.0f));
		m_ShadowVolume.Add(Pr1);
		m_ShadowVolume.Add(Pr0);
		
		IsShadow[nEdge] = false;
	}

	Tri = m_Indices.ToPtr();
	for(int index = 0; index < m_Indices.Count(); index += 3, Tri += 3) {
		if(IsFrontFacing[index / 3]) {
			const cVec3 &t0 = Verts->GetAt(Tri[0]).Pos;
			const cVec3 &t1 = Verts->GetAt(Tri[1]).Pos;
			const cVec3 &t2 = Verts->GetAt(Tri[2]).Pos;
			
			m_ShadowVolume.Add(cVec4(t0, 1.0f));
			m_ShadowVolume.Add(cVec4(t1, 1.0f));
			m_ShadowVolume.Add(cVec4(t2, 1.0f));
//		} else {
			m_ShadowVolume.Add(Proj[Tri[0]]);
			m_ShadowVolume.Add(Proj[Tri[2]]);
			m_ShadowVolume.Add(Proj[Tri[1]]);
		}
	}
} // cMesh::CreateShadowVolume

//-----------------------------------------------------------------------------
// cMesh::RenderShadowVolume
//-----------------------------------------------------------------------------
void cMesh::RenderShadowVolume() const {
	cRender::GetInstance()->DrawPrimitives(cRender::Topology::TriangleList, cVertex::PositionRhw::GetFormat(), m_ShadowVolume.ToPtr(), m_ShadowVolume.Count());
} // cMesh::RenderShadowVolume

//-----------------------------------------------------------------------------
// cMesh::CalcEnvTexCoords
//-----------------------------------------------------------------------------
void cMesh::CalcEnvTexCoords() {
	cVertex::Bump *Verts = m_TransformedVerts.Count() ? m_TransformedVerts.ToPtr() : m_Verts.ToPtr();
	const int Count = m_Verts.Count();
	for(int i = 0; i < Count; i++) {
		cVertex::Bump &u = *Verts++;
		const cVec3 Pos = cVec3::TransformCoordinate(u.Pos, cRender::GetInstance()->GetViewer()->GetViewMatrix());
		const cVec3 Normal = cVec3::TransformNormal(u.Normal, cRender::GetInstance()->GetViewer()->GetViewMatrix());
		
		const cVec3 r = cVec3::Reflect(Pos.ToNormal(), Normal);
		const float m = 2.0f * cMath::Sqrt(r.x * r.x + r.y * r.y + cMath::Square(r.z + 1.0f));
		u.TexCoord.x = r.x / m + 0.5f;
		u.TexCoord.y = r.y / m + 0.5f;
	}
} // cMesh::CalcEnvTexCoords


//-------------------------------------------------------------------------------------------------------------------------------
// cMesh::RayIntersection
//-------------------------------------------------------------------------------------------------------------------------------
bool cMesh::RayIntersection(const cVec3 &RayOrig, const cVec3 &RayDir, float &Scale, const bool BackFaceCull /*= false*/) const {
	float u, v, t;
	Scale = cMath::Infinity;
	const Vertex *pVerts = m_TransformedVerts.Count() ? m_TransformedVerts.ToPtr() : m_Verts.ToPtr();
	for(int index = 0; index < m_Indices.Count(); index += 3) {
		const int *Tri = m_Indices.ToPtr() + index;
		const cVec3 &t0 = pVerts[*Tri].Pos;
		const cVec3 &t1 = pVerts[*(Tri + 1)].Pos;
		const cVec3 &t2 = pVerts[*(Tri + 2)].Pos;
		if(cMath::RayTri(RayOrig, RayDir, t0, t1, t2, u, v, t)) {
			Scale = cMath::Min(Scale, t);
		}
	}
	return Scale != cMath::Infinity;
} // cMesh::RayIntersection

//*****************************************************************************
// GenTorus
//*****************************************************************************

//-----------------------------------------------------------------------------
// cMesh::TorusArgs::SetDefaults
//-----------------------------------------------------------------------------
void cMesh::TorusArgs::SetDefaults() {
	RadiusMajor = 1.0f;
	RadiusMinor = 0.5f;
	Sections = 20;
	Slices = 20;
	Axis = cVec3::AxisY;
} // cMesh::TorusArgs::SetDefaults

//-----------------------------------------------------------------------------
// cMesh::TorusArgs::Validate
//-----------------------------------------------------------------------------
void cMesh::TorusArgs::Validate() {
	const TorusArgs Def;
	if(RadiusMajor < 0.0f) {
		RadiusMajor = 0.0f;
	}
	if(RadiusMinor < cMath::SpaceEpsilon) {
		RadiusMinor = Def.RadiusMinor;
	}
	if(Sections < 3) {
		Sections = 3;
	}
	if(Slices < 3) {
		Slices = 3;
	}
	Axis.Normalize();
	if(!Axis.IsNormalized()) {
		Axis = Def.Axis;
	}
} // cMesh::TorusArgs::Validate

//-----------------------------------------------------------------------------
// cMesh::GenTorus
//-----------------------------------------------------------------------------
cMesh * cMesh::GenTorus(TorusArgs Args) {
	Args.Validate();
	
	const float dSection = cMath::TwoPi / (float)Args.Sections;
	const float dSlice = cMath::TwoPi / (float)Args.Slices;
	const float iSections = 1.0f / (float)Args.Sections;
	const float iSlices = 1.0f / (float)Args.Slices;

	const int NVerts = (Args.Slices + 1) * (Args.Sections + 1);
	Vertex *pVerts = new Vertex[NVerts];
	const int NIndices = 3 * 2 * Args.Slices * Args.Sections;
	int *pIndices = new int[NIndices];
	
	// Vertices:
	int nVert = 0;
	for(int nRing = 0; nRing <= Args.Sections; nRing++) {
		float Theta = (float)nRing * dSection;
		float CosTheta = cMath::Cos(Theta);
		float SinTheta = cMath::Sin(Theta);
		for(int nSeg = 0; nSeg <= Args.Slices; nSeg++, nVert++) {
			float Phi = nSeg * dSlice;
			float CosPhi = cMath::Cos(Phi);
			float SinPhi = cMath::Sin(Phi);
			float DistToCenter = Args.RadiusMajor + Args.RadiusMinor * CosPhi;

			Vertex &r = pVerts[nVert];
			r.Pos.Set(CosTheta * DistToCenter, - SinTheta * DistToCenter, Args.RadiusMinor * SinPhi);
			r.TexCoord.Set((float)nSeg * iSlices, (float)nRing * iSections);

			float dxdTheta = -SinTheta * DistToCenter;
			float dydTheta = -CosTheta * DistToCenter;
			float d = 1.0f / cMath::Sqrt(dxdTheta * dxdTheta + dydTheta * dydTheta);
			r.BiTangent.Set(dxdTheta * d, dydTheta * d, 0.0f);
			r.Normal.Set(CosTheta * CosPhi, - SinTheta * CosPhi, SinPhi);
			r.Tangent = cVec3::Cross(r.BiTangent, r.Normal);
		}
	}

	// Indices:
	int *Tri = pIndices;
	for(int nSec = 0; nSec < Args.Sections; nSec++) {
		for(int nSlice = 0; nSlice < Args.Slices; nSlice++) {
			Tri[0] = nSec * (Args.Slices + 1) + nSlice;
			Tri[1] = (nSec + 1) * (Args.Slices + 1) + nSlice + 1;
			Tri[2] = (nSec + 1) * (Args.Slices + 1) + nSlice;
			cAssert(cVec3::Dot(cVec3::Cross(pVerts[Tri[1]].Pos - pVerts[Tri[0]].Pos,
				pVerts[Tri[2]].Pos - pVerts[Tri[0]].Pos), pVerts[Tri[0]].Normal) > 0.0f);
			Tri += 3;

			Tri[0] = nSec * (Args.Slices + 1) + nSlice;
			Tri[1] = nSec * (Args.Slices + 1) + nSlice + 1;
			Tri[2] = (nSec + 1) * (Args.Slices + 1) + nSlice + 1;
			cAssert(cVec3::Dot(cVec3::Cross(pVerts[Tri[1]].Pos - pVerts[Tri[0]].Pos,
				pVerts[Tri[2]].Pos - pVerts[Tri[0]].Pos), pVerts[Tri[0]].Normal) > 0.0f);
			Tri += 3;
		}
	}

	return new cMesh("Torus", cMesh::SetCtor, pVerts, NVerts, pIndices, NIndices);
} // cMesh::GenTorus

//*****************************************************************************
// GenSphere
//*****************************************************************************

//-----------------------------------------------------------------------------
// cMesh::SphereArgs::SetDefaults
//-----------------------------------------------------------------------------
void cMesh::SphereArgs::SetDefaults() {
	Radius = 1.0f;
	Sections = 20;
	Slices = 20;
	Axis = cVec3::AxisY;
} // cMesh::SphereArgs::SetDefaults

//-----------------------------------------------------------------------------
// cMesh::SphereArgs::Validate
//-----------------------------------------------------------------------------
void cMesh::SphereArgs::Validate() {
	const SphereArgs Defs;
	if(Radius < cMath::SpaceEpsilon) {
		Radius = Defs.Radius;
	}
	if(Sections < 3) {
		Sections = 3;
	}
	if(Slices < 2) {
		Slices = 2;
	}
	Axis.Normalize();
	if(!Axis.IsNormalized()) {
		Axis = Defs.Axis;
	}
} // cMesh::SphereArgs::Validate

//----------------------------------------------------------------------------------
// cMesh::GenSphere
//----------------------------------------------------------------------------------
cMesh * cMesh::GenSphere(SphereArgs Args) {
	Args.Validate();
	
	const float SectionAngle = 360.0f / (float)Args.Sections;
	const float SliceAngle = 180.0f / (float)Args.Slices;
	const float iSections = 1.0f / (float)Args.Sections;
	const float iSlices = 1.0f / (float)Args.Slices;

	const int NVerts = Args.Sections * 2 + (Args.Sections + 1) * (Args.Slices - 1);
	const int NIndices = Args.Sections * 2 * 3 + Args.Sections * (Args.Slices - 2) * 2 * 3;
	cVertex::Bump *pVerts = new cVertex::Bump[NVerts];
	int *pIndices = new int [NIndices];

	// Vertices:
	int nVert = 0;
	cRotation AxisTube(cVec3::Zero, Args.Axis, 0.0f);
	const cVec3 Pole = AxisTube.GetAxis() * Args.Radius;
	const cVec3 StartRef = AxisTube.GetAxis().ToPerp().ToNormal() * Args.Radius;
	for(int nRing = 0; nRing <= Args.Sections; nRing++) {
		const float Theta = (float)nRing * SectionAngle;
		const float s = (float)nRing * iSections;
		AxisTube.SetAngle(Theta);
		const cVec3 RingRef = nRing % Args.Sections ? cVec3::Rotate(StartRef, AxisTube) : StartRef;
		cRotation RingTube(cVec3::Zero, cVec3::Cross(Pole, RingRef).ToNormal(), 0.0f);
		// Top pole:
		if(nRing < Args.Sections) {
			Vertex &r = pVerts[nVert];
			r.Pos = Pole;
			r.Normal = AxisTube.GetAxis();
			r.TexCoord.Set(s + iSections * 0.5f, 0.0f);
			nVert++;
		}
		// Central slices:
		for(int nSeg = 1; nSeg < Args.Slices; nSeg++, nVert++) {
			const float t = (float)nSeg * iSlices;
			Vertex &r = pVerts[nVert];
			const float Phi = -90.0f + (float)nSeg * SliceAngle;
			RingTube.SetAngle(Phi);
			r.Pos = cVec3::Rotate(RingRef, RingTube);
			r.Normal = r.Pos.ToNormal();
			r.TexCoord.Set(s, t);
		}
		// Bottom pole:
		if(nRing < Args.Sections) {
			Vertex &r = pVerts[nVert];
			r.Pos = -Pole;
			r.Normal = -AxisTube.GetAxis();
			r.TexCoord.Set(s + iSections * 0.5f, 1.0f);
			nVert++;
		}
	}
	cAssert(NVerts == nVert);

	// Indices:
	int *Tri = pIndices;
	for(int nSec = 0; nSec < Args.Sections; nSec++) {
		const bool EndSec = nSec + 1 == Args.Sections;
		const int Shift = EndSec ? 0 : 1;
		// Top pole:
		Tri[0] = nSec * (Args.Slices + 1);
		Tri[1] = nSec * (Args.Slices + 1) + 1;
		Tri[2] = (nSec + 1) * (Args.Slices + 1) + Shift;
		cAssert(cVec3::Dot(cVec3::Cross(pVerts[Tri[1]].Pos - pVerts[Tri[0]].Pos,
			pVerts[Tri[2]].Pos - pVerts[Tri[0]].Pos), pVerts[Tri[0]].Normal) > 0.0f);
		Tri += 3;
		// Central slices:
		for(int nSlice = 1; nSlice < Args.Slices - 1; nSlice++) {
			const int tr = nSec * (Args.Slices + 1) + nSlice;
			const int bl = (nSec + 1) * (Args.Slices + 1) + nSlice + Shift;
			Tri[0] = tr;
			Tri[1] = tr + 1;
			Tri[2] = bl;
			cAssert(cVec3::Dot(cVec3::Cross(pVerts[Tri[1]].Pos - pVerts[Tri[0]].Pos,
				pVerts[Tri[2]].Pos - pVerts[Tri[0]].Pos), pVerts[Tri[0]].Normal) > 0.0f);
			Tri += 3;

			Tri[0] = tr;
			Tri[1] = bl;
			Tri[2] = bl - 1;
			cAssert(cVec3::Dot(cVec3::Cross(pVerts[Tri[1]].Pos - pVerts[Tri[0]].Pos,
				pVerts[Tri[2]].Pos - pVerts[Tri[0]].Pos), pVerts[Tri[0]].Normal) > 0.0f);
			Tri += 3;
		}

		// Bottom pole:
		Tri[0] = nSec * (Args.Slices + 1) + Args.Slices - 1;
		Tri[1] = nSec * (Args.Slices + 1) + Args.Slices;
		Tri[2] = (nSec + 1) * (Args.Slices + 1) + Args.Slices - 2 + Shift;
		cAssert(cVec3::Dot(cVec3::Cross(pVerts[Tri[1]].Pos - pVerts[Tri[0]].Pos,
			pVerts[Tri[2]].Pos - pVerts[Tri[0]].Pos), pVerts[Tri[0]].Normal) > 0.0f);
		Tri += 3;
	}
	cAssert(Tri - pIndices == NIndices);

	return new cMesh("Sphere", cMesh::SetCtor, pVerts, NVerts, pIndices, NIndices);
} // cMesh::GenSphere

//*****************************************************************************
// GenPlane
//*****************************************************************************

//-----------------------------------------------------------------------------
// cMesh::PlaneArgs::SetDefaults
//-----------------------------------------------------------------------------
void cMesh::PlaneArgs::SetDefaults() {
	Width = 1.0f;
	Height = 1.0f;
	wSubDivs = 10;
	hSubDivs = 10;
	Axis = cVec3::AxisY;
	TwoSided = false;
} // cMesh::PlaneArgs::SetDefaults

//-----------------------------------------------------------------------------
// cMesh::PlaneArgs::Validate
//-----------------------------------------------------------------------------
void cMesh::PlaneArgs::Validate() {
	const PlaneArgs Def;
	if(Width < cMath::SpaceEpsilon) {
		Width = Def.Width;
	}
	if(Height < cMath::SpaceEpsilon) {
		Height = Def.Height;
	}
	if(wSubDivs < 1) {
		wSubDivs = Def.wSubDivs;
	}
	if(hSubDivs < 1) {
		hSubDivs = Def.hSubDivs;
	}
	Axis.Normalize();
	if(!Axis.IsNormalized()) {
		Axis = Def.Axis;
	}
} // cMesh::PlaneArgs::Validate

//-----------------------------------------------------------------------------
// cMesh::GenPlane
//-----------------------------------------------------------------------------
cMesh * cMesh::GenPlane(PlaneArgs Args) {
	Args.Validate();
	
	const int NVerts = (Args.wSubDivs + 1) * (Args.hSubDivs + 1) * (Args.TwoSided ? 2 : 1);
	Vertex *pVerts = new Vertex[NVerts];
	const int NIndices = 3 * 2 * Args.wSubDivs * Args.hSubDivs * (Args.TwoSided ? 2 : 1);
	int *pIndices = new int[NIndices];

	// Vertices:
	const float iwSubDivs = 1.0f / (float)Args.wSubDivs;
	const float ihSubDivs = 1.0f / (float)Args.hSubDivs;

	int nVert = 0;
	float x0 = - 0.5f * Args.Width;
	float xCur = x0;
	float zCur = - 0.5f * Args.Height;
	float dx = Args.Width / (float)Args.wSubDivs;
	float dz = Args.Height / (float)Args.hSubDivs;
	int ix, iz;
	const int Half = NVerts / 2;
	for(iz = 0; iz <= Args.hSubDivs; iz++) {
		for(ix = 0; ix <= Args.wSubDivs; ix++) {
			Vertex &r = pVerts[nVert];
			r.Pos.Set(xCur, 0.0f, zCur);
			r.Normal = Args.Axis;
			r.TexCoord.Set((float)ix * iwSubDivs, (float)iz * ihSubDivs);
			r.Tangent = cVec3::AxisNegX;
			r.BiTangent = cVec3::AxisZ;

			if(Args.TwoSided) {
				Vertex &u = pVerts[Half + nVert];
				u = r;
				u.Normal *= -1.0f;
				u.Tangent *= -1.0f;
			}
			nVert++;

			xCur += dx;
		}
		zCur += dz;
		xCur = x0;
	}
	// Indices:
	int *Tri = pIndices;
	for(iz = 0; iz < Args.hSubDivs; iz++) {
		for(ix = 0; ix < Args.wSubDivs; ix++) {
			int i0 = (Args.wSubDivs + 1) * iz + ix;
			int i1 = (Args.wSubDivs + 1) * (iz + 1) + ix;

			Tri[0] = i0;
			Tri[1] = i1;
			Tri[2] = i1 + 1;
			Tri += 3;

			Tri[0] = i0;
			Tri[1] = i1 + 1;
			Tri[2] = i0 + 1;
			Tri += 3;

			if(Args.TwoSided) {
				Tri[0] = Half + i0;
				Tri[1] = Half + i1 + 1;
				Tri[2] = Half + i1;
				Tri += 3;
				
				Tri[0] = Half + i0;
				Tri[1] = Half + i0 + 1;
				Tri[2] = Half + i1 + 1;
				Tri += 3;
			}
		}
	}

	return new cMesh("Plane", cMesh::SetCtor, pVerts, NVerts, pIndices, NIndices);
} // cMesh::GenPlane

void cMesh::GlobeArgs::SetDefaults() {
	Radius = 1.0f;
	SubDivs = 1;
}

void cMesh::GlobeArgs::Validate() {
	const GlobeArgs Defs;
	if(Radius < cMath::SpaceEpsilon) {
		Radius = Defs.Radius;
	}
	if(SubDivs < 1) {
		SubDivs = 1;
	}
}

cMesh * cMesh::GenGlobe(GlobeArgs Args) {
	// Icosahedron on unit sphere:
	static const float h = 0.5f;
	static const float Rc = cMath::Sqrt(5.0f) / 2.0f;
	static const float r36 = cMath::Rad(36.0f);
	static const cVec3 vbIcosahedron[20] = {
		cVec3(cMath::Cos(r36 * 1.0f), h, cMath::Sin(r36 * 1.0f)) / Rc,
		cVec3(cMath::Cos(r36 * 2.0f), -h, cMath::Sin(r36 * 2.0f)) / Rc,
		cVec3(cMath::Cos(r36 * 3.0f), h, cMath::Sin(r36 * 3.0f)) / Rc,
		cVec3(cMath::Cos(r36 * 4.0f), -h, cMath::Sin(r36 * 4.0f)) / Rc,
		cVec3(cMath::Cos(r36 * 5.0f), h, cMath::Sin(r36 * 5.0f)) / Rc,
		cVec3(cMath::Cos(r36 * 6.0f), -h, cMath::Sin(r36 * 6.0f)) / Rc,
		cVec3(cMath::Cos(r36 * 7.0f), h, cMath::Sin(r36 * 7.0f)) / Rc,
		cVec3(cMath::Cos(r36 * 8.0f), -h, cMath::Sin(r36 * 8.0f)) / Rc,
		cVec3(cMath::Cos(r36 * 9.0f), h, cMath::Sin(r36 * 9.0f)) / Rc,
		cVec3(cMath::Cos(r36 * 10.0f), -h, cMath::Sin(r36 * 10.0f)) / Rc,
		// Top pole is splitted 5 times:
		cVec3(0.0f, 1.0f, 0.0f), // 10, 11, 12, 13, 14
		cVec3(0.0f, 1.0f, 0.0f),
		cVec3(0.0f, 1.0f, 0.0f),
		cVec3(0.0f, 1.0f, 0.0f),
		cVec3(0.0f, 1.0f, 0.0f),
		// Bottom pole is plitted 5 times:
		cVec3(0.0f, -1.0f, 0.0f), // 15, 16, 17, 18, 19
		cVec3(0.0f, -1.0f, 0.0f),
		cVec3(0.0f, -1.0f, 0.0f),
		cVec3(0.0f, -1.0f, 0.0f),
		cVec3(0.0f, -1.0f, 0.0f)
	};
	static const int ibIcosahedron[20 * 3] = {
		0, 10, 2,
		2, 11, 4,
		4, 12, 6,
		6, 13, 8,
		8, 14, 0,
		0, 2, 1,
		1, 2, 3,
		2, 4, 3,
		3, 4, 5,
		4, 6, 5,
		5, 6, 7,
		6, 8, 7,
		7, 8, 9,
		8, 0, 9,
		9, 0, 1,
		1, 15, 9,
		3, 16, 1,
		5, 17, 3,
		7, 18, 5,
		9, 19, 7
	};

	Args.Validate();
	const int NVerts = sizeof(vbIcosahedron) / sizeof(vbIcosahedron[0]);
	const int NIndices = sizeof(ibIcosahedron) / sizeof(ibIcosahedron[0]);
	cVertex::Bump *pVerts = new cVertex::Bump[NVerts];
	int *pIndices = new int[NIndices];
	for(int i = 0; i < NVerts; i++) {
		pVerts[i].Pos = vbIcosahedron[i];

	}
	CopyMemory(pIndices, ibIcosahedron, sizeof(ibIcosahedron));
	cMesh *pSphere = new cMesh("Sphere", cMesh::SetCtor, pVerts, NVerts, pIndices, NIndices);

	cVertex::Bump u;
	for(int Level = 1; Level < Args.SubDivs; Level++) {
		// Enuming edges and subdividing:
		for(int nEdge = 0; nEdge < pSphere->GetEdges().Count(); nEdge++) {
			Edge &e = pSphere->GetEdges()[nEdge];
			if(e.Verts[0] == e.Verts[1]) {
				continue;
			}
			u.Pos = cVec3::Lerp05(pSphere->GetVertices()[e.Verts[0]].Pos, pSphere->GetVertices()[e.Verts[1]].Pos).ToNormal();
			e.Tris[0] = pSphere->GetVertices().Add(u);
		}
		// Reindexing:
		cList<int> ReIndices;
		for(int index = 0; index < pSphere->GetIndices().Count(); index += 3) {
			const Edge &E0 = pSphere->GetEdges()[cMath::Abs(pSphere->GetEdgeIndices()[index])];
			const Edge &E1 = pSphere->GetEdges()[cMath::Abs(pSphere->GetEdgeIndices()[index + 1])];
			const Edge &E2 = pSphere->GetEdges()[cMath::Abs(pSphere->GetEdgeIndices()[index + 2])];
			const int i0 = pSphere->GetIndices()[index];
			const int i1 = pSphere->GetIndices()[index + 1];
			const int i2 = pSphere->GetIndices()[index + 2];
			ReIndices.Add(i0), ReIndices.Add(E0.Tris[0]), ReIndices.Add(E2.Tris[0]);
			ReIndices.Add(E0.Tris[0]), ReIndices.Add(E1.Tris[0]), ReIndices.Add(E2.Tris[0]);
			ReIndices.Add(E0.Tris[0]), ReIndices.Add(i1), ReIndices.Add(E1.Tris[0]);
			ReIndices.Add(E2.Tris[0]), ReIndices.Add(E1.Tris[0]), ReIndices.Add(i2);
			//      i2       //
			//      /\       //
			//     /  \      //
			// E2 /----\ E1  //
			//   / \  / \    //
			//  /___\/___\   //
			// i0   E0   i1  //
		}
		pSphere->GetIndices().Copy(ReIndices);
		pSphere->FillEdges();
	}

	// Postprocessing:
	for(int i = 0; i < pSphere->GetVertices().Count(); i++) {
		cVertex::Bump &u = pSphere->GetVertices()[i];
		// Normal:
		u.Normal = u.Pos;
		// Radius:
		u.Pos *= Args.Radius;
		// TexCoord:
		const float Phi = cMath::ATan(u.Normal.x, u.Normal.z);
		const float Psi = cMath::ATan(cMath::Sqrt(u.Normal.x * u.Normal.x + u.Normal.z * u.Normal.z), u.Normal.y);
		cAssert(cMath::IsInRangeEps(Phi, -cMath::Pi, cMath::Pi));
		cAssert(cMath::IsInRangeEps(Psi, 0.0f, cMath::Pi));
		u.TexCoord.Set(Phi / cMath::TwoPi + 0.5f, Psi / cMath::Pi);

		u.Tangent = cVec3::Cross(u.Normal, cVec3::AxisY).ToNormal();
		if(!u.Tangent.IsNormalized()) { // Fallback:
			u.Normal.ToPerps(u.Tangent, u.BiTangent);
		} else {
			u.BiTangent = cVec3::Cross(u.Normal, u.Tangent);
			cAssert(u.BiTangent.IsNormalized());
		}
	}
	for(int index = 0; index < pSphere->GetIndices().Count(); index += 3) {
		int *Tri = &pSphere->GetIndices()[index];
		cVertex::Bump *u[3] = { &pSphere->GetVertices()[Tri[0]], &pSphere->GetVertices()[Tri[1]], &pSphere->GetVertices()[Tri[2]] };
		const cVec3 Center = cVec3::Normalize(1.0f / 3.0f * (u[0]->Pos + u[1]->Pos + u[2]->Pos)).ToNormal();
		const float Phi = cMath::ATan(Center.x, Center.z);
		const float Psi = cMath::ATan(cMath::Sqrt(Center.x * Center.x + Center.z * Center.z), Center.y);
		cAssert(cMath::IsInRangeEps(Phi, -cMath::Pi, cMath::Pi));
		cAssert(cMath::IsInRangeEps(Psi, 0.0f, cMath::Pi));
		const cVec2 TexCoord(Phi / cMath::TwoPi + 0.5f, Psi / cMath::Pi);
		for(int k = 0; k < 3; k++) {
			bool Dup = false;
			cVertex::Bump d = *u[k];
			if(cMath::Abs(TexCoord.x - d.TexCoord.x) >= 0.5f) {
				Dup = true;
				if(d.TexCoord.x < TexCoord.x) {
					d.TexCoord.x += 1.0f;
				} else {
					d.TexCoord.x -= 1.0f;
				}
//				u[k]->Pos += u[k]->Normal;
			}
			if(cMath::Abs(TexCoord.y - d.TexCoord.y) >= 0.5f) {
				Dup = true;
				if(d.TexCoord.y < TexCoord.y) {
					d.TexCoord.y += 1.0f;
				} else {
					d.TexCoord.y -= 1.0f;
				}
//				u[k]->Pos += u[k]->Normal;
			}
			if(Dup) {
				Tri[k] = pSphere->GetVertices().Add(d);
				u[k] = &pSphere->GetVertices().GetBack();
			}
		}
		for(int k = 0; k < 3; k++) {
			if(cVec3::Equals(cVec3::Abs(u[k]->Normal), cVec3::AxisY)) {
//				u[k]->Pos += u[k]->Normal;
				u[k]->TexCoord.x = cMath::Lerp(u[(k + 1) % 3]->TexCoord.x, u[(k + 2) % 3]->TexCoord.x, 0.5f);
			}
		}
	}


	return pSphere;
}
/*
cMesh * cMesh::GenGlobe(GlobeArgs Args) {
	// Icosahedron on unit sphere:
	static const float h = 0.5f;
	static const float Rc = cMath::Sqrt(5.0f) / 2.0f;
	static const float r36 = cMath::Rad(36.0f);
	static const cVec3 vbIcosahedron[20] = {
		cVec3(cMath::Cos(r36 * 1.0f), h, cMath::Sin(r36 * 1.0f)) / Rc,
		cVec3(cMath::Cos(r36 * 2.0f), -h, cMath::Sin(r36 * 2.0f)) / Rc,
		cVec3(cMath::Cos(r36 * 3.0f), h, cMath::Sin(r36 * 3.0f)) / Rc,
		cVec3(cMath::Cos(r36 * 4.0f), -h, cMath::Sin(r36 * 4.0f)) / Rc,
		cVec3(cMath::Cos(r36 * 5.0f), h, cMath::Sin(r36 * 5.0f)) / Rc,
		cVec3(cMath::Cos(r36 * 6.0f), -h, cMath::Sin(r36 * 6.0f)) / Rc,
		cVec3(cMath::Cos(r36 * 7.0f), h, cMath::Sin(r36 * 7.0f)) / Rc,
		cVec3(cMath::Cos(r36 * 8.0f), -h, cMath::Sin(r36 * 8.0f)) / Rc,
		cVec3(cMath::Cos(r36 * 9.0f), h, cMath::Sin(r36 * 9.0f)) / Rc,
		cVec3(cMath::Cos(r36 * 10.0f), -h, cMath::Sin(r36 * 10.0f)) / Rc,
		// Top pole is splitted 5 times:
		cVec3(0.0f, 1.0f, 0.0f), // 10, 11, 12, 13, 14
		cVec3(0.0f, 1.0f, 0.0f),
		cVec3(0.0f, 1.0f, 0.0f),
		cVec3(0.0f, 1.0f, 0.0f),
		cVec3(0.0f, 1.0f, 0.0f),
		// Bottom pole is plitted 5 times:
		cVec3(0.0f, -1.0f, 0.0f), // 15, 16, 17, 18, 19
		cVec3(0.0f, -1.0f, 0.0f),
		cVec3(0.0f, -1.0f, 0.0f),
		cVec3(0.0f, -1.0f, 0.0f),
		cVec3(0.0f, -1.0f, 0.0f)
	};
	static const int ibIcosahedron[20 * 3] = {
		0, 10, 2,
		2, 10, 4,
		4, 10, 6,
		6, 10, 8,
		8, 10, 0,
		0, 2, 1,
		1, 2, 3,
		2, 4, 3,
		3, 4, 5,
		4, 6, 5,
		5, 6, 7,
		6, 8, 7,
		7, 8, 9,
		8, 0, 9,
		9, 0, 1,
		1, 11, 9,
		3, 11, 1,
		5, 11, 3,
		7, 11, 5,
		9, 11, 7
	};

	Args.Validate();
	const int NVerts = sizeof(vbIcosahedron) / sizeof(vbIcosahedron[0]);
	const int NIndices = sizeof(ibIcosahedron) / sizeof(ibIcosahedron[0]);
	cVertex::Bump *pVerts = new cVertex::Bump[NVerts];
	int *pIndices = new int[NIndices];
	for(int i = 0; i < NVerts; i++) {
		pVerts[i].Pos = vbIcosahedron[i];
	}
	CopyMemory(pIndices, ibIcosahedron, sizeof(ibIcosahedron));
	cMesh *pSphere = new cMesh("Sphere", cMesh::SetCtor, pVerts, NVerts, pIndices, NIndices);

	cVertex::Bump u;
	for(int Level = 1; Level < Args.SubDivs; Level++) {
		// Enuming edges and subdividing:
		for(int nEdge = 0; nEdge < pSphere->GetEdges().Count(); nEdge++) {
			Edge &e = pSphere->GetEdges()[nEdge];
			if(e.Verts[0] == e.Verts[1]) {
				continue;
			}
			u.Pos = cVec3::Lerp05(pSphere->GetVertices()[e.Verts[0]].Pos, pSphere->GetVertices()[e.Verts[1]].Pos).ToNormal();
			e.Tris[0] = pSphere->GetVertices().Add(u);
		}
		// Reindexing:
		cList<int> ReIndices;
		for(int index = 0; index < pSphere->GetIndices().Count(); index += 3) {
			const Edge &E0 = pSphere->GetEdges()[cMath::Abs(pSphere->GetEdgeIndices()[index])];
			const Edge &E1 = pSphere->GetEdges()[cMath::Abs(pSphere->GetEdgeIndices()[index + 1])];
			const Edge &E2 = pSphere->GetEdges()[cMath::Abs(pSphere->GetEdgeIndices()[index + 2])];
			const int i0 = pSphere->GetIndices()[index];
			const int i1 = pSphere->GetIndices()[index + 1];
			const int i2 = pSphere->GetIndices()[index + 2];
			ReIndices.Add(i0), ReIndices.Add(E0.Tris[0]), ReIndices.Add(E2.Tris[0]);
			ReIndices.Add(E0.Tris[0]), ReIndices.Add(E1.Tris[0]), ReIndices.Add(E2.Tris[0]);
			ReIndices.Add(E0.Tris[0]), ReIndices.Add(i1), ReIndices.Add(E1.Tris[0]);
			ReIndices.Add(E2.Tris[0]), ReIndices.Add(E1.Tris[0]), ReIndices.Add(i2);
			//      i2       //
			//      /\       //
			//     /  \      //
			// E2 /----\ E1  //
			//   / \  / \    //
			//  /___\/___\   //
			// i0   E0   i1  //
		}
		pSphere->GetIndices().Copy(ReIndices);
		pSphere->FillEdges();
	}

	// Postprocessing:
	for(int i = 0; i < pSphere->GetVertices().Count(); i++) {
		cVertex::Bump &u = pSphere->GetVertices()[i];
		// Normal:
		u.Normal = u.Pos;
		// Radius:
		u.Pos *= Args.Radius;
		// TexCoord:
		const float Phi = cMath::ATan(u.Normal.x, u.Normal.z);
		const float Psi = cMath::ATan(cMath::Sqrt(u.Normal.x * u.Normal.x + u.Normal.z * u.Normal.z), u.Normal.y);
		cAssert(cMath::IsInRangeEps(Phi, -cMath::Pi, cMath::Pi));
		cAssert(cMath::IsInRangeEps(Psi, 0.0f, cMath::Pi));
		u.TexCoord.Set(Phi / cMath::TwoPi + 0.5f, Psi / cMath::Pi);

		u.Tangent = cVec3::Cross(u.Normal, cVec3::AxisY).ToNormal();
		if(!u.Tangent.IsNormalized()) { // Fallback:
			u.Normal.ToPerpNormals(u.Tangent, u.Binormal);
		} else {
			u.Binormal = cVec3::Cross(u.Normal, u.Tangent);
			cAssert(u.Binormal.IsNormalized());
		}
	}
	for(int index = 0; index < pSphere->GetIndices().Count(); index += 3) {
		int *Tri = &pSphere->GetIndices()[index];
		cVertex::Bump *u[3] = { &pSphere->GetVertices()[Tri[0]], &pSphere->GetVertices()[Tri[1]], &pSphere->GetVertices()[Tri[2]] };
		const cVec3 Center = cVec3::Normalize(1.0f / 3.0f * (u[0]->Pos + u[1]->Pos + u[2]->Pos)).ToNormal();
		const float Phi = cMath::ATan(Center.x, Center.z);
		const float Psi = cMath::ATan(cMath::Sqrt(Center.x * Center.x + Center.z * Center.z), Center.y);
		cAssert(cMath::IsInRangeEps(Phi, -cMath::Pi, cMath::Pi));
		cAssert(cMath::IsInRangeEps(Psi, 0.0f, cMath::Pi));
		const cVec2 TexCoord(Phi / cMath::TwoPi + 0.5f, Psi / cMath::Pi);
		for(int k = 0; k < 3; k++) {
			bool Dup = false;
			cVertex::Bump d = *u[k];
			if(cMath::Abs(TexCoord.x - d.TexCoord.x) >= 0.5f) {
				Dup = true;
				if(d.TexCoord.x < TexCoord.x) {
					d.TexCoord.x += 1.0f;
				} else {
					d.TexCoord.x -= 1.0f;
				}
//				u[k]->Pos += u[k]->Normal;
			}
			if(cMath::Abs(TexCoord.y - d.TexCoord.y) >= 0.5f) {
				Dup = true;
				if(d.TexCoord.y < TexCoord.y) {
					d.TexCoord.y += 1.0f;
				} else {
					d.TexCoord.y -= 1.0f;
				}
//				u[k]->Pos += u[k]->Normal;
			}
			if(Dup) {
				Tri[k] = pSphere->GetVertices().Add(d);
			}
		}
		for(int k = 0; k < 3; k++) {

		}
	}


	return pSphere;
}*/

/*
//-----------------------------------------------------------------------------
// cMesh::CreateBuffers
//-----------------------------------------------------------------------------
void cMesh::CreateBuffers() {
	// Vertex Buffer Object:
	glGenBuffersARB(1, &m_VertexBufferObject);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_VertexBufferObject);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_Verts.Count() * sizeof(Vertex), m_Verts.Ptr(), GL_STREAM_DRAW_ARB);
	
	// Index Buffer Object:
	glGenBuffersARB(1, &m_IndexBufferObject);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_IndexBufferObject);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_Indices.Count() * sizeof(int), m_Indices.Ptr(), GL_STATIC_DRAW_ARB);
	
	// Unbind buffers:
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
} // cMesh::CreateBuffers
*/

//-----------------------------------------------------------------------------
// cMesh::Render
//-----------------------------------------------------------------------------
void cMesh::Render() const {
	const void *Verts = m_TransformedVerts.Count() ? m_TransformedVerts.ToPtr() : m_Verts.ToPtr();
	cRender::GetInstance()->DrawPrimitives(cRender::Topology::TriangleList, Vertex::GetFormat(), Verts, m_Verts.Count(), m_Indices.ToPtr(), m_Indices.Count(), &m_Material);
} // cMesh::Render

//----------------------------------------------------------------------------------------------------
// cMesh::RenderEdges
//----------------------------------------------------------------------------------------------------
void cMesh::RenderEdges(const cColor &Color /*= cColor::White*/, const float Width /*= 1.0f*/) const {
	const cList<Vertex> &Verts = m_TransformedVerts.Count() ? m_TransformedVerts : m_Verts;
	for(int i = 0; i < m_Edges.Count(); i++) {
		const Edge &e = m_Edges[i];
		const cVec3 &l0 = Verts[e.Verts[0]].Pos;
		const cVec3 &l1 = Verts[e.Verts[1]].Pos;
		cRender::GetInstance()->DrawLine(l0, l1, Color, Width);
	}
} // cMesh::RenderEdges

//*****************************************************************************
// cMeshNode
//*****************************************************************************

//-----------------------------------------------------------------------------
// cMeshNode.ctor
//-----------------------------------------------------------------------------
cMeshNode::cMeshNode(cMesh *Mesh) {
	m_pMesh = Mesh;
} // cMeshNode.ctor

//-----------------------------------------------------------------------------
// cMeshNode.dtor
//-----------------------------------------------------------------------------
cMeshNode::~cMeshNode() {
	delete m_pMesh;
	for(Links::Iterator It = m_Links.GetIterator(); !It.End(); ++It) {
		delete (*It)->Node;
		delete *It;
	}
} // cMeshNode.dtor

//-----------------------------------------------------------------------------
// cMeshNode::SetMesh
//-----------------------------------------------------------------------------
void cMeshNode::SetMesh(cMesh *Mesh) {
	if(m_pMesh) {
		delete m_pMesh;
	}
	m_pMesh = Mesh;
} // cMeshNode::SetMesh

//-----------------------------------------------------------------------------------------------------------------------------
// cMeshNode::Attach
//-----------------------------------------------------------------------------------------------------------------------------
bool cMeshNode::Attach(const char *LinkName, cMesh *SubMesh, const cVec3 &Scale, const cQuat &Rotate, const cVec3 &Translate) {
	Link *L = new Link;
	L->Name = LinkName;
	L->Node = new cMeshNode(SubMesh);
	L->Scale = Scale;
	L->Rotate = Rotate;
	L->Translate = Translate;
	m_Links.Add(L);
	return true;
} // cMeshNode::Attach

//------------------------------------------------------------------------------------------------------------------------------
// cMeshNode::Attach
//------------------------------------------------------------------------------------------------------------------------------
bool cMeshNode::Attach(const char *LinkName, cMeshNode *Node, const cVec3 &Scale, const cQuat &Rotate, const cVec3 &Translate) {
	Link *L = new Link;
	L->Name = LinkName;
	L->Node = Node;
	L->Scale = Scale;
	L->Rotate = Rotate;
	L->Translate = Translate;
	m_Links.Add(L);
	return true;
} // cMeshNode::Attach

//-----------------------------------------------------------------------------
// cMeshNode::Attach
//-----------------------------------------------------------------------------
bool cMeshNode::Attach(cMeshNode::Link *Link) {
	m_Links.Add(Link);
	return true;
} // cMeshNode::Attach

//-----------------------------------------------------------------------------
// cMeshNode::GetLink
//-----------------------------------------------------------------------------
cMeshNode::Link * cMeshNode::GetLink(const char *Name) const {
	for(Links::Iterator It = m_Links.GetIterator(); !It.End(); ++It) {
		if((*It)->Name == Name) {
			return *It;
		}
	}
	for(Links::Iterator It = m_Links.GetIterator(); !It.End(); ++It) {
		if((*It)->Node != NULL) {
			Link *L = (*It)->Node->GetLink(Name);
			if(L != NULL) {
				return L;
			}
		}
	}
	return NULL;
} // cMeshNode::GetLink

void cMeshNode::GatherMeshes(cList<cMesh *> &List) {
	if(m_pMesh != NULL) {
		List.Add(m_pMesh);
	}
	for(Links::Iterator It = m_Links.GetIterator(); !It.End(); ++It) {
		if((*It)->Node != NULL) {
			(*It)->Node->GatherMeshes(List);
		}
	}
}

//-----------------------------------------------------------------------------
// cMeshNode::Transform
//-----------------------------------------------------------------------------
void cMeshNode::Transform(const cMat4 &ParentTransform) const {
	if(m_pMesh != NULL) {
		m_pMesh->Transform(ParentTransform);
	}
	for(Links::Iterator It = m_Links.GetIterator(); !It.End(); ++It) {
		const cMat4 T = cMat3(cMat3::DiagonalCtor, (*It)->Scale).ToMat4() * (*It)->Rotate.ToMat4() * cMat4::Translation((*It)->Translate);
		cMat4 CurT = cMat4::Mul(ParentTransform, T);
		if((*It)->Node != NULL) {
			(*It)->Node->Transform(CurT);
		}
	}
} // cMeshNode::Transform

//-----------------------------------------------------------------------------
// cMeshNode::LoadMaterials
//-----------------------------------------------------------------------------
void cMeshNode::LoadMaterials() {
	if(m_pMesh != NULL) {
		m_pMesh->GetMaterial().Load();
	}
	for(Links::Iterator It = m_Links.GetIterator(); !It.End(); ++It) {
		if((*It)->Node != NULL) {
			(*It)->Node->LoadMaterials();
		}
	}
} // cMeshNode::LoadMaterials

//-----------------------------------------------------------------------------
// cMeshNode::Render
//-----------------------------------------------------------------------------
void cMeshNode::Render() const {
	if(m_pMesh != NULL) {
		m_pMesh->Render();
	}
	
	for(Links::Iterator It = m_Links.GetIterator(); !It.End(); ++It) {
		if((*It)->Node != NULL) {
			(*It)->Node->Render();
		}
	}
} // cMeshNode::Render

//--------------------------------------------------------------------------------------------------------
// cMeshNode::RenderEdges
//--------------------------------------------------------------------------------------------------------
void cMeshNode::RenderEdges(const cColor &Color /*= cColor::White*/, const float Width /*= 1.0f*/) const {
	if(m_pMesh != NULL) {
		m_pMesh->RenderEdges(Color, Width);
	}

	for(Links::Iterator It = m_Links.GetIterator(); !It.End(); ++It) {
		if((*It)->Node != NULL) {
			(*It)->Node->RenderEdges(Color, Width);
		}
	}
} // cMeshNode::RenderEdges

//-----------------------------------------------------------------------------
// cMeshNode::RenderShadowVolume
//-----------------------------------------------------------------------------
void cMeshNode::RenderShadowVolume(const cVec3 &LightPos) const {
/*	if(m_pMesh != NULL) {
		m_pMesh->RenderShadowVolume(LightPos);
	}

	for(Links::Iterator It = m_Links.GetIterator(); !It.End(); ++It) {
		if((*It)->Node != NULL) {
			(*It)->Node->RenderShadowVolume(LightPos);
		}
	}*/
} // cMeshNode::RenderShadowVolume