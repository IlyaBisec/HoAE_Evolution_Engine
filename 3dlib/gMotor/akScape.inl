/*****************************************************************************/
/*	File:	akScape.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	06-11-2003
/*****************************************************************************/

/*****************************************************************************/
/*	ScapeQuad implementation
/*****************************************************************************/
WORD ScapeQuad::qIdx[c_QuadInd];

ScapeQuad::ScapeQuad()
{
	maxH	= 0;
	lod		= 0;
} // ScapeQuad::ScapeQuad

/*---------------------------------------------------------------------------*/
/*	Func:	ScapeQuad::IsVisible
/*	Desc:	Determines if given quad is fully or partially visible on screen
/*---------------------------------------------------------------------------*/
bool ScapeQuad::IsVisible()
{
	return true;
} // ScapeQuad::IsVisible