#ifndef __COMPLEX__FUNCTION__MODE__
#define __COMPLEX__FUNCTION__MODE__

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// be_CBaseUnaryOperator MODIFICATORS 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DWORD fn_BE_UO_CFMODES_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// be_CBaseUnaryStatement MODIFICATORS 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DWORD fn_BE_US_CFMODES_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// be_CBaseBinaryOperator MODIFICATORS 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DWORD fn_BE_BO_CFMODES_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// be_CBaseBinaryStatement MODIFICATORS 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DWORD fn_BE_BS_CFMODES_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// be_CBaseTrinaryOperator MODIFICATORS 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DWORD fn_BE_TO_CFMODES_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// be_CBaseTrinaryStatement MODIFICATORS 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DWORD fn_BE_TS_CFMODES_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash);

//////////////////////////////////////////////////////////////////////////
void	be_REG_CLASS_CFMODES();

#endif//__COMPLEX__FUNCTION__MODE__