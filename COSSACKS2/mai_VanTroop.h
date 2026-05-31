#pragma once

class mai_VanTroop: public BaseClass
{
public:
	mai_VanTroop(void);
	~mai_VanTroop(void);
	//
	byte NI;
	int Index;
	//
	SAVE(mai_VanTroop){	
		REG_MEMBER(_BYTE, NI);
		REG_MEMBER(_int,Index);		
	}ENDSAVE;
};
