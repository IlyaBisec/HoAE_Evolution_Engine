#ifndef __CSTR__
#define __CSTR__

#include "cUtils.h"

namespace comms {

#define cStr_BaseBufferLen	20
#define cStr_nGrowBy		32

const int cStr_ccEscape		= '^';
const int cStr_ccDefault	= '0';

class cStr {
public:
	// Default constructor:
	cStr();
	// Copy - constructors:
	cStr(const cStr &Str);
	cStr(const char *pStr);
	// Default destructor:
	~cStr();

	const char *c_str() const;
	operator const char * () const;
	operator const char * ();

	char operator [] (int index) const;
	char & operator [] (int index);

	cStr & operator = (const cStr &Str);
	cStr & operator = (const char *pStr);

	int Length() const;
	int LengthWithoutColors() const;
	static int LengthWithoutColors(const char *pStr);

	void Free();

	static bool IsColor(const char *pStr);
	static bool IsColor(const char *pStr, COLORREF &Color);
	bool IsColor() const;
	static COLORREF & ColorForIndex(int index);

	void Append(const cStr &Str);
	void Append(const char *pStr);

	friend cStr operator + (const cStr &, const cStr &);
	friend cStr operator + (const cStr &, const char *);
	friend cStr operator + (const char *, const cStr &);

	friend cStr operator + (const cStr &, int);
	friend cStr operator + (const cStr &, double);

	cStr & operator += (const cStr &);
	cStr & operator += (const char *);
	cStr & operator += (int);
	cStr & operator += (double);

	// Hash keys generators:
	static int Hash(const char *string);
	static int CaseInsHash(const char *string);
	// chars:
	static bool CharIsLower(int c);
	static bool CharIsUpper(int c);
	static bool CharIsAlpha(int c);
	static bool CharIsNumeric(int c);
	static char ToLower(char c);
	static char ToUpper(char c);

	static cStr FromArray(const double *pDoubles, int nLen, int nPrec = 2, const char *pSep = " ");

	// Color escape strings:
	static const cStr csDefault;
	static const cStr csBlack;
	static const cStr csGray75;
	static const cStr csGray50;
	static const cStr csGray25;
	static const cStr csWhite;
	static const cStr csRed;
	static const cStr csGreen;
	static const cStr csBlue;
	static const cStr csCyan;
	static const cStr csMagenta;
	static const cStr csYellow;
private:
	int m_nLen, m_nAllocated;
	char *m_pStr, m_pBaseBuffer[cStr_BaseBufferLen];
	void Init();
	void Check(int nLen, bool bKeepOld = true);
	void Resize(int nLen, bool bKeepOld);
};

// cStr::Init
inline void cStr::Init() {
	m_nLen = 0;
	m_nAllocated = cStr_BaseBufferLen;
	m_pStr = m_pBaseBuffer;
	m_pStr[0] = '\0';
}

// cStr::Check
inline void cStr::Check(int nLen, bool bKeepOld) {
	if(nLen > m_nAllocated) {
		Resize(nLen, bKeepOld);
	}
}

// cStr
inline cStr::cStr() {
	Init();
}

// ~cStr
inline cStr::~cStr() {
	Free();
}

// cStr(const cStr &)
inline cStr::cStr(const cStr &Str) {
	Init();
	int l = Str.Length();
	Check(l + 1);
	strcpy(m_pStr, Str.m_pStr);
	m_nLen = l;
}

// cStr(const char *)
inline cStr::cStr(const char *pStr) {
	Init();
	if(pStr) {
		int l = (int)strlen(pStr);
		Check(l + 1);
		strcpy(m_pStr, pStr);
		m_nLen = l;
	}
}

// cStr::Length
inline int cStr::Length() const {
	return m_nLen;
} // cStr::Length

// cStr::LengthWithoutColors
inline int cStr::LengthWithoutColors() const {
	return cStr::LengthWithoutColors(m_pStr);
} // cStr::LengthWithoutColors

// cStr::c_str
inline const char *cStr::c_str() const {
	return m_pStr;
}

// cStr::operator [] const
inline char cStr::operator [] (int index) const {
	assert(index >= 0 && index <= m_nLen);
	return m_pStr[index];
} // cStr::operator [] const

// cStr::operator []
inline char & cStr::operator [] (int index) {
	assert(index >= 0 && index <= m_nLen);
	return m_pStr[index];
} // cStr::operator []

// cStr::operator = (const cStr &)
inline cStr & cStr::operator = (const cStr &Str) {
	int l = Str.Length();
	Check(l + 1, false);
	memcpy(m_pStr, Str.m_pStr, l);
	m_pStr[l] = '\0';
	m_nLen = l;
	return *this;
}

// cStr::operator const char *() const
inline cStr::operator const char *() const {
	return c_str();
}

// cStr::operator const char *()
inline cStr::operator const char *() {
	return c_str();
}

// cStr::Hash
inline int cStr::Hash(const char *string) {
	int hash = 0;
	for(int i = 0; *string != '\0'; i++) {
		hash += (*string++) * (i + 119);
	}
	return hash;
}

// cStr::CaseInsHash
inline int cStr::CaseInsHash(const char *string) {
	int hash = 0;
	for(int i = 0; *string != '\0'; i++) {
		hash += ToLower(*string++) * (i + 119);
	}
	return hash;
}

// cStr::CharIsLower
inline bool cStr::CharIsLower(int c) {
	return c >= 'a' && c <= 'z';
}

// cStr::CharIsUpper
inline bool cStr::CharIsUpper(int c) {
	return c >= 'A' && c <= 'Z';
}

// cStr::CharIsAlpha
inline bool cStr::CharIsAlpha(int c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// cStr::CharIsNumeric
inline bool cStr::CharIsNumeric(int c) {
	return c >= '0' && c <= '9';
}

// cStr::ToLower
inline char cStr::ToLower(char c) {
	if(c >= 'A' && c <= 'Z') return c + ('a' - 'A');
	return c;
}

// cStr::ToUpper
inline char cStr::ToUpper(char c) {
	if(c >= 'a' && c <= 'z') return c - ('a' - 'A');
	return c;
}

// cStr::IsColor(const char *)
inline bool cStr::IsColor(const char *pStr) {
	return pStr[0] == cStr_ccEscape && pStr[1] != '\0' && pStr[1] != ' ';
} // cStr::IsColor(const char *)

// cStr::IsColor(const char *, COLORREF &)
inline bool cStr::IsColor(const char *pStr, COLORREF &Color) {
	if(IsColor(pStr)) {
		Color = ColorForIndex(pStr[1] - cStr_ccDefault);
		return true;
	}
	return false;
} // cStr::IsColor(const char *, COLORREF &)

// cStr::IsColor() const
inline bool cStr::IsColor() const {
	return cStr::IsColor(m_pStr);
} // cStr::IsColor() const

// cStr::Append(const cStr &)
inline void cStr::Append(const cStr &Str) {
	int L = m_nLen + Str.m_nLen;
	Check(L + 1);
	for(int i = 0; i < Str.m_nLen; i++) {
		m_pStr[m_nLen + i] = Str[i];
	}
	m_nLen = L;
	m_pStr[m_nLen] = '\0';
} // cStr::Append(const cStr &)

// cStr::Append(const char *)
inline void cStr::Append(const char *pStr) {
	if(!pStr) return;
	int L = m_nLen + int(strlen(pStr));
	Check(L + 1);
	for(int i = 0; pStr[i]; i++) {
		m_pStr[m_nLen + i] = pStr[i];
	}
	m_nLen = L;
	m_pStr[m_nLen] = '\0';
} // cStr::Append(const char *)

// cStr::operator += (const cStr &)
inline cStr & cStr::operator += (const cStr &Str) {
	Append(Str);
	return *this;
} // cStr::operator += (const cStr &)

// cStr::operator += (const char *)
inline cStr & cStr::operator += (const char *pStr) {
	Append(pStr);
	return *this;
} // cStr::operator += (const char *)

// cStr::operator += (int)
inline cStr & cStr::operator += (int c) {
	char Buffer[64];
	sprintf(Buffer, "%d", c);
	Append(Buffer);
	return *this;
} // cStr::operator += (int)

// cStr::operator += (double)
inline cStr & cStr::operator += (double c) {
	char Buffer[64];
	sprintf(Buffer, "%f", c);
	Append(Buffer);
	return *this;
} // cStr::operator += (double)

// operator + (const cStr &, const cStr&)
inline cStr operator + (const cStr &StrA, const cStr &StrB) {
	cStr r(StrA);
	r.Append(StrB);
	return r;
} // operator + (const cStr &, const cStr&)

// operator + (const cStr &, const char *)
inline cStr operator + (const cStr &Str, const char *pStr) {
	cStr r(Str);
	r.Append(pStr);
	return r;
} // operator + (const cStr &, const char *)

// operator + (const char *, const cStr &)
inline cStr operator + (const char *pStr, const cStr &Str) {
	cStr r(pStr);
	r.Append(Str);
	return r;
} // operator + (const char *, const cStr &)

// operator + (const cStr &, int)
inline cStr operator + (const cStr &Str, int c) {
	cStr r(Str);
	char Buffer[64];
	sprintf(Buffer, "%d", c);
	r.Append(Buffer);
	return r;
} // operator + (const cStr &, int)

// operator + (const cStr &, double)
inline cStr operator + (const cStr &Str, double c) {
	cStr r(Str);
	char Buffer[64];
	sprintf(Buffer, "%f", c);
	r.Append(Buffer);
	return r;
} // operator + (const cStr &, double)

} // comms

#endif // __CSTR__