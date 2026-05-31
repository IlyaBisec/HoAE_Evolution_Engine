// Config file: Ability.dat

CIMPORT int GetAbilityByName(char* Name);
CIMPORT void GetAbilityCost(int Index,int* Cost);
CIMPORT char* GetAbilityMessage(int Index);
CIMPORT void GetAbilitySprite(int Index,int& File,int& Sprite);

CIMPORT int AddAbilityToBrigade(byte NI,word BrIndex,int AblIndex);
CIMPORT void ApplyAbility(byte NI,word BrIndex,int AblIndex);
CIMPORT void UseAbility(byte NI,word BrIndex,int AblIndex);
CIMPORT void DelAbility(byte NI,word BrIndex,int AblIndex);