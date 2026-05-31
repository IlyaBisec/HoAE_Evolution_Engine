//==================================================================================================================//
class BrigadeAbilityExtension : public GameExtension
{
public:
	virtual void OnUnloading();
	virtual bool OnGameSaving(xmlQuote& xml);
	virtual bool OnGameLoading(xmlQuote& xml);
};
//==================================================================================================================//