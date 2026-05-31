//==================================================================================================================//
class WeaponSystemExtension : public GameExtension
{
public:
	virtual void OnClassRegistration();
	virtual void OnUnloading();
	virtual bool OnGameSaving(xmlQuote& xml);
	virtual bool OnGameLoading(xmlQuote& xml);
};
//==================================================================================================================//