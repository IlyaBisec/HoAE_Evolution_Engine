class OneFeature:public BaseClass{
public:
	int add_time;
	virtual void Draw(){};
	virtual int GetHash(){return 0;};
};
class DrawArrowWhenAttEnm:public OneFeature{
public:
	Brigade* BSrc;
	//Brigade* BDest;
	virtual void Draw();
	virtual int GetHash(){return BSrc->ID*6361;};
};
class DrawArrowBetweenBrigades:public OneFeature{
public:
	Brigade* BSrc;
	Brigade* BDest;
	virtual void Draw();
	virtual int GetHash(){return BSrc->ID+BDest->ID*9791+773;};
};
class DrawAttackRangesOfBrigade:public OneFeature{
public:
	Brigade* BR;
	int Line;
	virtual void Draw();
	virtual int GetHash(){return BR->ID*98765+981+Line*776;};
};
class DrawSpecialAttackRange:public OneFeature{
public:
	Brigade* BR;
	int AttType;
	virtual void Draw();
	virtual int GetHash(){return BR->ID*33371+567*AttType+661;};
};
class DrawMeleeOpponentForBrigade:public OneFeature{
public:
	Brigade* BR;
	virtual void Draw();
	virtual int GetHash(){return BR->ID*13371+961;};
};
class DrawCircleSomewhere:public OneFeature{
public:
	int xc,yc;
	int R1;
	int R2;
	DWORD FillColor1;
	DWORD FillColor2;
	DWORD LineColor1;
	DWORD LineColor2;
	virtual void Draw();
};
class FeaturesList:public BaseClass{
	ClassArray<OneFeature> Features;
public:
	void AddNewFeature(OneFeature* F);
    void Draw();
	void Clear();
    void AddArrowBetweenBrigades(Brigade* B1,Brigade* B2);
	void AddArrowWhenAttEnm(Brigade* BR);
	void AddAttackRangesOfBrigade(Brigade* BR,int Line=-1);
	void AddSpecialAttackRange(Brigade* BR,int AttType);
	void AddMeleeOpponentForBrigade(Brigade* BR);
	void AddCircleSomewhere(int xc,int yc,int R1,int R2,DWORD LineColor1,DWORD LineColor2,DWORD Color1,DWORD Color2);

};
extern FeaturesList FList;
