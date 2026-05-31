#ifndef __BE_Curve__
#define __BE_Curve__

// aiCURVE ///////////////////////////////////////////////////////////////
class aiCURVE
{
public:
	virtual float getY(const float X, const float _y0=0,const float _y1=1) = 0;
};

// aiLINE ////////////////////////////////////////////////////////////////
class aiLINE : public aiCURVE
{
public:
	aiLINE();
	aiLINE(const float _x0,const float _y0, const float _x1, const float _y1);
public:
	float x0,y0,x1,y1;
public:
	void	Clean();
	bool	set(const float _x0,const float _y0, const float _x1, const float _y1);
	bool	check();

	virtual float getY(const float X,const float _y0=0,const float _y1=1);
	float getX(const aiLINE& line);
};

// aiLINE2D //////////////////////////////////////////////////////////////
class aiLINE2D : public aiCURVE
{
public:
	aiLINE	lLINE;
	aiLINE	rLINE;

	virtual float getY(const float X, const float _y0=0,const float _y1=1);
};

//////////////////////////////////////////////////////////////////////////
#endif//__BE_Curve__