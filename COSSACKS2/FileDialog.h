class ceFileDialog{
public:
	bool LoadDialogStyle;
	_str FileName;
	_str FullFileName;
	ClassArray<_str> FileType;
	ClassArray<_str> Pass;
	ClassArray<_str> DirArr;
	ceFileDialog();
	int AddFileType(char* type);
	void DrawDir(DialogsDesk* DD,char* Mask);
	bool CreateDialogEditor(int x,int y,int x1,int y1);
};