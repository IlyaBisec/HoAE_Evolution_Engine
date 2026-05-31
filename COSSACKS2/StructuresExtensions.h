#if !defined(STRUCTURESEXTENSIONS_H___INCLUDED_)
#define STRUCTURESEXTENSIONS_H___INCLUDED_
//ATTENTION! if you should extend these structures you should anyway
//COMMENT and describe EVERY field.
struct DIALOGS_API OneObjectExtension{
	//you can access these fieds in OneObject.Ext
};
struct DIALOGS_API NewMonsterExtension:public BaseClass{
	//you can access these fieds in NewMonster.Ext and AdvancedCharacter.Ext
	SAVE(NewMonsterExtension);
	ENDSAVE;
};
struct DIALOGS_API ObjCharacterExtension:public BaseClass{
	//you can access these fieds in ObjCharacter.Ext 
	SAVE(ObjCharacterExtension);
	ENDSAVE;
};
#endif