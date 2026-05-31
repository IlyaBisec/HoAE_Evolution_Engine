
class Fractal3D{
	float* fHeight;
	int SizeX;
public:
	Fractal3D(){		
		SizeX=64;        
		fHeight=NULL;		
	}
	float GetF3D(int x,int y,int z){
		x+=SizeX;y+=SizeX;z+=SizeX;
		x%=SizeX;    
		y%=SizeX;
		z%=SizeX;
		return fHeight[x+y*SizeX+z*SizeX*SizeX];
	}
	void SetF3D(int x,int y,int z,float val){
		x+=SizeX;y+=SizeX;z+=SizeX;
		x%=SizeX;    
		y%=SizeX;
		z%=SizeX;
		fHeight[x+y*SizeX+z*SizeX*SizeX]=val;
	}
	void SetSize(int L);
	void CreateEmptyFractal();
	void Create3DFractal();
	void CreateMassLines(float Density,float Length,float R);
	void SaveFractalUVA(char* bmp24_name);
	void SaveHMap(char* bmp24_name);
	void Add3DLine(Vector3D p1,Vector3D p2,float thickness,float Val);
	float Check3DLine(Vector3D p1,Vector3D p2,float thickness);
	float GetAverageDensity();
};