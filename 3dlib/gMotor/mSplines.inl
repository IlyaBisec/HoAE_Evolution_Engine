/*****************************************************************************/
/*	File:	mSplines.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	07-15-2003
/*****************************************************************************/

_inl float LinearInterpolate( float t, float ta, float a, float tb, float b )
{
	return a + (t - ta) * (b - a) / (tb - ta);
} // LinearInterpolate

_inl float CosineInterpolate( float t, float a, float b ) 
{
	float ft = t * c_PI;
	ft = (1.0f - cosf( ft ))*0.5f;
	return a*(1.0f - ft) + b*ft;
} // CosineInterpolate

_inl float CatmullRomInterpolate( float t, float p0, float p1, float p2, float p3 )
{
	float t2 = t*t;
	float t3 = t2*t;
	return 0.5f*( 2*p1 + (-p0 + p2)*t + 
				 (2*p0 - 5*p1 + 4*p2 - p3)*t2 + 
				 ( -p0 + 3*p1 - 3*p2 + p3)*t3 );
} // CatmullRomInterpolate

_inl float	HermiteInterpolate( float t, float p0, float q0, float p1, float q1 )
{
	float t2 = t*t;
	float t3 = t2*t;
	return	p0*(1.0f - 3.0f*t2 + 2.0f*t3) + 
			p1*(3.0f*t2 - 2.0f*t3) + 
			q0*(t - 2.0f*t2 + t3) + 
			q1*(t3 - t2);
} // HermiteInterpolate

_inl float	BSplineInterpolate( float t, float p0, float p1, float p2, float p3 )
{
	float t2 = t*t;
	float t3 = t2*t;
	return (p0*(1.0f - 3.0f*t  + 3.0f*t2 - t3) + 
			p1*(4.0f - 6.0f*t2 + 3.0f*t3) +
			p2*(1.0f + 3.0f*t  + 3.0f*t2 - 3.0f*t3) +
			p3*t3)/6.0f;
} // BSplineInterpolate

_inl float	BezierInterpolate( float t, float p0, float p1, float p2, float p3 )
{
	float t2 = t*t;
	float t3 = t2*t;
	return	p0*(1.0f - 3.0f*t + 3.0f*t2 - t3) +
			p1*(3.0f*t - 6.0f*t2 + 3.0f*t3) +
			p2*(3.0f*t2 - 3.0f*t3) + 
			p3*t3;
} // BezierInterpolate


_inl float CatmullRomTangent( float t, float p0, float p1, float p2, float p3 )
{
	float t2 = t*t;
	return 0.5f*(	(-p0 + p2) + 
					2.0f*(2*p0 - 5*p1 + 4*p2 - p3)*t + 
					3.0f*( -p0 + 3*p1 - 3*p2 + p3)*t2 );
} // CatmullRomTangent

_inl float	HermiteTangent( float t, float p0, float q0, float p1, float q1 )
{
	float t2 = t*t;
	return	p0*(6.0f*t + 6.0f*t2) + 
			p1*(6.0f*t - 6.0f*t2) + 
			q0*(1.0f - 4.0f*t + 3.0f*t2) + 
			q1*(3.0f*t2 - 2.0f*t);
} // HermiteTangent

_inl float	BSplineTangent( float t, float p0, float p1, float p2, float p3 )
{
	float t2 = t*t;
	return (p0*(-3.0f + 6.0f*t - 3.0f*t2) + 
		p1*(-12.0f*t + 9.0f*t2) +
		p2*(3.0f + 6.0f*t - 9.0f*t2) +
		3.0f*p3*t2)/6.0f;
} // BSplineTangent

_inl float	BezierTangent( float t, float p0, float p1, float p2, float p3 )
{
	float t2 = t*t;
	return	p0*(-3.0f + 6.0f*t - 3.0f*t2) +
		p1*(3.0f - 12.0f*t + 9.0f*t2) +
		p2*(6.0f*t - 9.0f*t2) + 
		3.0f*p3*t2;
} // BezierTangent


_inl Vector3D CatmullRomInterpolate( float t, const Vector3D& p0, const Vector3D& p1,
									  const Vector3D& p2, const Vector3D& p3 )
{
	return Vector3D( CatmullRomInterpolate( t, p0.x, p1.x, p2.x, p3.x ),
					 CatmullRomInterpolate( t, p0.y, p1.y, p2.y, p3.y ),
					 CatmullRomInterpolate( t, p0.z, p1.z, p2.z, p3.z ) );
}

_inl Vector3D	HermiteInterpolate( float t, const Vector3D& p0, const Vector3D& q0,
								   const Vector3D& p1, const Vector3D& q1 )
{
	return Vector3D( HermiteInterpolate( t, p0.x, q0.x, p1.x, q1.x ),
					 HermiteInterpolate( t, p0.y, q0.y, p1.y, q1.y ),
					 HermiteInterpolate( t, p0.z, q0.z, p1.z, q1.z ) );
}

_inl Vector3D	BSplineInterpolate( float t, const Vector3D& p0, const Vector3D& p1,
								   const Vector3D& p2, const Vector3D& p3 )
{
	return Vector3D( BSplineInterpolate( t, p0.x, p1.x, p2.x, p3.x ),
					 BSplineInterpolate( t, p0.y, p1.y, p2.y, p3.y ),
					 BSplineInterpolate( t, p0.z, p1.z, p2.z, p3.z ) );
}

_inl Vector3D	BezierInterpolate( float t,	const Vector3D& p0, const Vector3D& p1,
											const Vector3D& p2, const Vector3D& p3 )
{
	return Vector3D( BezierInterpolate( t, p0.x, p1.x, p2.x, p3.x ),
					 BezierInterpolate( t, p0.y, p1.y, p2.y, p3.y ),
					 BezierInterpolate( t, p0.z, p1.z, p2.z, p3.z ) );
}


