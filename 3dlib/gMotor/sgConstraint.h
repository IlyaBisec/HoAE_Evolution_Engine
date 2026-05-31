/*****************************************************************************/
/*	File:	sgConstraint.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	Dec 2003
/*****************************************************************************/
#ifndef __SGCONSTRAINT_H__
#define __SGCONSTRAINT_H__

namespace sg{
/*****************************************************************************/
/*	Class:	Constraint
/*	Desc:	Controls scene node state
/*****************************************************************************/
class Constraint : public Controller
{
public:
	
	NODE(Constraint,Controller,CSTR);
}; // class Constraint

/*****************************************************************************/
/*	Class:	PosConstraint
/*	Desc:	Constrains node position to the some other node's position
/*****************************************************************************/
class PosConstraint : public Constraint
{
	TransformNode*				m_pHost;
	TransformNode*				m_pConstrained;
	Vector3D					m_Delta;

public:
							PosConstraint() : m_Delta( Vector3D::null ) 
							{
								m_pHost			= NULL;
								m_pConstrained	= NULL;
							}
	virtual void			Render();
	virtual void			Expose( PropertyMap& pm );

	NODE(PosConstraint,Constraint,PCST);
}; // class PosConstraint

}; // namespace sg

#endif // __SGCONSTRAINT_H__
