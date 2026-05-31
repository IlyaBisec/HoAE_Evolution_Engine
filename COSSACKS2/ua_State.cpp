#include "stdheader.h"
#include "ua_Ability.h"

//
extern int TrueTime;
//

bool uae_LeftClick::Check(ActiveUnitAbility* AA){ return AA->evLeftClick; }
bool uae_RightClick::Check(ActiveUnitAbility* AA){ return AA->evRightClick; }
bool uae_TimelineIsDone::Check(ActiveUnitAbility* AA){ return AA->evTimelineIsDone; }

//void uao_ChangeState::Perform(ActiveUnitAbility* AA){ AA->ChangeState(NewState); }
void uao_ChangeState::Perform(ActiveUnitAbility* AA){
	AA->toState=NewState;
}
void uao_Disappear::Perform(ActiveUnitAbility* AA){ AA->opDisappear=true; }

uat_VisualEffect::uat_VisualEffect(){
	for(int i=0;i<16;i++){
		RandDir.Add(rand());
	}

	Nation=-1;

};

bool CheckObjectVisibility(int x,int y,int z,int R);
bool GetObjectVisibilityInFog(int x,int y,int z,OneObject* OB);
bool DrawSelPatch(float x,float y,int Type,float Radius,DWORD Color);
CEXPORT DWORD GetNatColor( int natIdx );
bool DrawSelPatchDir(float x,float y,byte Dir,int Type,float ScaleX,float ScaleY,DWORD Color);

bool uat_VisualEffect::CheckDraw(ActiveUnitAbility* AA){
	UnitAbility* UA=AA->GetA();
	if(UA/*&&AA->Visible*/){

		if( Nation!=-1 && AA->OB->NNUM!=Nation ){
			return false;
		}

		int TA=AA->TypeAbil;
		if( Place==0 && (TA==2||TA==4||TA==1/*&&(!UA->Button||AA->AutoCast||AA->Casting)*/ || UA->System&&UA->ClassType==1&&UA->Conditions.Check(AA)) && AA->OB->Sdoxlo==0 || 
			Place==1 && TA!=0 /*&& AA->Casting*/ || 
			Place==2 && TA==0 ||
			Place==3 && TA!=0 /*&& AA->Casting */) {
			return true;
		}

	}
	return false;
}

bool GetObjectVisibilityInFog(OneObject* OB);

void uat_VisualEffect::Process(ActiveUnitAbility* AA, int Index){
	UnitAbility* UA=AA->GetA();
	if(UA&&AA->Visible){

		//if( Nation!=-1 && AA->OB->NNUM!=Nation ){
		//	return;
		//}

		//bool Casting=(TrueTime-AA->CastingTime<UA->CoolDownDuration);
		int TA=AA->TypeAbil;
		if( Place==0 && (TA==2||TA==4&&AA->Clothed||TA==1&&(!UA->Button||AA->AutoCast||AA->Casting) || UA->System&&UA->ClassType==1&&UA->Conditions.Check(AA) )&&AA->OB->Sdoxlo==0 || 
			Place==1 && TA!=0/*&&AA->Casting*/ || 
			Place==2 && TA==0 ||
			Place==3 && TA!=0&&AA->Casting ) {

			int ST=AA->EffectTime;
			if(TA==2)
				ST=AA->BirthTime;

			int dT=TrueTime-(ST/*StateTime*/+StartTime);
			if( UnlimitDuration && Place!=1 || dT>=0 && dT<Duration ){
				
				if( TA==0 ){
					GlobalUnitsContext::setCaster(AA->CasterID,AA->CasterSN);
				}else{
					GlobalUnitsContext::setCaster(AA->OB->Index,AA->OB->Serial);
				}
				
				int x,y,z;

				// Area center
				x=AA->arX;
				y=AA->arY;

				switch(Place){
					case 0: // Caster
					case 2: // Units
						x=AA->OB->/*RealX*/GetAttX()>>4;
						y=AA->OB->/*RealY*/GetAttY()>>4;
						break;
					case 1: // Area center
						break;
				}

				//z=GetHeight(x,y);
				z=AA->OB->RZ+AA->OB->OverEarth;

				if( !(CheckObjectVisibility(x,y,z,150)&&GetObjectVisibilityInFog(x,y,z,NULL)) )
					return;

				NewAnimation* NA=&Effect;

				int Reset=0;
				if(ResetPeriod){
					Reset=(dT/ResetPeriod)%RandDir.GetAmount();
				}

				DWORD handle=(AA->OB->Serial<<16)+(AA->Serial<<8)+(Reset<<4)+Index;

				byte Dir=0;
				if(RandomDirection){
					int i=handle%RandDir.GetAmount();
					Dir=RandDir[i];
				}

				float scale=1;//NA->Scale;
				if(ScaleToRadius){
					float r=UA->Radius.Get();
					scale=r/300.0;
				}
				if(AllowFormFactor){
					scale*=float(AA->OB->newMons->FormFactor)/float(256);
				}
				
				int frame=0;
				if(NA->NFrames){
					frame=(dT/40)%NA->NFrames;
					if (NA->NFrames>1 && PauseFrame<NA->NFrames) {
						int t=dT;
						int dur=Duration;
						if(UnlimitDuration){
							dur=UA->EffectDuration-StartTime;
						}
						frame=PauseFrame;
                        int PD=PauseDuration.Get();
						if (PD && dur>PD) {
							int fd=(dur-PD)/(NA->NFrames-1);
							if(fd>0){
								int pt=fd*PauseFrame;
								if (t<pt) {
									frame=t/fd;
								}else
								if (t>=pt+PD) {
									frame=(t-PD)/fd+1;
								}
							}
						}
					}
				}
				AddAnimation(x,y,z,NA,frame*256,Dir,0xFFFFFFFF,AA->OB,scale,0,0,handle);

				if(CycledSound!=-1){
					extern CDirSound* CDS;
					CDS->HitSound(CycledSound);
					AddEffect(AA->OB->RealX/16,AA->OB->RealY/16,CycledSound);
				}

				if(UseSelType){
					OneObject* OB=AA->OB;
					if( !OB->Sdoxlo && GetObjectVisibilityInFog(OB) ){
						bool inv=false;
						if(OB&&OB->ActiveAbility){
							byte mask=0;
							bool EnableSearchVictim;
							OB->ActiveAbility->ActiveAbilities.modifyInvisibility(false,inv,mask,EnableSearchVictim); // не рисовать тень для невидимок
						}
						if(!inv){
							NewMonster* NM=OB->newMons;						
							DrawSelPatch(x,y,SelType,NM->UnitCylinderRadius,GetNatColor(AA->OB->NNUM)|0xFF000000);
							//DrawSelPatchDir(x,y,OB->RealDir,SelType,NM->selScaleX,NM->selScaleY,GetNatColor(AA->OB->NNUM));
						}
					}
				}

			}	
		}
	}
};

void uat_Modificator::Start(ActiveUnitAbility* AA){
	//for(int i=0;i<Modificators.GetAmount;i++){
	//}
	
};
void uat_Modificator::Stop(ActiveUnitAbility* AA){
};

bool VisualEffectArray::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	dest="Visual Effects:";
	if(!ClassEditor::CheckIfOpen(this)){
		int N=GetAmount();
		if(N){
			dest+="\n";
			for (int i=0;i<N;i++){
				dest+="   %$";
				dest+=i;
				dest+="$%";
				if (i+1<N) dest+="\n";
			};
		}else{
			dest+=" Empty";
		}
	}
	return true;
};