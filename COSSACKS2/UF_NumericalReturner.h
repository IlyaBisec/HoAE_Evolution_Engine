//==================================================================================================================//
class UserFriendlyNumericalReturner_editor:public OneElementEditor
{
public:
	char str[256];
	char str2[256];
	InputBox* IB;
	UserFriendlyNumericalReturner* NR;
	ClassEditor* Owner;

	virtual const char* GetClassID(){return "UserFriendlyNumericalReturner";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam);
	virtual bool Assign(xmlQuote* xml);
	virtual int Get(xmlQuote* xml);
	virtual OneElementEditor* _new(){return (OneElementEditor*)new UserFriendlyNumericalReturner_editor;}
};