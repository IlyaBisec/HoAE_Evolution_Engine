class DlgBatcher{
public:    
    void AddPicture(int x,int y,int gpid,int pic,int nation);
    void AddBorder(int x0,int y0,int x1,int y1,int BorderID);
    void AddSimpleText(int x,int y,const char* message,RLCFont* Font);
    void AddFormatedText(int x,int y,int Lx,const char* message,RLCFont* Font);
    void AddLine(float x0,float y0,float w,float h,DWORD Color);
    void AddLine(float x0,float y0,float w,float h,DWORD Color1,DWORD Color2);
    void AddQuad(float x0,float y0,float w,float h,DWORD Color);
    void AddQuad(float x0,float y0,float w,float h,DWORD Color1,DWORD Color2,DWORD Color3,DWORD Color4);
    void AddPercentGauge(float percent,const Rct& ext.DWORD clr);
    bool AddCustom(SimpleDialog* SD);

};