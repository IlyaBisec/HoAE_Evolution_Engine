//VertexBasedMesh Functions:
//		void CreateNormals(); //��������� ������� ������ ��� ����� ����� ��������, ���������� ���� nx, ny, nz.
//		(exmp. MESH. CreateNormals();)  
//		void CreateShadow(Vector3D vec, int min, int max); //��������� ����������� ��� ����� ����� ��������. vec - ���� �� ����������� �����(�� ����� �� �����); min, max - ��������� � ����������� ��������� !!nx, ny, nz ������� ���� ���������!!
//		(exmp. MESH.CreateShadow(vec, 20, 200);)
//		void CreateSurface(int Nx,int Ny,float Step,BaseSurfaceFunction* Func); //������� ������� �� ������� �������� (Func - ������� ������� z=F(x,y);) 2*Nx - ������� ���������� �� �� Ox, Ny - ������� ���������� �� �� Oy, Step - ������� ������ � ������ ����������.
//		(exmp. MESH.CreateSurface(32, 32, 40, &Func);)
//		void CreateRandomPatch(int Nx,int Ny,float Step); //������� ��������� ���� (z==0) 2*Nx - ������� ���������� �� �� Ox, Ny - ������� ���������� �� �� Oy, Step - ������� ������ � ������ ����������.
//		(exmp. MESH.CreateRandomPatch(32, 32, 40);)
//		void ModifySurface(int MaxSubDiv,BaseSurfaceFunction* Func); //������� ���������� � ���� ���������� �������(��� �����������) � ����� MaxSubDiv(�����>=0).
//		(exmp. MESH.ModifySurface(3, & Func);) 
//		void Smooth(float Proc, int Smnum); //������ ��������. proc - ����� ����������� (����� ����� �� 0.0 �� 1.0), Snnum - ������� ����������.
//		(exmp. MESH.Smooth(1, 3);) 
//		void Smooth(DynArray<bool>& WhatSm, float Proc, int Smnum); //��������� ����������� �� i - �� �����, ���� WhatSm[i]==true. proc - ����� ����������� (����� ����� �� 0.0 �� 1.0), Snnum - ������� ����������.
//		(exmp. MESH.Smooth(WhatSm, 1, 3);) 
//		void CreateSphere(Vector3D Center,float Radius,DWORD Color,int Facture,int NSubDiv); //������� ����� � ����� ������� - Cebter, ������� - Radiys, �������� - Color, ��������� - Facture � ����� NsubDiv(>=0)
//		(exmp. MESH.CreateSphere(Center, 100, 0&770044, 1, 3);
//VertexBasedMeshRef Functions:
//		void ModifySurface(int MaxSubDiv,BaseSurfaceFunction* Func); //������� ���������� � ���� ���������� ������� �������(��� �����������, �� ���������� ������) � ����� MaxSubDiv(�����>=0).
//		(exmp. Mref.ModifySurface(3, & Func);) 
//		void Smooth(float Proc, int Smnum);  //������ �������  ��������. proc - ����� ����������� (����� ����� �� 0.0 �� 1.0), Snnum - ������� ����������.
//		(exmp. MESH.Smooth(1, 3);) 
//		void VertexList(DynArray<int>& List); //������� ������ ������� ������, �� ������� �   �������  ��������.
//		(exmp. Mref.VertexList(List);)
// struct Triang
//	{	
//		int Finish; //�������� �����
//		int Num; //������� ����� (����������, �� ����� ������ ������ �������)
//		int Ivm[3]; //������� ������� ����� ����������(� ������������� �������)
//		int Iv[3]; //������� ����� ����������
//		int L[3]; //1 - ������� ����� �����, 0 - ��
//		int Fr[3]; //������ ����� � BaseTriang bt
//		int bFr[3]; //1 - ��� �, 0 - ��
//		int Nfr; //������� �����
//		//Ivm[i], L[i], Fr[i], bFr[i] - ����������� �� i-��� �  (i+1)%3 ���������
//	};
//class BaseTriang
//	{	
//	public:
//		DynArray<Triang> Tri;
//		.............................................
//	}
//BaseTriang Functions:
//		void bInit(VertexBasedMesh& MESH); //������� � �������� ����������� ������ ���� BaseTriang bt ��� ������ ��������
//		(exmp. bt.bInit( MESH);)
//		void Init(VertexBasedMeshRef& Mref); //������� � �������� ����������� ������ ���� BaseTriang bt ��� ������ ������� ��������
//		(exmp. bt.bInit( Mref);)
//		int bFindFriends(VertexBasedMesh& MESH,BaseSurfaceFunction* Func); //��������� ����� � �������, �� ����� �����(���� � MESH ������� �������� ����� �� ����� �����)
//		(exmp. bt.bFindFriends(MESH, & Func);)
//		int gFindFriends(); //��������� �����
//		(exmp. bt.gFindFriends();)
//		int bFindFriends(VertexBasedMesh& MESH,DynArray<int>& ChInd,BaseSurfaceFunction* Func); //��������� ����� � ������� ��� ���������� ������� ���� ������� � ChInd, �� ����� �����(���� � MESH ������� �������� ����� �� ����� �����)
//		(exmp. bt.bFindFriends(MESH, ChInd, & Func);)
//		void Partition(DynArray<int>& ChInd); //������� ���������� �� ������������ � ������� ������� ���, ��� ���� ����� ������ ������ �����
//		(exmp. bt.Partition(ChInd);)
//		void Partition(DynArray<int>& InInd, DynArray<int>& ChInd);	 //������� ����������(������� ���� ������� � InInd) �� ������������ � ������� ������� ���, ��� ���� ����� ������ ������ �����
//		(exmp. bt.Partition(ChInd);)
//		int FindFriends(VertexBasedMeshRef& Mref,BaseSurfaceFunction* Func); //������ bFindFriends(VertexBasedMesh& MESH,BaseSurfaceFunction* Func);  ���������� ��������� �������� ����
//		(exmp. bt.FindFriends(Mref, & Func);)
//		int FindFriends(VertexBasedMeshRef& Mref,DynArray<int>& ChInd,BaseSurfaceFunction* Func);//������ bFindFriends(VertexBasedMesh& MESH, DynArray<int>& ChIndBase, SurfaceFunction* Func);  ���������� ��������� �������� ����
//		(exmp. bt.FindFriends(Mref,  ChInd, & Func);)
//		void bReturnIndex(VertexBasedMesh& MESH); //������� ������� � MESH � BaseTriange bt
//		(exmp. bt.bReturnIndex(MESH);)
//		void ReturnIndex(VertexBasedMeshRef& Mref); //������� ������� � Mref � BaseTriange bt 
//		(exmp. bt.bReturnIndex(Mref);)
//BaseSurfaceFunction Function:
//		virtual void ModifyVertex(OneMapVertex& V); //��������� ������� �� �������
//		(exmp. Func.ModifyVertex(V);)
//		virtual bool CheckIfSubDivNeed(const OneMapVertex* V1,const OneMapVertex* V2,OneMapVertex& V); //������� true, ���� ������� (V1, V2) ����� ����� � ������� � V ��������� ������������ ������� �� ��������
//		(exmp. Func.CheckIfSubDivNeed(V1, V2, V);)
//		virtual bool ShiftVertex(OneMapVertex& V); //��������� ������� ����� �� V
//		(exmp. Func.ShiftVertex(V);)
//BaseDirectionalFunction Function:
//		void BaseDirectionalFunction::ReturnMeshRef(UniversalMap& UniMap, VertexBasedMeshRef& Mref, int& ox1, int& ox2, int& oy1, int& oy2); //� UniMap ������ Mref �� ���� ������ ���� �������, ������� �����������(ox1, ox2, oy1, oy2) � ����� ����������� Mref
//		(exmp. Func0.ReturnMeshRef(UniMap, Mref, ox1, ox2, oy1, oy2);)
//		void BaseDirectionalFunction::DeleteDirectionalSurface(VertexBasedMeshRef& Mref); //������� � Mref ������ �������
//		(exmp. Func0.DeleteDirectionalSurface(Mref);)
//UniversalMap Functions:
//		void ModifySurface(int x1, int y1, int x2, int y2, int MaxSubDiv,BaseSurfaceFunction* Func); //������� ���������� �� ����������� � �������� �������(ox1, ox2, oy1, oy2) � ����� MaxSubDiv(�����>=0), ��������� ��������� ���������� m_pQuads.
//		(exmp. MESH.ModifySurface(3, & Func);) 
#include "stdheader.h"
#include "NewSurface\Surface.h"
///---------Without Class Functions------------///
OneMapVertex MidPoint(OneMapVertex v1,OneMapVertex v2)
{
	OneMapVertex v;
	v.x=(v1.x+v2.x)/2;
	v.y=(v1.y+v2.y)/2;
	v.z=(v1.z+v2.z)/2;	
	return v;
}
Vector3D VectMul(Vector3D a, Vector3D b)
{
	Vector3D c;
	c.x=a.y*b.z-a.z*b.y;
	c.y=a.z*b.x-a.x*b.z;
	c.z=a.x*b.y-a.y*b.x;
	return c;
}
OneMapVertex Proection(OneMapVertex a, OneMapVertex b,Vector3D Center,float Radius)
{
	OneMapVertex c;
	float x,y,z;
	float Leng;
	x=(a.x+b.x)/2-Center.x;
	y=(a.y+b.y)/2-Center.y;
	z=(a.z+b.z)/2-Center.z;
	Leng=sqrt(x*x+y*y+z*z);
	c.x=x*Radius/Leng+Center.x;
	c.y=y*Radius/Leng+Center.y;
	c.z=z*Radius/Leng+Center.z;
	return c;
}
float FN1(float x,float y)
{
	float r=sqrt((x-700)*(x-700)+(y-700)*(y-700))-350;
	if(r<0)r=0;	
	float f=150*exp(-r*r/50/50);
	if(f>150)f=150;
	return f;
}
float GetFractV(float x,float y)
{
	int GetFractalVal(int x,int y);
	float f=(float(GetFractalVal(x*3,y*3))/256.0f)-0.5f;
	return f;
}
float TestFunc(Vector3D V)
{
	return FN1(V.x+100*GetFractV(V.x,V.y)/2,V.y+100*GetFractV(V.x+1234,V.y+761)/2);
}
float MovePoint(OneMapVertex& V)
{	
	Vector3D Vec;
	Vec.x=V.x;
	Vec.y=V.y;
	Vec.z=V.z;
	V.z=TestFunc(Vec);
	return fabs(Vec.z-V.z);
}
bool CompereVertex(OneMapVertex& v1, OneMapVertex& v2)
{
	return ((v1.x==v2.x) && (v1.y==v2.y) && (v1.z==v2.z));
}
void DrawDebugText(Vector3D V,char* s,...)
{
	char cc[512];
	va_list va;
	va_start( va, s );
	vsprintf ( cc, s, va );   
	va_end( va );
	int LX=100000;
	int Ly=DrawMultilineText(0,0,s,&SmallWhiteFont,LX,100000,1,false);
	WorldToScreenSpace(V);
	V.x-=LX/2;
	V.y-=LX/2;
	GPS.DrawFillRect(V.x-1,V.y-1,LX+2,Ly+2,0x80000000);
	GPS.DrawLine(V.x-2,V.y+Ly/2,V.x+LX+2,V.y+Ly/2,0x60FF0000);
	GPS.DrawLine(V.x+LX/2,V.y-2,V.x+LX/2,V.y+Ly+2,0x60FF0000);
	GPS.FlushBatches();
	DrawMultilineText(V.x,V.y,cc,&SmallWhiteFont,LX,100000,1,true);
	GPS.FlushBatches();
}
void NvertexOut(VertexBasedMesh& MESH)
{
	for (int i=0; i<MESH.Vert.GetAmount(); i++)
	{
		OneMapVertex& v=MESH.Vert[i];
		Vector3D vec;
		vec.x=v.x;
		vec.y=v.y;
		vec.z=v.z;
		DrawDebugText(vec,"%d",i);
	}
}
void FindXYZ(OneMapVertex& v0,Vector3D& Vec1)
{
	float x,y,z;
	x=((v0.ny*v0.ny+v0.nz*v0.nz)*Vec1.x-v0.nx*(v0.ny*Vec1.y+v0.nz*Vec1.z))/127/127;
	y=((v0.nx*v0.nx+v0.nz*v0.nz)*Vec1.y-v0.ny*(v0.nx*Vec1.x+v0.nz*Vec1.z))/127/127;
	z=((v0.ny*v0.ny+v0.nx*v0.nx)*Vec1.z-v0.nz*(v0.ny*Vec1.y+v0.nx*Vec1.x))/127/127;
	Vec1.x=x;
	Vec1.y=y;
	Vec1.z=z;
}
float FindAngle(OneMapVertex& v0,OneMapVertex& v1,OneMapVertex& v2)
{
	Vector3D Vec1,Vec2;
	Vec1.x=v1.x-v0.x;
	Vec1.y=v1.y-v0.y;
	Vec1.z=v1.z-v0.z;
	Vec2.x=v2.x-v0.x;
	Vec2.y=v2.y-v0.y;
	Vec2.z=v2.z-v0.z;
	FindXYZ(v0,Vec1);
	FindXYZ(v0,Vec2);	
	return Vec1.Angle(Vec2);
}
void AddAngles(OneMapVertex& v0,OneMapVertex& v1,OneMapVertex& v2,float& Angle0,float& Angle1,float& Angle2)
{
	Angle0+=FindAngle(v0,v1,v2);
	Angle1+=FindAngle(v1,v0,v2);	
	Angle2+=FindAngle(v2,v0,v1);
}
void Sorting(DynArray<float>& AngM,DynArray<int>& Smas)
{
	for (int j=0; j<AngM.GetAmount()-1; j++)
		for (int i=0; i<AngM.GetAmount()-1; i++)		
			if (AngM[i]>AngM[i+1])
			{
				swap(AngM[i],AngM[i+1]);
				swap(Smas[i],Smas[i+1]);
			}
}
void Change(int& s,int Ls,int point)
{
	if (abs(s)%Ls>0)
	{
		if (point==-1)
		{
			if (s>0) s=s-s%Ls;
			else s=((s-Ls)/Ls)*Ls;
		}
		else
		{
			if (s<0) s=(s/Ls)*Ls;
			else s=s-s%Ls+Ls;
		}
	}
}
struct Shift
{
	bool init;
	Vector3D dir;
	float min,max;
};
void ChangeSh(DynArray<Shift>& sh, int i0, float& min, float& max, Vector3D minV, Vector3D maxV)
{
	Shift& onesh=sh[i0];
	if (min>max)
	{
		swap(min,max);
		swap(minV,maxV);
	}
	if (onesh.init)
	{		
		if (onesh.max<max) { onesh.max=max; onesh.dir=maxV; }
		if (onesh.min>min) onesh.min=min;
	}
	else
	{		
		onesh.max=max;
		onesh.dir=maxV;
		onesh.min=min;
		onesh.init=true;
	}
}
bool MoveVertex(OneMapVertex& v, OneMapVertex& v0, BaseDirectionalFunction *Func0)
{
	if (Func0->ReturnInfo(0, v)) return true;
	Vector3D vec;
	vec.x=v0.x-v.x;
	vec.y=v0.y-v.y;
	vec.z=v0.z-v.z;	
	float norm=vec.norm();
	vec/=norm;
	OneMapVertex v1=v;
	int i=0;
	while (!Func0->ReturnInfo(0, v1))
	{	
		i++;		
		v1.x=v.x+float(vec.x*i);
		v1.y=v.y+float(vec.y*i);
		v1.z=v.z+float(vec.z*i);
	}
	v=v1;
	return true;
}
Vector3D GetVector(OneMapVertex V)
{
	Vector3D vec;
	vec.x=V.x;
	vec.y=V.y;
	vec.z=V.z;
	return vec;
}
bool NotFr(Triang tri, int fr)
{
	for (int i=0; i<3; i++)
		if (tri.Fr[i]==fr) return false;
	return true;
}
///------------------Structures-------------------///
struct Dir
{
	Vector3D vect;
	int num;
};
struct MasVert
{
	int num;	
	bool CanSm;
	Dir d;
	unsigned int hash() const {return num;}
	bool equal(const MasVert& el){return num==el.num;}
	void copy(const MasVert& el){num=el.num; CanSm=el.CanSm; d=el.d;}
};
struct MasLine
{
	int Iv[2];
	int ind,num;
	unsigned int hash() const {return Iv[0]+100*Iv[1];}
	bool equal(const MasLine& el){return ((Iv[0]==el.Iv[0]) && (Iv[1],el.Iv[1]));}
	void copy(const MasLine& el){Iv[0]=el.Iv[0]; Iv[1]=el.Iv[1]; ind=el.ind; num=el.num;}		
};
DynArray<int> IndexTri;
DynArray<int> LastTri;
const eps=4;
struct NumDir
{
	int num;
	Dir dir;
	bool sm;
};
struct ChTri
{
	int ind;
	int num;
};

///---------VertexBasedMesh Functions-------------///
/*void VertexBasedMesh::CreateShadow(Vector3D vec, int min, int max)
{
	int i, n; 
	float x, a, b;	    
	n=Vert.GetAmount();
	vec.x/=vec.norm();
	vec.y/=vec.norm();
	vec.z/=vec.norm();
	for (i=1; i<n; i++)
	{
		OneMapVertex& v=Vert[i];		
		x=(v.nx*vec.x+v.ny*vec.y+v.nz*vec.z);
		if (x>127*vec.z) 
			v.Shadow=(x-127*vec.z)*(max-128)/127/(1-vec.z )+128;		
		if (x<127*vec.z)
			v.Shadow=(x-127*vec.z)*(128-min)/127/(1+vec.z)+128;		
		if (x==127*vec.z) 
			v.Shadow=128;		
	}
}*/
void VertexBasedMesh::CreateNormals()
{
	int n, i;
	DynArray<Vector3D> Norm;
	DynArray<Vector3D> MidNorm;
	Vector3D OneNorm,a,b;
	n=Vert.GetAmount();
	a.x=0;
	a.y=0;
	a.z=0;
	MidNorm.Add(a,n);
	n=Idxs.GetAmount();
	for (i=0; i<n/3; i++)
	{
		OneMapVertex& v0=Vert[Idxs[3*i]];
		OneMapVertex& v1=Vert[Idxs[3*i+1]];
		OneMapVertex& v2=Vert[Idxs[3*i+2]];
		a.x=v1.x-v0.x;
		a.y=v1.y-v0.y;
		a.z=v1.z-v0.z;
		b.x=v2.x-v0.x;
		b.y=v2.y-v0.y;
		b.z=v2.z-v0.z;
		Norm.Add(VectMul(b,a));
	}
	for (i=0; i<n; i++)
	{
		Vector3D& N=Norm[i/3];
		Vector3D& M=MidNorm[Idxs[i]];
		M.x+=N.x;
		M.y+=N.y;
		M.z+=N.z;
	}
	n=Vert.GetAmount();
	for (i=0; i<n; i++)
	{
		OneMapVertex& v=Vert[i];
		Vector3D& M=MidNorm[i];
		v.nx=-M.x*127/M.norm();	
		v.ny=-M.y*127/M.norm();	
		v.nz=-M.z*127/M.norm();
	}

}

void VertexBasedMesh::CreateSphere(Vector3D Center,float Radius,DWORD Color,int Facture,int NSubDiv)
{
	OneMapVertex VM; 	
	VM.x=Center.x;
	VM.y=Center.y;
	VM.z=Center.z+Radius;
	Vert.Add(VM);

	VM.x=Center.x+0.9428*Radius;
	VM.y=Center.y;
	VM.z=Center.z-Radius/3;
	Vert.Add(VM);

	VM.x=Center.x-0.4714*Radius;
	VM.y=Center.y+0.8165*Radius;
	VM.z=Center.z-Radius/3;
	Vert.Add(VM);

	VM.x=Center.x-0.4714*Radius;
	VM.y=Center.y-0.8165*Radius;
	VM.z=Center.z-Radius/3;
	Vert.Add(VM);

	Idxs.Add(1);
	Idxs.Add(2);
	Idxs.Add(3);

	Idxs.Add(1);
	Idxs.Add(0);
	Idxs.Add(2);

	Idxs.Add(0);
	Idxs.Add(3);
	Idxs.Add(2);

	Idxs.Add(1);
	Idxs.Add(3);
	Idxs.Add(0);

	DWORD i, j, Inum, Vnum, Mnum;
	for (j=0; j<NSubDiv; j++)
	{		
		DynArray<OneMapVertex> Vertex;
		DynArray<DWORD> Index;

		Vnum=Vert.GetAmount();
		Inum=Idxs.GetAmount();

		for (i=0; i<Vnum; i++) Vertex.Add(Vert[i]);
		for (i=0; i<Inum; i++) Index.Add(Idxs[i]);
		for (i=0; i<Inum/3; i++)
		{				
			OneMapVertex v12,v13,v23;
			DWORD& I1=Index[3*i];
			DWORD& I2=Index[3*i+1];
			DWORD& I3=Index[3*i+2];
			OneMapVertex& v1=Vertex[I1];
			OneMapVertex& v2=Vertex[I2];
			OneMapVertex& v3=Vertex[I3];
			v12=Proection(v1,v2,Center,Radius);
			v13=Proection(v1,v3,Center,Radius);
			v23=Proection(v2,v3,Center,Radius);	

			word k12=0,k13=0,k23=0;
			DWORD m12, m13, m23;
			for (DWORD k=0; k<Vert.GetAmount(); k++)
			{
				OneMapVertex& v=Vert[k];
				if ((v12.x==v.x) && (v12.y==v.y) && (v12.z==v.z))
				{
					m12=k;
					k12++;
				}
				if ((v13.x==v.x) && (v13.y==v.y) && (v13.z==v.z))
				{
					m13=k;
					k13++;
				}
				if ((v23.x==v.x) && (v23.y==v.y) && (v23.z==v.z))
				{
					m23=k;
					k23++;
				}
			}
			if (k12==0)
				m12=Vert.Add(v12);				
			if (k13==0)				
				m13=Vert.Add(v13);				
			if (k23==0)
				m23=Vert.Add(v23);				

			Idxs.Add(m12);
			Idxs.Add(m23);
			Idxs.Add(m13);

			Idxs.Add(m12);
			Idxs.Add(I2);
			Idxs.Add(m23);

			Idxs.Add(m13);
			Idxs.Add(m23);
			Idxs.Add(I3);

			Idxs[3*i+1]=m12;
			Idxs[3*i+2]=m13;			
		}
	}
	for (i=0; i<Vert.GetAmount(); i++)
	{
		Vert[i].Color=Color;
		Vert[i].nFacture=Facture;
	}

}
void VertexBasedMesh::CreateSurface(int Nx,int Ny,float Step,BaseSurfaceFunction* Func)
{
//	Release();
	Free();
	int mrand();
	int r3=Step*2/3;
	OneMapVertex v;	
	//Start vertexes	
	v.x=0;
	v.y=0;
	v.z=0;
	Func->ModifyVertex(v);
	Vert.Add(v);
	for (int j=0; j<=Ny-1; j++)
	{			
		v.y=Step*j+Step/2;
		Func->ModifyVertex(v);		
		Vert.Add(v);
	}	
	v.y=Step*Ny;
	Func->ModifyVertex(v);	
	Vert.Add(v);		
	for (int i=1; i<Nx; i++)
	{				
		if (i&1)
		{						
			v.x=Step*i;
			v.y=0;
			Func->ModifyVertex(v);			
			Vert.Add(v);
			for (int j=1; j<Ny; j++)
			{					
				v.x=Step*i+rand()%r3-r3/2;
				v.y=Step*j+rand()%r3-r3/2;
				Func->ModifyVertex(v);				
				Vert.Add(v);
			}
			v.x=Step*i;
			v.y=Step*Ny;
			Func->ModifyVertex(v);
			Vert.Add(v);
		}
		else
		{
			v.x=Step*i;
			v.y=0;
			Func->ModifyVertex(v);
			Vert.Add(v);
			for (int j=0; j<=Ny-1; j++)
			{			
				v.x=Step*i+rand()%r3-r3/2;
				v.y=Step*j+Step/2+rand()%r3-r3/2;
				Func->ModifyVertex(v);
				Vert.Add(v);
			}
			v.x=Step*i;
			v.y=Step*Ny;
			Func->ModifyVertex(v);
			Vert.Add(v);		
		}				
	}	
	if (Nx&1)
	{		
		for (int j=0; j<=Ny; j++)
		{			
			v.x=Step*Nx;
			v.y=Step*j;
			Func->ModifyVertex(v);
			Vert.Add(v);
		}		
	}
	else
	{
		v.x=Step*Nx;
		v.y=0;
		Func->ModifyVertex(v);
		Vert.Add(v);
		for (int j=0; j<=Ny-1; j++)
		{			
			v.y=Step*j+Step/2;
			Func->ModifyVertex(v);
			Vert.Add(v);
		}		
		v.y=Step*Ny;
		Func->ModifyVertex(v);
		Vert.Add(v);		
	}				
	//Start indexes		
	for (int i=0; i<Nx; i++)
	{			
		int ind=i*(Ny+2)-i/2;
		if (i&1)
		{			
			Idxs.Add(ind);			
			Idxs.Add(ind+Ny+1);
			Idxs.Add(ind+Ny+2);
			for (int j=0; j<Ny; j++)
			{					
				Idxs.Add(j+ind);				
				Idxs.Add(j+ind+Ny+2);
				Idxs.Add(j+ind+1);
			}
			for (int j=0; j<Ny-1; j++)
			{	
				Idxs.Add(j+ind+Ny+2);				
				Idxs.Add(j+ind+Ny+3);
				Idxs.Add(j+ind+1);
			}			
			Idxs.Add(ind+Ny);			
			Idxs.Add(ind+2*Ny+1);
			Idxs.Add(ind+2*Ny+2);
		}
		else
		{			
			Idxs.Add(ind);			
			Idxs.Add(ind+Ny+2);
			Idxs.Add(ind+1);
			for (int j=0; j<Ny; j++)
			{					
				Idxs.Add(j+ind+1);				
				Idxs.Add(j+ind+Ny+2);
				Idxs.Add(j+ind+Ny+3);
			}
			for (int j=0; j<Ny-1; j++)
			{	
				Idxs.Add(j+ind+Ny+3);				
				Idxs.Add(j+ind+2);
				Idxs.Add(j+ind+1);
			}			
			Idxs.Add(ind+Ny);			
			Idxs.Add(ind+2*Ny+2);
			Idxs.Add(ind+Ny+1);
		}				
	}
	// Inverting indices order to provide anticlockwise direction
	// in left coordinate system (which is used in "GSC engine"):
	int NTri = Idxs.NValues / 3;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		int i1 = 3 * nTri + 1, i2 = i1 + 1;
		int t = Idxs[i1];
		Idxs[i1] = Idxs[i2];
		Idxs[i2] = t;
	}
}
void VertexBasedMesh::ModifySurface(int MaxSubDiv, BaseSurfaceFunction* Func)
{
	BaseTriang t;			
	t.bInit(*this);
	int I=t.bFindFriends(*this,Func);
	DynArray<int> ChInd;
	t.Partition(ChInd);	
	int exit=0;
	while((I>0) && (exit<MaxSubDiv))
	{	
		I=t.bFindFriends(*this,ChInd,Func);
		DynArray<int> InInd; 
		for (int i=0; i<ChInd.GetAmount(); i++) InInd.Add(ChInd[i]);
		ChInd.Clear();		
		t.Partition(InInd,ChInd);		
		exit++;
	}	
	t.bReturnIndex(*this);
	for(int i=0;i<Vert.GetAmount();i++)
	{
		Func->ShiftVertex(Vert[i]);
	}
}
void VertexBasedMesh::CreateRandomPatch(int Nx,int Ny,float Step)
{
	int mrand();
	int r3=Step*2/3;
	OneMapVertex v;	
	//Start vertexes
	v.x=0;
	v.y=0;
	v.z=0;	
	Vert.Add(v);
	for (int j=0; j<=Ny-1; j++)
	{			
		v.y=Step*j+Step/2;			
		Vert.Add(v);
	}	
	v.y=Step*Ny;	
	Vert.Add(v);		
	for (int i=1; i<Nx; i++)
	{				
		if (i&1)
		{
			v.x=Step*i;
			v.y=0;					
			Vert.Add(v);
			for (int j=1; j<Ny; j++)
			{			
				v.x=Step*i+rand()%r3-r3/2;
				v.y=Step*j+rand()%r3-r3/2;							
				Vert.Add(v);
			}
			v.x=Step*i;
			v.y=Step*Ny;
			Vert.Add(v);
		}
		else
		{
			v.x=Step*i;
			v.y=0;			
			Vert.Add(v);
			for (int j=0; j<=Ny-1; j++)
			{			
				v.x=Step*i+rand()%r3-r3/2;
				v.y=Step*j+Step/2+rand()%r3-r3/2;							
				Vert.Add(v);
			}
			v.x=Step*i;
			v.y=Step*Ny;				
			Vert.Add(v);		
		}				
	}	
	if (Nx&1)
	{		
		for (int j=0; j<=Ny; j++)
		{			
			v.x=Step*Nx;
			v.y=Step*j;					
			Vert.Add(v);
		}		
	}
	else
	{
		v.x=Step*Nx;
		v.y=0;		
		Vert.Add(v);
		for (int j=0; j<=Ny-1; j++)
		{			
			v.y=Step*j+Step/2;			
			Vert.Add(v);
		}		
		v.y=Step*Ny;		
		Vert.Add(v);		
	}				
	//Start indexes		
	for (int i=0; i<Nx; i++)
	{			
		int ind=i*(Ny+2)-i/2;
		if (i&1)
		{			
			Idxs.Add(ind);
			Idxs.Add(ind+Ny+2);
			Idxs.Add(ind+Ny+1);
			for (int j=0; j<Ny; j++)
			{					
				Idxs.Add(j+ind);
				Idxs.Add(j+ind+1);
				Idxs.Add(j+ind+Ny+2);
			}
			for (int j=0; j<Ny-1; j++)
			{	
				Idxs.Add(j+ind+Ny+2);
				Idxs.Add(j+ind+1);
				Idxs.Add(j+ind+Ny+3);				
			}			
			Idxs.Add(ind+Ny);
			Idxs.Add(ind+2*Ny+2);
			Idxs.Add(ind+2*Ny+1);
		}
		else
		{			
			Idxs.Add(ind);
			Idxs.Add(ind+1);
			Idxs.Add(ind+Ny+2);
			for (int j=0; j<Ny; j++)
			{					
				Idxs.Add(j+ind+1);
				Idxs.Add(j+ind+Ny+3);
				Idxs.Add(j+ind+Ny+2);
			}
			for (int j=0; j<Ny-1; j++)
			{	
				Idxs.Add(j+ind+Ny+3);
				Idxs.Add(j+ind+1);
				Idxs.Add(j+ind+2);
			}			
			Idxs.Add(ind+Ny);
			Idxs.Add(ind+Ny+1);
			Idxs.Add(ind+2*Ny+2);
		}				
	}	
}
void VertexBasedMesh::Smooth(float Proc, int Smnum)
{
	int Nind=Idxs.GetAmount();
	int Nver=Vert.GetAmount();
	DynArray<bool> WhatSm;
	DynArray<Dir> dir;
	Dir OneDir;
	OneDir.vect.x=0;
	OneDir.vect.y=0;
	OneDir.vect.z=0;
	OneDir.num=0;
	for (int i=0; i<Nver; i++)			
		WhatSm.Add(true);	
	BaseTriang bt;
	bt.bInit(*this);
	bt.gFindFriends();
	for (int i=0; i<Nind/3; i++)
	{
		Triang& tri=bt.Tri[i];		
		int k=tri.Nfr;
		if (k<3)
		{
			if (k<2)
			{
				WhatSm[tri.Iv[0]]=false;
				WhatSm[tri.Iv[1]]=false;
				WhatSm[tri.Iv[2]]=false;
			}
			else
				for (int ii=0; ii<3; ii++)
					if (tri.bFr[ii]==0)
					{
						WhatSm[tri.Iv[ii]]=false;
						WhatSm[tri.Iv[(ii+1)%3]]=false;
					}
		}
	}
	for (int i=0; i<Smnum; i++)
	{
		dir.Clear();
		for (int i=0; i<Nver; i++)	
			dir.Add(OneDir);
		for (int i=0; i<Nind/3; i++)
		{
			int i0=Idxs[3*i];
			int i1=Idxs[3*i+1];
			int i2=Idxs[3*i+2];
			OneMapVertex& v0=Vert[i0];
			OneMapVertex& v1=Vert[i1];
			OneMapVertex& v2=Vert[i2];		
			dir[i0].vect.x+=v1.x+v2.x;
			dir[i0].vect.y+=v1.y+v2.y;
			dir[i0].vect.z+=v1.z+v2.z;
			dir[i0].num++;
			dir[i1].vect.x+=v0.x+v2.x;
			dir[i1].vect.y+=v0.y+v2.y;
			dir[i1].vect.z+=v0.z+v2.z;
			dir[i1].num++;
			dir[i2].vect.x+=v1.x+v0.x;
			dir[i2].vect.y+=v1.y+v0.y;
			dir[i2].vect.z+=v1.z+v0.z;
			dir[i2].num++;
		}
		for (int i=0; i<Nver; i++)
			if (WhatSm[i])
			{
				OneMapVertex& V=Vert[i];
				Vector3D vec=dir[i].vect;
				vec/=(2*dir[i].num);
				V.x=(1-Proc)*V.x+Proc*vec.x;
				V.y=(1-Proc)*V.y+Proc*vec.y;
				V.z=(1-Proc)*V.z+Proc*vec.z;
			}
	}
}
void VertexBasedMesh::Smooth(DynArray<bool>& VertIsIn, float Proc, int Smnum)
{
	int Nind=Idxs.GetAmount();
	int Nver=Vert.GetAmount();	
	DynArray<Dir> dir;
	Dir OneDir;
	OneDir.vect.x=0;
	OneDir.vect.y=0;
	OneDir.vect.z=0;
	OneDir.num=0;	
	BaseTriang bt;
	bt.bInit(*this);
	bt.gFindFriends();	
	for (int j=0; j<Smnum; j++)
	{
		dir.Clear();
		for (int i=0; i<Nver; i++)	
			dir.Add(OneDir);
		for (int i=0; i<Nind/3; i++)
		{
			int i0=Idxs[3*i];
			int i1=Idxs[3*i+1];
			int i2=Idxs[3*i+2];
			OneMapVertex v0=Vert[i0];
			OneMapVertex v1=Vert[i1];
			OneMapVertex v2=Vert[i2];		
			dir[i0].vect.x+=v1.x+v2.x;
			dir[i0].vect.y+=v1.y+v2.y;
			dir[i0].vect.z+=v1.z+v2.z;
			dir[i0].num++;
			dir[i1].vect.x+=v0.x+v2.x;
			dir[i1].vect.y+=v0.y+v2.y;
			dir[i1].vect.z+=v0.z+v2.z;
			dir[i1].num++;
			dir[i2].vect.x+=v1.x+v0.x;
			dir[i2].vect.y+=v1.y+v0.y;
			dir[i2].vect.z+=v1.z+v0.z;
			dir[i2].num++;
		}
		for (int i=0; i<Nver; i++)
			if (VertIsIn[i])
			{
				OneMapVertex& V=Vert[i];
				Vector3D vec=dir[i].vect;
				vec/=(2*dir[i].num);
				V.x=(1-Proc)*V.x+Proc*vec.x;
				V.y=(1-Proc)*V.y+Proc*vec.y;
				V.z=(1-Proc)*V.z+Proc*vec.z;
			}
	}
}
///---------VertexBasedMeshRef Function-----------///
void VertexBasedMeshRef::ReturnVertrex(DynArray<VecPr>& MasOut)
{
	DynArray<float> Angle;
	pOrigin->CreateNormals();
	VecPr v;
	int Nver=pOrigin->Vert.GetAmount();
	for (int i=0; i<Nver; i++)
	{
		LastTri.Add(-1);
		Angle.Add(0);
	}
	for (int i=0; i<nTriangles.GetAmount(); i++)
	{	
		int i0=pOrigin->Idxs[3*i];
		int i1=pOrigin->Idxs[3*i+1];
		int i2=pOrigin->Idxs[3*i+2];
		OneMapVertex v0=pOrigin->Vert[i0];
		OneMapVertex v1=pOrigin->Vert[i1];
		OneMapVertex v2=pOrigin->Vert[i2];
		IndexTri.Add(i);
		IndexTri.Add(LastTri[i0]);
		LastTri[i0]=IndexTri.GetAmount()-2;
		IndexTri.Add(i);
		IndexTri.Add(LastTri[i1]);
		LastTri[i1]=IndexTri.GetAmount()-2;
		IndexTri.Add(i);
		IndexTri.Add(LastTri[i2]);
		LastTri[i2]=IndexTri.GetAmount()-2;
		AddAngles(v0,v1,v2,Angle[i0],Angle[i1],Angle[i2]);
	}
	for (int i=0; i<Nver; i++)		
	{
		v.ind=i;
		v.ang=Angle[i];		
		MasOut.Add(v);
	}
}
void VertexBasedMeshRef::Triangulation(DynArray<int>& MasIn)
{
	VecPr v;
	int minX,minY,maxX,maxY,dx,dy,nx,ny;		
	DynArray<int> PosL;
	DynArray<int> Tlist;
	DynArray<VecPr> MasTri;
	ReturnVertrex(MasTri);
	OneMapVertex& V=pOrigin->Vert[MasTri[0].ind];
	minX=V.x;
	minY=V.y;
	maxX=V.x;
	maxY=V.y;
	int Nnew=MasIn.GetAmount();
	v.ang=0;
	for (int i=0; i<Nnew; i++)
	{
		v.ind=MasIn[i];
		MasTri.Add(v);
	}	
	int Ntri=MasTri.GetAmount();
	for (int i=1; i<Ntri; i++)
	{
		OneMapVertex& v=pOrigin->Vert[MasTri[i].ind];
		if (v.x<minX) minX=v.x;
		if (v.y<minY) minY=v.y;
		if (v.x>maxX) maxX=v.x;
		if (v.y>maxY) maxY=v.y;
	}	
	minX=minX-minX%StepTri;
	minY=minY-minY%StepTri;
	maxX=maxX-maxX%StepTri+StepTri;
	maxY=maxY-maxY%StepTri+StepTri;
	dx=maxX-minX;
	dy=maxY-minY;
	nx=(maxX-minX)/StepTri;
	ny=(maxY-minY)/StepTri;
	for (i=0; i<nx*ny; i++)
		PosL.Add(-1);	
	for (int i=0; i<Ntri; i++)
	{
		OneMapVertex& V=pOrigin->Vert[MasTri[i].ind];		
		int ind=(V.x-minX)/StepTri+(V.y-minY)/StepTri*dx;
		Tlist.Add(i);
		Tlist.Add(PosL[ind]);
		PosL[ind]=Tlist.GetAmount()-2;
	}
	for (int i=0; i<Ntri; i++)
	{
		v=MasTri[i];
		if (v.ang<6.2831)
		{
			OneMapVertex& V=pOrigin->Vert[v.ind];
			int tx=(V.x-minX)/StepTri;
			int ty=(V.y-minY)/StepTri;
			int ind;
			DynArray<int> Smas;
			DynArray<float> AngM;
			int num=0;
			int okil=0;
			do
			{
				for(int i0=tx-okil; i0<=tx+okil; i0++)
				{
					ind=i0+(ty-okil)*dx;
					ind=PosL[ind];
					while(ind>=0)
					{
						Smas.Add(Tlist[ind]);
						ind=PosL[ind+1];
						num++;
					}
					ind=i0+(ty+okil)*dx;
					ind=PosL[ind];
					while(ind>=0)
					{
						Smas.Add(Tlist[ind]);
						ind=PosL[ind+1];
						num++;
					}
				}
				for(int i0=ty-okil+1; i0<=ty+okil-1; i0++)
				{
					ind=tx-okil+i0*dx;
					ind=PosL[ind];
					while(ind>=0)
					{
						Smas.Add(Tlist[ind]);
						ind=PosL[ind+1];
						num++;
					}
					ind=tx+okil+i0*dx;
					ind=PosL[ind];
					while(ind>=0)
					{
						Smas.Add(Tlist[ind]);
						ind=PosL[ind+1];
						num++;
					}
				}					
			}while (num<10);
			for (int i0=0; i0<Smas.GetAmount(); i0++)
			{
				DynArray<int> Smas1;
				Vector3D vec;
				OneMapVertex& V1=pOrigin->Vert[Smas[i0]];
				vec.x=V1.x-V.x;
				vec.y=V1.y-V.y;
				vec.z=V1.z-V.z;
				for (int ii=0; ii<Smas.GetAmount(); ii++)
				{
					OneMapVertex& V2=pOrigin->Vert[Smas[ii]];					
					if ((V1.x*vec.x+V1.y*vec.y+V1.z*vec.z)*(V2.x*vec.x+V2.y*vec.y+V2.z*vec.z)>0) Smas1.Add(ii);
				}			
				Smas.Clear();
				for (ii=0; ii<Smas1.GetAmount(); ii++)
					Smas.Add(Smas[ii]);
			}
			for (int i0=0; i0<Smas.GetAmount(); i0++)
			{
				DynArray<int> Smas1;
				Vector3D vec;
				OneMapVertex& V=pOrigin->Vert[Smas[i0]];				
				int ind=LastTri[i0];
				while(ind>=0)
				{
					int index=IndexTri[ind];
					int ind[3];
					ind[0]=3*index;
					ind[1]=3*index+1;
					ind[2]=3*index+2;
					for (int ii=0; ii<3; ii++)
						if (i0=ind[ii])
						{							
							OneMapVertex& V1=pOrigin->Vert[(i0+1)%3];							
							Vector3D vec;
							vec.x=V1.x-V.x;
							vec.y=V1.y-V.y;
							vec.z=V1.z-V.z;
							FindXYZ(V,vec);
							for (int ii=0; ii<Smas.GetAmount(); ii++)
							{
								OneMapVertex& V2=pOrigin->Vert[Smas[ii]];					
								if ((V1.x*vec.x+V1.y*vec.y+V1.z*vec.z)*(V2.x*vec.x+V2.y*vec.y+V2.z*vec.z)>0) Smas1.Add(ii);
							}		
						}
				}
				Smas.Clear();
				for (int ii=0; ii<Smas1.GetAmount(); ii++)
					Smas.Add(Smas[ii]);
			}
			OneMapVertex& V1=pOrigin->Vert[Smas[0]];
			Vector3D v0;
			v0.x=V1.x-V.x;
			v0.y=V1.y-V.y;
			v0.z=V1.z-V.z;
			AngM.Add(0);
			for (int i0=1; i0<Smas.GetAmount(); i0++)
			{
				OneMapVertex& V1=pOrigin->Vert[Smas[0]];
				Vector3D v1,n;
				n.x=V.nx;
				n.y=V.ny;
				n.z=V.nz;
				v1.x=V1.x-V.x;
				v1.y=V1.y-V.y;
				v1.z=V1.z-V.z;				
				AngM.Add(v1.Angle(v0,n));
			}
			Sorting(AngM,Smas);
		}
	}
}
void VertexBasedMeshRef::ModifySurface(int MaxSubDiv, BaseSurfaceFunction* Func)
{
	BaseTriang t;	
	t.Init(*this);
	int I=t.FindFriends(*this,Func);
	DynArray<int> ChInd;
	t.Partition(ChInd);	
	int exit=0;
	while((I>0) && (exit<MaxSubDiv))
	{	
		I=t.FindFriends(*this,ChInd,Func);
		DynArray<int> InInd; 
		for (int i=0; i<ChInd.GetAmount(); i++) InInd.Add(ChInd[i]);
		ChInd.Clear();		
		t.Partition(InInd,ChInd);		
		exit++;
	}	
	t.ReturnIndex(*this);	
}
void VertexBasedMeshRef::Smooth(float Proc, int Smnum)
{
	BaseTriang b;
	b.Init(*this);
	b.gFindFriends();
	for (int ii=0; ii<Smnum; ii++)
	{
		Hash<MasVert> Temp;		
		DynArray<NumDir> WhatSm;
		NumDir nd;
		MasVert mv;		
		mv.CanSm=true;
		mv.d.num=1;		
		int Ntri=b.Tri.GetAmount();
		for (int i=0; i<Ntri; i++)
		{
			Triang& tri=b.Tri[i];
			int i0=tri.Iv[0];
			int i1=tri.Iv[1];
			int i2=tri.Iv[2];
			OneMapVertex& v0=this->pOrigin->Vert[i0];
			OneMapVertex& v1=this->pOrigin->Vert[i1];
			OneMapVertex& v2=this->pOrigin->Vert[i2];			
			mv.num=i0;
			int j=Temp.find(mv);
			if (j>-1)
			{
				Dir& dir=Temp.elem(j).d;
				dir.vect.x+=(v1.x+v2.x);
				dir.vect.y+=(v1.y+v2.y);
				dir.vect.z+=(v1.z+v2.z);
				dir.num++;				
			}
			else
			{
				mv.d.vect.x=v1.x+v2.x;
				mv.d.vect.y=v1.y+v2.y;
				mv.d.vect.z=v1.z+v2.z;
				mv.d.num=1;
				Temp.add(mv);							
			}
			mv.num=i1;
			j=Temp.find(mv);
			if (j>-1)
			{
				Dir& dir=Temp.elem(j).d;
				dir.vect.x+=(v0.x+v2.x);
				dir.vect.y+=(v0.y+v2.y);
				dir.vect.z+=(v0.z+v2.z);
				dir.num++;				
			}
			else
			{
				mv.d.vect.x=v0.x+v2.x;
				mv.d.vect.y=v0.y+v2.y;
				mv.d.vect.z=v0.z+v2.z;
				mv.d.num=1;
				Temp.add(mv);			
			}
			mv.num=i2;
			j=Temp.find(mv);
			if (j>-1)
			{
				Dir& dir=Temp.elem(j).d;
				dir.vect.x+=(v1.x+v0.x);
				dir.vect.y+=(v1.y+v0.y);
				dir.vect.z+=(v1.z+v0.z);
				dir.num++;				
			}
			else
			{
				mv.d.vect.x=v1.x+v0.x;
				mv.d.vect.y=v1.y+v0.y;
				mv.d.vect.z=v1.z+v0.z;
				mv.d.num=1;
				Temp.add(mv);								
			} 
		}
		mv.CanSm=false;
		mv.d.vect.x=0;
		mv.d.vect.y=0;
		mv.d.vect.z=0;
		mv.d.num=0;
		for (int i=0; i<Ntri; i++)
		{
			Triang& tri=b.Tri[i];			
			if (tri.Nfr==2)
				for (int i0=0; i0<3; i0++)
					if (tri.bFr[i0]==0)
					{
						mv.num=tri.Iv[i0];
						int j=Temp.find(mv);
						Temp.elem(j).CanSm=false;						
						mv.num=tri.Iv[(i0+1)%3];
						j=Temp.find(mv);
						Temp.elem(j).CanSm=false;												
					}
			if (tri.Nfr<2)
			{
				mv.num=tri.Iv[0];
				int j=Temp.find(mv);
				Temp.elem(j).CanSm=false;
				mv.num=tri.Iv[1];
				j=Temp.find(mv);
				Temp.elem(j).CanSm=false;
				mv.num=tri.Iv[2];
				j=Temp.find(mv);
				Temp.elem(j).CanSm=false;				
			}
		}	
		for (int i=0; i<Ntri; i++)
		{
			Triang& tri=b.Tri[i];
			int i0=tri.Iv[0];
			int i1=tri.Iv[1];
			int i2=tri.Iv[2];
			mv.num=i0;
			int j=Temp.find(mv);
			assert(j>-1);
			MasVert& mv0=Temp.elem(j);
			if (mv0.CanSm) 
			{				
				nd.num=mv0.num;
				nd.dir=mv0.d;
				WhatSm.Add(nd); 
				mv0.CanSm=false;
			}
			mv.num=i1;
			j=Temp.find(mv);
			assert(j>-1);
			MasVert& mv1=Temp.elem(j);
			if (mv1.CanSm) 
			{				
				nd.num=mv1.num;
				nd.dir=mv1.d;
				WhatSm.Add(nd); 
				mv1.CanSm=false;
			}
			mv.num=i2;
			j=Temp.find(mv);
			assert(j>-1);
			MasVert& mv2=Temp.elem(j);
			if (mv2.CanSm) 
			{				
				nd.num=mv2.num;
				nd.dir=mv2.d;
				WhatSm.Add(nd); 
				mv2.CanSm=false;
			}
		}
		int Nver=WhatSm.GetAmount();
		for (int i0=0; i0<Nver; i0++)		
		{
			i=WhatSm[i0].num;
			Dir dir=WhatSm[i0].dir;
			OneMapVertex& V=pOrigin->Vert[i];
			Vector3D vec=dir.vect;
			vec/=(2*dir.num);
			V.x=(1-Proc)*V.x+Proc*vec.x;
			V.y=(1-Proc)*V.y+Proc*vec.y;
			V.z=(1-Proc)*V.z+Proc*vec.z;			
		}	
	}
}
void VertexBasedMeshRef::ChangeIndex(Hash<boolh>& WhatDel, int j1, int i1)
{
	int j=pOrigin->Idxs[j1];
	int i=pOrigin->Idxs[i1];
	int Ntri=nTriangles.GetAmount();
	for (int ii=0; ii<Ntri; ii++)
	{
		int i0=nTriangles[ii];
		int k0=3*i0;
		int k1=3*i0+1;
		int k2=3*i0+2;
		DWORD& ind0=pOrigin->Idxs[k0];
		if (ind0==j)
			ind0=i;
		DWORD& ind1=pOrigin->Idxs[k1];
		if (ind1==j)
			ind1=i;
		DWORD& ind2=pOrigin->Idxs[k2];
		if (ind2==j)
			ind2=i;		
	}
	for (int ii=0; ii<Ntri; ii++)
	{
		int i0=nTriangles[ii];
		int k0=3*i0;
		int k1=3*i0+1;
		int k2=3*i0+2;
		DWORD& ind0=pOrigin->Idxs[k0];		
		DWORD& ind1=pOrigin->Idxs[k1];		
		DWORD& ind2=pOrigin->Idxs[k2];		
		if (ind0==ind1 || ind0==ind2 || ind2==ind1)
		{			
			boolh bh;			
			bh.ind=ii;
			WhatDel.add(bh);
		}
	}
}
void VertexBasedMeshRef::VertexList(DynArray<int>& List)
{
	Hash<MasVert> Temp;
	MasVert mv;	
	int Ntri=nTriangles.GetAmount();
	for (int ii=0; ii<Ntri; ii++)
	{
		int i=nTriangles[ii];
		int i0=pOrigin->Idxs[3*i];
		int i1=pOrigin->Idxs[3*i+1];
		int i2=pOrigin->Idxs[3*i+2];		
		mv.num=i0;
		int j=Temp.find(mv);
		if (j==-1)
		{	
			Temp.add(mv);
			List.Add(i0);
		}
		mv.num=i1;
		j=Temp.find(mv);
		if (j==-1)
		{	
			Temp.add(mv);
			List.Add(i1);
		}
		mv.num=i2;
		j=Temp.find(mv);
		if (j==-1)
		{	
			Temp.add(mv);
			List.Add(i2);
		}
	}
}
void VertexBasedMeshRef::RecursDeleting(Hash<boolh>& WhatDel, int ii, int jj, int k)
{	
	boolh bh;
	int i=nTriangles[ii];
	bh.ind=i;
	WhatDel.add(bh);
	int j=nTriangles[jj];	
	bh.ind=j;
	WhatDel.add(bh);
	int i1=3*i;
	int j1=3*j+k;
	ChangeIndex(WhatDel, j1, i1);
	i1++;
	j1=3*j+(k+2)%3;
	ChangeIndex(WhatDel, j1, i1);
	i1++;
	j1=3*j+(k+1)%3;
	ChangeIndex(WhatDel, j1, i1);	
	/*pOrigin->Idxs[3*i]=0;
	pOrigin->Idxs[3*i+1]=0;
	pOrigin->Idxs[3*i+2]=0;
	pOrigin->Idxs[3*j]=0;
	pOrigin->Idxs[3*j+1]=0;
	pOrigin->Idxs[3*j+2]=0;*/
}
bool VertexBasedMeshRef::IfDelTri(int ii, int jj, int& k)
{
	int i=nTriangles[ii];
	int j=nTriangles[jj];
	int i0=3*i;
	int i1=3*i+1;
	int i2=3*i+2;
	if (i0==i1 || i0==i2 || i2==i1) return false;
	OneMapVertex& v0=pOrigin->Vert[pOrigin->Idxs[i0]];
	OneMapVertex& v1=pOrigin->Vert[pOrigin->Idxs[i1]];
	OneMapVertex& v2=pOrigin->Vert[pOrigin->Idxs[i2]];
	Vector3D a, b, cen, cen1;
	a.x=v1.x-v0.x;
	a.y=v1.y-v0.y;
	a.z=v1.z-v0.z;
	b.x=v2.x-v0.x;
	b.y=v2.y-v0.y;
	b.z=v2.z-v0.z;
	Vector3D nor=VectMul(a, b);
	float nr=nor.norm();
	if (nr!=0) nor/=nr;
	else return false;
	cen.x=v0.x+v1.x+v2.x;
	cen.y=v0.y+v1.y+v2.y;
	cen.z=v0.z+v1.z+v2.z;	
	int j0=3*j;
	int j1=3*j+1;
	int j2=3*j+2;
	if (j0==j1 || j0==j2 || j2==j1) return false;
	OneMapVertex& sv0=pOrigin->Vert[pOrigin->Idxs[j0]];
	OneMapVertex& sv1=pOrigin->Vert[pOrigin->Idxs[j1]];
	OneMapVertex& sv2=pOrigin->Vert[pOrigin->Idxs[j2]];				
	a.x=sv1.x-sv0.x;
	a.y=sv1.y-sv0.y;
	a.z=sv1.z-sv0.z;
	b.x=sv2.x-sv0.x;
	b.y=sv2.y-sv0.y;
	b.z=sv2.z-sv0.z;
	Vector3D nor1=VectMul(a, b);
	nr=nor1.norm();
	if (nr!=0) nor1/=nr;	
	else return false;
	cen1.x=sv0.x+sv1.x+sv2.x;
	cen1.y=sv0.y+sv1.y+sv2.y;
	cen1.z=sv0.z+sv1.z+sv2.z;
	Vector3D Len=cen;
	Len-=cen1;	
	if (nor.x*nor1.x+nor.y*nor1.y+nor.z*nor1.z<-0.8 && Len.norm()<4)
	{							
		Len=GetVector(v0);
		Vector3D vec0=GetVector(sv0);
		Vector3D vec1=GetVector(sv1);
		Vector3D vec2=GetVector(sv2);
		k=0;
		float min=Len.distance(vec0);
		float temp;
		temp=Len.distance(vec1);
		if (temp<min)
		{
			min=temp;
			k=1;
		}
		temp=Len.distance(vec2);
		if (temp<min)
		{
			min=temp;
			k=2;
		}		
		return true;
	}
	return false;
}
void VertexBasedMeshRef::DeleteFlattening()
{
	DynArray<bool> CanDel;
	Hash<boolh> WhatDel;
	boolh bh;
	int Ntri=nTriangles.GetAmount();
	BaseTriang bt;
	bt.Init(*this);
	int out=bt.gFindFriends();
	for (int i=0; i<Ntri; i++)
	{
		Triang& tri=bt.Tri[i];
		bool b=(tri.Nfr==3);
		for (int i0=0; i0<3; i0++)		
		{
			Triang& tri1=bt.Tri[tri.Fr[i0]];
			b=(b && tri1.Nfr==3);
		}
		CanDel.Add(b);
	}	
	for (int ii=0; ii<Ntri; ii++)			
		for (int jj=0; jj<Ntri; jj++)
		{
			int k;			
			if (ii!=jj && CanDel[ii] && CanDel[jj])
				if (IfDelTri(ii, jj, k))
				{
					RecursDeleting(WhatDel, ii, jj, k);					
				}
		}	
	for (int i=0; i<Ntri; i++)
	{
		int ii=nTriangles[i];
		bh.ind=ii;
		int k0=3*ii;
		int k1=3*ii+1;
		int k2=3*ii+2;
		if (WhatDel.find(bh)!=-1)
		{
			DWORD& ind0=pOrigin->Idxs[k0];
			DWORD& ind1=pOrigin->Idxs[k1];
			DWORD& ind2=pOrigin->Idxs[k2];
			ind0=0;
			ind1=0;
			ind2=0;
		}
	}
	/*int iii=0, exit;
	do
	{
		iii++;
		exit=0;
		bt.Init(*this);
		bt.gFindFriends();
		for (int i=0; i<Ntri; i++)
		{
			Triang tri=bt.Tri[i];
			if (CanDel[i] && tri.Nfr<2)
			{
				int ii=nTriangles[i];				
				int k0=3*ii;
				int k1=3*ii+1;
				int k2=3*ii+2;			
				DWORD& ind0=pOrigin->Idxs[k0];
				DWORD& ind1=pOrigin->Idxs[k1];
				DWORD& ind2=pOrigin->Idxs[k2];
				ind0=0;
				ind1=0;
				ind2=0;
				exit++;
			}
		}
	} while (exit>0 && iii<9);*/
}
///----------BaseTriang-Functions-----------------///
void BaseTriang::bReturnIndex(VertexBasedMesh& MESH)
{
	MESH.Idxs.Clear();
	for (int i=0; i<Tri.GetAmount(); i++)
	{
		Triang& t=Tri[i];
		for(int i0=0; i0<3; i0++)		
			MESH.Idxs.Add(t.Iv[i0]);
	}
}
void BaseTriang::ReturnIndex(VertexBasedMeshRef& Mref)
{
	int Mnum=Mref.nTriangles.GetAmount();
	int Mtri=Tri.GetAmount();
	for (int i=0; i<Mnum; i++)
	{
		Triang& t=Tri[i];
		int ind=Mref.nTriangles[i];
		for (int i0=0; i0<3; i0++)		
			Mref.pOrigin->Idxs[3*ind+i0]=t.Iv[i0];		
	}
	for (int i=Mnum; i<Mtri; i++)
	{
		Triang& t=Tri[i];			
		int ind=Mref.pOrigin->Idxs.Add(t.Iv[0])/3;
		Mref.pOrigin->Idxs.Add(t.Iv[1]);
		Mref.pOrigin->Idxs.Add(t.Iv[2]);
		Mref.nTriangles.Add(ind);
	}
}
void BaseTriang::bInit(VertexBasedMesh& MESH)
{	
	int Nind=MESH.Idxs.GetAmount();
	Triang t;
	Vector3D a;		
	for (int i=0; i<Nind/3; i++)
	{
		t.Finish=0; 
		t.Num=0;
		t.Nfr=0;
		int i0=MESH.Idxs[3*i];
		int i1=MESH.Idxs[3*i+1];
		int i2=MESH.Idxs[3*i+2];			
		t.Iv[0]=i0;
		t.Iv[1]=i1;
		t.Iv[2]=i2;		
		t.L[0]=0;
		t.L[1]=0;
		t.L[2]=0;
		t.Ivm[0]=0;
		t.Ivm[1]=0;
		t.Ivm[2]=0;
		t.bFr[0]=0;
		t.bFr[1]=0;
		t.bFr[2]=0;	
		t.Fr[0]=0;
		t.Fr[1]=0;
		t.Fr[2]=0;	
		Tri.Add(t);		
	}
}
void BaseTriang::Init(VertexBasedMeshRef& Mref)
{	
	int Ntri=Mref.nTriangles.GetAmount();
	Tri.Clear();
	Triang t;
	Vector3D a;
	t.Finish=0; 
	t.Num=0;
	t.Nfr=0;
	t.L[0]=0;
	t.L[1]=0;
	t.L[2]=0;
	t.Ivm[0]=0;
	t.Ivm[1]=0;
	t.Ivm[2]=0;
	t.bFr[0]=0;
	t.bFr[1]=0;
	t.bFr[2]=0;
	t.Fr[0]=0;
	t.Fr[1]=0;
	t.Fr[2]=0;
	for (int i=0; i<Ntri; i++)
	{
		int ii=Mref.nTriangles[i];
		int i0=Mref.pOrigin->Idxs[3*ii];
		int i1=Mref.pOrigin->Idxs[3*ii+1];
		int i2=Mref.pOrigin->Idxs[3*ii+2];	
		t.Iv[0]=i0;
		t.Iv[1]=i1;
		t.Iv[2]=i2;
		Tri.Add(t);
	}
}
int BaseTriang::gFindFriends()
{
	int out=0;
	Hash<MasLine> Baze;
	OneMapVertex v;
	MasLine m;
	int Iv;
	int Itri=Tri.GetAmount();
	for (int i=0; i<Itri; i++)
	{
		Triang& t=Tri[i];
		for (int i0=0; i0<3; i0++)
		{
			if (t.bFr[i0]==0)
			{
				m.Iv[0]=t.Iv[(i0+1)%3];
				m.Iv[1]=t.Iv[i0];
				m.ind=i;
				m.num=i0;
				int j=Baze.find(m);
				Iv=m.Iv[0];
				m.Iv[0]=m.Iv[1];
				m.Iv[1]=Iv;
				/*int j1=Baze.find(m);
				if (j1>j) j=j1;*/
				if (j==-1)
					Baze.add(m);
				else
				{
					int fr=Baze.elem(j).ind;
					int mn=Baze.elem(j).num;
					t.bFr[i0]=1;
					t.Fr[i0]=fr;
					t.Nfr++;
					Tri[fr].bFr[mn]=1;
					Tri[fr].Fr[mn]=i;
					Tri[fr].Nfr++;
					out+=2;
				}
			}
		}
	}
	return out;
}
int BaseTriang::bFindFriends(VertexBasedMesh& MESH,BaseSurfaceFunction* Func)
{
	int out=0;
	Hash<MasLine> Baze;	
	OneMapVertex v;
	MasLine m; 
	int Iv;	
	int Itri=Tri.GetAmount();
	for (int i=0; i<Itri; i++)	
		Tri[i].Nfr=0;	
	for (int i=0; i<Itri; i++)	
	{
		Triang& t=Tri[i];
		for (int i0=0; i0<3; i0++)
		{			
			if (t.bFr[i0]==0)
			{					
				m.Iv[0]=t.Iv[(i0+1)%3];
				m.Iv[1]=t.Iv[i0];
				m.ind=i;
				m.num=i0;
				int j=Baze.find(m);	
				Iv=m.Iv[0];
				m.Iv[0]=m.Iv[1];
				m.Iv[1]=Iv;
				int j1=Baze.find(m);
				if (j1>j) j=j1;
				if (j==-1)
				{
					OneMapVertex v1=MESH.Vert[t.Iv[i0]];
					OneMapVertex v2=MESH.Vert[t.Iv[(i0+1)%3]];
					v=MidPoint(v1,v2);
					float z1;
					if (Func->CheckIfSubDivNeed(&v1,&v2,v))
					{						
						t.Num++;
						t.L[i0]=1;
						t.Ivm[i0]=MESH.Vert.Add(v);						
						out++;
					}
					Baze.add(m);				
				}
				else
				{
					int fr=Baze.elem(j).ind;
					int mn=Baze.elem(j).num;
					t.bFr[i0]=1;
					t.Fr[i0]=fr;
					t.Nfr++;
					Tri[fr].bFr[mn]=1;
					Tri[fr].Fr[mn]=i;
					Tri[fr].Nfr++;
					t.L[i0]=Tri[fr].L[mn];					
					if (t.L[i0]==1)
					{
						t.Num++;						
						t.Ivm[i0]=Tri[fr].Ivm[mn];						
					}					
				}					
			}			
		}	
		if (t.Num==2) 
		{
			for (int i0=0; i0<3; i0++)
				if (t.L[i0]==0)
				{
					OneMapVertex v1=MESH.Vert[t.Iv[i0]];
					OneMapVertex v2=MESH.Vert[t.Iv[(i0+1)%3]];
					v=MidPoint(v1,v2);					
					Func->ModifyVertex(v);
					t.Num++;
					t.L[i0]=1;
					t.Ivm[i0]=MESH.Vert.Add(v);
					if (t.bFr[i0]==1)
					{
						int fr=t.Fr[i0];
						Triang& t1=Tri[fr];
						int mn=0;
						for (int ii=0; ii<3; ii++)
							if ((t1.bFr[ii]==1) && (t1.Fr[ii]==i)) mn=ii;
						t1.L[mn]=1;
						t1.Ivm[mn]=t.Ivm[i0];
						t1.Num++;
					}
					out++;					
				}
		}
	}
	return out;
}
int BaseTriang::bFindFriends(VertexBasedMesh& MESH,DynArray<int>& ChInd,BaseSurfaceFunction* Func)
{
	int out=0;
	Hash<MasLine> Baze;
	OneMapVertex v;
	MasLine m; 	
	int Nch=ChInd.GetAmount();
	int Iv;
	for (int i1=0; i1<Nch; i1++)
	{
		int i=ChInd[i1];
		Tri[i].Nfr=0;
	}
	for (int i1=0; i1<Nch; i1++)
	{
		int i=ChInd[i1];
		Triang& t=Tri[i];
		for (int i0=0; i0<3; i0++)
		{			
			if (t.bFr[i0]==0)
			{					
				m.Iv[0]=t.Iv[(i0+1)%3];
				m.Iv[1]=t.Iv[i0];
				m.ind=i;
				m.num=i0;
				int j=Baze.find(m);	
				Iv=m.Iv[0];
				m.Iv[0]=m.Iv[1];
				m.Iv[1]=Iv;
				int j1=Baze.find(m);
				if (j1>j) j=j1;
				if (j==-1)
				{
					OneMapVertex v1=MESH.Vert[t.Iv[i0]];
					OneMapVertex v2=MESH.Vert[t.Iv[(i0+1)%3]];
					v=MidPoint(v1,v2);
					float z1;
					if (Func->CheckIfSubDivNeed(&v1,&v2,v))
					{						
						t.Num++;
						t.L[i0]=1;
						t.Ivm[i0]=MESH.Vert.Add(v);						
						out++;
					}
					Baze.add(m);				
				}
				else
				{
					int fr=Baze.elem(j).ind;
					int mn=Baze.elem(j).num;
					t.bFr[i0]=1;
					t.Fr[i0]=fr;
					Tri[fr].bFr[mn]=1;
					Tri[fr].Fr[mn]=i;
					t.L[i0]=Tri[fr].L[mn];					
					if (t.L[i0]==1)
					{
						t.Num++;						
						t.Ivm[i0]=Tri[fr].Ivm[mn];						
					}					
				}					
			}			
		}	
		if (t.Num==2) 
		{
			for (int i0=0; i0<3; i0++)
				if (t.L[i0]==0)
				{
					OneMapVertex v1=MESH.Vert[t.Iv[i0]];
					OneMapVertex v2=MESH.Vert[t.Iv[(i0+1)%3]];
					v=MidPoint(v1,v2);												
					Func->ModifyVertex(v);
					t.Num++;
					t.L[i0]=1;
					t.Ivm[i0]=MESH.Vert.Add(v);
					if (t.bFr[i0]==1)
					{
						int fr=t.Fr[i0];
						Triang& t1=Tri[fr];
						int mn=0;
						for (int ii=0; ii<3; ii++)
							if ((t1.bFr[ii]==1) && (t1.Fr[ii]==i)) mn=ii;
						t1.L[mn]=1;
						t1.Ivm[mn]=t.Ivm[i0];
						t1.Num++;
					}
					out++;					
				}
		}
	}	
	return out;
}
int BaseTriang::FindFriends(VertexBasedMeshRef& Mref,BaseSurfaceFunction* Func)
{
	int out=0;
	Hash<MasLine> Baze;	
	OneMapVertex v;	
	MasLine m; 
	int Iv;	
	int Itri=Tri.GetAmount();
	for (int i=0; i<Itri; i++)	
		Tri[i].Nfr=0;		
	for (int i=0; i<Itri; i++)	
	{
		Triang& t=Tri[i];
		for (int i0=0; i0<3; i0++)
		{			
			if (t.bFr[i0]==0)
			{					
				m.Iv[0]=t.Iv[(i0+1)%3];
				m.Iv[1]=t.Iv[i0];
				m.ind=i;
				m.num=i0;
				int j=Baze.find(m);	
				Iv=m.Iv[0];
				m.Iv[0]=m.Iv[1];
				m.Iv[1]=Iv;
				int j1=Baze.find(m);
				if (j1>j) j=j1;
				if (j==-1)
				{
					Baze.add(m);				
				}
				else
				{
					int fr=Baze.elem(j).ind;
					int mn=Baze.elem(j).num;
					t.bFr[i0]=1;
					t.Fr[i0]=fr;
					t.Nfr++;
					Tri[fr].bFr[mn]=1;
					Tri[fr].Fr[mn]=i;
					Tri[fr].Nfr++;					
				}					
			}			
		}					
	}		
	for (int i=0; i<Itri; i++)	
	{
		Triang& t=Tri[i];
		if (t.Nfr==3)
		{
			for (int i0=0; i0<3; i0++)				
			{			
				int fr=t.Fr[i0];
				int mn=0;
				Triang& t1=Tri[fr];
				if (t1.Nfr==3 && t.bFr[i0]==1)
				{
					t.bFr[i0]=2;
					for (int ii=0; ii<3; ii++)
						if (t1.Fr[ii]==i) mn=ii;
					t1.bFr[mn]=2;
					OneMapVertex v1=Mref.pOrigin->Vert[t.Iv[i0]];
					OneMapVertex v2=Mref.pOrigin->Vert[t.Iv[(i0+1)%3]];
					v=MidPoint(v1,v2);
					float z1;												
					if (Func->CheckIfSubDivNeed(&v1,&v2,v))
					{											
						t.Num++;
						t.L[i0]=1;
						t.Ivm[i0]=Mref.pOrigin->Vert.Add(v);
						out++;											
						t1.L[mn]=1;
						t1.Ivm[mn]=t.Ivm[i0];
						t1.Num++;
					}					
				}			
			}	
			if (t.Num==2) 
			{
				for (int i0=0; i0<3; i0++)
					if (t.L[i0]==0)
					{
						int fr=t.Fr[i0];
						Triang& t1=Tri[fr];
						int mn=0;
						if (t1.Nfr==3)
						{
							OneMapVertex v1=Mref.pOrigin->Vert[t.Iv[i0]];
							OneMapVertex v2=Mref.pOrigin->Vert[t.Iv[(i0+1)%3]];
							if ((v1.x-v2.x)*(v1.x-v2.x)+(v1.y-v2.y)*(v1.y-v2.y)+(v1.z-v2.z)*(v1.z-v2.z)>3*3)
							{
								v=MidPoint(v1,v2);
								Func->ModifyVertex(v);
								t.Num++;
								t.L[i0]=1;
								t.Ivm[i0]=Mref.pOrigin->Vert.Add(v);
								for (int ii=0; ii<3; ii++)
									if (t1.Fr[ii]==i) mn=ii;
								t1.L[mn]=1;
								t1.Ivm[mn]=t.Ivm[i0];
								t1.Num++;
								out++;
							}
						}
					}
			}
		}
	}		
	for (int i=0; i<Itri; i++)
	{
		Triang& t=Tri[i];		
		for (int i0=0; i0<3; i0++)
		{	
			if (t.bFr[i0]==2) t.bFr[i0]=1;
		}		
	}
	return out;
}
int BaseTriang::FindFriends(VertexBasedMeshRef& Mref,DynArray<int>& ChInd,BaseSurfaceFunction* Func)
{
	int out=0;
	Hash<MasLine> Baze;
	OneMapVertex v;
	MasLine m; 	
	int Nch=ChInd.GetAmount();
	int Iv;	
	for (int i1=0; i1<Nch; i1++)
	{
		int i=ChInd[i1];
		Triang& t=Tri[i];
		for (int i0=0; i0<3; i0++)
		{			
			if (t.bFr[i0]==0)
			{					
				m.Iv[0]=t.Iv[(i0+1)%3];
				m.Iv[1]=t.Iv[i0];
				m.ind=i;
				m.num=i0;
				int j=Baze.find(m);	
				Iv=m.Iv[0];
				m.Iv[0]=m.Iv[1];
				m.Iv[1]=Iv;
				int j1=Baze.find(m);
				if (j1>j) j=j1;
				if (j==-1)
				{
					Baze.add(m);				
				}
				else
				{
					int fr=Baze.elem(j).ind;
					int mn=Baze.elem(j).num;
					t.bFr[i0]=1;
					t.Fr[i0]=fr;				
					Tri[fr].bFr[mn]=1;
					Tri[fr].Fr[mn]=i;									
				}					
			}			
		}					
	}		
	for (int i1=0; i1<Nch; i1++)
	{
		int i=ChInd[i1];
		Triang& t=Tri[i];
		if (t.Nfr==3)
		{
			for (int i0=0; i0<3; i0++)				
			{			
				int fr=t.Fr[i0];
				int mn=0;
				Triang& t1=Tri[fr];
				if (t1.Nfr==3 && t.bFr[i0]==1)
				{
					t.bFr[i0]=2;
					for (int ii=0; ii<3; ii++)
						if (t1.Fr[ii]==i) mn=ii;
					t1.bFr[mn]=2;
					OneMapVertex v1=Mref.pOrigin->Vert[t.Iv[i0]];
					OneMapVertex v2=Mref.pOrigin->Vert[t.Iv[(i0+1)%3]];
					v=MidPoint(v1,v2);
					float z1;												
					if (Func->CheckIfSubDivNeed(&v1,&v2,v))
					{					
						t.Num++;
						t.L[i0]=1;
						t.Ivm[i0]=Mref.pOrigin->Vert.Add(v);
						out++;											
						t1.L[mn]=1;
						t1.Ivm[mn]=t.Ivm[i0];
						t1.Num++;
					}					
				}			
			}	
			if (t.Num==2) 
			{
				for (int i0=0; i0<3; i0++)
					if (t.L[i0]==0)
					{
						int fr=t.Fr[i0];
						Triang& t1=Tri[fr];
						int mn=0;
						if (t1.Nfr==3)
						{
							OneMapVertex v1=Mref.pOrigin->Vert[t.Iv[i0]];
							OneMapVertex v2=Mref.pOrigin->Vert[t.Iv[(i0+1)%3]];
							if ((v1.x-v2.x)*(v1.x-v2.x)+(v1.y-v2.y)*(v1.y-v2.y)+(v1.z-v2.z)*(v1.z-v2.z)>3*3)
							{
								v=MidPoint(v1,v2);
								Func->ModifyVertex(v);
								t.Num++;
								t.L[i0]=1;
								t.Ivm[i0]=Mref.pOrigin->Vert.Add(v);
								for (int ii=0; ii<3; ii++)
									if (t1.Fr[ii]==i) mn=ii;
								t1.L[mn]=1;
								t1.Ivm[mn]=t.Ivm[i0];
								t1.Num++;
								out++;
							}
						}
					}
			}
		}
	}		
	for (int i1=0; i1<Nch; i1++)
	{
		int i=ChInd[i1];
		Triang& t=Tri[i];		
		for (int i0=0; i0<3; i0++)
		{	
			if (t.bFr[i0]==2) t.bFr[i0]=1;
		}		
	}
	return out;
}
void BaseTriang::Partition(DynArray<int>& ChInd) 
{		
	BaseTriang ChTri;
	float leng,dist;
	int Ntri=Tri.GetAmount();		
	for (int i=0; i<Ntri; i++) ChTri.Tri.Add(Tri[i]);	
	Triang tr;
	for (int i=0; i<Ntri; i++)
	{
		Triang& t1=ChTri.Tri[i];			
		if (t1.Finish==0)
			switch(t1.Num)			
		{			 
			case 1: 
				{
					t1.Finish++;
					for (int i0=0; i0<3; i0++)
					{
						if (t1.bFr[i0]==1)
						{
							int k=t1.Fr[i0];
							Triang& t1F=ChTri.Tri[k];
							if ((t1F.Num==0) && (t1F.Finish==0)) 
							{ 
								ChInd.Add(k); 
								t1F.Finish=1;
								Triang& t=Tri[k];
								int mn=0;
								for (int ii=0; ii<3; ii++)
									if((t.bFr[ii]==1) && (t.Fr[ii]==i)) mn=ii;
								t.Fr[mn]=0;							
							}
						}
						if (t1.L[i0]==1)
						{
							ChInd.Add(i);
							int n=t1.Ivm[i0];							 					

							tr.Iv[0]=n;
							tr.Iv[1]=t1.Iv[(i0+1)%3];
							tr.Iv[2]=t1.Iv[(i0+2)%3];
							tr.Ivm[0]=0;
							tr.Ivm[1]=0;
							tr.Ivm[2]=0;
							tr.L[1]=0;
							tr.L[2]=0;
							tr.L[0]=0;
							tr.bFr[0]=0;
							tr.bFr[1]=0;
							tr.bFr[2]=0;
							tr.Fr[0]=0;
							tr.Fr[1]=0;
							tr.Fr[2]=0;
							tr.Nfr=3;
							tr.Num=0;	
							tr.Finish=0;						
							int m=Tri.Add(tr);
							ChInd.Add(m);
							Triang& t=Tri[i];						
							t.Iv[i0]=t1.Iv[i0];
							t.Iv[(i0+1)%3]=n;
							t.Iv[(i0+2)%3]=t1.Iv[(i0+2)%3];						
							t.bFr[0]=0;
							t.bFr[1]=0;
							t.bFr[2]=0;	
							t.Fr[0]=0;
							t.Fr[1]=0;
							t.Fr[2]=0;	
							t.Ivm[0]=0;
							t.Ivm[1]=0;
							t.Ivm[2]=0;
							t.L[1]=0;
							t.L[2]=0;
							t.L[0]=0;
							t.Num=0;
							t.Nfr=3;
							t.Finish=0;
						}
					}
				}
				break;
			case 2:
				{
					t1.Finish++;
					for (int i0=0; i0<3; i0++)
					{
						if (t1.bFr[i0]==1)
						{
							int k=t1.Fr[i0];
							Triang& t1F=ChTri.Tri[k];
							if ((t1F.Num==0) && (t1F.Finish==0)) 
							{ 
								ChInd.Add(k); 
								t1F.Finish=1;
								Triang& t=Tri[k];
								int mn=0;
								for (int ii=0; ii<3; ii++)
									if((t.bFr[ii]==1) && (t.Fr[ii]==i)) mn=ii;
								t.Fr[mn]=0;	
							}
						}
						if (t1.L[i0]==0)
						{
							ChInd.Add(i);
							int n1=t1.Ivm[(i0+1)%3];
							int n2=t1.Ivm[(i0+2)%3];

							tr.Iv[0]=t1.Iv[i0];
							tr.Iv[1]=n1;
							tr.Iv[2]=n2;
							tr.L[1]=0;
							tr.L[2]=0;
							tr.L[0]=0;
							tr.bFr[0]=0;
							tr.bFr[1]=0;
							tr.bFr[2]=0;
							tr.Fr[0]=0;
							tr.Fr[1]=0;
							tr.Fr[2]=0;
							tr.Finish=0;
							tr.Num=0;
							tr.Nfr=3;
							tr.Ivm[0]=0;
							tr.Ivm[1]=0;
							tr.Ivm[2]=0;
							int m1=Tri.Add(tr);
							ChInd.Add(m1);

							tr.Iv[0]=t1.Iv[i0];
							tr.Iv[1]=t1.Iv[(i0+1)%3];
							tr.Iv[2]=n1;						
							int m2=Tri.Add(tr);
							ChInd.Add(m2);						

							Triang& t=Tri[i];						
							t.Iv[i0]=n2;
							t.Iv[(i0+1)%3]=n1;
							t.Iv[(i0+2)%3]=t1.Iv[(i0+2)%3];						
							t.Num=0;
							t.L[1]=0;
							t.L[2]=0;
							t.L[0]=0;						
							t.bFr[0]=0;
							t.bFr[1]=0;
							t.bFr[2]=0;
							t.Fr[0]=0;
							t.Fr[1]=0;
							t.Fr[2]=0;
							t.Ivm[0]=0;
							t.Ivm[1]=0;
							t.Ivm[2]=0;
							t.Finish=0;
							t.Nfr=3;
						}
					}
				}
				break;
			case 3:
				{
					t1.Finish++;
					for (int i0=0; i0<3; i0++)
					{
						if (t1.bFr[i0]==1)
						{
							int k=t1.Fr[i0];
							Triang& t1F=ChTri.Tri[k];
							if ((t1F.Num==0) && (t1F.Finish==0)) 
							{ 
								ChInd.Add(k); 
								t1F.Finish=1;
								Triang& t=Tri[k];
								int mn=0;
								for (int ii=0; ii<3; ii++)
									if((t.bFr[ii]==1) && (t.Fr[ii]==i)) mn=ii;
								t.Fr[mn]=0;
							}	
						}
					}					
					ChInd.Add(i);
					int n0=t1.Ivm[0];
					int n1=t1.Ivm[1];
					int n2=t1.Ivm[2];				

					tr.Iv[0]=n2;
					tr.Iv[1]=t1.Iv[0];
					tr.Iv[2]=n0;
					tr.L[1]=0;
					tr.L[2]=0;
					tr.L[0]=0;
					tr.bFr[0]=0;
					tr.bFr[1]=0;
					tr.bFr[2]=0;	
					tr.Fr[0]=0;
					tr.Fr[1]=0;
					tr.Fr[2]=0;
					tr.Num=0;		
					tr.Finish=0;
					tr.Nfr=3;
					tr.Ivm[0]=0;
					tr.Ivm[1]=0;
					tr.Ivm[2]=0;
					int m0=Tri.Add(tr);
					ChInd.Add(m0);

					tr.Iv[0]=n0;
					tr.Iv[1]=t1.Iv[1];
					tr.Iv[2]=n1;					
					int m1=Tri.Add(tr);
					ChInd.Add(m1);

					tr.Iv[0]=n1;
					tr.Iv[1]=t1.Iv[2];
					tr.Iv[2]=n2;					
					int m2=Tri.Add(tr);
					ChInd.Add(m2);

					Triang& t=Tri[i];				
					t.Iv[0]=n0;
					t.Iv[1]=n1;
					t.Iv[2]=n2;
					t.L[1]=0;
					t.L[2]=0;
					t.L[0]=0;
					t.bFr[0]=0;
					t.bFr[1]=0;
					t.bFr[2]=0;	
					t.Fr[0]=0;
					t.Fr[1]=0;
					t.Fr[2]=0;
					t.Ivm[0]=0;
					t.Ivm[1]=0;
					t.Ivm[2]=0;
					t.Num=0;
					t.Finish=0;
					t.Nfr=3;
				}
				break;			
		}	
	}
}
void BaseTriang::Partition(DynArray<int>& InInd, DynArray<int>& ChInd) 
{	
	float leng,dist;
	BaseTriang ChTri;
	int Ntri=Tri.GetAmount();
	for (int i=0; i<Ntri; i++) ChTri.Tri.Add(Tri[i]);	
	Triang tr;
	int Nin=InInd.GetAmount();
	for (int i1=0; i1<Nin; i1++)
	{
		i=InInd[i1];
		Triang& t1=ChTri.Tri[i];			
		if (t1.Finish==0)
			switch(t1.Num)			
		{			 
			case 1: 
				{
					t1.Finish++;
					for (int i0=0; i0<3; i0++)
					{
						if (t1.bFr[i0]==1)
						{
							int k=t1.Fr[i0];
							Triang& t1F=ChTri.Tri[k];
							if ((t1F.Num==0) && (t1F.Finish==0)) 
							{ 
								ChInd.Add(k); 
								t1F.Finish=1;
								Triang& t=Tri[k];
								int mn=0;
								for (int ii=0; ii<3; ii++)
									if((t.bFr[ii]==1) && (t.Fr[ii]==i)) mn=ii;
								t.Fr[mn]=0;							
							}
						}
						if (t1.L[i0]==1)
						{
							ChInd.Add(i);
							int n=t1.Ivm[i0];							 					

							tr.Iv[0]=n;
							tr.Iv[1]=t1.Iv[(i0+1)%3];
							tr.Iv[2]=t1.Iv[(i0+2)%3];
							tr.Ivm[0]=0;
							tr.Ivm[1]=0;
							tr.Ivm[2]=0;
							tr.L[1]=0;
							tr.L[2]=0;
							tr.L[0]=0;
							tr.bFr[0]=0;
							tr.bFr[1]=0;
							tr.bFr[2]=0;
							tr.Fr[0]=0;
							tr.Fr[1]=0;
							tr.Fr[2]=0;						
							tr.Num=0;	
							tr.Finish=0;
							tr.Nfr=3;
							int m=Tri.Add(tr);
							ChInd.Add(m);
							Triang& t=Tri[i];						
							t.Iv[i0]=t1.Iv[i0];
							t.Iv[(i0+1)%3]=n;
							t.Iv[(i0+2)%3]=t1.Iv[(i0+2)%3];						
							t.bFr[0]=0;
							t.bFr[1]=0;
							t.bFr[2]=0;	
							t.Fr[0]=0;
							t.Fr[1]=0;
							t.Fr[2]=0;	
							t.Ivm[0]=0;
							t.Ivm[1]=0;
							t.Ivm[2]=0;
							t.L[1]=0;
							t.L[2]=0;
							t.L[0]=0;
							t.Num=0;
							t.Finish=0;
							t.Nfr=3;
						}
					}
				}
				break;
			case 2:
				{
					t1.Finish++;
					for (int i0=0; i0<3; i0++)
					{
						if (t1.bFr[i0]==1)
						{
							int k=t1.Fr[i0];
							Triang& t1F=ChTri.Tri[k];
							if ((t1F.Num==0) && (t1F.Finish==0)) 
							{ 
								ChInd.Add(k); 
								t1F.Finish=1;
								Triang& t=Tri[k];
								int mn=0;
								for (int ii=0; ii<3; ii++)
									if((t.bFr[ii]==1) && (t.Fr[ii]==i)) mn=ii;
								t.Fr[mn]=0;	
							}
						}
						if (t1.L[i0]==0)
						{
							ChInd.Add(i);
							int n1=t1.Ivm[(i0+1)%3];
							int n2=t1.Ivm[(i0+2)%3];

							tr.Iv[0]=t1.Iv[i0];
							tr.Iv[1]=n1;
							tr.Iv[2]=n2;
							tr.L[1]=0;
							tr.L[2]=0;
							tr.L[0]=0;
							tr.bFr[0]=0;
							tr.bFr[1]=0;
							tr.bFr[2]=0;
							tr.Fr[0]=0;
							tr.Fr[1]=0;
							tr.Fr[2]=0;
							tr.Finish=0;
							tr.Num=0;
							tr.Nfr=3;
							tr.Ivm[0]=0;
							tr.Ivm[1]=0;
							tr.Ivm[2]=0;
							int m1=Tri.Add(tr);
							ChInd.Add(m1);

							tr.Iv[0]=t1.Iv[i0];
							tr.Iv[1]=t1.Iv[(i0+1)%3];
							tr.Iv[2]=n1;						
							int m2=Tri.Add(tr);
							ChInd.Add(m2);						

							Triang& t=Tri[i];						
							t.Iv[i0]=n2;
							t.Iv[(i0+1)%3]=n1;
							t.Iv[(i0+2)%3]=t1.Iv[(i0+2)%3];						
							t.Num=0;
							t.L[1]=0;
							t.L[2]=0;
							t.L[0]=0;						
							t.bFr[0]=0;
							t.bFr[1]=0;
							t.bFr[2]=0;
							t.Fr[0]=0;
							t.Fr[1]=0;
							t.Fr[2]=0;
							t.Ivm[0]=0;
							t.Ivm[1]=0;
							t.Ivm[2]=0;
							t.Finish=0;
							t.Nfr=3;
						}
					}
				}
				break;
			case 3:
				{
					t1.Finish++;
					for (int i0=0; i0<3; i0++)
					{
						if (t1.bFr[i0]==1)
						{
							int k=t1.Fr[i0];
							Triang& t1F=ChTri.Tri[k];
							if ((t1F.Num==0) && (t1F.Finish==0)) 
							{ 
								ChInd.Add(k); 
								t1F.Finish=1;
								Triang& t=Tri[k];
								int mn=0;
								for (int ii=0; ii<3; ii++)
									if((t.bFr[ii]==1) && (t.Fr[ii]==i)) mn=ii;
								t.Fr[mn]=0;
							}	
						}
					}					
					ChInd.Add(i);
					int n0=t1.Ivm[0];
					int n1=t1.Ivm[1];
					int n2=t1.Ivm[2];				

					tr.Iv[0]=n2;
					tr.Iv[1]=t1.Iv[0];
					tr.Iv[2]=n0;
					tr.L[1]=0;
					tr.L[2]=0;
					tr.L[0]=0;
					tr.bFr[0]=0;
					tr.bFr[1]=0;
					tr.bFr[2]=0;	
					tr.Fr[0]=0;
					tr.Fr[1]=0;
					tr.Fr[2]=0;
					tr.Num=0;		
					tr.Finish=0;
					tr.Nfr=3;
					tr.Ivm[0]=0;
					tr.Ivm[1]=0;
					tr.Ivm[2]=0;
					int m0=Tri.Add(tr);
					ChInd.Add(m0);

					tr.Iv[0]=n0;
					tr.Iv[1]=t1.Iv[1];
					tr.Iv[2]=n1;					
					int m1=Tri.Add(tr);
					ChInd.Add(m1);

					tr.Iv[0]=n1;
					tr.Iv[1]=t1.Iv[2];
					tr.Iv[2]=n2;					
					int m2=Tri.Add(tr);
					ChInd.Add(m2);

					Triang& t=Tri[i];				
					t.Iv[0]=n0;
					t.Iv[1]=n1;
					t.Iv[2]=n2;
					t.L[1]=0;
					t.L[2]=0;
					t.L[0]=0;
					t.bFr[0]=0;
					t.bFr[1]=0;
					t.bFr[2]=0;	
					t.Fr[0]=0;
					t.Fr[1]=0;
					t.Fr[2]=0;
					t.Ivm[0]=0;
					t.Ivm[1]=0;
					t.Ivm[2]=0;
					t.Num=0;
					t.Finish=0;
					t.Nfr=3;
				}
				break;			
		}

	}
}
///-----------BaseDirectionalFunction-------------///
void BaseDirectionalFunction::ProectOnCyl(OneMapVertex& V, float& H, float MaxR)
{
	Vector3D vec,v;
	v.x=V.x;
	v.y=V.y;
	v.z=V.z;
	vec.x=xc-float(H*dir.x/127);
	vec.y=yc-float(H*dir.y/127);
	vec.z=zc-float(H*dir.z/127);
	v-=vec;
	v*=(MaxR/v.norm());
	V.x=vec.x+v.x;
	V.y=vec.y+v.y;
	V.z=vec.z+v.z;
}
bool BaseDirectionalFunction::InCyl(float eps, const OneMapVertex& V, float& H, float& R, float& Fi)
{	
	Vector3D vec;
	vec.x=V.x-xc;
	vec.y=V.y-yc;
	vec.z=V.z-zc;
	OneMapVertex v0;	
	v0.x=xc;
	v0.y=yc;
	v0.z=zc;
	v0.nx=dir.x;
	v0.ny=dir.y;
	v0.nz=dir.z;
	Vector3D vec1=vec;	
	FindXYZ(v0,vec1);	
	R=vec1.norm();	
	vec-=vec1;
	H=vec.norm();	
	Vector3D vec0,dr;
	vec0.x=0;
	vec0.y=0;
	vec0.z=1;
	if (dir.x*dir.x+dir.y*dir.y==0) 
	{
		dr.x=1;
		dr.z=0;
		dr.y=0;
	}
	else
		dr=VectMul(vec0, dir);	
	Fi=vec1.Angle(dr,dir);	
	if (vec.x*dir.x+vec.y*dir.y+vec.z*dir.z>0) H*=-1;	
	return (R<=GetMaxR(Fi)+eps && H>0);
}
bool BaseDirectionalFunction::InCyl(float eps, const OneMapVertex& V)
{
	float H, Fi, R;
	return InCyl(eps, V, H, R, Fi);
}
bool BaseDirectionalFunction::InEnd(float eps, const OneMapVertex& V, float& Fi, float& Theta)
{
	float H, R;
	InCyl(0, V, H, R, Fi);
	Theta=0;
	if (H>0) return false;
	Vector3D vec;
	vec.x=V.x-xc;
	vec.y=V.y-yc;
	vec.z=V.z-zc;
	Theta=vec.Angle(dir);
	return ( R<=GetMaxR(Fi)+eps && vec.norm()<=GetModulator(Theta,Fi)+eps );
}
bool BaseDirectionalFunction::InEnd(float eps, const OneMapVertex& V)
{
	float Fi, Theta;
	return InEnd(eps, V, Fi, Theta);
}
void BaseDirectionalFunction::Approximation(VertexBasedMeshRef& Mref, DynArray<int>& List, Hash<boolh>& VertIsIn, int rep)
{	
	float Mod, Fi, H, R, Theta;
	boolh bh;	
	VertexBasedMeshRef Mref0;
	Mref0.pOrigin=Mref.pOrigin;
	int Ntri=Mref.nTriangles.GetAmount();	
	for (int ii=0; ii<Ntri; ii++)
		{
			int i=Mref.nTriangles[ii];
			int i0=Mref.pOrigin->Idxs[3*i];
			int i1=Mref.pOrigin->Idxs[3*i+1];
			int i2=Mref.pOrigin->Idxs[3*i+2];
			bh.ind=i0;
			int j=VertIsIn.find(bh);
			bool b0=(j>-1);
			bh.ind=i1;
			j=VertIsIn.find(bh);
			bool b1=(j>-1);
			bh.ind=i2;
			j=VertIsIn.find(bh);
			bool b2=(j>-1);
			if (b0 || b1 || b2)
			{
				Mref0.nTriangles.Add(i);
				OneMapVertex& v0=Mref.pOrigin->Vert[i0];
				OneMapVertex& v1=Mref.pOrigin->Vert[i1];
				OneMapVertex& v2=Mref.pOrigin->Vert[i2];
				if (!b0 && !b1 && b2)
				{
					MoveVertex(v0, v2, this);
					MoveVertex(v1, v2, this);
				}
				if (!b1 && !b2 && b0)
				{
					MoveVertex(v1, v0, this);
					MoveVertex(v2, v0, this);
				}
				if (!b2 && !b0 && b1)
				{
					MoveVertex(v2, v1, this);
					MoveVertex(v0, v1, this);
				}
			}
		}		
	int Nver=List.GetAmount();
	for (int i0=0; i0<Nver; i0++)
	{
		int i=List[i0];
		OneMapVertex& V=Mref.pOrigin->Vert[i];
		bh.ind=i;
		int j=VertIsIn.find(bh);
		bool b=(j>-1);
		if (b)
		{		
			InCyl(0, V, H, R, Fi);
			if (H>0)
			{
				V.x+=(H+2*(GetMaxR(Fi)-R))*dir.x/127;
				V.y+=(H+2*(GetMaxR(Fi)-R))*dir.y/127;
				V.z+=(H+2*(GetMaxR(Fi)-R))*dir.z/127;
			}			
			ModifyVertex(V);		
		}
	}
	for (int j=0; j<rep; j++)
	{		
		Mref0.Smooth(0.2, 1);
		for (int i0=0; i0<Nver; i0++)
		{
			int i=List[i0];
			OneMapVertex& V=Mref.pOrigin->Vert[i];
			bh.ind=i;
			int jj=VertIsIn.find(bh);			
			bool b=(jj>-1);
			if (b)			
				ModifyVertex(V);			
		}
	}
	int Nind=Mref.pOrigin->Idxs.GetAmount();
	Mref0.ModifySurface(0, this);	
	int Nind1=Mref.pOrigin->Idxs.GetAmount();
	for (int i=Nind/3; i<Nind/3; i++)
		Mref.nTriangles.Add(i);
	//Mref0.DeleteFlattening();
}
void BaseDirectionalFunction::DeleteDirectionalSurface(VertexBasedMeshRef& Mref)
{
	Hash<boolh> VertIsIn;
	boolh bh;
	int Ntri=Mref.nTriangles.GetAmount();
	DynArray<int> List;
	Mref.VertexList(List);
	int Nver=List.GetAmount();
	for (int ii=0; ii<Nver; ii++)	
	{
		int i=List[ii];
		OneMapVertex& V=Mref.pOrigin->Vert[i];
		bh.ind=i;
		if (ReturnInfo(0, V)) VertIsIn.add(bh);
	}	
	Approximation(Mref, List, VertIsIn, 4);	
}
void BaseDirectionalFunction::ModifyVertex(OneMapVertex& V)
{
	float H, R, Fi, Theta;
	ReturnInfo(0,V, H, R, Fi, Theta);	
	if (H>0) ProectOnCyl(V,H,GetMaxR(Fi));
	else 
	{
		Vector3D vec;
		vec.x=V.x-xc;
		vec.y=V.y-yc;
		vec.z=V.z-zc;
		vec*=(GetModulator(Theta,Fi)/vec.norm());
		V.x=vec.x+xc;
		V.y=vec.y+yc;
		V.z=vec.z+zc;
	}
}
bool BaseDirectionalFunction::CheckIfSubDivNeed(const OneMapVertex* V1,const OneMapVertex* V2,OneMapVertex& V)
{	
	Vector3D v,v1,v2;
	float H1, R1, Fi1, Theta1, H2, R2, Fi2, Theta2;
	//V=MidPoint(V1,V2);
	bool b1=ReturnInfo(2, *V1, H1, R1, Fi1, Theta1);
	if (!b1) return false;
	bool b2=ReturnInfo(2, *V2, H2, R2, Fi2, Theta2);
	if (!b2) return false;		
	v1.x=V1->x;
	v1.y=V1->y;
	v1.z=V1->z;
	v2.x=V2->x;
	v2.y=V2->y;
	v2.z=V2->z;
	v.x=V.x;
	v.y=V.y;
	v.z=V.z;	
	ModifyVertex(V);
	if (InEnd(2, V)) return true;
	Vector3D vhelp=v1;
	vhelp-=v2;
	if (vhelp.norm()>32) return true;	
	float dz2=(V.x-v.x)*(V.x-v.x)+(V.y-v.y)*(V.y-v.y)+(V.z-v.z)*(V.z-v.z);
	float ang=fabs(Fi1-Fi2);
	if (ang>c_PI) ang=2*c_PI-ang;
	return (dz2>3*3 || ang>c_PI/24 || fabs(R1-R2)>5 || fabs(Theta1-Theta2)>c_PI/24);	
}
void BaseDirectionalFunction::ReturnMeshRef(UniversalMap& UniMap, VertexBasedMeshRef& Mref, int& ox1, int& ox2, int& oy1, int& oy2)
{
	int x1=(xc-UniMap.QuadX0)/UniMap.QuadLx;
	int y1=(yc-UniMap.QuadY0)/UniMap.QuadLy;
	int ind;
	int x2=x1;
	int y2=y1;
	ox1=x1+1;
	ox2=x2-1;
	oy1=y1+1;
	oy2=y2-1;
	if (x1>0) x1--;
	if (x2<UniMap.QuadsNx-1) x2++;
	if (y1>0) y1--;
	if (y2<UniMap.QuadsNx-1) y2++;
	int stop=1;	
	while (stop>0)
	{	
		for (int xi=x1; xi<=x2; xi++)
			for (int yi=y1; yi<=y2; yi++)
				if (!(xi>=ox1 && xi<=ox2 && yi>=oy1 && yi<=oy2))
				{
					ind=xi+yi*UniMap.QuadsNx;
					int Ntri=UniMap.m_pQuads[ind]->nTriangles.GetAmount();
					for (int i=0; i<Ntri; i++)
						Mref.nTriangles.Add(UniMap.m_pQuads[ind]->nTriangles[i]);
				}
		ox1=x1;
		ox2=x2;
		oy1=y1;
		oy2=y2;
		stop=0;
		if (x1>0) x1--;
		if (x2<UniMap.QuadsNx-1) x2++;
		if (y1>0) y1--;
		if (y2<UniMap.QuadsNx-1) y2++;
		for (int xi=x1; xi<=x2; xi++)
			for (int yi=y1; yi<=y2; yi++)
				if (!(xi>=ox1 && xi<=ox2 && yi>=oy1 && yi<=oy2))
				{
					ind=xi+yi*UniMap.QuadsNx;
					int Ntri=UniMap.m_pQuads[ind]->nTriangles.GetAmount();
					for (int i=0; i<Ntri; i++)
					{
						int nt=UniMap.m_pQuads[ind]->nTriangles[i];
						int i0=Mref.pOrigin->Idxs[3*nt];
						int i1=Mref.pOrigin->Idxs[3*nt+1];
						int i2=Mref.pOrigin->Idxs[3*nt+2];
						OneMapVertex& v0=UniMap.Vert[i0];
						OneMapVertex& v1=UniMap.Vert[i1];
						OneMapVertex& v2=UniMap.Vert[i2];
						if (ReturnInfo(0, v0) || ReturnInfo(0, v1) || ReturnInfo(0, v2))
							stop++;
					}
				}		
	}
	ox1*=UniMap.QuadLx;	
	ox2*=UniMap.QuadLx;
	ox2+=UniMap.QuadLx;
	oy1*=UniMap.QuadLy;
	oy2*=UniMap.QuadLy;
	oy2+=UniMap.QuadLy;	
}
///---------BaseSurfaceFunction Functions---------///
void BaseSurfaceFunction::ModifyVertex(OneMapVertex& V)
{
	V.z=FN1(V.x+250*GetFractV(V.x,V.y)/2,V.y+250*GetFractV(V.x+234,V.y+761)/2);
}
bool BaseSurfaceFunction::CheckIfSubDivNeed(const OneMapVertex* V1,const OneMapVertex* V2,OneMapVertex& V)
{
	int dz=abs(V1->z-V2->z);
	if(dz<3)return false;
	V.x=(V1->x+V2->x)/2;
	V.y=(V1->y+V2->y)/2;
	V.z=(V1->z+V2->z)/2;	
	ModifyVertex(V);	
	float dz2=V.z-(V1->z+V2->z)/2;
	dz2=fabs(dz2);
	return (dz2>3||dz>8);
}
float av=0;
int nv=0;
bool BaseSurfaceFunction::ShiftVertex(OneMapVertex& V){
	float dx=GetFractV(V.x*1.21+1765,V.y*1.34+3987);
	float dy=GetFractV(V.x*0.98+1407,V.y*0.873+2327)-0.27;
	av+=dy;
	nv++;
	float zv=sqrt(abs(V.z*(150-V.z)))*1.5;
	V.x+=dx*zv;
	V.y+=dy*zv;
	return (zv>0);
}
///--------BaseSphericalFunction Function---------///
bool BaseSphericalFunction::ShiftVertex(OneMapVertex& V)
{
	Vector3D Vec;
	Vec.x=V.x-xc;
	Vec.y=V.y-yc;
	Vec.z=V.z-zc;	
	float norm=Vec.norm();
	float Theta=atan2(Vec.x,Vec.y);
	float Fi=asin(Vec.z/norm);
	float leng=GetModulator(Theta,Fi);
	if (leng>norm) Vec*=(leng/norm);
	V.x=Vec.x;
	V.y=Vec.y;
	V.z=Vec.z;
	return (leng>norm);
}
void BaseSphericalFunction::DeleteSphericalSurface(VertexBasedMesh& MESH)
{
	DynArray<int> IndLive;
	DynArray<ChTri> IndTri;
	DynArray<int> VertIn;
	DynArray<OneMapVertex> NewVert;
	DynArray<int> NewIdxs;
	ChTri ct;
	int Nver=MESH.Vert.GetAmount();
	for (int i=0; i<Nver; i++)
	{
		OneMapVertex V0=MESH.Vert[i];		
		if (ShiftVertex(V0)) VertIn.Add(1);
		else VertIn.Add(0); 
	}
	int Nind=MESH.Idxs.GetAmount();
	for (int i=0; i<Nind/3; i++)
	{
		int n0=VertIn[MESH.Idxs[3*i]];
		int n1=VertIn[MESH.Idxs[3*i+1]];
		int n2=VertIn[MESH.Idxs[3*i+2]];
		int sum=n0+n1+n2;
		switch(sum)
		{
		case 0: IndLive.Add(i); break;			
		case 1: 
			{
				ct.ind=i; 
				if (n0==1) ct.num=0;
				if (n1==1) ct.num=1;
				if (n2==1) ct.num=2;
				IndTri.Add(ct);
			} break;
		}
	}
	OneMapVertex v;
	for (int i=0; i<Nver; i++)
		if (VertIn[i]==0)
		{
			VertIn[i]=NewVert.GetAmount();
			NewVert.Add(MESH.Vert[i]);
		}
		v.x=xc;
		v.y=yc;
		v.z=zc;
		int CenInd=NewVert.Add(v);
		for (int i=0; i<IndTri.GetAmount(); i++)
		{
			int ind=IndTri[i].ind;
			int num=IndTri[i].num;
			int i1=MESH.Idxs[3*ind+(num+1)%3];
			int i2=MESH.Idxs[3*ind+(num+2)%3];
			NewIdxs.Add(CenInd);
			NewIdxs.Add(VertIn[i1]);
			NewIdxs.Add(VertIn[i2]);
		}
		for (int i=0; i<IndLive.GetAmount(); i++)
		{		
			int ind=IndLive[i];
			int i0=MESH.Idxs[3*ind];
			int i1=MESH.Idxs[3*ind+1];
			int i2=MESH.Idxs[3*ind+2];
			NewIdxs.Add(VertIn[i0]);
			NewIdxs.Add(VertIn[i1]);
			NewIdxs.Add(VertIn[i2]);
		}
		MESH.Vert.Clear();
		for (int i=0; i<NewVert.GetAmount(); i++) MESH.Vert.Add(NewVert[i]);
		MESH.Idxs.Clear();
		for (int i=0; i<NewIdxs.GetAmount(); i++) MESH.Idxs.Add(NewIdxs[i]);
}
bool BaseSphericalFunction::CheckIfSubDivNeed(const OneMapVertex* V1,const OneMapVertex* V2,OneMapVertex& V)
{
	return true;
}
void BaseSphericalFunction::ModifyVertex(OneMapVertex& V)
{
}
///------------UniversalMap Functions-------------///
/*void UniversalMap::InitUniversalMapQuad(void)
{
	// Clearing previous quad fragmentation
	int N = m_pQuads.GetAmount();
	for(int i = 0; i < N; i++)
		if(m_pQuads[i] != NULL) delete m_pQuads[i];
	QuadLx = 0;
	QuadLy = 0;
	QuadsNx = 0;
	QuadsNy = 0;
	QuadX0 = 0;
	QuadY0 = 0;
	// Performing new quad fragmentation
	const int Lx = 128, Ly = 128;
	QuadLx=Lx;
	QuadLy=Ly;
	int Nver=Vert.GetAmount();
	int minX=Vert[0].x;
	int maxX=minX;
	int minY=Vert[0].y;
	int maxY=minY;
	for (int i=0; i<Nver; i++)
	{
		OneMapVertex& v=Vert[i];
		if (v.x>maxX) maxX=v.x;
		if (v.x<minX) minX=v.x;
		if (v.y>maxY) maxY=v.y;
		if (v.y<minY) minY=v.y;
	}
	Change(minX,Lx,-1);
	Change(maxX,Lx,1);
	Change(minY,Ly,-1);
	Change(maxY,Ly,1);
	QuadX0=minX;
	QuadY0=minY;
	QuadsNx=(maxX-minX)/Lx;
	QuadsNy=(maxY-minY)/Ly;
	UMLx = QuadsNx * QuadLx, UMLy = QuadsNy * QuadLy;
	m_pQuads.Clear();	
	for (int i=0; i<QuadsNx*QuadsNy; i++)
	{
		OneUniversalMapQuad* q=new OneUniversalMapQuad;
		q->Dirty=true;
		q->nTriangles.Clear();
		q->pOrigin = this;
		m_pQuads.Add(q);
	}
	int Nind=Idxs.GetAmount();
	for (i=0; i<Nind/3; i++)
	{
		OneMapVertex& v0=Vert[Idxs[3*i]];
		OneMapVertex& v1=Vert[Idxs[3*i+1]];
		OneMapVertex& v2=Vert[Idxs[3*i+2]];
		OneMapVertex v;
		v.x=(v0.x+v1.x+v2.x)/3;
		v.y=(v0.y+v1.y+v2.y)/3;		
		int ind=(v.x-QuadX0)/QuadLx+((v.y-QuadY0)/QuadLy)*QuadsNx;
		m_pQuads[ind]->nTriangles.Add(i);
	}
}*/
void UniversalMap::ModifySurface(int x1, int y1, int x2, int y2, int MaxSubDiv,BaseSurfaceFunction* Func)
{
	int xi0=(x1-QuadX0)/QuadLx;	
	if (xi0<0) xi0=0;	
	int xi1=(x2-QuadX0)/QuadLx;	
	if (xi1>=QuadsNx) xi1=QuadsNx-1;
	int yi0=(y1-QuadY0)/QuadLy;		
	if (yi0<0) yi0=0;
	int yi1=(y2-QuadY0)/QuadLy;	
	if (yi1>=QuadsNy) yi1=QuadsNy-1;	
	VertexBasedMeshRef ref;
	ref.pOrigin=this;
	for (int xi=xi0; xi<=xi1; xi++)
		for (int yi=yi0; yi<=yi1; yi++)
		{
			int ind=xi+yi*QuadsNx;
			OneUniversalMapQuad& q=*m_pQuads[ind];			
			q.Dirty=true;
			for (int i=0; i<q.nTriangles.GetAmount(); i++)
				ref.nTriangles.Add(q.nTriangles[i]);
			q.nTriangles.Clear();
		}
		int Nind=ref.pOrigin->Idxs.GetAmount();
		int Ntri=ref.nTriangles.GetAmount();
		ref.ModifySurface(MaxSubDiv,Func);		
		for (int i0=0; i0<Ntri; i0++)
		{
			int i=ref.nTriangles[i0];
			OneMapVertex& v0=Vert[Idxs[3*i]];
			OneMapVertex& v1=Vert[Idxs[3*i+1]];
			OneMapVertex& v2=Vert[Idxs[3*i+2]];
			OneMapVertex v;
			v.x=(v0.x+v1.x+v2.x)/3;
			v.y=(v0.y+v1.y+v2.y)/3;			
			int ind=(v.x-QuadX0)/QuadLx+((v.y-QuadY0)/QuadLy)*QuadsNx;
			m_pQuads[ind]->nTriangles.Add(i);
		}
		int Nind1=ref.pOrigin->Idxs.GetAmount();
		for (int i=Nind/3; i<Nind1/3; i++)
		{
			OneMapVertex& v0=Vert[Idxs[3*i]];
			OneMapVertex& v1=Vert[Idxs[3*i+1]];
			OneMapVertex& v2=Vert[Idxs[3*i+2]];
			OneMapVertex v;
			v.x=(v0.x+v1.x+v2.x)/3;
			v.y=(v0.y+v1.y+v2.y)/3;			
			int ind=(v.x-QuadX0)/QuadLx+((v.y-QuadY0)/QuadLy)*QuadsNx;
			m_pQuads[ind]->nTriangles.Add(i);
		}
}
extern UniversalMap g_UniversalMap;
void InitLayersSystem(){
	bool LoadMLayersFromFileInCompactForm(char* filename);
	LoadMLayersFromFileInCompactForm("SurfaceShape.dat");
	SURF.reset_class(&SURF);
	SURF.SafeReadFromFile("Surface3D.xml");
}
void CreateNew3DMap(int size,DWORD Color){
    g_UniversalMap.CreateRandomPatch(size/32,size/32,32);
	for(int i=0;i<g_UniversalMap.Vert.GetAmount();i++){
		g_UniversalMap.Vert[i].Color=Color;
		g_UniversalMap.Vert[i].Shadow=128;
		g_UniversalMap.Vert[i].z=50;
		g_UniversalMap.Vert[i].nFacture=0;
	}
	g_UniversalMap.CalcNormals();
	g_UniversalMap.QuadsSystem_Init();	
	InitLayersSystem();
}
///-----------------MAIN FUNCTION-----------------///
bool MathTestDraw()
{		
	UniversalMap UniMap;
	VertexBasedMeshRef pVBMeshRef;
	Vector3D a, dir;
	BaseSurfaceFunction Func;
	a.x=400; 
	a.y=970; 
	a.z=30;
	UniMap.CreateSphere(a, 300, 0&000077, 1, 4);
	//UniMap.CreateSurface(32, 32, 48, &Func);
	//UniMap.ModifySurface(0, 0, 1536, 1536, 2, &Func);	
	BaseDirectionalFunction Func0, Func1;
	dir.x=0;
	dir.y=-1;
	dir.z=0;
	Func0.Create(a, dir, 60);
	///*dir.y=0;
	//dir.z=-1;
	//Func1.Create(a, dir);*/
	VertexBasedMeshRef Mref;
	Mref.pOrigin=&UniMap;
	Mref.nTriangles.Clear();
	int x1, x2, y1, y2;
	//Func0.ReturnMeshRef(UniMap, Mref, x1, x2, y1, y2);
	UniMap.GetMeshRefSphere(a, 310, &Mref);
	Func0.DeleteDirectionalSurface(Mref);
	////UniMap.ModifySurface(0, 0, 1600, 1600, 0, &Func0);
	////UniMap.ModifySurface(x1, y1, x2, y2, 0, &Func0);
	///*Mref.nTriangles.Clear();
	//Func1.ReturnMeshRef(UniMap, Mref, x1, x2, y1, y2);
	//Func1.DeleteDirectionalSurface(Mref);
	//UniMap.ModifySurface(x1, y1, x2, y2, 0, &Func1);*/
	////Mref.DeleteFlattening();
	////Mref.ModifySurface(1, &Func0);
	////Mref.Smooth(1, 2);
	////UniMap.ModifySurface(0, 0, 1600, 1600, 0, &Func1);
	UniMap.Smooth(1, 2);
	UniMap.CreateNormals();
	a.x=0;
	a.y=1;
	a.z=1;
//	UniMap.CreateShadow(a,30,210);
	do{
		//processing
//        UniMap.TestDraw();
		/*for (int i=0; i<Mref.nTriangles.GetAmount(); i++)
		{
			Vector3D V;
			int k=Mref.nTriangles[i];
			OneMapVertex& v0=Mref.pOrigin->Vert[Mref.pOrigin->Idxs[3*k]];
			OneMapVertex& v1=Mref.pOrigin->Vert[Mref.pOrigin->Idxs[3*k+1]];
			OneMapVertex& v2=Mref.pOrigin->Vert[Mref.pOrigin->Idxs[3*k+2]];
			V.x=(v0.x+v1.x+v2.x)/3;
			V.y=(v0.y+v1.y+v2.y)/3;
			V.z=(v0.z+v1.z+v2.z)/3;
			DrawDebugText(V, "*");
		}*/
		/*for (int i=0; i<UniMap.m_pQuads[119].nTriangles.GetAmount(); i++)
		{
			Vector3D V;
			int k=UniMap.m_pQuads[119].nTriangles[i];
			OneMapVertex& v0=Mref.pOrigin->Vert[Mref.pOrigin->Idxs[3*i]];
			OneMapVertex& v1=Mref.pOrigin->Vert[Mref.pOrigin->Idxs[3*i+1]];
			OneMapVertex& v2=Mref.pOrigin->Vert[Mref.pOrigin->Idxs[3*i+2]];
			V.x=(v0.x+v1.x+v2.x)/3;
			V.y=(v0.y+v1.y+v2.y)/3;
			V.z=(v0.z+v1.z+v2.z)/3;
			DrawDebugText(V, "%d", k);
		}*/
		//NvertexOut(MESH);
		FlipPages();
		ProcessMessages();
		IRS->ClearDevice(1,true,true);
		extern float fMapX;
		extern float fMapY;
		float fstep=0.05;
		if(GetKeyState(VK_LEFT)&0x8000){			
			fMapX-=fstep;
		}
		if(GetKeyState(VK_RIGHT)&0x8000){			
			fMapX+=fstep;;
		}
		if(GetKeyState(VK_UP)&0x8000){			
			fMapY-=fstep;
		}
		if(GetKeyState(VK_DOWN)&0x8000){			
			fMapY+=fstep;
		}
		void SetupCamera();
		SetupCamera();
	}while(true);
	return true;
}