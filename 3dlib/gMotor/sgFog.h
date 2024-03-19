#ifndef __SGFOG_H__
#define __SGFOG_H__

#include "mHeightmap.h"
#include "IMediaManager.h"



/*****************************************************************************/
/*    Class:    Fog
/*    Desc:    Applies fog effect to all children
/*****************************************************************************/
class Fog: public SNode
{
public:
    enum FogType
    {
        ftUnknown   = 0,
        ftVertex    = 1,
        ftPixel     = 2
    };

    enum FogMode
    {
        fmUnknown   = 0,
        fmLinear    = 1,
        fmExp       = 2,
        fmExp2      = 3
    };

                    Fog             ();
                    ~Fog            ();

    virtual void    Render          ();
    virtual void    Serialize       ( OutStream& os   ) const;
    virtual void    Unserialize     ( InStream& is    );
    virtual void    Expose          ( PropertyMap& pm );


    _inl void       SetColor        ( DWORD clr )    { m_Color = clr;    }
    _inl DWORD      GetColor        () const         { return m_Color;   }

    _inl void       SetStart        ( float val )    { m_Start = val;    }
    _inl float      GetStart        () const         { return m_Start;   }
    
    _inl void       SetEnd          ( float val )    { m_End = val;      }
    _inl float      GetEnd          () const         { return m_End;     }

    _inl void       SetDensity      ( float val )    { m_Density = val;  }
    _inl float      GetDensity      () const         { return m_Density; }

    _inl void       SetType         ( FogType val )  { m_Type = val;     }
    _inl FogType    GetType         () const         { return m_Type;    }

    _inl void       SetMode         ( FogMode val )  { m_Mode = val;     }
    _inl FogMode    GetMode         () const         { return m_Mode;    }
    
    _inl bool       GetIsRangeBased () const         { return m_bRangeBased; }
    _inl void       SetIsRangeBased ( bool val )     { m_bRangeBased = val; }

    DECLARE_SCLASS(Fog,SNode,FOGN);

private:
    DWORD            m_Color;
    float            m_Start;
    float            m_End;

    float            m_Density;
    FogType            m_Type;
    FogMode            m_Mode;

    bool            m_bRangeBased;

    bool            m_bEnabled;

}; // Fog



ENUM( Fog::FogType, "Type", 
                            en_val( Fog::ftUnknown,    "Unknown"    ) <<
                            en_val( Fog::ftVertex,    "Vertex"    ) <<
                            en_val( Fog::ftPixel,    "Pixel"        ) );

ENUM( Fog::FogMode, "Mode", 
                            en_val( Fog::fmUnknown,    "Unknown"    ) <<
                            en_val( Fog::fmLinear,    "Linear"    ) <<
                            en_val( Fog::fmExp,        "Exp"        ) <<
                            en_val( Fog::fmExp,        "Exp2"        ) );

#endif // __SGFOG_H__