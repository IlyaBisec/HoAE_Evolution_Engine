#include "stdheader.h"
#include "BE_HEADERS.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UNARY FUNCTION SECTION
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// be_CBaseUnaryOperator /////////////////////////////////////////////////
be_CBaseUnaryOperator::be_CBaseUnaryOperator(){
	Modificator = "----";
};
void	be_CBaseUnaryOperator::checkModificator(){
	if ( Modificator!="----" && Modificator!="NOT" && Modificator!="-" ){
		Modificator = "----";
	};
};
int	be_CBaseUnaryOperator::CallFunction(){ 
	g_beDebuger.callFunction( getCB_ID(0) );
	int retVal = be_CBaseUnary<be_CBasePtrFunction>::CallFunction();
	if (g_beCheckChar(Modificator.pchar())){
		if (Modificator=="-")					return retVal = -retVal; 
		else if (Modificator=="NOT"){
			if (FirstOperand.CallFunction()==0) return retVal = 1;
			else								return retVal = 0;
 		};
	};
	return retVal;
};

// be_CBaseUnaryStatement ////////////////////////////////////////////////
be_CBaseUnaryStatement::be_CBaseUnaryStatement(){
	Hide = false;
	Descr = "Hidden Section";
	Modificator = "----";
};
void	be_CBaseUnaryStatement::checkModificator(){
	if (	
			Modificator!="----"	&& 
			Modificator!="AND"	&& 
			Modificator!="OR"	&& 
			Modificator!="SUM"	&&
			Modificator!="-SUM" 
		)
	{
		Modificator = "----";
	};
};
DWORD	be_CBaseUnaryStatement::GetClassMask(){
	DWORD MASK = 0x00000001;
	if (g_beCheckChar(Modificator.pchar())&&Modificator=="----" ){
		MASK += 0x00000002;
	};
	if ( MASK&0x00000002 && Hide )
	{
		if ( g_beDebuger._visible==false || g_beDebuger._SCRIPT_SRC ){
			MASK  = 0x00000004+0x00000002;
		};
	};
	return MASK;
};
int	be_CBaseUnaryStatement::CallFunction(){ 
	if (g_beCheckChar(Modificator.pchar())){
		int retValue = 0;
		int tempValue = 0;
		g_beDebuger.callFunction( getCB_ID(0) );
		for ( int i=0; i<FirstOperand.GetAmount(); i++ ){
			if(FirstOperand[i]){
				tempValue = FirstOperand[i]->CallFunction();
				if (Modificator=="----"){
					retValue = tempValue;
				}else  if (Modificator=="AND") {
					if ( tempValue==false )	return 0;
					if (i==0) retValue = tempValue;
					else retValue = ( ( (bool)tempValue && (bool)retValue ) ? (1) : (0) );
				}else if (Modificator=="OR") {
					if ( tempValue==true )	return 1;
					if (i==0) retValue = 0;
					retValue = ( ( (bool)tempValue || (bool)retValue ) ? (1) : (0) );
				}else if (Modificator=="SUM") {
					if (i==0) retValue = tempValue;
					else retValue += tempValue;
				}else if (Modificator=="-SUM") {
					if (i==0) retValue = tempValue;
					else retValue -= tempValue;
				};
			}
		};
		if ( Modificator=="----" ){
			g_beDebuger.callFunction( getCB_ID(1) );
		};
		return	retValue;
	};
	return be_CBaseUnary<be_CBaseArrayFunction>::CallFunction(); 
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BINARY FUNCTION SECTION
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// be_CBaseBinaryOperator ////////////////////////////////////////////////
be_CBaseBinaryOperator::be_CBaseBinaryOperator(){
	Modificator = "AND";
};
void	be_CBaseBinaryOperator::checkModificator(){
	if (	
			Modificator!="AND"	&& 
			Modificator!="OR"	&& 
			Modificator!="-"	&& 
			Modificator!="+"	&&
			Modificator!="*"	&&
			Modificator!=">"	&& 
			Modificator!=">="	&& 
			Modificator!="<"	&& 
			Modificator!="<="	&&
			Modificator!="=="	&&
			Modificator!="!=" 
		)
	{
		Modificator = "AND";
	};
};
int	be_CBaseBinaryOperator::CallFunction(){ 
	g_beDebuger.callFunction( getCB_ID(0) );
	int retFirst  = FirstOperand.CallFunction();
	int retSecond = SecondOperand.CallFunction();
	int retValue = retSecond;
	if (g_beCheckChar(Modificator.pchar())){
		if		 (Modificator=="AND") {
			retValue = ( ( (bool)retFirst && (bool)retSecond ) ? (1) : (0) );
		}else if (Modificator=="OR") {
			retValue = ( ( (bool)retFirst || (bool)retSecond ) ? (1) : (0) );
		}else if (Modificator=="-") {
			retValue = retFirst - retSecond;
		}else if (Modificator=="+") {
			retValue = retFirst + retSecond;
		}else if (Modificator=="*") {
			retValue = retFirst * retSecond;
		}else if (Modificator==">") {
			retValue = retFirst > retSecond;
		}else if (Modificator==">=") {
			retValue = retFirst >= retSecond;
		}else if (Modificator=="<") {
			retValue = retFirst < retSecond;
		}else if (Modificator=="<=") {
			retValue = retFirst <= retSecond;
		}else if (Modificator=="==") {
			retValue = retFirst == retSecond;
		}else if (Modificator=="!=") {
			retValue = retFirst != retSecond;
		};
	};
	return	retValue;
};	

// be_CBaseBinaryStatement ///////////////////////////////////////////////	
be_CBaseBinaryStatement::be_CBaseBinaryStatement(){
	Modificator = "if (...) then ...";
};
void	be_CBaseBinaryStatement::checkModificator(){
	if (	
			Modificator!="if (...) then ..."	&& 
			Modificator!="while(...) do ..."	&& 
			Modificator!="do ... while(...)"	
		)
	{
		Modificator = "if (...) then ...";
	};
};
int	be_CBaseBinaryStatement::CallFunction(){
	int retValue = 0;
	g_beDebuger.callFunction( getCB_ID(0) );
	if (g_beCheckChar(Modificator.pchar())){
		if		 (Modificator=="if (...) then ...") {
			retValue = FirstOperand.CallFunction();
			g_beDebuger.callFunction( getCB_ID(1) );
			if (retValue){
				SecondOperand.CallFunction();
			};
		}else if (Modificator=="while(...) do ...") {
			while ( FirstOperand.CallFunction() ) {
				retValue = 1;
				g_beDebuger.callFunction( getCB_ID(1) );
				SecondOperand.CallFunction();
			};
		}else if (Modificator=="do ... while(...)") {
			retValue = 1;
			do {
				SecondOperand.CallFunction();
				g_beDebuger.callFunction( getCB_ID(1) );
			} while( FirstOperand.CallFunction() );
		};
		g_beDebuger.callFunction( getCB_ID(2) );
		return retValue;
	};
	return be_CBaseBinary<be_CBasePtrFunction,be_CBaseArrayFunction>::CallFunction();
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TRINARY FUNCTION SECTION
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// be_CBaseTrinaryOperator ///////////////////////////////////////////////
be_CBaseTrinaryOperator::be_CBaseTrinaryOperator(){
	Modificator = "----";	
};
void	be_CBaseTrinaryOperator::checkModificator(){
	if ( Modificator!="----" ) Modificator="----";
};
int	be_CBaseTrinaryOperator::CallFunction(){
	return be_CBaseTrinary<be_CBasePtrFunction>::CallFunction();
};

// be_CBaseTrinaryStatement //////////////////////////////////////////////
be_CBaseTrinaryStatement::be_CBaseTrinaryStatement(){
	Modificator = "----";
};
void	be_CBaseTrinaryStatement::checkModificator(){
	if ( Modificator!="----" ) Modificator="----";
};
int	be_CBaseTrinaryStatement::CallFunction(){ 
	return be_CBaseTrinary<be_CBasePtrFunction,be_CBaseArrayFunction>::CallFunction(); 
};

//////////////////////////////////////////////////////////////////////////

void be_REG_CALSS_ComplexFunction(){
	REG_CLASS_EX(be_CBaseUnaryOperator,"{C ff0000FF}BASE COMPLEX");
	REG_CLASS_EX(be_CBaseUnaryStatement,"{C ff0000FF}BASE COMPLEX");
	REG_CLASS_EX(be_CBaseBinaryOperator,"{C ff0000FF}BASE COMPLEX");
	REG_CLASS_EX(be_CBaseBinaryStatement,"{C ff0000FF}BASE COMPLEX");
	REG_CLASS_EX(be_CBaseTrinaryOperator,"{C ff0000FF}BASE COMPLEX");
	REG_CLASS_EX(be_CBaseTrinaryStatement,"{C ff0000FF}BASE COMPLEX");
};





















