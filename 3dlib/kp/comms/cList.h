#pragma once

template<class T>
inline int cListSortCompare(const T *u, const T *v) {
	return *u - *v;
}

// Dynamic list template class.
// This template class implements a list allocated and expanded in execution time.
template <class T>
class cList {
friend class dVectors;
public:
	// Default constructor:
	cList(int nGrowBy = 32);
	// Copy - constructor:
	cList(const cList<T> &src);
	// Default destructor:
	~cList();

	// Returns total size of allocated memory (in bytes).
	size_t Allocated() const;
	// Returns total size of allocated memory including size of cList type.
	size_t Size() const;

	// Assignment operator:
	cList<T> & Copy(const cList<T> &Src);
	cList<T> & operator = (const cList<T> &src);
	// Access operators:
	const T & operator [] (int index) const;
	T & operator [] (int index);
	T * operator + (int index);
	const T & GetAt(int index) const;
	T & GetAt(int index);
	const T & GetBack() const;
	T & GetBack();
	void SetAt(int index, const T &value);
	const T * ToPtr() const; // Returns a const pointer to the list.
	T * ToPtr(); // Returns a pointer to the list.

	void Free(); // Removes all items from the list and frees all the space.
	void FreeContents();
	void Clear();
	int Count() const; // Returns the number of elements contained in the list.
	int Num() const;
	int Add(const T &value, int nCount = 1); // Adds nCounr elements to the end of the list. Returns the index of the added element.
	int AddHashed(const T &value);
	int Append(const cList<T> &src); // Appends another list to the list. Returns the index of the last appended element.
	int Append(const T *Src, int nCount);
	int IndexOf(const T &value) const; // Returns the index of value if found in the list; otherwise, -1.
	void IndicesOf(const T &value, cList<DWORD> &Indices) const;
	bool Contains(const T &value) const; // Returns true if value is found in the list; otherwise, false.
	void RemoveAt(int index, int nCount = 1); // Removes the nCount list items at the specified index.
	void RemoveBack();
	bool Remove(const T &value); // Removes the first occurence of a specific object from the list. Returns true if value is found; otherwise, false.
	void RemoveDups(cList<DWORD> *pRemap = NULL);
	int InsertAt(int index, const T &value, int nCount = 1); // Inserts nCount items to the list at the specified position. Returns the index of the last inserted item.
	int InsertAt(int index, const cList<T> &src); // Inserts another list to the list at the specified position. Returns the index of the last inserted item.
	int Insert(const T &value, int index, int nCount = 1);
	void Reverse(); // Reverses the order of elements in the list.
	void Check(int nCount); // Checks for required amount of space for value count and allocates if needed.
	void Lock(int nCount); // Locks the list in requested value count. Number of items in the list will be equal to requested.
	void Lock(int nCount, const T &value); // Locks the list in requested value count and sets all items to value.
	void Pack(); // Resizes the list to exactly the number of elements it contains.
	void BeginHashedSearch();
	void EndHashedSearch();
	typedef int CmpFn(const T *, const T *);
	void Sort(CmpFn *fn = (CmpFn *)&cListSortCompare<T>);
	void SortSubSection(int StartIndex, int EndIndex, CmpFn *fn = (CmpFn *)&cListSortCompare<T>);
private:
	int m_nUsed, m_nAllocated, m_nGrowBy;
	T *m_pList;
	cHashIndex *m_pHash;
	void Resize(int nNewAllocated);
};

typedef cList<int> ints;
typedef cList<float> floats;
typedef cList<double> doubles;
typedef cList<DWORD> DWORDS;
typedef cList<bool> bools;

// cList<T>::cList(int)
template <class T>
inline cList<T>::cList(int nGrowBy) {
	assert(nGrowBy > 0);
	m_pList = NULL;
	m_pHash = NULL;
	m_nGrowBy = nGrowBy;
	Free();
}

// cList<T>::cList(const cList<T> &)
template <class T>
inline cList<T>::cList(const cList<T> &src) {
	m_pList = NULL;
	m_pHash = NULL;
	*this = src;
}

// cList<T>::~cList<T>
template <class T>
inline cList<T>::~cList() {
	Free();
}

// cList<T>::Allocated
template <class T>
inline size_t cList<T>::Allocated() const {
	return m_nAllocated * sizeof(T);
}

// cList<T>::Size
template <class T>
inline size_t cList<T>::Size() const {
	return sizeof(cList<T>) + Allocated();
}

//-----------------------------------------------------------------------------
// Copy : cList<T> & Copy(const cList<T> &)
//-----------------------------------------------------------------------------
template <class T>
inline cList<T> & cList<T>::Copy(const cList<T> &Src) {
	Free();

	m_nUsed = Src.m_nUsed;
	m_nAllocated = Src.m_nAllocated;
	m_nGrowBy = Src.m_nGrowBy;

	if(m_nAllocated) {
		m_pList = new T[m_nAllocated];
		for(int i = 0; i < m_nUsed; i++) m_pList[i] = Src.m_pList[i];
	}
	return *this;
}

// cList<T>::operator =
template <class T>
inline cList<T> & cList<T>::operator = (const cList<T> &src) {
	return Copy(src);
}

// cList<T>::operator [] const
template <class T>
inline const T & cList<T>::operator [] (int index) const {
	assert(index >= 0);
	assert(index < m_nUsed);
	return m_pList[index];
}

// cList<T>::operator []
template <class T>
inline T & cList<T>::operator [] (int index) {
	assert(index >= 0);
	assert(index < m_nUsed);
	return m_pList[index];
}

// cList<T>::operator +
template <class T>
inline T * cList<T>::operator + (int index) {
	assert(index >= 0);
	assert(index < m_nUsed);
	return m_pList + index;
}

// cList<T>::GetAt const
template <class T>
inline const T & cList<T>::GetAt(int index) const {
	assert(index >= 0);
	assert(index < m_nUsed);
	if(index >= 0 && index < m_nUsed) return m_pList[index];
	return m_pList[0];
}

// cList<T>::GetAt
template <class T>
inline T & cList<T>::GetAt(int index) {
	assert(index >= 0);
	assert(index < m_nUsed);
	if(index >= 0 && index < m_nUsed) return m_pList[index];
	return m_pList[0];
}

// cList<T>::GetBack : const T & () const
template<class T>
inline const T & cList<T>::GetBack() const {
	assert(m_nUsed >= 1);
	return m_pList[m_nUsed - 1];
}

// cList<T>::GetBack : T & ()
template<class T>
inline T & cList<T>::GetBack() {
	assert(m_nUsed >= 1);
	return m_pList[m_nUsed - 1];
}

// cList<T>::SetAt
template <class T>
inline void cList<T>::SetAt(int index, const T &value) {
	assert(index >= 0);
	assert(index < m_nUsed);
	if(index >= 0 && index < m_nUsed) m_pList[index] = value;
}

// cList<T>::ToPtr() const
template <class T>
inline const T * cList<T>::ToPtr() const {
	return m_pList;
}

// cList<T>::ToPtr
template <class T>
inline T * cList<T>::ToPtr() {
	return m_pList;
}

template <class T>
inline void cList<T>::Clear() {
	m_nUsed = 0;
}

// cList<T>::Free
template <class T>
inline void cList<T>::Free() {
	if(m_pList) {
		delete[] m_pList;
		m_pList = NULL;
	}
	if(m_pHash) {
		delete m_pHash;
		m_pHash = NULL;
	}
	m_nUsed = m_nAllocated = 0;
}

// cList<T>::FreeContents
template<class T>
inline void cList<T>::FreeContents() {
	for(int i = 0; i < m_nUsed; i++) {
		delete m_pList[i];
		m_pList[i] = NULL;
	}
}

// cList<T>::Count
template <class T>
inline int cList<T>::Count() const {
	return m_nUsed;
}

// cList<T>::Num : int() const
template<class T>
inline int cList<T>::Num() const {
	return m_nUsed;
}

// cList<T>::Add
template <class T>
inline int cList<T>::Add(const T &value, int nCount) {
	assert(nCount >= 0);
	Check(m_nUsed + nCount);
	for(int i = 0; i < nCount; i++) {
		m_pList[m_nUsed] = value;
		m_nUsed++;
	}
	return m_nUsed - 1;
}

template <class T>
inline int cList<T>::AddHashed(const T &value) {
	assert(m_pHash);
	int key = m_pHash->GenerateKey(value);
	int index = Add(value);
	m_pHash->Add(key, index);
	return index;
}

//-----------------------------------------------------------------------------
// cList<T>::Append : int(const cList<T> &)
//-----------------------------------------------------------------------------
template <class T>
inline int cList<T>::Append(const cList<T> &src) {
	Check(m_nUsed + src.m_nUsed);
	for(int i = 0; i < src.m_nUsed; i++) {
		m_pList[m_nUsed] = src.m_pList[i];
		m_nUsed++;
	}
	return m_nUsed - 1;
}

//-----------------------------------------------------------------------------
// Append : int(int T *, int)
//-----------------------------------------------------------------------------
template <class T>
inline int cList<T>::Append(const T *Src, int nCount) {
	assert(nCount >= 0);
	Check(m_nUsed + nCount);
	for(int i = 0; i < nCount; i++) {
		m_pList[m_nUsed] = Src[i];
		m_nUsed++;
	}
	return m_nUsed - 1;
}

//-----------------------------------------------------------------------------
// IndexOf
//-----------------------------------------------------------------------------
template <class T>
inline int cList<T>::IndexOf(const T &value) const {
	if(m_pHash) {
		int key = m_pHash->GenerateKey(value);
		for(int index = m_pHash->First(key); index != -1; index = m_pHash->Next(index)) {
			if(value == m_pList[index]) break;
		}
		return index;
	} else {
		for(int i = 0; i < m_nUsed; i++)
			if(value == m_pList[i]) return i;
		return -1;
	}
}

//-----------------------------------------------------------------------------
// IndicesOf
//-----------------------------------------------------------------------------
template <class T>
inline void cList<T>::IndicesOf(const T &value, cList<DWORD> &Indices) const {
	Indices.Free();
	if(m_pHash) {
		int key = m_pHash->GenerateKey(value);
		for(int index = m_pHash->First(key); index != -1; index = m_pHash->Next(index)) {
			if(value == m_pList[index]) Indices.Add(index);
		}
	} else {
		for(int i = 0; i < m_nUsed; i++) {
			if(value == m_pList[i]) Indices.Add(i);
		}
	}
}

//-----------------------------------------------------------------------------
// Contains
//-----------------------------------------------------------------------------
template <class T>
inline bool cList<T>::Contains(const T &value) const {
	if(m_pHash) {
		int key = m_pHash->GenerateKey(value);
		for(int index = m_pHash->First(key); index != -1; index = m_pHash->Next(index)) {
			if(value == m_pList[index]) return true;
		}
		return false;
	} else {
		for(int i = 0; i < m_nUsed; i++)
			if(value == m_pList[i]) return true;
		return false;
	}
}

// cList<T>::RemoveAt
template <class T>
inline void cList<T>::RemoveAt(int index, int nCount) {
	assert(m_pList != NULL);
	assert(index >= 0);
	assert(nCount >= 0);
	assert(index + nCount <= m_nUsed);

	if(index < 0 || nCount <= 0 || index + nCount > m_nUsed) return;

	m_nUsed -= nCount;
	for(int i = index; i < m_nUsed; i++) m_pList[i] = m_pList[i + nCount];
}

// cList<T>::RemoveBack : void()
template<class T>
inline void cList<T>::RemoveBack() {
	assert(m_pList != NULL);
	assert(m_nUsed >= 1);

	if(m_nUsed <= 0) return;
	m_nUsed--;
}

// cList<T>::Remove
template <class T>
inline bool cList<T>::Remove(const T &value) {
	int index = IndexOf(value);
	if(index >= 0) {
		RemoveAt(index);
		return true;
	}
	return false;
}

// cList<T>::InsertAt(int, int, int = 1)
template <class T>
inline int cList<T>::InsertAt(int index, const T &value, int nCount) {
	assert(index >= 0);
	assert(index <= m_nUsed);
	assert(nCount >= 0);

	if(nCount <= 0) return -1;
	if(index < 0) index = 0;
	else if(index > m_nUsed) index = m_nUsed;

	Check(m_nUsed + nCount);
	for(int i = m_nUsed + nCount - 1; i >= index + nCount; i--) m_pList[i] = m_pList[i - nCount];
	m_nUsed += nCount;

	assert(index + nCount <= m_nUsed);
	while(nCount--)
		m_pList[index++] = value;
	return index - 1;
}

// cList<T>::InsertAt(int, int, int = 1)
template<class T>
inline int cList<T>::Insert(const T &value, int index, int nCount) {
	assert(index >= 0);
	assert(index <= m_nUsed);
	assert(nCount >= 0);

	if(nCount <= 0) return -1;
	if(index < 0) index = 0;
	else if(index > m_nUsed) index = m_nUsed;

	Check(m_nUsed + nCount);
	for(int i = m_nUsed + nCount - 1; i >= index + nCount; i--) m_pList[i] = m_pList[i - nCount];
	m_nUsed += nCount;

	assert(index + nCount <= m_nUsed);
	while(nCount--)
		m_pList[index++] = value;
	return index - 1;
}

// cList<T>::InsertAt(int, const cList<T> &)
template <class T>
inline int cList<T>::InsertAt(int index, const cList<T> &src) {
	assert(index >= 0);
	assert(index <= m_nUsed);

	int nCount = src.Count();
	if(nCount <= 0) return -1;
	if(index < 0) index = 0;
	else if(index > m_nUsed) index = m_nUsed;

	Check(m_nUsed + nCount);
	for(int i = m_nUsed + nCount - 1; i >= index + nCount; i--) m_pList[i] = m_pList[i - nCount];
	m_nUsed += nCount;

	assert(index + nCount <= m_nUsed);
	for(i = 0; i < nCount; i++)
		m_pList[index++] = src.m_pList[i];
	return index - 1;
}

// cList<T>::Reverse
template <class T>
inline void cList<T>::Reverse() {
	if(m_nUsed <= 1) return;
	cList<T> TList(m_nGrowBy);
	for(int i = 0; i < m_nUsed; i++) TList.Append(m_pList[m_nUsed - i - 1]);
	*this = TList;
}

// cList<T>::Resize
template <class T>
inline void cList<T>::Resize(int nNewAllocated) {
	assert(nNewAllocated >= 0);

	if(nNewAllocated <= 0) {
		Free();
		return;
	}

	if(nNewAllocated == m_nAllocated) return;

	T *pTList = m_pList;
	m_nAllocated = nNewAllocated;
	if(m_nAllocated < m_nUsed) m_nUsed = m_nAllocated;

	m_pList = new T[m_nAllocated];
	for(int i = 0; i < m_nUsed; i++) m_pList[i] = pTList[i];

	if(pTList) delete[] pTList;
}

// cList<T>::Check
template <class T>
inline void cList<T>::Check(int nCount) {
	int n2Allocate = 0;
	if(nCount >= m_nAllocated) {
		if(m_nAllocated == 0) n2Allocate = nCount + m_nGrowBy;
		else n2Allocate = nCount + m_nGrowBy + m_nAllocated / 2;
		Resize(n2Allocate);
	}
}

// cList<T>::Lock(int)
template <class T>
inline void cList<T>::Lock(int nCount) {
	assert(nCount >= 0);
	if(nCount < 0) nCount = 0;

	Check(nCount);
	m_nUsed = nCount;
}

// cList<T>::Lock(int, const T &)
template <class T>
inline void cList<T>::Lock(int nCount, const T &value) {
	Lock(nCount);
	for(int i = 0; i < m_nUsed; i++) m_pList[i] = value;
}

// cList<T>::Pack
template <class T>
inline void cList<T>::Pack() {
	if(m_pList) {
		if(m_nUsed) Resize(m_nUsed);
		else Free();
	}
}

//-----------------------------------------------------------------------------
// BeginHashedSearch
//-----------------------------------------------------------------------------
template <class T>
void cList<T>::BeginHashedSearch() {
	assert(NULL == m_pHash);
	int nSize = cMath::Max(cHashIndex::HASH_DEF_SIZE, cMath::CalcPowerOfTwo(Count()) / 2);
	m_pHash = new cHashIndex(nSize, nSize);
	assert(m_pHash);
	for(int index = 0; index < Count(); index++) {
		T n = m_pList[index];
		int key = m_pHash->GenerateKey(n);
		m_pHash->Add(key, index);
	}
}

//-----------------------------------------------------------------------------
// EndHashedSearch
//-----------------------------------------------------------------------------
template <class T>
void cList<T>::EndHashedSearch() {
	if(m_pHash) {
		delete m_pHash;
		m_pHash = NULL;
	}
}

//-----------------------------------------------------------------------------
// RemoveDups
//-----------------------------------------------------------------------------
template <class T>
void cList<T>::RemoveDups(cList<DWORD> *pRemap) {
	if(!Count()) return;
	if(pRemap) {
		pRemap->Free();
		pRemap->Lock(Count());
		pRemap->SetAt(0, 0);
	}
	if(Count() < 2) return;
	int nSize = cMath::Max(cHashIndex::HASH_DEF_SIZE, cMath::CalcPowerOfTwo(Count()) / 2);
	cHashIndex *pHash = new cHashIndex(nSize, nSize);
	assert(pHash);
	cList<T> L;
	for(int i = 0; i < Count(); i++) {
		T n = m_pList[i];
		int key = pHash->GenerateKey(n);
		for(int index = pHash->First(key); index != -1; index = pHash->Next(index)) {
			if(L[index] == n) break;
		}
		if(-1 == index) {
			index = L.Add(n);
			pHash->Add(key, index);
		}
		if(pRemap) {
			pRemap->SetAt(i, index);
		}
	}
	delete pHash;
	Copy(L);
}

//-----------------------------------------------------------------------------
// cList<T>::Sort : void(CmpFn *)
//-----------------------------------------------------------------------------
template<class T>
inline void cList<T>::Sort(CmpFn *fn) {
	if(!m_pList) return;

	typedef int Cmp(const void *, const void *);
	Cmp *f = (Cmp *)fn;
	qsort((void *)m_pList, (size_t)m_nUsed, sizeof(T), f);
}

//-----------------------------------------------------------------------------
// cList<T>::SortSubSection : void(int, int, CmpFn *)
//-----------------------------------------------------------------------------
template<class T>
inline void cList<T>::SortSubSection(int StartIndex, int EndIndex, CmpFn *fn) {
	if(!m_pList) return;

	if(StartIndex < 0) StartIndex = 0;
	if(EndIndex >= m_nUsed) EndIndex = m_nUsed - 1;
	if(StartIndex >= EndIndex) return;

	typedef int Cmp(const void *, const void *);
	Cmp *f = (Cmp *)fn;
	qsort((void *)(&m_pList[StartIndex]), (size_t)(EndIndex - StartIndex + 1), sizeof(T), f);
}