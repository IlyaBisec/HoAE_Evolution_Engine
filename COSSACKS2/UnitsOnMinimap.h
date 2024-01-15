struct UnitOnMinimap{
public:
	float x,y,R;//coord on minimap
	DWORD Color;
	int   SpriteIndex;	
	int   SpriteIndex2;	
};
extern int MiniLx;
extern int MiniLy;
const int maxmm3D=256;
class UnitsListOnMinimap{
	DynArray<UnitOnMinimap> Units;
	float GlobScaleX;
	float GlobScaleY;
	float GlobScaleR;
public:
	void Clear(){
		Units.Clear();
		GlobScaleX=MiniLx/240.0f/64.0f/16.0f;
		GlobScaleY=MiniLy/240.0f/64.0f/16.0f;
        GlobScaleR=1.0;
	}
	void AddUnit(OneObject* OB){
		if(OB->ActiveAbility){
			bool invis=false;
			byte det=0;
			bool se=false;
			OB->ActiveAbility->ActiveAbilities.modifyInvisibility(invis,invis,det,se); // UnitsListOnMinimap.AddUnit
			if(invis && !(det&NATIONS[MyNation].NMask) ){
				return;
			}
		}
		UnitOnMinimap UM;
		UM.x=GetXOnMiniMap(OB->RealX>>4,OB->RealY>>4);
		UM.y=GetYOnMiniMap(OB->RealX>>4,OB->RealY>>4);
		if(UM.x<minix || UM.x<miniy)return;
		HeroVariableStorage* DetectHero(OneObject* OB);
		UM.SpriteIndex=0;
		UM.SpriteIndex2=-1;
		if(DetectHero(OB)){
			UM.R=5;
			UM.SpriteIndex2=1;
		}else{
			UM.R=4;
		}
		UM.SpriteIndex=0;
		DWORD GetNatColor(int i);
		UM.Color=GetNatColor(OB->NNUM);
		Units.Add(UM);
    }
	void DrawPoints(){
		static int T=IRS->GetTextureID("Textures\\MinimapIcons.dds");
		static int S=IRS->GetShaderID("hud_smooth");
		rsSetTexture(T);
		rsSetShader(S);
		for(int i=0;i<Units.GetAmount();i++){
			UnitOnMinimap& um=Units[i];
            Vector3D VA(um.x,um.y,0);
			Vector3D VB=VA;
			Vector3D VC=VA;
			Vector3D VD=VA;
			Vector3D R=Vector3D(um.R,um.R,0);
			Vector3D RT=Vector3D(um.R,-um.R,0);
			VA-=R;
			VB+=RT;
			VC-=RT;
			VD+=R;
			float u0=(um.SpriteIndex%4)/4.0f;
			float v0=(um.SpriteIndex/4)/4.0f;
			Rct RC(u0,v0,0.25,0.25);			
			rsQuad2D(VA,VB,VC,VD,um.Color,um.Color,um.Color,um.Color,RC);
			if(um.SpriteIndex2!=-1){
				u0=(um.SpriteIndex2%4)/4.0f;
				v0=(um.SpriteIndex2/4)/4.0f;
				Rct R1(u0,v0,0.25,0.25);			
				rsQuad2D(VA,VB,VC,VD,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,R1);
			}
		}
		rsFlush();
		rsRestoreShader();
	}
	void FixDistanceBetweenPoints(){
		int n=Units.GetAmount();
		for(int p=0;p<4;p++){
			for(int i=0;i<n;i++){
				UnitOnMinimap& um=Units[i];
				for(int j=0;j<i;j++){
					UnitOnMinimap& um2=Units[j];
					float dx=um2.x-um.x;
					float dy=um2.y-um.y;
					float d=sqrt(dx*dx+dy*dy);
					if(d<(um.R+um2.R)*0.75){
                        dx/=d*2;
						dy/=d*2;
						um.x-=dx;
						um.y-=dy;
						um2.x+=dx;
						um2.y+=dy;
					}
				}
			}
		}
	}
};