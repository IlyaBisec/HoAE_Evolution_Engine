//==================================================================================================================//
#include "stdheader.h"
#include "unitability.h"
#include "GameExtension.h"
#include "BrigadeAbilityExtension.h"
//==================================================================================================================//
void BrigadeAbilityExtension::OnUnloading()
{
	for(int j=0;j<8;j++)
	{
		City* C = &(CITY[j]);
		for(int i=0;i<C->NBrigs;i++)
		{
			Brigade* BRIG=&C->Brigs[i];
			if(BRIG->Enabled&&BRIG->Ability)
			{
				delete (BRIG->Ability);
				BRIG->Ability=NULL;
			}
		}
	}
}
bool BrigadeAbilityExtension::OnGameSaving(xmlQuote& xml)
{
	for(int j=0;j<8;j++)
	{
		City* C = &(CITY[j]);
		for(int i=0;i<C->NBrigs;i++)
		{
			Brigade* BRIG=&C->Brigs[i];
			if(BRIG->Enabled&&BRIG->Ability)
			{
				int n = BRIG->Ability->GetAmount();
				if(n)
				{
					xmlQuote* ua = new_xmlQuote("BrigadeAbilityList");
					ua->AddSubQuote("NI",j);
					ua->AddSubQuote("BrIndex",i);
					xmlQuote* ua0 = new_xmlQuote("Ability");
					BRIG->Ability->Save(*ua0,BRIG->Ability,NULL);
					ua->AddSubQuote(ua0);
					xml.AddSubQuote(ua);
				}
			}
		}
	}
	return true;
}
bool BrigadeAbilityExtension::OnGameLoading(xmlQuote& xml)
{
	int n = xml.GetNSubQuotes();
	for(int i=0;i<n;i++)
	{
		xmlQuote* ua = xml.GetSubQuote(i);
		if(ua&&ua->GetSubQuote(0)&&ua->GetSubQuote(1))
		{
			int NI = ua->GetSubQuote(0)->Get_int();
			int BrIndex = ua->GetSubQuote(1)->Get_int();
			Brigade* BRIG = &CITY[NI].Brigs[BrIndex]; 
			if(BRIG->Enabled&&ua->GetSubQuote(2))
			{
				ErrorPager Error;
				//if((!BRIG->Ability))
					BRIG->Ability = new BrigadeActiveAbilityList();
				BRIG->Ability->Load(*(ua->GetSubQuote(2)),BRIG->Ability,&Error,NULL);
			}
		}
	}
	return true;
}
//==================================================================================================================//
