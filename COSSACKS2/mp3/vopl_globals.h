#pragma once

#ifndef _vopl_globals_included_
#define _vopl_globals_included_

#define VOPL_MUTEX_NAME "voplMutex"
#define VOPL_MAPPING_NAME "voplMapping"

enum VOPL_COMMAND{
	vcFree=0,
	vcPlay,	
	vcStop,	
	vcResume,
	vcKill,	
	vcVolume,
	vcFinished,
	vcLength
};

typedef struct COMMAND_BUFFER{
	VOPL_COMMAND			Command;
	char					szFileName[MAX_PATH];
	DWORD					Volume;
	DWORD					Length;
	DWORD					Finished;
	DWORD					Continous;
}*LP_COMMAND_BUFFER;

#endif // _vopl_globals_included_

