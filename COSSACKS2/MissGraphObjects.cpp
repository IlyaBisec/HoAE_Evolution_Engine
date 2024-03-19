#include "stdheader.h"
#include "BE_HEADERS.h"

be_CCloud	g_beCloudTest;
be_CCloud	g_beCloudTest_Scirmish;

bool	be_CCloud::NeedCancel = false;
bool	be_CCloud::NeedCenter = false;

bool	be_CCloud::SetCanceled(SimpleDialog* pSD){
	NeedCancel = true;
	return true;	
};

bool	be_CCloud::SetCentered(SimpleDialog* pSD){
	NeedCenter = true;
	return true;
};

void	be_CCloud::SetOnUserClickCallBack(){
	SimpleDialog* pSD = NULL;
	if( ds_MAIN.DSS.GetAmount()==0 || ds_MAIN.DSS[0]==NULL ) 
		return;
	pSD = ds_MAIN.DSS[0]->Find("CV00");
	if (pSD!=NULL){
		pSD->OnUserClick = be_CCloud::SetCanceled;
		pSD->OnUserRightClick = be_CCloud::SetCentered;
	}
	pSD = ds_MAIN.DSS[0]->Find("CV00_X");
	if (pSD!=NULL){
		pSD->OnUserClick = be_CCloud::SetCanceled;
	}
    pSD = ds_MAIN.DSS[1]->Find("CV01");
	if (pSD!=NULL){
		pSD->OnUserClick = be_CCloud::SetCanceled;
		pSD->OnUserRightClick = be_CCloud::SetCentered;
	};
	pSD = ds_MAIN.DSS[1]->Find("CV01_X");
	if (pSD!=NULL){
		pSD->OnUserClick = be_CCloud::SetCanceled;
	};
	pSD = ds_MAIN.DSS[2]->Find("CV02");
	if (pSD!=NULL){
		pSD->OnUserClick = be_CCloud::SetCanceled;
		pSD->OnUserRightClick = be_CCloud::SetCentered;
	};
	pSD = ds_MAIN.DSS[2]->Find("CV02_X");
	if (pSD!=NULL){
		pSD->OnUserClick = be_CCloud::SetCanceled;
	};
	pSD = ds_MAIN.DSS[3]->Find("CV03");
	if (pSD!=NULL){
		pSD->OnUserClick = be_CCloud::SetCanceled;
		pSD->OnUserRightClick = be_CCloud::SetCentered;
	};
	pSD = ds_MAIN.DSS[3]->Find("CV03_X");
	if (pSD!=NULL){
		pSD->OnUserClick = be_CCloud::SetCanceled;
	};
};

void				be_CCloud::SetDefDSFile(){
	ds_FileName = "Missions\\GRAPHICS\\Dialogs\\CloudDialogs.DialogsSystem.xml";
};
void				be_CCloud::InitDS(){
	if (ds_Init){
        ds_Init = false;
		SetDefDSFile();
		xmlQuote	xmlDS;
		xmlDS.ReadFromFile(ds_FileName.pchar());
		ErrorPager	Err;
		ds_MAIN.Load(xmlDS,&ds_MAIN,&Err);
		hideAll();
		SetOnUserClickCallBack();
		ds_MAIN.ProcessDialogs();
	};
};
void				be_CCloud::hideAll(){
	InitDS();
	for ( int i=0; i<getCloudTypeNUM(); i++ ){
		ds_MAIN.DSS[i]->Visible = false;	
	};
};
void				be_CCloud::vievCloudType(int cloudTypeLID){
	InitDS();
	ds_MAIN.DSS[cloudTypeLID]->Visible = true;	
};
void				be_CCloud::centreTextY(int cloudTypeLID){
	InitDS();
	TextButton*	pTB = getTextOBJ(cloudTypeLID);
    GPPicture*	pGP = getCloudOBJ(cloudTypeLID,getSubCloudNUM(cloudTypeLID)-1);
	if (pTB&&pGP) {
		pTB->Sety( ( (pGP->y1 - pGP->y) - (pTB->y1 - pTB->y) )/2 - 2 );
	};
};
void				be_CCloud::centreTextX(int cloudTypeLID){
	if (cloudTypeLID==getCloudTypeNUM()-1) return;
	InitDS();
	TextButton*	pTB = getTextOBJ(cloudTypeLID);
    GPPicture*	pGP = getCloudOBJ(cloudTypeLID,getSubCloudNUM(cloudTypeLID)-1);
	if (pTB&&pGP) {
		pTB->Setx( ( (pGP->x1 - pGP->x) - (pTB->x1 - pTB->x) )/2 );
	};
};
int					be_CCloud::getProperCloudType(char* _text){
	InitDS();
	int ctN = getCloudTypeNUM();
	for (int i=0; i<ctN; i++){
		if (isTextTooLarge(i,_text)==false){
			return i;
		};
	};
	return ctN-1;
};
void				be_CCloud::setRandCloud(int cloudTypeLID){
	InitDS();
	GPPicture*	pGP = getCloudOBJ(cloudTypeLID,getSubCloudNUM(cloudTypeLID)-1);
	if (pGP) {
		pGP->SpriteID = ( (pGP->SpriteID + 1) % GPS.GPNFrames(pGP->FileID) );
	};
};
void				be_CCloud::setText(int cloudTypeLID,char* _text){
	InitDS();
	TextButton* pTB = getTextOBJ(cloudTypeLID);
	bool showBig = false;
	if (pTB!=NULL){
		pTB->SetMessage(_text);
		showBig = ( (pTB->y1 - pTB->y) <= pTB->ID );
		pTB->Visible = showBig;
	};
	pTB = getTextOBJ(cloudTypeLID,true);
	if ( pTB!=NULL ){
		pTB->SetMessage(_text);
		pTB->Visible = ( (showBig) ? (false) : ( (pTB->y1 - pTB->y) <= pTB->ID ) );
		if ( cloudTypeLID==3 && showBig==false && pTB->Visible==false ) pTB->Visible==true;
	};
};
int					be_CCloud::getCloudTypeNUM(){
	InitDS();
	return ds_MAIN.DSS.GetAmount();
};
TextButton*			be_CCloud::getTextOBJ(int cloudTypeLID,bool st/*=false*/){
	InitDS();
	if ( !st )
		return dynamic_cast<TextButton*>(ds_MAIN.DSS[cloudTypeLID]->Find("Text"));
	else	
		return dynamic_cast<TextButton*>(ds_MAIN.DSS[cloudTypeLID]->Find("TextS"));
};
bool				be_CCloud::isTextTooLarge(int cloudTypeLID,char* _text){
	InitDS();
	setText(cloudTypeLID,_text);
	TextButton*	pTB = getTextOBJ(cloudTypeLID);
	if ( pTB!=NULL && (pTB->y1 - pTB->y) > pTB->ID ){
		pTB = getTextOBJ(cloudTypeLID,true);
		if(! pTB )return true;
		return ( (pTB->y1 - pTB->y) > pTB->ID );
	};
	return false;
};
GPPicture*			be_CCloud::getPictureOBJ(int cloudTypeLID){
	InitDS();
	return dynamic_cast<GPPicture*>(ds_MAIN.DSS[getSubCloudNUM(cloudTypeLID)-1]->Find("Portrait"));
};
void				be_CCloud::setProperPictureOBJ(int cloudTypeLID,char* EmotionName){
	if ( UnitID==-1 ) return;
	OneObject* pOB  = Group[UnitID];
	GPPicture* pGPP = getPictureOBJ(cloudTypeLID);
    if (pGPP!=NULL&&pOB!=NULL&&pOB->UniqID==UnitUniqID) {
		pGPP->FileID	= 0;
		pGPP->SpriteID	= -1;
		// Set proper picture for speaker
		if ( g_beCheckChar(EmotionName)==true ){
			int EN = pOB->newMons->EmotIcons.GetAmount();
			while (EN--) {
				if ( pOB->newMons->EmotIcons[EN]->EmotionName == EmotionName ){
					pGPP->FileID	= pOB->newMons->EmotIcons[EN]->File;
					pGPP->SpriteID	= pOB->newMons->EmotIcons[EN]->Sprite;
					break;
				}
			}
		}
		static Enumerator* E=ENUM.Get("BE_SPEAKERSPICS_ENUM");
		int sp=E->Get(EmotionName);
		if(sp!=-1){
			pGPP->FileID=sp&0xFFFF;
			pGPP->SpriteID=sp>>16;
		}else{
            char emot[64];
			strcpy(emot,EmotionName);
			char* s=strchr(emot,'_');
			if(s){
				strcpy(s,"_Normal");
				int sp=E->Get(emot);
				if(sp!=-1){
					pGPP->FileID=sp&0xFFFF;
					pGPP->SpriteID=sp>>16;
				}else{
					s[0]=0;
					int L=strlen(emot);
					int N=E->GetAmount();
					for(int i=0;i<N;i++){
						if(!strncmp(E->GetStr(i),emot,L)){
							int sp=E->GetVal(i);
							pGPP->FileID=sp&0xFFFF;
							pGPP->SpriteID=sp>>16;
							break;
						}
					}
				}
			}
		}
	}	
}
int					be_CCloud::getSubCloudNUM(int cloudTypeLID){
	InitDS();
	int n = ds_MAIN.DSS[cloudTypeLID]->DSS.GetAmount();
	int rem = 0;
	for ( int i=0; i<n; i++ ){
		if ( ds_MAIN.DSS[cloudTypeLID]->DSS[i]->ID != 0 ) rem++;
	}
	return max(n-rem,0);
};
GPPicture*			be_CCloud::getCloudOBJ(int cloudTypeLID,int i){
	InitDS();
	return dynamic_cast<GPPicture*>(ds_MAIN.DSS[cloudTypeLID]->DSS[i]);
};
bool				be_CCloud::getUnitTopXY(int& _x,int& _y){
	if (UnitID!=-1) {
		OneObject* pOB = Group[UnitID];
		if (pOB&&(!pOB->Sdoxlo||pOB->Hidden)&&pOB->UniqID==UnitUniqID) {
			int _CR,_CH,_UH;
			pOB->GetCylinder(_CR,_CH,_UH);
//			Vector4D	UnitTOP((float)(pOB->RealX>>4),(float)(pOB->RealY>>4),(float)(_CH+_UH),1);
//			WorldToScreenSpace(UnitTOP);
			Vector3D	UnitTOP((float)(pOB->RealX>>4),(float)(pOB->RealY>>4),(float)(_CH+_UH));
//			void WorldToClosestScreenPoint(Vector3D &Pos);
			WorldToClosestScreenPoint(UnitTOP);
			_x = (int)(UnitTOP.x+0.5f);
			_y = (int)(UnitTOP.y+0.5f);
			return true;
		};
	};
	UnitID		= -1;
	UnitUniqID	= 0xFFFFFFFF;
	return false;
};
void				be_CCloud::getStartXY(int& _x,int& _y){
	if ( getUnitTopXY(_x,_y)==false ){
		_x = startX;
		_y = startY;
	};
};
int					be_CCloud::getScreenDX(){
	if		( CloudTypeID==0 )	return 220;
	else if ( CloudTypeID==1 )	return 250;
	return	400;
};
int					be_CCloud::getScreenDY(){
	if		( CloudTypeID==0 )	return 150;
	else if ( CloudTypeID==1 )	return 180;
	return	250;
};
void				be_CCloud::get_addD_Fi(int& _addD,int& _Fi){
	addD = 5;//GEC MEGA HACK . v headere stoit addD = 25;
	_addD  = addD;

	// Calculate hearer center position
	int N = pSPEAKERS.GetAmount();
	if ( N==1 ) Fi = 160;
	else if (Group[UnitID]!=NULL){
		int Xw=0,Yw=0,GN=0;
		while (N--) {
			if ( pSPEAKERS[N]->_CheckGID(UnitID) == 0xFFFF ){
				Xw += pSPEAKERS[N]->GetGroupX();
				Yw += pSPEAKERS[N]->GetGroupY();
				GN++;
			};
		};
		if ( GN!=0 ){
			Xw /= GN;
			Yw /= GN;
		};
		int Xu = Group[UnitID]->RealX / 16;
		if (abs(Xu - Xw)>80){
			Fi = ( (Xu>=Xw) ? (160) : (224) );
		};
	};

	int ux=0,uy=0;
	getStartXY(ux,uy);	
	int DELTAx = getScreenDX();
	int DELTAy = getScreenDY();
	if ( uy < DELTAy ){
		if		( ux<DELTAx )			Fi = 32;
		else if ( ux>RealLx-DELTAx )	Fi = 96;
		else{
			if		( Fi==160 )			Fi = 96;
			else if ( Fi==224 )			Fi = 32;
		}
	}else if ( uy > RealLy-DELTAy ){
		if		( ux<DELTAx )			Fi = 224;
		else if ( ux>RealLx-DELTAx )	Fi = 160;
		else{
			if		( Fi==160 )			Fi = 160;
			else if ( Fi==224 )			Fi = 224;
		}
	}else{
		if		( ux<DELTAx )			Fi = 224;
		else if ( ux>RealLx-DELTAx )	Fi = 160;
		else{
			if		( Fi==160 )			Fi = 160;
			else if ( Fi==224 )			Fi = 224;
		}
	};
	
	_Fi = (((Fi%256)+256)%256);
};
float				be_CCloud::getRADIAN(int angle){
	float absANGLE = (float)(((angle%256)+256)%256);
	return (absANGLE/256.f)*(2.f*3.1415f);
};
void				be_CCloud::getCrossdXdY(int& dx,int& dy,int fi,int w,int h){
	Line2D TopLine(0,h,w,h);
	Line2D RightLine(w,0,w,h);
	int _fi = ((fi%256)+256)%256;
	_fi = ( ( (0<=fi&&fi<64)||(128<=fi&&fi<192) ) ? (fi%64) : (64-fi%64) );
	Line2D ReyLine(0,0,20.f*cos(getRADIAN(_fi)),20.f*sin(getRADIAN(_fi)));
	Vector2D crT = ReyLine.Intersect( TopLine );
    Vector2D crR = ReyLine.Intersect( RightLine );
	dx = ( ( crT.norm()<=crR.norm() ) ? (crT.x) : (crR.x) );
	dy = ( ( crT.norm()<=crR.norm() ) ? (crT.y) : (crR.y) );
	if ( fi> 64  && fi<=192 ) dx = -dx;
	if ( fi>=128 && fi< 256 ) dy = -dy;
};
void				be_CCloud::correctForDrawOnlyOnScreenRect(int& _sX,int& _sY,int& _Fi){
/*
	if ( _sX<0 || RealLx<_sX || _sY<0 || RealLy<_sY ){
		int DELTA = getScreenDX();
		float	ay = ( (_sX-RealLx/2!=0) ? ((float)(_sY-RealLy/2)/(float)(_sX-RealLx/2)) : (0.f) ); 
		float	by = ( (_sX-RealLx/2!=0) ? ((float)(RealLy/2)-(float)(RealLx/2)*ay) : (RealLy/2) );
		float	ax = ( (_sY-RealLy/2!=0) ? ((float)(_sX-RealLx/2)/(float)(_sY-RealLy/2)) : (0.f) ); 
		float	bx = ( (_sY-RealLy/2!=0) ? ((float)(RealLx/2)-(float)(RealLy/2)*ax) : (RealLx/2) );
		if			( 0<=ay*0.0f+by && ay*0.0f+by<=RealLy && _sX<0 ) {							// LEFT
			_sX = 0;
			_sY = ay*0.0f+by;
			Fi = ( (_sY<=DELTA) ? (32) : (224) );
		}else if	( 0<=ay*(float)RealLx+by && ay*(float)RealLx+by<=RealLy && _sX>RealLx ){	// RIGHT
			_sX = RealLx;
			_sY = ay*(float)RealLx+by;
			Fi = ( (_sY<=DELTA) ? (96) : (160) );
		}else if	( 0<=ax*0.0f+bx && ax*0.0f+bx<=RealLx && _sY<0 ) {							// TOP
			_sY = 0;
			_sX = ax*0.0f+bx;
			Fi = ( (_sX<=RealLx/2) ? (32) : (96) );
		}else if	( 0<=ax*(float)RealLy+bx && ax*(float)RealLy+bx<=RealLx && _sY>RealLy ){	// BOTTOM
			_sY = RealLy;
			_sX = ax*(float)RealLy+bx;
			Fi = ( (_sX<=RealLx/2) ? (224) : (160) );
		};
	};
*/
	int DELTA = getScreenDX();
	if ( _sX<=1 )
	{		// LEFT
		Fi = ( (_sY<=DELTA) ? (32) : (224) );
	}else
	if ( _sX>=RealLx-1 )
	{		// RIGHT
		Fi = ( (_sY<=DELTA) ? (96) : (160) );
	}else
	if ( _sY<=1 )
	{		// TOP
		Fi = ( (_sX<=RealLx/2) ? (32) : (96) );
	}else
	if ( _sY>=RealLy-1 )
	{		// BOTTOM
		Fi = ( (_sX<=RealLx/2) ? (224) : (160) );
	}
	// GEC add coorection against user interface
	// current coorection only move up cloud by 255 pixels along oY 
	if ( _sY>RealLy-225 ) _sY = RealLy-225;
	// GEC
	_Fi = Fi;
};
void				be_CCloud::Draw(){
	if (UnitID==-1) return;
	if( (GSets.CGame.ViewMask&4)==0 ) return;

	if ( Group[UnitID]==NULL || !(!Group[UnitID]->Sdoxlo||Group[UnitID]->Hidden) || Group[UnitID]->UniqID!=UnitUniqID ){
		UnitUniqID = 0xFFFFFFFF;
		UnitID = -1;
		return;
	}

	InitDS();

	int _addD,_Fi,sX,sY;
	get_addD_Fi(_addD,_Fi);
	getStartXY(sX,sY);
	correctForDrawOnlyOnScreenRect(sX,sY,_Fi);
	int _N = getSubCloudNUM(CloudTypeID);
	int _dx = (int)( (float)_addD * (float)cos(getRADIAN(_Fi)) );
	int _dy = (int)( (float)_addD * (float)sin(getRADIAN(_Fi)) );
	int _DX=0;
	int _DY=0;
	GPPicture*	pCurCloud = NULL;
	for ( int i=0; i<_N; i++ ){
		pCurCloud = getCloudOBJ(CloudTypeID,i);
		int _W = pCurCloud->x1 - pCurCloud->x;
		int _H = pCurCloud->y1 - pCurCloud->y;
		int _dW,_dH;
		getCrossdXdY(_dW,_dH,_Fi,_W,_H);
//		if ( i+1==_N ) { _dx=_dy=0; };
		_DX += _dx + _dW/2;
		_DY += _dy + _dH/2;
		pCurCloud->Setxy(sX+_DX-_W/2,sY+_DY-_H/2);
		_DX += _dW/2;
		_DY += _dH/2;
	};
	// Draw big cloud
	centreTextY(CloudTypeID);
	centreTextX(CloudTypeID);
	ds_MAIN.ProcessDialogs();
};
bool				be_CCloud::selectSPEAKER(be_CGROUP* pSP_GR,be_CGROUP* pGR_INIT){
	if ( pSP_GR==NULL ) return false;
	OneObject* pOB = NULL;
	int N = pSPEAKERS.GetAmount();
	if ( N==1 && pGR_INIT==pSP_GR ){
		int NU = pSP_GR->GetTotalAmount();
		while ( NU-- ) {
			if ( pOB=(*pSP_GR)[NU] ) {
				UnitID		= pOB->Index;
				UnitUniqID	= pOB->UniqID;
				return true;
			};
		};
	}else if (N==2||(N==1 && pGR_INIT!=NULL && pGR_INIT!=pSP_GR)){
		int minDIST = -1;
		/////////
		be_CGROUP* pGR0 = ( (N==2) ? (pSPEAKERS[0]) : (pGR_INIT)	 );
		be_CGROUP* pGR1 = ( (N==2) ? (pSPEAKERS[1]) : (pSPEAKERS[0]) );
		if ( pGR0==NULL || pGR1==NULL )	return false;
		////////
		int NU0 = pGR0->GetTotalAmount();
		int NU1 = pGR1->GetTotalAmount();
		OneObject* pOB0 = NULL;
		OneObject* pOB1 = NULL;
		int _UID0 = -1;
		int _UID1 = -1;
		for ( int i=0; i<NU0; i++ ){
			pOB0 = (*pGR0)[i];
			for ( int j=0; j<NU1; j++ ){
				pOB1 = (*pGR1)[j];
				if ( pOB0!=NULL && pOB1!=NULL ){
					if ( minDIST==-1 ){
						minDIST = Norma( (pOB0->RealX-pOB1->RealX)/16, (pOB0->RealY-pOB1->RealY)/16 );
						_UID0 = pOB0->Index;
						_UID1 = pOB1->Index;
					}else{
						if ( minDIST > Norma( (pOB0->RealX-pOB1->RealX)/16, (pOB0->RealY-pOB1->RealY)/16 ) ){
							minDIST = Norma( (pOB0->RealX-pOB1->RealX)/16, (pOB0->RealY-pOB1->RealY)/16 );
							_UID0 = pOB0->Index;
							_UID1 = pOB1->Index;
						};
					};
				};
			};
		};
		if ( N==2 ){
			if		 ( pSPEAKERS[0]->Name == pSP_GR->Name ){
				if ( _UID0!=-1 ){
					UnitID = _UID0;
					UnitUniqID = Group[UnitID]->UniqID;
					return true;
				};
			}else if ( pSPEAKERS[1]->Name == pSP_GR->Name ){
				if ( _UID1!=-1 ){
					UnitID = _UID1;
					UnitUniqID = Group[UnitID]->UniqID;
					return true;
				};
			};
		}else{
			if ( _UID1!=-1 ){
				UnitID = _UID1;
				UnitUniqID = Group[UnitID]->UniqID;
				return true;
			};
		};
	}else if (N>=3){
		int NU = pSP_GR->GetTotalAmount();
		int minDIST = -1;
		int _UID = -1;
		OneObject* pOB = NULL;
		for ( int i=0; i<NU; i++ ){
			pOB = (*pSP_GR)[i];
			if (pOB!=NULL) {
				for ( int j=0; j<pSPEAKERS.GetAmount(); j++ ){
					if ( pSPEAKERS[j]->Name != pSP_GR->Name ){
						int XC,YC;
						pSPEAKERS[j]->GetGroupCenter(XC,YC);
						if ( minDIST==-1 ){
							minDIST = Norma( (pOB->RealX/16-XC), (pOB->RealY/16-YC) );
							_UID = pOB->Index;
						}else{
							if ( minDIST > Norma( (pOB->RealX/16-XC), (pOB->RealY/16-YC) ) ){
								minDIST = Norma( (pOB->RealX/16-XC), (pOB->RealY/16-YC) );
								_UID = pOB->Index;
							};
						};	
					};
				};
			};
		};
		if ( _UID!=-1 ){
			UnitID = _UID;
			UnitUniqID = Group[UnitID]->UniqID;
			return true;
		};
	};
	return false;
};
bool	be_CCloud::getCenterOfSPEAKERS(int& x,int& y,be_CGROUP* pGR){
	bool needAddGR = true;
	bool donotAddGR = false;
	x=y=0;
	int tx=0;
	int ty=0;
	int div = 0;
	int N = pSPEAKERS.GetAmount();
	be_CGROUP* _pGR = NULL;
	for ( int i=0; i<N; i++ ){
		_pGR = pSPEAKERS[i];
		if (_pGR){
			donotAddGR=false;
			if ( UnitID!=-1 ){
				donotAddGR = (bool)(_pGR->_CheckGID(UnitID)!=0xFFFF);
			};
			if(!donotAddGR){
				tx=ty=0;
				_pGR->GetGroupCenter(tx,ty);
				if ( tx!=0 && ty!=0 ){
					x += tx;
					y += ty;
					div++;
				};
			};
			if ( pGR && strcmp(pGR->GetName(),_pGR->GetName())==0 ) needAddGR=false;
		};
	};
	if (needAddGR){
		pGR->GetGroupCenter(tx,ty);
		if ( tx!=0 && ty!=0 ) {
			x += tx;
			y += ty;
			div++;
		};
	};
	if ( x!=0 && y!=0 ){
		if ( UnitID!=-1 && Group[UnitID]!=NULL ){
			x += Group[UnitID]->RealX/16;
			y += Group[UnitID]->RealY/16;
			div++;
		};
		if ( div!=0 ) { x/=div; y/=div; };
		return ( div!=0 );
	};
	if ( UnitID!=-1 && Group[UnitID]!=NULL ){
		x = Group[UnitID]->RealX/16;
		y = Group[UnitID]->RealY/16;
		return true;
	};
	if (pGR){
		pGR->GetGroupCenter(x,y);
		return true;
	};
	return false;
};
bool	be_CCloud::getCurrentSpeakerPosition(int& x,int& y){
	x=y=0;
	if (UnitID!=-1) {
		OneObject* pOB = Group[UnitID];	
		if (pOB&&(!pOB->Sdoxlo||pOB->Hidden)&&pOB->UniqID==UnitUniqID) {
			x=pOB->RealX>>4;
			y=pOB->RealY>>4;
			return true;
		};	
	};
	return false;
};
//////////////////////////////////////////////////////////////////////////
void	be_gShowCloudDialog( char* pText, OneObject* pOB ){
	g_beCloudTest_Scirmish.InitDS();
	if ( g_beCheckChar(pText)==false || pOB==NULL ){
		g_beCloudTest_Scirmish.hideAll();
		g_beCloudTest_Scirmish.UnitID=-1;
		g_beCloudTest_Scirmish.UnitUniqID=0xFFFFFFFF;
		return;
	};
	g_beCloudTest_Scirmish.UnitID = pOB->Index;
	g_beCloudTest_Scirmish.UnitUniqID = pOB->UniqID;
	g_beCloudTest_Scirmish.hideAll();
	g_beCloudTest_Scirmish.CloudTypeID = g_beCloudTest_Scirmish.getProperCloudType(pText);
	g_beCloudTest_Scirmish.setRandCloud(g_beCloudTest_Scirmish.CloudTypeID);
	g_beCloudTest_Scirmish.centreTextY(g_beCloudTest_Scirmish.CloudTypeID);
	g_beCloudTest_Scirmish.centreTextX(g_beCloudTest_Scirmish.CloudTypeID);
	g_beCloudTest_Scirmish.setProperPictureOBJ(g_beCloudTest_Scirmish.CloudTypeID,"Normal");
	g_beCloudTest_Scirmish.vievCloudType(g_beCloudTest_Scirmish.CloudTypeID);
};
void	be_gShowCloudDialog(be_CSTR_DIALOG* pSTR,be_CGROUP* pGRP,be_CGROUP* pGR_INIT){
	g_beCloudTest.InitDS();
	if (pSTR==NULL||pGRP==NULL){
		g_beCloudTest.hideAll();
		g_beCloudTest.UnitID=-1;
		g_beCloudTest.UnitUniqID=0xFFFFFFFF;
		return;
	};
	// Create cloud dialog
	pSTR->Update();
	OneObject* OB=pGRP->GetOneObj_lID(0);
	if (OB){
		if ( g_beCloudTest.selectSPEAKER(pGRP,pGR_INIT)==false ){
			g_beCloudTest.UnitID = OB->Index;
			g_beCloudTest.UnitUniqID = Group[g_beCloudTest.UnitID]->UniqID;
		};
	}else{
		g_beCloudTest.hideAll();
		g_beCloudTest.UnitID=-1;
		g_beCloudTest.UnitUniqID=0xFFFFFFFF;
		return;
	};

	// «акрыть магазин если приходит сообщение
	void ExitMagazine(byte NI);
	ExitMagazine(OB->NNUM);

	g_beCloudTest.hideAll();
	g_beCloudTest.CloudTypeID = g_beCloudTest.getProperCloudType(pSTR->Text.pchar());
	g_beCloudTest.setRandCloud(g_beCloudTest.CloudTypeID);
	g_beCloudTest.centreTextY(g_beCloudTest.CloudTypeID);
	g_beCloudTest.centreTextX(g_beCloudTest.CloudTypeID);
	char sprt[256];
	sprintf(sprt,"%s_%s",pSTR->Speaker.pchar(),pSTR->Spirit.pchar());
	g_beCloudTest.setProperPictureOBJ(g_beCloudTest.CloudTypeID,sprt);//pSTR->Spirit.pchar());
	g_beCloudTest.vievCloudType(g_beCloudTest.CloudTypeID);
};
void	be_gShowCloudDialog(be_CSTR_DIALOG* pSTR,be_CGROUP* pGR_INIT){
	if ( pSTR==NULL ){
		g_beCloudTest.hideAll();
		g_beCloudTest.UnitID=-1;
		g_beCloudTest.UnitUniqID=0xFFFFFFFF;
		return;
	};
	be_CGROUP* pGRP = (be_CGROUP*)(pSTR->Group.Get());
	be_gShowCloudDialog(pSTR,pGRP,pGR_INIT);
};
int		be_gGetLineCountInCloudDialog(be_CSTR_DIALOG* pSTR){
	int cloudID = g_beCloudTest.getProperCloudType(pSTR->Text.pchar());
	if (cloudID<3){
		return cloudID+1;
	};
	return cloudID+2;
};
bool	be_gGetCloudDilogCancel(){
	return be_CCloud::NeedCancel;	
};
void	be_gSetCloudDilogCancel(bool state){
	be_CCloud::NeedCancel = state;
};
bool	be_gGetCloudDilogCenter(){
	return be_CCloud::NeedCenter;	
};
void	be_gSetCloudDilogCenter(bool state){
	be_CCloud::NeedCenter = state;
};
void	be_gClearCloudDilogSPEAKERS(){
	g_beCloudTest.pSPEAKERS.DynArray<be_CGROUP*>::Clear();
};
void	be_gAddCloudDilogSPEAKERS(be_CGROUP* pSP){
	if ( pSP==NULL ) return;
	int N = g_beCloudTest.pSPEAKERS.GetAmount();
	bool PRESENT = false;
	while ( !PRESENT && N-- ) {
		PRESENT = (bool)( pSP->Name == g_beCloudTest.pSPEAKERS[N]->Name );
	};
	if ( PRESENT==false ){
		g_beCloudTest.pSPEAKERS.Add(pSP);
	};
};
bool	be_getCenterOfSPEAKERS(int& x,int& y,be_CGROUP* pGR){
	return g_beCloudTest.getCenterOfSPEAKERS(x,y,pGR);
};
bool	be_getCurrentSpeakerPosition(int& x,int& y){
	return 	g_beCloudTest.getCurrentSpeakerPosition(x,y);
};
//////////////////////////////////////////////////////////////////////////
// Camera
void	be_gDRAW_CAMERA(Vector3D& pos, Vector3D& dir, Vector3D& lookAt){
	void DrawArrow(const Vector3D&,const Vector3D&,DWORD,float,float,float);
	//DrawArrow(pos,dir,0xFFffffff,1.0f,0.1f,0.05f);
	Vector3D visiblePOS = pos;
	visiblePOS -= lookAt;
	visiblePOS /= 8.f;
	visiblePOS += lookAt;
	DrawCircle( lookAt,Vector3D(0.f,0.f,1.f),300.f,0x77ffff00,0xFF0000ff,50);
//	DrawCube( visiblePOS, 0xFF0000ff, 50.f );
	GPS.DrawLine(visiblePOS,lookAt,0xFF0000ff);

	// Draw 3D model CAMERA
	static int mID	= IMM->GetModelID("camera");
	static int tID	= IRS->GetTextureID("concrete.jpg");
	static int shID	= IRS->GetShaderID("wire");


	IRS->SetShader( shID );
	IRS->SetTexture( tID, 0 );

	Vector3D	oX( Vector3D::oX );
	Vector3D	oZ( Vector3D::oY );
	Vector3D	oY( dir );

	oY.orthonormalize( oX, oZ );

	Matrix3D	rot;

	oY.reverse(oY);
	oZ.reverse(oZ);
	rot = Matrix3D( oX, oY, oZ );
		
	Matrix4D	tm( Vector3D(0.25f,0.25f,0.25f), rot, visiblePOS );
	
	IMM->StartModel( mID, tm );
    IMM->DrawModel();

	return;
};

// Info Dialog
//CInfoDialog		g_beInfoDialog;
//CInfoDialog::CInfoDialog(){
//	Visible = true;
//	Empty	= true;
//	for ( int i=0; i<5; i++ ){
//		(*this)[i] = "";
//	};
//	InitDS = true;
//	HideAnyWay = false;
//};
//bool	CInfoDialog::LoadDefDS(){
//	xmlQuote	xmlDS;
//	xmlDS.ReadFromFile("Missions\\GRAPHICS\\Dialogs\\Tasks.DialogsSystem.xml");
//	ErrorPager	Err;
//	return ds_MAIN.Load(xmlDS,&ds_MAIN,&Err);
//};
//void	CInfoDialog::SetMessage( char* _message, int _pos ){
//	if ( 0<=_pos && _pos<5 ){
//		if ( g_beCheckChar(_message) )	(*this)[_pos] = _message;
//		else							(*this)[_pos] = "";
//		if (!HideAnyWay) Visible = true;
//	};
//};
//bool	CInfoDialog::SetMessageInDS( char* _message, int _pos ){
//	if(ds_MAIN.DSS.GetAmount()){
//		TextButton* pTB = dynamic_cast<TextButton*>(ds_MAIN.DSS[0]->Find(_pos));
//		if ( pTB!=NULL ){
//			pTB->SetMessage(_message);
//			return true;
//		};
//	}
//	return false;
//};
//void	CInfoDialog::Init(){
//	if (InitDS){
//		InitDS = false;
//		LoadDefDS();
//		for ( int i=0; i<5; i++ ){
//			SetMessageInDS( (*this)[i].pchar(), i );
//		};
//	};	
//};
//void	CInfoDialog::Update(){
//	Init();
//	Empty = true;
//
//	for ( int i=0; i<5; i++ ){
//		Empty = Empty && ( ((*this)[i]).pchar()[0] == 0 );
//	};
//	if ( !Empty ){
//		for ( int i=0; i<5; i++ ){
//			SetMessageInDS( (*this)[i].pchar(), i );
//		};
//		int maxW = getMaxWidth()+5+5;
//		DialogsDesk* pDS = dynamic_cast<DialogsDesk*>(ds_MAIN.DSS[0]);
//		pDS->SetWidth(maxW);
//		pDS->SetHeight(getMaxHeight());
//		TextButton* pTB = dynamic_cast<TextButton*>(ds_MAIN.DSS[0]->Find(9));
//		pTB->Setx( maxW/2 - ( pTB->x1 - pTB->x )/2 );
//	};
//};
//void	CInfoDialog::Render(){
//	if ( HideAnyWay ){
//		ds_MAIN.Visible = false;
//		return;
//	}
//	Update();
//	if ( !Empty && Visible ){
//		ds_MAIN.ProcessDialogs();
//	};
//};
//_str&	CInfoDialog::operator[](int _ixd){
//	switch(_ixd) {
//	case 0:
//		return Message0;
//		break;
//	case 1:
//		return Message1;
//		break;
//	case 2:
//		return Message2;
//		break;
//	case 3:
//		return Message3;
//		break;
//	case 4:
//		return Message4;
//		break;
//	};
//	return Message0;
//};
//int		CInfoDialog::getMaxWidth(){
//	TextButton* pTB = dynamic_cast<TextButton*>(ds_MAIN.DSS[0]->Find(9));	
//	int maxW = pTB->x1 - pTB->x;
//	for ( int i=0; i<5; i++ ){
//		pTB = dynamic_cast<TextButton*>(ds_MAIN.DSS[0]->Find(i));
//		if ( pTB->x1 - pTB->x > maxW ) maxW = pTB->x1 - pTB->x;
//	};
//	return maxW;
//};
//int		CInfoDialog::getMaxHeight(){
//	TextButton* pTB = dynamic_cast<TextButton*>(ds_MAIN.DSS[0]->Find(9));
//	int maxH = pTB->y1 - pTB->y + 5 + 5;
//	for ( int i=0; i<5; i++ ){
//		if ( ((*this)[i]).pchar()[0] != 0 ){
//			pTB = dynamic_cast<TextButton*>(ds_MAIN.DSS[0]->Find(i));
//			maxH += pTB->y1 - pTB->y + 5;
//		};
//	};
//	return maxH;
//};

CInfoDialog		g_beInfoDialog( "TaskInfo" );

void	CInfoDialog::init(char* _name/*=NULL*/)
{
	Name = ( (_name!=NULL&&_name[0]!=0) ? (_name) : ("TaskInfoExemple") );
	Visible = false;

	_error = false;
	_init  = true;
} // CInfoDialog::init

CInfoDialog::CInfoDialog()
{
	init();
} // CInfoDialog::CInfoDialog

CInfoDialog::CInfoDialog(char* _name)
{
	init(_name);
	GetObjectGlobalID();
} // CInfoDialog::CInfoDialog

void	CInfoDialog::clear()
{
	m_Tasks.Clear();
} // CInfoDialog::clear

CInfoDialog::~CInfoDialog()
{
	clear();
} // CInfoDialog::~CInfoDialog

void	CInfoDialog::AddTask(_str& l_name,_str& l_title,_str& l_descr,_str& l_task)
{
	CTaskInfo* pNewTask = NULL;
	for ( int i=0; i<m_Tasks.GetAmount(); i++ ){
		if ( m_Tasks[i]->m_Name == l_name ){
			pNewTask = m_Tasks[i];
		}
	}
	bool	_newTask = (bool)(pNewTask==NULL);
	if ( _newTask ) pNewTask = new CTaskInfo;
	pNewTask->m_Title	= l_title;
	pNewTask->m_Descr	= l_descr;
	pNewTask->m_Task	= l_task;
	pNewTask->m_Name	= l_name;
	if (_newTask) m_Tasks.Add( pNewTask );
	pNewTask = NULL;

} // CInfoDialog::AddTask

void	CInfoDialog::DelTask(_str& l_name)
{
	int TN = m_Tasks.GetAmount();
    while ( TN-- ){
		if ( m_Tasks[TN]->m_Name == l_name ){
			m_Tasks.DelElement(TN);
		}
	}
} // CInfoDialog::DelTask

bool	CInfoDialog::Process	(
									int mx,
									int my,
									bool& LeftPressed,
									bool& RightPressed,
									int MapCoordX,
									int MapCoordY,
									bool OverMiniMap
								)
{
	OnRender();	
	return false;
} // CInfoDialog::Process

bool	CInfoDialog::OnRender()
{
	if ( _error ) return false;
	if ( _init ) OnInit( "Missions\\GRAPHICS\\Dialogs\\Tasks_01.DialogsSystem.xml" );
	if ( !_error ){
		_mainDS.ProcessDialogs();
	};	
} // CInfoDialog::OnRender

void	CInfoDialog::OnInit(char* fileName)
{
	_error = !loadDS(fileName);
	if ( !_error ){
		_init = false;
	};
} // CInfoDialog::OnInit

bool	CInfoDialog::loadDS(char* fileName)
{
	_mainDS.reset_class(&_mainDS);
	if ( g_beCheckChar(fileName)==false ) return false;
	return	_mainDS.ReadFromFile(fileName);
} // CInfoDialog::loadDS

//////////////////////////////////////////////////////////////////////////
void	be_REG_CLASS_MissGraph(){
	REG_CLASS(be_CCloud);	
	REG_CLASS(CTaskInfo);
	REG_CLASS(CInfoDialog);
};

































