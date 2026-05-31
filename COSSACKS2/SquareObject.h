extern IRenderSystem*	IRS;
typedef int tpGetNetValue(int x,int y,void* param,float& u,float& v);
typedef DWORD tpGetNetValueRGBA(int x,int y,void* param);
typedef int tpGetTexCoor(float& tx,float& ty,int px,int py,void* tparam);
enum QuantType{
	qt_128=1,
	qt_64,
	qt_32,
	qt_16,
	qt_8
};
class SquareObject{
public:
	int DimX;
	int DimY;
	BaseMesh* BM;
	void Clear();
	SquareObject(){
		BM=NULL;
		DimX=0;
		DimY=0;
	};
	~SquareObject();
	bool Create(int gx,int gy,tpGetNetValue* FN,int x0,int y0,QuantType QTX,QuantType QTY,void* param,float tex_X0,float tex_Y0,float tex_LX,float tex_LY);
	bool CreateRGBA(int gx,int gy,tpGetNetValueRGBA* FN,int x0,int y0,QuantType QTX,QuantType QTY,void* param,float tex_X0,float tex_Y0,float tex_LX,float tex_LY);
	bool CreateTnL2(int gx,int gy,tpGetNetValue* FN,int x0,int y0,QuantType QTX,QuantType QTY,void* param,float tex_X0,float tex_Y0,float tex_LX,float tex_LY);
	bool Create2T(int gx,int gy,tpGetNetValue* FN,int x0,int y0,int sx,int sy,float Ratio,QuantType QTX,QuantType QTY,void* param,bool Opt=1);
};