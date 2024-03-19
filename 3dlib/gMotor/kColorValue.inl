/*****************************************************************
/*  File:   ColorValue.inl                                           
/*  Desc:   Color operations inlines
/*  Author: Silver, Copyright (C) GSC Game World                 
/*  Date:   January 2002                                         
/*****************************************************************/
#include "mConstants.h"
#include "mUtil.h"
/*****************************************************************
/*	ColorValue implementation
/*****************************************************************/
_inl DWORD ColorValue::ToARGB( BYTE a, BYTE r, BYTE g, BYTE b )
{
		return  (
				((a & 0xFF) << 24) | 
				((r & 0xFF) << 16) |
				((g & 0xFF) << 8)  |
				(b & 0xFF)
				);
}

_inl DWORD ColorValue::ToARGB( float a, float r, float g, float b )
{
	clamp( a, 0.0f, 1.0f );
	clamp( r, 0.0f, 1.0f );
	clamp( g, 0.0f, 1.0f );
	clamp( b, 0.0f, 1.0f );

	BYTE ba, br, bg, bb;
	ba = (BYTE)(a * 255.0f);
	br = (BYTE)(r * 255.0f);
	bg = (BYTE)(g * 255.0f);
	bb = (BYTE)(b * 255.0f);
	return ToARGB( ba, br, bg, bb );
} // ColorValue::ToARGB

_inl void ColorValue::FromARGB( DWORD col, BYTE& a, BYTE& r, BYTE& g, BYTE& b )
{
	a = (col & 0xFF000000) >> 24;
	r = (col & 0x00FF0000) >> 16;
	g = (col & 0x0000FF00) >> 8;
	b = (col & 0x000000FF);
}

_inl void ColorValue::FromARGB( DWORD col, float& a, float& r, float& g, float& b )
{
	BYTE ba, br, bg, bb;
	FromARGB( col, ba, br, bg, bb );
	a = float( ba ) / 255.0f;
	r = float( br ) / 255.0f;
	g = float( bg ) / 255.0f;
	b = float( bb ) / 255.0f;
} // ColorValue::FromARGB

_inl DWORD ColorValue::Gradient( DWORD a, DWORD b, float grad )
{
	float aa, ar, ag, ab;
	float ba, br, bg, bb;

	FromARGB( a, aa, ar, ag, ab );
	FromARGB( b, ba, br, bg, bb );
	
	float ra = (ba - aa)* grad + aa;
	float rb = (bb - ab)* grad + ab;
	float rg = (bg - ag)* grad + ag;
	float rr = (br - ar)* grad + ar;
	
	return ToARGB( ra, rr, rg, rb );
} // ColorValue::Gradient

_inl DWORD ColorValue::ARGB4To8( DWORD color )
{
	return  ((color & 0xF000) << 16) | ((color & 0x0F00) << 12) |
			((color & 0x00F0) << 8)  | ((color & 0x000F) << 4);
}

/*****************************************************************
/*	ColorRamp implementation
/*****************************************************************/
_inl float LinInterp( float t, float ta, float a, float tb, float b )
{
	return a + (t - ta) * (b - a) / (tb - ta);
} 

_inl DWORD ColorRamp::GetColor( float t ) const
{
	if (t <= 0.0f) return m_Key[0];
	int lastKey = m_NKeys - 1;
	if (t >= 1.0f) return m_Key[m_NKeys - 1];

	for (int i = 0; i < lastKey; i++)
	{
		if (m_Time[i] == t) return m_Key[i];
		if (m_Time[i] <= t && m_Time[i + 1] > t) 
		{
			ColorValue val;
			float ta = m_Time[i];
			float tb = m_Time[i + 1];
			val.a = LinInterp( t, ta, m_Key[i].a, tb, m_Key[i + 1].a );
			val.r = LinInterp( t, ta, m_Key[i].r, tb, m_Key[i + 1].r );
			val.g = LinInterp( t, ta, m_Key[i].g, tb, m_Key[i + 1].g );
			val.b = LinInterp( t, ta, m_Key[i].b, tb, m_Key[i + 1].b );
			return val;
		}
	}
	return 0xFFFFFFFF;
} // ColorRamp::GetColor

_inl DWORD ColorRamp::GetKey( int idx ) const
{
	if (idx <= 0) return m_Key[0];
	if (idx >= m_NKeys) return m_Key[m_NKeys - 1];
	return m_Key[idx];
}

_inl float ColorRamp::GetKeyTime( int idx ) const
{
	if (idx <= 0) return 0.0f;
	if (idx >= m_NKeys) return 1.0f;
	return m_Time[idx];
}

_inl void ColorRamp::SetKey( int idx, DWORD color )
{
	if (idx < 0 || idx >= m_NKeys) return;
	m_Key[idx] = color;
}

_inl void ColorRamp::SetKeyTime( int idx, float t )
{
	if (idx <= 0 || idx >= m_NKeys - 1) return;
	m_Time[idx] = t;
}

_inl void ColorRamp::DeleteKey( int idx )
{
    if (idx < 0) return;
	int lastKey = m_NKeys - 1;
	if (idx <= 0 || idx >= lastKey) return;
	for (int i = idx; i < lastKey; i++)
	{
		m_Key [i] = m_Key [i + 1];
		m_Time[i] = m_Time[i + 1];
	}
	m_NKeys--;
} // ColorRamp::DeleteKey


/*****************************************************************
/*	AlphaRamp implementation
/*****************************************************************/
_inl float AlphaRamp::GetAlpha( float t ) const
{
	if (t <= 0.0f) return m_Key[0];
	int lastKey = m_NKeys - 1;
	if (t >= 1.0f) return m_Key[m_NKeys - 1];

	for (int i = 0; i < lastKey; i++)
	{
		if (m_Time[i] == t) return m_Key[i];
		if (m_Time[i] <= t && m_Time[i + 1] > t) 
		{
			float val;
			float ta = m_Time[i];
			float tb = m_Time[i + 1];
			val = LinInterp( t, ta, m_Key[i], tb, m_Key[i + 1] );
			return val;
		}
	}
	return 1.0f;
} // AlphaRamp::GetColor

_inl float AlphaRamp::GetKey( int idx ) const
{
	if (idx <= 0) return m_Key[0];
	if (idx >= m_NKeys) return m_Key[m_NKeys - 1];
	return m_Key[idx];
}

_inl float AlphaRamp::GetKeyTime( int idx ) const
{
	if (idx <= 0) return 0.0f;
	if (idx >= m_NKeys) return 1.0f;
	return m_Time[idx];
}

_inl void AlphaRamp::SetKey( int idx, float alpha )
{
	if (idx < 0 || idx >= m_NKeys) return;
	m_Key[idx] = alpha;
}

_inl void AlphaRamp::SetKeyTime( int idx, float t )
{
	if (idx <= 0 || idx >= m_NKeys - 1) return;
	m_Time[idx] = t;
}

_inl void AlphaRamp::DeleteKey( int idx )
{
    if (idx < 0) return;
	int lastKey = m_NKeys - 1;
	if (idx <= 0 || idx >= lastKey) return;
	for (int i = idx; i < lastKey; i++)
	{
		m_Key [i] = m_Key [i + 1];
		m_Time[i] = m_Time[i + 1];
	}
	m_NKeys--;
} // AlphaRamp::DeleteKey
