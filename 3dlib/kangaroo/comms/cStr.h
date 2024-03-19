#pragma once

class cStr {
public:
	// Default constructor:
	cStr();
	// Copy - constructors:
	cStr(const cStr &Str);
	cStr(const char *pStr);
	// Default destructor:
	~cStr();

	const char * ToChar() const;
	char * ToChar();
	operator const char * () const;
	operator char * ();

	char operator [] (int index) const;
	char & operator [] (int index);

	cStr & Copy(const cStr &Str);
	cStr & Copy(const char *pStr);
	cStr & operator = (const cStr &Str);
	cStr & operator = (const char *pStr);

	static bool Equals(const char *, const char *);
	static bool IEquals(const char *, const char *);
	static int Cmp(const char *, const char *);
	int Cmp(const char *) const;
	friend bool operator == (const cStr &, const cStr &);
	friend bool operator == (const cStr &, const char *);
	friend bool operator == (const char *, const cStr &);

	int Length() const;
	int LengthWithoutColors() const;
	static int LengthWithoutColors(const char *pStr);

	void Free();

	static bool IsColor(const char *pStr);
	static bool IsColor(const char *pStr, cColor &Color);
	bool IsColor() const;
	static cColor & ColorForIndex(int index);

	void Append(const cStr &Str);
	void AppendBkSp();
	void Append(const char *pStr);
	void Append(const float f, const int nPrec = 2);

	friend cStr operator + (const cStr &, const cStr &);
	friend cStr operator + (const cStr &, const char *);
	friend cStr operator + (const char *, const cStr &);
	friend cStr operator + (const cStr &, int);
	friend cStr operator + (const cStr &, double);

	friend cStr operator + (const cStr &, const float);

	cStr & operator += (const cStr &);
	cStr & operator += (const char *);
	cStr & operator << (const char *);
	cStr & operator << (int);
	cStr & operator << (const float);
	cStr & operator += (int);
	cStr & operator += (double);

	cStr & operator += (const float);

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

	static cStr FromFloat(const float f, const int nPrec = 2);
	static cStr FromArray(const double *pDoubles, int nLen, int nPrec = 2, const char *pSep = " ");
	static cStr FromArray(const float *pFloats, int nLen, int nPrec = 2, const char *pSep = " ");

	// Color escape strings:
	static const cStr csDefault;
	static const cStr csBlack;
	static const cStr csGrey75;
	static const cStr csGrey50;
	static const cStr csGrey25;
	static const cStr csWhite;
	static const cStr csRed;
	static const cStr csGreen;
	static const cStr csBlue;
	static const cStr csCyan;
	static const cStr csMagenta;
	static const cStr csYellow;
	static const cStr csOrange;
	static const cStr csPink;
	static const cStr csBrown;
	static const cStr csPurple;

	void toFile(const char *) const;
private:
	static const int BASE_BUFFER_LEN = 20;
	static const int GROW_BY = 32;
	static const int COLOR_ESCAPE = '^';
	static const int COLOR_DEFAULT = 'A';
	
	int m_nLen, m_nAllocated;
	char *m_pStr, m_pBaseBuffer[BASE_BUFFER_LEN];
	void Init();
	void Check(int nLen, bool bKeepOld = true);
	void Resize(int nLen, bool bKeepOld);
};

// cStr::Init
inline void cStr::Init() {
	m_nLen = 0;
	m_nAllocated = BASE_BUFFER_LEN;
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

//-----------------------------------------------------------------------------
// cStr::ToChar : const char * () const
//-----------------------------------------------------------------------------
inline const char * cStr::ToChar() const {
	return m_pStr;
}

//-----------------------------------------------------------------------------
// cStr::ToChar : char * ()
//-----------------------------------------------------------------------------
inline char * cStr::ToChar() {
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

//-----------------------------------------------------------------------------
// cStr::Copy : cStr & (const cStr &)
//-----------------------------------------------------------------------------
inline cStr & cStr::Copy(const cStr &Str) {
	int l = Str.Length();
	Check(l + 1, false);
	CopyMemory(m_pStr, Str.m_pStr, l);
	m_pStr[l] = '\0';
	m_nLen = l;
	return *this;
}

//-----------------------------------------------------------------------------
// cStr::operator = : cStr & (const cStr &)
//-----------------------------------------------------------------------------
inline cStr & cStr::operator = (const cStr &Str) {
	return Copy(Str);
}

// cStr::operator const char *() const
inline cStr::operator const char *() const {
	return ToChar();
}

// cStr::operator const char *()
inline cStr::operator char *() {
	return ToChar();
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
	return pStr[0] == COLOR_ESCAPE && pStr[1] != '\0' && pStr[1] != ' ';
} // cStr::IsColor(const char *)

// cStr::IsColor(const char *, cColor &)
inline bool cStr::IsColor(const char *pStr, cColor &Color) {
	if(IsColor(pStr)) {
		Color = ColorForIndex(pStr[1] - COLOR_DEFAULT);
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

// cStr::AppendBkSp()
inline void cStr::AppendBkSp() {
	assert(Length());
	if(m_pStr[Length() - 1] != '\\') Append("\\");
}

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

// cStr::FromFloat : cStr(const float, const int)
inline cStr cStr::FromFloat(const float f, const int nPrec) {
	char Format[16], Buffer[64];
	sprintf(Format, "%%.%df", nPrec);
	cStr Str;
	sprintf(Buffer, Format, f);
	return cStr(Buffer);
}

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

//-----------------------------------------------------------------------------
inline cStr & cStr::operator << (const char *pStr) {
	Append(pStr);
	return *this;
}

inline cStr & cStr::operator << (int c) {
	return (*this) += c;
}

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

// cStr::operator += : cStr & (const float)
inline cStr & cStr::operator += (const float f) {
	char Buffer[64];

	sprintf(Buffer, "%f", f);
	Append(Buffer);

	return *this;
}

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

// operator + : cStr(const cStr &, const float)
inline cStr operator + (const cStr &Str, const float f) {
	cStr r(Str);
	char Buffer[64];

	sprintf(Buffer, "%f", f);
	r.Append(Buffer);

	return r;
}

inline int cStr::Cmp(const char *pStr) const {
	assert(pStr);
	return cStr::Cmp(m_pStr, pStr);
}

inline bool operator == (const cStr &Str1, const cStr &Str2) {
	return !cStr::Cmp(Str1.m_pStr, Str2.m_pStr);
}

inline bool operator == (const cStr &Str, const char *pText) {
	assert(pText);
	return !cStr::Cmp(Str.m_pStr, pText);
}

inline bool operator == (const char *pText, const cStr &Str) {
	assert(pText);
	return !cStr::Cmp(pText, Str.m_pStr);
}