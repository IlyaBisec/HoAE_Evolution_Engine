#include "stdheader.h"
#include ".\cvi_HeroButtons.h"
#include "unitability.h"
#include "ua_Modificators.h"
#include ".\cvi_MainMenu.h"
#include "UnitsInterface.h"
//////////////////////////////////////////////////////////////////////////
cvi_HeroButtons vHeroButtons;
//////////////////////////////////////////////////////////////////////////

bool GetHero(OneObject* OB);

bool cvi_HeroButtons::Add(OneObject* OB){
	
	if(GetHero(OB)){
		cvi_Hero* H=new cvi_Hero;
		H->Index=OB->Index;
		H->Serial=OB->Serial;
		H->DamageTime=0;
		Hero.Add(H);
		return true;
	}

	return false;
}
bool cvi_HeroButtons::Remove(OneObject* OB){
	if(OB){
		int n=Hero.GetAmount();
		for(int i=0;i<n;i++){
			cvi_Hero* H=Hero[i];
			if( H && H->Serial==OB->Serial ){
				Hero.DelElement(i);
				return true;
			}
		}
	}
	return false;
};

bool cvi_HeroButtons::isHero(OneObject* OB){
	if(OB){
		return Get(OB);
	}
	return false;
};	
void cvi_HeroButtons::checkDamage(OneObject* OB){
	if(OB){
		cvi_Hero* H=Get(OB);
		if(H){
			H->DamageTime=TrueTime+2000;
		}
	}
};
int cvi_HeroButtons::GetAmount(byte NI){
	int n=Hero.GetAmount();
	int j=0;
	for(int i=0;i<n;i++){
		cvi_Hero* H=Hero[i];
		if(H){
			OneObject* OB=Group[H->Index];
			if( OB && OB->Serial==H->Serial && !OB->Sdoxlo && OB->NNUM==NI){
				j++;
			}
		}
	}
	return j;
}
OneObject* cvi_HeroButtons::GetObject(byte NI, int ID){
	int n=Hero.GetAmount();
	int j=0;
	for(int i=0;i<n;i++){
		cvi_Hero* H=Hero[i];
		if(H){
			OneObject* OB=Group[H->Index];			
			if( OB && OB->Serial==H->Serial && !OB->Sdoxlo && OB->NNUM==NI ){
				if(ID==j){
					return OB;
				}
				j++;
			}
		}
	}
	return NULL;
}
cvi_Hero* cvi_HeroButtons::Get(OneObject* OB){
	if(OB){
		int n=Hero.GetAmount();
		for(int i=0;i<n;i++){
			cvi_Hero* H=Hero[i];
			if( H && H->Index==OB->Index && H->Serial==OB->Serial ){
				return H;
			}
		}
	}
	return NULL;
};	

//
cva_Hero_Button::cva_Hero_Button(){
	PicLx=39;
	PicLy=62;
};
void cva_Hero_Button::OnDraw(SimpleDialog* SD){
	
	OneObject* OB=Group[SD->ID];

	if( SD->ID!=0xFFFF && OB && OB->Serial==SD->UserParam && OB->ActiveAbility ){
		GPPicture* GP=Pic.Get();
		if(GP){
			NewMonster* NM=NATIONS->Mon[OB->ActiveAbility->BornNIndex]->newMons;
			if( NM->ExIcon.Icons.GetAmount() && NM->NewParams.PortretHeroFile==-1 ){
				DWORD col=GPS.GetCurrentDiffuse();
				GPS.SetCurrentDiffuse(GP->Diffuse);

				//NM->ExIcon.Draw(GP->LastDrawX,GP->LastDrawY,EngSettings.BuildIconLx,EngSettings.BuildIconLy,MyNation);
				NM->ExIcon.Draw(SD->LastDrawX+GP->x,SD->LastDrawY+GP->y,PicLx,PicLy,MyNation);

				GPS.SetCurrentDiffuse(col);
			}
		}
	}

}

extern bool InEdit;
extern bool InfoMode;

void GetDieOrStunCoolDown(OneObject* OB, int& DieCD, int& StunCD){
	DieCD=0;
	StunCD=0;
	HeroVariableStorage* HVS=DetectHero(OB);
	if(HVS){
		ActiveUnitAbility* AADie=NULL;
		ActiveUnitAbility* AAStun=NULL;

		UnitAbility* DieUA=NULL;
		HeroAbility* HA=(HeroAbility*)HVS->GetA();
		if(HA){
			DieUA=HA->OnDieUnitAbility.Get();
		}

		itr_AA.Create(OB);
		while(ActiveUnitAbility* AA=itr_AA.Next()){
			if( AA && AA->TypeAbil==0 ){
				UnitAbility* UA=AA->GetA();
				if( UA ){
					if(UA==DieUA){
						AADie=AA;
						DieCD=AADie->GetCoolDownProc();
						break;
					}
				}
				if(!AAStun){
					itr_UM.Create(AA,ABL_MotionSpeed);
					while(mod_MotionSpeed* M=(mod_MotionSpeed*)itr_UM.Next()){
						if( M->Stun ){
							AAStun=AA;
							StunCD=AAStun->GetCoolDownProc();
							break;
						}
					}
				}
			}
		}
	}
}

void cva_Hero_Button::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	if( !vHeroButtons.GetVisible() || InEdit&&InfoMode )
		return;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if( SP && DetectHero(SP->OB) && IGI.Inventory )
		return;
	ParentFrame* PF=(ParentFrame*)SD;	
	SD->ID=0xFFFF;
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	int n=vHeroButtons.GetAmount(NI);
	int j=0;
	for(int i=0;i<n;i++){
		OneObject* OB=vHeroButtons.GetObject(NI,i);
		if( OB && OB->ActiveAbility ){
			if(j==Index){
				ActiveRomeHero* ARH=NULL;
				for(int a=0;a<OB->ActiveAbility->ActiveAbilities.GetAmount();a++){
					ARH=dynamic_cast<ActiveRomeHero*>(OB->ActiveAbility->ActiveAbilities[a]);
					if(ARH) break;
				}
				SD->Visible=true;
				SD->ID=OB->Index;
				SD->UserParam=OB->Serial;
				cvi_Hero* H=vHeroButtons.Get(OB);
				bool pain=H->DamageTime>TrueTime&&(TrueTime&512);
				PF->Diffuse=0xFFFFFFFF;
				if(OB->Sdoxlo){
					SD->Enabled=false;
					PF->Diffuse=0x80FFFFFF;
				}else{
					SD->Enabled=true;						
					//PF->Diffuse=pain?0xFFFF0000:0xFFFFFFFF;
				}
				GPPicture* GP=Pic.Get();
				if(GP){
					NewMonster* NM=NATIONS->Mon[OB->ActiveAbility->BornNIndex]->newMons;
					if( NM->NewParams.PortretHeroFile!=-1 ){
						GP->Visible=true;
						GP->FileID=NM->NewParams.PortretHeroFile;
						GP->SetSpriteID(3);
					}else if(NM->ExIcon.Icons.GetAmount()){
						GP->Visible=false;
						//NM->ExIcon.Draw(GP->x,GP->y,EngSettings.BuildIconLx,EngSettings.BuildIconLy,MyNation);
						//NM->ExIcon.Draw(SD->LastDrawX+GP->x,SD->LastDrawY+GP->y,PicLx,PicLy,MyNation);
						GP->Diffuse=pain?0xFFFF0000:0xFFFFFFFF;
					}else{
						GP->Visible=true;
						if(NM->MinIconFile!=0xFFFF){
							GP->SetFileID(&NM->MinIconFile);
							GP->SetSpriteID(NM->MinIconIndex);
						}else{
							GP->SetFileID(&NM->IconFileID);
							GP->SetSpriteID(NM->IconID);
						}
					}
				}
				if(OB->newMons->GetLongMessage()[0]){
					SD->SetHint(OB->newMons->GetLongMessage());
				}else{
					SD->SetHint(OB->newMons->Message);
				}
				
				ProgressBar* LB=LifeBar.Get();
				if( !LB && SD->DSS.GetAmount()>1 ){
					LB=dynamic_cast<ProgressBar*>(SD->DSS[1]);
				}
				if(LB){
					LB->Value=OB->Life;
					LB->MaxValue=OB->MaxLife;
					//PB->BackColor=pain?0xFFFF0000:0xFFFFFFFF;
					LB->ProgressColor=OB->Life<OB->MaxLife/4?0xFFFF0000:0xFFFFFFFF;
				}

				ProgressBar* MB=ManaBar.Get();
				if(MB){
					MB->Value=OB->Mana;
					MB->MaxValue=OB->MaxMana;
				}

				VitButton* vbFL=FreeLevel.Get();
				if(vbFL){
					vbFL->Visible=false;
					if(ARH){
						int fl=ARH->GetAmountFreeLevels();
						if(fl){
							vbFL->Visible=true;
							char txt[32];
							itoa(fl,txt,10);
							vbFL->SetMessage(txt);
						}
					}
				}
				
				HeroVariableStorage* HVS=DetectHero(OB);
				if(HVS){
					ProgressBar* pbCD=CoolDown.Get();
					if(pbCD){
						pbCD->Value=0;
						int DieCD,StunCD;
						GetDieOrStunCoolDown(OB,DieCD,StunCD);
						int v=DieCD!=0?DieCD:StunCD;
						//if(v>0)
						//	pbCD->Value=100-v;
						pbCD->Value=v;
					}
					SimpleDialog* H0=SD->Find("Heart0");
					if(H0)
						H0->Visible=HVS->Lives>0;
					SimpleDialog* H1=SD->Find("Heart1");
					if(H1)
						H1->Visible=HVS->Lives>1;
					SimpleDialog* H2=SD->Find("Heart2");
					if(H2)
						H2->Visible=HVS->Lives>2;
				}
				break;
			}
			j++;
		}
		/*else{
			vHeroButtons.Hero.DeleteElement(i);
			i--;
			n--;
		}*/
	}
};
bool cva_Hero_Button::LeftClick(SimpleDialog* SD){	
	if(SD->ID!=0xFFFF){
		OneObject* OB=Group[SD->ID];
		if(OB&&OB->Serial==SD->UserParam&&!OB->Sdoxlo){
			byte NI=MyNation;
			if(OB->ImSelected&(1<<NI)){
				SetScreenCenterToXY(OB->RealX>>4,OB->RealY>>4);
				void SetCentralUnit(OneObject* OB);
				SetCentralUnit(OB);
				if(!(GetKeyState(VK_SHIFT)&0x8000)&&ImNSL[NI]>1){
					byte NI=MyNation;
					PrepareToImSelection(NI);					
					ImClearSelection(NI);					
					AddUnitToImSelected(NI,OB);
					FinalizeImSelection(NI);			
				}
			}else{				
				PrepareToImSelection(NI);
				if(!(GetKeyState(VK_SHIFT)&0x8000)){
					ImClearSelection(NI);
				}
				AddUnitToImSelected(NI,OB);
				FinalizeImSelection(NI);			
			}
		}
	}
	return true;
};

DIALOGS_API void SetAbilityInfoActivation(int& mcTimeActivation, word& mcIndex, word& mcSerial, short& mcFileID, int& mcSpriteID, int& mcX, int& mcY){
	OneObject* Hero=vHeroButtons.GetObject(MyNation,0);
	cvi_Hero* HB=vHeroButtons.Get(Hero);
	if(HB){
		int n=HB->InAbl.GetAmount();
		if(n>0){
			IncomingAbility* IA=HB->InAbl[0];
			if(IA){
				DWORD AblSerial=IA->Serial;
				itr_AA.Create(Hero);
				while(ActiveUnitAbility* AA=itr_AA.Next()){
					//UnitAbility* UA=AA->GetA();
					if( /*UA && */AA->Serial==AblSerial ){
						mcTimeActivation=GetTickCount()+200;
						mcIndex=Hero->Index;
						mcSerial=Hero->Serial;
						mcX=IA->x;
						mcY=IA->y;

						int FileID,fchr,finv;
						//_str hint;						
						AA->GetButton(FileID,mcSpriteID,fchr,finv);
						mcFileID=FileID;

						break;
					}
				}
			}
			HB->InAbl.DelElement(0);
		}
	}
}
extern CHeroesCollector HeroesCollector;
void EraseFromCollector(int index){
	for(int i=0;i<vHeroButtons.Hero.GetAmount();i++){
		if(vHeroButtons.Hero[i]->Index==index){
			vHeroButtons.Hero.DelElement(i);
			i--;			
		}
	}
	for(int i=0;i<HeroesCollector.HeroID.GetAmount();i++){
		if(HeroesCollector.HeroID[i]==index){
			HeroesCollector.HeroID.Del(i,1);
			HeroesCollector.HeroSN.Del(i,1);
			i--;
		}
	}
}
void ChangeHeroIndex(int srcindex,int dstindex){
	OneObject* OB=Group[dstindex];
	if(OB){
		for(int i=0;i<vHeroButtons.Hero.GetAmount();i++){
			if(vHeroButtons.Hero[i]->Index==srcindex){
				vHeroButtons.Hero[i]->Index=dstindex;
				vHeroButtons.Hero[i]->Serial=OB->Serial;
			}
		}
		for(int i=0;i<HeroesCollector.HeroID.GetAmount();i++){
			if(HeroesCollector.HeroID[i]==srcindex){
				HeroesCollector.HeroID[i]=dstindex;
				HeroesCollector.HeroSN[i]=OB->Serial;
			}
		}
	}
}
void CheckIfHeroInstalled(OneObject* OB){
	int N=vHeroButtons.Hero.GetAmount();
	for(int i=0;i<N;i++){
		if(vHeroButtons.Hero[i]->Index==OB->Index){
			return;
		}
	}
    vHeroButtons.Add(OB);
}