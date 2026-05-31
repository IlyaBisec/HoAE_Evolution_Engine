#ifndef __cList__
#define __cList__

#include <windows.h>
#include <assert.h>
#include <stdio.h>

#pragma pack(push)
#pragma pack(1)

namespace comms {

// Dynamic list template class.
// This template class implements a list allocated and expanded in execution time.
template <class T>
class cList {
friend class cVectors;
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
	cList<T> & operator = (const cList<T> &src);
	// Access operators:
	const T & operator [] (int index) const;
	T & operator [] (int index);
	T * operator + (int index);
	const T & GetAt(int index) const;
	T & GetAt(int index);
	void SetAt(int index, const T &value);
	const T * Ptr() const; // Returns a const pointer to the list.
	T * Ptr(); // Returns a pointer to the list.

	void Free(); // Removes all items from the list and frees all the space.
	int Count() const; // Returns the number of elements contained in the list.
	int Add(const T &value); // Adds an element to the end of the list. Returns the index of the added element.
	int Append(const cList<T> &src); // Appends another list to the list. Returns the index of the last appended element.
	int IndexOf(const T &value) const; // Returns the index of value if found in the list; otherwise, -1.
	bool Contains(const T &value) const; // Returns true if value is found in the list; otherwise, false.
	void RemoveAt(int index, int nCount = 1); // Removes the nCount list items at the specified index.
	bool Remove(const T &value); // Removes the first occurence of a specific object from the list. Returns true if value is found; otherwise, false.
	int InsertAt(int index, const T &value, int nCount = 1); // Inserts nCount items to the list at the specified position. Returns the index of the last inserted item.
	int InsertAt(int index, const cList<T> &src); // Inserts another list to the list at the specified position. Returns the index of the last inserted item.
	void Reverse(); // Reverses the order of elements in the list.
	void Check(int nCount); // Checks for required amount of space for value count and allocates if needed.
	void Lock(int nCount); // Locks the list in requested value count. Number of items in the list will be equal to requested.
	void Lock(int nCount, const T &value); // Locks the list in requested value count and sets all items to value.
	void Pack(); // Resizes the list to exactly the number of elements it contains.
private:
	int m_nUsed, m_nAllocated, m_nGrowBy;
	T *m_pList;
	void Resize(int nNewAllocated);
};

// cList<T>::cList(int)
template <class T>
inline cList<T>::cList(int nGrowBy) {
	assert(nGrowBy > 0);
	m_pList = NULL;
	m_nGrowBy = nGrowBy;
	Free();
}

// cList<T>::cList(const cList<T> &)
template <class T>
inline cList<T>::cList(const cList<T> &src) {
	m_pList = NULL;
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

// cList<T>::operator =
template <class T>
inline cList<T> & cList<T>::operator = (const cList<T> &src) {
	Free();

	m_nUsed = src.m_nUsed;
	m_nAllocated = src.m_nAllocated;
	m_nGrowBy = src.m_nGrowBy;

	if(m_nAllocated) {
		m_pList = new T[m_nAllocated];
		for(int i = 0; i < m_nUsed; i++) m_pList[i] = src.m_pList[i];
	}
	return *this;
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
}

// cList<T>::GetAt
template <class T>
inline T & cList<T>::GetAt(int index) {
	assert(index >= 0);
	assert(index < m_nUsed);
	if(index >= 0 && index < m_nUsed) return m_pList[index];
	return m_pList[0];
}

// cList<T>::SetAt
template <class T>
inline void cList<T>::SetAt(int index, const T &value) {
	assert(index >= 0);
	assert(index < m_nUsed);
	if(index >= 0 && index < m_nUsed) m_pList[index] = value;
}

// cList<T>::Ptr() const
template <class T>
inline const T * cList<T>::Ptr() const {
	return m_pList;
}

// cList<T>::Ptr
template <class T>
inline T * cList<T>::Ptr() {
	return m_pList;
}

// cList<T>::Free
template <class T>
inline void cList<T>::Free() {
	if(m_pList) delete[] m_pList;
	m_pList = NULL;
	m_nUsed = m_nAllocated = 0;
}

// cList<T>::Count
template <class T>
inline int cList<T>::Count() const {
	return m_nUsed;
}

// cList<T>::Add
template <class T>
inline int cList<T>::Add(const T &value) {
	Check(m_nUsed + 1);
	m_pList[m_nUsed] = value;
	m_nUsed++;
	return m_nUsed - 1;
}

// cList<T>::Append
template <class T>
inline int cList<T>::Append(const cList<T> &src) {
	Check(m_nUsed + src.m_nUsed);
	for(int i = 0; i < src.m_nUsed; i++) {
		m_pList[m_nUsed] = src.m_pList[i];
		m_nUsed++;
	}
	return m_nUsed - 1;
}

// cList<T>::IndexOf
template <class T>
inline int cList<T>::IndexOf(const T &value) const {
	for(int i = 0; i < m_nUsed; i++)
		if(value == m_pList[i]) return i;
	return -1;
}

// cList<T>::Contains
template <class T>
inline bool cList<T>::Contains(const T &value) const {
	for(int i = 0; i < m_nUsed; i++)
		if(value == m_pList[i]) return true;
	return false;
}

// cList<T>RemoveAt
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

} // comms

#endif // __cList__