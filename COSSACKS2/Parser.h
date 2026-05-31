class OneTrigger;
typedef int tpGetConditionID(char* id,int& param);
typedef bool tpCheckCondition(int ID,int param,void* UserParam);
typedef char* tpComArray[64];
class OneTrigger{
public:
	OneTrigger** SubTrigger;
	int NSubTriggers;
	int MaxTrigg;
	int* ConditionID;
	int* ConditionParam;
	int NConditions;
	int MaxCond;
	char** Command;
	int NCommands;
	int MaxComm;
	static char ErrMessage[256];
	OneTrigger(){
		memset(this,0,sizeof *this);
	};
	~OneTrigger(){
		for(int i=0;i<NCommands;i++)if(Command[i])free(Command[i]);
		if(Command)free(Command);
		if(ConditionID)free(ConditionID);
		if(ConditionParam)free(ConditionParam);
		for(int i=0;i<NSubTriggers;i++)if(SubTrigger[i])delete(SubTrigger[i]);
		if(SubTrigger)free(SubTrigger);
		memset(this,0,sizeof *this);
	}
	void AddCondition(int ID,int Param){
		if(NConditions>=MaxCond){
            MaxCond+=4;
			ConditionID=(int*)realloc(ConditionID,MaxCond*4);
			ConditionParam=(int*)realloc(ConditionParam,MaxCond*4);
		}
		ConditionID[NConditions]=ID;
		ConditionParam[NConditions]=Param;
		NConditions++;
	}
	void AddCommand(char* cmd){
		if(NCommands>=MaxComm){
            MaxComm+=4;
			Command=(char**)realloc(Command,MaxComm*4);
		}
		int L=strlen(cmd)+1;
		Command[NCommands]=znew(char,L);
		strcpy(Command[NCommands],cmd);
		NCommands++;
	}
	OneTrigger* AddTrigger(){
		if(NSubTriggers>=MaxTrigg){
            MaxTrigg+=4;
			SubTrigger=(OneTrigger**)realloc(SubTrigger,MaxTrigg*4);
		}
		SubTrigger[NSubTriggers]=new OneTrigger;
		NSubTriggers++;
		return SubTrigger[NSubTriggers-1];
	}
	int ParseOneScope(char* s,tpGetConditionID* ParseF,int& Line){
		if((s[0]=='$'&&s[1]=='i'&&s[2]=='f'&&s[3]==' ')||(s[0]=='$'&&s[1]=='d'&&s[2]=='o')){
			//reading condition
			char cond[512];
			int p;
			int sp=s[1]=='i'?4:3;
			bool EndLine=0;
			do{
				char cc;
				p=0;
				cond[0]=0;
				while((cc=s[sp])&&cc!=' '&&cc!=0x0D&&cc!=0x0A&&cc!=0x9){
					cond[p]=cc;
					cond[p+1]=0;
					p++;
					sp++;
				}
				while((cc=s[sp])&&(cc==' '||cc==0x9||cc==0x0D||cc==0x0A)){
					if(cc==0x0D){
						Line++;
						EndLine=1;
					}
					sp++;
				}
				if(!cc){
					strcpy(ErrMessage,"unexpected end of file");
					return -2;//unexpected end of file
				}
				if(cond[0]){
					int param;
					int id=ParseF(cond,param);
					if(id<0){
						sprintf(ErrMessage,"unknown identifier: %s",cond);
						return id;
					}
					AddCondition(id,param);
				}
			}while(!EndLine);
			do{
				if(s[sp]=='$'){
					if(s[sp+1]=='e'){
						sp+=4;
						char cc;
						while((cc=s[sp])&&(cc==' '||cc==0x9||cc==0x0D||cc==0x0A)){
							if(cc==0x0D){
								Line++;
								EndLine=1;
							}
							sp++;
						}
						return sp;
					}
					OneTrigger* OT=AddTrigger();
					int r=OT->ParseOneScope(s+sp,ParseF,Line);
					if(r>0)sp+=r;
					else return r;
				}else{
                    //command is there
					int sp0=sp;
					char cc;
					while((cc=s[sp])&&cc!=' '&&cc!=0x0D&&cc!=0x0A&&cc!=0x9)sp++;
					memcpy(cond,s+sp0,sp-sp0);
					cond[sp-sp0]=0;
					AddCommand(cond);
					while((cc=s[sp])&&(cc==' '||cc==0x9||cc==0x0D||cc==0x0A)){
						if(cc==0x0D){
							Line++;
							EndLine=1;
						}
						if(cc==0){
							strcpy(ErrMessage,"unexpected end of file");
							return -2;//unexpected end of file
						}
						sp++;
					}
				}
			}while(1);
		}else{ 
			strcpy(ErrMessage,"unrecognised condition");
			return -1;//unrecognized condition
		}
	}
	int GetCommand(tpCheckCondition* Checker,void* Param,tpComArray* ComList,int pos,int MaxCom,byte& exit){
        int N=0;
		bool OK=NConditions==0;
		for(int i=0;i<NConditions;i++){
			if(Checker(ConditionID[i],ConditionParam[i],Param)){
				OK=1;
				int NP=min(NCommands,MaxCom);
				int ps=0;
				for(int q=0;q<NP;q++){
					char* cmd=Command[q];
					if(!strcmp(cmd,"exit")){
						exit=1;
                        return ps;                        
					}else{
						(*ComList)[pos+ps]=cmd;
						ps++;
					}
				}
				N=ps;
				break;
			}
		}
		if(OK){
			if(N<MaxCom){
				for(int i=0;i<NSubTriggers;i++){
					byte exit1=0;
					N+=SubTrigger[i]->GetCommand(Checker,Param,ComList,N+pos,MaxCom-N-pos,exit1);
					if(exit1){
						exit=1;
						return N;
					}
				}
			}
		}
		return N;
	}
	int GetCommand(tpCheckCondition* Checker,void* Param,tpComArray* ComList,int MaxCom){
		int pos=0;
		byte exit=0;
		return GetCommand(Checker,Param,ComList,pos,MaxCom,exit);
	}
};