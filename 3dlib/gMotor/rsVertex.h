/*****************************************************************
/*  File:   rsVertex.h                                             
/*    Desc:    Set of vertex formats
/*  Date:   November 2001                                        
/*****************************************************************/
#ifndef __RSVERTEX_H__
#define __RSVERTEX_H__

#include "gmDefines.h"
#include "IVertexBuffer.h"

class AABoundBox;
/*****************************************************************
/*    Class:    Vertex
/*  Desc:    Generic vertex class - serves as different vertex
/*                types factory.                                              
/*****************************************************************/
class Vertex
{
public:
    static void*        CreateVBuf          ( VertexFormat vf, int numVert );
    static int          GetStride           (  VertexFormat vf );
}; // class Vertex

class Vector3D;
/*****************************************************************************/
/*    Class:    Vertex32
/*    Desc:    This class is a hack for working with 32-bytes vertices
/*                in vertex coordinates mode only
/*****************************************************************************/
class Vertex32
{
public:
    float    x, y, z;

private:
    DWORD    dummy[5];
}; // class Vertex32

/*****************************************************************
/*  Class:  Vertex2t                                             
/*****************************************************************/
class Vertex2t : public Vertex
{
public:
    float                    x;
    float                    y;
    float                    z;

    DWORD                    diffuse;    //  diffuse vertex color
    float                    u, v;        //  1st texture coordinates
    float                    u2, v2;        //  2nd texture coordinates

    Vertex2t() : x(0.0f), y(0.0f), z(0.0f), diffuse(0xFF584E40),
                    u(0.0f), v(0.0f), u2(0.0f), v2(0.0f) {}
    const Vertex2t& operator =( const Vector3D& vec )
    { x = vec.x; y = vec.y; z = vec.z; return *this; }
    operator Vector3D() const{return Vector3D( x, y, z );}

    static VertexFormat        format()    { return vfVertex2t;    }
};  // class Vertex2t

/*****************************************************************
/*  Class:  VertexTS                                             
/*****************************************************************/
class VertexTS : public Vertex
{
public:
    VertexTS() {}

    float                    x;
    float                    y;
    float                    z;

    float                    w;         
    DWORD                    diffuse;     //  diffuse vertex color
    DWORD                    specular;
    float                    u, v;        //  1st texture coordinates

    const VertexTS&    operator =(const Vector3D& vec);
    operator Vector3D() const { return Vector3D( x, y, z );}

    static VertexFormat        format()    { return vfVertexTS;    }
};  // class VertexTS

/*****************************************************************
/*  Class:  VertexUV3C                                             
/*****************************************************************/
class VertexUV3C : public Vertex
{
public:
	VertexUV3C() {}

	float                    x;
	float                    y;
	float                    z;

	DWORD                    diffuse;     //  diffuse vertex color
	DWORD                    specular;
	DWORD					 color2;
	float                    u, v;        //  1st texture coordinates

	const VertexUV3C&    operator =(const Vector3D& vec);
	operator Vector3D() const { return Vector3D( x, y, z );}

	static VertexFormat        format()    { return vfVertexUV3C;    }
};  // class VertexUV3C

/*****************************************************************
/*  Class:  VertexUV3C                                             
/*****************************************************************/
class Vertex5C : public Vertex
{
public:
	Vertex5C() {}

	float                    x;
	float                    y;
	float                    z;

	DWORD                    color0;     //  diffuse vertex color
	DWORD                    color1;
	DWORD					 color2;
	DWORD					 color3;
	DWORD					 color4;
	

	const Vertex5C&    operator =(const Vector3D& vec);
	operator Vector3D() const { return Vector3D( x, y, z );}

	static VertexFormat        format()    { return vfVertex5C;    }
};  // class Vertex5C


/*****************************************************************
/*  Class:  Vertex2F                                             
/*****************************************************************/
class Vertex2F : public Vertex
{
public:
    Vertex2F() {}
    DWORD               diffuse;
    DWORD               specular;

    static VertexFormat        format()    { return vfVertex2F;    }
};  // class Vertex2F

/*****************************************************************
/*  Class:  VertexMP1                                             
/*  Desc:    Vertex used in matrix palette blending, vertex is
/*                bound to single bone
/*****************************************************************/
class VertexMP1 : public Vertex
{
public:
                            VertexMP1();

    float                    x;
    float                    y;
    float                    z;

    DWORD                    matrIdx;    //  index of the corresponding transform matrix in palette
    DWORD                    diffuse;    //  diffuse vertex color
    float                    u, v;        //  1st texture coordinates
    float                    u2, v2;        //  2nd texture coordinates
    
    const Vertex2t& operator =( const Vector3D& vec );

    static VertexFormat        format()    { return vfVertexMP1;        }
};  // class VertexMP1

/*****************************************************************
/*  Class:  VertexW1                                             
/*  Desc:    Vertex used in matrix palette blending, vertex is
/*                bound to single bone, also normal is included
/*****************************************************************/
class VertexW1 : public Vertex
{
public:
    VertexW1(){}

    float                    x;
    float                    y;
    float                    z;

    DWORD                    matrIdx;    //  index of the corresponding transform matrix
                                        //  in palette
    float                    nx;
    float                    ny;
    float                    nz;

    DWORD                    diffuse;    //  diffuse vertex color
    float                    u, v;        //  1st texture coordinates
    float                    u2, v2;        //  2nd texture coordinates
    

    const Vertex2t& operator =(const Vector3D& vec);

    static VertexFormat        format()    { return vfVertexW1;    }
};  // class VertexW1

/*****************************************************************
/*  Class:  VertexW2                                             
/*  Desc:    Vertex used in matrix palette blending, vertex is
/*                bound to 2 bones, also normal is included
/*****************************************************************/
class VertexW2 : public Vertex
{
public:
    VertexW2(){}

    float                    x;
    float                    y;
    float                    z;

    float                    weight[1];
    DWORD                    matrIdx;    //  index of the corresponding transform matrix
                                        //  in palette
    float                    nx;
    float                    ny;
    float                    nz;

    DWORD                    diffuse;    //  diffuse vertex color
    float                    u, v;        //  1st texture coordinates
    float                    u2, v2;        //  2nd texture coordinates

    static VertexFormat        format()    { return vfVertexW2;    }
};  // class VertexW2

/*****************************************************************
/*  Class:  VertexW3                                             
/*  Desc:    Vertex used in matrix palette blending, vertex is
/*                bound to 3 bones, also normal is included
/*****************************************************************/
class VertexW3 : public Vertex
{
public:
    VertexW3(){}

    float                    x;
    float                    y;
    float                    z;

    float                    weight[2];
    DWORD                    matrIdx;    //  index of the corresponding transform matrix
                                        //  in palette
    float                    nx;
    float                    ny;
    float                    nz;

    DWORD                    diffuse;    //  diffuse vertex color
    float                    u, v;        //  1st texture coordinates
    float                    u2, v2;        //  2nd texture coordinates
    

    const Vertex2t& operator =(const Vector3D& vec);
    
    static VertexFormat        format()    { return vfVertexW3;    }
};  // class VertexW3

/*****************************************************************
/*  Class:  VertexW4                                         
/*  Desc:    Vertex used in matrix palette blending, vertex is
/*                bound to 4 bones, also normal is included
/*****************************************************************/
class VertexW4 : public Vertex
{
public:
    VertexW4(){}

    float                    x;
    float                    y;
    float                    z;

    float                    weight[3];
    DWORD                    matrIdx;    //  index of the corresponding transform matrix
                                        //  in palette
    float                    nx;
    float                    ny;
    float                    nz;

    DWORD                    diffuse;    //  diffuse vertex color
    DWORD                    specular;    //  specular vertex color
    float                    u, v;        //  1st texture coordinates
    float                    u2, v2;        //  2nd texture coordinates
    

    const VertexW4& operator =(const Vector3D& vec);
    
    static VertexFormat        format()    { return vfVertexW4;    }
};  // class VertexW4

/*****************************************************************
/*  Class:  VertexN                                             
/*  Desc:    Vertex with normal and pair of tex coords
/*****************************************************************/
class VertexN : public Vertex
{
public:
    float                    x;
    float                    y;
    float                    z;

    float                    nx;
    float                    ny;
    float                    nz;

    float                    u, v;        //  1st texture coordinates

    VertexN() : x( 0.0f ), y( 0.0f ), z( 0.0f ), 
                nx( 0.0f ), ny( 0.0f ), nz( 0.0f ),
                u( 0.0f ), v( 0.0f ) {}

    VertexN& operator =( const Vector3D& vec ) {
        x = vec.x; y = vec.y; z = vec.z; return *this;
    } 

    static VertexFormat        format()    { return vfVertexN;    }
};  // class VertexN

/*****************************************************************
/*  Class:  VertexN2T                                             
/*****************************************************************/
class VertexN2T : public Vertex
{
public:
    VertexN2T(){}

    float                    x, y, z;    //  position
    float                    nx, ny, nz;    //  normal
    
    DWORD                    diffuse;
    DWORD                    specular;
    
    float                    u, v;        //  1st texture coordinates
    float                    u2, v2;        //  2nd texture coordinates
    
    VertexN2T&                operator =( const Vector3D& vec ); 

    static VertexFormat        format()    { return vfVertexN2T;    }
};  // class VertexN2T

/*****************************************************************
/*  Class:  VertexT                                             
/*  Desc:    XYZUV
/*****************************************************************/
class VertexT : public Vertex
{
public:
                        VertexT();

    float                x;
    float                y;
    float                z;

    float                u;
    float                v;
    
    static VertexFormat    format()            { return vfVertexT;    }
};  // class VertexT

/*****************************************************************
/*  Class:  VertexTnL                                              
/*  Desc:   Already transformed and lit vertex                     
/*****************************************************************/
class VertexTnL : public Vertex
{
public:
    float                x, y;        //  x and y coords in screen space
    float                z;            //  depth coordinate
    float                w;            //  reciprocal homogeneous w

    DWORD                diffuse;    //  diffuse color
    DWORD                specular;    
    float                u, v;        //  texture coordinates

    VertexTnL() :   x(0.0f), y(0.0f), z(0.0f), w(1.0f), 
                    diffuse(0xFFFFFFFF), specular(0xFFFFFFFF),
                    u(0.0f), v(0.0f) {}

    VertexTnL& operator =( const Vector3D& vec ){
        x = vec.x; y = vec.y; z = vec.z; return *this;
    }

    static VertexFormat    format()            { return vfVertexTnL;    }
}; // class VertexTnL

/*****************************************************************
/*  Class:  VertexTnL2                                              
/*  Desc:   Already transformed and lit vertex                     
/*****************************************************************/
class VertexTnL2 : public Vertex
{
public:
    float                x, y;        //  x and y coords in screen space
    float                z;            //  depth coordinate
    float                w;            //  reciprocal homogeneous w

    DWORD                diffuse;    //  diffuse color
    float                u, v;        //  texture coordinates
    float                u2, v2;        //  texture coordinates

    static VertexFormat    format()    { return vfVertexTnL2;    }
}; // class VertexTnL
/*****************************************************************
/*  Class:  VertexTnL2S                                             *
/*  Desc:   Already transformed and lit vertex                    *
/*****************************************************************/
class VertexTnL2S : public Vertex
{
public:
    float                x, y;        //  x and y coords in screen space
    float                z;            //  depth coordinate
    float                w;            //  reciprocal homogeneous w

    DWORD                diffuse;    //  diffuse color
    DWORD                specular;    //  diffuse color
    float                u, v;        //  texture coordinates
    float                u2, v2;        //  texture coordinates
    
    static VertexFormat    format()    { return vfVertexTnL2S;    }
}; // class VertexTnL

/*****************************************************************
/*  Class:  Vertex1W                                            
/*  Desc:   Vertex bound to single bone                                         
/*****************************************************************/
class Vertex1W : public Vertex
{
public:
    Vector3D            pos;        //  position
    Vector3D            normal;     //  normal
    DWORD               m;          //  bone index
    float               u, v;       //  texture coordinates

    static VertexFormat        format()    { return vfVertex1W;    }
}; // struct Vertex1W

/*****************************************************************
/*  Class:  Vertex2W                                            
/*  Desc:   Vertex bound to 2 bones                                         
/*****************************************************************/
class Vertex2W : public Vertex
{
public:
    Vector3D        pos;        //  position
    Vector3D        normal;     //  normal
    DWORD           m0;         //  bone index 0
    DWORD           m1;         //  bone index 1
    float           w;          //  bone blending weight
    float           u, v;       //  texture coordinates

    static VertexFormat    format()            { return vfVertex2W;    }
}; // struct Vertex2W

/*****************************************************************
/*  Class:  Vertex3W                                            
/*  Desc:   Vertex bound to 3 bones                                         
/*****************************************************************/
class Vertex3W : public Vertex
{
public:
    Vector3D        pos;        //  position
    Vector3D        normal;     //  normal
    DWORD           m0;         //  bone index 0
    DWORD           m1;         //  bone index 1
    DWORD           m2;         //  bone index 2
    float           w0;         //  bone blending weight 0
    float           w1;         //  bone blending weight 1
    float           u, v;       //  texture coordinates

    static VertexFormat    format()            { return vfVertex3W;    }
}; // struct Vertex3W

/*****************************************************************
/*  Class:  Vertex4W                                            
/*  Desc:   Vertex bound to 4 bones                                         
/*****************************************************************/
class Vertex4W : public Vertex
{
public:
    Vector3D        pos;        //  position
    Vector3D        normal;     //  normal
    DWORD           m0;         //  bone index 0
    DWORD           m1;         //  bone index 1
    DWORD           m2;         //  bone index 2
    DWORD           m3;         //  bone index 3
    float           w0;         //  bone blending weight 0
    float           w1;         //  bone blending weight 1
    float           w2;         //  bone blending weight 2
    float           u, v;       //  texture coordinates

    static VertexFormat    format() { return vfVertex4W; }
}; // struct Vertex4W

/*****************************************************************
/*  Class:  VertexBump                                            
/*  Desc:   Vertex used with bump-mapping                                          
/*****************************************************************/
class VertexBump : public Vertex
{
public:
    float                    x, y, z;    // position
    float                    nx, ny, nz; // normal
    float                    u, v;       // texture coordinates
    float                    bx, by, bz; // binormal
    float                    tx, ty, tz; // tangent

    static VertexFormat    format() { return vfVertexBump;    }
}; // class VertexBump

VertexDeclaration CreateVertexDeclaration( VertexFormat vf );

#endif // __RSVERTEX_H__