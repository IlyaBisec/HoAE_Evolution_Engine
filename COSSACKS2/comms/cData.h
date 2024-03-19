#pragma once

//*****************************************************************************
// cData
//*****************************************************************************
class cData {
public:
	enum eCopyCtor { CopyCtor };
	enum eSetCtor { SetCtor };
	
	cData(const char *FileName = "Untitled") : m_Pos(0), m_FileName(FileName) {}
	cData(const eCopyCtor, const void *Src, const int Size, const char *FileName = NULL);
	cData(const eSetCtor, void *Src, const int Size, const char *FileName = NULL);
	
	const cStr & GetFileName() const { return m_FileName; }
	void SetFileName(const char *FileName) { m_FileName = FileName; }
	bool IsEof() const { return m_Pos >= m_Data.Count(); }
	int GetSize() const { return m_Data.Count(); }
	int GetPos() const { return m_Pos; }
	
	// Moves the data position pointer to a new location that is "Offset" bytes from origin.
	// Returns the offset, in bytes, of the new position from the beginning of the data.
	int SeekCur(const int Offset) const;
	int SeekEnd(const int Offset) const;
	int SeekSet(const int Offset) const;
	
	// Returns the number of bytes actually read, which may be less than MaxSize if the end of data
	// is encountered before reaching MaxSize.
	int Read(void *To, const int MaxSize) const;
	void Write(const void *Fm, const int Size);
	
	const void * GetData() const { return m_Data.ToPtr(); } // Can be NULL.
	
	int	ReadByte() const;
	void WriteByte(int b);
	
	short ReadShort() const;
	void WriteShort(short s);

	word ReadWord() const;
	void WriteWord(word w);

	long ReadLong() const;
	void WriteLong(long l);

	dword ReadDword() const;
	void WriteDword(dword dw);

	float ReadFloat() const;
	void WriteFloat(float f);

	double ReadDouble() const;
	void WriteDouble(double d);

	bool ReadLine(cStr &Str) const;
	bool ReadStr(cStr &Str, const char Term = '\r') const;
	void WriteStr(const char *Str);
private:
	cStr m_FileName;
	cList<char> m_Data;
	mutable int m_Pos;
};

// cData::SeekCur
inline int cData::SeekCur(const int Offset) const {
	m_Pos = cMath::Clamp(m_Pos + Offset, 0, m_Data.Count());
	return m_Pos;
}

// cData::SeekEnd
inline int cData::SeekEnd(const int Offset) const {
	m_Pos = cMath::Clamp(m_Data.Count() + Offset, 0, m_Data.Count());
	return m_Pos;
}

// cData::SeekSet
inline int cData::SeekSet(const int Offset) const {
	m_Pos = cMath::Clamp(Offset, 0, m_Data.Count());
	return m_Pos;
}

// cData::ReadByte
inline int cData::ReadByte() const {
	if(m_Pos >= m_Data.Count()) {
		return -1;
	}
	const int b = m_Data[m_Pos];
	m_Pos++;
	return b;
}

// cData::WriteByte
inline void cData::WriteByte(int b) {
	m_Data.InsertAt(m_Pos, b);
	m_Pos++;
}

// cData::ReadShort
inline short cData::ReadShort() const {
	if(m_Pos + 1 >= m_Data.Count()) {
		return -1;
	}
	const short s = *(short *)(&m_Data[m_Pos]);
	m_Pos += 2;
	return s;
}

// cData::WriteShort
inline void cData::WriteShort(short s) {
	m_Data.InsertAt(m_Pos, 0, 2);
	*(short *)&m_Data[m_Pos] = s;
	m_Pos += 2;
}

// cData::ReadWord
inline word cData::ReadWord() const {
	if(m_Pos + 1 >= m_Data.Count()) {
		return -1;
	}
	const word w = *(word *)(&m_Data[m_Pos]);
	m_Pos += 2;
	return w;
}

// cData::WriteWord
inline void cData::WriteWord(word w) {
	m_Data.InsertAt(m_Pos, 0, 2);
	*(word *)&m_Data[m_Pos] = w;
	m_Pos += 2;
}

// cData::ReadLong
inline long cData::ReadLong() const {
	if(m_Pos + 3 >= m_Data.Count()) {
		return -1;
	}
	const long l = *(long *)(&m_Data[m_Pos]);
	m_Pos += 4;
	return l;
}

// cData::WriteLong
inline void cData::WriteLong(long l) {
	m_Data.InsertAt(m_Pos, 0, 4);
	*(long *)&m_Data[m_Pos] = l;
	m_Pos += 4;
}

// cData::ReadDword
inline dword cData::ReadDword() const {
	if(m_Pos + 3 >= m_Data.Count()) {
		return -1;
	}
	const dword dw = *(dword *)(&m_Data[m_Pos]);
	m_Pos += 4;
	return dw;
}

// cData::WriteDword
inline void cData::WriteDword(dword dw) {
	m_Data.InsertAt(m_Pos, 0, 4);
	*(dword *)&m_Data[m_Pos] = dw;
	m_Pos += 4;
}

// cData::ReadFloat
inline float cData::ReadFloat() const {
	if(m_Pos + 3 >= m_Data.Count()) {
		return 0.0f;
	}
	const float f = *(float *)(&m_Data[m_Pos]);
	m_Pos += 4;
	return f;
}

// cData::WriteFloat
inline void cData::WriteFloat(float f) {
	m_Data.InsertAt(m_Pos, 0, 4);
	*(float *)&m_Data[m_Pos] = f;
	m_Pos += 4;
}

// cData::ReadDouble
inline double cData::ReadDouble() const {
	if(m_Pos + 7 >= m_Data.Count()) {
		return 0.0;
	}
	const double d = *(double *)(&m_Data[m_Pos]);
	m_Pos += 8;
	return d;
}

// cData::WriteDouble
inline void cData::WriteDouble(double d) {
	m_Data.InsertAt(m_Pos, 0, 8);
	*(double *)&m_Data[m_Pos] = d;
	m_Pos += 8;
}