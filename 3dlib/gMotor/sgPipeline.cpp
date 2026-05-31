/*****************************************************************************/
/*	File:	sgPipeline.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-27-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgPipeline.h"
#include "kIOHelpers.h"

BEGIN_NAMESPACE(sg)

/*****************************************************************************/
/*	PipeSection implementation
/*****************************************************************************/
PipeSection::PipeSection()
{
	SetFlagState( nfInvisible, true );
}

PipeSection::~PipeSection()
{

}

bool PipeSection::Process()
{
	return true;
}

void PipeSection::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << message;
} // PipeSection::Serialize

void PipeSection::Unserialize( InStream& is	)
{
	Parent::Unserialize( is );
	is >> message;
} // PipeSection::Unserialize

/*****************************************************************************/
/*	Pipeline implementation
/*****************************************************************************/
Pipeline::Pipeline() : pCurSection(NULL)
{
}

Pipeline::~Pipeline()
{
}

bool Pipeline::Process()
{
	pCurSection = NULL;
	Node::Iterator it( this, PipeSection::FnFilter );
	while (it)
	{
		Node* pNode = (Node*)it;
		if (pNode == this) 
		{
			++it;
			continue;
		}
		pCurSection = (PipeSection*)pNode;
		pCurSection->Process();
		++it;
	}

	pCurSection = NULL;
	return true;
} // Pipeline::Process

/*****************************************************************************/
/*	PipeData implementation
/*****************************************************************************/
PipeData::PipeData()
{

}

PipeData::~PipeData()
{

}

END_NAMESPACE(sg)

