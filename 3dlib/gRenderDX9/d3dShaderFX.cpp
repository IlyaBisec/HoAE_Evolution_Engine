/*****************************************************************************/
/*	File:	d3dShaderFX.cpp
/*  Desc:	ShaderFX interface implementation for DirectX9 
/*	Author:	Ruslan Shestopalyuk
/*	Date:	02.11.2004
/*****************************************************************************/
#include "gRenderPch.h"
#include "d3dx9shader.h"
#include "d3dShaderFX.h"
#include "direct.h"
#include "IMediaManager.h"

/*****************************************************************************/
/*  FXIncluder implementation
/*****************************************************************************/
HRESULT FXIncluder::Open( D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, 
                            LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes )
{
    int size = 0;
    int resID = IRM->FindResource( pFileName );
    if (resID == -1) return false;
    if (m_pCurShader) IRM->BindResource( resID, m_pCurShader );

#ifdef DEBUG_VS
    FILE* fp = fopen( IRM->GetPath( resID ), "rb" );
    if (!fp) return E_FAIL;
    fseek( fp, 0, SEEK_END );
    size = ftell( fp );
    fseek( fp, 0, SEEK_SET );
    delete []m_pData;
    m_pData = new BYTE[size + 1];
    m_pData[size] = 0;
    fread( m_pData, size, 1, fp );
    fclose( fp );
    *ppData = m_pData;
    *pBytes = size;
    m_ResID = 0xFFFFFFFF;
    return S_OK;
#endif 

    m_pData = IRM->LockData( resID, size );
    if (!m_pData) return E_FAIL;
    *pBytes = size;
    *ppData = m_pData;
    m_ResID = resID;
    return S_OK;
} // FXIncluder::Open

HRESULT FXIncluder::Close( LPCVOID pData )
{
    if (m_ResID == 0xFFFFFFFF)
    {
        delete []m_pData;
        m_pData = NULL;
        return S_OK;
    }
    IRM->UnlockData( m_ResID );
    m_pData = NULL;
    return S_OK;
} // FXIncluder::Close

void FXIncluder::SetCurShader( ShaderFX* pShader ) 
{ 
    m_pCurShader = pShader; 
}

/*****************************************************************************/
/*  ShaderFX implementation
/*****************************************************************************/
Timer       ShaderFX::s_Timer;
FXIncluder  ShaderFX::s_FXIncluder;

ShaderFX::ShaderFX( IDirect3DDevice9* pDevice )
{
    m_pEffect       = NULL;
    m_ID            = -1;
    m_ActiveTech    = 0;
} // ShaderFX::ShaderFX

ShaderFX::ShaderFX()
{
    m_pEffect = NULL;
} // ShaderFX::ShaderFX

ShaderFX::~ShaderFX()
{
}

int ShaderFX::GetNTech() const
{
    return m_Techniques.size();
} // ShaderFX::GetNTechniques

const char* ShaderFX::GetTechName( int techID ) const
{
    if (techID < 0 || techID >= m_Techniques.size()) return ""; 
    return  m_Techniques[techID].m_Name.c_str();
} // ShaderFX::GetTechName

bool ShaderFX::IsTechValid( int techID ) const
{
    if (techID < 0 || techID >= m_Techniques.size()) return false;
    return (m_pEffect->ValidateTechnique( m_Techniques[techID].m_Handle ) == D3D_OK);
} // ShaderFX::IsTechValid

int ShaderFX::GetNPasses( int techID ) const
{
    if (techID < 0 || techID >= m_Techniques.size()) return 0;
    return m_Techniques[techID].m_NPasses;
} // ShaderFX::GetNPasses

ShaderVarValue ShaderFX::GetVariableValue( D3DXHANDLE h, ShaderVarType type )
{
    ShaderVarValue v;
    switch (type)
    {
    case svtBool:   
        {
            BOOL bVal;
            DX_CHK( m_pEffect->GetBool( h, &bVal ) ); 
            v.bVal = (bVal == 0) ? FALSE : TRUE;
        }break;
    case svtFloat:  DX_CHK( m_pEffect->GetFloat( h, &v.fVal ) ); break;
    case svtInt:    DX_CHK( m_pEffect->GetInt( h, &v.nVal ) ); break;
    case svtMatrix: 
        DX_CHK( m_pEffect->GetMatrix( h, (D3DXMATRIX*)&v.mVal ) ); 
        break;
    case svtVector: 
        DX_CHK( m_pEffect->GetVector( h, (D3DXVECTOR4*)&v.vVal ) ); 
        break;
    }
    return v;
} // ShaderFX::GetVariableValue

void ShaderFX::EnumerateTechniques()
{
    m_Techniques.clear();
    if (!m_pEffect) return;
    D3DXEFFECT_DESC desc;
    m_pEffect->GetDesc( &desc );
    int nT = desc.Techniques;
    for (int i = 0; i < nT; i++)
    {
        D3DXHANDLE h = m_pEffect->GetTechnique( i );
        ShaderFXTechnique tech;
        D3DXTECHNIQUE_DESC desc;
        m_pEffect->GetTechniqueDesc( h, &desc );
        tech.m_Handle       = h;
        tech.m_Name         = desc.Name;
        tech.m_NPasses      = desc.Passes;
        tech.m_NAnnotations = desc.Annotations;
        m_Techniques.push_back( tech );
    }
} // ShaderFX::EnumerateTechniques

void ShaderFX::EnumerateVariables()
{
    if (!m_pEffect) return;
    D3DXEFFECT_DESC desc;
    m_pEffect->GetDesc( &desc );
    int nC = desc.Parameters;

    m_Variables.clear();
    m_Variables.resize( nC );
    for (int i = 0; i < nC; i++)
    {
        ShaderFXVariable& c = m_Variables[i];
        D3DXHANDLE h = m_pEffect->GetParameter( NULL, i );
        D3DXPARAMETER_DESC desc;
        m_pEffect->GetParameterDesc( h, &desc );
        c.m_Name    = desc.Name;
        c.m_Type    = GetShaderVarType( desc.Class, desc.Type );
        //c.m_Value   = GetVariableValue( h, c.m_Type );
        c.m_Handle  = h;
    }
    //  search for automatic variables
    m_AutoVars.clear();
    for (int i = 0; i < nC; i++)
    {
        m_Variables[i].m_bAuto = false;
        for (int j = 0; j < (int)acLAST; j++)
        {
            if (!strcmp( c_AutoShaderVNames[j], m_Variables[i].m_Name.c_str() ))
            {
                ShaderFXAutoVar avar;
                avar.m_Idx = i;
                avar.m_Type = (AutoShaderVariable)j;
                m_Variables[i].m_bAuto = true;
                m_AutoVars.push_back( avar );
            }
        }
    }

} // ShaderFX::EnumerateVariables

bool ShaderFX::IsAutoVar( int cID )
{
    if (cID < 0 || cID >= m_Variables.size()) return true;
    return m_Variables[cID].m_bAuto;
}

const char* ShaderFX::GetVariableName( int cID ) const
{
    if (!m_pEffect || cID < 0 || cID >= m_Variables.size()) return "";
    return m_Variables[cID].m_Name.c_str();
} // ShaderFX::GetVariableName

int ShaderFX::GetShaderVarID( const char* cName )
{
    int nC = m_Variables.size();
    for (int i = 0; i < nC; i++)
    {
        if (!strcmp( cName, m_Variables[i].m_Name.c_str() )) return i;
    }
    return -1;
} // ShaderFX::GetShaderVarID

bool ShaderFX::GetShaderVar( int cID, bool& val )
{
    if (!m_pEffect || cID < 0 || cID >= m_Variables.size()) return false;
    HRESULT hRes = m_pEffect->GetBool( m_Variables[cID].m_Handle, (BOOL*)&val );
    return (hRes == S_OK);
} // ShaderFX::GetShaderVar

bool ShaderFX::GetShaderVar( int cID, float& val )
{
    if (!m_pEffect || cID < 0 || cID >= m_Variables.size()) return false;
    HRESULT hRes = m_pEffect->GetFloat( m_Variables[cID].m_Handle, &val );
    return (hRes == S_OK);
} // ShaderFX::GetShaderVar

bool ShaderFX::GetShaderVar( int cID, int& val )
{
    if (!m_pEffect || cID < 0 || cID >= m_Variables.size()) return false;
    HRESULT hRes = m_pEffect->GetInt( m_Variables[cID].m_Handle, &val );
    return (hRes == S_OK);
} // ShaderFX::GetShaderVar

bool ShaderFX::GetShaderVar( int cID, Matrix4D& val )
{
    if (!m_pEffect || cID < 0 || cID >= m_Variables.size()) return false;
    HRESULT hRes = m_pEffect->GetMatrix( m_Variables[cID].m_Handle, (D3DXMATRIX*)&val );
    return (hRes == S_OK);
} // ShaderFX::GetShaderVar

bool ShaderFX::GetShaderVar( int cID, Vector4D& val )
{
    if (!m_pEffect || cID < 0 || cID >= m_Variables.size()) return false;
    HRESULT hRes = m_pEffect->GetVector( m_Variables[cID].m_Handle, (D3DXVECTOR4*)&val );
    return (hRes == S_OK);
} // ShaderFX::GetShaderVar

bool ShaderFX::GetShaderVar( int cID, Vector3D& val )
{
    if (!m_pEffect || cID < 0 || cID >= m_Variables.size()) return false;
    HRESULT hRes = m_pEffect->GetVector( m_Variables[cID].m_Handle, (D3DXVECTOR4*)&val );
    return (hRes == S_OK);
} // ShaderFX::GetShaderVar}

bool ShaderFX::SetShaderVar( int cID, bool val )
{
    if (!m_pEffect || cID < 0 || cID >= m_Variables.size()) return false;
    HRESULT hRes = m_pEffect->SetBool( m_Variables[cID].m_Handle, (BOOL)val );
	DX_CHK( m_pEffect->CommitChanges() );
    return (hRes == S_OK);
} // ShaderFX::SetShaderVar

bool ShaderFX::SetShaderVar( int cID, float val )
{
    if (!m_pEffect || cID < 0 || cID >= m_Variables.size()) return false;
    HRESULT hRes = m_pEffect->SetFloat( m_Variables[cID].m_Handle, val );
	DX_CHK( m_pEffect->CommitChanges() );
    return (hRes == S_OK);
} // ShaderFX::SetShaderVar

bool ShaderFX::SetShaderVar( int cID, int val )
{
    if (!m_pEffect || cID < 0 || cID >= m_Variables.size()) return false;
    HRESULT hRes = m_pEffect->SetInt( m_Variables[cID].m_Handle, val );
	DX_CHK( m_pEffect->CommitChanges() );
    return (hRes == S_OK);
} // ShaderFX::SetShaderVar

bool ShaderFX::SetShaderVar( int cID, const Matrix4D& val )
{
    if (!m_pEffect || cID < 0 || cID >= m_Variables.size()) return false;
    Matrix4D tVal;
    tVal.transpose( val );
    HRESULT hRes = m_pEffect->SetMatrix( m_Variables[cID].m_Handle, (const D3DXMATRIX*)&tVal );
	DX_CHK( m_pEffect->CommitChanges() );
    return (hRes == S_OK);
} // ShaderFX::SetShaderVar

bool ShaderFX::SetShaderVar( int cID, const Vector4D& val )
{
    if (!m_pEffect || cID < 0 || cID >= m_Variables.size()) return false;
    HRESULT hRes = m_pEffect->SetVector( m_Variables[cID].m_Handle, (const D3DXVECTOR4*)&val );
	DX_CHK( m_pEffect->CommitChanges() );
    return (hRes == S_OK);
} // ShaderFX::SetShaderVar

bool ShaderFX::SetShaderVar( int cID, const Vector4D* val ,int count)
{
	if (!m_pEffect || cID < 0 || cID >= m_Variables.size()) return false;
	HRESULT hRes = m_pEffect->SetVectorArray( m_Variables[cID].m_Handle, (const D3DXVECTOR4*)val , count);
	DX_CHK( m_pEffect->CommitChanges() );
	return (hRes == S_OK);
} // ShaderFX::SetShaderVar

bool ShaderFX::SetShaderVar( int cID, const Vector3D& val )
{
    if (!m_pEffect || cID < 0 || cID >= m_Variables.size()) return false;
    HRESULT hRes = m_pEffect->SetVector( m_Variables[cID].m_Handle, (const D3DXVECTOR4*)&val );
	DX_CHK( m_pEffect->CommitChanges() );
    return (hRes == S_OK);
} // ShaderFX::SetShaderVar

void ShaderFX::SetAutoVars()
{
    if (!m_pEffect) return;
    //  TODO - some funky virtual dispatching?..
    int nV = m_AutoVars.size();
    for (int i = 0; i < nV; i++)
    {
        const ShaderFXAutoVar& av = m_AutoVars[i];
        D3DXHANDLE h = m_Variables[av.m_Idx].m_Handle;
        switch (av.m_Type)
        {
            case acWorldTM: 
                {
                    Matrix4D wTM = IRS->GetWorldTM();
                    wTM.transpose();
                    DX_CHK( m_pEffect->SetMatrix( h, (const D3DXMATRIX*)&wTM ) ); 
                }break;			
            case acViewTM: 
                {
                    Matrix4D vTM = IRS->GetViewTM();
                    vTM.transpose();
                    DX_CHK( m_pEffect->SetMatrix( h, (const D3DXMATRIX*)&vTM ) );
                }break;				
            case acProjTM: 
                {
                    Matrix4D pTM = IRS->GetProjTM();
                    pTM.transpose();
                    DX_CHK( m_pEffect->SetMatrix( h, (const D3DXMATRIX*)&pTM ) );
                }break;				
            case acViewProjTM: 
                {
                    Matrix4D vpTM;
                    vpTM.mul( IRS->GetViewTM(), IRS->GetProjTM() );
                    vpTM.transpose();
                    DX_CHK( m_pEffect->SetMatrix( h, (const D3DXMATRIX*)&vpTM ) );
                }break;			
            case acWorldViewProjTM: 
                {
                    Matrix4D wvpTM( IRS->GetWorldTM() );
                    wvpTM *= IRS->GetViewTM();
                    wvpTM *= IRS->GetProjTM();
                    wvpTM.transpose();
					//DX_CHK( m_pEffect->SetMatrix( h, (const D3DXMATRIX*)&IRS->GetWorldViewProjTM() ) );
                    DX_CHK( m_pEffect->SetMatrix( h, (const D3DXMATRIX*)&wvpTM) );
                }break;	
            case acWorldViewTM: 
                {
                    Matrix4D wvTM;
                    wvTM.mul( IRS->GetWorldTM(), IRS->GetViewTM() );
                    wvTM.transpose();
                    DX_CHK( m_pEffect->SetMatrix( h, (const D3DXMATRIX*)&wvTM ) );
                }break;	
            case acViewPos:	    
                {
                    const Matrix4D& vTM = IRS->GetViewTM();
                    Vector4D pos;
                    pos.x = -(vTM.e00*vTM.e30 + vTM.e01*vTM.e31 + vTM.e02*vTM.e32);
                    pos.y = -(vTM.e10*vTM.e30 + vTM.e11*vTM.e31 + vTM.e12*vTM.e32);
                    pos.z = -(vTM.e20*vTM.e30 + vTM.e21*vTM.e31 + vTM.e22*vTM.e32);
					Matrix4D ctm;
					ctm.inverse( vTM );
					pos.x = ctm.e30;
					pos.y = ctm.e31;
					pos.z = ctm.e32;
                    pos.w = 1.0f;
                    DX_CHK( m_pEffect->SetVector( h, (const D3DXVECTOR4*)&pos ) );
                }break;
            case acLightPos:            break;				
            case acLightDir:            break;				
            case acLightPosObjSpace:    break;		
            case acLightDirObjSpace:    break;		
            case acLightDiffuse:
                {
                    static int LightID=IMM->GetNodeID( "GameLight" );
                    ILight* IL=IMM->GetLight(LightID);                    
                    ColorValue cv( IL->GetDiffuse() );
                    DX_CHK( m_pEffect->SetVector( h, &D3DXVECTOR4( cv.r, cv.g, cv.b, cv.a ) ) );
                }
                break;
            case acLightSpecular:
                {
                    static int LightID=IMM->GetNodeID( "GameLight" );
                    ILight* IL=IMM->GetLight(LightID);                    
                    ColorValue cv( IL->GetSpecular() );
                    DX_CHK( m_pEffect->SetVector( h, &D3DXVECTOR4( cv.r, cv.g, cv.b, cv.a ) ) );
                }
                break;
            case acLightAmbient:
                {
                    static int LightID=IMM->GetNodeID( "GameLight" );
                    ILight* IL=IMM->GetLight(LightID);                    
                    ColorValue cv( IL->GetAmbient() );
                    DX_CHK( m_pEffect->SetVector( h, &D3DXVECTOR4( cv.r, cv.g, cv.b, cv.a ) ) );
                }
                break;
            case acFogDensity:
                DX_CHK( m_pEffect->SetFloat( h, IRS->GetFogDensity() ) );
                break;
            case acMaterialDiffuse:
                {
                    static int LightID=IMM->GetNodeID( "GameLight" );
                    ILight* IL=IMM->GetLight(LightID);                    
                    ColorValue cv( IL->GetDiffuse() );
                    DX_CHK( m_pEffect->SetVector( h, &D3DXVECTOR4( cv.r, cv.g, cv.b, cv.a ) ) );
                }
                break;
            case acMaterialSpecular:
                {
                    static int LightID=IMM->GetNodeID( "GameLight" );
                    ILight* IL=IMM->GetLight(LightID);                    
                    ColorValue cv( IL->GetSpecular() );
                    DX_CHK( m_pEffect->SetVector( h, &D3DXVECTOR4( cv.r, cv.g, cv.b, cv.a ) ) );
                }
                break;
            case acMaterialAmbient:
                {
                    static int LightID=IMM->GetNodeID( "GameLight" );
                    ILight* IL=IMM->GetLight(LightID);                    
                    ColorValue cv( IL->GetAmbient() );
                    DX_CHK( m_pEffect->SetVector( h, &D3DXVECTOR4( cv.r, cv.g, cv.b, cv.a ) ) );
                }
                break;

            case acTime:   
                {
					FLOAT t;
					if(!IRS->TimeOverrideIsEnabled(&t)) {
						t = s_Timer.seconds();
					}
                    DX_CHK( m_pEffect->SetFloat( h, t ) );
                }break;	

            case acTextureTM0:   
                {
                    Matrix4D tm = IRS->GetTextureTM( 0 );
                    tm.transpose();
                    DX_CHK( m_pEffect->SetMatrix( h, (const D3DXMATRIX*)&tm ) );
                }break;

            case acTextureTM1:   
                {
                    Matrix4D tm = IRS->GetTextureTM( 1 );
                    tm.transpose();
                    DX_CHK( m_pEffect->SetMatrix( h, (const D3DXMATRIX*)&tm ) );
                }break;
            case acTextureTM2:   
                {
                    Matrix4D tm = IRS->GetTextureTM( 2 );
                    tm.transpose();
                    DX_CHK( m_pEffect->SetMatrix( h, (const D3DXMATRIX*)&tm ) );
                }break;
            case acTextureTM3:   
                {
                    Matrix4D tm = IRS->GetTextureTM( 3 );
                    tm.transpose();
                    DX_CHK( m_pEffect->SetMatrix( h, (const D3DXMATRIX*)&tm ) );
                }break;
            case acTFactor:
                {
                    ColorValue cv( IRS->GetTextureFactor() );
                    DX_CHK( m_pEffect->SetVector( h, &D3DXVECTOR4( cv.r, cv.g, cv.b, cv.a ) ) );
                }break;
            case acColorConst:
                {
                    ColorValue cv( IRS->GetColorConst() );
                    DX_CHK( m_pEffect->SetVector( h, &D3DXVECTOR4( cv.r, cv.g, cv.b, cv.a ) ) );
                }break;
			case acFarPlane:
				{
					const Matrix4D& pTM = IRS->GetProjTM();
					DX_CHK( m_pEffect->SetFloat( h, pTM.e32/(1.0f - pTM.e22) ) );
				}break;
			case acNearPlane:
				{
					const Matrix4D& pTM = IRS->GetProjTM();
					DX_CHK( m_pEffect->SetFloat( h, -pTM.e32/pTM.e22 ) );
				}break;
			case acShaderConst0:
			case acShaderConst1:
			case acShaderConst2:
			case acShaderConst3:
				DX_CHK( m_pEffect->SetFloat( h, IRS->GetShaderConst(av.m_Type-acShaderConst0) ) );
				break;
        }
    }

    DX_CHK( m_pEffect->CommitChanges() );
} // ShaderFX::SetAutoVars

bool ShaderFX::Load( const char* fName )
{
    m_FileName = fName;
    int size = 0;
    int resID = IRM->FindResource( fName );
    if (resID == -1) return false;
    IRM->BindResource( resID, this );

#ifdef DEBUG_VS
    FilePath path( IRM->GetFullPath( resID ) );
    path.SetFileName( fName );
    return LoadFromFile( path.GetFullPath() );
#endif 

    BYTE* pData = IRM->LockData( resID, size );
    bool res = LoadFromMemory( pData, size );
    IRM->UnlockData( resID );
    
    return res;
} // ShaderFX::Load

IDirect3DDevice9* GetDirect3DDevice();
bool ShaderFX::LoadFromFile( const char* fName )
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = 0;
#ifdef DEBUG_VS
    dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
    dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif
    ID3DXBuffer* pErrBuffer = NULL;
    IDirect3DDevice9* pDevice = GetDirect3DDevice();
    s_FXIncluder.SetCurShader( this );
    hr = D3DXCreateEffectFromFile( pDevice, fName, NULL, NULL/*&s_FXIncluder*/, dwShaderFlags, NULL, &m_pEffect, &pErrBuffer );
    if (hr != S_OK && pErrBuffer)
    {
        const char* pErrText = (const char*)pErrBuffer->GetBufferPointer();
        Log.Error( "Error in shader %s: %s", fName, pErrText );
    }
    EnumerateVariables();
    EnumerateTechniques();
    return (hr == S_OK);
} // ShaderFX::LoadFromFile

bool ShaderFX::LoadFromMemory( const BYTE* pBuf, int bufSize )
{
    if (!pBuf || bufSize == 0) return false;
    HRESULT hr = S_OK;
    DWORD dwShaderFlags = 0;
#ifdef DEBUG_VS
    dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
    dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif
    ID3DXBuffer* pErrBuffer = NULL;
    IDirect3DDevice9* pDevice = GetDirect3DDevice();
    s_FXIncluder.SetCurShader( this );
    hr = D3DXCreateEffect( pDevice, pBuf, bufSize, NULL, &s_FXIncluder, dwShaderFlags, NULL, &m_pEffect, &pErrBuffer );
    
    if (hr != S_OK)
    {
        DWORD err = GetLastError();
        if (pErrBuffer)
        {
            const char* pErrText = (const char*)pErrBuffer->GetBufferPointer();
            Log.Warning( "Error in shader %s: %s", m_FileName.c_str(), pErrText );
        }
        else if (hr == E_OUTOFMEMORY)
        {
            Log.Warning( "Not enough memory to compile shader %s", m_FileName.c_str() );
        }
        else 
        {
            Log.Warning( "Could not compile shader %s:  %s", m_FileName.c_str(), GetD3DErrorDesc( hr ) );
        }
    }
    
    EnumerateVariables();
    EnumerateTechniques();
    return (hr == S_OK);
} // ShaderFX::LoadFromMemory
bool ShaderFX::Begin()
{
    if (!m_pEffect) return false;
    UINT nPasses = 0;
    m_pEffect->Begin( &nPasses, 0 );
    return true;
} // ShaderFX::Begin

bool ShaderFX::BeginPass( int passID )
{
    if (!m_pEffect) return false;
    m_pEffect->BeginPass( passID );
    return true;
} // ShaderFX::BeginPass

bool ShaderFX::EndPass()
{
    if (!m_pEffect) return false;
    m_pEffect->EndPass();
    return true;
} // ShaderFX::EndPass

bool ShaderFX::End()
{
    if (!m_pEffect) return false;
    m_pEffect->End();
    return true;
} // ShaderFX::End

bool ShaderFX::Reload() 
{
    DeleteDeviceObjects();
    return Load( m_FileName.c_str() );
} // ShaderFX::Reload

void ShaderFX::DeleteDeviceObjects()
{
    SAFE_RELEASE( m_pEffect );
} // ShaderFX::DeleteDeviceObjects

void ShaderFX::InvalidateDeviceObjects()
{
    if (m_pEffect) m_pEffect->OnLostDevice();
} // ShaderFX::InvalidateDeviceObjects

void ShaderFX::RestoreDeviceObjects() 
{
    if (m_pEffect) m_pEffect->OnResetDevice();
} // ShaderFX::RestoreDeviceObjects

