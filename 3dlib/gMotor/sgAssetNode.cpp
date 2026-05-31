/*****************************************************************************/
/*	File:	sgAssetNode.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	09-11-2003
/*****************************************************************************/
#include "stdafx.h"
#include "kIO.h"
#include "sgAssetNode.h"

BEGIN_NAMESPACE( sg )

/*****************************************************************************/
/*	AssetNode implementation 
/*****************************************************************************/
void AssetNode::SetEmbedded( bool embedded ) 
{ 
	bool wasEmbedded = GetFlagState( nfEmbeddedData );

	if (wasEmbedded == embedded) return;
	if (wasEmbedded && !embedded)
	{
		data.Clear();
	}
	SetFlagState( nfEmbeddedData, embedded );
	if (!wasEmbedded && embedded)
	{
		FInStream is( GetName() );
		if (is.NoFile()) return;
		data.Reserve( is.GetFileSize() );
		is.Read( data.GetData(), is.GetFileSize() );
	}
}

void AssetNode::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	if (GetFlagState( nfEmbeddedData ))
	{
		os << data;
	}
} // AssetNode::Serialize

void AssetNode::Unserialize( InStream& is )
{
	Parent::Unserialize( is );

	if (GetFlagState( nfEmbeddedData ))
	{
		is >> data;
	}
} // AssetNode::Unserialize

END_NAMESPACE( sg )

