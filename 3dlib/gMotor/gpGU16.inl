/*****************************************************************************/
/*	File:	gpGU16.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	03.03.2003
/*****************************************************************************/

BEGIN_NAMESPACE(sg)

/*****************************************************************************/
/*	GU16Seq implementation
/*****************************************************************************/
_inl GU16Seq::GU16Seq( MediaFileLoader& mFile ) : GPSeq()
{
	SetType( gptUniform );
	CreateFromMFile( mFile );
} // GU16Seq::GU16Seq

END_NAMESPACE(sg)
