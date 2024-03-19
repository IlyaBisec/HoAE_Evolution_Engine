/*****************************************************************************/
/*	File:	sgAssetNode.h
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	09-11-2003
/*****************************************************************************/
#ifndef __SGASSETNODE_H__
#define __SGASSETNODE_H__

namespace sg{

/*****************************************************************************/
/*	Class: AssetNode
/*****************************************************************************/
class AssetNode : public Node
{
	std::string		refFile;
	ByteBuffer		data;

public:
					AssetNode			(){}
					AssetNode			( const char* _name ) : Node( _name ) {}

	BYTE*			GetData() const		{ return data.GetData();  }
	int				GetDataSize() const	{ return data.GetSize();  }

	bool			IsEmbedded			() const { return GetFlagState( nfEmbeddedData ); }
	void			SetEmbedded			( bool embedded = true );
	void			ReserveEmbedded		( int size ) 
	{ 
		data.Reserve( size ); 
		SetEmbedded(); 
	}

	virtual void	Serialize			( OutStream& os ) const;
	virtual void	Unserialize			( InStream& is	);

	NODE(AssetNode,Node,ASST);
}; // class AssetNode

}; // namespace sg

#endif // __SGASSETNODE_H__