#pragma once
//////////////////////////////////////////////////////////////////////////
#include "MapTemplates.h"
#include ".\mai_script.h"
#include ".\mai_vantroop.h"
#include ".\mai_extension.h"
//////////////////////////////////////////////////////////////////////////
class mai_Unit: public BaseClass
{
public:
	int N;
	float M;
	LinearArray<float,_float> A;
	LinearArray<float,_float> P;
	SAVE(mai_Unit){
		REG_MEMBER(_int, N);
		REG_MEMBER(_float,M);
		REG_AUTO(A);
		REG_AUTO(P);
	}ENDSAVE;
	//
	bool Set(AdvCharacter* Adv, int n){
		N=n;
		M=0;		
		A.Clear();
		P.Clear();
		if(Adv&&n){			
			M=Adv->Life/N;			
			//AverageUnit.A.Check(16);
			//AverageUnit.A.NValues=16;			
			//AverageUnit.P.Check(16);
			//AverageUnit.P.NValues=16;			
			for(int i=0;i<NAttTypes;i++){
				A.Add(Adv->MaxDamage[i]/N);				
			}
			for(i=0;i<16;i++){				
				P.Add(Adv->Protection[i]/N);
			}
			return true;
		}
		return false;
	};
	
};

class mai_VanGuard: public BaseClass
{
public:
	mai_VanGuard(void);
	~mai_VanGuard(void);
	//
	byte NI;
	mai_Script Script;
	ClonesArray<mai_VanTroop> Troops;
	//
	int CenterX;
	int CenterY;
	int Radius;
	byte Dir;
	// geometry
	LinearArray<int,_int> Line;
	LinearArray<int,_int> LineW;
	//
	mai_Unit AverageUnit;
	//
	class f_AverageUnit:public BaseFunction
	{
	public:		
		void EvaluateFunction(){
			mai_VanGuard* VG=get_parent<mai_VanGuard>();
			if(VG){
				/*
				AdvCharacter ADC;
				int N;
				bool mai_getAverageUnit(byte NI, AdvCharacter& ADC, int& N);
				mai_getAverageUnit(VG->NI,ADC,N);
				VG->AverageUnit.Set(&ADC,N);
				*/
				bool mai_getAverageUnit(byte NI, mai_Unit& Unit);
				mai_getAverageUnit(VG->NI,VG->AverageUnit);
			}
		}
		SAVE(f_AverageUnit){
			REG_PARENT(BaseFunction);			
		}ENDSAVE;
	};	
	f_AverageUnit setAverageUnit;	
	//
	SAVE(mai_VanGuard){	
		REG_MEMBER(_BYTE, NI);
		REG_AUTO(Script);
		REG_AUTO(Troops);
		REG_MEMBER(_int, CenterX);
		REG_MEMBER(_int, CenterY);
		REG_MEMBER(_int, Radius);
		REG_MEMBER(_BYTE, Dir);
		//		
		REG_AUTO(setAverageUnit);		
		REG_AUTO(AverageUnit);
	}ENDSAVE;

};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
