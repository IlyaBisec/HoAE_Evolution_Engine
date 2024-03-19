struct OneBrigadePosition{
	int  BrigadeID;
	byte NI;
	int  xc,yc;
	byte Dir;
	
	int Dest2DestP;
	int TopDest2DestP;
	int xcb, ycb;
	byte Dirb;
	byte GroupFlag;
};
class BrigadesList 
{
public:
	DynArray<OneBrigadePosition> BrPosArr;
	byte AverageDir;
	word AttDir; 
	int xc,yc;
    void CreateFromSelected(byte NI,int dx, int dy);
	void CreateFromSelected(byte NI,int dxx, int dyy,int Nsel,word* SMon,word* MSN);
	void Transform(int x,int y,byte DestDir);
	void ShowPositions();
	void SendToPositions(byte Prio,int OrdType);
	int SumLength;
private:
	void MarkNearest(int OBPIndex);
	void CreateFromMarked();
	void CreateFromNonMarked(byte NDir);
	int GetNearestPointToBr(int OBPFrom, int OBPTo, int& nx, int& ny, int& NDir);
	void ResortPos();
};