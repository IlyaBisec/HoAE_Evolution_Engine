#pragma once

class cStr {
private:
	char *m_pStr;
	int m_nLen, m_nAllocated;

	static const int BASE_BUFFER_LEN = 20;
	static const int GROW_BY = 32;
	static const int COLOR_ESCAPE = '^';
	static const int COLOR_DEFAULT = 'A';

	char m_pBaseBuffer[BASE_BUFFER_LEN];
	void Init();
	void EnsureAllocated(int Len, bool KeepOld = true);
	void Resize(int nLen, bool bKeepOld);
public:
	// Default constructor:
	cStr();
	// Copy - constructors:
	cStr(const cStr &Str);
	cStr(const char *pStr);

	enum eSetLengthCtor { SetLengthCtor };
	cStr(const eSetLengthCtor, const int Length);
	cStr(const eSetLengthCtor, const int Length, const char Fill);

	// Default destructor:
	~cStr();

	const char * ToCharPtr() const;
	char * ToCharPtr();
	operator const char * () const;
	operator char * ();

	char operator [] (int index) const;
	char & operator [] (int index);

	cStr & Copy(const cStr &Str);
	cStr & Copy(const char *pStr);
	cStr & operator = (const cStr &Str);
	cStr & operator = (const char *pStr);

	static bool Equals(const char *, const char *);
	static bool Equals(const char *, const char *, const int MaxLength);
	static bool EqualsNoCase(const char *, const char *);
	static int Cmp(const char *, const char *);
	int Cmp(const char *) const;
	friend bool operator == (const cStr &, const cStr &);
	friend bool operator == (const cStr &, const char *);
	friend bool operator == (const char *, const cStr &);
	friend bool operator != (const cStr &, const cStr &);
	friend bool operator != (const cStr &, const char *);
	friend bool operator != (const char *, const cStr &);

	int Length() const;
	void Fill(const char c);
	void SetLength(const int Length);
	int CalcLength();
	int LengthWithoutColors() const;
	static int LengthWithoutColors(const char *pStr);
	static int LinesCount(const char *pStr);
	static int LinesMaxLengthWithoutColors(const char *pStr);
	int LinesCount() const;
	int LinesMaxLengthWithoutColors() const;

	void Empty();
	bool IsEmpty() const;

	void Free();

	static bool IsColor(const char *pStr);
	static bool IsColor(const char *pStr, cColor &Color);
	bool IsColor() const;
	static cColor & ColorForIndex(int index);

	void Append(const cStr &Str);
	void AppendBkSp();
	void Append(const char *pStr);
	void Append(const char *pStr, const int Len);
	void Append(const float f, const int nPrec = 2);

	cStr & Append(const char c);
	cStr & operator += (const char);
	cStr & operator << (const char);


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

	const cStr GetMid(const int Start, const int Len) const;
	const cStr GetMid(const int Start) const;
	const cStr GetLeft(const int Len) const;
	const cStr GetRight(const int Len) const;
	
	void CapLength(const int MaxLen);

	void StripFileExtension();
	void StripFileName();
	void StripFilePath();

	void SetFileExtension(const char *Ext);
	void SetFileDefaultExtension(const char *DefExt); // If there's no extension set the default.
	void SetFilePath(const char *Path);
	void SetFileDefaultPath(const char *DefPath); // If there's no path set the default.

	const cStr GetFileExtension() const;
	const cStr GetFileName() const;
	const cStr GetFileBase() const;
	const cStr GetFilePath() const;

	void AppendPath(const char *Path);

	void SlashesToBackSlashes();
	void BackSlashesToSlashes();
	
	// Hash keys generators:
	static int Hash(const char *Str);
	static int HashNoCase(const char *Str);
	// chars:
	static bool CharIsLower(int c);
	static bool CharIsUpper(int c);
	static bool CharIsAlpha(int c);
	static bool CharIsNumeric(int c);
	static bool CharIsNewLine(const int c);
	static bool CharIsTab(const int c);
	static bool CharIsWhitespace(const int c);
	static bool CharIsDecimalPoint(const int c);
	static bool CharIsSign(const int c);
	static bool CharIsExponent(const int c);
	static char ToLower(char c);
	static char ToUpper(char c);

	static int ToInt(const char *pStr);
	static float ToFloat(const char *pStr);

	static int Length(const char *Str);
	static char * ToLower(char *Str);
	static char * ToUpper(char *Str);
	cStr & ToLower();
	cStr & ToUpper();

	static int FindChar(const char *Str, const char c, int Start = 0, int End = -1);
	static int FindText(const char *Str, const char *Text, bool NoCase = false, int Start = 0, int End = -1);
	int Find(const char c, int Start = 0, int End = -1) const;
	int Find(const char *Text, bool NoCase = false, int Start = 0, int End = -1) const;

	void StripLeading(const char c);
	void StripLeading(const char *Str);
	void StripTrailing(const char c);
	void StripTrailing(const char *Str);

	void ReplaceChar(const char Fm, const char To);
	void ReplaceChars(const char *Fm, const char To);
	int Replace(const char *Fm, const char *To);

	static void Split(const char *Src, cList<cStr> &List, const char *Delimiters = " ,;\t");
	void Split(cList<cStr> &List, const char *Delimiters = " ,;\t") const;
	
	static const cStr FromFloat(const float f, const int Prec = 2);
	static const cStr FromFloatArray(const float *Floats, const int Count, const int Prec = 2, const char *Delimiter = " ");
	static const cStr FromDouble(const double d, const int Prec = 6);
	static const cStr FromDoubleArray(const double *Doubles, const int Count, const int Prec = 6, const char *Delimiter = " ");
	static const cStr FromIntArray(const int *Ints, const int Count, const char *Delimiter = " ");

	static void Copy(char *To, const char *Fm, int Size);
	static int Printf(char *Str, int Size, const char *Format, ...);
	static int Printfv(char *Str, int Size, const char *Format, va_list Args);
	static int Printf(cStr &Str, const char *Format, ...);
	static int Printfv(cStr &Str, const char *Format, va_list Args);
	static const cStr Format(const char *Format, ...);
	
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
	static const cStr csOrange;
	static const cStr csPink;
	static const cStr csBrown;
	static const cStr csPurple;
};

// cStr::Init
inline void cStr::Init() {
	m_nLen = 0;
	m_nAllocated = BASE_BUFFER_LEN;
	m_pStr = m_pBaseBuffer;
	m_pStr[0] = '\0';
}

// cStr::EnsureAllocated
inline void cStr::EnsureAllocated(int Len, bool KeepOld) {
	if(Len > m_nAllocated) {
		Resize(Len, KeepOld);
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
	EnsureAllocated(l + 1);
	CopyMemory(m_pStr, Str.m_pStr, l);
	m_pStr[l] = '\0';
	m_nLen = l;
}

// cStr(const char *)
inline cStr::cStr(const char *pStr) {
	Init();
	const int l = Length(pStr);
	if(l > 0) {
		EnsureAllocated(l + 1);
		CopyMemory(m_pStr, pStr, l);
		m_pStr[l] = '\0';
		m_nLen = l;
	}
}

//-----------------------------------------------------------------------------
// cStr.ctor : (const eSetLengthCtor, const int)
//-----------------------------------------------------------------------------
inline cStr::cStr(const eSetLengthCtor, const int Length) {
	Init();
	cAssert(Length >= 0);
	if(Length <= 0) {
		Free();
	} else {
		EnsureAllocated(Length + 1, false);
		m_pStr[Length] = '\0';
		m_nLen = Length;
	}
} // cStr.ctor

//-----------------------------------------------------------------------------
// cStr.ctor : (const eSetLengthCtor, const int, const char)
//-----------------------------------------------------------------------------
inline cStr::cStr(const eSetLengthCtor, const int Length, const char Fill) {
	Init();
	cAssert(Length >= 0);
	if(Length <= 0) {
		Free();
	} else {
		EnsureAllocated(Length + 1);
		m_nLen = Length;
		for(int i = 0; i < Length; i++) {
			m_pStr[i] = Fill;
		}
		m_pStr[Length] = '\0';
	}
} // cStr.ctor

inline void cStr::Fill(const char c) {
	for(int i = 0; i < m_nLen; i++) {
		m_pStr[i] = c;
	}
}

// cStr::CalcLength
inline int cStr::CalcLength() {
	int l = Length(m_pStr);
	SetLength(l);
	return l;
}

inline void cStr::SetLength(const int Length) {
	cAssert(Length >= 0);
	if(Length <= 0) {
		Free();
	} else {
		EnsureAllocated(Length + 1);
		m_nLen = Length;
		m_pStr[Length] = '\0';
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

// cStr::LinesCount
inline int cStr::LinesCount() const {
	return cStr::LinesCount(m_pStr);
}

// cStr::LinesMaxLengthWithoutColors
inline int cStr::LinesMaxLengthWithoutColors() const {
	return cStr::LinesMaxLengthWithoutColors(m_pStr);
}

//-----------------------------------------------------------------------------
// cStr::ToCharPtr : const char * () const
//-----------------------------------------------------------------------------
inline const char * cStr::ToCharPtr() const {
	return m_pStr;
}

//-----------------------------------------------------------------------------
// cStr::ToCharPtr : char * ()
//-----------------------------------------------------------------------------
inline char * cStr::ToCharPtr() {
	return m_pStr;
}

// cStr::operator [] const
inline char cStr::operator [] (int index) const {
	cAssert(index >= 0 && index <= m_nLen);
	return m_pStr[index];
} // cStr::operator [] const

// cStr::operator []
inline char & cStr::operator [] (int index) {
	cAssert(index >= 0 && index <= m_nLen);
	return m_pStr[index];
} // cStr::operator []

//-----------------------------------------------------------------------------
// cStr::Copy : cStr & (const cStr &)
//-----------------------------------------------------------------------------
inline cStr & cStr::Copy(const cStr &Str) {
	int l = Str.Length();
	EnsureAllocated(l + 1, false);
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
	return ToCharPtr();
}

// cStr::operator const char *()
inline cStr::operator char *() {
	return ToCharPtr();
}

// cStr::Hash
inline int cStr::Hash(const char *Str) {
	int hash = 0;
	for(int i = 0; *Str != '\0'; i++) {
		hash += (*Str++) * (i + 119);
	}
	return hash;
}

// cStr::CaseInsHash
inline int cStr::HashNoCase(const char *Str) {
	int hash = 0;
	for(int i = 0; *Str != '\0'; i++) {
		hash += ToLower(*Str++) * (i + 119);
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

// cStr::CharIsNewLine
inline bool cStr::CharIsNewLine(const int c) {
	return c == '\n' || c == '\r' || c == '\v';
}

// cStr::CharIsTab
inline bool cStr::CharIsTab(const int c) {
	return c == '\t';
}

// cStr::CharIsWhitespace
inline bool cStr::CharIsWhitespace(const int c) {
	return ' ' == c || '\t' == c;
}

// cStr::CharIsDecimalPoint
inline bool cStr::CharIsDecimalPoint(const int c) {
	return '.' == c || ',' == c;
}

// cStr::CharIsSign
inline bool cStr::CharIsSign(const int c) {
	return '+' == c || '-' == c;
}

// cStr::CharIsExponent
inline bool cStr::CharIsExponent(const int c) {
	return 'd' == c || 'D' == c || 'e' == c || 'E' == c;
}


// cStr::GetLeft
inline const cStr cStr::GetLeft(const int Len) const {
	return GetMid(0, Len);
}

// cStr::GetRight
inline const cStr cStr::GetRight(const int Len) const {
	if(Len >= Length()) {
		return *this;
	}
	return GetMid(Length() - Len, Len);
}

// cStr::GetMid
inline const cStr cStr::GetMid(const int Start) const {
	return GetMid(Start, Length() - Start);
}

// cStr::CapLength
inline void cStr::CapLength(const int MaxLen) {
	if(m_nLen <= MaxLen) {
		return;
	}
	m_pStr[MaxLen] = 0;
	m_nLen = MaxLen;
}

// cStr::Length : int (const char *)
inline int cStr::Length(const char *Str) {
	if(Str == NULL) {
		return 0;
	}
	int l;
	for(l = 0; Str[l]; l++) {}
	return l;
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

// cStr::ToLower : char * (char *)
inline char * cStr::ToLower(char *Str) {
	for(int i = 0; Str[i]; i++) {
		Str[i] = ToLower(Str[i]);
	}
	return Str;
}

// cStr::ToUpper : char * (char *)
inline char * cStr::ToUpper(char *Str) {
	for(int i = 0; Str[i]; i++) {
		Str[i] = ToUpper(Str[i]);
	}
	return Str;
}

// cStr::ToLower : cStr & ()
inline cStr & cStr::ToLower() {
	for(int i = 0; m_pStr[i]; i++) {
		m_pStr[i] = ToLower(m_pStr[i]);
	}
	return *this;
}

// cStr::ToUpper : cStr & ()
inline cStr & cStr::ToUpper() {
	for(int i = 0; m_pStr[i]; i++) {
		m_pStr[i] = ToUpper(m_pStr[i]);
	}
	return *this;
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
	EnsureAllocated(L + 1);
	for(int i = 0; i < Str.m_nLen; i++) {
		m_pStr[m_nLen + i] = Str[i];
	}
	m_nLen = L;
	m_pStr[m_nLen] = '\0';
} // cStr::Append(const cStr &)

// cStr::AppendBkSp()
inline void cStr::AppendBkSp() {
	cAssert(Length());
	if(m_pStr[Length() - 1] != '\\') Append("\\");
}

// cStr::Append(const char *)
inline void cStr::Append(const char *pStr) {
	if(!pStr) return;
	int L = m_nLen + int(strlen(pStr));
	EnsureAllocated(L + 1);
	for(int i = 0; pStr[i]; i++) {
		m_pStr[m_nLen + i] = pStr[i];
	}
	m_nLen = L;
	m_pStr[m_nLen] = '\0';
} // cStr::Append(const char *)

//-----------------------------------------------------------------------------
// cStr::Append
//-----------------------------------------------------------------------------
inline void cStr::Append(const char *pStr, const int Len) {
	if(pStr && Len) {
		int L = m_nLen + Len;
		EnsureAllocated(L + 1);
		for(int i = 0; pStr[i] && i < Len; i++) {
			m_pStr[m_nLen + i] = pStr[i];
		}
		m_nLen = L;
		m_pStr[m_nLen] = '\0';
	}
} // cStr::Append

//-----------------------------------------------------------------------------
// cStr::Append : cStr & (const char)
//-----------------------------------------------------------------------------
inline cStr & cStr::Append(const char c) {
	int L = m_nLen + 1;
	EnsureAllocated(L + 1);
	m_pStr[m_nLen] = c;
	m_nLen = L;
	m_pStr[m_nLen] = '\0';
	return *this;
}

// cStr::operator += : cStr & (const char)
inline cStr & cStr::operator += (const char c) {
	return Append(c);
}

// cStr::operator << : cStr & (const char)
inline cStr & cStr::operator << (const char c) {
	return Append(c);
}

// cStr::FromFloat
inline const cStr cStr::FromFloat(const float f, const int Prec /*= 2*/) {
	return Format(Format("%%.%df", Prec), f);
}

// cStr::FromDouble
inline const cStr cStr::FromDouble(const double d, const int Prec /*= 6*/) {
	return Format(Format("%%.%df", Prec), d);
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
	Append(Format("%d", c));
	return *this;
} // cStr::operator += (int)



// cStr::operator += (double)
inline cStr & cStr::operator += (double c) {
	Append(Format("%f", c));
	return *this;
} // cStr::operator += (double)

// cStr::operator += : cStr & (const float)
inline cStr & cStr::operator += (const float f) {
	Append(Format("%f", f));
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
	r += c;
	return r;
} // operator + (const cStr &, int)

// operator + (const cStr &, double)
inline cStr operator + (const cStr &Str, double c) {
	cStr r(Str);
	r += c;
	return r;
} // operator + (const cStr &, double)

// operator + : cStr(const cStr &, const float)
inline cStr operator + (const cStr &Str, const float f) {
	cStr r(Str);
	r += f;
	return r;
}

inline int cStr::Cmp(const char *pStr) const {
	cAssert(pStr);
	return cStr::Cmp(m_pStr, pStr);
}

inline bool operator == (const cStr &Str1, const cStr &Str2) {
	return !cStr::Cmp(Str1.m_pStr, Str2.m_pStr);
}

inline bool operator == (const cStr &Str, const char *pText) {
	cAssert(pText);
	return !cStr::Cmp(Str.m_pStr, pText);
}

inline bool operator == (const char *pText, const cStr &Str) {
	cAssert(pText);
	return !cStr::Cmp(pText, Str.m_pStr);
}

inline bool operator != (const cStr &lStr, const cStr &rStr) {
	return !cStr::Equals(lStr, rStr);
}

inline bool operator != (const cStr &Str, const char *pChar) {
	return !cStr::Equals(Str, pChar);
}

inline bool operator != (const char *pChar, const cStr &Str) {
	return !cStr::Equals(pChar, Str);
}

// cStr::Empty
inline void cStr::Empty() {
	EnsureAllocated(1);
	m_pStr[0] = '\0';
	m_nLen = 1;
}

// cStr::IsEmpty
inline bool cStr::IsEmpty() const {
	return Equals(m_pStr, "");
}

// cStr::Find
inline int cStr::Find(const char c, int Start /*= 0*/, int End /*= -1*/) const {
	return FindChar(m_pStr, c, Start, End);
}

// cStr::Find
inline int cStr::Find(const char *Text, bool NoCase /*= false*/, int Start /*= 0*/, int End /*= -1*/) const {
	return FindText(m_pStr, Text, NoCase, Start, End);
}

// cStr::Split
inline void cStr::Split(cList<cStr> &List, const char *Delimiters /*= " ,;\t"*/) const {
	cStr::Split(m_pStr, List, Delimiters);
}