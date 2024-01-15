#include "comms\comms.h"
#include "bitset.h"

struct SurfPix{
    DWORD Face;
    WORD  x;
    WORD  y;
};
struct ExpResult:public SurfPix{
    void sp(SurfPix _sp){
        Face=_sp.Face;
        x=_sp.x;
        y=_sp.y;
    }
    float W;
    float			NeibLen[4];
	SurfPoint3D*	Center;
	SurfPoint3D*	Neib[4];
	Vector3D		Force;
};
class ExpandSurface{
public:
    static DynArray<SurfPix> Stack1;
    static DynArray<SurfPix> Stack2;
    static DynArray<ExpResult> Result;	
    static UnlimitedBitset   Used;
    static UnlimitedBitset   UsedFaces;
	static UnlimitedBitset	 UsedInsFaces;
    static float Diff;
    static Vector4D SrcColor;

    virtual float GetDiff(SurfPoint3D* Src,SurfPoint3D* Curr);
    bool Start(PolyMesh* M,DWORD Face,int x,int y);
};