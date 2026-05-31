#include "stdheader.h"
#include "DrawFeatures.h"
void DrawSmartArrow(int x0,int y0,int x1,int y1,byte StartDirection
					,DWORD Color1,DWORD Color2,int StartThickness,int EndThickness);
void DrawColoredCircle(int xc,int yc,int R0,int R1,DWORD LineColor1,DWORD LineColor2,DWORD FillColor1,DWORD FillColor2);

float sq_norma(float x,float y);
Brigade* GetEnemyForMeleeAttack(Brigade* BR){
	int x,y;
	byte Dir;
	int xe,ye;
	byte d1;
	if(BR->GetCenter(&x,&y,&Dir)){
		Brigade* EB=BR->GetNearesEnmBrigade();
		if(EB&&EB->GetCenter(&xe,&ye,&d1)){
			int R=sq_norma(xe-x,ye-y);
			if(R<EngSettings.BrigadesArrowParam.RadiusOfEnemyDetection)return EB;
		}
	}
	return NULL;
}
void DrawArrowWhenAttEnm::Draw(){
	int x,y;
	byte Dir;
	if(BSrc->GetCenter(&x,&y,&Dir)){
		extern int LastMx;
		extern int LastMy;
		DrawSmartArrow(x,y,LastMx,LastMy,Dir,EngSettings.BrigadesArrowParam.StartArrowColor
			,EngSettings.BrigadesArrowParam.FinalArrowColor,EngSettings.BrigadesArrowParam.StartArrowWidth/3
			,EngSettings.BrigadesArrowParam.FinalArrowWidth/2);
	}
}
void DrawArrowBetweenBrigades::Draw(){
	int x,y;
	byte Dir;
	int xe,ye;
	byte d1;
	if(BSrc->GetCenter(&x,&y,&Dir)&&BDest->GetCenter(&xe,&ye,&d1)){
		DrawSmartArrow(x,y,xe,ye,Dir,EngSettings.BrigadesArrowParam.StartArrowColor
			,EngSettings.BrigadesArrowParam.FinalArrowColor,EngSettings.BrigadesArrowParam.StartArrowWidth
			,EngSettings.BrigadesArrowParam.FinalArrowWidth);
	}
}
void DrawAttackRangesOfBrigade::Draw(){
	void ShowUnitsRanges(word* IDS,word* SNS,int NU);
	if(Line==-1)ShowUnitsRanges(BR->Memb,BR->MembSN,BR->NMemb);
	else{

		word IDS[1024];
		word SNS[1024];
		int N=0;

		if(BR->WarType){
			OrderDescription* ODE=ElementaryOrders+BR->WarType-1;
			int NA=ODE->NActualLines;
			int NL=0;			
			int FL=ODE->FirstActualLine;
			int bp=NBPERSONAL;
			for(int p=0;p<NA;p++){
				int NU=ODE->LineNU[p+FL];
				if(NU){					
					for(int q=0;q<NU;q++){
						if(bp<BR->NMemb){
							word ID=BR->Memb[bp];
							if(ID!=0xFFFF&&p==Line){
								OneObject* OB=Group[ID];
								if(OB&&OB->Serial==BR->MembSN[bp]&&N<1024){
                                    IDS[N]=OB->Index;
									SNS[N]=OB->Serial;
									N++;
								}
							}
							bp++;
						}
					}
					NL++;
				}
			}
		}
		ShowUnitsRanges(IDS,SNS,N);
	}
}

void DrawSpecialAttackRange::Draw(){
	void ShowAttTypeRange(word* IDS,word* SNS,int NU,int AttType);
	ShowAttTypeRange(BR->Memb,BR->MembSN,BR->NMemb,AttType);
}


void DrawMeleeOpponentForBrigade::Draw(){
	int x,y;
	byte Dir;
	if(BR->GetCenter(&x,&y,&Dir)){
		DrawColoredCircle(x,y,0,EngSettings.BrigadesArrowParam.RadiusOfEnemyDetection
			,0x00FFFFFF,EngSettings.BrigadesArrowParam.CircleLineColor
			,EngSettings.BrigadesArrowParam.StartCircleColor,EngSettings.BrigadesArrowParam.FinalCircleColor);
		Brigade* EB=GetEnemyForMeleeAttack(BR);
		if(EB){
			int xe,ye;
			byte d1;
			if(EB->GetCenter(&xe,&ye,&d1)){				
				DrawSmartArrow(x,y,xe,ye,Dir,EngSettings.BrigadesArrowParam.StartArrowColor
					,EngSettings.BrigadesArrowParam.FinalArrowColor,EngSettings.BrigadesArrowParam.StartArrowWidth
					,EngSettings.BrigadesArrowParam.FinalArrowWidth);
			}
		}
	}
}

void DrawCircleSomewhere::Draw(){
    void DrawColoredCircle(int xc,int yc,int R0,int R1,DWORD LineColor1,DWORD LineColor2,DWORD FillColor1,DWORD FillColor2);
	DrawColoredCircle(xc,yc,R1,R2,LineColor1,LineColor2,FillColor1,FillColor2);
}

void FeaturesList::Draw(){
	for(int i=0;i<Features.GetAmount();i++){
		OneFeature* OF=Features[i];
		if(OF){
			OF->Draw();
			if(GetTickCount()-OF->add_time>150){
				Features.DelElement(i);
				i--;
			}
		}
	}
	//Features.Clear();
}
void FeaturesList::Clear(){
	Features.Clear();
}
void FeaturesList::AddArrowWhenAttEnm(Brigade* BR){
	for(int i=0;i<Features.GetAmount();i++){
		OneFeature* OF=dynamic_cast<DrawArrowBetweenBrigades*>(Features[i]);
		if(OF){
			return;
		}
	}
	DrawArrowWhenAttEnm* E=new DrawArrowWhenAttEnm;
	E->BSrc=BR;	
	AddNewFeature(E);
};
void FeaturesList::AddArrowBetweenBrigades(Brigade* B1,Brigade* B2){
    DrawArrowBetweenBrigades* E=new DrawArrowBetweenBrigades;
	E->BSrc=B1;
	E->BDest=B2;
	AddNewFeature(E);
}
void FeaturesList::AddAttackRangesOfBrigade(Brigade* BR,int Line){
	DrawAttackRangesOfBrigade* E=new DrawAttackRangesOfBrigade;
	E->BR=BR;	
	E->Line=Line;
	AddNewFeature(E);
}
void FeaturesList::AddSpecialAttackRange(Brigade* BR,int AttType){
	DrawSpecialAttackRange* E=new DrawSpecialAttackRange;
	E->BR=BR;	
	E->AttType=AttType;
	AddNewFeature(E);
}
void FeaturesList::AddMeleeOpponentForBrigade(Brigade* BR){
	DrawMeleeOpponentForBrigade* E=new DrawMeleeOpponentForBrigade;
	E->BR=BR;	
	AddNewFeature(E);
}
void FeaturesList::AddCircleSomewhere(int xc,int yc,int R1,int R2,DWORD LineColor1,DWORD LineColor2,DWORD Color1,DWORD Color2){
	DrawCircleSomewhere* E=new DrawCircleSomewhere;
	E->xc=xc;
	E->yc=yc;
	E->R1=R1;
	E->R2=R2;
	E->FillColor1=Color1;
	E->FillColor2=Color2;
	AddNewFeature(E);
}
void FeaturesList::AddNewFeature(OneFeature* F){
	int H=F->GetHash();
	for(int i=0;i<Features.GetAmount();i++){
		if(Features[i]->GetHash()==H){
			delete[](F);
			Features[i]->add_time=GetTickCount();
			return;
		}
	}
	F->add_time=GetTickCount();
	Features.Add(F);
}
FeaturesList FList;