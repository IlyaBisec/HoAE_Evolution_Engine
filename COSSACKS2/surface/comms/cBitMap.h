#pragma once

class cBitMap {
public:
	cBitMap(int nBits = 1); // Default constructor.
	cBitMap(const cBitMap &src); // Copy - constructor.
	cBitMap & operator = (const cBitMap &src); // Assignment operator.
	~cBitMap(); // Default destructor.

	void Set(int index); // Sets ith bit (1).
	void Clear(int index); // Clears ith bit (0).
	void SetAll(); // Sets all bits (1).
	void ClearAll(); // Clears all bits (0).

	int operator [] (int index) const; // Gets ith bit (1 or 0).
private:
	DWORD *m_pBits;
	int m_nBits, m_nCount;

	void Null();
	void Init(int nBits);
	void Free();
};

// cBitMap::Null
inline void cBitMap::Null() {
	m_pBits = NULL;
	m_nBits = 0;
	m_nCount = 0;
}

// cBitMap::Free
inline void cBitMap::Free() {
	if(m_pBits) delete[] m_pBits;
	Null();
}

// cBitMap::Init
inline void cBitMap::Init(int nBits) {
	assert(nBits > 0);

	m_nBits = nBits;
	m_nCount = m_nBits / 32 + (m_nBits % 32 ? 1 : 0);
	m_pBits = new DWORD[m_nCount];
	assert(m_pBits);

	ClearAll();
}

// cBitMap(int)
inline cBitMap::cBitMap(int nBits) {
	Null();
	Init(nBits);
}

// cBitMap::operator =
inline cBitMap & cBitMap::operator = (const cBitMap &src) {
	Free();

	m_nBits = src.m_nBits;
	m_nCount = src.m_nCount;

	if(m_nCount) {
		m_pBits = new DWORD[m_nCount];
		assert(m_pBits);
		CopyMemory(m_pBits, src.m_pBits, m_nCount * sizeof(DWORD));
	}
	return *this;
}

// cBitMap(const cBitMap &)
inline cBitMap::cBitMap(const cBitMap &src) {
	Null();
	*this = src;
}

// ~cBitMap
inline cBitMap::~cBitMap() {
	Free();
}

// cBitMap::Set
inline void cBitMap::Set(int index) {
	assert(index >= 0);
	assert(index < m_nBits);
	m_pBits[index >> 5 /* /32 */] |= 1 << (index & 0x1F /* %32 */);
}

// cBitMap::Clear
inline void cBitMap::Clear(int index) {
	assert(index >= 0);
	assert(index < m_nBits);
	m_pBits[index >> 5 /* /32 */] &= ~(1 << (index & 0x1F /* %32 */));
}

// cBitMap::SetAll
inline void cBitMap::SetAll() {
	assert(m_pBits);
	FillMemory(m_pBits, m_nCount * sizeof(DWORD), 0xff);
}

// cBitMap::ClearAll
inline void cBitMap::ClearAll() {
	assert(m_pBits);
	ZeroMemory(m_pBits, m_nCount * sizeof(DWORD));
}

// cBitMap::operator []
inline int cBitMap::operator [] (int index) const {
	assert(index >= 0);
	assert(index < m_nBits);
	return m_pBits[index >> 5 /* /32 */] & 1 << (index & 0x1F /* %32 */) ? 1 : 0;
}