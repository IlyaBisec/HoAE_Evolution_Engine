#include <ClassEngine.h>
#include <more_types.h>
#include "GameSettings.h"
#pragma pack( push )
#pragma pack( 1 )
#include "EngineSettings.h"
/*class LocalClass:public BaseClass{
public:
	void SaveFile(char* fName);
	bool LoadFile(char* fname);
	SAVE(LocalClass)		
	ENDSAVE
};*/
//class GameClass;
class LocalizationSettings:public BaseClass{
public:
	int LanguageSetForBikVideo;
	int LanguageIndex;
	SAVE(LocalizationSettings){
	REG_MEMBER(_int,LanguageSetForBikVideo);
	REG_MEMBER(_int,LanguageIndex);
	}	ENDSAVE;
};
extern LocalizationSettings LocSettings;
