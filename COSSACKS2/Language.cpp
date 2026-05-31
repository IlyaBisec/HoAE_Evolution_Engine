#include "stdheader.h"
#include "BE_HEADERS.h"
#include "Pinger.h"
#include "CurrentMapOptions.h"
#include "Camera\Navigator.h"
void Language::Init(){
	LangIndex=0;
}
void Language::reset_class(void* DataPtr){
	LangClass::reset_class(DataPtr);
	((Language*)DataPtr)->Init();
}