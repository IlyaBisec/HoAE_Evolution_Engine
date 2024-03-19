/*****************************************************************************/
/*	File:	sgPipeline.h
/*	Desc:	Pipelined processing
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-27-2003
/*****************************************************************************/
#ifndef __PIPELINE_H__
#define __PIPELINE_H__

namespace sg{

/*****************************************************************************/
/*	Class:	PipeSection
/*	Desc:	Generic pipelined processing algorithm description
/*****************************************************************************/
class PipeSection : public Node
{
	std::string		message;
	
public:
					PipeSection();
	virtual			~PipeSection();
	virtual	bool	Process();

	virtual void	Serialize	( OutStream& os ) const;
	virtual void	Unserialize	( InStream& is	);

	const char*		GetMessage() { return message.c_str(); }
	void			SetMessage( const char* _message ) { message = _message; }


	NODE(PipeSection, Node, PIPS);
}; // class PipeSection

/*****************************************************************************/
/*	Class:	Pipeline
/*	Desc:	Generic pipelined processing algorithm description
/*****************************************************************************/
class Pipeline : public PipeSection 
{
	PipeSection*		pCurSection;

public:
						Pipeline();
	virtual				~Pipeline();
	virtual	bool		Process();
	virtual void		Construct(){}

	NODE(Pipeline, PipeSection, PIPE);
}; // class Pipeline

/*****************************************************************************/
/*	Class:	PipeData
/*	Desc:	Represents data node in the pipeline. Doesn't do any processing
/*****************************************************************************/
class PipeData : public PipeSection
{
public:
	PipeData();
	virtual				~PipeData();
	virtual	bool		Process(){ return true; }
	virtual void		Clear(){}

	NODE(PipeData, PipeSection, PIPD);
}; // class PipeData

/*****************************************************************************/
/*	Class:	PipeDataBlock
/*	Desc:	PipeData grouping node
/*****************************************************************************/
class PipeDataBlock : public PipeData
{
public:
	virtual	bool		Process(){ return true; }
	virtual void		Clear(){}

	NODE(PipeDataBlock, PipeData, PDBL);
}; // class PipeDataBlock

}; // namnespace sg

#endif  // __PIPELINE_H__
