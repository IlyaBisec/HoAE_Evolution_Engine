/*****************************************************************
/*  File:   mQuaternion.inl                                          
/*  Author: Silver, Copyright (C) GSC Game World                 
/*  Date:   January 2002                                         
/*****************************************************************/

/*****************************************************************
/*	Quaternion implementation
/*****************************************************************/
_inl bool Quaternion::operator ==( const Quaternion& q ) const
{
	return v.isEqual( q.v ) && (fabs( s - q.s ) < c_SmallEpsilon);
}

/*---------------------------------------------------------------*
/*  Func:	Quaternion::norm2
/*	Desc:	Squared norm of the quaternion
/*---------------------------------------------------------------*/
_inl float Quaternion::norm2() const
{
	return s * s + v.norm2();
}

_inl void Quaternion::reverse()
{
	s = -s;
	v.reverse();
}

/*---------------------------------------------------------------*
/*  Func:	Quaternion::dot
/*	Desc:	finds dot product of two quaternions
/*---------------------------------------------------------------*/
_inl float Quaternion::dot( const Quaternion& q ) const
{
	return s * q.s + v.x * q.v.x + v.y * q.v.y + v.z * q.v.z;
}

/*---------------------------------------------------------------*
/*  Func:	Quaternion::setIdentity
/*	Desc:	sets identity quaternion
/*---------------------------------------------------------------*/
_inl void Quaternion::setIdentity()
{
	v.zero();
	s = 1.0f;
}//  Quaternion::setIdentity

/*---------------------------------------------------------------*
/*  Func:	Quaternion::operator *=
/*	Desc:	Quaternion composition
/*---------------------------------------------------------------*/
_inl const Quaternion& Quaternion::operator *=( const Quaternion& q )
{
	Vector3D tmp;
	tmp.cross( v, q.v ); 
	tmp.x += v.x * q.s + q.v.x * s;
	tmp.y += v.y * q.s + q.v.y * s;
	tmp.z += v.z * q.s + q.v.z * s;
	s = s * q.s - v.dot( q.v );
	v.copy( tmp );
	return *this;
}

/*---------------------------------------------------------------*
/*  Func:	Quaternion::axisToAxis
/*	Desc:	Takes two points on unit sphere an angle THETA apart, 
/*			returns quaternion that represents a rotation around 
/*			cross product by theta.
/*---------------------------------------------------------------*/
_inl void Quaternion::AxisToAxis( const Vector3D& from, const Vector3D& to )
{
	Vector3D tmp, half;
	tmp.copy( from );
	half.copy( to );

	tmp.normalize();
	half.normalize();

	half.add( tmp );
	half.normalize();

	v.cross( tmp, half );
	s = tmp.dot( half );
}//  axisToAxis

/*---------------------------------------------------------------*
/*  Func:	Quaternion::fromAxisAngle
/*	Desc:	Builds unit quaternion from rotational axis and angle
/*---------------------------------------------------------------*/
_inl void Quaternion::FromAxisAngle( const Vector3D& axis, float angle )
{
	float half = angle / 2.0f;
	s = cosf( half );
	v.copy( axis );
    v.normalize();
	v *= sinf( half );
    normalize();
}//  fromAxisAngle

_inl void Quaternion::ToAxisAngle( Vector3D& axis, float& angle ) const
{
    angle = acosf( s )*2.0f;
    axis = v;
    axis.normalize();
}//  ToAxisAngle

/*---------------------------------------------------------------*
/*  Func:	Quaternion::fromEulerAngles
/*	Desc:	Builds unit quaternion from Euler angles
/*---------------------------------------------------------------*/
_inl void Quaternion::FromEulerAngles( float rotX, float rotY, float rotZ )
{
	float sinP	= sin( rotX * 0.5f );
	float cosP	= cos( rotX * 0.5f );
	float sinY	= sin( rotY * 0.5f );
	float cosY	= cos( rotY * 0.5f );
	float sinR	= sin( rotZ * 0.5f );
	float cosR	= cos( rotZ * 0.5f );
	float cosPcosY = cosP * cosY;
	float sinPsinY = sinP * sinY;

	s	= cosR * cosPcosY + sinR * sinPsinY;
	v.x = sinR * cosPcosY - cosR * sinPsinY;
	v.y = cosR * sinP * cosY + sinR * cosP * sinY;
	v.z = cosR * cosP * sinY - sinR * sinP * cosY;
} // Quaternion::fromEulerAngles

/*---------------------------------------------------------------------------*/
/*	Func:	Quaternion::fromMatrix	
/*	Desc:	builds quaternion from rotational matrix
/*  Rmrk:	From Ken Shoemake's article "Quaternion Calculus and Fast Animation"
/*---------------------------------------------------------------------------*/
_inl void Quaternion::FromMatrix( const Matrix3D& rotM )
{	
	float trace = rotM.trace();
	float root;
	if (trace > 0.0f)
	{
		root = sqrtf( trace + 1.0f );
		s	 = root*0.5f;

		root = 0.5f/root;
		v.x	 = (rotM.e12 - rotM.e21) * root;
		v.y	 = (rotM.e20 - rotM.e02) * root;
		v.z	 = (rotM.e01 - rotM.e10) * root;
	}
	else
	{
		float maxC = tmax( rotM.e00, rotM.e11, rotM.e22 );
		if (maxC == rotM.e00)
		{
			root = sqrtf( 1.0f + rotM.e00 - rotM.e11 - rotM.e22 );
			v.x = 0.5f*root;
			if (root > c_SmallEpsilon) root = 0.5f/root;

			v.y = (rotM.e01 + rotM.e10)*root;
			v.z = (rotM.e02 + rotM.e20)*root;
			s   = (rotM.e12 - rotM.e21)*root;
		}
		else if (maxC == rotM.e11)
		{
			root = sqrtf( 1.0f + rotM.e11 - rotM.e00 - rotM.e22 );
			v.y = 0.5f * root;
			if (root > c_SmallEpsilon) root = 0.5f/root;

			v.x = (rotM.e01 + rotM.e10)*root;
			v.z = (rotM.e12 + rotM.e21)*root;
			s   = (rotM.e20 - rotM.e02)*root;
		}
		else if (maxC == rotM.e22)
		{
			root = sqrtf( 1.0f + rotM.e22 - rotM.e00 - rotM.e11 );
			v.z = 0.5f*root;
			if (root > c_SmallEpsilon) root = 0.5f/root;

			v.x = (rotM.e02 + rotM.e20)*root;
			v.y = (rotM.e12 + rotM.e21)*root;
			s   = (rotM.e01 - rotM.e10)*root;
		}
		else assert( false );
	}
} // Quaternion::fromMatrix

_inl void Quaternion::FromMatrix( const Matrix4D& rotM )
{
	Matrix3D m( rotM );
	FromMatrix( m );
}

_inl Vector3D Quaternion::EulerXYZ()
{
    Matrix3D m( *this );
	return m.EulerXYZ();
} // Quaternion::EulerXYZ

_inl float Quaternion::norm() const
{
	return sqrtf( norm2() );
} // Quaternion::norm

_inl void Quaternion::normalize()
{
	float n = norm();
	if (fabs( n ) > c_SmallEpsilon)
	{
		v /= n;
		s /= n;
	}
} // Quaternion::normalize


