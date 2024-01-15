/*****************************************************************
/*  File:   IHardwareCaps.h                                      
/*  Desc:   Interface to the rendering device capabilities
/*    Author:    Ruslan Shestopalyuk
/*  Date:   Jun 2004                                             
/*****************************************************************/
#ifndef __IHARDWARECAPS_H__ 
#define __IHARDWARECAPS_H__ 
#pragma    once

/*****************************************************************/
/*    Enum:    HardwareCap    
/*    Desc:    Identifies hardware capability bit
/*****************************************************************/
enum HardwareCap
{
    hcUnknown               = 0,
    hcHasTnL                = 1,    //  we got hardware TnL pipeline
    hcHasStencil            = 2,    //  stencil buffer is present and enabled
    hcMaxLights             = 3,    //  maximal number of hardware lights
    hcMaxVSConstants        = 4,    //  maximal vertex shader constants
    hcMaxPSConstants        = 5,    //  maximal pixel shader constants
    hcHasBumpEnvMapping     = 6,    //  support fixed pipeline hardware bump env mapping
    hcMaxTextureLayers      = 7,    //  maximal simultaneous textures in one pass
    hcMaxBlendLayers        = 8,    //  maximal blending layers in one pass
    hcMaxTexWidth           = 9,    //  maximal allowed width of texture
    hcMaxTexHeight          = 10,   //  maximal allowed height of texture
    hcHasDynamicTextures    = 11,   //  driver supports dynamic textures
    hcHasTexGen             = 12,   //  hardware can generate texture coordinates

    hcHasPS11               = 13,   //  hardware supports pixel shaders v1.1
    hcHasPS12               = 14,   //  hardware supports pixel shaders v1.2
    hcHasPS13               = 15,   //  hardware supports pixel shaders v1.3
    hcHasPS14               = 16,   //  hardware supports pixel shaders v1.4
    hcHasPS20               = 15,   //  hardware supports pixel shaders v2.0

    hcHasVS11               = 20,   //  hardware supports vertex shaders v1.1    
    hcHasVS20               = 21,   //  hardware supports vertex shaders v2.0

}; // enum HardwareCap

/*****************************************************************/
/*    Class:    IHardwareCaps
/*    Desc:    Interface for retreiving device capabilities
/*****************************************************************/
class IHardwareCaps
{
public:
    virtual bool           HasTnL                () const = 0;
    virtual bool           HasStencil            () const = 0;
    virtual int            MaxLights             () const = 0;
    virtual int            MaxVSConstants        () const = 0;
    virtual int            MaxPSConstants        () const = 0;

    virtual bool           HasBumpEnvMapping     () const = 0;
    
    //  texture caps
    virtual int            MaxTextureLayers      () const = 0;
    virtual int            MaxBlendLayers        () const = 0;
    virtual int            MaxTexWidth           () const = 0;
    virtual int            MaxTexHeight          () const = 0;
    virtual bool           HasDynamicTextures    () const = 0;
    virtual bool           HasTexGen             () const = 0;

    virtual bool           HasPS11               () const = 0; 
    virtual bool           HasPS12               () const = 0; 
    virtual bool           HasPS13               () const = 0; 
    virtual bool           HasPS14               () const = 0; 
    virtual bool           HasPS20               () const = 0; 
    virtual bool           HasVS11               () const = 0; 
    virtual bool           HasVS20               () const = 0; 

    virtual DWORD          GetCap                ( HardwareCap cap ) const = 0;


    //  initializes hardware caps values from current device
    virtual void           Init                  () = 0;
}; // class IHardwareCaps

//  global interface pointer
extern IHardwareCaps*    ICaps;

#endif // __IHARDWARECAPS_H__ 