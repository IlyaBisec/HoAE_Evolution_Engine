#pragma once

class TexturePattern{
	int			 SizeX;
	int			 SizeY;
	DWORD*		 Data;	    
	static float ProjCX;
	static float ProjCY;
	static float LastFreePosX;//position when RMB is unpressed
	static float LastFreePosY;
	static float LastPosX;
	static float LastPosY;
	
public:	
	int			 TextureID;    
    Vector4D     Average;
	static Matrix4D ScreenToUVSpace;
	static Matrix4D WorldToUVSpace;	

	static void		ToUV(Vector3D& pos);

				TexturePattern();
				~TexturePattern();
	bool		LoadTexture		(const char* Name);
	void		clear			();
	Vector4D	getPixelUV		(float u,float v);	
	Vector4D	getPixel		(Vector3D pos);	    
	int			getWidth		();
	int			getHeight		();	
	void		setTexture		(int ID);
	int			getTexture		();	
	void		RenderOnPen		(Vector3D pos,float Radius);
	//orientation control
	void		tex_reset		();
	void		tex_rotate		(float ScrX,float ScrY,float Angle);
	void		tex_translate	(float dx,float dy);
	void		tex_scale		(float ScrX,float ScrY,float scalex,float scaley);
	static void	CreateWorldToUVSpace();
	void		ProcessMouse	();
    void        CreateStroke    (TexturePattern* src);
};
DWORD V4D2DW(Vector4D& V);
Vector4D DW2V4D(DWORD C);
