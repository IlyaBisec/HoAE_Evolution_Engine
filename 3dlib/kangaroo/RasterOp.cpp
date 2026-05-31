#include "stdafx.h"
#include "Rasterop.h"
void RasterOp::Rasterize(Vector3D V1,Vector3D V2,Vector3D V3){
	int y1=floorf(V1.y);
	int y2=floorf(V2.y);
	int y3=floorf(V3.y);	
	int ymin=min(y1,min(y3,y2));
	int ymax=max(y1,max(y3,y2));
	Vector3D Vt[]={V1,V2,V3,V1};
    Vector3D UV[]={Vector3D(0,0,V1.z),Vector3D(1,0,V2.z),Vector3D(0,1,V3.z),Vector3D(0,0,V1.z)};
	//rasterisation of internal area
	for(int i=ymin;i<=ymax;i++){
		float cy=float(i)+0.5001f;
		float cxmin=1000000;
		float cxmax=-1000000;
        Vector3D uvz_min;
        Vector3D uvz_max;
		for(int j=0;j<3;j++){
			if((cy>=Vt[j].y&&cy<Vt[j+1].y)||(cy<=Vt[j].y&&cy>Vt[j+1].y)){
                float c=(cy-Vt[j].y)/(Vt[j+1].y-Vt[j].y);
				float cx=Vt[j].x+(Vt[j+1].x-Vt[j].x)*c;				
                Vector3D uvz=UV[j]+(UV[j+1]-UV[j])*c;
				if(cx<cxmin){
					cxmin=cx;
                    uvz_min=uvz;
				}
				if(cx>cxmax){
					cxmax=cx;
					uvz_max=uvz;
				}
			}
		}
		if(cxmin<cxmax){
			//found
			int imin=floorf(cxmin+0.5f);
			int imax=floorf(cxmax+0.5f);
			for(int k=imin;k<imax;k++){
				float z;
				float x=float(k)+0.5;
                Vector3D uvz;
                float c=(x-cxmin)/(cxmax-cxmin);
				uvz=uvz_min+(uvz_max-uvz_min)*c;				
				OnPoint(k,i,uvz.z,uvz.x,uvz.y);
			}
		}
	}	
}