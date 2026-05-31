/*****************************************************************************/
/*	File:	sgGQuad.inl
/*	Author:	Ruslan Shestopalyuk
/*****************************************************************************/


/*****************************************************************
/*	FrameChunkHeader inplementation
/*****************************************************************/
_inl int FrameChunkHeader::GetSidePow() const
{
	return (*((DWORD*)this)) >> 28;
}

_inl int FrameChunkHeader::GetSide() const
{
	return 1 << GetSidePow();
}

_inl int FrameChunkHeader::GetX() const
{
	DWORD res = ((*((DWORD*)this)) >> 12) & 0xFFF;
	if (res & 0x800) res |= 0xFFFFF000;
	return res;
}

_inl int FrameChunkHeader::GetY() const
{
	DWORD res = (*((DWORD*)this)) & 0xFFF;
	if (res & 0x800) res |= 0xFFFFF000;
	return res;
}

#define FGA_POLY_CONF_ALL    0        
#define FGA_POLY_CONF_LU     1      
#define FGA_POLY_CONF_RB     2       
#define FGA_POLY_CONF_RU     3        
#define FGA_POLY_CONF_LB     4      

_inl bool FrameChunkHeader::IsFull() const
{
	return (((*((DWORD*)this)) >> 24) & 0xF) == FGA_POLY_CONF_ALL;
}

_inl bool FrameChunkHeader::IsLU() const
{
	return (((*((DWORD*)this)) >> 24) & 0xF) == FGA_POLY_CONF_LU;
}

_inl bool FrameChunkHeader::IsLB() const
{
	return (((*((DWORD*)this)) >> 24) & 0xF) == FGA_POLY_CONF_LB;
}

_inl bool FrameChunkHeader::IsRU() const
{
	return (((*((DWORD*)this)) >> 24) & 0xF) == FGA_POLY_CONF_RU;
}

_inl bool FrameChunkHeader::IsRB() const
{
	return (((*((DWORD*)this)) >> 24) & 0xF) == FGA_POLY_CONF_RB;
}

_inl BYTE* FrameChunkHeader::GetPixelData() const
{
	return (BYTE*)(this + 1);
}

_inl int FrameChunkHeader::GetSizeBytes() const
{
	return sizeof(*this) + ((1 << (GetSidePow() << 1)) << 1);
}


