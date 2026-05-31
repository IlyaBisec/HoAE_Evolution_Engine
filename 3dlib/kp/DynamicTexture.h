#pragma once

class DynamicTexture{
public:
    char*       Name;
    int         SrcTextureID;
    int         TextureID;
    int         RT_TextureID;
    int         SizeX;
    int         SizeY;
    int         QuadSize;
    int         DirtySizeX;
    int         DirtySizeY;
    DWORD*      Colors;
    BYTE*       DirtyQuads;

    
                DynamicTexture();
                ~DynamicTexture();
    void        Clear();
    bool        LoadTexture(const char* tex);
    void        SaveTexture();
    void        CreateTexture(DWORD DefColor);
    void        SetPixel(float u,float v,Vector4D Color);
    void        SetPixel(float u,float v,DWORD Color);
    void        SetQuad(float u0,float v0,DWORD Color0,
                        float u1,float v1,DWORD Color1,
                        float u2,float v2,DWORD Color2,
                        float u3,float v3,DWORD Color3);
    Vector4D    GetPixel(float u,float v);
    void        Update();
};
DWORD V4D2DW(Vector4D& V);
Vector4D DW2V4D(DWORD C);