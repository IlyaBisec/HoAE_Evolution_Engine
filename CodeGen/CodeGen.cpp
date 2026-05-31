// CodeGen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "math.h"

DWORD GetReq(DWORD C){
    float XX=float(C)/1365536.0f;    
    DWORD R[2];
    memcpy(&R[0],&XX,8);    
    return R[0]^C;
}

int main(int argc, char* argv[])
{
    if(argc==2){
        DWORD K;
        sscanf(argv[1],"%X",&K);
        DWORD K2=GetReq(K);
        FILE* F=fopen("serial.txt","w");
        if(F){
            fprintf(F,"%X",K2);
            fclose(F);
            printf("%X",K2);
        }
    }
	return 0;
}

