/*****************************************************************************/
/*    File:    sgNodeMaker.cpp
/*    Desc:    Stub file for registering scene node types
/*    Author:    Ruslan Shestopalyuk
/*    Date:    06-26-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgAttachedEffect.h"

void LinkSG()
{
LINK_CLASS( TransformNode       );
LINK_CLASS( HudNode             );
LINK_CLASS( AttachedEffect      );

LINK_CLASS( LightSource         );
LINK_CLASS( PointLight          );
LINK_CLASS( DirectionalLight    );
LINK_CLASS( SpotLight           );

LINK_CLASS( GeometryRef         );
LINK_CLASS( Geometry            );
LINK_CLASS( Locator             );
LINK_CLASS( Group               );
LINK_CLASS( SurfaceMaterial     );
LINK_CLASS( Texture             );
LINK_CLASS( Shader              );
LINK_CLASS( SetTextureFactor	);
LINK_CLASS( SetColorConst		);

LINK_CLASS( Root                );
LINK_CLASS( Background          );
LINK_CLASS( Bone                );
LINK_CLASS( Skin                );
LINK_CLASS( SkinGPU				);
LINK_CLASS( MorphedGeometry     );
LINK_CLASS( ZBias               );
LINK_CLASS( PRSAnimation        );
LINK_CLASS( UVAnimation         );
LINK_CLASS( Controller          );
LINK_CLASS( Animation           );
LINK_CLASS( VectorField         );
LINK_CLASS( Control             );
LINK_CLASS( Dialog              );
LINK_CLASS( PushButton          );
LINK_CLASS( CheckBoxButton      );
LINK_CLASS( Chart               );
LINK_CLASS( KeyframeChart       );
LINK_CLASS( FloatTrackEdit      );
LINK_CLASS( QuatTrackEdit       );
LINK_CLASS( ColorTrackEdit      );
LINK_CLASS( ColorRampEdit       );
LINK_CLASS( AlphaRampEdit       );
LINK_CLASS( WeightEdit          );
LINK_CLASS( AnimationBlock      );

LINK_CLASS( ScreenSprite        );
LINK_CLASS( WorldSprite         );

LINK_CLASS( Fog                 );
LINK_CLASS( LensFlare           );
LINK_CLASS( LensFlareElement    );
LINK_CLASS( Transform2D         );
LINK_CLASS( BumpMatrix          );
LINK_CLASS( TextureMatrix       );
LINK_CLASS( Switch              );
LINK_CLASS( RenderTarget        );
LINK_CLASS( Model               );

//  cursors
LINK_CLASS( Cursor              );
LINK_CLASS( SystemCursor        );
LINK_CLASS( TexturedCursor      );
LINK_CLASS( HardwareCursor      );
LINK_CLASS( FramerateCursor     );

//  kangaroo stuff
LINK_CLASS( PhysicsEditor       );
LINK_CLASS( StatManager         );

//  manipulators
LINK_CLASS( TransformTool       );
LINK_CLASS( MoveTool            );
LINK_CLASS( RotateTool          );
LINK_CLASS( ScaleTool           );
LINK_CLASS( SelectionTool       );

//  services
LINK_CLASS( TextureManager      );

LINK_CLASS( SurfaceCache        );
LINK_CLASS( SurfaceCacheItem    );
LINK_CLASS( ReflectionMap       );

LINK_CLASS( Overlay             );
LINK_CLASS( FontManager         );
//LINK_CLASS( CameraPathEditor    );
LINK_CLASS( FieldPatch          );

LINK_CLASS( OrthoCamera         );
LINK_CLASS( PerspCamera         );
}


