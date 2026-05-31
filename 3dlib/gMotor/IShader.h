/*****************************************************************
/*  File:   IShader.h                                      
/*  Desc:   Interface to the shader manipulation
/*    Author:    Ruslan Shestopalyuk
/*  Date:   Jun 2004                                             
/*****************************************************************/
#ifndef __ISHADER_H__ 
#define __ISHADER_H__

class Matrix4D;
class Vector4D;
class Vector3D;

/*****************************************************************/
/*    Enum:    AutoShaderVariable
/*    Desc:    Predefined types of the shader constants
/*****************************************************************/
enum AutoShaderVariable
{
    acWorldTM               = 0,     //    current object's world transform
    acViewTM                = 1,     //    camera view transform matrix
    acProjTM                = 2,     // camera projection transform matrix
    acViewProjTM            = 3,     //    combined view/projection matrix
    acWorldViewProjTM       = 4,     //    combined world/view/projection matrix
    acWorldViewTM           = 5,     //    combined world/view matrix
    acViewPos               = 6,     //    viewer position

    acLightPos              = 7,     //    light source position
    acLightDir              = 8,     //    light source direction
    acLightPosObjSpace      = 9,     //    light source position in object's space
    acLightDirObjSpace      = 10,    //    light source direction in object's space

    acLightDiffuse          = 11,    //    light diffuse color
    acLightSpecular         = 12,    //    light specular color
    acLightAmbient          = 13,    //    light ambient color

    acMaterialDiffuse       = 14,    //    material diffuse color
    acMaterialSpecular      = 15,    //    material specular color
    acMaterialAmbient       = 16,    //    material ambient color

    acTime                  = 17,    // current application time, in seconds 
    acFogDensity            = 18,

    acTextureTM0            = 19,    //  0th texture stage texture transform
    acTextureTM1            = 20,    //  1st texture stage texture transform
    acTextureTM2            = 21,    //  2nd texture stage texture transform
    acTextureTM3            = 22,    //  3rd texture stage texture transform

    acTFactor               = 23,

	acFarPlane              = 24,
	acNearPlane             = 25,

    acColorConst            = 26,

	acShaderConst0			= 27,
	acShaderConst1			= 28,
	acShaderConst2			= 29,
	acShaderConst3			= 30,

    acLAST                  = 31,    // MUST be last!!!
}; // enum AutoShaderVariable

static const char* c_AutoShaderVNames[] = 
{
    "WorldTM",
    "ViewTM",
    "ProjTM",            
    "ViewProjTM",        
    "WorldViewProjTM",    
    "WorldViewTM",        
    "ViewPos",            
    "LightPos",            
    "LightDir",            
    "LightPosObjSpace",    
    "LightDirObjSpace",    
    "LightDiffuse",        
    "LightSpecular",        
    "LightAmbient",        
    "MaterialDiffuse",    
    "MaterialSpecular",    
    "MaterialAmbient",
    "Time",
    "FogDensity",
    "TextureTM0",
    "TextureTM1",
    "TextureTM2",
    "TextureTM3",
    "TFactor",
	"FarPlane",
	"NearPlane",
    "ColorConst",
	"ShaderConst0",
	"ShaderConst1",
	"ShaderConst2",
	"ShaderConst3"
}; // c_AutoShaderVNames

/*****************************************************************/
/*    Enum:    ShaderVarType
/*    Desc:    Data types of the shader variables
/*****************************************************************/
enum ShaderVarType
{
    svtUnknown  = 0,
    svtBool     = 1,       
    svtFloat    = 2,
    svtInt      = 3,
    svtMatrix   = 4,
    svtVector   = 5
}; // enum ShaderVarType

/*****************************************************************/
/*    Class:    IShader
/*    Desc:    Interface to the object shader, describes most of the 
/*                possible object rendering aspects
/*****************************************************************/
class IShader
{
public:
    virtual const char*             GetName         () const = 0;
    virtual void                    SetName         ( const char* name ) = 0;
    
    virtual int                     GetID           () const = 0;
    virtual void                    SetID           ( int id ) = 0;

    virtual int                     GetNTech        () const = 0;
    virtual bool                    IsTechValid     ( int techID ) const = 0;
    virtual const char*             GetTechName     ( int techID ) const = 0;
    virtual bool                    SetActiveTech   ( int techID ) = 0;

    virtual int                     GetNPasses      ( int techID = 0 ) const = 0;
    virtual int                     GetNShaderVars  () const = 0;
    virtual const char*             GetVariableName ( int cID ) const = 0;
    virtual int                     GetShaderVarID  ( const char* cName ) = 0;

    virtual bool                    SetShaderVar    ( int cID, bool val ) = 0;
    virtual bool                    SetShaderVar    ( int cID, float val ) = 0;
    virtual bool                    SetShaderVar    ( int cID, int val ) = 0;
    virtual bool                    SetShaderVar    ( int cID, const Matrix4D& val ) = 0;
    virtual bool                    SetShaderVar    ( int cID, const Vector4D& val ) = 0;
	virtual bool                    SetShaderVar    ( int cID, const Vector4D* val, int count ) = 0;
    virtual bool                    SetShaderVar    ( int cID, const Vector3D& val ) = 0;

    virtual bool                    GetShaderVar    ( int cID, bool& val ) = 0;
    virtual bool                    GetShaderVar    ( int cID, float& val ) = 0;
    virtual bool                    GetShaderVar    ( int cID, int& val ) = 0;
    virtual bool                    GetShaderVar    ( int cID, Matrix4D& val ) = 0;
    virtual bool                    GetShaderVar    ( int cID, Vector4D& val ) = 0;
    virtual bool                    GetShaderVar    ( int cID, Vector3D& val ) = 0;

    virtual void                    SetAutoVars     () = 0;
    virtual bool                    IsAutoVar       ( int cID ) = 0;
    
    virtual bool                    Load            ( const char* fName ) = 0;
   
    virtual bool                    Begin           () = 0;
    virtual bool                    BeginPass       ( int passID ) = 0;
    virtual bool                    EndPass         () = 0;
    virtual bool                    End             () = 0;
    
    virtual bool                    Reload          () = 0;
    
    virtual void                    DeleteDeviceObjects     () = 0;
    virtual void                    InvalidateDeviceObjects () = 0;
    virtual void                    RestoreDeviceObjects    () = 0;

}; // class IShader

#endif // __ISHADER_H__ 