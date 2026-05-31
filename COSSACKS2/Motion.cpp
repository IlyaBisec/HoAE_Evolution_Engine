#include "stdheader.h"

extern MotionField UnitsField;
typedef bool PhysicsFn(OneObject* OB);
extern short TSin[257];
extern short TCos[257];
word GetDir(int dx,int dy);
struct STRIKE_param{
	DWORD NMask;
	char StrikeDir;
};
bool STRIKE_Callback(OneObject* OB,void* param){
	STRIKE_param* SP=(STRIKE_param*)param;
	if(!(SP->NMask&OB->NMask)){
		char dd=OB->RealDir-SP->StrikeDir;
		int d=abs(abs(int(dd))-64);
		if(d<20){
			OB->Die();
			SP->NMask=0xFF;
		}
	}
	return true;
}
typedef bool tpUnitsCallback(OneObject* OB,void* param);
int PerformActionOverUnitsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
void DetectStrikenEnemy(OneObject* OBJ){
	int D=OBJ->Lx*8+140;
	int x1=OBJ->RealX/16+D*TCos[OBJ->RealDir]/256;
	int y1=OBJ->RealY/16+D*TSin[OBJ->RealDir]/256;
	STRIKE_param SP;
	SP.NMask=OBJ->NMask;
	SP.StrikeDir=OBJ->RealDir;	
	PerformActionOverUnitsInRadius(x1,y1,80,&STRIKE_Callback,&SP);
}
bool CalculateMotion2(OneObject* OB){
	NewMonster* NM=OB->newMons;
	int MaxV=NM->MotionDist;
	GeneralObject* GO=OB->Ref.General;
	AdvCharacter* ADC=GO->MoreCharacter;
	MaxV=(MaxV*ADC->Rate[0])>>(4-SpeedSh);
	int MinV=MaxV>>1;
	if(NM->NVesel)MaxV+=16;
    int DestX=OB->DestX;
    int DestY=OB->DestY;
    bool UseDestDir=false;
    if(OB->DestDirectionIsActive){
        UseDestDir=true;
        DestX=OB->RealX;
        DestY=OB->RealY;
        OB->DestDirectionIsActive=false;
    }
	if(DestX>0){
		int DIS=Norma(DestX-OB->RealX,DestY-OB->RealY);
		if(DIS>32*16 || UseDestDir){
            if(DIS>32*16){
			    if(OB->Speed<MaxV){
				    OB->Speed++;
			    }else OB->Speed=MaxV;
            }
		    char DIR1;
            if(DIS>32*16)DIR1=char(GetDir(DestX-OB->RealX,DestY-OB->RealY));
            else DIR1=OB->DestDirection;
			char ddir=DIR1-OB->RealDir;
			if(abs(ddir)<2 && DIS>0){
				OB->RealDir=DIR1;
                OB->RealDirPrecise=int(OB->RealDir)<<8;
				int spf=(OB->Speed*GameSpeed)>>8;
				OB->RealVx=((DestX-OB->RealX)*spf/DIS);
				OB->RealVy=((DestY-OB->RealY)*spf/DIS);
				OB->GraphDir=OB->RealDir;
				if(NM->KineticLimit){
					OB->KineticPower+=GameSpeed;
					if(OB->KineticPower>=NM->KineticLimit){
						OB->KineticPower=NM->KineticLimit;
						DetectStrikenEnemy(OB);
					}
				}				
				return true;
			}else{
                if(DIS==0)DIS=64*16;
				OB->KineticPower=0;
				if(DIS>4000)DIS=4000;
				int DR=((GameSpeed*OB->newMons->MinRotator)>>5)*(3000+DIS)/(300+DIS);
				int dd20=max(abs(ddir),20);
				int dd16=max(abs(ddir),16);
				DR=DR*dd20/20;
				if(OB->LockType==1){
					if(DR>450)DR=450;
				}else{
					if(DR>768)DR=768;
				}
				int sp1=MaxV*(100+DIS)/(1000+DIS);
				sp1=sp1*16/dd16;
				if(sp1<OB->Speed)OB->Speed-=2;
				int spf=(OB->Speed*GameSpeed)>>8;
				if(ddir>0){
					OB->RealDirPrecise+=DR;
				}else{ 
					OB->RealDirPrecise-=DR;
				};
				OB->RealDir=OB->RealDirPrecise>>8;
				OB->RealVx=(spf*TCos[OB->RealDir])>>8;
				OB->RealVy=(spf*TSin[OB->RealDir])>>8;
			};
		}else{
			OB->DestX=-1;
			OB->Speed=0;
		};
		OB->GraphDir=OB->RealDir;
		
	}else{
		OB->KineticPower=0;
		int V=OB->Speed;
		if(V<=8){
			OB->RealVx=0;
			OB->RealVy=0;
			OB->Speed=0;
		}else{
			OB->RealVx-=OB->RealVx>>2;
			OB->RealVy-=OB->RealVy>>2;
			OB->Speed-=OB->Speed>>2;
		};
	};
	
	return false;
};

//				   0   1   2   3   4   5   6   7   0   1
const int DX8[10]={0  ,0  ,1  ,0  ,0  ,-1 ,-1 ,-1 ,0  ,0};
const int DY8[10]={-1 ,-1 ,0  ,0  ,1  ,0  ,0  ,-1 ,-1 ,-1};
const int DLX[10]={0  ,1  ,0  ,1  ,0  ,1  ,0  ,1  ,0  ,1};
const int DLY[10]={0  ,1  ,0  ,1  ,0  ,1  ,0  ,1  ,0  ,1};
bool TryDir(OneObject* OB,char Dir){
	Dir=(Dir+8)&7;
	int ULX=int(OB->Lx)<<7;
	int NewX=((OB->RealX-ULX)>>8)+DX8[Dir];
	int NewY=((OB->RealY-ULX)>>8)+DY8[Dir];
	MotionField* MFI=MFIELDS+OB->LockType;
	return !(MFI->CheckBar(NewX,NewY,OB->Lx+(Dir&1),OB->Lx+(Dir&1))||
		UnitsField.CheckBar(NewX,NewY,OB->Lx+(Dir&1),OB->Lx+(Dir&1)));
};

void MoveObject(OneObject* OB){
	int vx=OB->RealVx;
	int vy=OB->RealVy;
	OB->RealX+=vx;
	OB->RealY+=vy;	
	addrand(OB->RealX);
	addrand(OB->RealY);
};
void StopObject(OneObject* OB){
	//TryToStand(OB,false);
	OB->Speed=0;
	OB->RealVx=0;
	OB->RealVy=0; 
	//OB->StandTime++;
};
void EraseLodkiInCell(int cell,int x,int y,int Lx,int Ly){
	cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(!NMon)return;
	int ofs1=cell<<SHFCELL;
	word MID;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->newMons->Usage==FisherID){
				int mx=x+x+Lx;
				int my=y+y+Ly;
				int eLx=OB->Lx;
				int ex=(OB->x<<1)+eLx;
				int ey=(OB->y<<1)+eLx;
				int dx=abs(mx-ex);
				int dy=abs(my-ey);
				int r=dx<dy?dy:dx;
				if(r<Lx+eLx)
					OB->Die();
			};
		};
	};
};
int GetShipDangerInCell(int cell,int x,int y){
	int DNG=0;
	cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(!NMon)return 0;
	int ofs1=cell<<SHFCELL;
	word MID;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->LockType&&OB->newMons->Usage!=FisherID){
				int rx=(OB->RealX>>4)-x;
				int ry=(OB->RealY>>4)-y;
				int vx=OB->RealVx<<2;
				int vy=OB->RealVy<<2;
				int r0=Norma(rx,ry);
				int VX=vx;int VY=vy;
				for(int p=0;p<10;p++){
					int r=Norma(rx+VX,ry+VY);
					if(r<r0)r0=r;
					VX+=vx;
					VY+=vy;
				};
				if(r0<80)r0=80;
				DNG+=10000/r0;
			};
		};
	};
	return DNG;
};
int GetShipDanger(int x,int y){
	int x0=x>>7;
	int y0=y>>7;
	int maxx=msx>>2;
	int maxy=msy>>2;
	int DNG=0;
	for(int r=0;r<5;r++){
		char* xi=Rarr[r].xi;
		char* yi=Rarr[r].yi;
		int N=Rarr[r].N;
		for(int i=0;i<N;i++){
			int xx=x0+xi[i];
			int yy=y0+yi[i];
			if(xx>=0&&yy>=0&&xx<maxx&&yy<maxy){
				DNG+=GetShipDangerInCell(xx+(yy<<VAL_SHFCX),x,y);
			};
		};
	};
	return DNG;
};
int GetShipDanger1(int x,int y){
	int x0=x>>7;
	int y0=y>>7;
	int maxx=msx>>2;
	int maxy=msy>>2;
	int DNG=0;
	for(int r=0;r<7;r++){
		char* xi=Rarr[r].xi;
		char* yi=Rarr[r].yi;
		int N=Rarr[r].N;
		for(int i=0;i<N;i++){
			int xx=x0+xi[i];
			int yy=y0+yi[i];
			if(xx>=0&&yy>=0&&xx<maxx&&yy<maxy){
				DNG+=GetShipDangerInCell(xx+(yy<<VAL_SHFCX),x,y);
			};
		};
	};
	return DNG;
};
void EraseLodki(int x,int y,int Lx,int Ly){
	int x0=x>>3;
	int y0=y>>3;
	int maxx=msx>>1;
	int maxy=msy>>1;
	for(int r=0;r<3;r++){
		char* xi=Rarr[r].xi;
		char* yi=Rarr[r].yi;
		int N=Rarr[r].N;
		for(int i=0;i<N;i++){
			int xx=x0+xi[i];
			int yy=y0+yi[i];
			if(xx>=0&&yy>=0&&xx<maxx&&yy<maxy){
				EraseLodkiInCell(xx+(yy<<VAL_SHFCX),x,y,Lx,Ly);
			};
		};
	};
};
void AddTrace(int x,int y,byte Dir);
void CreateTrace(OneObject* OB);
bool RemoveShipsInCell(int cell,int x,int y,int Lx,OneObject* MyObj){
	cell+=VAL_MAXCX+1;
	if(cell<0||cell>=VAL_MAXCX*MAXCY)return false;
	word MyMid=MyObj->Index;
	int mx1=x+Lx-1;
	int my1=y+Lx-1;
    int NMon=MCount[cell];
	if(!NMon)return false;
	int ofs1=cell<<SHFCELL;
	word MID;
	int cx=(x<<1)+Lx;
	int cy=(y<<1)+Lx;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&OB->Index!=MyMid&&OB->LockType&&!(OB->Transport&&OB->DstX>0)){
				int olx=OB->Lx;
				int ox=((OB->x)<<1)+olx;
				int oy=((OB->y)<<1)+olx;
				int dx=abs(ox-cx);
				int dy=abs(oy-cy);
				if(!(dx>=(olx+Lx)||dy>=(olx+Lx))){
					//intersection
					if(OB->Speed<(100<<SpeedSh)){
						OB->Speed=(100<<SpeedSh);
						OB->RealVx=(OB->Speed*TCos[OB->RealDir])>>8;
						OB->RealVy=(OB->Speed*TSin[OB->RealDir])>>8;
					};
				};
			};
		};
	};
	return false;
};
void TryToPushShipsBy(OneObject* MyMid,int x,int y,int Lx){
	int cell=((x+6)>>3)+(((y+6)>>3)<<VAL_SHFCX);
	byte D=MyMid->RealDir;
	int DX=0;
	int DY=0;
	if(D==0)x++;
	else if(D==64)DY=1;
	else if(D==128)DX=-1;
	else if(D==192)DY=-1;
	else if(D>0&&D<64){DX=1;DY=1;}
	else if(D>64&&D<128){DX=-1;DY=1;}
	else if(D>128&&D<192){DX=-1;DY=-1;}
	else {DX=1;DY=-1;};
	if(DX){
		for(int dx=-2;dx<=2;dx++){
			for(int dy=-2;dy<=2;dy++){
				RemoveShipsInCell(cell+dx+dy*VAL_MAXCX,x+DX,y,Lx,MyMid);
			};
		};
	};
	if(DY){
		for(int dx=-2;dx<=2;dx++){
			for(int dy=-2;dy<=2;dy++){
				RemoveShipsInCell(cell+dx+dy*VAL_MAXCX,x,y+DY,Lx,MyMid);
			};
		};
	};
};
struct GULNP_param{
	int x;
	int y;
	word* buf;
	int max;
	DWORD Lock;
};
bool GULNP_Callback(OneObject* OB,void* param){
	GULNP_param* GP=(GULNP_param*)param;
	if(GP->max&&OB&&(!OB->Sdoxlo)&&(1<<OB->LockType)&GP->Lock&&Norma(OB->RealX/16-GP->x,OB->RealY/16-GP->y)>1){
        GP->max--;
		GP->buf[0]=OB->Index;
		GP->buf++;
		return true;
	}
	return false;
}
typedef bool tpUnitsCallback(OneObject* OB,void* param);
int PerformActionOverUnitsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
int GetUnitsListNearPoint(int x,int y,int R,DWORD LockMask,word* Buf,int MaxSize){
	GULNP_param GP;
	GP.buf=Buf;
	GP.max=MaxSize;
	GP.Lock=LockMask;
	GP.x=x;
	GP.y=y;
    return PerformActionOverUnitsInRadius(x,y,R,GULNP_Callback,&GP);
}
#define ExtraR 12
float sq_norma(float x,float y);
bool TestCollision(int Lx,int x,int y,word* UList,int NU){
    int R=Lx*8+ExtraR;
	for(int j=0;j<NU;j++){
		OneObject* OB=Group[UList[j]];
		int R1=int(OB->Lx)*8+ExtraR;
		if(R+R1>sq_norma(OB->RealX/16-x,OB->RealY/16-y))
			return false;
	}    	
	return true;
}
void DrawCollision(OneObject* OB,int R=0){
	extern byte LockGrid;
	if(OB/*&&OB->LockType==LockGrid-1*/){
        //int R=OB->Lx*8+ExtraR;
		int px=-1;
		int py=-1;
		for(int A=0;A<=32;A++){
            int X=OB->RealX/16+R*TCos[(A*8)&255]/256;
			int Y=OB->RealY/16+R*TSin[(A*8)&255]/256;
			if(px!=-1){
				void DrawWorldLine(float x0,float y0,float x1,float y1,DWORD Diffuse);
				DrawWorldLine(X,Y,px,py,0xFF00FFFF);
			}
			px=X;
			py=Y;
		}
	}
}
_inline bool TestMotionPossibilityTo(MotionField* MFI,int x,int y,int LX,word* Ulist,int NU){
	int ULX=(LX)<<7;
	int LockX=(x-ULX)>>8;
	int LockY=(y-ULX)>>8;
	return !(MFI->CheckRound(LockX,LockY,LX)||!TestCollision(LX,x/16,y/16,Ulist,NU));
}
bool TryToMoveUnitTo(OneObject* OB,int dx,int dy,int Norm,word* UList,int NU){
    int LockX,LockY;
	int cdx=dx;
	int cdy=dy;
	if(Norm){
        int n=Norma(dx,dy);
		if(n){
			cdx=dx*Norm/16;
			cdy=dy*Norm/16;
		}
	}
	if(TestMotionPossibilityTo(MFIELDS+OB->LockType,OB->RealX+cdx,OB->RealY+cdy,OB->Lx,UList,NU)||OB->UnlimitedMotion){		
		int ULX=int(OB->Lx)<<7;
		LockX=(OB->RealX+dx-ULX)>>8;
		LockY=(OB->RealY+dy-ULX)>>8;
        OB->x=LockX;
		OB->y=LockY;
		OB->RealX+=dx;
		OB->RealY+=dy;
		addrand(OB->RealX);
		addrand(OB->RealY);
		return true;
	}
	return false;
}
void RotateVec(int& x,int& y,int x0,int y0,byte angl){
    int COS=TCos[angl];
	int SIN=TSin[angl];
	x=(x0*COS-y0*SIN)/256;
	y=(x0*SIN+y0*COS)/256;
}
void MotionHandlerOfNewSheeps(OneObject* OB){
	bool FirstTime=true;
	bool CanCrashLimit=false;
	bool LODKA=OB->newMons->Usage==FisherID;
	if(OB->Sdoxlo)return;

    OB->UsesDestDidection=true;

	word OLDPDIR=OB->RealDirPrecise;

	if(OB->FrameFinished)OB->SetZeroFrame();

	NewMonster* NMN=OB->newMons;

	CalculateMotion2(OB);

	MotionField* MFI=MFIELDS+OB->LockType;
	int LX=OB->Lx;
	
	if(OB->Speed){
		if(OB->GLock)MFI->ClearRound(OB->x,OB->y,LX);
		UnitsField.ClearRound(OB->x,OB->y,LX);
		int NSp=NMN->GetAnimation(anm_MotionL)->NFrames;
		int DX=OB->RealVx;
		int DY=OB->RealVy;
		int dx0=DX;
		int dy0=DY;
		int success=1;
		int nrm=17*16;
		word BUF[64];
		int NU=OB->LockType==1?GetUnitsListNearPoint(OB->RealX/16,OB->RealY/16,360,1<<OB->LockType,BUF,64):0;
		if(!TryToMoveUnitTo(OB,DX,DY,0,BUF,NU)){			
			RotateVec(DX,DY,dx0,dy0,8);
			if(!TryToMoveUnitTo(OB,DX,DY,nrm,BUF,NU)){
				RotateVec(DX,DY,dx0,dy0,-8);
				if(!TryToMoveUnitTo(OB,DX,DY,nrm,BUF,NU)){
					if(rando()<1024){
						OB->DeletePath();
						OB->DestX=-1;
						OB->DestY=-1;
						OB->CPdestX=-1;
					}
					success=2;
					RotateVec(DX,DY,dx0,dy0,16);
					if(!TryToMoveUnitTo(OB,DX,DY,nrm,BUF,NU)){
						RotateVec(DX,DY,dx0,dy0,-16);
						if(!TryToMoveUnitTo(OB,DX,DY,nrm,BUF,NU)){							
							RotateVec(DX,DY,dx0,dy0,32);
							if(!TryToMoveUnitTo(OB,DX/2,DY/2,nrm,BUF,NU)){
								RotateVec(DX,DY,dx0,dy0,-32);
								if(!TryToMoveUnitTo(OB,DX/2,DY/2,nrm,BUF,NU)){
									RotateVec(DX,DY,dx0,dy0,64);
									if(!TryToMoveUnitTo(OB,DX/4,DY/4,nrm,BUF,NU)){
										RotateVec(DX,DY,dx0,dy0,-64);
										if(!TryToMoveUnitTo(OB,DX/4,DY/4,nrm,BUF,NU)){
											success=false;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		/*if(success==2){
			OB->DestX=-1;
			OB->DeletePath();
			OB->CPdestX=-1;
			OB->CPdestY=-1;
		}*/		
		if(success){							
			OB->GLock=false;							
			OB->StandTime=0;
			OB->SetNextFrame();
		}else{
			OB->GLock=true;
			MFI->SetRound(OB->x,OB->y,LX);
		}
		//OB->Speed=Sp;
		if(!OB->UnlimitedMotion)UnitsField.SetRound(OB->x,OB->y,LX);
	}else{
		OB->StandTime++;
		if(!OB->GLock){
			OB->GLock=true;
			MFI->SetRound(OB->x,OB->y,LX);
			if(!OB->UnlimitedMotion)UnitsField.SetRound(OB->x,OB->y,LX);
		}
	}
	OB->RZ=OB->LockType==1?GetHeight(OB->RealX/16,OB->RealY/16):GetTotalHeight(OB->RealX/16,OB->RealY/16);
	if(OB->RZ<0)OB->RZ=0;
	OB->Phase+=short(OB->RealDirPrecise-OLDPDIR);
	//OB->SetNextFrame();
	if(OB->FrameFinished){
		if(NMN->InviteMask){
			bool CheckLeaveContainerAbility(OneObject* OB);
			if(OB->DestX>0 || !CheckLeaveContainerAbility(OB)){
				if(OB->NewState!=0){
					OB->NewState=0;
					OB->LocalNewState=1;
					TryToStand(OB,false);
					return;
				}
			}else{
				if(OB->NewState!=1){
					OB->NewState=1;
					OB->LocalNewState=0;
					TryToStand(OB,false);
					return;
				}                        
			}
		}
		if(OB->NewState)OB->NewAnm=NMN->GetAnimation(anm_PStand+OB->NewState-1);
		else OB->NewAnm=NMN->GetAnimation(anm_Stand);
		OB->SetZeroFrame();
	};
	//if(OB->NewAnm->Code!=anm_Stand&&OB->Speed==0){
		//OB->SetNextFrame();
	//}
};
