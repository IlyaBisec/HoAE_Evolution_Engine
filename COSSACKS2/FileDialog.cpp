#include "stdheader.h"
#include "FilePreview.h"
#include "FileDialog.h"
#include "kSystemDialogs.h"
//CEXPORT int ItemChoose;
bool MMItemChoose(SimpleDialog* SD);
int ceFileDialog::AddFileType(char* type){
	_str* ss=new _str;
	ss->Assign(type);
	return this->FileType.Add(ss);
}
ceFileDialog::ceFileDialog(){	
}
int OnlyName(char* name,_str* ss,char* Mask){
	char m2[64];
	strcpy(m2,Mask);
	strupr(m2);
	char m3[256];
	strcpy(m3,name);
	strupr(m3);
	char* s4=strstr(m3,m2);
	char cc[128];
	if(s4){
		int L=s4-m3;
		strcpy(cc,name);
		cc[L]=0;
		ss->Assign(cc);
		return true;;
	}	
	int L=strlen(name);
	if(!L)return 0;
	sprintf(cc,"%s",name);
	int ch=L-1;
	while(cc[ch]!=46 && ch>=0)
		ch--;
	if(L-ch>5)ch=L;
	cc[ch]=0;
	ss->Assign(cc);
	return ch;
}
void ConvertToUp(char *str);
void ceFileDialog::DrawDir(DialogsDesk* DD,char* Mask){
	this->DirArr.Clear();
	DD->DSS.Clear();
	WIN32_FIND_DATA	FD;
	HANDLE	hFindDir;
	_str root;
	int N=this->Pass.GetAmount();
	for(int i=0;i<N;i++){
		root.Add(this->Pass[i]->pchar());
		root.Add("\\");
	}
	root.Add("*");
	hFindDir=FindFirstFile(root.pchar(),&FD);
	if(hFindDir!=INVALID_HANDLE_VALUE){
		RLCFont* F=&WhiteFont;
		RLCFont* AF=&YellowFont;
		int h=3*GetRLCHeight(F->RLC,'Y')/2;
		int height=0;
		int count=0;
		do{
			if(strcmp(FD.cFileName,".") && strcmp(FD.cFileName,"..")){
				if(FD.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
					_str* ss=new _str;
					ss->Assign(FD.cFileName);
					this->DirArr.Add(ss);
					char cc[128];
					sprintf(cc,"{P Interf3\\btnz 26 12 3 -1 15}%s",FD.cFileName);
					TextButton* TB=DD->addTextButton(NULL,DD->x+20,height,cc,F,AF,AF,0);
					TB->UserParam=10+count;
					TB->OnUserClick=&MMItemChoose;
					height+=25;
					count++;
				}
			}
		}while(FindNextFile(hFindDir,&FD));
		hFindDir=FindFirstFile(root.pchar(),&FD);
		do{
			char m2[64];
			strcpy(m2,Mask);
			ConvertToUp(m2);
			if(strcmp(FD.cFileName,".") && strcmp(FD.cFileName,"..")){
				if(!(FD.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)){
					char cc[512];
					strcpy(cc,FD.cFileName);
					ConvertToUp(cc);
					if(strstr(cc,m2)){
						_str* ss=new _str;
						ss->Assign(FD.cFileName);
						this->DirArr.Add(ss);
						TextButton* TB=DD->addTextButton(NULL,DD->x+10,height,FD.cFileName,F,AF,AF,0);
						TB->UserParam=1000+count;
						TB->OnUserClick=&MMItemChoose;
						height+=25;
						count++;
					}
				}
			}
		}while(FindNextFile(hFindDir,&FD));
		FindClose(hFindDir);
	}
}
#define WINDOWS_LOADSAVE_STYLE
bool ceFileDialog::CreateDialogEditor(int x,int y,int x1,int y1){
#ifdef WINDOWS_LOADSAVE_STYLE
	_str nm="";
	for(int i=0;i<Pass.GetAmount();i++){
        nm+=*Pass[i];
		nm+="\\";
		_chdir(nm.cpchar());
	}
	if(LoadDialogStyle){
		OpenFileDialog dlg;
		for(int i=0;i<FileType.GetAmount();i++){
			char cc[256];
			sprintf(cc,"*%s",FileType[i]->pchar());
			if(i==0)dlg.SetDefaultExtension(FileType[i]->pchar()+1);
			dlg.AddFilter(cc,cc);
		}
		if (!dlg.Show()) return false;
		FullFileName = dlg.GetFilePath();				
	}else{
		SaveFileDialog dlg;
		for(int i=0;i<FileType.GetAmount();i++){
			char cc[256];
			sprintf(cc,"*%s",FileType[i]->pchar());
			if(i==0)dlg.SetDefaultExtension(FileType[i]->pchar()+1);
			dlg.AddFilter(cc,cc);
		}		
		if (!dlg.Show())return false;
		FullFileName = dlg.GetFilePath();		
	}
	int L=strlen(FullFileName.pchar());
	int LR=strlen(IRM->GetHomeDirectory());
	
	if(!strnicmp(FullFileName.pchar(),IRM->GetHomeDirectory(),LR)){
		if(LR>0 && IRM->GetHomeDirectory()[LR-1]!='\\'){
			LR++;
		}
        FullFileName.DeleteChars(0,LR);
	}
	for(int j=0;j<L;j++)if(FullFileName.pchar()[j]=='/')FullFileName.pchar()[j]='\\';
	_chdir( IRM->GetHomeDirectory() );
	return true;

#else //WINDOWS_LOADSAVE_STYLE
	Lpressed=false;
	DialogsSystem DSS;
	if(!DSS.SafeReadFromFile("Dialogs\\FILEDIALOG.DIALOGSSYSTEM.XML"))
		return false;
	DialogsDesk* DD=dynamic_cast<DialogsDesk*>(DSS.Find("DeskMain"));
	if(!DD) return false;
	DD->x=x;
	DD->y=y;
	DD->x1=x1;
	DD->y1=y1;
	DialogsDesk* DD1=dynamic_cast<DialogsDesk*>(DSS.Find("Desk"));
	if(!DD1) return false;
	DialogsDesk* Preview=dynamic_cast<DialogsDesk*>(DSS.Find("Preview"));	
	ComboBox* CB=dynamic_cast<ComboBox*>(DSS.Find("Combo"));
	if(!CB) return false;
	int NT=this->FileType.GetAmount();
	for(int i=0;i<NT;i++)
		CB->AddLine(this->FileType[i]->pchar());
	InputBox* IB=dynamic_cast<InputBox*>(DSS.Find("Input"));
	if(!IB) return false;	
	if(!FileName.pchar())FileName="";
    IB->_Str=&FileName;
	GP_Button* UP=dynamic_cast<GP_Button*>(DSS.Find("Up"));
	if(!UP) return false;
	UP->UserParam=1;
	UP->OnUserClick=&MMItemChoose;
	GP_Button* GO=dynamic_cast<GP_Button*>(DSS.Find("Go"));
	if(!GO) return false;
	GO->UserParam=2;
	GO->OnUserClick=&MMItemChoose;
	GP_Button* EX=dynamic_cast<GP_Button*>(DSS.Find("Exit"));
	if(!EX) return false;
	EX->UserParam=3;
	EX->OnUserClick=&MMItemChoose;
	TextButton* TB=dynamic_cast<TextButton*>(DSS.Find("Path"));
	if(!TB) return false;
	int CurrentLine=CB->CurLine;
	char Mask[64];
	sprintf(Mask,"%s",CB->GetLineMessage(CB->CurLine));
	this->DrawDir(DD1,Mask);
	ItemChoose=-1;
	FilePreview* FPV=NULL;
	char CMASK[64]="";
	bool res=1;
    FullFileName="";
	do{
		if(strcmp(CMASK,Mask)){
			if(FPV)delete(FPV);
			strcpy(CMASK,Mask);
			FPV=GetPreviewer(Mask);
		}
		ProcessMessages();
		DSS.ProcessDialogs();
		if(FPV&&Preview&&FullFileName.pchar()&&bActive){			
			char cc2[256];
			strcpy(cc2,FullFileName.pchar());
			int L=strlen(cc2);
			for(int j=0;j<L;j++)if(cc2[j]=='/')cc2[j]='\\';			
			TempWindow TW;
			PushWindow(&TW);
			IntersectWindows(Preview->LastDrawX,Preview->LastDrawY,
				Preview->LastDrawX+Preview->x1-Preview->x,
				Preview->LastDrawY+Preview->y1-Preview->y);
			//GPS.DrawRect(Preview->LastDrawX,Preview->LastDrawY,Preview->x1-Preview->x,Preview->y1-Preview->y,0xFFFFFFFF);
			FPV->DrawPreview(Preview->LastDrawX,Preview->LastDrawY,
				Preview->LastDrawX+Preview->x1-Preview->x,
				Preview->LastDrawY+Preview->y1-Preview->y,FullFileName.pchar());
			PopWindow(&TW);
		}
		DSS.RefreshView();
		if(ItemChoose==1){
			int N=this->Pass.GetAmount();
			if(N){
				this->Pass.DelElement(N-1);
				this->DrawDir(DD1,Mask);
			}
		}else if(ItemChoose==2){
			break;
		}else if(ItemChoose==3){
			res=0;
			break;
		}else if(ItemChoose>=10 && ItemChoose<1000){
			int NPath=this->Pass.GetAmount();
			_str* ss=new _str;
			ss->Assign(this->DirArr[ItemChoose-10]->pchar());
			this->Pass.Add(ss);
			this->DrawDir(DD1,Mask);
		}else if(ItemChoose>=1000){
			this->FileName=this->DirArr[ItemChoose-1000]->pchar();			
			IB->CursPos=100;
			IB->Selected=-1;
		}
		if(CurrentLine!=CB->CurLine){
			CurrentLine=CB->CurLine;
			sprintf(Mask,"%s",CB->GetLineMessage(CB->CurLine));
			this->DrawDir(DD1,Mask);
		}
		this->FullFileName.Clear();
		int NPath=this->Pass.GetAmount();
		if(NPath){
			for(int i=0;i<NPath;i++){
				this->FullFileName.Add(this->Pass[i]->pchar());
				this->FullFileName.Add("/");
			}
		}
		if(IB->_Str->pchar()&&IB->_Str->pchar()[0]){
			_str str;
			if(OnlyName(this->FileName.pchar(),&str,Mask)){
				this->FullFileName.Add(str);
				this->FullFileName.Add(Mask);
			}
		}
		TB->SetMessage(this->FullFileName.pchar());
		ItemChoose=-1;
	}while(ItemChoose==-1);
	ProcessMessages();
	DSS.ProcessDialogs();
	DSS.RefreshView();
	if(FPV)delete(FPV);
	int L=strlen(FullFileName.pchar());
	for(int j=0;j<L;j++)if(FullFileName.pchar()[j]=='/')FullFileName.pchar()[j]='\\';
	ItemChoose=-1;
	return res;
#endif //WINDOWS_LOADSAVE_STYLE
}