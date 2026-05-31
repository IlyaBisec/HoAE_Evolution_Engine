/*****************************************************************
/*  File:   mSegmentSet.inl	                                         
/*	Desc:	System of segments in 3D space 
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   Mar 2004                                          
/*****************************************************************/

/*****************************************************************/
/*	SegmentChain implementation
/*****************************************************************/
_inl void SegmentChain::Transform( const Matrix4D& tm )
{
	for (int i = 0; i < m_Points.size(); i++) tm.transformPt( m_Points[i] );
}

/*****************************************************************/
/*	SegmentSet implementation
/*****************************************************************/
_inl void SegmentSet::Transform( const Matrix4D& tm )
{
	for (int i = 0; i < m_Segments.size(); i++) 
	{
		tm.transformPt( m_Segments[i].Orig() );
		tm.transformVec( m_Segments[i].Dir() );
	}
} // SegmentSet::Transform
