/*****************************************************************************/
/*	File:	gpGN16.inl
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	03.03.2003
/*****************************************************************************/

BEGIN_NAMESPACE(sg)
/*****************************************************************************/
/*	GN16Seq implementation
/*****************************************************************************/
_inl GN16Seq::GN16Seq( MediaFileLoader& mFile ) : GPSeq()
{
	SetType( gptUniform );
	CreateFromMFile( mFile );
} // GN16Seq::GN16Seq

END_NAMESPACE(sg)
