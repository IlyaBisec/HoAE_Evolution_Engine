#include "stdheader.h"
#include "GameExtension.h"
#include "BoidsExtension.h"

extern int tmtmt;
void FillNeighboringList();
void CalculatePushForce();
extern DynArray<word> NeighboringUnits;
extern DynArray<int> UnitsCoordAndPushForce;
extern EngineSettings EngSettings;
void BoidsExtension::ProcessingGame()
{
	if(MAXOBJECT<EngSettings.BoidsOffLimit)
	{
		if(tmtmt%10==0)
			FillNeighboringList();
#ifdef NOBOIDS
		CalculatePushForce();
#endif //NOBOIDS
	}
}
void BoidsExtension::OnGameStart()
{
	NeighboringUnits.Clear();
	UnitsCoordAndPushForce.Clear();
}
const int MotionDistAfterCollision = 1024;
int CollisionPushCoef = 8;
void ChangeUnitCoor(OneObject* OB,int newX,int newY);
bool CheckMovingInDirectionPossibility(int LockType,int x0,int y0,int x1,int y1){
	int rx=(x0>>8)-1;
	int ry=(y0>>8)-1;
	if(MFIELDS[LockType].CheckBar(rx,ry,3,3)){
        int dx=x1-x0;
		int dy=y1-y0;
        int N=Norma(dx,dy);
		if(N==0)return true;
		dx=dx*5/N/3;
		dy=dy*5/N/3;
		return !MFIELDS[LockType].CheckBar(rx+dx,ry+dy,3,3);
	}else return true;
}
void CalculateCollision(){
	extern bool GecOptimCollision;
	CollisionPushCoef=EngSettings.CollisionPushCoef;
	if(GecOptimCollision) return;
	//return;
    int N=NeighboringUnits.GetAmount();
	for(int i=0;i<N;i+=2){
		word ID1=NeighboringUnits[i];
		word ID2=NeighboringUnits[i+1];
		OneObject* OB1=Group[ID1];
		OneObject* OB2=Group[ID2];
		if( OB1 && OB2 && OB1->LockType==OB2->LockType ){
			int RX1=OB1->RealX;
			int RY1=OB1->RealY;
			int RX2=OB2->RealX;
			int RY2=OB2->RealY;

			if(RX1==RX2 && RY1==RY2){
				RX1+=(rando()&3)-1;
				RY1+=(rando()&3)-1;
				RX2+=(rando()&3)-1;
				RY2+=(rando()&3)-1;
			}

			int R1=OB1->CollisionRadius;
			int R2=OB2->CollisionRadius;	
            int Mass1=OB1->NoCollision?4:400;
            int Mass2=OB2->NoCollision?4:400;
            /*
			if(!(OB1->NMask & OB2->NMask)){
				if(OB1->DestX<=0 && OB2->DestX>0 ){
                    Mass2=80;
				}else
				if(OB2->DestX<=0 && OB1->DestX>0 ){
					Mass1=80;
				}				
			}
			*/

			int ed1=0;//(rando()&7)==0;
			int ed2=0;//(rando()&7)==0;

			if(OB1->DestX>0 && R1>15){
                int dx=OB1->DestX-RX1;
				int dy=OB1->DestY-RY1;
				int N=Norma(dx,dy);
				if(N==0){
					dx=rando()&15-7;
					dy=rando()&15-7;
					N=Norma(dx,dy);
				}
				if(N>0){
					//RX1+=dx*R1*12/N;
					//RY1+=dy*R1*12/N;
                    if(dx*(RY1-RY2)-dy*(RX1-RX2)<0)ed2=1;else ed2=-1;
				}
			}
			if(OB2->DestX>0 && R2>15){
				int dx=OB2->DestX-RX2;
				int dy=OB2->DestY-RY2;
				int N=Norma(dx,dy);
				if(N==0){
					dx=rando()&15-7;
					dy=rando()&15-7;
					N=Norma(dx,dy);
				}
				if(N>0){
					//RX2+=dx*R2*12/N;
					//RY2+=dy*R2*12/N;
					if(-dx*(RY1-RY2)+dy*(RX1-RX2)<0)ed1=1;else ed1=-1;
				}
			}
			int R=Norma(RX1-RX2,RY1-RY2);
			int Rn=R>>4;			
			int Rm=R1+R2;
			if(Rn<Rm+Rm){
				if(!( (OB1->NMask & OB2->NMask) || OB1->newMons->LowCollision ) ){
					if(OB1->DestX>0){
						int spx=OB1->DestX-OB1->RealX;
						int spy=OB1->DestY-OB1->RealY;
						int dux=OB2->RealX-OB1->RealX;
						int duy=OB2->RealY-OB1->RealY;
						if(spx*dux+spy*duy>0)OB1->CurUnitSpeed=6+OB1->CurUnitSpeed/2;
					}
					if(OB2->DestX>0){
						int spx=OB2->DestX-OB2->RealX;
						int spy=OB2->DestY-OB2->RealY;
						int dux=OB1->RealX-OB2->RealX;
						int duy=OB1->RealY-OB2->RealY;
						if(spx*dux+spy*duy>0)OB2->CurUnitSpeed=6+OB2->CurUnitSpeed/2;
					}
				}
			}
			if(Rn<Rm){
				int Rm2=R1*R1+R2*R2;
                //Rm<<=4;
                int DR=(Rm-Rn);
				//int DR=(int(OB1->GroupSpeed)+int(OB2->GroupSpeed))*GameSpeed/512;
                int DR1=DR*CollisionPushCoef*R2*R2*Mass1/Rm2/(Mass1+Mass2);
				int DR2=DR*CollisionPushCoef*R1*R1*Mass2/Rm2/(Mass1+Mass2);				
				int dx2=RX2-RX1;
				int dy2=RY2-RY1;
				int D=Norma(dx2,dy2);
				if(D>0){                    
					if(DR1>0){
						int dx1=dx2-ed1*dy2;
						int dy1=dy2+ed1*dx2;
						int D1=OB1->DestX;int D2=OB1->DestY;
						int _x1=OB1->RealX-DR1*dx1/D;
						int _y1=OB1->RealY-DR1*dy1/D;
						if(CheckMovingInDirectionPossibility(OB1->LockType,OB1->RealX,OB1->RealY,_x1,_y1)){							
							ChangeUnitCoor(OB1,_x1,_y1);
							OB1->DestX=D1;OB1->DestY=D2;
							if(R1>20){
								if(D1<=0){
									D=Norma(dx1,dy1);
									if(D>0 && !OB1->Attack){
										OB1->DestX=OB1->RealX-dx1*MotionDistAfterCollision/D;                                                        
										OB1->DestY=OB1->RealY-dy1*MotionDistAfterCollision/D;
										OB1->PathDelay=4;
									}
								}else{
									if(OB1->NewAnm->Code==anm_Stand){
										NewAnimation* R=OB1->newMons->Animations->Get(anm_Rest);
										if(R->Enabled){
											OB1->NewAnm=R;
											OB1->SetZeroFrame();
										}
									}
								}
							}
						}
					}
					if(DR2>0){
						int dx1=dx2-ed2*dy2;
						int dy1=dy2+ed2*dx2;
						int D1=OB2->DestX;int D2=OB2->DestY;
						int _x2=OB2->RealX+DR2*dx1/D;
						int _y2=OB2->RealY+DR2*dy1/D;
						if(CheckMovingInDirectionPossibility(OB2->LockType,OB2->RealX,OB2->RealY,_x2,_y2)){
							ChangeUnitCoor(OB2,_x2,_y2);
							OB2->DestX=D1;OB2->DestY=D2;
							if(R2>20){
								if(D1<=0 && !OB2->Attack){
									D=Norma(dx1,dy1);
									if(D>0){
										OB2->DestX=OB2->RealX+dx1*MotionDistAfterCollision/D;
										OB2->DestY=OB2->RealY+dy1*MotionDistAfterCollision/D;
										OB2->PathDelay=4;
									}
								}
							}else{
								if(OB2->NewAnm->Code==anm_Stand){
									NewAnimation* R=OB2->newMons->Animations->Get(anm_Rest);
									if(R->Enabled){
										OB2->NewAnm=R;
										OB2->SetZeroFrame();
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
void DrawCollision(OneObject* OB,int R);
bool DrawCollisionMode=false;
void DrawWorldLine(float x0,float y0,float x1,float y1,DWORD Diffuse);
void DrawCollision(){
	if(!DrawCollisionMode)return;
	int N=NeighboringUnits.GetAmount();
	for(int i=0;i<N;i+=2){
		word ID1=NeighboringUnits[i];
		word ID2=NeighboringUnits[i+1];
		OneObject* OB1=Group[ID1];
		OneObject* OB2=Group[ID2];
		if( OB1 && OB2 && (OB1->ImSelected || OB2->ImSelected) ){
			DrawCollision(OB1,OB1->CollisionRadius);
			if(OB1->ImSelected)DrawCollision(OB1,OB1->CollisionRadius-1);
			DrawCollision(OB2,OB2->newMons->CollisionRadius);
			if(OB2->ImSelected)DrawCollision(OB2,OB2->CollisionRadius-1);
		}
	}
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB && !OB->Sdoxlo){
			int dx=TCos[OB->RealDir]>>2;
			int dy=TSin[OB->RealDir]>>2;
			DrawWorldLine(OB->RealX/16,OB->RealY/16,OB->RealX/16+dx,OB->RealY/16+dy,0xFFFFFFFF);
			DrawCollision(OB,OB->CollisionRadius);
		}
	}
}
void DrawExitPath(OneObject* OB){
	NewMonster* NM=OB->newMons;
	if(NM->BornPtX.GetAmount()){
		int x=OB->RealX>>4;
		int y=OB->RealY>>4;
		int z=OB->RZ+OB->OverEarth;
		for(int i=1;i<NM->BornPtX.GetAmount();i++){
			Vector3D V1(x+NM->BornPtX[i],y+NM->BornPtY[i],z+NM->BornPtZ[i]);
			Vector3D V2(x+NM->BornPtX[i-1],y+NM->BornPtY[i-1],z+NM->BornPtZ[i-1]);			
			Vector3D V3=V1;V3.z=z;
			Vector3D V4=V2;V4.z=z;
			rsEnableZ(true);
			GPS.DrawLine(V1,V2,0xFFFF0000);
			GPS.DrawLine(V1,V3,0x60FF4000);
			GPS.DrawFillRect(V1,V2,V3,V4,0x60FF4000,0x60FF4000,0x60FF4000,0x60FF4000);			
			DrawAABB(AABoundBox(V1,1,1,1),0xFFFF4000,0xFFFF4000);
			DrawAABB(AABoundBox(V2,1,1,1),0xFFFF4000,0xFFFF4000);
			rsFlush();			
			rsEnableZ(false);
			GPS.DrawLine(V1,V2,0x80FF0000);
			GPS.DrawLine(V1,V3,0x40FF4000);
			GPS.DrawFillRect(V1,V2,V3,V4,0x60FF4000,0x60FF4000,0x60FF4000,0x60FF4000);			
			DrawAABB(AABoundBox(V1,1,1,1),0x30FF4000,0x30FF4000);
			DrawAABB(AABoundBox(V2,1,1,1),0x30FF4000,0x30FF4000);
			rsFlush();						            
		}
	}
}
bool DrawExitPathMode=false;
void DrawSelUnitExitPath(){
	if(!DrawExitPathMode)return;
	itr_GetImSelected.Create(MyNation);
	while(OneObject* OB=itr_GetImSelected.Next()){
		DrawExitPath(OB);
	}
}