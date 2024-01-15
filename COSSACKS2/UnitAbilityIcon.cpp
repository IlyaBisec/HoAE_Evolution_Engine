#include "stdheader.h"
#include "UnitAbilityIcon.h"
//----------------------------------------------------------------------------------------------------------------//
IconElement::IconElement()
{
	FileID=-1;
	SpriteID=-1;
	Text=NULL;
	Hint=NULL;
}
//----------------------------------------------------------------------------------------------------------------//
UnitAbilityIcon::UnitAbilityIcon()
{}
UnitAbilityIcon::~UnitAbilityIcon()
{
	int n=Elements.GetAmount();
	for(int i=0;i<n;i++)
	{
		if(Elements[i])
			delete Elements[i];
	}
	Elements.Clear();
}
int UnitAbilityIcon::GetNElements()
{
	return Elements.GetAmount();
}
int UnitAbilityIcon::GetGPFileID(int Element)
{
	if(Element<Elements.GetAmount())
	{
		return Elements[Element]->FileID;
	}
	return -1;
}
int UnitAbilityIcon::GetGPSpriteID(int Element)
{
	if(Element<Elements.GetAmount())
	{
		return Elements[Element]->SpriteID;
	}
	return -1;
}
const char* UnitAbilityIcon::GetText(int Element)
{
	if(Element<Elements.GetAmount())
	{
		return Elements[Element]->Text;
	}
	return NULL;
}
const char* UnitAbilityIcon::GetHint(int Element)
{
	if(Element<Elements.GetAmount())
	{
		return Elements[Element]->Hint;
	}
	return NULL;
}
void UnitAbilityIcon::AddElement(IconElement* IE)
{
	Elements.Add(IE);
}
//----------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------//
