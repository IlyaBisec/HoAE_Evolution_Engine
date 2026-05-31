/*****************************************************************
/*  File:   StrUtil.inl                                          *
/*  Desc:                                                        *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   Mar 2002                                             *
/*****************************************************************/

const string c_EmptyStr = "";

/*****************************************************************
/*	StringTable implementation
/*****************************************************************/

_inl void StringTable::clean()
{
	rows.clear();
}

_inl int StringTable::getNRows() const
{
	return rows.size();
}

_inl const string& StringTable::getRowID( int rowIdx ) const
{
	assert (rowIdx >= 0 && rowIdx < rows.size());
	return rows[rowIdx].id;
}

_inl const string& StringTable::getRowStr( int rowIdx ) const
{
	assert (rowIdx >= 0 && rowIdx < rows.size());
	return rows[rowIdx].str;
}

_inl void StringTable::addRow( const string& id, const string& str )
{
	STRecord nrec;
	nrec.id = id;
	nrec.str = str;
	rows.push_back( nrec );
}

_inl void StringTable::setRowID( int rowIdx, const string& id )
{
	if (rowIdx < 0 || rowIdx >= rows.size()) return;
	rows[rowIdx].id = id;
}

_inl void StringTable::setRowStr( int rowIdx, const string& str )
{
	if (rowIdx < 0 || rowIdx >= rows.size()) return;
	rows[rowIdx].str = str;
}