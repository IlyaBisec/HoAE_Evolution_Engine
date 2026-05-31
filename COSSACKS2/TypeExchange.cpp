#include "stdheader.h"
#include "TypeExchange.h"

BaseTriplet VariableExchangeReference::FindClass(){
	BaseTriplet T;
	T.Data=T.Extra=T.ClassPtr=NULL;
	T.OCMS=NULL;
	ReferableBaseClass* B=RootClass.Get();
	if(B){
		const char* rstr=VariableReferenceName.pchar();
		if(rstr[0]=='$'){
			char* s0=strchr(rstr,'.');
			if(s0){
				s0++;
				T.ClassPtr=B;
				T.Data=B;
				T.Extra=NULL;
				T.OCMS=NULL;
				do{
					if(s0[0]=='['){
						if(s0[1]==']'){
							BaseClass* P=GetParent();
							if(P){
								P=P->GetParent();
								if(P){
									SimpleDialog* SD=dynamic_cast<SimpleDialog*>(P);
									if(SD){
										int idx=SD->AutoIndexInArray;
										if(idx<T.ClassPtr->GetAmountOfElements()){
											BaseTriplet T2=T.ClassPtr->GetElementInfo(idx,T.GetData());
											if(T2.Data){
												T=T2;
											}
                                            /*
											BaseClass* BC=NULL;
											void* Data;
											void* Extra;
											BC=T.BC->GetElementClass(idx);
											if(BC){
												Data=T.BC->GetElementPtr(idx,T.Data);
												Extra=T.BC->GetElementExtraPtr(idx);
												T.BC=BC;
												T.Data=Data;
												T.Extra=Extra;
												T.OCMS=T.BC->GetElementCMS(idx);
											}
											*/
										}
									}
								}
							}
							s0+=2;
							if(s0[0]=='.')s0++;
						}else{
							int idx=atoi(s0+1);
							char* s1=strchr(s0,']');
							if(s1){
								s0=s1+1;
								if(s0[0]=='.')s0++;
							}else{
								T.Data=NULL;
								break;                            
							}
							BaseTriplet T2=T.ClassPtr->GetElementInfo(idx,T.GetData());
							if(T2.GetData()){
								T=T2;
							}
							/*
							BaseClass* BC=NULL;
							void* Data;
							void* Extra;
							BC=T.BC->GetElementClass(idx);
							if(BC){
								Data=T.BC->GetElementPtr(idx,T.Data);
								Extra=T.BC->GetElementExtraPtr(idx);
							}
							*/
						}
					}else{
						char* s1=strchr(s0,'[');
						char* s2=strchr(s0,'.');
						char* s3=s0+strlen(s0);

                        if(s1 && s2 && s2-s1 < 0)s1=s2;
						if(!s1)s1=s2;
						if(!s1)s1=s3;

						char name[128];
						int L=s1-s0;
						strncpy(name,s0,L);
						s0=s1;
						if(s0[0]=='.')s0++;
						name[L]=0;
						BaseTriplet T2=T.ClassPtr->GetElementInfo(name,T.GetData());
						if(T2.Data){
							T=T2;
						}
						/*
						BaseClass* BC=NULL;
						void* Data;
						void* Extra;
						if(T.BC->GetElementByName(name,&BC,&Data,&Extra,T.Data)){
							T.BC=BC;
							T.Data=Data;
							T.Extra=Extra;
							if(T.BC->CheckDirectCasting())T.BC=(BaseClass*)Data;
						}else{
							T.BC=NULL;
							break;
						}
						*/
					}
				}while(s0[0]);
			}
		}
	}
	return T;
}
void ClassSyncroniser::_excahnge(){
	for(int i=0;i<GetAmount();i++)(*this)[i]->_exchange();
}
void VariableExchangeReference::_exchange(){
	BaseClass* P=GetParent();
	if(!P)return;
	P=P->GetParent();
	if(!P)return;
	if(!ParentMember.GetClass()){        
		ParentMember=P->GetElementInfo(FieldName.pchar(),P);
		if(ParentMember.GetClass()){
		//if(!P->GetElementByName(FieldName.pchar(),&ParentMember.BC,&ParentMember.Data,&ParentMember.Extra,P)){
            static int nWarn=100;
            if(nWarn){
			    Log.Warning("VariableExchangeReference: member \"%s\" not found in class %s for variable %s",FieldName.pchar(),P->GetClassName(),VariableReferenceName.pchar());
                nWarn--;
            }
		}		
	}
	//if(!ClassMember.BC){
        ClassMember=FindClass();
		if(!ClassMember.GetData()){
			if(!RootClass.Get()){
                static int nWarn=100;
                if(nWarn){
				    Log.Warning("VariableExchangeReference: RootClass is NULL in class %s for variable: %s",P->GetClassName(),FieldName.pchar());
                    nWarn--;
                }
			}
            static int nWarn=100;
            if(nWarn){
			    Log.Warning("VariableExchangeReference: Variable (%s) referenced in class %s not found: %s",VariableReferenceName.pchar(),P->GetClassName(),FieldName.pchar());
                nWarn--;
            }
		}
	//}
	if(ParentMember.GetData() && ClassMember.GetData()){		
		_exchange(ParentMember,ClassMember);
	}
}
void VariableExchangeReference::_exchange(BaseTriplet& T1,BaseTriplet& T2){
    if(!strcmp(T2.GetClass()->GetClassName(),"_functor")){
        _functor<BaseClass,void>* fn=(_functor<BaseClass,void>*)T2.GetClass();
        xmlQuote xml1;       
        const char* s=fn->getText(T2.Data);
        ErrorPager ep;
        if(!prev_str.equal(s)){
            xml1.Assign_string((char*)s);
			T1.Load(&xml1);
        }
    }else{
	    xmlQuote xml1;
	    T1.Save(&xml1);
	    xmlQuote xml2;
	    T2.Save(&xml2);
	    const char* s1=xml1.Get_string();
	    const char* s2=xml2.Get_string();    
        if(!strcmp(s1,s2)){
            if(!prev_str.equal(s1))prev_str=s1;
            return;
        }
	    ErrorPager ep;
	    if(prev_str.equal(s2)){
		    prev_str=s1;
            T2.Load(&xml1);
	    }else{
		    prev_str=s2;
		    T1.Load(&xml2);
	    }
    }
}
void BooleanExchange::_exchange(BaseTriplet& T1,BaseTriplet& T2){
    bool& v=*((bool*)T1.Data);
    xmlQuote xml2;
    T2.Save(&xml2);    
    const char* s2=xml2.Get_string();  
    const char* v_true=VariableValueFor_true.cpchar();
    const char* v_false=VariableValueFor_false.cpchar();
    if(v_true[0]){
        v=strcmp(s2,v_true)==0;        
    }else
    if(v_false[0]){
        v=strcmp(s2,v_false)!=0;
    }
}
DWORD RecreateSimpleFieldsEnumerator_L2(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	BaseClass* B=in->Context;
	if(B){
		B=B->GetParent();
		if(B){
			B=B->GetParent();
			if(B){
				if(DWORD(B)==PrevCallHash)return DWORD(B);
                OneClassStorage* OCS=B->GetClassStorage();
				if(OCS){
					for(int i=0;i<OCS->Members.GetAmount();i++){
                        OneClassMemberStorage* OCMS=OCS->Members[i];
						if(OCMS->Member){
							const char* n=OCMS->Member->GetClassName();
							if((!strstr(n,".")) && (!strstr(n,"Delimiter")) && (!strstr(n,"SubSection"))){
								if(!dynamic_cast<BaseFunction*>(OCMS->Member)){
									out->add(OCMS->xmlID,i,NULL);
								}
							}
						}
					}					
				}
				return DWORD(B);
			}
		}
	}
	return 0;
}
DWORD RecreateSimpleFieldsEnumerator_L2_bool(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
    BaseClass* B=in->Context;
    if(B){
        B=B->GetParent();
        if(B){
            B=B->GetParent();
            if(B){
                if(DWORD(B)==PrevCallHash)return DWORD(B);
                OneClassStorage* OCS=B->GetClassStorage();
                if(OCS){
                    for(int i=0;i<OCS->Members.GetAmount();i++){
                        OneClassMemberStorage* OCMS=OCS->Members[i];
                        if(OCMS->Member){
                            const char* n=OCMS->Member->GetClassName();
                            if(!strcmp(n,"_bool")){                                
                                out->add(OCMS->xmlID,i,NULL);
                            }
                        }
                    }					
                }
                return DWORD(B);
            }
        }
    }
    return 0;
}
bool VariableExchangeReference::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	dest="Connect {CY}%$FieldName$%{C} to variable {CY}%$VariableReferenceName$%{C}";
	return true;
}
bool ClassSyncroniser::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	if(GetAmount()){
		dest="ConnectingDialogToClassMembers: {CY}";
		for(int i=0;i<GetAmount();i++){
			dest+=(*this)[i]->FieldName;
			if(i<GetAmount()-1)dest+=",";
		}
		return true;
	}else{	
		return false;
	}	
}
void REG_EXCHANGE_CLASSES(){
	REG_CLASS(VariableExchangeReference);
    REG_CLASS(BooleanExchange);
	Enumerator* E=ENUM.Get("ClassFields_L2");
	E->SetProcEnum(new RefreshableEnumerator(RecreateSimpleFieldsEnumerator_L2));
    E=ENUM.Get("ClassFields_L2_bool");
    E->SetProcEnum(new RefreshableEnumerator(RecreateSimpleFieldsEnumerator_L2_bool));
}