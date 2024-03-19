	x+=Pic->dx;
	y+=Pic->dy;
	int Lx=Pic->Lx;
	int NLines=Pic->NLines;
	int ofst=int(Pic)+23;
	if(y+NLines<=WindY||x+Lx<=WindX||x>WindX1||y>WindY1)return;
	//vertical clipping
	//top clipper
	int CDPOS=int(CData)+ddd;
	if(y<WindY){
		__asm{
			mov		ecx,WindY
			sub		ecx,y	
			sub		NLines,ecx
			add		y,ecx
			mov		ebx,ofst
			xor		eax,eax
			xor		edx,edx
NLINE:		mov		al,[ebx]
			test	al,128
			jz		SIMPLE_LINE
			inc		ebx
			mov		ah,al
			and		al,31
			or		al,al
			jz		COMPLINE_1
			and		ah,32
			shr		ah,1
COMPLINE_LOOP1:
			mov		dl,[ebx]
			shr		dl,4
			or		dl,ah
			add		CDPOS,edx
			inc		ebx
			dec		al
			jnz		COMPLINE_LOOP1
COMPLINE_1:
			xor		eax,eax
			dec		ecx
			jnz		NLINE
			jmp		END_VCLIP
SIMPLE_LINE:inc		ebx
			or		eax,eax
			jz		SIMPLINE_1
SIMPLINE_LOOP1:
			mov		dl,[ebx+1]
			add		CDPOS,edx
			add		ebx,2
			dec		al
			jnz		SIMPLINE_LOOP1
SIMPLINE_1:	dec		ecx
			jnz		NLINE
END_VCLIP:	mov		ofst,ebx
		};
	};
	//bottom clipper
	if(y+NLines>WindY1)NLines=WindY1-y+1;
	//horisontal clipper
	int x1=x+Lx-1;
	int scrofs=int(ScreenPtr)+x+y*ScrWidth;
	int TEMP1;
	int LineStart;
	int CLIP;
	int CURCLIP;
	byte SPACE_MASK;
	byte PIX_MASK;
	if(x>=WindX&&x1<=WindX1){
//***********************************************************//
//******************(((((((((((())))))))))))*****************//
//**                      NO CLIPPING                      **//
//******************(((((((((((())))))))))))*****************//
//***********************************************************//
		//no clipping
		__asm{
			pushf
			push	esi
			push	edi
			//initialisation
			mov		edi,scrofs			//edi-screen pointer
			mov		esi,CDPOS			//esi-points array
			mov		ebx,Encoder			//ebx-encoding pointer
			mov		edx,ofst			//edx-mask offset
			xor		eax,eax
			xor		ecx,ecx
			cld
			sub		edi,ScrWidth
			mov		LineStart,edi
START_SCANLINE:
			mov		edi,LineStart
			mov		al,[edx]
			inc		edx
			add		edi,ScrWidth
			or		al,al
			mov		LineStart,edi
			jnz		DRAW_LINE
			dec		NLines
			jnz		START_SCANLINE
			jmp		END_DRAW_PICTURE
DRAW_LINE:	test	al,128
			jnz		DRAW_COMPLEX_LINE
			//drawing simple line
			or		al,al
			jz		NEXT_SEGMENT
			mov		TEMP1,eax
START_SIMPLE_SEGMENT:
			mov		al,[edx]	//empty space
			add		edi,eax
			mov		cl,[edx+1]
			add		edx,2
//*****************************************//
//put various code with encoding&loop there//
//*****************************************//
			GP_PROC_PARAM(0)
//*****************************************//
//          end of variation zone          //
//*****************************************//
			dec		TEMP1
			jnz		START_SIMPLE_SEGMENT
NEXT_SEGMENT:
			dec		NLines
			jnz		START_SCANLINE
			jmp		END_DRAW_PICTURE
DRAW_COMPLEX_LINE:
			//complex packed line
			mov		SPACE_MASK,0
			mov		PIX_MASK,0
			test	al,64
			jz		DCL1
			mov		SPACE_MASK,16
DCL1:		test	al,32
			jz		DCL2
			mov		PIX_MASK,16
DCL2:		and		al,31
			jz		NEXT_SEGMENT
			mov		TEMP1,eax
START_COMPLEX_SEGMENT:
			mov		al,[edx]
			mov		cl,al
			and		al,15
			or		al,SPACE_MASK
			add		edi,eax
			shr		cl,4
			or		cl,PIX_MASK
			inc		edx
//*****************************************//
//put various code with encoding&loop there//
//*****************************************//
			GP_PROC_PARAM(1)
//*****************************************//
//          end of variation zone          //
//*****************************************//
			dec		TEMP1
			jnz		START_COMPLEX_SEGMENT
			dec		NLines
			jnz		START_SCANLINE
END_DRAW_PICTURE:
			//end code
			pop		edi
			pop		esi
			popf
		};
	}else{
		//image must be clipped
		if(x<WindX&&x1<=WindX1){
//****************************************************************//
//****************************************************************//
//**                                                            **//
//**					clipping LEFT edge						**//
//**                                                            **//
//****************************************************************//
//****************************************************************//
			CLIP=WindX-x;
			__asm{
				pushf
				push	esi
				push	edi
				//initialisation
				mov		edi,scrofs			//edi-screen pointer
				mov		esi,CDPOS			//esi-points array
				mov		ebx,Encoder			//ebx-encoding pointer
				mov		edx,ofst			//edx-mask offset
				xor		eax,eax
				xor		ecx,ecx
				cld
				sub		edi,ScrWidth
				mov		LineStart,edi
CLIPLEFT_START_SCANLINE:
				mov		edi,LineStart
				mov		al,[edx]
				inc		edx
				xchg	CLIP,edx
				add		edi,ScrWidth
				mov		CURCLIP,edx
				or		al,al
				xchg	CLIP,edx
				mov		LineStart,edi
				jnz		CLIPLEFT_DRAW_LINE
				dec		NLines
				jnz		CLIPLEFT_START_SCANLINE
				jmp		CLIPLEFT_END_DRAW_PICTURE
CLIPLEFT_DRAW_LINE:		test	al,128
				jnz		CLIPLEFT_DRAW_COMPLEX_LINE
				//drawing simple line
				or		al,al
				jz		CLIPLEFT_NEXT_SEGMENT
				mov		TEMP1,eax
CLIPLEFT_START_SIMPLE_SEGMENT:
				mov		al,[edx]	//empty space
				add		edi,eax
				mov		cl,[edx+1]
				add		edx,2
				sub		CURCLIP,eax
				jle		CLIPLEFT_DRAW_SIMPLE_SEGMENT
				cmp		CURCLIP,ecx
				jl		CLIPLEFT_PARTIAL_SIMPLE
				//full  clipping
				sub		CURCLIP,ecx
				add		esi,ecx
				add		edi,ecx
				dec		TEMP1
				jnz		CLIPLEFT_START_SIMPLE_SEGMENT
				dec		NLines
				jnz		CLIPLEFT_START_SCANLINE
				jmp		CLIPLEFT_END_DRAW_PICTURE
CLIPLEFT_PARTIAL_SIMPLE:
				//partial clipping
				add		esi,CURCLIP
				add		edi,CURCLIP
				sub		ecx,CURCLIP
				mov		CURCLIP,-1
CLIPLEFT_DRAW_SIMPLE_SEGMENT:
//*****************************************//
//put various code with encoding&loop there//
//*****************************************//
				GP_PROC_PARAM(2)
//*****************************************//
//          end of variation zone          //
//*****************************************//
				dec		TEMP1
				jnz		CLIPLEFT_START_SIMPLE_SEGMENT
CLIPLEFT_NEXT_SEGMENT:
				dec		NLines
				jnz		CLIPLEFT_START_SCANLINE
				jmp		CLIPLEFT_END_DRAW_PICTURE
CLIPLEFT_DRAW_COMPLEX_LINE:
				//complex packed line
				mov		SPACE_MASK,0
				mov		PIX_MASK,0
				test	al,64
				jz		CLIPLEFT_DCL1
				mov		SPACE_MASK,16
CLIPLEFT_DCL1:	test	al,32
				jz		CLIPLEFT_DCL2
				mov		PIX_MASK,16
CLIPLEFT_DCL2:	and		al,31
				jz		CLIPLEFT_NEXT_SEGMENT
				mov		TEMP1,eax
CLIPLEFT_START_COMPLEX_SEGMENT:
				mov		al,[edx]
				mov		cl,al
				and		al,15
				or		al,SPACE_MASK
				add		edi,eax
				shr		cl,4
				or		cl,PIX_MASK
				inc		edx
				sub		CURCLIP,eax
				jle		CLIPLEFT_DRAW_COMPLEX_SEGMENT
				cmp		CURCLIP,ecx
				jl		CLIPLEFT_PARTIAL_COMPLEX
				//full  clipping
				sub		CURCLIP,ecx
				add		esi,ecx
				add		edi,ecx
				dec		TEMP1
				jnz		CLIPLEFT_START_COMPLEX_SEGMENT
				dec		NLines
				jnz		CLIPLEFT_START_SCANLINE
				jmp		CLIPLEFT_END_DRAW_PICTURE
CLIPLEFT_PARTIAL_COMPLEX:
				//partial clipping
				add		esi,CURCLIP
				add		edi,CURCLIP
				sub		ecx,CURCLIP
				mov		CURCLIP,-1
CLIPLEFT_DRAW_COMPLEX_SEGMENT:
//*****************************************//
//put various code with encoding&loop there//
//*****************************************//
				GP_PROC_PARAM(3)
//*****************************************//
//          end of variation zone          //
//*****************************************//
				dec		TEMP1
				jnz		CLIPLEFT_START_COMPLEX_SEGMENT
				dec		NLines
				jnz		CLIPLEFT_START_SCANLINE
CLIPLEFT_END_DRAW_PICTURE:
				//end code
				pop		edi
				pop		esi
				popf
			};
		}else
		if(x1>WindX1&&x>=WindX){
//****************************************************************//
//****************************************************************//
//**      **     **                                **    **     **//
//****************   	clipping RIGHT edge			**************//
//**      **     **                                **    **     **//
//****************************************************************//
//****************************************************************//
			CLIP=WindX1-x+1;
			int ADDESI;
			__asm{
				pushf
				push	esi
				push	edi
				//initialisation
				mov		edi,scrofs			//edi-screen pointer
				mov		esi,CDPOS			//esi-points array
				mov		ebx,Encoder			//ebx-encoding pointer
				mov		edx,ofst			//edx-mask offset
				xor		eax,eax
				xor		ecx,ecx
				cld
				sub		edi,ScrWidth
				mov		LineStart,edi
CLIPRIGHT_START_SCANLINE:
				mov		edi,LineStart
				mov		al,[edx]
				inc		edx
				mov		ADDESI,0
				xchg	CLIP,edx
				add		edi,ScrWidth
				mov		CURCLIP,edx
				or		al,al
				xchg	CLIP,edx
				mov		LineStart,edi
				jnz		CLIPRIGHT_DRAW_LINE
				dec		NLines
				jnz		CLIPRIGHT_START_SCANLINE
				jmp		CLIPRIGHT_END_DRAW_PICTURE
CLIPRIGHT_DRAW_LINE:
				test	al,128
				jnz		CLIPRIGHT_DRAW_COMPLEX_LINE
				//drawing simple line
				or		al,al
				jz		CLIPRIGHT_NEXT_SEGMENT
				mov		TEMP1,eax
CLIPRIGHT_START_SIMPLE_SEGMENT:
				mov		al,[edx]	//empty space
				add		edi,eax
				mov		cl,[edx+1]
				add		edx,2
				sub		CURCLIP,eax
				jg		CLIPRIGHT_TRY_TO_CLIP
				//full clipping until the end of line
				add		esi,ecx
				dec		TEMP1
				jnz		CLIPRIGHT_START_SIMPLE_SEGMENT
				dec		NLines
				jnz		CLIPRIGHT_START_SCANLINE
				jmp		CLIPRIGHT_END_DRAW_PICTURE
CLIPRIGHT_TRY_TO_CLIP:
				cmp		CURCLIP,ecx
				jl		CLIPRIGHT_PARTIAL_SIMPLE
				sub		CURCLIP,ecx
				jmp		CLIPRIGHT_DRAW_SIMPLE_SEGMENT
CLIPRIGHT_PARTIAL_SIMPLE:
				//partial clipping
				sub		ecx,CURCLIP
				mov		ADDESI,ecx
				mov		ecx,CURCLIP
				mov		CURCLIP,-1
CLIPRIGHT_DRAW_SIMPLE_SEGMENT:
//*****************************************//
//put various code with encoding&loop there//
//*****************************************//
				GP_PROC_PARAM(4)
//*****************************************//
//          end of variation zone          //
//*****************************************//
				add		esi,ADDESI
				dec		TEMP1
				jnz		CLIPRIGHT_START_SIMPLE_SEGMENT
CLIPRIGHT_NEXT_SEGMENT:
				dec		NLines
				jnz		CLIPRIGHT_START_SCANLINE
				jmp		CLIPRIGHT_END_DRAW_PICTURE
CLIPRIGHT_DRAW_COMPLEX_LINE:
				//complex packed line
				mov		SPACE_MASK,0
				mov		PIX_MASK,0
				test	al,64
				jz		CLIPRIGHT_DCL1
				mov		SPACE_MASK,16
CLIPRIGHT_DCL1:		test	al,32
				jz		CLIPRIGHT_DCL2
				mov		PIX_MASK,16
CLIPRIGHT_DCL2:		and		al,31
				jz		CLIPRIGHT_NEXT_SEGMENT
				mov		TEMP1,eax
CLIPRIGHT_START_COMPLEX_SEGMENT:
				mov		al,[edx]
				mov		cl,al
				and		al,15
				or		al,SPACE_MASK
				add		edi,eax
				shr		cl,4
				or		cl,PIX_MASK
				inc		edx
				sub		CURCLIP,eax
				jg		CLIPRIGHT_TRY_TO_CLIP_COMPLEX
				//full clipping until the end of line
				add		esi,ecx
				dec		TEMP1
				jnz		CLIPRIGHT_START_COMPLEX_SEGMENT
				dec		NLines
				jnz		CLIPRIGHT_START_SCANLINE
				jmp		CLIPRIGHT_END_DRAW_PICTURE
CLIPRIGHT_TRY_TO_CLIP_COMPLEX:
				cmp		CURCLIP,ecx
				jl		CLIPRIGHT_PARTIAL_COMPLEX
				sub		CURCLIP,ecx
				jmp		CLIPRIGHT_DRAW_COMPLEX_SEGMENT
CLIPRIGHT_PARTIAL_COMPLEX:
				//partial clipping
				sub		ecx,CURCLIP
				mov		ADDESI,ecx
				mov		ecx,CURCLIP
				mov		CURCLIP,-1
CLIPRIGHT_DRAW_COMPLEX_SEGMENT:
//*****************************************//
//put various code with encoding&loop there//
//*****************************************//
				GP_PROC_PARAM(5)
//*****************************************//
//          end of variation zone          //
//*****************************************//
				add		esi,ADDESI
				dec		TEMP1
				jnz		CLIPRIGHT_START_COMPLEX_SEGMENT
				dec		NLines
				jnz		CLIPRIGHT_START_SCANLINE
CLIPRIGHT_END_DRAW_PICTURE:
				//end code
				pop		edi
				pop		esi
				popf
			};
		}else{
//****************************************************************//
//****************************************************************//
//**      **     **                                **    **     **//
//****************    clipping RIGHT&LEFT edges		**************//
//**      **     **                                **    **     **//
//****************************************************************//
//****************************************************************//
			CLIP=WindX-x;
			int CLIP2=WindX1-x+1;
			int CURCLIP2;
			__asm{
				pushf
				push	esi
				push	edi
				//initialisation
				mov		edi,scrofs			//edi-screen pointer
				mov		esi,CDPOS			//esi-points array
				mov		ebx,Encoder			//ebx-encoding pointer
				mov		edx,ofst			//edx-mask offset
				xor		eax,eax
				xor		ecx,ecx
				cld
				sub		edi,ScrWidth
				mov		LineStart,edi
CLIPLR_START_SCANLINE:
				mov		edi,LineStart
				mov		al,[edx]
				inc		edx
				xchg	CLIP,edx
				add		edi,ScrWidth
				or		al,al
				mov		CURCLIP,edx
				xchg	CLIP,edx
				xchg	CLIP2,edx
				mov     CURCLIP2,edx
				xchg    CLIP2,edx
				mov		LineStart,edi
				jnz		CLIPLR_DRAW_LINE
				dec		NLines
				jnz		CLIPLR_START_SCANLINE
				jmp		CLIPLR_END_DRAW_PICTURE
CLIPLR_DRAW_LINE:		test	al,128
				jnz		CLIPLR_DRAW_COMPLEX_LINE
				//drawing simple line
				or		al,al
				jz		CLIPLR_NEXT_SEGMENT
				mov		TEMP1,eax
CLIPLR_START_SIMPLE_SEGMENT:
				mov		al,[edx]	//empty space
				add		edi,eax
				mov		cl,[edx+1]  //pixels line width
				add		edx,2
				sub		CURCLIP,eax
				jle		CLIPLR_CHECK_RIGHT_CLIPPING//pereshli levuju granicu
				cmp		CURCLIP,ecx
				jl		CLIPLR_PARTIAL_SIMPLE//kusok linii zalazit na levuju granicu
				//full  clipping
				sub		CURCLIP,ecx
				sub     CURCLIP2,ecx
				sub		CURCLIP2,eax
				add		esi,ecx
				add		edi,ecx
				dec		TEMP1
				jnz		CLIPLR_START_SIMPLE_SEGMENT
				dec		NLines
				jnz		CLIPLR_START_SCANLINE
				jmp		CLIPLR_END_DRAW_PICTURE
CLIPLR_PARTIAL_SIMPLE:
				//partial clipping
				add		esi,CURCLIP
				add		edi,CURCLIP
				sub		ecx,CURCLIP
				xchg    edx,CURCLIP2
				sub		edx,CURCLIP
				xchg    edx,CURCLIP2
				mov		CURCLIP,-1
CLIPLR_CHECK_RIGHT_CLIPPING:
				sub     CURCLIP2,eax
				jg      CLIPLR_PARTIAL_RIGHT_CLIP
				//full right clipping
CLIPLR_FULL_RIGHT_CLIPPING:
				add		esi,ecx
				add		edi,ecx
				dec		TEMP1
				jnz		CLIPLR_START_SIMPLE_SEGMENT
				dec		NLines
				jnz		CLIPLR_START_SCANLINE
				jmp		CLIPLR_END_DRAW_PICTURE
CLIPLR_PARTIAL_RIGHT_CLIP:
				sub     CURCLIP2,ecx
				jge     CLIPLR_DRAW_SIMPLE_SEGMENT
				add     ecx,CURCLIP2
CLIPLR_DRAW_SIMPLE_SEGMENT:
//*****************************************//
//put various code with encoding&loop there//
//*****************************************//
				GP_PROC_PARAM(6)
//*****************************************//
//          end of variation zone          //
//*****************************************//
				cmp		CURCLIP2,0
				jge     CLIPLR_SKIP_RIGHT_CLIP
				sub		esi,CURCLIP2
				sub		edi,CURCLIP2
CLIPLR_SKIP_RIGHT_CLIP:
				dec		TEMP1
				jnz		CLIPLR_START_SIMPLE_SEGMENT
CLIPLR_NEXT_SEGMENT:
				dec		NLines
				jnz		CLIPLR_START_SCANLINE
				jmp		CLIPLR_END_DRAW_PICTURE
CLIPLR_DRAW_COMPLEX_LINE:
				//complex packed line
				mov		SPACE_MASK,0
				mov		PIX_MASK,0
				test	al,64
				jz		CLIPLR_DCL1
				mov		SPACE_MASK,16
CLIPLR_DCL1:	test	al,32
				jz		CLIPLR_DCL2
				mov		PIX_MASK,16
CLIPLR_DCL2:	and		al,31
				jz		CLIPLR_NEXT_SEGMENT
				mov		TEMP1,eax
CLIPLR_START_COMPLEX_SEGMENT:
				mov		al,[edx]
				mov		cl,al
				and		al,15
				or		al,SPACE_MASK
				add		edi,eax
				shr		cl,4
				or		cl,PIX_MASK
				inc		edx
				sub		CURCLIP,eax
				jle		CLIPLR_CHECK_COMPLEX_RIGHT_CLIPPING//CLIPLR_DRAW_COMPLEX_SEGMENT
				cmp		CURCLIP,ecx
				jl		CLIPLR_PARTIAL_COMPLEX
				//full  clipping
				sub		CURCLIP,ecx
				sub     CURCLIP2,ecx
				sub     CURCLIP2,eax
				add		esi,ecx
				add		edi,ecx
				dec		TEMP1
				jnz		CLIPLR_START_COMPLEX_SEGMENT
				dec		NLines
				jnz		CLIPLR_START_SCANLINE
				jmp		CLIPLR_END_DRAW_PICTURE
CLIPLR_PARTIAL_COMPLEX:
				//partial clipping
				add		esi,CURCLIP
				add		edi,CURCLIP
				sub		ecx,CURCLIP
				xchg    edx,CURCLIP2
				sub		edx,CURCLIP
				xchg    edx,CURCLIP2
				mov		CURCLIP,-1
CLIPLR_CHECK_COMPLEX_RIGHT_CLIPPING:
				cmp     CURCLIP2,0
				jg      CLIPLR_PARTIAL_COMPLEX_RIGHT_CLIP
				//full right clipping
CLIPLR_FULL_COMPLEX_RIGHT_CLIPPING:
				add		esi,ecx
				add		edi,ecx
				dec		TEMP1
				jnz		CLIPLR_START_COMPLEX_SEGMENT
				dec		NLines
				jnz		CLIPLR_START_SCANLINE
				jmp		CLIPLR_END_DRAW_PICTURE
CLIPLR_PARTIAL_COMPLEX_RIGHT_CLIP:
				sub     CURCLIP2,ecx
				jge     CLIPLR_DRAW_COMPLEX_SEGMENT
				add     ecx,CURCLIP2
CLIPLR_DRAW_COMPLEX_SEGMENT:
//*****************************************//
//put various code with encoding&loop there//
//*****************************************//
				GP_PROC_PARAM(7)
//*****************************************//
//          end of variation zone          //
//*****************************************//
				cmp		CURCLIP2,0
				jge     CLIPLR_SKIP_COMPLEX_RIGHT_CLIP
				sub		esi,CURCLIP2
				sub		edi,CURCLIP2
CLIPLR_SKIP_COMPLEX_RIGHT_CLIP:
				dec		TEMP1
				jnz		CLIPLR_START_COMPLEX_SEGMENT
				dec		NLines
				jnz		CLIPLR_START_SCANLINE
CLIPLR_END_DRAW_PICTURE:
				//end code
				pop		edi
				pop		esi
				popf
			};
		};
	};
};
