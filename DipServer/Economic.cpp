#include "Economic.h"
#include <crtdbg.h>
// /MISSION temp2.m3d
GAMEOBJ GD1, GD2, GD3, ONE; 
int NowTime;
 
FILE* 	Flog;
#define FBEGIN()
// fprintf(Flog,"%s %d\n",__FILE__,__LINE__);fflush(Flog);

int Norma(int x,int y);
bool ReadWinString(GFILE* F,char* STR,int Max);

CIMPORT
void ActivateDipDialog(char* request);
CIMPORT
void SendDipCommand(char* Data,int size);

void ErrM(char* s)
{
	MessageBox(NULL,s,"LOADING FAILED...",MB_ICONWARNING|MB_OK);
};
//-------------------------------------------//
Economic::Economic()
{
   TradePoints=NULL;
   TaxPayerList=NULL;
   GlobalTime=0;
   gggg=0;
}

Economic::~Economic()
{
	if (TradePoints)free(TradePoints);
	if (TaxPayerList)free(TaxPayerList);
	ActivFlag=false;


	list<TaxCollector*>::iterator a;
	for (a=TaxCollectorList.begin();a!=TaxCollectorList.end();++a)
		delete (*a);
	list<Village*>::iterator v;
	for (v=VillageList.begin();v!=VillageList.end();++v)
		delete (*v);
	list<Transport*>::iterator t;
	for (t=TransportList.begin();t!=TransportList.end();++t)
		delete (*t);
	list<Manufacture*>::iterator m;
	for (m=ManufactureList.begin();m!=ManufactureList.end();++m)
		delete (*m);
	list<Barrack*>::iterator b;
	for (b=BarrackList.begin();b!=BarrackList.end();++b)
		delete (*b);
	list<Carrier*>::iterator c;
	for(c=CarrierList.begin();c!=CarrierList.end();++c)
		delete (*c);
}
void Economic::Process()
{	
	////////////////////
	//Flog=fopen("Eco\\CachBug.log","w");
	////////////////////
	if(ActivFlag)
	{
		NowTime=GetGlobalTime();
		int GT=NowTime;
		//char c[16];
		//sprintf(c,"GlobalTime=%d",GT);
		//AssignHint1(c,10);
		//if (GT>(GlobalTime+8))
		if (!gggg)
		{
			//ActivateDipDialog("LW_new&|LW_file&Eco/cml/tmp.cml");
			gggg=1;
			SetInterfaceCallback(InterfaceOBJ::Show);
			//BarrackList.begin()->InProduceStack.push_back(0);
			//BarrackList.begin()->FillTradePoints(TradePoints,TradePointsSize);
			//SetResource(0,GOLD,0);
		}
		SaveSelectedUnits(GetMyNation(),&Temp,0);
		if (GT>200)
		{
			list<TaxCollector*>::iterator c;
			for (c=TaxCollectorList.begin();c!=TaxCollectorList.end();++c)
			{
				(*c)->Process(TaxPayerList,TaxPayerListSize,&TaxCollectorList);
			}
		}
		for(int i=0;i<TradePointsSize;i++)
			TradePoints[i]->CorrectPrice(TradePoints,TradePointsSize);

		if (GT>(GlobalTime+19))
		{
			GlobalTime=GT;
			list<Village*>::iterator v;
			for (v=VillageList.begin();v!=VillageList.end();++v)
			{
				(*v)->Process();	
			}
			list<Transport*>::iterator t;
			for (t=TransportList.begin();t!=TransportList.end();++t)
			{
				(*t)->Process(TradePoints,TradePointsSize,&TransportList);
			}
			list<Manufacture*>::iterator m;
			for (m=ManufactureList.begin();m!=ManufactureList.end();++m)
			{
				(*m)->Process(TradePoints,TradePointsSize);
			}
			list<Barrack*>::iterator b;
			for (b=BarrackList.begin();b!=BarrackList.end();++b)
			{
				if(!(*b)->InitFlag)
				{
					(*b)->FillTradePoints(TradePoints,TradePointsSize);
					(*b)->InitFlag=true;
					Carrier* Ca = new Carrier();
					Ca->OwnerB=(*b)->This();
					Bearer* B1 = new Bearer();
					Bearer* B2 = new Bearer();
					Bearer* B3 = new Bearer();
					Bearer* B4 = new Bearer();
					Ca->BearerList.push_back(B1);
					Ca->BearerList.push_back(B2);
					Ca->BearerList.push_back(B3);
					Ca->BearerList.push_back(B4);
					CarrierList.push_back(Ca);
				}
				(*b)->Process();
			}
			list<Carrier*>::iterator c;
			for(c=CarrierList.begin();c!=CarrierList.end();++c)
				(*c)->Process();
			//
			Mobilization();
			//
		}

		if (GetUnitsByNation(&Temp,GetMyNation())==1)
		{
			OneUnit U;
			GetUnitInfo(&Temp,0,&U);
			bool flg=0;
			list<Village*>::iterator v;
			for (v=VillageList.begin();v!=VillageList.end();++v)
			{
				if ((*v)->Unit.GetIndex()==U.Index)
				{
					flg=1;
					(*v)->ShowParams();
				}
			}
			list<Warehouse*>::iterator w;
			if(!flg)
			for (w=WarehouseList.begin();w!=WarehouseList.end();++w)
			{
				if ((*w)->Unit.GetIndex()==U.Index)
				{
					flg=1;
					(*w)->ShowParams();
				}
			}
		
			//InterfaceOBJ::Show(U.Index);

			list<Transport*>::iterator t;
			if(!flg)
			for (t=TransportList.begin();t!=TransportList.end();++t)
			{
				if ((*t)->Unit.GetIndex()==U.Index)
				{
					flg=1;
					(*t)->ShowParams();
				}
			}
			list<Manufacture*>::iterator m;
			for (m=ManufactureList.begin();m!=ManufactureList.end();++m)
			{
				if ((*m)->Unit.GetIndex()==U.Index)
				{
					flg=1;
					(*m)->ShowParams();
					(*m)->CMLobjectInfo("Eco\\cml\\tmp.cml");
					//if (!m->ActivDipD)
					{
						//ActivateDipDialog("LW_new&|LW_file&Eco/cml/tmp.cml");
						//DString DST;
						//DST.ReadFromFile("Eco/cml/tmp.cml");
						(*m)->ActivDipD=1;
					}
				}
				else
					(*m)->ActivDipD=0;
			}
			list<Barrack*>::iterator b;
			for (b=BarrackList.begin();b!=BarrackList.end();++b)
			{
				if ((*b)->GetIndex()==U.Index)
				{
					//m->ShowParams();
					//b->FillTradePoints(TradePoints,TradePointsSize);
					(*b)->CMLobjectInfo("Eco\\cml\\tmp.cml");
					ActivateDipDialog("LW_new&|LW_file&Eco/cml/tmp.cml");
				}	
			}
		}
		
		//ClearSelection(GetMyNation());
		//SelectUnits(&Temp,0);
	/*	if(!_CrtCheckMemory())
		{
			MessageBox(NULL,"MEMORY FAILED!!!","MEMORY FAILED...",MB_ICONWARNING|MB_OK);
		}
	*/	////////////////
		//if(Flog)fclose(Flog);
		////////////////
	}
}
 
void Economic::ResearchMap()
{
	Init();

	if(ActivFlag)
	{
		AssignHint1("Economic::ResearchMap()",40);
		RegisterDynGroup(&Temp);
		RegisterDynGroup(&GD1);
		RegisterDynGroup(&GD2);
		RegisterDynGroup(&GD3);
		RegisterFormation(&ONE,"#ODIN");

		for(int j=0;j<8;j++)NP[j].Init(j);

		int MAXOB = GetMaxObject();

		OneUnit U;
		for (int i=0;i<MAXOB;i++)
		{
			if(GetUnitGlobalInfo(i,&U))
			{
				FindIfUnitDescription::NType=U.NIndex;
				vector<UnitDescription>::iterator UDi;
				UDi = find_if(UDList.begin(), UDList.end(), FindIfUnitDescription());
				if (UDi!=UDList.end())
				{
					if(!strcmp(UDi->Type,"VILLAGE"))
					{
						Village* V = new Village();
						V->FillParams(UDi,U.Index);
						V->NP=&(NP[U.NI]);
						V->AddNewIndex(U.Index,V);
						VillageList.push_back(V);
						//AddNewIndex(Unit.GetIndex(),this);
						
					}
					else
					if(!strcmp(UDi->Type,"TRANSPORT"))
					{
						Transport* T = new Transport();
						T->FillParams(UDi,U.Index);
						T->NP=&(NP[U.NI]);
						TransportList.push_back(T);
					}
					else
					if(!strcmp(UDi->Type,"TAXCOLLECTOR"))
					{
						TaxCollector* C = new TaxCollector();
						C->FillParams(UDi,U.Index);
						C->NP=&(NP[U.NI]);
						TaxCollectorList.push_back(C);
					}
					else
					if(!strcmp(UDi->Type,"WAREHOUSE"))
					{
						Warehouse* W = new Warehouse();
						W->FillParams(UDi,U.Index);
						W->NP=&(NP[U.NI]);
						WarehouseList.push_back(W);
					}
					else
					if(!strcmp(UDi->Type,"MANAFACTURE"))
					{
						Manufacture* M = new Manufacture();
						M->FillParams(UDi,U.Index);
						M->NP=&(NP[U.NI]);
						ManufactureList.push_back(M);
					}
					else
					if(!strcmp(UDi->Type,"MARKET"))
					{
						Market* M = new Market();
						M->FillParams(UDi,U.Index);
						M->NP=&(NP[U.NI]);
						MarketList.push_back(M);
					}
					else
					if(!strcmp(UDi->Type,"BARRACK"))
					{
						Barrack* B = new Barrack();
						B->FillParams(UDi,U.Index);
						B->NP=&(NP[U.NI]);
						BarrackList.push_back(B);
					}
				}
			}
		}
		FillTradePoints();
	}
};
void Economic::FillTradePoints()
{
	TradePointsSize=VillageList.size();
	TradePointsSize+=WarehouseList.size();
	TradePointsSize+=ManufactureList.size();
	
	TaxPayerListSize=TradePointsSize;
	TaxPayerListSize+=TransportList.size();
	//TaxPayerListSize+=ManufactureList.size();

	TradePointsSize+=MarketList.size();
	
	if (TradePoints)free(TradePoints);
	TradePoints = (Warehouse**)malloc(TradePointsSize<<2);

	if (TaxPayerList)free(TaxPayerList);
	TaxPayerList = (Business**)malloc(TaxPayerListSize<<2);

	int i=0;
	int j=0;
	list<Village*>::iterator v;
	for (v=VillageList.begin();v!=VillageList.end();++v)
	{
		TradePoints[i]=(*v)->GetObject();
		TaxPayerList[j]=(*v)->GetObject();
		i++;
		j++;
	}
	list<Warehouse*>::iterator w;
	for (w=WarehouseList.begin();w!=WarehouseList.end();++w)
	{
		TradePoints[i]=(*w)->GetObject();
		TaxPayerList[j]=(*w)->GetObject();
		i++;
		j++;
	}
	
	list<Manufacture*>::iterator m;
	for (m=ManufactureList.begin();m!=ManufactureList.end();++m)
	{
		TradePoints[i]=(*m)->GetObject();
		TaxPayerList[j]=(*m)->GetObject();
		i++;
		j++;
	}

	list<Market*>::iterator ma;
	for (ma=MarketList.begin();ma!=MarketList.end();++ma)
	{
		TradePoints[i]=(*ma)->GetObject();
		i++;
	}
	
	list<Transport*>::iterator t;
	for (t=TransportList.begin();t!=TransportList.end();++t)
	{
		TaxPayerList[j]=(*t)->GetBusiness();
		j++;
	}

	for(i=0;i<TradePointsSize;i++)
	{
		Warehouse* mm=TradePoints[i];
		mm->MinPrice.SetAmo(i,mm->Price.BasePrice[i]);
		mm->Price.SetAmo(i,mm->Price.BasePrice[i]);
	}
}
void Economic::EconomicDipComm(char* com,char** params,int npr,ParsedRQ* Result)
{
	if(!strcmp(com,"e_test"))
	{
		DString DST;
		DST.ReadFromFile("Eco\\cml\\test.cml");
		char c[16];
		itoa(++gggg,c,10);
		DST.Replace("$COUNT$",c);
		DST.WriteToFile("Eco\\cml\\tmp.cml");
		Result->Clear();
		Result->Parse("LW_file&Eco/cml/tmp.cml");
	}
	else
	if(!strcmp(com,"e_produce"))
	{
		bool flag=true;
		list<Manufacture*>::iterator m;
		int e=-1;
		for (m=ManufactureList.begin();m!=ManufactureList.end()&&flag;++m)
		{
			e++;
			if((*m)->Unit.GetIndex()==atoi(params[0]))
			{
				flag=false;
				word BUF[32];
				BUF[0]=1024;
				BUF[1]=1;//Command ID 1-produce unit
				BUF[2]=e;//Manufacture ID
				BUF[3]=atoi(params[2]);//what create
				SendDipCommand((char*)BUF,8);

			}
		}
	}
	else
	if(!strcmp(com,"e_prodform"))
	{
		bool flag=true;
		list<Barrack*>::iterator m;
		int e=-1;
		for (m=BarrackList.begin();m!=BarrackList.end()&&flag;++m)
		{
			e++;
			if((*m)->GetIndex()==atoi(params[0]))
			{
				flag=false;
				word BUF[32];
				BUF[0]=1024;
				BUF[1]=2;//Command ID 2-produce formation
				BUF[2]=e;//Manufacture ID
				BUF[3]=atoi(params[2]);//what create
				SendDipCommand((char*)BUF,8);
			}
		}
	}
	else
	if(!strcmp(com,"e_orderform"))
	{
		bool flag=true;
		list<Barrack*>::iterator m;
		int e=-1;
		for (m=BarrackList.begin();m!=BarrackList.end()&&flag;++m)
		{
			e++;
			if((*m)->GetIndex()==atoi(params[0]))
			{
				flag=false;
				int formation=atoi(params[1]);
				(*m)->Order.push_back(formation);
				(*m)->OrderRL.LoadValue((*m)->Formations.FL[formation].NeedRL.Amo);
			}
		}
	}
	else
	if(!strcmp(com,"e_prodorder"))
	{
		bool flag=true;
		list<Barrack*>::iterator m;
		int e=-1;
		for (m=BarrackList.begin();m!=BarrackList.end()&&flag;++m)
		{
			e++;
			if((*m)->GetIndex()==atoi(params[0]))
			{
				flag=false;
				int sz=(*m)->Order.size();
				if(sz)
				{
					int const size=sz+3;
					word BUF[64];
					BUF[0]=1024;
					BUF[1]=3;//Command ID 3-produce formation order list
					BUF[2]=e;//Manufacture ID
					list<int>::iterator v=(*m)->Order.begin(); 
					for(int i=3;i<size;i++,v++)
						BUF[i]=(word)(*v);
					SendDipCommand((char*)BUF,size*2);
					(*m)->Order.clear();
				}
			}
		}
	}
}
void Economic::PerformCommand(char* Data,int size)
{
	word* wData=(word*)Data;
	if(wData[0]==1024)
	{
		switch (wData[1])
		{
		case 1:
			if(wData[2]>-1&&wData[2]<ManufactureList.size())
			{
				list<Manufacture*>::iterator m;
				m=ManufactureList.begin();
				for(int i=0;i<wData[2];i++,m++);
				(*m)->InProduceStack.push_back(wData[3]);
				int np=(*m)->PL.GetThing(wData[3])->NeedPeasant;
				if(np)
				{
					for(int i=0;i<np;i++)
					{
						Peasant P;
						P.Nation=(*m)->Unit.OB->NNUM;
						P.BildingUnitsCollector=&((*m)->OBJ_ComeIntoBilding);
						NeededPeasantList.push_back(P);
					}
				}
			}
			break;
		case 2:
			if(wData[2]>-1&&wData[2]<BarrackList.size())
			{
				list<Barrack*>::iterator m;
				m=BarrackList.begin();
				for(int i=0;i<wData[2];i++,m++);
				(*m)->InProduceStack.push_back(wData[3]);
			}
			break;
		case 3:
			if(wData[2]>-1&&wData[2]<BarrackList.size())
			{
				list<Barrack*>::iterator m;
				m=BarrackList.begin();
				for(int i=0;i<wData[2];i++,m++);
				(*m)->FillTradePoints(TradePoints,TradePointsSize);
				for(i=3;i<(size/2);i++)
				{
					(*m)->InProduceStack.push_back(wData[i]);
					int p=(*m)->GetFormationsPriceAndReservRes(wData[i]);
					(*m)->NP->MoneyAdd(-p);
					(*m)->MoneyToNeededRes+=p;
				}
				(*m)->OrderRL.ClearAmo();
			}
			break;
		}
	}
}
void Economic::Init()
{
	GFILE* 	F=Gopen("Eco\\res.lst","r");
	if(F)
	{
		ActivFlag=true;
		char ccc[257];
		char res[64];
		bool Done;
		int type=0, baseprice=0;
		ResourceList RL;
		do{
		Done=ReadWinString(F,ccc,256);
			if(Done&&ccc[0]!='/')
			{
				_strupr(ccc);
				if(sscanf(ccc,"%s %d %d",res,&type,&baseprice))
				{
					RL.AddResource(res,type,baseprice);
				}
			}
		}while(Done);
		F->Close();
	}
	else
	{
		//ErrM("Unable to open Eco\\res.lst");
	}

	if(ActivFlag)
	{
		F=Gopen("Eco\\nat.lst","r");
		if(F)
		{
			char ccc[257];
			char nat[64];
			bool Done;
			do{
			Done=ReadWinString(F,ccc,256);
				if(Done&&ccc[0]!='/')
				{
					//_strupr(ccc);
					if(sscanf(ccc,"%s",nat))
					{
						LoadNation(nat);
					}
				}
			}while(Done);
			F->Close();
		}
		else
		{
			ErrM("Unable to open Eco\\nat.lst");
		}
		FormationList FL;
		FL.FillFromFile("Eco\\form.lst");
	}
}
void Economic::LoadNation(char *nat)
{
	char path[256];
	sprintf(path,"Eco\\%s",nat);
	GFILE* F=Gopen(path,"r");
	if(F)
	{
		char ccc[257];
		char un[64];
		bool Done;
		do{
		Done=ReadWinString(F,ccc,256);
			if(Done&&ccc[0]!='/')
			{
				_strupr(ccc);
				if(sscanf(ccc,"%s",un))
				{
					if (!strcmp(un,"PRODUCELIST"))
					{		
						Manufacture::PL.ReadPL(F);
					}
					else
					{
						UnitDescription UD;
						if (UD.Load(un))
							UDList.push_back(UD);
					}
				}
			}
		}while(Done);
		F->Close();
	 }
	 else
	 {
		sprintf(path,"Unable to open %s",path);
		ErrM(path);
	 }
}
int Economic::GetFreePeasant(byte Nation)
{
	int rez=-1;
	int max=0;
	int hang=100;
	for(int i=0;i<TradePointsSize;i++)
	{
	   if(TradePoints[i]->Unit.OB->NNUM==Nation)
	   {
		   if((TradePoints[i]->Hunger<hang)&&(TradePoints[i]->Population>0))
		   {
			   hang=TradePoints[i]->Hunger;
			   rez=i;
		   }
	   }
	}
	return rez;
}
void Economic::Mobilization()
{
	int sz=NeededPeasantList.size();
	for(int i=0;i<sz;i++)
	{
		int nat=NeededPeasantList.begin()->Nation;
		int man=GetFreePeasant(nat);
		if(man>-1)
		{
			TradePoints[man]->MobilizateUnit(NeededPeasantList.begin()->BildingUnitsCollector);
			NeededPeasantList.pop_front();
		}
	}
}
void Economic::AddInPeasantList()
{
	//
}
//-------------------------------------------//
ResourceList::ResourceList()
{
	ResourceAmount=Resources.size();
	Amo.reserve(64);
	for (int i=0;i<ResourceAmount;i++)
	   Amo.push_back(0);
};
ResourceList::~ResourceList()
{
	
};
/*
void AddResource(char *T, int type);
	void SetAmo(int index);
	int GetAmo(int index);
*/
void ResourceList::AddResource(char *T, int t, int BP)
{
	//FBEGIN()
	Resource r;
	sprintf(r.name,"%s",T);
	r.type=t;
	Resources.push_back(r);
	//Amo.push_back(0);
	BasePrice.push_back(BP);
	ResourceAmount=Resources.size();
}
void ResourceList::SetAmo(int Index, int vle)
{
	//FBEGIN()
	if (Index>-1&&Index<ResourceAmount)
	{
		vector<int>::iterator i = Amo.begin()+Index;
		*i=vle;
	}
}
int ResourceList::GetAmo(int Index)
{
	if (Index<ResourceAmount)
	{
		//vector<int>::iterator i = Amo.begin();
		//for(int in=0;in<Index;in++,i++)
		//return *i;
		return Amo[Index];
	}
	return 0;
}
char* ResourceList::GetName(int Index)
{
	if (Index<ResourceAmount)
	{
		vector<Resource>::iterator i = Resources.begin()+Index;
		return i->name;
	}
	return "";
}
void ResourceList::AddAmo(int Index, int vle)
{
	if (Index<ResourceAmount)
	{
		vector<int>::iterator i = Amo.begin()+Index;
		*i+=vle;
		if (*i<0) *i=0;
	}
}
void ResourceList::ClearAmo()
{
	for(int i=0;i<ResourceAmount;i++) 
		SetAmo(i,0);
}
void ResourceList::First()
{
	ResourcesI=Resources.begin();
	AmoI=Amo.begin();
}
bool ResourceList::Next()
{
	ResourcesI++;
	AmoI++;
	bool rez=false;
	if (ResourcesI!=Resources.end())
		rez=true;
	return rez;
}
void ResourceList::SetAmo(int vle)
{
	*AmoI=vle;
}
int ResourceList::GetAmo()
{
	return *AmoI;
}
void ResourceList::AddAmo(int vle)
{
	*AmoI+=*AmoI+vle;
}
int ResourceList::GetIndex(char *c)
{
	for (int i=0;i<ResourceAmount;i++)
	{
		if (strcmp(Resources[i].name,c)==0)
			return i;
	}
	return -1;
}
void ResourceList::LoadValue(vector<int> Up)
{
	if (Up.size()==ResourceAmount)
	{
		for (int i=0;i<ResourceAmount;i++)
		{
			Amo[i]=Up[i];
		}
	}
}
void ResourceList::AddValueVector(vector<int> Up)
{
	if (Up.size()==ResourceAmount)
	{
		for (int i=0;i<ResourceAmount;i++)
		{
			Amo[i]+=Up[i];
		}
	}
}
bool ResourceList::ReadFromFile(GFILE *f)
{
	bool Done, ende=true;
	char ccc[257];
	char atom[64];
	int vle;
	do{
		Done=ReadWinString(f,ccc,256);
		if(strstr(ccc,"\\RESOURCELIST"))
			ende=false;
		else
		if(sscanf(ccc,"%s %d",atom, &vle))
			SetAmo(GetIndex(atom),vle);
	}while(Done&&ende);
	return true;
}
int ResourceList::ResourceAmount=0;
vector<Resource> ResourceList::Resources;
vector<int> ResourceList::BasePrice;
//-------------------------------------------//
Transport::Transport()
{
	Target=NULL;
	IsLoad=false;
	LoadTarget=NULL;
}
Transport::~Transport()
{ 
	Target=NULL;
	LoadTarget=NULL;
	Station=NULL;
}
void Transport::Process(Warehouse **TradePoints, int TPSize, list<Transport*> *TrL)
{
	FBEGIN()
	OneUnit U;
	GetUnitGlobalInfo(Unit.GetIndex(),&U);
	Unit.x=U.x;
	Unit.y=U.y;
	if (Target)
	{
		if ((Unit.GetDist(Target->Unit.x-Unit.x,Target->Unit.y-Unit.y)<200))//priehali
		{
			UnLoad();
			Warehouse* Dis = GetTarget(TradePoints, TPSize, TrL); 
			if(Target)
			{
				if (Target!=LoadTarget)
				{
					Load(LoadTarget);//get ByGruz&Target	
					Target=LoadTarget;
				}
				else
				{
					Load(Dis);
					Target=Dis;
				}
			}
		}
		else
		{
			Unit.MoveTo(Target->Unit.x,Target->Unit.y);
		}
	}
	else
	{
		GetTarget(TradePoints, TPSize, TrL);
		Target=LoadTarget;
	}
}
int Transport::GetEffect(Warehouse *W1,Warehouse *W2, list<Transport*> *TrL)
{
	FBEGIN()
	int kol=W2->NeededResourcesRL.ResourceAmount;
	int rez=0;
	int sum=0;
	list<Transport>::iterator T;
	for (int i=0;i<kol;i++)
	{
		int N=W2->NeedAmo(i);
		if (N&&W2->MyMoney)
		{
			int E=W1->AvailableRL.GetAmo(i)-W1->NeededResourcesRL.GetAmo(i)+RL.GetAmo(i);
			if(E<0)E=RL.GetAmo(i);
			if (E)
			{
				int p1=W1->GetPrice(i);
				int p2=W2->GetPrice(i);
				if ((p2>p1))
				{
					int vol = N;
					if (E<N) vol=E;
					if (W2->MyMoney<vol*p2)
					{
						vol=W2->MyMoney/p2;
						if(W2->MyMoney<p2)vol=0;
					}
					sum+=vol*(p2-p1);
				}
			}
		}
	}
	if (sum)
	{
		bool flag=true;
		int ts1=Unit.GetDist(W1->Unit.x-Unit.x,W1->Unit.y-Unit.y);
		list<Transport*>::iterator ii;
		for (ii=TrL->begin();(ii!=TrL->end())&&flag;++ii)
		{
			if (((*ii)->Unit.GetIndex()!=Unit.GetIndex())&&((*ii)->Target==W1))
			{
				int ts2=Unit.GetDist(W1->Unit.x-(*ii)->Unit.x,W1->Unit.y-(*ii)->Unit.y);
				if(ts2<ts1)
					flag=false;
			}
			if (flag&&(((*ii)->Unit.GetIndex()!=Unit.GetIndex())&&((*ii)->Target==W2)))
			{
				/*int voll=0;
				int nvoll=0;
				for (int i=0;i<kol;i++)
				{		
					int nee=W2->NeedAmo(i);
					int axx=ii->RL.GetAmo(i);
					nvoll+=nee;
					if(nee)
						voll+=axx;
				}
				if(voll>=nvoll)
				*/
					flag=false;
			}
		}
		if (flag)
		{
			int s1=Unit.GetDist(W1->Unit.x-Unit.x,W1->Unit.y-Unit.y);
			int s2=Unit.GetDist(W1->Unit.x-W2->Unit.x,W1->Unit.y-W2->Unit.y);
			rez=sum*1000/(s1*2+s2)+1;
		}
	}
	return rez;
}
void Transport::Load(Warehouse *Kuda)
{
	FBEGIN()
	if (Kuda&&MyMoney&&Target&&Kuda->MyMoney)
	{
		int kol=Kuda->NeededResourcesRL.ResourceAmount;
		for (int i=0;i<kol;i++)
		{
			int N=Kuda->NeedAmo(i);
			int E=Target->AvailableRL.GetAmo(i)-Target->NeededResourcesRL.GetAmo(i);
			if (E<0)E=0;
			if (N&&MyMoney&&E)
			{
				int p1=Target->GetPrice(i);
				int p2=Kuda->GetPrice(i);
				if (p2>p1)
				{
					int vol = N;
					if (E<N) vol=E;
					int sum=vol*p1;
					if (sum>MyMoney) 
					{
						vol=MyMoney/p1;
						sum=vol*p1;
					}
					if (sum>Kuda->MyMoney)
					{
						vol=Kuda->MyMoney/p2;
						sum=vol*p1;
					}
					if (Bay(Target,sum))
					{
						Target->AddResource(i,-vol);
						RL.AddAmo(i,vol);
					}
				}
			}
		}
	}
	//Target=Kuda;
}
void Transport::UnLoad()
{
	FBEGIN()
	if (Target)
	{
		int kol=Target->NeededResourcesRL.ResourceAmount;
		for (int i=0;i<kol;i++)
		{
			int vol=RL.GetAmo(i);
			int nee=Target->NeedAmo(i);
			if (vol&&Target->MyMoney&&nee)
			{
				int p1=Target->GetPrice(i);
				int sum=vol*p1;
				if (sum>Target->MyMoney) 
				{
					vol=Target->MyMoney/p1;
					sum=vol*p1;
				}
				if (Sel(Target,sum))
				{
					Target->AddResource(i,vol);
					RL.AddAmo(i,-vol);
				}
			}
		}
	}
	//Target=NULL;
}
Warehouse* Transport::GetTarget(Warehouse **TradePoints, int TPSize, list<Transport*> *TrL)
{
	Warehouse* W=NULL;
	//
	int max=0;
	for (int i=0;i<TPSize;i++)
	{
		if(Unit.OB->NMask&TradePoints[i]->Unit.OB->NMask)
			for (int j=0;j<TPSize;j++)
			{
				if(Unit.OB->NMask&TradePoints[j]->Unit.OB->NMask)
					if (i!=j)
					{
						int mass=GetEffect(TradePoints[i],TradePoints[j],TrL);
						if (mass>max)
						{
							max=mass;
							W=TradePoints[j];
							LoadTarget=TradePoints[i];
						}
					}
			}
	}
	if (max==0)
		Target=NULL;
	return W;
}
void Transport::ShowParams()
{
	FBEGIN()
	char s[128];
	sprintf(s,"Transport Index %d",Unit.GetIndex());
	AssignHint1(s,60);
	sprintf(s,"%s","Gruz resources: ");
	for (int i=0;i<RL.ResourceAmount;i++)
	{
		if(RL.GetAmo(i)>0)
			sprintf(s,"%s %s-%d",s,RL.GetName(i),RL.GetAmo(i));
	}
	AssignHint1(s,60);
	sprintf(s,"Money:%d, Nalogi:%d",MyMoney,Nalogi);
	AssignHint1(s,60);
	if (IsLoad)
		sprintf(s,"IsLoad: %s, TargetIndex: ","True");
	else
		sprintf(s,"IsLoad: %s, TargetIndex: ","False");
	if (Target)
		sprintf(s,"%s %d",s,Target->Unit.GetIndex());
	AssignHint1(s,60);
}
void Transport::FillParams(UnitDescription* UD, int Index)
{
	Business::FillParams(UD,Index);
	RL.LoadValue(UD->GetRLByName("RESOURCES"));
}
//-------------------------------------------//
Business::Business()
{
	MyMoney=0;
	Nalogi=0;
	BaseMoney=0;
	Tax=20;//%
	Profit=30;//%
	AutoFlag=true;
}
Business::~Business()
{}
int Business::PayTax(int Sum)
{
	int nalog=Sum*Tax/100;
	Nalogi+=nalog;
	return Sum-nalog;
}
bool Business::Bay(Business* Ukogo,int Sum)
{
	FBEGIN()
	bool rez=false;
	if (MyMoney>=Sum)
	{
		MyMoney-=Sum;
		Ukogo->MyMoney+=Ukogo->PayTax(Sum);
		rez=true;
	}
	return rez;
}
bool Business::Sel(Business* Komu,int Sum)
{
	bool rez=false;
	if(Komu)
	{
		if (Komu->MyMoney>=Sum)
		{
			MyMoney+=PayTax(Sum);
			Komu->MyMoney-=Sum;
			rez=true;
		}
	}
	else
		MyMoney+=PayTax(Sum);
	return rez;
}
Business* Business::GetBusiness()
{
	return this;
}
void Business::FillParams(UnitDescription* UD, int Index)
{
	Nalogi=0;
	BaseMoney=0;
	MyMoney=UD->StartMoney;
	Tax=UD->Tax;
	Unit.SetIndex(Index);
}
void Business::Show()
{
	char s[64];
	sprintf(s,"MyMoney %d: Nalogi %d",MyMoney,Nalogi);
	AssignHint1(s,60);
	ActivateCMLInterface(7,"LW_new&|LW_file&Eco/cml/tmp.cml");
	DrawCMLInterface(7);
	
}
//-------------------------------------------//
MobileUnit::MobileUnit()
{
	Z.Type = 0x40000000 + 200;
}
MobileUnit::~MobileUnit()
{}
void MobileUnit::MoveTo(int x, int y)
{
	/*ClearSelection(OB->NNUM);
	AddUnitToSel(GetIndex());
	Z.Index=x;
	Z.Serial=y;
	SelSendTo(OB->NNUM,&Z,0,0);
	*/
	OBJ_SendTo(GetIndex(),x,y,128+1,0);
}
void MobileUnit::UpdateCoord()
{
	//
}
//-------------------------------------------//
Peasant::Peasant()
{
	//
}
//-------------------------------------------//
Village::Village()
{
	Hunger=0;
}
Village::~Village()
{}
void Village::Process()
{
	bool flag=false;
	for (int i=0;i<AvailableRL.ResourceAmount;i++)
	{
		int ou=OutgoingRL.GetAmo(i);
		int av=AvailableRL.GetAmo(i);
		if (ou&&(av==0))
			flag=true;
	}
	if (flag)
	{
		if (Population)
			Hunger--;
	}
	else
		Hunger++;

	if(Population&&(Hunger<-100))
	{
		Population--;
		Hunger=0;
	}
	if(Hunger>100)
	{
		Population++;
		Hunger=0;
	}
	if (Population)
	{
		for (i=0;i<AvailableRL.ResourceAmount;i++)
		{
			int ou=OutgoingRL.GetAmo(i);
			if(ou)
			{
				AddResource(i,(-ou*Population)/10);
				NeededResourcesRL.SetAmo(i,(ou*Population/10)*40+1);
			}
			AddResource(i,(ProduceRL.GetAmo(i)*Population)/10);
		}
	}
	ProofPrice();
}
void Village::ShowParams()
{
	Warehouse::ShowParams();
	char s[128];
	sprintf(s,"Population %d, Hunger: %d",Population,Hunger);
	AssignHint1(s,60);
}
void Village::FillParams(UnitDescription* UD, int Index)
{
	Warehouse::FillParams(UD, Index);
	ProduceRL.LoadValue(UD->GetRLByName("PRODUCE"));
	Population=UD->StartPopulation;	
}
//-------------------------------------------//
Warehouse::Warehouse()
{
	ActivDipD=0;
	TimeInterval=300;
	CheckTime=0;
	CheckTime2=0;
	for(int i=0;i<PriceKf.ResourceAmount;i++)
	{
		PriceKf.SetAmo(i,100);
		MinPrice.SetAmo(i,MinPrice.BasePrice[i]);
		Price.SetAmo(i,Price.BasePrice[i]);
	}
	WarehouseType=0;
}
Warehouse::~Warehouse()
{}
void Warehouse::ShowParams()
{
	FBEGIN()
	char s[1024], cp[1024];
	sprintf(s,"WareHouse Index %d; Money:%d, Nalogi:%d",Unit.GetIndex(),MyMoney,Nalogi);
	AssignHint1(s,60);
	sprintf(s,"%s","Available resources: ");
	for (int i=0;i<AvailableRL.ResourceAmount;i++)
	{
		if (AvailableRL.GetAmo(i)>0)
		{
			strcpy(cp,s);
			sprintf(s,"%s %s-%d:%d",cp,AvailableRL.GetName(i),AvailableRL.GetAmo(i),GetPrice(i));
		}
	}
	AssignHint1(s,60);
	sprintf(s,"%s","Needed resources: ");
	for (i=0;i<AvailableRL.ResourceAmount;i++)
	{
		if (NeededResourcesRL.GetAmo(i)>0)
		{
			strcpy(cp,s);
			sprintf(s,"%s %s-%d:%d",cp,NeededResourcesRL.GetName(i),NeededResourcesRL.GetAmo(i),GetPrice(i));
		}
	}
	//AssignHint1(s,60);
	//AssignHint1(" ",60);
}
Warehouse* Warehouse::GetObject()
{
	return this;
}
int Warehouse::GetPrice(int ResID)
{
	FBEGIN()
	int rez=0;
	int ne=NeededResourcesRL.GetAmo(ResID);
	int av=AvailableRL.GetAmo(ResID);
	/*if (av>=ne)
	{
		//rez=av/(av-ne);
		rez=AvailableRL.BasePrice[ResID];
	}
	else
	{
		int x=ne-av;
		rez=AvailableRL.BasePrice[ResID]+x*0.03;
	}
	*/
	if((AvailableRL.GetAmo(ResID)-NeededResourcesRL.GetAmo(ResID))>0)
		rez=Price.GetAmo(ResID);
	else
		rez=(MinPrice.GetAmo(ResID)*PriceKf.GetAmo(ResID))/100;

	/*
	rez=(AvailableRL.BasePrice[ResID]*PriceKf.GetAmo(ResID))/100;
	if(!rez)rez=AvailableRL.BasePrice[ResID];
	*/
	if(rez<1)rez=1;
	return rez;
}
int Warehouse::NeedAmo(int ResourceID)
{
	int rez=0;
	int N=NeededResourcesRL.GetAmo(ResourceID);
	int A=AvailableRL.GetAmo(ResourceID);
	if(N>A)rez=N-A; 
	return rez;
}
void Warehouse::FillParams(UnitDescription* UD, int Index)
{
	Business::FillParams(UD,Index);
	AvailableRL.LoadValue(UD->GetRLByName("AVAILABLERESOURCES"));
	NeededResourcesRL.LoadValue(UD->GetRLByName("NEEDEDRESOURCES"));
	OutgoingRL.LoadValue(UD->GetRLByName("OUTGOINGRESOURCES"));
	OneUnit U;
	GetUnitGlobalInfo(Index,&U);
	Unit.x=U.x;
	Unit.y=U.y+150;
	RegisterDynGroup(&OBJ_ComeIntoBilding);
	SGP_ComeIntoBuilding(&OBJ_ComeIntoBilding,0,Index);
}
void Warehouse::ProofPrice()
{
	if(NowTime>=(CheckTime+TimeInterval))
	{
		for(int i=0;i<AvailableRL.ResourceAmount;i++)
		{
			int amo=AvailableRL.GetAmo(i);
			int K1=amo-NeededResourcesRL.GetAmo(i);
			if(K1)
			{
				int K2=0;
				if(amo)K2=-((amo-CheckAmo.GetAmo(i))*2000)/(amo*1000);
				if(K1>0)PriceKf.AddAmo(i,-2+K2);
				if(K1<0)PriceKf.AddAmo(i,2+K2);
				if(K2)
					CheckAmo.SetAmo(i,amo);
			}
		}
		CheckTime=NowTime;
	}
}
void Warehouse::MobilizateUnit(GAMEOBJ* Kuda)
{
	ClearGroup(&GD1);
	InsertUnitToGroup(&GD1,Unit.GetIndex());
	RegisterUnitType(&GD2,"UnitKri(FR)");
	ProduceUnitFast(&GD1,&GD2,Kuda,0);
	Population--;
	Hunger+=50;
}
void Warehouse::AddResource(int ResourceID, int Amo)
{
	AvailableRL.AddAmo(ResourceID, Amo);
	if(Amo>0)
		In.AddAmo(ResourceID, Amo);
	else
		Out.AddAmo(ResourceID, -Amo);
}
void Warehouse::CorrectPrice(Warehouse **TradePoints, int TradePointsSize)
{
	if((CheckTime+TimeInterval)<NowTime)
	{
		for(int i=0;i<AvailableRL.ResourceAmount;i++)
		{
			int av=AvailableRL.GetAmo(i);
			int ne=NeededResourcesRL.GetAmo(i);
			int in=In.GetAmo(i);
			int out=Out.GetAmo(i);
			if(ne)
			{
				if(ne>av)
				{
					if(in==0)
						PriceKf.AddAmo(i,5);
					else
					if(in<out)
						PriceKf.AddAmo(i,2);
				}
				if((av>0)&&in)
				{
					if(in==out)
						;//PriceKf.AddAmo(i,10);
					else
					if(in>out)
						if(PriceKf.GetAmo(i)>100)
							PriceKf.AddAmo(i,-2);
				}
			}
			if((av-ne)>0)
			{
				if(in>out)
					Price.SetAmo(i,(Price.GetAmo(i)*97)/100);
				if(out>in)
					Price.SetAmo(i,(Price.GetAmo(i)*103)/100);
			}
		}
		CheckTime=NowTime;
	}
	if((CheckTime2+TimeInterval*10+9)<NowTime)
	{
		for(int i=0;i<AvailableRL.ResourceAmount;i++)
		{
			int av=AvailableRL.GetAmo(i);
			int ne=NeededResourcesRL.GetAmo(i);
			if(ne)
			{
				int min=GetMinPrice(TradePoints, TradePointsSize, i);
				if(((min*100)/MinPrice.GetAmo(i))<70)
					PriceKf.SetAmo(i,100);
				MinPrice.SetAmo(i,min);
			}
			In.SetAmo(i,0);
			Out.SetAmo(i,0);
		}
		CheckTime2=NowTime;
	}
}
int Warehouse::GetMinPrice(Warehouse **TradePoints, int TradePointsSize, int ResID)
{
	int min=999999;
	for(int i=0;i<TradePointsSize;i++)
	{
		Warehouse* w=TradePoints[i];
		if((this!=w)&&(Unit.NMask()&w->Unit.NMask()))
		if((w->AvailableRL.GetAmo(ResID)-w->NeededResourcesRL.GetAmo(ResID))>0)
		{
			if(w->GetPrice(ResID)<min)
				min=w->GetPrice(ResID);
		}
	}
	return min;
}
void Warehouse::MakeGovernmentOrder(int ResID, int Amo)
{
	BookedPrice.SetAmo(ResID,GetPrice(ResID));
	AvailableRL.AddAmo(ResID,-Amo);
	Booked.AddAmo(ResID,Amo);
}
//-------------------------------------------//
Market::Market()
{
	WarehouseType=1;
}
Market::~Market()
{}
int Market::GetPrice(int ResourceID)
{
	return AvailableRL.BasePrice[ResourceID]*5;
}
void Market::FillParams(UnitDescription* UD, int Index)
{
	Warehouse::FillParams(UD,Index);
	for(int i=0;i<AvailableRL.ResourceAmount;i++)
	{
		AvailableRL.SetAmo(i,10000000);
	}
}
//-------------------------------------------//
UnitInfo::UnitInfo()
{
	//OneObject* GetOBJ(int Index)
	Index=-1;
}
UnitInfo::~UnitInfo()
{}
UnitInfo* UnitInfo::GetUnitI()
{
	return this;
}
int UnitInfo::GetDist(int dx,int dy)
{
	return Norma(dx,dy);
}
void UnitInfo::UpdateCoord()
{
//	OneUnit U;
//	GetUnitGlobalInfo(UnitInfo::Index,&U);
//	x=U.x;
//	y=U.y;
}
byte UnitInfo::NMask()
{
	return  OB->NMask;
}
word UnitInfo::NIndex()
{
	return OB->NIndex;
}
void UnitInfo::SetIndex(int In)
{
	Index=In;
	OB=GetOBJ(Index);
}
int UnitInfo::GetIndex()
{
	return Index;
}

//-------------------------------------------//
TaxCollector::TaxCollector()
{
	Money=0;
}
TaxCollector::~TaxCollector()
{}
void TaxCollector::Process(Business **TaxPayerList,int TPLSize, list<TaxCollector*> *TCL)
{
	int max=0;
	int in=0;
	//Unit.UpdateCoord();
	OneUnit U;
	GetUnitGlobalInfo(Unit.GetIndex(),&U);
	Unit.x=U.x;
	Unit.y=U.y;
	Business *B, *D;
	for(int i=0;i<TPLSize;i++)
	{
		if(Unit.OB->NNUM==TaxPayerList[i]->Unit.OB->NNUM)
		{
			B=TaxPayerList[i];
			//B->Unit.UpdateCoord();
			int dist=Unit.GetDist(Unit.x-B->Unit.x,Unit.y-B->Unit.y);
			if (dist<180)
			{
				Money+=B->Nalogi;
				//
				//AddResource(0,GOLD,B->Nalogi);
				NP->MoneyAdd(B->Nalogi);
				//
				B->Nalogi=0;
			}
			if (dist==0) dist=1;
			int w=(B->Nalogi*1000)/dist;
			if (w>max)
			{
				bool flag=true;
				list<TaxCollector*>::iterator tcli;
				for (tcli=TCL->begin();(tcli!=TCL->end())&&flag;++tcli)
				{
					if (dist>Unit.GetDist(B->Unit.x-(*tcli)->Unit.x,B->Unit.y-(*tcli)->Unit.y))
						flag=false;
				}
				if(flag)
				{
					in=i;
					max=w;
					D=B;
				}
			}
		}
	}
	if (max)
	{
		Unit.MoveTo(TaxPayerList[in]->Unit.x,TaxPayerList[in]->Unit.y);
		D->Unit.MoveTo(B->Unit.x,B->Unit.y);
	}
}
void TaxCollector::FillParams(UnitDescription* UD, int Index)
{
	Unit.SetIndex(Index);
}
//-------------------------------------------//
Thing::Thing()
{
	NeedPeasant=0;
	NeedHorce=0;
}
//-------------------------------------------//
ProduceList::ProduceList()
{}
ProduceList::~ProduceList()
{}
void ProduceList::AddThing(Thing Th)
{
	Things.push_back(Th);
}
Thing* ProduceList::GetThing(int id)
{
	Thing* rez=NULL;
	if (id>-1&&id<Things.size())
		 rez=&Things[id];
	return rez;
}
bool ProduceList::ReadPL(GFILE *f)
{
	char ccc[257];
	bool Done;
	do{
	Done=ReadWinString(f,ccc,256);
			if(Done&&ccc[0]!='/')
			{
			//_strupr(ccc);
			if (strstr(ccc,"\\PRODUCELIST"))
			{
				Done=false;
			}
			else
			if (strstr(ccc,"CREATE"))
			{
				ReadCreate(f);		
			}
		}
	}while(Done);

	return true;
}
bool ProduceList::ReadCreate(GFILE *f)
{
	char ccc[257];
	char atom[64], tmp[64];
	bool Done;
	Thing Th;
	do{
	Done=ReadWinString(f,ccc,256);
		if(Done&&ccc[0]!='/')
		{
			//_strupr(ccc);
			if (strstr(ccc,"\\CREATE"))
			{
				Things.push_back(Th);
				Done=false;
			}
			if (strstr(ccc,"RESOURCE "))
			{
				sscanf(ccc,"%s %s",tmp,atom);
				Th.ResourceID=Th.SpendRL.GetIndex(atom);
				Th.Type=1;
				Th.UnitTypeID=-1;
			}
			else
			if (strstr(ccc,"UNIT"))
			{
				sscanf(ccc,"%s %s",tmp,Th.UnitTypeName);
				GAMEOBJ G;
				RegisterUnitType(&G,Th.UnitTypeName);
				Th.UnitTypeID=G.Index;
				Th.Type=2;
				Th.ResourceID=-1;
			}
			else
			if (strstr(ccc,"TIME"))
			{
				sscanf(ccc,"%s %d",tmp, &Th.Time);	
			}
			else
			if (strstr(ccc,"NEEDPEASANT"))
			{
				sscanf(ccc,"%s %d",tmp, &Th.NeedPeasant);	
			}
			else
			if (strstr(ccc,"RESOURCELIST "))
			{
				sscanf(ccc,"%s %s", tmp, atom);
				if (strstr(atom,"SPENDRL"))
					Th.SpendRL.ReadFromFile(f);
			}
		}
	}while(Done);
	return true;
}
int ProduceList::GetProduceIndex(int ResourceID)
{
	//FBEGIN()
	if (ResourceID>-1)
	{
		int sz=Things.size();
		for (int i=0;i<sz;i++)
			if (Things[i].ResourceID==ResourceID)
				return i;
	}
	return -1;
}
int ProduceList::GetProduceIndexByUnitName(char* UnitName)
{
	FBEGIN()
	int sz=Things.size();
	for (int i=0;i<sz;i++)
		if (strstr(Things[i].UnitTypeName,UnitName))
			return i;
	return -1;
}
vector<Thing> ProduceList::Things;
//-------------------------------------------//
UnitDescription::UnitDescription()
{
	NType=-1;
	StartMoney=0;
	Tax=0;
	StartPopulation=0;
}
UnitDescription::~UnitDescription()
{}
bool UnitDescription::Load(char *f)
{
	bool rez=false;
	char path[256], errm[256];
	sprintf(path,"Eco\\%s.eud",f);
	GFILE* F=Gopen(path,"r");
	GAMEOBJ G;
	if(F)
	{
		rez=true;
		char ccc[257];
		char atom[64];
		char RLNAME[64];
		bool RLStart=false;
		bool Done;
		
		do{
		Done=ReadWinString(F,ccc,256);
			if(Done&&ccc[0]!='/')
			{
				//_strupr(ccc);
				if (strstr(ccc,"NAME"))
				{
					if(sscanf(ccc,"NAME %s",atom))
						if(RegisterUnitType(&G,atom))
							NType=G.Index;
				}
				else
				if (strstr(ccc,"TYPE"))
				{
					sscanf(ccc,"TYPE %s",Type);
				}
				else
				if (strstr(ccc,"STARTMONEY"))
				{
					sscanf(ccc,"STARTMONEY %d",&StartMoney);
				}
				else
				if (strstr(ccc,"TAX"))
				{
					sscanf(ccc,"TAX %d",&Tax);
				}
				else
				if (strstr(ccc,"STARTPOPULATION"))
				{
					sscanf(ccc,"STARTPOPULATION %d",&StartPopulation);
				}
				else
				if(sscanf(ccc,"RESOURCELIST %s",RLNAME))
				{
					ReadRL(F, RLNAME);
				}
				else
				if(strstr(ccc,"CAN_PRODUCE"))
				{
					ReadCP(F);
				}			
			}
		}while(Done);
		F->Close();
	 }
	 else
	 {
		sprintf(errm,"Unable to open %s",path);
		ErrM(errm);
	 }
	 return rez;
}
vector<int> UnitDescription::GetRLByName(char* n)
{
	vector<int> rez; 
	int sz=RLVector.size();
	for (int i=0;i<sz;i++)
	{
		if (!strcmp(RLVector[i].Name,n))
			return RLVector[i].RL.Amo;
	}
	return rez;
}
bool UnitDescription::ReadRL(GFILE* f, char* name)
{
	bool Done, ende=true;
	NamedResourceList NRL;
	char ccc[257];
	char atom[64];
	int vle;
	strcpy(NRL.Name,name);
	do{
		Done=ReadWinString(f,ccc,256);
		if(strstr(ccc,"\\RESOURCELIST"))
		{
			RLVector.push_back(NRL);
			ende=false;
		}
		else
		if(sscanf(ccc,"%s %d",atom, &vle))
		{
			NRL.RL.SetAmo(NRL.RL.GetIndex(atom),vle);
		}
	}while(Done&&ende);
	return true;
}
bool UnitDescription::ReadCP(GFILE* f)
{
	bool Done, ende=true;
	char ccc[257];
	do{
		Done=ReadWinString(f,ccc,256);
		Can_produse Can;
		if(strstr(ccc,"\\CAN_PRODUCE"))
		{
			ende=false;
		}
		else
		if(sscanf(ccc,"%s %d",Can.Name, &Can.Param))
		{
			CP.push_back(Can);
		}
	}while(Done&&ende);
	return true;
}
//-------------------------------------------//
int FindIfUnitDescription::NType=-1;
//-------------------------------------------//
Manufacture::Manufacture()
{
	IsWork=false;
}
Manufacture::~Manufacture()
{}
void Manufacture::FillParams(UnitDescription* UD, int Index)
{
	Warehouse::FillParams(UD, Index);
	//ProduceRL.LoadValue(UD->GetRLByName("PRODUCE"));
	//Population=UD->StartPopulation;
	int sz=UD->CP.size();
	for (int i=0;i<sz;i++)
	{
		int n;
		if(UD->CP[i].Param==1)
			n=PL.GetProduceIndex(InProduce.GetIndex(UD->CP[i].Name));
		else
		if(UD->CP[i].Param==2)
			n=PL.GetProduceIndexByUnitName(UD->CP[i].Name);
		if(n>-1)
			ProduceListIndex.push_back(n);
	}
}
void Manufacture::Process(Warehouse **TradePoints, int TPSize)
{
	//find work
	int am=ProduceListIndex.size();
	list<int>::iterator pli;
	
	bool inproduse=false;
	if(InProduceStack.size())
		inproduse=true;
	else
	for (int i=0;i<am;i++)
	{
		/*for (pli=InProduceStack.begin();!inproduse&&pli!=InProduceStack.end();++pli)
		{
			if (*pli==ProduceListIndex[i])
				inproduse=true;
		}
		*/
		int sum=0;
		if(!inproduse)
		{
			for(int w=0;w<TPSize;w++)
			{
				if (PL.GetThing(ProduceListIndex[i])->Type==1)
				{
					if(Unit.GetIndex()!=TradePoints[w]->Unit.GetIndex())
					if((Unit.OB->NMask&TradePoints[w]->Unit.OB->NMask)&&(TradePoints[w]->WarehouseType==0))
					{
						int res=PL.GetThing(ProduceListIndex[i])->ResourceID;
						sum+=TradePoints[w]->NeededResourcesRL.GetAmo(res)-TradePoints[w]->AvailableRL.GetAmo(res);
					}
				}
			}
		}
		if(sum>0)
		{
			InProduceStack.push_back(ProduceListIndex[i]);
			for (int q=0;q<sum;q++)
				InProduceStack.push_back(ProduceListIndex[i]);
		}
		if (PL.GetThing(ProduceListIndex[i])->Type==1)
		{
			int res=PL.GetThing(ProduceListIndex[i])->ResourceID;
			if(AvailableRL.GetAmo(res)<20)
				InProduceStack.push_back(ProduceListIndex[i]);
		}
	}
	
	if(InProduceStack.size())
	{
		//Produce
		pli=InProduceStack.begin();
		Thing* v=&(PL.Things[*pli]);
		if (!IsWork)
		{
			InProduce.ClearAmo();
			ProduceResAmo=0;
			for (int i=0;i<InProduce.ResourceAmount;i++)
				ProduceResAmo+=v->SpendRL.GetAmo(i);
			GlobalTime=NowTime;
			IsWork=true;
			ProduceTime=0;
			InProduceTotalAmo=0;
		}
		else
		{
			int n=0;
			if (v->Time)
				n=(NowTime-GlobalTime)*ProduceResAmo/v->Time; 
			else
				n=ProduceResAmo;
			if(n)
			{
				GlobalTime=NowTime;
				for (int i=0;i<InProduce.ResourceAmount;i++)
				{
					int nnd=v->SpendRL.GetAmo(i);
					int sp=InProduce.GetAmo(i);
					if (n&&nnd>sp)
					{
						int av=AvailableRL.GetAmo(i);
						if(av)
						{
							if((nnd-sp)<=av)
								av=nnd-sp;
							else
								;//deficit
							if(av>n)av=n;
							n-=av;
							InProduce.AddAmo(i,av);
							AddResource(i,-av);
							ProduceTime+=av*v->Time/ProduceResAmo;
							InProduceTotalAmo+=av;
						}
						else
							;//deficit
					}
				}
			}
			if ((ProduceResAmo==InProduceTotalAmo)||(ProduceTime>=v->Time))
			{
				IsWork=false;
				InProduceStack.pop_front();
				InProduce.ClearAmo();
				ProduceResAmo=0;
				ProduceTime=0;
				InProduceTotalAmo=0;
				if(v->Type==1)
				{
					AddResource(v->ResourceID,1);
				}
				else
				if(v->Type==2)
				{
					//ProduseUnit
					ClearGroup(&GD1);
					//ClearSelection(Unit.OB->NNUM);
					//AddUnitToSel(Unit.GetIndex());
					//SaveSelectedUnits(Unit.OB->NNUM,&GD1,0);
					InsertUnitToGroup(&GD1,Unit.GetIndex());
					GD2.Type='UTYP';
					GD2.Index=v->UnitTypeID;
					ProduceUnitFast(&GD1,&GD2,&GD3,0);
				}
				else
				if(v->Type==3)
				{
					//InitialUpgrade
				}
			}
		}
	}
	//order resources
	NeededResourcesRL.ClearAmo();
	for (pli=InProduceStack.begin();pli!=InProduceStack.end();++pli)
	{
		NeededResourcesRL.AddValueVector(PL.GetThing(*pli)->SpendRL.Amo);
	}
	//ProofPrice();
}
void Manufacture::ShowParams()
{
	Warehouse::ShowParams();
	char s[1024], sc[1024];
	strcpy(s,"InProduce resources: ");
	for (int i=0;i<AvailableRL.ResourceAmount;i++)
	{   
		if(InProduce.GetAmo(i)>0)
		{
			strcpy(sc,s);
			sprintf(s,"%s %s-%d:%d",sc,InProduce.GetName(i),InProduce.GetAmo(i),GetPrice(i));
		}
	}
	AssignHint1(s,60);
}
void Manufacture::CMLunitsList(char* box)
{
	//box[0]=0;
	int sz=ProduceListIndex.size();
	for(int i=0;i<sz;i++)
	{
		if(PL.GetThing(ProduceListIndex[i])->Type==2)
		{
			sprintf(box,"%s#txt[%%T](%%EB[x:10,y:10+%d,w:100%%-20,h:24],{DS|e_produce&%d&2&%d},\"{%s - %d}\")\n",box,i*20,Unit.GetIndex(),ProduceListIndex[i],PL.GetThing(ProduceListIndex[i])->UnitTypeName,GetProductPrice(ProduceListIndex[i]));
		}
		if(PL.GetThing(ProduceListIndex[i])->Type==1)
		{
			sprintf(box,"%s#txt[%%T](%%EB[x:10,y:10+%d,w:100%%-20,h:24],{DS|e_produce&%d&1&%d},\"{%s - %d}\")\n",box,i*20,Unit.GetIndex(),ProduceListIndex[i],AvailableRL.GetName(PL.GetThing(ProduceListIndex[i])->ResourceID),GetProductPrice(ProduceListIndex[i]));
		}
	}
}
void Manufacture::CMLInProduceStack(char* box)
{
	int sz=InProduceStack.size();
	if(sz)
	{
		strcpy(box,"#txt[%T](%EB[x:10,y:%T+20,w:100%-20,h:24],{},\"InProduceStack\")\n");
		list<int>::iterator p=InProduceStack.begin();
		int itm=*p;
		int co=0;
		char tm[2048];
		for(int i=0;i<(sz+1);i++)
		{
			if((i<sz)&&(*p==itm))
			{
				co++;
			}
			else
			{
				strcpy(tm,box);
				if(PL.GetThing(itm)->Type==2)
					sprintf(box,"%s#txt[%%T](%%EB[x:20,y:%%T,w:100%%-30,h:24],{},\"%s > %d\")\n",tm,PL.GetThing(itm)->UnitTypeName,co);
				if(PL.GetThing(itm)->Type==1)
					sprintf(box,"%s#txt[%%T](%%EB[x:20,y:%%T,w:100%%-30,h:24],{},\"%s > %d\")\n",tm,InProduce.GetName(PL.GetThing(itm)->ResourceID),co);
				itm=*p;
				co=1;
			}
			if(i<sz)p++;
		}
	}
}
void Manufacture::CMLobjectInfo(char* file)
{
	DString DS;
	DS.ReadFromFile("Eco\\cml\\head.cml");
	char box[4096];
	box[0]=0;
	CMLunitsList(box);
	DS.Add(box);
	box[0]=0;
	CMLInProduceStack(box);
	DS.Add(box);
	DS.WriteToFile(file);
}
int Manufacture::GetProductPrice(int PLIndex)
{
	int rez=0;
	//Thing *Th=PL.GetThing(ProduceListIndex[PLIndex]);
	Thing *Th=PL.GetThing(PLIndex);
	if(Th)
	{
		for (int i=0;i<InProduce.ResourceAmount;i++)
			rez+=Th->SpendRL.GetAmo(i)*Warehouse::GetPrice(i);
	}
	return rez+(rez*Profit)/100;
}
int Manufacture::GetPrice(int ResourceID)
{
	int re=PL.GetProduceIndex(ResourceID);
	if(re>-1)
	{
		for(int i=0;i<ProduceListIndex.size();i++)
			if(ProduceListIndex[i]==re)
				return GetProductPrice(re);
	}
	return Warehouse::GetPrice(ResourceID);
}
ProduceList Manufacture::PL;
//-------------------------------------------//
Formation::Formation()
{
	//strcpy(FormTypeTxt,"#LINE120");
}
Formation::~Formation()
{}
//-------------------------------------------//
FormationList::FormationList()
{}
FormationList::~FormationList()
{}
void FormationList::FillFromFile(char *file)
{
	GFILE* 	F=Gopen(file,"r");
	if(F)
	{
		char ccc[257], ss[64];
		char  fname[64], sold[64], off[64], bar[64], flg[64], form[64];
		int amo;
		bool Done;
		bool StartRead=false;
		ProduceList PL;
		do{
		Done=ReadWinString(F,ccc,256);
			if(Done&&ccc[0]!='/')
			{
				if(sscanf(ccc,"%s",ss))
				{
					if (!strcmp(ss,"[FORMATIONS]"))
					{
						StartRead=true;
					}
					else
					if (!strcmp(ss,"[\\FORMATIONS]"))
					{
						Done=false;
						StartRead=false;
					}
					else
					if(StartRead&&sscanf(ccc,"%s %s %d %s %s %s %s",fname,sold,&amo,off,bar,flg,form))
					{
						 Formation Fo;
						 strcpy(Fo.name,fname);
						 strcpy(Fo.FormTypeTxt,form);
						 Fo.KolUnitsInForm=amo;
						 Fo.ProduceListUnitID=PL.GetProduceIndexByUnitName(sold);
						 Fo.UnitTh=PL.GetThing(Fo.ProduceListUnitID);
						 Fo.ProduceListOffID=PL.GetProduceIndexByUnitName(off);
						 Fo.OffTh=PL.GetThing(Fo.ProduceListOffID);
						 Fo.ProduceListBarID=PL.GetProduceIndexByUnitName(bar);
						 Fo.BarTh=PL.GetThing(Fo.ProduceListBarID);
						 Fo.ProduceListFlagID=PL.GetProduceIndexByUnitName(flg);
						 Fo.FlagTh=PL.GetThing(Fo.ProduceListFlagID);
						 for(int i=0;i<Fo.NeedRL.ResourceAmount;i++)
						 {
							 if(Fo.UnitTh)Fo.NeedRL.AddAmo(i,Fo.UnitTh->SpendRL.GetAmo(i)*Fo.KolUnitsInForm);
							 if(Fo.OffTh)Fo.NeedRL.AddAmo(i,Fo.OffTh->SpendRL.GetAmo(i));
							 if(Fo.BarTh)Fo.NeedRL.AddAmo(i,Fo.BarTh->SpendRL.GetAmo(i));
							 if(Fo.FlagTh)Fo.NeedRL.AddAmo(i,Fo.FlagTh->SpendRL.GetAmo(i));
						 }
						 FL.push_back(Fo);
					}
				}
			}
		}while(Done);
		F->Close();
	}
	else
	{
		char mm[128];
		sprintf(mm,"Unable to open %s",file);
		ErrM(mm);
	}
}
vector<Formation> FormationList::FL;
//-------------------------------------------//
AutoFormation::AutoFormation()
{
	RegisterDynGroup(&Officers);
	RegisterDynGroup(&Drummers);
	RegisterDynGroup(&FlagBearers);
	RegisterDynGroup(&Soldiers);
	RegisterDynGroup(&DestGroup);
}
AutoFormation::~AutoFormation()
{}
void AutoFormation::CreateForm()
{
	OneUnit U;
	GetUnitInfo(&Officers,0,&U);
	OneObject* OB=GetOBJ(U.Index);
	CreateFormation(OB->NNUM,&Officers,&Drummers,&FlagBearers,&Soldiers,&DestGroup,Form->FormTypeTxt);
}
void AutoFormation::MoveTo(int x, int y)
{
	OneUnit U;
	GetUnitInfo(&DestGroup,0,&U);
	OneObject* OB=GetOBJ(U.Index);
	SGP_MoveToPoint(OB->NNUM,&DestGroup,x,y,512,0,0,0);
}
void AutoFormation::Clear()
{
	EraseGroup(&Officers);
	EraseGroup(&Drummers);
	EraseGroup(&FlagBearers);
	EraseGroup(&Soldiers);
	EraseGroup(&DestGroup);
}
bool AutoFormation::IsFormationReady()
{
	bool rez=false;
	if(Form)
	{
		rez=true;
		if(Form->OffTh)
		{
			if(GetTotalAmount0(&Officers)!=1)
				rez=false;
		}
		if(Form->BarTh)
		{
			if(GetTotalAmount0(&Drummers)!=1)
				rez=false;
		}
		if(Form->FlagTh)
		{
			if(GetTotalAmount0(&FlagBearers)!=1)
				rez=false;
		}
		if(GetTotalAmount0(&Soldiers)!=Form->KolUnitsInForm)
			rez=false;
	}
	return rez;
}
//-------------------------------------------//
Barrack::Barrack()
{
	IsProduce=false;
	for(int i=0;i<Formations.FL.size();i++)
		ProduceList.push_back(i);
	TradePointsSorted=NULL;
	MoneyToOrder=0;
	InitFlag=false;
}
Barrack::~Barrack()
{
	if (TradePointsSorted)free(TradePointsSorted);
}
void Barrack::Process()
{
	Produce();
}
void Barrack::Produce()
{
	if(InProduceStack.size())
	{
		if(!IsProduce)
		{
			if(StartProduce())
				IsProduce=true;
		}
		else
			if(FormationInProduce.IsFormationReady())
			{
				InProduceStack.pop_front();
				FormationInProduce.CreateForm();
				IsProduce=false;
			}
	}
	else
		IsProduce=false;
}
bool Barrack::StartProduce()
{
	FormationInProduce.Clear();
	int in=InProduceStack.front();
	Formation* F=&(Formations.FL[in]);
	for(int i=0;i<F->NeedRL.ResourceAmount;i++)
		if(AvailableRL.GetAmo(i)<F->NeedRL.GetAmo(i))
			return false;

	bool isdiffict=false;
	if(F)
	{
		FormationInProduce.Form=F;
		if(F->OffTh)
			isdiffict=ProduceFast(F->OffTh,&FormationInProduce.Officers);
		if(F->BarTh)
			isdiffict=ProduceFast(F->BarTh,&FormationInProduce.Drummers);
		if(F->FlagTh)
			isdiffict=ProduceFast(F->FlagTh,&FormationInProduce.FlagBearers);
		if(isdiffict)
		for(int i=0;i<F->KolUnitsInForm;i++)
			isdiffict=ProduceFast(F->UnitTh,&FormationInProduce.Soldiers);
	}
	return true;
}
bool Barrack::ProduceFast(Thing *Chto,GAMEOBJ* Kuda)
{
	bool rez=true;
	if(Chto)
	{
		for(int i=0;i<AvailableRL.ResourceAmount;i++)
			if(AvailableRL.GetAmo(i)<Chto->SpendRL.GetAmo(i))
				rez=false;
		if(rez)
		{
			for(i=0;i<AvailableRL.ResourceAmount;i++)
				AvailableRL.AddAmo(i,-Chto->SpendRL.GetAmo(i));
			ClearGroup(&GD1);
			InsertUnitToGroup(&GD1,GetIndex());
			GD2.Type='UTYP';
			GD2.Index=Chto->UnitTypeID;
			ProduceUnitFast(&GD1,&GD2,Kuda,1);
		}
	}
	return rez;
}
void Barrack::FillParams(UnitDescription* UD, int Index)
{
	SetIndex(Index);
	AvailableRL.LoadValue(UD->GetRLByName("AVAILABLERESOURCES"));
	OB=GetOBJ(Index);
}
void Barrack::CMLunitsList(char* box)
{
	int sz=ProduceList.size();
	for(int i=0;i<sz;i++)
	{
		int expen;
		int price=GetFormationsPrice(ProduceList[i],expen);
		//sprintf(box,"%s#txt[%%T](%%EB[x:10,y:10+%d,w:100%%-20,h:24],{DS|e_prodform&%d&2&%d},\"{%s - %d(%d)}\")\n",box,i*20,GetIndex(),ProduceList[i],Formations.FL[i].name,price,expen);
		sprintf(box,"%s#txt[%%T](%%EB[x:10,y:10+%d,w:100%%-20,h:24],{DS|e_orderform&%d&%d},\"{%s - %d(%d)}\")\n",box,i*20,GetIndex(),ProduceList[i],Formations.FL[i].name,price,expen);
	}
}
void Barrack::CMLInProduceStack(char* box)
{
	int sz=InProduceStack.size();
	if(sz)
	{
		strcpy(box,"#txt[%T](%EB[x:10,y:%T+20,w:100%-20,h:24],{},\"InProduceStack\")\n");
		list<int>::iterator p=InProduceStack.begin();
		int itm=*p;
		int co=0;
		char tm[2048];
		for(int i=0;i<(sz+1);i++)
		{
			if((i<sz)&&(*p==itm))
			{
				co++;
			}
			else
			{
				strcpy(tm,box);
				sprintf(box,"%s#txt[%%T](%%EB[x:20,y:%%T,w:100%%-30,h:24],{},\"%s > %d\")\n",tm,Formations.FL[itm].name,co);
				itm=*p;
				co=1;
			}
			if(i<sz)p++;
		}
	}
}
void Barrack::CMLOrderList(char *box)
{
	int sz=Order.size();
	if(sz)
	{
		strcpy(box,"#txt[%T](%EB[x:10,y:%T+20,w:100%-20,h:24],{},\"OrderList\")\n");
		list<int>::iterator p=Order.begin();
		int itm=*p;
		int co=0;
		char tm[2048];
		for(int i=0;i<(sz+1);i++)
		{
			if((i<sz)&&(*p==itm))
			{
				co++;
			}
			else
			{
				strcpy(tm,box);
				sprintf(box,"%s#txt[%%T](%%EB[x:20,y:%%T,w:100%%-30,h:24],{},\"%s > %d\")\n",tm,Formations.FL[itm].name,co);
				itm=*p;
				co=1;
			}
			if(i<sz)p++;
		}
		strcpy(tm,box);
		int in=GetIndex();
		sprintf(box,"%s#txt[%%T](%%EB[x:10,y:%%T+20,w:100%%-20,h:24],{DS|e_prodorder&%d},\"{MakeOrder}\")\n",tm,in);
	}

}
void Barrack::CMLobjectInfo(char *file)
{
	DString DS;
	DS.ReadFromFile("Eco\\cml\\head.cml");
	char box[4096];
	box[0]=0;
	CMLunitsList(box);
	DS.Add(box);
	box[0]=0;
	CMLInProduceStack(box);
	DS.Add(box);
	box[0]=0;
	CMLOrderList(box);
	DS.Add(box);
	DS.WriteToFile(file);
}
int Barrack::GetFormationsPrice(int FormationsID, int &Expensive)
{
	int price=0;
	Expensive=0;
	ResourceList *NRL=&Formations.FL[FormationsID].NeedRL;
	for(int i=0;i<NRL->ResourceAmount;i++)
	{
		int NDR=NRL->GetAmo(i);
		if(NDR)
		{
			int br=OrderRL.GetAmo(i);
			for(int tp=0;(TradePointsSorted[TradePointsSize*i+tp]!=0)&&NDR;tp++)
			{
				Warehouse* W=TradePointsSorted[TradePointsSize*i+tp];
				//Manufacture* W=(Manufacture*)TradePointsSorted[TradePointsSize*i+tp];
				int av=W->AvailableRL.GetAmo(i)-W->NeededResourcesRL.GetAmo(i);
				if(av>0)
				{
					if(br>av)br-=av;
					else
					{
						av-=br;
						if(av>=NDR)
						{
							price+=NDR*W->GetPrice(i);
							if(W->WarehouseType==1)Expensive+=NDR*W->GetPrice(i);
							//OrderRL.AddAmo(i,NDR);
							NDR=0;
						}
						else
						{
							NDR-=av;
							price+=av*W->GetPrice(i);
							if(W->WarehouseType==1)Expensive+=av*W->GetPrice(i);
							//OrderRL.AddAmo(i,av);
						}
					}
				}
			}
		}
	}
	return price;
}
int Barrack::GetFormationsPriceAndReservRes(int FormationsID)
{	
	int price=0;
	ResourceList *NRL=&Formations.FL[FormationsID].NeedRL;
	for(int i=0;i<NRL->ResourceAmount;i++)
	{
		int NDR=NRL->GetAmo(i);
		if(NDR)
		{
			int br=NeededRL.GetAmo(i);
			for(int tp=0;(TradePointsSorted[TradePointsSize*i+tp]!=0)&&NDR;tp++)
			{
				Warehouse* W=TradePointsSorted[TradePointsSize*i+tp];
				int av=W->AvailableRL.GetAmo(i)-W->NeededResourcesRL.GetAmo(i);
				if(av>0)
				{
					if(br>av)br-=av;
					else
					{
						av-=br;
						if(av>=NDR)
						{
							price+=NDR*W->GetPrice(i);
							W->MakeGovernmentOrder(i,NDR);
							NeededRL.AddAmo(i,NDR);
							NDR=0;
						}
						else
						{
							NDR-=av;
							price+=av*W->GetPrice(i);
							W->MakeGovernmentOrder(i,av);
							NeededRL.AddAmo(i,av);
						}
					}
				}
			}
		}
	}
	return price;
}
void Barrack::FillTradePoints(Warehouse **TradePoints, int TPSize)
{
	if(TradePointsSorted)free(TradePointsSorted);
	TradePointsSize=TPSize;
	int ResAm=NeededRL.ResourceAmount;
	TradePointsSorted = (Warehouse**)malloc((ResAm*TPSize)<<2);
	memset(TradePointsSorted,0,(ResAm*TPSize)<<2);
	ThisBilding=(UnitInfo*)this;
	for(ResID=0;ResID<ResAm;ResID++)
	{
		int ColTP=0;
		for(int i=0;i<TPSize;i++)
		{
			if(OB->NMask&TradePoints[i]->Unit.OB->NMask)
				if((TradePoints[i]->AvailableRL.GetAmo(ResID)-TradePoints[i]->NeededResourcesRL.GetAmo(ResID)+TradePoints[i]->Booked.GetAmo(ResID))>0)
				{
					TradePointsSorted[TPSize*ResID+ColTP]=TradePoints[i];
					ColTP++;
				}
		}
		qsort(TradePointsSorted+TPSize*ResID,ColTP,4,Compare);
	}
}
int Barrack::Compare(const void *ar1, const void *ar2)
{
	int rez=0;
	Warehouse* W1= *(Warehouse**)ar1;
	Warehouse* W2= *(Warehouse**)ar2;
	
	if(W1&&W2)
	{
		int dist1=ThisBilding->GetDist(W1->Unit.x-(ThisBilding->OB->RealX>>4),W1->Unit.y-(ThisBilding->OB->RealY>>4));
		int dist2=ThisBilding->GetDist(W2->Unit.x-(ThisBilding->OB->RealX>>4),W2->Unit.y-(ThisBilding->OB->RealY>>4));
		if(((W1->GetPrice(ResID)*1000)/dist1)<((W2->GetPrice(ResID)*1000)/dist2))rez=1;
		else
		if(((W1->GetPrice(ResID)*1000)/dist1)>((W2->GetPrice(ResID)*1000)/dist2))rez=-1;
	}
	return rez;
}
Barrack* Barrack::This()
{
	return this;
}
int Barrack::ResID=0;
FormationList Barrack::Formations;
UnitInfo* Barrack::ThisBilding=NULL;
//-------------------------------------------//
Bearer::Bearer()
{
	State=0;
	RegisterDynGroup(&Dyn);
	RegisterUnitType(&GType,"HGu(NW)");
}
Bearer::~Bearer()
{}
void Bearer::Load(Barrack* Owner)
{
	State=2;
	for(int i=0;i<RL.ResourceAmount;i++)
	{
		int vlm=Target->Booked.GetAmo(i);
		int pr=Target->BookedPrice.GetAmo(i);
		if(Money<(vlm*pr))
			vlm=Money/pr;
		Target->Booked.AddAmo(i,-vlm);
		RL.AddAmo(i,vlm);
		Money-=vlm*pr;
		Target->Sel(NULL,vlm*pr);
	}
}
void Bearer::UnLoad(Barrack* Owner)
{
	for(int i=0;i<RL.ResourceAmount;i++)
	{
		Owner->AvailableRL.AddAmo(i,RL.GetAmo(i));
		Owner->NeededRL.AddAmo(i,-RL.GetAmo(i));
		Owner->PoehalZa.AddAmo(i,-RL.GetAmo(i));
		RL.SetAmo(i,0);
	}
	State=0;
	Erase();
}
void Bearer::Erase()
{
	DieUnit(GetIndex());
	State=0;
	Target=NULL;
}
void Bearer::Create(int x, int y, byte Nat)
{
	GAMEOBJ Zon;
	Zon.Index=x;
	Zon.Serial=y;
	Zon.Type= 0x40000000 + 200;
	CreateObject0(&Dyn,&ONE,&GType,Nat,&Zon,0);
	OneUnit U;
	GetUnitInfo(&Dyn,0,&U);
	SetIndex(U.Index);
	Target=NULL;
	Money=0;
	OB=GetOBJ(U.Index);
	State=0;
}
//-------------------------------------------//
Carrier::Carrier()
{
	Timer=0;
	TimerDelay=50;
	OwnerW=NULL;
	OwnerB=NULL;
}
Carrier::~Carrier()
{}
void Carrier::Process()
{
	if(OwnerB)
	{
		for(int i=0;i<BearerList.size();i++)
		{ 
			Bearer* B=BearerList[i];
			if(B->State==1)
			{
				if ((B->GetDist(B->Target->Unit.x-(B->OB->RealX>>4),B->Target->Unit.y-(B->OB->RealY>>4))<200))//priehali	
					B->Load(OwnerB);
				else
					B->MoveTo(B->Target->Unit.x,B->Target->Unit.y);
			}
			if(B->State==2)
			{
				if ((B->GetDist((OwnerB->OB->RealX>>4)+200-(B->OB->RealX>>4),(OwnerB->OB->RealY>>4)+200-(B->OB->RealY>>4))<200))//priehali	
					B->UnLoad(OwnerB);
				else
					B->MoveTo((OwnerB->OB->RealX>>4)+200,(OwnerB->OB->RealY>>4)+200);
			}
		}

		for(i=0;i<OwnerB->NeededRL.ResourceAmount;i++)
		{
			if((OwnerB->NeededRL.GetAmo(i)-OwnerB->PoehalZa.GetAmo(i))>0)
			{
				Warehouse* W=OwnerB->TradePointsSorted[OwnerB->TradePointsSize*i];
				for(int j=1;W!=NULL;j++)
				{
					bool ff=true;
					for(int b=0;(b<BearerList.size())&&ff;b++)
						if(W==BearerList[b]->Target)
							ff=false;
					for(b=0;ff&&(b<BearerList.size());b++)
						if(BearerList[b]->State==0)
						{
							//create and move
							BearerList[b]->Create((OwnerB->OB->RealX>>4)+200,(OwnerB->OB->RealY>>4)+200,OwnerB->OB->NNUM);
							BearerList[b]->State=1;
							BearerList[b]->Target=W;
							int msum=0;
							for(int k=0;k<W->Booked.ResourceAmount;k++)
							{
								int gr=W->Booked.GetAmo(k);
								int ndd=OwnerB->NeededRL.GetAmo(k)-OwnerB->PoehalZa.GetAmo(k);
								if(gr&&(ndd>0))
								{
									int vlm=0;
									if(gr>ndd)
										vlm=ndd;
									else
										vlm=gr;
									OwnerB->PoehalZa.AddAmo(k,vlm);
									msum+=vlm*W->BookedPrice.GetAmo(k);
									if((k==i)&&(vlm>=ndd))
										ff=false;
								}
							}
							BearerList[b]->Money=msum;
							OwnerB->MoneyToNeededRes-=msum;
							break;
						}
					if(ff)
						W=OwnerB->TradePointsSorted[OwnerB->TradePointsSize*i+j];
					else
						W=NULL;
				}
			}
		}
	}
}
//-------------------------------------------//
NationParams::NationParams()
{
	Nalog=20;
	Kazna=1000000;
	Profit=50;
}
NationParams::~NationParams()
{}
void NationParams::Init(int Ni)
{
	Nalog=20;
	Kazna=1000000;
	Profit=50;
	NI=Ni;
	SetResource(NI,GOLD,Kazna);
}
void NationParams::MoneyAdd(int Sum)
{
	Kazna+=Sum;
	SetResource(NI,GOLD,Kazna);
}
//-------------------------------------------//
InterfaceOBJ** InterfaceOBJ::FLIndex[512];
int InterfaceOBJ::ArrSZ=256;

InterfaceOBJ::InterfaceOBJ()
{
	//
}
InterfaceOBJ::~InterfaceOBJ()
{
	//for(int i=0;i<ArrSZ;i++)
	//	if(FLIndex[i])free((FLIndex[i]));
}
void InterfaceOBJ::Show()
{
	//
}
void InterfaceOBJ::AddNewIndex(int Index, InterfaceOBJ* OBJ)
{
	/*if(!FLIndex)
	{
		FLIndex = (InterfaceOBJ***)malloc(ArrSZ<<2);	
		memset(FLIndex,0,ArrSZ<<2);
	}
	*/
	if(Index>0)
	{
		int FL=Index>>8;
		int SL=Index&255;
		if(FLIndex[FL])
			FLIndex[FL][SL]=OBJ;
		else
		{
			FLIndex[FL] = (InterfaceOBJ**)malloc(ArrSZ<<2);	
			memset(FLIndex[FL],0,ArrSZ<<2);
			FLIndex[FL][SL]=OBJ;
		}
	}
}
bool InterfaceOBJ::Show(word Index)
{
	int FL=Index>>8;
	int SL=Index&255;
	if((FLIndex[FL]))
		if(FLIndex[FL][SL])
		{
			FLIndex[FL][SL]->Show();
			return true;
		}
	return false;
}
//-------------------------------------------//



















////////////
Troop::Troop()
{
	OB=NULL;
	Index=-1;
}
Troop::Troop(int In)
{
	Troop();
	Index=In;
	OB=GetOBJ(Index);
	Weight=0;
	Strength=100;
	Speed=10;
}
void Troop::Move(int x, int y)
{
	OBJ_SendTo(Index,x,y,128+1,0);
}
Troop::~Troop()
{}
//////////
FormMovement::FormMovement()
{
	FormList=NULL;
	FormListSize=0;
	EnmFormList=NULL;
	EnmFormListSize=0;
	ImportantPointSize=0;
	TIMER=0;
	CounterI=0;
	//
	ImportantPointSize=1;
	IP[0].Weight=15;
	IP[0].x=6000;
	IP[0].y=4000;
}
FormMovement::~FormMovement()
{
	for(int i=0;i<FormListSize;i++)
		delete FormList[i];
	for(i=0;i<EnmFormListSize;i++)
		delete EnmFormList[i];
	if (FormList)free(FormList);

}
void FormMovement::ResearchMap()
{
	int MAXOB = GetMaxObject();
	
	if(FormList)free(FormList);
	FormList = (Troop**)malloc(MAXOB<<2);
	if (EnmFormList)free(EnmFormList);
	EnmFormList = (Troop**)malloc(MAXOB<<2);
	
	for(int i=0;i<MAXOB;i++)
	{
		OneObject* OB=GetOBJ(i);
		if(OB)
		{
			if(OB->NNUM!=0)
			{
				FormList[FormListSize] = new Troop(i);
				FormListSize++;
			}
			else
			{
				EnmFormList[EnmFormListSize] = new Troop(i);
				EnmFormListSize++;
			}
		}
	}
}
void FormMovement::Process()
{
	FillImportantPoint();
	if(GetGlobalTime()>(TIMER+10))
	{
		if(CounterI<FormListSize)
		{
			int x=-1,y=-1;
			Troop* T=FormList[CounterI];
			GetDangerCoord(T,x,y);
			if(x>-1&&y>-1)
				T->Move(x,y);
			CounterI++;
		}
		else
		{
			CounterI=0;
			Flog=fopen("Eco\\CachBug.log","w");	
			for(int i=0;i<64;i++)
			{
				for(int j=0;j<64;j++)
					fprintf(Flog,"%d\t",DangMap[i][j]-ForceMap[i][j]);
				fprintf(Flog,"\n");
			}
			if(Flog)fclose(Flog);
		}
		TIMER=GetGlobalTime();
	}
}
void FormMovement::FillImportantPoint()
{
	memset(&DangMap,0,64*64*sizeof(int));
	memset(&ForceMap,0,64*64*sizeof(int));

	for(int i=0;i<EnmFormListSize;i++)
	{
		Troop* T=EnmFormList[i];
		for(int j=0;j<ImportantPointSize;j++)
		{
			ImportantPoint* ip=&(IP[j]);
			int dist=GetDist(T,ip);
			if(dist>0)
			{
				int Wadd=200;//(T->Strength*200)/(dist/T->Speed);
				T->Weight=Wadd;
				//ip->Weight+=Wadd*2;
				int sx=((ip->x*2+(T->OB->RealX>>4))/3)>>8;
				int sy=((ip->y*2+(T->OB->RealY>>4))/3)>>8;
				AddEnerPoint(DangMap,Wadd, sx, sy);
			}
		}
	}	
	for(i=0;i<FormListSize;i++)
	{
		Troop* T=FormList[i];
		for(int j=0;j<ImportantPointSize;j++)
		{
			ImportantPoint* ip=&(IP[j]);
			int dist=GetDist(T,ip);
			if(dist>0)
			{
				T->Weight=100;//(T->Strength*100*T->Speed)/dist;
				AddEnerPoint(ForceMap,T->Weight,T->OB->RealX>>12,T->OB->RealY>>12);
			}
		}
	}
}
int FormMovement::GetDist(Troop* T1,Troop* T2)
{
	int rez=-1;
	if(T1&&T2)
		rez=Norma((T1->OB->RealX>>4)-(T2->OB->RealX>>4),(T1->OB->RealY>>4)-(T2->OB->RealY>>4));
	return rez;
}
int FormMovement::GetDist(Troop* T1,ImportantPoint* IP)
{
	int rez=-1;
	if(T1&&IP)
		rez=Norma((T1->OB->RealX>>4)-IP->x,(T1->OB->RealY>>4)-IP->y);
	return rez;
}
void FormMovement::GetDangerCoord(Troop* Tr, int &x,int &y)
{
	if(Tr)
	{
		int tx=Tr->OB->RealX>>12;
		int ty=Tr->OB->RealY>>12;
		int max=0;
		int mx=ty,my=tx;
		for(int i=0;i<20;i++)
		{
			for(int j=0;j<20;j++)
			{
				int qm=GetDangFor(Tr,tx+i-10,ty+j-10);
				if(qm>max){max=qm; mx=tx+i-10; my=ty+j-10;}
			}
		}
		x=mx<<8;
		y=my<<8;
	/*int tx=Tr->OB->RealX>>12;
		int ty=Tr->OB->RealY>>12;
		if(x<0&&y<0)
		{
			x=tx; y=ty;
			GetDangerCoord(Tr,x,y);
		}
		else
		{
			int W=GetDangFor(Tr,x,y);
			int max=W;
			int rx=x;
			int ry=y;
			
			int Mx=x+1;int My=y+1;int PreW=GetDangFor(Tr,Mx,My);if(max<PreW){max=PreW;rx=Mx;ry=My;}
			Mx=x-1;My=y+1;		      PreW=GetDangFor(Tr,Mx,My);if(max<PreW){max=PreW;rx=Mx;ry=My;}
			Mx=x;My=y+1;              PreW=GetDangFor(Tr,Mx,My);if(max<PreW){max=PreW;rx=Mx;ry=My;}

			Mx=x+1;My=y-1;            PreW=GetDangFor(Tr,Mx,My);if(max<PreW){max=PreW;rx=Mx;ry=My;}
			Mx=x;My=y-1;              PreW=GetDangFor(Tr,Mx,My);if(max<PreW){max=PreW;rx=Mx;ry=My;}
			Mx=x-1;My=y-1;            PreW=GetDangFor(Tr,Mx,My);if(max<PreW){max=PreW;rx=Mx;ry=My;}

			Mx=x+1;My=y;              PreW=GetDangFor(Tr,Mx,My);if(max<PreW){max=PreW;rx=Mx;ry=My;}
			Mx=x-1;My=y;              PreW=GetDangFor(Tr,Mx,My);if(max<PreW){max=PreW;rx=Mx;ry=My;}


			if(max>W)
			{
				x=rx; y=ry;
				if(x<0||y<0)
				{
					x<<=8;
					y<<=8;
				}
				else
					GetDangerCoord(Tr,x,y);
			}
			else
			{
				x<<=8;
				y<<=8;
			}
		}
		*/
	}
}
int FormMovement::GetDangFor(Troop* T, int x,int y)
{
	int tx=T->OB->RealX>>12;
	int ty=T->OB->RealY>>12;
	
	int PreW=0;
	if((x>-1)&&(x<64)&&(y>-1)&&(y<64))
	{
		PreW=DangMap[x][y]-ForceMap[x][y];
				
		int TF=0;
		int d=abs(x-tx);
		if(d<abs(y-ty))d=abs(y-ty);
		TF=T->Weight-T->Weight*d/10;
		if(TF<0)TF=0;
		PreW+=TF;
	}
	return PreW; 
}
void FormMovement::AddEnerPoint(tp2D& Map, int Force, int sx, int sy)
{
	int scal=10;
	int nsx=sx-scal/2;
	int nsy=sy-scal/2;
	for(int i=0;i<scal;i++)
	{
		for(int j=0;j<scal;j++)
		{
			int n=i;
			if(j>i)n=j;
			int Fadd=Force-abs(scal/2-n)*Force/scal;
			int x=nsx+i;int y=nsy+j;
			if((x>-1)&&(x<64)&&(y>-1)&&(y<64))
				Map[x][y]+=Fadd;
			/*
			x=nsx+i; y=nsy-j;
			if((x>-1)&&(x<64)&&(y>-1)&&(y<64))
				Map[x][y]+=Fadd;
			x=nsx-i; y=nsy-j;
			if((x>-1)&&(x<64)&&(y>-1)&&(y<64))
				Map[x][y]+=Fadd;
			x=nsx-i; y=nsy+j;
			if((x>-1)&&(x<64)&&(y>-1)&&(y<64))
				Map[x][y]+=Fadd;
			*/
		}
	}
}
//////////