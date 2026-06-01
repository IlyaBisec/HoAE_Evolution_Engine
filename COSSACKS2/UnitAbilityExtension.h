//==================================================================================================================//
class UnitAbilityExtension : public GameExtension
{
public:
	int CampaignKillerAward[8];
	int SkirmishKillerAward[8][8];

	virtual void ProcessingGame();

	virtual void OnClassRegistration();
	virtual void OnUnloading();
	virtual bool OnGameSaving(xmlQuote& xml);
	virtual bool OnGameLoading(xmlQuote& xml);
	virtual bool OnAttemptToAttack(OneObject* Attacker,OneObject* Victim);
	//virtual bool OnAttemptToMove(OneObject* Unit,int x,int y)				{UnMask(56);return true;}
	virtual bool OnUnitDamage(OneObject* DamagedUnit,OneObject* Damager,int& Damage,byte AttType);
	virtual bool OnUnitDie(OneObject* Dead,OneObject* Killer);

	virtual void OnUnitBirth(OneObject* NewUnit);
	virtual void OnUnitWasProduced(OneObject* Producer,OneObject* NewObject);

	virtual void OnDrawOnMapAfterLandscape();
	virtual void OnDrawOnMapAfterFogOfWar();

	virtual bool OnCheatEntering(const char* Cheat);
};
//==================================================================================================================//