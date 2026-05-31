#pragma once

#define MaxChildWeap 8
class Weapon;

class DIALOGS_API ChildWeapon{
public:
	Weapon* Child[MaxChildWeap];
	byte    NChildWeapon;
	byte    MaxChild;
	byte    MinChild;
	byte    Type;
	void InitChild();
};
class WeaponModificator;
class DIALOGS_API Weapon{
public:
	Weapon() { Modificator=NULL; };
	NewAnimation* NewAnm;
	ChildWeapon Default;
	ChildWeapon* CustomEx;
	byte    NCustomEx;
	Weapon* ShadowWeapon;
	Weapon* TileWeapon[4];
	word    TileProbability;
	byte	NTileWeapon;
	byte    MinChild;
	byte    MaxChild;
	byte    HotFrame;
	Weapon* SyncWeapon[4];
	byte	NSyncWeapon;
	char    dz;
	//see scripts
	short   Damage;
	short   Radius;
	short   Speed;
	short   Times;
	short	DamageHeight;
	short   DetonationForce;
	short   DamageDec;
	short   RandomAngle;
	short   RandomAngleZX;
	byte    Transparency;
	byte    Gravity;
	byte    Propagation;
	bool    FullParent:1;
	bool    HiLayer:1;
	bool    Splash:1;
	short	SoundID;
#ifdef _USE3D
	byte FogR;
	byte FogWeight;
	//extended params 
	byte LayerID;  //exists 4 layers, 0-highest
	//1.Fading
	DWORD Fade0;
	DWORD Fade1;
	DWORD Fade2;
	word  FadeFirstStep;
	//2. Size params
	byte SizeFactor;//0-small, does not require flushing,
	//1-medium, requires smart flushing, 
	//2-big,need to flush always
	//3. Projection params
	byte Projection;//0-billboard
	//1-ground shaped
	//4. Texturing params
	byte CenterTexWeight;
	byte CenterFactWeight;
	byte TextureID;
	byte FactureID;
	byte TexRadius;
#endif //_USE3D
	word MyIndex;
	_str Name;
	//WeaponModificator* Modificator;
	ClassRef<WeaponModificator>* Modificator;
};