//////////////////////////////////////////////////////////////////////////
//
// Victor Levchenko, 15.02.2005
//
// Same usful templates and macros 
//
//////////////////////////////////////////////////////////////////////////

#ifndef __TL_UTILS__
#define __TL_UTILS__

#define SAFEDELETE(pOB)	if ( NULL != pOB ) { delete pOB; pOB = NULL; }

class	CID_BASE : public BaseClass
{
public:
	virtual void	operator=	( int _id )=0;	// for UNIVERSAL_EMPTY_ID
	virtual bool	operator==	( int _id )=0;	// for UNIVERSAL_EMPTY_ID
	virtual bool	operator!=	( int _id )=0;	// for UNIVERSAL_EMPTY_ID

	virtual	void	SetEmpty()=0;		// set UNIVERSAL_EMPTY_ID
	virtual	bool	GetEmpty()=0;		// return true if UNIVERSAL_EMPTY_ID
};



	
template< class T_FIRST, class T_SECOND >
class paraBC : public BaseClass
{
public:
	paraBC(){};
	paraBC( const T_FIRST& first, const T_SECOND& second ) : m_first(first), m_second(second) {};
public:
	T_FIRST		m_first;
	T_SECOND	m_second;
};



#endif//__TL_UTILS__