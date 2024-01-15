/*****************************************************************/
/*  File:   sgSpriteManager.inl
/*  Desc:   Sprite manager
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/


#pragma warning ( disable : 4311 )
/*****************************************************************************/
/*	SpritePackage implementation
/*****************************************************************************/
_inl bool SpritePackage::AdjustWorkBuffer( int size )
{
	if (s_WorkBufferSize < size)
	{
		delete []s_WorkBuffer;
		s_WorkBuffer = new BYTE[size];
		s_WorkBufferSize = size;
		return true;
	}
	return false;
} // SpritePackage::AdjustWorkBuffer

_inl bool SpritePackage::AdjustUnpackBuffer( int size )
{
	if (s_UnpackBufferSize < size)
	{
		delete []s_UnpackBuffer;
		s_UnpackBuffer = new BYTE[size];
		s_UnpackBufferSize = size;
		return true;
	}
	return false;
} // SpritePackage::AdjustUnpackBuffer

/*****************************************************************************/
/*	SpriteSurface implementation
/*****************************************************************************/
int SpriteSurface::AddFrameInstance( FrameInstance* pInstance )
{
	if (!pInstance) return 0;
	m_pFrameInstance.push_back( pInstance );
	return m_pFrameInstance.size() - 1;
} // SpriteSurface::AddFrameInstance

/*****************************************************************/
/*	SpriteManager implementation
/*****************************************************************/
_inl FrameInstance*	SpriteManager::FindFrameInstance( int gpID, int sprID, DWORD color, WORD lod )
{
	int frameID = m_FrameReg.find( FrameInstance::Key( gpID, sprID, color, lod ) );
	if (frameID == NO_ELEMENT) return NULL;
	return m_FrameReg.elem( frameID );
} // SpriteManager::FindFrameInstance

_inl int SpriteManager::AllocateQuad( int sidePow, WORD& ax, WORD& ay, FrameInstance* pInst, int prevID )
{
	SpriteSurface* pSurface = &m_Surface[m_CurSurface];
	bool res = pSurface->m_Layout.AllocChunk( sidePow, ax, ay ); 
	if (res && prevID != m_CurSurface) pSurface->AddFrameInstance( pInst );
	
	if (!res) 
	//  no more place on the current texture
	{
		DWORD frame = IRS->GetCurFrame();
		bool bRes = false;
        Flush();
        while (!bRes)
        {
		    m_CurSurface = (m_CurSurface + 1) % m_Surface.size();
		    pSurface = &m_Surface[m_CurSurface];
		    bRes = pSurface->Free();
        }
		res = pSurface->m_Layout.AllocChunk( sidePow, ax, ay );
		if (!res ) return -1;
		pSurface->AddFrameInstance( pInst );
	}	
	return m_CurSurface;
} // SpriteManager::AllocateQuad

_inl FrameInstance* SpriteManager::GetFrameInstance( int& gpID, int& sprID, DWORD color, WORD lod, bool bPrecache )
{
	FrameInstance* frameInst = NULL; 
	if (gpID < 0 || gpID >= m_NPackages || sprID < 0) return NULL;
	PackageStub& stub = m_PackageReg[gpID];
	if (stub.m_bNoFile) return NULL; 
	SpritePackage* pPackage = stub.m_pPackage;

	if (!pPackage) 
	{
		LoadPackage( gpID );
		pPackage = GetPackage( gpID );
		if (!pPackage) return NULL;
	}
    

	sprID = UnswizzleFrameIndex( gpID, sprID );

    FrameInstance* pInst = FindFrameInstance( gpID, sprID, color, lod );
    //if (pInst && pInst->IsCached()) return pInst;
    if (pInst) return pInst;
    pInst = pPackage->PrecacheFrame( sprID, color, lod );
	return pInst;
} // SpriteManager::GetFrameInstance

/*---------------------------------------------------------------------------*/
/*	Func:	SpriteManager::UnswizzleFrameIndex
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
_inl int SpriteManager::UnswizzleFrameIndex( int gpID, int sprID )
{
	SpritePackage* pPackage = GetPackage( gpID );
	assert( pPackage );
	int nDir = pPackage->GetNDirections();
	if (nDir > 1)
	{
		int nSpr = pPackage->GetNFrames();
		int seqLen = nSpr / nDir;
		sprID = sprID / nDir + seqLen * (sprID % nDir);
	}
	return sprID;
} // SpriteManager::UnswizzleFrameIndex

_inl int SpriteManager::GetNFrames( int seqID ) 
{
	if (seqID < 0 || seqID >= m_NPackages) return 0x7FFFFFFF;
	const SpritePackage* seq = GetPackage( seqID );
    if (!seq) 
    {
        LoadPackage( seqID );
        seq = GetPackage( seqID );
    }  
	if (!seq) return 0x7FFFFFFF;
	return seq->GetNFrames();
} // SpriteManager::GetNFrames

_inl bool SpriteManager::IsPackageLoaded( int seqID ) const
{
	if (seqID < 0 || seqID >= m_NPackages) return false;
	const SpritePackage* seq = GetPackage( seqID );
	if (!seq) return false;
	return true;
} // SpriteManager::IsPackageLoaded

_inl const char* SpriteManager::GetPackageName( int seqID )
{
	static char empty[] = "";
	if (seqID < 0 || seqID >= m_NPackages) return empty;
	return m_PackageReg[seqID].m_Name;
} // SpriteManager::GetPackageName

