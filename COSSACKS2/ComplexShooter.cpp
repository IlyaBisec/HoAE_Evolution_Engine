#include "stdheader.h"
#include "unitability.h"
#include "AI_Scripts.h"
#include "WeaponSystem.h"
#include "BrigadeAI.h"
#include "cvi_singleplayerdata.h"
#include "cvi_Campaign.h"
#include "UnitAbilityIcon.h"
#include "ua_Item.h"
#include "ua_Modificators.h"
#include "ComplexShooter.h"
#include "TankController.h"

#define AblSpeed 1
void AddUnitToActiveList(OneObject* OB);
ActiveUnitAbility* ExtraShootingElement::GetNewActiveAbility(){
	ActiveUnitAbility* nAA=new ActiveExtraShootingElement;
	nAA->SetA(this);
	return nAA;
}
void AttackObjLink(OneObject* OBJ);
bool CheckPossibilityToAttackEnemy(OneObject* OB,OneObject* EOB);
bool ActiveExtraShootingElement::CheckPossibilityToAttackEnemy(OneObject* OB,OneObject* EOB){
	//—тарое условие
	//if( EOB==NULL || ( EOB->AI_Guard && !EOB->Attack ) ) return false;
	//ƒобавил указатель на индекс врага дл€ услови€
	OneObject* Enemy=Group[OB->EnemyID];
	//”словие работы - враг есть или это атакующий нейтрал, и на него навешен таргет атаки
	if( EOB==NULL || (( EOB->AI_Guard && !EOB->Attack )&&!(( OB->LocalOrder && OB->LocalOrder->DoLink==&AttackObjLink )&&EOB==Enemy) )) return false;
	if( !::CheckPossibilityToAttackEnemy(OB,EOB) ) return false;
	ExtraShootingElement* SE=(ExtraShootingElement*)GetA();
	int R=Norma(OB->RealX-EOB->RealX,OB->RealY-EOB->RealY)>>4;
    int _MaxDistance=SE->MaxDistance;
    //_MaxDistance+=_MaxDistance>>4;
    if(_MaxDistance==0){
        _MaxDistance=OB->MoreCharacter->AttackRadius2[0];
    }
	//checking distance
	if(R>SE->MinDistance && R<=_MaxDistance){
		//checking angles
		char DestDir=GetDir(EOB->RealX-OB->RealX,EOB->RealY-OB->RealY);
		char dDir=DestDir-OB->RealDir-SE->CentralShootAngle;
		if(abs(int(dDir))<SE->ShootAngleDifference || SE->AnyDirectionAllowed){
			return true;
		}
	}
	return false;
}

bool CheckAbilityToRotateInShortWay(char CentralDir,int Ddir,char CurDir,char DestDir){
	if(Ddir>=127)return true;
    char dc=CurDir-CentralDir;
	char dd=DestDir-CentralDir;
    char ddc=CurDir-DestDir;
    return (dd<=0 && dc<=0) || (dd>=0 && dc>=0) || (dd<=0 && dc>=0 && ddc>=0) || (dd>=0 && dc<=0 && ddc<=0);
}
extern bool PeaceMode;
extern bool NOPAUSE;
bool ActiveExtraShootingElement::Process(OneObject* OB){
	if(OB->Sdoxlo)return false;
	if(g_TankController.IsActive() && g_TankController.GetTank()==OB)return true;
    if( !NOPAUSE || OB->NewBuilding&&!OB->Ready )return true;
	InfluenceMask.set_bit(ABL_ExtraAnimation);
    AddUnitToActiveList(OB);
	OB->ActiveAbility->ActiveAbilities.InfluenceMask.set_bit(ABL_ExtraAnimation);
	ExtraShootingElement* SE=(ExtraShootingElement*)GetA();
	if(OB && OB->EnemyID!=0xFFFF){
		OneObject* EOB=Group[OB->EnemyID];
		if(EOB && EOB->Serial==OB->EnemySN){
			if(CheckPossibilityToAttackEnemy(OB,EOB)){
				EnemyID=OB->EnemyID;
				EnemySN=OB->EnemySN;
			}
		}
	}

    int _MaxDistance=SE->MaxDistance;
    int _Damage=SE->Damage;
    if(_MaxDistance==0){
        _MaxDistance=OB->MoreCharacter->AttackRadius2[0];
    }
    if(_Damage==0){
        _Damage=OB->MoreCharacter->MaxDamage[0];
    }

	if(EnemyID!=0xFFFF){
		if(LastShotTime>0)LastShotTime-=GameSpeed*AblSpeed;
		if(LastShotTime<0)LastShotTime=0;		
		OneObject* EOB=Group[EnemyID];
		if(EOB && EOB->Serial==EnemySN){
			if(CheckPossibilityToAttackEnemy(OB,EOB)){
				char DestDir=GetDir(EOB->RealX-OB->RealX,EOB->RealY-OB->RealY);
				char dDir=DestDir-OB->RealDir-(CurrentAngle>>8);
				if(ShootAnimationStage>0){
					int T=SE->TimeOfShooting*256/30;
					if(T>=ShootAnimationStage && T<ShootAnimationStage+GameSpeed*AblSpeed && SE->Weapon.GetObjectName()){
						//pubuxxx
						float Scale=OB->NewAnm->Scale;
						if(OB->ActiveAbility){
							word SC=OB->NewAnm->Scale*256;
							OB->ActiveAbility->ActiveAbilities.modifyScale(SC,SC);
							Scale=SC/256.0;
						}
						float COS=TCos[OB->RealDir]/256.0f;
						float SIN=TSin[OB->RealDir]/256.0f;
						float x=OB->RealX/16.0f+(SE->TowerCenterX*COS-SE->TowerCenterY*SIN)*Scale;
						float y=OB->RealY/16.0f+(SE->TowerCenterY*COS+SE->TowerCenterX*SIN)*Scale;
						COS=TCos[byte(OB->RealDir+(CurrentAngle>>8))]/256.0f;
						SIN=TSin[byte(OB->RealDir+(CurrentAngle>>8))]/256.0f;
						x+=SE->CannonRadius*COS*Scale;
						y+=SE->CannonRadius*SIN*Scale;
						float z=SE->TowerCenterZ*Scale+OB->RZ+OB->OverEarth;
						void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, int Damage, int AttType);
						CreateNewActiveWeapon((char*)SE->Weapon.GetObjectName(),OB->Index,x,y,z,EOB->Index,EOB->RealX/16,EOB->RealY/16,EOB->RZ+EOB->OverEarth,_Damage,0);
						LastShotTime=SE->TotalRechargeTime*256/30;
						LastShotTime=LastShotTime/OB->GetAttackSpeedBonus()*100;
					}
					ShootAnimationStage+=GameSpeed*AblSpeed;
					int TT=SE->WholeShootTime*256/30;
					if(ShootAnimationStage>TT){
						ShootAnimationStage=0;
					}
				}else{
					if(dDir==0 && LastShotTime==0){
						ShootAnimationStage=1;
					}else if(dDir){
						bool NormA=CheckAbilityToRotateInShortWay(SE->CentralShootAngle,SE->ShootAngleDifference,(CurrentAngle>>8),DestDir-OB->RealDir);
						if(!NormA)dDir=-dDir;
						if(dDir>0){
							CurrentAngle+=SE->RotationSpeed;
							dDir=DestDir-OB->RealDir-(CurrentAngle>>8);
							if(NormA && dDir<=0){
                                CurrentAngle=int(DestDir-OB->RealDir)<<8;
							}
						}else{
							CurrentAngle-=SE->RotationSpeed;
							dDir=DestDir-OB->RealDir-(CurrentAngle>>8);
							dDir=DestDir-OB->RealDir-(CurrentAngle>>8);
							if(NormA && dDir>=0){
								CurrentAngle=int(DestDir-OB->RealDir)<<8;
							}
						}
						if(abs(int(dDir)>8) && (rando()<4000) && !EOB->Attack){
                            EnemyID=0xFFFF;
						}
					}
				}
			}else EnemyID=0xFFFF;
		}else EnemyID=0xFFFF;
	}else{
        if(!PeaceMode){
            itr_UnitsInRadius.Create(OB->RealX>>4,OB->RealY>>4,_MaxDistance);
		    int minD=10000;
		    OneObject* bestE=NULL;
		    while(OneObject* EOB=itr_UnitsInRadius.Next()){
			    if(CheckPossibilityToAttackEnemy(OB,EOB)){
				    char DestDir=GetDir(EOB->RealX-OB->RealX,EOB->RealY-OB->RealY);
				    char dDir=DestDir-OB->RealDir-(CurrentAngle>>8);
				    int A=abs(int(dDir));
				    if(A<minD){
                        minD=A;
					    bestE=EOB;
				    }
			    }
		    }
		    if(bestE){
			    EnemyID=bestE->Index;
			    EnemySN=bestE->Serial;
		    }
        }
	}    
	CurrentAngle=(CurrentAngle+65536)&65535;
    return true;
}
void ActiveExtraShootingElement::modifyExtraAnimation(int Basic, int& Current){
	if(g_TankController.IsActive() && g_TankController.GetTank()==OB)return;
	ExtraShootingElement* SE=(ExtraShootingElement*)GetA();
	float RTime=IMM->GetAnimTime(SE->RotateAnimation);
	if(RTime>0)IMM->AnimateModel(SE->RotateAnimation,RTime*CurrentAngle/65536.0f,0.0f);
	float ATime=IMM->GetAnimTime(SE->ShootAnimation);
	if(ATime>0 && SE->WholeShootTime)IMM->AnimateModel(SE->ShootAnimation,ShootAnimationStage*ATime*30/256/SE->WholeShootTime,0.0f);
}
bool Active_3D_shooter_simulator::Process(OneObject* OB){	
    AddUnitToActiveList(OB);
    InfluenceMask.set_bit(ABL_ExtraAnimation);
    OB->ActiveAbility->ActiveAbilities.InfluenceMask.set_bit(ABL_ExtraAnimation);
    _3D_shooter_simulator* SE=(_3D_shooter_simulator*)GetA();

	if( Bullets<=0 ){
		if( TrueTime>CartridgeReadyTime )
			Bullets=SE->BulletsAmount;
		else
			return true;
	}

    if(ShootAnimationStage ==0 && NeedShot && LastShotTime==0){
        ShootAnimationStage=1;
    }
    CurDestToDraw=Vector3D(0,0,0);
    if(g_TankController.IsActive() && !SE->HideSpot){
        Matrix4D* GetActiveBone(int UIndex,char* BoneName);
        Matrix4D* M=GetActiveBone(OB->Index,SE->ShootDirectionNodeName.pchar());
        if(M){ 
            Vector3D src(M->e30,M->e31,M->e32);
            Vector3D dir(M->e00,M->e01,M->e02);
            dir.normalize();
            float zz=dir.z;
            Vector3D dst=src;
            dir*=-1;        
            Vector3D ShowWeaponDestination(char* WMName,int sx, int sy, int sz, int speedX, int speedY, int speedZ);
            CurDestToDraw=ShowWeaponDestination((char*)SE->Weapon.GetObjectName(),src.x,src.y,src.z,dir.x*1024,dir.y*1024,dir.z*1024);                    			
        }    
    }
    ///////////////
    NeedShot=false;
    if(LastShotTime>0)LastShotTime-=GameSpeed*AblSpeed;
    if(LastShotTime<0)LastShotTime=0;		
    char DestDir=destRotation>>8;
	char dDir=DestDir-/*OB->RealDir-*/(CurrentAngle>>8);
    if(ShootAnimationStage>0){
        int T=SE->TimeOfShooting*256/30;
        if(T>=ShootAnimationStage && T<ShootAnimationStage+GameSpeed*AblSpeed && SE->Weapon.GetObjectName()){
            //pubuxxx
			Bullets--;
			if(Bullets<=0){
				CartridgeReadyTime=TrueTime+SE->CartridgeRechargeTime;
			}
            float Scale=OB->NewAnm->Scale;
            if(OB->ActiveAbility){
                word SC=OB->NewAnm->Scale*256;
                OB->ActiveAbility->ActiveAbilities.modifyScale(SC,SC);
                Scale=SC/256.0;
            }
            float COS=TCos[OB->RealDir]/256.0f;
            float SIN=TSin[OB->RealDir]/256.0f;
            float x=OB->RealX/16.0f+(SE->TowerCenterX*COS-SE->TowerCenterY*SIN)*Scale;
            float y=OB->RealY/16.0f+(SE->TowerCenterY*COS+SE->TowerCenterX*SIN)*Scale;
            COS=TCos[byte(OB->RealDir+(CurrentAngle>>8))]/256.0f;
            SIN=TSin[byte(OB->RealDir+(CurrentAngle>>8))]/256.0f;
            x+=SE->CannonRadius*COS*Scale;
            y+=SE->CannonRadius*SIN*Scale;
            float z=SE->TowerCenterZ*Scale+OB->RZ+OB->OverEarth;
            //CreateNewActiveWeapon((char*)SE->Weapon.GetObjectName(),OB->Index,x,y,z,EOB->Index,EOB->RealX/16,EOB->RealY/16,EOB->RZ+EOB->OverEarth,SE->Damage,0);
            Matrix4D* GetActiveBone(int UIndex,char* BoneName);
            Matrix4D* M=GetActiveBone(OB->Index,SE->ShootDirectionNodeName.pchar());
            if(M){
                //void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, int Damage, int AttType);
                void CreateNewActiveWeaponByDirection(char* WMName,char* WpToCreate,int Index,int sx, int sy, int sz, int DestIndex, int speedX, int speedY, int speedZ, int Damage, int AttType);
                Vector3D src(M->e30,M->e31,M->e32);
                Vector3D dir(M->e00,M->e01,M->e02);
                dir.normalize();
				void ApplyImpulse(OneObject* OB,Vector3D dir,float Force);
				ApplyImpulse(OB,dir,SE->ShotImpulseForce);
                float zz=dir.z;
                Vector3D dst=src;
                dir*=-1;
				/*
				if(SE->HideSpot){
					if(OB->ActiveAbility){
						int n=OB->ActiveAbility->ActiveAbilities.GetAmount();
						for(int i=0;i<n;i++){
							ActiveUnitAbility* AA=OB->ActiveAbility->ActiveAbilities[i];
							Active_3D_shooter_simulator* AU=dynamic_cast<Active_3D_shooter_simulator*>(AA);
							if(AU){
								_3D_shooter_simulator* SE2=(_3D_shooter_simulator*)AU->GetA();
								CreateNewActiveWeaponByDirection((char*)SE2->Weapon.GetObjectName(),(char*)SE->Weapon.GetObjectName(),OB->Index,src.x,src.y,src.z,0xFFFF,dir.x*1024,dir.y*1024,dir.z*1024,SE->Damage,0);
								break;
							}
						}
					}                                        
				}else{
				*/
				CreateNewActiveWeaponByDirection((char*)SE->Weapon.GetObjectName(),(char*)SE->Weapon.GetObjectName(),OB->Index,src.x,src.y,src.z,0xFFFF,dir.x*1024,dir.y*1024,dir.z*1024,SE->Damage,0);
				//}
            }
            LastShotTime=SE->TotalRechargeTime*256/30;
			LastShotTime=LastShotTime/OB->GetAttackSpeedBonus()*100;
        }
        ShootAnimationStage+=GameSpeed*AblSpeed;
        int TT=SE->WholeShootTime*256/30;
        if(ShootAnimationStage>TT){
            ShootAnimationStage=0;
        }
    }else{
        if(dDir!=0){
            bool NormA=CheckAbilityToRotateInShortWay(SE->CentralShootAngle,SE->ShootAngleDifference,(CurrentAngle>>8),DestDir-OB->RealDir);
            if(!NormA)dDir=-dDir;
            int RS=SE->RotationSpeed*GameSpeed*AblSpeed/256;
            if(dDir>0){
                CurrentAngle+=RS;
                dDir=DestDir-(CurrentAngle>>8);
                if(NormA && dDir<=0){
                    CurrentAngle=int(DestDir)<<8;
                }
            }else{
                CurrentAngle-=RS;
                dDir=DestDir-(CurrentAngle>>8);
                dDir=DestDir-(CurrentAngle>>8);
                if(NormA && dDir>=0){
                    CurrentAngle=int(DestDir)<<8;
                }
            }            
        }
    }
    int DL=CurrentLiftStage-destLift;
    int LS=SE->LiftSpeed*GameSpeed*AblSpeed/256;
    if(abs(DL<=LS)){
        CurrentLiftStage=destLift;
    }else{
        if(DL<0)CurrentLiftStage+=LS;
        else CurrentLiftStage-=LS;
    }
    CurrentAngle=(CurrentAngle+65536)&65535;
    return true;
}
void Active_3D_shooter_simulator::modifyExtraAnimation(int Basic, int& Current){
    //NeedShot=true;
    _3D_shooter_simulator* SE=(_3D_shooter_simulator*)GetA();
    float RTime=IMM->GetAnimTime(SE->RotateAnimation);
    if(RTime>0)IMM->AnimateModel(SE->RotateAnimation,RTime*CurrentAngle/65536.0f,0);
    float LTime=IMM->GetAnimTime(SE->LiftAnimation);
    if(LTime>0)IMM->AnimateModel(SE->LiftAnimation,LTime*CurrentLiftStage/65536.0f,0);
    float ATime=IMM->GetAnimTime(SE->ShootAnimation);
    if(ATime>0 && SE->WholeShootTime)IMM->AnimateModel(SE->ShootAnimation,ShootAnimationStage*ATime*30/256/SE->WholeShootTime,0);

    if(CurDestToDraw.norm()>0){
		rsEnableZ(false);
        float D = CurDestToDraw.distance(Vector3D(OB->RealX>>4,OB->RealY>>4,OB->RZ));
        DrawSphere(Sphere(CurDestToDraw,D/256),0xFFFF0000,0xFFFF0000,16);
        CurDestToDraw=Vector3D(0,0,0);
		rsFlush();
    }
	if(CurDestToDraw2.norm()>0){
		rsEnableZ(false);
		float D = CurDestToDraw2.distance(Vector3D(OB->RealX>>4,OB->RealY>>4,OB->RZ));
		DrawSphere(Sphere(CurDestToDraw2,D/256),0xFF00FF00,0xFF00FF00,8);
		CurDestToDraw2=Vector3D(0,0,0);
		rsFlush();
	}
}
ActiveUnitAbility* _3D_shooter_simulator::GetNewActiveAbility(){
    Active_3D_shooter_simulator* nAA=new Active_3D_shooter_simulator;
    nAA->destLift=32768;
    nAA->CurrentLiftStage=32768;
    nAA->SetA(this);
    return nAA;
}
int getDestRotation(OneObject* OB){
    if(OB->ActiveAbility){
        int n=OB->ActiveAbility->ActiveAbilities.GetAmount();
        for(int i=0;i<n;i++){
            ActiveUnitAbility* AA=OB->ActiveAbility->ActiveAbilities[i];
            Active_3D_shooter_simulator* AU=dynamic_cast<Active_3D_shooter_simulator*>(AA);
            if(AU){
                return AU->destRotation;
            }
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
// GetCurAzim
//-----------------------------------------------------------------------------
int GetCurAzim(OneObject *pOB) {
	if(pOB->ActiveAbility) {
		int n = pOB->ActiveAbility->ActiveAbilities.GetAmount();
		for(int i = 0; i < n; i++) {
			ActiveUnitAbility *pAA = pOB->ActiveAbility->ActiveAbilities[i];
			Active_3D_shooter_simulator *pAU = dynamic_cast<Active_3D_shooter_simulator *>(pAA);
			if(pAU) {
				return pAU->CurrentAngle;
			}
		}
	}
	return 0;
} // GetCurAzim

//-----------------------------------------------------------------------------
// SetCurAzim
//-----------------------------------------------------------------------------
void SetCurAzim(OneObject *pOB, int Angle) {
	if(pOB->ActiveAbility) {
		int n = pOB->ActiveAbility->ActiveAbilities.GetAmount();
		for(int i = 0; i < n; i++) {
			ActiveUnitAbility *pAA = pOB->ActiveAbility->ActiveAbilities[i];
			Active_3D_shooter_simulator *pAU = dynamic_cast<Active_3D_shooter_simulator *>(pAA);
			if(pAU) {
				pAU->CurrentAngle = Angle;
				pAU->destRotation = Angle;
			}
		}
	}
} // SetCurAzim

//-----------------------------------------------------------------------------
// GetCurElev
//-----------------------------------------------------------------------------
int GetCurElev(OneObject *pOB) {
	if(pOB->ActiveAbility) {
		int n = pOB->ActiveAbility->ActiveAbilities.GetAmount();
		for(int i = 0; i < n; i++) {
			ActiveUnitAbility *pAA = pOB->ActiveAbility->ActiveAbilities[i];
			Active_3D_shooter_simulator *pAU = dynamic_cast<Active_3D_shooter_simulator *>(pAA);
			if(pAU) {
				return pAU->CurrentLiftStage;
			}
		}
	}
	return 0;
} // GetCurElev

//-----------------------------------------------------------------------------
// SetCurElev
//-----------------------------------------------------------------------------
void SetCurElev(OneObject *pOB, int Angle) {
	if(pOB->ActiveAbility) {
		int n = pOB->ActiveAbility->ActiveAbilities.GetAmount();
		for(int i = 0; i < n; i++) {
			ActiveUnitAbility *pAA = pOB->ActiveAbility->ActiveAbilities[i];
			Active_3D_shooter_simulator *pAU = dynamic_cast<Active_3D_shooter_simulator *>(pAA);
			if(pAU) {
				pAU->CurrentLiftStage = Angle;
				pAU->destLift = Angle;
			}
		}
	}
} // SetCurElev

void setDestRotation(OneObject* OB,int State){
    if(OB->ActiveAbility){
        int n=OB->ActiveAbility->ActiveAbilities.GetAmount();
        for(int i=0;i<n;i++){
            ActiveUnitAbility* AA=OB->ActiveAbility->ActiveAbilities[i];
            Active_3D_shooter_simulator* AU=dynamic_cast<Active_3D_shooter_simulator*>(AA);
            if(AU){
                AU->destRotation=State;
            }
        }
    }    
}
void setDestLift(OneObject* OB,int State){
    if(OB->ActiveAbility){
        int n=OB->ActiveAbility->ActiveAbilities.GetAmount();
        for(int i=0;i<n;i++){
            ActiveUnitAbility* AA=OB->ActiveAbility->ActiveAbilities[i];
            Active_3D_shooter_simulator* AU=dynamic_cast<Active_3D_shooter_simulator*>(AA);
            if(AU){
                AU->destLift=State;
            }
        }
    }    
}
int getDestLift(OneObject* OB){
    if(OB->ActiveAbility){
        int n=OB->ActiveAbility->ActiveAbilities.GetAmount();
        for(int i=0;i<n;i++){
            ActiveUnitAbility* AA=OB->ActiveAbility->ActiveAbilities[i];
            Active_3D_shooter_simulator* AU=dynamic_cast<Active_3D_shooter_simulator*>(AA);
            if(AU){
                return AU->destLift;
            }
        }
    }
    return 0;
}
void RequireShot(OneObject* OB,int WeapIdx){
    if(OB->ActiveAbility){
        int n=OB->ActiveAbility->ActiveAbilities.GetAmount();
        for(int i=0;i<n;i++){
            ActiveUnitAbility* AA=OB->ActiveAbility->ActiveAbilities[i];
            Active_3D_shooter_simulator* AU=dynamic_cast<Active_3D_shooter_simulator*>(AA);
            if(AU){
				if(WeapIdx==0){
					if(AU->ShootAnimationStage==0)AU->NeedShot=true;				
				}
				WeapIdx--;
            }
        }
    }
}