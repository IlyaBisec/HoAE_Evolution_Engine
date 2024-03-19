/*****************************************************************************/
/*	File:	kHash.hpp
/*	Desc:	Hash tables
/*	Author:	Ruslan Shestopalyuk
/*	Date:	19.12.2002
/*****************************************************************************/

#ifndef __KHASH_HPP__
#define __KHASH_HPP__

const int c_DefTableSize	= 17389; // 2000s prime number
const int c_DefMinPoolSize	= 16384;

#define NO_ELEMENT		0xFFFFFFFF 
/*****************************************************************************/
/*	Class:	Hash
/*	Desc:	pooled hash for generic elements, with self-adjustable pool
/*				size
/*	Remark:	EType should have functions
/*				unsigned int	hash	() const;
/*				bool			equal	( const EType& el );
/*				void			copy	( const EType& el );
/*			defined. 
/*****************************************************************************/
template	<	
			class	EType, 
			int		TableSize	= c_DefTableSize, 
			int		MinPoolSize = c_DefMinPoolSize
			>
class Hash
{
	//  internal class Entry
	class Entry : public EType
	{
	public:
		Entry() : next((Entry*)0xFFFFFFFF), EType() {}
		bool		isFree() const			{ return (next == (Entry*)0xFFFFFFFF);	}
		void		setFree()				{ next = (Entry*)0xFFFFFFFF;		    }
		Entry*		next;
	}; // class Entry

	Entry*			m_Pool;				//  pool for elements
	int				m_PoolSize;			//  current pool size
	int				m_NEntries;		    //  current used number of pool entries
	Entry*			m_Table[TableSize];	//  hash table
public:
	//  ctor
	Hash() 
	{
		m_Pool		= new Entry[MinPoolSize];
		m_PoolSize	= MinPoolSize;
		m_NEntries  = 0;
		memset( m_Table, 0, TableSize * sizeof( Entry* ) );
	}
    Hash( const Hash& h ) 
    {
        m_PoolSize	= h.m_PoolSize;
        m_Pool		= new Entry[m_PoolSize];
        m_NEntries  = h.m_NEntries;
        memcpy( m_Table, h.m_Table, TableSize * sizeof( Entry* ) );
        if (m_NEntries > 0) memcpy( m_Pool, h.m_Pool, m_NEntries * sizeof( Entry ) );
    }


	//  dtor
	virtual ~Hash()
	{
		delete []m_Pool;
        m_Pool     = NULL;
        m_PoolSize = 0;
	}
	int		numElem() const { return m_NEntries; }
	EType&	elem( int idx ) { assert( idx >= 0 && idx < m_NEntries ); return m_Pool[idx]; }
	//  reset all nafiq
	void	reset()
			{
				m_NEntries= 0;
				memset( m_Table, 0, TableSize * sizeof( Entry* ) );	
                for (int i = 0; i < m_PoolSize; i++) m_Pool[i].setFree();
			}
	//  adds element to hash m_Table, returns its pool index, if exists
	int		add		( const EType& el )
			{
				//  get hash index
				unsigned int h = el.hash() % TableSize;
				Entry* entry = m_Table[h];
				//  null pointer in the hash m_Table => allocate
				if (!entry)
				{
					Entry& added	= AllocEntry();
					added.next		= 0;
					m_Table[h]		= &added;
					added.copy( el );
					return (&added) - m_Pool;	
				}
				if (entry->isFree())
				{
					entry->copy( el );
					entry->next = 0;
					return entry - m_Pool;
				}
				int idx = LocateEntry( *entry, el );
				//  check if we have already that element
				if (idx != 0xFFFFFFFF) return idx;
				//  no element - add it to the head of linked list
				Entry& added	= AllocEntry();
				added.next		= m_Table[h];
				m_Table[h]		= &added;
				added.copy( el );
				return (&added) - m_Pool;
			}
	int		add		( const EType& el, int idx )
			{
				//  get hash index
				unsigned int h = el.hash() % TableSize;
				Entry* entry = m_Table[h];
				//  null pointer in the hash m_Table => allocate
				if (!entry)
				{
					Entry& added	= m_Pool[idx];
					added.next		= 0;
					m_Table[h]		= &added;
					added.copy( el );
					return (&added) - m_Pool;	
				}
				if (entry->isFree())
				{
					entry->copy( el );
					entry->next = 0;
					return entry - m_Pool;
				}
				//  no element - add it to the head of linked list
				Entry& added	= m_Pool[idx];
				added.next		= m_Table[h];
				m_Table[h]		= &added;
				added.copy( el );
				return (&added) - m_Pool;
			}

    //  removes element from the hash table
    void delElem( int idx )
            {
                Entry& entry = m_Pool[idx];
                if (entry.isFree()) return;
                unsigned int h = entry.key.hash() % TableSize;
                if (&entry == m_Table[h]) m_Table[h] = entry.next;
                if (entry.prev) entry.prev->next = entry.next;
                if (entry.next) entry.next->prev = entry.prev;
                entry.setFree();
                freeEntry = idx;
                m_NEntries--;
                if (idx == lastEntry) lastEntry--;
            }

	int		find	( const EType& el )
			{
				//  get hash index
				unsigned int h = el.hash() % TableSize;
				if (m_Table[h] == 0 ||
					m_Table[h]->isFree()) return NO_ELEMENT;
				//  ok, one or bunch of entries with same hash value
				return LocateEntry( *(m_Table[h]), el );
			}
	void	dump	( FILE* fp )
			{
				fprintf( fp, "Dumping hash m_Table: %X\n", this );
				for (int i = 0; i < m_NEntries; i++)
				{
					m_Pool[i].dump( fp );
				}
			}
protected:
	//  enlarges pool with given number of entries
	void GrowPool( int nToGrow )
	{
		m_PoolSize += nToGrow;
		Entry* npool = new Entry[m_PoolSize];
		memcpy( npool, m_Pool, m_NEntries * sizeof( Entry ) );
		//  redirect table references
		for (int i = 0; i < TableSize; i++)
		{
			if (m_Table[i]) m_Table[i] = npool + (m_Table[i] - m_Pool);
		}
		for (int i = 0; i < m_NEntries; i++)
		{
			if (npool[i].next)
			{
				npool[i].next = npool + (npool[i].next - m_Pool);
			}
		}
		delete []m_Pool;
		m_Pool = npool;
	} // GrowPool
	
    //  allocate hash entry from the pool
	Entry& AllocEntry()
	{
		if (m_NEntries == m_PoolSize)
		//  no more entries, grow pool
		{
			GrowPool( m_PoolSize / 2 );
		}
		return m_Pool[m_NEntries++];
	}

	//  finds entry for element in the linked list
	//  returns 0xFFFFFFFF if not found
	int	LocateEntry( const Entry& first, const EType& el )
	{
		Entry* cur = const_cast<Entry*>( &first );
		if (cur->isFree()) return 0xFFFFFFFF;
		do
		{
			if (cur->equal( el )) return cur - m_Pool;
			cur = cur->next;
		}
		while (cur && !(cur->isFree()));
		return 0xFFFFFFFF;
	}
}; // class Hash

/*****************************************************************************/
/*	Class:	StaticHash
/*	Desc:	pooled hash for generic elements, with static pool size
/*****************************************************************************/
template	<	
			class	EType, 
			class	KType,
			int		tableSize	= c_DefTableSize, 
			int		maxEntries	= c_DefMinPoolSize
			>
class StaticHash
{
protected:
	//  internal class Entry
	class Entry
	{
	public:
		EType		val;
		KType		key;
	}; // class Entry

    static const DWORD  c_FreeEntry  = 0xFFFFFFFF;
    static const DWORD  c_ChainEnd   = 0xBAADF00D;

	Entry			pool[maxEntries];	//  pool for elements
    DWORD           next[maxEntries];	//  pointer to next entry

    int				nEntriesUsed;		//  current used number of pool entries
	int				freeEntry;
	int				lastEntry;
	
	Entry*			table[tableSize];	//  hash table
public:
	
	typedef StaticHash<EType, KType, tableSize, maxEntries> HashType;
	//  used for traversing object list
	class iterator
	{
		HashType&		hash;
		int				pos;

		void rewind()
		{
			while (hash.next[pos] == c_FreeEntry && pos <= hash.lastEntry) pos++;
		}

	public:
		iterator( HashType& h ) : hash( h ), pos(0) { rewind(); }
		void operator++()
		{
			pos++;
			rewind();
		}

		operator bool()
		{
			return pos <= hash.lastEntry;
		}

		const EType& operator*() const
		{	
			return hash.elem( pos );
		}
		
		void reset()
		{
			pos = 0;
			rewind();
		}

		EType& operator*()
		{	
			return hash.elem( pos );
		}
	}; // class iterator

    class const_iterator
    {
        const HashType&		hash;
        int				    pos;

        void rewind()
        {
            while (hash.next[pos] == c_FreeEntry && pos <= hash.lastEntry) pos++;
        }

    public:
        const_iterator( const HashType& h ) : hash( h ), pos(0) { rewind(); }
        void operator++()
        {
            pos++;
            rewind();
        }

        operator bool()
        {
            return pos <= hash.lastEntry;
        }

        const EType& operator*() const
        {	
            return hash.elem( pos );
        }

        void reset()
        {
            pos = 0;
            rewind();
        }

        const EType& operator*()
        {	
            return hash.elem( pos );
        }
    }; // class const_iterator

                    StaticHash  () { reset(); } 
	iterator        begin       () { return iterator( *this ); }
    const_iterator  begin       () const { return const_iterator( *this ); }

	int				numElem     () const { return nEntriesUsed; }
	int				maxElem     () const { return lastEntry + 1; }
    int             poolSize    () const { return maxEntries; }
	EType&			elem        ( int idx ) 
    { 
        assert( idx >= 0 && idx <= lastEntry ); 
        return pool[idx].val; 
    }

    const EType&	operator[]  ( int idx ) const 
    {  
        assert( idx >= 0 && idx <= lastEntry ); 
        return pool[idx].val; 
    }

    EType&	operator[]  ( int idx ) 
    {  
        assert( idx >= 0 && idx <= lastEntry ); 
        return pool[idx].val; 
    }

	const EType&	elem        ( int idx ) const 
    { 
        assert( idx >= 0 && idx <= lastEntry ); 
        return pool[idx].val; 
    }
    
	int		nextInChain( int idx )
	{
		assert( idx >= 0 && idx <= lastEntry );
		Entry& entry = pool[idx];
        return next[idx] == c_ChainEnd ? NO_ELEMENT : next[idx];
	}
	//  reset all nafiq
	void	reset()
	{
		nEntriesUsed	= 0;
		freeEntry		= 0;
		lastEntry		= -1;
		memset( table,  0,      tableSize * sizeof( Entry* )    );
        memset( next,   0xFF,   maxEntries * sizeof( DWORD )    );
	}
	//  adds element to hash table, returns its pool index, if exists
	int		add		( const KType& key, const EType& el )
	{
		//  get hash index
		unsigned int h = key.hash() % tableSize;
		Entry* entry = table[h];
		//  null pointer in the hash table => allocate
		if (!entry)
		{
			Entry* added	    = allocEntry();
            int idx             = index( added );          
			next[idx]	        = c_ChainEnd;
			table[h]		    = added;
			added->val		    = el;
			added->key		    = key; 
			nEntriesUsed++;
			return idx;	
		}
		int idx = locateEntry( *entry, key );
		//  check if we have already that element
		if (idx != c_FreeEntry) return idx;
		//  no element - add it to the head of linked list
		Entry* added	= allocEntry();
        idx             = index( added );
		next[idx]		= index( table[h] );
		table[h]		= added;
		added->val		= el;
		added->key		= key; 
		nEntriesUsed++;
		return index( added );
	}

    //  constructs hash table entry or returns its index, if already exists
    int		add		( const KType& key )
    {
        //  get hash index
        unsigned int h = key.hash() % tableSize;
        Entry* entry = table[h];
        //  null pointer in the hash table => allocate
        if (!entry)
        {
            Entry* added	    = allocEntry();
            int idx             = index( added );          
            next[idx]	        = c_ChainEnd;
            table[h]		    = added;
            added->key		    = key; 
            nEntriesUsed++;
            return idx;	
        }
        int idx = locateEntry( *entry, key );
        //  check if we have already that element
        if (idx != c_FreeEntry) return idx;
        //  no element - add it to the head of linked list
        Entry* added	= allocEntry();
        idx             = index( added );
        next[idx]		= index( table[h] );
        table[h]		= added;
        added->key		= key; 
        nEntriesUsed++;
        return index( added );
    }
	//  removes element from the hash table
	void delElem( int idx )
	{
		Entry& entry = pool[idx];
		if (next[idx] == c_FreeEntry) return;
		unsigned int h = entry.key.hash() % tableSize;
        if (table[h] == NULL) 
        { 
            next[idx]  = c_FreeEntry;
            return;
        }
        if (&entry == table[h]) 
        {
            table[h] = (next[idx] == c_ChainEnd) ? NULL : &pool[next[idx]];
        }
        else
        {
            int bIdx = index( table[h] );
		    while ( next[bIdx] != idx && 
                    next[bIdx] != c_ChainEnd &&
                    next[bIdx] != c_FreeEntry) bIdx = next[bIdx];
		    next[bIdx] = next[idx];
        }
        next[idx]  = c_FreeEntry;
		freeEntry  = idx;
		nEntriesUsed--;
		if (idx == lastEntry) lastEntry--;
	}

    int		find	( const KType& key ) const
    {
        //  get hash index
        unsigned int h = key.hash() % tableSize;
        if (table[h] == 0) return NO_ELEMENT;
        //  ok, one or bunch of entries with same hash value
        return locateEntry( *(table[h]), key );
    }
	void	dump	( FILE* fp )
	{
		fprintf( fp, "Dumping hash table: %X\n", this );
		for (int i = 0; i < nEntriesUsed; i++)
		{
			pool[i].dump( fp );
		}
	}
protected:

    int     index( const Entry* e ) const { return e - pool; }
	//  allocate hash entry from the pool
	Entry*	allocEntry()
	{
		assert( nEntriesUsed < maxEntries );
		while (next[freeEntry] != c_FreeEntry && freeEntry < maxEntries) freeEntry++;
		if (freeEntry == maxEntries)
		{
			freeEntry = 0;
			while (next[freeEntry] != c_FreeEntry) freeEntry++;
		}
		if (lastEntry < freeEntry) lastEntry = freeEntry;
		return &pool[freeEntry];
	}
	//  finds entry for element in the linked list
	//  returns NO_ELEMENT if not found
	int		locateEntry( const Entry& first, const KType& key ) const
	{
        int idx = index( &first );
        if (next[idx] == c_FreeEntry) return NO_ELEMENT;
        int nIter = 0;
		do{
            const Entry* cur = &pool[idx];
            if (cur->key == key) return idx;
            idx = next[idx];
            nIter++;
		}while (idx != c_ChainEnd && nIter < maxEntries);
		return NO_ELEMENT;
	}
}; // class StaticHash

/*****************************************************************************/
/*	Class:	PointerHash
/*	Desc:	hash for generic elements' pointers, with self-adjustable pool size
/*	Remark:	KType should have functions
/*				unsigned int	hash	() const;
/*				bool			equal	( const EType& el );
/*				void			copy	( const EType& el );
/*			defined. 
/*****************************************************************************/
template	<	
			class	EType, 
			class	KType,
			int		tableSize	= c_DefTableSize, 
			int		minPoolSize = c_DefMinPoolSize
			>
class PointerHash
{
	//  internal class Entry
	class Entry 
	{
	public:
		Entry() : next((Entry*)0xFFFFFFFF) {}
		bool		isFree() const			{ return (next == (Entry*)0xFFFFFFFF);	}
		void		setFree()				{ next = 0xFFFFFFFF;					}
		
		EType*		el;
		KType		key;

		Entry*		next;
	}; // class Entry

	Entry*			pool;				//  pool for elements
	int				poolSize;			//  current pool size
	int				nEntriesUsed;		//  current used number of pool entries
	Entry*			table[tableSize];	//  hash table
public:
	//  ctor
			PointerHash() 
			{
				pool		= new Entry[minPoolSize];
				poolSize	= minPoolSize;
				nEntriesUsed= 0;
				memset( table, 0, tableSize * sizeof( Entry* ) );
			}
	//  dtor
			virtual ~PointerHash()
			{
				delete []pool;
			}
	int		numElem() const { return nEntriesUsed; }
	EType*	elem( int idx ) 
	{ 
		assert( idx >= 0 && idx < nEntriesUsed ); 
		return pool[idx].el; 
	}
	Entry& entry( int idx ) 
	{ 
		assert( idx >= 0 && idx < nEntriesUsed ); 
		return pool[idx]; 
	}
	EType*	elem( int idx ) const 
		{ 
			assert( idx >= 0 && idx < nEntriesUsed ); 
			return pool[idx].el; 
		}
	
	//  reset all nafiq
	void	reset()
			{
				nEntriesUsed= 0;
				memset( table, 0, tableSize * sizeof( Entry* ) );	
			}
	//  adds element to hash table, returns its pool index, if exists
	int		add		( const KType& key, EType* el )
			{
				//  get hash index
				unsigned int h = key.hash() % tableSize;
				Entry* entry = table[h];
				//  null pointer in the hash table => allocate
				if (!entry)
				{
					Entry& added	= allocEntry();
					added.next		= 0;
					table[h]		= &added;
					added.key		= key;
					added.el		= el;
					return (&added) - pool;	
				}
				if (entry->isFree())
				{
					entry->el		= el;
					entry->next		= 0;
					entry->key		= key;
					return entry - pool;
				}
				int idx = locateEntry( *entry, key );
				//  check if we have already that element
				if (idx != 0xFFFFFFFF) return idx;
				//  no element - add it to the head of linked list
				Entry& added	= allocEntry();
				added.next		= table[h];
				table[h]		= &added;
				added.el		= el;
				added.key		= key;
				return (&added) - pool;
			}
	int		find	( const KType& key )
			{
				//  get hash index
				unsigned int h = key.hash() % tableSize;
				if (table[h] == 0 ||
					table[h]->isFree()) return NO_ELEMENT;
				//  ok, one or bunch of entries with same hash value
				return locateEntry( *(table[h]), key );
			}
	void	dump	( FILE* fp )
			{
				fprintf( fp, "Dumping hash table: %X\n", this );
				for (int i = 0; i < nEntriesUsed; i++)
				{
					pool[i].dump( fp );
				}
			}
protected:
	//  enlarges pool with given number of entries
	void	growPool( int numEntriesToGrow )
			{
				poolSize += numEntriesToGrow;
				Entry* npool = new Entry[poolSize];
				memcpy( npool, pool, nEntriesUsed * sizeof( Entry ) );
				//  redirect table references
				for (int i = 0; i < tableSize; i++)
				{
					if (table[i]) table[i] = npool + (table[i] - pool);
				}
				for (int i = 0; i < nEntriesUsed; i++)
				{
					if (npool[i].next)
					{
						npool[i].next = npool + (npool[i].next - pool);
					}
				}
				delete []pool;
				pool = npool;
			}
	//  allocate hash entry from the pool
	Entry&	allocEntry()
			{
				if (nEntriesUsed == poolSize)
				//  no more entries, grow pool
				{
					growPool( poolSize / 2 );
				}
				return pool[nEntriesUsed++];
			}
	//  finds entry for element in the linked list
	//  returns 0xFFFFFFFF if not found
	int		locateEntry( const Entry& first, const KType& key )
			{
				Entry* cur = const_cast<Entry*>( &first );
				if (cur->isFree()) return 0xFFFFFFFF;
				do
				{
					if (cur->key == key) return cur - pool;
					cur = cur->next;
				}
				while (cur && !(cur->isFree()));
				return 0xFFFFFFFF;
			}
}; // class PointerHash

/*****************************************************************************/
/*	Class:	Hache
/*	Desc:	Hash table with cached elements allocated from the pool
/*****************************************************************************/
template <	class	EType,
            int		tableSize, 
            int		minPoolSize
         >
class Hache
{
	//  internal class Entry
	class Entry : public EType
	{
	public:
		Entry() : next(0), prev(0), factor(0), EType() {}

		//  entrys with the same hash values are stored in the double-linked
		//  list chains
		Entry*			next;
		Entry*			prev;
		unsigned int	factor;
	}; // class Entry

	Entry*			pool;				//  pool for elements
	int				poolSize;			//  current pool size
	int				nEntriesUsed;		//  current used number of pool entries
	Entry*			table[tableSize];	//  hash table

	typedef typename EType::Attr AttrType; 
public:
	Hache() 
	{
		pool		= new Entry[minPoolSize];
		poolSize	= minPoolSize;
		nEntriesUsed= 0;
		memset( table, 0, tableSize * sizeof( Entry* ) );
	}

	virtual ~Hache()
	{
		delete []pool;
	}

	void		reset()
	{
		nEntriesUsed = 0;
		memset( table, 0, tableSize * sizeof( Entry* ) );	
	}

	EType*		getEl	( const AttrType& attr )
	{
		unsigned int h = hashVal % tableSize;
		if (table[h] == 0) return 0;
		//  ok, one or bunch of entries with same hash value
		return locateEntry( *(table[h]), attr );
	}

	EType*		getHitAllocEl( const AttrType& attr, unsigned int factor )
	{
		unsigned int h = attr.hash() % tableSize;
		Entry* entry;
		if (table[h] == 0) 
		{
			entry		= allocEntry();
			entry->next = 0;
			entry->prev = 0;
			table[h]	= entry;
		}
		else
		{
			entry = locateEntry( *(table[h]), attr );
			if (!entry) 
			{
				entry			= allocEntry(); 
				entry->next		= table[h];
				entry->prev		= 0;
				if (table[h]) table[h]->prev = entry;
				table[h]		= entry;
			}
		}

		entry->factor = factor;
		return entry;
	}

	int			numElem() const { return nEntriesUsed;	}
	int			maxElem() const { return poolSize;		}

	EType*		elem( int idx ) 
	{ 
		assert( idx >= 0 && idx < poolSize ); 
		return &(pool[idx]); 
	}

	int getSizeBytes() const
	{
		int sz = 0;
		for (int i = 0; i < poolSize; i++)
		{
			sz += pool[i].getSizeBytes();
			sz += sizeof( Entry ) - sizeof( EType );
		}
		return sz + sizeof( this ) 
			+ tableSize * sizeof( Entry* );
	}

protected:
	Entry*		allocEntry()
	{
		if (nEntriesUsed == poolSize)
			//  no more entries, need to dismiss one 
		{
			unsigned int tmin = pool[0].factor;
			int dismIdx = 0;
			for (int i = 1; i < poolSize; i++)
			{
				if (pool[i].factor < tmin) {dismIdx = i; tmin = pool[i].factor;}
			}
			//  pull dismissed item from chain
			Entry* entry = &(pool[dismIdx]);

			if (entry->prev)
			{
				entry->prev->next = entry->next;
			}
			else
				//  dismissed entry is at the chain root 
				//  and is pointed from hash table
			{
				unsigned int h = entry->hash() % tableSize;
				assert( entry == table[h] );
				table[h] = entry->next;
			}

			if (entry->next)
			{
				entry->next->prev = entry->prev;
			}

			return entry;
		}
		return &(pool[nEntriesUsed++]);
	}

	Entry* locateEntry( const Entry& first, const AttrType& attr )
	{
		Entry* cur = const_cast<Entry*>( &first );
		do
		{
			if (cur->hasAttr( attr )) return cur;
			cur = cur->next;
		}
		while (cur);
		return 0;
	}
}; // class Hache

//  some default hash functions
inline unsigned int hash0( BYTE* s, int nBytes )
{
	unsigned int h = 0;
	for (int i = 0; i < nBytes; i++)
	{
		h = (h * 729 + (*s) * 37) ^ (h >> 1);
		s++;
	}
	return h;
} // hash

inline unsigned int hash_csz( const char* s )
{
	unsigned int h = 0;
	while (*s)
	{
		h = (h * 729 + (*s) * 37) ^ (h >> 1);
		s++;
	}
	return h;
} // hash

#endif // __KHASH_HPP__
