#pragma once
DString::DString(){
	str=NULL;
	L=0;
	MaxL=0;
};
DString::DString(char* s){
	str=NULL;
	L=0;
	MaxL=0;
	Assign(s);
};
DString::~DString(){
	if(str)POOL.Free((BYTE*)str,MaxL);
	str=NULL;
	L=0;
	MaxL=0;
}
void DString::Allocate(int LN){
	if(MaxL<LN){
		if(LN<8)LN=8;
		else if(LN<16)LN=16;
		else if(LN<32)LN=32;
		else if(LN<64)LN=64;
		else if(LN<128)LN=128;
		else LN=(LN+15)&0xFFFFFFF0;
		if(LN>512)LN=((LN+LN/2+128)&0xFFFFFFF0);
		str=(char*)POOL.Realloc((BYTE*)str,MaxL,LN);
		str[L]=0;
		MaxL=LN;
	}
}
void DString::ReadFromFile(char* file){
	Clear();
    ResFile F=RReset(file);
    if(F!=INVALID_HANDLE_VALUE){
        int sz=RFileSize(F);
        Allocate(sz+1);
        RBlockRead(F,str,sz);
        str[sz]=0;
        RClose(F);
    }
    /*
	FILE* F=fopen(file,"rb");
	if(F){
		fseek(G,0,SEEK_END);
		int sz=ftell(F);
		fseek(F,0,SEEK_SET);
		if(sz){
			Allocate(sz+1);
			fread(str,1,sz,F);
			str[sz]=0;
			L=strlen(str);
		};
		fclose(F);
	}
    */
}
void DString::WriteToFile (char* file){
	ResFile F=RRewrite(file);
	if(F!=INVALID_HANDLE_VALUE){
		if(str)RBlockWrite(F,str,L);
		RClose(F);
	};
};
void DString::Add(DString& Str){
	if(!Str.str)return;
	Allocate(L+Str.L+1);
	strcat(str+L,Str.str);
	L+=Str.L;
};
void DString::Add(const char* Str){ 
	if(Str){
		int L1=strlen(Str);
		Allocate(L+L1+1);
		strcat(str+L,Str);
		L+=L1;
	}
}
void DString::Add(char Ch){
	if(Ch!=0){
		Allocate(L+1+1);
		str[L]=Ch;
		str[L+1]=0;
		L++;
	}
}
void DString::Add(int v){
	char s[16];
	sprintf(s,"%d",v);
	Add(s);
};
void DString::print(char* mask,...){
	va_list args;
	va_start(args,mask);
	char temp[4096];
	vsprintf(temp,mask,args);
	va_end(args);
	Add(temp);
}
void DString::Replace0(char* src,char* dst){
	if(!str)return;
	char* s=str;
	int L0=strlen(src);
	int L1=strlen(dst);
	do{
		s=strstr(s,src);
		if(s){
			int pos=s-str;
			Allocate(L+L1-L0+1);
			memmove(str+pos+L1,str+pos+L0,L-pos-L0+1);
			memcpy(str+pos,dst,L1);
			L=strlen(str);
			s=str+pos+L1;
		};
	}while(s);
};
void DString::Replace(char* src,DString& dst){
	if(!dst.str)return;
	Replace(src,dst.str);
};
void DString::Replace(char* src,char* dst,...){
	assert(strlen(dst)<4000);
	char ccc[4096];
	va_list va;
	va_start(va,dst);
	vsprintf (ccc,dst,va);   
	va_end(va);
	assert(strlen(ccc)<4000);
	Replace0(src,ccc);
};
void DString::Replace(char* src,int value){
	char cc[16];
	sprintf(cc,"%d",value);
	Replace(src,cc);
};
void DString::Clear(){
	Free();
	//L=0;
	//if(str)str[0]=0;
};
void DString::Free(){
	if(str)POOL.Free((BYTE*)str,MaxL);
	str=NULL;
	L=0;
	MaxL=0;
};
void DString::Assign(char* Str){
	if(Str){
        if(Str[0]){		    
		    int L1=strlen(Str);
            if(L>256)Free();
            L=L1;
		    Allocate(L+1);
		    strcpy(str,Str);
        }else{
            if(str)Free();
        }
	}
};
void DString::Assign(DString& Str){
	if(Str.str)Assign(Str.str);
	else Clear();
};
void DString::Assign(const int& a){
	char s[16];
	sprintf(s,"%d",a);
	Assign(s);
};
void DString::ExtractLine(DString& dst)
{
	dst.Clear();
	char seps[]="\n";
	char *token;
	DString rez;
	rez.Assign(str);
	token=strtok(str,seps);
	if(token!=NULL)
	{
		dst.Add(token);
		int s=strlen(token);
		char* c;
		if(s<rez.L)
			c=(char*)(strstr(rez.str,token)+s+1);
		else 
			c=(char*)(strstr(rez.str,token)+s);
		Assign(c);
	}
}
void DString::ExtractWord(DString& dst)
{
	dst.Clear();
	char seps[]=" ,;:\t\n\r";
	char *token;
	DString rez;
	rez.Assign(str);
	token=strtok(str,seps);
	if(token!=NULL)
	{
		dst.Add(token);
		int s=strlen(token);
		char* c;
		if(s<rez.L)
			c=(char*)(strstr(rez.str,token)+s+1);
		else 
			c=(char*)(strstr(rez.str,token)+s);
		Assign(c);
	}
}
void DString::ExtractWord2(DString& dst)
{
	dst.Clear();
	char seps[]=" ,;:\t\n";
	char *token;
	DString rez;
	rez.Assign(str);
	token=strtok(str,seps);
	if(token!=NULL)
	{
		dst.Add(token);
		int s=strlen(token);
		char* c;
		if(s<rez.L)
			c=(char*)(strstr(rez.str,token)+s+1);
		else 
			c=(char*)(strstr(rez.str,token)+s);
		Assign(c);
	}
	else
	{
		Clear();
	}

}
void DString::DeleteChars(int From, int N)
{
	if((From+N)<=L)
	{
		memmove(str+From,str+From+N,L-From-N);
		L-=N;
		str[L]=0;
	}
}
void DString::Limit(int maxL){
	if(str&&L>maxL){
		str[maxL]=0;
		L=maxL;
	}
}