class FilePreview:public BaseClass{
public:
	virtual ~FilePreview(){};
	virtual int EstimateSize(int TotalYSize){return 0;}
	virtual void DrawPreview(int x,int y,int x1,int y1,const char* File){}
	virtual bool CheckAbilityToPreview(const char* Ext){return false;}
	virtual FilePreview* new_element(){return new FilePreview;}
};
FilePreview* GetPreviewer(const char* Ext);
