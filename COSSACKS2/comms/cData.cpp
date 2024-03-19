#include "comms.h"

//-----------------------------------------------------------------------------------------------
// cData.ctor
//-----------------------------------------------------------------------------------------------
cData::cData(const cData::eSetCtor, void *Src, const int Size, const char *FileName /*= NULL*/) {
	m_Data.Set((char *)Src, Size);
	m_Pos = 0;
	m_FileName = FileName;
} // cData.ctor

//------------------------------------------------------------------------------------------------------
// cData.ctor
//------------------------------------------------------------------------------------------------------
cData::cData(const cData::eCopyCtor, const void *Src, const int Size, const char *FileName /*= NULL*/) {
	if(Size > 0 && Src) {
		m_Data.Copy((const char *)Src, Size);
	}
	m_Pos = 0;
	m_FileName = FileName;
} // cData.ctor

// cData::Read
int cData::Read(void *To, const int MaxSize) const {
	if(m_Pos >= m_Data.Count()) {
		return -1;
	}
	
	const int Size = cMath::Min(MaxSize, m_Data.Count() - m_Pos);
	CopyMemory(To, &m_Data[m_Pos], Size);
	m_Pos += Size;
	
	return Size;
}

// cData::Write
void cData::Write(const void *Fm, const int Size) {
	m_Data.AppendAt(m_Pos, (const char *)Fm, Size);
	m_Pos += Size;
}

// cData::ReadLine

//-----------------------------------------------------------------------------
// cData::ReadLine
//-----------------------------------------------------------------------------
bool cData::ReadLine(cStr &Str) const {
	if(m_Pos >= m_Data.Count()) {
		return false;
	}

	Str = "";

	while(m_Pos < m_Data.Count() && m_Data[m_Pos] != '\r' && m_Data[m_Pos] != '\n') { // Read untill '\r' or '\n'.
		Str.Append((char)m_Data[m_Pos]);
		m_Pos++;
	}

	if(m_Pos < m_Data.Count() && (m_Data[m_Pos] == '\r' || m_Data[m_Pos] == '\n')) { // Skip '\r' or '\n'.
		m_Pos++;
	}

	if(m_Pos < m_Data.Count() && m_Data[m_Pos - 1] == '\r' && m_Data[m_Pos] == '\n') { // Skip if "\r\n".
		m_Pos++;
	}
	return true;
} // cData::ReadLine

//-----------------------------------------------------------------------------
// cData::ReadStr
//-----------------------------------------------------------------------------
bool cData::ReadStr(cStr &Str, const char Term /*= '\r'*/) const {
	if(m_Pos >= m_Data.Count()) {
		return false;
	}

	Str = "";

	while(m_Pos < m_Data.Count() && m_Data[m_Pos] != Term) {
		Str.Append((char)m_Data[m_Pos]);
		m_Pos++;
	}

	if(m_Pos < m_Data.Count() && m_Data[m_Pos] == Term) {
		m_Pos++;
	}

	if(Term == '\r' && m_Pos < m_Data.Count() && m_Data[m_Pos] == '\n') {
		m_Pos++;
	}

	return true;
} // cData::ReadStr

// cData::WriteStr
void cData::WriteStr(const char *Str) {
	const int l = cStr::Length(Str);
	m_Data.AppendAt(m_Pos, Str, l); // w/o trailing zero.
	m_Pos += l;
}