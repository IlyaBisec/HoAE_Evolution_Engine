#include "CommonDip.h"

void BasicDiploRelation::InitThisTribe(char* Preffix,char* FirstGroupName,char* CommonID){
};
void BasicDiploRelation::InitThisTribe2(char* FileID, char* Dip, char* GDance, char* ZDance){
};
int BasicDiploRelation::GetAmountOfPropositions(){
	return 0;
};
DWORD BasicDiploRelation::GetPossibility(int Index,char* Message,char* Hint){
	return 0;
};
void BasicDiploRelation::PerformPossibility(DWORD Code){
};
void BasicDiploRelation::ProcessTribe(){
};
void BasicDiploRelation::PerformCommand(char* Data,int size){
};
int BasicDiploRelation::GetSaveData(byte* buf,bool write){
	return 0;
};
void BasicDiploRelation::SetSaveData(byte* buf,int size){
};
void BasicDiploRelation::Init(){
	memset(this,0,((byte*)&DIP)-((byte*)&Owner));
	DIP=NULL;
};
void BasicDiploRelation::Free(){
};