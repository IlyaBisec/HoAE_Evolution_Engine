#ifndef __BE_Archer__
#define __BE_Archer__

// aiARCHER_FEELINGS /////////////////////////////////////////////////////
enum	aiFL_TYPE { aiFL_MAX=0, aiFL_RAND=1, aiFL_CENTROID=2 };

struct iaARCHER_FILINGS
{
	float distance;
	float life;
	float melePART;

	aiFL_TYPE	fl_type;

	void Clean() { distance=life=melePART=0.f; };
};

const int aiMAX_ARCHER_STATES = 3;
enum aiARCHER_STATES { aiFREE=0, aiATACK=1, aiESCAPE=2, aiERROR=255 };

// aiARCHER_LOGIC ////////////////////////////////////////////////////////
class aiARCHER_LOGIC
{
public:
	aiARCHER_LOGIC() {	stateV[0].state=aiFREE; stateV[1].state=aiATACK; stateV[2].state=aiESCAPE; };
public:
	static	const DIST_N	= 3;
	static	const LIFE_N	= 3;
	static	const MELE_N	= 2;
	static	const TRUST_N	= 3;
	aiLINE2D	DISTANCE[DIST_N];
	aiLINE2D	LIFE	[LIFE_N];
	aiLINE2D	MELE_ENM[MELE_N];
	aiARCHER_STATES		FUZZY_LOGIC[DIST_N][LIFE_N][MELE_N];
	aiLINE2D	TRUST_LEVEL[TRUST_N];

	static	bool		saveLogF;
	static	char		nameLogF[256];

	void				INIT();
	aiARCHER_STATES		getFL_STATE(const iaARCHER_FILINGS& filings);
private:
	struct ST_WT {
		aiARCHER_STATES		state;
		float				weight;
		int					num;
		};

	float	distV[DIST_N];
	float	lifeV[LIFE_N];
	float	meleV[MELE_N];
	void	setDIST_V(const float dist);
	void	setMELE_V(const float part);
	void	setLIFE_V(const float life);

	ST_WT	stateV[aiMAX_ARCHER_STATES];
	void	clsSTATE_V();
	void	setSTATE_V(aiARCHER_STATES st, float wh);
	void	setSTATES_V();

	static	int		SaveLog(LPCSTR format, ...);
	char*	GetSateStr(int _state);

	aiARCHER_STATES	getRAND_STATE();
	aiARCHER_STATES	getMAXX_STATE();
	aiARCHER_STATES	getCENT_STATE();
};

//////////////////////////////////////////////////////////////////////////
#endif//__BE_Archer__