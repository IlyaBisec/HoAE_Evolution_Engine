//Summary of interface substituting functions:

typedef int tpUnitInterfaceSubst(int x0,int y0,int Lx0,int Ly0,int x1,int y1,int Index);
//return value:
//0-interface not changed
//1-miss default left panel
//2-miss default right panel
//3-miss default left&right panel
//7-dont draw anything
CIMPORT tpUnitInterfaceSubst* SetUnitInterfaceCallback(tpUnitInterfaceSubst* cb);

typedef void HandlePro(int);

CIMPORT void* AddIconToPanel(int x,int y,int FileID,int SpriteID);
CIMPORT void AddHintToIcon(void* OI,char* Hint);
CIMPORT void AddProcToIcon(void* OI,HandlePro* L,int Lparam,HandlePro* R,int Rparam);
CIMPORT void AddStageToIcon(void* OI,int Value,int MaxValue);
CIMPORT void AddNumberToIcon(void* OI,int Value);
CIMPORT void AddLevelToIcon(void* OI,int Value);

//Summary of the graphical functions:

void SetDrawScale(float scale);
void DrawPicture(int x,int y,int File,int Sprite,byte Nat);
int GetPictureIndex(char* name);
void FlushPictures();
void SetCurrentDiffuse(DWORD Color);
void DrawRotatedPicture(int x,int y,int File,int Sprite,byte Nat,
						int dx,int dy,float Angle,float sx,float sy);
void TransformToScreen(int x,int y,int z,float& scrx,float& scry,float& Scale);

//

CIMPORT int GetIconByNameExt(char* Name);
