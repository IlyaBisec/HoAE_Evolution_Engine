/*****************************************************************************/
/*	File:	sgBillboardGroup.h
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-21-2003
/*****************************************************************************/
#ifndef __SG_BILLBOARDGROUP_H__
#define __SG_BILLBOARDGROUP_H__

namespace sg{

const int c_DefaultMaxBillboardQuads = 64;
/*****************************************************************************/
/*	Class:	BillboardGroup
/*	Desc:	Group of equally oriented billboards 
/*****************************************************************************/
class BillboardGroup : public TransformNode
{
public:
						BillboardGroup();
	virtual				~BillboardGroup();
	virtual void		Render();

	void				SetBillboardTM( const Matrix3D& m );

	NODE(BillboardGroup,TransformNode,BBGR);

protected:
	void				ResetQuadBuffer();
	bool				AddQuad( const Vector3D& pos, float sizeX, float sizeY,
								 float rot, DWORD color, const Rct& uv );
	bool				AddQuad( const Vector3D& pos, float size, DWORD color, const Rct& uv );
	void				SetMaxQuads( int nQuads );

private:
	BaseMesh			m_QuadBuffer;
	int					m_NQuads;
	int					m_MaxQuads;
	Matrix3D			m_BillboardTM;
	Vector3D			m_LT, m_RT, m_LB, m_RB;		//  cached rotated corners

}; // class BillboardGroup

} // namespace sg

#endif // __SG_BILLBOARDGROUP_H__
