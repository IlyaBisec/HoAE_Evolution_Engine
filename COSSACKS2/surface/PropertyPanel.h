class PropertyPanel : public GameExtension{
	static DialogsDesk*		EditDesk;
	static DialogsSystem*	DS;
	static BaseClass*		CurrentClass;
	static bool				Visible;
	static void				TestInit();
public:
	static void ShowProperty(ReferableBaseClass* BC);
	static void Hide();
	static void Show();
	static void Discard();
	static bool isVisible();
	virtual void DrawEditorInterface();
	virtual void ClearActivityOfEditor();
	SAVE(PropertyPanel){
	}ENDSAVE;
};