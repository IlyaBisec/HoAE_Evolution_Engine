
#include "CommonDip.h"

//==================================================================================================================//
int Norma(int,int);
//==================================================================================================================//
CityzenType::CityzenType()
{
	NDSNameID=-1;
	LifeTime=0;
	
	Behaviour.Clear();
	Behaviour.Add(0);// 0 Walk
	Behaviour.Add(0);// 1 Run
	Behaviour.Add(0);// 2 Meeting
	Behaviour.Add(0);// 3 GoHome

	MaxWalkDist=900;
	MinWalkDist=100;
	MaxActionTime=30*25*256;
}
CityzenType::~CityzenType()
{}
void CityzenType::Load(xmlQuote* x)
{
	if(x)
	{
		if(!strcmp(x->GetQuoteName(),"A"))
		{
			xmlQuote* t=x->GetSubQuote("Name");
			if(t)Name.Add((char*)t->Get_string());
			t=x->GetSubQuote("NDSName");
			if(t)
			{
				DString ds;
				ds.Add((char*)t->Get_string());
				RegisterUnitType(&UnitType,ds.pchar());
				NDSNameID=UnitType.Index;
			}

			t=x->GetSubQuote("MaxWalkDist");
			if(t)MaxWalkDist=t->Get_int();
			t=x->GetSubQuote("MinWalkDist");
			if(t)MinWalkDist=t->Get_int();
			t=x->GetSubQuote("MaxActionTime");
			if(t)MaxActionTime=t->Get_int()*25*256;

			t=x->GetSubQuote("Walk");
			if(t)Behaviour[0]=t->Get_int();
			t=x->GetSubQuote("Run");
			if(t)Behaviour[1]=t->Get_int();
			t=x->GetSubQuote("Meeting");
			if(t)Behaviour[2]=t->Get_int();

			t=x->GetSubQuote("LifeTime");
			if(t)LifeTime=t->Get_int()*25*256;
			t=x->GetSubQuote("ComeIn");
			if(t)
			{
				int n=t->GetNSubQuotes();
				for(int i=0;i<n;i++)
				{
					xmlQuote* tt=t->GetSubQuote(i);
					if(tt&&(!strcmp(tt->GetQuoteName(),"A")))
					{
						SComeIn tm;
						tm.BT=NULL;
						xmlQuote* ttt=tt->GetSubQuote("Name");
						if(ttt)
							tm.BildTypeName=ttt->Get_string();
						ttt=tt->GetSubQuote("Probability");
						if(ttt)
							tm.Probability=ttt->Get_int();
						ComeIn.Add(tm);
					}
				}
			}
		}
	}
}
//==================================================================================================================//
BildType::BildType()
{
	NDSNameID=-1;
}
BildType::~BildType()
{}
void BildType::Load(xmlQuote *x)
{
	if(x)
	{
		if(!strcmp(x->GetQuoteName(),"A"))
		{
			xmlQuote* t=x->GetSubQuote("Name");
			if(t)Name.Add((char*)t->Get_string());
			t=x->GetSubQuote("Born");
			if(t)
			{
				int n=t->GetNSubQuotes();
				for(int i=0;i<n;i++)
				{
					xmlQuote* tt=t->GetSubQuote(i);
					if(tt&&(!strcmp(tt->GetQuoteName(),"A")))
					{
						SBorn tm;
						tm.CT=NULL;
						xmlQuote* ttt=tt->GetSubQuote("MaxTime");
						if(ttt)
							tm.MaxTime=ttt->Get_int()*25*256;
						ttt=tt->GetSubQuote("Name");
						if(ttt)
							tm.CityzenTypeName=ttt->Get_string();
						CanBorn.Add(tm);
					}
				}
			}
		}
	}
}
//==================================================================================================================//
Cityzen::Cityzen()
{
	CT=NULL;
	LifeTime=GetAnimTime();//GetGlobalTime();
	Index=0xFFFF;
	OB=NULL;
	RegisterDynGroup(&Dyn);
	State=1;
	ProcesedTime=0;
	RandX=-1;
	RandY=-1;
	ActionTime=1;
	FreeForTalk=false;
}
Cityzen::~Cityzen()
{
	DeleteDynGroup(&Dyn);
}
void Cityzen::Process()
{
	int time=GetAnimTime();//GetGlobalTime();
	if(State&&(ProcesedTime+ActionTime)<time)
	{
		if(State==1&&GetTotalAmount0(&Dyn)==1)
		{
			OneUnit U;
			if(GetUnitInfo(&Dyn,0,&U))
			{
				Index=U.Index;
				OB=GetOBJ(Index);
				OB->NMask=255;
				OB->NotSelectable=true;
				if(!OB->UnlimitedMotion)
				{
					State=2;
					bool flag=true;
					while(flag)
					{
						int ang=GetRND(256);
						int ar=GetRND(CT->MaxWalkDist-CT->MinWalkDist)+CT->MinWalkDist;
						RandX=(OB->RealX>>4)+((ar*GetTSin((byte)ang))>>8);
						RandY=(OB->RealY>>4)+((ar*GetTCos((byte)ang))>>8);
						flag=CheckLandLocking(RandX,RandY,20,20);
					} 
				}
			}
		}
		if(State==2)
		{
			if((time-LifeTime)>CT->LifeTime)
			{
				ExecuteActions(3);
			}
			else
			{
				ExecuteActions(GetRandActions());
			}
		}
		if(State==3&&OB->Sdoxlo&&OB->Hidden)
		{
			State=0;
			DieUnit(Index);
			OB->Sdoxlo=30000;
		}
		ProcesedTime=time;
	}
}
void Cityzen::ExecuteActions(int Comm)
{
	FreeForTalk=false;
	switch (Comm)
	{
	case 0://Wolk
		if(OB)
		{
			if(abs((OB->RealX>>4)-RandX)<10&&abs((OB->RealY>>4)-RandY)<10)
			{
				OB->Speed=64-GetRND(32);
				OBJ_SendTo(Index,RandX,RandY,128,0);
				ActionTime=GetRND(CT->MaxActionTime);
			}
		}
		break;
	case 1://Run
		if(OB)
		{
			OB->Speed=160-GetRND(32);
			OBJ_SendTo(Index,RandX,RandY,128,0);
			ActionTime=GetRND(CT->MaxActionTime);
		}
		break;
	case 2://Meeting
		if(OB)
		{
			//OB->Speed=128-GetRND(32);
			//OBJ_SendTo(Index,(OB->RealX>>4)-500+GetRND(1000),(OB->RealY>>4)-500+GetRND(1000),128,0);
			OBJ_SendTo(Index,OB->RealX>>4,OB->RealY>>4,128,0); 
			ActionTime=GetRND(CT->MaxActionTime);
			SetMeeting();
		}
		break;
	case 3://GoHome
		int Ib=GetSweetHomeIndex();
		if(Ib>-1)
		{
			SGP_ComeIntoBuilding(&Dyn,0,Ib);
			ActionTime=20*25*256;
			State=3;
		}
		else
		{
			int ff=34;
		}
		break;
	}

}
int Cityzen::GetSweetHomeIndex()
{
	int n=CityPopulation::CBild.GetAmount();
	int rez=-1;
	int wid=0;
	for(int i=0;i<n;i++)
	{
		OneObject* B=CityPopulation::CBild[i]->OB;
		if(B)
		{
			if((OB->NMask&B->NMask)&&B->Life==B->MaxLife)
			{
				int m=CT->ComeIn.GetAmount();
				for(int j=0;j<m;j++)
				{
					if(CT->ComeIn[j].BT) 
					if(!strcmp(CityPopulation::CBild[i]->BT->Name.pchar(),CT->ComeIn[j].BT->Name.pchar()))
					{
						int d=Norma((OB->RealX>>4)-(B->RealX>>4),(OB->RealY>>4)-(B->RealY>>4));
						if(d<2000&&GetRND(2))
						{
							int w=CT->ComeIn[j].Probability*(2000-d)/2000;
							if(w>wid)
							{
								wid=w;
								rez=B->Index;
							}
						}
					}
				}
			}
		}
	}
	return rez;
}
int Cityzen::GetRandActions()
{
	int rez=-1;
	int n=CT->Behaviour.GetAmount();
	int ss=0;
	for(int i=0;i<n;i++)
	{
		ss+=CT->Behaviour[i];
	}
	int rr=GetRND(ss);
	int s2=0;
	for(int i=0;i<n;i++)
	{
		s2+=CT->Behaviour[i];
		if(s2>=rr)
			return i;
	}
	return rez;
}
void Cityzen::SetMeeting()
{
	if(OB)
	{
		int n=CityPopulation::Civ.GetAmount();
		FreeForTalk=true;
		
		DynArray<word> InList;
		InList.Add(OB->Index);
		int sx=OB->RealX;
		int sy=OB->RealY;
		for(int i=0;i<n;i++)
		{
			Cityzen* C=CityPopulation::Civ[i];
			if(C&&C->OB&&(!C->OB->InMotion)&&C->OB!=OB&&(C->OB->NMask&OB->NMask))
			{
				if(Norma((OB->RealX>>4)-(C->OB->RealX>>4),(OB->RealY>>4)-(C->OB->RealY>>4))<200)
				{
					InList.Add(C->OB->Index);
					sx+=C->OB->RealX;
					sy+=C->OB->RealY;
				}
			}
		}
		int m=InList.GetAmount();
		if(m>1)
		{
			sx/=m;
			sy/=m;
			for(int j=0;j<m;j++)
			{
				OneObject* Ci=GetOBJ(InList[j]);
				if(Ci)
				{
					byte dr=getDir(sx-Ci->RealX,sy-Ci->RealY);
					RotUnit(Ci->Index,dr,128);
					//Ci->RealDir=dr;
					//Ci->GraphDir=dr;
				}
			}
		}
	}
}
//==================================================================================================================//
CityBild::CityBild()
{
	BT=NULL;
	LastBornTime=GetAnimTime();//GetGlobalTime();
	Index=0xFFFF;
	OB=NULL;
	RegisterDynGroup(&DynGroup);
}
CityBild::~CityBild()
{
	DeleteDynGroup(&DynGroup);
}
void CityBild::Process()
{
	int time=GetAnimTime();//GetGlobalTime();
	if((ProcesedTime+3*25*256)<time)
	{
		int pause=time-LastBornTime;
		int n=BT->CanBorn.GetAmount();
		for(int i=0;i<n;i++)
		{
			if(pause>(GetRND(BT->CanBorn[i].MaxTime/1024))*1024)
			{
				Cityzen* ct = new Cityzen();
				//ProduceUnitFast(&DynGroup,&BT->CanBorn[i].CT->UnitType,&ct->Dyn,1);
				ProduceUnitTicks(&DynGroup,&BT->CanBorn[i].CT->UnitType,&ct->Dyn,10);
				ct->CT=BT->CanBorn[i].CT;
				ct->LifeTime=time;
				CityPopulation::Civ.Add(ct);
				LastBornTime=time;
			}
		}
		ProcesedTime=time;
	}
}
//==================================================================================================================//
CityPopulation::CityPopulation()
{
	FirstStart=true;
	SetCallBack=false;
}
CityPopulation::~CityPopulation()
{
	Clear();
}
void CityPopulation::Process()
{
	if(!SetCallBack)
	{
		SetCallBack=true;
		PreviosBirthCallback = SetBirthCallback(&CityPopulation::BirthCallback);
	}
	if( !(GetPeaceMode() || CheckNoCityLife() ) )
	{
		if(FirstStart)
		{
			FirstStart=false;
			//Init();
		}
		int n=CBild.GetAmount();
		for(int i=0;i<n;i++)
		{
			if(CBild[i])
				CBild[i]->Process();
		}
		int m=Civ.GetAmount();
		for(int j=0;j<m;j++)
		{
			if(Civ[j])
			{
				if(Civ[j]->State)
					Civ[j]->Process();
				else
				{
					delete (Civ[j]);
					Civ.Del(j,1);
					m--;
					j--;
				}
			}
		}
	}
}
void CityPopulation::LoadFromFile(char *file)
{
	xmlQuote Q;
	if(Q.ReadFromFile(file))
	{
		if(!strcmp(Q.GetQuoteName(),"City"))
		{
			xmlQuote* t=Q.GetSubQuote("CityzenType");
			if(t)
			{
				int n=t->GetNSubQuotes();
				for(int i=0;i<n;i++)
				{
					xmlQuote* tt=t->GetSubQuote(i);
					if(tt)
					{
						CityzenType* c =  new CityzenType();
						c->Load(tt);
						CType.Add(c);
					}
				}
			}
			t=Q.GetSubQuote("BildType");
			if(t)
			{
				int n=t->GetNSubQuotes();
				for(int i=0;i<n;i++)
				{
					xmlQuote* tt=t->GetSubQuote(i);
					if(tt)
					{
						int n=tt->GetNSubQuotes();
						for(int i=0;i<n;i++)
						{
							xmlQuote* ttt=tt->GetSubQuote(i);
							if(!strcmp("NDSName",ttt->GetQuoteName()))
							{
								BildType* b = new BildType();
								b->Load(tt);
								b->NDSNameID=GetUnitTypeIndex(ttt->Get_string(),&b->UnitType);
								int m=b->CanBorn.GetAmount();
								for(int j=0;j<m;j++)
								{
									word in=GetCityzenTypeIndexByName(b->CanBorn[j].CityzenTypeName);
									if(in!=-1)
										b->CanBorn[j].CT=CType[in];
								}
								BType.Add(b);
							}
						}
					}
				}
			}
			int n=CType.GetAmount();
			for(int i=0;i<n;i++)
			{
				if(CType[i])
				{
					int m=CType[i]->ComeIn.GetAmount();
					for(int j=0;j<m;j++)
					{
						int in = GetBildTypeIndexByName(CType[i]->ComeIn[j].BildTypeName);
						if(in!=-1)
							CType[i]->ComeIn[j].BT=BType[in];
					}
				}
			}
		}
	}
}
int CityPopulation::GetCityzenTypeIndexByName(const char* name)
{
	int n=CType.GetAmount();
	for(int i=0;i<n;i++)
	{
		if(CType[i])
			if(!strcmp(name,CType[i]->Name.pchar()))
				return i;
	}
	return -1;
}
int CityPopulation::GetBildTypeIndexByName(const char* name)
{
	int n=BType.GetAmount();
	for(int i=0;i<n;i++)
	{
		if(BType[i])
			if(!strcmp(name,BType[i]->Name.pchar()))
				return i;
	}
	return -1;
}
int CityPopulation::GetUnitTypeIndex(const char *NDSName, GAMEOBJ* GG)
{
	DString ds;
	ds.Add((char*)NDSName);
	RegisterUnitType(GG,ds.pchar());
	return GG->Index;
}
void CityPopulation::Init()
{
	LoadFromFile("CityLife\\CityPopulation.xml");
}
DLLIMPORT bool GetBuildingEntryPoint(int* x,int* y,word Index,int AddDistance);
void CityPopulation::ResearchMap()
{
	Clear();
	Init();
	int n=GetMaxObject();
	for(int i=0;i<n;i++)
	{
		OneObject* OB=GetOBJ(i);
		if(OB&&OB->NewBuilding&&OB->NNUM==7)	// Vital correction for GEC purpose
		{
			int m=BType.GetAmount();
			for(int j=0;j<m;j++)
			{
				if(OB->NIndex==BType[j]->NDSNameID)
				{
					int x, y;
					if(GetBuildingEntryPoint(&x,&y,OB->Index,16))
					{
						if(!CheckLandLocking(x,y,3,3))
						{
							CityBild* NCB = new CityBild();
							NCB->BT=BType[j];
							NCB->Index=OB->Index;
							NCB->OB=OB;
							AddOneUnitToGroup(&NCB->DynGroup,OB);
							CBild.Add(NCB);
						}
					}
				}
			}
		}
	}
}
void CityPopulation::Clear()
{
	int i;
	int n=CType.GetAmount();
	for(i=0;i<n;i++)
	{
		if(CType[i])
		{
			delete (CType[i]);
			CType[i]=NULL;
		}
	}
	CType.Clear();
	n=BType.GetAmount();
	for(i=0;i<n;i++)
	{
		if(BType[i])
		{
			delete (BType[i]);
			BType[i]=NULL;
		}
	}
	BType.Clear();
	int nn=CBild.GetAmount();
	for(i=0;i<nn;i++)
	{
		if(CBild[i])
		{
			delete (CBild[i]);
			CBild[i]=NULL;
		}
	}
	CBild.Clear();
	n=Civ.GetAmount();
	for(i=0;i<n;i++)
	{
		if(Civ[i])
		{
			delete (Civ[i]);
			Civ[i]=NULL;
		}
	}
	Civ.Clear();
	FirstStart=true;
}
int CityPopulation::BirthCallback(int NewbornIndex)
{
	if(NewbornIndex!=0xFFFF)
	{
		OneObject* OB=GetOBJ(NewbornIndex);
		if(OB&&OB->NewBuilding)
		{
			int m=BType.GetAmount();
			for(int j=0;j<m;j++)
			{
				if(OB->NIndex==BType[j]->NDSNameID)
				{
					CityBild* NCB = new CityBild();
					NCB->BT=BType[j];
					NCB->Index=OB->Index;
					NCB->OB=OB;
					NCB->DynGroup;
					AddOneUnitToGroup(&NCB->DynGroup,OB);
					CBild.Add(NCB);
				}
			}
		}
	}
	return 0;
}
DynArray<CityzenType*> CityPopulation::CType;
DynArray<BildType*>    CityPopulation::BType;
DynArray<CityBild*>	   CityPopulation::CBild;
DynArray<Cityzen*>     CityPopulation::Civ;

tpBirthCallback* CityPopulation::PreviosBirthCallback=NULL;
//==================================================================================================================//
CityPopulation CP;
//==================================================================================================================//