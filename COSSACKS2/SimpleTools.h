__inline int lrintf (float flt){
	int intgr;
	_asm{
		fld flt
		fistp intgr
	};
	return intgr;
}
__inline DWORD V4D2DW(Vector4D& v){
	int temp;
	int temp1;
	_asm{
		fld   v.w
		fistp temp
		fld   v.z
		mov   eax,temp		
		fistp temp1
		mov   ebx,temp1
		fld   v.y		
        shr   eax,8
		fistp temp
		mov	  al,bl		
        mov   ebx,temp
		fld	  v.x
        shr   eax,8
		fistp temp
		mov   ebx,temp
		mov   al,bl                		
	}
}
__inline DWORD BYTE4(BYTE x,BYTE y,BYTE z,BYTE w){
	__asm{
        mov ah,w
        mov al,z
		mov bh,y
		shl eax,16
		mov bl,x
		mov ax,bx
	}
}