#include "stdheader.h"
#include "unitability.h"
#include "BrigadeAbility.h"
//
#include "Groups.h"
bool CheckIfPossibleToBreakOrder(OneObject* OB);
extern int PeaceTimeLeft;
extern int AnimTime;
void CheckArmies(City* CT);
void CorrectBrigadesSelection(byte NT);
void ImCorrectBrigadesSelection(byte NT);
void NewMonsterSmartSendToLink(OneObject* OBJ);
void NewMonsterSendToLink(OneObject* OBJ);
void AI_AttackPointLink(OneObject* OBJ);
extern word SelCenter[8];
int CheckCreationAbility(byte NI,int Type,int* x2i,int* y2i,word* BLD,int NBLD,bool Manual); 
void BuildWithSelected(byte NI,word ObjID,byte OrdType); 
void GoToMineWithSelected(byte NI,word ID);
int GetBMIndex(OneObject* OB);
float sq_norma(float x,float y);
byte GetBrigadeDirectionByUnitPositions(Brigade* BR);
bool CheckDupInBrigade(Brigade* BR){
	return false;
	if(rando()>32000)return false;
	for(int i=0;i<BR->NMemb;i++){
		if(BR->Memb[i]!=0xFFFF){
			OneObject* OB=Group[BR->Memb[i]];
			if(OB&&OB->Serial==BR->MembSN[i]){
				//assert(OB->BrIndex==i);
				if(OB->BrIndex!=i)
				{
					OB->Die();
					//AssignHint1("WARNING! Brigade integrity failure!!!",200);
					return true;
				}
			}
		}
	}
	return false;
}
byte RED_COLOR=0;

void CheckBrIntegrity(){
	return;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB){
			if(OB->BrigadeID!=0xFFFF){
				assert(OB->Nat->CITY->Brigs[OB->BrigadeID].Memb[OB->BrIndex]==OB->Index);
			};
		};
	};
};
void CheckIfOneObjectHaveLongOrder()
{
	for(int i=0;i<MAXOBJECT;i++)
	{
		OneObject* OB = Group[i];
		if(OB&&OB->LocalOrder)
		{
			Order1* Or = OB->LocalOrder->NextOrder;
			int Con = 0;
			while(Or)
			{
				Con++;
				assert(Con<100);
				Or = Or->NextOrder;
			}
		}
	}
}
void Brigade::Init(City* ct,word id){
	memset(this,0,sizeof Brigade);
	CT=ct;
	ID=id;
	ArmyID=0xFFFF;
	AutoFormID=0xFFFF;
#ifdef COSSACKS2
	VeraVPobedu=0;
	NKills=0;
	OfficerID=0xFFFF;
	BarID=0xFFFF;
	FlagID=0xFFFF;
#endif
#ifdef GETTIRED
	IsTired=0;
	LastTireCheckTime=id&63;
#endif
	if(Ability)
	{
		Ability->Clear();
		delete Ability;
		Ability=NULL;
	}
	lastTimeGetCenter=-1;
};
bool Brigade::RemoveOne(int Index,Brigade* Dest){
	if(Index<0||Index>=NMemb)return false;
	OneObject* OB=Group[Memb[Index]];
	if((!OB)||OB->Serial!=MembSN[Index])return false;
	(&BM.Peons)[GetBMIndex(OB)]--;
	if(Index<NMemb-1){
		memcpy(Memb+Index,Memb+Index+1,(NMemb-Index-1)<<1);
		memcpy(MembSN+Index,MembSN+Index+1,(NMemb-Index-1)<<1);
		if(PosCreated){
			memcpy(posX+Index,posX+Index+1,(NMemb-Index-1)<<2);
			memcpy(posY+Index,posY+Index+1,(NMemb-Index-1)<<2);
		};
	};
	NMemb--;
	Dest->AddObject(OB);
	SetIndex();
	CheckBrIntegrity();
	return true;
};
void Brigade::FreeMember(int Index){
	if(Index<0||Index>=NMemb)return;
	OneObject* OB=Group[Memb[Index]];
	if((!OB)||OB->Serial!=MembSN[Index])return;
	(&BM.Peons)[GetBMIndex(OB)]--;
	if(Index<NMemb-1){
		memcpy(Memb+Index,Memb+Index+1,(NMemb-Index-1)<<1);
		memcpy(MembSN+Index,MembSN+Index+1,(NMemb-Index-1)<<1);
		if(PosCreated){
			memcpy(posX+Index,posX+Index+1,(NMemb-Index-1)<<2);
			memcpy(posY+Index,posY+Index+1,(NMemb-Index-1)<<2);
		};
	};
	NMemb--;
	OB->BrigadeID=0xFFFF;
	lastTimeGetCenter=0;
	OB->BrIndex=0;
	OB->InArmy=0;
	SetIndex();
	CheckBrIntegrity();
};
void Brigade::RemovePeasants(Brigade* Dest){
	int N=NMemb;
	for(int i=0;i<N;i++){
		OneObject* OB=Group[Memb[i]];
		if(OB&&OB->Serial==MembSN[i]&&OB->newMons->Peasant){
			RemoveOne(i,Dest);
			i--;
			N--;
		};
	};
	CheckBrIntegrity();
};
void Brigade::AddObject(OneObject* OB){
	lastTimeGetCenter=0;
	if(NMemb>=MaxMemb){
		MaxMemb+=128;
		Memb=(word*)realloc(Memb,MaxMemb<<1);
		MembSN=(word*)realloc(MembSN,MaxMemb<<1);
	};
	if(OB){
		OB->DoNotCall=false;
		PosCreated=false;
		Memb[NMemb]=OB->Index;
		MembSN[NMemb]=OB->Serial;
		OB->BrigadeID=ID;
		OB->BrIndex=NMemb;
		(&BM.Peons)[GetBMIndex(OB)]++;
		NMemb++;
		MembID=OB->NIndex;
	}else{
		PosCreated=false;
		Memb[NMemb]=0xFFFF;
		MembSN[NMemb]=0xFFFF;
		NMemb++;
	};
};
void Brigade::CheckMembers(City* CT){
	if(WarType)return;
	/*
	byte CCC[100];
	memset(CCC,0,100);
	for(int k=0;k<NMemb;k++){
		if(Memb[k]<100){
			CCC[Memb[k]]++;
			assert(CCC[Memb[k]]<2);
		};
	};
	*/
	/*
	for(int i=0;i<NMemb;i++){
		OneObject* OB=Group[Memb[i]];
		if(OB&&OB->BrIndex!=ID){
			MembSN[i]-=31;
		};
	};
	*/
	for(int i=0;i<NMemb;i++){
		OneObject* OB=Group[Memb[i]];
		if((!OB)||OB->Serial!=MembSN[i]||OB->Sdoxlo){
			(&BM.Peons)[GetBMIndex(OB)]--;
			if(i<NMemb-1){
				memcpy(Memb+i,Memb+i+1,(NMemb-i-1)<<1);
				memcpy(MembSN+i,MembSN+i+1,(NMemb-i-1)<<1);
				if(PosCreated){
					memcpy(posX+i,posX+i+1,(NMemb-i-1)<<2);
					memcpy(posY+i,posY+i+1,(NMemb-i-1)<<2);
				};
			};
			NMemb--;
			i--;
		};
	};
	SetIndex();
	//memset(CCC,0,100);
	if(CFN){
		for(int i=0;i<NMemb;i++){
			OneObject* OB=Group[Memb[i]];
			if(OB){
				int br=CFN(OB,CT);
				if(br!=-1){
					RemoveOne(i,&CT->Brigs[br]);
					i--;
					//NMemb--;
				};
			};
		};
	};
	/*
	memset(CCC,0,100);
	for(k=0;k<NMemb;k++){
		if(Memb[k]<100){
			CCC[Memb[k]]++;
			assert(CCC[Memb[k]]<2);
		};
	};
	*/
};
void Brigade::RemoveObjects(int NObj,Brigade* Dest){
	/*
	byte CCC[100];
	memset(CCC,0,100);
	for(int k=0;k<NMemb;k++){
		if(Memb[k]<100){
			CCC[Memb[k]]++;
			assert(CCC[Memb[k]]<2);
		};
	};
	*/
	for(int i=0;i<NObj;i++){
		if(NMemb){
			OneObject* OB=Group[Memb[NMemb-1]];
			if(OB){
				(&BM.Peons)[GetBMIndex(OB)]--;
				Dest->AddObject(OB);
			};
			NMemb--;
		};
	};
	/*
	memset(CCC,0,100);
	for(k=0;k<NMemb;k++){
		if(Memb[k]<100){
			CCC[Memb[k]]++;
			assert(CCC[Memb[k]]<2);
		};
	};
	*/
	SetIndex();
};
void Brigade::DeleteAll(){
	//ClearBOrders();
	ClearNewBOrders();
	if(Memb)free(Memb);
	if(MembSN)free(MembSN);
	Init(CT,ID);
	CheckBrIntegrity();
};
void Brigade::SetIndex(){
	lastTimeGetCenter=0;
	for(int i=0;i<NMemb;i++){
		word MID=Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==MembSN[i]){
				OB->BrigadeID=ID;
				OB->BrIndex=i;
			};
		};
	};
};
Brigade* OneObject::GetBrigade(){
	if(BrigadeID==0xFFFF)return NULL;
	Brigade* BRI=NULL;
	switch(BrigadeID){
	case 0://Free
		BRI=&Nat->CITY->Free;
		break;
	case 1://OnWood
		BRI=&Nat->CITY->OnWood;
		break;
	case 2://OnStone
		BRI=&Nat->CITY->OnStone;
		break;
	case 3:
		BRI=&Nat->CITY->OnField;
		break;
	case 4:
		BRI=&Nat->CITY->Builders;
		break;
	case 5:
		BRI=&Nat->CITY->InMines;
		break;
	case 6:
		BRI=&Nat->CITY->FreeArmy;
		break;
	};
	if(!BRI){
		BrigadeID=0xFFFF;
		return NULL;
	};
	if(BrIndex>=BRI->NMemb||Group[BRI->Memb[BrIndex]]!=this){
		BrigadeID=0xFFFF;
		return NULL;
	};
	return BRI;
};
BrigadeOrder* Brigade::CreateBOrder(byte Type,int Size){
	for(int i=0;i<NMemb;i++){
		word ID=Memb[i];
		if(ID!=0xFFFF){
			OneObject* OB=Group[ID];
			if(OB&&OB->Serial==MembSN[i]){
				OB->Illusion=0;
			}
		}
	}
	BrigadeOrder* OR1=(BrigadeOrder*)malloc(Size);
	memset(OR1,0,Size);
	BrigadeOrder* OR2;
	switch(Type){
	case 1:
		OR1->Next=BOrder;
		BOrder=OR1;
		break;
	case 2:
		OR1->Next=NULL;
		if(BOrder){
			OR2=BOrder;
			while(OR2->Next)OR2=OR2->Next;
			OR2->Next=OR1;
		}else BOrder=OR1;
		break;
	default:
		ClearBOrders();
		BOrder=OR1;
		OR1->Next=NULL;
	};
	return OR1;
};
void B_GoOnRoadLink(Brigade* BR);
extern City CITY[8];
word GetDir(int,int);
Brigade* Brigade::GetNearesEnmBrigade()
{
	Brigade* rez=NULL;
	byte mmask=GetNMask();
	int xx=0;
	int yy=0;
	byte ddr=0;
	int minl=999999;
	int dl[8]={0,0,0,0,0,0,0,0};
	Brigade* bl[8]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
	int nl=0;
	this->GetCenter(&xx,&yy,&ddr);
	for(int i=0;i<8;i++)
	{
		int n=CITY[i].NBrigs;
		for(int j=0;j<n;j++)
		{
			if(CITY[i].Brigs[j].WarType>0)
			{
				if(!(CITY[i].Brigs[j].GetNMask()&mmask))
				{
					int xx2=0;
					int yy2=0;
					byte ddr2=0;
					CITY[i].Brigs[j].GetCenter(&xx2,&yy2,&ddr2);
					int dds=sq_norma(xx-xx2,yy-yy2);
					//if(dds<2500)
					{
						if(dds<minl)
						{
							minl=dds;
							rez=&CITY[i].Brigs[j];
						}
						if(dds<1200)
						{
							if(nl<7)
							{
								dl[nl]=dds;
								bl[nl]=&CITY[i].Brigs[j];
								nl++;
							}
							else
							{
								int mdl=dds;
								int mdind=-1;
								for(int k=0;k<8;k++)
								{
									if(dl[k]>mdl)
									{
										mdl=dl[k];
										mdind=k;
									}
								}
								if(mdind!=-1)
								{
									dl[mdind]=dds;
									bl[mdind]=&CITY[i].Brigs[j];
								}
							}
						}
					}
				}
			}
		}
	}
	for(int i=0;i<nl;i++)
	{
		int avrD=0;
		int avrN=0;
		for(int j=0;j<bl[i]->NMemb;j++)
		{
			if(bl[i]->Memb[j]!=0xFFFF)
			{
				OneObject* OB=Group[bl[i]->Memb[j]];
				if(OB)
				{
					int td=sq_norma(xx-OB->RealX/16,yy-OB->RealY/16);
					if(td<(dl[i]+100))
					{
						avrD+=td;
						avrN++;
					}
				}
			}
		}
		if(avrN)
		{
			avrD/=avrN;
			if(avrD<minl)
			{
				minl=avrD;
				rez=bl[i];
			}
		}
	}
	return rez;
}
byte Brigade::GetDirToNearesEnmBrigade()
{
	Brigade* Enm = GetNearesEnmBrigade();
	if(Enm)
	{
		addrand(Enm->ID);
		int xx=0;
		int yy=0;
		byte ddr=0;
		GetCenter(&xx,&yy,&ddr);
		addrand(xx);
		addrand(yy);
		int xx1=0;
		int yy1=0;
		byte ddr1=0;
		Enm->GetCenter(&xx1,&yy1,&ddr1);
		addrand(xx1);
		addrand(yy1);
		addrand(ddr1);		
		return GetDir(xx1-xx,yy1-yy);
	}
	addrand(Direction);
	return Direction;
}
extern Musorozbornik MusorStorage;
void Brigade::DeleteBOrder(){
	if(BOrder){
		BrigadeOrder* OR1=BOrder->Next;
		if(BOrder->BLink==&B_GoOnRoadLink)
			MusorStorage.AddBrigadeLinkInfo(this);
		free(BOrder);
		BOrder=OR1;
//		if(OR1)assert(int(OR1->BLink)!=0xcdcdcdcd);
	}
	//if(!BOrder){
	//	if(EngSettings.AutoChangeFormationType){
	//		int ac=GetActivity();
	//		if(ac==1){//defensive				
	//			int bt=GetFormIndex();
	//			if(bt==1){
	//				void MakeReformation(byte NI,word BrigadeID,byte FormType);
	//				MakeReformation(CT->NI,ID,0);
	//			}
	//		}
	//	}
	//}
};
void Brigade::ClearBOrders(){
	while(BOrder)DeleteBOrder();
};
void Brigade::DeleteNewBOrder()
{
	if(NewBOrder)
	{
		CBrigadeOrder* OR1=NewBOrder->Next.Get();
		NewBOrder->Next.Set(NULL);
		delete NewBOrder;
		NewBOrder=OR1;
	}
}
void Brigade::ClearNewBOrders()
{
	if(NewBOrder)
		delete NewBOrder;
	NewBOrder=NULL;
}
bool Brigade::CreateNewBOrder(byte Type,CBrigadeOrder* Order)
{
	if(Order)
	{
		for(int i=0;i<NMemb;i++)
		{
			word ID=Memb[i];
			if(ID!=0xFFFF)
			{
				OneObject* OB=Group[ID];
				if(OB&&OB->Serial==MembSN[i])
				{
					OB->Illusion=0;
				}
			}
		}
		switch(Type)
		{
		case 1:
			Order->Next.Set(NewBOrder);
			NewBOrder=Order;
			break;
		case 2:
			if(NewBOrder)
			{
				CBrigadeOrder* OR2;
				OR2=NewBOrder;
				while(OR2->Next.Get())OR2=OR2->Next.Get();
				OR2->Next.Set(Order);
			}
			else 
				NewBOrder=Order;
			break;
		default:
			ClearNewBOrders();
			NewBOrder=Order;
		};
		return true;
	}
	return false;
}
void Brigade::CreateConvoy(byte Type){
	posX=(int*)realloc(posX,NMemb<<2);
	posY=(int*)realloc(posY,NMemb<<2);
	int LX=int(sqrt(NMemb));
	int LY=LX;
	if(LX*LY<NMemb)LX++;
	if(LX*LY<NMemb)LY++;
	int Ldis=32;
	if(Type==1)Ldis=128;
	int dx=-(LX*Ldis)>>1;
	int dy=-(LY*Ldis)>>1;
	int n=0;
	PosCreated=true;
	for(int xx=0;xx<LX;xx++)
		for(int yy=0;yy<LY;yy++){
			posX[n]=(xx*Ldis)+dx;
			posY[n]=(yy*Ldis)+dx;
			n++;
			if(n>=NMemb)return;
		};
};
void FindShotWayPoint(word Amo,int* FromXY,int* ToXY,word* Result);
void Brigade::ResortMembByPos()
{
	lastTimeGetCenter=0;
	int N=NMemb-3;
	int* DestPoints=(int*)malloc(N<<3);
	int* FromPoints=(int*)malloc(N<<3);
	word* DestIndex=(word*)malloc(N<<1);
	int count=0;

	word* MembCopy=(word*)malloc(NMemb<<1);
	word* SerCopy=(word*)malloc(NMemb<<1);
	word* Ord=(word*)malloc(NMemb<<1);
	memset(Ord,0xFF,NMemb<<1);
	
	for(int i=0;i<NMemb;i++)
	{
		MembCopy[i]=Memb[i];
		SerCopy[i]=MembSN[i];
	}
	for(int i=0;i<N;i++)
	{
		//DestPoints[i*2]=posX[i+3]>>4;
		//DestPoints[i*2+1]=posY[i+3]>>4;
		if(Memb[i+3]!=0xFFFF)
		{
			OneObject* OB=Group[Memb[i+3]];
			if(OB)
			{
				//FromPoints[i*2]=OB->RealX>>4;
				//FromPoints[i*2+1]=OB->RealY>>4;
				DestPoints[count*2]  =posX[i+3]<<4;
				DestPoints[count*2+1]=posY[i+3]<<4;
	
				FromPoints[count*2]  =OB->RealX;
				FromPoints[count*2+1]=OB->RealY;
				Ord[i+3]=count;
				count++;
			}
		}
	}
	FindShotWayPoint(count,FromPoints,DestPoints,DestIndex);
	/*
	int count2=0;
	for(int i=0;i<N;i++)
	{
		//DestPoints[i*2]=posX[i+3]>>4;
		//DestPoints[i*2+1]=posY[i+3]>>4;
		if(MembCopy[i+3]!=0xFFFF)
		{
			OneObject* OB=Group[MembCopy[i+3]];
			if(OB)
			{
				Memb[i+3]=MembCopy[DestIndex[count2]+3];
				MembSN[i+3]=SerCopy[DestIndex[count2]+3];
				count2++;
			}
		}
	}
	*/

	for(int i=0;i<count;i++)
	{
		word Inu=0xFFFF;
		word Pls=0xFFFF;
		for(int j=3;j<NMemb;j++)
		{
			if(Ord[j]==DestIndex[i])
			{
				Inu=j;
			}
			if(Ord[j]==i)
			{
				Pls=j;
			}
		}
		if(Inu!=0xFFFF&&Pls!=0xFFFF)
		{
			Memb[Inu]=MembCopy[Pls];
			OneObject* OB=Group[Memb[Inu]];
			if(OB)
			{
				OB->BrIndex=Inu;
				MembSN[Inu]=OB->Serial;
			}
		}
		
	}
	
	free(Ord);
	free(MembCopy);
	free(SerCopy);
	free(DestIndex);
	free(DestPoints);
	free(FromPoints);

}
bool Brigade::GetAbilityChange(int& AddDam, int& AddShd)
{
	bool rez=false;
	if(Ability)
	{
		int n=Ability->GetAmount();
		for(int i=0;i<n;i++)
		{
			ActiveBrigadeAbility* ABA = (*Ability)[i];
			if(ABA)
			{
				AddDam+=ABA->GetAddDamage();
				AddShd+=ABA->GetAddShield();
				rez=true;
			}
		}
	}
	return rez;
}
bool Brigade::GetBrigadeDestination(int& dx, int& dy)
{
	bool rez=false;
	CBrigadeOrder* Next=NewBOrder;
	int kpx=-1;
	int kpy=-1;
	while (Next&&!rez) 
	{
		if(Next->GetBrigadeOrderID()==BRIGADEORDER_KEEPPOSITION&&kpx==-1)
		{
			int cx=0;
			int cy=0;
			int N=0;
			for(int i=NBPERSONAL;i<NMemb;i++)
			{
				if(Memb[i]!=0xFFFF)
				{
					cx+=posX[i];
					cy+=posY[i];
					N++;
				}
			}
			if(N)
			{
				kpx=cx/N;
				kpy=cy/N;
			}
		}
		if(Next->GetBrigadeOrderID()==BRIGADEORDER_HUMANGLOBALSENDTO)
		{
			BrigadeOrder_HumanGlobalSendTo* Or = (BrigadeOrder_HumanGlobalSendTo*)Next;
			dx=Or->xx;
			dy=Or->yy;
			rez=true;
		}
		else
			Next=Next->Next.Get();
	}
	if(!rez&&kpx!=-1)
	{
		dx=kpx;
		dy=kpy;
		rez=true;
	}
	return rez;
}
bool CorrectOnePosition(int* x,int* y){
	int xx=*x;
	int yy=*y;
	for(int r=0;r<RRad;r++){
		char* xi=Rarr[r].xi;
		char* yi=Rarr[r].yi;
		int N=Rarr[r].N;
		for(int j=0;j<N;j++){
			int x1=xx+xi[j];
			int y1=yy+yi[j];
			if(!CheckBar(x1,y1,6,6)){
				*x=x1;
				*y=y1;
				return true;
			};
		};
	};
	return false;
};
bool PosChanged;

void CheckTopPointPos(int xc,int yc,int* xd,int* yd){
	xc<<=4;
	yc<<=4;
	int x=(*xd)<<4;
	int y=(*yd)<<4;
	int mx=msx<<9;
	int my=msy<<9;

	if(xc<=0)xc=1;
	if(yc<=0)yc=1;
	if(xc>=mx)xc=mx-1;
	if(yc>=my)yc=my-1;

	if(x<=0)x=1;
	if(y<=0)y=1;
	if(x>=mx)x=mx-1;
	if(y>=my)y=my-1;

	int dx=xc-x;
	int dy=yc-y;
	int NRM=Norma(dx,dy);
	int N1=(NRM>>9)+1;
	dx/=N1;
	dy/=N1;
	for(int j=0;j<N1;j++){
		if(xc>0&&yc>0&&xc<mx&&yc<my){
			int tpx=xc>>10;
			int tpy=yc>>10;
			if(SafeTopRef(tpx,tpy)>=0xFFFE){
				xc-=dx;
				yc-=dy;
				if(xc>0&&yc>0&&xc<mx&&yc<my){
					*xd=xc>>4;
					*yd=yc>>4;
				};
				return;
			};
		};
	};
};
bool CorrectPositions(int* pos,word N){
	PosChanged=false;
	for(int i=0;i<N;i++){
		int ti=i+i+i;
		int xx=pos[ti];
		int yy=pos[ti+1];
		int xs=(xx-48)>>4;
		int ys=(yy-48)>>4;
		if(CheckBar(xs,ys,6,6)){
			if(!CorrectOnePosition(&xs,&ys))return false;
			pos[ti]=(xs<<4)+48;
			pos[ti+1]=(ys<<4)+48;
			PosChanged=true;
		};
	};
	return true;
};
int Brigade::SelectPeasants(byte NI){
	SelCenter[NI]=0;
	int np=0;
	for(int i=0;i<NMemb;i++){
		OneObject* OB=Group[Memb[i]];
		if(OB&&OB->Serial==MembSN[i]){
			if(OB->newMons->Peasant&&!(OB->Sdoxlo||OB->Hidden))np++;
		};
	};
	if(NSL[NI]){
		int N=NSL[NI];
		for(int i=0;i<N;i++){
			word MID=Selm[NI][i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SerN[NI][i])OB->Selected&=~GM(NI);
			};
		};
		free(Selm[NI]);
		free(SerN[NI]);
		Selm[NI]=NULL;
		SerN[NI]=NULL;
	};
	NSL[NI]=np;
	addrand(NSL[NI]+1);
	Selm[NI]=znew(word,np);
	SerN[NI]=znew(word,np);
	np=0;
	for(int i=0;i<NMemb;i++){
		OneObject* OB=Group[Memb[i]];
		if(OB&&OB->Serial==MembSN[i]){
			if(OB->newMons->Peasant&&!(OB->Sdoxlo||OB->Hidden)){
				Selm[NI][np]=OB->Index;
				SerN[NI][np]=OB->Serial;
				OB->Selected|=GM(NI);
				np++;
			};
		};
	};
	return NSL[NI];
};
//------------------------------Local send to---------------------------//
int GetTopology(int x,int y, byte LockType=0){
	int xc=x>>6;
	int yc=y>>6;
	word tr;
	if(xc<0||yc<0||xc>=TopLx||yc>=TopLy)tr=0xFFFF;
	else tr=GetTopRef(xc+(yc<<TopSH),LockType);
	if(tr<0xFFFE)return tr;
	for(int i=0;i<20;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int xx=xc+xi[j];
			int yy=yc+yi[j];
			if(xx>=0&&yy>=0&&xx<TopLx&&yy<TopLy){
				tr=GetTopRef(xx+(yy<<TopSH),LockType);
				if(tr<0xFFFE)return tr;
			};
		};
	};
	return 0xFFFF;
};
int GetTopology(int* x,int* y, byte LockType=0){
	int xc=(*x)>>6;
	int yc=(*y)>>6;
	word tr;
	if(xc<0||yc<0||xc>=TopLx||yc>=TopLy)tr=0xFFFF;
	else tr=GetTopRef(xc+(yc<<TopSH),LockType);
	if(tr<0xFFFE)return tr;
	for(int i=0;i<20;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int xx=xc+xi[j];
			int yy=yc+yi[j];
			if(xx>=0&&yy>=0&&xx<TopLx&&yy<TopLy){
				tr=GetTopRef(xx+(yy<<TopSH),LockType);
				if(tr<0xFFFE){
					*x=(xx<<6)+32;
					*y=(yy<<6)+32;
					return tr;
				};
			};
		};
	};
	return 0xFFFF;
};
class Brigade_LST:public BrigadeOrder{
public:
	int time;
	int N;
	int ZastTime;
	int PrevSumm1;
	int PrevSumm2;
	int PrevSumm3;
	int StartTime;
	int Position[30];
};
extern int tmtmt;
void TakeResLink(OneObject* OBJ);
void AttackObjLink(OneObject* OBJ);
void B_LocalSendToLink(Brigade* BR){
	Brigade_LST* BLS=(Brigade_LST*)BR->BOrder;
	if(tmtmt-BLS->StartTime>300){
		BR->DeleteBOrder();
		return;
	};
	int* pos=BLS->Position;
	int N=BLS->N;
	int S=0;
	for(int i=0;i<N;i++){
		int ti=i+i+i;
		int ID=pos[ti+2];
		word SN=ID>>13;
		ID&=8191;
		OneObject* OB=Group[ID];
		if(OB&&OB->Serial==SN&&!OB->Sdoxlo){
			S+=OB->RealX+OB->RealY;
		};
	};
	if(S==BLS->PrevSumm1&&S==BLS->PrevSumm2&&S==BLS->PrevSumm3&&tmtmt-BLS->StartTime>50){
		BR->DeleteBOrder();
		return;
	};
	byte prio=BLS->Prio;
	for(int i=0;i<N;i++){
		int ti=i+i+i;
		int ID=pos[ti+2];
		word SN=ID>>13;
		ID&=8191;
		OneObject* OB=Group[ID];
		if(OB&&OB->Serial==SN&&!OB->Sdoxlo){
			if(OB->LocalOrder){
				if(OB->EnemyID!=0xFFFF){
					BR->DeleteBOrder();
					return;
				};
				if(OB->LocalOrder->DoLink==&TakeResLink){
					OB->NewMonsterSendTo(BLS->Position[ti]<<4,BLS->Position[ti+1]<<4,(prio&127)+128,0);
				};
			};
			int dstt=Norma((OB->RealX>>4)-pos[ti],(OB->RealY>>4)-pos[ti+1]);
			if(dstt>250&&!OB->LocalOrder){
				OB->NewMonsterSendTo(BLS->Position[ti]<<4,BLS->Position[ti+1]<<4,(prio&127)+128,0);
			};
		};
	};
	BLS->PrevSumm3=BLS
		->PrevSumm2;
	BLS->PrevSumm2=BLS->PrevSumm1;
	BLS->PrevSumm1=S;
/*
	bool moved=true;
	bool zastr=false;
	if(BLS->time<-1)BLS->time++;
	if(BLS->time>=0){
		if(tmtmt-BLS->time>550){
			zastr=true;
			BLS->time=-10;
		};
	};
	int NZast=0;
	byte prio=BLS->Prio;
	for(int i=0;i<N;i++){
		int ti=i+i+i;
		int ID=pos[ti+2];
		word SN=ID>>13;
		ID&=8191;
		OneObject* OB=Group[ID];
		if(OB&&OB->Serial==SN&&!OB->Sdoxlo){
			if(OB->LocalOrder&&OB->LocalOrder->DoLink==&AttackObjLink){
				BR->DeleteBOrder();
				return;
			};
			int dstt=Norma((OB->RealX>>4)-pos[ti],(OB->RealY>>4)-pos[ti+1]);
			if(dstt>250){
				NZast++;
				if(BLS->ZastTime!=-1){
					if(tmtmt-BLS->ZastTime>800){
						if(OB->newMons->Artilery){
							BLS->ZastTime=tmtmt;
						}else OB->Die();
					};
				};
				moved=false;
				if(zastr){
					int xx=OB->RealX;
					int yy=OB->RealY;
					bool done=false;
					int nattm=0;
					while((!done)&&nattm<6){
						int xx1=xx+(rando()&2048)-1024;
						int yy1=yy+(rando()&2048)-1024;
						if(!CheckBar((xx1>>8)-2,(yy1>>8)-2,4,4)){
							done=true;
							OB->NewMonsterSmartSendTo(xx1>>4,yy1>>4,0,0,prio,0);
						};
						nattm++;
					};
					if(!done)OB->Die();
				};
			}else{
				if(BLS->time==-1)BLS->time=tmtmt;
			};
			if(dstt>=250&&!(OB->LocalOrder||OB->Sdoxlo)){
				OB->NewMonsterSmartSendTo(pos[ti],pos[ti+1],0,0,prio,0);
			};
		};
	};
	if(BLS->ZastTime==-1&&NZast&&NZast<4){
		BLS->ZastTime=tmtmt;
	};
	if(rando()<2000){
		Brigade_LST* OR1=(Brigade_LST*)BR->BOrder;
		CorrectPositions(pos,N);
		int Nmx=BR->NMemb;
		if(PosChanged){
			for(i=0;i<N&&i<Nmx;i++){
				int ti=i+i+i;
				OneObject* OB=Group[BR->Memb[i]];
				if(OB)OB->NewMonsterSendTo(OR1->Position[ti]<<4,OR1->Position[ti+1]<<4,prio,0);
			};
		};
	};
	if(moved)BR->DeleteBOrder();
	*/
};
char* LS_Mess="[Local sent to]";
bool Brigade::LinearLocalSendTo(int x,int y,byte prio,byte OrdType){
	//calculating center
	/*
	int N=0;
	int xa=0;
	int ya=0;
	for(int i=0;i<NMemb;i++){
		OneObject* OB=Group[Memb[i]];
		if(OB&&!OB->Sdoxlo){
			xa+=OB->RealX>>4;
			ya+=OB->RealY>>4;
			N++;
		};
	};
	if(!N)return false;

	xa=div(xa,N).quot;
	ya=div(ya,N).quot;
	*/
//	assert(x<20000);
	Brigade_LST* OR1=(Brigade_LST*)CreateBOrder(OrdType,sizeof(Brigade_LST)-120+(NMemb*12));
	OR1->Size=sizeof(Brigade_LST)-120+(NMemb*12);
	OR1->OrdType=1;
	OR1->BLink=&B_LocalSendToLink;
	OR1->N=NMemb;
	OR1->time=-1;
	OR1->Message=LS_Mess;
	OR1->ZastTime=-1;
	OR1->Prio=prio;
	OR1->PrevSumm1=0;
	OR1->PrevSumm2=1;
	OR1->PrevSumm3=2;
	OR1->StartTime=tmtmt;
	if(!PosCreated)CreateConvoy(0);
	int avx=0;
	int avy=0;
	int na=0;
	for(int i=0;i<NMemb;i++){
		OneObject* OB=Group[Memb[i]];
		if(OB){
			avx+=(OB->RealX>>4);
			avy+=(OB->RealY>>4);
			na++;
		};
	};
	if(!na){
		DeleteBOrder();
		return false;
	};
	avx/=na;
	avy/=na;
	int DX=x-avx;
	int DY=y-avy;
	int NR=Norma(DX,DY);
	if(!NR){
		DeleteBOrder();
		return false;
	};
	int dx1=DY*70/NR;
	int dy1=-DX*70/NR;
	int dx0=(-dx1*na)>>1;
	int dy0=(-dy1*na)>>1;
	for(int i=0;i<NMemb;i++){
		int ti=i+i+i;
		OR1->Position[ti  ]=x+dx0;
		OR1->Position[ti+1]=y+dy0;
		OR1->Position[ti+2]=Memb[i]+(int(MembSN[i])<<13);
		dx0+=dx1;
		dy0+=dy1;
	};
	if(!CorrectPositions(OR1->Position,NMemb)){
		DeleteBOrder();
		return false;
	};
	for(int i=0;i<NMemb;i++){
		int ti=i+i+i;
		OneObject* OB=Group[Memb[i]];
		if(OB)OB->NewMonsterSendTo(OR1->Position[ti]<<4,OR1->Position[ti+1]<<4,prio,0);
	};
	return true;
};
bool Brigade::LocalSendTo(int x,int y,byte prio,byte OrdType){
	if(WarType){
		int N=0;
		int xa=0;
		int ya=0;
		for(int i=NBPERSONAL;i<NMemb;i++)if(Memb[i]!=0xFFFF){
			OneObject* OB=Group[Memb[i]];
			if(OB&&!OB->Sdoxlo){
				xa+=OB->RealX>>4;
				ya+=OB->RealY>>4;
				N++;
			};
		};
		if(!N)return false;

		xa=div(xa,N).quot;
		ya=div(ya,N).quot;
		HumanLocalSendTo(x<<4,y<<4,GetDir(x-xa,y-ya),prio,OrdType);
		return true;
	};
	//calculating center
	/*
	int N=0;
	int xa=0;
	int ya=0;
	for(int i=0;i<NMemb;i++){
		OneObject* OB=Group[Memb[i]];
		if(OB&&!OB->Sdoxlo){
			xa+=OB->RealX>>4;
			ya+=OB->RealY>>4;
			N++;
		};
	};
	if(!N)return false;

	xa=div(xa,N).quot;
	ya=div(ya,N).quot;
	*/
//	assert(x<20000);
	Brigade_LST* OR1=(Brigade_LST*)CreateBOrder(OrdType,sizeof(Brigade_LST)-120+(NMemb*12));
	OR1->Size=sizeof(Brigade_LST)-120+(NMemb*12);
	OR1->OrdType=1;
	OR1->BLink=&B_LocalSendToLink;
	OR1->N=NMemb;
	OR1->time=-1;
	OR1->Message=LS_Mess;
	OR1->ZastTime=-1;
	OR1->Prio=prio;
	OR1->PrevSumm1=0;
	OR1->PrevSumm2=1;
	OR1->PrevSumm3=2;
	OR1->StartTime=tmtmt;
	if(!PosCreated)CreateConvoy(0);
	for(int i=0;i<NMemb;i++){
		int ti=i+i+i;
		OR1->Position[ti  ]=x+posX[i];
		OR1->Position[ti+1]=y+posY[i];
		OR1->Position[ti+2]=Memb[i]+(int(MembSN[i])<<13);
	};
	if(!CorrectPositions(OR1->Position,NMemb)){
		DeleteBOrder();
		return false;
	};
	for(int i=0;i<NMemb;i++){
		int ti=i+i+i;
		OneObject* OB=Group[Memb[i]];
		if(OB&&OB->Serial==MembSN[i])OB->NewMonsterSendTo(OR1->Position[ti]<<4,OR1->Position[ti+1]<<4,prio,0);
	};
	return true;
};

void B_WideLocalSendToLink(Brigade* BR){
	Brigade_LST* BLS=(Brigade_LST*)BR->BOrder;
	int* pos=BLS->Position;
	int N=BLS->N;
	bool moved=true;
	bool zastr=false;
	if(BLS->time<-1)BLS->time++;
	if(BLS->time>=0){
		if(tmtmt-BLS->time>350){
			zastr=true;
			BLS->time=-10;
		};
	};
	int NZast=0;
	byte prio=BLS->Prio;
	for(int i=0;i<N;i++){
		int ti=i+i+i;
		int ID=pos[ti+2];
		word SN=ID>>13;
		ID&=8191;
		OneObject* OB=Group[ID];
		if(OB&&OB->Serial==SN&&!OB->Sdoxlo){
			if(OB->EnemyID!=0xFFFF){
				BR->DeleteBOrder();
				return;
			};
			int dstt=Norma((OB->RealX>>4)-pos[ti],(OB->RealY>>4)-pos[ti+1]);
			if(dstt>300){
				NZast++;
				if(BLS->ZastTime!=-1){
					if(tmtmt-BLS->ZastTime>600){
						if(OB->newMons->Artilery){
							BLS->ZastTime=tmtmt;
						}else OB->Die();
					};
				};
				moved=false;
				if(zastr){
					int xx=OB->RealX;
					int yy=OB->RealY;
					bool done=false;
					int nattm=0;
					while((!done)&&nattm<6){
						int xx1=xx+(rando()&2048)-1024;
						int yy1=yy+(rando()&2048)-1024;
						if(!CheckBar((xx1>>8)-2,(yy1>>8)-2,4,4)){
							done=true;
							OB->NewMonsterSendTo(xx1,yy1,prio,0);
						};
						nattm++;
					};
					if(!done)OB->Die();
				};
			}else{
				if(BLS->time==-1)BLS->time=tmtmt;
			};
			if(dstt>=300&&!(OB->LocalOrder||OB->Sdoxlo)){
				OB->NewMonsterSendTo(pos[ti]<<4,pos[ti+1]<<4,prio,0);
			};
		};
	};
	if(BLS->ZastTime==-1&&NZast&&NZast<4){
		BLS->ZastTime=tmtmt;
	};
	if(rando()<2000){
		Brigade_LST* OR1=(Brigade_LST*)BR->BOrder;
		CorrectPositions(pos,N);
		int Nmx=BR->NMemb;
		if(PosChanged){
			for(int i=0;i<N&&i<Nmx;i++){
				int ti=i+i+i;
				OneObject* OB=Group[BR->Memb[i]];
				if(OB&&OB->Serial==BR->MembSN[i])OB->NewMonsterSendTo(OR1->Position[ti]<<4,OR1->Position[ti+1]<<4,prio,0);
			};
		};
	};
	if(moved)BR->DeleteBOrder();
};
char* WLS_Mess="[Wide Local sent to]";
bool Brigade::WideLocalSendTo(int x,int y,byte prio,byte OrdType){
	//calculating center
	/*
	int N=0;
	int xa=0;
	int ya=0;
	for(int i=0;i<NMemb;i++){
		OneObject* OB=Group[Memb[i]];
		if(OB&&!OB->Sdoxlo){
			xa+=OB->RealX>>4;
			ya+=OB->RealY>>4;
			N++;
		};
	};
	if(!N)return false;

	xa=div(xa,N).quot;
	ya=div(ya,N).quot;
	*/
//	assert(x<20000);
	Brigade_LST* OR1=(Brigade_LST*)CreateBOrder(OrdType,sizeof(Brigade_LST)-120+(NMemb*12));
	OR1->Size=sizeof(Brigade_LST)-120+(NMemb*12);
	OR1->OrdType=1;
	OR1->BLink=&B_WideLocalSendToLink;
	OR1->N=NMemb;
	OR1->time=-1;
	OR1->Message=LS_Mess;
	OR1->ZastTime=-1;
	OR1->Prio=prio;
	if(!PosCreated)CreateConvoy(1);
	for(int i=0;i<NMemb;i++){
		int ti=i+i+i;
		OR1->Position[ti  ]=x+posX[i];
		OR1->Position[ti+1]=y+posY[i];
		OR1->Position[ti+2]=Memb[i]+(int(MembSN[i])<<13);
	};
	if(!CorrectPositions(OR1->Position,NMemb)){
		DeleteBOrder();
		return false;
	};
	for(int i=0;i<NMemb;i++){
		int ti=i+i+i;
		OneObject* OB=Group[Memb[i]];
		if(OB&&OB->Serial==MembSN[i])OB->NewMonsterSendTo(OR1->Position[ti]<<4,OR1->Position[ti+1]<<4,prio,0);
	};
	return true;
};
int Brigade::AddInRadius(int x,int y,int r,BrigMemb* BMem,Brigade* Dest){
	word* IDIS=znew(word,NMemb);
	int NIDS=0;
	//word* NKINDS=&BMem->Peons;
	word* NKI=&BMem->Peons;
	//SortClass SORT;
	for(int i=0;i<6;i++){
		word id1=IDIS[i];
		int n=0;
		for(int j=0;j<NMemb;j++){
			OneObject* OB=Group[Memb[j]];
			if(OB&&Norma((OB->RealX>>4)-x,(OB->RealY>>4)-y)<r){
				int k=GetBMIndex(OB);
				if(NKI[k]){
					IDIS[NIDS]=j;
					NKI[k]--;
				};
			};
		};
	};
	int nn=0;
	for(int i=0;i<NIDS;i++){
		if(RemoveOne(IDIS[i]-nn,Dest)){
			nn++;
		};
	};
	free(IDIS);
	return BMem->Grenaderov+BMem->Infantry+BMem->Mortir+BMem->Peons+BMem->Pushek+BMem->Strelkov;
};
void BuildObjLink(OneObject* OBJ);
void GoToMineLink(OneObject* OB);
int Brigade::AddInRadius(int x,int y,int r,BrigMemb* BM){
	int xc=x>>7;
	int yc=y>>7;
	r=(r>>7)+1;
	if(r>=RRad)r=RRad-1;
	int maxx=msx>>2;
	int maxy=msy>>2;
	byte NI=CT->NI;
	word MyID=ID;
	int M=BM->Grenaderov+BM->Infantry+BM->Mortir+BM->Peons+BM->Pushek+BM->Strelkov;
	word* mem=(word*)BM;
	for(int i=0;i<r;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int xx=xc+xi[j];
			int yy=yc+yi[j];
			if(xx>=0&&yy>=0&&xx<=maxx&&yy<=maxy){
				int ofst=xx+1+((yy+1)<<VAL_SHFCX);
				int N1=MCount[ofst];
				if(N1){
					ofst<<=SHFCELL;
					for(int k=0;k<N1;k++){
						word MID=GetNMSL(ofst+k);
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&OB->NNUM==NI&&!(OB->DoNotCall||OB->Sdoxlo||OB->InArmy)){
								byte nn=GetBMIndex(OB);
								if(mem[nn]){
									bool Allow=true;
									Order1* OR1=OB->LocalOrder;
									if(OR1&&OR1->DoLink==NewMonsterSendToLink&&OR1->NextOrder)OR1=OR1->NextOrder;
									if(OR1&&OR1->DoLink==NewMonsterSmartSendToLink&&OR1->NextOrder)OR1=OR1->NextOrder;
									if((!OR1)||(OR1->DoLink!=&BuildObjLink&&OR1->DoLink!=GoToMineLink)){
										if(OB->BrigadeID!=0xFFFF){
											if(OB->BrigadeID!=MyID){
												Brigade* SRC=CT->Brigs+OB->BrigadeID;
												if(SRC->Memb&&SRC->Memb[OB->BrIndex]==OB->Index){
													SRC->RemoveOne(OB->BrIndex,this);
													mem[nn]--;
													M--;
												};
											};
										}else{
											AddObject(OB);
											mem[nn]--;
											M--;
										};
										if(!M)return 0;
									};
								};
							};
						};
					};
				};
			};
		};
	};
	return M; 
};
//------------------------------Global send to---------------------------//
class Brigade_GST:public BrigadeOrder{
public:
	bool Final;
	int DestX;
	int DestY;
};
void B_GlobalSendToLink(Brigade* BR){
	Brigade_GST* OR1=(Brigade_GST*)BR->BOrder;
	if(OR1->Final){
		BR->DeleteBOrder();
		return;
	};
	int xc=0;
	int yc=0;
	int N=0;
	int NM=BR->NMemb;
	int Dsx=OR1->DestX;
	int Dsy=OR1->DestY;
	word Top=GetTopology(Dsx,Dsy);
	if(Top==0xFFFF){
		BR->DeleteBOrder();
		return;
	};
	for(int i=0;i<NM;i++){
		OneObject* OB=Group[BR->Memb[i]];
		if(OB&&OB->Serial==BR->MembSN[i]){
			xc+=OB->RealX>>4;
			yc+=OB->RealY>>4;
			N++;
		};
	};
	if(N){
		xc=div(xc,N).quot;
		yc=div(yc,N).quot;
		word Top1=GetTopology(xc,yc);
		if(Top1==0xFFFF){
			BR->DeleteBOrder();
			return;
		};
		word NextTop=GetMotionLinks(Top*GetNAreas()+Top1);
		if(NextTop==Top){
			OR1->Final=true;
			BR->LocalSendTo(Dsx,Dsy,128,1);
		}else{
			Area* AR=GetTopMap(Top1);
			BR->LocalSendTo((AR->x<<6)+32,(AR->y<<6)+32,128,1);
		};
	}else{
		BR->DeleteBOrder();
	};
};
char* GS_Mess="[Global send to]";
bool Brigade::GlobalSendTo(int x,int y,byte prio,byte OrdType){
	Brigade_GST* OR1=(Brigade_GST*)CreateBOrder(OrdType,sizeof(Brigade_GST));
	OR1->DestX=x;
	OR1->DestY=y;
	OR1->Final=false;
	OR1->Message=GS_Mess;
	OR1->Size=sizeof(Brigade_GST);
	OR1->BLink=&B_GlobalSendToLink;
	return true;
};
//---------------------------Capture mine-------------------------//
class Brigade_CM:public BrigadeOrder{
public:
	int time;
	int Stage;
	int SID;
	int  DestX;
	int  DestY;
};
void AllowBuilder(Brigade* BR){
	int N=BR->NMemb;
	for(int i=0;i<N;i++){
		word mid=BR->Memb[i];
		if(mid!=0xFFFF){
			OneObject* OB=Group[mid];
			if(OB&&OB->Serial==BR->MembSN[i]){
				OB->NoBuilder=false;
			};
		};
	};
};
OneObject* DetermineMineBySprite(int Spr);
void MoveAway(int x,int y);
void B_CaptureMineLink(Brigade* BR){
	
};
char* CM_Mess="[Capture mine]";
bool Brigade::CaptureMine(int SID,byte prio,byte OrdType){
	Brigade_CM* OR1=(Brigade_CM*)CreateBOrder(OrdType,sizeof(Brigade_CM));
	OneSprite* OS=&Sprites[SID];
	if(!OS->Enabled)return false;
	OR1->DestX=OS->x;
	OR1->DestY=OS->y;
	OR1->Message=CM_Mess;
	OR1->SID=SID;
	OR1->time=0;
	OR1->Stage=0;
	OR1->Size=sizeof(Brigade_CM);
	OR1->BLink=&B_CaptureMineLink;
	return true;
};
void PanicUnit(OneObject* OB);
void Brigade::Rospusk(){
	lastTimeGetCenter=0;
	int N=NMemb;
	bool Panic=0;
	if(EngSettings.MoraleType==0&&WarType&&Morale<32*10000)Panic=1;
	for(int i=0;i<N;i++){
		word mid=Memb[i];
		if(mid!=0xFFFF){
			OneObject* OB=Group[mid];
			if(OB&&OB->Serial==MembSN[i]){
				OB->UnitSpeed=64;
				OB->CurUnitSpeed=64;
				OB->GroupSpeed=OB->newMons->MotionDist;//(OB->newMons->MotionDist*OB->MoreCharacter->Speed)/100;
				if(OB->newMons->Peasant){
					RemoveOne(i,&CT->Free);
					if(Panic)PanicUnit(OB);
					i--;
					N--;
				}else{
					RemoveOne(i,&CT->FreeArmy);
					if(Panic)PanicUnit(OB);
					i--;
					N--;
				};
			};
		};
	};
};
//----------------------------------WAR----------------------------------
class Brigade_BTL:public BrigadeOrder{
public:
	int xdest;
	int ydest;
	int time;
	word BestEnemy;
	word EnemyList[64];
	word NEnemy;
};
void B_MakeBattleLink(Brigade* BR){
	Brigade_BTL* OR1=(Brigade_BTL*)BR->BOrder;
	if(!BR->NMemb){
		BR->DeleteAll();
		BR->Enabled=false;
	};
	int xc=0;
	int yc=0;
	int N=0;
	int NM=BR->NMemb;
	for(int i=0;i<NM;i++){
		word mid=BR->Memb[i];
		if(mid!=0xFFFF){
			OneObject* OB=Group[mid];
			if(OB&&OB->Serial==BR->MembSN[i]){
				//if(OB->LocalOrder&&OB->LocalOrder->DoLink==&AttackObjLink)return;
				if(OB->EnemyID!=0xFFFF)return;
				OB->DoNotCall=true;
				OB->NoBuilder=true;
				xc+=OB->RealX>>4;
				yc+=OB->RealY>>4;
				N++;
			};
		};
	};
	if(N){
		xc=div(xc,N).quot;
		yc=div(yc,N).quot;
	}else{
		BR->DeleteAll();
		BR->Enabled=false;
		return;
	};
	int Dsx=xc;
	int Dsy=yc;
	if(!GNFO.FindNearestEnemy(BR->CT->NI,&Dsx,&Dsy,false,0,1)){
		return;
	};
	word Top=GetTopology(Dsx,Dsy);
	if(Top==0xFFFF){
		//BR->DeleteBOrder();
		return;
	};
	word Top1=GetTopology(xc,yc);
	if(Top1==0xFFFF){
		BR->DeleteBOrder();
		return;
	};
	word NextTop=GetMotionLinks(Top1*GetNAreas()+Top,0,BR->CT->NI);
	if(NextTop==0xFFFF)return;
	if(NextTop==Top){
		//OR1->Final=true;
		BR->LocalSendTo(Dsx,Dsy,128,1);
	}else{
		Area* AR=GetTopMap(NextTop);
		BR->LocalSendTo((AR->x<<6)+32,(AR->y<<6)+32,128,1);
	};
};
void SetStandState(Brigade* BR,byte State);
char* BTL_Mess="[Make Battle]";
void Brigade::MakeBattle(){
	Brigade_BTL* OR1=(Brigade_BTL*)CreateBOrder(0,sizeof(Brigade_BTL));
	OR1->Message=BTL_Mess;
	OR1->time=0;
	OR1->xdest=-1;
	OR1->ydest=-1;
	OR1->Size=sizeof(Brigade_BTL);
	OR1->BLink=&B_MakeBattleLink;
	return;	
};
//----------------------------------WAR2----------------------------//
int GetDamage(word ShooterID,word VictimID,int AttType);
class Brigade_Battle:public BrigadeOrder{
public:
	word BrigID;
	int x;
	int y;
	bool Process(Brigade *BR);
	bool FirstProces;
	word EnemyList[512];
	word EnemyLife[512];
	word EnemyFireCount[512];
	int EnemyX;
	int EnemyY;
	static int EX;
	static int EY;
	word NEnemy;
	word NEnemyLife;
	word FireOrder[256];
	void FillEnemyList(Brigade* BR);
	int FillTime;
	bool SetTargetObj(OneObject *OB);
	static bool AddEnemy(OneObject* OB,void* param);
	static int compare(const void* p1,const void* p2);
	word OnlyOneBrig;
	void ClearTargetObj(Brigade *BR);
	void ClearLastOrders(Brigade *BR);
};
int Brigade_Battle::EX=0;
int Brigade_Battle::EY=0;
void Brigade_Battle::ClearTargetObj(Brigade *BR)
{
	if(BR)
	{
		for(int i=0;i<BR->NMemb;i++)
		{
			OneObject *OB=Group[BR->Memb[i]];
			if(OB&&OB->Attack)
			{
				if(OB->LocalOrder->DoLink==AttackObjLink)
					OB->DeleteLastOrder();
			}
		}
	}
}
void Brigade_Battle::ClearLastOrders(Brigade *BR)
{
	if(BR)
	{
		for(int i=0;i<BR->NMemb;i++)
		{
			OneObject *OB=Group[BR->Memb[i]];
			if(OB)
			{
				OB->DeleteLastOrder();
			}
		}
	}
}
bool Brigade_Battle::Process(Brigade *BR)
{
	bool rez=true;
	if(FillTime<AnimTime)
	{
		bool Rec=true;
		for(int i=0;i<BR->NMemb&&Rec;i++)
		{
			OneObject *OB=Group[BR->Memb[i]];
  			if(OB&&OB->RifleAttack&&OB->delay==0&&OB->EnemyID!=0xFFFF)
			{
 				OneObject *OBt=Group[OB->EnemyID]; 
				if(OBt&&(!OBt->Sdoxlo))
					Rec=false;
			}
		}
		if(Rec)
		{
			FillEnemyList(BR);
			//FillTime=tmtmt+((160*GameSpeed)>>8);  
 			FillTime=AnimTime+(42+29+21)*256; 
			rez=false;
		}
	}
	NEnemyLife=NEnemy;
	for(int i=0;i<BR->NMemb&&NEnemyLife>0;i++)
	{
		if(FireOrder[i]!=0xFFFF)
		{
			OneObject *OB=Group[FireOrder[i]];
			if(OB&&OB->RifleAttack&&OB->delay==0)
			{
				if(OB&&(!OB->Attack))
				{
					if(SetTargetObj(OB))
						rez=true;
				}
				else
				if(OB&&OB->Attack)
				{
					if(OB->EnemyID!=0xFFFF)
					{
						OneObject* BB=Group[OB->EnemyID]; 
						if(BB&&BB->Sdoxlo)
						{
							if(SetTargetObj(OB))
								rez=true;
						}
					}
					else
						if(SetTargetObj(OB))
							rez=true;
				}
			}
			else
				if(OB&&(OB->Index==BR->Memb[0]||OB->Index==BR->Memb[1]||OB->Index==BR->Memb[2]))
					OB->ClearOrders();
		}
		NEnemyLife=NEnemy;
	}
	return rez;
}
bool Brigade_Battle::SetTargetObj(OneObject *OB)
{
	bool rez=false;
	int dist=9999999;
	int TIndex=-1;
	int ni=NEnemy/8+5; 
	for(int i=0;i<ni;i++)
	{
		int rn=rando()%NEnemy;
		OneObject *tOB=NULL;
		tOB=Group[EnemyList[rn]];
		if(tOB&&(!tOB->Sdoxlo))
		{
			if(tOB->Life>EnemyLife[rn]&&((EnemyFireCount[rn]<5&&tOB->Life<101)||tOB->Life>100))
			{
				int nd=Norma((OB->RealX>>4)-(tOB->RealX>>4),(OB->RealY>>4)-(tOB->RealY>>4));
				if(OB->Usage==EgerID)
				{
					nd=nd/(tOB->Ref.General->NeedRes[1]+1);
				}
				//int sea=OB->newMons->VisRange>>4;
				int sea=OB->MoreCharacter->VisRange>>4;
				if(nd<dist&&nd<=sea)
				{
					Coor3D C3D;
					tOB->GetDamagePoint(OB->x,OB->y,&C3D,0);
					int x0,y0,z0,x1,y1,z1;
					x0=(OB->RealX>>4);
					y0=(OB->RealY>>4);
					z0=OB->RZ+32;
					x1=C3D.x;
					y1=C3D.y;
					z1=C3D.z;
					int CheckBreaksBetweenPoints(int x,int y,int z,int xd,int yd,int zd,int Index);
					//int r=CheckBreaksBetweenPoints(x0,y0,z0,x1,y1,z1,OB->Index);
					//if(r==-1)
					{
						dist=nd;
						TIndex=rn;
					}
				}
			}
		}
	}
	if(TIndex!=-1)
	{
		if(OB->AttackObj(EnemyList[TIndex],128+15,0,1))
		{
			EnemyLife[TIndex]+=GetDamage(OB->Index,EnemyList[TIndex],1);
			EnemyFireCount[TIndex]++;
			rez=true;
		}
	}
	
	return rez;
}

typedef bool tpUnitsCallback(OneObject* OB,void* param);
int PerformActionOverUnitsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
void Brigade_Battle::FillEnemyList(Brigade* BR)
{
	byte dir;
	BR->GetCenter(&x,&y,&dir);
	NEnemy=0;
	memset(EnemyList,0,512*2);
	memset(EnemyLife,0,512*2);
	memset(EnemyFireCount,0,512*2);
	word mask=BR->GetNMask();
	EnemyX=0;
	EnemyY=0;
	NEnemy=0;
	word* ex=(word*)(&EnemyX);
	word* ey=(word*)(&EnemyY);
	word* pr[6]={EnemyList,&NEnemy,&mask,ex,ey,&OnlyOneBrig};
	PerformActionOverUnitsInRadius(x,y,1200,&Brigade_Battle::AddEnemy,(void*)pr);
	if(NEnemy)
	{
		EnemyX=EnemyX/NEnemy;
		EnemyY=EnemyY/NEnemy;
	}
	for(int i=0;i<BR->NMemb;i++)
		FireOrder[i]=BR->Memb[i];
	EX=EnemyX;
	EY=EnemyY;
	addrand(NEnemy);
	for(int j=0;j<NEnemy;j++)addrand(EnemyList[j]);
	if(NEnemy<40)
	{
		qsort(FireOrder,BR->NMemb,sizeof(word),Brigade_Battle::compare);
	}
	for(int j=0;j<NEnemy;j++)addrand(EnemyList[j]);
}
int Brigade_Battle::compare(const void *p1,const void *p2)
{
	int rez=0;
	word* i1=(word*)p1;
	word* i2=(word*)p2;
	if((*i1)==0xFFFF)
		rez++;
	if((*i2)==0xFFFF)
		rez--;
	if((*i1)!=0xFFFF&&(*i2)!=0xFFFF)
	{
		OneObject* OB1=Group[(*i1)];
		OneObject* OB2=Group[(*i2)];
		if(OB1&&OB2)
		{
			int d1=Norma((OB1->RealX>>4)-EX,(OB1->RealY>>4)-EY);
			int d2=Norma((OB2->RealX>>4)-EX,(OB2->RealY>>4)-EY);
			rez=d1-d2;
		}   
	}
	return rez;
}
bool Brigade_Battle::AddEnemy(OneObject *OB, void *param)
{
	bool rez=false;
	word** pr=(word**)param;
	word* EL=pr[0];
	word* Nen=pr[1];
	int* ex=(int*)pr[3];
	int* ey=(int*)pr[4];
	word* OnlyBrig=pr[5];
	byte mask=(byte)(*(pr[2]));
	if((!(mask&OB->NMask))&&(((*OnlyBrig)==0xFFFF)||((*OnlyBrig)==OB->BrigadeID))&&!OB->Sdoxlo)
		if((*Nen)<240)
		{
			EL[(*Nen)]=OB->Index;
			(*Nen)++;
			(*ex)+=(OB->RealX>>4);
			(*ey)+=(OB->RealY>>4);
			rez=true;
		}
	return rez;
}
void B_BrigadeBattleLink(Brigade* BR)
{
	Brigade_Battle* OR1=(Brigade_Battle*)BR->BOrder;
	if(!OR1->FirstProces)
	{
		OR1->ClearTargetObj(BR);
		OR1->ClearLastOrders(BR);
		OR1->FirstProces=true;
		OR1->FillTime=0;
	}
	if(!BR->NMemb)
	{
		BR->DeleteAll();
		BR->Enabled=false;
		BR->AttEnm=0;
	}
	else
		if(!OR1->Process(BR))
		{
			BR->DeleteBOrder();
			OR1->ClearTargetObj(BR);
			for(int i=0;i<BR->NMemb;i++)
			{
				OneObject* OB=Group[BR->Memb[i]];
				if(OB)OB->RifleAttack=false;
			}
		}
}
extern City CITY[8];
/*
void BrigadeRifleAttack(OneObject *OB)
{
	//if(rando()>2048)return;
	Brigade* BR=&(CITY[OB->NNUM].Brigs[OB->BrigadeID]);
	if((!BR->BOrder)||(BR->BOrder->BLink!=&B_BrigadeBattleLink))
	{
		//BR->AttEnm=1;
		Brigade_Battle* BB=(Brigade_Battle*)BR->CreateBOrder(1,sizeof(Brigade_Battle));
		BB->Prio=0;
		BB->Size=sizeof(Brigade_Battle);
		BB->BLink=&B_BrigadeBattleLink;
		bool OnlyW = false;
		if (OB->SearchOnlyThisBrigadeToKill!=0xFFFF) 
		{
			for(int i=0;i<8;i++)
			{
				Brigade* BR=&(CITY[i].Brigs[OB->SearchOnlyThisBrigadeToKill]);
				if(BR&&BR->WarType>0)
					if (!(OB->NMask&BR->GetNMask())) 
					{
						OnlyW=true;
						break;
					}
			}
		}
		if(OnlyW)
			BB->OnlyOneBrig=OB->SearchOnlyThisBrigadeToKill;
		else
			BB->OnlyOneBrig=0xFFFF;
		B_BrigadeBattleLink(BR);
	}
}
*/
void BrigadeRifleAttack(OneObject *OB)
{
	//if(rando()>2048)return;
	addrand(OB->Index);
	Brigade* BR=&(CITY[OB->NNUM].Brigs[OB->BrigadeID]);
	if((!BR->NewBOrder)||(BR->NewBOrder&&BR->NewBOrder->GetBrigadeOrderID()!=BRIGADEORDER_RIFLEATTACK))
	{
		BrigadeOrder_RifleAttack* RA = new BrigadeOrder_RifleAttack();
		
		bool OnlyW = false;
		if (OB->SearchOnlyThisBrigadeToKill!=0xFFFF) 
		{
			for(int i=0;i<8;i++)
			{
				Brigade* BR=&(CITY[i].Brigs[OB->SearchOnlyThisBrigadeToKill]);
				if(BR&&BR->WarType>0)
					if (!(OB->NMask&BR->GetNMask())) 
					{
						OnlyW=true;
						break;
					}
			}
		}
		if(OnlyW)
			RA->OnlyOneBrig=OB->SearchOnlyThisBrigadeToKill;
		else
			RA->OnlyOneBrig=0xFFFF;
		BR->CreateNewBOrder(1,RA);
		addrand(BR->ID);
		RA->Process(BR);
	}
}
//---------------------------Protect mines--------------------------//
class  PFM_InfoSingle:public Inform{
public:
	int time;
};
class  Brigade_PFM:public BrigadeOrder{
public:
	word SID;
	word M_ID;
	word M_SN;
	int  xtop;
	int  ytop;
	word TowID;
	word TowSN;
	word Delay;
	word Time;
};
MineBase* CorrectMB(MineBase* MB){
	if(MB->ID!=0xFFFF){
		OneObject* OB=Group[MB->M_ID];
		if(OB&&OB->Serial==MB->M_SN)return MB;
		else return NULL;
	}else return NULL;
};
void B_ProtectOrder(Brigade* BR){
	/*
	Brigade_PFM* BPF=(Brigade_PFM*)BR->BOrder;
	if(BPF->Delay){
		BPF->Delay--;
		return;
	};
	if(BPF->SID==0xFFFF){
		//need to search base to upgrade
		word BestSID=0xFFFF;
		int MostDist=0;
		MineBase* MBS=NULL;
		int Cx=(BR->CT->CenterX<<1);
		int Cy=(BR->CT->CenterY<<1);
		do{
			MBS=(MineBase*)BR->CT->SearchInform(0x4519,(Inform*)MBS);
			if(MBS&&CorrectMB(MBS)&&!MBS->PSend){
				if(MBS->TowID==0xFFFF){
					int dis=Norma(MBS->topx-Cx,MBS->topy-Cy);
					if(dis>MostDist){
						MostDist=dis;
						BestSID=MBS->Essence;
					};
				};
			};
		}while(MBS);
		if(BestSID!=0xFFFF){
			//OK! Helpless base is found now
			MineBase* MBS=(MineBase*)BR->CT->SearchInform(0x4519,BestSID,NULL);
			BPF->M_ID=MBS->M_ID;
			BPF->M_SN=MBS->M_SN;
			BPF->SID=MBS->Essence;
			BPF->xtop=MBS->topx;
			BPF->ytop=MBS->topy;
			MBS->PSend=1;
			return;
		}else{
			BPF->Delay=30;
		};
	}else{
		if(!BR->NMemb){
			BR->DeleteAll();
			BR->Enabled=false;
			return;
		};
		int xc=0;
		int yc=0;
		int N=0;
		int NM=BR->NMemb;
		for(int i=0;i<NM;i++){
			OneObject* OB=Group[BR->Memb[i]];
			if(OB&&OB->Serial==BR->MembSN[i]){
				if(OB->LocalOrder&&OB->LocalOrder->DoLink==&AttackObjLink)return;
				OB->DoNotCall=true;
				OB->NoBuilder=true;
				xc+=OB->RealX>>4;
				yc+=OB->RealY>>4;
				N++;
			};
		};
		if(N){
			xc=div(xc,N).quot;
			yc=div(yc,N).quot;
		}else{
			BR->DeleteAll();
			BR->Enabled=false;
			return;
		};
		int Dsx=BPF->xtop<<6;
		int Dsy=BPF->ytop<<6;
		word Top=GetTopology(Dsx,Dsy);
		if(Top==0xFFFF){
			//BR->DeleteBOrder();
			return;
		};
		word Top1=GetTopology(xc,yc);
		if(Top1==0xFFFF){
			BR->DeleteBOrder();
			return;
		};
		word NextTop=GetMotionLinks[Top1*GetNAreas()+Top];
		if(NextTop==0xFFFF&&Top!=Top1)return;
		if(NextTop==Top||NextTop==0xFFFF){
			if(BPF->TowID==0xFFFF){
				//attempt to cteate tower
				OneObject* OB=Group[BPF->M_ID];
				if(OB&&OB->Serial==BPF->M_SN){
					int xc=OB->RealX;
					int yc=OB->RealY;
					int MaxHi=0;
					int Mindis=100000;
					int BestX=-1;
					int BestY=-1;
					Nation* NT=BR->CT->Nat;
					GeneralObject* GO=NT->Mon[NT->UID_TOWER];
					NewMonster* NM=GO->newMons;
					for(int i=0;i<10;i++){
						int x1=xc+(rando()&16383)-8192;
						int y1=yc+(rando()&16383)-8192;
						int ID=CheckCreationAbility(NM,&x1,&y1);
						if(ID!=-1){
							int H=GetHeight(x1>>4,y1>>4);
							int dis=Norma(x1-xc,y1-yc);
							if(H>MaxHi||(H<=MaxHi&&dis<Mindis)){
								Mindis=dis;
								MaxHi=H;
								BestX=x1;
								BestY=y1;
							};
						};
					};
					if(BestX!=-1){
						int ID=NT->CreateNewMonsterAt(BestX,BestY,NT->UID_TOWER,false);
						if(ID!=-1){
							BPF->TowID=ID;
							OneObject* TOB=Group[ID];
							BPF->TowSN=TOB->Serial;
							MineBase* MBS=(MineBase*)BR->CT->SearchInform(0x4519,BPF->SID,NULL);
							if(MBS){
								MBS->TowID=ID;
								MBS->TowSN=TOB->Serial;
								MBS->PSend=0;
							};
							return;
						}else return;
					};
					return;
				}else{
					BPF->SID=0xFFFF;
					BPF->M_ID=0xFFFF;
					BPF->M_SN=0xFFFF;
					return;
				};
			};
			OneObject* TOB=Group[BPF->TowID];
			if(TOB->Serial==BPF->TowSN){
				if(Norma((TOB->RealX>>4)-xc,(TOB->RealY>>4)-yc)>140){
					BR->LocalSendTo(TOB->RealX>>4,TOB->RealY>>4,128,1);
					return;
				}else{
					if(TOB->Life==TOB->MaxLife){
						BPF->SID=0xFFFF;
						BPF->TowID=0xFFFF;
						return;
					};
					BR->SelectPeasants(BR->CT->NI);
					BuildWithSelected(BR->CT->NI,TOB->Index,1);
					BPF->Delay=20;
				};
			};
		}else{
			Area* AR=GetTopMap+NextTop;
			BR->LocalSendTo((AR->x<<6)+32,(AR->y<<6)+32,128,1);
		};	
	};
	*/
};
char* PFM_Mess="[Protect mines]";
void Brigade::ProtectFarMines(){
	Brigade_PFM* BPF=(Brigade_PFM*)CreateBOrder(0,sizeof Brigade_PFM);
	BPF->Delay=0;
	BPF->Time=0;
	BPF->TowID=0xFFFF;
	BPF->TowSN=0xFFFF;
	BPF->M_ID=0xFFFF;
	BPF->M_SN=0xFFFF;
	BPF->SID=0xFFFF;
	BPF->xtop=0;
	BPF->ytop=0;
	BPF->BLink=&B_ProtectOrder;
	BPF->Message=PFM_Mess;
	BPF->Prio=0;
	BPF->Size=sizeof Brigade_PFM;
	BOrder=BPF;
};
bool Brigade::GetCenter(int* x,int* y,byte* Dir){
	*x=0;*y=0;*Dir=0;
	if(!NMemb)return false;
	if(tmtmt&&lastTimeGetCenter==tmtmt){
		*x=lastCenterX;
		*y=lastCenterY;
		*Dir=lastDir;
		return true;
	}
	int xx=0;
	int yy=0;
	int DX=0;
	int DY=0;
	int N=0;
	for(int i=0;i<NMemb;i++){
		word MID=Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==MembSN[i]){
				xx+=(OB->RealX>>4);
				yy+=(OB->RealY>>4);
				//DX+=TCos[OB->RealDir];
				//DY+=TSin[OB->RealDir];
				N++;
			};
		};
	};
	if(N){
		*x=xx/N;
		*y=yy/N;
		//*Dir=GetDir(DX,DY);
		*Dir=GetBrigadeDirectionByUnitPositions(this);
		lastCenterX=*x;
		lastCenterY=*y;
		lastDir=*Dir;
		lastTimeGetCenter=tmtmt;
		return true;
	}else return false;
}; 
bool Brigade::GetCenter(int* x,int* y){
	*x=0;*y=0;
	if(!NMemb)return false;
	if(tmtmt&&lastTimeGetCenter==tmtmt){
		*x=lastCenterX;
		*y=lastCenterY;
		return true;
	}
	int xx=0;
	int yy=0;
	int N=0;
	for(int i=0;i<NMemb;i++){
		word MID=Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==MembSN[i]){
				xx+=(OB->RealX>>4);
				yy+=(OB->RealY>>4);
				N++;
			};
		};
	};
	if(N){
		*x=xx/N;
		*y=yy/N;
		return true;
	}else return false;
};
int Brigade::GetActivity(){
	if(!Memb)return 0;
	for(int i=0;i<NMemb;i++){
		word MID=Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==MembSN[i]){
				return OB->ActivityState;
			}
		}
	}
	return 0;
}
bool Brigade::SetActivityState(int State){
	if(EngSettings.DontUseAgressiveState)return false;
	if(Enabled&&WarType&&State>=0&&State<3){
		byte FormID[3];
		memset(FormID,0xFF,sizeof(FormID));
		OrderDescription* ODE=ElementaryOrders+WarType-1;
		if(ODE->GroupID!=0xFF){
			SingleGroup* FGD=FormGrp.Grp+ODE->GroupID;
			for(int j=0;j<FGD->NCommon&&j<3;j++){
				FormID[j]=FGD->IDX[j];
				/*
				if(fID==FormID[j]){
				CurForm=j;
				}
				*/
			}
		}
		char ids[]={1,0,2};
		int ft=FormID[ids[State]];//0
		if(ft<0xFF&&ft!=GetFormIndex()){
			//REFORMA(I->BrigID+(ft<<16));
			/*
			vui_IS_Sel_MakeReformation MR;
			MR.Data.NI=MyNation;
			MR.Data.BrigadeID=I->BrigID;
			MR.Data.FormType=ft;
			*/
			for(int i=0;i<NMemb;i++){
				word MID=Memb[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==MembSN[i]){
						OB->ActivityState=State;
					}
				}
			}
			//
			void MakeReformation(Brigade* BR,byte FormType);
			MakeReformation(this,ft);
			return true;
		}						
	}
	return false;
};
/*
int Brigade::GetReformaStatus(){
	OrderDescription* ODE=ElementaryOrders+WarType-1;
	if(ODE->GroupID!=0xFF){
		SingleGroup* FGD=FormGrp.Grp+ODE->GroupID;
		for(int j=0;j<FGD->NCommon&&j<3;j++){
			if(FGD->IDX[j]==WarType-1)return j;
		}
	}
	return -1;
}
*/
bool GetBrCenter(Brigade* BR,int* x,int* y){
	if(BR->NMemb<NBPERSONAL)return false;
	if(BR->PosCreated&&BR->NMemb>NBPERSONAL){
		int xs=0;
		int ys=0;
		int Nu=BR->NMemb;
		for(int i=NBPERSONAL;i<Nu;i++){
			xs+=BR->posX[i];
			ys+=BR->posY[i];
		};
		xs/=(Nu-NBPERSONAL);
		ys/=(Nu-NBPERSONAL);
		*x=xs;
		*y=ys;
		return true;
	};
	return BR->GetCenter(x,y);
};

//------------------Human functions--------------

int FindUnitsInCell(int cell,int x,int y,int r,int Type,byte Nation,int Need,word* Dest){
	int N=0;
	if(!Need)return 0;
	if(cell>0){
		cell+=VAL_MAXCX+1;
		if(cell>=VAL_MAXCIOFS)return 0;
		int NMon=MCount[cell];
		if(!NMon)return NULL;
		int ofs1=cell<<SHFCELL;
		word MID;
		for(int i=0;i<NMon;i++){
			MID=GetNMSL(ofs1+i);
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&(!(OB->Guard!=0xFFFF||OB->Sdoxlo||OB->UnlimitedMotion||OB->LockedInBuilding!=0xFFFF))&&OB->BrigadeID==0xFFFF&&OB->NNUM==Nation&&OB->NIndex==Type&&Norma(OB->RealX-x,OB->RealY-y)<r){
					Dest[N]=OB->Index;
					N++;
					if(N>=Need)return N;
				};
			};
		};
	};
    return N;
};
int FindUnits(int x,int y,int r,int Type,byte Nation,int Need,word* Dest){
    int rx1=(r>>11)+1;
	int N=0;
	int x0=x>>11;
	int y0=y>>11;
	int maxx=msx>>2;
	for(int R=0;R<=rx1;R++){
		char* xi=Rarr[R].xi;
		char* yi=Rarr[R].yi;
		int NN=Rarr[R].N;
		for(int p=0;p<NN;p++){
			int xx=x0+xi[p];
			int yy=y0+yi[p];
			if(xx>=0&&yy>=0&&xx<maxx&&yy<maxx){
				int cell=xx+(yy<<VAL_SHFCX);
				N+=FindUnitsInCell(cell,x,y,r,Type,Nation,Need-N,Dest+N);
				if(N>=Need)return N;
			};
		};
	};
	return N;
};
void MakeReformation(byte NI,word BrigadeID,byte FormType);
void CancelStandGroundAnyway(Brigade* BR);
/*
word GetMyOfficerInf(byte NI,word UnitID,word WarType){
	Nation* NAT=NATIONS+NI;
	int N=NAT->NMon;
	for(int i=0;i<N;i++)if(NAT->Mon[i]->OFCR){
		OfficerRecord* OFCR=NAT->Mon[i]->OFCR;
		for(int j=0;j<OFCR->NStroi;j++){
			OFCR->SDES[j].ID==UnitID&&OFCR->SDES[j].
		};
	};
};
*/
void CheckBrigadeDUP(Brigade* BR){
	//byte PRES[8192];
	//memset(PRES,0,8192);
	for(int i=0;i<BR->NMemb;i++){
		word MID=BR->Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==BR->MembSN[i]){
				assert(OB->BrIndex==i);
			};
		};
	};
};
CEXPORT void VitalError(char*);
byte Brigade::GetNMask(){
	word* id=Memb;
	word* sn=MembSN;
	for(int i=0;i<NMemb;i++,id++,sn++){
		OneObject* OB=Group[*id];
		if(OB&&OB->Serial==*sn&&!OB->Sdoxlo){
			return OB->NMask;
		}
	}
	//VitalError("Brigade::GetNMask");
	return 0;
}
void Brigade::SetNMask(byte Mask){
	word* id=Memb;
	word* sn=MembSN;
	for(int i=0;i<NMemb;i++,id++,sn++){
		OneObject* OB=Group[*id];
		if(OB&&OB->Serial==*sn){
			OB->NMask=Mask;
		}
	}
};

bool Brigade::CreateNearOfficer(OneObject* OB,word Type,int ODIndex){
	if(!OB->Ref.General->OFCR)return false;
	bool Dem=false;
	word BarbID=OB->Ref.General->OFCR->BarabanID;
	if(BarbID==Type)Dem=true;
	int RX=OB->RealX;
	int RY=OB->RealY;
	OrderDescription* ODS=ElementaryOrders+ODIndex;
	word ULOCAL[512];
	int N=FindUnits(RX,RY,MobilR*16,Type,OB->NNUM,ODS->NUnits,ULOCAL);
	if(N!=ODS->NUnits)return false;
	AddObject(OB);
	OneObject* OOB=OB;
	if(Dem){
		OB->BrigadeID=0xFFFF;
		OB->BrIndex=0xFFFF;
		OB->InArmy=false;
		Memb[0]=0xFFFF;
		MembSN[0]=0xFFFF;
	}else OB->StandGround=true;
	if(!Dem)OB->InArmy=true;
	word BMID;
	if(BarbID==0xFFFF)N=1;
	else N=FindUnits(RX,RY,MobilR*16,OB->Ref.General->OFCR->BarabanID,OB->NNUM,1,&BMID);
	if(N==1){
		if(BarbID==0xFFFF){
			AddObject(NULL);
		}else{
			OB=Group[BMID];
			AddObject(OB);
			if(Dem){
				OB->BrigadeID=0xFFFF;
				OB->BrIndex=0xFFFF;
				OB->InArmy=false;
				Memb[1]=0xFFFF;
				MembSN[1]=0xFFFF;
				memset(&BM,0,sizeof BM);
			}else{ 
				OB->StandGround=true;
				OB->InArmy=1;
			};
		};		
		for(int j=2;j<NBPERSONAL;j++){
			Memb[j]=0xFFFF;
			MembSN[j]=0xFFFF;
		};
		if(OOB->Ref.General->OFCR->FlagID!=0xFFFF){
			N=FindUnits(RX,RY,MobilR*16,OOB->Ref.General->OFCR->FlagID,OB->NNUM,1,&BMID);
			if(N==1){
				OB=Group[BMID];
				AddObject(OB);
				if(Dem){
					OB->BrigadeID=0xFFFF;
					OB->BrIndex=0xFFFF;
					OB->InArmy=false;
					Memb[2]=0xFFFF;
					MembSN[2]=0xFFFF;
					memset(&BM,0,sizeof BM);
				}else{ 
					OB->StandGround=true;
					OB->InArmy=true;
				};
			};
		};
		NMemb=NBPERSONAL;
		if(!Dem)OB->InArmy=true;
		//OB->AddDamage=ODS->AddDamage1;
		//OB->AddShield=ODS->AddShield1;
		AddShield=ODS->AddShield2;
		AddDamage=ODS->AddDamage2;
		if(Memb[2]!=0xFFFF){
			AddShield+=ODS->FAddShield;
			AddDamage+=ODS->FAddDamage;
		};
		N=FindUnits(RX,RY,MobilR*16,Type,OB->NNUM,ODS->NUnits,ULOCAL);
		for(int i=0;i<N;i++){
			OneObject* OB=Group[ULOCAL[i]];
			AddObject(OB);
			OB->InArmy=true;
			if(EngSettings.MoraleType!=1){
				OB->AddDamage=ODS->AddDamage2;
				OB->AddShield=ODS->AddShield2;
			}
			OB->StandGround=0;
			if(Memb[2]!=0xFFFF){
				OB->AddShield+=ODS->FAddShield;
				OB->AddDamage+=ODS->FAddDamage;
			};
		};
		WarType=ODIndex+1;
		PosCreated=false;
		CreateOrderedPositions(RX,RY,0);
		addrand(128+16);
		KeepPositions(0,128+16);
		MembID=Type;
		//byte usag=NATIONS[OB->NNUM].Mon[Type]->newMons->Usage;
		NewMonster* NM=NATIONS[OB->NNUM].Mon[Type]->newMons;
#ifndef COSSACKS2
		if(NM->FormLikeShooters)Strelki=1;else Strelki=0;
#endif //COSSACKS2
		//HumanLocalSendTo(OB->RealX,OB->RealY,128+16,0);
#ifdef COSSACKS2
		OfficerID=OOB->NIndex;
		BarID=OOB->Ref.General->OFCR->BarabanID;
		FlagID=OOB->Ref.General->OFCR->FlagID;
#endif //COSSACKS2
#ifdef NEWMORALE
		int GetMaxMorale(Brigade* BR);
		MaxMorale=GetMaxMorale(this);
		Morale=MaxMorale;
		PrevMorale=Morale;
		WasLess30Lost=1;
#endif //NEWMORALE
	}else{
		FreeMember(0);
		return 0;
	};
	SetNMask(OB->NMask);
	return 1;
};
	/*
	OrderDescription* ODS=ElementaryOrders+ODIndex;
	if(UG->N<ODS->NUnits+0)return false;
	OneObject* OB=NULL;
	if(UG->IDS[0]!=0xFFFF)	OB=Group[UG->IDS[0]];
	word Type=Group[UG->IDS[2]]->NIndex;
	word BarbID=0xFFFF;
	if(UG->IDS[1]!=0xFFFF) BarbID=Group[UG->IDS[1]]->NIndex;
	bool Dem=false;
	if(BarbID==Type)Dem=true;
	int RX=0;
	int RY=0;
	if(OB){
		RX=OB->RealX;
		RY=OB->RealY;
	}else{
		int n=0;
		for(int i=1;i<UG->N;i++){
			OneObject* ob=Group[UG->IDS[i]];
			if(ob){
				RX+=ob->RealX;
				RY+=ob->RealY;
				n++;
			}
		}
		if(n>0){
			RX/=n;
			RY/=n;
		}else{
			return false;
		}
	}
	
	word ULOCAL[512];
	memcpy(ULOCAL,UG->IDS+2,ODS->NUnits<<1);
	int N=ODS->NUnits;
	AddObject(OB);
	OneObject* OOB=OB;
	if(Dem){
		if(OB){
			OB->BrigadeID=0xFFFF;
			OB->BrIndex=0xFFFF;
			OB->InArmy=false;
		}
		Memb[0]=0xFFFF;
		MembSN[0]=0xFFFF;
	}else if(OB)OB->StandGround=true;
	if(!Dem&&OB)OB->InArmy=true;
	word BMID=UG->IDS[1];
	OB=Group[BMID];
	AddObject(OB);
	if(Dem){
		if(OB){
			OB->BrigadeID=0xFFFF;
			OB->BrIndex=0xFFFF;
			OB->InArmy=false;
		}
		Memb[1]=0xFFFF;
		MembSN[1]=0xFFFF;
		memset(&BM,0,sizeof BM);
	}else{ 
		if(OB){
			OB->StandGround=true;
			OB->InArmy=1;
		}
	};
	
	for(int j=2;j<NBPERSONAL;j++){
		Memb[j]=0xFFFF;
		MembSN[j]=0xFFFF;
	};
	if(UG->N==ODS->NUnits+3){
		OB=Group[UG->IDS[UG->N-1]];
		AddObject(OB);
		if(Dem){
			if(OB){
				OB->BrigadeID=0xFFFF;
				OB->BrIndex=0xFFFF;
				OB->InArmy=false;
			}
			Memb[2]=0xFFFF;
			MembSN[2]=0xFFFF;
			memset(&BM,0,sizeof BM);
		}else{ 
			if(OB){
				OB->StandGround=true;
				OB->InArmy=true;
			}
		};
	};
	NMemb=NBPERSONAL;
	if(!Dem&&OB)OB->InArmy=true;
	//OB->AddDamage=ODS->AddDamage1;
	//OB->AddShield=ODS->AddShield1;
	AddShield=ODS->AddShield2;
	AddDamage=ODS->AddDamage2;
	if(Memb[2]!=0xFFFF){
		AddShield+=ODS->FAddShield;
		AddDamage+=ODS->FAddDamage;
	};
	N=ODS->NUnits;
	byte NI=0xFF;
	for(int i=0;i<N;i++){
		OneObject* OB=Group[ULOCAL[i]];
		AddObject(OB);
		if(NI==0xFF) NI=OB->NNUM;
		OB->InArmy=true;
		OB->AddDamage=ODS->AddDamage2;
		OB->AddShield=ODS->AddShield2;
		OB->StandGround=0;
		if(Memb[2]!=0xFFFF){
			OB->AddShield+=ODS->FAddShield;
			OB->AddDamage+=ODS->FAddDamage;
		};
	};
	WarType=ODIndex+1;
	PosCreated=false;
	CreateOrderedPositions(RX,RY,0);
	addrand(128+16);
	KeepPositions(0,128+16);
	MembID=Type;
	//byte usag=NATIONS[OB->NNUM].Mon[Type]->newMons->Usage;
	//if(usag==StrelokID||usag==HorseStrelokID||OB->newMons->Artilery)Strelki=1;else Strelki=0;
	NewMonster* NM=NATIONS[NI].Mon[Type]->newMons;
#ifndef COSSACKS2
	if(NM->FormLikeShooters)Strelki=1;else Strelki=0;
#endif //COSSACKS2
#ifdef COSSACKS2
		if(OOB){	
			OfficerID=OOB->NIndex;
			BarID=OOB->Ref.General->OFCR->BarabanID;
			FlagID=OOB->Ref.General->OFCR->FlagID;
		}else{
			OOB=Group[Memb[3]];
			GetOfficersType(OOB->NNUM,OOB->NIndex,OfficerID,BarID,FlagID);
		}
#endif //COSSACKS2
#ifdef NEWMORALE
	int GetMaxMorale(Brigade* BR);
	MaxMorale=GetMaxMorale(this);
	Morale=MaxMorale;
	PrevMorale=Morale;
	WasLess30Lost=1;
#endif //NEWMORALE
	//HumanLocalSendTo(OB->RealX,OB->RealY,128+16,0);
#ifdef COSSACKS2
	void NewBrigadeIsBorn(Brigade* B);
	NewBrigadeIsBorn(this);
#endif COSSACKS2
	return 1;
	*/
DLLEXPORT int GetRND(int);
bool GetOfficersType(byte NI, word UnitType, word &OffID, word &BarabanID, word &FlagID);
bool Brigade::CreateFromGroup(UnitsGroup* UG,int ODIndex,byte Dir){
	// convention about UG
	// UG->IDS[0] - officer
	// UG->IDS[1] - baraban
	// UG->IDS[N-1] - flag
	// if not group contains only UnitType units
	addrand(ODIndex);
	addrand(Dir);
	OrderDescription* ODS=ElementaryOrders+ODIndex;
	//if(UG->N<ODS->NUnits+0)return false;
	OneObject* OB=NULL;
	if(UG->IDS[0]!=0xFFFF){
		OB=Group[UG->IDS[0]];
		if(OB->UnlimitedMotion){
			addrand(123);
			return false;
		}
	}
	word Type=Group[UG->IDS[2]]->NIndex;
	if(Group[UG->IDS[2]]->UnlimitedMotion||Type>=2048){
		addrand(132);
		return false;
	}
	
	GetOfficersType(Group[UG->IDS[2]]->NNUM,Type,OfficerID,BarID,FlagID);

	int RX=0;
	int RY=0;
	if(OB){
		RX=OB->RealX;
		RY=OB->RealY;
	}else{
		int n=0;
		addrand(UG->N);
		for(int i=1;i<UG->N;i++){
			OneObject* ob=Group[UG->IDS[i]];
			if(ob){
				addname(ob->Index);
				RX+=ob->RealX;
				RY+=ob->RealY;
				addrand(RX);
				addrand(RY);
				n++;
			}
		}
		if(n>0){
			RX/=n;
			RY/=n;
		}else{
			addrand(999);
			return false;
		}
	}
	int st=0;
	int en=0;
	
	if(NMemb>=MaxMemb){
		MaxMemb+=128;
		Memb=(word*)realloc(Memb,MaxMemb<<1);
		MembSN=(word*)realloc(MembSN,MaxMemb<<1);		
	};
	memset(Memb,0xFF,MaxMemb<<1);	
	memset(MembSN,0xFF,MaxMemb<<1);

	if(OfficerID!=0xFFFF&&UG->IDS[0]!=0xFFFF&&OfficerID==Group[UG->IDS[0]]->NIndex)
	{
		OB=Group[UG->IDS[0]];
		if(OB)
		{
			AddObject(OB);
			OB->StandGround=true;
			OB->InArmy=true;
			OB->RifleAttack=0;
			OB->ActivityState=0;
		}
		else
		{
			Memb[0]=0xFFFF;
			MembSN[0]=0xFFFF;
			NMemb++;
		}
		st++;
	}
	else
	{
		Memb[0]=0xFFFF;
		MembSN[0]=0xFFFF;
		NMemb++;
	}
	if((!st)&&UG->IDS[0]==0xFFFF){
		st++;
	}
	if(BarID!=0xFFFF&&UG->IDS[st]!=0xFFFF&&BarID==Group[UG->IDS[st]]->NIndex)
	{
		OB=Group[UG->IDS[st]];
		if(OB)
		{
			addname(OB->Index);
			AddObject(OB);
			OB->StandGround=true;
			OB->InArmy=true;
			OB->RifleAttack=0;
			OB->ActivityState=0;
		}
		else
		{
			Memb[st]=0xFFFF;
			MembSN[st]=0xFFFF;
			NMemb++;
		}
		st++;
	}
	else
	{
		Memb[1]=0xFFFF;
		MembSN[1]=0xFFFF;
		NMemb++;
	}
	if(FlagID!=0xFFFF&&UG->IDS[UG->N-1]!=0xFFFF&&FlagID==Group[UG->IDS[UG->N-1]]->NIndex)
	{
		OB=Group[UG->IDS[UG->N-1]];
		if(OB)
		{
			addname(OB->Index);
			AddObject(OB);
			OB->StandGround=true;
			OB->InArmy=true;
			OB->RifleAttack=0;
			OB->ActivityState=0;
		}
		else
		{
			Memb[2]=0xFFFF;
			MembSN[2]=0xFFFF;
			NMemb++;
		}
		en++;
	}
	else
	{
		Memb[2]=0xFFFF;
		MembSN[2]=0xFFFF;
		NMemb++;
	}
	byte NI=0xFF;
	AddShield=ODS->AddShield2;
	AddDamage=ODS->AddDamage2;
	if(UG->N<=(ODS->NUnits+st+en))
	{
		for(int i=st;i<(UG->N-en);i++)
		{
			OB=Group[UG->IDS[i]];
			if(OB&&OB->Serial==UG->SNS[i]&&OB->NIndex==Type&&!(OB->Sdoxlo||OB->UnlimitedMotion))
			{
				addname(OB->Index);
				AddObject(OB);
				if(NI==0xFF) NI=OB->NNUM;
				OB->InArmy=true;
				if(EngSettings.MoraleType!=1){
					OB->AddDamage=ODS->AddDamage2;
					OB->AddShield=ODS->AddShield2;
				}
				OB->StandGround=0;
				if(Memb[2]!=0xFFFF)
				{
					OB->AddShield+=ODS->FAddShield;
					OB->AddDamage+=ODS->FAddDamage;
				}
				OB->RifleAttack=0;
				OB->ActivityState=0;
			}
		}
		if(NI==0xFF)
			return false;
		int nmemb=ODS->NUnits+NBPERSONAL;
		if(nmemb>NMemb){
			if(nmemb>=MaxMemb){
				MaxMemb=nmemb;
				Memb=(word*)realloc(Memb,MaxMemb<<1);
				MembSN=(word*)realloc(MembSN,MaxMemb<<1);				
			}
			memset(Memb+NMemb,0xFF,(nmemb-NMemb)<<1);
			memset(MembSN+NMemb,0xFF,(nmemb-NMemb)<<1);				
			NMemb=nmemb;
		};
		WarType=ODIndex+1;
		PosCreated=false;
		addrand(RX);
		addrand(RY);
		addrand(Dir);
		CreateOrderedPositions(RX,RY,Dir);
		addrand(128+16);
		KeepPositions(0,128+16);
		MembID=Type;
		NewMonster* NM=NATIONS[NI].Mon[Type]->newMons;
		if(NM->FormLikeShooters)Strelki=1;else Strelki=0;
	#ifdef NEWMORALE
		int GetMaxMorale(Brigade* BR);
		MaxMorale=GetMaxMorale(this);
		Morale=MaxMorale;
		PrevMorale=Morale;
		WasLess30Lost=1;
	#endif //NEWMORALE
		void NewBrigadeIsBorn(Brigade* B);
		NewBrigadeIsBorn(this);
		byte Mask=GetNMask();
		SetNMask(Mask);
		return true;
	}
	return false;
};

int FindCommandPlace(int* x,int* y,byte dir,OneObject* OB,int Indx,int Indx1,OrderDescription* ODS){
	int xx=*x;
	int yy=*y;
	addrand(xx);
	addrand(yy);
	int SIN=TCos[dir];
	int COS=TSin[dir];
	int N=ODS->NCom;
	int MinR=100000000;
	int idx=-1;
	int rx=OB->RealX;
	int ry=OB->RealY;
	int bx=rx;
	int by=ry;
	addrand(N);
	for(int i=0;i<N;i++){
		if(i!=Indx&&i!=Indx1){
			int xc=ODS->ComX[i];
			int yc=ODS->ComY[i];
			addrand(ODS->ComX[i]);
			addrand(ODS->ComY[i]);
			int xci=xx+OScale(OScale(-xc*COS+yc*SIN))/2;
			int yci=yy+OScale(OScale(yc*COS+xc*SIN))/2;
			addrand(xci);
			addrand(yci);
			int R=Norma(rx-xci,ry-yci);
			if(R<MinR){
				MinR=R;
				idx=i;
				bx=xci;
				by=yci;
			};
		};
	};
	addrand(idx);
	if(idx!=-1){
		*x=bx;
		*y=by;
	};
	addrand(*x);
	addrand(*y);
	return idx;
};
void OptimiseBrigadePosition2(Brigade* BR);
void Brigade::CreateOrderedPositions(int x,int y,char dir){
#ifdef CHECK_SYNC_FAILURE
	for(int i=0;i<NMemb;i++){
		addrand(Memb[i]);
		addrand(MembSN[i]);
	}
#endif //CHECK_SYNC_FAILURE
	CreateSimpleOrderedPositions(x,y,dir);
#ifdef CHECK_SYNC_FAILURE
	for(int i=0;i<NMemb;i++){
		addrand(Memb[i]);
		addrand(MembSN[i]);
	}
#endif //CHECK_SYNC_FAILURE
	if(posX)OptimiseBrigadePosition2(this);
#ifdef CHECK_SYNC_FAILURE
	for(int i=0;i<NMemb;i++){
		addrand(Memb[i]);
		addrand(MembSN[i]);
	}
#endif //CHECK_SYNC_FAILURE
	/*
	if(NMemb>NBPERSONAL){
		int NM=ElementaryOrders[WarType-1].NUnits;
		int N=PORD.CreateOrdPos(x,y,dir,NM,Memb+NBPERSONAL,ElementaryOrders+WarType-1);
		//NMemb=N+2;
		if(!PosCreated){
			posX=znew(int,MaxMemb);
			posY=znew(int,MaxMemb);
			PosCreated=true;
		};
		for(int i=0;i<N;i++){
			posX[i+NBPERSONAL]=PORD.px[i]>>4;
			posY[i+NBPERSONAL]=PORD.py[i]>>4;
			addrand(posX[i+NBPERSONAL]);
			addrand(posY[i+NBPERSONAL]);
			addrand(i);
			word MID=PORD.Ids[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB){
					OB->BrIndex=i+NBPERSONAL;
					Memb[i+NBPERSONAL]=MID;
					MembSN[i+NBPERSONAL]=OB->Serial;
				}else{
					Memb[i+NBPERSONAL]=0xFFFF;
					MembSN[i+NBPERSONAL]=0xFFFF;
				};
			}else{
				Memb[i+NBPERSONAL]=0xFFFF;
				MembSN[i+NBPERSONAL]=0xFFFF;
			};
		};
		for(i=N;i<NM;i++){
			Memb[i+NBPERSONAL]=0xFFFF;
			MembSN[i+NBPERSONAL]=0xFFFF;
		};
		NMemb=NM+NBPERSONAL;
		int xx=x;
		int yy=y;
		word MID0=Memb[0];
		int id1=-1;
		int id2=-1;
		if(MID0!=0xFFFF){
			OneObject* OB=Group[MID0];
			if(OB&&!OB->Sdoxlo){
				addrand(xx);
				addrand(yy);
				id1=FindCommandPlace(&xx,&yy,dir,OB,-1,-1,ElementaryOrders+WarType-1);
			};
			posX[0]=xx>>4;
			posY[0]=yy>>4;
		};
		xx=x;
		yy=y;
		word MID1=Memb[1];
		if(MID1!=0xFFFF){
			OneObject* OB=Group[MID1];
			if(OB&&!OB->Sdoxlo){
				addrand(xx);
				addrand(yy);
				id2=FindCommandPlace(&xx,&yy,dir,OB,id1,-1,ElementaryOrders+WarType-1);
			};
			posX[1]=xx>>4;
			posY[1]=yy>>4;
		};
		xx=x;
		yy=y;
		word MID2=Memb[2];
		if(MID2!=0xFFFF){
			OneObject* OB=Group[MID2];
			if(OB&&!OB->Sdoxlo){
				addrand(xx);
				addrand(yy);
				id1=FindCommandPlace(&xx,&yy,dir,OB,id2,id1,ElementaryOrders+WarType-1);
			};
			posX[2]=xx>>4;
			posY[2]=yy>>4;
		};
	};
	if(posX)OptimiseBrigadePosition2(this);
	*/
};
void Brigade::CreateSimpleOrderedPositions(int x,int y,char dir){
	addrand(x);
	addrand(y);
	addrand(dir);
	addrand(ID);
	addrand(CT->NI);
#ifdef CHECK_SYNC_FAILURE
	for(int i=0;i<NMemb;i++){
		addrand(Memb[i]);
		addrand(MembSN[i]);
	}
#endif //CHECK_SYNC_FAILURE
	if(NMemb>NBPERSONAL){
		int NM=ElementaryOrders[WarType-1].NUnits;
		int N=PORD.CreateSimpleOrdPos(x,y,dir,NM,Memb+NBPERSONAL,ElementaryOrders+WarType-1);
		addrand(NM);
		addrand(N);
		//NMemb=N+2;
		if(!PosCreated){
			posX=znew(int,MaxMemb);
			posY=znew(int,MaxMemb);
			PosCreated=true;
		};
		for(int i=0;i<NM;i++){ //i<N
			posX[i+NBPERSONAL]=PORD.px[i]>>4;
			posY[i+NBPERSONAL]=PORD.py[i]>>4;
			if(i<N)
			{
				word MID=PORD.Ids[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB){
						OB->BrIndex=i+NBPERSONAL;
						Memb[i+NBPERSONAL]=MID;
						MembSN[i+NBPERSONAL]=OB->Serial;
					}else{
						Memb[i+NBPERSONAL]=0xFFFF;
						MembSN[i+NBPERSONAL]=0xFFFF;
					};
				}else{
					Memb[i+NBPERSONAL]=0xFFFF;
					MembSN[i+NBPERSONAL]=0xFFFF;
				};
			}
		};
		for(int i=N;i<NM;i++){
			Memb[i+NBPERSONAL]=0xFFFF;
			MembSN[i+NBPERSONAL]=0xFFFF;
		};
		NMemb=NM+NBPERSONAL;
		int xx=x;
		int yy=y;
		word MID0=Memb[0];
		int id1=-1;
		int id2=-1;
		if(MID0!=0xFFFF){
			OneObject* OB=Group[MID0];
			if(OB&&!OB->Sdoxlo){
				id1=FindCommandPlace(&xx,&yy,dir,OB,-1,-1,ElementaryOrders+WarType-1);
			};
			posX[0]=xx>>4;
			posY[0]=yy>>4;
		};
		xx=x;
		yy=y;
		word MID1=Memb[1];
		if(MID1!=0xFFFF){
			OneObject* OB=Group[MID1];
			if(OB&&!OB->Sdoxlo){
				id2=FindCommandPlace(&xx,&yy,dir,OB,id1,-1,ElementaryOrders+WarType-1);
			};
			posX[1]=xx>>4;
			posY[1]=yy>>4;
		};
		xx=x;
		yy=y;
		word MID2=Memb[2];
		if(MID2!=0xFFFF){
			OneObject* OB=Group[MID2];
			if(OB&&!OB->Sdoxlo){
				id1=FindCommandPlace(&xx,&yy,dir,OB,id2,id1,ElementaryOrders+WarType-1);
			};
			posX[2]=xx>>4;
			posY[2]=yy>>4;
		}
		OneObject* OB0=Group[MID0];
		OneObject* OB1=Group[MID1];
		OneObject* OB2=Group[MID2];

		int ID[3]={MID0,MID1,MID2};        		

		int SN0[3]={MembSN[0],MembSN[1],MembSN[2]};
		int SN[3]={MembSN[0],MembSN[1],MembSN[2]};

		int MinD=1000;

		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				for(int k=0;k<3;k++){
					if(i!=j&&i!=k&&j!=k){
                        word xMid0=Memb[i];
						word xMid1=Memb[j];
						word xMid2=Memb[k];
						if( (xMid0!=0xFFFF||MID0==0xFFFF) && (xMid1!=0xFFFF||MID1==0xFFFF) && (xMid2!=0xFFFF||MID2==0xFFFF) ){
							int MinR=10000;
							int MaxR=0;
							int Summ=0;

							int DX0=0;
							int DY0=0;
							int DX1=0;
							int DY1=0;
							int DX2=0;
							int DY2=0;							

							if(OB0){
								DX0=OB0->RealX/16-posX[i];
								DY0=OB0->RealY/16-posY[i];
                                int R=Norma(DX0,DY0);
								if(R){
									DX0=DX0*64/R;
									DY0=DY0*64/R;
								}
								if(R<MinR)MinR=R;
								if(R>MaxR)MaxR=R;
								Summ+=R;
							}
							if(OB1){
								DX1=OB1->RealX/16-posX[j];
								DY1=OB1->RealY/16-posY[j];
								int R=Norma(DX1,DY1);
								if(R){
									DX1=DX1*64/R;
									DY1=DY1*64/R;
								}
								if(R<MinR)MinR=R;
								if(R>MaxR)MaxR=R;
								Summ+=R;
							}
							if(OB2){
								DX2=OB2->RealX/16-posX[k];
								DY2=OB2->RealY/16-posY[k];
								int R=Norma(DX2,DY2);
								if(R){
									DX2=DX2*64/R;
									DY2=DY2*64/R;
								}
								if(R<MinR)MinR=R;
								if(R>MaxR)MaxR=R;
								Summ+=R;
							}

							int DX=DX0+DX1+DX2;
							int DY=DY0+DY1+DY2;
							int R=Norma(DX,DY);
							if(R){
								DX=DX*64/R;
								DY=DY*64/R;
							}

							int DR=MaxR-MinR;
							int V =-(DX*(DX0+DX1+DX2)+DY*(DY0+DY1+DY2));
							if(V<MinD){
								MinD=V;
								ID[i]=MID0;
								ID[j]=MID1;
								ID[k]=MID2;
								SN[i]=SN0[0];
								SN[j]=SN0[1];
								SN[k]=SN0[2];
							}
						}
					}
				}
			}
		}
		Memb[0]=ID[0];
		Memb[1]=ID[1];
		Memb[2]=ID[2];
		MembSN[0]=SN[0];
		MembSN[1]=SN[1];
		MembSN[2]=SN[2];
	}
#ifdef CHECK_SYNC_FAILURE
	for(int i=0;i<NMemb;i++){
		addrand(Memb[i]);
		addrand(MembSN[i]);
	}
#endif //CHECK_SYNC_FAILURE

	/*
	addrand(x);
	addrand(y);
	addrand(dir);
	if(NMemb>NBPERSONAL){
		int N=PORD.CreateSimpleOrdPos(x,y,dir,NMemb-NBPERSONAL,Memb+NBPERSONAL,ElementaryOrders+WarType-1);
		//NMemb=N+2;
		memset(Memb+NBPERSONAL,0xFF,(NMemb-NBPERSONAL)*2);
		memset(MembSN+NBPERSONAL,0xFF,(NMemb-NBPERSONAL)*2);
		if(!PosCreated){
			posX=znew(int,MaxMemb);
			posY=znew(int,MaxMemb);
			PosCreated=true;
		};
		for(int i=0;i<N;i++){
			posX[i+NBPERSONAL]=PORD.px[i]>>4;
			posY[i+NBPERSONAL]=PORD.py[i]>>4;
			word MID=PORD.Ids[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB){
					OB->BrIndex=i+NBPERSONAL;
					Memb[i+NBPERSONAL]=MID;
					MembSN[i+NBPERSONAL]=OB->Serial;
				}else{
					Memb[i+NBPERSONAL]=0xFFFF;
					MembSN[i+NBPERSONAL]=0xFFFF;
				};
			}else{
				Memb[i+NBPERSONAL]=0xFFFF;
				MembSN[i+NBPERSONAL]=0xFFFF;
			};
		};
		int xx=x;
		int yy=y;
		word MID0=Memb[0];
		int id1=-1;
		int id2=-1;
		if(MID0!=0xFFFF){
			OneObject* OB=Group[MID0];
			if(OB&&!OB->Sdoxlo){
				addrand(xx);
				addrand(yy);
				id1=FindCommandPlace(&xx,&yy,dir,OB,-1,-1,ElementaryOrders+WarType-1);
			};
			posX[0]=xx>>4;
			posY[0]=yy>>4;
			addrand(posX[0]);
			addrand(posY[0]);
		};
		xx=x;
		yy=y;
		MID0=Memb[1];
		if(MID0!=0xFFFF){
			OneObject* OB=Group[MID0];
			if(OB&&!OB->Sdoxlo){
				addrand(xx);
				addrand(yy);
				id2=FindCommandPlace(&xx,&yy,dir,OB,id1,-1,ElementaryOrders+WarType-1);
			};
			posX[1]=xx>>4;
			posY[1]=yy>>4;
			addrand(posX[1]);
			addrand(posY[1]);
		};
		xx=x;
		yy=y;
		MID0=Memb[2];
		if(MID0!=0xFFFF){
			OneObject* OB=Group[MID0];
			if(OB&&!OB->Sdoxlo){
				addrand(xx);
				addrand(yy);
				id2=FindCommandPlace(&xx,&yy,dir,OB,id1,id2,ElementaryOrders+WarType-1);
			};
			posX[2]=xx>>4;
			posY[2]=yy>>4;
			addrand(posX[2]);
			addrand(posY[2]);
		};
	};
	OptimiseBrigadePosition2(this);
	*/
};
void Brigade::HumanCheckUnits(){
	for(int i=0;i<NMemb;i++){
		word MID=Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(!(OB&&OB->Serial==MembSN[i]&&!OB->Sdoxlo)){
				Memb[i]=0xFFFF;
				MembSN[i]=0xFFFF;
			};
		};
	};
};
word GetDir(int,int);
class B_HSend:public BrigadeOrder{
public:
	int x,y;
	short Dir;
	byte Prio,OrdType;
};
short FilterDir(short Dir){
	return (((Dir)>>3)<<3);
};
void ApplySwap(Brigade* BR,word* swp){
	word tm[512];
	word tmsn[512];
	word tm1[512];
	word tmsn1[512];
	if(BR->WarType){
		int N=BR->NMemb;
		if(N<3)return;

		memset(tm,0xFF,N<<1);
		memset(tmsn,0xFF,N<<1);
		memset(tm1,0xFF,N<<1);
		memset(tmsn1,0xFF,N<<1);

		OrderDescription* ODE=ElementaryOrders+BR->WarType-1;
		if(N-NBPERSONAL<ODE->NUnits){
			int s=(ODE->NUnits+NBPERSONAL-N)<<1;
			memset(BR->Memb+N,0xFFFF,s);
			memset(BR->MembSN+N,0xFFFF,s);
			BR->NMemb=ODE->NUnits+NBPERSONAL;
			N=BR->NMemb;
		};
		for(int i=NBPERSONAL;i<N;i++){
			word id=swp[i-NBPERSONAL]+NBPERSONAL;
			addrand(id);
			tm[i]=BR->Memb[id];
			tmsn[i]=BR->MembSN[id];
			tm1[i]=BR->Memb[i];
			tmsn1[i]=BR->MembSN[i];

		};
		for(int i=NBPERSONAL;i<N;i++){
			word MID=tm[i];
			addrand(MID);
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB){
					BR->Memb[i]=tm[i];
					BR->MembSN[i]=tmsn[i];
					OB->BrIndex=i;
					addrand(i);
				}else{
					BR->Memb[i]=0xFFFF;
					BR->MembSN[i]=0xFFFF;
				};
			}else{
				BR->Memb[i]=0xFFFF;
				BR->MembSN[i]=0xFFFF;
			};
		};
	};
};
void HumanLocalSendToLink(Brigade* BR){
	if(BR->NMemb<4){
		BR->DeleteBOrder();
		return;
	};
	B_HSend* BS=(B_HSend*)BR->BOrder;
	int x=BS->x;
	int y=BS->y;
	addrand(x);
	addrand(y);
	int xc=0;
	int yc=0;
	int N=0;
	for(int i=NBPERSONAL;i<BR->NMemb;i++){
		word MID=BR->Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo){
				xc+=OB->RealX;
				yc+=OB->RealY;
				N++;
			};
		};
	};
	if(N){
		xc/=N;
		yc/=N;
	}else return;
	if(Norma(x-xc,y-yc)<256){
		BR->DeleteBOrder();
		return;
	};
	byte Dir;
	if(BS->Dir>256)Dir=GetDir((x-xc)>>4,(y-yc)>>4);
	else Dir=BS->Dir;
	//Dir=FilterDir(Dir);
	char dd=char(Dir)-char(BR->Direction);
	byte pri=BS->Prio;
	if(abs(dd)<4){
		addrand(x);
		addrand(y);
		//BR->CreateSimpleOrderedPositions(x,y,Dir);
		BR->CreateOrderedPositions(x,y,Dir);
		BR->Direction=Dir;
		BR->DeleteBOrder();
	}else{
		if(abs(dd)<32){
			addrand(xc);
			addrand(yc);
			//BR->CreateSimpleOrderedPositions(xc,yc,Dir);
			BR->CreateOrderedPositions(xc,yc,Dir);
			BR->Direction=Dir;
		}else{
			OrderDescription* ODE=ElementaryOrders+BR->WarType-1;
			if(ODE->Sym4f){
				//maximal symmetry(rotation pi/4)
				if(dd>=32&&dd<96)ApplySwap(BR,ODE->Sym4f);
				else if(dd<=-32&&dd>-96)ApplySwap(BR,ODE->Sym4i);
				else ApplySwap(BR,ODE->SymInv);
				addrand(xc);
				addrand(yc);
				//BR->CreateSimpleOrderedPositions(xc,yc,Dir);
				BR->CreateOrderedPositions(xc,yc,Dir);
				BR->Direction=Dir;
			}else
			if(ODE->SymInv){
				if(dd>64||dd<-64)ApplySwap(BR,ODE->SymInv);
				addrand(xc);
				addrand(yc);
				//BR->CreateSimpleOrderedPositions(xc,yc,Dir);
				BR->CreateOrderedPositions(xc,yc,Dir);
				BR->Direction=Dir;
			}else{
				if(dd>0){
					addrand(xc);
					addrand(yc);
					//BR->CreateSimpleOrderedPositions(xc,yc,BR->Direction+32);
					BR->CreateOrderedPositions(xc,yc,BR->Direction+32);
					BR->Direction+=32;
				}else{
					addrand(xc);
					addrand(yc);
					//BR->CreateSimpleOrderedPositions(xc,yc,BR->Direction-32);
					BR->CreateOrderedPositions(xc,yc,BR->Direction-32);
					BR->Direction-=32;
				};
			};
		};
	};
	for(int i=0;i<BR->NMemb;i++){
		word MID=BR->Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo){
				addrand(pri);
				OB->NewMonsterPreciseSendTo(int(BR->posX[i])<<4,int(BR->posY[i])<<4,pri,0);
			};
		};
	};
	addrand(pri);
	BR->KeepPositions(1,pri);
};
void HumanEscapeLink(Brigade* BR){
	if(BR->NMemb<4){
		BR->DeleteBOrder();
		addrand(654);
		return;
	};
	B_HSend* BS=(B_HSend*)BR->BOrder;
	int x=BS->x;
	int y=BS->y;
	addrand(x);
	addrand(y);
	addrand(BS->Dir);
	int xc=0;
	int yc=0;
	int N=0;
	int AR=0;
	for(int i=NBPERSONAL;i<BR->NMemb;i++){
		word MID=BR->Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo){
				xc+=OB->RealX;
				yc+=OB->RealY;
				N++;
				if(BR->PosCreated){
					AR+=Norma(BR->posX[i]-(OB->RealX>>4),BR->posY[i]-(OB->RealY>>4));
				};
			}
		};
	};
	if(N){
		xc/=N;
		yc/=N;
		AR/=N;
	}else return;
#ifndef COSSACKS2
	if(Norma(x-xc,y-yc)<256){
		BR->DeleteBOrder();
		return;
	};
#endif
	byte Dir;
	if(BS->Dir>256)Dir=GetDir((x-xc)>>4,(y-yc)>>4);
	else Dir=BS->Dir;
	//Dir=FilterDir(Dir);
	char dd=char(Dir)-char(BR->Direction);
	byte pri=BS->Prio;
	if(abs(dd)<32||AR>128){
		addrand(x);
		addrand(y);
		//BR->CreateSimpleOrderedPositions(x,y,Dir);
		BR->CreateOrderedPositions(x,y,Dir);
		if(AR>32&&dd>0){
			BR->ResortMembByPos();
		}
		BR->Direction=Dir;
	}else{
		OrderDescription* ODE=ElementaryOrders+BR->WarType-1;
		if(ODE->Sym4f){
			//maximal symmetry(rotation pi/4)
			if(dd>=32&&dd<96)ApplySwap(BR,ODE->Sym4f);
			else if(dd<=-32&&dd>-96)ApplySwap(BR,ODE->Sym4i);
			else ApplySwap(BR,ODE->SymInv);
			addrand(x);
			addrand(y);
			//BR->CreateSimpleOrderedPositions(x,y,Dir);
			BR->CreateOrderedPositions(x,y,Dir);
			BR->Direction=Dir;
		}else
		if(ODE->SymInv){
			if(dd>64||dd<-64)ApplySwap(BR,ODE->SymInv);
			addrand(x);
			addrand(y);
			//BR->CreateSimpleOrderedPositions(x,y,Dir);
			BR->CreateOrderedPositions(x,y,Dir);
			BR->Direction=Dir;
		}else{
			if(dd>0){
				addrand(x);
				addrand(y);
				//BR->CreateSimpleOrderedPositions(x,y,BR->Direction+32);
				BR->CreateOrderedPositions(x,y,BR->Direction+32);
				BR->Direction+=32;
			}else{
				addrand(x);
				addrand(y);
				//BR->CreateSimpleOrderedPositions(x,y,BR->Direction-32);
				BR->CreateOrderedPositions(x,y,BR->Direction-32);
				BR->Direction-=32;
			};
		};
	};
#ifndef COSSACKS2
	for(i=0;i<BR->NMemb;i++){
		word MID=BR->Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo){
				addrand(pri);
				addrand(BR->posX[i]);
				addrand(BR->posY[i]);
				OB->NewMonsterPreciseSendTo(int(BR->posX[i])<<4,int(BR->posY[i])<<4,pri,0);
			};
		};
	};
	BR->DeleteBOrder();
	addrand(pri);
	BR->KeepPositions(0,pri);
#else
	BR->DeleteBOrder();
	addrand(pri);
	BR->KeepPositions(1,pri);
#endif
};
char* HLST_Message="[HumanLocalSendTo]";
byte GetBrigadeDirectionByUnitPositions(Brigade* BR);
void Brigade::HumanLocalSendTo(int x,int y,short Dir,byte Prio,byte OrdType){
	addrand(x);
	addrand(y);
	addrand(Dir);
	if(NMemb<4)return;
	if(WarType){
		OrderDescription* ODE=ElementaryOrders+WarType-1;
		if(ODE->OrdUsage==2)return;
	};	
	int xc=0;
	int yc=0;
	int N=0;
	int AR=0;
	int DX=0;
	int DY=0;
	byte UnitsAvrDir;
	for(int i=NBPERSONAL;i<NMemb;i++){
		word MID=Memb[i];
		addrand(MID);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo){
				xc+=OB->RealX;
				yc+=OB->RealY;
				DX+=TCos[OB->RealDir];
				DY+=TSin[OB->RealDir];
				N++;
				if(PosCreated){
					//AR+=Norma(posX[i]-(OB->RealX>>4),posY[i]-(OB->RealY>>4));
				};
			}
		};
	};
	if(N){
		xc/=N;
		yc/=N;
		AR/=N;
		UnitsAvrDir=GetDir(DX,DY);
	}else return;
	//if(Norma(x-xc,y-yc)<256)return;
	bool dontch=Dir>1024;
	if(Dir>1024)Dir=Dir&255;
	byte Dir2=0;
	if(Dir>256)Dir2=GetDir((x-xc)>>4,(y-yc)>>4);
	else Dir2=Dir;
	Dir=Dir2;
	addrand(Dir2);
	//Dir=FilterDir(Dir);
	//char dd=char(Dir2)-char(Direction);
#ifdef CHECK_SYNC_FAILURE
	for(int i=0;i<NMemb;i++){
		addrand(Memb[i]);
		addrand(MembSN[i]);
	}
#endif //CHECK_SYNC_FAILURE
	byte ND = GetBrigadeDirectionByUnitPositions(this);
#ifdef CHECK_SYNC_FAILURE
	for(int i=0;i<NMemb;i++){
		addrand(Memb[i]);
		addrand(MembSN[i]);
	}
#endif //CHECK_SYNC_FAILURE
	//char dd=char(Dir2)-char(UnitsAvrDir);
	char dd=char(Dir2)-char(ND);
	addrand(ND);
	addrand(dd);
	if(dontch==false&&Norma(x-xc,y-yc)<256&&abs(dd)<8)return;
	byte pri=Prio;
	if(abs(dd)<32||AR>128){
		addrand(x);
		addrand(y);
		CreateOrderedPositions(x,y,Dir);
		Direction=Dir;
	}else{
		OrderDescription* ODE=ElementaryOrders+WarType-1;
		if(ODE->Sym4f){
#ifdef CHECK_SYNC_FAILURE
			for(int i=0;i<NMemb;i++){
				addrand(Memb[i]);
				addrand(MembSN[i]);
			}
#endif //CHECK_SYNC_FAILURE
			//maximal symmetry(rotation pi/4)
			addrand(dd);
			if(dd>=32&&dd<96){
				addrand(1);
				ApplySwap(this,ODE->Sym4f);
			}else if(dd<=-32&&dd>-96){
				addrand(2);
				ApplySwap(this,ODE->Sym4i);
			}else{ 
				addrand(3);
				ApplySwap(this,ODE->SymInv);
			}
			addrand(x);
			addrand(y);
#ifdef CHECK_SYNC_FAILURE
			for(int i=0;i<NMemb;i++){
				addrand(Memb[i]);
				addrand(MembSN[i]);
			}
#endif //CHECK_SYNC_FAILURE
			addrand(Dir);
			CreateOrderedPositions(x,y,Dir);
#ifdef CHECK_SYNC_FAILURE
			for(int i=0;i<NMemb;i++){
				addrand(Memb[i]);
				addrand(MembSN[i]);
			}
#endif //CHECK_SYNC_FAILURE
			Direction=Dir;
		}else
			if(ODE->SymInv){
				if(dd>64||dd<-64)ApplySwap(this,ODE->SymInv);
				addrand(x);
				addrand(y);
				CreateOrderedPositions(x,y,Dir);
				Direction=Dir;
			}else{
				if(dd>0){
					addrand(x);
					addrand(y);
					CreateOrderedPositions(x,y,Direction+32);
					Direction+=32;
				}else{
					addrand(x);
					addrand(y);
					CreateOrderedPositions(x,y,Direction-32);
					Direction-=32;
				};
			};
	};
	/*for(i=0;i<NMemb;i++){
		word MID=Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo){
				addrand(pri);
				addrand(posX[i]);
				addrand(posY[i]);
				OB->NewMonsterPreciseSendTo(int(posX[i])<<4,int(posY[i])<<4,pri,0);
			};
		};
	};*/
	byte Ord=OrdType;
	//DeleteNewBOrder();
	addrand(pri);
	KeepPositions(Ord,pri);		
};
extern MotionField UnitsField;
int GetPState(int dir);
int TryToMove(OneObject* OB,byte NewDir,bool Dirc);
void RotUnitLink(OneObject* OB){
	
	OB->PrioryLevel=OB->LocalOrder->PrioryLevel;

    if(OB->UsesDestDidection){
        OB->DestDirection=OB->LocalOrder->info.MoveFrom.dir;
        if(OB->DestDirection==OB->RealDir){
            OB->DeleteLastOrder();
            return;
        }
        OB->DestDirectionIsActive=true;
        return;
    }

	if(OB->newMons->HaveRotateAnimations&&!OB->newMons->RotationAtPlaceSpeed){
		OB->DeleteLastOrder();
		return;
	}
	if(OB->CObjIndex!=0xFFFF){
		bool SetDestDirForComplexObject(OneObject* OB,byte DestDir);
		if(SetDestDirForComplexObject(OB,OB->LocalOrder->info.MoveFrom.dir)){
			OB->DeleteLastOrder();
		}
		return;
	}
	int RotSpeed=OB->newMons->RotationAtPlaceSpeed;
	if(RotSpeed&&OB->BrigadeID!=0xFFFF){
		Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
		if(BR->NewBOrder&&BR->NewBOrder->GetBrigadeOrderID()==BRIGADEORDER_GOONROAD)RotSpeed=0;
	}
	if(RotSpeed){
		DWORD Dir=((OB->LocalOrder->info.MoveFrom.dir+8)&0xF0)<<8;
		short dd=short(OB->RealDirPrecise)-short(Dir);
		int dr=(int(OB->newMons->RotationAtPlaceSpeed)*GameSpeed)>>8;
		if(abs(dd)<dr){
			OB->RealDirPrecise=Dir;
			OB->RealDir=OB->RealDirPrecise>>8;
			OB->GraphDir=OB->RealDir;
			TryToStand(OB,0);
			OB->DeleteLastOrder();
			return;
		}else{
			if(dd<0){
                OB->RealDirPrecise+=dr;
				OB->RealDir=OB->RealDirPrecise>>8;
			}else{
				OB->RealDirPrecise-=dr;
				OB->RealDir=OB->RealDirPrecise>>8;
			}
			NewAnimation* NA=OB->newMons->GetAnimation(anm_RotateAtPlace);
			if(NA){
                OB->NewAnm=NA;
				OB->NewCurSprite=(int(word(OB->RealDirPrecise-64*256))*NA->NFrames)>>16;
				OB->CurrentFrameLong=OB->NewCurSprite<<8;
				if(rando()<256)OB->DestX=-1;
			}
		}
	}else{
		if(OB->MotionStyle==3){
			//obsolete
		}else{
			int Mrot=OB->newMons->MinRotator;
            if(OB->LockType==1)Mrot>>=5;
			if(!Mrot)Mrot=1;
			char Dir=OB->LocalOrder->info.MoveFrom.dir;
			char dd=char(OB->RealDir)-Dir;
			if(abs(dd)<=Mrot){
				OB->RealDir=Dir;
				OB->RealDirPrecise=int(OB->RealDir)<<8;
				OB->GraphDir=OB->RealDir;
				OB->DeleteLastOrder();
                OB->RealDirPrecise=int(OB->RealDir)<<8;
				return;
			};
			int dd2=Mrot*GameSpeed;
			if(dd2>Mrot*256)dd2=Mrot*256;
			if(dd>0)OB->RealDirPrecise-=dd2;
			else OB->RealDirPrecise+=dd2;
			OB->RealDir=OB->RealDirPrecise>>8;
			OB->GraphDir=OB->RealDir;
		};
	}
};

void RotUnit(OneObject* OB,char Dir,byte OrdType, byte PrioryLevel){
	addrand(1028);
	if(OB->LocalOrder&&OB->LocalOrder->DoLink==&RotUnitLink)return;
	if(OB->Transport||OB->MotionStyle==5||OB->NewBuilding||OB->newMons->ShowInsideUnits)return;	
	addrand(1029);
	int RotSpeed=OB->newMons->RotationAtPlaceSpeed;
	if(RotSpeed&&OB->BrigadeID!=0xFFFF){
		Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
		if(BR->NewBOrder&&BR->NewBOrder->GetBrigadeOrderID()==BRIGADEORDER_GOONROAD)RotSpeed=0;
	}
	if(RotSpeed){
		OB->RealDir=(OB->RealDir+8)&0xF0;
		OB->RealDirPrecise=int(OB->RealDir)<<8;
		byte Dir1=(Dir+8)&0xF0;
		if(OB->RealDir==Dir1){
			OB->RealDir=Dir;
			OB->RealDirPrecise=int(OB->RealDir)<<8;
			return;
		}
	}
    bool unl=OB->UnlimitedMotion;
	OB->UnlimitedMotion=0;
	Order1* Or1=OB->CreateOrder(OrdType);
	if(OrdType!=2)OB->PrioryLevel=PrioryLevel&127;
	OB->UnlimitedMotion=unl;
	if(Or1){
		Or1->PrioryLevel=PrioryLevel;
		Or1->info.MoveFrom.dir=Dir;
		Or1->DoLink=&RotUnitLink;
		Or1->OrderType=12;
	};
	/*if(OB->LockedInBuilding&&(OrdType==0||OrdType==1)){		
		Or1->DoLink(OB);
		if(OB->LocalOrder==Or1)OB->DeleteLastOrder();
	}*/
};
CEXPORT void RotUnit(OneObject* OB,char Dir,byte OrdType){
	addrand(1842);
	RotUnit(OB,Dir,OrdType,0);
}

CEXPORT void RotUnit(int Index,char Dir,byte OrdType)
{
	if(Index!=0xFFFF)
	{
		addrand(Index);
		OneObject* OB=Group[Index];
		if(OB)
			RotUnit(OB, Dir, OrdType);
	}
}
bool RotateUnit(OneObject* OB,char Dir){
	addrand(OB->Index);
	int Mrot=OB->newMons->MinRotator;
	char dd=char(OB->RealDir)-Dir;
	if(abs(dd)<=Mrot)return false;
	RotUnit(OB,Dir,0);
	return true;
};
void NewMonsterPreciseSendToLink(OneObject* OB);
void B_KeepPositionsLink(Brigade* BR){
	byte pri=BR->BOrder->Prio;
	bool InAttack=0;
	if(pri==128){
		word* Memb=BR->Memb;
		int N=BR->NMemb;
		for(int i=0;i<N;i++){
			word MID=Memb[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&(!OB->Sdoxlo)&&(OB->Attack&&!OB->ActivityState==1))return;
			};
		};
	};
	if(BR->PosCreated){
		// Change unit speed
		int RS=0;
		int RS2=0;
		int RSOld=0;
		int NR=0;
		for(int i=0;i<BR->NMemb;i++)
		{
			word MID=BR->Memb[i];
			if(MID!=0xFFFF)
			{
				OneObject* OB=Group[MID];
				if(OB&&!OB->Sdoxlo)
				{
					if(!OB->newMons->BRandomSpeed)
					{
						int R=Norma((OB->RealX>>4)-BR->posX[i],(OB->RealY>>4)-BR->posY[i]);
						RSOld+=R;
						RS+=R*(R);
						RS2+=R;
						NR++;
					}
				}
			}
		}
	
		if(RS2)
		{
			RS=RS/RS2;
			//RSOld=RSOld/NR;
			//if(RSOld>1000)
				//RS=RSOld;
		}

		if(RS>0)
		{
			for(int i=0;i<BR->NMemb;i++)
			{
				word MID=BR->Memb[i];
				if(MID!=0xFFFF)
				{
					OneObject* OB=Group[MID];
					if(OB&&!OB->Sdoxlo)
					{
						int R=Norma((OB->RealX>>4)-BR->posX[i],(OB->RealY>>4)-BR->posY[i]);
						int V=(R*64)/RS;
						if(V>112)V=112;
						if(V<40)V=40;
						addrand(V);
						OB->UnitSpeed=V;
					}
				}
			}
		}
		//
		void NewAttackPointLink(OneObject* OBJ);
		int State=0;
		bool Ready=1;
		if(BR->AttEnm)State=1;
		int Immediate=0;
		if(BR->PosCreated){
			for(int p=NBPERSONAL;p<BR->NMemb;p++){
				word MID=BR->Memb[p];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&!OB->Sdoxlo){
						if(OB->InMotion)Immediate++;
					};
				};			
			};
		};
		if(Immediate<6&&!BR->DontWait){
			for(int p=NBPERSONAL;p<BR->NMemb;p++){
				word MID=BR->Memb[p];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&!(OB->InMotion||OB->Sdoxlo||(!CheckIfPossibleToBreakOrder(OB)))){
						if(Norma((OB->RealX>>4)-BR->posX[p],(OB->RealY>>4)-BR->posY[p])>5){
							if(OB->NewState!=State){							
								if((!BR->AttEnm)&&OB->Attack)OB->ClearOrders();
								OB->NewState=State;
								OB->GroundState=State;
								addrand(OB->GroundState+1);
								TryToStand(OB,0);
							}
							bool FN=0;
							if(!(OB->LocalNewState==State&&(FN=(OB->FrameFinished||OB->NewAnm->CanBeBrokenAfterFrame<=OB->NewCurSprite)))){
								Ready=0;
								if(!FN){
									OB->SetNextFrame();
								};
							};
							OB->StandTime=0;
							char dd=BR->Direction-OB->RealDir;
							char ad=char(abs(int(dd)));
							if(ad){
								int md=OB->newMons->MinRotator;//*4;
								if(ad<md){
									OB->RealDir=BR->Direction;
								}else{
									if(dd>0)OB->RealDir+=md;
									else OB->RealDir-=md;
								};
								OB->GraphDir=OB->RealDir;
							}
						}
					}
				}
			}
			if(!Ready)return;
		};
		if(BR->Precise){
			bool Done=true;
			word* Memb=BR->Memb;
			int N=BR->NMemb;
			char DIRC=BR->Direction;
			int MAXD=0;
			for(int i=0;i<N;i++){
				word MID=Memb[i];
				int PD=Done;
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&!(OB->Sdoxlo||(!CheckIfPossibleToBreakOrder(OB)))){
						int xi=int(BR->posX[i])<<4;
						int yi=int(BR->posY[i])<<4;
						int dd=Norma(xi-OB->RealX,yi-OB->RealY);
						if(dd>MAXD&&i>=NBPERSONAL)MAXD=dd;
						if((!OB->LocalOrder)||(OB->EnemyID!=0xFFFF&&(pri&127))){
							
							//int dd=Norma(xi-OB->RealX,yi-OB->RealY);
							//if(dd>MAXD)MAXD=dd;
							word CheckMotionThroughEnemyAbility(OneObject* OB,int px,int py);
							word ENMID=CheckMotionThroughEnemyAbility(OB,OB->RealX,OB->RealY);
							if(ENMID==0xFFFF){
								int Lx=OB->Lx;
								if(dd>100){
									bool PDN=Done;
									Done=false;
									if(dd<768){
										int xx1=(xi-(Lx<<7))>>8;
										int yy1=(yi-(Lx<<7))>>8;
										if(!UnitsField.CheckBar(xx1,yy1,Lx,Lx)){
											addrand(pri);
											OB->NewMonsterPreciseSendTo(xi,yi,pri,0);
										}else{
											if(RotateUnit(OB,DIRC))Done=false;
											else Done=PDN;
										};
									}else{
										addrand(pri+1);
										OB->NewMonsterPreciseSendTo(xi,yi,pri,0);
									}
								}else{
									if(RotateUnit(OB,DIRC))Done=false;
								}
							}
						}else Done=false;
						
					};
				};
				if(PD&&!Done){
					if(rando()<18000)Done=true;
				};
			};
			if(MAXD<1600&&BR->DontWait)Done=1;
			if(Done){
				BR->DeleteBOrder();
				void MakeStandGroundTemp(Brigade *B);
				MakeStandGroundTemp(BR);
			};
		}else{
			bool Done=true;
			word* Memb=BR->Memb;
			int N=BR->NMemb;
			for(int i=0;i<N;i++){
				word MID=Memb[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&!OB->Sdoxlo){
						if(OB->LocalOrder&&rando()<12000)Done=false;
					};
				};
			};
			if(Done){
				BR->DeleteBOrder();
				void MakeStandGroundTemp(Brigade *B);
				MakeStandGroundTemp(BR);
			};
		};
	};
};
void B_LeaveAttackLink(Brigade* BR){
	if(BR->PosCreated){
		bool Done=true;
		word* Memb=BR->Memb;
		int N=BR->NMemb;
		for(int i=0;i<N;i++){
			word MID=Memb[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&!OB->Sdoxlo){
					if(OB->LocalOrder){
						if(OB->EnemyID==0xFFFF)Done=false;
						else OB->NewMonsterSendTo(int(BR->posX[i])<<4,int(BR->posY[i])<<4,16,0);
					};
				};
			};
		};
		if(Done){
			for(int i=0;i<N;i++){
				word MID=Memb[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&!OB->Sdoxlo){
						if(OB->LocalOrder){
							if(OB->EnemyID!=0xFFFF){
								BR->CT->UnRegisterNewUnit(OB);
							};
						};
					};
				};
			};
			BR->DeleteBOrder();
		};
	};
};
char* KP_Message="[KeepPosition]";
int GetNorm(int dx,int dy){
	return sqrt(dx*dx+dy*dy);
}
int CreateCurvedWay(int x0,int y0,char dir0,int x1,int y1,int* xy,int& N){
	char dir1=GetDir(x1-x0,y1-y0);
	int dd=abs(int(char(dir1-dir0)));
	if(dd<32){
		int dx=x1-x0;
		int dy=y1-y0;
        int R0=GetNorm(x1-x0,y1-y0);
		if(R0>0){
			int dvx=TCos[byte(dir0)];
			int dvy=TSin[byte(dir0)];
			int sc=(dvx*dx+dvy*dy)/R0;
			dvx=dvx*R0/2/sc;
			dvy=dvy*R0/2/sc;

			int NP=4;
			for(int i=0;i<=NP;i++){
				xy[i+i]=x0+dvx*i/NP+i*i*(dx-dvx)/NP/NP;
				xy[i+i+1]=y0+dvy*i/NP+i*i*(dy-dvy)/NP/NP;
			}
			N=NP+1;
			int L=0;
			for(int i=1;i<=NP;i++)L+=GetNorm(xy[i+i]-xy[i+i-2],xy[i+i+1]-xy[i+i-1]);
			return L;
		}
	}
	xy[0]=x0;
	xy[1]=y0;
	xy[2]=x1;
	xy[3]=y1;
	N=2;
	return GetNorm(x1-x0,y1-y0);
}
void Brigade::KeepPositions(byte OrdType,byte Prio){
	BrigadeOrder_KeepPositions* KP = new BrigadeOrder_KeepPositions();
	CreateNewBOrder(OrdType,KP);
    KP->OrderTime=AnimTime;
	for(int i=0;i<NMemb;i++){
		word MID=Memb[i];
		if(MID!=0xFFFF){
            OneObject* OB=Group[MID];
			if(OB&&OB->Serial==MembSN[i])
			{
				//OneObject* ENM=NULL;
				//if(OB->EnemyID!=0xFFFF){
				//	ENM=Group[OB->EnemyID];
				//}
				addname(OB->Index);
				addrand(OB->RealX);
				addrand(OB->RealY);
				if(CheckIfPossibleToBreakOrder(OB)){				
					if(!(OB->Attack&&(OB->ActivityState==2/*||(ENM&&ENM->NewBuilding)*/))){						
						addrand(OB->LocalOrder!=NULL);
						OB->ClearOrders();
					}
				}
				//OB->RifleAttack=0;
				if(!OB->Sdoxlo&&OB->newMons->BRandomPos)
				{
					addrand(posX[i]);
					addrand(posY[i]);
					posX[i]=posX[i]+(OB->newMons->BRandomPos/2-rando()%OB->newMons->BRandomPos);
					posY[i]=posY[i]+(OB->newMons->BRandomPos/2-rando()%OB->newMons->BRandomPos);
					addrand(posX[i]);
					addrand(posY[i]);
					addrand(i);
				}
			}
		}
	}
	KP->Process(this);
	/*
	int N=NMemb;
	char DIRC=Direction;
	bool Attack=false;
	int RS=0;
	int RS2=0;
	int RSOld=0;
	int NR=0;
	{
		for(int i=0;i<N;i++){
			word MID=Memb[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&!OB->Sdoxlo){
					if(OB->newMons->BRandomPos)
					{
						addrand(posX[i]);
						addrand(posY[i]);
						posX[i]=posX[i]+(OB->newMons->BRandomPos/2-rando()%OB->newMons->BRandomPos);
						posY[i]=posY[i]+(OB->newMons->BRandomPos/2-rando()%OB->newMons->BRandomPos);
						addrand(posX[i]);
						addrand(posY[i]);
						addrand(i);
					}
					int R=Norma((OB->RealX>>4)-posX[i],(OB->RealY>>4)-posY[i]);
					RSOld+=R;
					RS+=R*(R+64);
					RS2+=R+64;
					NR++;
					if(Prio==142&&OB->newMons->ArmAttack){
						OB->RifleAttack=0;
					}
				};
			};
		};		
		RS=RS/RS2;
		if(NR)
		{
			RSOld=RSOld/NR;
			if(RSOld>1000)
				RS=RSOld;
		}
	}
	Precise=1;
	for(int i=0;i<N;i++){
		word MID=Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo){
				if(OB->EnemyID!=0xFFFF)Attack=true;
				int xi=int(posX[i])<<4;
				int yi=int(posY[i])<<4;
				if(RS){
					int R=Norma((OB->RealX>>4)-posX[i],(OB->RealY>>4)-posY[i]);
					addrand(posX[i]);
					addrand(posY[i]);
					addrand(OB->RealX);
					addrand(OB->RealY);
					addrand(i);
					addrand(R);
					int V=(R*64)/RS;
					addrand(V);
					addrand(OB->newMons->BRandomSpeed);
					if(OB->newMons->BRandomSpeed)
					{
						int NS=OB->newMons->BRandomSpeed*100; 
						V=V+(V*(NS/2-rando()%NS))/10000;
						addrand(V);
						addrand(123);
					}
  					if(V>112)V=112;
 					if(V<24)V=24;
					addrand(V);
					OB->UnitSpeed=V;
					addrand(OB->UnitSpeed);
				}else OB->UnitSpeed=64;				
				addrand(Prio);
				OB->NewMonsterPreciseSendTo(xi,yi,Prio,0);
			};
		};
	};	
	*/
};

void DrawPlaneLine(int x0,int y0,int x1,int y1,byte c){
	int dx=mapx<<5;
	int dy=mapy<<4;
	int h0=GetHeight(x0,y0);
	int h1=GetHeight(x1,y1);
	x0-=dx;
	y0=(y0>>1)-h0-dy;
	x1-=dx;
	y1=(y1>>1)-h1-dy;
	DrawLine(x0,y0,x1,y1,c);
};
void DrawPlaneLine2(int x0,int y0,int x1,int y1,DWORD c){	
	int h0=GetHeight(x0,y0);
	int h1=GetHeight(x1,y1);
	Vector3D V0 = SkewPt( x0,y0,h0);
	Vector3D V1 = SkewPt( x1,y1,h1);
	GPS.DrawLine(V0,V1,c);
};
void DrawMultiLine(int x0,int y0,int x1,int y1,byte c){
	int dx=(x1-x0)<<16;
	int dy=(y1-y0)<<16;
	int r=Norma(dx,dy);
	if(!r)return;
	int xx=0;
	int yy=0;
	int N=(r>>(16+5))+1;
	dx/=N;
	dy/=N;
	for(int i=0;i<N;i++){
		DrawPlaneLine(x0+(xx>>16),y0+(yy>>16),x0+((xx+dx)>>16),y0+((yy+dy)>>16),c);
		xx+=dx;
		yy+=dy;
	};
};
void DrawRotLine(int x0,int y0,int dx0,int dy0,int dx1,int dy1,int SIN,int COS,byte c){
	DrawMultiLine(x0+((dx0*COS-dy0*SIN)>>8),y0+((dy0*COS+dx0*SIN)>>8),
		          x0+((dx1*COS-dy1*SIN)>>8),y0+((dy1*COS+dx1*SIN)>>8),c);
};
word GetBrigLockType(Brigade* BR){
	addrand(BR->ID);
	addrand(BR->CT->NI);
	bool Change=false;
	byte NewLockType=0;
	OrderDescription* ODS=NULL;
	if(BR->WarType){
		ODS=ElementaryOrders+BR->WarType-1;
		if(ODS->OrdUsage==0){
			NewLockType=3;
		}
	}
	int N=BR->NMemb;
	word* ID=BR->Memb;
	word* SN=BR->MembSN;
	for(int i=0;i<N;i++){
		word UID=ID[i];
		if(UID!=0xFFFF){
			OneObject* OB=Group[UID];
			if(OB&&OB->Serial==SN[i]&&!OB->Sdoxlo){
				if(Change){
					OB->LockType=NewLockType;
					continue;
				}
				byte LT=OB->LockType;
				if(ODS&&(LT==0||LT==3)&&LT!=NewLockType){
					OB->LockType=NewLockType;
					Change=true;					
				}else{
					return OB->LockType;
				}
			}
		}
	}
	return NewLockType;
};
word GetBrigLockTypeIntf(Brigade* BR){	
	int N=BR->NMemb;
	word* ID=BR->Memb;
	word* SN=BR->MembSN;
	for(int i=0;i<N;i++){
		word UID=ID[i];
		if(UID!=0xFFFF){
			OneObject* OB=Group[UID];
			if(OB&&OB->Serial==SN[i]&&!OB->Sdoxlo){
				return OB->LockType;
			}
		}
	}
	return 0xFFFF;
};
void B_BitvaLink(Brigade* BR);
byte GetPaletteColor(int r,int g,int b);
extern bool StrelMode;
extern DWORD SStartTime;
void SetCurrentDiffuse(DWORD);
bool GetObjectVisibilityInFog(int x,int y,int z,OneObject* OB);

void DrawBorder(Brigade* BR, byte cl)
{
	///if(LMode)return;
	GPS.EnableZBuffer(0);
	//static int SB_img = GPS.PreLoadGPImage("selbar");
	int SB_img = GSets.SelBarGP; //GPS.PreLoadGPImage("selbar");
	//static int sh=IRS->GetShaderID("hud");
	//GPS.SetCurrentShader(sh);
	//GPS.SetCurrentDiffuse(0xFFFFFFFF);
	
	if(BR->WarType&&BR->NMemb>NBPERSONAL&&BR->PosCreated)
	{
		static int pt=0;
		if(tmtmt-pt>1){
			pt=tmtmt;
		}
		pt=tmtmt;
		OrderDescription* ODS=ElementaryOrders+BR->WarType-1;
		int SIN=-TSin[byte(BR->Direction)];
		int COS=TCos[byte(BR->Direction)];
		int xt=0;
		int yt=0;
		int Nt=0;
		int MinX=10000000;
		int MaxX=-10000000;
		int MinY=10000000;
		int MaxY=-10000000;
		int N=BR->NMemb;
		bool Cross=false;
		/*
		if(BR->BOrder){
			if(BR->BOrder->BLink==&B_KeepPositionsLink&&(!StrelMode||(GetTickCount()-SStartTime)<200))
				Cross=true;
			if(BR->BOrder->BLink==&B_BitvaLink){
				//Cross=true;
				cl=0xC9;
			};
		};
		*/
		if(BR->NewBOrder){
			if(BR->NewBOrder->GetBrigadeOrderID()==BRIGADEORDER_KEEPPOSITION&&(!StrelMode||(GetTickCount()-SStartTime)<200))
				Cross=true;
			if(BR->NewBOrder->GetBrigadeOrderID()==BRIGADEORDER_BITVA){
				cl=0xC9;
			};
		};
		int nd=0;
        int xFin=-1;
		int yFin=-1;
		byte FinDir=0;
		int posXc=0;
		int posYc=0;
		int nPos=0;
		bool drawfin=false;
		if(BR->NewBOrder){
			CBrigadeOrder* BO=BR->NewBOrder;
			while(BO){
				word ID=BO->GetBrigadeOrderID();
				if(ID==BRIGADEORDER_HUMANGLOBALSENDTO){
                    BrigadeOrder_HumanGlobalSendTo* BOHG=(BrigadeOrder_HumanGlobalSendTo*)BO;
					xFin=BOHG->xx;
					yFin=BOHG->yy;
					FinDir=BOHG->Dir;
					drawfin=true;
				}
				BO=BO->Next.Get();
			}
		}
		if(drawfin&&BR->PosCreated){
			for(int i=NBPERSONAL;i<BR->NMemb;i++)			{
				posXc+=BR->posX[i];
				posYc+=BR->posY[i];
				nPos++;
			}
		}
		if(nPos){
			posXc/=nPos;
			posYc/=nPos;
		}
		if(xFin!=-1){
			BrigadesList BLIST;
			BLIST.CreateFromSelected(BR->CT->NI,TCos[FinDir],TSin[FinDir],BR->NMemb,BR->Memb,BR->MembSN);
			BLIST.Transform(xFin,yFin,FinDir);
			BLIST.ShowPositions();
		}
		for(int i=NBPERSONAL;i<N;i++){
			word MID=BR->Memb[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&!OB->Sdoxlo){
					if(GetObjectVisibilityInFog(OB->RealX>>4,OB->RealY>>4,OB->RZ,OB)){
						xt+=OB->RealX>>4;
						yt+=OB->RealY>>4;
						Nt++;
						int xx,yy;
						if(Cross&&EngSettings.DrawBrigadeDestPositions){
							xx=BR->posX[i];
							yy=BR->posY[i];
							void DrawPlaneLine2(int x0,int y0,int x1,int y1,DWORD c);														
							if(drawfin){
								int dxx=xFin-posXc;
								int dyy=yFin-posYc;
								if(Norma(dxx,dyy)>400){
									DrawPlaneLine2(xx,yy-2,xx,yy+2,0xFF00FFFF);
									DrawPlaneLine2(xx-2,yy,xx+2,yy,0xFF00FFFF);
								}
								//xx=BR->posX[i]+dxx;
								//yy=BR->posY[i]+dyy;
								//DrawPlaneLine2(xx,yy-2,xx,yy+2,0xFF00FFFF);
								//DrawPlaneLine2(xx-2,yy,xx+2,yy,0xFF00FFFF);								
							}else{
								DrawPlaneLine2(xx,yy-2,xx,yy+2,0xFF00FFFF);
								DrawPlaneLine2(xx-2,yy,xx+2,yy,0xFF00FFFF);
							}
						};
						
						static Vector3D s_Pivot( GPS.GetGPWidth( SB_img, 0 )*0.5f, GPS.GetGPHeight( SB_img, 0 )*0.5f, 0.0f );
						xx = (OB->RealX>>4)+EngSettings.MarkerDx;
						yy = (OB->RealY>>4)+EngSettings.MarkerDy;
						Vector3D pos = SkewPt( xx, yy, OB->RZ );
						NewMonster* NM=OB->newMons;
						int id=NM->selIndexBR;
						if(id==-1){
							Matrix4D tm = GetAlignTerraTransform( pos, s_Pivot );
							tm.translate( pos );
							GPS.DrawWSprite( SB_img, 0, tm );
						}else{
							bool DrawSelPatchDir(float x,float y,byte Dir,int Type,float ScaleX,float ScaleY,DWORD Color);
							DrawSelPatchDir(OB->RealX/16,OB->RealY/16,BR->Direction,id,NM->selScaleX,NM->selScaleY,0xFFFFFFFF);
						}
						nd++;
					}
				}
			}
		}
		if(nd!=N-NBPERSONAL){
			nd=1;
		}
		if(StrelMode&&(GetTickCount()-SStartTime)>200){
			if(!RED_COLOR){
				RED_COLOR=GetPaletteColor(255,0,0);
			};
			word LockType=GetBrigLockTypeIntf(BR);
			int N=PORD.NUnits;
			int* x=PORD.px;
			int* y=PORD.py;
			for(int i=0;i<N;i++){
				int xx=x[i]>>4;
				int yy=y[i]>>4;
				int ofs=(xx>>6)+((yy>>6)<<TopSH);
				if(ofs>=0&&ofs<(1<<(TopSH+TopSH))){
					if(GetTopRef(ofs,LockType)<0xFFFE){
						DrawPlaneLine2(xx,yy-2,xx,yy+2,0xFFFFFF00);
						DrawPlaneLine2(xx-2,yy,xx+2,yy,0xFFFFFF00);
					}else{
						DrawPlaneLine2(xx,yy-2,xx,yy+2,0xFFFF0000);
						DrawPlaneLine2(xx-2,yy,xx+2,yy,0xFFFF0000);
					}
				}
			}
		}
		
		return;
		if(Nt){
			xt/=Nt;
			yt/=Nt;
		}else return;
		for(int i=NBPERSONAL;i<N;i++){
			word MID=BR->Memb[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&!OB->Sdoxlo){
					int x=(OB->RealX>>4)-xt;
					int y=(OB->RealY>>4)-yt;
					int xs=(x*SIN+y*COS)>>8;
					int ys=(y*SIN-x*COS)>>8;
					if(xs<MinX)MinX=xs;
					if(xs>MaxX)MaxX=xs;
					if(ys<MinY)MinY=ys;
					if(ys>MaxY)MaxY=ys;
				};
			};
		};
		if(MaxX>MinX){
			MaxX+=30;
			MinX-=30;
			MaxY+=30;
			MinY-=30;
			DrawRotLine(xt,yt,MinX,MinY,MinX,MaxY,COS,SIN,clrYello);
			DrawRotLine(xt,yt,MinX,MaxY,MaxX,MaxY,COS,SIN,clrYello);
			DrawRotLine(xt,yt,MaxX,MaxY,MaxX,MinY,COS,SIN,clrYello);
			DrawRotLine(xt,yt,MaxX,MinY,MinX,MinY,COS,SIN,clrYello);
		};
	};
	///GPS.FlushBatches();
};
void DrawBorder(Brigade* BR){
	DrawBorder(BR,clrYello);	
}
extern int FORMDIST;
int OScale(int x){
	return (x*FORMDIST)>>8;
};
bool FindCorrectTopPos(int x0,int y0,int* x,int* y){
	int top0=GetTopology(&x0,&y0);
	if(top0==0xFFFF)return false;
	x0<<=8;
	y0<<=8;
	int dx=((*x)<<8)-x0;
	int dy=((*y)<<8)-y0;
	int Nr=Norma(dx,dy);
	int N=(Nr>>(8+5))+1;
	dx/=N;
	dy/=N;
	for(int i=0;i<=N;i++){
		if(CheckBar((x0>>12)-2,(y0>>12)-2,4,4)){
			*x=x0>>8;
			*y=y0>>8;
			return true;
		}else{
			if(i<N){
				x0+=dx;
				y0+=dy;
			}else{
				*x=x0>>8;
				*y=y0>>8;
				return true;
			};
		};
	};
	return false;
};
bool FindCorrectTopPos1(int x0,int y0,int* x,int* y,byte LockType=0){
	int top0=GetTopology(&x0,&y0,LockType);
	if(top0==0xFFFF)return false;
	x0>>=4;
	y0>>=4;
	for(int r=0;r<20;r++){
		char* xi=Rarr[r].xi;
		char* yi=Rarr[r].yi;
		int N=Rarr[r].N;
		for(int j=0;j<N;j++){
			int x1=x0+xi[j];
			int y1=y0+yi[j];
			int tpx1=x1>>2;
			int tpy1=y1>>2;
			if(tpx1>0&&tpy1>0&&tpx1<TopLx&&tpy1<TopLx){
				if(!MFIELDS[LockType].CheckBar(x1-1,y1-1,3,3)){
					word top=GetTopRef(tpx1+(tpy1<<TopSH),LockType);
					if(top==top0){
						*x=(x1<<4);
						*y=(y1<<4);
						return true;
					};
				};
			};
		};
	};
	return false;
};
int FindSuperSmartBestPosition(OneObject* OB,int* cx,int* cy,int dx,int dy,word Top,byte LTP,int uidx);
int FindSuperSmartBestPositionForBrigade(OneObject* OB,int* cx,int* cy,int dx,int dy,word Top,byte LTP,int uidx);
void OptimiseBrigadePosition(Brigade* BR){
	int MinX= 10000000;
	int MinY= 10000000;
	int MaxX=-10000000;
	int MaxY=-10000000;
	int N=BR->NMemb;
	word* Mem=BR->Memb;
	int* px=BR->posX;
	int* py=BR->posY;
	byte LockType=0xFF;
	for(int i=0;i<N;i++){
		if(Mem[i]!=0xFFFF){
			OneObject* OB=Group[Mem[i]];
			if(OB&&!OB->Sdoxlo) LockType=OB->LockType;
			int x=px[i];
			int y=py[i];
			addrand(x);
			addrand(y);
			addrand(i);
			if(x>MaxX)MaxX=x;
			if(x<MinX)MinX=x;
			if(y>MaxY)MaxY=y;
			if(y<MinY)MinY=y;
		};
	};
	if(LockType==0xFF) LockType=0;
	MotionField* MF=MFIELDS+LockType;

	int GMin=64;
	int GMax=(msx-2)<<5;
	int Dx0=0;
	int Dy0=0;
	if(MinX<GMin)Dx0=-MinX+GMin;
	if(MinY<GMin)Dy0=-MinY+GMin;
	if(MaxX>GMax)Dx0=GMax-MaxX;
	if(MaxY>GMax)Dy0=GMax-MaxY;
	int x0=((MinX+MaxX)>>1);
	int y0=((MinY+MaxY)>>1);

	int Top0=GetTopology(x0,y0,LockType);
	//search for best place
	int MinFailed=10000;
	int BestDx=0;
	int BestDy=0;
	bool UnFoundBP=true;
	for(int r=0;r<6;r++){
		int Np=Rarr[r].N;
		char* xi=Rarr[r].xi;
		char* yi=Rarr[r].yi;
		for(int j=0;j<Np;j++){
			int DX=(int(xi[j])<<5);
			int DY=(int(yi[j])<<5);
			int dx=Dx0+DX;
			int dy=Dy0+DY;
			int xt=(x0+DX)>>6;
			int yt=(y0+DY)>>6;
			int Top1=0xFFFF;
			if(xt>0&&xt<TopLx&&yt>0&&yt<TopLx){
				Top1=GetTopRef(xt+(yt<<TopSH),LockType);
			};
			if(Top1==Top0){
				int nf=0;
				for(int k=0;k<N;k++){
					if(Mem[k]!=0xFFFF){
						if(MF->CheckBar(((px[k]+dx)>>4)-1,((py[k]+dy)>>4)-1,3,3))nf++;
					};
				};
				if(!nf){
					BestDx=dx;
					BestDy=dy;
					MinFailed=0;
					goto ChangePos;
				};
				if(nf<MinFailed){
					BestDx=dx;
					BestDy=dy;
					MinFailed=nf;
				};
			};
		};
	};
ChangePos:
	if(BestDx||BestDy){
		for(int i=0;i<N;i++){
			if(Mem[i]!=0xFFFF){
				px[i]+=BestDx;
				py[i]+=BestDy;
			};
		};
	};
	if(MinFailed){
		int uidx=0;
		//need to shift some positions
		int x0=((MinX+MaxX)>>1)+BestDx;
		int y0=((MinY+MaxY)>>1)+BestDy;
		for( int i=0;i<N;i++){
			if(Mem[i]!=0xFFFF){
				addrand(px[i]);
				addrand(py[i]);
				int x=px[i];
				int y=py[i];
				int xx=x0;
				int yy=y0;
				int v=FindSuperSmartBestPositionForBrigade(NULL,&xx,&yy,x-xx,y-yy,0,LockType,uidx);
				if(v){
					px[i]=xx;
					py[i]=yy;
					if(v==2)uidx++;
					//assert(!CheckBar((xx>>4)-1,(yy>>4)-1,3,3));
				}else{
					FindCorrectTopPos1(x0,y0,&x,&y,LockType);
					px[i]=x;
					py[i]=y;
					//assert(!CheckBar((x>>4)-1,(y>>4)-1,3,3));
				};
				addrand(px[i]);
				addrand(py[i]);
				addrand(i);
			};
		};
		BR->Precise=false;
	}else{
		BR->Precise=true;
	};
	//BR->Precise=false;
};
//===================================================================================================================//
class BrigadeUnitPosition
{
public:
	BrigadeUnitPosition()
	{
		State=0;
		Unit=0xFFFF;
		UnitIndexInBrigade=0xFFFF;
		x=-1;
		y=-1;
		Shift=0;
		Part=NULL;
	};
	byte State;
	word Unit;
	word UnitIndexInBrigade;
	int x;
	int y;
	int Shift;
	int** Part;
};

DynArray<BrigadeUnitPosition*> positionPool;
int positionPoolIndex;

class BrigadeUnitPositionLine
{
public:
	DynArray<BrigadeUnitPosition*> Line;
	int LineNumber;
	~BrigadeUnitPositionLine()
	{	
		/*
		int n = Line.GetAmount();
		for(int i=0;i<n;i++)
		{
			if(Line[i])
				delete (Line[i]);
			Line[i]=NULL;
		}
		*/
	};
	bool AddUnitPos(int Shift, word Unit, word UnitIndexInBrigade, int x, int y, MotionField* MF,BrigadeUnitPosition* neighbor)
	{
		bool rez=true;
		int n=positionPool.GetAmount();
		BrigadeUnitPosition* P = NULL;
		if(positionPoolIndex<n)
		{
			P=positionPool[positionPoolIndex];
			positionPoolIndex++;
		}
		else
		{
			positionPool.Add(new BrigadeUnitPosition);
			P=positionPool[positionPoolIndex];
			positionPoolIndex++;
		}
		//BrigadeUnitPosition* P = new BrigadeUnitPosition();
		P->Shift=Shift;
		P->Unit=Unit;
		P->UnitIndexInBrigade=UnitIndexInBrigade;
		P->x=x;
		P->y=y;
		P->Part=NULL;
		if(MF->CheckBar((x>>4)-1,(y>>4)-1,3,3))
		{
			P->State=0xFF;
			rez=false;
		}
		else
		{
			/*
			if(neighbor&&(neighbor->State==2||neighbor->State==1))
			{
				P->State=2;//Svobodno
			}
			else
			{
				P->State=3;
				rez=false;
			}
			*/
			P->State=3;
			rez=true;
			if(Unit!=0xFFFF)
			{
				P->State=1;
			}
		}
		Line.Add(P);
		return rez;
	};
	bool AddPosInLeft(MotionField* MF)
	{
		bool rez=false;
		int N = Line.GetAmount();
		if(N>1)
		{
			BrigadeUnitPosition* P = NULL;
			int n=positionPool.GetAmount();
			if(positionPoolIndex<n)
			{
				P=positionPool[positionPoolIndex];
				positionPoolIndex++;
			}
			else
			{
				positionPool.Add(new BrigadeUnitPosition);
				P=positionPool[positionPoolIndex];
				positionPoolIndex++;
			}
			//BrigadeUnitPosition* P = new BrigadeUnitPosition();
			int dx=Line[0]->x-Line[1]->x;
			int dy=Line[0]->y-Line[1]->y;
			P->x=Line[0]->x+dx;
			P->y=Line[0]->y+dy;
			P->Shift=Line[0]->Shift+Line[0]->Shift-Line[1]->Shift;
			P->Part=NULL;
			//if(MF->CheckBar((P->x>>4)-1,(P->y>>4)-1,3,3))
			if(MF->CheckPt(P->x>>4,P->y>>4))
			{
				P->State=0xFF;
			}
			else
			{
				if(Line[0]->State==2||Line[0]->State==1)
				{
					P->State=2;
					//rez=true;
				}
				else
					P->State=3;//Island
				P->State=3;
			}
			Line.Insert(0,P);
		}
		return rez;
	};
	bool AddPosInRight(MotionField* MF)
	{
		bool rez=false;
		int N = Line.GetAmount();
		if(N>1)
		{
			int l=N-1;
			int pl=l-1;
			//BrigadeUnitPosition* P = new BrigadeUnitPosition();
			BrigadeUnitPosition* P = NULL;
			int n=positionPool.GetAmount();
			if(positionPoolIndex<n)
			{
				P=positionPool[positionPoolIndex];
				positionPoolIndex++;
			}
			else
			{
				positionPool.Add(new BrigadeUnitPosition);
				P=positionPool[positionPoolIndex];
				positionPoolIndex++;
			}
			int dx=Line[l]->x-Line[pl]->x;
			int dy=Line[l]->y-Line[pl]->y;
			P->x=Line[l]->x+dx;
			P->y=Line[l]->y+dy;
			P->Shift=Line[l]->Shift+Line[l]->Shift-Line[pl]->Shift;
			P->Part=NULL;
			//if(MF->CheckBar((P->x>>4)-1,(P->y>>4)-1,3,3))
			if(MF->CheckPt(P->x>>4,P->y>>4))
			{
				P->State=0xFF;
			}
			else
			{
				if(Line[l]->State==2||Line[l]->State==1)
				{
					P->State=2;
					//rez=true;
				}
				else
					P->State=3;//Island
				P->State=3;
			}
			Line.Add(P);
		}
		return rez;
	};
};
class BrigadeUnitPositionList
{
public:
	~BrigadeUnitPositionList()
	{
		int n = PositionList.GetAmount();		
		for(int i=0;i<n;i++)
		{
			if(PositionList[i])
				delete (PositionList[i]);
			PositionList[i]=NULL;
		}
		PositionList.Clear();
	};
	DynArray<BrigadeUnitPositionLine*> PositionList;
	
	void AddFirstLine(BrigadeUnitPositionLine* Line)
	{
		PositionList.Insert(0,Line);
	};
	void AddLastLine(BrigadeUnitPositionLine* Line)
	{
		PositionList.Add(Line);
	};
	int AddFirstLine(MotionField* MF)
	{
		int rez=0;
		int N = PositionList.GetAmount();
		if(N>1)
		{
			BrigadeUnitPositionLine* L0 = PositionList[0];
			BrigadeUnitPositionLine* L1 = PositionList[1];
			int N0=L0->Line.GetAmount();
			int N1=L1->Line.GetAmount();
			if(N0>0&&N1>0)
			{
				BrigadeUnitPosition* P0 = L0->Line[0];
				BrigadeUnitPosition* P1 = L1->Line[0];
				int dx=P0->x-P1->x;
				int dy=P0->y-P1->y;
				BrigadeUnitPositionLine* NL =new BrigadeUnitPositionLine();
				NL->LineNumber=L0->LineNumber+L0->LineNumber-L1->LineNumber;
				for(int i=0;i<N0;i++)
				{
					BrigadeUnitPosition* P = L0->Line[i];
					if(NL->AddUnitPos(P->Shift,0xFFFF,0xFFFF,P->x+dx,P->y+dy,MF,P))
						rez++;
				}
				PositionList.Insert(0,NL);
			}
		}
		return rez;
	};
	int AddLastLine(MotionField* MF)
	{
		int rez=0;
		int N = PositionList.GetAmount();
		if(N>1)
		{
			BrigadeUnitPositionLine* L0 = PositionList[N-1];
			BrigadeUnitPositionLine* L1 = PositionList[N-2];
			int N0=L0->Line.GetAmount();
			int N1=L1->Line.GetAmount();
			if(N0>0&&N1>0)
			{
				BrigadeUnitPosition* P0 = L0->Line[0];
				BrigadeUnitPosition* P1 = L1->Line[0];
				int dx=P0->x-P1->x;
				int dy=P0->y-P1->y;
				BrigadeUnitPositionLine* NL =new BrigadeUnitPositionLine();
				NL->LineNumber=L0->LineNumber+L0->LineNumber-L1->LineNumber;
				for(int i=0;i<N0;i++)
				{
					BrigadeUnitPosition* P = L0->Line[i];
					if(NL->AddUnitPos(P->Shift,0xFFFF,0xFFFF,P->x+dx,P->y+dy,MF,P))
						rez++;
				}
				PositionList.Add(NL);
			}
		}
		return rez;
	};
	int AddSidePos(MotionField* MF)
	{
		int rez=0;
		int N= PositionList.GetAmount();
		for(int i=0;i<N;i++)
		{
			if(PositionList[i]->AddPosInLeft(MF))
				rez++;
			if(PositionList[i]->AddPosInRight(MF))
				rez++;
		}
		return rez;
	};
	int SetState(int Line, int Pos,  int FromState, int NewState)
	{
		int rez=0;
		int NL = PositionList.GetAmount();
		if(Line>-1&&Line<NL&&Pos>-1)
		{
			BrigadeUnitPositionLine* MyLine=PositionList[Line];
			int NU = MyLine->Line.GetAmount();
			if(Pos<NU)
			{
				if(MyLine->Line[Pos]->State==FromState)
				{
					rez++;
					MyLine->Line[Pos]->State=NewState;
					rez+=SetState(Line,Pos+1,FromState,NewState);
					rez+=SetState(Line,Pos-1,FromState,NewState);
					rez+=SetState(Line+1,Pos,FromState,NewState);
					rez+=SetState(Line-1,Pos,FromState,NewState);
				}
			}
		}
		return rez;
	};
};
bool CheckParts(BrigadeUnitPosition* P1, BrigadeUnitPosition* P2)
{
	bool rez=false;
	if(P1->State!=0xFF&&P2->State!=0xFF&&(P1->Part||P2->Part))
	{
		if(P1->Part&&!P2->Part)
		{
			(*(*(P1->Part)))++;
			P2->Part=P1->Part;
		}
		if(P2->Part&&!P1->Part)
		{
			(*(*(P2->Part)))++;
			P1->Part=P2->Part;
		}
		if(P1->Part&&P2->Part&&(*P1->Part)!=(*P2->Part))
		{
			(*(*(P1->Part)))+=(*(*(P2->Part)));
			(*(P2->Part))=(*(P1->Part));
		}
		rez=true;
	}
	return rez;
}
bool CheckAndFindDestPos(int cx, int cy, int& dx, int& dy, MotionField* MF)
{
	if(MF->CheckBar((dx>>4)-2,(dy>>4)-2,5,5))
	{
		int dist = Norma(cx-dx,cy-dy);
		if(dist>32&&dist<65535)
		{
			byte md = GetDir(cx-dx,cy-dy);
			int ddx=(TCos[md]*32)>>8;
			int ddy=(TSin[md]*32)>>8;
			dx+=ddx;
			dy+=ddy;
			CheckAndFindDestPos(cx,cy,dx,dy,MF);
		}
		return true;
	}
	return false;
}
void OptimiseBrigadePosition2(Brigade* BR)
{
	//bool dup = false;	
	positionPoolIndex=0;

	BR->NUnitsNotInPos=0;
	int N=BR->NMemb;
	word* Mem=BR->Memb;
	int* px=BR->posX;
	int* py=BR->posY;
	byte LockType=0xFF;
	int bcx=0;
	int bcy=0;
	int cx=0;
	int cy=0;
	int BScale=100;
	for(int i=0;i<N;i++)
	{
		if(Mem[i]!=0xFFFF)
		{
			OneObject* OB=Group[Mem[i]];
			if(OB&&!OB->Sdoxlo) 
			{
				LockType=OB->LockType;
				BScale=OB->newMons->FormationDistanceScale;
				bcx+=px[i];
				bcy+=py[i];
				cx+=OB->RealX>>4;
				cy+=OB->RealY>>4;
			}
		}
	}
	bcx/=N;
	bcy/=N;
	cx/=N;
	cy/=N;
	if(LockType==0xFF) LockType=0;
	MotionField* MF=MFIELDS+LockType;
	/* 
	if(CheckAndFindDestPos(cx,cy,bcx,bcy,MF))//Dest pos changet
	{
		BR->CreateSimpleOrderedPositions(bcx<<4,bcy<<4,BR->Direction);
	}
	*/
	
	OrderDescription* ODS=ElementaryOrders+BR->WarType-1;
	int NA=ODS->NActualLines;
	int FL=ODS->FirstActualLine;
	int bp=NBPERSONAL;
	int CP=0;

	int NLine=0;
	int NUnitsInPos=0;
	int NFreePos=0;
	DynArray<int> UnitsWithoutPos;
	BrigadeUnitPositionList BrigadePosition; 
	
	for(int p=0;p<NA;p++)
	{
		int NU=ODS->LineNU[p+FL];
		if(NU)
		{
			BrigadeUnitPositionLine* Li = new BrigadeUnitPositionLine();
			Li->LineNumber=p;
			short* Line = ODS->Lines[p+FL];
			for(int q=0;q<NU;q++)
			{
				bool tem=false;
				if(bp<BR->NMemb)
				{
					word ID=BR->Memb[bp];
					if(ID!=0xFFFF&&Group[ID]!=NULL)
					{
						tem=true;
						if(Li->AddUnitPos(Line[q],ID,bp,px[bp],py[bp],MF,NULL))
						{
							NUnitsInPos++;
						}
						else
						{
							UnitsWithoutPos.Add(bp);
						}
					}
				}
				if(!tem)
				{
					if(Li->AddUnitPos(Line[q],0xFFFF,0xFFFF,px[bp],py[bp],MF,NULL))
					{
						//NFreePos++;
					}
				}
				bp++;
			}
			BrigadePosition.AddLastLine(Li);
		}
	}
	/*
	if(NUnitsInPos==0)
	{		
		
		for(int i=0;i<BR->NMemb;i++)
		{
			word ID=BR->Memb[i];
			if(ID!=0xFFFF)
			{
				OneObject* OB = Group[BR->Memb[i]];
				if(OB)
				{
					px[i]=OB->RealX/16;
					py[i]=OB->RealY/16;
				}
			}
		}
		return;
	}
	*/
	
	DynArray<int> BrigadeParts;
	DynArray<int*> Parts;
	BrigadeParts.Check(BR->NMemb);
	Parts.Check(BR->NMemb);
	
	int nbl=BrigadePosition.PositionList.GetAmount();
	BrigadeUnitPositionLine* Pree=NULL;
	bool have2=false;
	for(int j=0;j<nbl;j++)
	{
		BrigadeUnitPositionLine* LL = BrigadePosition.PositionList[j];
		int nu = LL->Line.GetAmount();
		int nup = 0;
		if(Pree)
			nup=Pree->Line.GetAmount();
		for(int k=0;k<nu;k++)
		{
			if(LL->Line[k]->State!=0xFF)
			{
				if(k>0&&LL->Line[k-1]->State!=0xFF)
				{
					if(!CheckParts(LL->Line[k],LL->Line[k-1]))
					{
						BrigadeParts.Add(2);
						Parts.Add(&BrigadeParts[BrigadeParts.GetAmount()-1]);
						LL->Line[k]->Part=&(Parts[Parts.GetAmount()-1]);
						LL->Line[k-1]->Part=&(Parts[Parts.GetAmount()-1]);
					}
				}
				if(Pree&&k<nup&&Pree->Line[k]->State!=0xFF)
				{
					if(!CheckParts(LL->Line[k],Pree->Line[k]))
					{
						BrigadeParts.Add(2);
						Parts.Add(&BrigadeParts[BrigadeParts.GetAmount()-1]);
						LL->Line[k]->Part=&(Parts[Parts.GetAmount()-1]);
						Pree->Line[k]->Part=&(Parts[Parts.GetAmount()-1]);
					}
				}
				if(k<(nu-1)&&LL->Line[k+1]->State!=0xFF)
				{
					if(!CheckParts(LL->Line[k],LL->Line[k+1]))
					{
						BrigadeParts.Add(2);
						Parts.Add(&BrigadeParts[BrigadeParts.GetAmount()-1]);
						LL->Line[k]->Part=&(Parts[Parts.GetAmount()-1]);
						LL->Line[k+1]->Part=&(Parts[Parts.GetAmount()-1]);
					}
				}
			}
		}
		Pree=LL;
	}
	
	int nw = Parts.GetAmount();
	/*
	if(nw==0){
		for(int i=0;i<BR->NMemb;i++)
		{
			word ID=BR->Memb[i];
			if(ID!=0xFFFF)
			{
				OneObject* OB = Group[BR->Memb[i]];
				if(OB)
				{
					px[i]=OB->RealX/16;
					py[i]=OB->RealY/16;
				}
			}
		}
		return;
	}
	*/
	int max=0; 
	for(int i=0;i<nw;i++)
	{
		if((*Parts[i])>max)
		{
			max=(*Parts[i]);
		}
	}
	for(int j=0;j<nbl;j++)
	{
		BrigadeUnitPositionLine* LL = BrigadePosition.PositionList[j];
		int nu = LL->Line.GetAmount();
		for(int k=0;k<nu;k++)
		{
			if(LL->Line[k]->State!=0xFF)
			{
				if(LL->Line[k]->Part==NULL||(*(*(LL->Line[k]->Part)))!=max)
				{
					if(LL->Line[k]->State==1)
						UnitsWithoutPos.Add(LL->Line[k]->UnitIndexInBrigade);
					LL->Line[k]->State=3;
				}
			}
		}
	}
	
	bool Changet=false;
	int NIter=0;
	for(byte k=0;k<NBPERSONAL;k++)
	{
		if(Mem[k]!=0xFFFF)
		{
			//if(MF->CheckBar((px[k]>>4)-1,(py[k]>>4)-1,3,3))
			if(MF->CheckPt(px[k]>>4,py[k]>>4))
			{
				UnitsWithoutPos.Add(k);
			}
		}
	}
	int NWP = UnitsWithoutPos.GetAmount();
	while(NWP>NFreePos&&NIter<16)
	{
		Changet=true;
		BrigadePosition.AddFirstLine(MF);
		//if(NWP>NFreePos)
		BrigadePosition.AddLastLine(MF);
		//if(NWP>NFreePos)
		BrigadePosition.AddSidePos(MF);
		int nl=BrigadePosition.PositionList.GetAmount();
		
		BrigadeUnitPositionLine* Pre=NULL;
		for(int j=0;j<nl;j++)
		{
			BrigadeUnitPositionLine* LL = BrigadePosition.PositionList[j];
			int nu = LL->Line.GetAmount();
			for(int k=1;k<(nu-1);k++)
			{
				if(LL->Line[k]->State==1||LL->Line[k]->State==2||(nw==0 && LL->Line[k]->State==3))
				{
					if(LL->Line[k+1]->State==3)
						NFreePos+=BrigadePosition.SetState(j,k+1,3,2);
					if(LL->Line[k-1]->State==3)
						NFreePos+=BrigadePosition.SetState(j,k-1,3,2);
					if(Pre)
					{
						if(Pre->Line.GetAmount()>k&&Pre->Line[k]->State==3)
							NFreePos+=BrigadePosition.SetState(j-1,k,3,2);
					}
				}
			}
			Pre=LL;
		}
		NIter++;
	}
	if(NFreePos==0&&NWP>0){
		for(int i=0;i<BR->NMemb;i++)
		{
			word ID=BR->Memb[i];
			if(ID!=0xFFFF)
			{
				OneObject* OB = Group[BR->Memb[i]];
				if(OB)
				{
					px[i]=OB->RealX/16;
					py[i]=OB->RealY/16;
				}
			}
		}
		return;
	}
	bool find=false;
	if(NWP)
	{
		int nl=BrigadePosition.PositionList.GetAmount();
		int cl=nl/2;
		for(int i=0;i<NWP;i++)
		{
			find=false;
			BrigadeUnitPositionLine* LL = BrigadePosition.PositionList[cl];
			int nu = LL->Line.GetAmount();
			for(int k=0;k<nu&&!find;k++)
			{
				if(LL->Line[k]->State==2)
				{
					px[UnitsWithoutPos[i]]=LL->Line[k]->x;
					py[UnitsWithoutPos[i]]=LL->Line[k]->y;
					LL->Line[k]->State=1;
					find=true;
				}
			}

			for(int j=0;j<cl&&!find;j++)
			{
				if((cl-1-j)>-1)
				{
					BrigadeUnitPositionLine* LL = BrigadePosition.PositionList[cl-1-j];
					int nu = LL->Line.GetAmount();
					for(int k=0;k<nu&&!find;k++)
					{
						if(LL->Line[k]->State==2)
						{
							px[UnitsWithoutPos[i]]=LL->Line[k]->x;
							py[UnitsWithoutPos[i]]=LL->Line[k]->y;
							LL->Line[k]->State=1;
							find=true;
						}
					}
				}
				if((cl+1+j)<nl)
				{
					BrigadeUnitPositionLine* LL = BrigadePosition.PositionList[cl+1+j];
					int nu = LL->Line.GetAmount();
					for(int k=0;k<nu&&!find;k++)
					{
						if(LL->Line[k]->State==2)
						{
							px[UnitsWithoutPos[i]]=LL->Line[k]->x;
							py[UnitsWithoutPos[i]]=LL->Line[k]->y;
							LL->Line[k]->State=1;
							find=true;
						}
					}
				}
			}
		}
	}
	BR->NUnitsNotInPos=NWP;
	
	if(Changet&&find&&NWP>15)
		BR->ResortMembByPos();
	
	
}
//===================================================================================================================//
class B_SmartSend:public BrigadeOrder{
public:
	int x,y,NextX,NextY;
	short Dir;
	word NextTop;
};
bool CheckBDirectWay(int x0,int y0,int x1,int y1,byte LockType=0){
	int bx=x0<<10;
	int by=y0<<10;
	int Lx=x1-x0;
	int Ly=y1-y0;
	int r=Norma(Lx,Ly);
	int N=(r>>6)+1;
	Lx=(Lx<<10)/N;
	Ly=(Ly<<10)/N;
	MotionField* MF=MFIELDS+LockType;
	for(int i=0;i<N;i++){
		bx+=Lx;
		by+=Ly;
		int xx=bx>>14;
		int yy=by>>14;
		if(MF->CheckPt(xx,yy))return false;
		if(MF->CheckPt(xx-8,yy))return false;
		if(MF->CheckPt(xx+8,yy))return false;
		if(MF->CheckPt(xx,yy-8))return false;
		if(MF->CheckPt(xx,yy+8))return false;
	};
	return true;
};
bool CalcCenter(Brigade* BR,int* x,int* y){
	if(BR->NMemb<3)return false;
	int xx=0;
	int yy=0;
	int N=0;
	int M=BR->NMemb;
	for(int i=NBPERSONAL;i<M;i++){
		word MID=BR->Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==BR->MembSN[i]){
				xx+=OB->RealX;
				yy+=OB->RealY;
				N++;
			};
		};
	};
	if(N){
		*x=xx/N;
		*y=yy/N;
		return true; 
	}else return false;
};
float GetNorm(float dx,float dy){
	return sqrt(dx*dx+dy*dy);
};
struct CurveParams{
	float x1,y1,x2,y2,x3,y3;
	float TScale;
};
void GetCurveVector(float& x,float& y,CurveParams& CP,float t){
	x=CP.x1*t+CP.x3*(1.0f-t)+(CP.x2*4.0f-(CP.x1+CP.x3)*2.0f)*t*(1.0f-t);
	y=CP.y1*t+CP.y3*(1.0f-t)+(CP.y2*4.0f-(CP.y1+CP.y3)*2.0f)*t*(1.0f-t);
};
void InitCurveParams(CurveParams& CP){
	CP.TScale=GetNorm(CP.x1-CP.x2,CP.y1-CP.y2)+GetNorm(CP.x3-CP.x2,CP.y3-CP.y2);
};
void Get2DCurveVector(float& x,float& y,CurveParams& CP,float tL,float tO){
	float tc=(tL/CP.TScale)+0.5;
	float cx0,cy0,cxp,cyp,cxn,cyn;
	GetCurveVector(cx0,cy0,CP,tc);
	GetCurveVector(cxp,cyp,CP,tc-0.1);
	GetCurveVector(cxn,cyn,CP,tc+0.1);
	cxn-=cxp;
	cyn-=cyp;
	float N=GetNorm(cxn,cyn);
	cxn/=N;
	cyn/=N;
	x=cx0-tO*cyn;
	y=cy0+tO*cxn;
};
void CreateCurvedPositionForFormation(Brigade* BR,int x0,int y0,int x1,int y1,int x2,int y2,int x3,int y3){
	byte Dir=GetDir(x2-x0,y2-y0);

	int xc=(x0+x2+x1*4)/6;
	int yc=(y0+y2+y1*4)/6;

	x2=(x1+x3+x2*4)/6;
	y2=(y1+y3+y2*4)/6;

	CurveParams CP;
	CP.x1=x0;
	CP.y1=y0;
	CP.x2=xc;
	CP.y2=yc;
	CP.x3=x2;
	CP.y3=y2;
	InitCurveParams(CP);
	addrand(xc);
	addrand(yc);
	//BR->CreateSimpleOrderedPositions(xc<<4,yc<<4,Dir);
	BR->CreateOrderedPositions(xc<<4,yc<<4,Dir);
	float vx=TCos[Dir];
	float vy=TSin[Dir];
	for(int i=NBPERSONAL;i<BR->NMemb;i++){
		float px=float(BR->posX[i])-xc;
		float py=float(BR->posY[i])-yc;
		float tu=-((px*vx)+(py*vy))/256.0;
		float tv=-((px*vy)-(py*vx))/256.0;
		float x,y;
		Get2DCurveVector(x,y,CP,tu,tv);
		BR->posX[i]=int(x);
		BR->posY[i]=int(y);
	};
};
//================================================================================================================//
#define MaxPointsXY 1024
///#define DistInColumn 16
bool CheckIfRoadZone(int Zone);
int GetNextWayPoints(int From, int Dest, int *PointsXYList);
int GetNextWayPoints(int From, int Dest, int *PointsXYList, word &Flag, DWORD Requestor);
int SetPreLock(int From, int Dest);
int SetPreLock(int From, int Dest, DWORD Requestor);

class B_GoOnRoad : public BrigadeOrder
{
public:
	B_GoOnRoad();
	~B_GoOnRoad();
	int DestTopZone;// зона куда идем
	bool Process(Brigade* BR);
	int PointsXY[MaxPointsXY]; // дорожные точки
	byte PointsDir[MaxPointsXY]; // направление в каждой точке
	int ShiftInColumn[16]; //сдвиг в колоне от дорожной точки
	int HeadIndex; // индекс головы колонны в PointsXY
	int HeadTopZoneIndex; // индекс топологической зоны головы колонны 
	int MaxPIndex; // сколько точек получили
	int LastMoveTime; // время последнего шага
	void SetNextPosAndDest(Brigade* BR);// шаг вперед
	int UnitsInLine; // ширена колонны
	void GetUnitCoordInColumn(int PointI,byte ColumnI, int &x, int &y);
	void Init(Brigade *BR);
	int lockType;
	int NLines;
	int HeadOBJIndex; // ведуший юнит
	word SA[256]; // служебный
	static int  compare(const void *arg1, const void *arg2); 
	static int fx, fy;
	static Brigade* fBrig;
	int GetMinDistBrigToPoint(Brigade* BR, int x, int y);
	bool FirstStep; // прямо как в жизни :)
	bool OffPlaseHead; // офицеры в голове колонны (или нет)
	void SetOffPoss(int LineI,Brigade* BR); // задать позиции офицерскому составу
	DWORD Owner; // идентификатор отряда для дороги
	word RoadFlag; // место на дороге (норма, стоп, обочина)
	void LockNextPoint(); // забронировать следующий отрезок дороги
	int LastStepTime;
	int DistInColumn;
	int PassNPoints;
	void SetUnitsSpeed(Brigade* BR);
	void SortBrigUnits(Brigade* BR, int xx, int yy);
	void RangeInLineMemb(Brigade* BR);
};
void B_GoOnRoad::RangeInLineMemb(Brigade* BR)
{
	bool ff=false;
	int lf=-1;
	for(int i=3;i<(BR->NMemb-1);i++)
	{
		if(BR->Memb[i]==0xFFFF)
		{
			if(!ff)
			{
				lf=i;
				ff=true;
			}
		}
		else
		{
			if(ff)
			{
				for(int j=i;j<BR->NMemb;j++)
				{
					BR->Memb[lf+j-i]=BR->Memb[j];
					BR->MembSN[lf+j-i]=BR->MembSN[j];
					OneObject* OB = Group[BR->Memb[lf+j-i]];
					if(OB)
					{
						OB->BrIndex=lf+j-i;
					}
					BR->Memb[j]=0xFFFF;
					BR->MembSN[j]=0xFFFF;
				}
				ff=false;
			}
		}
	}
}


void B_GoOnRoad::SortBrigUnits(Brigade *BR, int xx, int yy)
{
	fBrig=BR;  
	fx=xx;
	fy=yy;
	for(int i=3;i<BR->NMemb;i++)
		SA[i-3]=i;
	qsort((void*)SA,BR->NMemb-3,sizeof(word),B_GoOnRoad::compare);
	int N=BR->NMemb-3;
	for(int i=0;i<N;i++)
	{
		SA[i]=BR->Memb[SA[i]];
	} 
	for(int i=0;i<N;i++)
	{
		BR->Memb[i+3]=SA[i];
		if(BR->Memb[i+3]!=0xFFFF)
		{
			OneObject* OB=Group[BR->Memb[i+3]];
			if(OB)
			{
				OB->BrIndex=i+3;
				BR->MembSN[i+3]=OB->Serial;
			}
		}
	}
}

void B_GoOnRoad::SetUnitsSpeed(Brigade* BR)
{
	if(BR)
	{
		int AvrDst=0;
		int NCalk=0;
		for(int i=0;i<BR->NMemb;i++)
		{
			if(BR->Memb[i]!=0xFFFF)
			{
				OneObject* OB=Group[BR->Memb[i]];
				if(OB)
				{
					int ds=Norma((OB->RealX>>4)-(OB->DestX>>4),(OB->RealY>>4)-(OB->DestY>>4));
					if(ds<200)
					{
						AvrDst+=ds;
						NCalk++;
					}
				}
			}
		}
		if(NCalk)
		{
			AvrDst/=NCalk;
			for(int i=0;i<BR->NMemb;i++)
			{
				if(BR->Memb[i]!=0xFFFF)
				{
					OneObject* OB=Group[BR->Memb[i]];
					if(OB)
					{
						int ds=Norma((OB->RealX>>4)-(OB->DestX>>4),(OB->RealY>>4)-(OB->DestY>>4))+1;
						OB->UnitSpeed=(64*ds)/(AvrDst+1);
						if(OB->UnitSpeed<32)OB->UnitSpeed=32;
						if(OB->UnitSpeed>128)OB->UnitSpeed=128;
					}
				}
			}
		}
	}
}

Brigade* B_GoOnRoad::fBrig=NULL;
int B_GoOnRoad::fx=-1;
int B_GoOnRoad::fy=-1;
void B_GoOnRoad::LockNextPoint()
{
	if(HeadTopZoneIndex!=DestTopZone)
	{
		int NA=GetNAreas(lockType);
		int NTP=GetMotionLinks(DestTopZone+NA*HeadTopZoneIndex,lockType,(Owner>>16)&0xFF);
		if(NTP<0xFFFE&&CheckIfRoadZone(NTP))
		{
			SetPreLock(HeadTopZoneIndex, NTP, Owner);
		}
	}
}
void B_GoOnRoad::SetOffPoss(int LineI,Brigade* BR)
{
	int xx,yy;
	if(BR->Memb[0]!=0xFFFF)
	{
		OneObject* OB=Group[BR->Memb[0]];
		if(OB)
		{
			GetUnitCoordInColumn(LineI,0, xx, yy);
			OB->DestX=xx<<4; 
			OB->DestY=yy<<4;
			addrand(OB->DestX);
			addrand(OB->DestY);
			BR->posX[0]=xx<<4;
			BR->posY[0]=yy<<4; 
			if(OffPlaseHead)
				HeadOBJIndex=BR->Memb[0];
		}
	}
	if(BR->Memb[1]!=0xFFFF)
	{
		OneObject* OB=Group[BR->Memb[1]];
		if(OB)
		{
			GetUnitCoordInColumn(LineI,UnitsInLine/2, xx, yy);
			OB->DestX=xx<<4; 
			OB->DestY=yy<<4;
			addrand(OB->DestX);
			addrand(OB->DestY);
			BR->posX[1]=xx<<4;
			BR->posY[1]=yy<<4; 
			if(OffPlaseHead)
				HeadOBJIndex=BR->Memb[1];
		}
	}
	if(BR->Memb[2]!=0xFFFF)
	{
		OneObject* OB=Group[BR->Memb[2]]; 
		if(OB)
		{
			GetUnitCoordInColumn(LineI,UnitsInLine-1, xx, yy);
			OB->DestX=xx<<4; 
			OB->DestY=yy<<4;
			addrand(OB->DestX);
			addrand(OB->DestY);
			BR->posX[2]=xx<<4;
			BR->posY[2]=yy<<4; 
		}
	}
}
int B_GoOnRoad::GetMinDistBrigToPoint(Brigade* BR, int x, int y)
{
	int rez=-1;
	if(BR)
	{
		for(int i=0;i<BR->NMemb;i++)
		{
			if(BR->Memb[i]!=0xFFFF)
			{
				OneObject* OB = Group[BR->Memb[i]];
				if(OB)
				{
					int ds=Norma((OB->RealX>>4)-x,(OB->RealY>>4)-y);
					if(rez==-1)
						rez=ds;
					else
					if(rez>ds)
						rez=ds;
				}
			}
		}
	}
	return rez;
}
int B_GoOnRoad::compare(const void *a1, const void *a2)
{
	int rez=0;
	word i1=*(word*)a1;
	word i2=*(word*)a2;
	if(fBrig->Memb[i1]==0xFFFF)
		rez--;
	if(fBrig->Memb[i2]==0xFFFF)
		rez++;
	if(fBrig->Memb[i1]!=0xFFFF&&fBrig->Memb[i2]!=0xFFFF)
	{
		OneObject* OB1=Group[fBrig->Memb[i1]];
		OneObject* OB2=Group[fBrig->Memb[i2]];
		if(OB1&&OB2)
		{
			int d1=Norma((OB1->RealX>>4)-fx,(OB1->RealY>>4)-fy);
			int d2=Norma((OB2->RealX>>4)-fx,(OB2->RealY>>4)-fy);
			return d1-d2;
		}
	}
	return rez;
}
bool B_GoOnRoad::Process(Brigade *BR)
{
	//NTP=GetMotionLinks(FinalTop+NA*NTP,lockType);
	//if(NTP<0xFFFE&&NTP!=FinalTop&&CheckIfRoadZone(NTP)){
	if(tmtmt>LastMoveTime)
	{
		if(HeadIndex==-1)
		{
			int xx,yy;
			BR->GetCenter(&xx,&yy);
			HeadTopZoneIndex=GetTopology(xx,yy,lockType);
			if(HeadTopZoneIndex==DestTopZone)
				return false;
		}
		if(HeadTopZoneIndex==DestTopZone&&((MaxPIndex-HeadIndex)<2))
			return false;
		if((MaxPIndex-HeadIndex)<8)
		{
			int NA=GetNAreas(lockType);
			int NTP=GetMotionLinks(DestTopZone+NA*HeadTopZoneIndex,lockType,(Owner>>16)&255);
			if(NTP<0xFFFE&&HeadIndex==-1)
			{
				HeadTopZoneIndex=NTP;
				NTP=GetMotionLinks(DestTopZone+NA*HeadTopZoneIndex,lockType,(Owner>>16)&255);
			}
			if(NTP<0xFFFE&&CheckIfRoadZone(NTP))
			{
				int addM=GetNextWayPoints(HeadTopZoneIndex,NTP,PointsXY+MaxPIndex*2,RoadFlag,Owner);
				if(addM)
				{
					MaxPIndex+=addM;
					if(HeadIndex==-1)
					{
						/*
						bool flag=true;
						for(;flag;)
						{
							//if(GetMinDistBrigToPoint(BR,PointsXY[(MaxPIndex-1)*2],PointsXY[(MaxPIndex-1)*2+1])>300)
							if(GetMinDistBrigToPoint(BR,PointsXY[(HeadIndex+1)*2],PointsXY[(HeadIndex+1)*2+1])>100) 
								flag=false;
							else
							{
								HeadIndex=MaxPIndex-1;
								HeadTopZoneIndex=NTP;
								NTP=GetMotionLinks(DestTopZone+NA*HeadTopZoneIndex,lockType);
								if(NTP<0xFFFE&&CheckIfRoadZone(NTP))
								{
									int addMn=GetNextWayPoints(HeadTopZoneIndex,NTP,PointsXY+MaxPIndex*2,RoadFlag,Owner);
									MaxPIndex+=addMn;
									addM+=addMn;
									HeadTopZoneIndex=NTP;
									LockNextPoint();
								}
								else
									flag=false;
							}
						}
						*/
						if(NLines>MaxPIndex)
						{
							int ax,ay;
							byte aang;
							BR->GetCenter(&ax,&ay,&aang);
							byte apd=GetDir(ax-PointsXY[(MaxPIndex-1)*2],ay-PointsXY[(MaxPIndex-1)*2+1]);
							int np=NLines-MaxPIndex;
							for(int j=MaxPIndex-1;j>-1;j--)
							{
								PointsXY[(j+np)*2]=PointsXY[j*2];
								PointsXY[(j+np)*2+1]=PointsXY[j*2+1];
							}
							//memmove(PointsXY+np*2,PointsXY,4*2*np);
							int pdd=25;
							for(int i=0;i<np;i++)
							{
								int nx=PointsXY[np*2]+((pdd*(i+1)*TCos[apd])>>8);
								int ny=PointsXY[np*2+1]+((pdd*(i+1)*TSin[apd])>>8);
								PointsXY[(np-1-i)*2]=nx;
								PointsXY[(np-1-i)*2+1]=ny;
							}
							MaxPIndex+=np;
							addM=MaxPIndex;
						}
						HeadIndex=MaxPIndex-1;
					}
 					//setDir
					for(int i=(MaxPIndex-addM);i<(MaxPIndex-1);i++)
					{
						PointsDir[i]=GetDir(PointsXY[(i+1)*2]-PointsXY[i*2],PointsXY[(i+1)*2+1]-PointsXY[i*2+1]);
					}
					PointsDir[MaxPIndex-1]=PointsDir[MaxPIndex-2];

					HeadTopZoneIndex=NTP;
					LockNextPoint();
					int const MemShift=80;
					if(HeadIndex>(NLines*PassNPoints+MemShift)&&HeadIndex>MemShift)
					{
						memmove(PointsXY,PointsXY+MemShift*2,4*2*(MaxPIndex-MemShift));
						memmove(PointsDir,PointsDir+MemShift,MaxPIndex-MemShift);
						MaxPIndex-=MemShift;
						HeadIndex-=MemShift;
					}
				}
				//else
					//return false;
			}
			else
				return false;
		}
		
		OneObject* OB=NULL;
		if(HeadOBJIndex!=0xFFFF)
		{
			OB=Group[HeadOBJIndex];
			if(OB&&OB->Sdoxlo)
			{
				OB=NULL;
				HeadOBJIndex=0xFFFF;
			}
		}
		int dds=0;
		if(OB)dds=Norma(PointsXY[HeadIndex*2]-(OB->RealX>>4),PointsXY[HeadIndex*2+1]-(OB->RealY>>4));
		if(dds<100||(HeadIndex<30&&dds<150)) 
		{
			if(HeadIndex<(MaxPIndex-1)&&(RoadFlag!=1))
			{
				SetNextPosAndDest(BR);
				LastMoveTime=tmtmt;
			}
		}
		else
		{
			if(HeadOBJIndex!=0xFFFF)
			{
				OB=Group[HeadOBJIndex];
				if(OB&&(OB->StandTime>40||(tmtmt-LastStepTime)>40))
				{
					HeadOBJIndex=0xFFFF;
					if(BR->NMemb>20&&(BR->Memb[3]==0xFFFF||BR->Memb[4]==0xFFFF||BR->Memb[5]==0xFFFF))
					{
						RangeInLineMemb(BR);
						HeadOBJIndex=0xFFFF;
					}
					SetNextPosAndDest(BR);
					LastMoveTime=tmtmt;
				}
			}
			else
			{
				SetUnitsSpeed(BR);
			}
		}
	}
	return true;
}
void B_GoOnRoad::SetNextPosAndDest(Brigade *BR)
{
	if(BR&&BR->WarType&&(HeadIndex<(MaxPIndex-1)))
	{
		HeadIndex++;
		LastStepTime=tmtmt;
		if(FirstStep)
		{
			// определяем место офицерского состава (спереди-сзади:))
			// и порядок юнитов в колонне
			OffPlaseHead=true;
			if(BR->Memb[0]!=0xFFFF)
			{
				OneObject* OB=Group[BR->Memb[0]];
				if(OB)
				{
					if(Norma((OB->RealX>>4)-PointsXY[(HeadIndex)*2],(OB->RealY>>4)-PointsXY[(HeadIndex)*2+1])>300)
						OffPlaseHead=false;
				}
			}
			bool inHead=false;
			bool inBack=false;
			for(int i=3;i<(UnitsInLine+3);i++)
				if(BR->Memb[i]!=0xFFFF)
				{
					OneObject* OB=Group[BR->Memb[i]];
					if(OB)
					{
						if(Norma((OB->RealX>>4)-PointsXY[(HeadIndex)*2],(OB->RealY>>4)-PointsXY[(HeadIndex)*2+1])<300)
							inHead=true;
					}
				}
			for(int i=(BR->NMemb-UnitsInLine);i<BR->NMemb;i++)
				if(BR->Memb[i]!=0xFFFF)
				{
					OneObject* OB=Group[BR->Memb[i]];
					if(OB)
					{
						if(Norma((OB->RealX>>4)-PointsXY[(HeadIndex)*2],(OB->RealY>>4)-PointsXY[(HeadIndex)*2+1])<300)
							inBack=true;
					}
				}

			if(inHead)
			{
				//идем в томже порядке
				//RangeInLineMemb(BR);
			}
			else
			if(inBack)
			{
				//реверс
				int N=BR->NMemb-3;
				for(int j=0;j<N;j++)
				{
					SA[j]=BR->Memb[BR->NMemb-j-1];
				}
				for(int i=0;i<N;i++)
				{
					BR->Memb[i+3]=SA[i];
					if(BR->Memb[i+3]!=0xFFFF)
					{
						OneObject* OB=Group[BR->Memb[i+3]];
						if(OB)
						{
							OB->BrIndex=i+3;
							BR->MembSN[i+3]=OB->Serial;
						}
					}
				}
				//RangeInLineMemb(BR);
			}
			else
			{
				// не то и не се - сортируем по новому 
				SortBrigUnits(BR,PointsXY[(HeadIndex)*2],PointsXY[(HeadIndex)*2+1]);
				//RangeInLineMemb(BR);
			}
		
			FirstStep=false;
		}
		HeadOBJIndex=0xFFFF;
		if(OffPlaseHead)
		{
			SetOffPoss(HeadIndex,BR);
		}
		else
		{
			if((HeadIndex-NLines)>=0)
				SetOffPoss(HeadIndex-NLines,BR);
		}
		int AvrDist=0;
		int NMemb=0;
		for(int i=3;i<BR->NMemb;i++)
		{
			if(BR->Memb[i]!=0xFFFF)
			{
				OneObject* OB=Group[BR->Memb[i]];
				if(OB)
				{
					int ln=(i-3)/UnitsInLine;
					int pi=HeadIndex-ln*PassNPoints;
					if(OffPlaseHead)
						pi--;
					if(pi>-1)
					{
						int pos=(i-3)%UnitsInLine; 
						int xx=0,yy=0;
						GetUnitCoordInColumn(pi,pos, xx, yy);
						int AD=Norma((OB->RealX>>4)-xx,(OB->RealY>>4)-yy);
						if(AD<300)
						{
							AvrDist+=AD;
							NMemb++;
						}
						if(OB->NewState!=0){
							OB->NewState=0;
							TryToStand(OB,0);
						}
						OB->DestX=xx<<4; 
						OB->DestY=yy<<4;
						addrand(OB->DestX);
						addrand(OB->DestY);
						BR->posX[i]=xx<<4;
						BR->posY[i]=yy<<4; 
						if(HeadOBJIndex==0xFFFF&&pos==(UnitsInLine/2)&&AD<400)
						{
							OneObject* OO=Group[BR->Memb[i]];
							if(OO&&!OO->Sdoxlo)
							{
								HeadOBJIndex=BR->Memb[i];
							}
						}
					}
				}
			}
		}
		
		if(NMemb)
		{
			AvrDist/=NMemb;
			int LastSpeed=0;
			if(BR->Memb[0]!=0xFFFF)
			{
				OneObject* OB=Group[BR->Memb[0]];
				if(OB)
				{
					int ds=Norma((OB->RealX>>4)-(OB->DestX>>4),(OB->RealY>>4)-(OB->DestY>>4));
					OB->UnitSpeed=(64*ds)/(AvrDist+1);
					addrand(OB->UnitSpeed);
					if(OB->UnitSpeed<32)OB->UnitSpeed=32;
					if(OB->UnitSpeed>128)OB->UnitSpeed=128;
					addrand(OB->UnitSpeed);
					if(!OffPlaseHead&&(HeadIndex-NLines*PassNPoints)<0)
					{
						int xx=0,yy=0;
						GetUnitCoordInColumn(0,0, xx, yy);
						OB->DestX=xx<<4;
						OB->DestY=yy<<4;
						addrand(OB->DestX);
						addrand(OB->DestY);
						BR->posX[0]=xx<<4;
						BR->posY[0]=yy<<4;
						//OB->UnitSpeed=64; 
						addrand(OB->UnitSpeed);
					}
					//if(HeadIndex<30){
						//OB->UnitSpeed=100; 
						addrand(OB->UnitSpeed);
				}
			}
			if(BR->Memb[1]!=0xFFFF)
			{
				OneObject* OB=Group[BR->Memb[1]];
				if(OB)
				{
					int ds=Norma((OB->RealX>>4)-(OB->DestX>>4),(OB->RealY>>4)-(OB->DestY>>4));
					OB->UnitSpeed=(64*ds)/(AvrDist+1);
					addrand(OB->UnitSpeed);
					if(OB->UnitSpeed<32)OB->UnitSpeed=32;
					if(OB->UnitSpeed>128)OB->UnitSpeed=128;
					addrand(OB->UnitSpeed);
					if(!OffPlaseHead&&(HeadIndex-NLines*PassNPoints)<0)
					{
						int xx=0,yy=0;
						GetUnitCoordInColumn(0,UnitsInLine/2, xx, yy);
						OB->DestX=xx<<4;
						OB->DestY=yy<<4;
						addrand(OB->DestX);
						addrand(OB->DestY);
						BR->posX[1]=xx<<4;
						BR->posY[1]=yy<<4;
						//OB->UnitSpeed=64; 
					}
					//if(HeadIndex<30)
						//OB->UnitSpeed=100;  
				}
			}
			if(BR->Memb[2]!=0xFFFF)
			{
				OneObject* OB=Group[BR->Memb[2]];
				if(OB)
				{
					int ds=Norma((OB->RealX>>4)-(OB->DestX>>4),(OB->RealY>>4)-(OB->DestY>>4));
					OB->UnitSpeed=(64*ds)/(AvrDist+1);
					if(OB->UnitSpeed<32)OB->UnitSpeed=32;
					if(OB->UnitSpeed>128)OB->UnitSpeed=128;
					addrand(OB->UnitSpeed);
					if(!OffPlaseHead&&(HeadIndex-NLines*PassNPoints)<0)
					{
						int xx=0,yy=0;
						GetUnitCoordInColumn(0,UnitsInLine*PassNPoints-1, xx, yy);
						OB->DestX=xx<<4;
						OB->DestY=yy<<4;
						addrand(OB->DestX);
						addrand(OB->DestY);
						BR->posX[2]=xx<<4;
						BR->posY[2]=yy<<4;
						//OB->UnitSpeed=64; 
					}
					//if(HeadIndex<30)
						//OB->UnitSpeed=100; 
				}
			}
			for(int k=0;k<3;k++)
			{
				OneObject* OB=Group[BR->Memb[k]];
				if(OB)
				{
					int ds=Norma((OB->RealX>>4)-(OB->DestX>>4),(OB->RealY>>4)-(OB->DestY>>4));
					OB->UnitSpeed=(64*ds)/(AvrDist+1);
 					if(HeadIndex<15&&OB->UnitSpeed<72)
					{
						OB->UnitSpeed=100;
					}
				}
			}
			for(int i=3;i<BR->NMemb;i++)
			{
				if(BR->Memb[i]!=0xFFFF)
				{
					OneObject* OB=Group[BR->Memb[i]];
					if(OB)
					{
						int ln=(i-3)/UnitsInLine;
						int pi=HeadIndex-ln*PassNPoints;
						if(OffPlaseHead)
							pi--;
						if(pi>-1)
						{
							int ds=Norma((OB->RealX>>4)-(OB->DestX>>4),(OB->RealY>>4)-(OB->DestY>>4));
							OB->UnitSpeed=(64*ds)/(AvrDist+1);
							if(OB->UnitSpeed<32)OB->UnitSpeed=32;
							if(OB->UnitSpeed>128)OB->UnitSpeed=128; 
							if(HeadIndex<30&&pi<6&&OB->UnitSpeed<72)
								OB->UnitSpeed=100;
							//LastSpeed=OB->UnitSpeed;
							addrand(OB->UnitSpeed);
						}
						else
						{
							if(pi>-4)
							{
								int pos=(i-3)%UnitsInLine; 
								int xx=0,yy=0;
								GetUnitCoordInColumn(0,pos, xx, yy);
								OB->DestX=xx<<4;
								OB->DestY=yy<<4;
								addrand(OB->DestX);
								addrand(OB->DestY);
								BR->posX[i]=xx<<4;
								BR->posY[i]=yy<<4;
								//int ds=Norma((OB->RealX>>4)-(OB->DestX>>4),(OB->RealY>>4)-(OB->DestY>>4));
								//OB->UnitSpeed=(32*ds)/AvrDist+pi*5;  
								//if(OB->UnitSpeed<0)
									//OB->UnitSpeed=0;
								OB->UnitSpeed=32;							
							}
							else
							{
								int pos=(i-3)%UnitsInLine; 
								int xx=0,yy=0;
								GetUnitCoordInColumn(0,pos, xx, yy);
								OB->DestX=xx<<4;
								OB->DestY=yy<<4;
								addrand(OB->DestX);
								addrand(OB->DestY);
								BR->posX[i]=xx<<4;
								BR->posY[i]=yy<<4;
								OB->UnitSpeed=0;
								/*
								byte ddr=GetDir((OB->RealX>>4)-xx,(OB->RealY>>4)-yy);
								if(OB->RealDir!=ddr)
								{
									OB->DeleteLastOrder();
									OB->RealDir=ddr;
									OB->GraphDir=OB->RealDir;
								}
								*/
							}
						}
					}
				}
			}
		}
	}
}
void B_GoOnRoad::GetUnitCoordInColumn(int PointI,byte ColumnI, int &xxx, int &yyy)
{
	if(PointI<0)PointI=0;
	if(PointI>=MaxPIndex)PointI=MaxPIndex-1;

	if(PointI>-1&&PointI<MaxPIndex)
	{
		int bx=PointsXY[PointI*2];
		int by=PointsXY[PointI*2+1];
		xxx=bx-((ShiftInColumn[ColumnI]*TSin[PointsDir[PointI]])>>8);
		yyy=by+((ShiftInColumn[ColumnI]*TCos[PointsDir[PointI]])>>8);
	}else{
		xxx=0;
		yyy=0;
	}
}
void B_GoOnRoad::Init(Brigade *BR)
{
	LastMoveTime=0;
	MaxPIndex=0;
	HeadIndex=-1;
	HeadOBJIndex=0xFFFF;
	lockType=0;//GetBrigLockType(BR);
	int H=100;
	DistInColumn=16;
	PassNPoints=1;
	for(int i=3;i<BR->NMemb;i++)
	{
		if(BR->Memb[i]!=0xFFFF)
		{
			OneObject* OB = Group[BR->Memb[i]];
			if(OB)
			{
				DistInColumn=OB->newMons->Radius2/10+2;
				PassNPoints+=(DistInColumn/19)*3;
				break;
			}
		}
	}
	UnitsInLine=H/DistInColumn;
	int Hig=(DistInColumn*(UnitsInLine-1))/2;
	for(int i=0;i<UnitsInLine;i++)
		ShiftInColumn[i]=i*DistInColumn-Hig;
	NLines=BR->NMemb/UnitsInLine+1;
	if(BR->NMemb%UnitsInLine)NLines++;
	for(int i=3;i<BR->NMemb;i++)
		SA[i-3]=i;
	FirstStep=true;
	OffPlaseHead=true;
	Owner=BR->CT->NI<<16;
	Owner+=BR->ID;
	RoadFlag=0;
	LastStepTime=0;
}
//
void B_GoOnRoadLink(Brigade* BR)
{
	B_GoOnRoad* ONR=(B_GoOnRoad*)BR->BOrder;
	addrand(ONR->Prio);
	if(!ONR->Process(BR))
	{
		int Prio=ONR->Prio;
		int x,y;
		byte ddr;
		BR->GetCenter(&x,&y,&ddr);
		addrand(x);
		addrand(y);
		//BR->CreateSimpleOrderedPositions(ONR->PointsXY[ONR->HeadIndex*2]<<4,ONR->PointsXY[ONR->HeadIndex*2+1]<<4,ddr);
		BR->CreateOrderedPositions(ONR->PointsXY[ONR->HeadIndex*2]<<4,ONR->PointsXY[ONR->HeadIndex*2+1]<<4,ddr);
		BR->Direction=ddr;
		BR->ResortMembByPos();
		BR->DeleteBOrder();
		BR->KeepPositions(1,Prio);
	}
}
//================================================================================================================//
void B_HumanGlobalSendToLink(Brigade* BR);
bool GetBrigDest(CBrigadeOrder* BO,int &x, int &y){
	//if(BO->BLink==B_HumanGlobalSendToLink)
	bool done=false;
	while(BO){
		if(BO->GetBrigadeOrderID()==BRIGADEORDER_HUMANGLOBALSENDTO)
		{
			BrigadeOrder_HumanGlobalSendTo* BS=(BrigadeOrder_HumanGlobalSendTo*)BO;
			x=BS->xx;
			y=BS->yy;	
			done=true;
		}
		BO=BO->Next.Get();
	}
	return done;
}
void B_HumanGlobalSendToLink(Brigade* BR){
	B_SmartSend* BS=(B_SmartSend*)BR->BOrder;
	int x=BS->x;
	int y=BS->y;
	int NextX=BS->NextX;
	int NextY=BS->NextY;
	int NextTop=BS->NextTop;
	byte prior=BS->Prio;
	short dir=BS->Dir;
	int xc,yc;
	if(!CalcCenter(BR,&xc,&yc)){
		BR->DeleteBOrder();
		return;
	};
	byte lockType=GetBrigLockType(BR);
	int CTOP=GetTopology(xc>>4,yc>>4,lockType);
	int FinalTop=GetTopology(&x,&y,lockType);
	int bcx=xc;
	int bcy=yc;
#ifdef COSSACKS2
	//checking for roads
	if(BR->WarType){
		OrderDescription* ODS=ElementaryOrders+BR->WarType-1;
		if(ODS->OrdUsage==1||ODS->OrdUsage==3){//square or move
			int NA=GetNAreas(lockType);
			//checking if there is road on the way to final point
			int NTP=CTOP;
			bool CheckIfRoadZone(int Zone);
			if(!CheckIfRoadZone(CTOP)){
				if(ODS->OrdUsage==3&&ODS->GroupID!=0xFF){
					SingleGroup* FGD=FormGrp.Grp+ODS->GroupID;
					BrigadeOrder* BO=BR->BOrder;
					BR->BOrder=NULL;
					MakeReformation(BR->CT->NI,BR->ID,FGD->IDX[1]);
					BR->DontWait=0;
					BR->BOrder=BO;
					addrand(BS->Prio|128);
					BR->KeepPositions(1,BS->Prio|128);
					return;
				};
				int NTP0;
				int NATT=0;
				do{
					NTP0=NTP;
					NTP=GetMotionLinks(FinalTop+NA*NTP,lockType,BR->CT->NI);
					if(NTP<0xFFFE&&NTP!=FinalTop&&CheckIfRoadZone(NTP)){
						Area* AR=GetTopMap(NTP,lockType);
						int xc,yc;
						void GetPreciseTopCenter(int Zone,int LockType,int& x,int& y);
						GetPreciseTopCenter(NTP,lockType,xc,yc);
						BR->HumanGlobalSendTo(xc,yc,512,BS->Prio|128,1);
						if(BS->Prio==128)BR->AttEnm=1;
						return;
					};
					NATT++;
				}while(NTP<0xFFFE&&NTP!=FinalTop&&NTP!=NTP0&&NATT<10);
			}else{				
NTOP:;
				int RoadsLockType=lockType;
				int CTOPr=GetTopology(xc>>4,yc>>4,RoadsLockType);
				int FinalTopR=GetTopology(&x,&y,RoadsLockType);
				int RoadsNA=GetNAreas(RoadsLockType);
				int NTOP=GetMotionLinks(FinalTopR+RoadsNA*CTOPr,RoadsLockType,BR->CT->NI);
				if(NTOP<0xFFFE){
					bool b1=false;
					int NTOP1=GetMotionLinks(FinalTopR+RoadsNA*NTOP,RoadsLockType,BR->CT->NI);
					if(NTOP1<0xFFFE&&NTOP1!=FinalTopR&&BR->AttEnm==0)
					{
						int NTOP2=GetMotionLinks(FinalTopR+RoadsNA*NTOP1,RoadsLockType,BR->CT->NI);
						if(NTOP2<0xFFFE&&NTOP2!=FinalTopR)
							if(CheckIfRoadZone(NTOP1)&&CheckIfRoadZone(NTOP2))
								b1=true;
					}					
					//if(CheckIfRoadZone(NTOP)&&b1&&CheckIfRoadZone(CTOP))
					
					int CTOP3=GetTopology(xc>>4,yc>>4,3);
					int FinalTop3=GetTopology(BS->x,BS->y,3);
					int TopDst=GetLinksDist(FinalTop3+GetNAreas(3)*CTOP3,3,BR->CT->NI)<<6;
					int MTDST=EngSettings.MinTopDistanceToEnterRoad;
					OneObject* ONE=NULL;

					itr_GetNotCommandUnitsOfBrigade.Create(BR);
					ONE=itr_GetNotCommandUnitsOfBrigade.Next();		
					if(ONE && ONE->newMons->MinTopDistanceToEnterRoad)MTDST=ONE->newMons->MinTopDistanceToEnterRoad;

 					if(b1&&CheckIfRoadZone(CTOPr)&&TopDst>MTDST)
					{
						B_GoOnRoad* OR1=(B_GoOnRoad*)BR->CreateBOrder(1,sizeof(B_GoOnRoad));
						
						DWORD BrigadeID=BR->CT->NI<<16;
						BrigadeID+=BR->ID;
						B_GoOnRoad* OldOr=(B_GoOnRoad*)MusorStorage.GetLastBrigadeLinkData(BrigadeID);
						bool useold=false;
						if(OldOr)
						{
							word ST11=GetMotionLinks(FinalTopR+RoadsNA*CTOPr,RoadsLockType,BR->CT->NI);
							word ST12=GetMotionLinks(OldOr->DestTopZone+RoadsNA*CTOPr,RoadsLockType,BR->CT->NI);
							if(ST12==ST11&&ST11!=0xFFFF)
							{
								word ST21=GetMotionLinks(FinalTopR+RoadsNA*ST11,RoadsLockType,BR->CT->NI);
								word ST22=GetMotionLinks(OldOr->DestTopZone+RoadsNA*ST12,RoadsLockType,BR->CT->NI);
								if(ST22==ST21&&ST21!=0xFFFF)
								{
									useold=true;
									
									OldOr->OrdType=OR1->OrdType;
									OldOr->Message=OR1->Message;
									OldOr->Next=OR1->Next;
									memcpy(OR1,OldOr,sizeof(B_GoOnRoad));
								}
							}
						}
						if(!useold)
						{
							OR1->Init(BR);
						}
						OR1->Fast=1;
						OR1->BLink=&B_GoOnRoadLink;
						BR->AttEnm=0;
						OR1->Prio=BS->Prio;
						addrand(BS->Prio);
						OR1->DestTopZone=FinalTopR;
						OR1->Size=sizeof(B_GoOnRoad);
						return;
					
						/*
						if(ODS->OrdUsage==1){
							SingleGroup* FGD=FormGrp.Grp+ODS->GroupID;
							if(FGD->NCommon>3){
								BrigadeOrder* BO=BR->BOrder;
								BR->BOrder=NULL;
								MakeReformation(BR->CT->NI,BR->ID,FGD->IDX[3]);
								BR->DontWait=0;
								BR->BOrder=BO;
								//BR->KeepPositions(1,BS->Prio|128);
								return;
							};
						};
						int xc,yc;
						int xp,yp,xn,yn,xnn,ynn;
						void GetPreciseTopCenter(int Zone,int LockType,int& x,int& y);
						GetPreciseTopCenter(NTOP,lockType,xc,yc);
						GetPreciseTopCenter(CTOP,lockType,xp,yp);
						int NNTOP=GetMotionLinks(FinalTop+NA*NTOP,lockType);
						if(NNTOP!=0xFFFF){
							GetPreciseTopCenter(NNTOP,lockType,xn,yn);
						}else{
							xn=xc*2-xp;
							yn=yc*2-yp;
						};
						int NNNTOP=GetMotionLinks(FinalTop+NA*NNTOP,lockType);
						if(NNTOP!=0xFFFF){
							GetPreciseTopCenter(NNTOP,lockType,xnn,ynn);
						}else{
							xnn=xn*2-xc;
							ynn=yn*2-yc;
						};
						CreateCurvedPositionForFormation(BR,((bcx>>4)+xp+xp)/3,((bcy>>4)+yp+yp)/3,xc,yc,xn,yn,xnn,ynn);
						int MAXR=0;
						for(int i=NBPERSONAL;i<BR->NMemb;i++){
							word MID=BR->Memb[i];
							if(MID!=0xFFFF){
								OneObject* OB=Group[MID];
								if(OB&&OB->Serial==BR->MembSN[i]){
									int R=Norma((OB->RealX>>4)-BR->posX[i],(OB->RealY>>4)-BR->posY[i]);
									if(R>MAXR)MAXR=R;
								};
							};
						};
						if(MAXR<100){
							CTOP=NTOP;
							goto NTOP;
						};
						//BR->HumanLocalSendTo(xc<<4,yc<<4,512,BS->Prio|128,1);
						BR->KeepPositions(1,BS->Prio|128);
						BR->DontWait=1;
						if(BS->Prio==128)BR->AttEnm=1;
						return;
						*/
					}else{
						if(ODS->OrdUsage==3&&ODS->GroupID!=0xFF){
							SingleGroup* FGD=FormGrp.Grp+ODS->GroupID;
							BrigadeOrder* BO=BR->BOrder;
							BR->BOrder=NULL;
							MakeReformation(BR->CT->NI,BR->ID,FGD->IDX[1]);
							BR->DontWait=0;
							BR->BOrder=BO;
							addrand(BS->Prio|128);
							BR->KeepPositions(1,BS->Prio|128);
							return;
						};
					};
					
				};
			};
		};
		if(ODS->OrdUsage==3&&ODS->GroupID!=0xFF){
			SingleGroup* FGD=FormGrp.Grp+ODS->GroupID;
			BrigadeOrder* BO=BR->BOrder;
			BR->BOrder=NULL;
			MakeReformation(BR->CT->NI,BR->ID,FGD->IDX[1]);
			BR->DontWait=0;
			BR->BOrder=BO;
			addrand(BS->Prio|128);
			BR->KeepPositions(1,BS->Prio|128);
			return;
		};
		BR->DontWait=0;
	};	
#endif
	if(CheckBDirectWay(xc>>4,yc>>4,x,y,lockType)){
		//bool au=EngSettings.AutoChangeFormationType;
		//EngSettings.AutoChangeFormationType=0;
		BR->DeleteBOrder();
		//EngSettings.AutoChangeFormationType=au;
		//addrand(33);
		BR->HumanLocalSendTo(x<<4,y<<4,dir,prior,1);
		return;
	};
	if(NextTop==0xFFFF){
		NextTop=CTOP;//GetTopology(xc>>4,yc>>4,lockType);
		if(NextTop==0xFFFF){
			BR->DeleteBOrder();
			return;
		};
	};
	
	if(FinalTop==0xFFFF){
		BR->DeleteBOrder();
		return;
	};
	BS->x=x;
	BS->y=y;
	int NA=GetNAreas(lockType);
	word NextNextTop=GetMotionLinks(FinalTop+NA*NextTop,lockType,BR->CT->NI);
	if(NextNextTop==FinalTop||FinalTop==NextTop){
		BR->DeleteBOrder();
		addrand(34);
		BR->HumanLocalSendTo(x<<4,y<<4,dir,prior,1);
		return;
	};
	if(NextNextTop!=0xFFFF){
		//atttempt to optomise way
		int MaxPre=5;
		int cox=xc>>4;
		int coy=yc>>4;
		bool NSteps=0;		
		do{
			int Next2=GetMotionLinks(FinalTop+NA*NextNextTop,lockType,BR->CT->NI);
			if(Next2!=0xFFFF){
				Area* AR2=GetTopMap(Next2,lockType);
				int nx=(int(AR2->x)<<6)+32;
				int ny=(int(AR2->y)<<6)+32;
				int rx=nx;
				int ry=ny;
				if(CheckBDirectWay(cox,coy,rx,ry,lockType)||NSteps<1){
					NextNextTop=Next2;
					MaxPre--;
				}else MaxPre=0;
			}else MaxPre=0;
			NSteps++;
		}while(MaxPre);
		Area* NextAr=GetTopMap(NextNextTop,lockType);
		//------------------------
		int nx=(int(NextAr->x)<<6)+32;
		int ny=(int(NextAr->y)<<6)+32;
		BS->NextX=nx;
		BS->NextY=ny;
		BS->NextTop=NextNextTop;
		addrand(35);
		BR->HumanLocalSendTo(nx<<4,ny<<4,512,prior,1);
		return;
	}else{
		BR->DeleteBOrder();
		return;
	};
};
char* HGST_Message="[HumanGlobalSendTo]";
void Brigade::HumanGlobalSendTo(int x,int y,short Dir,byte Prio,byte OrdType){

	if(WarType){
		OrderDescription* ODS=ElementaryOrders+WarType-1;
		if(ODS->OrdUsage==2)return;
	}

	addrand(Prio);
	if(LastOrderTime==REALTIME/*tmtmt*/)return;
	//checking if there is really need global sending
	int xc,yc;
	GetCenter(&xc,&yc);	
	if(WarType){
		OrderDescription* ODS=ElementaryOrders+WarType-1;
		byte Use=ODS->OrdUsage;
		int R0=EngSettings.MinDistanceToEnterRoad;
		OneObject* ONE=NULL;

		itr_GetNotCommandUnitsOfBrigade.Create(this);
		ONE=itr_GetNotCommandUnitsOfBrigade.Next();		
		if(ONE && ONE->newMons->MinDistanceToEnterRoad)R0=ONE->newMons->MinDistanceToEnterRoad;

		if(Use==0){
			R0=EngSettings.MinDistForLineFormations;
			if(ONE && ONE->newMons->MinDistForLineFormations)R0=ONE->newMons->MinDistForLineFormations;
		}
		//if(Use==0)R0=120000;
		int RR=Norma(xc-x,yc-y);
		if(RR<R0){
			if(Use==3&&ODS->GroupID!=0xFF){
				SingleGroup* FGD=FormGrp.Grp+ODS->GroupID;
				//BrigadeOrder* BO=BOrder;
				CBrigadeOrder *NBO=NewBOrder;
				BOrder=NULL;
				addrand(443);
				MakeReformation(CT->NI,ID,FGD->IDX[1]);
				DontWait=0;
				//BOrder=BO;
				NewBOrder=NBO;
			}
			addrand(444);
			addrand(x);
			addrand(y);
			addrand(36);
			if(NewBOrder && NewBOrder->GetBrigadeOrderID()==BRIGADEORDER_GOONROAD){
				//KeepPositions(OrdType,Prio);
				CreateOrderedPositions(x,y,Dir);
				ResortMembByPos();
				//KeepPositions(OrdType,Prio);
			}
			HumanLocalSendTo(x<<4,y<<4,Dir,Prio,OrdType);
			/*
			//-------  -TEST-  ----------
			char dl = (char)Dir - (char)Direction;
			DontWait=true;
			HumanLocalSendTo(x<<4,y<<4,Dir,Prio,0);
			
			int Wth = ElementaryOrders[WarType-1].Width/2;
			
			int Xr=xc-((TSin[Direction]*Wth)>>8);
			int Yr=yc+((TCos[Direction]*Wth)>>8);
			int Xl=xc+((TSin[Direction]*Wth)>>8);
			int Yl=yc-((TCos[Direction]*Wth)>>8);
			
			int dXr=x-((TSin[Dir]*Wth)>>8);
			int dYr=y+((TCos[Dir]*Wth)>>8);
			int dXl=x+((TSin[Dir]*Wth)>>8);
			int dYl=y-((TCos[Dir]*Wth)>>8);

			int Drr=Norma(Xr-dXr,Yr-dYr);
			int Dll=Norma(Xl-dXl,Yl-dYl);

			int Drl=Norma(Xr-dXl,Yr-dYl);
			int Dlr=Norma(Xl-dXr,Yl-dYr);

			int x1=(x+xc)/2;
			int y1=(y+yc)/2;

			int cXr=(Xr+dXr)/2;
			int cYr=(Yr+dYr)/2;
			int cXl=(Xl+dXl)/2;
			int cYl=(Yl+dYl)/2;
			if((Drr+Dll)>(Drl+Dlr))
			{
				cXr=(Xr+dXl)/2;
				cYr=(Yr+dYl)/2;
				cXl=(Xl+dXr)/2;
				cYl=(Yl+dYr)/2;
			}
			x1=(cXr+cXl)/2;
			y1=(cYr+cYl)/2;
			byte nd=GetDir(cXl-cXr,cYl-cYr)+64;//GetDir(x-x1,y-y1)
			HumanLocalSendTo(x1<<4,y1<<4,nd,Prio,1);
			//---------------------------
			*/
			return;
		}
	}
#ifdef COSSACKS2
	if(WarType){
		OrderDescription* ODE=ElementaryOrders+WarType-1;
		if(ODE->OrdUsage==2)return;
	};
#endif //COSSACKS2
	if((Prio&127)>1)AttEnm=false;
	LastOrderTime=REALTIME;//tmtmt;

	// TOP_HASH
	byte LockType=0xFF;
	for(int i=0;i<NMemb;i++){
		if(Memb[i]!=0xFFFF){
			OneObject* OB=Group[Memb[i]];
			if(OB&&!OB->Sdoxlo){
				LockType=OB->LockType;
				void NewAttackPointLink(OneObject* OBJ);
				if(CheckIfPossibleToBreakOrder(OB))OB->ClearOrders();
			}
		};
	};
	if(LockType==0xFF) LockType=0;
	// TOP_HASH

	word Top=GetTopology(&x,&y,LockType);
	if(Top==0xFFFF)return;
	/* //
	B_SmartSend* OR1=(B_SmartSend*)CreateBOrder(OrdType,sizeof B_SmartSend);
	OR1->BLink=&B_HumanGlobalSendToLink;
	OR1->Prio=Prio;
	addrand(Prio);
	OR1->x=x;
	OR1->y=y;
	OR1->Dir=Dir;
	OR1->Size=sizeof B_SmartSend;
	OR1->NextX=0xFFFF;
	OR1->NextY=0xFFFF;
	OR1->NextTop=0xFFFF;
	OR1->Message=HGST_Message;	
	//if(EngSettings.AutoChangeFormationType){
	//	int R=Norma(xc-x,yc-y);
	//	if(R>500){
	//      int ac=GetActivity();
	//		if(ac==1){//defensive				
	//          int bt=GetFormIndex();
	//			if(bt==0){
	//				void MakeReformation(byte NI,word BrigadeID,byte FormType);
	//				MakeReformation(CT->NI,ID,1);
	//			}
	//		}
	//	}
	//}else{
	//	OR1->BLink(this);
	//}
	OR1->BLink(this);
	*/ //
	BrigadeOrder_HumanGlobalSendTo* ST = new BrigadeOrder_HumanGlobalSendTo();
	ST->xx=x;
	ST->yy=y;
	ST->Dir=Dir;
	ST->NextX=0xFFFF;
	ST->NextY=0xFFFF;
	ST->NextTop=0xFFFF;
	CreateNewBOrder(OrdType,ST);
	ST->Process(this);
};
extern word NPlayers;
void Pricol(byte NI){
	//checking if smoke online :)
	{
		char* other=NULL;
		byte OtherC=0;
		bool smoke=false;
		for(int i=0;i<NPlayers;i++){
			if(!stricmp(GSets.CGame.PL_INFO[i].name,"Smoke")){
				if(NI==GSets.CGame.PL_INFO[i].ColorID)smoke=true;
			}else{
				OtherC=GSets.CGame.PL_INFO[i].ColorID;
				other=GSets.CGame.PL_INFO[i].name;
			}
		}
		if(smoke&&other){
			char ccc[256];
			sprintf(ccc,"%s: Smoke, konchaj materitsa, nu kak ne stidno!",other);
			AssignHint1(ccc,250,16+OtherC);
		}
	}
	{
		char* other=NULL;
		byte OtherC=0;
		bool smoke=false;
		for(int i=0;i<NPlayers;i++){
			if(!stricmp(GSets.CGame.PL_INFO[i].name,"Tiamat")){
				if(NI==GSets.CGame.PL_INFO[i].ColorID)smoke=true;
			}else{
				OtherC=GSets.CGame.PL_INFO[i].ColorID;
				other=GSets.CGame.PL_INFO[i].name;
			}
		}
		if(smoke&&other){
			char ccc[256];
			sprintf(ccc,"%s: Tiamat, a neploxo igra idet, ne tormozit i ne gluchit!",other);
			if(rand()<3000)AssignHint1(ccc,250,16+OtherC);
		}
	}
};
void SearchArmy(OneObject* OB);
void EraseBrigade(Brigade* BR){
	if(BR->vLastEnemyID>=0xFFFE){
		ext_OnBrigadeFreedManually(BR);
	}else{
		ext_OnBrigadeKilled(BR,BR->vLastEnemyNI);		
		if(BR->NMemb>3)LogBattle(BR->CT->NI,"^ST_LOSTF^ %d %s",BR->NMemb-NBPERSONAL,GetSt_UName(BR->MembID));
	}
	if(BR->Ability)
	{
		BR->Ability->Clear();
		delete BR->Ability;
		BR->Ability==NULL;
	}
	// add info statistics in academy
	if(BR->WarType){
		byte NI=BR->CT->NI;
		Nation* NT=NATIONS+NI;
		NewMonster* NM=NT->Mon[BR->MembID]->newMons;
		if(NT&&NM){
			Nation* ENT=NULL;
			if(NI!=BR->vLastEnemyNI) ENT=NATIONS+BR->vLastEnemyNI;
			if(NM->PortBranch==0){
				NT->NBrInfantry.Lost++;
				if(ENT) ENT->NBrInfantry.Killed++;
			}else
			if(NM->PortBranch==1){
				NT->NBrCavalery.Lost++;
				if(ENT) ENT->NBrCavalery.Killed++;
			}
			// erased and killed by NIndex
			NT->NBrLost[BR->MembID]++;
			if(ENT) ENT->NBrErased[BR->MembID]++;
		}
	}
	//Pricol(NI);
	bool PANIC=0;	
	PANIC=EngSettings.MoraleType==0&&BR->Morale<32*10000&&BR->WarType;	
	if(BR->WarType&&BR->ArmyID!=0xFFFF){
		BR->WarType=0;
		int nr=0;
		for(int i=0;i<BR->NMemb;i++){
			if(i<NBPERSONAL){
				word mid=BR->Memb[i];
				if(mid!=0xFFFF){
					OneObject* OB=Group[mid];
					if(OB&&(!OB->Sdoxlo)&&OB->Serial==BR->MembSN[i]){
						OB->BrigadeID=0xFFFF;
						OB->BrIndex=0xFFFF;
						OB->InArmy=0;
						OB->ClearOrders();
						SearchArmy(OB);
						if(PANIC)PanicUnit(OB);
					};
				};
			}else{
				word mid=BR->Memb[i];
				if(mid!=0xFFFF){
					OneObject* OB=Group[mid];
					if(OB&&(!OB->Sdoxlo)&&OB->Serial==BR->MembSN[i]){
						BR->Memb[nr]=OB->Index;
						OB->BrIndex=nr;
						OB->InArmy=0;
						BR->MembSN[nr]=OB->Serial;
						nr++;
					};
				};
			};
		};
		BR->NMemb=nr;
		free(BR->posX);
		BR->posX=0;
		free(BR->posY);
		BR->posY=0;
		BR->PosCreated=0;
		return;
	};
	for(int i=0;i<BR->NMemb;i++){
		word MID=BR->Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				OB->BrigadeID=0xFFFF;
				OB->BrIndex=0xFFFF;
				OB->InArmy=false;
				OB->StandGround=false;
				OB->AddDamage=0;
				OB->AddShield=0;
				OB->GroundState=0;
				addrand(OB->GroundState+1);
				if(PANIC)PanicUnit(OB);
				OB->UnitSpeed=64;
				OB->CurUnitSpeed=0;
				OB->GroupSpeed=OB->newMons->MotionDist;//(OB->newMons->MotionDist*OB->MoreCharacter->Speed)/100;
			};
		};
	};
	if(BR->Memb){
		free(BR->Memb);
		free(BR->MembSN);
		BR->Memb=NULL;
		BR->MembSN=NULL;
	};
	City* CT=BR->CT;
	int id=BR->ID;
	BR->DeleteAll();
	memset(BR,0,sizeof Brigade);
	BR->ID=id;
	BR->CT=CT;
};
int GetEnemyDensity(int x,int y,byte Mask,word& EnemyID,byte opt){
	int px=x>>11;
	int py=y>>11;	
	byte mask=~Mask;
	int NEN=0;
	int wt=0;
	if(px>=0&&py>=0&&px<VAL_MAXCX&&py<VAL_MAXCX){
		int cell=px+(py<<VAL_SHFCX);
		if(NPresence[cell]&mask){
			//Enemy present!!!
			cell+=VAL_MAXCX+1;
			int NMon=MCount[cell];
			if(!NMon)return 0xFFFF;
			int ofs1=cell<<SHFCELL;
			word MID;
			for(int i=0;i<NMon;i++){
				MID=GetNMSL(ofs1+i);
				if(MID!=0xFFFF){
					OneObject* OBJ=Group[MID];
					int RR=0;
					if(OBJ&&(!OBJ->Sdoxlo)&&(!(OBJ->NMask&Mask||OBJ->newMons->Artilery||OBJ->UnlimitedMotion))&&(RR=Norma(x-OBJ->RealX,y-OBJ->RealY))<800){
						byte br=OBJ->BrigadeID==0xFFFF?1:2;
						if(opt&br){
							NEN++;
							wt+=5120/(512+RR);
							EnemyID=MID;
						}
					}
				}
			}
		}		
	}
	if(NEN<=2)wt=0;
	return wt;
}
word CheckMotionThroughEnemyAbility(OneObject* OB,int px,int py){
	byte opt=1+2;//usual units + units int formations
	if(OB->BrigadeID!=0xFFFF&&OB->Attack)return 0xFFFF;
	if(OB->GroundState==0&&OB->BrigadeID!=0xFFFF){
		opt=2;
	}
	word EnID=0xFFFF;
    int ws=GetEnemyDensity(OB->RealX,OB->RealY,OB->NMask,EnID,opt);
	ws+=ws/10;
	int wd=GetEnemyDensity(px,py,OB->NMask,EnID,opt);
	if(ws>=wd)return 0xFFFF;
	if(EnID!=0xFFFF&&!(OB->NoSearchVictim||OB->ActivityState!=2))OB->AttackObj(EnID,128+1,0,0);
	return EnID;
};

//------------------------------Bitva------------------------
char* BBIT_Message="[Brigade::Bitva]";
//#define maxen
class Brigade_Bitva:public BrigadeOrder{
public:
	word StartTop;
	byte BitMask[1024*8];
	word Enm[512];
	word EnSN[512];
	byte NDang[512];
	int  NEn;
	int MinX;
	int MinY;
	int MaxX;
	int MaxY;
	void AddEnemXY(int x,int y,int MyTop,byte mask);
};
word GetTopFast(int x,int y);
void Brigade_Bitva::AddEnemXY(int x,int y,int MyTop,byte mask){
	if(NEn>=512)return;
	int x0=x<<1;
	int y0=y<<1;

	int HimTop1=GetTopFast(x0,y0);
	int HimTop2=GetTopFast(x0+1,y0);
	int HimTop3=GetTopFast(x0,y0+1);
	int HimTop4=GetTopFast(x0+1,y0+1);
	if(HimTop1>=0xFFFE&&HimTop2>=0xFFFE&&HimTop3>=0xFFFE&&HimTop4>=0xFFFE)return;
	int NTP=MyTop*GetNAreas();
	if(HimTop1<0xFFFE){
		if(HimTop1!=MyTop&&GetLinksDist(HimTop1+NTP)>30)return;
	};
	if(HimTop2<0xFFFE){
		if(HimTop2!=MyTop&&GetLinksDist(HimTop2+NTP)>30)return;
	};
	if(HimTop3<0xFFFE){
		if(HimTop3!=MyTop&&GetLinksDist(HimTop3+NTP)>30)return;
	};
	if(HimTop4<0xFFFE){
		if(HimTop4!=MyTop&&GetLinksDist(HimTop4+NTP)>30)return;
	};
	int cell=x+1+((y+1)<<VAL_SHFCX);
	if(cell>0&&cell<MAXCIOFS){
		int NMon=MCount[cell];
		if(!NMon)return;
		int ofs1=cell<<SHFCELL;
		word MID;
		for(int i=0;i<NMon;i++){
			MID=GetNMSL(ofs1+i);
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&!(OB->NMask&mask||OB->Sdoxlo||OB->LockType==1)){
					int id=OB->Index;
					int ms=1<<(id&7);
					int of=id>>3;
					if(!(BitMask[of]&ms)){
						BitMask[of]|=ms;
						Enm[NEn]=id;
						EnSN[NEn]=OB->Serial;
						int xx=OB->RealX>>10;
						int yy=OB->RealY>>10;
						if(xx>0&&yy>0){
							int off=xx+(yy<<TopSH);
							NDang[NEn]=0;//((InflMap[off-1]&255)+(InflMap[off+1]&255)+(InflMap[off+TopLx]&255)+(InflMap[off-TopLx]&255))>>2;
						}else NDang[NEn]=0;
						NEn++;
						if(NEn>=512)return;
					};
				};
			};
		};
	};
};
word GetDir(int dx,int dy);
void AttackObjLink(OneObject* OBJ);
bool CheckAttDist(int x,int y,word Victim){
	if(Victim!=0xFFFF){		
		OneObject* VOB=Group[Victim];
		if(VOB){
			return Norma(x-(VOB->RealX>>4),y-(VOB->RealY>>4))<800;
		}
	}
	return false;
}
void B_BitvaLink(Brigade* BR){

	for(int i=0;i<BR->NMemb;i++)
	{
		if(BR->Memb[i]!=0xFFFF)
		{
			OneObject* OB = Group[BR->Memb[i]];
			if(OB&&(!OB->Sdoxlo)&&OB->delay==0&&OB->RifleAttack)
			{
				BR->DeleteBOrder();
				BrigadeRifleAttack(OB);
				return;
			}
		}
	}
	Brigade_Bitva* OR1=(Brigade_Bitva*)BR->BOrder;
	byte Mask=1<<BR->CT->NI;
	//1.Check range of battle
	int xc=0;
	int yc=0;
	BR->GetCenter(&xc,&yc);
	if(rando()<1024||OR1->StartTop==0xFFFF){
		word Top=0xFFFF;
		int MinX=10000000;
		int MinY=10000000;
		int MaxX=0;
		int MaxY=0;	
		int N=BR->NMemb;
		word* Mem=BR->Memb;
		word* MSN=BR->MembSN;
		for(int j=0;j<N;j++){
			word MID=Mem[j];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==MSN[j]){
					if(Top==0xFFFF){
						Top=GetTopology(OB->RealX>>4,OB->RealY>>4);
						OR1->StartTop=Top;
					};
					int xx=OB->RealX>>11;
					int yy=OB->RealY>>11;
					if(xx<MinX)MinX=xx;
					if(yy<MinY)MinY=yy;
					if(xx>MaxX)MaxX=xx;
					if(yy>MaxY)MaxY=yy;
				};
			};
		};
		if(MaxX>=MinX){
			MinX-=4;
			MaxX+=4;
			MinY-=4;
			MaxY+=4;
			if(MinX<0)MinX=0;
			if(MinY<0)MinY=0;
			OR1->MinX=MinX;
			OR1->MaxX=MaxX;
			OR1->MinY=MinY;
			OR1->MaxY=MaxY;
		}else{
			BR->DeleteBOrder();
			return;
		};
	};
	//2.renew enemy list
	if(rando()<1024){
		int N=OR1->NEn;
		word* Mem=OR1->Enm;
		word* MSN=OR1->EnSN;
		byte* ATT=OR1->NDang;
		for(int i=0;i<N;i++){
			OneObject* OB=Group[Mem[i]];
			if((!OB)||(OB&&(OB->Sdoxlo||OB->Serial!=MSN[i]))){
				//delete enemy from list
				if(i<N-1){
					memcpy(Mem+i,Mem+i+1,(N-i-1)<<1);
					memcpy(MSN+i,MSN+i+1,(N-i-1)<<1);
					memcpy(ATT+i,ATT+i+1,N-i-1);
				};
				i--;
				N--;
			};
		};
		OR1->NEn=N;
	};
	int MaxAttServ=64;
	bool InBattle=false;
	//3.Adding new units to battle
	int MinX=OR1->MinX;
	int MinY=OR1->MinY;
	int Dx=OR1->MaxX-OR1->MinX+1;
	int Dy=OR1->MaxY-OR1->MinY+1;
	for(int p=0;p<64;p++){
		int xx=((int(rando())*Dx)>>15)+MinX;
		int yy=((int(rando())*Dy)>>15)+MinY;		
		OR1->AddEnemXY(xx,yy,OR1->StartTop,Mask);
	};
	//4.Attack service
	bool MorPresent=false;
	int N=BR->NMemb;
	word* Mem=BR->Memb;
	word* MSN=BR->MembSN;
	for(int j=0;j<N;j++){
		word MID=Mem[j];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			void NewAttackPointLink(OneObject* OBJ);			
			if(OB&&OB->Serial==MSN[j]&&!OB->newMons->Artilery){
				if(OB->EnemyID!=0xFFFF){
					if(!(CheckAttDist(xc,yc,OB->EnemyID)||OB->RifleAttack)){
						OB->ClearOrders();
					}
				}else{
					if((!(OB->LocalOrder))||(OB->EnemyID==0xFFFF&&CheckIfPossibleToBreakOrder(OB))){
						//need to find enemy
						NewMonster* NM=OB->newMons;
						AdvCharacter* ADC=OB->MoreCharacter;
						byte mms=NM->KillMask;
						int MinR=ADC->MinR_Attack;
						int MaxR=ADC->MaxR_Attack;
						int MaxMaxR=700;
						int ARD=OB->newMons->ArmRadius;
						if(!OB->RifleAttack){
							if(ARD<200)ARD=200;
							MaxR=ARD;							
						}
						int myx=OB->RealX;
						int myy=OB->RealY;
						if(MaxR){
							//search best enemy
							int NearDist=1000000;
							int ReadyDist=1000000;
							if(!OB->RifleAttack){
								//NearDist=ARD;
								//ReadyDist=ARD;
							}
							word NearMID=0xFFFF;
							word ReadyMID=0xFFFF;
							int NEn=OR1->NEn;
							word* Mem=OR1->Enm;
							word* MSN=OR1->EnSN;
							byte* DANG=OR1->NDang;
							char mdr=OB->RealDir;
							for(int t=0;t<NEn;t++){
								word MID=Mem[t];
								OneObject* EOB=Group[MID];
								int dan=DANG[t];
								NewMonster* ENM=NULL;
								if(EOB)ENM=EOB->newMons;
								if(EOB&&(ENM->MathMask&mms)&&(!EOB->Sdoxlo)&&EOB->Serial==MSN[t]){
									int Eusage=ENM->Usage;
									if(Eusage==MortiraID)MorPresent=true;
									//nash paren!
									int R=Norma(myx-EOB->RealX,myy-EOB->RealY)>>4;
									if(R>MinR){
										int NA=ATTLIST.GetNAttackers(EOB);
										int RE=R+NA*160*16;
										if((RE<NearDist&&(dan<7||R<MaxR||MorPresent))||(RE<NearDist&&OB->SearchOnlyThisBrigadeToKill!=0xFFFF&&EOB->BrigadeID==OB->SearchOnlyThisBrigadeToKill&&R<MaxMaxR)){
											//checking for direction of motion
											if(EOB->InMotion){
												int dr1=GetDir(EOB->RealX-myx,EOB->RealY-myy);
												char dr=EOB->RealDir-dr1;
												if(abs(dr)>64){
													NearMID=MID;
													NearDist=RE;
												};
											}else{
												NearMID=MID;
												NearDist=RE;
											};
										};
										if(R<MaxR&&RE<ReadyDist){
											ReadyMID=MID;
											ReadyDist=RE;
										};
									};
								};
							};
							if(ReadyMID==0xFFFF)ReadyMID=NearMID;
							if(ReadyMID!=0xFFFF&&(CheckAttDist(xc,yc,ReadyMID)||OB->RifleAttack)){								
								OneObject* EOB=Group[ReadyMID];
								int R=Norma(OB->RealX-EOB->RealX,OB->RealY-EOB->RealY)>>4;
								if(OB->RifleAttack||R<200||(OB->SearchOnlyThisBrigadeToKill!=0xFF&&EOB->BrigadeID==OB->SearchOnlyThisBrigadeToKill)){
									if(!OB->AttackObj(ReadyMID,128+8)){
										if(Norma((BR->posX[j]<<4)-OB->RealX,(BR->posY[j]<<4)-OB->RealY)>16*16){
											OB->NewMonsterPreciseSendTo(BR->posX[j]<<4,BR->posY[j]<<4,128+1,0);
										}
									}
								}else{
									if(Norma((BR->posX[j]<<4)-OB->RealX,(BR->posY[j]<<4)-OB->RealY)>16*16){
										OB->NewMonsterPreciseSendTo(BR->posX[j]<<4,BR->posY[j]<<4,128+1,0);
									}
								}
								InBattle=true;
							}else{
								addrand(128+1);
								if(Norma((BR->posX[j]<<4)-OB->RealX,(BR->posY[j]<<4)-OB->RealY)>16*16){
									OB->NewMonsterPreciseSendTo(BR->posX[j]<<4,BR->posY[j]<<4,128+1,0);
								}
							};
						};
					}else{
						if(!(rando()<8192&&!OB->RifleAttack))InBattle=true;
					}
				}
			}
		}
	}
	if(!InBattle){
		BR->DeleteBOrder();
	};
};
void Brigade::Bitva(){
	if(NewBOrder&&NewBOrder->GetBrigadeOrderID()==BRIGADEORDER_BITVA)return;
	
	int N=NMemb;
	word* Mem=Memb;
	word* MSN=MembSN;
	for(int j=0;j<N;j++)
	{
		word MID=Mem[j];
		if(MID!=0xFFFF)
		{
			OneObject* OB=Group[MID];
			if(OB&&OB->RifleAttack&&OB->delay==0)
			{
				BrigadeRifleAttack(OB);
				return;
			}
		}
	}
	/* //
	Brigade_Bitva* OR1=(Brigade_Bitva*)CreateBOrder(1,sizeof Brigade_Bitva);
	SetStandState(this,1);
	OR1->StartTop=0xFFFF;
	OR1->BLink=&B_BitvaLink;
	OR1->Size=sizeof Brigade_Bitva;
	OR1->Prio=0;
	OR1->Message=BBIT_Message;
	OR1->NEn=0;
	memset(OR1->BitMask,0,sizeof OR1->BitMask);
	*/ //
	int MinX=10000000;
	int MinY=10000000;
	int MaxX=0;
	int MaxY=0;

	BrigadeOrder_Bitva* Bt = new BrigadeOrder_Bitva();
	CreateNewBOrder(1,Bt);
	SetStandState(this,1);
	Bt->StartTop=0xFFFF;
	Bt->NEn=0;


	
	word Top=0xFFFF;
	for(int j=0;j<N;j++){
		word MID=Mem[j];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==MSN[j]){
				if(Top==0xFFFF){
					Top=GetTopology(OB->RealX>>4,OB->RealY>>4);
				};
				int xx=OB->RealX>>11;
				int yy=OB->RealY>>11;
				if(xx<MinX)MinX=xx;
				if(yy<MinY)MinY=yy;
				if(xx>MaxX)MaxX=xx;
				if(yy>MaxY)MaxY=yy;
			};
		};
	};
	byte Mask=1<<CT->NI;
	if(MaxX>=MinX){
		MinX-=4;
		MaxX+=4;
		MinY-=4;
		MaxY+=4;
		if(MinX<0)MinX=0;
		if(MinY<0)MinY=0;
		for(int ix=MinX;ix<=MaxX;ix++){
			for(int iy=MinY;iy<=MaxY;iy++){
				//OR1->AddEnemXY(ix,iy,Top,Mask);
				Bt->AddEnemXY(ix,iy,Top,Mask);
			};
		};
		/*
		OR1->MinX=MinX;
		OR1->MaxX=MaxX;
		OR1->MinY=MinY;
		OR1->MaxY=MaxY;
		*/
		Bt->MinX=MinX;
		Bt->MaxX=MaxX;
		Bt->MinY=MinY;
		Bt->MaxY=MaxY;
	}else{
		//DeleteBOrder();
		DeleteNewBOrder();
		return;
	};
};
extern City CITY[8];
void SelBrigade(byte NI,byte Type,byte,int ID);
void ImSelBrigade(byte NI,byte Type,byte BNAT,int ID);
void CorrectBrigadesSelection(byte NT){
	NT&=7;
	int BrigsID[512];
	byte BNats[512];
	byte BFlags[128];
	memset(BFlags,0,sizeof BFlags);
	int NBR=0;
	int N=NSL[NT];
	word* mon=Selm[NT];
	City* CT=CITY+GSets.CGame.cgi_NatRefTBL[NT];
	for(int i=0;i<N;i++){
		word MID=mon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo){
				if(OB->InArmy&&OB->BrigadeID!=0xFFFF){
					int idx=OB->BrigadeID;
					int xx=idx>>3;
					int sms=1<<(idx&7);
					if(!(BFlags[xx>>3]&sms)){
						if(NBR<512){
							BrigsID[NBR]=idx;
							BNats[NBR]=OB->NNUM;
							NBR++;
							BFlags[xx>>3]|=sms;
						};
					};
				};
			};
		};
	};
	for(int i=0;i<NBR;i++){
		SelBrigade(NT,1,BNats[i],BrigsID[i]);
	};
};
void CorrectImSelectionInGroups(byte NI);
void ImCorrectBrigadesSelection(byte NT){
	int BrigsID[512];
	byte BNats [512];
	byte BFlags[128];
	memset(BFlags,0,sizeof BFlags);
	int NBR=0;
	int N=ImNSL[NT];
	word* mon=ImSelm[NT];
	City* CT=CITY+GSets.CGame.cgi_NatRefTBL[NT];
	for(int i=0;i<N;i++){
		word MID=mon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo){
				if(OB->InArmy&&OB->BrigadeID!=0xFFFF){
					int idx=OB->BrigadeID;
					int xx=idx>>3;
					int sms=1<<(idx&7);
					if(!(BFlags[xx>>3]&sms)){
						if(NBR<512){
							BrigsID[NBR]=idx;
							BNats  [NBR]=OB->NNUM;
							NBR++;
							BFlags[xx>>3]|=sms;
						};
					};
				};
			};
		};
	};
	for(int i=0;i<NBR;i++){
		ImSelBrigade(NT,1,BNats[i],BrigsID[i]);
	};
	CorrectImSelectionInGroups(NT);
};
//                                 //\\
//                               // /\ \\
//                             // / /\ \ \\
//----------------------------<< WATER AI >>--------------------------//
//                             \\ \ \/ / //
//                               \\ \/ //
//                                 \\//

//Human ships location system



//==========================RESEARCH ISLANDS===============================//
int NIslands=0;
#define MaxIsl 512
int IslandX[MaxIsl];
int IslandY[MaxIsl];
byte IslPrs[MaxIsl];

word* TopIslands=NULL;
word* NearWater=NULL;
void CheckGP();
void ArrangeAreas(int ID,byte IsID,int Deep){
	TopIslands[ID]=IsID;
	if(Deep<100){
		Area* AR=GetTopMap(ID);
		int NA=GetNAreas(0);
		for(int i=0;i<AR->NLinks;i++){
			int D1=AR->Link[i].NextAreaID;
			if(D1>=0&&D1<NA&&TopIslands[D1]==0xFFFF)ArrangeAreas(D1,IsID,Deep+1);
		};
	};
};
void ResearchIslands(){
	if(!GetNAreas())return;
	if(TopIslands){
		TopIslands=(word*)realloc(TopIslands,2*GetNAreas());
		NearWater=(word*)realloc(NearWater,2*GetNAreas());
	}else{
		TopIslands=znew(word,GetNAreas());
		NearWater=znew(word,GetNAreas());
	};
	memset(TopIslands,0xFF,2*GetNAreas());
	NIslands=0;
	int NA=GetNAreas();
	for(int j=0;j<NA;j++)if(TopIslands[j]==0xFFFF){
		ArrangeAreas(j,NIslands,0);
		for(int p=0;p<NA;p++)if(TopIslands[p]==NIslands)ArrangeAreas(p,NIslands,0);
		NIslands++;
	}
	word NTIsl[MaxIsl];
	assert(NIslands<MaxIsl);
	//calculating center
	for(int i=0;i<NIslands;i++){
		IslandX[i]=0;
		IslandY[i]=0;
		NTIsl[i]=0;
	};
	for(int i=0;i<NA;i++){
		Area* Ar=GetTopMap(i);
		int ISL=TopIslands[i];
		IslandX[ISL]+=Ar->x;
		IslandY[ISL]+=Ar->y;
		NTIsl[ISL]++;
	};
	for(int i=0;i<NIslands;i++){
		IslandX[i]/=NTIsl[i];
		IslandY[i]/=NTIsl[i];
		IslPrs[i]=0;
	};
	//calculating the cost
	memset(NearWater,0xFF,GetNAreas()<<1);
	int tpmax=msx>>1;
	for(int ix=3;ix<tpmax-3;ix++)
		for(int iy=3;iy<tpmax-3;iy++){
			int ctp=SafeTopRef(ix,iy);
			if(ctp<0xFFFE){
				for(int dx=-3;dx<=3;dx++)
					for(int dy=-3;dy<=3;dy++){
						int wtp=GetTopRef(ix+dx+((iy+dy)<<TopSH),1);
						if(wtp<0xFFFE){
							if(NearWater[ctp]==0xFFFF){
								NearWater[ctp]=wtp;
							}else{
								Area* CAR=GetTopMap(ctp);
								Area* OLD=GetTopMap(NearWater[ctp],1);
								Area* NEW=GetTopMap(wtp,1);
								if(Norma(CAR->x-OLD->x,CAR->y-OLD->y)>Norma(CAR->x-NEW->x,CAR->y-NEW->y)){
									NearWater[ctp]=wtp;
								};
							};
						};
					};
			};
		};
};

void ResearchCurrentIsland(byte Nat){
	///return;
	if(NATIONS[Nat].AI_Enabled&&CITY[Nat].MyIsland!=0xFF){
		if(SCENINF.hLib&&!SCENINF.StandartVictory)return;
	};
	if(!TopIslands)return;
	//delete it:
	int NA=GetNAreas();
	addrand(NA);
	for(int f=0;f<NA;f++)addrand(TopIslands[f]);
	//-----------
	int NInIsl[MaxIsl];
	memset(NInIsl,0,sizeof NInIsl);
	memset(IslPrs,0,sizeof IslPrs);
	int N=NtNUnits[Nat];
	word* Uni=NatList[Nat];
	if(!N){
		CITY[Nat].MyIsland=0xFF;
		return;
	};
	for(int i=0;i<N;i++){
		OneObject* OB=Group[Uni[i]];
		if(OB){
			int tof=(OB->RealX>>10)+((OB->RealY>>10)<<TopSH);
			if(tof>=0&&tof<TopLx*TopLx){
				int tp=GetTopRef(tof);
				if(tp<0xFFFE){
					NInIsl[TopIslands[tp]]++;
					addrand(NInIsl[TopIslands[tp]]);//!!
					if(!(OB->NewBuilding||OB->Sdoxlo||OB->LockType)){
						IslPrs[TopIslands[tp]]|=1<<OB->NNUM;
						addrand(IslPrs[TopIslands[tp]]);//!!
					};
				};
			};
		};
	};
	int NMax=0;
	int CISL=0xFF;
	addrand(NIslands);
	for(int i=0;i<NIslands;i++){
		addrand(NInIsl[i]);
		addrand(NMax);
		if(NInIsl[i]>NMax){
			NMax=NInIsl[i];
			CISL=i;
			addrand(CISL);
		};
	};
	CITY[Nat].MyIsland=CISL;
	addrand(CITY[Nat].MyIsland);//to delete
};
//======================RESEARCH BEST PORT TO FISH=========================//
void ResearchBestPortToFish(byte Nat){
	addrand(Nat);
	int N=NtNUnits[Nat];
	word* Uni=NatList[Nat];
	if(!N)return;
	addrand(N);
	addrand(CITY[0].MyIsland==255);
	addrand(NATIONS[Nat].AI_Enabled);
	if(CITY[0].MyIsland==255||!NATIONS[Nat].AI_Enabled)return;
	int x0=IslandX[CITY[0].MyIsland]<<10;
	int y0=IslandY[CITY[0].MyIsland]<<10;
	addrand(CITY[0].MyIsland);
	addrand(x0);
	addrand(y0);
	int MaxR=0;
	int BestID=-1;
	int NP=0;
	addrand(BestID);
	for(int i=0;i<N;i++){
		OneObject* OB=Group[Uni[i]];
		if(OB&&(!OB->Sdoxlo)&&OB->newMons->Usage==PortID){
			int R=Norma(OB->RealX-x0,OB->RealY-y0);
			if(R>MaxR){
				MaxR=R;
				BestID=OB->Index;
			};
			NP++;
		};
	};
	addrand(BestID);
	if(BestID!=-1){
		for(int i=0;i<N;i++){
			OneObject* OB=Group[Uni[i]];
			if(OB&&(!OB->Sdoxlo)&&OB->newMons->Usage==PortID){
				if(OB->Index==BestID){
					addname(OB->Index);
					if(NP){
						OB->StandGround=1;
						addrand(OB->NoSearchVictim);
						OB->NoSearchVictim=1;
						addrand(OB->NoSearchVictim);
					}else{
						OB->StandGround=1;
						addrand(OB->NoSearchVictim);
						OB->NoSearchVictim=0;
						addrand(OB->NoSearchVictim);
					};
				}else{
					addname(OB->Index);
					if(NP){
						OB->StandGround=0;
						addrand(OB->NoSearchVictim);
						OB->NoSearchVictim=0;
						addrand(OB->NoSearchVictim);
					}else{
						OB->StandGround=1;
						addrand(OB->NoSearchVictim);
						OB->NoSearchVictim=0;
						addrand(OB->NoSearchVictim);
					};
				};
			};
		};
	};
};
void ProduceBattleShip(byte nat,word ID){
	int N=NtNUnits[nat];
	word* Uni=NatList[nat];
	for(int i=0;i<N;i++){
		OneObject* OB=Group[Uni[i]];
		if(OB&&(!(OB->Sdoxlo||OB->LocalOrder))&&OB->newMons->Usage==PortID){
			//if(!OB->NoSearchVictim)
			OB->Produce(ID);
		};
	};
};
void ProduceLodka(byte nat,word ID){
	int N=NtNUnits[nat];
	word* Uni=NatList[nat];
	for(int i=0;i<N;i++){
		OneObject* OB=Group[Uni[i]];
		if(OB&&(!(OB->Sdoxlo||OB->LocalOrder))&&OB->newMons->Usage==PortID){
			if(OB->StandGround)OB->Produce(ID);
		};
	};
};
void ProduceByAI(byte nat,word Producer,word ID){
	byte Use=NATIONS[nat].Mon[ID]->newMons->Usage;
	switch(Use){
	case FisherID:
		ProduceLodka(nat,ID);
		break;
	case GaleraID:
	case LinkorID:
	case IaxtaID:
	case FregatID:
	case ShebekaID:
		ProduceBattleShip(nat,ID);
		break;
	default:
		{
			OneObject* OB=Group[Producer];
			int N=0;
			Order1* OR1=OB->LocalOrder;
			while(OR1){
                OR1=OR1->NextOrder;
				N++;
			}            
			if(N<4)Group[Producer]->Produce(ID,CITY[nat].DestUnitsGroup);
		}
	};
};
//====================Research human places for attack=====================//




//extern word TowsID[24];
//extern word TowsSN[24];
word SearchMineToDestroy(Brigade* BR){
	int NGalley=0;
	word GalleyID[32];
	int GalleyX[32];
	int GalleyY[32];
	int GalleyR;
	for(int i=0;i<BR->NMemb&&NGalley<32;i++){
		word mid=BR->Memb[i];
		if(mid!=0xFFFF){
			OneObject* OB=Group[mid];
			if(OB&&OB->newMons->Usage==GaleraID&&!OB->Sdoxlo){
				if(!NGalley){
					GalleyR=OB->MoreCharacter->MaxR_Attack<<4;
				};
				GalleyID[NGalley]=OB->Index;
				GalleyX[NGalley]=OB->RealX;
				GalleyY[NGalley]=OB->RealY;
				NGalley++;
			};
		};
	};
	if(!NGalley)return 0xFFFF;
	
	int MinD=1000000;
	word BestMINE=0xFFFF;
	GalleyR-=64*16;
	if(BR->NMemb>=4){
		for(int MID=0;MID<MAXOBJECT;MID++){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo){
				int mx=OB->RealX;
				int my=OB->RealY;
				byte Use=OB->newMons->Usage;
				if(Use==MineID||Use==TowerID){
					for(int j=0;j<NGalley;j++){
						int D=Norma(GalleyX[j]-mx,GalleyY[j]-my);
						if(D<MinD&&D<GalleyR){
							MinD=D;
							BestMINE=OB->Index;
						};
					};
				};
			};
		};
	}else{
		for(int MID=0;MID<MAXOBJECT;MID++){
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->newMons->Usage==MineID&&!OB->Sdoxlo){
					int mx=OB->RealX;
					int my=OB->RealY;
					for(int j=0;j<NGalley;j++){
						int D=Norma(GalleyX[j]-mx,GalleyY[j]-my);
						if(D<MinD&&D<GalleyR){
							MinD=D;
							BestMINE=OB->Index;
						};
					};
				};
			};
		};
	};
	if(BestMINE!=0xFFFF){
		for(int i=0;i<NGalley;i++){
			Group[GalleyID[i]]->AttackObj(BestMINE,128+16,0,0);
		};
	};
	return BestMINE;
};
int GetShipForce(byte Usage);
word SearchEnemyToDestroy(Brigade* BR){
	int RMIN[128];
	int RMAX[128];
	int DANG[128];
	int XI[128];
	int YI[128];
	int XC=0;
	int YC=0;
	int DR=0;
	int NS=0;
	EnemyInfo* EIN=GNFO.EINF[BR->CT->NI];
	if(!EIN)return 0xFFFF;
	bool CanAttBLD=0;
	for(int i=0;i<BR->NMemb;i++){
		word MID=BR->Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(NS<128&&OB&&(!OB->Sdoxlo)&&OB->Serial==BR->MembSN[i]){
				NewMonster* NM=OB->newMons;
				byte Usage=NM->Usage;
				if(NM->AI_use_against_buildings)CanAttBLD=1;
				AdvCharacter* ADC=OB->MoreCharacter;
				int RR=0;
				int RR2=10000;
				RMAX[NS]=ADC->MaxR_Attack;
				RMIN[NS]=ADC->MinR_Attack;
				//if(RMAX[NS]>NM->AI_PreferredAttR_Max)RMAX[NS]=NM->AI_PreferredAttR_Max;
				/*
				if(Usage==GaleraID){
					RMIN[NS]=ADC->AttackRadius1[1];
					RMAX[NS]=ADC->AttackRadius2[1]-64-DR;
				}else{
					RMIN[NS]=ADC->AttackRadius1[0];
					RMAX[NS]=ADC->AttackRadius2[0]-64-DR;
				};
				*/
				RMIN[NS]<<=4;
				RMAX[NS]<<=4;

				DANG[NS]=GetShipForce(Usage);
				XI[NS]=OB->RealX;
				YI[NS]=OB->RealY;
				NS++;
				XC+=OB->RealX;
				YC+=OB->RealY;
			};
		};
	};
	if(NS){
		int MAXR=0;
		XC/=NS;
		YC/=NS;
		int MAXF=1000000;//-1;
		word BestID=0xFFFF;
		for(int j=0;j<NS;j++){
			int R=Norma(XI[j]-XC,YI[j]-YC);
			if(R+RMAX[j]>MAXR)MAXR=R+RMAX[j];
		};
		if(BR->LastEnemyID!=0xFFFF){
			OneObject* OB=Group[BR->LastEnemyID];
			if(OB&&(!OB->Sdoxlo)&&OB->Serial==BR->LastEnemySN&&OB->LockType){
				if(Norma(XC-OB->RealX,YC-OB->RealY)<MAXR)BestID=BR->LastEnemyID;
			};
		};
		if(BestID==0xFFFF){
			HumanShip* HS=EIN->SHIPS;		
			for(int i=0;i<EIN->NHSHIPS;i++){
				int DR=Norma(XC-HS->x,YC-HS->y);
				if(DR<MAXR){
					if(MAXF>DR){
						OneObject* OB=Group[HS->ID];
						if(OB&&!OB->Sdoxlo){
							MAXF=DR;
							BestID=HS->ID;
						};
					};
				};
				HS++;
			};
		};
		if(BestID!=0xFFFF)return BestID;
		//search for tower
		int MinTowR=100000;
		BestID=0xFFFF;
		if(CanAttBLD){
			for(int i=0;i<24;i++){
				if(EIN->TowsID[i]!=0xFFFF){
					OneObject* OB=Group[EIN->TowsID[i]];
					if(OB&&(!OB->Sdoxlo)&&OB->Serial==EIN->TowsSN[i]){
						int x=OB->RealX;
						int y=OB->RealY;
						int DR=Norma(x-XC,y-YC);
						if(DR<MAXR&&DR<MinTowR){
							MinTowR=DR;
							BestID=OB->Index;
						};
					};
				};
			};
			//search for port||useful building
			if(rando()<8192&&BestID==0xFFFF){
				bool BFND=false;
				int MinTowR=100000;
				BestID=0xFFFF;
				int N=NtNUnits[0];
				word* Units=NatList[0];
				for(int i=0;i<N;i++){
					OneObject* OB=Group[Units[i]];
					if(OB&&OB->NewBuilding&&OB->Ready&&!OB->Sdoxlo){
						byte Usage=OB->newMons->Usage;
						//if(Usage!=FarmID){
							int x=OB->RealX;
							int y=OB->RealY;
							int DR=Norma(x-XC,y-YC);
							if(DR<MAXR&&DR<MinTowR){
								MinTowR=DR;
								BestID=OB->Index;
								BFND=true;
							};
						//};
					};
				};
			};
			//if(BFND){
				//if(Group[BestID]->MoreCharacter->Shield>50)BestID=0xFFFF;
			//};
			if(BestID==0xFFFF){
				//search for unit
			};
		};
		
		return BestID;
	}else return 0xFFFF;
};
void WaterAttackLink(OneObject* OBJ);
int TryToBeatByShips(Brigade* BR){
	word ENEMY=SearchEnemyToDestroy(BR);
	if(ENEMY!=0xFFFF){
		OneObject* EOB=Group[ENEMY];
		if(EOB->LockType){
			BR->LastEnemyID=ENEMY;
			BR->LastEnemySN=EOB->Serial;
			for(int i=0;i<BR->NMemb;i++){
				word MID=BR->Memb[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&(!OB->Sdoxlo)&&OB->Serial==BR->MembSN[i]){
						if(OB->LocalOrder&&OB->LocalOrder->DoLink==WaterAttackLink){
							if(OB->EnemyID!=ENEMY){
								OB->DeleteLastOrder();
								OB->AttackObj(ENEMY,128+16,1,0);
								if(OB->LocalOrder&&OB->LocalOrder->DoLink==&WaterAttackLink&&OB->newMons->Usage==GaleraID){
									OB->LocalOrder->info.BuildObj.AttMethod=1;
								};
							};
						}else{
							OB->AttackObj(ENEMY,128+16,1,0);
							if(OB->LocalOrder&&OB->LocalOrder->DoLink==&WaterAttackLink&&OB->newMons->Usage==GaleraID){
								OB->LocalOrder->info.BuildObj.AttMethod=1;
							};
						};
					};
				};
			};
		}else{
			int X0=EOB->RealX;
			int Y0=EOB->RealY;
			int NA=0;
			for(int i=0;i<BR->NMemb;i++){
				word MID=BR->Memb[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&(!OB->Sdoxlo)&&OB->Serial==BR->MembSN[i]){
						bool DoAttack=false;
						int R=Norma(OB->RealX-X0,OB->RealY-Y0)>>4;
						AdvCharacter* ADC=OB->MoreCharacter;
						NewMonster* NM=OB->newMons;

						byte Usage=OB->newMons->Usage;
						//if(Usage==GaleraID){
						if(R>ADC->MinR_Attack+64&&R<NM->AI_PreferredAttR_Max-64)DoAttack=true;
						//}else{
						//	if(R>ADC->AttackRadius1[0]+64&&R<ADC->AttackRadius2[0]-64)DoAttack=true;
						//};
						if(DoAttack&&OB->EnemyID!=EOB->Index)OB->AttackObj(EOB->Index,128+16,0,10);
						if(OB->EnemyID==EOB->Index)NA++;
					};
				};
			};
			if(NA)return 2;
			else return 0;
		};
		return 1;
	}else{
		for(int i=0;i<BR->NMemb;i++){
			word MID=BR->Memb[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&(!OB->Sdoxlo)&&OB->Serial==BR->MembSN[i]&&
					OB->LocalOrder&&OB->LocalOrder->DoLink==&WaterAttackLink&&
					OB->LocalOrder->info.BuildObj.SN==0){
						OB->DeleteLastOrder();
				};
			};
		};
		return 0;
	};
};
int TryToBeatByShipsWhenStand(Brigade* BR){
	int RS=TryToBeatByShips(BR);
	if(RS==0||RS==2){

	};
	return 0;
};

class LSS_Order:public BrigadeOrder{
public:
	int FirstTime;
	int NPos;
	int Position[30];
};
void LocalSendShipsLink(Brigade* BR){
	if(TryToBeatByShips(BR))return;
	LSS_Order* OR=(LSS_Order*)BR->BOrder;
	int NReady=0;
	int NTot=0;
	for(int i=0;i<OR->NPos;i++){
		int i3=i+i+i;
		word ID=OR->Position[i3+2]&0xFFFF;
		word SN=DWORD(OR->Position[i3+2])>>16;
		if(ID!=0xFFFF){
			OneObject* OB=Group[ID];
			if(OB&&!OB->Sdoxlo){
				int r=Norma(OB->RealX-OR->Position[i3],OB->RealY-OR->Position[i3+1]);
				if(r<150*16)NReady++;
				else if(!OB->LocalOrder){
					OB->NewMonsterSmartSendTo(OR->Position[i3]>>4,OR->Position[i3+1]>>4,0,0,18+16,0);
				};
				NTot++;
			};
		};
	};
	if(NReady==NTot){
		BR->DeleteBOrder(); 
		return;
	};
	if(OR->FirstTime==-1){
		if(NReady)OR->FirstTime=tmtmt;
	}else{
		if(tmtmt-OR->FirstTime>350)
			BR->DeleteBOrder();
	};
};
extern int RealLx;
extern int RealLy;
void CBar(int x,int y,int Lx,int Ly,byte c);
void ShowShipsDest(Brigade* BR){
	if(BR->BOrder&&BR->BOrder->BLink==LocalSendShipsLink){
		LSS_Order* LO=(LSS_Order*)BR->BOrder;
		for(int i=0;i<LO->NPos;i++){
			int x=LO->Position[i+i+i]>>4;
			int y=LO->Position[i+i+i+1]>>4;
			int sx=x-(mapx<<5);
			int sy=(y>>1)-(mapy<<4)-GetHeight(x,y);
			if(sx>0&&sy>0&&sx<RealLx&&sy<RealLy){
				CBar(sx-4,sy-4,8,8,255);
			};
		};
	};
};
void LocalSendShipsOld(Brigade* BR,int x,int y,byte Type){
	LSS_Order* OR=(LSS_Order*)BR->CreateBOrder(Type,sizeof(LSS_Order)-120+BR->NMemb*12);
	OR->Size=sizeof(LSS_Order)-120+BR->NMemb*12;
	OR->NPos=BR->NMemb;
	OR->Message=NULL;
	int N1=0;
	int MinX=1000;
	int MaxX=-1000;
	int MinY=1000;
	int MaxY=-1000;
	int Quant=4*64*16;
	int px[200];
	int py[200];
	int Np=0;
	int Maxp=BR->NMemb;
	for(int r=0;r<10;r++){
		char* xi=Rarr[r].xi;
		char* yi=Rarr[r].yi;
		int N=Rarr[r].N;
		for(int j=0;j<N;j++){
			int dx=xi[j];
			int dy=yi[j];
			int xx=x+dx*Quant;
			int yy=y+dy*Quant;
			if(!MFIELDS[1].CheckBar((xx>>8)-7,(yy>>8)-7,14,14)){
				px[Np]=dx;
				py[Np]=dy;
				Np++;
				if(dx<MinX)MinX=dx;
				if(dx>MaxX)MaxX=dx;
				if(dy<MinY)MinY=dy;
				if(dy>MaxY)MaxY=dy;
				if(Np>=Maxp)goto pos1;
			};
		};
	};
pos1:
	SortClass SCL;
	//need to sort units
	UNISORT.CreateByY(BR->Memb,BR->NMemb,true);
    UNISORT.Sort();
    int NU=UNISORT.NUids;
	word Ids[200];
	word IdPos[200];
    UNISORT.Copy(Ids);
	word pxi[200];
	int pxx[200];
	int pxpos=0;
	for(int yp=MinY;yp<=MaxY;yp++){
		int npx=0;
		for(int j=0;j<Np;j++)if(py[j]==yp){
			pxi[npx]=j;
			pxx[npx]=px[j];
			npx++;
		};
		//sorting positions by x
		SCL.CheckSize(npx);
		memcpy(SCL.Uids,pxi,npx<<1);
		memcpy(SCL.Parms,pxx,npx<<2);
		SCL.NUids=npx;
		SCL.Sort();
		SCL.Copy(IdPos+pxpos);
		UNISORT.CreateByX(Ids+pxpos,npx,true);
		UNISORT.Sort();
		UNISORT.Copy(Ids+pxpos);
		pxpos+=npx;
	};
	//sending to positions
	for(int i=0;i<pxpos;i++){
		OneObject* OB=Group[Ids[i]];
		int ps=IdPos[i];
		int xx=x+px[ps]*Quant;
		int yy=y+py[ps]*Quant;
		OB->NewMonsterSmartSendTo(xx>>4,yy>>4,0,0,128+16,0);
		int i3=i+i+i;
		OR->Position[i3]=xx;
		OR->Position[i3+1]=yy;
		DWORD PP=Ids[i]+(DWORD(OB->Serial)<<16);
		OR->Position[i3+2]=PP;
	};
	OR->NPos=pxpos;
	OR->FirstTime=-1;
	OR->BLink=&LocalSendShipsLink;
};
byte FormSize[55*3]=
				{1,1,2,//1
				 2,1,3,//2
				 3,1,4,//3
				 4,1,5,//4
				 5,1,6,//5
				 3,2,7,//6
				 4,2,9,//7
				 4,2,9,//8
				 5,2,11,//9
				 5,2,11,//10
				 6,2,13,//11
				 6,2,13,//12
				 5,3,16,//13
				 5,3,16,//14
				 5,3,16,//15
				 6,3,19,//16
				 6,3,19,//17
				 6,3,19,//18
				 7,3,22,//19
				 7,3,22,//20
				 7,3,22,//21
				 8,3,25,//22
				 8,3,25,//23
				 8,3,25,//24
				 7,4,29,//25
				 7,4,29,//26
				 7,4,29,//27
				 7,4,29,//28
				 8,4,33,//29
				 8,4,33,//30
				 8,4,33,//31
				 8,4,33,//32
				 9,4,37,//33
				 9,4,37,//34
				 9,4,37,//35
				 9,4,37,//36
				 10,4,41,//37
				 10,4,41,//38
				 10,4,41,//39
				 10,4,41,//40
				 9,5,46,//41
				 9,5,46,//42
				 9,5,46,//43
				 9,5,46,//44
				 9,5,46,//45
				 10,5,51,//46
				 10,5,51,//47
				 10,5,51,//48
				 10,5,51,//49
				 10,5,51,//50
				 11,5,56,//51
				 11,5,56,//52
				 11,5,56,//53
				 11,5,56,//54
				 11,5,56};//55
int MTRLX;
int MTRLY;
byte* MTRPTR;
inline byte GetPX(int x,int y){
	if(x>=0&&y>=0&&x<MTRLX&&y<MTRLY)return MTRPTR[x+y*MTRLX];
	else return 0;
};
inline void SetPX(int x,int y,byte Val){
	if(x>=0&&y>=0&&x<MTRLX&&y<MTRLY)MTRPTR[x+y*MTRLX]=Val;
};
void LocalSendShips(Brigade* BR,int x,int y,byte Type){
	//getting the direction
	int xc=0;
	int yc=0;
	int Nc=0;
	int N=BR->NMemb;
	for(int i=0;i<N;i++){
		word MID=BR->Memb[i];
		OneObject* OB=Group[MID];
		Nc++;
		xc+=OB->RealX;
		yc+=OB->RealY;
	};
	if(Nc){
		xc/=Nc;
		yc/=Nc;
	}else return;
	int dx=x-xc;
	int dy=y-yc;
	int Nr=Norma(dx,dy)>>4;
	if(!Nr)return;
	dx=(dx*6*64)/Nr;
	dy=(dy*6*64)/Nr;
	int fdx=-dy;
	int fdy= dx;
	byte Matrix[8*32];
	MTRPTR=Matrix;
	byte NInLine[16];
	int NSLines;
	int NTot=Nc;
	int NP,Nx,Ny;

	do{
		if(Nc>55){
			LocalSendShipsOld(BR,x,y,Type);
			return;
		};
		Nx=FormSize[Nc+Nc+Nc-3];
		Ny=FormSize[Nc+Nc+Nc-2];
		MTRLX=Nx;
		MTRLY=Ny;
		int x0=x-((fdx*Nx-dx*Ny)>>1);
		int y0=y-((fdy*Nx-dy*Ny)>>1);
		int ofs=0;
		int ix,iy;
		for(ix=0;ix<Nx;ix++){
			for(iy=0;iy<Ny;iy++){
				if(MFIELDS[1].CheckBar(((x0+fdx*ix-dx*iy)>>8)-8,((y0+fdy*ix-dy*iy)>>8)-8,16,16))Matrix[ofs]=0;
				else Matrix[ofs]=1;
				ofs++;
			};
		};
		int cpx=Nx>>1;
		int cpy=Ny>>1;
		bool Change=false;
		NP=0;
		int r0=2;
		if(GetPX(cpx,cpy)){
			SetPX(cpx,cpy,2);
			NP++;
		};
		do{
			Change=false;
			int rrr=r0>>1;
			char* xi=Rarr[rrr].xi;
			char* yi=Rarr[rrr].yi;
			int Ni=Rarr[rrr].N;
			for(int j=0;j<Ni;j++){
				int xx=cpx+xi[j];
				int yy=cpy+yi[j];
				if(NP<NTot&&GetPX(xx,yy)==1&&(GetPX(xx+1,yy)==2||GetPX(xx-1,yy)==2||GetPX(xx,yy-1)==2||GetPX(xx,yy+1)==2)){
					Change=true;
					NP++;
					SetPX(xx,yy,2);
				};
			};
			r0++;
		}while(Change);
		if(NP!=NTot){
			Nc=FormSize[Nc+Nc+Nc-1];
		};
	}while(NP!=NTot);

	LSS_Order* OR=(LSS_Order*)BR->CreateBOrder(Type,sizeof(LSS_Order)-120+BR->NMemb*12);
	OR->Size=sizeof(LSS_Order)-120+BR->NMemb*12;
	OR->NPos=BR->NMemb;
	OR->Message=NULL;
	OR->FirstTime=-1;
	OR->BLink=&LocalSendShipsLink;

	UNISORT.CreateByLine(BR->Memb,BR->NMemb,fdx,fdy);
	UNISORT.Sort();
	word Ids[100];
	int upos=0;
	int x0=x-((fdx*Nx-dx*Ny)>>1);
	int y0=y-((fdy*Nx-dy*Ny)>>1);
	UNISORT.Copy(Ids);
	int ppos=0;
	for(int ix=0;ix<Nx;ix++){
		int NN=0;
		int pos0=ppos;
		for(int iy=0;iy<Ny;iy++){
			if(GetPX(ix,iy)==2){
				OR->Position[ppos]=x0+fdx*ix-dx*iy;
				OR->Position[ppos+1]=y0+fdy*ix-dy*iy;
				//assert(!MFIELDS[1].CheckBar((OR->Position[ppos]>>8)-8,(OR->Position[ppos+1]>>8)-8,16,16));
				NN++;
				ppos+=3;
			};
		};
		if(NN){
			UNISORT.CreateByLineForShips(Ids+upos,NN,-dx,-dy);
			UNISORT.Sort();
			UNISORT.Copy(Ids+upos);
			int j3=0;
			for(int j=0;j<NN;j++){
				OR->Position[pos0+j3+2]=Ids[upos+j];
				word MID=Ids[upos+j];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB)OB->NewMonsterSmartSendTo(OR->Position[pos0+j3]>>4,OR->Position[pos0+j3+1]>>4,0,0,128+16,0);
				};
				j3+=3;
			};
			upos+=NN;
		};
	};
};
class Ship_Battle:public BrigadeOrder{
public:
	byte ComType;
	word CurTop;
	int Params[4];
};
int WNAreas();
word GetNearestBrig(City* CT,word Top){
	int D=1000000;
	int BRB=-1;
	int TP1=Top*WNAreas();
	for(int i=0;i<128;i++){
		int N=CT->WaterBrigs[i];
		if(N!=0xFFFF){
			Brigade* BR=CT->Brigs+N;
			if(BR->Enabled){
				int TP=BR->LastTopology;
				if(TP!=0xFFFF){
					if(TP==Top){
						return N;
					}else{
						int R=GetLinksDist(TP1+TP,1);
						if(R<D){
							D=R;
							BRB=N;
						};
					};
				};
			};
		};
	};
	return BRB;
};
word GetNearestBrigToKillUnitsAndTowers(City* CT,word Top){
	int D=1000000;
	int BRB=-1;
	int TP1=Top*WNAreas();
	for(int i=0;i<128;i++){
		int N=CT->WaterBrigs[i];
		if(N!=0xFFFF){
			Brigade* BR=CT->Brigs+N;
			if(BR->Enabled&&BR->Direction==0){
				int TP=BR->LastTopology;
				if(TP!=0xFFFF){
					if(TP==Top){
						return N;
					}else{
						int R=GetLinksDist(TP1+TP,1);
						if(R<D){
							D=R;
							BRB=N;
						};
					};
				};
			};
		};
	};
	return BRB;
};
word GetNearestBrigToKillMines(City* CT,word Top){
	int D=1000000;
	int BRB=-1;
	int TP1=Top*WNAreas();
	for(int i=0;i<128;i++){
		int N=CT->WaterBrigs[i];
		if(N!=0xFFFF){
			Brigade* BR=CT->Brigs+N;
			if(BR->Enabled&&BR->Direction==1){
				int TP=BR->LastTopology;
				if(TP!=0xFFFF){
					if(TP==Top){
						return N;
					}else{
						int R=GetLinksDist(TP1+TP,1);
						if(R<D){
							D=R;
							BRB=N;
						};
					};
				};
			};
		};
	};
	return BRB;
};
word GetNearestBrigWithType(City* CT,word Top,byte Type){
	int D=1000000;
	int BRB=-1;
	int TP1=Top*WNAreas();
	for(int i=0;i<128;i++){
		int N=CT->WaterBrigs[i];
		if(N!=0xFFFF){
			Brigade* BR=CT->Brigs+N;
			if(BR->Enabled&&BR->Direction==Type){
				int TP=BR->LastTopology;
				if(TP!=0xFFFF){
					if(TP==Top){
						return N;
					}else{
						int R=GetLinksDist(TP1+TP,1);
						if(R<D){
							D=R;
							BRB=N;
						};
					};
				};
			};
		};
	};
	return BRB;
};
void MakeShipBattle(Brigade* BR);
void MakeShipBattleLink(Brigade* BR){
	if(PeaceTimeLeft)return;
	if(TryToBeatByShips(BR))return;
	Ship_Battle* OR=(Ship_Battle*)BR->BOrder;
	if(OR->Params[1]!=-1){
		OR->CurTop=OR->Params[1];
		OR->Params[1]=-1;
	};
	if(BR->NMemb==0){
		BR->DeleteBOrder();
		return;
	};
	int CurTop=OR->CurTop;
	int NextTop=-1;
	int FinalTop=-1;
	//test for connection
	City* CT=BR->CT;
	int NWA=GetNAreas(1);
	int XC=int(CT->CenterX)<<(7+4);
	int YC=int(CT->CenterY)<<(7+4);
	EnemyInfo* EIN=GNFO.EINF[BR->CT->NI];

	int MaxShip[2]={100,100};
	int SHIPFORCE=EIN->ShipsForce;
	if(SHIPFORCE<=15)MaxShip[0]=3;
	else if(SHIPFORCE<=25)MaxShip[0]=6;
	else if(SHIPFORCE<=40)MaxShip[0]=12;

	if(BR->NMemb>MaxShip[BR->Direction]){
		int ID1=CT->GetFreeBrigade();
		if(ID1!=-1){
			Brigade* BR1=CT->Brigs+ID1;
			CT->RegisterWaterBrigade(ID1);
			BR->RemoveObjects(BR->NMemb-MaxShip[BR->Direction],BR1);
			BR1->Enabled=true;
			MakeShipBattle(BR1);
			return;
		};
	}else{
		if(CurTop!=0xFFFF){
			BR->LastTopology=CurTop;
			for(int i=0;i<128;i++){
				int ID=CT->WaterBrigs[i];
				if(ID!=0xFFFF&&ID!=BR->ID){
					Brigade* BRF=CT->Brigs+ID;
					if(BR->Enabled&&BRF->Direction==BR->Direction&&BR->NMemb+BRF->NMemb<=MaxShip[BR->Direction]){
						int FTop=BRF->LastTopology;
						if(FTop!=0xFFFF){
							int D=GetLinksDist(FTop*NWA+CurTop,1);
							if(FTop==CurTop||D<24){
								if(BR->NMemb<BRF->NMemb){
									BR->RemoveObjects(BR->NMemb,BRF);
									BR->DeleteAll();
									BR->Enabled=false;
									CT->UnRegisterWaterBrigade(BR->ID);
								}else{
									BRF->RemoveObjects(BRF->NMemb,BR);
									BRF->DeleteAll();
									BRF->Enabled=false;
									CT->UnRegisterWaterBrigade(BRF->ID);
								};
								return;
							};
						};
					};
				};
			};
		};
	};
	int MyForce=0;
	for(int i=0;i<BR->NMemb;i++){
		word MID=BR->Memb[i];
		if(MID!=0xFFFF){
			word SN=BR->MembSN[i];
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SN)MyForce+=GetShipForce(OB->newMons->Usage);
		};
	};
	int MinF=MyForce>>2;
	if(MinF>15)MinF=15;
	//switch(OR->ComType){
	//case 0://razvedka
	//case 1:
		/*
		if(BR->NMemb>1000){
			OR->Params[0]=-1;
			OR->ComType=2;
		}else{
			if(OR->Params[0]==-1){
				//need to search final zone
				int NTop=[1].GetNAreas();
				int NAtt=0;
				int FinalTop=-1;
				while(NAtt<50&&FinalTop==-1){
					NAtt++;
					int Topz=(int(rando())*NTop)>>15;
					int D=[1].GetLinksDist(Topz*NTop+CurTop];
					if(D>20&&D<50)FinalTop=Topz;
				};
				if(FinalTop!=-1){
					OR->Params[0]=FinalTop;
				};
			}else{
				if(OR->Params[0]==CurTop){
					OR->Params[0]=-1;
				}else{
					FinalTop=OR->Params[0];
				};
			};
		};
		if(MyForce>1)OR->ComType=1;
		*/
		//search for the friend
		{
			int MinD=0xFFFF;
			for(int i=0;i<128;i++){
				int ID=CT->WaterBrigs[i];
				if(ID!=0xFFFF&&ID!=BR->ID){
					Brigade* BRF=CT->Brigs+ID;
					if(BR->Enabled&&BR->Direction==BRF->Direction&&BRF->NMemb>=BR->NMemb&&BRF->NMemb+BR->NMemb<=MaxShip[BR->Direction]){
						int FTop=BRF->LastTopology;
						if(FTop!=0xFFFF){
							int D=GetLinksDist(FTop*NWA+CurTop,1);
							if(D<MinD){
								int F=EIN->GetMaxForceOnTheWay(CurTop,FTop);
								if(MyForce>=(F-(F>>2))){
									MinD=D;
									FinalTop=FTop;
								};
							};
						};
					};
				};
			};
		};
	//	break;
	//case 1://go to strong enemy group
		{
			int FGrp=-1;
			int MinTopDst=10000;
			HShipsGroup* SGR=EIN->SGRP;
			for(int i=0;i<EIN->NSGRP;i++)if(SGR->CTop!=0xFFFF){
				int DT=0;
				if(SGR->CTop!=CurTop)DT=GetLinksDist(CurTop*WNAreas()+SGR->CTop,1);
				int R0=Norma(((SGR->xL+SGR->xR)>>1)-XC,((SGR->yL+SGR->yR)>>1)-YC);
				int F=EIN->GetMaxForceOnTheWay(CurTop,SGR->CTop);
				if(CurTop==SGR->CTop)DT=0;
				else DT=GetLinksDist(CurTop*WNAreas()+SGR->CTop,1);
				if(DT<MinTopDst){
					F-=F>>1;
					if(((R0<1024*7*16||MinTopDst>DT)&&MyForce>=F)||DT<20){	
						if(F<=8){
							int NB=GetNearestBrig(CT,SGR->CTop);
							if(NB==BR->ID){
								MinTopDst=DT;
								FGrp=i;
							};
						}else{
							MinTopDst=DT;
							FGrp=i;
						};
					};
				};
				SGR++;
			};
			if(FGrp!=-1){
				FinalTop=EIN->SGRP[FGrp].CTop;
			}else OR->ComType=0;
		};
		//attempt to erase human mines
		word MIN=SearchMineToDestroy(BR);
		if(MIN==0xFFFF&&FinalTop==-1&&BR->Direction==0){
			int BestTop=-1;
			int DT=10000;
			for(int i=0;i<EIN->NWTopNear;i++){
				int top=EIN->WTopNear[i];
				int D=GetLinksDist(top*WNAreas()+CurTop,1);
				if(top==CurTop)D=0;
				if(D<DT){
					int F=EIN->GetMaxForceOnTheWay(CurTop,top);
					if(MyForce>=(F-(F>>2))){
						if(F<=10){
							int N=GetNearestBrigToKillUnitsAndTowers(CT,top);
							if(N==BR->ID){
								DT=D;
								BestTop=top;
							};
						}else{
							DT=D;
							BestTop=top;
						};
					};
				};
			};
			if(BestTop!=-1)FinalTop=BestTop;
		};
		if(FinalTop==-1&&MIN==0xFFFF){
			int NTop=GetNAreas(1);
			int NAtt=0;
			while(NAtt<3&&FinalTop==-1){
				NAtt++;
				int Topz=(int(rando())*NTop)>>15;
				int N=GetNearestBrigWithType(CT,Topz,BR->Direction);
				if(N==BR->ID){
					FinalTop=Topz;
				};
			};
			if(FinalTop!=-1){
				OR->Params[0]=FinalTop;
			};			
		};
		if(FinalTop==CurTop)FinalTop=-1;				
	if(FinalTop!=-1){
		NextTop=GetMotionLinks(FinalTop+CurTop*GetNAreas(1),1);
		if(NextTop<0xFFFE){
			OR->Params[1]=NextTop;
			Area* TA=GetTopMap(NextTop,1);
			LocalSendShips(BR,int(TA->x)<<10,int(TA->y)<<10,1);
			BR->LastTopology=NextTop;
			return;
		};
	};
};
word DetermineWaterTopology(Brigade* BR){
	int NTops=0;
	//int NInTop[64];
	int Tops[128];
	int xc=0;
	int yc=0;
	int Nu=0;
	for(int i=0;i<BR->NMemb;i++){
		word MID=BR->Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo){
				int tx=OB->RealX>>10;
				int ty=OB->RealY>>10;
				xc+=tx;
				yc+=ty;
				Nu++;
				int Top=tx>=0&&ty>=0&&tx<TopLx&&ty<=TopLy?GetTopRef(tx+(ty<<TopSH),1):0xFFFF;
				int j;
				for(j=0;j<NTops&&Tops[j]!=Top;j++);
				if(j>=NTops&&Top<0xFFFE){
					Tops[NTops]=Top;
					//NInTop[NTops]=1;
					NTops++;
				};
			};
		};
	};
	if(Nu){
		xc/=Nu;
		yc/=Nu;
	};
	if(NTops){
		int BestTop=-1;
		int mind=10000;
		for(int p=0;p<NTops;p++){
			Area* TA=GetTopMap(Tops[p],1);
			int d=Norma(int(TA->x)-xc,int(TA->y)-yc);
			if(mind>d){
				mind=d;
				BestTop=Tops[p];
			};
		};
		return BestTop;
	};
	return 0xFFFF;
};
void MakeShipBattle(Brigade* BR){
	word Top=DetermineWaterTopology(BR);
	if(Top==0xFFFF)return;
	Ship_Battle* OR=(Ship_Battle*)BR->CreateBOrder(0,sizeof Ship_Battle);
	OR->BLink=&MakeShipBattleLink;
	OR->Message=NULL;
	OR->Size=sizeof Ship_Battle;
	OR->ComType=0;
	OR->Params[0]=-1;
	OR->Params[1]=-1;
	OR->CurTop=Top;
};
void City::RegisterWaterBrigade(int ID){
	for(int i=0;i<128;i++){
		if(WaterBrigs[i]==0xFFFF){
			WaterBrigs[i]=ID;
			return;
		};
	};
};
void City::UnRegisterWaterBrigade(int ID){
	for(int i=0;i<128;i++){
		if(WaterBrigs[i]==ID){
			WaterBrigs[i]=0xFFFF;
			return;
		};
	};

};
//=====================DESANT======================//
//=============1.Research cost spots===============//
CostPlace* COSTPL;
int NCost;
int MaxCost;
extern byte* WaterDeep;
extern int WLX;
void CreateCostPlaces(){
	if(!TopIslands){
		ResearchIslands();
	};
	NCost=0;
	MotionField* MFT=new MotionField;
	MFT->Allocate();
	MFT->ClearMaps();
	int maxx=(msx)-15;
	for(int ix=15;ix<maxx;ix++){
		for(int iy=15;iy<maxx;iy++){
			int ofs=ix+(iy<<(8+ADDSH));
			int Z0=GetHeight(ix*32,iy*32);
			int Z1=GetHeight(ix*32+32,iy*32);
			int Z2=GetHeight(ix*32,iy*32+32);
			int Z3=GetHeight(ix*32+32,iy*32+32);
			byte x=0;
			if(Z0<0)x|=1;else x|=2;
			if(Z1<0)x|=1;else x|=2;
			if(Z2<0)x|=1;else x|=2;
			if(Z3<0)x|=1;else x|=2;
			if(x==3){
				if(!MFT->CheckBar(ix+ix-10,iy+iy-10,20,20)){
					int DX=Z0+Z2-Z1-Z3;
					int DY=Z0+Z1-Z2-Z3;
					int N=Norma(DX,DY);
					if(N){
						DX=(DX*24)/N;
						DY=(DY*24)/N;
						if(!(MFIELDS[1].CheckBar(ix+ix+DX-6,iy+iy+DY-6,12,12)||
							CheckBar(ix+ix-DX-6,iy+iy-DY-6,12,12))){
							int R0=6;
							while(!CheckBar(ix+ix-DX-R0-1,iy+iy-DY-R0-1,R0+R0+2,R0+R0+2))R0++;
							MFT->BSetBar(ix+ix-10,iy+iy-10,20);
							if(NCost==MaxCost){
								MaxCost+=100;
								COSTPL=(CostPlace*)realloc(COSTPL,MaxCost*sizeof CostPlace);
							};
							COSTPL[NCost].xc=ix+ix-DX;
							COSTPL[NCost].yc=iy+iy-DY;
							COSTPL[NCost].xw=ix+ix+DX;
							COSTPL[NCost].yw=iy+iy+DY;
							COSTPL[NCost].R=R0;
							int Top=SafeTopRef((COSTPL[NCost].xc>>2),(COSTPL[NCost].yc>>2));
							if(Top<0xFFFE){
								COSTPL[NCost].Island=TopIslands[Top];
							};
							COSTPL[NCost].Access=true;
							NCost++;
						};
					};
				};
			};
		};
	};
	MFT->FreeAlloc();
	delete(MFT);
};
int GetWarInCell(int cell){
    cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(!NMon)return NULL;
	int ofs1=cell<<SHFCELL;
	word MID;
	int NU=0;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->NNUM==0&&!(OB->LockType||OB->newMons->Capture))NU++;
		};
	};
    return NU;
};
bool CheckUnitsNearPoint(int x,int y){
    int cell=((y>>3)<<VAL_SHFCX)+(x>>3);
    int rx1=2;
	int NU=0;
    int rx2=rx1+rx1+1;
    int stcell=cell-rx1-(rx1<<VAL_SHFCX);
    byte* bpt=NPresence+stcell;
    for(int nx=0;nx<rx2;nx++){
        for(int ny=0;ny<rx2;ny++){
            if(stcell>=0&&stcell<VAL_MAXCIOFS){
                if(bpt[0]&1){
					NU+=GetWarInCell(stcell);
				};
            };
            stcell++;
            bpt++;
        };
        stcell+=VAL_MAXCX-rx2;
        bpt+=VAL_MAXCX-rx2;
    };
	return NU>6;
};
int GetTrInCell(int cell){
    cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(!NMon)return NULL;
	int ofs1=cell<<SHFCELL;
	word MID;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&OB->Transport)return OB->Index;
		};
	};
    return 0xFFFF;
};
word CheckTransportNearPoint(int x,int y){
	int cell=((y>>3)<<VAL_SHFCX)+(x>>3);
    int rx1=2;
    int rx2=rx1+rx1+1;
    int stcell=cell-rx1-(rx1<<VAL_SHFCX);
    byte* bpt=NPresence+stcell;
    for(int nx=0;nx<rx2;nx++){
        for(int ny=0;ny<rx2;ny++){
            if(stcell>=0&&stcell<VAL_MAXCIOFS){
				word T=GetTrInCell(stcell);
				if(T)return T;
            };
            stcell++;
            bpt++;
        };
        stcell+=VAL_MAXCX-rx2;
        bpt+=VAL_MAXCX-rx2;
    };
	return 0xFFFF;

};
void ProcessCostPoints(){
	if(!NCost)return;
	int N=tmtmt%NCost;
	int R=COSTPL[N].R;
	COSTPL[N].Access=!CheckBar(COSTPL[N].xc-R,COSTPL[N].yc-R,R+R,R+R);
	if(CheckUnitsNearPoint(COSTPL[N].xc,COSTPL[N].yc))COSTPL[N].Access=false;
	COSTPL[N].Transport=CheckTransportNearPoint(COSTPL[N].xw,COSTPL[N].yw);
};
void ClearCostPlaces(){
	NCost=0;
};
extern int RealLx;
extern int RealLy;
void DrawMetka(int x,int y,byte c){
	x<<=4;
	y<<=4;
	int xx=x-(mapx<<5);
	int yy=(y>>1)-(mapy<<4)-GetHeight(x,y);
	if(xx>-20&&yy>-20&&xx<RealLx+20&&yy<RealLy+20){
		DrawLine(xx-20,yy-10,xx+20,yy+10,c);
		DrawLine(xx-20,yy+10,xx+20,yy-10,c);
	};
};
void DrawNum(int x,int y,byte v){
	x<<=4;
	y<<=4;
	int xx=x-(mapx<<5);
	int yy=(y>>1)-(mapy<<4)-GetHeight(x,y);
	if(xx>-20&&yy>-20&&xx<RealLx+20&&yy<RealLy+20){
		char cc[16];
		sprintf(cc,"%d",v);
		ShowString(xx-10,yy-5,cc,&WhiteFont);
	};
};
void ShowCostPlaces(){
	
};
//=============2.Desant============================//
class B_DiversionOrder:public BrigadeOrder{
public:
	word IDX[320];
	word USN[320];
	int NU;
	int MaxU;
	int Phase;
	int LoadX;
	int LoadY;
	int CostPointIndex;
	int StartTime;
	int LastPassangerTime;
};
int FindCostPoint(int x,int y,byte Isl){
	if(!NCost)return -1;
	int MinR=100000;
	int BestPL=-1;
	for(int i=0;i<NCost;i++){
		int R=Norma(x-COSTPL[i].xw,y-COSTPL[i].yw);
		if(R>20&&R<MinR&&COSTPL[i].Access&&COSTPL[i].Island==Isl&&COSTPL[i].Transport==0xFFFF){
			MinR=R;
			BestPL=i;
		};
	};
	return BestPL;
};
int FindCostPointEx(int x,int y,byte Mask){
	if(!NCost)return -1;
	int MinR=100000;
	int BestPL=-1;
	byte ISLSET[32];
	memset(ISLSET,0,32);
	for(int i=0;i<7;i++)if(!(NATIONS[i].NMask&Mask)){
		int isl=CITY[i].MyIsland;
		if(isl<255){
			ISLSET[isl>>3]|=1<<(isl&7);
		};
	};
	for(int i=0;i<NCost;i++){
		int R=Norma(x-COSTPL[i].xw,y-COSTPL[i].yw);
		if(R>20&&R<MinR&&COSTPL[i].Access&&COSTPL[i].Transport==0xFFFF){
			int ISL=COSTPL[i].Island;
			if(ISLSET[ISL>>3]&(1<<(ISL&7))){
				MinR=R;
				BestPL=i;
			};
		};
	};
	return BestPL;
};
void LeaveAll(OneObject* OB);
void ArmyMakeBattleLink(AI_Army* ARM);
int CleanGroup(UnitsGroup* GRP);
void MakeDiversionLink(Brigade* BR){
	if(PeaceTimeLeft)return;
	City* CT=BR->CT;
	if(CT->DesantStartGroup==0xFFFF)return;
	//CheckArmies(CT);
	B_DiversionOrder* DORD=(B_DiversionOrder*)BR->BOrder;
	int MaxInMy=50;
	if(IslPrs[CT->MyIsland]&1)MaxInMy=150;
	if(BR->NMemb){
		OneObject* TRANS=Group[BR->Memb[0]];
		OneObject* CurTrans=NULL;
		if(CT->TransportID!=0xFFFF){
			CurTrans=Group[CT->TransportID];
			if(!(CurTrans&&CurTrans->Serial==CT->TransportSN&&CurTrans->DstX>0)){
				CurTrans=NULL;
				CT->TransportID=0xFFFF;
				CT->TransportSN=0xFFFF;
			};
		};
		if(TRANS&&(!TRANS->Sdoxlo)&&TRANS->Serial==BR->MembSN[0]){
			if(TRANS->StandTime>200&&TRANS->DstX<=0){
				TRANS->Die();
				return;
			};
			if(TRANS->delay>100&&TRANS->delay<130){
				TRANS->Die();
				return;
			};
			if(TRANS->DstX>0&&DORD->Phase==0){
				if(DORD->NU<DORD->MaxU){
					if(CurTrans==TRANS){
						OneObject* UNI=NULL;
						UnitsGroup* UG=SCENINF.UGRP+CT->DesantStartGroup;
						CleanGroup(UG);
						if(UG->N){
							UNI=Group[UG->IDS[UG->N-1]];
							UG->N--;
						};
UNIFOUND:
						if(UNI){
							CT->UnRegisterNewUnit(UNI);
							UNI->NewMonsterSmartSendTo(TRANS->DstX>>4,TRANS->DstY>>4,0,0,128+16,0);
							DORD->IDX[DORD->NU]=UNI->Index;
							DORD->USN[DORD->NU]=UNI->Serial;
							DORD->NU++;
						};
					}else{
						if(CurTrans==NULL){
							CT->TransportID=TRANS->Index;
							CT->TransportSN=TRANS->Serial;
							CurTrans=TRANS;
						};
					};
				}else{
					if(CurTrans==TRANS){
						CT->TransportID=0xFFFF;
						CT->TransportSN=0xFFFF;
						CurTrans=NULL;
					};
					if(TRANS->NInside>=DORD->MaxU||tmtmt-DORD->LastPassangerTime>512){
						DORD->Phase=1;
						DORD->CostPointIndex=-1;
						TRANS->DstX=-1;
						TRANS->StandTime=0;
					};
				};
				//conrol of loading
				int x0=TRANS->DstX;
				int y0=TRANS->DstY;
				for(int q=0;q<DORD->NU;q++){
					OneObject* OB=Group[DORD->IDX[q]];
					bool remove=false;
					if(OB&&OB->Serial==DORD->USN[q]){
						if(!OB->Sdoxlo){
							if((!OB->LocalOrder)||(OB->LocalOrder&&OB->LocalOrder->DoLink==&TakeResLink)){
								if(Norma(OB->RealX-x0,OB->RealY-y0)<160*16){
									OB->GoToTransport(TRANS->Index,128+16);
								}else OB->NewMonsterSmartSendTo(x0>>4,y0>>4,0,0,128+16,0);
							};
							DORD->LastPassangerTime=tmtmt;
						};
						if(OB->Sdoxlo&&!OB->Hidden)remove=true;
					}else remove=true;
					if(remove){
						if(q<DORD->NU-1){
							memcpy(DORD->IDX+q,DORD->IDX+q+1,(DORD->NU-q-1)<<1);
							memcpy(DORD->USN+q,DORD->USN+q+1,(DORD->NU-q-1)<<1);
						};
						q--;
						DORD->NU--;
					};
				};
				if(TRANS->NInside){
					if(DORD->StartTime>tmtmt){
						DORD->StartTime=tmtmt;
					}else{
						if(tmtmt-DORD->LastPassangerTime>256){
							for(int q=0;q<DORD->NU;q++){
								OneObject* OB=Group[DORD->IDX[q]];
								bool remove=false;
								if(OB&&OB->Serial==DORD->USN[q]){
									if(!OB->Sdoxlo){
										CT->RegisterNewUnit(OB);
										if(q<DORD->NU-1){
											memcpy(DORD->IDX+q,DORD->IDX+q+1,(DORD->NU-q-1)<<1);
											memcpy(DORD->USN+q,DORD->USN+q+1,(DORD->NU-q-1)<<1);
										};
										q--;
										DORD->NU--;
									};
								};
							};
							DORD->MaxU=DORD->NU;
							DORD->StartTime=tmtmt;
						};
					};
				};
			}else{
				if(TRANS->DstX<=0&&DORD->Phase==0){
					//goto my cost
					if(DORD->CostPointIndex==-1){
						DORD->CostPointIndex=FindCostPoint(TRANS->RealX>>8,TRANS->RealY>>8,CT->MyIsland);
						if(DORD->CostPointIndex!=-1){
							CostPlace* CP=COSTPL+DORD->CostPointIndex;
							TRANS->NewMonsterSmartSendTo(CP->xw<<4,CP->yw<<4,0,0,128+16,0);
						};
					};
					if(DORD->CostPointIndex!=-1){
						CostPlace* CP=COSTPL+DORD->CostPointIndex;
						if((!CP->Access)||(CP->Transport!=0xFFFF&&CP->Transport!=TRANS->Index))DORD->CostPointIndex=-1;
						else{
							if(!TRANS->LocalOrder){
								if(Norma((TRANS->RealX>>8)-CP->xw,(TRANS->RealY>>8)-CP->yw)<18){
									TRANS->NewMonsterSmartSendTo(CP->xc<<4,CP->yc<<4,0,0,128+16,0);
								}else TRANS->NewMonsterSmartSendTo(CP->xw<<4,CP->yw<<4,0,0,128+16,0);
							};
							
						};
					};
				};
				if(DORD->Phase==1&&TRANS->DstX<=0){
					if(rando()<512)DORD->CostPointIndex=-1;
					if(DORD->CostPointIndex==-1){
						DORD->CostPointIndex=FindCostPointEx(TRANS->RealX>>8,TRANS->RealY>>8,1<<CT->NI);
						if(DORD->CostPointIndex!=-1){
							CostPlace* CP=COSTPL+DORD->CostPointIndex;
							TRANS->NewMonsterSmartSendTo(CP->xw<<4,CP->yw<<4,0,0,128+16,0);
						};
					};
					if(DORD->CostPointIndex!=-1){
						CostPlace* CP=COSTPL+DORD->CostPointIndex;
						if((!CP->Access)||(CP->Transport!=0xFFFF&&CP->Transport!=TRANS->Index))DORD->CostPointIndex=-1;
						else{
							int NN=Norma((TRANS->RealX>>8)-CP->xw,(TRANS->RealY>>8)-CP->yw);
							if(NN<40||!TRANS->LocalOrder){
								if(NN<40){
									if(!TRANS->LocalOrder)TRANS->NewMonsterSmartSendTo(CP->xc<<4,CP->yc<<4,0,0,128+16,0);
								}else TRANS->NewMonsterSmartSendTo(CP->xw<<4,CP->yw<<4,0,0,128+16,0);
							};
							
						};
					};
				};
				if(DORD->Phase==1&&TRANS->DstX>0){
					if(TRANS->NInside&&!TRANS->LocalOrder){
						LeaveAll(TRANS);
					};
					//attempt to register units
					int NUN=0;
					for(int i=0;i<DORD->NU;i++){
						OneObject* OB=Group[DORD->IDX[i]];
						if(OB&&OB->Serial==DORD->USN[i]&&OB->BrigadeID==0xFFFF){
							if(OB->Hidden||OB->UnlimitedMotion){
								NUN++;
							}else{
								if(CT->DesantDestGroup!=0xFFFF){
									UnitsGroup* UG=SCENINF.UGRP+CT->DesantDestGroup;
									UG->AddNewUnit(OB);
									if(i<DORD->NU-1){
										memcpy(DORD->IDX+i,DORD->IDX+i+1,(DORD->NU-i-1)<<1);
										memcpy(DORD->USN+i,DORD->USN+i+1,(DORD->NU-i-1)<<1);
									};
									DORD->NU--;
									i--;
								};
							};
						};
					};
					if(!(TRANS->NInside||NUN)){
						TRANS->DstX=-1;
						DORD->Phase=0;
						DORD->CostPointIndex=-1;
						DORD->NU=0;
						//CT->Brigs[TRANS->BrigadeID].DeleteAll();
						//CT->Brigs[TRANS->BrigadeID].Enabled=false;
						TRANS->delay=350;
						TRANS->StandTime=0;
					};
				};

			};
		}else{
			BR->NMemb=0;
		};
	}else{
		BR->DeleteAll();
		BR->Enabled=false;
	};
};
void MakeDiversion(Brigade* BR){
	BrigadeOrder_MakeDiversion* DOR = new BrigadeOrder_MakeDiversion();
	DOR->NU=0;
	DOR->MaxU=BR->CT->MaxInTransport;
	DOR->Phase=0;
	DOR->LoadX=-1;
	DOR->LoadY=-1;
	DOR->CostPointIndex=-1;
	DOR->LastPassangerTime=0;
	DOR->StartTime=10000000;
	BR->CreateNewBOrder(0,DOR);
	/*
	B_DiversionOrder* DOR=(B_DiversionOrder*)BR->CreateBOrder(0,sizeof B_DiversionOrder);
	BR->BOrder=DOR;
	DOR->Size=sizeof B_DiversionOrder;
	DOR->NU=0;
	DOR->MaxU=BR->CT->MaxInTransport;
	DOR->Phase=0;
	DOR->BLink=&MakeDiversionLink;
	DOR->LoadX=-1;
	DOR->LoadY=-1;
	DOR->CostPointIndex=-1;
	DOR->LastPassangerTime=0;
	DOR->StartTime=10000000;
	*/
};
//-----------------------------------NEW!!! Formations for AI!!!------------------------//
void CalculateFreeUnits(AI_Army* AIR);
void CheckCap(){
	return;
	Brigade* BR=CITY[1].Brigs;
	for(int i=0;i<MaxBrig;i++){
		int N=BR->NMemb;
		word* Mem=BR->Memb;
		word* Msn=BR->MembSN;
		for(int j=0;j<N;j++){
			word MID=Mem[j];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==Msn[j]){
//					assert(OB->BrigadeID==i);
				};
			};
		};
		BR++;
	};
};
void SearchArmyLink(OneObject* OBJ){
	word ArmID=OBJ->LocalOrder->info.BuildObj.ObjIndex;
	bool OFC=OBJ->Ref.General->OFCR!=NULL;
	if(ArmID==0xFFFF){
		int Aid=-1;
		//need to find nearest appropriate army

		int MyTop=GetTopology(OBJ->RealX>>4,OBJ->RealY>>4);
		if(MyTop>=0xFFFE)return;
		int MMTop=MyTop*GetNAreas();
		City* CT=CITY+OBJ->NNUM;
		int MinR=0xFFFE;
		int id=-1;
		for(int i=0;i<MaxArm;i++){
			AI_Army* ARM=CT->ARMS+i;
			if(ARM->Enabled&&ARM->TopPos<0xFFFE){
				if((OFC&&ARM->NFreeUnits/36>ARM->NCom&&ARM->NCom<8)||((!OFC)&&ARM->NFreeUnits/36>ARM->NBar&&ARM->NBar<8)){
					int topa=ARM->TopPos;
					int dr=0;
					if(topa!=MyTop)dr=GetLinksDist(topa+MMTop);
					if(dr<MinR){
						MinR=dr;
						Aid=i;
					};
				};
			};
		};
		if(Aid!=-1){
			AI_Army* ARM=CT->ARMS+Aid;
			if(OFC){
				ARM->ComID[ARM->NCom]=OBJ->Index;
				ARM->ComSN[ARM->NCom]=OBJ->Serial;
				ARM->NCom++;
			}else{
				ARM->BarID[ARM->NBar]=OBJ->Index;
				ARM->BarSN[ARM->NBar]=OBJ->Serial;
				ARM->NBar++;
			};
			OBJ->LocalOrder->info.BuildObj.ObjIndex=Aid;
		};
		return;
	}else{
		AI_Army* ARM=CITY[OBJ->NNUM].ARMS+ArmID;
		if(OFC){
			bool pr=1;
			for(int i=0;i<ARM->NCom&&pr;i++)if(ARM->ComID[i]==OBJ->Index)pr=0;
			if(pr){
				OBJ->LocalOrder->info.BuildObj.ObjIndex=0xFFFF;
				return;
			};
		}else{
			bool pr=1;
			for(int i=0;i<ARM->NBar&&pr;i++)if(ARM->BarID[i]==OBJ->Index)pr=0;
			if(pr){
				OBJ->LocalOrder->info.BuildObj.ObjIndex=0xFFFF;
				return;
			};
		};
		if( (OFC&&ARM->NFreeUnits/36>ARM->NCom)||((!OFC)&&ARM->NFreeUnits/36>ARM->NBar)){
			word* CID;
			word* CSN;
			int CN;
			if(OFC){
				CID=ARM->ComID;
				CSN=ARM->ComSN;
				CN=ARM->NCom;
			}else{
				CID=ARM->BarID;
				CSN=ARM->BarSN;
				CN=ARM->NBar;
			};
			//search farest commander/bar..
			int maxr=-1;
			int comid=0xFFFF;
			int cps=0;
			int top0=ARM->TopPos;
			if(top0>=0xFFFE)return;
			int NA=GetNAreas();
			for(int i=0;i<CN;i++){
				OneObject* OB=Group[CID[i]];
				if(OB&&OB->Serial==CSN[i]&&!OB->Sdoxlo){
					int top1=GetTopology(OB->RealX>>4,OB->RealY>>4);
					int dd=0xFFFF;
					if(top1<0xFFFE)dd=top1==top0?0:GetLinksDist(top1*NA+top0);
					if(dd>maxr){
						maxr=dd;
						comid=OB->Index;
						cps=i;
					};
				};
			};
			if(comid==OBJ->Index){
				//delete ofcr from army!
				CSN[cps]++;
				CalculateFreeUnits(ARM);
				OBJ->LocalOrder->info.BuildObj.ObjIndex=0xFFFF;
				return;
			};
		}else{ 
			
		};
		//we need to go to the army
		int top0=ARM->TopPos;
		if(top0>=0xFFFE)return;
		int top1=GetTopology(OBJ->RealX>>4,OBJ->RealY>>4);
		if(top1>=0xFFFE)return;
		if(top1!=top0){
			top1=GetMotionLinks(top0+top1*GetNAreas());
			Area* AR=GetTopMap(top1);
			int x1=int(AR->x)<<6;
			int y1=int(AR->y)<<6;
			int x0=OBJ->RealX>>4;
			int y0=OBJ->RealY>>4;
			OBJ->NewState=0;
			OBJ->GroundState=0;
			addrand(OBJ->GroundState+1);
			if(Norma(x0-x1,y0-y1)>250){
				OBJ->CreatePath(x1>>4,y1>>4);
			}else{
				OBJ->DestX=0;
				OBJ->DestY=0;
			};
		}else{
			//need create formation now!
			if(OFC){
				//need to check barabanschiks...
				word Bar=0xFFFF;
				int bpos=-1;
				for(int i=0;i<ARM->NBar;i++){
					OneObject* BOB=Group[ARM->BarID[i]];
					if(BOB&&BOB->Serial==ARM->BarSN[i]&&!BOB->Sdoxlo){
						int top1=GetTopology(BOB->RealX>>4,BOB->RealY>>4);
						if(top1==top0){
							bpos=i;
							Bar=BOB->Index;
						};
					};
				};
				if(Bar!=0xFFFF){
					word FormUID=NATIONS[ARM->CT->NI].FormUnitID;
					//now we are ready to create formation!
					//1.Search the biggest brigade
					int BID=-1;
					int MaxU=0;
					int N=ARM->NExBrigs;
					for(int i=0;i<N;i++){
						Brigade* BR=ARM->ExBrigs[i].Brig;
						if(!BR->WarType){
							int Nu=BR->NMemb;
							word* mem=BR->Memb;
							word* msn=BR->MembSN;
							int NFU=0;
							for(int j=0;j<Nu;j++){
								word mid=mem[j];
								if(mid!=0xFFFF){
									OneObject* OB=Group[mid];
									if(OB&&OB->Serial==msn[j]&&(!OB->Sdoxlo)&&OB->NIndex==FormUID)NFU++;
								};
							};
							if(NFU>MaxU){
								MaxU=NFU;
								BID=i;
							};
						};
					};
					if(BID!=-1){
						Brigade* FBR=ARM->ExBrigs[BID].Brig;
						FBR->WarType=0xFF;
						//let us exclude other members
						int N=FBR->NMemb;
						word* mem=FBR->Memb;
						word* msn=FBR->MembSN;
						word TrueMem[38];
						int  NTrue=0;
						City* CT=ARM->CT;
						int id=CT->GetFreeBrigade();
						if(id==-1){
							FBR->WarType=0;
							return;
						};
						Brigade* OTB=CT->Brigs+id;
						OTB->Enabled=1;
						for(int i=0;i<N;i++){
							word mid=mem[i];
							if(mid!=0xFFFF){
								OneObject* OB=Group[mid];
								if(OB&&(OB->Serial==msn[i])&&(!OB->Sdoxlo)){
									if(OB->NIndex!=FormUID||NTrue>=36){
										FBR->RemoveOne(i,OTB);
										i--;
										N--;
									}else{
										TrueMem[NTrue+2]=OB->Index;
										NTrue++;
									};
								};
							};
						};
						if(OTB->NMemb){
							ARM->AddBrigade(OTB);
						};
						if(NTrue<36){
							//not enough units to create formaton.Need to collect
							int Need=36-NTrue;
							int NB=ARM->NExBrigs;
							for(int i=0;i<NB&&Need;i++){
								Brigade* MBR=ARM->ExBrigs[i].Brig;
								if(!MBR->WarType){
									word* mem=MBR->Memb;
									word* msn=MBR->MembSN;
									int nm=MBR->NMemb;
									for(int j=0;j<nm&&Need;j++){
										word mid=mem[j];
										if(mid!=0xFFFF){
											OneObject* OB=Group[mid];
											if(OB&&(!OB->Sdoxlo)&&OB->Serial==msn[j]&&OB->NIndex==FormUID){
												MBR->RemoveOne(j,OTB);
												Need--;
												j--;
											};
										};
									};
								};
							};
							OTB->RemoveObjects(OTB->NMemb,FBR);
						};
						NTrue=0;
						for(int i=0;i<FBR->NMemb;i++){
							word mid=FBR->Memb[i];
							if(mid!=0xFFFF){
								OneObject* OB=Group[mid];
								if(OB&&(!OB->Sdoxlo)&&OB->Serial==FBR->MembSN[i]){
									TrueMem[NTrue+2]=OB->Index;
									NTrue++;
								};
							};
						};
						TrueMem[0]=OBJ->Index;
						TrueMem[1]=Bar;
						NTrue+=2;
						FBR->Memb=(word*)realloc(FBR->Memb,38*2);
						FBR->MembSN=(word*)realloc(FBR->MembSN,38*2);
						memset(FBR->Memb,0xFF,38*2);
						memset(FBR->MembSN,0xFF,38*2);
						FBR->NMemb=38;
						FBR->MaxMemb=38;
						int cidx=-1;
						for(int p=0;p<NEOrders&&cidx==-1;p++){
							if(!strcmp(ElementaryOrders[p].ID,"#SQUARE36"))cidx=p;
						};
						if(cidx==-1){
							MessageBox(NULL,"Could not find fomation: #SQUARE36","ERROR!",0);
							//assert(cidx);
						};
						OrderDescription* ODS=ElementaryOrders+cidx;
						int AddShield=ODS->AddShield2;
						int AddDamage=ODS->AddDamage2;
						if(FBR->Memb[2]!=0xFFFF){
							AddShield+=ODS->FAddShield;
							AddDamage+=ODS->FAddDamage;
						};
						FBR->AddDamage=AddDamage;
						FBR->AddShield=AddShield;
						int ncr=0;
						int xs=0;
						int ys=0;
						for(int i=0;i<NTrue;i++){
							OneObject* OB=Group[TrueMem[i]];
							FBR->Memb[i]=TrueMem[i];
							FBR->MembSN[i]=OB->Serial;
							OB->BrigadeID=FBR->ID;
							OB->BrIndex=i;
							if(EngSettings.MoraleType!=1){
								OB->AddShield=AddShield;
								OB->AddDamage=AddDamage;
							}
							OB->InArmy=1;
							OB->Zombi=1;
							OB->DoNotCall=1;
							OB->NoBuilder=1;
							if(i>1){
								xs+=OB->RealX>>4;
								ys+=OB->RealY>>4;
								ncr++;
							};
						};
						xs/=ncr;
						ys/=ncr;
						OBJ->ClearOrders();
						Group[Bar]->ClearOrders();
						FBR->NMemb=38;
						FBR->ArmyID=ArmID;
						FBR->PosCreated=false;
						FBR->WarType=cidx+1;
						FBR->MembID=FormUID;
						FBR->CreateOrderedPositions(xs<<4,ys<<4,0);
						addrand(128+16);
						FBR->KeepPositions(0,128+16);
						OTB->DeleteAll();
						OTB->Enabled=false;
						for(int i=0;i<ARM->NCom;i++){
							if(ARM->ComID[i]==OBJ->Index)ARM->ComSN[i]++;
						};
						for(int i=0;i<ARM->NBar;i++){
							if(ARM->BarID[i]==Bar)ARM->BarSN[i]++;
						};
						CalculateFreeUnits(ARM);
						return;
					};
				};
			};
			Area* AR=GetTopMap(top1);
			int x1=int(AR->x)<<6;
			int y1=int(AR->y)<<6;
			int x0=OBJ->RealX>>4;
			int y0=OBJ->RealY>>4;
			OBJ->NewState=0;
			OBJ->GroundState=0;
			addrand(OBJ->GroundState+1);
			if(Norma(x0-x1,y0-y1)>250){
				OBJ->CreatePath(x1>>4,y1>>4);
			}else{
				OBJ->DestX=-1;
				OBJ->DestY=-1;
			};
		};
	};
};
void SearchArmy(OneObject* OB){
	Order1* OR1=OB->CreateOrder(0);
	if(OR1){
		OR1->info.BuildObj.ObjIndex=0xFFFF;
		OR1->DoLink=&SearchArmyLink;
		if(OB->newMons->Officer){
			OB->PrioryLevel=0;
			addrand(OB->NoSearchVictim);
			OB->NoSearchVictim=1;
			addrand(OB->NoSearchVictim);
		}else{
			OB->PrioryLevel=16;
			addrand(OB->NoSearchVictim);
			OB->NoSearchVictim=1;
			addrand(OB->NoSearchVictim);
		};
		OR1->PrioryLevel=0;
	};
};
void CalculateFreeUnits(AI_Army* AIR){
	AIR->NFreeUnits=0;
	word FormUID=NATIONS[AIR->CT->NI].FormUnitID;
	for(int i=0;i<AIR->NExBrigs;i++){
		Brigade* BR=AIR->ExBrigs[i].Brig;
		if((!BR->WarType)&&BR->NMemb){
			word* Mem=BR->Memb;
			word* SN=BR->MembSN;
			int N=BR->NMemb;
			for(int i=0;i<N;i++){
				word MID=Mem[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==SN[i]&&!OB->Sdoxlo){
						if(OB->NIndex==FormUID)AIR->NFreeUnits++;
					};
				};
			};
		};
	};
	//handle officers
	int id=AIR->ArmyID;
	for(int i=0;i<AIR->NCom;i++){
		OneObject* OB=Group[AIR->ComID[i]];
		bool remove=false;
		if(OB){
			if(!(OB->Serial==AIR->ComSN[i]&&!OB->Sdoxlo))remove=1;
			else{
				Order1* OR1=OB->LocalOrder;
				remove=true;
				while(OR1){
					if(OR1->DoLink==&SearchArmyLink){
						if(OR1->info.BuildObj.ObjIndex==id)remove=false;
						OR1=NULL;
					}else OR1=OR1->NextOrder;
				};
			};
		};
		if(remove){
			if(i<AIR->NCom-1){
				word NCp=(AIR->NCom-i-1)<<1;
				memcpy(AIR->ComID+i,AIR->ComID+i+1,NCp);
				memcpy(AIR->ComSN+i,AIR->ComSN+i+1,NCp);
			};
			i--;
			AIR->NCom--;
		};
	};
	for(int i=0;i<AIR->NBar;i++){
		OneObject* OB=Group[AIR->BarID[i]];
		bool remove=false;
		if(OB){
			if(!(OB->Serial==AIR->BarSN[i]&&!OB->Sdoxlo))remove=1;
			else{
				Order1* OR1=OB->LocalOrder;
				remove=true;
				while(OR1){
					if(OR1->DoLink==&SearchArmyLink){
						if(OR1->info.BuildObj.ObjIndex==id)remove=false;
						OR1=NULL;
					}else OR1=OR1->NextOrder;
				};
			};
		};
		if(remove){
			if(i<AIR->NBar-1){
				word NCp=(AIR->NBar-i-1)<<1;
				memcpy(AIR->BarID+i,AIR->BarID+i+1,NCp);
				memcpy(AIR->BarSN+i,AIR->BarSN+i+1,NCp);
			};
			i--;
			AIR->NBar--;
		};
	};
};
void NewAttackPointLink(OneObject* OBJ);
bool CheckIfPossibleToBreakOrder(OneObject* OB){
	if(OB&&OB->LocalOrder&&(OB->LocalOrder->DoLink==&NewAttackPointLink)){ return false;}
	return true;
}

byte Brigade::GetFormIndex(){
	if(WarType){
		OrderDescription* ODE=ElementaryOrders+WarType-1;
		if(ODE->GroupID!=0xFF){
			SingleGroup* FGD=FormGrp.Grp+ODE->GroupID;
			for(int i=0;i<FGD->NCommon;i++)if(FGD->IDX[i]<FormGrp.NGrp){
				SingleGroup* sg=FormGrp.Grp+FGD->IDX[i];
				for(int j=0;j<sg->NForms;j++)
					if(WarType-1==sg->Forms[j]){
						return FGD->IDX[i];//i;
					}
			}
		}
	}
	return -1;
}
void Brigade::GetBrigadeDamage(int AttType,int& Damage,int& ExDamage,OneObject* Killer,OneObject* Victim){
	ExDamage=0;
	Damage=0;
	if(!WarType)return;
    GeneralObject* GO;
	if(Killer)GO=Killer->Ref.General;
	else GO=NATIONS[CT->NI].Mon[MembID];
	NewMonster* SNM=GO->newMons;

	Damage=GO->MoreCharacter->MaxDamage[AttType];

	if(AttType==0){
		ExDamage+=GetAbilityValue(32);//Arm damage
	}else if(AttType==0){
		ExDamage+=GetAbilityValue(33);//Rifle damage
	}
	if(SNM->SkillDamageFormationBonus){
		byte msk=SNM->SkillDamageMask;
		if(msk&(1<<AttType)){					
			int SC=SNM->SkillDamageFormationBonusStep;
			int V;
			if(SC)V=(GetBrigExp()/SC)*SC;
			else V=GetBrigExp();
			int ddm=(V*int(SNM->SkillDamageFormationBonus))/100;
			if(ddm>Damage*19)ddm=Damage*19;
			if(ddm>EngSettings.MaxBrigAddDamage)ddm=EngSettings.MaxBrigAddDamage;
			ExDamage+=ddm;
		};
	};
	if(SNM->VeteranKills&&SNM->ExpertKills){
		int akills=GetBrigExp()/(NMemb-NBPERSONAL);
		if(akills>=SNM->ExpertKills){
			ExDamage+=SNM->ExpertExtraDamage;
		}else
			if(akills>=SNM->VeteranKills){
				ExDamage+=SNM->VeteranExtraDamage;
			}
	}
	if(EngSettings.MoraleType==1){//Rome
		int M=Morale/10000;
		int NU=NMemb-NBPERSONAL;
		if(M>200)M=200;
		if(NU>0){
			if(M>NU+NU)M=NU+NU;
			int dd=(AddDamage*M+NU/2)/NU;
			if(ElementaryOrders[WarType-1].DirectionalBonus&&Killer&&Victim){
				char dir=GetDir(Victim->RealX-Killer->RealX,Victim->RealY-Killer->RealY)-Direction;
				int addr=abs(dir);
				if(addr>=70)dd=0;
			};
			ExDamage+=dd;
		}				
	}
	if(EngSettings.MoraleType==0){//Cossacks2
		int adm=AddDamage;		
		if(InStandGround&&ElementaryOrders[WarType-1].DirectionalBonus&&Killer&&Victim){
			char dir=GetDir(Victim->RealX-Killer->RealX,Victim->RealY-Killer->RealY)-Direction;
			int addr=abs(dir);
			if(addr>40&&addr<70)adm>>=1;
			if(addr>=70)adm=0;			
		};
		ExDamage+=adm;
	}
	int ads=0,adm=0;
	GetAbilityChange(adm,ads);
	ExDamage+=adm;
}
void Brigade::GetBrigadeProtection (int AttType,int& Shield,int& ExShield,OneObject* Killer,OneObject* Victim){
	ExShield=0;
	Shield=0;
	if(!WarType)return;
	GeneralObject* GO;
	if(Killer)GO=Killer->Ref.General;
	else GO=NATIONS[CT->NI].Mon[MembID];
	NewMonster* SNM=GO->newMons;

	Shield=GO->MoreCharacter->Protection[AttType]+GO->MoreCharacter->Shield;

	ExShield+=GetAbilityValue(48);//shield bonus			
	if(SNM->VeteranKills&&SNM->ExpertKills){				
		int akills=GetBrigExp()/(NMemb-NBPERSONAL);
		if(akills>=SNM->ExpertKills){
			ExShield+=SNM->ExpertExtraShield;
		}else{
			if(akills>=SNM->VeteranKills){
				ExShield+=SNM->VeteranExtraShield;
			}
		}
	}
	if(EngSettings.MoraleType==1){//Rome
		int M=Morale/10000;
		int NU=NMemb-NBPERSONAL;
		if(NU>0){
			if(M>NU+NU)M=NU+NU;
			int ds=(AddShield*M+NU/2)/NU;
			if(ElementaryOrders[WarType-1].DirectionalBonus&&Killer&&Victim){
				char dir=GetDir(Victim->RealX-Killer->RealX,Victim->RealY-Killer->RealY)-Direction;
				int addr=abs(dir);
				if(addr>=70)ds=0;
			};
			ExShield+=ds;
		}
	}
	if(EngSettings.MoraleType==0){//Cossacks2		
		int adm=Victim?Victim->AddShield:0;
		if(InStandGround&&ElementaryOrders[WarType-1].DirectionalBonus&&Killer&&Victim){
			char dir=GetDir(Killer->RealX-Victim->RealX,Killer->RealY-Victim->RealY)-Direction;
			int addr=abs(dir);
			if(addr>40&&addr<70)adm>>=1;
			if(addr>=70)adm=0;
		};
		ExShield+=adm;
	}
	int ads=0,adm=0;
	GetAbilityChange(adm,ads);
	ExShield+=ads;
}
int Brigade::GetSkillStatus(float& avkills){
	GeneralObject* GO=NATIONS[CT->NI].Mon[MembID];
	avkills=0;
	NewMonster* SNM=GO->newMons;
	if(SNM->VeteranKills&&SNM->ExpertKills){
		avkills=float(GetBrigExp())/(NMemb-NBPERSONAL);
		int akills=GetBrigExp()/(NMemb-NBPERSONAL);
		if(akills>=SNM->ExpertKills){
			return 2;
		}else{
			if(akills>=SNM->VeteranKills){
				return 1;
			}
		}
	}
	return 0;
}
void Brigade::IncBrigExperience(int Value){
    NKills+=Value*ExpGrowSpeed/100;
}
int Brigade::GetBrigExp(){
	return NKills/100;
}
