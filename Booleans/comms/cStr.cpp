#include "cStr.h"

namespace comms {

// cStr::Resize
void cStr::Resize(int nLen, bool bKeepOld) {
	assert(nLen > 0);

	int nMod = nLen % cStr_nGrowBy;
	int nNewAllocated;
	if(!nMod) {
		nNewAllocated = nLen;
	} else {
		nNewAllocated = nLen + cStr_nGrowBy - nMod;
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

const cStr cStr::csDefault	= "^0";
const cStr cStr::csBlack	= "^1";
const cStr cStr::csGray75	= "^2";
const cStr cStr::csGray50	= "^3";
const cStr cStr::csGray25	= "^4";
const cStr cStr::csWhite	= "^5";
const cStr cStr::csRed		= "^6";
const cStr cStr::csGreen	= "^7";
const cStr cStr::csBlue		= "^8";
const cStr cStr::csCyan		= "^9";
const cStr cStr::csMagenta	= "^10";
const cStr cStr::csYellow	= "^11";

COLORREF ColorTable[16] = {
	cBlack, cBlack, cGray75, cGray50, cGray25, cWhite, cRed, cGreen, cBlue, cCyan, cMagenta, cYellow, 0, 0, 0, 0 };

// cStr::Color
COLORREF & cStr::ColorForIndex(int index) {
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
			if(comms::RangeCheck(c1, (int)'A', (int)'Z')) {
				d += 'a' - 'A';
				if(!d) break;
			}
			if(comms::RangeCheck(c2, (int)'A', (int)'Z')) {
				d -= 'a' - 'A';
				if(!d) break;
			}
			return false;
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

} // comms