/*****************************************************************************/
/*	File:	rsShaderDX.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	05.02.2003
/*****************************************************************************/

/*****************************************************************************/
/*	ShaderCache implementation
/*****************************************************************************/
_inl bool ShaderCache::SetShader( int ID )
{
	if (ID == currentShader || ID == 0) return false;
	HashedShader& sh = hash.elem( ID );
	HashedShader* inSh = table[ID][currentShader];
	if (!inSh)
	{
		int cachedID = getShaderID( hash.elem( ID ).name, currentShader );
		inSh = &(hash.elem( cachedID ));
		table[ID][currentShader] = inSh;
	}
	inSh->Apply( pDevice, this );
	currentShader = ID;

	INC_COUNTER( Shaders, 1 );

	return true;
} // ShaderCache::SetShader

/*****************************************************************************/
/*	Shader implementation
/*****************************************************************************/
_inl bool Shader::Apply( DXDevice* pDevice, ShaderCache* shaderCache )
{
	if (stateBlockID == -1)
	//  create state block
	{
		//  reset device to default state
		if (shaderCache && id > 0) shaderCache->SetShader( 0 );

		BOOL Lighting = FALSE;
		pDevice->GetRenderState( D3DRS_LIGHTING, (DWORD*)&Lighting );
	
		/*
		if (Lighting) pDevice->GetMaterial( &(ShaderState::curMtl) );
		else memset( &(ShaderState::curMtl), 0, sizeof(DXMaterial) );
		ShaderState::mchanged = false;

		
		for (int l = 0; l < c_MaxLights; l++)
		{
			if (Lighting) pDevice->GetLight( l, &(ShaderState::curLight[l]) );
			else memset( &(ShaderState::curLight[l]), 0, sizeof(DXLight) );
			ShaderState::lchanged[l] = false;
		}
		*/

		if (pDevice->BeginStateBlock() != S_OK) return false;

		DWORD token;

		for (int i = 0; i < nStates; i++)
		{
			if (!state[i].apply( pDevice )) 
			{
				if (pDevice->EndStateBlock( &token ) != S_OK) return false;
				stateBlockID = token;
				return false;
			}
		}

		
		//if (ShaderState::mchanged) pDevice->SetMaterial( &(ShaderState::curMtl) );
		//for (l = 0; l < c_MaxLights; l++)
		//{
		//	if (ShaderState::lchanged[l]) 
		//		pDevice->SetLight( l, &(ShaderState::curLight[l]) );
		//}
		//

		if (pDevice->EndStateBlock( &token ) != S_OK) return false;
		stateBlockID = token;

		////  measure performance
		//__int64 tm, tm1;
		//QueryPerformanceCounter( (LARGE_INTEGER*)&tm );
		//Apply( pDevice );
		//DWORD numPasses;
		//pDevice->ValidateDevice( &numPasses );
		//QueryPerformanceCounter( (LARGE_INTEGER*)&tm1 );
		//tm1 -= tm;
		//execTime = ((double)(tm1 * 1000000)) / perfCounterRes;

		return true;
	}

	//  apply corresponding state block
	DX_CHK( pDevice->ApplyStateBlock( stateBlockID ) );
	
	////  vertex shader
	//if (vshID != 0)
	//{
	//	DX_CHK( pDevice->SetVertexShader( vshID ) );
	//}

	
	/*static DXEffect* prevEff = 0;
	if (prevEff) prevEff->End();
	unsigned int numPasses;

	if (iEffect)
	{
		iEffect->SetTechnique( "T0" );
		iEffect->Begin( &numPasses, 0 );
		iEffect->Pass( 0 );
	}
	prevEff = iEffect;*/
	
	return true;
} // Shader::Apply
