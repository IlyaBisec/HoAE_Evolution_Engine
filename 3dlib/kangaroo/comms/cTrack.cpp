#include "comms.h"
#include "d3dx9math.h"
//#include "..\Quaternion.h"

namespace LW {
/*
======================================================================
range()

Given the value v of a periodic function, returns the equivalent value
v2 in the principal interval [lo, hi].  If i isn't NULL, it receives
the number of wavelengths between v and v2.

   v2 = v - i * (hi - lo)

For example, range( 3 pi, 0, 2 pi, i ) returns pi, with i = 1.
====================================================================== */

static float range( float v, float lo, float hi, int *i )
{
   float v2, r = hi - lo;

   if ( r == 0.0 ) {
      if ( i ) *i = 0;
      return lo;
   }

   v2 = lo + v - r * ( float ) floor(( double ) v / r );
   if ( i ) *i = -( int )(( v2 - v ) / r + ( v2 > v ? 0.5 : -0.5 ));

   return v2;
}

/*
======================================================================
hermite()

Calculate the Hermite coefficients.
====================================================================== */

static void hermite( float t, float *h1, float *h2, float *h3, float *h4 )
{
   float t2, t3;

   t2 = t * t;
   t3 = t * t2;

   *h2 = 3.0f * t2 - t3 - t3;
   *h1 = 1.0f - *h2;
   *h4 = t3 - t2;
   *h3 = *h4 - t2 + t;
}

/*
======================================================================
bezier()

Interpolate the value of a 1D Bezier curve.
====================================================================== */

static float bezier( float x0, float x1, float x2, float x3, float t )
{
   float a, b, c, t2, t3;

   t2 = t * t;
   t3 = t2 * t;

   c = 3.0f * ( x1 - x0 );
   b = 3.0f * ( x2 - x1 ) - c;
   a = x3 - x0 - c - b;

   return a * t3 + b * t2 + c * t + x0;
}

/*
======================================================================
bez2_time()

Find the t for which bezier() returns the input time.  The handle
endpoints of a BEZ2 curve represent the control points, and these have
(time, value) coordinates, so time is used as both a coordinate and a
parameter for this curve type.
====================================================================== */

static float bez2_time( float x0, float x1, float x2, float x3, float time,
   float *t0, float *t1 )
{
   float v, t;

   t = *t0 + ( *t1 - *t0 ) * 0.5f;
   v = bezier( x0, x1, x2, x3, t );
   if ( fabs( time - v ) > .0001f ) {
      if ( v > time )
         *t1 = t;
      else
         *t0 = t;
      return bez2_time( x0, x1, x2, x3, time, t0, t1 );
   }
   else
      return t;
}

#define SHAPE_TCB   0
#define SHAPE_HERM  1
#define SHAPE_BEZI  2
#define SHAPE_LINE  3
#define SHAPE_STEP  4
#define SHAPE_BEZ2  5

#define BEH_RESET      0
#define BEH_CONSTANT   1
#define BEH_REPEAT     2
#define BEH_OSCILLATE  3
#define BEH_OFFSET     4
#define BEH_LINEAR     5


/*
======================================================================
bez2()

Interpolate the value of a BEZ2 curve.
====================================================================== */

static float bez2( lwKey *key0, lwKey *key1, float time )
{
   float x, y, t, t0 = 0.0f, t1 = 1.0f;

   if ( key0->shape == SHAPE_BEZ2 )
      x = key0->time + key0->param[ 2 ];
   else
      x = key0->time + ( key1->time - key0->time ) / 3.0f;

   t = bez2_time( key0->time, x, key1->time + key1->param[ 0 ], key1->time,
      time, &t0, &t1 );

   if ( key0->shape == SHAPE_BEZ2 )
      y = key0->value + key0->param[ 3 ];
   else
      y = key0->value + key0->param[ 1 ] / 3.0f;

   return bezier( key0->value, y, key1->param[ 1 ] + key1->value, key1->value, t );
}

/*
======================================================================
outgoing()

Return the outgoing tangent to the curve at key0.  The value returned
for the BEZ2 case is used when extrapolating a linear pre behavior and
when interpolating a non-BEZ2 span.
====================================================================== */

static float outgoing( lwKey *key0, lwKey *key1 )
{
   float a, b, d, t, out;

   switch ( key0->shape )
   {
      case SHAPE_TCB:
         a = ( 1.0f - key0->tension )
           * ( 1.0f + key0->continuity )
           * ( 1.0f + key0->bias );
         b = ( 1.0f - key0->tension )
           * ( 1.0f - key0->continuity )
           * ( 1.0f - key0->bias );
         d = key1->value - key0->value;

         if ( key0->prev ) {
            t = ( key1->time - key0->time ) / ( key1->time - key0->prev->time );
            out = t * ( a * ( key0->value - key0->prev->value ) + b * d );
         }
         else
            out = b * d;
         break;

      case SHAPE_LINE:
         d = key1->value - key0->value;
         if ( key0->prev ) {
            t = ( key1->time - key0->time ) / ( key1->time - key0->prev->time );
            out = t * ( key0->value - key0->prev->value + d );
         }
         else
            out = d;
         break;

      case SHAPE_BEZI:
      case SHAPE_HERM:
         out = key0->param[ 1 ];
         if ( key0->prev )
            out *= ( key1->time - key0->time ) / ( key1->time - key0->prev->time );
         break;

      case SHAPE_BEZ2:
         out = key0->param[ 3 ] * ( key1->time - key0->time );
         if ( fabs( key0->param[ 2 ] ) > 1e-5f )
            out /= key0->param[ 2 ];
         else
            out *= 1e5f;
         break;

      case SHAPE_STEP:
      default:
         out = 0.0f;
         break;
   }

   return out;
}

/*
======================================================================
incoming()

Return the incoming tangent to the curve at key1.  The value returned
for the BEZ2 case is used when extrapolating a linear post behavior.
====================================================================== */

static float incoming( lwKey *key0, lwKey *key1 )
{
   float a, b, d, t, in;

   switch ( key1->shape )
   {
      case SHAPE_LINE:
         d = key1->value - key0->value;
         if ( key1->next ) {
            t = ( key1->time - key0->time ) / ( key1->next->time - key0->time );
            in = t * ( key1->next->value - key1->value + d );
         }
         else
            in = d;
         break;

      case SHAPE_TCB:
         a = ( 1.0f - key1->tension )
           * ( 1.0f - key1->continuity )
           * ( 1.0f + key1->bias );
         b = ( 1.0f - key1->tension )
           * ( 1.0f + key1->continuity )
           * ( 1.0f - key1->bias );
         d = key1->value - key0->value;

         if ( key1->next ) {
            t = ( key1->time - key0->time ) / ( key1->next->time - key0->time );
            in = t * ( b * ( key1->next->value - key1->value ) + a * d );
         }
         else
            in = a * d;
         break;

      case SHAPE_BEZI:
      case SHAPE_HERM:
         in = key1->param[ 0 ];
         if ( key1->next )
            in *= ( key1->time - key0->time ) / ( key1->next->time - key0->time );
         break;
         return in;

      case SHAPE_BEZ2:
         in = key1->param[ 1 ] * ( key1->time - key0->time );
         if ( fabs( key1->param[ 0 ] ) > 1e-5f )
            in /= key1->param[ 0 ];
         else
            in *= 1e5f;
         break;

      case SHAPE_STEP:
      default:
         in = 0.0f;
         break;
   }

   return in;
}

/*
======================================================================
evalEnvelope()

Given a list of keys and a time, returns the interpolated value of the
envelope at that time.
====================================================================== */

float evalEnvelope( lwEnvelope *env, float time )
{
   lwKey *key0, *key1, *skey, *ekey;
   float t, h1, h2, h3, h4, in, out, offset = 0.0f;
   int noff;


   /* if there's no key, the value is 0 */

   if ( env->nkeys == 0 ) return 0.0f;

   /* if there's only one key, the value is constant */

   if ( env->nkeys == 1 )
      return env->key->value;

   /* find the first and last keys */

   skey = ekey = env->key;
   while ( ekey->next ) ekey = ekey->next;

   /* use pre-behavior if time is before first key time */

   if ( time < skey->time ) {
      switch ( env->behavior[ 0 ] )
      {
         case BEH_RESET:
            return 0.0f;

         case BEH_CONSTANT:
            return skey->value;

         case BEH_REPEAT:
            time = range( time, skey->time, ekey->time, NULL );
            break;

         case BEH_OSCILLATE:
            time = range( time, skey->time, ekey->time, &noff );
            if ( noff % 2 )
               time = ekey->time - skey->time - time;
            break;

         case BEH_OFFSET:
            time = range( time, skey->time, ekey->time, &noff );
            offset = noff * ( ekey->value - skey->value );
            break;

         case BEH_LINEAR:
            out = outgoing( skey, skey->next )
                / ( skey->next->time - skey->time );
            return out * ( time - skey->time ) + skey->value;
      }
   }

   /* use post-behavior if time is after last key time */

   else if ( time > ekey->time ) {
      switch ( env->behavior[ 1 ] )
      {
         case BEH_RESET:
            return 0.0f;

         case BEH_CONSTANT:
            return ekey->value;

         case BEH_REPEAT:
            time = range( time, skey->time, ekey->time, NULL );
            break;

         case BEH_OSCILLATE:
            time = range( time, skey->time, ekey->time, &noff );
            if ( noff % 2 )
               time = ekey->time - skey->time - time;
            break;

         case BEH_OFFSET:
            time = range( time, skey->time, ekey->time, &noff );
            offset = noff * ( ekey->value - skey->value );
            break;

         case BEH_LINEAR:
            in = incoming( ekey->prev, ekey )
               / ( ekey->time - ekey->prev->time );
            return in * ( time - ekey->time ) + ekey->value;
      }
   }

   /* get the endpoints of the interval being evaluated */

   key0 = env->key;
   while ( time > key0->next->time )
      key0 = key0->next;
   key1 = key0->next;

   /* check for singularities first */

   if ( time == key0->time )
      return key0->value + offset;
   else if ( time == key1->time )
      return key1->value + offset;

   /* get interval length, time in [0, 1] */

   t = ( time - key0->time ) / ( key1->time - key0->time );

   /* interpolate */

   switch ( key1->shape )
   {
      case SHAPE_TCB:
      case SHAPE_BEZI:
      case SHAPE_HERM:
         out = outgoing( key0, key1 );
         in = incoming( key0, key1 );
         hermite( t, &h1, &h2, &h3, &h4 );
		 return h1 * key0->value + h2 * key1->value + h3 * out + h4 * in + offset;
      case SHAPE_BEZ2:
         return bez2( key0, key1, time ) + offset;

      case SHAPE_LINE:
         return key0->value + t * ( key1->value - key0->value ) + offset;

      case SHAPE_STEP:
         return key0->value + offset;

      default:
         return offset;
   }
}


};


static void CompElementDeriv(float pp, float p, float pn, 
float *ds, float *dd,float ksm, 
float ksp, float kdm, float kdp ) 
{ 
float delm, delp;
delm = p - pp; 
delp = pn - p; 
*ds = ksm*delm + ksp*delp; 
*dd = kdm*delm + kdp*delp; 
}
/*------------------------------------------------------- 
This computes the derivative at key, as a weighted average of the linear
slopes into and out of key, the weights being determined by the tension
and continuity parameters. Actually two derivatives are computed at key: 
"ds" is the "source derivative", or "arriving derivative" 
"dd" is the "destination derivative" or "departing derivative" 
----------------------------------------------------------*/ 
struct PosKey {
	float frame;
	cVec3 pos;
	float tens;
	float cont;
	float bias;

	cVec3 ds;
	cVec3 dd;
};
static void CompDeriv( PosKey *keyp, PosKey *key, PosKey *keyn ) 
{ 
int i; 
/* Full TCB computation */ 
float tm,cm,cp,bm,bp,tmcm,tmcp,ksm,ksp,kdm,kdp,delm,delp,c; 
float dt,fp,fn; 
/* fp,fn apply speed correction when continuity is 0.0 */ 
dt = .5f * (float)( keyn->frame - keyp->frame ); 
fp = ( (float)( key->frame - keyp->frame ) ) / dt; 
fn = ( (float)( keyn->frame - key->frame ) ) / dt; 
c = fabs( key->cont ); 
fp = fp + c - c * fp; 
fn = fn + c - c * fn; 
cm = 1.0f - key->cont; 
tm = 0.5f * ( 1.0f - key->tens ); 
cp = 2.0f - cm; 
bm = 1.0f - key->bias; 
bp = 2.0f - bm; 
tmcm = tm*cm; tmcp = tm*cp; 
ksm = tmcm*bp*fp; ksp = tmcp*bm*fp; 
kdm = tmcp*bp*fn; kdp = tmcm*bm*fn; 
for( i = 0; i <= 2; i++ ) { 
CompElementDeriv( keyp->pos[i], key->pos[i], keyn->pos[i], 
&key->ds[i], &key->dd[i], ksm, ksp, kdm, kdp ); 
} 
}
/* ----------------------------------------------------------- 
Compute the "a" and "b" terms at key "cur", which determine the incoming
and outgoing tangents (in quaternion space ) 
-----------------------------------------------------------*/
struct RotKey {
	float frame;
	float angle; // rad
	cVec3 axis;
	cQuat quat;
	float tens;
	float cont;
	float bias;

	cQuat a;
	cQuat b;
};
static int CompAB( RotKey *prev, RotKey *cur, RotKey *next ) 
{ 
int i; 
cQuat qprev,qnext,q,qzero;
cQuat qp,qm,qa,qb,qae,qbe;
float tm,cm,cp,bm,bp,tmcm,tmcp,ksm,ksp,kdm,kdp,c; 
float dt,fp,fn; 
if( prev != NULL ) { 
if( cur->angle > cMath::TwoPi-.00001f ) {
q.Set(cur->axis, 0.0f);
qm = cQuat::Ln(q);//qlog( q,qm );
} else { 
qprev = prev->quat;//qcopy( qprev, prev->quat ); 
if( cQuat::Dot(qprev, cur->quat) < 0 ) qprev = - qprev;//qnegate( qprev ); 
qm = cQuat::LnDif(qprev, cur->quat);//qlndif( qprev, cur->quat, qm ); 
} 
} 
if( next != NULL ) { 
if( next->angle > cMath::TwoPi-.00001 ) {
q.Set(next->axis, 0.0f);//COPY_POINT3( q, next->axis ); q[3] = 0; 
qp = cQuat::Ln(q);//qlog( q, qp ); 
} else {
qnext = next->quat;//qcopy( qnext, next->quat ); 
if(cQuat::Dot(qnext, cur->quat) < 0 ) qnext = -qnext;//qnegate( qnext ); 
qp = cQuat::LnDif(cur->quat, qnext);//qlndif( cur->quat, qnext, qp ); 
} 
} 
if( prev == NULL ) qm = qp;//qcopy( qm, qp ); 
if( next == NULL ) qp = qm;//qcopy( qp, qm ); 
fp = fn = 1.0f; 
cm = 1.0f - cur->cont; 
if( prev && next ) { 
dt = 0.5f * (float)(next->frame - prev->frame ); 
fp = ((float)(cur->frame - prev->frame))/dt; 
fn = ((float)(next->frame - cur->frame))/dt; 
c = fabs( cur->cont ); 
fp = fp + c - c * fp; 
fn = fn + c - c * fn; 
}
tm = .5f*(1.0f - cur->tens); 
cp = 2.0f - cm; 
bm = 1.0f - cur->bias; 
bp = 2.0f - bm; 
tmcm = tm * cm; 
tmcp = tm * cp; 
ksm = 1.0f - tmcm * bp * fp; 
ksp = -tmcp * bm * fp; 
kdm = tmcp * bp * fn; 
kdp = tmcm * bm * fn - 1.0f; 
for( i = 0; i < 4; i++ ) { 
qa[i] = .5f * ( kdm * qm[i] + kdp * qp[i] ); 
qb[i] = .5f * ( ksm * qm[i] + ksp * qp[i] ); 
}
qae = cQuat::Exp(qa);//qexp( qa, qae ); 
qbe = cQuat::Exp(qb);//qexp( qb, qbe ); 
cur->a = cQuat::Mul(cur->quat, qae);//qmul( cur->quat, qae, cur->a ); 
cur->b = cQuat::Mul(cur->quat, qbe);//qmul( cur->quat, qbe, cur->b ); 
return TRUE; 
}

//-----------------------------------------------------------------------------
// cTrack.ctor
//-----------------------------------------------------------------------------
/*cTrack::cTrack() {
	m_Ends = E_OPEN;
	m_CachedIndex = -1;
}

//-----------------------------------------------------------------------------
// cTrack::Free
//-----------------------------------------------------------------------------
void cTrack::Free() {
	m_Keys.Free();
	m_Segments.Free();
	lFunc.Free();
	m_CachedIndex = -1;
}

//-----------------------------------------------------------------------------
// cTrack::Copy
//-----------------------------------------------------------------------------
cTrack & cTrack::Copy(const cTrack &Src) {
	m_Keys.Copy(Src.m_Keys);
	m_CachedIndex = Src.m_CachedIndex;
	m_Ends = Src.m_Ends;
	m_CloseTime = Src.m_CloseTime;
	m_Segments.Copy(Src.m_Segments);
	lFunc.Copy(Src.lFunc);

	return *this;
}*/
/*
//-------------------------------------------------------------------------------------------------------------
// cTrack::Key::Set
//-------------------------------------------------------------------------------------------------------------
cTrack::Key & cTrack::Key::Set(const float Time, const cVec3 &Pos, const cAngles &Orient, const float Offset, const int Count) {
	this->Time = Time;
	this->Pos = Pos;
	this->Orient = Orient;
	CV0 = cVec3::Infinity;
	CV0IsValid = false;
	CV1 = cVec3::Infinity;
	CV1IsValid = false;
	this->Offset = Offset;
	this->Count = Count;

	return *this;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// cTrack::Key::Set
//-----------------------------------------------------------------------------------------------------------------------------------------------
cTrack::Key & cTrack::Key::Set(const float Time, const cVec3 &Pos, const cAngles &Orient, const float Offset, const cVec3 &CV0, const cVec3 &CV1) {
	this->Time = Time;
	this->Pos = Pos;
	this->Orient = Orient;
	this->CV0 = CV0;
	CV0IsValid = true;
	this->CV1 = CV1;
	CV1IsValid = true;
	this->Offset = Offset;
	return *this;
}
*/
//-----------------------------------------------------------------------------
// cTrack::AddKey
//-----------------------------------------------------------------------------
int cTrack::AddKey(const cTrack::Key &k) {
	int index = IndexForTime(k.Time);
	m_Keys.InsertAt(index, k);
//	Update();
	Setup(m_IsClosed);
	
	return index;
}

//-----------------------------------------------------------------------------
// cTrack::RemoveKey
//-----------------------------------------------------------------------------
void cTrack::RemoveKey(const int index) {
	m_Keys.RemoveAt(index);
	Setup(m_IsClosed);
//	Update();
}

//-----------------------------------------------------------------------------
// cTrack::CopyKeys
//-----------------------------------------------------------------------------
void cTrack::CopyKeys(const cTrack::Key *pSrc, const int nCount) {
	m_Keys.Free();
	for(int i = 0; i < nCount; i++) {
		AddKey(pSrc[i]);
	}
	Setup(m_IsClosed);
//	ReCreateCVs();
}

//-----------------------------------------------------------------------------
// cTrack::IndexForTime
//-----------------------------------------------------------------------------
int cTrack::IndexForTime(const float Time) const {
	if(m_CachedIndex >= 0 && m_CachedIndex <= m_Keys.Count()) {
		if(m_CachedIndex == 0) {
			if(Time <= m_Keys[m_CachedIndex].Time) {
				return m_CachedIndex;
			}
		} else if(m_CachedIndex == m_Keys.Count()) {
			if(Time > m_Keys[m_CachedIndex - 1].Time) {
				return m_CachedIndex;
			}
		} else if(Time > m_Keys[m_CachedIndex - 1].Time && Time <= m_Keys[m_CachedIndex].Time) {
			return m_CachedIndex;
		} else if(Time > m_Keys[m_CachedIndex].Time &&
			(m_CachedIndex + 1 == m_Keys.Count() || Time <= m_Keys[m_CachedIndex + 1].Time)) {
			m_CachedIndex++;
			return m_CachedIndex;
		}
	}

	int N = m_Keys.Count();
	int Mid = N;
	int Offset = 0, Res = 0;
	while(Mid > 0) {
		Mid = N >> 1;
		if(Time == m_Keys[Offset + Mid].Time) {
			return Offset + Mid;
		} else if(Time > m_Keys[Offset + Mid].Time) {
			Offset += Mid;
			N -= Mid;
			Res = 1;
		} else {
			N -= Mid;
			Res = 0;
		}
	}
	m_CachedIndex = Offset + Res;
	return m_CachedIndex;
}
/*
//-----------------------------------------------------------------------------
// cTrack::AdjustIndex
//-----------------------------------------------------------------------------
int cTrack::AdjustIndex(const int index) const {
	if(index < 0) {
		if(E_OPEN == m_Ends) {
			return 0;
		} else {
			return m_Keys.Count() + index;
		}
	} else if(index >= m_Keys.Count()) {
		if(E_OPEN == m_Ends) {
			return m_Keys.Count() - 1;
		} else {
			return index - m_Keys.Count();
		}
	} else {
		return index;
	}
}
*/
//-----------------------------------------------------------------------------
// cTrack::ShiftTime
//-----------------------------------------------------------------------------
void cTrack::ShiftTime(const float DeltaTime) {
	for(int i = 0; i < m_Keys.Count(); i++) {
		m_Keys[i].Time += DeltaTime;
	}
	Setup(m_IsClosed);
//	Update();
}

//-----------------------------------------------------------------------------
// cTrack::TranslateAll
//-----------------------------------------------------------------------------
/*void cTrack::TranslateAll(const cVec3 &Translation) {
	for(int i = 0; i < m_Keys.Count(); i++) {
		Key &k = m_Keys[i];
		k.Pos += Translation;
		if(k.CV0IsValid) {
			k.CV0 += Translation;
		}
		if(k.CV1IsValid) {
			k.CV1 += Translation;
		}
	}
	Update();
}*/

//-----------------------------------------------------------------------------
// cTrack::TranslateKey
//-----------------------------------------------------------------------------
void cTrack::TranslateKey(const int nKey, const cVec3 &Translation) {
	Key &k = m_Keys[nKey];
	k.Pos += Translation;
	Setup(m_IsClosed);
//	if(k.CV0IsValid) {
//		k.CV0 += Translation;
//	}
//	if(k.CV1IsValid) {
//		k.CV1 += Translation;
//	}
//	Update();
}

//-----------------------------------------------------------------------------
// cTrack::TranslateCV0
//-----------------------------------------------------------------------------
/*void cTrack::TranslateCV0(const int nKey, const cVec3 &Translation) {
	Key &k = m_Keys[nKey];
	assert(k.CV0IsValid);
	k.CV0 += Translation;
	if(k.CV1IsValid) {
		k.AdjustCV1();
	}
	Update();
}

//-----------------------------------------------------------------------------
// cTrack::TranslateCV1
//-----------------------------------------------------------------------------
void cTrack::TranslateCV1(const int nKey, const cVec3 &Translation) {
	Key &k = m_Keys[nKey];
	assert(k.CV1IsValid);
	k.CV1 += Translation;
	if(k.CV0IsValid) {
		k.AdjustCV0();
	}
	Update();
}

//-----------------------------------------------------------------------------
// cTrack::Key::AdjustCV0
//-----------------------------------------------------------------------------
void cTrack::Key::AdjustCV0() {
	assert(CV0IsValid);
	assert(CV1IsValid);
	const cVec3 u = cVec3::Normalize(CV1 - Pos);
	if(u.IsNormalized()) {
		CV0 = Pos - u * cVec3::Distance(Pos, CV0);
	}
}

//-----------------------------------------------------------------------------
// cTrack::Key::AdjustCV1
//-----------------------------------------------------------------------------
void cTrack::Key::AdjustCV1() {
	assert(CV0IsValid);
	assert(CV1IsValid);
	const cVec3 u = cVec3::Normalize(CV0 - Pos);
	if(u.IsNormalized()) {
		CV1 = Pos - u * cVec3::Distance(Pos, CV1);
	}
}*/

float KochanekBartelsAdjustOutCoeff(const float BeforeDuration, const float AfterDuration) {
	return 2.0f * BeforeDuration / (BeforeDuration + AfterDuration);
}

float KochanekBartelsAdjustInCoeff(const float BeforeDuration, const float AfterDuration) {
	return 2.0f * AfterDuration / (BeforeDuration + AfterDuration);
}

float Ease(float t, float a, float b) {
	float s = a + b;
	
	if(0.0f == s) {
		return t;
	}

	if(s > 1.0f) {
		a /= s;
		b /= s;
	}

	float k = 1.0f / (2.0f - a - b);
	if(t < a) {
		return ((k / a) * t * t);
	} else {
		if(t < 1.0f - b) {
			return (k * (2.0f * t - a));
		} else {
			t = 1.0f - t;
			return (1.0f - (k / b) * t * t);
		}
	}
}

bool Equals(const cQuat &q, const D3DXQUATERNION &Q) {
	return cMath::Equals(q.x, Q.x) && cMath::Equals(q.y, Q.y) && cMath::Equals(q.z, Q.z) && cMath::Equals(q.w, Q.w);
}

/*const cVec3 cTrack::GetCurrent2ndDerivative(const float Time) const {
	if(!m_Keys.Count()) {
		return cVec3::Zero;
	}

	if(m_Keys.Count() == 1 || Time <= m_Keys[0].Time) {
		return cVec3::Zero;
	}

	if(Time >= m_Keys.GetBack().Time) {
		return cVec3::Zero;
	}

	const float lCur = lFunc.GetCurrentValue(Time);
	const Place P = GetPlaceByLengthAccurate(lCur);
	const Segment &S = m_Segments[P.nSegment];
	return S.Curve3.Get2ndDerivative(P.LocalTime);
}*/

//-----------------------------------------------------------------------------
// cTrack::Setup
//-----------------------------------------------------------------------------
void cTrack::Setup(const bool IsClosed) {
	m_IsClosed = IsClosed;

	for(int c = CX; c < CLAST; c++) {
		lwKeys[c].Free();
	}

	LW::lwKey lw;
	for(int i = 0; i < m_Keys.Count(); i++) {
		const Key &k = m_Keys[i];
		
		ZeroMemory(&lw, sizeof(lw));
		lw.shape = SHAPE_TCB;
		lw.time = k.Time;
		lw.tension = k.Tension;
		lw.continuity = k.Continuity;
		lw.bias = k.Bias;

		lw.value = k.Pos.x;
		lwKeys[CX].Add(lw);

		lw.value = k.Pos.y;
		lwKeys[CY].Add(lw);

		lw.value = k.Pos.z;
		lwKeys[CZ].Add(lw);

		lw.value = k.Offset;
		lwKeys[CO].Add(lw);

		lw.value = k.Orient.Pitch;
		lwKeys[CPITCH].Add(lw);

		lw.value = k.Orient.Yaw;
		lwKeys[CYAW].Add(lw);

		lw.value = k.Orient.Roll;
		lwKeys[CROLL].Add(lw);
	}
	
	if(IsClosed) {
		const float tBefore = lwKeys[0][lwKeys[0].Count() - 2].time - lwKeys[0].GetBack().time + lwKeys[0][0].time;
		const float tAfter = lwKeys[0].GetBack().time + (lwKeys[0][1].time - lwKeys[0][0].time);
		for(int c = CX;  c < CLAST; c++) {
			lw = lwKeys[c][lwKeys[c].Count() - 2];
			lwKeys[c].InsertAt(0, lw);
			lwKeys[c][0].time = tBefore;
			lw = lwKeys[c][2];
			lwKeys[c].Add(lw);
			lwKeys[c].GetBack().time = tAfter;
		}
	}
	
	// Alt angles:
	for(int c = CPITCH; c <= CROLL; c++) {
		for(i = 0; i < lwKeys[c].Count(); i++) {
			lwKeys[c][i].value = cMath::AngleNormalize180(lwKeys[c][i].value);
			if(i > 0) {
				float d = cMath::AngleNormalize180(lwKeys[c][i].value - lwKeys[c][i - 1].value);
				lwKeys[c][i].value = lwKeys[c][i - 1].value + d;
			}
		}
	}

	for(c = CX; c < CLAST; c++) {
		for(i = 0; i < lwKeys[c].Count(); i++) {
			if(i > 0) {
				lwKeys[c][i].prev = &lwKeys[c][i - 1];
			}
			if(i < lwKeys[c].Count() - 1) {
				lwKeys[c][i].next = &lwKeys[c][i + 1];
			}
		}
	}
	
	for(c = CX; c < CLAST; c++) {
		lwEnvelopes[c].key = lwKeys[c].ToPtr();
		lwEnvelopes[c].nkeys = lwKeys[c].Count();
		lwEnvelopes[c].behavior[0] = lwEnvelopes[c].behavior[1] = BEH_CONSTANT;
	}
}

//-----------------------------------------------------------------------------
// cTrack::GetCurrentKey
//-----------------------------------------------------------------------------
const cTrack::Key cTrack::GetCurrentKey(const float Time) const {
	Key ck;
	ZeroMemory(&ck, sizeof(ck));
	ck.Time = Time;

	if(!m_Keys.Count()) {
		return ck;
	}

	if(m_Keys.Count() == 1 || Time <= m_Keys[0].Time) {
		return m_Keys[0];
	}

	if(Time >= m_Keys.GetBack().Time) {
		return m_Keys.GetBack();
	}

/*	Place P;
	float DerivTime;
	if(ParameterizePosByLength) {
		const float lCur = lFunc.GetCurrentValue(Time);
		P = GetPlaceByLengthAccurate(lCur);
	} else {
		P = GetPlaceByTimeAccurate(Time);
	}
	const Segment &S = m_Segments[P.nSegment];
	ck.Pos = S.Curve3.GetValue(P.LocalTime);
	if(ParameterizePosByLength && ParameterizeDerivativesByLength) {
		DerivTime = cMath::Lerp(S.tStart, S.tEnd, P.LocalTime);
	} else {
		DerivTime = Time;
	}*/


	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	
	float x = LW::evalEnvelope(&lwEnvelopes[CX], Time);
	float y = LW::evalEnvelope(&lwEnvelopes[CY], Time);
	float z = LW::evalEnvelope(&lwEnvelopes[CZ], Time);
	float o = LW::evalEnvelope(&lwEnvelopes[CO], Time);
	float Pitch = LW::evalEnvelope(&lwEnvelopes[CPITCH], Time);
	float Yaw = LW::evalEnvelope(&lwEnvelopes[CYAW], Time);
	float Roll = LW::evalEnvelope(&lwEnvelopes[CROLL], Time);
	
	ck.Pos.Set(x, y, z);
	ck.Offset = o;
	ck.Orient.Set(Pitch, Yaw, Roll);

	// cSpline:
/*	cTCBSpline<cVec3> SPos;
	cTCBSpline<cAngles> SOrient;
	cTCBSpline<float> SOffset;
	for(int i = 0; i < m_Keys.Count(); i++) {
		const Key &k = m_Keys[i];
		SPos.AddValue(k.Time, k.Pos, k.Tension, k.Continuity, k.Bias);
		SOrient.AddValue(k.Time, k.Orient, k.Tension, k.Continuity, k.Bias);
		SOffset.AddValue(k.Time, k.Offset, k.Tension, k.Continuity, k.Bias);
	}
	cVec3 tPos = SPos.GetCurrentValue(Time);
	assert(cVec3::Equals(ck.Pos, tPos));
	cAngles tOrient = SOrient.GetCurrentValue(Time);
	cAngles::Equals(ck.Orient, tOrient);
	float tOffset = SOffset.GetCurrentValue(Time);
	cMath::Equals(ck.Offset, tOffset);*/


	return ck;
	
/*	int index = IndexForTime(Time);
	if(index >= m_Keys.Count()) {
		ck.Orient = m_Keys.GetBack().Orient;
	} else {
		const Key &k2 = m_Keys[index];
		if(Time == k2.Time) {
			ck.Orient = k2.Orient;
		} else {
			int i0 = cMath::Clamp(index - 2, 0, m_Keys.Count() - 1);
			int i1 = cMath::Clamp(index - 1, 0, m_Keys.Count() - 1);
			int i3 = cMath::Clamp(index + 1, 0, m_Keys.Count() - 1);

			const Key &k0 = m_Keys[i0];
			const Key &k1 = m_Keys[i1];
			const Key &k3 = m_Keys[i3];

			const cQuat q0 = k0.Orient.ToQuat();
			const cQuat q1 = k1.Orient.ToQuat();
			const cQuat q2 = k2.Orient.ToQuat();
			const cQuat q3 = k3.Orient.ToQuat();

			D3DXQUATERNION Q0, Q1, Q2, Q3;
			CopyMemory((float *)&Q0, q0.ToPtr(), sizeof(D3DXQUATERNION));
			CopyMemory((float *)&Q1, q1.ToPtr(), sizeof(D3DXQUATERNION));
			CopyMemory((float *)&Q2, q2.ToPtr(), sizeof(D3DXQUATERNION));
			CopyMemory((float *)&Q3, q3.ToPtr(), sizeof(D3DXQUATERNION));

			cQuat a, b, c, q;
			D3DXQUATERNION A, B, C, Q;

			D3DXQuaternionSquadSetup(&A, &B, &C, &Q0, &Q1, &Q2, &Q3);
			cQuat::SquadSetup(q0, q1, q2, q3, a, b, c);

			assert(Equals(q1, Q1));
			assert(Equals(a, A));
			assert(Equals(b, B));
			assert(Equals(c, C));

			float t = cMath::LerperClamp01(k1.Time, k2.Time, Time);
			assert(cMath::IsZeroToOneExact(t));

			D3DXQuaternionSquad(&Q, &Q1, &A, &B, &C, t);
			q = cQuat::Squad(q1, a, b, c, t);

			assert(Equals(q, Q));
//			CopyMemory(q.ToPtr(), (float *)&Q, sizeof(q));
			ck.Orient = q.ToAngles();
		}
	}

	return ck;*/
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/*
	if(Time <= m_Keys[0].Time) {
		return m_Keys[0];
	}
	if(Time >= m_Keys.GetBack().Time) {
		return m_Keys.GetBack();
	}

	const float lCur = lFunc.GetCurrentValue(Time);
	R.l = lCur;
	const Place FixedPlace = GetPlaceByLengthAccurate(lCur);

	int n = FixedPlace.nSegment;
	const Key &k0 = m_Keys[n];
	const Key &k1 = m_Keys[n + 1];
*/
//	int n = 0;
//	while(Time > m_Keys[n + 1].Time) {
//		n++;
//	}
//	const Key &k0 = m_Keys[n];
//	const Key &k1 = m_Keys[n + 1];

//	const float t = cMath::LerperClamp01(k0.Time, k1.Time, Time);

//	k.Pos = cVec3::Lerp(k0.Pos, k1.Pos, t);
//	k.Offset = cMath::Lerp(k0.Offset, k1.Offset, t);

/*	cBezierCurve1 C1;
//	cBezierCurve3 C3;

	if(m_Keys.Count() <= 2) {
		assert(m_Keys.Count() == 2);
//		const cVec3 OutIn3 = cMath::KochanekBartelsSetupStartOutEndIn(m_Keys[0].Pos, m_Keys[1].Pos);
//		k.Pos = cMath::Hermite(m_Keys[0].Pos, OutIn3, m_Keys[1].Pos, OutIn3, t); // <<<<<<<<<<<<<<<<<<<<<<<<
//		C3.SetFromEnds(m_Keys[0].Pos, OutIn3, m_Keys[1].Pos, OutIn3);
//		assert(cVec3::Equals(k.Pos, C3.GetValue(t))); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//		k.Pos = C3.GetValue(t);
		
		const float OutIn = cMath::TCBSetupStartOutEndIn(m_Keys[0].Offset, m_Keys[1].Offset);
//		k.Offset = cMath::Hermite(m_Keys[0].Offset, OutIn, m_Keys[1].Offset, OutIn, t); // <<<<<<<<<<<<<<<<<<<<<<<<<<
		C1.SetFromEnds(m_Keys[0].Offset, OutIn, m_Keys[1].Offset, OutIn);
//		assert(cMath::Equals(k.Offset, C1.GetValue(t))); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		R.Offset = C1.GetValue(FixedPlace.LocalTime);
	} else {
//		cList<cVec3> Ins3, Outs3;
//		cVec3 In3, Out3;

//		cList<cQuat> InsQ, OutsQ;
//		cQuat InQ, OutQ;

		cList<float> Ins, Outs;
		float In, Out;
		for(int i = 1; i < m_Keys.Count() - 1; i++) {
			const Key &Prev = m_Keys[i - 1];
			const Key &Cur = m_Keys[i];
			const Key &Next = m_Keys[i + 1];
//			cMath::KochanekBartelsSetup(Prev.Pos, Cur.Pos, Next.Pos, In3, Out3);
//			Ins3.Add(In3);
//			Outs3.Add(Out3);
			
			cMath::TCBSetup(Prev.Offset, Cur.Offset, Next.Offset, In, Out);
			Ins.Add(In);
			Outs.Add(Out);

//			cMath::TCBSetup(Prev.Orient.ToQuat(), Cur.Orient.ToQuat(), Next.Orient.ToQuat(), InQ, OutQ);
//			InsQ.Add(InQ);
//			OutsQ.Add(OutQ);
		}
//		Outs3.InsertAt(0, cMath::KochanekBartelsSetupStartOut(m_Keys[0].Pos, m_Keys[1].Pos, Ins3[0]));
//		Ins3.Add(cMath::KochanekBartelsSetupEndIn(m_Keys[m_Keys.Count() - 2].Pos, Outs3.GetBack(), m_Keys.GetBack().Pos));

		Outs.InsertAt(0, cMath::TCBSetupStartOut(m_Keys[0].Offset, m_Keys[1].Offset, Ins[0]));
		Ins.Add(cMath::TCBSetupEndIn(m_Keys[m_Keys.Count() - 2].Offset, Outs.GetBack(), m_Keys.GetBack().Offset));

//		OutsQ.InsertAt(0, cMath::TCBSetupStartOut(m_Keys[0].Orient.ToQuat(), m_Keys[1].Orient.ToQuat()));
//		InsQ.Add(cMath::TCBSetupEndIn(m_Keys[m_Keys.Count() - 2].Orient.ToQuat(), OutsQ.GetBack()));

		for(int i = 1; i < m_Keys.Count() - 1; i++) {
			const Key &Prev = m_Keys[i - 1];
			const Key &Cur = m_Keys[i];
			const Key &Next = m_Keys[i + 1];
			float kin = cMath::TCBAdjInCoeff(Prev.Time, Cur.Time, Next.Time);
			float kout = cMath::TCBAdjOutCoeff(Prev.Time, Cur.Time, Next.Time);
//			if(GetKeyState(VK_CONTROL) < 0) {
//				Outs3[i] *= kout;
//				Ins3[i - 1] *= kin;
//				Outs[i] *= kout;
//				Ins[i - 1] *= kin;

//				OutsQ[i] *= kout;
//				InsQ[i - 1] *= kin;
//			}
		}*/
	
//		k.Pos = cMath::Hermite(k0.Pos, Outs3[n], k1.Pos, Ins3[n], t); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//		R.Offset = cMath::Hermite(k0.Offset, Outs[n], k1.Offset, Ins[n], FixedPlace.LocalTime); // <<<<<<<<<<<<<<<<<<<<<<<<<<<

//		cQuat QQQ = cMath::Hermite(k0.Orient.ToQuat(), OutsQ[n], k1.Orient.ToQuat(), InsQ[n], FixedPlace.LocalTime);
//		k.Orient = QQQ.ToAngles();

//		cVec3 HP1stD = cMath::HermiteFirstDerivative(k0.Pos, Outs3[n], k1.Pos, Ins3[n], t);
//		cVec3 HP2ndD = cMath::HermiteSecondDerivative(k0.Pos, Outs3[n], k1.Pos, Ins3[n], t);
//		float HO1stD = cMath::HermiteFirstDerivative(k0.Offset, Outs[n], k1.Offset, Ins[n], t);
//		float HO2ndD = cMath::HermiteSecondDerivative(k0.Offset, Outs[n], k1.Offset, Ins[n], t);
//
//		C3.SetFromEnds(k0.Pos, Outs3[n], k1.Pos, Ins3[n]);
//		C1.SetFromEnds(k0.Offset, Outs[n], k1.Offset, Ins[n]);

//		cVec3 BP = C3.GetValue(t);
//		float BO = C1.GetValue(t);
//		cVec3 BP1stD = C3.Get1stDerivative(t);
//		cVec3 BP2ndD = C3.Get2ndDerivative(t);
//		float BO1stD = C1.Get1stDerivative(t);
//		float BO2ndD = C1.Get2ndDerivative(t);

//		assert(cVec3::Equals(k.Pos, BP));
//		assert(cMath::Equals(k.Offset, BO));
		
//		assert(cVec3::Equals(HP1stD, BP1stD));
//		assert(cMath::Equals(HO1stD, BO1stD));

//		assert(cVec3::Equals(HP2ndD, BP2ndD));
//		assert(cMath::Equals(HO2ndD, BO2ndD));

//		assert(cVec3::Equals(C3.GetValue(0), C3.GetValue0()));
//		assert(cVec3::Equals(C3.GetValue(1), C3.GetValue1()));

//		assert(cVec3::Equals(C3.Get1stDerivative(0), C3.Get1stDerivative0()));
//		assert(cVec3::Equals(C3.Get1stDerivative(1), C3.Get1stDerivative1()));

//		assert(cVec3::Equals(C3.Get2ndDerivative(0), C3.Get2ndDerivative0()));
//		assert(cVec3::Equals(C3.Get2ndDerivative(1), C3.Get2ndDerivative1()));

/*		Key k0 = m_Keys[AdjustIndex(n - 1)];
		Key k1 = m_Keys[AdjustIndex(n)];
		Key k2 = m_Keys[AdjustIndex(n + 1)];
		Key k3 = m_Keys[AdjustIndex(n + 2)];

		cQuat q0 = k0.Orient.ToQuat();
		cQuat q1 = k1.Orient.ToQuat();
		cQuat q2 = k2.Orient.ToQuat();
		cQuat q3 = k3.Orient.ToQuat();

		D3DXQUATERNION Q0, Q1, Q2, Q3;
		Q0.x = q0.x, Q0.y = q0.y, Q0.z = q0.z, Q0.w = q0.w;
		Q1.x = q1.x, Q1.y = q1.y, Q1.z = q1.z, Q1.w = q1.w;
		Q2.x = q2.x, Q2.y = q2.y, Q2.z = q2.z, Q2.w = q2.w;
		Q3.x = q3.x, Q3.y = q3.y, Q3.z = q3.z, Q3.w = q3.w;

		D3DXQUATERNION A, B, C, Qt;
		D3DXQuaternionSquadSetup(&A, &B, &C, &Q0, &Q1, &Q2, &Q3);

		D3DXQuaternionSquad(&Qt, &Q1, &A, &B, &C, FixedPlace.LocalTime);
		cQuat q(Qt.x, Qt.y, Qt.z, Qt.w);
//		cQuat q = cQuat::Squad(q1, a, b, c, FixedPlace.LocalTime);
		R.Orient = q.ToAngles();
*/
		
//		k0.Orient.Normalize180();
//		k1.Orient.Normalize180();
//		k2.Orient.Normalize180();
//		k3.Orient.Normalize180();
/*		D3DXQUATERNION Q0, Q1, Q2, Q3;
		D3DXQuaternionRotationYawPitchRoll(&Q0, cMath::Rad(k0.Orient.Yaw), cMath::Rad(k0.Orient.Pitch), cMath::Rad(k0.Orient.Roll));
		D3DXQuaternionRotationYawPitchRoll(&Q1, cMath::Rad(k1.Orient.Yaw), cMath::Rad(k1.Orient.Pitch), cMath::Rad(k1.Orient.Roll));
		D3DXQuaternionRotationYawPitchRoll(&Q2, cMath::Rad(k2.Orient.Yaw), cMath::Rad(k2.Orient.Pitch), cMath::Rad(k2.Orient.Roll));
		D3DXQuaternionRotationYawPitchRoll(&Q3, cMath::Rad(k3.Orient.Yaw), cMath::Rad(k3.Orient.Pitch), cMath::Rad(k3.Orient.Roll));

		D3DXQUATERNION A, B, C, Qt;
		D3DXQuaternionSquadSetup(&A, &B, &C, &Q0, &Q1, &Q2, &Q3);
		D3DXQuaternionSquad(&Qt, &Q1, &A, &B, &C, FixedPlace.LocalTime);

		D3DXVECTOR3 Axis;
		float Angle;
		D3DXQuaternionToAxisAngle(&Qt, &Axis, &Angle);

		k.Orient.SetZero();
		k.Orient.Yaw = cMath::Deg(Angle);

//		cQuat q(Qt.x, Qt.y, Qt.z, Qt.w);
//		k.Orient = q.ToAngles();



//		cList<float> Times;
//		cList<cQuat> Quats;
//		for(int i = 0; i < m_Keys.Count(); i++) {
//			const Key &k = m_Keys[i];
//			Times.Add(k.Time);
//			Quats.Add(k.Orient.ToQuat());
//		}

//		cQuat rr;
//		const Segment &S = m_Segments[FixedPlace.nSegment];
//		float tt = cMath::Lerp(S.tStart, S.tEnd, FixedPlace.LocalTime);
//		CatmullRom_Evaluate(tt, Times.Count(), Times.ToPtr(), Quats.ToPtr()->ToPtr(), rr.ToPtr());
//		cQuat tst = cQuat::_CatmullRomEvaluate(tt, Times.Count(), Times.ToPtr(), Quats.ToPtr());
//
//		assert(cQuat::Equals(rr, tst));
//		k.Orient = rr.ToAngles();

//		cQuat rr;
//		QSpline(q0.ToPtr(), q1.ToPtr(), q2.ToPtr(), q3.ToPtr(), FixedPlace.LocalTime, rr.ToPtr());
//		cQuat tst = cQuat::_QSpline(q0, q1, q2, q3, FixedPlace.LocalTime);
//		assert(cQuat::Equals(rr, tst, 0.01f));

//		const float kin = cMath::TCBAdjOutCoeff(k0.Time, k1.Time, k2.Time);
//		const float kout = cMath::TCBAdjInCoeff(k1.Time, k2.Time, k3.Time);
*/
/*		assert(q0.IsNormalized());
		assert(q1.IsNormalized());
		assert(q2.IsNormalized());
		assert(q3.IsNormalized());
		
		

		cQuat a, b, c;

//		cQuat::SquadSetupMs(q0, q1, q2, q3, a, b, c);

		a.x = A.x, a.y = A.y, a.z = A.z, a.w = A.w;
		b.x = B.x, b.y = B.y, b.z = B.z, b.w = B.w;
		c.x = C.x, c.y = C.y, c.z = C.z, c.w = C.w;

		assert(a.IsNormalized());
		assert(b.IsNormalized());
		assert(c.IsNormalized());

//		assert(cMath::Equals(C.x, Q2.x) && cMath::Equals(C.y, Q2.y) && cMath::Equals(C.z, Q2.z) && cMath::Equals(C.w, Q2.w));

//		if(GetKeyState(VK_CONTROL) < 0) {
//			if(kout) {
//				A *= kout;
//			}
//			if(kin) {
//				B *= kin;
//			}
//		}

		D3DXQuaternionSquad(&Qt, &Q1, &A, &B, &C, FixedPlace.LocalTime);
		cQuat q = cQuat::Squad(q1, a, b, c, FixedPlace.LocalTime);
		k.Orient = q.ToAngles();*/

		// TEST! TEST! TEST!
/*		PosKey pk;
		cList<PosKey> pks;
		RotKey rk;
		cList<RotKey> rks;
		for(int i = 0; i < m_Keys.Count(); i++) {
			// PosKey:
			ZeroMemory(&pk, sizeof(pk));
			const Key &k = m_Keys[i];
			pk.frame = k.Time;
			pk.pos = k.Pos;
			pks.Add(pk);
			// RotKey:
			ZeroMemory(&rk, sizeof(rk));
			rk.frame = k.Time;
			rk.quat = k.Orient.ToQuat();
			const cRotation r = rk.quat.ToRotation();
			rk.axis = r.GetAxis();
			rk.angle = cMath::Rad(r.GetAngle());
			rks.Add(rk);
		}
//		for(i = 0; i < pks.Count(); i++) {
//			int iPrev = cMath::Clamp(i - 1, 0, pks.Count() - 1);
//			int iNext = cMath::Clamp(i + 1, 0, pks.Count() - 1);
//			CompDeriv(&pks[iPrev], &pks[i], &pks[iNext]);
//		}
//		int iNext = cMath::Clamp(FixedPlace.nSegment + 1, 0, pks.Count() - 1);
//		k.Pos = cMath::Hermite(pks[FixedPlace.nSegment].pos, pks[FixedPlace.nSegment].ds, pks[iNext].pos, pks[iNext].dd, FixedPlace.LocalTime);
		for(i = 0; i < rks.Count(); i++) {
			int iPrev = cMath::Clamp(i - 1, 0, rks.Count() - 1);
			int iNext = cMath::Clamp(i + 1, 0, rks.Count() - 1);
			RotKey *pPrev = iPrev == i ? NULL : &rks[iPrev];
			RotKey *pNext = iNext == i ? NULL : &rks[iNext];
			CompAB(pPrev, &rks[i], pNext);
		}
		int iNext = cMath::Clamp(FixedPlace.nSegment + 1, 0, rks.Count() - 1);
		RotKey &rc = rks[FixedPlace.nSegment];
		RotKey &rn = rks[iNext];
		k.Orient = cQuat::_Squad(rc.quat, rc.b, rn.a, rn.quat, FixedPlace.LocalTime).ToAngles();*/
//	}

//	Place P = GetPlaceByTimeAccurate(Time);
//	const Segment &S = m_Segments[FixedPlace.nSegment];
//	R.Pos = S.Curve3.GetValue(FixedPlace.LocalTime);
//	k.Orient = cQuat::Bezier(S.Q0, S.A, S.B, S.Q1, P.LocalTime).ToAngles();
//	k.Offset = S.Curve1.GetValue(P.LocalTime);
//	k.Offset = GetKeyState(VK_SHIFT) < 0 ? cMath::Lerp(S.Curve1.GetValue0(), S.Curve1.GetValue1(), P.LocalTime) : S.Curve1.GetValue(P.LocalTime);
///	k.Offset = S.lFunc.GetValue(P.LocalTime);
//	return R;
}

//-----------------------------------------------------------------------------
// cTrack::GetPlaceByTimeAccurate
//-----------------------------------------------------------------------------
/*const cTrack::Place cTrack::GetPlaceByTimeAccurate(const float Time) const {
	Place P;
	ZeroMemory(&P, sizeof(P));

	if(m_Segments.Count()) {
		if(Time <= m_Segments[0].tStart) {
		} else if(Time >= m_Segments.GetBack().tEnd) {
			P.nSegment = m_Segments.Count() - 1;
			P.LocalTime = 1.0f;
		} else {
			assert(cMath::IsInRangeExact(Time, m_Segments[0].tStart, m_Segments.GetBack().tEnd));
			
			while(Time > m_Segments[P.nSegment].tEnd) {
				P.nSegment++;
			}
			
			const Segment &S = m_Segments[P.nSegment];
			assert(cMath::IsInRangeExact(Time, S.tStart, S.tEnd));
			
			const float t = cMath::LerperClamp01(S.tStart, S.tEnd, Time);
			P.LocalTime = t;
//			const float l = cMath::Lerp(0.0f, S.lEnd - S.lStart, t);
//			const float L = S.lFunc.GetValue(t);
//			const float l = cMath::Clamp(L - (S.AccumLength - S.Length), 0.0f, S.Length);

//			P.LocalTime = S.Curve3.CalcParameterForLength(l);
		}
	}
	return P;
}

float cTrack::GetTotalLength() const {
	if(!m_Segments.Count()) {
		return 0.0f;
	}
	return m_Segments.GetBack().lEnd;
}


const cTrack::Place cTrack::GetPlaceByLength(const float l) const {
	const float L = cMath::Clamp(l, 0.0f, GetTotalLength());

	Place P;
	ZeroMemory(&P, sizeof(P));
	
	if(m_Segments.Count()) {
		int nSegment = 0;

		while(L > m_Segments[nSegment].lEnd) {
			nSegment++;
		}

		const Segment &S = m_Segments[nSegment];
		assert(L >= S.lStart && L <= S.lEnd);
		P.nSegment = nSegment;
		P.LocalTime = cMath::LerperClamp01(S.lStart, S.lEnd, L);
	}
	return P;
}

const cTrack::Place cTrack::GetPlaceByLengthAccurate(const float l) const {
	const float L = cMath::Clamp(l, 0.0f, GetTotalLength());
	
	Place P = GetPlaceByLength(L);
	const Segment &S = m_Segments[P.nSegment];
	assert(L >= S.lStart && L <= S.lEnd);
	P.LocalTime = m_Segments[P.nSegment].Curve3.CalcParameterForLength(L - S.lStart);

	return P;
}

//-----------------------------------------------------------------------------
// cTrack::Close
//-----------------------------------------------------------------------------
void cTrack::Close(const float Time) {
	m_Ends = E_CLOSED;
	m_CloseTime = Time;
	Update();
}

//-----------------------------------------------------------------------------
// cTrack::GetTotalLength
//-----------------------------------------------------------------------------
/*float cTrack::GetTotalLength() const {
	float L = 0.0f;
	for(int i = 0; i < m_Segments.Count(); i++) {
		const Segment &S = m_Segments[i];
		L += S.lEnd - S.lStart;
	}
	return L;
}*/

//-----------------------------------------------------------------------------
// cTrack::MakeTimeUniform
//-----------------------------------------------------------------------------
/*void cTrack::MakeTimeUniform(const float TotalTime) {
	if(m_Keys.Count() <= 1) {
		return;
	}
	assert(m_Segments.Count());

	float L = GetTotalLength();
	m_Keys[0].Time = 0.0f;
	float AccumLength = 0.0f;
	for(int i = 0; i < m_Segments.Count(); i++) {
		int iKey = AdjustIndex(i + 1);
		if(iKey != 0) {
			AccumLength += m_Segments[i].lEnd - m_Segments[i].lStart;
			m_Keys[iKey].Time = cMath::Lerp(0.0f, TotalTime, AccumLength / L);
		} else {
			assert(IsClosed());
			Close(TotalTime);
		}
	}
	Update();
}

//----------------------------------------------------------------------------------------------------------
// cTrack::GetKeyTriplet
//----------------------------------------------------------------------------------------------------------
void cTrack::GetKeyTriplet(const int nKey, cTrack::Key **ppCur, cTrack::Key **ppPrev, cTrack::Key **ppNext) {
	if(ppCur) {
		*ppCur = &m_Keys[nKey];
	}
	int iPrev = AdjustIndex(nKey - 1);
	int iNext = AdjustIndex(nKey + 1);
	if(ppPrev) {
		*ppPrev = iPrev != nKey ? &m_Keys[iPrev] : NULL;
	}
	if(ppNext) {
		*ppNext = iNext != nKey ? &m_Keys[iNext] : NULL;
	}
}

//-----------------------------------------------------------------------------
// cTrack::ReCreateCVs
//-----------------------------------------------------------------------------
void cTrack::ReCreateCVs() {
	if(m_Keys.Count() < 1) {
		return;
	}

	Key *pCur, *pPrev, *pNext;
	for(int iCur = 0; iCur < m_Keys.Count(); iCur++) {
		GetKeyTriplet(iCur, &pCur, &pPrev, &pNext);
		// Cutting ends off:
		if(!pPrev) {
			pCur->CV0 = cVec3::Infinity;
			pCur->CV0IsValid = false;
		}
		if(!pNext) {
			pCur->CV1 = cVec3::Infinity;
			pCur->CV1IsValid = false;
		}
		// Create CVs:
		if(!pPrev && pNext) {
			pCur->CV1 = cVec3::Lerp(pCur->Pos, pNext->Pos, 0.33f);
			pCur->CV1IsValid = true;
		} else if(pPrev && !pNext) {
			pCur->CV0 = cVec3::Lerp(pCur->Pos, pPrev->Pos, 0.33f);
			pCur->CV0IsValid = true;
		} else if(pPrev && pNext) {
			const cVec3 CurPrev = pPrev->Pos - pCur->Pos;
			const cVec3 CurNext = pNext->Pos - pCur->Pos;
			const float l0 = cMath::Lerp(0.0f, CurPrev.Length(), 0.33f);
			const float l1 = cMath::Lerp(0.0f, CurNext.Length(), 0.33f);
			cVec3 PrevNext = cVec3::Normalize(pNext->Pos - pPrev->Pos);
			if(!PrevNext.IsNormalized()) {
				PrevNext = cVec3::zAxis;
			}
			pCur->CV1 = pCur->Pos + l1 * PrevNext;
			pCur->CV0 = pCur->Pos - l0 * PrevNext;
			pCur->CV0IsValid = pCur->CV1IsValid = true;
		}
	}
	Update();
}
*/
/*
//-----------------------------------------------------------------------------
// cTrack::Update
//-----------------------------------------------------------------------------
void cTrack::Update() {
	m_Segments.Free();
	lFunc.Free();
/*
	if(m_Keys.Count() <= 1) {
		return;
	}

	// Create / adjust CVs:
	for(int iCur = 0; iCur < m_Keys.Count(); iCur++) {
		Key &Cur = m_Keys[iCur];
		int iPrev = AdjustIndex(iCur - 1);
		int iNext = AdjustIndex(iCur + 1);
		Key *pPrev = iPrev != iCur ? &m_Keys[iPrev] : NULL;
		Key *pNext = iNext != iCur ? &m_Keys[iNext] : NULL;
		if(!Cur.CV0IsValid && !Cur.CV1IsValid && pPrev) {
			Cur.CV0 = cVec3::Lerp(Cur.Pos, pPrev->Pos, 0.33f);
			Cur.CV0IsValid = true;
		} else if(!Cur.CV0IsValid && !Cur.CV1IsValid && pNext) {
			Cur.CV1 = cVec3::Lerp(Cur.Pos, pNext->Pos, 0.33f);
			Cur.CV1IsValid = true;
		} else if(Cur.CV0IsValid && !Cur.CV1IsValid && pNext) {
			Cur.CV1 = cVec3::Lerp(Cur.Pos, pNext->Pos, 0.33f);
			Cur.CV1IsValid = true;
			Cur.AdjustCV1();
		} else if(!Cur.CV0IsValid && Cur.CV1IsValid && pPrev) {
			Cur.CV0 = cVec3::Lerp(Cur.Pos, pPrev->Pos, 0.33f);
			Cur.CV0IsValid = true;
			Cur.AdjustCV0();
		}
	}

	// Gen curves:
	Segment S;
	for(int iCur = 0; iCur < m_Keys.Count(); iCur++) {
		Key &Cur = m_Keys[iCur];
		const int iPrev = AdjustIndex(iCur - 1);
		const int iNext = AdjustIndex(iCur + 1);
		const int iNextNext = AdjustIndex(iCur + 2);
		const Key &Prev = m_Keys[iPrev];
		const Key &Next = m_Keys[iNext];
		const Key &NextNext = m_Keys[iNextNext];
		if(iNext != iCur) {
			assert(Cur.CV1IsValid);
			assert(Next.CV0IsValid);
			S.Curve3.SetFromPoints(Cur.Pos, Cur.CV1, Next.CV0, Next.Pos);
			const float l = S.Curve3.CalcLength();
			if(m_Segments.Count()) {
				S.lStart = m_Segments.GetBack().lEnd;
			} else {
				S.lStart = 0.0f;
			}
			S.lEnd = S.lStart + l;

			S.tStart = Cur.Time;
			S.tEnd = Next.Time;
			
			const cQuat Q0 = Prev.Orient.ToQuat();
			S.Q0 = Cur.Orient.ToQuat();
			S.Q1 = Next.Orient.ToQuat();
			const cQuat Q3 = NextNext.Orient.ToQuat();
//			cQuat::BezierSetup(Q0, S.Q0, S.Q1, Q3, S.A, S.B);

			const float d0 = (Next.Offset - Prev.Offset) * 0.5f;
			const float d1 = (NextNext.Offset - Cur.Offset) * 0.5f;
			S.Curve1.SetFromEnds(Cur.Offset, d0, Next.Offset, d1);
			
			m_Segments.Add(S);
		}
	}

	lFunc.Free();
	
//	float tBefore = m_Keys[0].Time - (m_Keys[1].Time - m_Keys[0].Time);
//	float lBefore = -m_Segments[0].lEnd;
//	lFunc.AddValue(tBefore, lBefore);
//	lFunc.AddValue(m_Keys[0].Time, 0.0f, m_Keys[0].Weight);
//	lFunc.AddValue(m_Keys[0].Time, 0.0f, m_Keys[0].Weight);
	for(int n = 0; n < m_Keys[0].Count; n++) {
		lFunc.AddValue(m_Keys[0].Time, 0.0f);
	}
	for(int nS = 0; nS < m_Segments.Count(); nS++) {
		const Key &k = m_Keys[nS + 1];
		const Segment &S = m_Segments[nS];
		for(n = 0; n < k.Count; n++) {
			lFunc.AddValue(k.Time, S.lEnd);
		}
 	}

//	const int nSBack = m_Segments.Count() - 1;
//	for(int nS = 0; nS <= nSBack; nS++) {
//		int nSPrev = cMath::Clamp(nS - 1, 0, nSBack);
//		int nSNext = cMath::Clamp(nS + 1, 0, nSBack);
//		Segment &SCur = m_Segments[nS];
//		const Segment &SPrev = m_Segments[nSPrev];
//		const Segment &SNext = m_Segments[nSNext];

//		const float l0 = (SCur.lEnd - SPrev.lStart) * 0.5f;
//		const float l1 = (SNext.lEnd - SCur.lEnd) * 0.5f;
		
//		SCur.lFunc.SetFromEnds(SCur.lStart, l0, SCur.lEnd, l1);
//	}*/
//}

//-----------------------------------------------------------------------------
// cTrack::ReCreateSpeed
//-----------------------------------------------------------------------------
/*
void cTrack::ReCreateSpeed() {
/*	for(int iCur = 0; iCur < m_Keys.Count(); iCur++) {
		m_Keys[iCur].Speed = 1.0f;
	}
	Update();*/
//}