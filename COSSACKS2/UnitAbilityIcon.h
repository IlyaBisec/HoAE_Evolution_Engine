#pragma once
//----------------------------------------------------------------------------------------------------------------//
class IconElement
{
public:
	IconElement();
	short FileID;
	int SpriteID;
	char* Text;
	char* Hint;
};
//----------------------------------------------------------------------------------------------------------------//
class UnitAbilityIcon
{
public:
	UnitAbilityIcon();
	~UnitAbilityIcon();

	int GetNElements();
	int GetGPFileID(int Element);
	int GetGPSpriteID(int Element);
	const char* GetText(int Element);
	const char* GetHint(int Element);

	void AddElement(IconElement* IE);

private:
	DynArray<IconElement*> Elements;
};
//----------------------------------------------------------------------------------------------------------------//