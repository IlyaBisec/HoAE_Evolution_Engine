/*****************************************************************************/
/*    File:    mShapes.h
/*    Desc:    Predefined mesh shapes
/*    Author:    Ruslan Shestopalyuk
/*    Date:    05-4-2003
/*****************************************************************************/
#ifndef __MSHAPES_H__
#define __MSHAPES_H__

/*****************************************************************************/
/*    Enum:    ShapeAction
/*****************************************************************************/
enum ShapeAction
{
    scCreate        = 0,
    scGetNVert        = 1,
    scGetNPoly        = 2
}; // enum ShapeAction

typedef void (*VertCallback)( float x, float y, float z, float u, float v );
typedef void (*PolyCallback)( int a, int b, int c );

//  creates cone shape
int CreateCone(    int                hSeg,        //  height segments        
                int                cSeg,        //    circle segments
                float            height,        //  cone height
                float            topR,        //  top radius
                float            botR,        //  bottom radius
                VertCallback    putVert,    //  callback that returns vertices
                PolyCallback    putPoly,    //    callback that returns indexed polys
                ShapeAction        action = scCreate
                );

//  creates sphere shape
int CreateSphere(    int                hSeg,        //  height segments        
                    int                cSeg,        //    circle segments
                    float            radius,        //  cone height
                    VertCallback    putVert,    //  callback that returns vertices
                    PolyCallback    putPoly,    //    callback that returns indexed polys
                    ShapeAction        action = scCreate,
                    float           polePhi = 0.0f,
                    float           poleTheta = 0.0f
               );

//  creates sphere shape
int CreateHemisphere(    int                hSeg,        //  height segments        
                         int                cSeg,        //    circle segments
                         float            radius,        //  cone height
                         VertCallback    putVert,    //  callback that returns vertices
                         PolyCallback    putPoly,    //    callback that returns indexed polys
                         ShapeAction        action = scCreate,
                        float           polePhi = 0.0f,
                        float           poleTheta = 0.0f
                 );

//  creates part of the sphere shape
int CreateSpherePatch(    int                hSeg,        //  height segments        
                        int                cSeg,        //    circle segments
                        float            radius,        //  cone height
                        float            phi0,        //  start lattitude
                        float            phi1,        //  end lattitude
                        float            theta0,        //  start longtitude
                        float            theta1,        //  end longtitude
                        VertCallback    putVert,    //  callback that returns vertices
                        PolyCallback    putPoly,    //    callback that returns indexed polys
                        ShapeAction        action = scCreate,
                        float           polePhi = 0.0f,
                        float           poleTheta = 0.0f
                    );

//  creates icosahedron shape
int CreateIcosahedron(    VertCallback    putVert,        //  callback that returns vertices
                        PolyCallback    putPoly,        //  callback that returns indexed polys
                        float            radius = 1.0f,    //  radius
                        int                order = 2,        //  subdivision order
                        bool            bHalf = false,    //    whether create only upper half
                        ShapeAction        action = scCreate
                    );    

#endif  // __MSHAPES_H__
