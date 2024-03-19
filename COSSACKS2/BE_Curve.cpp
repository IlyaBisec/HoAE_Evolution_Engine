#include "stdheader.h"
#include "BE_HEADERS.h"

// aiLINE ////////////////////////////////////////////////////////////////
aiLINE::aiLINE(){
	Clean();
};

aiLINE::aiLINE(const float _x0,const float _y0, const float _x1, const float _y1){
	set(_x0,_y0,_x1,_y1);
};

void	aiLINE::Clean(){
	x0 = 0.f;
	y0 = 0.f;
	x1 = 1.f;
	y1 = 1.f;
};

bool	aiLINE::set(const float _x0,const float _y0, const float _x1, const float _y1){
	x0 = _x0;
	y0 = _y0;
	x1 = _x1;
	y1 = _y1;
	if (!check()){
		Clean();
		return	false;
	};
	return	true;
};

bool	aiLINE::check(){
	return ( (x0!=x1) || (y0!=y1) );
};

float	aiLINE::getY(const float X, const float _y0/* =0 */,const float _y1/* =1 */){
	if (!check())	return	0.0f;
	if (x0==x1) {
		if (X>=x0)	return _y1;
		else		return _y0;
	}else if (y0==y1){
		if (_y0<=y0&&y0<=_y1)	return  y0;
		else if (y0>_y1)		return _y1;
		else if (y0<_y0)		return _y0;
	}else{
		float Y = ( (X-x0) / (x1-x0) ) * (y1-y0) + y0;
		if (Y>=_y1)		return _y1;
		else if (Y<_y0)	return _y0;
		return	Y;
	};
};

float	aiLINE::getX(const aiLINE& line){
	float	Z = (y1-y0)*(line.x0-line.x1)+(line.y0-line.y1)*(x0-x1);	
	if (-0.001f<Z&&Z<0.001f) return 0.0f;
	float	X = ( (x0-x1)*(line.x0*(line.y0-line.y1)+line.y0*(line.x1-line.x0)) 
		- (line.x0-line.x1)*(x0*(y0-y1)+y0*(x0-x1)) )
		/Z;
	return	X;
};

// aiLINE2D //////////////////////////////////////////////////////////////
float aiLINE2D::getY(const float X, const float _y0/* =0 */,const float _y1/* =1 */){
	float B = lLINE.getX(rLINE);
	if (X <= B)	 return	lLINE.getY(X,_y0,_y1);
	if (X >  B)	 return	rLINE.getY(X,_y0,_y1);
};

//////////////////////////////////////////////////////////////////////////
