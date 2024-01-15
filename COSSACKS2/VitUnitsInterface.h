#ifndef __VitUnitsInterface__
#define __VitUnitsInterface__

class VitUnitButton;
class VitUnitInterface;

// Units button functions
typedef void VitUBF_getState(OneObject*, GPPicture*);//VitUnitButton*);
typedef bool VitUBF_leftClick(OneObject*);

class VitUnitButton{
public:
	bool Visible;
	int x,y;
    int GPFile;
	int GPSprite;	

	int getState;
	int leftClick;

	VitUnitInterface* VUI;
private:
};

class VitUnitInterface{
public:
	int x,y;
    int GPFile;
	int GPSprite;
	int dx,dy;		// soldatik sdvig
	DynArray<VitUnitButton*> Button;
};

class VitUnitsInterface{
public:
	~VitUnitsInterface();
	void Init();
	void FreeUnit();
	void LoadUnit();

	StringsHash UnitName;
	DynArray<VitUnitInterface*> Unit;	

	StringsHash getStateName;
	DynArray<VitUBF_getState*> getState;
	StringsHash leftClickName;
	DynArray<VitUBF_leftClick*> leftClick;

};

#define Add_getState(x)\
	getStateName.add(#x);\
	getState.Add(&x)

#define Add_leftClick(x)\
	leftClickName.add(#x);\
	leftClick.Add(&x)

// functions prototipes
void GetState0(OneObject* _OB, GPPicture* _VUB);//VitUnitButton* _VUB);
bool LeftClick0(OneObject* _OB);

void GetAttState0(OneObject* _OB, GPPicture* _VUB);//VitUnitButton* _VUB);
bool UseAttState0(OneObject* _OB);
void GetAttState1(OneObject* _OB, GPPicture* _VUB);//VitUnitButton* _VUB);
bool UseAttState1(OneObject* _OB);
void GetAttState2(OneObject* _OB, GPPicture* _VUB);//VitUnitButton* _VUB);
bool UseAttState2(OneObject* _OB);
void GetKartechState(OneObject* _OB, GPPicture* _VUB);//VitUnitButton* _VUB);
bool UseKartech(OneObject* OB);

#endif __VitUnitsInterface__