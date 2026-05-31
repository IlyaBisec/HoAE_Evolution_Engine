/*****************************************************************
/*  File:   GPSystem.inl                                         *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   Mar 2002                                             *
/*****************************************************************/

const int c_BigPositiveInt = INT_MAX;

/*****************************************************************
/*  GPSystem implementation										 *
/*****************************************************************/
_inl int GPSystem::SetCurrentShader( int shID ) 
{ 
	int res = GPBucket::s_CurShader; 
	GPBucket::s_CurShader = shID; 
	return res;
} // GPSystem::SetCurShader

_inl int GPSystem::GetCurrentShader() const
{ 
	return GPBucket::s_CurShader;
} // GPSystem::SetCurShader
	
_inl void GPSystem::SetCurrentZ( float z )
{
	curZ = z;	
}

_inl float	GPSystem::GetCurrentZ()	const
{
	return curZ;
}
_inl DWORD	GPSystem::GetCurrentDiffuse() const
{
	return curDiffuse;
}

_inl void GPSystem::SetCurrentDiffuse( DWORD color )
{
	curDiffuse = color;
}

_inl void GPSystem::Drop( GPFrameInst* frInst )
{
	int id = frameHash.find( GPFrameInst::Key( frInst->gpID, frInst->sprID, 
												frInst->color, frInst->lod ) );
	if (id != NO_ELEMENT)
	{
		frameHash.entry( id ).el = NULL;
	}
}

_inl int GPSystem::GPNFrames( int seqID ) const
{
	if (seqID < 0 || seqID >= gpHash.numElem()) return 0;
	const GPSeq* seq = c_GetSeq( seqID );
	if (!seq || (DWORD)seq == c_NoGPFile) return 0x7FFFFFFE;
	return seq->GetNFrames();
} // GPSystem::GPNFrames

_inl const char* GPSystem::GetGPName( int seqID )
{
	static char empty[] = "";
	if (seqID < 0 || seqID >= gpHash.numElem()) return empty;
	return gpHash.entry( seqID ).key;
} // GPSystem::GetGPName

_inl int GPSystem::GetGPHeight( int seqID, int frameIdx )
{
	if (seqID		<  0					|| 
		seqID		>= gpHash.numElem()		||
		frameIdx	>= GPNFrames( seqID )	|| 
		frameIdx	<  0) 
	{
		return c_BigPositiveInt;
	}
	const GPFrame* frame = c_GetFrame( seqID, frameIdx );
	if (!frame) 
	{
		LoadGP( seqID );
		frame = c_GetFrame( seqID, frameIdx );
		if (!frame) return c_BigPositiveInt;
	}	
	return frame->height;
} // GPSystem::GetGPHeight

_inl bool GPSystem::GetGPBoundFrame( int seqID, int frameIdx, Rct& frame )
{
	if (seqID < 0 || seqID >= gpHash.numElem() ||
		frameIdx >= GPNFrames( seqID ) || frameIdx < 0) return false;
	const GPFrameInst* frInst = GetFrameInst( seqID, frameIdx, 0 );
	if (!frInst) return false;
	massert( frInst, "GPSystem::GetGPBoundFrame - no frame instance." );
	int left, top;
	if (!frInst->GetLeftTop( left, top )) return false;
	frame.x = left;
	frame.y = top;
	frame.w = GetGPWidth( seqID, frameIdx ) - frame.x;
	frame.h = GetGPHeight( seqID, frameIdx ) - frame.y;
	return true;
} // GPSystem::GetGPBoundFrame
	

_inl GPFrame* GPSystem::GetFrame( int seqID, int sprID )
{
	return c_GetSeq( seqID )->GetFrame( UnswizzleFrameIndex( seqID, sprID ) );
} // GPSystem::GetFrame

_inl const GPFrame* GPSystem::c_GetFrame( int seqID, int sprID ) const
{
	//  lots of crap here...
	GPSeq* seq = (GPSeq*)c_GetSeq( seqID );
	if (!seq || (DWORD)seq == c_NoGPFile) return NULL;
	return seq->GetFrame( UnswizzleFrameIndex( seqID, sprID ) );
} // GPSystem::GetFrame

_inl int GPSystem::GetGPWidth( int seqID, int frameIdx )
{
	if (seqID		<  0					|| 
		seqID		>= gpHash.numElem()		||
		frameIdx	>= GPNFrames( seqID )	|| 
		frameIdx	<  0) 
	{
		return c_BigPositiveInt;
	}
	const GPFrame* frame = c_GetFrame( seqID, frameIdx );
	if (!frame) 
	{
		LoadGP( seqID );
		frame = c_GetFrame( seqID, frameIdx );
		if (!frame) return c_BigPositiveInt;
	}
	return frame->width;
} // GPSystem::GetGPWidth

_inl void GPSystem::SetGPPath( int seqID, const char* gpPath )
{
	if (seqID < 0 || seqID >= gpHash.numElem()) 
	{
		Log.Warning( "Incorrect GPID in GPSystem::SetGPPath, seqID:%d, gpPath:<%s>",
						seqID, gpPath );
		return;
	}
	GetSeq( seqID )->SetPath( gpPath );
} // GPSystem::SetGPPath

_inl int GPSystem::GetGPNum() const
{
	return gpHash.numElem();
}

_inl void GPSystem::SetCurrentCamera( Camera* cam )
{
	curCamera = cam;
} // GPSystem::setCurrentCamera

_inl float GPSystem::GetScale() const
{
	return gpScale;
} // GPSystem::GetScale
	
_inl float GPSystem::SetScale( float scale )
{
	float old = gpScale;
	gpScale = scale;

	if (gpScale > 0.5f)
	{
		curLOD = 0;
	}
	else if (gpScale > 0.25f)
	{
		curLOD = 1;
	}
	else if (gpScale > 0.125f)
	{
		curLOD = 2;
	}
	else
	{
		curLOD = 3;
	}

	return scale;
} // GPSystem::SetScale

_inl void GPSystem::EnableClipping( bool enable )
{
	clippingEnabled = enable;
}

_inl void GPSystem::FlushPrimBucket()
{
	if (lineBucket2D.getNVert() != 0)
	{
		RC::iRS->Draw( lineBucket2D );
		lineBucket2D.setNInd ( 0 );
		lineBucket2D.setNVert( 0 );
		lineBucket2D.setNPri ( 0 );
	}

	if (lineBucket3D.getNVert() != 0)
	{
		RC::iRS->Draw( lineBucket3D );
		lineBucket3D.setNInd ( 0 );
		lineBucket3D.setNVert( 0 );
		lineBucket3D.setNPri ( 0 );
	}

	if (rectBucket2D.getNVert() != 0)
	{
		RC::iRS->Draw( rectBucket2D );
		rectBucket2D.setNInd ( 0 );
		rectBucket2D.setNVert( 0 );
		rectBucket2D.setNPri ( 0 );
	}

	if (rectBucket3D.getNVert() != 0)
	{
		RC::iRS->Draw( rectBucket3D );
		rectBucket3D.setNInd ( 0 );
		rectBucket3D.setNVert( 0 );
		rectBucket3D.setNPri ( 0 );
	}
} // GPSystem::flushBucket

_inl void GPSystem::FlushSpriteBucket( int bucketIdx )
{
	assert( bucketIdx >= 0 && bucketIdx < s_GPTexNum );
	sprBucket[bucketIdx].Flush();
}

_inl int GPSystem::GetNumTextureCacheSurfaces()
{
	return s_GPTexNum;
}

_inl bool GPSystem::SetClipArea( DWORD x, DWORD y, 
								 DWORD w, DWORD h )
{
	if (curViewPort.x == x && curViewPort.y == y &&
		curViewPort.width == w && curViewPort.height == h)
	{
		return false;
	}
	curViewPort.x		= x;
	curViewPort.y		= y;
	curViewPort.width	= w;
	curViewPort.height	= h;
	curViewPort.MinZ	= 0.0f;
	curViewPort.MaxZ	= 1.0f;

	//  viewport is changed, so we need to flush all batched stuff here
	if (RC::iRS) 
	{
		FlushBatches();
		RC::iRS->SetViewPort( curViewPort );
	}
	return true;
} // GPSystem::SetClipArea

_inl const ViewPort& GPSystem::GetClipArea() const
{
	return curViewPort;
} // GPSystem::GetClipArea

_inl GPSeq*	GPSystem::GetSeq( int idx ) 
{
	return gpHash.elem( idx );
} // GPSystem::GetSeq

_inl GPSeq*	GPSystem::c_GetSeq( int idx ) const
{
	return gpHash.c_elem( idx );
}

_inl GPFrameInst* GPSystem::GetFrameInstForDrawing( int& gpID, int& sprID, DWORD color, WORD lod )
{
	GPSeq* seq = gpHash.elem( gpID );
	if (!seq) LoadGP( gpID );
	seq = gpHash.elem( gpID );
	if (reinterpret_cast<DWORD>( seq ) == c_NoGPFile) return NULL;
	if (!seq) return NULL;
	if (sprID < 0 || sprID >= seq->GetNFrames())
	{
		Log.Warning( "Frame index is outside boundary: gp:%s fr:%d", seq->GetPath(), sprID );
		return NULL;
	}

	sprID = UnswizzleFrameIndex( gpID, sprID );
	GPFrameInst* frameInst = GetFrameInst( gpID, sprID, color, lod );

	// ensure that pixel data is already in texture memory
	if (!frameInst || !frameInst->Touch()) return NULL;
	frameInst->Hit( curCacheFactor );
	return frameInst;
} // GPSystem::GetFrameInstForDrawing

/*---------------------------------------------------------------------------*/
/*	Func:	GPSystem::UnswizzleFrameIndex
/*	Desc:	Unswizzle frame index in case of "directional indexing"
/*	Parm:	Converts frame indexing from representation where frames are
/*			ordedered like
/*			<dir0:anmFrame0, dir1:anmFrame0, ...> 
/*			<dir0:anmFrame1, dir1:anmFrame1, ...>...
/*			to the order
/*			<dir0:anmFrame0, dir0:anmFrame1, ...> 
/*			<dir1:anmFrame0, dir1:anmFrame1, ...>...
/*			First case is convenient for cossacks engine, second for compression
/*	Ret:	Converted frame index
/*---------------------------------------------------------------------------*/
_inl int GPSystem::UnswizzleFrameIndex( int gpID, int sprID ) const
{
	GPSeq* seq = c_GetSeq( gpID );
	if (!seq) 
	{
		((GPSystem*)this)->LoadGP( gpID );
		seq = c_GetSeq( gpID );
		if (!seq) return sprID;
	}
	assert( seq );
	int nDir = seq->GetNDirections();
	if (nDir > 1)
	{
		int nSpr = seq->GetNFrames();
		int seqLen = nSpr / nDir;
		sprID = sprID / nDir + seqLen * (sprID % nDir);
	}
	return sprID;
} // GPSystem::UnswizzleFrameIndex

_inl GPFrameInst* GPSystem::GetFrameInst( int gpID, int sprID, DWORD color, WORD lod )
{
	//  find given frame reference in hash
	int frameID = frameHash.find( GPFrameInst::Key( gpID, sprID, color, lod ) );
	if (frameID == NO_ELEMENT || frameHash.elem( frameID ) == NULL)
	//  create frame instance
	{
		GPSeq* seq = gpHash.elem( gpID );
		if (!seq) return NULL;
		if (reinterpret_cast<DWORD>( seq ) == c_NoGPFile) return NULL;

		int nChunks	= seq->GetFrame( sprID )->nChunks;
		int nExtra	= nChunks * sizeof( GPChunk );
		GPFrameInst* pInst = alloc.NewInstance( nExtra );
		assert( pInst );

		pInst->SetNChunks( nChunks, (void*)(pInst + 1) );

		if (!pInst) return NULL;

		pInst->gpID		= seq->GetID(); 
		pInst->pFrame	= seq->GetFrame( sprID );
		pInst->color	= color;
		pInst->lod		= lod;
		pInst->pSeq		= seq;

		frameID = frameHash.add( GPFrameInst::Key( gpID, sprID, color, lod ), pInst );
		pInst->InitPrefix();
	}

	return frameHash.elem( frameID );
} // GPSystem::GetFrameInst





