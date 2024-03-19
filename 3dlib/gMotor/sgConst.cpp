#include "stdafx.h"
#include "sg.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "sgConst.h"

void SetTextureFactor::Render(){
	IRS->SetTextureFactor(Const);
}
void SetTextureFactor::Expose( PropertyMap& pm ){
	pm.start<Parent>( "SetTextureFactor", this );
    pm.f( "Color",      Const,    "color" );    
}
void SetTextureFactor::Serialize( OutStream& os ) const {
	Parent::Serialize( os );
	os << Const;
}
void SetTextureFactor::Unserialize( InStream& is ){
	Parent::Unserialize( is );
	is >> Const;
}
void SetColorConst::Render(){
	IRS->SetColorConst(GetColor());
}
void SetColorConst::Expose( PropertyMap& pm ){
	pm.start<Parent>( "SetColorConst", this );
    pm.f( "Color",      Const,    "color" );    
}
void SetColorConst::Serialize( OutStream& os ) const {
	Parent::Serialize( os );
	os << Const;
}
void SetColorConst::Unserialize( InStream& is ){
	Parent::Unserialize( is );
	is >> Const;
}

IMPLEMENT_CLASS( SetTextureFactor	);
IMPLEMENT_CLASS( SetColorConst		);