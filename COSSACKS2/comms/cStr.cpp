#include "comms.h"

// cStr::Resize
void cStr::Resize(int nLen, bool bKeepOld) {
	cAssert(nLen > 0);

	int nMod = nLen % GROW_BY;
	int nNewAllocated;
	if(!nMod) {
		nNewAllocated = nLen;
	} else {
		nNewAllocated = nLen + GROW_BY - nMod;
	}
	m_nAllocated = nNewAllocated;

	char *pNewStr = new char[m_nAllocated];

	if(bKeepOld && m_pStr) {
		m_pStr[m_nLen] = '\0';
		CopyMemory(pNewStr, m_pStr, m_nLen + 1);
	}

	if(m_pStr && m_pStr != m_pBaseBuffer) {
		delete[] m_pStr;
	}

	m_pStr = pNewStr;
}

// cStr::Free
void cStr::Free() {
	if(m_pStr && m_pStr != m_pBaseBuffer) {
		delete[] m_pStr;
	}
	Init();
}

//-----------------------------------------------------------------------------
// cStr::Copy : cStr & (const char *)
//-----------------------------------------------------------------------------
cStr & cStr::Copy(const char *pStr) {
	if(!pStr) {
		EnsureAllocated(1, false);
		m_pStr[0] = '\0';
		m_nLen = 0;
		return *this;
	}
	if(pStr == m_pStr) return *this;

	if(pStr >= m_pStr && pStr <= m_pStr + m_nLen) {
		int d = int(pStr - m_pStr);
		cAssert(strlen(pStr) < (unsigned)m_nLen);
		int i;
		for(i = 0; pStr[i]; i++) {
			m_pStr[i] = pStr[i];
		}
		m_pStr[i] = '\0';
		m_nLen -= d;
		return *this;
	}

	int l = Length(pStr);
	EnsureAllocated(l + 1, false);
	CopyMemory(m_pStr, pStr, l + 1);
	m_nLen = l;
	return *this;
}

//-----------------------------------------------------------------------------
// cStr::operator = : cStr & (const char *)
//-----------------------------------------------------------------------------
cStr & cStr::operator = (const char *pStr) {
	return Copy(pStr);
}

// cStr::FromFloatArray
const cStr cStr::FromFloatArray(const float *Floats, const int Count, const int Prec /*= 2*/, const char *Delimiter /*= " "*/) {
	cStr Str;
	for(int i = 0; i < Count; i++) {
		Str += FromFloat(Floats[i], Prec);
		if(i < Count - 1) {
			Str += Delimiter;
		}
	}
	return Str;
}

// cStr::FromDoubleArray
const cStr cStr::FromDoubleArray(const double *Doubles, const int Count, const int Prec /*= 6*/, const char *Delimiter /*= " "*/) {
	cStr Str;
	for(int i = 0; i < Count; i++) {
		Str += FromDouble(Doubles[i], Prec);
		if(i < Count - 1) {
			Str += Delimiter;
		}
	}
	return Str;
}

// cStr::FromIntArray
const cStr cStr::FromIntArray(const int *Ints, const int Count, const char *Delimiter /*= " "*/) {
	cStr Str;
	for(int i = 0; i < Count; i++) {
		Str += Format("%d", Ints[i]);
		if(i < Count - 1) {
			Str += Delimiter;
		}
	}
	return Str;
}

const cStr cStr::csDefault	= "^A";
const cStr cStr::csBlack	= "^B";
const cStr cStr::csGray75	= "^C";
const cStr cStr::csGray50	= "^D";
const cStr cStr::csGray25	= "^E";
const cStr cStr::csWhite	= "^F";
const cStr cStr::csRed		= "^G";
const cStr cStr::csGreen	= "^H";
const cStr cStr::csBlue		= "^I";
const cStr cStr::csCyan		= "^J";
const cStr cStr::csMagenta	= "^K";
const cStr cStr::csYellow	= "^L";
const cStr cStr::csOrange	= "^M";
const cStr cStr::csPink		= "^N";
const cStr cStr::csBrown	= "^O";
const cStr cStr::csPurple	= "^P";

cColor ColorTable[16] = {
	cColor::Black, cColor::Black, cColor::Gray75, cColor::Gray50, cColor::Gray25, cColor::White,
		cColor::Red, cColor::Green, cColor::Blue, cColor::Cyan, cColor::Magenta, cColor::Yellow,
		cColor::Orange, cColor::Pink, cColor::Brown, cColor::Purple };

// cStr::Color
cColor & cStr::ColorForIndex(int index) {
	return ColorTable[index & 15];
} // cStr::Color

// cStr::LengthWithoutColors(const char *)
int cStr::LengthWithoutColors(const char *pStr) {
	if(!pStr) {
		return 0;
	}
	int nLen = 0;
	while(*pStr) {
		if(IsColor(pStr)) {
			pStr += 2;
			continue;
		}
		pStr++;
		nLen++;
	}
	return nLen;
} // cStr::LengthWithoutColors(const char *)

//-----------------------------------------------------------------------------
// cStr::LinesMaxLengthWithoutColors
//-----------------------------------------------------------------------------
int cStr::LinesMaxLengthWithoutColors(const char *pStr) {
	if(!pStr) {
		return 0;
	}
	int MaxLen = 0, CurLen = 0;
	while(*pStr) {
		if(IsColor(pStr)) {
			pStr += 2;
			continue;
		}
		if(CharIsNewLine(*pStr)) {
			pStr++;
			MaxLen = cMath::Max(MaxLen, CurLen);
			CurLen = 0;
			continue;
		}
		pStr++;
		CurLen++;
	}
	return cMath::Max(MaxLen, CurLen);
} // cStr::LinesMaxLengthWithoutColors

//-----------------------------------------------------------------------------
// cStr::LinesCount
//-----------------------------------------------------------------------------
int cStr::LinesCount(const char *pStr) {
	if(!pStr) {
		return 0;
	}
	int Count = 1;
	while(*pStr) {
		if(CharIsNewLine(*pStr)) {
			Count++;
		}
		pStr++;
	}
	return Count;
} // cStr::LinesCount

//-----------------------------------------------------------------------------
// Equals
//-----------------------------------------------------------------------------
bool cStr::Equals(const char *s1, const char *s2) {
	while(*s1 && *s2)
		if(*s1++ != *s2++) return false;
	if(*s1 != *s2) return false;
	else return true;
}

//-----------------------------------------------------------------------------
// Equals
//-----------------------------------------------------------------------------
bool cStr::Equals(const char *s1, const char *s2, const int MaxLength) {
	int index = 0;
	while(*s1 && *s2 && index < MaxLength) {
		if(*s1++ != *s2++) {
			return false;
		}
		index++;
	}
	if(*s1 != *s2 && index < MaxLength) {
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
// EqualsNoCase
//-----------------------------------------------------------------------------
bool cStr::EqualsNoCase(const char *s1, const char *s2) {
	while(*s1 && *s2) {
		int c1 = *s1++;
		int c2 = *s2++;
		int d = c1 - c2;
		if(d) {
			if(cMath::IsInRange(c1, (int)'A', (int)'Z')) {
				d += 'a' - 'A';
			} if(d) {
				if(cMath::IsInRange(c2, (int)'A', (int)'Z')) {
					d -= 'a' - 'A';
				}
			}
			if(d) {
				return false;
			}
		}
	}
	if(*s1 != *s2) return false;
	else return true;
}

//-----------------------------------------------------------------------------
// Cmp
//-----------------------------------------------------------------------------
int cStr::Cmp(const char *s1, const char *s2) {
	int c1, c2, d;
	do {
		c1 = *s1++;
		c2 = *s2++;
		d = c1 - c2;
		if(d) {
			return 1; // ?????????????????????????
		}
	} while(c1);
	return 0;
}

//--------------------------------------------------------------------------------------
// cStr::FindChar
//--------------------------------------------------------------------------------------
int cStr::FindChar(const char *Str, const char c, int Start /*= 0*/, int End /*= -1*/) {
	if(-1 == End) {
		End = Length(Str) - 1;
	}

	for(int i = Start; i <= End; i++) {
		if(Str[i] == c) {
			return i;
		}
	}
	return -1;
} // cStr::FindChar

//-------------------------------------------------------------------------------------------------------------------
// cStr::FindText
//-------------------------------------------------------------------------------------------------------------------
int cStr::FindText(const char *Str, const char *Text, bool NoCase /*= false*/, int Start /*= 0*/, int End /*= -1*/) {
	const int StrLen = Length(Str);
	if(StrLen < 1) { // Nowhere to find.
		return -1;
	}
	if(-1 == End) {
		End = StrLen;
	}

	const int TextLen = Length(Text);
	if(TextLen < 1) { // Nothing to find.
		return -1;
	}

	const int Shift = TextLen > 0 ? TextLen - 1 : 0;
	int l = End - Shift;
	for(int i = Start; i <= l; i++) {
		int j;
		if(NoCase) {
			for(j = 0; Text[j]; j++) {
				if(ToUpper(Str[i + j]) != ToUpper(Text[j])) {
					break;
				}
			}
		} else {
			for(j = 0; Text[j]; j++) {
				if(Str[i + j] != Text[j]) {
					break;
				}
			}
		}
		if(!Text[j]) {
			return i;
		}
	}
	return -1;
} // cStr::FindText

// cStr::StripLeading : void (const char)
void cStr::StripLeading(const char c) {
	while(c == m_pStr[0]) {
		MoveMemory(&m_pStr[0], &m_pStr[1], m_nLen);
		m_nLen--;
	}
}

// cStr::StripLeading : void (const char *)
void cStr::StripLeading(const char *Str) {
	const int L = Length(Str);
	if(L > 0) {
		while(Equals(m_pStr, Str, L)) {
			MoveMemory(m_pStr, m_pStr + L, m_nLen - L + 1);
			m_nLen -= L;
		}
	}
}

// cStr::StripTrailing
void cStr::StripTrailing(const char c) {
	for(int i = m_nLen; i > 0 && m_pStr[i - 1] == c; i--) {
		m_pStr[i - 1] = '\0';
		m_nLen--;
	}
}

// cStr::StripTrailing
void cStr::StripTrailing(const char *Str) {
	const int L = Length(Str);
	if(L < 1) { // Nothing to strip.
		return;
	}
	
	while(m_nLen >= L && Equals(Str, m_pStr + m_nLen - L, L)) {
		m_nLen -= L;
		m_pStr[m_nLen] = '\0';
	}
}

//-----------------------------------------------------------------------------
// cStr::ReplaceChar
//-----------------------------------------------------------------------------
void cStr::ReplaceChar(const char Fm, const char To) {
	for(int i = 0; i < m_nLen; i++) {
		if(Fm == m_pStr[i]) {
			m_pStr[i] = To;
		}
	}
} // cStr::ReplaceChar

//-----------------------------------------------------------------------------
// cStr::ReplaceChars
//-----------------------------------------------------------------------------
void cStr::ReplaceChars(const char *Fm, const char To) {
	while(*Fm) {
		for(int i = 0; i < m_nLen; i++) {
			if(*Fm == m_pStr[i]) {
				m_pStr[i] = To;
			}
		}
		Fm++;
	}
} // cStr::ReplaceChars

//-----------------------------------------------------------------------------
// cStr::Replace
//-----------------------------------------------------------------------------
int cStr::Replace(const char *Fm, const char *To) {
	cStr S(m_pStr);
	const int FmLen = Length(Fm);
	const int ToLen = Length(To);

	// How big the new string will be:
	int Count = 0;
	for(int i = 0; i < S.Length(); i++) {
		if(Equals(&S[i], Fm, FmLen)) {
			Count++;
			i += FmLen - 1;
		}
	}
	int Replaced = 0;
	if(Count) {
		EnsureAllocated(m_nLen + (ToLen - FmLen) * Count + 2, false);
		int j = 0;
		int i;
		for(i = 0, j = 0; i < S.Length(); i++) {
			if(Equals(&S[i], Fm, FmLen)) {
				CopyMemory(m_pStr + j, To, ToLen);
				i += FmLen - 1;
				j += ToLen;
				Replaced++;
			} else {
				m_pStr[j] = S[i];
				j++;
			}
		}
		m_pStr[j] = '\0';
		m_nLen = Length(m_pStr);
	}
	return Replaced;
} // cStr::Replace

//------------------------------------------------------------------------------------------
// cStr::Split
//------------------------------------------------------------------------------------------
void cStr::Split(const char *Src, cList<cStr> &List, const char *Delimiters /*= " ,;\t"*/) {
	List.Clear();

	cStr S(Src);
	if(S.Length() < 1) { // Nothing to split.
		return;
	}

	if(Length(Delimiters) < 1) { // No delimiters. Add whole string.
		List.Add(S);
		return;
	}
	
	S.ReplaceChars(Delimiters, Delimiters[0]);
	cList<char *> Words;
	char *c = S.ToCharPtr();
	while(c != '\0') {
		// Jump over leading delimiters:
		while(Delimiters[0] == *c) {
			c++;
		}
		// Break if we reach the end:
		if('\0' == *c) {
			break;
		}
		// Save pointer to the beginning of word:
		Words.Add(c);
		// Jump over non delimiters:
		while(*c != '\0' && *c != Delimiters[0]) {
			c++;
		}
		// Break if we reach the end:
		if('\0' == *c) {
			break;
		}
		// Place a null char here to mark the end of the word:
		*c++ = '\0';
	}

	for(int i = 0; i < Words.Count(); i++) {
		List.Add(cStr(Words[i]));
	}
} // cStr::Split

//-----------------------------------------------------------------------------
// cStr::GetMid
//-----------------------------------------------------------------------------
const cStr cStr::GetMid(const int Start, const int Len) const {
	cStr Res;

	const int l = Length();
	if(l == 0 || Len <= 0 || Start >= l) {
		return Res;
	}

	Res.Append(&m_pStr[Start], Start + Len >= l ? l - Start : Len);
	return Res;
} // cStr::GetMid

//-----------------------------------------------------------------------------
// cStr::StripFileExtension
//-----------------------------------------------------------------------------
void cStr::StripFileExtension() {
	for(int i = m_nLen - 1; i >= 0; i--) {
		if(m_pStr[i] == '.') {
			CapLength(i);
			break;
		}
	}
} // cStr::StripFileExtension

//-----------------------------------------------------------------------------
// cStr::StripFileName
//-----------------------------------------------------------------------------
void cStr::StripFileName() {
	int Pos = Length() - 1;
	while(Pos > 0 && m_pStr[Pos] != '/' && m_pStr[Pos] != '\\') {
		Pos--;
	}
	if(Pos < 0) {
		Pos = 0;
	}
	CapLength(Pos);
} // cStr::StripFileName

//-----------------------------------------------------------------------------
// cStr::StripFilePath
//-----------------------------------------------------------------------------
void cStr::StripFilePath() {
	int Pos = Length();
	while(Pos > 0 && m_pStr[Pos - 1] != '/' && m_pStr[Pos - 1] != '\\') {
		Pos--;
	}
	*this = GetRight(Length() - Pos);
} // cStr::StripFilePath

//-----------------------------------------------------------------------------
// cStr::SetFileExtension
//-----------------------------------------------------------------------------
void cStr::SetFileExtension(const char *Ext) {
	StripFileExtension();
	if(*Ext != '.') {
		Append('.');
	}
	Append(Ext);
} // cStr::SetFileExtension

//-----------------------------------------------------------------------------
// cStr::SetFileDefaultExtension
//-----------------------------------------------------------------------------
void cStr::SetFileDefaultExtension(const char *DefExt) {
	for(int i = m_nLen - 1; i >= 0; i--) {
		if(m_pStr[i] == '.') {
			return; // String already has an extension.
		}
	}
	if(*DefExt != '.') {
		Append('.');
	}
	Append(DefExt);
} // cStr::SetDefaultFileExtension

// cStr::SetFilePath
void cStr::SetFilePath(const char *Path) {
	cStr fn = GetFileName();
	*this = Path;
	AppendPath(fn);
}

//-----------------------------------------------------------------------------
// cStr::SetDefaultFilePath
//-----------------------------------------------------------------------------
void cStr::SetFileDefaultPath(const char *DefPath) {
	if(!GetFilePath().Length()) {
		cStr fn = *this;
		*this = DefPath;
		AppendPath(fn);
	}
}

//-----------------------------------------------------------------------------
// cStr::GetFileExtension
//-----------------------------------------------------------------------------
const cStr cStr::GetFileExtension() const {
	int Pos = Length() - 1;
	while(Pos > 0 && m_pStr[Pos - 1] != '.') {
		Pos--;
	}
	cStr Ext;
	if(Pos > 0) {
		Ext = GetRight(Length() - Pos);
	}
	return Ext;
} // cStr::GetFileExtension

//-----------------------------------------------------------------------------
// cStr::GetFilePath
//-----------------------------------------------------------------------------
const cStr cStr::GetFilePath() const {
	int Pos = Length();
	while(Pos > 0 && m_pStr[Pos - 1] != '/' && m_pStr[Pos - 1] != '\\') {
		Pos--;
	}
	return GetLeft(Pos);
} // cStr::GetFilePath

//-----------------------------------------------------------------------------
// cStr::GetFileName
//-----------------------------------------------------------------------------
const cStr cStr::GetFileName() const {
	int Pos = Length() - 1;
	while(Pos > 0 && m_pStr[Pos - 1] != '/' && m_pStr[Pos - 1] != '\\') {
		Pos--;
	}
	return GetRight(Length() - Pos);
} // cStr::GetFileName

//-----------------------------------------------------------------------------
// cStr::GetFileBase
//-----------------------------------------------------------------------------
const cStr cStr::GetFileBase() const {
	int Pos = Length() - 1;
	while(Pos > 0 && m_pStr[Pos - 1] != '/' && m_pStr[Pos - 1] != '\\') {
		Pos--;
	}
	int Start = Pos;
	while(Pos < Length() && m_pStr[Pos] != '.') {
		Pos++;
	}
	return GetMid(Start, Pos - Start);
} // cStr::GetFileBase

//-----------------------------------------------------------------------------
// cStr::AppendPath
//-----------------------------------------------------------------------------
void cStr::AppendPath(const char *Path) {
	if(Path && Path[0]) {
		EnsureAllocated(m_nLen + Length(Path) + 2);
		int Pos = m_nLen;
		if(Pos) {
			if(m_pStr[Pos - 1] != '/' && m_pStr[Pos - 1] != '\\') {
				m_pStr[Pos++] = '\\';
			}
		}
		int i = 0;
		if(Path[i] == '/' || Path[i] == '\\') {
			i++;
		}
		for(; Path[i]; i++) {
			if(Path[i] == '/') {
				m_pStr[Pos++] = '\\';
			} else {
				m_pStr[Pos++] = Path[i];
			}
		}
		m_nLen = Pos;
		m_pStr[Pos] = '\0';
	}
} // cStr::AppendPath

//-----------------------------------------------------------------------------
// cStr::BackSlashesToSlashes
//-----------------------------------------------------------------------------
void cStr::BackSlashesToSlashes() {
	for(int i = 0; i < m_nLen; i++) {
		if(m_pStr[i] == '\\') {
			m_pStr[i] = '/';
		}
	}
} // cStr::BackSlashesToSlashes

//-----------------------------------------------------------------------------
// cStr::SlashesToBackSlashes
//-----------------------------------------------------------------------------
void cStr::SlashesToBackSlashes() {
	for(int i = 0; i < m_nLen; i++) {
		if(m_pStr[i] == '/') {
			m_pStr[i] = '\\';
		}
	}
}

//-----------------------------------------------------------------------------
// cStr::Copy
//-----------------------------------------------------------------------------
void cStr::Copy(char *To, const char *Fm, int Size) {
	if(!Fm) {
		cLog::Warning("cStr::Copy(): Fm is NULL");
		return;
	}
	if(Size < 1) {
		cLog::Warning("cStr::Copy(): Size < 1");
		return;
	}
	CopyMemory(To, Fm, Size - 1);
	To[Size - 1] = '\0';
} // cStr::Copy

//-----------------------------------------------------------------------------
// cStr::Format
//-----------------------------------------------------------------------------
const cStr cStr::Format(const char *Format, ...) {
	va_list Args;
	va_start(Args, Format);
	cStr Str;
	Printfv(Str, Format, Args);
	va_end(Args);
	return Str;
} // cStr::Format

//-----------------------------------------------------------------------------
// cStr::Printf
//-----------------------------------------------------------------------------
int cStr::Printf(char *Str, int Size, const char *Format, ...) {
	char Buffer[10000];
	va_list Args;
	va_start(Args, Format);
	int Len = vsprintf(Buffer, Format, Args);
	va_end(Args);
	if(Len >= sizeof(Buffer)) {
		cLog::Error("cStr::Printf: Buffer is overflowed");
	}
	if(Len >= Size) {
		cLog::Warning("cStr::Printf: Formatted Buffer[%i] exceeds reserved Str[%i]", Len, Size);
		Len = Size;
	}
	Copy(Str, Buffer, Size);
	return Len;
} // cStr::Printf

//-----------------------------------------------------------------------------
// cStr::Printfv
//-----------------------------------------------------------------------------
int cStr::Printfv(char *Str, int Size, const char *Format, va_list Args) {
	int Len = _vsnprintf(Str, Size - 1, /*_TRUNCATE, */Format, Args);
	Str[Size - 1] = '\0';
	if(Len < 0 || Len >= Size) {
		return -1;
	}
	return Len;
} // cStr::Printfv

//-----------------------------------------------------------------------------
// cStr::Printf
//-----------------------------------------------------------------------------
int cStr::Printf(cStr &Str, const char *Format, ...) {
	char Buffer[10000];
	va_list Args;
	va_start(Args, Format);
	int Len = Printfv(Buffer, sizeof(Buffer) - 1, Format, Args);
	va_end(Args);
	Buffer[sizeof(Buffer) - 1] = '\0';
	Str = Buffer;
	return Len;
} // cStr::Printf

//-----------------------------------------------------------------------------
// cStr::Printfv
//-----------------------------------------------------------------------------
int cStr::Printfv(cStr &Str, const char *Format, va_list Args) {
	char Buffer[10000];
	int Len = Printfv(Buffer, sizeof(Buffer) - 1, Format, Args);
	Buffer[sizeof(Buffer) - 1] = '\0';
	Str = Buffer;
	return Len;
} // cStr::Printfv

//-----------------------------------------------------------------------------
// cStr::ToInt
//-----------------------------------------------------------------------------
int cStr::ToInt(const char *pStr) {
	// [Whitespace]						Consists of space or tab characters, which are ignored.
	// [Sign]							Is either plus (+) or minus (-).
	// Digits							Are one or more decimal digits.

	enum States {
		StartScanning, ScannedSign, ScanDigits
	};

	States State = StartScanning;
	cList<char> Stack;
	int Sign = 1;
	bool Done = false;

	while(*pStr && !Done) {
		const int c = *pStr++;

		switch(State) {
			case StartScanning:
				if(CharIsWhitespace(c)) {
					// Ignoring.
				} else if(CharIsNumeric(c)) {
					Stack.Add(c);
					State = ScanDigits;
				} else if(CharIsSign(c)) {
					if('-' == c) {
						Sign = -1;
					}
					State = ScannedSign;
				} else {
					Done = true;
				}
				break;
			case ScannedSign:
				if(CharIsNumeric(c)) {
					Stack.Add(c);
					State = ScanDigits;
				} else {
					Done = true;
				}
				break;
			case ScanDigits:
				if(CharIsNumeric(c)) {
					Stack.Add(c);
				} else {
					Done = true;
				}
				break;
		}
	}

	// Is current state accepting?
	if(State != ScanDigits) {
		return 0; // Error.
	}

	// Building value:
	int Value = 0;
	for(int i = 0; i < Stack.Count(); i++) {
		const int d = Stack[i] - '0';
		Value *= 10;
		Value += d;
	}
	Value *= Sign;

	return Value;
} // cStr::ToInt

//-----------------------------------------------------------------------------
// cStr::ToFloat
//-----------------------------------------------------------------------------
float cStr::ToFloat(const char *pStr) {
	// [Whitespace]						Consists of space or tab characters, which are ignored.
	// [Sign]							Is either plus (+) or minus (-).
	// [Digits]							Are one or more decimal digits.
	// [.Digits]						If no digits appear before the decimal point,
	//									at least one must appear after the decimal point.
	// [ {d | D | e | E }[Sign]Digits]	The decimal point may be followed by an exponent,
	//									which consists of an introductory letter (d, D, e, or E) and
	//									an optionally signed decimal integer.
	
	enum States {
		StartScanning, ScannedSign, ScanLeadDigits, ScannedDecimalPoint, ScanDecimalDigits
	};
	
	States State = StartScanning;
	cList<char> Integer, Fraction;
	float Sign = 1.0f;
	bool Done = false;
	int Exponent = 0;
	while(*pStr && !Done) {
		const int c = *pStr++;

		switch(State) {
			case StartScanning:
				if(CharIsWhitespace(c)) {
					// Ignoring.
				} else if(CharIsNumeric(c)) {
					Integer.Add(c);
					State = ScanLeadDigits;
				} else if(CharIsDecimalPoint(c)) {
					State = ScannedDecimalPoint;
				} else if(CharIsSign(c)) {
					if('-' == c) {
						Sign = -1.0f;
					}
					State = ScannedSign;
				} else {
					Done = true;
				}
				break;
			case ScannedSign:
				if(CharIsDecimalPoint(c)) {
					State = ScannedDecimalPoint;
				} else if(CharIsNumeric(c)) {
					Integer.Add(c);
					State = ScanLeadDigits;
				} else {
					Done = true;
				}
				break;
			case ScanLeadDigits:
				if(CharIsNumeric(c)) {
					Integer.Add(c);
				} else if(CharIsDecimalPoint(c)) {
					State = ScanDecimalDigits;
				} else if(CharIsExponent(c)) {
					Exponent = ToInt(pStr);
					Done = true;
				} else {
					Done = true;
				}
				break;
			case ScannedDecimalPoint:
				if(CharIsNumeric(c)) {
					Fraction.Add(c);
					State = ScanDecimalDigits;
				} else {
					Done = true;
				}
				break;
			case ScanDecimalDigits:
				if(CharIsNumeric(c)) {
					Fraction.Add(c);
				} else if(CharIsExponent(c)) {
					Exponent = ToInt(pStr);
					Done = true;
				} else {
					Done = true;
				}
				break;
		}
	}

	// Is current state accepting?
	if(State != ScanLeadDigits && State != ScanDecimalDigits) {
		return 0.0f; // Error.
	}

	// Integer part:
	int IntegerPart = 0;
	for(int i = 0; i < Integer.Count(); i++) {
		const int d = Integer[i] - '0';
		IntegerPart *= 10;
		IntegerPart += d;
	}
	// Fraction part:
	float FractionPart = 0.0f;
	for(int i = Fraction.Count() - 1; i >= 0; i--) {
		const int d = Fraction[i] - '0';
		FractionPart += (float)d;
		FractionPart *= 0.1f;
	}
	// Building value:
	float Value = Sign * ((float)IntegerPart + FractionPart) * cMath::Pow(10.0f, (float)Exponent);
	
	return Value;
} // cStr::ToFloat