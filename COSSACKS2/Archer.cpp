#include "stdheader.h"
#include "BE_HEADERS.h"

// aiARCHER_FEELINGS /////////////////////////////////////////////////////
void	aiARCHER_LOGIC::INIT(){
	//====================== DISTANSE ===========================//
	// NEAR
	DISTANCE[0].lLINE.x0 = 0.f;		DISTANCE[0].rLINE.x0 = 200.f;
	DISTANCE[0].lLINE.y0 = 0.f;		DISTANCE[0].rLINE.y0 = 1.f;
	DISTANCE[0].lLINE.x1 = 50.f;	DISTANCE[0].rLINE.x1 = 250.f;
	DISTANCE[0].lLINE.y1 = 1.f;		DISTANCE[0].rLINE.y1 = 0.f;
	// NORMAL
	DISTANCE[1].lLINE.x0 = 200.f;	DISTANCE[1].rLINE.x0 = 500.f;
	DISTANCE[1].lLINE.y0 = 0.f;		DISTANCE[1].rLINE.y0 = 1.f;
	DISTANCE[1].lLINE.x1 = 300.f;	DISTANCE[1].rLINE.x1 = 600.f;
	DISTANCE[1].lLINE.y1 = 1.f;		DISTANCE[1].rLINE.y1 = 0.f;
	// FAR
	DISTANCE[2].lLINE.x0 = 500.f;	DISTANCE[2].rLINE.x0 = 5000.f;
	DISTANCE[2].lLINE.y0 = 0.f;		DISTANCE[2].rLINE.y0 = 1.f;
	DISTANCE[2].lLINE.x1 = 700.f;	DISTANCE[2].rLINE.x1 = 5000.f;
	DISTANCE[2].lLINE.y1 = 1.f;		DISTANCE[2].rLINE.y1 = 0.f;

	//======================== LIFE =============================//
	// WEAK
	LIFE[0].lLINE.x0 = 0.f;		LIFE[0].rLINE.x0 = 10.f;
	LIFE[0].lLINE.y0 = 0.f;		LIFE[0].rLINE.y0 = 1.f;
	LIFE[0].lLINE.x1 = 10.f;	LIFE[0].rLINE.x1 = 25.f;
	LIFE[0].lLINE.y1 = 1.f;		LIFE[0].rLINE.y1 = 0.f;
	// NORMAL
	LIFE[1].lLINE.x0 = 20.f;	LIFE[1].rLINE.x0 = 30.f;
	LIFE[1].lLINE.y0 = 0.f;		LIFE[1].rLINE.y0 = 1.f;
	LIFE[1].lLINE.x1 = 25.f;	LIFE[1].rLINE.x1 = 45.f;
	LIFE[1].lLINE.y1 = 1.f;		LIFE[1].rLINE.y1 = 0.f;
	// POWERFUL
	LIFE[2].lLINE.x0 = 40.f;	LIFE[2].rLINE.x0 = 1000.f;
	LIFE[2].lLINE.y0 = 0.f;		LIFE[2].rLINE.y0 = 1.f;
	LIFE[2].lLINE.x1 = 45.f;	LIFE[2].rLINE.x1 = 1000.f;
	LIFE[2].lLINE.y1 = 1.f;		LIFE[2].rLINE.y1 = 0.f;

	//====================== MELE_ENM ===========================//	
	// MELE UNITS
	MELE_ENM[0].lLINE.x0 = .5f;		MELE_ENM[0].rLINE.x0 = 1.f;
	MELE_ENM[0].lLINE.y0 = 0.f;		MELE_ENM[0].rLINE.y0 = 1.f;
	MELE_ENM[0].lLINE.x1 = .8f;		MELE_ENM[0].rLINE.x1 = 1.f;
	MELE_ENM[0].lLINE.y1 = 1.f;		MELE_ENM[0].rLINE.y1 = 0.f;
	// RANGE ATTACK UNITS
	MELE_ENM[1].lLINE.x0 = 0.f;		MELE_ENM[1].rLINE.x0 = .4f;
	MELE_ENM[1].lLINE.y0 = 0.f;		MELE_ENM[1].rLINE.y0 = 1.f;
	MELE_ENM[1].lLINE.x1 = 0.f;		MELE_ENM[1].rLINE.x1 = .8f;
	MELE_ENM[1].lLINE.y1 = 1.f;		MELE_ENM[1].rLINE.y1 = 0.f;

	//======================= MATRIX ============================//
	// MELE
	//			WEAK								NORMAL								PUWERFUL
	/*NEAR*/	FUZZY_LOGIC[0][0][0] = aiESCAPE;	FUZZY_LOGIC[0][1][0] = aiESCAPE;	FUZZY_LOGIC[0][2][0] = aiATACK;
	/*NORM*/	FUZZY_LOGIC[1][0][0] = aiESCAPE;	FUZZY_LOGIC[1][1][0] = aiATACK;		FUZZY_LOGIC[1][2][0] = aiATACK;
	/*FARE*/	FUZZY_LOGIC[2][0][0] = aiFREE;		FUZZY_LOGIC[2][1][0] = aiFREE;		FUZZY_LOGIC[2][2][0] = aiFREE;

	// RANGE

	/*NEAR*/	FUZZY_LOGIC[0][0][1] = aiESCAPE;	FUZZY_LOGIC[0][1][1] = aiATACK;	FUZZY_LOGIC[0][2][1] = aiATACK;
	/*NORM*/	FUZZY_LOGIC[1][0][1] = aiESCAPE;	FUZZY_LOGIC[1][1][1] = aiATACK;	FUZZY_LOGIC[1][2][1] = aiATACK;
	/*FARE*/	FUZZY_LOGIC[2][0][1] = aiFREE;		FUZZY_LOGIC[2][0][1] = aiFREE;	FUZZY_LOGIC[2][0][1] = aiFREE;

	//===================== TRUST_LELEV ==========================//
	// aiESCAPE
	TRUST_LEVEL[0].lLINE.x0 = 0.f;		TRUST_LEVEL[0].rLINE.x0 = 2.f;
	TRUST_LEVEL[0].lLINE.y0 = 0.f;		TRUST_LEVEL[0].rLINE.y0 = 1.f;
	TRUST_LEVEL[0].lLINE.x1 = 2.f;		TRUST_LEVEL[0].rLINE.x1 = 4.f;
	TRUST_LEVEL[0].lLINE.y1 = 1.f;		TRUST_LEVEL[0].rLINE.y1 = 0.f;
	// aiATTACK
	TRUST_LEVEL[1].lLINE.x0 = 3.f;		TRUST_LEVEL[1].rLINE.x0 = 6.f;
	TRUST_LEVEL[1].lLINE.y0 = 0.f;		TRUST_LEVEL[1].rLINE.y0 = 1.f;
	TRUST_LEVEL[1].lLINE.x1 = 6.f;		TRUST_LEVEL[1].rLINE.x1 = 9.f;
	TRUST_LEVEL[1].lLINE.y1 = 1.f;		TRUST_LEVEL[1].rLINE.y1 = 0.f;
	// aiFREE
	TRUST_LEVEL[2].lLINE.x0 = 8.f;		TRUST_LEVEL[2].rLINE.x0 = 9.f;
	TRUST_LEVEL[2].lLINE.y0 = 0.f;		TRUST_LEVEL[2].rLINE.y0 = 1.f;
	TRUST_LEVEL[2].lLINE.x1 = 9.f;		TRUST_LEVEL[2].rLINE.x1 = 10.f;
	TRUST_LEVEL[2].lLINE.y1 = 1.f;		TRUST_LEVEL[2].rLINE.y1 = 0.f;

};

aiARCHER_STATES		aiARCHER_LOGIC::getFL_STATE(const iaARCHER_FILINGS& filings){
	SaveLog("%s","////////////////////////////////////////////////////////////////////////////////////");
	SaveLog("%s","NEW DATA");
	SaveLog("%s%.5f%s%.5f%s%.5f","DIASTANCE = ",filings.distance,"; LIFE = ",filings.life,"; MELE PART = ",filings.melePART);

	setDIST_V(filings.distance  );
	setLIFE_V(filings.life		);
	setMELE_V(filings.melePART	);

	setSTATES_V();

	aiARCHER_STATES retSTATE = aiERROR;
	switch(filings.fl_type) {
	case aiFL_MAX:
		retSTATE = getMAXX_STATE();
		break;
	case aiFL_RAND:
		retSTATE = getRAND_STATE();
		break;
	case aiFL_CENTROID:
		retSTATE = getCENT_STATE();
		break;
		};

	SaveLog("%s","////////////////////////////////////////////////////////////////////////////////////");
	return	retSTATE;
};

void	aiARCHER_LOGIC::setDIST_V(const float dist){
	for (int i=0; i<DIST_N; i++)	distV[i] = DISTANCE[i].getY(dist);

	SaveLog("%s","\nDISTANCE");
	SaveLog("%s%.5f%s%.5f%s%.5f","Near = ",distV[0]," Norma = ",distV[1]," Far = ",distV[2]);
};

void	aiARCHER_LOGIC::setLIFE_V(const float life){
	for (int i=0; i<LIFE_N; i++)	lifeV[i] = LIFE[i].getY(life);

	SaveLog("%s","\nLIFE");
	SaveLog("%s%.5f%s%.5f%s%.5f","Weak = ",lifeV[0]," Norma = ",lifeV[1]," Power = ",lifeV[2]);
};

void	aiARCHER_LOGIC::setMELE_V(const float part){
	for (int i=0; i<MELE_N; i++)	meleV[i] = MELE_ENM[i].getY(part);

	SaveLog("%s","\nMELLE");
	SaveLog("%s%.5f%s%.5f","Mele = ",meleV[0]," Range = ",meleV[1]);
};

void	aiARCHER_LOGIC::clsSTATE_V(){
	for (int i=0; i<aiMAX_ARCHER_STATES; i++){
		stateV[i].weight = 0.f;
		stateV[i].num	 = 0;
	};
};

void	aiARCHER_LOGIC::setSTATE_V(aiARCHER_STATES st, float wh){
	int N = aiMAX_ARCHER_STATES;
	while (N--) {
		if (stateV[N].state==st) {
			stateV[N].weight += wh;
			stateV[N].num++;
			N=0;
		};
	};
};

void	aiARCHER_LOGIC::setSTATES_V(){
	clsSTATE_V();
	float	w = 0.f;
	for (int M=0; M<MELE_N; M++){
		for (int L=0; L<LIFE_N; L++){
			for (int D=0; D<DIST_N; D++){
				w = min(min(distV[D],lifeV[L]),meleV[M]);
				setSTATE_V(FUZZY_LOGIC[D][L][M],w);
			};
		};
	};

	if (saveLogF) {
		SaveLog("%s","\nMATRIX");
		for (int i=0; i<DIST_N; i++){
			float w0,w1,w2;
			w0 = min(min(distV[i],lifeV[0]),meleV[0]);
			w1 = min(min(distV[i],lifeV[1]),meleV[0]);
			w2 = min(min(distV[i],lifeV[2]),meleV[0]);
			SaveLog("%s%d%s%.5f%s%d%s%.5f%s%d%s%.5f",
				" [",FUZZY_LOGIC[i][0][0],"]",w0,
				" [",FUZZY_LOGIC[i][1][0],"]",w1,
				" [",FUZZY_LOGIC[i][2][0],"]",w2 );
		};
		SaveLog("%s","\n");
		for (int i=0; i<DIST_N; i++){
			float w0,w1,w2;
			w0 = min(min(distV[i],lifeV[0]),meleV[1]);
			w1 = min(min(distV[i],lifeV[1]),meleV[1]);
			w2 = min(min(distV[i],lifeV[2]),meleV[1]);
			SaveLog("%s%d%s%.5f%s%d%s%.5f%s%d%s%.5f",
				" [",FUZZY_LOGIC[i][0][1],"]",w0,
				" [",FUZZY_LOGIC[i][1][1],"]",w1,
				" [",FUZZY_LOGIC[i][2][1],"]",w2 );
		};
		SaveLog("%s%.5f%s%.5f%s%.5f",
			"\nFREE = ",stateV[0].weight,
			" ATTACK = ",stateV[1].weight,
			" ESCAPE = ",stateV[2].weight );
	};
};

int	aiARCHER_LOGIC::SaveLog(LPCSTR format, ...){
	int	result = -1;
	if (saveLogF) {
		FILE* pFile = NULL;
		if ( ( pFile = fopen(nameLogF,"a") ) != NULL ){
			size_t		length = strlen(format);
			LPSTR		_format = (LPSTR)malloc((length + 3)*sizeof(char));
			strcpy		(_format,format);
			strcat		(_format,"\n");

			va_list		marker;
			va_start	(marker,format);
			result = vfprintf(pFile,_format,marker);
			va_end		(marker);

            //  Added by Silver, 23.06.04. People, please do this
            free        (_format);

			fclose(pFile);
		};
	};
	return		(result);
};

char*	aiARCHER_LOGIC::GetSateStr(int _state){
	switch(_state) {
	case 0:
		return "FREE";
	case 1:
		return "ATTACK";
	case 2:
		return "ESCAPE";
		};
	return "ERROR";
};

aiARCHER_STATES	aiARCHER_LOGIC::getRAND_STATE(){
	float	SUMM = 0.f;
	for (int i=0; i<aiMAX_ARCHER_STATES; i++)	SUMM += stateV[i].weight;
	if (SUMM==0)	return aiERROR;
	float RND = rand()%(int)(1000.f*SUMM);
	SUMM = 0.f;
	int stID = -1;
	i = aiMAX_ARCHER_STATES;
	while (stID==-1&&i--) {
		if ( SUMM<=RND && RND<SUMM+stateV[i].weight ) {
			stID = i;
		};
		SUMM += stateV[i].weight;
	};
	if (stID!=-1){
		SaveLog("%s%s","RAND STATE LOGIC = ",GetSateStr(stateV[stID].state));
		return stateV[stID].state;
	};
	SaveLog("%s","RAND STATE LOGIC = ERROR");
	return aiERROR;
};

aiARCHER_STATES	aiARCHER_LOGIC::getMAXX_STATE(){
	aiARCHER_STATES retSTATE = aiERROR;
	float maxWH = 0.f;
	for (int i=0; i<aiMAX_ARCHER_STATES; i++){
		if (stateV[i].weight > maxWH) {
			maxWH	 = stateV[i].weight;
			retSTATE = stateV[i].state;
		};
	};
	SaveLog("%s%s","MAX STATE LOGIC = ",GetSateStr(retSTATE));
	return retSTATE;
};

aiARCHER_STATES	aiARCHER_LOGIC::getCENT_STATE(){
	return aiERROR;
};

//////////////////////////////////////////////////////////////////////////
