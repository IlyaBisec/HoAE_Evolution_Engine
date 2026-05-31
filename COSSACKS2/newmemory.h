
#ifndef __NEWMEMORY_H__
#define __NEWMEMORY_H__
#define free _ExFree
#define malloc _ExMalloc
#define realloc _ExRealloc
void* _ExMalloc(int Size);
void _ExFree(void* Ptr);
void* _ExRealloc(void* ptr,int Size);
inline void* _cdecl operator new(size_t size)
{
 return malloc(size);
};
inline void* _cdecl operator new[](size_t size)
{
	return malloc(size);
};

inline void __cdecl operator delete(void *ptr)
{
 free(ptr);
};
inline void __cdecl operator delete[](void *ptr)
{
 free(ptr);
};
#define znew(t,s) (t*)_ExMalloc((s)*sizeof(t))
#define renew(p,t,s) (t*)_ExRealloc(p,(s)*sizeof(t))
#endif //__NEWMEMORY_H__
