#include "comms.h"

void cStr::toFile(const char *FileName) const {
	assert(FileName);
	FILE *pFile;
	if(NULL == (pFile = fopen(FileName, "w"))) {
		return;
	}
	fputs(m_pStr, pFile);
	fclose(pFile);
}

// cStr::Resize
void cStr::Resize(int nLen, bool bKeepOld) {
	assert(nLen > 0);

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
		strcpy(pNewStr, m_pStr);
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
		Check(1, false);
		m_pStr[0] = '\0';
		m_nLen = 0;
		return *this;
	}
	if(pStr == m_pStr) return *this;

	if(pStr >= m_pStr && pStr <= m_pStr + m_nLen) {
		int d = int(pStr - m_pStr);
		assert(strlen(pStr) < (unsigned)m_nLen);
		for(int i = 0; pStr[i]; i++) {
			m_pStr[i] = pStr[i];
		}
		m_pStr[i] = '\0';
		m_nLen -= d;
		return *this;
	}

	int l = (int)strlen(pStr);
	Check(l + 1, false);
	strcpy(m_pStr, pStr);
	m_nLen = l;
	return *this;
}

//-----------------------------------------------------------------------------
// cStr::operator = : cStr & (const char *)
//-----------------------------------------------------------------------------
cStr & cStr::operator = (const char *pStr) {
	return Copy(pStr);
}

//----------------------------------------------------------------------------------
// cStr::FromArray : cStr(const float *, int, int, const char *)
//----------------------------------------------------------------------------------
cStr cStr::FromArray(const float *pFloats, int nLen, int nPrec, const char *pSep) {
	char Format[64], Buffer[128];
	// Format:
	strcpy(Format, "%.");
	sprintf(Buffer, "%d", nPrec);
	strcat(Format, Buffer);
	strcat(Format, "f");
	// sprintf:
	cStr Str;
	for(int i = 0; i < nLen; i++) {
		float f = pFloats[i];
		sprintf(Buffer, Format, f);
		Str += Buffer;
		if(i < nLen - 1) Str += pSep;
	}
	return Str;
}

// cStr::FromArray
cStr cStr::FromArray(const double *pDoubles, int nLen, int nPrec, const char *pSep) {
	char Format[64], Buffer[128];
	// Format:
	strcpy(Format, "%.");
	sprintf(Buffer, "%d", nPrec);
	strcat(Format, Buffer);
	strcat(Format, "f");
	// sprintf:
	cStr Str;
	for(int i = 0; i < nLen; i++) {
		double d = pDoubles[i];
		sprintf(Buffer, Format, d);
		Str += Buffer;
		if(i < nLen - 1) Str += pSep;
	}
	return Str;
} // cStr::FromArray

const cStr cStr::csDefault	= "^A";
const cStr cStr::csBlack	= "^B";
const cStr cStr::csGrey75	= "^C";
const cStr cStr::csGrey50	= "^D";
const cStr cStr::csGrey25	= "^E";
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
	cColor::Black, cColor::Black, cColor::Grey75, cColor::Grey50, cColor::Grey25, cColor::White,
		cColor::Red, cColor::Green, cColor::Blue, cColor::Cyan, cColor::Magenta, cColor::Yellow,
		cColor::Orange, cColor::Pink, cColor::Brown, cColor::Purple };

// cStr::Color
cColor & cStr::ColorForIndex(int index) {
	return ColorTable[index & 15];
} // cStr::Color

// cStr::LengthWithoutColors(const char *)
int cStr::LengthWithoutColors(const char *pStr) {
	if(!pStr) return 0;
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
// Equals
//-----------------------------------------------------------------------------
bool cStr::Equals(const char *s1, const char *s2) {
	while(*s1 && *s2)
		if(*s1++ != *s2++) return false;
	if(*s1 != *s2) return false;
	else return true;
}

//-----------------------------------------------------------------------------
// IEquals
//-----------------------------------------------------------------------------
bool cStr::IEquals(const char *s1, const char *s2) {
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