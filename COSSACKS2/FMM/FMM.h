#ifndef FMMHEADER_H
#define FMMHEADER_H

#include "FMManager.h"
//-----------------------------------------------------------------------------------
extern FMManager MManager;
//-----------------------------------------------------------------------------------

inline void * __cdecl operator new(unsigned int size)
{
	return (void*)MManager.Allocate(size);
}
inline void * __cdecl operator new[](unsigned int size)
{
	return (void*)MManager.Allocate(size);
}
//-----------------------------------------------------------------------------------
inline void __cdecl operator delete(void *p)
{
	MManager.Deallocate((FMPTR)p);
}
inline void __cdecl operator delete[](void *p)
{
	MManager.Deallocate((FMPTR)p);
}
//-----------------------------------------------------------------------------------

#endif