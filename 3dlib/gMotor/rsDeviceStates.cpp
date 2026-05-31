/*****************************************************************************/
/*	File:	rsDeviceStates.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	09-11-2003
/*****************************************************************************/
#include "stdafx.h"
#include "kHash.hpp"
#include "rsDeviceStates.h"
#include "rsDSSParser.h"
#include "kIOHelpers.h"

/*****************************************************************************/
/*	DSBlock implementation
/*****************************************************************************/
DSBlock::DSBlock() 
	:	m_DevHandle		( c_BadDevHandle	), 
		m_bSorted		( false				),
		m_bDefaultBase	( true				)
{
}

OutStream& DSBlock::operator <<( OutStream& os )
{
	os << m_Name << m_RS << m_bDefaultBase;
	for (int i = 0; i < c_MaxTextureStages; i++)
	{
		os << m_TSS[i];
	}
	return os;
} // DSBlock::operator <<

InStream& DSBlock::operator >>( InStream& is )
{
	is >> m_Name >> m_RS >> m_bDefaultBase;
	for (int i = 0; i < c_MaxTextureStages; i++)
	{
		is >> m_TSS[i];
	}
	return is;
} // DSBlock::operator >>

bool DSBlock::AddRS( const DeviceState& ds )
{
	m_RS.push_back( ds );
	return false;
}

bool DSBlock::AddTSS( int stage, const DeviceState& ds )
{
	m_TSS[stage].push_back( ds );
	return false;
}

void DSBlock::Clear()
{
	m_RS.clear();
	for (int i = 0; i < c_MaxTextureStages; i++)
	{
		m_TSS[i].clear();
	}
} // DSBlock::Clear

bool DSBlock::ParseScript( const char* fileName )
{
	FInStream is( fileName );
	if (is.NoFile()) return false;
	int sz = is.GetFileSize();
	char* pBuf = new char[sz + 1];
	is.Read( pBuf, sz );
	pBuf[sz] = 0;
	bool res = ParseScript( pBuf, is.GetFileSize() );
	delete []pBuf;
	return res;
} // DSBlock::ParseScript

bool DSBlock::ParseScript( char* buf, int bufSize )
{
	Clear();
	bool allOK = dss::DSSParser::instance().ParseBuffer( buf, *this ); 
	SortByState();
	return allOK;
} // DSBlock::ParseScript

void DSBlock::SortByState()
{
	if (m_bSorted) return;
	if (m_RS.size() > 1)
	{
		qsort( &(m_RS[0]), m_RS.size(), sizeof( DeviceState ), DeviceState::CompareByDevID );
	}

	for (int i = 0; i < c_MaxTextureStages; i++)
	{
		if (m_TSS[i].size() > 1)
		{
			qsort( &((m_TSS[i])[0]), m_TSS[i].size(), sizeof( DeviceState ), DeviceState::CompareByDevID );
		}
	}
	m_bSorted = true;
} // DSBlock::SortByState

DSBlock& DSBlock::operator=( const DSBlock& orig )
{
	m_Name		= orig.m_Name;
	m_DevHandle	= orig.m_DevHandle;
	m_RS		= orig.m_RS;
	for (int i = 0; i < c_MaxTextureStages; i++)
	{
		m_TSS[i]	= orig.m_TSS[i];
	}
	return *this;
} // DSBlock::operator

void DSBlock::Sub( DSBlock& b1, DSBlock& b2 )
{
	Clear();

} // DSBlock::operator

void DSBlock::Append( DSBlock& block )
{
	for (int i = 0; i < block.GetNRS(); i++)
	{
		if (!HaveRS( *block.GetRS( i ) )) AddRS( *block.GetRS( i ) );
	}

	for (int i = 0; i < c_MaxTextureStages; i++)
	{
		for (int j = 0; j < block.GetNTSS( i ); j++)
		{
			if (!HaveTSS( i, *block.GetTSS( i, j ) )) AddTSS( i, *block.GetTSS( i, j ) );
		}
	}

} // DSBlock::operator

bool DSBlock::SaveToScript( const char* savePath ) const
{
	char xmlFName[_MAX_PATH];
	sprintf( xmlFName, "%s\\%s.xml", savePath, GetName() );
	FILE* fp = fopen( xmlFName, "wt" );
	if (!fp) return false;
	int indent = 1;

	fprintf( fp, "<%s>\n", GetName() );
	indent++;

	dss::DSSParser& parser = dss::DSSParser::instance();

	if (m_RS.size() > 0)
	{
		indent++;
		fprintf( fp, "\t<RenderState>\n" );

		indent++;
		for (int i = 0; i < m_RS.size(); i++)
		{
			const char* cName = parser.GetStateName( m_RS[i], true );
			const char* cValue = parser.GetStateValue( m_RS[i], true );
			fprintf( fp, "\t\t<%s>%s</%s>\n", cName, cValue, cName );
		}
		indent--;

		fprintf( fp, "\t</RenderState>\n", m_Name.c_str() );
		indent--;
	}

	for (int i = 0; i < c_MaxTextureStages; i++)
	{
		if (m_TSS[i].size() == 0) continue;
		indent++;
		fprintf( fp, "\t<TextureStageState Stage=\"%d\">\n", i );

		indent++;
		for (int j = 0; j < m_TSS[i].size(); j++)
		{
			const char* cName = parser.GetStateName( (m_TSS[i])[j], false );
			const char* cValue = parser.GetStateValue( (m_TSS[i])[j], false );
			fprintf( fp, "\t\t<%s>%s</%s>\n", cName, cValue, cName );
		}
		indent--;

		fprintf( fp, "\t</TextureStageState>\n" );
		indent--;
	}

	indent--;
	fprintf( fp, "</%s>", m_Name.c_str() );

	fclose( fp );
	return true;
} // DSBlock::SaveToScript

bool DSBlock::HaveRS( const DeviceState& ds )
{
	for (int i = 0; i < m_RS.size(); i++) if (ds.devID == m_RS[i].devID) return true;
	return false;
}

bool DSBlock::HaveTSS( int stage, const DeviceState& ds )
{
	for (int i = 0; i < m_TSS[stage].size(); i++) 
	{
		if (ds.devID == (m_TSS[stage])[i].devID) return true;
	}
	return false;
}

/*****************************************************************************/
/*	DeviceState implementation
/*****************************************************************************/
InStream&	operator >>( InStream& is, DeviceState& ds )
{
	is >> ds.devID >> ds.value;
	return is;
}

OutStream&	operator <<( OutStream& os, const DeviceState& ds )
{
	os << ds.devID << ds.value;
	return os;
}

DeviceState::DeviceState( int _devID, DWORD _value )
{
	devID	 = _devID;
	value	 = _value;
} // DeviceState::DeviceState

int	DeviceState::CompareByDevID( const void* pDevState1, const void* pDevState2 )
{
	assert( pDevState1 && pDevState2 );
	DeviceState& ds1 = *((DeviceState*) pDevState1);
	DeviceState& ds2 = *((DeviceState*) pDevState2);
	if (ds1.devID < ds2.devID) return -1;
	else if (ds1.devID == ds2.devID) return 0;
	return 1;
} // DeviceState::CompareByDevID