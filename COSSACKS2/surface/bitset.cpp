#include "..\comms\comms.h"
#include "bitset.h"
UnlimitedBitset::~UnlimitedBitset(){
    clear();
}
void UnlimitedBitset::set(int pos,bool value){
	int nr=1+pos/32;
	if(Data.Count()<nr){
		DWORD v=0xFFFFFFFF;
		Data.Add(v,nr-Data.Count());
	}
	DWORD& v=Data.GetAt(nr-1);
	DWORD ms=1<<(pos&31);
	if(value){
        v|=ms;        
	}else{
        v&=~ms;
	}
}
bool UnlimitedBitset::get(int pos){
	int nr=pos/32;
	if(nr<Data.Count()){
        return !!(Data[nr]&(1<<(pos&31)));		
	}
	return false;
}
void UnlimitedBitset::clear(){
	Data.Clear();
};