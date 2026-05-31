#ifndef __HelpSystem__
#define __HelpSystem__
//---------------------------For help miss--------------------------------
class vvPOINT2D;
class vvTEXT;
class CSingleMessage : public ReferableBaseClass 
{
public:
	CSingleMessage()	{ Use_Array=false; MessUID=-1; };
	~CSingleMessage()	{};

	virtual			bool	CheckIfObjectIsGlobal()			{ return true; }
	virtual const	char*	GetThisElementView(const char* LocalName){
								return Name.pchar();
							};

					DWORD	GetClassMask(){
						if (Use_Array)	return 0x00000001;
										return 0x00000002;
					};

					bool	NeedUpdatePointsCoord();

	_str		TextID;			//id текста.			
	_str		TalkerID;		//id говорящего.
	bool		Visible;		//Текс отображается.
	bool		Deleted;		//Текст зачеркнут.
	DWORD		Color;			//Цвер текста.
	_str		SoundFileName;	//Путь к файлу для проигрывания голоса.
	int			x,y;			//Координаты для подсветки.
	short		FileID;
	int         SpriteID;		//ID файла ис прайта для подсветки на minimap.
	
	bool		Use_Array;
	ClonesArray< ClassRef<vvPOINT2D> >	Points;

	int			MessUID;
    
	SAVE(CSingleMessage);
		REG_AUTO(Name);
		REG_AUTO(TextID);
		REG_AUTO(TalkerID);
		REG_MEMBER(_bool,Visible);
		REG_MEMBER(_bool,Deleted);
		REG_MEMBER(_color,Color);
		REG_FILEPATH(SoundFileName,".ogg");
		REG_MEMBER(_int,x);
		REG_MEMBER(_int,y);
		REG_MEMBER(_gpfile,FileID);
		REG_SPRITE(SpriteID,FileID);
		REG_MEMBER(_bool,Use_Array);
		SAVE_SECTION(0x00000001);
			REG_AUTO(Points);
	ENDSAVE;
};

class COneMissHint : public CSingleMessage
{
public:
	COneMissHint()	{};
	~COneMissHint()	{};

	//	bool		Visible;		// Active				CSingleMessage::Visible
	//	_str		TextID;			// id текста hint.		CSingleMessage::TextID
	_str			TextView;
	bool			HasRead;

	virtual const	char*	GetThisElementView(const char* LocalName){	
								TextView="HintState(";
								if (TextID.pchar()!=NULL)	TextView+=TextID.pchar();
								else					TextView+="NoHint";
								TextView+=",";
								if (Visible)	TextView+="Active";
								else			TextView+="NotActive";
								TextView+=")";
								return TextView.pchar();
							};

	SAVE(COneMissHint);
		REG_BASE(CSingleMessage);
		REG_AUTO(Name);
		REG_AUTO(TextID);
		REG_MEMBER(_bool,Visible);
		REG_MEMBER(_bool,HasRead);
	ENDSAVE;
};

class CPhraseChunk : public CSingleMessage
{
public:
	CPhraseChunk()	{};
	~CPhraseChunk()	{};

//	bool		Visible;
	_str		TitleID;
	ClonesArray< ClassRef<vvTEXT> >	PhraseList;

	_str		TextView;

	virtual const	char*	GetThisElementView(const char* LocalName){
		if (TitleID.pchar()!=NULL)	TextView = TitleID.pchar();
		else					TextView = "NoTitle";
		TextView+="[";
		int N=PhraseList.GetAmount();
		TextView+=N;
		TextView+=",";
		if (Visible)	TextView+="Complete";
		else			TextView+="NotComplete";
		TextView+="]";
		return TextView.pchar();
	};

	SAVE(CPhraseChunk);
		REG_BASE(CSingleMessage);
		REG_AUTO(Name);
		REG_MEMBER(_bool,Visible);
		REG_AUTO(TitleID);
		REG_AUTO(PhraseList);
	ENDSAVE;
};


typedef	ClonesArray<CSingleMessage> tpMessageArray;
typedef void tpHelpMessegesMiss(tpMessageArray** ppTASKS,tpMessageArray** ppTALKS,tpMessageArray** ppHINTS);
extern tpHelpMessegesMiss* vGetHelp;
CEXPORT	void SetHelpMessagesArrays(tpHelpMessegesMiss* dc);
//  [2/5/2004]
#endif//__HelpSystem__