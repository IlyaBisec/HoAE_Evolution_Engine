#include "stdheader.h"
#include "UnitsInterface.h"
#include ".\cvi_oboz.h"

//////////////////////////////////////////////////////////////////////////
//cvi_Academy vAcademy;
//////////////////////////////////////////////////////////////////////////

cvi_Oboz::cvi_Oboz(void)
{
}

cvi_Oboz::~cvi_Oboz(void)
{
}

// vui_Actions
void cvi_Act_Oboz::SetFrameState(SimpleDialog* SD){
	SD->Visible=false;
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&OIS.SelPoint.GetAmount()==1){
		OneObject* OB=SP->OB;
		if(!(OB->NewBuilding||OB->Stuff==0xFFFF)){
			SD->Visible=true;
			byte ResID,Owner;
			int ResAm;			
			dipf_SetSelOboz(OB,ResID,ResAm,Owner);
			GPPicture* RP=ResPic.Get();
			if(RP){
				static int spr=RP->SpriteID;
				RP->SpriteID=spr+ResID;				
			}
			_str str;
			TextButton* RA=ResAmount.Get();
			if(RA){
				str=ResAm;
				RA->SetMessage(str.pchar());
			}
			GPPicture* OP=OwnerPic.Get();
			if(OP){
				OP->Diffuse=GetNatColor(Owner);
			}			
			TextButton* ON=OwnerName.Get();
			if(ON){
				_str n;
				n=GetPlayerName(Owner);
				void LimitString(char* str,lpRLCFont FONT,int L);
				LimitString(n.pchar(),ON->PassiveFont,55);
				if(!n.isClear()){
					ON->SetMessage(n.pchar());
				}
			}
		}
	}
}
