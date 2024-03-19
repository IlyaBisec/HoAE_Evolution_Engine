#pragma once

template <int nDwords> class bitset{
    DWORD Bits[nDwords];
public:
	bitset(){
		clear_all();
	}
	void clear_all(){
		memset(&Bits,0,sizeof Bits);
	}
	void set_all(){
		memset(&Bits,0xFF,sizeof Bits);
	}
	__forceinline bool check_bit(int bitIdx){
		return Bits[bitIdx>>5] & (1<<(bitIdx&31));
	}
	__forceinline void set_bit(int bitIdx){
		Bits[bitIdx>>5] |= (1<<(bitIdx&31));
	}
	__forceinline void clear_bit(int bitIdx){
		Bits[bitIdx>>5] &= ~DWORD(1<<(bitIdx&31));
	}
	bitset<nDwords>& operator |= (bitset<nDwords>& arg){
		for(int i=0;i<nDwords;i++){
			Bits[i]|=arg.Bits[i];
		}
        return *this;
	}
};