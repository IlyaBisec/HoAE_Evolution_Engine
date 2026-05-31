/*****************************************************************************/
/*    File:    kMemorySpy.h
/*    Desc:    Gathers statistics about memory blocks allocations/deletions
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11.03.2004
/*****************************************************************************/
#ifndef __KMEMORYSPY_H__
#define __KMEMORYSPY_H__
void msAllocBlock( void* pData, int size, const char* name );
void msFreeBlock( void* pData );
#endif // __KMEMORYSPY_H__