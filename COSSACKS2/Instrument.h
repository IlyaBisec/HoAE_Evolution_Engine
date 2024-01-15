//#define PERFORMINSTRUMENTATION


#define PBUFSIZE 16*50000
extern DWORD ProfBuf[PBUFSIZE];
extern int ProfBufPos;
#define __TIMESTAMP(){\
	const char* s=__FUNCTION__;\
	const int L=__LINE__;\
	__asm{rdtsc										};\
	__asm{mov   ecx,[ProfBufPos]					};\
	__asm{mov   [ProfBuf  +ecx],eax					};\
	__asm{mov   [ProfBuf+4+ecx],edx					};\
	__asm{mov   eax,[s]								};\
	__asm{mov   [ProfBuf+8+ecx],eax					};\
	__asm{mov   edx,L								};\
	__asm{mov   [ProfBuf+12+ecx],edx				};\
	__asm{add   ecx,16								};\
	__asm{mov   [ProfBufPos],ecx					};}	

#define __TIMESTAMP2(x){\
	const char* s=#x;\
	__asm{rdtsc										};\
	__asm{mov   ecx,[ProfBufPos]					};\
	__asm{mov   [ProfBuf  +ecx],eax					};\
	__asm{mov   [ProfBuf+4+ecx],edx					};\
	__asm{mov   eax,[s]								};\
	__asm{mov   [ProfBuf+8+ecx],eax					};\
	__asm{mov   [ProfBuf+12+ecx],0					};\
	__asm{add   ecx,16								};\
	__asm{mov   [ProfBufPos],ecx					};}

#define __ENDSTAMP()\
	__asm{rdtsc										};\
	__asm{mov   ecx,[ProfBufPos]					};\
	__asm{or    edx,0x80000000						};\
	__asm{mov   [ProfBuf  +ecx],eax					};\
	__asm{mov   [ProfBuf+4+ecx],edx					};\
	__asm{add   ecx,8								};\
	__asm{mov   [ProfBufPos],ecx					};	
#ifdef PERFORMINSTRUMENTATION

#define PRFUNC(x)  __TIMESTAMP2(x);x;__ENDSTAMP();
#define PROFLINE   __TIMESTAMP();
#define PROF       __TIMESTAMP();
#define ENDPROF    __ENDSTAMP();
#define NEXTPROF   ENDPROF;PROFLINE;

#else //PERFORMINSTRUMENTATION

#define PRFUNC(x) x;
#define PROFLINE
#define PROF
#define ENDPROF
#define NEXTPROF

#endif //PERFORMINSTRUMENTATION