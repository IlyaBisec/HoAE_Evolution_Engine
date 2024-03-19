#ifndef FEXHELPERS_H
#define FEXHELPERS_H

int FEX_GetCallStackEBP(unsigned* CallStack, int MaxDepth);
bool FEX_GetDebugInfo(unsigned Base, unsigned Address, unsigned* pLineNum, char* ppFunc, char* ppSrc);

#endif