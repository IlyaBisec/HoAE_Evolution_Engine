#ifndef _vopl_globals_included_
#define _vopl_globals_included_

#define VOPL_NUM_CHAN 5

enum VOPL_COMMAND{
	vcFree=0,
	vcPlay,	
	vcStop,	
	vcVolume,
	vcCyclic	
};

typedef struct VOPL_CHAN_CMD{
	VOPL_COMMAND			m_vcCommand;
	char					m_szFileName[MAX_PATH];
	unsigned char			m_ucVolume;
	unsigned short			m_usLength;
	bool					m_bFinished;
	bool					m_bCyclic;
	bool					m_bFade;
	LONG					m_lTargetVolume;
}*LPVOPL_CHAN_CMD;

typedef struct VOPL_CMD_BUFF{
	VOPL_CHAN_CMD			m_Chan[VOPL_NUM_CHAN];
	bool					m_bTerminate;
}*LPVOPL_CMD_BUFF;


#endif // _vopl_globals_included_

