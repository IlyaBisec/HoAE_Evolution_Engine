#pragma once

class RasterOp{	
public:
	virtual void OnPoint(int x,int y,float z,float u,float v){};
	void Rasterize(Vector3D V1,Vector3D V2,Vector3D V3);
};