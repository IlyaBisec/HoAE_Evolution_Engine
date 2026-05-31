//This is sample of using ClassEngine.
//To use ClassEngine in your application you should include next headers an define some
//defines as it is described below. In one of cpp files you also should define IMPLEMENT_CLASS_FACTORY.
//But this define should be only in one cpp file, that includes ClassEngine.h and more_types.h

//-----------------THERE ARE REQUIRED HEADERS AND DEFINES--------------------//

#define IMPLEMENT_CLASS_FACTORY
#define DIALOGS_API
#define __STDAPPLICATION__
typedef unsigned short  word;

#include "gmDefines.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ResFile.h>
#include <DynArray.h>
#include <ClassEngine.h>
#include <more_types.h>
#include <typeinfo.h>

//----------------END: THERE ARE REQUIRED HEADERS AND DEFINES------------------//


class TestClass:public BaseClass{
public:
	int V1;
	LinearArray<int,_int> LARR;
	DWORD CL;

	int GetV1();
	void SetV1(int V);
	INT_PROPERTY(TestClass,GetV1,SetV1);

	void* G_GetV1(){
		return &V1;
	}
	void G_SetV1(void* data){
		V1=*((int*)data);
	}
	PROPERTY(TestClass,G_GetV1,G_SetV1);
	SAVE(TestClass)
		//REG_MEMBER(_int,V1);
		//REG_INTPROP(_int,V1,GetV1,SetV1);
		REG_AUTO_ENUM(LARR,NAMES);
	REG_PROP(_int,V1,G_GetV1,G_SetV1);	
	REG_MEMBER(_DWORD,CL);
	ENDSAVE
};
int TestClass::GetV1(){
	return V1; 
}
void TestClass::SetV1(int V){
	V1=V;
}
class TestClass3:public TestClass{
public:
	int VVV;
	SAVE(TestClass3)
		REG_PARENT(TestClass);
		REG_MEMBER(_int,V1);
		REG_MEMBER(_DWORD,CL);
		REG_MEMBER(_int,VVV);
	ENDSAVE
};
typedef LinearArray<int,_accamulator> LArray1 ;
class TestClass2:public BaseClass{
public:
	int V2;
	ClassArray<TestClass> CARRAY;
	LinearArray<int,_accamulator> LARRAY;
	int V12;
	DWORD CL2;
	TestClass TC;
	SAVE(TestClass2);
	REG_ENUM(_index,V2,ENUM1);
	REG_MEMBER(_int,V12);
	REG_MEMBER(_DWORD,CL2);
	REG_CLASS(TestClass);
	REG_CLASS(TestClass2);
	REG_CLASS(TestClass3);
	REG_AUTO(CARRAY);
	REG_AUTO_ENUM(LARRAY,NAMES);
	REG_AUTO(TC);
	ENDSAVE
};

class bVector3D : public BaseClass
{
public:
	float x, y, z;

	SAVE( bVector3D );
		REG_MEMBER( _float, x );
		REG_MEMBER( _float, y );
		REG_MEMBER( _float, z );
	ENDSAVE
}; // class bVector3D


void test(){
	
}

int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				    LPSTR lpCmdLine, int nCmdShow)
{
	test();
}