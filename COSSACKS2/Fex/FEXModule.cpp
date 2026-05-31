//#include "stdafx.h"
#include <windows.h>
#include <tlhelp32.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "FEXModule.h"
#include "imagehlp.h"
//-----------------------------------------------------------------------------------
int FEX_EnumModules(void);
//-----------------------------------------------------------------------------------
//#define COPY_PATH "\\\\-GEC-\\_Work\\bugzz\\"
char FEX_VersionString[] =
#include "..\Version.txt"
;
#define IMMEDIATE_ASSERT
//"COSACKS2_ALPHA70";
//char FEX_VersionString[] = "COS2_ALPHA15";
char FEX_SystemVersionString[1024];
char* GetFEX_Log();
//-----------------------------------------------------------------------------------
#define Nstddll 
#define SUIN char*

char FEX_tmp[1024];			

char		FEX_RootFolder[_MAX_PATH+1];
char*		FEX_fnameLog = "Crash\\!!crash!!.log";
char*		FEX_fnameDump = "Crash\\!!crash!!.dmp";
FILE*		FEX_logfile = NULL;
FILE*		FEX_dumpfile = NULL;
unsigned FEX_end_esp = 0;
int		FEX_DXVersion = 0;

__int64	FEX_WinDiskAllSpace = 0;
__int64	FEX_WinDiskFreeSpace1 = 0;
__int64	FEX_WinDiskFreeSpace2 = 0;
__int64	FEX_CurDiskAllSpace = 0;
__int64	FEX_CurDiskFreeSpace1 = 0;
__int64	FEX_CurDiskFreeSpace2 = 0;
int		FEX_WinDrive = 0;
int		FEX_CurDrive = 0;
MEMORYSTATUS FEX_MemStatus1;
MEMORYSTATUS FEX_MemStatus2;
char*		FEX_VideoName = NULL;
char*		FEX_AudioName = NULL;

DWORD FEX_GetDXVersion();
bool FEX_GetFreeSpace(int DiskNum, __int64& i64TotalBytes, __int64& i64FreeBytes);
char* FEX_GetSystemVersion(void);
char* FEX_GetDisplayName(void);
char* FEX_GetAudioName(void);
DWORD FEX_LastError = 0;
//-----------------------------------------------------------------------------------
LPTOP_LEVEL_EXCEPTION_FILTER FEX_OrigFilter = NULL;
//-----------------------------------------------------------------------------------
#ifdef FEX_LOGGING_ENABLE
	#define FEX_LOG fprintf
	#define FEX_LOG_OPEN() \
		if((FEX_logfile = fopen(FEX_fnameLog, "wt")) == NULL)\
		{\
			MessageBox(NULL, FEX_fnameLog, "Cannot open log file", MB_OK);\
			ExitProcess(-1);\
		}
	#define FEX_LOG_CLOSE()	fclose(FEX_logfile);
#else
	#define FEX_LOG_CLOSE()
	#define FEX_LOG_OPEN()
	#define FEX_LOG //
#endif

#define FEX_DUMP_OPEN() {\
	if((FEX_dumpfile = fopen(FEX_fnameDump, "wb")) == NULL)\
	{\
		MessageBox(NULL, FEX_fnameDump, "Cannot open dump file", MB_OK);\
		ExitProcess(-1);\
	}\
	fputc('F', FEX_dumpfile);\
	fputc('E', FEX_dumpfile);\
	fputc('X', FEX_dumpfile);\
	fputc('D', FEX_dumpfile);\
	int type = 0, size = 0;\
	type = FEX_BLOCK_TYPE_VERSION;\
	fwrite(&type, 4, 1, FEX_dumpfile);\
	size = strlen(FEX_VersionString)+1;\
	fwrite(&size, 4, 1, FEX_dumpfile);\
	fwrite(FEX_VersionString, size, 1, FEX_dumpfile);\
	}

#define FEX_DUMP_CLOSE()\
		fclose(FEX_dumpfile);
//-----------------------------------------------------------------------------------
LONG __stdcall FEX_CrashHandlerExceptionFilter(EXCEPTION_POINTERS* pEx)
{
	LONG lRet = EXCEPTION_CONTINUE_SEARCH;

   __try
   {
		static void* ExAddr = pEx->ExceptionRecord->ExceptionAddress;
		static unsigned _dr0 = pEx->ContextRecord->Dr0;
		static unsigned _dr1 = pEx->ContextRecord->Dr1;
		static unsigned _dr2 = pEx->ContextRecord->Dr2;
		static unsigned _dr3 = pEx->ContextRecord->Dr3;
		static unsigned _dr6 = pEx->ContextRecord->Dr6;
		static unsigned _dr7 = pEx->ContextRecord->Dr7;
		static unsigned _eax = pEx->ContextRecord->Eax;
		static unsigned _ebx = pEx->ContextRecord->Ebx;
		static unsigned _ecx = pEx->ContextRecord->Ecx;
		static unsigned _edx = pEx->ContextRecord->Edx;
		static unsigned _esi = pEx->ContextRecord->Esi;
		static unsigned _edi = pEx->ContextRecord->Edi;
		static unsigned _esp = pEx->ContextRecord->Esp;
		static unsigned _ebp = pEx->ContextRecord->Ebp;
		static unsigned _eip = pEx->ContextRecord->Eip;
		static unsigned _gs  = pEx->ContextRecord->SegGs;
		static unsigned _fs  = pEx->ContextRecord->SegFs;
		static unsigned _es  = pEx->ContextRecord->SegEs;
		static unsigned _ds  = pEx->ContextRecord->SegDs;
		static unsigned _ss  = pEx->ContextRecord->SegSs;
		// - get system info ----------------------------------------------------------
		FEX_LastError = GetLastError();
		int FEX_errno = errno;
		
		FEX_GetFreeSpace(FEX_WinDrive, FEX_WinDiskAllSpace, FEX_WinDiskFreeSpace2);
		FEX_GetFreeSpace(FEX_CurDrive, FEX_CurDiskAllSpace, FEX_CurDiskFreeSpace2);
		
		GlobalMemoryStatus(&FEX_MemStatus2);

		FEX_LOG_OPEN();
		FEX_DUMP_OPEN();

		FEX_LOG(FEX_logfile, "%s\n",FEX_VersionString);

		FEX_LOG(FEX_logfile, "Memory status before run:\n\n");
		FEX_LOG(FEX_logfile, "Memory load: %d\n",FEX_MemStatus1.dwMemoryLoad);
		FEX_LOG(FEX_logfile, "TotalPhys: %d\n",FEX_MemStatus1.dwTotalPhys);
		FEX_LOG(FEX_logfile, "AvailPhys: %d\n",FEX_MemStatus1.dwAvailPhys);
		FEX_LOG(FEX_logfile, "TotalPageFile: %d\n",FEX_MemStatus1.dwTotalPageFile);
		FEX_LOG(FEX_logfile, "AvailPageFile: %d\n",FEX_MemStatus1.dwAvailPageFile);
		FEX_LOG(FEX_logfile, "TotalVirtual: %d\n",FEX_MemStatus1.dwTotalVirtual);
		FEX_LOG(FEX_logfile, "AvailVirtual: %d\n",FEX_MemStatus1.dwAvailVirtual);
		FEX_LOG(FEX_logfile, "\nMemory status after crash:\n\n");
		FEX_LOG(FEX_logfile, "Memory load: %d\n",FEX_MemStatus2.dwMemoryLoad);
		FEX_LOG(FEX_logfile, "TotalPhys: %d\n",FEX_MemStatus2.dwTotalPhys);
		FEX_LOG(FEX_logfile, "AvailPhys: %d\n",FEX_MemStatus2.dwAvailPhys);
		FEX_LOG(FEX_logfile, "TotalPageFile: %d\n",FEX_MemStatus2.dwTotalPageFile);
		FEX_LOG(FEX_logfile, "AvailPageFile: %d\n",FEX_MemStatus2.dwAvailPageFile);
		FEX_LOG(FEX_logfile, "TotalVirtual: %d\n",FEX_MemStatus2.dwTotalVirtual);
		FEX_LOG(FEX_logfile, "AvailVirtual: %d\n\n",FEX_MemStatus2.dwAvailVirtual);
		FEX_LOG(FEX_logfile, "AvailVirtual: %d\n\n",FEX_MemStatus2.dwAvailVirtual);

		int FMM_GetNHandlers();
		int FMM_GetAllocatedBlocks();

		FEX_LOG(FEX_logfile, "Internal memory manager info:\n");
		FEX_LOG(FEX_logfile, "Handles: %d\n",FMM_GetNHandlers());
		FEX_LOG(FEX_logfile, "Blocks: %d\n\n",FMM_GetAllocatedBlocks());

		// - additional version info --------------------------------------------------
		WIN32_FIND_DATA WF2;
		HANDLE H = FindFirstFile("Versions\\*.version", &WF2);
		if(H != INVALID_HANDLE_VALUE)
		{
			do {
				static char Name[256];
				sprintf(Name, "Versions\\%s", WF2.cFileName);
				FILE* f = fopen(Name, "rb");
				if(f) 
				{
					static char buf[256];
					memset(buf, 0, 256);
					int size = fread(buf, 1, 255, f);
					if(size)
					{
						int type;
						type = FEX_BLOCK_TYPE_VERSION;
						fwrite(&type, 4, 1, FEX_dumpfile);
						size = strlen(buf)+1;
						fwrite(&size, 4, 1, FEX_dumpfile);
						fwrite(buf,size, 1, FEX_dumpfile);
					}
					fclose(f);
				}
			} while(FindNextFile(H, &WF2));
			FindClose(H);
		}
		
		//- enumerate loaded modules --------------------------------------------------
		{
			// - сохраняем список модулей задачи
			int type = 0, size = 0, modules = 0, addr = 0;
			type = FEX_BLOCK_TYPE_MODULESDUMP;

			fwrite(&type, 4, 1, FEX_dumpfile);
			addr = ftell(FEX_dumpfile);
			fwrite(&size, 4, 1, FEX_dumpfile);
			fwrite(&modules, 4, 1, FEX_dumpfile);

			char* sysdir = FEX_tmp;
			GetSystemDirectory(sysdir, _MAX_PATH+1);
			int len1 = strlen(sysdir);

			HANDLE hModuleSnap = NULL; 
			MODULEENTRY32 me32 = {0}; 
			me32.dwSize = sizeof(MODULEENTRY32); 
 
			hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
			if(hModuleSnap != INVALID_HANDLE_VALUE)
			{
				if(Module32First(hModuleSnap, &me32)) 
				{ 
					do { 
						fwrite(&me32.modBaseAddr, 4, 1, FEX_dumpfile);
						char* str1 = FEX_RootFolder;
						char* str2 = me32.szExePath;
						int len2 = strlen(str2);
						int len = len1 > len2 ? len2 : len1;

						// - если начало строки совпало с системным каталогом, 
						// то это - системная либа
						if(memicmp(sysdir, str2, len) == 0)
						{
							str2 = me32.szModule;
							len = strlen(str2)+2+4;
							fwrite(&len, 4, 1, FEX_dumpfile);
							fputc('*', FEX_dumpfile);
							len -= 5;
						}
						else
						{
							while(*str1 && *str2 && tolower(*str1) == tolower(*str2))
								str1++, str2++;
							len = strlen(str2)+1+4;
							fwrite(&len, 4, 1, FEX_dumpfile);
							len -= 4;
						}
						fwrite(str2, len, 1, FEX_dumpfile);
						fwrite(&me32.modBaseSize, 4, 1, FEX_dumpfile);
						modules++;
						FEX_LOG(FEX_logfile, "Module found %s   \t(%s) \tHandle: %08x Base address: %08x\n", me32.szExePath, str2, me32.hModule, me32.modBaseAddr);
					} while(Module32Next(hModuleSnap, &me32)); 
				} 
				CloseHandle(hModuleSnap); 
			} 
			long addr1 = ftell(FEX_dumpfile);
			fseek(FEX_dumpfile, addr, SEEK_SET);
			size = addr1 - addr - 4;
			fwrite(&size, 4, 1, FEX_dumpfile);
			fwrite(&modules, 4, 1, FEX_dumpfile);
			fseek(FEX_dumpfile, addr1, SEEK_SET);
		}
		//-----------------------------------------------------------------------------
		unsigned snum = FEX_end_esp - _esp;
		static unsigned buf[1024];
		static unsigned buf1[1024];
		static unsigned buf2[1024];
		int num = 0;

		// - берем указатель на вершину стека
		unsigned char* stack_pos = (unsigned char*)_esp;
		/*
		// Alexander insert
		bool CheckIfFileExists(char* Name);
		bool CrashStackToFile(const char* FileName,unsigned _ebp,unsigned _eip);
		if(!CheckIfFileExists("cossacks2.pdb"))
		{
			char CrashFilename[] = "crash.stack";
			if(CrashStackToFile(CrashFilename, _ebp, _eip))
			{
				sprintf(FEX_tmp, "sndbg.exe %s", CrashFilename);
				WinExec(FEX_tmp, SW_NORMAL);
			}
		}
		*/
		//
#ifdef IMMEDIATE_ASSERT
		bool CrashBox(char* Header,unsigned _ebp,unsigned _eip);
		CrashBox("CRASH!!!",_ebp,_eip);
#endif //IMMEDIATE_ASSERT
		extern bool EditMapMode;
		if(EditMapMode){
			void Save3DMap(char* Map);
			Save3DMap("Temp\\!map_autocrash.m3d");
			MessageBox(NULL,"Map saved!","Your last map was saved to 'Temp\\!map_autocrash.m3d'",0);
		}
			//-----------------------------------------------------------------------------
			// Dump it!
			//-----------------------------------------------------------------------------
		{
			int type = 0, size = 21*4;

			// - скидываем контекст задачи
			type = FEX_BLOCK_TYPE_CONTEXTDUMP;

			fwrite(&type, 4, 1, FEX_dumpfile);
			fwrite(&size, 4, 1, FEX_dumpfile);
			fwrite(&ExAddr, 4, 1, FEX_dumpfile);
			fwrite(&_eax, 4, 1, FEX_dumpfile);
			fwrite(&_ebx, 4, 1, FEX_dumpfile);
			fwrite(&_edx, 4, 1, FEX_dumpfile);
			fwrite(&_ecx, 4, 1, FEX_dumpfile);
			fwrite(&_esi, 4, 1, FEX_dumpfile);
			fwrite(&_edi, 4, 1, FEX_dumpfile);
			fwrite(&_esp, 4, 1, FEX_dumpfile);
			fwrite(&_ebp, 4, 1, FEX_dumpfile);
			fwrite(&_eip, 4, 1, FEX_dumpfile);
			fwrite(&_dr0, 4, 1, FEX_dumpfile);
			fwrite(&_dr1, 4, 1, FEX_dumpfile);
			fwrite(&_dr2, 4, 1, FEX_dumpfile);
			fwrite(&_dr3, 4, 1, FEX_dumpfile);
			fwrite(&_dr6, 4, 1, FEX_dumpfile);
			fwrite(&_dr7, 4, 1, FEX_dumpfile);
			fwrite(&_gs,  4, 1, FEX_dumpfile);
			fwrite(&_fs,  4, 1, FEX_dumpfile);
			fwrite(&_es,  4, 1, FEX_dumpfile);
			fwrite(&_ds,  4, 1, FEX_dumpfile);
			fwrite(&_ss,  4, 1, FEX_dumpfile);

			type = FEX_BLOCK_TYPE_STACKDUMP;
			
			fwrite(&type, 4, 1, FEX_dumpfile);
			size = snum;
			fwrite(&size, 4, 1, FEX_dumpfile);
			fwrite(stack_pos, size, 1, FEX_dumpfile);
		}
		//-----------------------------------------------------------------------------
		while(stack_pos <= (unsigned char*)FEX_end_esp-4)
		{
			// - берем из стека очередное двойное слово, это может быть адрес возврата
			unsigned char* ptr = (unsigned char*)(*(unsigned*)stack_pos);
			
			// - требуется определить, какая инструкция CALL находится ДО текущего адреса
			// [5 bytes] - E8 rel32 (CALL rel32 - относительный) 
			// -----------(FF /2 (CALL r/m32 - ближний косвенный))----------------------
			//	[2 bytes] - FF (D0,D1,D2,D3,D4,D5,D6,D7)
			//	[2 bytes] - FF (10,11,12,13,16,17)   
			//	[3 bytes] - FF (50,51,52,53,55,56,57) rel8
			//	[3 bytes] - FF 14 SIB   
			//	[4 bytes] - FF 54 SIB rel8  
			//	[6 bytes] - FF (90,91,92,93,95,96,97) rel32
			//	[6 bytes] - FF 15 rel32  
			//	[7 bytes] - FF 94 SIB rel32  

			// 00112233445566PP
			// 77665544332211PP
			//-----------------
			// FF94XXXXXXXXXX
			//   FF15XXXXXXXX
			//   FF9xXXXXXXXX
			//     E8-reloc32
			//       FF54XXXX
			//         FF14XX
			//         FF5xXX
			//           FFDx
			//-----------------

			struct CALL {
				unsigned char* dest_addr;		// - адрес вызова
				unsigned char* opcode_addr;	// - адрес инструкции CALL		
			} Calls[64];
			int calls = 0;			

			#define ADD_CALL(addr, pos) \
				{\
					Calls[calls].dest_addr = (unsigned char*)(addr);\
					Calls[calls].opcode_addr = &Opcode[pos];\
					calls++;\
				}
				
			unsigned char* Opcode = ptr-7;

			// - корректен ли адрес?
			if(!IsBadReadPtr(Opcode, 7))
			{
				if(Opcode[0] == 0xFF && Opcode[1] == 0x94) 
					ADD_CALL(0xFFFFFFFF, 0);
				
				if(Opcode[1] == 0xFF && 
				  ((Opcode[2] >= 0x90 && Opcode[2] <= 0x97 && Opcode[2] != 0x94) || 
				    Opcode[2] == 0x15))
					ADD_CALL(0xFFFFFFFF, 1);
				
				if(Opcode[2] == 0xE8)
				{
					unsigned* dest_addr = (unsigned*)((__int64)(*(int*)(Opcode+3)) + (unsigned)(unsigned*)ptr);
					if(!IsBadReadPtr(dest_addr, 4))
						ADD_CALL(dest_addr, 2);
				}
				
				if(Opcode[3] == 0xFF && Opcode[4] == 0x54)
					ADD_CALL(0xFFFFFFFF, 3);
				
				if(Opcode[4] == 0xFF && 
				  ((Opcode[5] >= 0x50 && Opcode[5] <= 0x57 && Opcode[5] != 0x54) ||
				    Opcode[5] == 0x14))
					ADD_CALL(0xFFFFFFFF, 4);

				if(Opcode[5] == 0xFF && Opcode[6] >= 0xD0 && Opcode[6] <= 0xD7)
					ADD_CALL(0xFFFFFFFF, 5);
			}

			if(calls)
				FEX_LOG(FEX_logfile, "\nCalls: %u before address: 0x%X (%X%X%X%X%X%X%X)\n", calls, ptr, Opcode[0], Opcode[1], Opcode[2], Opcode[3], Opcode[4], Opcode[5], Opcode[6], Opcode[7]);

			// - теперь мы имеем набор предположительных адресов вызова
			for(int i = 0; i < calls; i++)
			{
				// - получаем адрес функции, на которую ссылается CALL
				unsigned char* dest_addr = Calls[i].dest_addr;
				// - корректен ли адрес?
				if(!IsBadReadPtr(dest_addr, 1))
				{
					// - может оказаться, что вызов идет на JMP (E9XXXXXXXX), в цикле 
					// проходим по цепочке гипотетически возможных переадресаций
					for(int t = 0; t < 64; t++)
					{
						if(dest_addr[0] != 0xE9) break;
						dest_addr = (unsigned char*)(dest_addr + 5 + *(unsigned*)(dest_addr+1));
					}
				}
				FEX_LOG(FEX_logfile, "Call (0x%2X) detected from: 0x%08X to: 0x%08X!\n", Calls[i].opcode_addr[0], Calls[i].opcode_addr, dest_addr);
				// - адрес инструкции
				buf[num] = (unsigned)Calls[i].opcode_addr;
				// - адрес вызываемой функции
				buf1[num] = (unsigned)dest_addr;
				// - адрес в стеке адреса возврата
				buf2[num] = (unsigned)stack_pos;
				num++;
			}
			stack_pos++;
		}

		FEX_LOG(FEX_logfile, 
						 "\nAddress: %08X\n" 
						 "EAX = %08X EBX = %08X\n"
						 "ECX = %08X EDX = %08X\n"
						 "ESI = %08X EDI = %08X\n"
						 "ESP = %08X EBP = %08X\n"
						 "Stack size: %u\n"
						 "Function calls: %u\n",
						 ExAddr, _eax, _ebx, _ecx, _edx, _esi, _edi, _esp, _ebp, snum, num);

		//-----------------------------------------------------------------------------
		// Dump it!
		//-----------------------------------------------------------------------------
		{
			int type = 0, size = 0;
		
			// - скидываем call stack
			type = FEX_BLOCK_TYPE_CALLSTACK;
		
			fwrite(&type, 4, 1, FEX_dumpfile);
			size = num*4+4;
			fwrite(&size, 4, 1, FEX_dumpfile);
			fwrite(&num, 4, 1, FEX_dumpfile);
			fwrite(buf, num*4, 1, FEX_dumpfile);

			type = FEX_BLOCK_TYPE_FUNCADDRESSES;

			fwrite(&type, 4, 1, FEX_dumpfile);
			size = num*4+4;
			fwrite(&size, 4, 1, FEX_dumpfile);
			fwrite(&num, 4, 1, FEX_dumpfile);
			fwrite(buf1, num*4, 1, FEX_dumpfile);

			type = FEX_BLOCK_TYPE_CALLADDRESSES;

			fwrite(&type, 4, 1, FEX_dumpfile);
			size = num*4+4;
			fwrite(&size, 4, 1, FEX_dumpfile);
			fwrite(&num, 4, 1, FEX_dumpfile);
			fwrite(buf2, num*4, 1, FEX_dumpfile);
		}
		//-----------------------------------------------------------------------------
		// Dump it!
		//-----------------------------------------------------------------------------
		{
			int type = 0, size = 4;
		
			// - скидываем GetLastError()
			type = FEX_BLOCK_TYPE_LASTERROR;
		
			fwrite(&type, 4, 1, FEX_dumpfile);
			fwrite(&size, 4, 1, FEX_dumpfile);
			fwrite(&FEX_LastError, 4, 1, FEX_dumpfile);

			// - скидываем errno
			type = FEX_BLOCK_TYPE_ERRNO;

			fwrite(&type, 4, 1, FEX_dumpfile);
			fwrite(&size, 4, 1, FEX_dumpfile);
			fwrite(&FEX_errno, 4, 1, FEX_dumpfile);

			// - скидываем системную информацию
			type = FEX_BLOCK_TYPE_DXVERSION;

			fwrite(&type, 4, 1, FEX_dumpfile);
			fwrite(&size, 4, 1, FEX_dumpfile);
			fwrite(&FEX_DXVersion, 4, 1, FEX_dumpfile);

			type = FEX_BLOCK_TYPE_DISKSPACE;
			size = 6*8+2;
			fwrite(&type, 4, 1, FEX_dumpfile);
			fwrite(&size, 4, 1, FEX_dumpfile);
			fwrite(&FEX_WinDrive, 1, 1, FEX_dumpfile);
			fwrite(&FEX_WinDiskAllSpace, 8, 1, FEX_dumpfile);
			fwrite(&FEX_WinDiskFreeSpace1, 8, 1, FEX_dumpfile);
			fwrite(&FEX_WinDiskFreeSpace2, 8, 1, FEX_dumpfile);
			fwrite(&FEX_CurDrive, 1, 1, FEX_dumpfile);
			fwrite(&FEX_CurDiskAllSpace, 8, 1, FEX_dumpfile);
			fwrite(&FEX_CurDiskFreeSpace1, 8, 1, FEX_dumpfile);
			fwrite(&FEX_CurDiskFreeSpace2, 8, 1, FEX_dumpfile);

			type = FEX_BLOCK_TYPE_SYSTEMVERSION;
			size = strlen(FEX_SystemVersionString) + 1;
			fwrite(&type, 4, 1, FEX_dumpfile);
			fwrite(&size, 4, 1, FEX_dumpfile);
			fwrite(FEX_SystemVersionString, size, 1, FEX_dumpfile);

			type = FEX_BLOCK_TYPE_MEMSTATUS;
			size = 64;
			fwrite(&type, 4, 1, FEX_dumpfile);
			fwrite(&size, 4, 1, FEX_dumpfile);
			fwrite(&FEX_MemStatus1, 32, 1, FEX_dumpfile);
			fwrite(&FEX_MemStatus2, 32, 1, FEX_dumpfile);

			type = FEX_BLOCK_TYPE_VIDEONAME;
			size = strlen(FEX_VideoName) + 1;
			fwrite(&type, 4, 1, FEX_dumpfile);
			fwrite(&size, 4, 1, FEX_dumpfile);
			fwrite(FEX_VideoName, size, 1, FEX_dumpfile);

			FEX_LOG(FEX_logfile, "Video name: %s\n",FEX_VideoName);			

			type = FEX_BLOCK_TYPE_AUDIONAME;
			size = strlen(FEX_AudioName) + 1;
			fwrite(&type, 4, 1, FEX_dumpfile);
			fwrite(&size, 4, 1, FEX_dumpfile);
			fwrite(FEX_AudioName, size, 1, FEX_dumpfile);

			FEX_LOG(FEX_logfile, "Audio name: %s\n",FEX_AudioName);			

			char* TextInfoStr = GetFEX_Log();
			if(TextInfoStr)
			{
				type = FEX_BLOCK_TYPE_TEXTINFO;
				size = strlen(TextInfoStr) + 1;
				fwrite(&type, 4, 1, FEX_dumpfile);
				fwrite(&size, 4, 1, FEX_dumpfile);
				fwrite(TextInfoStr, size, 1, FEX_dumpfile);
			}
			FEX_LOG(FEX_logfile, "User info:\n%s\n",TextInfoStr);

		}
		//-----------------------------------------------------------------------------
		FEX_LOG(FEX_logfile, "Call stack due to calls\n\n");
		for(int j = 0; j < num; j++)
			FEX_LOG(FEX_logfile, "%08X\n", buf[j]);

		unsigned buff[64];
		buff[0]=_eip;
		int FEX_GetCallStackEBP_Ex(unsigned currEBP,unsigned* CallStack, int MaxDepth);
		int N=FEX_GetCallStackEBP_Ex(_ebp,buff+1,63);

		FEX_LOG(FEX_logfile, "\n[EBP_CALLSTACK]\n");
		for(int i=0;i<N;i++)
			FEX_LOG(FEX_logfile, "%08X\n", buff[i]);

		FEX_LOG_CLOSE();
		FEX_DUMP_CLOSE();

#ifdef COPY_PATH
		SYSTEMTIME ST;
		GetLocalTime(&ST);
		static char cc[256];
		static char tmp[MAX_COMPUTERNAME_LENGTH + 2];
		unsigned long ss = MAX_COMPUTERNAME_LENGTH + 1;

		GetComputerName(tmp, &ss);
		sprintf(cc,"%s%s_BUG_%d.%d.%d_%d@%d@%d",COPY_PATH,tmp,ST.wDay,ST.wMonth,ST.wYear,ST.wHour,ST.wMinute,ST.wSecond);
		CreateDirectory(cc,0);
		strcat(cc,"\\");
		strcat(cc,FEX_fnameDump);
		CopyFile(FEX_fnameDump,cc,false);

		sprintf(FEX_tmp, "sendbug.exe %s", FEX_fnameDump);
		WinExec(FEX_tmp, SW_NORMAL);
#else
		//sprintf(FEX_tmp, "sendbug.exe %s", FEX_fnameDump);
		sprintf(FEX_tmp, "sendbug.exe %s", FEX_fnameLog);
		WinExec(FEX_tmp, SW_NORMAL);
#endif
		lRet = EXCEPTION_EXECUTE_HANDLER;
   }
   __except(EXCEPTION_EXECUTE_HANDLER)
   {
		lRet = EXCEPTION_CONTINUE_SEARCH;
   }

	return lRet;
}
//-----------------------------------------------------------------------------------
void FEX_Install(void)
{
	#ifdef FEX_FUNC_HANDLER
		FEX_OrigFilter = SetUnhandledExceptionFilter(FEX_CrashHandlerExceptionFilter);
	#endif

	FEX_DXVersion = 2049;//FEX_GetDXVersion();
	FEX_VideoName = FEX_GetDisplayName();
	FEX_AudioName = "";//FEX_GetAudioName();

   GetWindowsDirectory(FEX_RootFolder, _MAX_PATH+1);
	FEX_WinDrive = tolower(FEX_RootFolder[0]) - 'a';
	GetCurrentDirectory(_MAX_PATH+1, FEX_RootFolder);
	FEX_CurDrive = tolower(FEX_RootFolder[0]) - 'a';

	FEX_GetFreeSpace(FEX_WinDrive, FEX_WinDiskAllSpace, FEX_WinDiskFreeSpace1);
	FEX_GetFreeSpace(FEX_CurDrive, FEX_CurDiskAllSpace, FEX_CurDiskFreeSpace1);
	FEX_GetSystemVersion();
	GlobalMemoryStatus(&FEX_MemStatus1);
}
//------------------------LOGGGING----------------------------------------------------
void _dbgPrintAllLogs(char* dest,int MaxL);
char* GetFEX_Log(){
	static char LOG[4096];
	_dbgPrintAllLogs(LOG,4095);
	return LOG;
}
HANDLE hMutex;
int GetAbsoluteTime(){	
	LARGE_INTEGER FREQ;
	if(!QueryPerformanceFrequency(&FREQ))return GetTickCount();
	LARGE_INTEGER DD;
	QueryPerformanceCounter(&DD);
	double Freq=double(FREQ.LowPart)+double(FREQ.HighPart)*double(0x100000000);
	double Time=double(DD.LowPart)+double(DD.HighPart)*double(0x100000000);
	return int((Time*1000)/Freq);	
};
bool CrashBox(char* Header,unsigned _ebp,unsigned _eip);
int LastProcMessageTime=0;

void ReportProcessMessages(int T){
	//WaitForSingleObject(hMutex,INFINITE);
	///LastProcMessageTime=GetAbsoluteTime();
	//ReleaseMutex(hMutex);
}
#include "..\smart_assert.h"
int susptime=20000;
DWORD WINAPI SuspThreadProc(LPVOID lpParam)
{ 
	// - get the thread context
	int TT0=GetTickCount();
	int DT0=0;
	do{
		DT0=GetAbsoluteTime()-TT0;
		HANDLE hParentThread = *(HANDLE*)lpParam;
		assert(LastProcMessageTime!=0);
		//WaitForSingleObject(hMutex,INFINITE);
		int TL=LastProcMessageTime;
		//ReleaseMutex(hMutex);
		if(LastProcMessageTime!=0&&GetAbsoluteTime()-TL>susptime){
			int DT=GetAbsoluteTime()-TL;
			CONTEXT cnt;
			ZeroMemory(&cnt, sizeof(cnt));
			cnt.ContextFlags = CONTEXT_FULL;
			DWORD res = SuspendThread(hParentThread);
			if(res!=0xFFFFFFFF){
				BOOL bRes = GetThreadContext(hParentThread,  &cnt);
				res = ResumeThread(hParentThread);
				if(CrashBox("SUSPENDED!!!",cnt.Ebp,cnt.Eip)){
					ReportProcessMessages(GetTickCount());
				}
			}
		}
		TT0=GetAbsoluteTime();
		Sleep(20);
	}while(1);
	return 0; 
} 
 
void RunAntiSuspendThread()
{ 
	static DWORD dwThreadId;
	static HANDLE hThread;
	char szMsg[80];
	static HANDLE hThrd = GetCurrentThread();
	static HANDLE hDupThrd;
	LastProcMessageTime=GetTickCount();
	hMutex=CreateMutex(0,1,"antisuspend");
	assert(hMutex!=INVALID_HANDLE_VALUE);
	BOOL fSuccess = DuplicateHandle(
		GetCurrentProcess(),
		hThrd,
		GetCurrentProcess(),
		&hDupThrd,
		0,
		FALSE,DUPLICATE_SAME_ACCESS);
	hThread = CreateThread( 
		NULL,                        // default security attributes 
		0,                           // use default stack size  
		SuspThreadProc,              // thread function 
		&hDupThrd,		           // argument to thread function 
		0,                           // use default creation flags 
		&dwThreadId);                // returns the thread identifier 
	SetThreadPriority(hThread,THREAD_PRIORITY_TIME_CRITICAL);
}

