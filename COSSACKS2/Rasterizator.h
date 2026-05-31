typedef void RasterFunc(int x,int y,float value,void* param);
void PerformRasterisation(Vector3D V1,Vector3D V2,Vector3D V3,RasterFunc* Fn,void* param);
class Rasterizator{
	void RasterizeInt(Vector3D V1,Vector3D V2,Vector3D V3,bool chxy);
public:
	virtual void OnPoint(int x,int y,float z){};
	void Rasterize(Vector3D V1,Vector3D V2,Vector3D V3);
};