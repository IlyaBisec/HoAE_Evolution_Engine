#pragma once
template<class Type>
class SquareMap{
	int sx,sy;
	Type* Values;
public:
	SquareMap(){
		sx=0;
		sy=0;
		Values=NULL;
	}
	~SquareMap(){
		if(Values)delete(Values);
	}
	void CopyTo(SquareMap<Type>& dest){
		if(dest.sx!=sx||dest.sy!=sy){
			dest.SetSize(sx,sy);
		}
		memcpy(dest.Values,Values,sx*sy*sizeof Type);
	}
	void SetSize(int Lx,int Ly,Type Default=0){
		if(Values)delete[](Values);
		Values=new Type[Lx*Ly];
		int N=Lx*Ly;
		for(int j=0;j<N;j++)Values[j]=Default;		
		sx=Lx;
		sy=Ly;
	}
	__forceinline void Set(int x,int y,Type Value){
		if(x>=0&&y>=0&&x<sx&&y<sy)Values[x+y*sx]=Value;
	}
	__forceinline Type Get(int x,int y,Type Default=0){
		if(x<0)x=0;
		if(y<0)y=0;
		if(x>=sx)x=sx-1;
		if(y>=sy)y=sy-1;
		return Values[x+y*sx];
	}
	void Fill(Type Value){
		int N=sx*sy;
		for(int j=0;j<N;j++)Values[j]=Value;		
	}
	void CreateVoronoiDiagramm(int* xi,int* yi,int NPoints){
		Fill(0xFFFF);
		bool change=false;
		int r=0;
		byte phase[32];
		for(int i=0;i<32;i++)phase[i]=mrand()&255;
		byte dirs[256];
		for(int i=0;i<256;i++)dirs[i]=(mrand()&127)+128;
		for(int i=1;i<256*4;i++){
			int v=(int(dirs[i&255])*2+int(dirs[(i+1)&255])+int(dirs[(i-1)&255]))/4;
			dirs[i&255]=v;
		}
		int maxd=0;
		for(int i=0;i<256;i++)if(dirs[i]>maxd)maxd=dirs[i];
		do{
			change=false;
            int N=Rarr[r].N;
			char* xp=Rarr[r].xi;
			char* yp=Rarr[r].yi;			
			for(int i=0;i<N;i++){
				for(int p=0;p<NPoints;p++){
					int dx=int(xp[i])*3;
					int dy=int(yp[i])*3;
					word GetDir(int dx,int dy);
					byte d=GetDir(dx,dy);
					int sc=dirs[byte(d/*+phase[p]*/)];
					dx=(dx*sc)/maxd;
					dy=(dy*sc)/maxd;
					int x=((xi[p]/3)*3)+dx;
					int y=((yi[p]/3)*3)+dy;
					x=(x/3)*3;
					y=(y/3)*3;
					Type V=Get(x,y,0xFFFE);
                    bool doit=V==0xFFFF||V==p;
					if(r>3){
						if(Get(x-3,y)!=p&&Get(x+3,y)!=p&&Get(x,y-3)!=p&&Get(x,y+3)!=p
							&&Get(x-3,y-3)!=p&&Get(x+3,y-3)!=p&&Get(x-3,y+3)!=p&&Get(x+3,y+3)!=p)doit=false;						
					}
					if(doit){
						for(int dx=-1;dx<=1;dx++)
							for(int dy=-1;dy<=1;dy++){
								Set(x+dx,y+dy,p);								
							}
						change=true;
					}
				}
			}
			r++;
		}while(change&&r<90);
	}
	int FillWithTopValues(Type FreeVal,Type LockVal){//returns amount of independent zones
		bool change;
		Type topV=0;
		do{
			change=false;
			for(int iy=0;iy<sy;iy++){
				for(int ix=0;ix<sx;ix++){
					Type V=Get(ix,iy);
					Type VP=V;
					if(V!=LockVal){
						Type VL=Get(ix-1,iy,LockVal);
						if(VL!=LockVal&&VL!=FreeVal&&VL<V){
							V=VL;
						}else{
							VL=Get(ix+1,iy,LockVal);
							if(VL!=LockVal&&VL!=FreeVal&&VL<V){
								V=VL;
							}else{
								VL=Get(ix,iy-1,LockVal);
								if(VL!=LockVal&&VL!=FreeVal&&VL<V){
									V=VL;
								}else{
									VL=Get(ix,iy+1,LockVal);
									if(VL!=LockVal&&VL!=FreeVal&&VL<V){
										V=VL;
									}
								}
							}
						}
						if(VP!=V){
							Set(ix,iy,V);
							change=true;
						}else{
                            if(VP==FreeVal)Set(ix,iy,++topV);
						}
					}
				}
			}
		}while(change);
		topV=0;
		for(int iy=0;iy<sy;iy++){
			for(int ix=0;ix<sx;ix++){
				Type V=Get(ix,iy);
				if(V!=FreeVal&&V!=LockVal){
					if(V>topV)topV=V;
				}
			}
		}
		return topV;
	}
	bool FindRandomPos(int& xc,int& yc,int rmin,int rmax,Type FreeVal,SquareMap<Type>& Dealer,int DealerValue){
        int natt=3500;
		if(rmin==0)natt+=2500;
		do{
			int r=rmin+((rmax-rmin)*mrand())/32768;
			byte ang=mrand()&255;
            int xx=xc+r*TCos[ang]/256;
			int yy=yc+r*TSin[ang]/256;
			if(Get(xx,yy,FreeVal+1)==FreeVal&&Dealer.Get(xx,yy)==DealerValue){
                xc=xx;
				yc=yy;
				return true;
			}
			natt--;
            if(mrand()<1000||rmax<20)rmax++;
			if(rmin>1&&mrand()<500)rmin--;
		}while(natt>0);
        return false;
	}
};